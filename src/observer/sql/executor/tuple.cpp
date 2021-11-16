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
#include <string>
#include <stdio.h>
#include "sql/executor/tuple.h"
#include "storage/common/table.h"
#include "common/log/log.h"

Tuple::Tuple(const Tuple &other)
{
  // LOG_PANIC("Copy constructor of tuple is not supported");
  // exit(1);
  // 提前分配空间：带智能指针的vector在自动扩容的时候可能会打乱内存分配
  values_.reserve(other.values_.size());
  for (auto const &value : other.values_) {
    values_.emplace_back(value->clone());
  }
}

Tuple::Tuple(Tuple &&other) noexcept : values_(std::move(other.values_))
{
}

Tuple &Tuple::operator=(Tuple &&other) noexcept
{
  if (&other == this)
  {
    return *this;
  }

  values_.clear();
  values_.swap(other.values_);
  return *this;
}

// add (Value && value)
void Tuple::add(TupleValue *value)
{
  values_.emplace_back(value);
}
void Tuple::add(const std::shared_ptr<TupleValue> &other)
{
  values_.emplace_back(other);
}
void Tuple::add(int value, bool is_null)
{
  add(new IntValue(value, is_null));
}

void Tuple::add(float value, bool is_null)
{
  add(new FloatValue(value, is_null));
}

void Tuple::add(const char *s, int len, bool is_null)
{
  add(new StringValue(s, len, is_null));
}

////////////////////////////////////////////////////////////////////////////////

std::string TupleField::to_string() const
{
  return std::string(table_name_) + "." + field_name_ + std::to_string(type_);
}

////////////////////////////////////////////////////////////////////////////////
void TupleSchema::from_table(const Table *table, TupleSchema &schema)
{
  const char *table_name = table->name();
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = 0; i < field_num; i++)
  {
    const FieldMeta *field_meta = table_meta.field(i);
    if (field_meta->visible())
    {
      schema.add(field_meta->type(), table_name, field_meta->name(), field_meta->nullable());
    }
  }
}

void TupleSchema::add(AttrType type, const char *table_name, const char *field_name, bool nullable)
{
  fields_.emplace_back(type, table_name, field_name, nullable);
}

void TupleSchema::add_if_not_exists(AttrType type, const char *table_name, const char *field_name, bool nullable)
{
  for (const auto &field : fields_)
  {
    if (0 == strcmp(field.table_name(), table_name) &&
        0 == strcmp(field.field_name(), field_name))
    {
      return;
    }
  }
  add(type, table_name, field_name, nullable);
}

void TupleSchema::append(const TupleSchema &other)
{
  fields_.reserve(fields_.size() + other.fields_.size());
  for (const auto &field : other.fields_)
  {
    fields_.emplace_back(field);
  }
}

int TupleSchema::index_of_field(const char *table_name, const char *field_name) const
{
  const int size = fields_.size();
  for (int i = 0; i < size; i++)
  {
    const TupleField &field = fields_[i];
    // LOG_ERROR("field.attr_name = %s", field.field_name());
    // LOG_ERROR("field.table_name = %s", field.table_name());
    if (0 == strcmp(field.table_name(), table_name) && 0 == strcmp(field.field_name(), field_name))
    {
      return i;
    }
  }
  return -1;
}

int TupleSchema::index_of_field(const char *field_name) const
{
  // 重载一个table_name = nullptr的版本，不改之前的防止原始代码出问题
  const int size = fields_.size();
  for (int i = 0; i < size; i++)
  {
    const TupleField &field = fields_[i];
    // LOG_ERROR("field.attr_name = %s", field.field_name());
    // LOG_ERROR("field.table_name = %s", field.table_name());
    if (0 == strcmp(field.field_name(), field_name))
    {
      return i;
    }
  }
  return -1;
}

void TupleSchema::print(std::ostream &os, bool is_multi_table, bool is_exp) const
{
  if (fields_.empty())
  {
    os << "No schema";
    return;
  }

  // 判断有多张表还是只有一张表
  std::set<std::string> table_names;
  for (const auto &field : fields_)
  {
    table_names.insert(field.table_name());
  }

  for (std::vector<TupleField>::const_iterator iter = fields_.begin(), end = --fields_.end();
       iter != end; ++iter)
  {
    if ((table_names.size() > 1 || is_multi_table == true) && !is_exp)
    {
      os << iter->table_name() << ".";
    }
    os << iter->field_name() << " | ";
  }

  if ((table_names.size() > 1 || is_multi_table == true) && !is_exp)
  {
    os << fields_.back().table_name() << ".";
  }
  os << fields_.back().field_name() << std::endl;
}

/////////////////////////////////////////////////////////////////////////////
TupleSet::TupleSet(TupleSet &&other) : tuples_(std::move(other.tuples_)), schema_(other.schema_)
{
  other.schema_.clear();
}

TupleSet &TupleSet::operator=(TupleSet &&other)
{
  if (this == &other)
  {
    return *this;
  }

  schema_.clear();
  schema_.append(other.schema_);
  other.schema_.clear();

  tuples_.clear();
  tuples_.swap(other.tuples_);
  return *this;
}

void TupleSet::add(Tuple &&tuple)
{
  tuples_.emplace_back(std::move(tuple));
}

void TupleSet::clear()
{
  tuples_.clear();
  schema_.clear();
}

void TupleSet::print(std::ostream &os, bool is_multi_table, bool is_exp) const
{
  if (schema_.fields().empty())
  {
    LOG_WARN("Got empty schema");
    return;
  }

  schema_.print(os, is_multi_table, is_exp);

  for (const Tuple &item : tuples_)
  {
    const std::vector<std::shared_ptr<TupleValue>> &values = item.values();
    for (std::vector<std::shared_ptr<TupleValue>>::const_iterator iter = values.begin(), end = --values.end();
         iter != end; ++iter)
    {
      (*iter)->to_string(os);
      os << " | ";
    }
    values.back()->to_string(os);
    os << std::endl;
  }
}

void TupleSet::set_schema(const TupleSchema &schema)
{
  schema_ = schema;
}

void TupleSet::copy_ith_to(TupleSet &lhs, int i) const {
  // 将第i个值
  Tuple tmp = tuples_[i];
  lhs.add(std::move(tmp));
}

const TupleSchema &TupleSet::get_schema() const
{
  return schema_;
}

bool TupleSet::is_empty() const
{
  return tuples_.empty();
}

int TupleSet::size() const
{
  return tuples_.size();
}

const Tuple &TupleSet::get(int index) const
{
  return tuples_[index];
}

const std::vector<Tuple> &TupleSet::tuples() const
{
  return tuples_;
}

/////////////////////////////////////////////////////////////////////////////
TupleRecordConverter::TupleRecordConverter(Table *table, TupleSet &tuple_set) : table_(table), tuple_set_(tuple_set)
{
}

void num2date(int n,char *str)
{
  int len = 10;
  char str1[len];
  sprintf(str1, "%d", n);
  // char str[len];

  for (int i = 0, j = 0; i < len; i++)
  {
    if (i == 4 || i == 7)
    {
      str[i] = '-';
      i++;
    }
    str[i] = str1[j++];
  }
}

void TupleRecordConverter::add_record(const char *record)
{
  const TupleSchema &schema = tuple_set_.schema();
  Tuple tuple;
  const TableMeta &table_meta = table_->table_meta();

  auto last_field = table_meta.field(table_meta.field_num() - 1);
  int null_field_index = last_field->offset() + last_field->len();

  for (const TupleField &field : schema.fields())
  {
    int i = table_meta.find_field_index_by_name(field.field_name());
    assert(i != -1);
    const FieldMeta *field_meta = table_meta.field(i);
    // LOG_INFO("field_meta->offset(): %d", field_meta->offset());
    // 不管什么类型都有可能插入null
    bool is_null = false;
    // -1是因为field[0]为_trx
    memcpy(&is_null, record + null_field_index + i - 1, 1);
    if (is_null)
    {
      // 插入null
      const char *s = "NULL";
      tuple.add(s, 4, true);
    }
    else
    {
      switch (field_meta->type())
      {
      case INTS:
      {
        int value = *(int *)(record + field_meta->offset());
        tuple.add(value, false);
      }
      break;
      case FLOATS:
      {
        float value = *(float *)(record + field_meta->offset());
        tuple.add(value, false);
      }
      break;
      case CHARS:
      {
        // TODO: 这里不知道为什么会出现strlen(s)超过field_meta->len()的情况
        //  insert的时候已经检查过长度strlen(s)没问题
        const char *s = record + field_meta->offset(); // 现在当做Cstring来处理
        int len = field_meta->len() < strlen(s) ? field_meta->len() : strlen(s);
        // tuple.add(s, strlen(s));
        tuple.add(s, len, false);
      }
      break;
      case DATES:
      {
        int value = *(int *)(record + field_meta->offset());
        char *str = (char*)malloc(sizeof(char)*10);
        num2date(value,str);
        // const char *s = str;
        //const char *s = num2date(value);
        tuple.add(str, 10);
        free(str);
      }
      break;
      case TEXTS: {
        const char *s = record + field_meta->offset();
        const int len = 4096;
        tuple.add(s, len, false);
      }
      break;
      default:
      {
        LOG_PANIC("Unsupported field type. type=%d", field_meta->type());
      }
      }
    }
  }

  tuple_set_.add(std::move(tuple));
}
