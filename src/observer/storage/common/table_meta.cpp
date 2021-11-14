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
// Created by Wangyunlai on 2021/5/12.
//

#include <algorithm>

#include "storage/common/table_meta.h"
#include "json/json.h"
#include "common/log/log.h"
#include "storage/trx/trx.h"

static const Json::StaticString FIELD_TABLE_NAME("table_name");
static const Json::StaticString FIELD_FIELDS("fields");
static const Json::StaticString FIELD_INDEXES("indexes");

std::vector<FieldMeta> TableMeta::sys_fields_;

TableMeta::TableMeta(const TableMeta &other) : name_(other.name_),
                                               fields_(other.fields_),
                                               indexes_(other.indexes_),
                                               record_size_(other.record_size_)
{
}

void TableMeta::swap(TableMeta &other) noexcept
{
  name_.swap(other.name_);
  fields_.swap(other.fields_);
  indexes_.swap(other.indexes_);
  std::swap(record_size_, other.record_size_);
}

RC TableMeta::init_sys_fields()
{
  sys_fields_.reserve(1);
  FieldMeta field_meta;
  RC rc = field_meta.init(Trx::trx_field_name(), Trx::trx_field_type(), 0, Trx::trx_field_len(), false, false);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to init trx field. rc = %d:%s", rc, strrc(rc));
    return rc;
  }

  sys_fields_.push_back(field_meta);
  return rc;
}
RC TableMeta::init(const char *name, int field_num, const AttrInfo attributes[])
{
  // 检查表名参数
  if (nullptr == name || '\0' == name[0])
  {
    LOG_ERROR("Name cannot be empty");
    return RC::INVALID_ARGUMENT;
  }

  // 检查属性参数
  if (field_num <= 0 || nullptr == attributes)
  {
    LOG_ERROR("Invalid argument. field_num=%d, attributes=%p", field_num, attributes);
    return RC::INVALID_ARGUMENT;
  }

  RC rc = RC::SUCCESS;
  if (sys_fields_.empty())
  {
    rc = init_sys_fields();
    if (rc != RC::SUCCESS)
    {
      LOG_ERROR("Failed to init_sys_fields, name:%s ", name);
      return rc;
    }
  }

  fields_.resize(field_num + sys_fields_.size());
  for (size_t i = 0; i < sys_fields_.size(); i++)
  {
    fields_[i] = sys_fields_[i];
  }

  int field_offset = sys_fields_.back().offset() + sys_fields_.back().len(); // 当前实现下，所有类型都是4字节对齐的，所以不再考虑字节对齐问题

  for (int i = 0; i < field_num; i++)
  {
    const AttrInfo &attr_info = attributes[i];
    rc = fields_[i + sys_fields_.size()].init(attr_info.name, attr_info.type, field_offset, attr_info.length, true, attr_info.is_nullable == 1);
    if (rc != RC::SUCCESS)
    {
      LOG_ERROR("Failed to init field meta. table name=%s, field name: %s", name, attr_info.name);
      return rc;
    }

    field_offset += attr_info.length;
  }

  record_size_ = field_offset;

  name_ = name;
  LOG_INFO("Init table meta success. table name=%s", name);
  return RC::SUCCESS;
}

RC TableMeta::add_index(const IndexMeta &index)
{
  indexes_.push_back(index);
  return RC::SUCCESS;
}

const char *TableMeta::name() const
{
  return name_.c_str();
}

const FieldMeta *TableMeta::trx_field() const
{
  return &fields_[0];
}

const FieldMeta *TableMeta::field(int index) const
{
  return &fields_[index];
}
const FieldMeta *TableMeta::field(const char *name) const
{
  if (nullptr == name)
  {
    return nullptr;
  }

  for (const FieldMeta &field : fields_)
  {
    if (0 == strcmp(field.name(), name))
    {
      return &field;
    }
  }
  return nullptr;
}

int TableMeta::find_field_index_by_name(const char *name) const {
  if (nullptr == name) {
    return -1;
  }

  for (int i = 0; i < fields_.size(); ++i) {
    if (0 == strcmp(fields_[i].name(), name)) {
      return i;
    }
  }

  return -1;
}

const FieldMeta * TableMeta::find_field_by_offset(int offset) const {
  for (const FieldMeta &field : fields_) {
    if (field.offset() == offset) {
      return &field;
    }
  }
  return nullptr;
}
int TableMeta::field_num() const
{
  return fields_.size();
}

int TableMeta::sys_field_num() const
{
  return sys_fields_.size();
}

const IndexMeta *TableMeta::index(const char *name) const
{
  for (const IndexMeta &index : indexes_)
  {
    if (0 == strcmp(index.name(), name))
    {
      return &index;
    }
  }
  return nullptr;
}

// for single index  当然这里有可能匹配到multi_index, 即当multi_index的第一个field与给定参数相同时
const IndexMeta *TableMeta::find_single_index_by_field(const char *field) const
{
  for (const IndexMeta &index : indexes_)
  {
    if (0 == strcmp(index.field(0), field))
    {
      return &index;
    }
  }
  return nullptr;
}
bool TableMeta::find_multi_index_by_fields_for_check(const char *field_names[], int field_num) const
{
  // 严格按照num是否相等来匹配
  for (const IndexMeta &index : indexes_)
  { 
    if(index.field_num()!=field_num){
      continue;
    }
    for(int i = 0; i < field_num; i++){
      if (0 != strcmp(index.field(i), field_names[i])){
          break;
      }else{
        if(i == field_num-1){
            return true;
        }
      }
    }
  }
  return false;
}
// initially for multi index but now can hold single index
// 查找index，不必考虑需要比较多少个attr。已经按照index的顺序进行检查field_names中是否存在对应的field，
// 对于multi-index已经是最左匹配，根据匹配到的index的field数量筛选最适合的匹配，如果是只有一个field_name（a），
// 可能会匹配到multi-index(a,b,c)和single-index(a)，这时multi-index也可以当做single-index使用。
// 只用于condition中attr匹配到最合适的index.
const IndexMeta *TableMeta::find_multi_index_by_fields(const char *field_names[], int field_num, int &match_num) const
{
  const IndexMeta * best_index_meta = nullptr;
  int best_match=0;   // 根据长度记录最佳匹配 
  // 设计一个hash表，方便后续循环查表
  std::unordered_map<std::string, bool> field_map;
  for (int i = 0; i < field_num; i++){
    field_map[field_names[i]] = true;
  }
  for (const IndexMeta &index : indexes_)
  { 
    int min_size;
    if(index.field_num()<=field_num){
      min_size = index.field_num();
    }else{
      min_size = field_num;
    }
    int match_num_tmp = 0;
    // 按照index中field的顺序进行查找的，符合最左匹配原则。
    for(int i = 0; i < min_size; i++){
      if (field_map[index.field(i)]){
        match_num_tmp +=1;
      }else{
        break;
      }
    }
    if(match_num_tmp>best_match){
      best_index_meta = &index;
      best_match = match_num_tmp;
    }
  }
  //best_index_meta->set_match_num(best_match);
  match_num = best_match;
  return best_index_meta;
}
const IndexMeta *TableMeta::index(int i) const
{
  return &indexes_[i];
}

int TableMeta::index_num() const
{
  return indexes_.size();
}

int TableMeta::record_size() const
{
  return record_size_;
}

int TableMeta::serialize(std::ostream &ss) const
{

  Json::Value table_value;
  table_value[FIELD_TABLE_NAME] = name_;

  Json::Value fields_value;
  for (const FieldMeta &field : fields_)
  {
    Json::Value field_value;
    field.to_json(field_value);
    fields_value.append(std::move(field_value));
  }

  table_value[FIELD_FIELDS] = std::move(fields_value);

  Json::Value indexes_value;
  for (const auto &index : indexes_)
  {
    // for(const auto &item : index.fields)
    Json::Value index_value;
    index.to_json(index_value);
    indexes_value.append(std::move(index_value));
  }
  table_value[FIELD_INDEXES] = std::move(indexes_value);

  Json::StreamWriterBuilder builder;
  Json::StreamWriter *writer = builder.newStreamWriter();

  std::streampos old_pos = ss.tellp();
  writer->write(table_value, &ss);
  int ret = (int)(ss.tellp() - old_pos);

  delete writer;
  return ret;
}

int TableMeta::deserialize(std::istream &is)
{
  if (sys_fields_.empty())
  {
    init_sys_fields();
  }

  Json::Value table_value;
  Json::CharReaderBuilder builder;
  std::string errors;

  std::streampos old_pos = is.tellg();
  if (!Json::parseFromStream(builder, is, &table_value, &errors))
  {
    LOG_ERROR("Failed to deserialize table meta. error=%s", errors.c_str());
    return -1;
  }

  const Json::Value &table_name_value = table_value[FIELD_TABLE_NAME];
  if (!table_name_value.isString())
  {
    LOG_ERROR("Invalid table name. json value=%s", table_name_value.toStyledString().c_str());
    return -1;
  }

  std::string table_name = table_name_value.asString();

  const Json::Value &fields_value = table_value[FIELD_FIELDS];
  if (!fields_value.isArray() || fields_value.size() <= 0)
  {
    LOG_ERROR("Invalid table meta. fields is not array, json value=%s", fields_value.toStyledString().c_str());
    return -1;
  }

  RC rc = RC::SUCCESS;
  int field_num = fields_value.size();
  std::vector<FieldMeta> fields(field_num);
  for (int i = 0; i < field_num; i++)
  {
    FieldMeta &field = fields[i];

    const Json::Value &field_value = fields_value[i];
    rc = FieldMeta::from_json(field_value, field);
    if (rc != RC::SUCCESS)
    {
      LOG_ERROR("Failed to deserialize table meta. table name =%s", table_name.c_str());
      return -1;
    }
  }

  std::sort(fields.begin(), fields.end(),
            [](const FieldMeta &f1, const FieldMeta &f2)
            { return f1.offset() < f2.offset(); });

  name_.swap(table_name);
  fields_.swap(fields);
  record_size_ = fields_.back().offset() + fields_.back().len();

  const Json::Value &indexes_value = table_value[FIELD_INDEXES];
  if (!indexes_value.empty())
  {
    if (!indexes_value.isArray())
    {
      LOG_ERROR("Invalid table meta. indexes is not array, json value=%s", fields_value.toStyledString().c_str());
      return -1;
    }
    const int index_num = indexes_value.size();
    std::vector<IndexMeta> indexes(index_num);
    for (int i = 0; i < index_num; i++)
    {
      IndexMeta &index = indexes[i];

      const Json::Value &index_value = indexes_value[i];
      rc = IndexMeta::from_json(*this, index_value, index);
      if (rc != RC::SUCCESS)
      {
        LOG_ERROR("Failed to deserialize table meta. table name=%s", table_name.c_str());
        return -1;
      }
    }
    indexes_.swap(indexes);
  }

  return (int)(is.tellg() - old_pos);
}

int TableMeta::get_serial_size() const
{
  return -1;
}

void TableMeta::to_string(std::string &output) const
{
}

void TableMeta::desc(std::ostream &os) const
{
  os << name_ << '(' << std::endl;
  for (const auto &field : fields_)
  {
    os << '\t';
    field.desc(os);
    os << std::endl;
  }

  for (const auto &index : indexes_)
  {
    os << '\t';
    index.desc(os);
    os << std::endl;
  }
  os << ')' << std::endl;
}
