/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Longda on 2021/4/13.
//

#include <string>
#include <sstream>
#include <algorithm>
#include "execute_stage.h"

#include "common/io/io.h"
#include "common/log/log.h"
#include "common/seda/timer_stage.h"
#include "common/lang/string.h"
#include "session/session.h"
#include "event/storage_event.h"
#include "event/sql_event.h"
#include "event/session_event.h"
#include "event/execution_plan_event.h"
#include "sql/executor/execution_node.h"
#include "sql/executor/tuple.h"
#include "storage/common/table.h"
#include "storage/default/default_handler.h"
#include "storage/common/condition_filter.h"
#include "storage/trx/trx.h"

using namespace common;


static RC schema_add_field(Table *table, const char *field_name, TupleSchema &schema) {
  const FieldMeta *field_meta = table->table_meta().field(field_name);
  if (nullptr == field_meta) {
    LOG_WARN("No such field. %s.%s", table->name(), field_name);
    return RC::SCHEMA_FIELD_MISSING;
  }

  schema.add_if_not_exists(field_meta->type(), table->name(), field_meta->name());
  return RC::SUCCESS;
}


RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node);

RC do_aggregation(TupleSet *tuple_set, AttrFunction *attr_function, std::vector<TupleSet> &results);

FuncType judge_function_type(char *window_function_name);

void quick_sort(TupleSet *tuple_set, int l, int r, OrderInfo *order_info);

//! Constructor
ExecuteStage::ExecuteStage(const char *tag) : Stage(tag) {}

//! Destructor
ExecuteStage::~ExecuteStage() {}

//! Parse properties, instantiate a stage object
Stage *ExecuteStage::make_stage(const std::string &tag)
{
  ExecuteStage *stage = new (std::nothrow) ExecuteStage(tag.c_str());
  if (stage == nullptr)
  {
    LOG_ERROR("new ExecuteStage failed");
    return nullptr;
  }
  stage->set_properties();
  return stage;
}

//! Set properties for this object set in stage specific properties
bool ExecuteStage::set_properties()
{
  //  std::string stageNameStr(stageName);
  //  std::map<std::string, std::string> section = theGlobalProperties()->get(
  //    stageNameStr);
  //
  //  std::map<std::string, std::string>::iterator it;
  //
  //  std::string key;

  return true;
}

//! Initialize stage params and validate outputs
bool ExecuteStage::initialize()
{
  LOG_TRACE("Enter");

  std::list<Stage *>::iterator stgp = next_stage_list_.begin();
  default_storage_stage_ = *(stgp++);
  mem_storage_stage_ = *(stgp++);

  LOG_TRACE("Exit");
  return true;
}

//! Cleanup after disconnection
void ExecuteStage::cleanup()
{
  LOG_TRACE("Enter");

  LOG_TRACE("Exit");
}

static RC schema_add_field(Table *table, const char *field_name, TupleSchema &schema)
{
  const FieldMeta *field_meta = table->table_meta().field(field_name);
  if (nullptr == field_meta)
  {
    LOG_WARN("No such field. %s.%s", table->name(), field_name);
    return RC::SCHEMA_FIELD_MISSING;
  }

  schema.add_if_not_exists(field_meta->type(), table->name(), field_meta->name());
  return RC::SUCCESS;
}

void ExecuteStage::handle_event(StageEvent *event)
{
  LOG_TRACE("Enter\n");

  handle_request(event);

  LOG_TRACE("Exit\n");
  return;
}

void ExecuteStage::callback_event(StageEvent *event, CallbackContext *context)
{
  LOG_TRACE("Enter\n");

  // here finish read all data from disk or network, but do nothing here.
  ExecutionPlanEvent *exe_event = static_cast<ExecutionPlanEvent *>(event);
  SQLStageEvent *sql_event = exe_event->sql_event();
  sql_event->done_immediate();

  LOG_TRACE("Exit\n");
  return;
}

void ExecuteStage::handle_request(common::StageEvent *event)
{
  ExecutionPlanEvent *exe_event = static_cast<ExecutionPlanEvent *>(event);
  SessionEvent *session_event = exe_event->sql_event()->session_event();
  Query *sql = exe_event->sqls();
  const char *current_db = session_event->get_client()->session->get_current_db().c_str();

  CompletionCallback *cb = new (std::nothrow) CompletionCallback(this, nullptr);
  if (cb == nullptr)
  {
    LOG_ERROR("Failed to new callback for ExecutionPlanEvent");
    exe_event->done_immediate();
    return;
  }
  exe_event->push_callback(cb);

  switch (sql->flag)
  {
  case SCF_SELECT:
  { // select
    do_select(current_db, sql, exe_event->sql_event()->session_event());
    exe_event->done_immediate();
  }
  break;

  case SCF_INSERT:
  case SCF_UPDATE:
  case SCF_DELETE:
  case SCF_CREATE_TABLE:
  case SCF_SHOW_TABLES:
  case SCF_DESC_TABLE:
  case SCF_DROP_TABLE:
  case SCF_CREATE_INDEX:
  case SCF_DROP_INDEX:
  case SCF_LOAD_DATA:
  {
    StorageEvent *storage_event = new (std::nothrow) StorageEvent(exe_event);
    if (storage_event == nullptr)
    {
      LOG_ERROR("Failed to new StorageEvent");
      event->done_immediate();
      return;
    }

    default_storage_stage_->handle_event(storage_event);
  }
  break;
  case SCF_SYNC:
  {
    RC rc = DefaultHandler::get_default().sync();
    session_event->set_response(strrc(rc));
    exe_event->done_immediate();
  }
  break;
  case SCF_BEGIN:
  {
    session_event->get_client()->session->set_trx_multi_operation_mode(true);
    session_event->set_response(strrc(RC::SUCCESS));
    exe_event->done_immediate();
  }
  break;
  case SCF_COMMIT:
  {
    Trx *trx = session_event->get_client()->session->current_trx();
    RC rc = trx->commit();
    session_event->get_client()->session->set_trx_multi_operation_mode(false);
    session_event->set_response(strrc(rc));
    exe_event->done_immediate();
  }
  break;
  case SCF_ROLLBACK:
  {
    Trx *trx = session_event->get_client()->session->current_trx();
    RC rc = trx->rollback();
    session_event->get_client()->session->set_trx_multi_operation_mode(false);
    session_event->set_response(strrc(rc));
    exe_event->done_immediate();
  }
  break;
  case SCF_HELP:
  {
    const char *response = "show tables;\n"
                           "desc `table name`;\n"
                           "create table `table name` (`column name` `column type`, ...);\n"
                           "create index `index name` on `table` (`column`);\n"
                           "insert into `table` values(`value1`,`value2`);\n"
                           "update `table` set column=value [where `column`=`value`];\n"
                           "delete from `table` [where `column`=`value`];\n"
                           "select [ * | `columns` ] from `table`;\n";
    session_event->set_response(response);
    exe_event->done_immediate();
  }
  break;
  case SCF_EXIT:
  {
    // do nothing
    const char *response = "Unsupported\n";
    session_event->set_response(response);
    exe_event->done_immediate();
  }
  break;
  case SCF_ERROR:
  {
    const char *response = "FAILURE\n";
    session_event->set_response(response);
    exe_event->done_immediate();
  }
  break;
  default:
  {
    const char *response = "FAILURE\n";
    session_event->set_response(response);
    exe_event->done_immediate();
    LOG_ERROR("Unsupported command=%d\n", sql->flag);
  }
  }
}

void end_trx_if_need(Session *session, Trx *trx, bool all_right)
{
  if (!session->is_trx_multi_operation_mode())
  {
    if (all_right)
    {
      trx->commit();
    }
    else
    {
      trx->rollback();
    }
  }
}

// 求两个TupleSet的笛卡尔积
TupleSet cartesian_product(const TupleSet& setA, const TupleSet& setB, bool hasCondition, const Condition& cond, const Selects &selects, const char *db){
    TupleSet ret;
    TupleSchema schema;

    if (hasCondition) {
        const char* left_rel_name = cond.left_attr.relation_name;
        const char* left_attr_name = cond.left_attr.attribute_name;
        const char* right_rel_name = cond.right_attr.relation_name;
        const char* right_attr_name = cond.right_attr.attribute_name;

        for (int i = selects.attr_num - 1; i >= 0; i--) {
        const RelAttr &attr = selects.attributes[i];
<<<<<<< HEAD
        if (nullptr == attr.relation_name || 0 == strcmp(left_rel_name, attr.relation_name)) {
          Table * left_table = DefaultHandler::get_default().find_table(db, left_rel_name);
          if (0 == strcmp("*", attr.attribute_name)) {
            // *则列出这张表所有字段
             TupleSchema::from_table(left_table, schema);
            break; // 没有校验，给出* 之后，再写字段的错误
          } else {
            // 列出这张表相关字段
            RC rc = schema_add_field(left_table, attr.attribute_name, schema);
            schema.print(std::cout);
            if (rc != RC::SUCCESS) {
              
            }
          }
        }
        

        if (nullptr == attr.relation_name || 0 == strcmp(right_rel_name, attr.relation_name)) {
          Table * right_table = DefaultHandler::get_default().find_table(db, right_rel_name);
          if (0 == strcmp("*", attr.attribute_name)) {
            // 列出这张表所有字段
             TupleSchema::from_table(right_table, schema);
            break; // 没有校验，给出* 之后，再写字段的错误
          } else {
            // 列出这张表相关字段
            RC rc = schema_add_field(right_table, attr.attribute_name, schema);
            if (rc != RC::SUCCESS) {
              
            }
          }
        }
      }
=======
        // select * from t1,t2;
        if (nullptr == attr.relation_name && 0 == strcmp("*", attr.attribute_name)) {
          Table * left_table = DefaultHandler::get_default().find_table(db, left_rel_name);  
          Table * right_table = DefaultHandler::get_default().find_table(db, right_rel_name);
          TupleSchema::from_table(left_table, schema);
          TupleSchema::from_table(right_table, schema);
        } else {    // 题目里说明了多表查询的输入SQL，只要是字段，都会带表名
            if (0 == strcmp(left_rel_name, attr.relation_name)) {
            Table * left_table = DefaultHandler::get_default().find_table(db, left_rel_name);
            if (0 == strcmp("*", attr.attribute_name)) {
                // *则列出这张表所有字段
                TupleSchema::from_table(left_table, schema);
            } else {
                // 列出这张表相关字段
                RC rc = schema_add_field(left_table, attr.attribute_name, schema);
                schema.print(std::cout);
                if (rc != RC::SUCCESS) {
                    LOG_ERROR("schema_add_field failed!");
                }
            }
            }
            
            if (0 == strcmp(right_rel_name, attr.relation_name)) {
            Table * right_table = DefaultHandler::get_default().find_table(db, right_rel_name);
            if (0 == strcmp("*", attr.attribute_name)) {
                // 列出这张表所有字段
                TupleSchema::from_table(right_table, schema);
            } else {
                // 列出这张表相关字段
                RC rc = schema_add_field(right_table, attr.attribute_name, schema);
                if (rc != RC::SUCCESS) {
                    LOG_ERROR("schema_add_field failed!");
                }
            }
            }
        }
        }
>>>>>>> main
      ret.set_schema(schema);
        if ((0 == strcmp(setA.get_schema().field(0).table_name(), left_rel_name)) &&
            (0 == strcmp(setB.get_schema().field(0).table_name(), right_rel_name))) {
                // 找出要比较的列在对应schema中的位置
                int left_index = setA.get_schema().index_of_field(left_rel_name, left_attr_name);
                int right_index = setB.get_schema().index_of_field(right_rel_name, right_attr_name);
                for(const auto& tuple_a : setA.tuples()) {
                    for (const auto& tuple_b : setB.tuples()) {
                        TupleValue* value_a = tuple_a.get_pointer(left_index).get();
                        TupleValue* value_b = tuple_b.get_pointer(right_index).get();
                        bool compare_result = false;
                        switch (cond.comp) {
                        case EQUAL_TO:
                            compare_result = (value_a->compare(*value_b) == 0);
                            break;
                        case LESS_EQUAL:
                            compare_result = (value_a->compare(*value_b) <= 0);
                            break;
                        case NOT_EQUAL:
                            compare_result = (value_a->compare(*value_b) != 0);
                            break;
                        case LESS_THAN:
                            compare_result = (value_a->compare(*value_b) < 0);
                            break;
                        case GREAT_EQUAL:
                            compare_result = (value_a->compare(*value_b) >= 0);
                            break;
                        case GREAT_THAN:
                            compare_result = (value_a->compare(*value_b) > 0);
                            break;
                        default:
                            break;
                        }
                        if (compare_result) { // 满足条件则加入结果Tuple
                            Tuple tmp;
                            // 按照select中列的顺序构造结果集
                        for (const auto& each : schema.fields()) {
                          if (0 == strcmp(each.table_name(), left_rel_name)) {
                            int l = setA.get_schema().index_of_field(each.table_name(), each.field_name());
                            tmp.add(tuple_a.get_pointer(l));
                          }
                          if (0 == strcmp(each.table_name(), right_rel_name)) {
                            int r = setB.get_schema().index_of_field(each.table_name(), each.field_name());
                            tmp.add(tuple_a.get_pointer(r));
                          }
                        }
                            ret.add(std::move(tmp));
                        }
                    }
                }
        }
    }  else {
        schema.append(setA.get_schema());
        schema.append(setB.get_schema());
        ret.set_schema(schema);
        for(const auto& tuple_a : setA.tuples()) {
            for (const auto& tuple_b : setB.tuples()) {
                Tuple tmp;
                for (auto value : tuple_a.values()) {
                    tmp.add(value);
                }
                for (auto value : tuple_b.values()) {
                    tmp.add(value);
                }
                ret.add(std::move(tmp));
            }
        }
    }
    return ret;
}

// 这里没有对输入的某些信息做合法性校验，比如查询的列名、where条件中的列名等，没有做必要的合法性校验
// 需要补充上这一部分. 校验部分也可以放在resolve，不过跟execution放一起也没有关系
RC ExecuteStage::do_select(const char *db, Query *sql, SessionEvent *session_event)
{
  RC rc = RC::SUCCESS;
  Session *session = session_event->get_client()->session;
  Trx *trx = session->current_trx();
  const Selects &selects = sql->sstr.selection;
  // 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
  std::vector<SelectExeNode *> select_nodes;

  for (size_t i = 0; i < selects.relation_num; i++)
  {
    // 遍历所有表
    const char *table_name = selects.relations[i];
    SelectExeNode *select_node = new SelectExeNode;

    rc = create_selection_executor(trx, selects, db, table_name, *select_node);
    if (rc != RC::SUCCESS)
    {
      session_event->set_response("FAILURE\n");
      delete select_node;
      for (SelectExeNode *&tmp_node : select_nodes)
      {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      return rc;
    }

    select_nodes.push_back(select_node);
  }

  if (select_nodes.empty())
  {
    LOG_ERROR("No table given");
    session_event->set_response("FAILURE\n");
    end_trx_if_need(session, trx, false);
    return RC::SQL_SYNTAX;
  }

  std::vector<TupleSet> tuple_sets;
  for (SelectExeNode *&node : select_nodes)
  {
    TupleSet tuple_set;
    // excute里设置了聚合函数的type，type定义于tuple.h文件

    rc = node->execute(tuple_set);
    if (rc != RC::SUCCESS)
    {
      for (SelectExeNode *&tmp_node : select_nodes)
      {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      return rc;
    }
    else
    {
      tuple_sets.push_back(std::move(tuple_set));
    }
  }

  std::stringstream ss;
  TupleSet result;
  bool isMultiTable = false;
  if (tuple_sets.size() > 1) // 本次查询了多张表，需要做join操作
  {                          // e.g. select t1.id, t2.name from t1, t2 where t1.id=t2.id;
    isMultiTable = true;
    // 首先要从where子句的Condition中找出两边都是属性的Condition
    Condition cond;
    bool hasCondition = false;
    for (size_t i = 0; i < selects.condition_num; i++)
    {
      const Condition &condition = selects.conditions[i];
      if (condition.left_is_attr == 1 && condition.right_is_attr == 1)
      {
        cond = condition; //先假设只有一个这样的condition
        hasCondition = true;
      }
    }
    // 然后要根据这个condition找出对应的两个表的TupleSet
    // 并根据condition的谓词过滤出满足条件的构造Tuple
    int len = tuple_sets.size();
<<<<<<< HEAD
    TupleSet tuple_set(std::move(tuple_sets[len - 1]));
    for (int i = len - 2; i >= 0; i--) {
        tuple_set = cartesian_product(tuple_set, tuple_sets[i], hasCondition, cond, selects, db);
    }
    // tuple_set.get_schema().print(ss);
    tuple_set.print(ss, true);
  } else {

=======
    result = std::move(tuple_sets[len - 1]);
    for (int i = len - 2; i >= 0; i--)
    {
      result = cartesian_product(result, tuple_sets[i], hasCondition, cond, selects, db);
    }
  }
  else
  {
>>>>>>> main
    // 当前只查询一张表，直接返回结果即可
    result = std::move(tuple_sets.front());
  }

  ////////////////////////////聚合函数开始/////////////////////////////
  // 不管是单表还是多表，到聚合这一步都应该只剩一个TupleSet
  std::vector<TupleSet> results;

  // 处理聚合函数
  if (result.size() > 0)
  {
    // 上一步有结果才进行聚合
    AttrFunction *attr_function = new AttrFunction;
    for (int i = selects.attr_num - 1; i >= 0; i--)
    {
      const RelAttr &attr = selects.attributes[i];

      // 确定该属性与这张表有关
      if (attr.window_function_name != nullptr)
      {
        // 注意这里attr.relation_name可能为nullptr
        FuncType function_type = judge_function_type(attr.window_function_name);
        attr_function->add_function_type(std::string(attr.attribute_name), function_type, attr.relation_name);
      }
    }

    rc = do_aggregation(&result, attr_function, results);

    if (rc != RC::SUCCESS)
    {
      session_event->set_response("FAILURE\n");
      for (SelectExeNode *&tmp_node : select_nodes)
      {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      return rc;
    }
  }
  ////////////////////////////聚合函数结束/////////////////////////////

  if (results.size() != 0)
  {
    result = std::move(results[0]);
    results.clear();
  }
  else
  {
    // 当前没有group by，先假设聚合和排序是矛盾的，仍然用tuples_sets进行快速排序
    ///////////////////////////////////排序开始////////////////////////////////
    // 提取排序信息
    OrderInfo *order_info = new OrderInfo;

    for (int i = selects.order_num - 1; i >= 0; i--)
    {
      const RelAttr &attr = selects.order_attrs[i];
      const TupleSchema &schema = result.get_schema();
      // 确定该属性与这张表有关
      int index = schema.index_of_field(attr.relation_name, attr.attribute_name);
      if (index != -1)
      {
        order_info->add(attr.attribute_name, index, attr.is_desc == 1);
      }
    }

    if (order_info->get_size() > 0 && result.size() > 0)
    {
      // 遍历每个TupleSet（即每个表的结果）
      quick_sort(&result, 0, result.size() - 1, order_info);
    }
  }

  result.print(ss, isMultiTable);

  for (SelectExeNode *&tmp_node : select_nodes)
  {
    delete tmp_node;
  }

  session_event->set_response(ss.str());
  end_trx_if_need(session, trx, true);
  return rc;
}

bool tuple_compare(const Tuple *lhs, const Tuple *rhs, OrderInfo *order_info)
{
  // 左在右前返回true
  for (int i = order_info->get_size() - 1; i >= 0; --i)
  {
    auto l_value = lhs->get_pointer(order_info->get_index(i));
    auto r_value = rhs->get_pointer(order_info->get_index(i));
    bool is_desc = order_info->get_is_desc(i);

    if (l_value->compare(*r_value) > 0)
    {
      return !is_desc;
    }

    if (l_value->compare(*r_value) < 0)
    {
      return is_desc;
    }
  }

  // 两者数据完全一样，返回true/false都可以
  return true;
}

int partition(TupleSet *tuple_set, int l, int r, OrderInfo *order_info)
{
  auto pivot = &(tuple_set->get(r));
  int i = l - 1;

  for (int j = l; j < r; j++)
  {
    if (tuple_compare(pivot, &(tuple_set->get(j)), order_info))
    {
      i++;
      tuple_set->swap_tuple(i, j);
    }
  }

  tuple_set->swap_tuple(i + 1, r);
  return i + 1;
}

/**
 * @brief 快速排序
 *
 * @param tuple_set 表的当前结果集合
 * @param index     按第index列进行排序
 * @param is_desc   是否降序，默认升序
 * @return RC
 */
void quick_sort(TupleSet *tuple_set, int l, int r, OrderInfo *order_info)
{
  if (l < r)
  {
    int mid = partition(tuple_set, l, r, order_info);
    quick_sort(tuple_set, l, mid - 1, order_info);
    quick_sort(tuple_set, mid + 1, r, order_info);
  }
}

/**
 * @brief 聚合函数运算
 *
 * @param tuple_set 表的当前结果集合
 * @param attr_function 表的聚合函数信息
 * @param results 表的聚合数结果
 * @return RC
 */
RC do_aggregation(TupleSet *tuple_set, AttrFunction *attr_function, std::vector<TupleSet> &results)
{
  TupleSchema tmp_scheme;
  Tuple tmp_tuple;

  // 遍历所有带函数的属性
  RC rc = RC::SUCCESS;
  for (int j = attr_function->get_size() - 1; j >= 0; --j)
  {
    const char *table_name = attr_function->get_table_name(j);
    const char *attr_name = attr_function->get_attr_name(j);
    auto func_type = attr_function->get_function_type(j);
    // 获取 func_type( 字符串
    std::string add_scheme_name = attr_function->to_string(j);

    if (strcmp(attr_name, "*") == 0)
    {
      // 处理COUNT(*)
      tmp_scheme.add_if_not_exists(AttrType::INTS, "", add_scheme_name.c_str());
      tmp_tuple.add((int)tuple_set->tuples().size());
      continue;
    }

    int index = -1;
    AttrType type = AttrType::UNDEFINED;
    if (table_name != nullptr)
    {
      index = tuple_set->get_schema().index_of_field(table_name, attr_name);
      type = tuple_set->get_schema().field(index).type();
    }
    else
    {
      // 手动搜寻
      const TupleSchema &schema = tuple_set->get_schema();
      int n = schema.fields().size();
      for (int i = 0; i < n; ++i)
      {
        if (strcmp(schema.field(i).field_name(), attr_name) == 0)
        {
          index = i;
          type = schema.field(i).type();
          break;
        }
      }
    }
    // const TupleField &field = tuple_set->get_schema().field(index);
    LOG_ERROR("here");

    if (func_type == FuncType::NOFUNC)
    {
      // 其实这个if应该永远不会执行
      LOG_ERROR("未定义的聚合函数");
      rc = RC::GENERIC_ERROR;
      return rc;
    }

    // 增加tuple
    AttrType add_type = AttrType::UNDEFINED;
    switch (func_type)
    {
    case FuncType::COUNT:
    {
      // 增加Scheme
      add_type = AttrType::INTS;

      // TODO: 考虑NULL值
      tmp_tuple.add((int)tuple_set->tuples().size());
      break;
    }
    case FuncType::AVG:
    {
      if (type == AttrType::CHARS || type == AttrType::DATES)
      {
        // CHARS和DATES不应该计算平均值
        rc = RC::GENERIC_ERROR;
        break;
      }
      // 增加Scheme
      // tmp_scheme.add_if_not_exists(AttrType::FLOATS, add_scheme_name.c_str(), add_attr_name.c_str());
      add_type = AttrType::FLOATS;

      int size = (int)tuple_set->tuples().size();
      float ans = 0;
      if (type == AttrType::FLOATS)
      {
        for (int tuple_i = 0; tuple_i < tuple_set->size(); ++tuple_i)
        {
          std::shared_ptr<FloatValue> value = std::dynamic_pointer_cast<FloatValue>(tuple_set->get(tuple_i).get_pointer(index));
          ans += value->GetValue();
        }
      }
      else if (type == AttrType::INTS)
      {
        for (int tuple_i = 0; tuple_i < tuple_set->size(); ++tuple_i)
        {
          std::shared_ptr<IntValue> value = std::dynamic_pointer_cast<IntValue>(tuple_set->get(tuple_i).get_pointer(index));
          ans += value->GetValue();
        }
      }

      tmp_tuple.add(ans / size);
      break;
    }
    case FuncType::MAX:
    {
      auto ans = tuple_set->get(0).get_pointer(index);
      for (int tuple_i = 0; tuple_i < tuple_set->size(); ++tuple_i)
      {

        auto value = tuple_set->get(tuple_i).get_pointer(index);

        if (value->compare(*ans) > 0)
        {
          ans = value;
        }
      }

      // 增加Scheme
      // tmp_scheme.add_if_not_exists(type, add_scheme_name.c_str(), add_attr_name.c_str());
      add_type = type;

      if (type == AttrType::FLOATS)
      {
        tmp_tuple.add(std::dynamic_pointer_cast<FloatValue>(ans)->GetValue());
      }
      else if (type == AttrType::INTS)
      {
        tmp_tuple.add(std::dynamic_pointer_cast<IntValue>(ans)->GetValue());
      }
      else // AttrType::CHARS和DATES一样计算
      {
        tmp_tuple.add(std::dynamic_pointer_cast<StringValue>(ans)->GetValue(),
                      std::dynamic_pointer_cast<StringValue>(ans)->GetLen());
      }

      break;
    }
    case FuncType::MIN:
    {
      auto ans = tuple_set->get(0).get_pointer(index);
      for (int tuple_i = 0; tuple_i < tuple_set->size(); ++tuple_i)
      {

        auto value = tuple_set->get(tuple_i).get_pointer(index);

        if (value->compare(*ans) < 0)
        {

          ans = value;
        }
      }

      // 增加Scheme
      // tmp_scheme.add_if_not_exists(type, add_scheme_name.c_str(), add_attr_name.c_str());
      add_type = type;

      if (type == AttrType::FLOATS)
      {
        tmp_tuple.add(std::dynamic_pointer_cast<FloatValue>(ans)->GetValue());
      }
      else if (type == AttrType::INTS)
      {
        tmp_tuple.add(std::dynamic_pointer_cast<IntValue>(ans)->GetValue());
      }
      else // AttrType::CHARS和DATES一样计算
      {
        tmp_tuple.add(std::dynamic_pointer_cast<StringValue>(ans)->GetValue(),
                      std::dynamic_pointer_cast<StringValue>(ans)->GetLen());
      }

      break;
    }
    default:
      break;
    }

    tmp_scheme.add_if_not_exists(add_type, "", add_scheme_name.c_str());

    if (rc != RC::SUCCESS)
    {
      return rc;
    }
  }
  if (tmp_tuple.size() > 0)
  {
    TupleSet tmp_set;
    tmp_set.set_schema(tmp_scheme);
    tmp_set.add(std ::move(tmp_tuple));
    results.push_back(std::move(tmp_set));
  }

  return rc;
}

bool match_table(const Selects &selects, const char *table_name_in_condition, const char *table_name_to_match)
{
  if (table_name_in_condition != nullptr)
  {
    return 0 == strcmp(table_name_in_condition, table_name_to_match);
  }

  return selects.relation_num == 1;
}

<<<<<<< HEAD

=======

FuncType judge_function_type(char *window_function_name)
{
  if (window_function_name == nullptr)
  {
    return FuncType::NOFUNC;
  }
  // 转换成小写
  char *p = window_function_name;
  while (*p)
  {
    *p = tolower(*p);
    ++p;
  }

  // 判断
  if (strcmp("count", window_function_name) == 0)
  {
    return FuncType::COUNT;
  }
  else if (strcmp("avg", window_function_name) == 0)
  {
    return FuncType::AVG;
  }
  else if (strcmp("max", window_function_name) == 0)
  {
    return FuncType::MAX;
  }
  else if (strcmp("min", window_function_name) == 0)
  {
    return FuncType::MIN;
  }

  return FuncType::NOFUNC;
}
>>>>>>> main

// 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode& select_node)
{
  // 列出跟这张表关联的Attr
  // 1. 找到表
  TupleSchema schema;
  Table *table = DefaultHandler::get_default().find_table(db, table_name);

  if (nullptr == table)
  {
    LOG_WARN("No such table [%s] in db [%s]", table_name, db);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

<<<<<<< HEAD

  for (int i = selects.attr_num - 1; i >= 0; i--) {
=======
  // 2. 遍历Select中所有属性
  for (int i = selects.attr_num - 1; i >= 0; i--)
  {
>>>>>>> main
    const RelAttr &attr = selects.attributes[i];

    // 确定该属性与这张表有关
    if (nullptr == attr.relation_name || 0 == strcmp(table_name, attr.relation_name))
    {
      // 对应select *的情况
      if (0 == strcmp("*", attr.attribute_name) || isdigit(attr.attribute_name[0]) || attr.attribute_name[0] == '-')
      {
        // 找到对应的表
        // 列出这张表所有字段
        TupleSchema::from_table(table, schema);
        break; // 没有校验，给出* 之后，再写字段的错误
      }
      else
      { // 对应select age/ select t1.age的情况
        // 列出这张表相关字段
        RC rc = schema_add_field(table, attr.attribute_name, schema);
        if (rc != RC::SUCCESS)
        {
          return rc;
        }
      }
    }
  }
<<<<<<< HEAD

  bool first = true; // 标记是不是第一次遇到多表连接语句
=======
    
    bool first = true; // 标记是不是第一次遇到多表连接语句
>>>>>>> main

  // 找出仅与此表相关的过滤条件, 或者都是值的过滤条件
  std::vector<DefaultConditionFilter *> condition_filters;
  for (size_t i = 0; i < selects.condition_num; i++)
  {
    const Condition &condition = selects.conditions[i];
    if ((condition.left_is_attr == 0 && condition.right_is_attr == 0) ||                                                                         // 两边都是值
        (condition.left_is_attr == 1 && condition.right_is_attr == 0 && match_table(selects, condition.left_attr.relation_name, table_name)) ||  // 左边是属性右边是值
        (condition.left_is_attr == 0 && condition.right_is_attr == 1 && match_table(selects, condition.right_attr.relation_name, table_name)) || // 左边是值，右边是属性名
<<<<<<< HEAD
         (condition.left_is_attr == 1 && condition.right_is_attr == 1 )
           //  match_table(selects, condition.left_attr.relation_name, table_name) && match_table(selects, condition.right_attr.relation_name, table_name)) // 左右都是属性名，并且表名都符合
            // t1.id = t2.id，左右两边的relation_name不可能都是table_name
        ) {

=======
        (condition.left_is_attr == 1 && condition.right_is_attr == 1) // &&
         // match_table(selects, condition.left_attr.relation_name, table_name) && match_table(selects, condition.right_attr.relation_name, table_name)) // 左右都是属性名，并且表名都符合
    )
    {
        // 检查where中的表名是否都是from中出现的表名
        // e.g. select t1.id from t1,t2 where t1.id=t3.id
        if ((condition.left_is_attr == 1 && condition.right_is_attr == 1)) {
            bool left_is_found = false;
            bool right_is_found = false;
            for (size_t i = 0; i < selects.relation_num; i++) {
                if (0 == strcmp(condition.left_attr.relation_name, selects.relations[i])) {
                    left_is_found = true;
                }
                if (0 == strcmp(condition.right_attr.relation_name, selects.relations[i])) {
                    right_is_found = true;
                }
            }
            // condition左右的表名都是from中表名才正确
            if (!(left_is_found && right_is_found)) {
                LOG_ERROR("Table in condition does not appear in from.");
                return RC::SCHEMA_TABLE_NAME_ILLEGAL;
            }
        }
>>>>>>> main
      DefaultConditionFilter *condition_filter = new DefaultConditionFilter();
      // 这个init函数里检查了where子句中的列名是否存在
      RC rc = condition_filter->init(*table, condition);
      if (rc != RC::SUCCESS)
      {
        delete condition_filter;
        for (DefaultConditionFilter *&filter : condition_filters)
        {
          delete filter;
        }
        return rc;
      }
      condition_filters.push_back(condition_filter);
    }
<<<<<<< HEAD

=======
>>>>>>> main
    // 如果是多表，取所有列
    if (first && condition.left_is_attr == 1 && condition.right_is_attr == 1 ) {
      schema.clear();
      TupleSchema::from_table(table, schema);
      first = false;
    }
  }
    LOG_INFO("condition_filters count: %d", condition_filters.size());
  return select_node.init(trx, table, std::move(schema), std::move(condition_filters));
}