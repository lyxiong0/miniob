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
// Created by wangyunlai.wyl on 2021/5/19.
//

#include "storage/common/index.h"

RC Index::init(const IndexMeta &index_meta, const FieldMeta &field_meta) {
  field_num_ = 1;
  index_meta_ = index_meta;
  fields_meta_.push_back(field_meta);
  return RC::SUCCESS;
}
RC Index::init(const IndexMeta &index_meta, const FieldMeta *field_meta[],int field_num){
  index_meta_ = index_meta;
  field_num_ = field_num;
  for(int i = 0; i < field_num; i++){
    fields_meta_.push_back(*field_meta[i]);
  }
  return RC::SUCCESS;
}
/*
int Index::Get_Field_Num() const
{
  return field_num_;
}
*/