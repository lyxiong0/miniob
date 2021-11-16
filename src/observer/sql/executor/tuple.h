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
// Created by Wangyunlai on 2021/5/14.
//

#ifndef __OBSERVER_SQL_EXECUTOR_TUPLE_H_
#define __OBSERVER_SQL_EXECUTOR_TUPLE_H_

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "sql/parser/parse.h"
#include "sql/executor/value.h"

enum FuncType
{
  COUNT,
  MAX,
  MIN,
  AVG,
  NOFUNC
};

class Table;
class OrderInfo;

class Tuple
{
public:
  Tuple() = default;

  Tuple(const Tuple &other);

  ~Tuple() = default;

  Tuple(Tuple &&other) noexcept;
  Tuple &operator=(Tuple &&other) noexcept;

  void add(TupleValue *value);
  void add(const std::shared_ptr<TupleValue> &other);
  void add(int value, bool is_null = false);
  void add(float value, bool is_null = false);
  void add(const char *s, int len, bool is_null = false);

  // 以下两个函数是成对出现的
  void merge(const Tuple &other)
  {
    values_.insert(values_.end(), other.values().begin(), other.values().end());
  }

  void remove(const Tuple &other)
  {
    auto iter = std::find(values_.begin(), values_.end(), other.get_pointer(0));
    values_.erase(iter, values_.end());
  }

  void print(std::ostream &os) const
  {
    for (const auto &each : values_)
    {
      each->to_string(os);
    }
    os << std::endl;
  }

  const std::vector<std::shared_ptr<TupleValue>> &values() const
  {
    return values_;
  }

  int size() const
  {
    return values_.size();
  }

  const TupleValue &get(int index) const
  {
    return *values_[index];
  }

  const std::shared_ptr<TupleValue> &get_pointer(int index) const
  {
    return values_[index];
  }

  size_t to_hash(const std::vector<int>& idx) const {
    size_t ans = 0;
    for (const int &i : idx) {
      ans ^= values_[i]->to_hash();
    }

    return ans;
  }

private:
  std::vector<std::shared_ptr<TupleValue>> values_;
  std::vector<std::string> attr_name_;
};

class TupleField
{
public:
  TupleField(AttrType type, const char *table_name, const char *field_name, bool nullable = false) : type_(type), table_name_(table_name), field_name_(field_name), nullable_(nullable)
  {
  }

  AttrType type() const
  {
    return type_;
  }

  const char *table_name() const
  {
    return table_name_.c_str();
  }

  const char *field_name() const
  {
    return field_name_.c_str();
  }

  bool is_nullable() const
  {
    return nullable_;
  }

  std::string to_string() const;

private:
  AttrType type_;
  std::string table_name_;
  std::string field_name_;
  bool nullable_;
};

class TupleSchema
{
public:
  TupleSchema() = default;
  ~TupleSchema() = default;

  void add(AttrType type, const char *table_name, const char *field_name, bool nullable = false);
  void add_if_not_exists(AttrType type, const char *table_name, const char *field_name, bool nullable = false);
  // void merge(const TupleSchema &other);
  void append(const TupleSchema &other);

  void append(const TupleField &other) {
    add(other.type(), other.table_name(), other.field_name(), other.is_nullable());
  }

  void remove(const TupleSchema &other)
  {
        while (!fields_.empty() && (0 == strcmp(fields_.back().table_name(), other.field(0).table_name()))) {
            fields_.pop_back();
        }
  }

  const std::vector<TupleField> &fields() const
  {
    return fields_;
  }

  const TupleField &field(int index) const
  {
    return fields_[index];
  }

  int index_of_field(const char *table_name, const char *field_name) const;
  void clear()
  {
    fields_.clear();
  }

  void print(std::ostream &os, bool is_multi_table = false, bool is_exp = false) const;

  int index_of_field(const char *field_name) const;

  bool empty() const
  {
    return fields_.empty();
  }

  int size() const {
    return fields_.size();
  }

public:
  static void from_table(const Table *table, TupleSchema &schema);

private:
  std::vector<TupleField> fields_;
};

class TupleSet
{
public:
  TupleSet() = default;
  TupleSet(TupleSet &&other);
  explicit TupleSet(const TupleSchema &schema) : schema_(schema)
  {
  }
  TupleSet &operator=(TupleSet &&other);

  ~TupleSet() = default;

  void set_schema(const TupleSchema &schema);

  const TupleSchema &get_schema() const;

  void add(Tuple &&tuple);

  void clear();

  bool is_empty() const;
  int size() const;
  const Tuple &get(int index) const;
  const std::vector<Tuple> &tuples() const;

  void print(std::ostream &os, bool is_multi_table = false, bool is_exp = false) const;

  void swap_tuple(int i, int j)
  {
    std::swap(tuples_[i], tuples_[j]);
  }

  void merge(const Tuple &lhs, int i) {
    tuples_[i].merge(lhs);
  }

  void copy_ith_to(TupleSet &lhs, int i) const;

  void clear_tuples() {
    tuples_.clear();
  }

public:
  const TupleSchema &schema() const
  {
    return schema_;
  }

private:
  std::vector<Tuple> tuples_;
  TupleSchema schema_;
};

class TupleRecordConverter
{
public:
  TupleRecordConverter(Table *table, TupleSet &tuple_set);

  void add_record(const char *record);

private:
  Table *table_;
  TupleSet &tuple_set_;
};

#endif //__OBSERVER_SQL_EXECUTOR_TUPLE_H_
