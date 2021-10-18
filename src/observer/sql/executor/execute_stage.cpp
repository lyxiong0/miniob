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

RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node, AttrFunction &attr_function);

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
  default:
  {
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
  std::vector<AttrFunction *> attr_functions; // 储存每个表的类型

  LOG_ERROR("start create");
  for (size_t i = 0; i < selects.relation_num; i++)
  {
    // 遍历所有表
    const char *table_name = selects.relations[i];
    SelectExeNode *select_node = new SelectExeNode;
    AttrFunction *attr_function = new AttrFunction;

    rc = create_selection_executor(trx, selects, db, table_name, *select_node, *attr_function);
    if (rc != RC::SUCCESS)
    {
      delete select_node;
      for (SelectExeNode *&tmp_node : select_nodes)
      {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      return rc;
    }

    select_nodes.push_back(select_node);
    attr_functions.push_back(attr_function);
  }

  if (select_nodes.empty())
  {
    LOG_ERROR("No table given");
    end_trx_if_need(session, trx, false);
    return RC::SQL_SYNTAX;
  }
  LOG_ERROR("after create");

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
  LOG_ERROR("after execute");
  // if (tuple_sets.size() > 1)
  // {
  //   // TODO(): 任务6 多表查询
  //   // 本次查询了多张表，需要做join操作
  //   // 生成tuple_sets
  // }
  // else
  // {
  //   // 当前只查询一张表，直接返回结果即可
  //   tuple_sets.front().print(ss);
  // }

  ////////////////////////////聚合函数开始/////////////////////////////
  LOG_ERROR("start function");
  std::vector<TupleSet> results;
  size_t n = tuple_sets.size();

  for (size_t i = 0; i < n; ++i)
  {
    TupleSchema tmp_scheme;
    Tuple tmp_tuple;
    // 每张查询有关的表对应一个tuple set
    // 仍然有可能存在空的tuple
    TupleSet *tuple_set = &tuple_sets[i];
    const char *table_name = tuple_set->get_schema().field(0).table_name();
    AttrFunction *attr_function = attr_functions[i];

    // COUNT(*)处理
    LOG_ERROR("attr_function->GetIsCount()2 = %d", attr_function->GetIsCount());
    if (attr_function->GetIsCount() == true)
    {
      LOG_ERROR("Start count");
      tmp_scheme.add_if_not_exists(AttrType::INTS, table_name, std::string("COUNT(*)").c_str());
      tmp_tuple.add((int)tuple_set->tuples().size());
      // tmp_scheme.print(std::cout);
      LOG_ERROR("size = %d", tmp_tuple.size());
    }

    LOG_ERROR("attr_function->GetSize() = %d", attr_function->GetSize());
    // 遍历所有带函数的属性
    RC rc = RC::SUCCESS;
    for (int j = 0; j < attr_function->GetSize(); ++j)
    {
      auto attr_name = attr_function->GetAttrName(j);
      auto func_type = attr_function->GetFunctionType(j);
      const char *add_scheme_name = attr_function->ToString(j).c_str();
      int index = tuple_set->get_schema().index_of_field(table_name, attr_name);
      // const TupleField &field = tuple_set->get_schema().field(index);
      auto type = tuple_set->get_schema().field(index).type();

      LOG_ERROR("attr_name = %s", attr_name);
      LOG_ERROR("table_name = %s", table_name);

      if (func_type == FUNCTION_TYPE::NOFUNC)
      {
        LOG_ERROR("未定义的聚合函数，跳过");
        continue;
      }

      LOG_ERROR("const char *name = %s", add_scheme_name);

      // 增加tuple
      switch (func_type)
      {
      case FUNCTION_TYPE::COUNT:
      {
        LOG_ERROR("start COUNT");
        // 增加Scheme
        LOG_ERROR("outside add_scheme_name = %s", add_scheme_name);
        tmp_scheme.add_if_not_exists(AttrType::INTS, table_name, add_scheme_name);

        // TODO: 考虑NULL值
        tmp_tuple.add((int)tuple_set->tuples().size());
        break;
      }
      case FUNCTION_TYPE::AVG:
      {
        if (type == AttrType::CHARS)
        {
          // CHARS不应该计算平均值
          LOG_ERROR("属性类型为CHARS或UNDEFINED，不应该计算AVG函数");
          rc = RC::GENERIC_ERROR;
          break;
        }
        // 增加Scheme
        LOG_ERROR("start AVG");
        LOG_ERROR("outside add_scheme_name = %s", add_scheme_name);
        tmp_scheme.add_if_not_exists(AttrType::FLOATS, table_name, add_scheme_name);

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
          LOG_ERROR("middle AVG");
        }

        tmp_tuple.add(ans / size);
        break;
      }
      case FUNCTION_TYPE::MAX:
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
        tmp_scheme.add_if_not_exists(type, table_name, add_scheme_name);

        if (type == AttrType::FLOATS)
        {
          tmp_tuple.add(std::dynamic_pointer_cast<FloatValue>(ans)->GetValue());
        }
        else if (type == AttrType::INTS)
        {
          tmp_tuple.add(std::dynamic_pointer_cast<IntValue>(ans)->GetValue());
        }
        else if (type == AttrType::CHARS)
        {
          tmp_tuple.add(std::dynamic_pointer_cast<StringValue>(ans)->GetValue(),
                        std::dynamic_pointer_cast<StringValue>(ans)->GetLen());
        }

        break;
      }
      case FUNCTION_TYPE::MIN:
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
        tmp_scheme.add_if_not_exists(type, table_name, add_scheme_name);

        if (type == AttrType::FLOATS)
        {
          tmp_tuple.add(std::dynamic_pointer_cast<FloatValue>(ans)->GetValue());
        }
        else if (type == AttrType::INTS)
        {
          tmp_tuple.add(std::dynamic_pointer_cast<IntValue>(ans)->GetValue());
        }
        else if (type == AttrType::CHARS)
        {
          tmp_tuple.add(std::dynamic_pointer_cast<StringValue>(ans)->GetValue(),
                        std::dynamic_pointer_cast<StringValue>(ans)->GetLen());
        }

        break;
      }
      default:
        break;
      }

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
  }

  ////////////////////////////聚合函数结束/////////////////////////////

  if (results.size() != 0)
  {
    results.front().print(ss);
  }
  else
  {
    tuple_sets.front().print(ss);
  }


  for (SelectExeNode *&tmp_node : select_nodes)
  {
    delete tmp_node;
  }

  session_event->set_response(ss.str());
  end_trx_if_need(session, trx, true);
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

  schema.add_if_not_exists(field_meta->type(), table->name(), field_meta->name());
  return RC::SUCCESS;
}

FUNCTION_TYPE JudgeFunctionType(char *window_function_name)
{
  LOG_ERROR("window_function_name = %s", window_function_name);

  if (window_function_name == nullptr)
  {
    return FUNCTION_TYPE::NOFUNC;
  }
  // 转换成小写
  char *p = window_function_name;
  while (*p)
  {
    *p = tolower(*p);
    ++p;
  }

  LOG_ERROR("window_function_name = %s", window_function_name);

  // 判断
  if (strcmp("count", window_function_name) == 0)
  {
    return FUNCTION_TYPE::COUNT;
  }
  else if (strcmp("avg", window_function_name) == 0)
  {
    return FUNCTION_TYPE::AVG;
  }
  else if (strcmp("max", window_function_name) == 0)
  {
    return FUNCTION_TYPE::MAX;
  }
  else if (strcmp("min", window_function_name) == 0)
  {
    return FUNCTION_TYPE::MIN;
  }

  return FUNCTION_TYPE::NOFUNC;
}

// 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node, AttrFunction &attr_function)
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

  // 2. 遍历Select中所有属性
  for (int i = selects.attr_num - 1; i >= 0; i--)
  {
    const RelAttr &attr = selects.attributes[i];

    // 确定该属性与这张表有关
    if (nullptr == attr.relation_name || 0 == strcmp(table_name, attr.relation_name))
    {
      FUNCTION_TYPE function_type = JudgeFunctionType(attr.window_function_name);

      // 对应select */ select count(*)的情况
      if (0 == strcmp("*", attr.attribute_name))
      {
        // 找到对应的表
        // 列出这张表所有字段
        TupleSchema::from_table(table, schema);
        if (function_type == FUNCTION_TYPE::COUNT)
        {
          attr_function.SetIsCount(true);
        }
        LOG_ERROR("bool = %d", attr_function.GetIsCount());

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
        // 聚合函数判断
        if (function_type != FUNCTION_TYPE::NOFUNC)
        {
          attr_function.AddFunctionType(std::string(attr.attribute_name), function_type);
        }
      }
    }
  }

  // 找出仅与此表相关的过滤条件, 或者都是值的过滤条件
  std::vector<DefaultConditionFilter *> condition_filters;
  for (size_t i = 0; i < selects.condition_num; i++)
  {
    const Condition &condition = selects.conditions[i];
    if ((condition.left_is_attr == 0 && condition.right_is_attr == 0) ||                                                                         // 两边都是值
        (condition.left_is_attr == 1 && condition.right_is_attr == 0 && match_table(selects, condition.left_attr.relation_name, table_name)) ||  // 左边是属性右边是值
        (condition.left_is_attr == 0 && condition.right_is_attr == 1 && match_table(selects, condition.right_attr.relation_name, table_name)) || // 左边是值，右边是属性名
        (condition.left_is_attr == 1 && condition.right_is_attr == 1 &&
         match_table(selects, condition.left_attr.relation_name, table_name) && match_table(selects, condition.right_attr.relation_name, table_name)) // 左右都是属性名，并且表名都符合
    )
    {
      DefaultConditionFilter *condition_filter = new DefaultConditionFilter();
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
  }

  return select_node.init(trx, table, std::move(schema), std::move(condition_filters));
}
