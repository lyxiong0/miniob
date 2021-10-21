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
#include <unordered_map>

class SessionEvent;

class AttrFunction
{
public:
  void add_function_type(const std::string &attr_name, FuncType function_type)
  {
    attr_function_type_.emplace_back(attr_name, function_type);
  }

  void set_is_count(bool is_count)
  {
    is_count_ = is_count;
  }

  std::string to_string(int i)
  {
    FuncType type = attr_function_type_[i].second;
    std::string attr = attr_function_type_[i].first;
    std::string s;

    switch (type)
    {
    case FuncType::COUNT:
    {
      s = std::string("COUNT(") + attr + std::string(")");
      break;
    }
    case FuncType::AVG:
    {
      s = std::string("AVG(") + attr + std::string(")");
      break;
    }
    case FuncType::MAX:
    {
      s = std::string("MAX(") + attr + std::string(")");
      break;
    }
    case FuncType::MIN:
    {
      s = std::string("MIN(") + attr + std::string(")");
      break;
    }
    default:
      s = std::string("UNDEFINED(") + attr + std::string(")");
      break;
    }

    // return s.c_str();
    return s;
  }

  FuncType get_function_type(int i)
  {
    return attr_function_type_[i].second;
  }

  const char *get_attr_name(int i)
  {
    return attr_function_type_[i].first.c_str();
  }

  int get_size()
  {
    return attr_function_type_.size();
  }

  bool get_is_count()
  {
    return is_count_;
  }

  const std::pair<std::string, FuncType> &get_attr_function_type(int i)
  {
    return attr_function_type_[i];
  }

private:
  bool is_count_ = false; // 是否执行count函数
  std::vector<std::pair<std::string, FuncType>> attr_function_type_; // 存储<属性名，函数类型>
};

class OrderInfo
{
public:
  void add_is_desc(bool is_desc)
  {
    is_desc_.emplace_back(is_desc);
  }

  void add_attr_name(const char *attr_name)
  {
    attr_name_.emplace_back(attr_name);
  }

  void add_index(int idx)
  {
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

  // void from_table(const Table *table)
  // {
  //   const char *table_name = table->name();
  //   const TableMeta &table_meta = table->table_meta();
  //   const int field_num = table_meta.field_num();

  //   for (int i = 0; i < field_num; i++)
  //   {
  //     index_.emplace_back(i);
  //   }
  // }

private:
  // 包含一个表的排序信息
  std::vector<bool> is_desc_; // 默认为升序asc
  std::vector<const char *> attr_name_;
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
  RC do_select(const char *db, Query *sql, SessionEvent *session_event);

protected:
private:
  Stage *default_storage_stage_ = nullptr;
  Stage *mem_storage_stage_ = nullptr;
};

#endif //__OBSERVER_SQL_EXECUTE_STAGE_H__
