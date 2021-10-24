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

#ifndef __OBSERVER_STORAGE_COMMON_FIELD_META_H__
#define __OBSERVER_STORAGE_COMMON_FIELD_META_H__

#include <string>

#include "rc.h"
#include "sql/parser/parse_defs.h"

namespace Json {
class Value;
} // namespace Json

class FieldMeta {
public:
  FieldMeta();
  ~FieldMeta() = default;
  /**
   * @brief 初始化字段
   * 
   * @param name 字段名
   * @param attr_type 字段类型
   * @param attr_offset 无视，不考虑字节对齐问题，默认4字节对齐
   * @param attr_len 字段长度
   * @param visible 字段是否可见
   * @return RC 
   */
  RC init(const char *name, AttrType attr_type, int attr_offset, int attr_len, bool visible, bool nullable);

public:
  const char *name() const;
  AttrType    type() const;
  int         offset() const;
  int         len() const;
  bool        visible() const;
  bool        nullable() const;

public:
  void desc(std::ostream &os) const;
public:
  void to_json(Json::Value &json_value) const;
  static RC from_json(const Json::Value &json_value, FieldMeta &field);

private:
  std::string  name_;
  AttrType     attr_type_;
  int          attr_offset_;
  int          attr_len_;
  bool         visible_;
  bool         nullable_; // 默认为false
};
#endif // __OBSERVER_STORAGE_COMMON_FIELD_META_H__