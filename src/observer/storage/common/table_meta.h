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

#ifndef __OBSERVER_STORAGE_COMMON_TABLE_META_H__
#define __OBSERVER_STORAGE_COMMON_TABLE_META_H__

#include <string>
#include <vector>

#include "rc.h"
#include "storage/common/field_meta.h"
#include "storage/common/index_meta.h"
#include "common/lang/serializable.h"

class TableMeta : public common::Serializable {
public:
  TableMeta() = default;
  ~TableMeta() = default;

  TableMeta(const TableMeta &other);

  void swap(TableMeta &other) noexcept;

  /**
   * @brief 初始化表
   * 
   * @param name 表名
   * @param field_num 字段个数 
   * @param attributes 字段
   * @return RC 
   */
  RC init(const char *name, int field_num, const AttrInfo attributes[]);

  RC add_index(const IndexMeta &index);

public:
  const char * name() const;
  const FieldMeta * trx_field() const;
  const FieldMeta * field(int index) const;
  const FieldMeta * field(const char *name) const;
  const FieldMeta * find_field_by_offset(int offset) const;
  int field_num() const;
  int sys_field_num() const;
  int find_field_index_by_name(const char *name) const;

  const IndexMeta * index(const char *name) const;
  //const IndexMeta * find_index_by_field(const char *field) const;
  bool find_multi_index_by_fields_for_check(const char *field_names[], int field_num) const;
  const IndexMeta * find_multi_index_by_fields(const char *field_names[], int field_num, int &match_num) const;
  const IndexMeta * find_single_index_by_field(const char *field) const;
  const IndexMeta * index(int i) const;
  const std::vector<IndexMeta> get_indexes() const{
    return indexes_;
  }
  int index_num() const;

  int record_size() const;

public:
  int  serialize(std::ostream &os) const override;
  int  deserialize(std::istream &is) override;
  int  get_serial_size() const override;
  void to_string(std::string &output) const override;
  void desc(std::ostream &os) const;

private:
  static RC init_sys_fields();
private:
  std::string   name_;
  std::vector<FieldMeta>  fields_; // 包含sys_fields，存储解析后的字段
  std::vector<IndexMeta>  indexes_;

  int  record_size_ = 0;

  static std::vector<FieldMeta> sys_fields_;
};

#endif // __OBSERVER_STORAGE_COMMON_TABLE_META_H__