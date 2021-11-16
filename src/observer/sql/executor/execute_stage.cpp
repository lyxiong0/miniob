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
#include <stack>
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

RC do_aggregation(TupleSet *tuple_set, AttrFunction *attr_function, std::vector<TupleSet> &results, int rel_num);

RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node, bool is_sub_select);

FuncType judge_function_type(char *agg_function_name);

RC calculate(const TupleSet &total_set, TupleSet &result, char *const *expression, int left, int right, std::vector<int> &is_include);

bool cmp_value(AttrType left_type, AttrType right_type, void *left_data, const std::shared_ptr<TupleValue> &right_value, CompOp op, const std::shared_ptr<TupleValue> &left_value, void *right_data);

int is_col_legal(const RelAttr &attr, const TupleSchema &schema);

void quick_sort(TupleSet *tuple_set, int l, int r, OrderInfo *order_info);

bool is_type_legal(AttrType left_type, AttrType right_type);

int find_rbrace(const char *s[], int start, int end);

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
    TupleSet tmp;
    is_related = false;
    do_select(current_db, sql->sstr.selection, exe_event->sql_event()->session_event(), tmp);
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

bool valueCompare(const TupleValue *value_a, const TupleValue *value_b, CompOp op)
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
bool isTupleSatisfy(const Tuple &tp, const TupleSchema &schema, const Selects &selects)
{
  bool valid = true;
  for (size_t i = 0; i < selects.condition_num; i++)
  {
    const Condition &cond = selects.conditions[i];
    // 只考虑两边都是attr的condition
    if (cond.left_is_attr == 1 && cond.right_is_attr == 1)
    {
      int left_index = schema.index_of_field(cond.left_attr.relation_name, cond.left_attr.attribute_name);
      int right_index = schema.index_of_field(cond.right_attr.relation_name, cond.right_attr.attribute_name);
      TupleValue *va = tp.get_pointer(left_index).get();
      TupleValue *vb = tp.get_pointer(right_index).get();
      if (valueCompare(va, vb, cond.comp) == false)
      {
        valid = false;
        break;
      }
    }
  }
  return valid;
}

// 检查condition两边的列是否再元组中
bool conditionInTuple(const TupleSchema &schema, const Condition &cond)
{
  bool left = false;
  bool right = false;
  auto fields = schema.fields();
  for (const auto &each : fields)
  {
    if ((0 == strcmp(cond.left_attr.relation_name, each.table_name())) &&
        (0 == strcmp(cond.left_attr.attribute_name, each.field_name())))
    {
      left = true;
    }
    if ((0 == strcmp(cond.right_attr.relation_name, each.table_name())) &&
        (0 == strcmp(cond.right_attr.attribute_name, each.field_name())))
    {
      right = true;
    }
  }
  return left && right;
}

void backtrack(TupleSet &ans, const std::vector<TupleSet> &sets, int index, Tuple &tmp, const Selects &selects, const TupleSchema &schema, TupleSchema &tmpSchema)
{
  // LOG_INFO("START OF backtrack:");
  // tmpSchema.print(std::cout, true);

  if (index == -1)
  {
    Tuple t(tmp);
    // 满足条件再加入
    if (isTupleSatisfy(t, schema, selects))
    {
      ans.add(std::move(t));
    }
  }
  else
  {

    int len = sets[index].size();
    for (int i = 0; i < len; i++)
    {
      tmp.merge(sets[index].get(i));
      tmpSchema.append(sets[index].get_schema());
      // 在这里应该检查一下相应的condition，不满足就不用回溯
      bool satisfied = true;
      for (size_t i = 0; i < selects.condition_num; i++)
      {
        const Condition &cond = selects.conditions[i];
        if (cond.right_is_attr == 2)
        {
          continue;
        }
        if ((cond.left_is_attr == 1) && (cond.right_is_attr == 1))
        {
          // LOG_INFO("Condition %d: %s.%s, %s.%s", i, cond.left_attr.relation_name, cond.left_attr.attribute_name, cond.right_attr.relation_name, cond.right_attr.attribute_name);
          // tmpSchema.print(std::cout, true);
          if (conditionInTuple(tmpSchema, cond))
          {
            int left_index = tmpSchema.index_of_field(cond.left_attr.relation_name, cond.left_attr.attribute_name);
            int right_index = tmpSchema.index_of_field(cond.right_attr.relation_name, cond.right_attr.attribute_name);
            TupleValue *va = tmp.get_pointer(left_index).get();
            TupleValue *vb = tmp.get_pointer(right_index).get();
            if (valueCompare(va, vb, cond.comp) == false)
            {
              satisfied = false;
              break;
            }
          }
          else
          {
          }
        }
      }
      if (satisfied)
      {
        backtrack(ans, sets, index - 1, tmp, selects, schema, tmpSchema);
      }
      // LOG_INFO("index : %d", index);
      // LOG_INFO("sets[index].get_schema()");
      // sets[index].get_schema().print(std::cout, true);
      // LOG_INFO("tmpSchema:");
      // tmpSchema.print(std::cout, true);
      tmp.remove(sets[index].get(i));
      tmpSchema.remove(sets[index].get_schema());
    }
  }
}

TupleSet do_join(const std::vector<TupleSet> &sets, const Selects &selects, const char *db)
{
  // 构造一个表名到对应的TupleSet的下标的map
  // std::unordered_map<std::string, int> name2set;
  // for (int i = 0; i < sets.size(); i++) {
  //     const char* table_name = sets[i].get_schema().field(0).table_name();
  //     name2set[std::string(table_name, strlen(table_name))] = i;
  // }
  // 先根据所有的tuple_set构造一个包含所有列的TupleSet
  TupleSchema total_schema;
  TupleSet total_set;
  for (auto it = sets.rbegin(); it != sets.rend(); it++)
  {
    total_schema.append(it->get_schema());
  }
  total_set.set_schema(total_schema);

  int len_sets = sets.size();
  Tuple tmp;
  TupleSchema tmpSchema;
  backtrack(total_set, sets, len_sets - 1, tmp, selects, total_schema, tmpSchema);

  return total_set;
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
        LOG_WARN("Table [%s.%s] not in from", attr.relation_name, attr.attribute_name);
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
          LOG_INFO("%d - %d, i = %d", condition.left_is_attr, condition.right_is_attr, i);
          LOG_INFO("condition.left_attr.relation_name = %s, condition.right_attr.relation_name = %s, relation = %s", condition.left_attr.relation_name, condition.right_attr.relation_name, selects.relations[i]);
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
RC ExecuteStage::do_select(const char *db, const Selects &selects, SessionEvent *session_event, TupleSet &ret_tuple_set, bool is_sub_select, char *main_table)
{
  LOG_INFO("start do_select");
  RC rc = RC::SUCCESS;
  Session *session = session_event->get_client()->session;
  Trx *trx = session->current_trx();

  // 这里先检查Select语句的合法性
  rc = check_table_name(selects, db);
  if (rc != RC::SUCCESS)
  {
    // 如果是子查询，抛出给主查询处理
    if (!is_sub_select)
    {
      session_event->set_response("FAILURE\n");
      end_trx_if_need(session, trx, false);
    }
    return rc;
  }

  std::vector<SelectExeNode *> select_nodes;

  for (size_t i = 0; i < selects.relation_num; i++)
  {
    // 遍历所有表
    const char *table_name = selects.relations[i];

    SelectExeNode *select_node = new SelectExeNode;
    rc = create_selection_executor(trx, selects, db, table_name, *select_node, is_sub_select);
    if (rc != RC::SUCCESS)
    {
      delete select_node;
      for (SelectExeNode *&tmp_node : select_nodes)
      {
        delete tmp_node;
      }
      if (!is_sub_select)
      {
        session_event->set_response("FAILURE\n");
        end_trx_if_need(session, trx, false);
      }
      return rc;
    }

    select_nodes.push_back(select_node);
  }

  if (select_nodes.empty())
  {
    LOG_ERROR("No table given");
    if (!is_sub_select)
    {
      session_event->set_response("FAILURE\n");
      end_trx_if_need(session, trx, false);
    }
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
      LOG_INFO("node->execute失败 rc=%d", rc);
      for (SelectExeNode *&tmp_node : select_nodes)
      {
        delete tmp_node;
      }
      if (!is_sub_select)
      {
        session_event->set_response("FAILURE\n");
        end_trx_if_need(session, trx, false);
      }
      return rc;
    }
    else
    {
      tuple_sets.push_back(std::move(tuple_set));
    }
  }

  std::stringstream ss;
  TupleSet result;
  bool is_multi_table = false;
  if (tuple_sets.size() > 1)
  { // 本次查询了多张表，需要做join操作
    is_multi_table = true;

    result = do_join(tuple_sets, selects, db);
    if (result.get_schema().size() == 0)
    {
      // 出现错误
      for (SelectExeNode *&tmp_node : select_nodes)
      {
        delete tmp_node;
      }
      if (!is_sub_select)
      {
        session_event->set_response("FAILURE\n");
        end_trx_if_need(session, trx, true);
      }
      return RC::GENERIC_ERROR;
    }
  }
  else
  {
    // 当前只查询一张表，直接返回结果即可
    result = std::move(tuple_sets.front());
  }

  LOG_INFO("多表后");
  result.print(std::cout, true);

  // 在此执行子查询操作
  bool has_subselect = false;

  if (main_table == nullptr)
  {
    main_table = (char *)malloc(strlen(selects.relations[0]) + 1);
    memcpy(main_table, selects.relations[0], strlen(selects.relations[0]) + 1);
  }

  for (size_t i = 0; i < selects.condition_num; i++)
  {
    const Condition &condition = selects.conditions[i];
    // 查看是否有子查询
    if (condition.right_is_attr != 2)
    {
      continue;
    }

    // 处理子查询
    has_subselect = true;
    CompOp comp = condition.comp;

    Selects *sub_select = new Selects();
    memcpy(sub_select, condition.sub_select, sizeof(Selects));

    // 检查是否为关联子查询
    int n = sub_select->condition_num;

    for (size_t i = 0; i < n; i++)
    {
      const Condition &sub_cond = condition.sub_select->conditions[i];

      // 查看条件中是否存在与主查询相关的条件，关联子查询必有表名
      if (sub_cond.right_is_attr == 1 && sub_cond.right_attr.relation_name != nullptr && strcmp(sub_cond.right_attr.relation_name, main_table) == 0)
      {
        // 加入子查询
        sub_select->relations[sub_select->relation_num++] = main_table;
        // 加上group by
        sub_select->group_num = 0;
        sub_select->group_attrs[sub_select->group_num++] = sub_cond.right_attr;
        sub_select->attributes[sub_select->attr_num++] = sub_cond.right_attr;
        if (sub_select->attributes[0].agg_function_name == nullptr)
        {
          const char *tmp = "max";
          // sub_select->attributes[sub_select->attr_num - 1].agg_function_name = (char *)malloc(4);
          // memcpy(sub_select->attributes[sub_select->attr_num - 1].agg_function_name, tmp, 4);
          sub_select->attributes[0].agg_function_name = (char *)malloc(4);
          memcpy(sub_select->attributes[0].agg_function_name, tmp, 4);
        }

        LOG_INFO("关联子查询 - agg_name = %s, attr_name = %s, table_name = %s", sub_select->attributes[0].agg_function_name, sub_select->attributes[0].attribute_name, sub_select->attributes[0].relation_name);

        is_related = true;
      }
      else if (sub_cond.left_is_attr == 1 && sub_cond.left_attr.relation_name != nullptr && strcmp(sub_cond.left_attr.relation_name, main_table) == 0)
      {
        // 加入子查询
        sub_select->relations[sub_select->relation_num++] = main_table;
        // 加上group by
        sub_select->group_num = 0;
        sub_select->group_attrs[sub_select->group_num++] = sub_cond.left_attr;
        sub_select->attributes[sub_select->attr_num++] = sub_cond.left_attr;
        if (sub_select->attributes[0].agg_function_name == nullptr)
        {
          sub_select->attributes[0].agg_function_name = (char *)malloc(4);
          const char *tmp = "max";
          memcpy(sub_select->attributes[0].agg_function_name, tmp, 4);
        }

        is_related = true;
      }
    }

    free(condition.sub_select);
    TupleSet sub_res;

    rc = do_select(db, *sub_select, session_event, sub_res, true, main_table);
    LOG_INFO("子查询执行结果");
    sub_res.print(std::cout, true);

    if (rc != RC::SUCCESS)
    {
      rc = RC::GENERIC_ERROR;
      break;
    }

    // sub_res.print(std::cout);
    // LOG_INFO("---------");
    // result.print(std::cout);

    // 如果查询结果不为单列则不合法
    if (!is_related && sub_res.get_schema().size() != 1)
    {
      rc = RC::GENERIC_ERROR;
      break;
    }

    if (condition.left_is_attr == 2)
    {
      // 左右两侧都是子查询的情况
      TupleSet left_sub_res;
      rc = do_select(db, *condition.another_sub_select, session_event, left_sub_res, true);
      if (rc != RC::SUCCESS)
      {
        rc = RC::GENERIC_ERROR;

        break;
      }

      // 两侧都是子查询，只能值比值
      if (left_sub_res.get_schema().size() != 1 || left_sub_res.size() != 1 || sub_res.size() != 1)
      {
        rc = RC::GENERIC_ERROR;
        break;
      }

      AttrType left_type = left_sub_res.get_schema().field(0).type();
      AttrType right_type = sub_res.get_schema().field(0).type();
      if (!is_type_legal(left_type, right_type))
      {
        rc = RC::GENERIC_ERROR;
        break;
      }

      const std::shared_ptr<TupleValue> &left_data = left_sub_res.get(0).get_pointer(0);
      const std::shared_ptr<TupleValue> &right_data = sub_res.get(0).get_pointer(0);

      if (!cmp_value(left_type, right_type, nullptr, right_data, comp, left_data, nullptr))
      {
        // 比较，不相等则清空
        result.clear_tuples();
        break;
      }

      continue;
    }

    // LOG_INFO("sub_res.size() = %d", sub_res.size());

    if (sub_res.size() == 1 && sub_res.get_schema().field(0).type() == CHARS) {
      const char *value = std::dynamic_pointer_cast<StringValue>(sub_res.get(0).get_pointer(0))->get_value();
      if (strcmp(value, "NULL") == 0) {
        sub_res.clear_tuples();
      }
    }

    if (sub_res.size() == 0)
    {
      // 子查询没有结果，如果是not in保留所有结果，否则清空
      if (comp == NOT_IN)
      {
        continue;
      }

      result.clear_tuples();
      break;
    }

    // 提取右侧类型和TupleValue
    AttrType right_type = sub_res.get_schema().field(0).type();

    // 如果左侧是列，提取index
    int index = -1;
    if (condition.left_is_attr)
    {
      const char *rel_name = condition.left_attr.relation_name;
      const char *attr_name = condition.left_attr.attribute_name;

      if (rel_name != nullptr)
      {
        index = result.get_schema().index_of_field(rel_name, attr_name);
      }
      else
      {
        index = result.get_schema().index_of_field(attr_name);
      }

      if (index == -1)
      {
        // 出现错误的列名
        rc = RC::GENERIC_ERROR;
        break;
      }
    }

    // 提取左侧类型
    AttrType left_type;
    if (condition.left_is_attr)
    {
      left_type = result.get_schema().field(index).type();
    }
    else
    {
      left_type = condition.left_value.type;
    }

    if (!is_type_legal(left_type, right_type))
    {
      rc = RC::GENERIC_ERROR;
      break;
    }

    // 开始处理操作符
    if (comp != CompOp::NOT_IN && comp != CompOp::IN_SUB)
    {
      // 处理比较操作符，超过一行且非关联子查询则不合法
      if (sub_res.size() > 1 && !is_related)
      {
        rc = RC::GENERIC_ERROR;
        break;
      }

      const std::shared_ptr<TupleValue> &right_data = sub_res.get(0).get_pointer(0);

      if (condition.left_is_attr == 0)
      {
        // 左侧也是值
        bool cmp_res = cmp_value(condition.left_value.type, right_type, condition.left_value.data, right_data, condition.comp, right_data, nullptr);

        if (!cmp_res)
        {
          result.clear_tuples();
        }
      }
      else
      {
        // 左侧是列
        TupleSet tmp_res;
        TupleSchema tmp_schema;
        tmp_schema.append(result.get_schema());
        tmp_res.set_schema(tmp_schema);

        int n = result.size();
        for (int j = 0; j < n; ++j)
        {
          // 遍历result，找出满足条件的tuple
          if (!is_related)
          {
            if (cmp_value(left_type, right_type, nullptr, right_data, comp, result.get(j).get_pointer(index), nullptr))
            {
              result.copy_ith_to(tmp_res, j);
            }
          }
          else
          {
            // 处理关联子查询
            int k = sub_res.size() - 1;
            int last_index = sub_res.get_schema().size() - 1;
            for (; k >= 0; --k)
            {
              if (cmp_value(left_type, right_type, nullptr, sub_res.get(k).get_pointer(last_index), comp, result.get(j).get_pointer(index), nullptr) == 0)
              {
                break;
              }
            }

            if (k < 0)
            {
              // 出现错误
              rc = RC::GENERIC_ERROR;
              break;
            }

            if (cmp_value(left_type, right_type, nullptr, sub_res.get(k).get_pointer(0), comp, result.get(j).get_pointer(index), nullptr))
            {
              result.copy_ith_to(tmp_res, j);
            }
          }
        }

        result = std::move(tmp_res);
      }
    }
    else
    {
      // 处理操作符in/not in，用哈希表
      // 生成哈希表
      // std::unordered_set<size_t> target_set;
      // int n = sub_res.size();

      // for (int j = 0; j < n; ++j)
      // {
      //   target_set.insert(sub_res.get(j).get_pointer(0)->to_hash());
      // }

      // 查询验证
      if (condition.left_is_attr == 0)
      {
        // 左侧是值
        bool in_target = false;
        int sub_size = sub_res.size();

        for (int k = 0; k < sub_size; ++k)
        {
          if (cmp_value(left_type, right_type, condition.left_value.data, sub_res.get(k).get_pointer(0), CompOp::EQUAL_TO, sub_res.get(k).get_pointer(0), nullptr))
          {
            in_target = true;
            break;
          }
        }
        // switch (condition.left_value.type)
        // {
        // case AttrType::CHARS:
        // {
        //   std::string v = std::string((const char *)condition.left_value.data);
        //   std::hash<std::string> hash_func;
        //   in_target = target_set.find(hash_func(v)) != target_set.end();
        //   break;
        // }
        // case AttrType::DATES:
        // case AttrType::INTS:
        // {
        //   int v = *(int *)condition.left_value.data;
        //   std::hash<int> hash_func;
        //   in_target = target_set.find(hash_func(v)) != target_set.end();
        //   break;
        // }
        // case AttrType::FLOATS:
        // {
        //   int v = *(float *)condition.left_value.data;
        //   std::hash<float> hash_func;
        //   in_target = target_set.find(hash_func(v)) != target_set.end();
        //   break;
        // }
        // default:
        //   break;
        // }

        if (comp == CompOp::IN_SUB && !in_target)
        {
          result.clear_tuples();
        }
        else if (comp == CompOp::NOT_IN && in_target)
        {
          result.clear_tuples();
        }
      }
      else
      {
        // 左侧是列
        TupleSet tmp_res;
        tmp_res.set_schema(result.get_schema());

        int m = result.size();
        for (int j = 0; j < m; ++j)
        {
          // 遍历result，找出满足条件的tuple
          // bool in_target = target_set.find(result.get(j).get_pointer(index)->to_hash()) != target_set.end();
          // LOG_INFO("index = %d", index);

          // if (comp == CompOp::IN_SUB)
          // {
          //   if (in_target)
          //   {
          //     result.copy_ith_to(tmp_res, j);
          //   }
          // }
          // else
          // {
          //   if (is_related || !in_target)
          //   {
          //     result.copy_ith_to(tmp_res, j);
          //   }
          // }
          bool in_target = false;
          int sub_size = sub_res.size();

          for (int k = 0; k < sub_size; ++k)
          {
            if (cmp_value(left_type, right_type, nullptr, sub_res.get(k).get_pointer(0), CompOp::EQUAL_TO, result.get(j).get_pointer(index), nullptr))
            {
              in_target = true;
              break;
            }
          }

          if (comp == CompOp::IN_SUB)
          {
            if (in_target)
            {
              result.copy_ith_to(tmp_res, j);
            }
          }
          else if (is_related)
          {
            result.copy_ith_to(tmp_res, j);
          }
          else
          {
            if (!in_target)
            {
              result.copy_ith_to(tmp_res, j);
            }
          }
        }

        result = std::move(tmp_res);
      }
    }

    rc = RC::SUCCESS;
    LOG_INFO("子查询结束");
    result.print(std::cout, true);
  }

  // 子查询结束
  LOG_INFO("子查询结束");
  result.print(std::cout, true);

  if (rc != RC::SUCCESS)
  {
    for (SelectExeNode *&tmp_node : select_nodes)
    {
      delete tmp_node;
    }
    if (!is_sub_select)
    {
      session_event->set_response("FAILURE\n");
      end_trx_if_need(session, trx, true);
    }
    return RC::GENERIC_ERROR;
  }

  // 表达式处理
  // 记录<表名+列名，对应index>的哈希表
  // std::unordered_map<size_t, int> map;
  // std::hash<const char *> hash_func;
  // for (int i = result.get_schema().size() - 1; i >= 0; --i)
  // {
  //   const char *table_name = result.get_schema().field(i).table_name();
  //   const char *field_name = result.get_schema().field(i).field_name();
  //   size_t key;

  //   if (table_name == nullptr)
  //   {
  //     key = hash_func(field_name);
  //   }
  //   else
  //   {
  //     key = hash_func(field_name) ^ hash_func(table_name);
  //   }

  //   map[key] = i;
  // }
  // 处理where里的表达式，即left_is_attr, right_is_attr至少有一个== 3
  for (int i = selects.condition_num - 1; i >= 0; --i)
  {
    std::vector<int> is_include(result.size(), 1);
    const Condition &condition = selects.conditions[i];
    TupleSet left_result;
    TupleSet right_result;
    int n = result.size();

    LOG_INFO("condition.left_is_attr = %d, right_is_attr = %d", condition.left_is_attr, condition.right_is_attr);

    if (condition.left_is_attr == 3)
    {
      if (calculate(result, left_result, condition.expression, 0, condition.exp_num, is_include) != RC::SUCCESS)
      {
        for (SelectExeNode *&tmp_node : select_nodes)
        {
          delete tmp_node;
        }

        if (!is_sub_select)
        {
          session_event->set_response("FAILURE\n");
          end_trx_if_need(session, trx, true);
        }
        return RC::GENERIC_ERROR;
      }

      LOG_INFO("where左表达式计算结果：");
      left_result.print(std::cout, true);
    }

    if (condition.right_is_attr == 3)
    {
      if (calculate(result, right_result, condition.right_expression, 0, condition.right_exp_num, is_include) != RC::SUCCESS)
      {
        for (SelectExeNode *&tmp_node : select_nodes)
        {
          delete tmp_node;
        }

        if (!is_sub_select)
        {
          session_event->set_response("FAILURE\n");
          end_trx_if_need(session, trx, true);
        }
        return RC::GENERIC_ERROR;
      }

      LOG_INFO("where右表达式计算结果：");
      // right_result.print(std::cout, true);
    }

    if (condition.left_is_attr == 3 && condition.right_is_attr == 0)
    {
      // 左表达式右值
      AttrType right_type = condition.right_value.type;
      void *right_data = condition.right_value.data;

      if (right_type == NULLS || right_type == CHARS)
      {
        for (SelectExeNode *&tmp_node : select_nodes)
        {
          delete tmp_node;
        }

        if (!is_sub_select)
        {
          session_event->set_response("FAILURE\n");
          end_trx_if_need(session, trx, true);
        }
        return RC::GENERIC_ERROR;
      }

      TupleSet new_result;
      new_result.set_schema(result.get_schema());

      for (int j = 0; j < left_result.size(); ++j)
      {
        const std::shared_ptr<TupleValue> &left_value = left_result.get(j).get_pointer(0);
        if (cmp_value(FLOATS, right_type, nullptr, left_value, condition.comp, left_value, right_data) && is_include[j] == 1)
        {
          result.copy_ith_to(new_result, j);
        }
      }

      result = std::move(new_result);
    }

    if (condition.left_is_attr == 0 && condition.right_is_attr == 3)
    {
      // 右表达式左值
      AttrType left_type = condition.left_value.type;
      void *left_data = condition.left_value.data;

      if (left_type == NULLS || left_type == CHARS)
      {
        for (SelectExeNode *&tmp_node : select_nodes)
        {
          delete tmp_node;
        }

        if (!is_sub_select)
        {
          session_event->set_response("FAILURE\n");
          end_trx_if_need(session, trx, true);
        }
        return RC::GENERIC_ERROR;
      }

      TupleSet new_result;
      new_result.set_schema(result.get_schema());
      for (int j = 0; j < right_result.size(); ++j)
      {
        const std::shared_ptr<TupleValue> &right_value = right_result.get(j).get_pointer(0);
        if (cmp_value(left_type, FLOATS, left_data, right_value, condition.comp, right_value, nullptr) && is_include[j] == 1)
        {
          result.copy_ith_to(new_result, j);
        }
      }

      result = std::move(new_result);
    }

    if (condition.left_is_attr == 3 && condition.right_is_attr == 3)
    {
      // 左右都是表达式
      TupleSet new_result;
      new_result.set_schema(result.get_schema());
      for (int j = 0; j < right_result.size(); ++j)
      {
        const std::shared_ptr<TupleValue> &left_value = left_result.get(j).get_pointer(0);
        const std::shared_ptr<TupleValue> &right_value = right_result.get(j).get_pointer(0);
        if (cmp_value(FLOATS, FLOATS, nullptr, right_value, condition.comp, left_value, nullptr) && is_include[j] == 1)
        {
          result.copy_ith_to(new_result, j);
        }
      }

      result = std::move(new_result);
    }
  }

  LOG_INFO("经过where表达式计算后");
  result.print(std::cout, true);

  // tuple_to_indexes记录<group by哈希值，在result中对应的列>
  std::unordered_map<size_t, std::vector<int>> tuple_to_indexes;
  std::vector<TupleSet> results;
  if (selects.group_num > 0)
  {
    // group by特点：不在group by中的列，在select中只能以聚合函数形式出现
    // 检查group by列名是否存在
    std::vector<int> group_idx; // 用于group by的列在tutal_set中的index
    int n = selects.group_num;

    for (int i = 0; i < n; ++i)
    {
      const RelAttr &attr = selects.group_attrs[i];
      // int index = is_col_legal(attr, result.get_schema());
      int index = -1;
      if (attr.relation_name == nullptr)
      {
        index = result.get_schema().index_of_field(attr.attribute_name);
      }
      else
      {
        index = result.get_schema().index_of_field(attr.relation_name, attr.attribute_name);
      }

      if (index == -1)
      {
        LOG_ERROR("出现错误列名或者二义性问题");
        for (SelectExeNode *&tmp_node : select_nodes)
        {
          delete tmp_node;
        }

        if (!is_sub_select)
        {
          session_event->set_response("FAILURE\n");
          end_trx_if_need(session, trx, true);
        }
        return RC::GENERIC_ERROR;
      }

      LOG_INFO("group index add - %d", index);
      group_idx.emplace_back(index);
    }

    // 存放<Tuple哈希值，Tuple在TupleSet中的index>
    // 遍历total set，哈希分组
    n = result.size();
    for (int i = 0; i < n; ++i)
    {
      tuple_to_indexes[result.get(i).to_hash(group_idx)].emplace_back(i);
    }

    // 遍历哈希桶，做成vector<TupleSet>的形式
    results = std::vector<TupleSet>(tuple_to_indexes.size());
    int i = 0;
    for (auto iter = tuple_to_indexes.begin(); iter != tuple_to_indexes.end(); ++iter)
    {
      const std::vector<int> hash_bin = iter->second;
      results[i].set_schema(result.get_schema());
      for (const int &idx : hash_bin)
      {
        result.copy_ith_to(results[i], idx);
      }
      ++i;
    }
  }
  //////////////////////////group by结束/////////////////////////////
  LOG_INFO("经过group by后");
  result.print(std::cout, true);

  // Select表达式计算
  TupleSet new_result;
  TupleSchema new_schema;
  int size = result.size();
  bool is_star = false;
  std::vector<int> is_include(result.size(), 1);
  bool is_exp = false;

  for (int i = 0; i < selects.total_exp; ++i) {
    if (selects.exp_num[i] > 1) {
      is_exp = true;
      is_multi_table = tuple_sets.size() > 1;
      break;
    }
  }

  for (int i = 0; i < selects.total_exp; ++i)
  {
    TupleSet tmp;

    if (selects.exp_num[i] > 1)
    {
      // 参考LeetCode 772计算器
      if (calculate(result, tmp, selects.expression[i], 0, selects.exp_num[i], is_include) != RC::SUCCESS)
      {
        for (SelectExeNode *&tmp_node : select_nodes)
        {
          delete tmp_node;
        }

        if (!is_sub_select)
        {
          session_event->set_response("FAILURE\n");
          end_trx_if_need(session, trx, true);
        }
        return RC::GENERIC_ERROR;
      }

      if (i == 0)
      {
        new_schema = tmp.get_schema();
        new_result = std::move(tmp);
      }
      else
      {
        new_schema.append(tmp.get_schema());
        for (int j = 0; j < tmp.size(); ++j)
        {
          new_result.merge(tmp.get(j), j);
        }
      }
    }
    else
    {
      // 没有算式，转移原始结果列
      // 处理ID或者ID.ID
      // 提取表名、列名，重复代码
      char tmp[20];
      int j = 0;
      const char *s = selects.expression[i][0];

      while (s[j] != '\0' && s[j] != '.')
      {
        tmp[j] = s[j];
        ++j;
      }

      tmp[j] = '\0';
      char *relation_name = nullptr;
      char *attribute_name = nullptr;

      if (s[j] == '.')
      {
        // ID.ID的形式
        relation_name = strdup(tmp);
        ++j;
        int idx = 0;
        while (s[j] != '\0')
        {
          tmp[idx++] = s[j];
          ++j;
        }

        tmp[idx] = '\0';
      }

      attribute_name = strdup(tmp);
      int index = -1;
      if (relation_name == nullptr)
      {
        index = result.get_schema().index_of_field(attribute_name);
      }
      else
      {
        index = result.get_schema().index_of_field(relation_name, attribute_name);
      }

      LOG_INFO("table_name = %s, attr_name = %s, index = %d", relation_name, attribute_name, index);
      if (strcmp(attribute_name, "*") == 0 || index == -1)
      {
        is_star = true;
        break;
      }
      if (relation_name == nullptr)
      {
        relation_name = strdup(result.get_schema().field(0).table_name());
      }

      if (is_multi_table && is_exp)
      {
        // 表达式多表，输出表名.列名
        char new_name[20] = "\0";
        strcat(new_name, relation_name);
        strcat(new_name, ".");
        strcat(new_name, attribute_name);
        new_schema.add(result.get_schema().field(index).type(), "", new_name, result.get_schema().field(index).is_nullable());
      }
      else
      {
        new_schema.add(result.get_schema().field(index).type(), relation_name, attribute_name, result.get_schema().field(index).is_nullable());
      }
      if (i == 0)
      {
        for (int j = 0; j < size; ++j)
        {
          Tuple t;
          t.add(result.get(j).get_pointer(index));
          new_result.add(std::move(t));
        }
      }
      else
      {
        for (int j = 0; j < size; ++j)
        {
          Tuple t;
          t.add(result.get(j).get_pointer(index));
          new_result.merge(t, j);
        }
      }
    }
  }

  if (selects.total_exp && !is_star)
  {
    // 如果出现*不会调用expression
    result.clear();
    // result = std::move(new_result);
    result.set_schema(new_schema);
    for (int i = 0; i < new_result.size(); ++i)
    {
      if (is_include[i])
      {
        new_result.copy_ith_to(result, i);
      }
    }
  }

  LOG_INFO("select表达式计算后, size = %d");
  result.print(std::cout, true);

  // for (int i = 0; i < tuple_to_indexes.size(); ++i) {
  //   results[i].print(std::cout);
  // }
  // result.print(std::cout);

  ////////////////////////////聚合函数开始/////////////////////////////
  // 由于加入group by，这里可能会开始出现多个TupleSet
  // 处理聚合函数
  AttrFunction *attr_function = new AttrFunction;
  for (int i = selects.attr_num - 1; i >= 0; i--)
  {
    const RelAttr &attr = selects.attributes[i];

    if (attr.agg_function_name != nullptr)
    {
      // 注意这里attr.relation_name可能为nullptr
      FuncType function_type = judge_function_type(attr.agg_function_name);
      attr_function->add_function_type(std::string(attr.attribute_name), function_type, attr.relation_name);
    }
    else if (selects.group_num > 0)
    {
      attr_function->add_function_type(std::string(attr.attribute_name), FuncType::NOFUNC, attr.relation_name);
    }
  }

  LOG_INFO("聚合函数之前");
  // result.print(std::cout, true);

  if (attr_function->get_size() > 0)
  {
    std::vector<TupleSet> tmp_res;
    int size = results.size();

    if (size == 0)
    {
      rc = do_aggregation(&result, attr_function, tmp_res, selects.relation_num);

      if (rc != RC::SUCCESS)
      {
        for (SelectExeNode *&tmp_node : select_nodes)
        {
          delete tmp_node;
        }
        if (!is_sub_select)
        {
          session_event->set_response("FAILURE\n");
          end_trx_if_need(session, trx, true);
        }
        return rc;
      }

      if (tmp_res[0].get_schema().size() > 0)
      {
        is_multi_table = false;
        result = std::move(tmp_res[0]);
        tmp_res.clear();
      }
    }
    else
    {
      for (int i = 0; i < size; ++i)
      {
        rc = do_aggregation(&results[i], attr_function, tmp_res, selects.relation_num);

        if (rc != RC::SUCCESS)
        {
          ;
          for (SelectExeNode *&tmp_node : select_nodes)
          {
            delete tmp_node;
          }
          if (!is_sub_select)
          {
            session_event->set_response("FAILURE\n");
            end_trx_if_need(session, trx, true);
          }
          return rc;
        }
      }

      is_multi_table = false;
      result = std::move(tmp_res[0]);
      for (int i = 1; i < size; ++i)
      {
        tmp_res[i].copy_ith_to(result, 0);
      }
    }
  }

  ////////////////////////////聚合函数结束/////////////////////////////

  // 有两种情况需要二次提取列
  // 1. 多表且没有group by，如果有group by，提取列已经在聚合里完成
  // 2. 单表且有子查询操作
  if (!is_exp && attr_function->get_size() == 0 && ((selects.relation_num > 1 && selects.group_num == 0) || (selects.relation_num == 1 && has_subselect)))
  {
    TupleSchema final_schema;
    const TupleSchema &result_schema = result.get_schema();

    // for (int i = selects.attr_num - 1; i >= 0; i--)
    int n = selects.attr_num;
    for (int i = 0; i < n; ++i)
    {
      const RelAttr &attr = selects.attributes[i];
      if (0 == strcmp(attr.attribute_name, "*"))
      {
        if (nullptr == attr.relation_name)
        {
          final_schema.append(result_schema);
        }
        else
        {
          Table *table = DefaultHandler::get_default().find_table(db, attr.relation_name);
          TupleSchema::from_table(table, final_schema);
        }
      }
      else
      {
        Table *table = nullptr;
        if (nullptr == attr.relation_name)
        {
          table = DefaultHandler::get_default().find_table(db, selects.relations[0]);
        }
        else
        {
          table = DefaultHandler::get_default().find_table(db, attr.relation_name);
        }
        schema_add_field(table, attr.attribute_name, final_schema);
      }
    }

    TupleSet final_set;
    // TupleSchema final_schema = buildSchema(selects, total_schema, db);
    final_set.set_schema(final_schema);

    LOG_INFO("提取列");
    // result.print(std::cout, true);
    // 取出需要的列
    for (const Tuple &tp : result.tuples())
    {
      Tuple t;

      for (const TupleField &s : final_schema.fields())
      {
        int index = -1;
        LOG_INFO("table_name = %s, field_name = %s", s.table_name(), s.field_name());
        if (s.table_name() != nullptr)
        {
          index = result_schema.index_of_field(s.table_name(), s.field_name());
        }
        else
        {
          index = result_schema.index_of_field(s.field_name());
        }
        t.add(tp.get_pointer(index));
      }
      final_set.add(std::move(t));
    }

    result = std::move(final_set);
  }

  if (selects.order_num > 0)
  {
    // 当前没有group by，先假设聚合和排序是矛盾的，仍然用tuples_sets进行快速排序
    ///////////////////////////////////排序开始////////////////////////////////
    // 提取排序信息
    OrderInfo *order_info = new OrderInfo;
    const TupleSchema &schema = result.get_schema();

    for (int i = selects.order_num - 1; i >= 0; i--)
    {
      const RelAttr &attr = selects.order_attrs[i];
      int index = is_col_legal(attr, schema);

      if (index == -1)
      {
        // 出现错误列名或者二义性问题
        for (SelectExeNode *&tmp_node : select_nodes)
        {
          delete tmp_node;
        }

        if (!is_sub_select)
        {
          session_event->set_response("FAILURE\n");
          end_trx_if_need(session, trx, true);
        }
        return RC::GENERIC_ERROR;
      }

      order_info->add(attr.attribute_name, index, attr.is_desc == 1);
    }

    quick_sort(&result, 0, result.size() - 1, order_info);
  }

  if (!is_sub_select)
  {
    LOG_INFO("is_exp = %d", is_exp);

    result.print(ss, is_multi_table, is_exp);

    session_event->set_response(ss.str());
    end_trx_if_need(session, trx, true);
  }
  else
  {
    // 子查询情况，将结果返回
    ret_tuple_set = std::move(result);
  }

  for (SelectExeNode *&tmp_node : select_nodes)
  {
    delete tmp_node;
  }

  return rc;
}

int is_col_legal(const RelAttr &attr, const TupleSchema &schema)
{
  int index = -1;
  int cnt = 0;

  LOG_INFO("is_col_legal - attr.relation_name = %s, attr.attribute_name = %s", attr.relation_name, attr.attribute_name);

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
    return -1;
  }

  return index;
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
      index = tuple_set->get_schema().index_of_field(attr_name);
      type = tuple_set->get_schema().field(index).type();
    }
    LOG_INFO("index = %d, table_name = %s, attr_name = %s", index, table_name, attr_name);
    // const TupleField &field = tuple_set->get_schema().field(index);

    if (func_type == FuncType::NOFUNC)
    {
      // 碰到group by
      if (table_name == nullptr)
      {
        tmp_scheme.add_if_not_exists(type, "", attr_name);
      }
      else
      {
        std::string add_name = table_name;
        add_name = add_name + "." + attr_name;
        tmp_scheme.add_if_not_exists(type, "", add_name.c_str());
      }
      auto ans = tuple_set->get(0).get_pointer(index);
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
      continue;
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

      if (size == 0 || cnt == 0)
      {
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

  TupleSet tmp_set;
  tmp_set.set_schema(tmp_scheme);
  if (tmp_tuple.size() > 0)
  {
    tmp_set.add(std ::move(tmp_tuple));
  }
  tmp_set.print(std::cout);
  results.push_back(std::move(tmp_set));

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

FuncType judge_function_type(char *agg_function_name)
{
  if (agg_function_name == nullptr)
  {
    return FuncType::NOFUNC;
  }
  // 转换成小写
  char *p = agg_function_name;
  while (*p)
  {
    *p = tolower(*p);
    ++p;
  }

  // 判断
  if (strcmp("count", agg_function_name) == 0)
  {
    return FuncType::COUNT;
  }
  else if (strcmp("avg", agg_function_name) == 0)
  {
    return FuncType::AVG;
  }
  else if (strcmp("max", agg_function_name) == 0)
  {
    return FuncType::MAX;
  }
  else if (strcmp("min", agg_function_name) == 0)
  {
    return FuncType::MIN;
  }

  return FuncType::NOFUNC;
}

// 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node, bool is_sub_select)
{
  // 列出跟这张表关联的Attr
  // 1. 找到表
  TupleSchema schema;
  TupleSchema group_schema;
  Table *table = DefaultHandler::get_default().find_table(db, table_name);

  // 提取group by中属性
  for (int i = selects.group_num - 1; i >= 0; --i)
  {
    const RelAttr &attr = selects.group_attrs[i];

    // 确定该属性与这张表有关
    if (nullptr == attr.relation_name || 0 == strcmp(table_name, attr.relation_name))
    {
      // 对应select *的情况
      if (0 == strcmp("*", attr.attribute_name) || isdigit(attr.attribute_name[0]) || attr.attribute_name[0] == '-')
      {
        LOG_ERROR("不能group by *");
        return RC::GENERIC_ERROR;
      }

      RC rc = schema_add_field(table, attr.attribute_name, group_schema);
      RC rc2 = schema_add_field(table, attr.attribute_name, schema);
      if (rc != RC::SUCCESS || rc2 != RC::SUCCESS)
      {
        return rc;
      }
    }
  }

  // 2. 遍历Select中所有属性
  bool attrIsStar = false;
  // int rel_num = selects.relation_num;
  int n = selects.attr_num;
  for (int i = 0; i < n; ++i)
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
        // 如果有group，检查合法性
        if (selects.group_num > 0)
        {
          int index = -1;
          if (attr.relation_name == nullptr)
          {
            index = group_schema.index_of_field(attr.attribute_name);
          }
          else
          {
            index = group_schema.index_of_field(attr.attribute_name);
          }

          // 如果属性不在group中，则一定要带有聚合函数
          if (index == -1 && attr.agg_function_name == nullptr)
          {
            LOG_ERROR("不允许select未带有聚合函数且没有在group by中的列, %s.%s", attr.relation_name, attr.attribute_name);
            return RC::GENERIC_ERROR;
          }
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

    if (condition.right_is_attr == 2)
    {
      // 如果select中不是*，需要把where中比较的列也添加到schema中
      // 出现子查询
      if (attrIsStar == false && condition.left_is_attr && (condition.left_attr.relation_name == nullptr || 0 == strcmp(condition.left_attr.relation_name, table_name)))
      {
        RC rc = schema_add_field(table, condition.left_attr.attribute_name, schema);
        if (rc != RC::SUCCESS)
        {
          return rc;
        }
      }
      continue;
    }

    if (condition.left_is_attr > 1 || condition.right_is_attr > 1)
    {
      continue;
    }

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

bool cmp_value(AttrType left_type, AttrType right_type, void *left_data, const std::shared_ptr<TupleValue> &right_value,
               CompOp op, const std::shared_ptr<TupleValue> &left_value, void *right_data)
{
  int ans = -1;

  switch (left_type)
  {
  case AttrType::CHARS:
  {
    const char *left;
    if (left_data != nullptr)
    {
      left = (const char *)left_data;
    }
    else
    {
      left = std::dynamic_pointer_cast<StringValue>(left_value)->get_value();
    }
    const char *right = std::dynamic_pointer_cast<StringValue>(right_value)->get_value();
    ans = strcmp(left, right);
    break;
  }
  case AttrType::DATES:
  {
    int left;
    if (left_data != nullptr)
    {
      left = *(int *)left_data;
    }
    else
    {
      left = std::dynamic_pointer_cast<IntValue>(left_value)->get_value();
    }
    ans = left - std::dynamic_pointer_cast<IntValue>(right_value)->get_value();
    break;
  }
  case AttrType::INTS:
  case AttrType::FLOATS:
  {
    if (left_type == AttrType::INTS && right_type == AttrType::INTS)
    {
      int left;
      if (left_data != nullptr)
      {
        left = *(int *)left_data;
      }
      else
      {
        left = std::dynamic_pointer_cast<IntValue>(left_value)->get_value();
      }

      int right = std::dynamic_pointer_cast<IntValue>(right_value)->get_value();

      ans = left - right;
    }

    // 混合类型或者都是float，转换成float比较
    float left;
    if (left_type == AttrType::INTS)
    {
      if (left_data != nullptr)
      {
        left = (*(int *)left_data) * 1.0;
      }
      else
      {
        left = 1.0 * std::dynamic_pointer_cast<IntValue>(left_value)->get_value();
      }
    }
    else
    {
      if (left_data != nullptr)
      {
        left = *(float *)left_data;
      }
      else
      {
        left = std::dynamic_pointer_cast<FloatValue>(left_value)->get_value();
      }
    }

    float right;
    if (right_type == AttrType::FLOATS)
    {
      if (right_data != nullptr)
      {
        right = *(float *)right_data;
      }
      else
      {
        right = std::dynamic_pointer_cast<FloatValue>(right_value)->get_value();
      }
    }
    else
    {
      if (right_data != nullptr)
      {
        right = (*(int *)right_data) * 1.0;
      }
      else
      {
        right = 1.0 * std::dynamic_pointer_cast<IntValue>(right_value)->get_value();
      }
    }

    float sub_res = left - right;
    // LOG_INFO("left = %f, right = %f, sub_res = %f", left, right, sub_res);
    if (sub_res > -1e-3 && sub_res < 1e-3)
    {
      ans = 0;
    }
    else
    {
      ans = sub_res > 0 ? 1 : -1;
    }

    break;
  }

  default:
    LOG_ERROR("错误的比较类型");
    break;
  }

  switch (op)
  {
  case CompOp::EQUAL_TO:
    return ans == 0;
    break;
  case CompOp::GREAT_EQUAL:
    return ans >= 0;
    break;
  case CompOp::GREAT_THAN:
    return ans > 0;
    break;
  case CompOp::LESS_EQUAL:
    return ans <= 0;
    break;
  case CompOp::LESS_THAN:
    return ans < 0;
    break;
  case CompOp::NOT_EQUAL:
    return ans != 0;
    break;
  default:
    LOG_ERROR("错误的运算符");
    break;
  }
}

bool is_type_legal(AttrType left_type, AttrType right_type)
{
  // 判断左右两侧类型是否可以比较
  if (left_type == AttrType::FLOATS || left_type == AttrType::INTS)
  {
    // INTS和FLOATS可以互相比较
    if (right_type != AttrType::INTS && right_type != AttrType::FLOATS)
    {
      return false;
    }
  }
  else if (left_type != right_type)
  {
    return false;
  }

  return true;
}

int find_rbrace(char *const *s, int start, int end)
{
  // 找到[start, end)中对应的右括号，如果没有则返回-1
  int level = 0;
  int i = start;

  for (; i < end; ++i)
  {
    if (strcmp(s[i], "(") == 0)
    {
      ++level;
    }
    else if (strcmp(s[i], ")") == 0)
    {
      --level;
      if (level == 0)
      {
        break;
      }
    }
  }

  return i == end ? -1 : i;
}

RC calculate(const TupleSet &total_set, TupleSet &result, char *const *expression, int left, int right, std::vector<int> &is_include)
{
  int i = left;
  int size = total_set.size();
  TupleSet pre;
  std::vector<TupleSet> tuple_stack;
  char sign = '+';
  char final_name[100] = "\0";
  bool add_to_stack;
  // std::vector<int> is_include(size, 1);

  for (int j = left; j < right; ++j)
  {
    strcat(final_name, expression[j]);
  }

  while (i < right)
  {
    add_to_stack = true;
    const char *s = expression[i];

    // 处理数字，或者表达式第一个为负数
    if (s[0] >= '0' && s[0] <= '9')
    {
      int j = 0;

      bool is_neg = false;
      if (s[j] == '-')
      {
        is_neg = true;
        ++j;
      }

      float digit = 0;
      while (s[j] != '\0' && s[j] != '.')
      {
        digit = digit * 10 + (s[j] - '0');
        ++j;
      }

      if (s[j] == '.')
      {
        // 存在小数
        float div_num = 10;
        ++j;
        while (s[j] != '\0')
        {
          digit += (s[j++] - '0') / div_num;
          div_num /= 10;
        }
      }

      if (is_neg)
      {
        digit = -digit;
      }

      TupleSchema pre_schema;
      pre_schema.add(FLOATS, "", s);
      for (int j = 0; j < size; ++j)
      {
        Tuple t;
        t.add(digit);
        pre.add(std::move(t));
      }
      pre.set_schema(pre_schema);
      add_to_stack = false;
    }
    else if ((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z'))
    {
      // 处理ID或者ID.ID
      // 提取表名、列名
      char tmp[20];
      int j = 0;

      while (s[j] != '\0' && s[j] != '.')
      {
        tmp[j] = s[j];
        ++j;
      }

      tmp[j] = '\0';
      char *relation_name = nullptr;
      char *attribute_name = nullptr;

      if (s[j] == '.')
      {
        // ID.ID的形式
        relation_name = strdup(tmp);
        ++j;
        int idx = 0;
        while (s[j] != '\0')
        {
          tmp[idx++] = s[j];
          ++j;
        }

        tmp[idx] = '\0';
      }

      attribute_name = strdup(tmp);
      int target_idx = -1;
      if (relation_name == nullptr)
      {
        target_idx = total_set.get_schema().index_of_field(attribute_name);
      }
      else
      {
        target_idx = total_set.get_schema().index_of_field(relation_name, attribute_name);
      }
      if (target_idx == -1)
      {
        return RC::GENERIC_ERROR;
      }

      AttrType type = total_set.get_schema().field(target_idx).type();
      if (type == CHARS)
      {
        // 暂定字符串不参与运算
        return RC::GENERIC_ERROR;
      }

      TupleSchema pre_schema;
      pre_schema.add(FLOATS, "", s);
      pre.set_schema(pre_schema);
      for (int j = 0; j < size; ++j)
      {
        // 全部转换成float计算
        Tuple t;
        float f;
        if (type == INTS)
        {
          f = std::dynamic_pointer_cast<IntValue>(total_set.get(j).get_pointer(target_idx))->get_value() * 1.0;
        }
        else
        {
          f = std::dynamic_pointer_cast<FloatValue>(total_set.get(j).get_pointer(target_idx))->get_value();
        }

        t.add(f);
        pre.add(std::move(t));
      }

      add_to_stack = false;
    }
    else if (strcmp(s, "(") == 0)
    {
      // 碰到左括号，把整个括号内当作一个新的被加数
      int j = find_rbrace(expression, i, right);
      // 没找到右括号，出现错误
      if (j == -1)
      {
        return RC::GENERIC_ERROR;
      }

      RC rc = calculate(total_set, pre, expression, i + 1, j, is_include);
      if (rc != RC::SUCCESS)
      {
        return rc;
      }
      i = j;
      add_to_stack = false;
    }
    else if (strcmp(s, ")") == 0)
    {
      // 出现右括号则错误
      return RC::GENERIC_ERROR;
    }

    if (i == right - 1)
    {
      add_to_stack = true;
    }

    if (add_to_stack)
    {
      // 处理操作符，将结果压栈
      if (i != left)
      {
        switch (sign)
        {
        case '+':
        {
          tuple_stack.push_back(std::move(pre));
          break;
        }
        case '-':
        {
          TupleSet tmp_set;
          tmp_set.set_schema(pre.get_schema());

          // pre已经全部转换成float形式
          for (int j = 0; j < size; ++j)
          {
            Tuple t;
            float f;

            f = -1.0 * std::dynamic_pointer_cast<FloatValue>(pre.get(j).get_pointer(0))->get_value();
            t.add(f);
            tmp_set.add(std::move(t));
          }

          tuple_stack.push_back(std::move(tmp_set));
          break;
        }
        case '*':
        {
          TupleSet tmp_set;
          tmp_set.set_schema(pre.get_schema());

          for (int j = 0; j < size; ++j)
          {
            Tuple t;
            float f;

            f = std::dynamic_pointer_cast<FloatValue>(tuple_stack.back().get(j).get_pointer(0))->get_value() * std::dynamic_pointer_cast<FloatValue>(pre.get(j).get_pointer(0))->get_value();
            t.add(f);
            tmp_set.add(std::move(t));
          }

          tuple_stack.pop_back();
          tuple_stack.push_back(std::move(tmp_set));
          break;
        }
        case '/':
        {
          tuple_stack.back().print(std::cout, true);
          pre.print(std::cout, true);

          TupleSet tmp_set;
          tmp_set.set_schema(pre.get_schema());

          for (int j = 0; j < size; ++j)
          {
            Tuple t;
            float f;
            float right = std::dynamic_pointer_cast<FloatValue>(pre.get(j).get_pointer(0))->get_value();

            if (right == 0)
            {
              // = 0需要把这行排除掉
              f = -999.9;
              is_include[j] = 0;
            }
            else
            {
              f = std::dynamic_pointer_cast<FloatValue>(tuple_stack.back().get(j).get_pointer(0))->get_value() / right;
            }
            t.add(f);
            tmp_set.add(std::move(t));
          }

          tuple_stack.pop_back();
          tuple_stack.push_back(std::move(tmp_set));
          break;
        }

        default:
          LOG_ERROR("错误操作符");
          return RC::GENERIC_ERROR;
        }
      }

      sign = s[0];
      pre.clear();
      // LOG_INFO("calculate - after sign = %c", sign);
      // tuple_stack.back().print(std::cout, true);
    }
    ++i;
  }

  // 剩下在栈里的全部弹出做加法
  TupleSchema result_schema;
  result_schema.add(FLOATS, "", final_name);
  // int k = 0;
  // result = std::move(tuple_stack[k++]);
  result = std::move(tuple_stack.back());
  tuple_stack.pop_back();

  // while (k != tuple_stack.size())
  while (!tuple_stack.empty())
  {
    TupleSet &other = tuple_stack.back();
    // other.print(std::cout);

    pre.clear();
    pre.set_schema(result_schema);

    for (int j = 0; j < size; ++j)
    {
      Tuple t;
      float f;

      f = std::dynamic_pointer_cast<FloatValue>(other.get(j).get_pointer(0))->get_value() + std::dynamic_pointer_cast<FloatValue>(result.get(j).get_pointer(0))->get_value();
      t.add(f);
      pre.add(std::move(t));
    }

    result = std::move(pre);
    tuple_stack.pop_back();
  }

  // 过滤除数为0的项
  pre.clear();
  pre.set_schema(result_schema);

  for (int j = 0; j < size; ++j)
  {
    // if (is_include[j] == 0)
    // {
    //   continue;
    // }
    Tuple t;
    float f;

    f = std::dynamic_pointer_cast<FloatValue>(result.get(j).get_pointer(0))->get_value();
    t.add(f);
    pre.add(std::move(t));
  }

  result = std::move(pre);

  result.set_schema(result_schema);
  LOG_INFO("表达式计算结果");
  result.print(std::cout, true);

  return RC::SUCCESS;
}
