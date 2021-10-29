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

static RC schema_add_field(Table *table, const char *field_name, TupleSchema &schema);

RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node);

RC do_aggregation(TupleSet *tuple_set, AttrFunction *attr_function, std::vector<TupleSet> &results, int rel_num);

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


bool valueCompare(const TupleValue* value_a, const TupleValue* value_b, CompOp op)
{
  bool compare_result = false;
  switch (op)
  {
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
  return compare_result;
}

// 判断Tuple是否满足条件
bool isTupleSatisfy(const Tuple& tp, const TupleSchema& schema, const Selects &selects)
{
    bool valid = true;
    for (size_t i = 0; i < selects.condition_num; i++) {
        const Condition& cond = selects.conditions[i];
        // 只考虑两边都是attr的condition
        if (cond.left_is_attr == 1 && cond.right_is_attr == 1) {
            int left_index = schema.index_of_field(cond.left_attr.relation_name, cond.left_attr.attribute_name);
            int right_index = schema.index_of_field(cond.right_attr.relation_name, cond.right_attr.attribute_name);
            TupleValue *va = tp.get_pointer(left_index).get();
            TupleValue *vb = tp.get_pointer(right_index).get();
            if (valueCompare(va, vb, cond.comp) == false) {
                valid = false;
                break;
            }
        }
    }
    return valid;
}

// 回溯法求笛卡尔积
void backtrack(TupleSet& ans, const std::vector<TupleSet>& sets, int index, Tuple& tmp, const Selects &selects, const TupleSchema& schema)
{
    if (index == -1) {
        Tuple t;
        for (const auto& each : tmp.values()) {
            t.add(each);
        }
        // 满足条件再加入
        if (isTupleSatisfy(t, schema, selects)) {
            ans.add(std::move(t));
        }
    } else {
        int len = sets[index].size();
        for (int i = 0; i < len; i++) {
            tmp.merge(sets[index].get(i));
            backtrack(ans, sets, index - 1, tmp, selects, schema);
            tmp.remove(sets[index].get(i));
        }
    }
}

TupleSet do_join(const std::vector<TupleSet>& sets, const Selects &selects, const char *db)
{
    // 先根据所有的tuple_set构造一个包含所有列的TupleSet
    TupleSchema total_schema;
    TupleSet total_set;
    for (auto it = sets.rbegin(); it != sets.rend(); it++) {
        total_schema.append(it->get_schema());
    }
    total_set.set_schema(total_schema);

    int len_sets = sets.size();
    Tuple tmp;
    backtrack(total_set, sets, len_sets - 1, tmp, selects, total_schema);

    // 根据Select中的列构造输出的schema
    TupleSchema final_schema;
    TupleSet final_set;
    for (int i = selects.attr_num - 1; i >= 0; i--) {
        const RelAttr &attr = selects.attributes[i];
        if ((nullptr == attr.relation_name) && (0 == strcmp(attr.attribute_name, "*"))) {
            final_schema.append(total_schema);
        } else if ((nullptr != attr.relation_name) && (0 == strcmp(attr.attribute_name, "*"))) {
            Table *table = DefaultHandler::get_default().find_table(db, attr.relation_name);
            TupleSchema::from_table(table, final_schema);
        } else {
            Table *table = DefaultHandler::get_default().find_table(db, attr.relation_name);
            schema_add_field(table, attr.attribute_name, final_schema);
        }
    }

    final_set.set_schema(final_schema);

      //////////////////////////至此生成全表笛卡尔积，开始group by/////////////////////////////
  // if (selects.group_num > 0)
  // {
  //   // group by特点：不在group by中的列，在select中只能以聚合函数形式出现
  //   // 检查group by列名是否存在
  //   std::vector<int> group_idx; // 用于group by的列在tutal_set中的index
  //   int n = selects.group_num;

  //   for (int i = 0; i < n; ++i)
  //   {
  //     int cnt = 0;
  //     const RelAttr &attr = selects.group_attrs[i];
  //     // 确定该属性与这张表有关
  //     int index = -1;

  //     if (attr.relation_name != nullptr)
  //     {
  //       index = total_schema.index_of_field(attr.relation_name, attr.attribute_name);
  //     }
  //     else
  //     {
  //       // 不带属性名，可能出现二义性问题
  //       const char *last_table_name = nullptr;
  //       const int size = total_schema.fields().size();

  //       for (int i = 0; i < size; ++i)
  //       {
  //         if (strcmp(attr.attribute_name, total_schema.field(i).field_name()) == 0)
  //         {
  //           if (cnt == 0)
  //           {
  //             ++cnt;
  //             last_table_name = total_schema.field(i).table_name();
  //             index = i;
  //           }
  //           else
  //           {
  //             if (last_table_name != total_schema.field(i).table_name())
  //             {
  //               ++cnt;
  //               break;
  //             }
  //           }
  //         }
  //       }
  //     }

  //     if (index == -1 || cnt > 1)
  //     {
  //       // 有order信息但没有提取出来，说明出现错误的列名
  //       for (SelectExeNode *&tmp_node : select_nodes)
  //       {
  //         delete tmp_node;
  //       }

  //       session_event->set_response("FAILURE\n");
  //       end_trx_if_need(session, trx, true);
  //       return RC::GENERIC_ERROR;
  //     }
  //   }
  // }
  //////////////////////////group by结束/////////////////////////////

    // 取出需要的列
    for (auto& tp : total_set.tuples()) {
        Tuple t;
        for (const auto& s : final_schema.fields()) {
            int index = total_schema.index_of_field(s.table_name(), s.field_name());
            t.add(tp.get_pointer(index));
        }
        final_set.add(std::move(t));
    }

    return final_set;
}

// 检查Select, where中的表名是否都出现在from中
RC check_table_name(const Selects &selects, const char *db)
{
  // 检查from中的表是否存在
  int rel_num = selects.relation_num;
  for (int i = 0; i < rel_num; i++)
  {
    Table *table = DefaultHandler::get_default().find_table(db, selects.relations[i]);
    if (nullptr == table)
    {
      LOG_WARN("No such table [%s] in db [%s]", selects.relations[i], db);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
  }

  // 检查select中的表是否在from中
  for (size_t i = 0; i < selects.attr_num; i++)
  {
    const RelAttr &attr = selects.attributes[i];
    if (rel_num > 1)
    { // 多表
      // 只有列名且不为"*"出错, id
      if ((nullptr == attr.relation_name) && (0 != strcmp(attr.attribute_name, "*")))
      {
        LOG_ERROR("Table name must appear.");
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
      // t1.id
      bool table_name_in_from = false;
      for (int j = 0; j < rel_num; j++)
      {
        if ((nullptr != attr.relation_name) && (0 == strcmp(attr.relation_name, selects.relations[j])))
        {
          table_name_in_from = true;
          break;
        }
      }
      // t1.*
      if ((nullptr == attr.relation_name) && (0 == strcmp(attr.attribute_name, "*")))
      {
        table_name_in_from = true;
      }
      if (table_name_in_from == false)
      {
        LOG_WARN("Table [%s] not in from", attr.relation_name);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
    }
    else if (rel_num == 1)
    {
      if ((attr.relation_name != nullptr) && (0 != strcmp(attr.relation_name, selects.relations[0])))
      {
        LOG_WARN("Table [%s] not in from", attr.relation_name);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
    }
  }

  // 检查where中的表名是否在from中
  for (size_t i = 0; i < selects.condition_num; i++)
  {
    const Condition &condition = selects.conditions[i];
    if (rel_num == 1)
    {
      // strcmp的参数如果为null会出现segmentation 错误
      for (size_t i = 0; i < selects.relation_num; i++)
      {
        if (((condition.left_is_attr == 1) && (nullptr != condition.left_attr.relation_name) && (0 != strcmp(condition.left_attr.relation_name, selects.relations[i]))) ||
            ((condition.right_is_attr == 1) && (nullptr != condition.right_attr.relation_name) && (0 != strcmp(condition.right_attr.relation_name, selects.relations[i]))))
        {
          LOG_WARN("Table name in where but not in from");
          return RC::SCHEMA_TABLE_NOT_EXIST;
        }
      }
    }
    else if (rel_num > 1)
    {
      if (condition.left_is_attr == 1)
      {
        // 检查列是否在
        bool left_col_found = false;
        for (size_t i = 0; i < selects.relation_num; i++)
        {
          Table *table = DefaultHandler::get_default().find_table(db, selects.relations[i]);
          const TableMeta &table_meta = table->table_meta();
          const FieldMeta *field = table_meta.field(condition.left_attr.attribute_name);
          if (nullptr != field)
          {
            left_col_found = true;
            break;
          }
        }
        if (left_col_found == false)
        {
          LOG_ERROR("The column [%s] does not exist in table [%s].", condition.left_attr.attribute_name, selects.relations[i]);
          return RC::SCHEMA_TABLE_NOT_EXIST;
        }
        // LOG_INFO("condition.left_attr.relation_name: %s, %s", condition.left_attr.relation_name, condition.left_attr.attribute_name);
        // where中的表名必须出现，不存在*
        if (nullptr == condition.left_attr.relation_name)
        {
          LOG_ERROR("Table name must appear.");
          return RC::SCHEMA_TABLE_NOT_EXIST;
        }
        bool left_is_found = false;
        for (int j = 0; j < rel_num; j++)
        {
          if (0 == strcmp(condition.left_attr.relation_name, selects.relations[j]))
          {
            left_is_found = true;
            break;
          }
        }
        if (left_is_found == false)
        {
          LOG_WARN("Table name %s appears in where but not in from", condition.left_attr.relation_name);
          return RC::SCHEMA_TABLE_NOT_EXIST;
        }
      }

      if (condition.right_is_attr == 1)
      {
        // 检查列是否在
        bool right_col_found = false;
        for (size_t i = 0; i < selects.relation_num; i++)
        {
          Table *table = DefaultHandler::get_default().find_table(db, selects.relations[i]);
          const TableMeta &table_meta = table->table_meta();
          const FieldMeta *field = table_meta.field(condition.right_attr.attribute_name);
          if (nullptr != field)
          {
            right_col_found = true;
            break;
          }
        }
        if (right_col_found == false)
        {
          LOG_ERROR("The column [%s] does not exist in table [%s].", condition.right_attr.attribute_name, selects.relations[i]);
          return RC::SCHEMA_TABLE_NOT_EXIST;
        }
        // where中的表名必须出现，不存在*
        if (nullptr == condition.right_attr.relation_name)
        {
          LOG_ERROR("Table name must appear.");
          return RC::SCHEMA_TABLE_NOT_EXIST;
        }
        bool right_is_found = false;
        for (int j = 0; j < rel_num; j++)
        {
          if (0 == strcmp(condition.right_attr.relation_name, selects.relations[j]))
          {
            right_is_found = true;
            break;
          }
        }
        if (right_is_found == false)
        {
          LOG_WARN("Table name %s appears in where but not in from", condition.right_attr.relation_name);
          return RC::SCHEMA_TABLE_NOT_EXIST;
        }
      }
    }
  }

  return RC::SUCCESS;
}

// 这里没有对输入的某些信息做合法性校验，比如查询的列名、where条件中的列名等，没有做必要的合法性校验
// 需要补充上这一部分. 校验部分也可以放在resolve，不过跟execution放一起也没有关系
RC ExecuteStage::do_select(const char *db, Query *sql, SessionEvent *session_event)
{
  RC rc = RC::SUCCESS;
  Session *session = session_event->get_client()->session;
  Trx *trx = session->current_trx();
  const Selects &selects = sql->sstr.selection;

  // 这里先检查Select语句的合法性
  rc = check_table_name(selects, db);
  if (rc != RC::SUCCESS)
  {
    session_event->set_response("FAILURE\n");
    end_trx_if_need(session, trx, false);
    return rc;
  }

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
    LOG_INFO("成功创建selection_executor");
    select_nodes.push_back(select_node);
  }
  LOG_INFO("select_nodes.size: %d", select_nodes.size());

  if (select_nodes.empty())
  {
    LOG_ERROR("No table given");
    session_event->set_response("FAILURE\n");
    end_trx_if_need(session, trx, false);
    return RC::SQL_SYNTAX;
  }
  LOG_INFO("select_nodes's size: %d", select_nodes.size());

  std::vector<TupleSet> tuple_sets;
  for (SelectExeNode *&node : select_nodes)
  {
    TupleSet tuple_set;
    // excute里设置了聚合函数的type，type定义于tuple.h文件
    LOG_INFO("开始执行select_node->execute函数");
    rc = node->execute(tuple_set);
    LOG_INFO("node->execute完毕并返回 rc=%d", rc);
    if (rc != RC::SUCCESS)
    {
      LOG_INFO("node->execute失败 rc=%d", rc);
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
  if (tuple_sets.size() > 1)
  { // 本次查询了多张表，需要做join操作
    isMultiTable = true;
    result = do_join(tuple_sets, selects, db);
  }
  else
  {
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

      if (attr.window_function_name != nullptr)
      {
        // 注意这里attr.relation_name可能为nullptr
        FuncType function_type = judge_function_type(attr.window_function_name);
        attr_function->add_function_type(std::string(attr.attribute_name), function_type, attr.relation_name);
      }
    }

    rc = do_aggregation(&result, attr_function, results, selects.relation_num);

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

    if (results.size() != 0)
    {
      result = std::move(results[0]);
      results.clear();
    }
  }
  ////////////////////////////聚合函数结束/////////////////////////////

  if (selects.order_num > 0)
  {
    // 当前没有group by，先假设聚合和排序是矛盾的，仍然用tuples_sets进行快速排序
    ///////////////////////////////////排序开始////////////////////////////////
    // 提取排序信息
    OrderInfo *order_info = new OrderInfo;
    const TupleSchema &schema = result.get_schema();

    for (int i = selects.order_num - 1; i >= 0; i--)
    {
      int cnt = 0;
      const RelAttr &attr = selects.order_attrs[i];
      // 确定该属性与这张表有关
      int index = -1;
      if (attr.relation_name != nullptr)
      {
        index = schema.index_of_field(attr.relation_name, attr.attribute_name);
      }
      else
      {
        // 不带属性名，可能出现二义性问题
        const char *last_table_name = nullptr;
        const int size = schema.fields().size();

        for (int i = 0; i < size; ++i)
        {
          if (strcmp(attr.attribute_name, schema.field(i).field_name()) == 0)
          {
            if (cnt == 0)
            {
              ++cnt;
              last_table_name = schema.field(i).table_name();
              index = i;
            }
            else
            {
              if (last_table_name != schema.field(i).table_name())
              {
                ++cnt;
                break;
              }
            }
          }
        }
      }

      if (index == -1 || cnt > 1)
      {
        // 有order信息但没有提取出来，说明出现错误的列名
        for (SelectExeNode *&tmp_node : select_nodes)
        {
          delete tmp_node;
        }

        session_event->set_response("FAILURE\n");
        end_trx_if_need(session, trx, true);
        return RC::GENERIC_ERROR;
      }
      order_info->add(attr.attribute_name, index, attr.is_desc == 1);
    }

    quick_sort(&result, 0, result.size() - 1, order_info);
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
RC do_aggregation(TupleSet *tuple_set, AttrFunction *attr_function, std::vector<TupleSet> &results, int rel_num)
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
    std::string add_scheme_name = attr_function->to_string(j, rel_num);

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
      // tmp_tuple.add((int)tuple_set->tuples().size());

      int ans = 0;

      for (int tuple_i = 0; tuple_i < tuple_set->size(); ++tuple_i)
      {
        std::shared_ptr<TupleValue> value = tuple_set->get(tuple_i).get_pointer(index);
        if (!value->is_null())
        {
          ++ans;
        }
      }
      tmp_tuple.add(ans);
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
      add_type = AttrType::FLOATS;

      int size = (int)tuple_set->tuples().size();
      float ans = 0;
      int cnt = 0;

      if (type == AttrType::FLOATS)
      {
        for (int tuple_i = 0; tuple_i < tuple_set->size(); ++tuple_i)
        {
          std::shared_ptr<TupleValue> val = tuple_set->get(tuple_i).get_pointer(index);
          if (val->is_null())
          {
            continue;
          }
          std::shared_ptr<FloatValue> value = std::dynamic_pointer_cast<FloatValue>(val);
          ans += value->get_value();
          ++cnt;
        }
      }
      else if (type == AttrType::INTS)
      {
        for (int tuple_i = 0; tuple_i < tuple_set->size(); ++tuple_i)
        {
          std::shared_ptr<TupleValue> val = tuple_set->get(tuple_i).get_pointer(index);
          if (val->is_null())
          {
            continue;
          }
          std::shared_ptr<IntValue> value = std::dynamic_pointer_cast<IntValue>(val);
          ans += value->get_value();
          ++cnt;
        }
      }

      if (size = 0 || cnt == 0)
      {
        // TODO: 显示什么，NULL会影响吗
        add_type = AttrType::CHARS;
        tmp_tuple.add("NULL", 4);
        break;
      }

      tmp_tuple.add(ans / cnt);

      break;
    }
    case FuncType::MAX:
    {
      int tuple_i = 0;
      for (; tuple_i < tuple_set->size(); ++tuple_i)
      {
        auto ans = tuple_set->get(tuple_i).get_pointer(index);
        if (!ans->is_null())
        {
          break;
        }
      }

      if (tuple_i == tuple_set->size())
      {
        // TODO: 显示什么，NULL会影响吗
        add_type = AttrType::CHARS;
        tmp_tuple.add("NULL", 4);
        break;
      }

      auto ans = tuple_set->get(tuple_i).get_pointer(index);
      for (; tuple_i < tuple_set->size(); ++tuple_i)
      {

        auto value = tuple_set->get(tuple_i).get_pointer(index);

        if (value->compare(*ans) > 0)
        {
          ans = value;
        }
      }

      add_type = type;

      if (type == AttrType::FLOATS)
      {
        tmp_tuple.add(std::dynamic_pointer_cast<FloatValue>(ans)->get_value());
      }
      else if (type == AttrType::INTS)
      {
        tmp_tuple.add(std::dynamic_pointer_cast<IntValue>(ans)->get_value());
      }
      else // AttrType::CHARS和DATES一样计算
      {
        tmp_tuple.add(std::dynamic_pointer_cast<StringValue>(ans)->get_value(),
                      std::dynamic_pointer_cast<StringValue>(ans)->get_len());
      }

      break;
    }
    case FuncType::MIN:
    {
      int tuple_i = 0;
      for (; tuple_i < tuple_set->size(); ++tuple_i)
      {
        auto ans = tuple_set->get(tuple_i).get_pointer(index);
        if (!ans->is_null())
        {
          break;
        }
      }

      if (tuple_i == tuple_set->size())
      {
        // TODO: 显示什么，NULL会影响吗
        add_type = AttrType::CHARS;
        tmp_tuple.add("NULL", 4);
        break;
      }

      auto ans = tuple_set->get(tuple_i).get_pointer(index);
      for (; tuple_i < tuple_set->size(); ++tuple_i)
      {

        auto value = tuple_set->get(tuple_i).get_pointer(index);

        if (value->compare(*ans) < 0)
        {

          ans = value;
        }
      }

      add_type = type;

      if (type == AttrType::FLOATS)
      {
        tmp_tuple.add(std::dynamic_pointer_cast<FloatValue>(ans)->get_value());
      }
      else if (type == AttrType::INTS)
      {
        tmp_tuple.add(std::dynamic_pointer_cast<IntValue>(ans)->get_value());
      }
      else // AttrType::CHARS和DATES一样计算
      {
        tmp_tuple.add(std::dynamic_pointer_cast<StringValue>(ans)->get_value(),
                      std::dynamic_pointer_cast<StringValue>(ans)->get_len());
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
    LOG_INFO("table_name_in_condition: %s", table_name_in_condition);
    LOG_INFO("table_name_to_match: %s", table_name_to_match);
    return 0 == strcmp(table_name_in_condition, table_name_to_match);
  }

  return selects.relation_num == 1;
}

static RC schema_add_field(Table *table, const char *field_name, TupleSchema &schema)
{
  const FieldMeta *field_meta = table->table_meta().field(field_name);
  if (nullptr == field_meta)
  {
    LOG_WARN("No such field. %s.%s", table->name(), field_name);
    return RC::SCHEMA_FIELD_MISSING;
  }

  schema.add_if_not_exists(field_meta->type(), table->name(), field_meta->name(), field_meta->nullable());
  return RC::SUCCESS;
}

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

// 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node)
{
  // 列出跟这张表关联的Attr
  // 1. 找到表

  TupleSchema schema;
  Table *table = DefaultHandler::get_default().find_table(db, table_name);

  // 2. 遍历Select中所有属性
  bool attrIsStar = false;
  // int rel_num = selects.relation_num;
  for (int i = selects.attr_num - 1; i >= 0; i--)
  {
    const RelAttr &attr = selects.attributes[i];

    // 确定该属性与这张表有关
    if (nullptr == attr.relation_name || 0 == strcmp(table_name, attr.relation_name))
    {
      // 对应select *的情况
      if (0 == strcmp("*", attr.attribute_name) || isdigit(attr.attribute_name[0]) || attr.attribute_name[0] == '-')
      {
        TupleSchema::from_table(table, schema); // 列出这张表所有字段
        attrIsStar = true;
        break; // 没有校验，给出* 之后，再写字段的错误
      }
      else
      { // 对应select age/ select t1.age的情况
        RC rc = schema_add_field(table, attr.attribute_name, schema);
        if (rc != RC::SUCCESS)
        {
          return rc;
        }
      }
    }
  } // for selects.attr_num

  // 找出仅与此表相关的过滤条件, 或者都是值的过滤条件
  // 构造schema, 包括select和where中需要的列
  std::vector<DefaultConditionFilter *> condition_filters;
  for (size_t i = 0; i < selects.condition_num; i++)
  {
    const Condition &condition = selects.conditions[i];

    // 这里其实已经做了下推，即先在单张表上进行了过滤
    if ((condition.left_is_attr == 0 && condition.right_is_attr == 0) ||                                                                         // 两边都是值
        (condition.left_is_attr == 1 && condition.right_is_attr == 0 && match_table(selects, condition.left_attr.relation_name, table_name)) ||  // 左边是属性右边是值
        (condition.left_is_attr == 0 && condition.right_is_attr == 1 && match_table(selects, condition.right_attr.relation_name, table_name)) || // 左边是值，右边是属性名
        (condition.left_is_attr == 1 && condition.right_is_attr == 1 && match_table(selects, condition.left_attr.relation_name, table_name) && match_table(selects, condition.right_attr.relation_name, table_name)))
    { // 左右都是属性名，并且表名都符合
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
    else if (condition.left_is_attr == 1 && condition.right_is_attr == 1)
    {
      // 如果select中不是*，需要把where中比较的列也添加到schema中
      if (attrIsStar == false)
      {
        if (0 == strcmp(condition.left_attr.relation_name, table_name))
        {
          RC rc = schema_add_field(table, condition.left_attr.attribute_name, schema);
          if (rc != RC::SUCCESS)
          {
            return rc;
          }
        }
        if (0 == strcmp(condition.right_attr.relation_name, table_name))
        {
          RC rc = schema_add_field(table, condition.right_attr.attribute_name, schema);
          if (rc != RC::SUCCESS)
          {
            return rc;
          }
        }
      }
    }

  } // for

  // 这里是为了处理 select t1.id from t1,t2; 这种情况
  if (schema.empty())
  {
    TupleSchema::from_table(table, schema);
  }

  return select_node.init(trx, table, std::move(schema), std::move(condition_filters));
}