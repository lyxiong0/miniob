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

#ifndef __OBSERVER_SQL_EXECUTE_STAGE_H__
#define __OBSERVER_SQL_EXECUTE_STAGE_H__

#include "common/seda/stage.h"
#include "sql/parser/parse.h"
#include "rc.h"
#include "sql/executor/tuple.h"

#include <cctype>
#include <iomanip>
#include <unordered_set>
#include <unordered_map>

class SessionEvent;

class AttrFunction
{
public:
  void add_function_type(const std::string &attr_name, FuncType function_type, const char *table_name)
  {
    
    attr_function_type_.emplace_back(attr_name, function_type);
    table_names_.emplace_back(table_name);
  }

  std::string to_string(int i, int rel_num)
  {
    FuncType type = attr_function_type_[i].second;
    std::string attr = attr_function_type_[i].first;
    std::string s;

    switch (type)
    {
    case FuncType::COUNT:
    {
      s = std::string("count(");
    }
    break;

    case FuncType::AVG:
    {
      s = std::string("avg(");
    }
    break;

    case FuncType::MAX:
    {
      s = std::string("max(");
    }
    break;

    case FuncType::MIN:
    {
      s = std::string("min(");
    }
    break;

    default:
      s = std::string("undefined(");
      break;
    }

    if (table_names_[i] != nullptr && rel_num > 1) {
      // 修改：只在多表的时候显示表名
      s = s + std::string(table_names_[i]) + std::string(".");
    }

    s = s + attr + std::string(")");

    return s;
  }

  FuncType get_function_type(int i)
  {
    return attr_function_type_[i].second;
  }

  const char *get_table_name(int i)
  {
    return table_names_[i];
  }

  const char *get_attr_name(int i)
  {
    return attr_function_type_[i].first.c_str();
  }

  int get_size()
  {
    return attr_function_type_.size();
  }

private:
  std::vector<std::pair<std::string, FuncType>> attr_function_type_; // 存储<属性名，函数类型>
  std::vector<const char *> table_names_;                            // 存储对应的table名
};

class OrderInfo
{
public:
  void add(const char *attr_name, int idx, bool is_desc = false)
  {
    is_desc_.emplace_back(is_desc);
    attr_name_.emplace_back(attr_name);
    index_.emplace_back(idx);
  }

  int get_size()
  {
    return is_desc_.size();
  }

  bool get_is_desc(int i)
  {
    return is_desc_[i];
  }

  int get_index(int i)
  {
    return index_[i];
  }

  const std::vector<int> &indexes() {
    return index_;
  }

private:
  // 包含一个表的排序信息
  std::vector<bool> is_desc_; // 默认为升序asc
  std::vector<const char *> attr_name_;
  std::vector<int> index_; // attr_name对应tuple里的index
};

class GroupInfo
{
public:
  void add(const char *attr_name, int idx)
  {
    index_.emplace_back(idx);
    attr_names_.insert(attr_name);
  }

  const std::vector<int> &indexes() {
    return index_;
  }

  bool is_attr_exist(const char *attr_name) {
    return attr_names_.find(attr_name) != attr_names_.end();
  }

private:
  // 包含一个表的排序信息
  std::unordered_set<const char *> attr_names_;
  std::vector<int> index_; // attr_name对应tuple里的index
};

class ExecuteStage : public common::Stage
{
public:
  ~ExecuteStage();
  static Stage *make_stage(const std::string &tag);

protected:
  // common function
  ExecuteStage(const char *tag);
  bool set_properties() override;

  bool initialize() override;
  void cleanup() override;
  void handle_event(common::StageEvent *event) override;
  void callback_event(common::StageEvent *event,
                      common::CallbackContext *context) override;

  void handle_request(common::StageEvent *event);
  RC do_select(const char *db, const Selects &selects, SessionEvent *session_event, TupleSet &ret_tuple_set, bool is_sub_select = false, char *main_table = nullptr);

protected:
private:
  Stage *default_storage_stage_ = nullptr;
  Stage *mem_storage_stage_ = nullptr;
  bool is_related = false;
};

#endif //__OBSERVER_SQL_EXECUTE_STAGE_H__
