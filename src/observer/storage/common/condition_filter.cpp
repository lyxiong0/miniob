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
// Created by Wangyunlai on 2021/5/7.
//

#include <stddef.h>
#include "condition_filter.h"
#include "record_manager.h"
#include "common/log/log.h"
#include "storage/common/table.h"

using namespace common;

ConditionFilter::~ConditionFilter()
{
}

DefaultConditionFilter::DefaultConditionFilter()
{
  left_.is_attr = false;
  left_.attr_length = 0;
  left_.attr_offset = 0;
  left_.value = nullptr;

  right_.is_attr = false;
  right_.attr_length = 0;
  right_.attr_offset = 0;
  right_.value = nullptr;
}
DefaultConditionFilter::~DefaultConditionFilter()
{
}

RC DefaultConditionFilter::init(const ConDesc &left, const ConDesc &right, AttrType attr_type, CompOp comp_op, AttrType another_attr_type)
{
  if (attr_type < CHARS || attr_type > NULLS)
  {
    LOG_ERROR("Invalid condition with unsupported attribute type: %d", attr_type);
    return RC::INVALID_ARGUMENT;
  }

  if (comp_op < EQUAL_TO || comp_op >= NO_OP)
  {
    LOG_ERROR("Invalid condition with unsupported compare operation: %d", comp_op);
    return RC::INVALID_ARGUMENT;
  }

  left_ = left;
  right_ = right;
  attr_type_ = attr_type;
  comp_op_ = comp_op;
  another_attr_type_ = another_attr_type;
  // LOG_INFO("default condition filter init 完成 comp_op = %d", comp_op_);
  return RC::SUCCESS;
}

RC DefaultConditionFilter::init(Table &table, const Condition &condition)
{
  const TableMeta &table_meta = table.table_meta();
  ConDesc left;
  ConDesc right;

  AttrType type_left = UNDEFINED;
  AttrType type_right = UNDEFINED;

  if (1 == condition.left_is_attr)
  {
    left.is_attr = true;
    int i = table_meta.find_field_index_by_name(condition.left_attr.attribute_name);
    if (-1 == i)
    {
      LOG_WARN("No such field in condition. %s.%s", table.name(), condition.left_attr.attribute_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
    const FieldMeta *field_left = table_meta.field(i);
    // const FieldMeta *field_left = table_meta.field(condition.left_attr.attribute_name);
    // 这里检查了where子句中的列名是否存在
    left.attr_length = field_left->len();
    left.attr_offset = field_left->offset();

    left.value = nullptr;

    type_left = field_left->type();

    auto last_field = table_meta.field(table_meta.field_num() - 1);
    left.null_field_index = last_field->offset() + last_field->len() + i - 1;
  }
  else
  {
    left.is_attr = false;
    left.value = condition.left_value.data; // 校验type 或者转换类型
    type_left = condition.left_value.type;

    left.attr_length = 0;
    left.attr_offset = 0;
  }

  if (1 == condition.right_is_attr)
  {
    right.is_attr = true;
    int i = table_meta.find_field_index_by_name(condition.right_attr.attribute_name);
    if (-1 == i)
    {
      LOG_WARN("No such field in condition. %s.%s", table.name(), condition.right_attr.attribute_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
    const FieldMeta *field_right = table_meta.field(i);
    // const FieldMeta *field_right = table_meta.field(condition.right_attr.attribute_name);
    // 这里检查了where子句中的列名是否存在
    right.attr_length = field_right->len();
    right.attr_offset = field_right->offset();
    type_right = field_right->type();

    right.value = nullptr;

    auto last_field = table_meta.field(table_meta.field_num() - 1);
    right.null_field_index = last_field->offset() + last_field->len() + i - 1;
  }
  else
  {
    right.is_attr = false;
    right.value = condition.right_value.data;
    type_right = condition.right_value.type;

    right.attr_length = 0;
    right.attr_offset = 0;
  }

  // 校验和转换
  if (condition.comp == CompOp::IS_NULL || condition.comp == CompOp::IS_NOT_NULL)
  {
    return init(left, right, type_left, condition.comp, type_right);
  }
  //  if (!field_type_compare_compatible_table[type_left][type_right]) {
  //    // 不能比较的两个字段， 要把信息传给客户端
  //    return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  //  }
  // NOTE：这里没有实现不同类型的数据比较，比如整数跟浮点数之间的对比
  // 但是选手们还是要实现。这个功能在预选赛中会出现
  // CompOp cmp_op = condition.comp;
  // LOG_INFO("condition init中，type_left = %d, type_right = %d",type_left,type_right);
  if (type_left != type_right && type_left != AttrType::NULLS && type_right != AttrType::NULLS)
  {
    // TODO: 不知道咋实现int和float比较
    // if (type_left == AttrType::INTS && type_right == AttrType::FLOATS && condition.right_is_attr == 0)
    // {
    //   支持(int)age > 1.1
    //   type_right = AttrType::INTS;
    //   float ori_v = *(float *)condition.right_value.data;
    //   int v = (int)ori_v;
    //   if (v * 1.0 != ori_v) {
    //     ++v;
    //     if (cmp_op == CompOp::GREAT_THAN) {
    //       // >1.1 -> >= 2
    //       cmp_op = CompOp::GREAT_THAN;
    //     } else if (cmp_op == CompOp::LESS_EQUAL) {
    //       // <=1.1 -> <2
    //       cmp_op = CompOp::LESS_THAN;
    //     }
    //     //其余情况：>=1.1 -> >= 2
    //     // <1.1 -> <2
    //   }
    //   delete right.value;
    //   right.value = malloc(sizeof(v));
    //   memcpy(right.value, &v, sizeof(v));
    // }
    // else if (type_left == AttrType::FLOATS && type_right == AttrType::INTS && condition.right_is_attr == 0)
    // {
    //   // 支持(float)score > 60;
    //   type_right = AttrType::FLOATS;
    //   int ori_v = *(int *)condition.right_value.data;
    //   float v = ori_v * 1.0;
    //   LOG_INFO("v = %f", v);
    //   delete right.value;
    //   right.value = malloc(sizeof(v));
    //   memcpy(right.value, &v, sizeof(v));
    //   LOG_INFO("right.value = %f", *(float *)right.value);
    // }
    // else
    // {
    return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    // }
  }

  return init(left, right, type_left, condition.comp, type_right);
}

bool DefaultConditionFilter::filter(const Record &rec) const
{
  // 根据record的type来判断比较
  AttrType main_type = UNDEFINED;
  if (attr_type_ == AttrType::NULLS || another_attr_type_ == AttrType::NULLS)
  {
    // 两侧只要有一侧的类型为null，则传递null类型
    main_type = NULLS;
  } else {
    main_type = attr_type_;
  }
    
  // 对于null的一般运算符，全部返回错误
  if (main_type == AttrType::NULLS && comp_op_ != IS_NULL && comp_op_ != IS_NOT_NULL)
  {
    return false;
  }

  bool left_is_value = false;
  char *left_value = nullptr;
  char *right_value = nullptr;

  if (left_.is_attr)
  { // value
    left_value = (char *)(rec.data + left_.attr_offset);
  }
  else
  {
    left_value = (char *)left_.value;
    left_is_value = true;
  }

  if (right_.is_attr)
  {
    right_value = (char *)(rec.data + right_.attr_offset);
  }
  else
  {
    right_value = (char *)right_.value;
  }

  int cmp_result = 0;
  switch (main_type)
  {
  case CHARS:
  { // 字符串都是定长的，直接比较
    // 按照C字符串风格来定
    // LOG_INFO("THE left_value length = %d right_value length = %d in defaultconditionfilter",strlen(left_value),strlen(right_value));
    cmp_result = strncmp(left_value, right_value, left_.attr_length);
  }
  break;
  case INTS:
  {
    // 没有考虑大小端问题
    // 对int和float，要考虑字节对齐问题,有些平台下直接转换可能会跪
    int left = *(int *)left_value;
    int right = *(int *)right_value;
    cmp_result = left - right;
  }
  break;
  case DATES:
  {
    // 没有考虑大小端问题
    // 对int和float，要考虑字节对齐问题,有些平台下直接转换可能会跪
    int left = *(int *)left_value;
    int right = *(int *)right_value;
    cmp_result = left - right;
  }
  break;
  case FLOATS:
  {
    float left = *(float *)left_value;
    float right = *(float *)right_value;
    float result = left - right;
    if (result < 1e-6 && result > -1e-6)
    {
      cmp_result = 0;
    }
    else if (result > 0)
    {
      cmp_result = 1;
    }
    else
    {
      cmp_result = -1;
    }
    // 原来这个写法有问题，差值在1以内都会判断为相等
    // cmp_result = (int)(left - right);
  }
  break;
  default:
  {
  }
  }

  bool is_null = false;
  if (!left_is_value)
  {
    memcpy(&is_null, rec.data + left_.null_field_index, 1);
    // LOG_INFO("left_.null_field_index = %d, is_null = %d", left_.null_field_index, is_null);
  }

  switch (comp_op_)
  {
  case EQUAL_TO:
    return 0 == cmp_result && !is_null;
  case LESS_EQUAL:
    return cmp_result <= 0 && !is_null;
  case NOT_EQUAL:
    return cmp_result != 0 && !is_null;
  case LESS_THAN:
    return cmp_result < 0 && !is_null;
  case GREAT_EQUAL:
    return cmp_result >= 0 && !is_null;
  case GREAT_THAN:
    return cmp_result > 0 && !is_null;
  case IS_NULL:
  {
    if (left_is_value)
    {
      if (attr_type_ == NULLS)
      {
        // null is null
        return true;
      }
      return false;
    }
    else
    {
      return is_null;
    }
  }
  break;
  case IS_NOT_NULL:
  {
    if (left_is_value)
    {
      if (attr_type_ == NULLS)
      {
        // null is not null
        return false;
      }
      return true;
    }
    else
    {
      return !is_null;
    }
  }
  break;

  default:
    break;
  }

  LOG_PANIC("Never should print this.");
  return cmp_result; // should not go here
}

CompositeConditionFilter::~CompositeConditionFilter()
{
  if (memory_owner_)
  {
    delete[] filters_;
    filters_ = nullptr;
  }
}

RC CompositeConditionFilter::init(const ConditionFilter *filters[], int filter_num, bool own_memory)
{
  filters_ = filters;
  filter_num_ = filter_num;
  memory_owner_ = own_memory;
  return RC::SUCCESS;
}
RC CompositeConditionFilter::init(const ConditionFilter *filters[], int filter_num)
{
  return init(filters, filter_num, false);
}

RC CompositeConditionFilter::init(Table &table, const Condition *conditions, int condition_num)
{
  if (condition_num == 0)
  {
    return RC::SUCCESS;
  }
  if (conditions == nullptr)
  {
    return RC::INVALID_ARGUMENT;
  }

  RC rc = RC::SUCCESS;
  ConditionFilter **condition_filters = new ConditionFilter *[condition_num];
  for (int i = 0; i < condition_num; i++)
  {
    DefaultConditionFilter *default_condition_filter = new DefaultConditionFilter();
    rc = default_condition_filter->init(table, conditions[i]);
    if (rc != RC::SUCCESS)
    {
      delete default_condition_filter;
      for (int j = i - 1; j >= 0; j--)
      {
        delete condition_filters[j];
        condition_filters[j] = nullptr;
      }
      delete[] condition_filters;
      condition_filters = nullptr;
      return rc;
    }
    condition_filters[i] = default_condition_filter;
  }
  return init((const ConditionFilter **)condition_filters, condition_num, true);
}

bool CompositeConditionFilter::filter(const Record &rec) const
{
  for (int i = 0; i < filter_num_; i++)
  {
    if (!filters_[i]->filter(rec))
    {
      return false;
    }
  }
  return true;
}
