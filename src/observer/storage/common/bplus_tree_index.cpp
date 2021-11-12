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

#include "storage/common/bplus_tree_index.h"
#include "common/log/log.h"

BplusTreeIndex::~BplusTreeIndex() noexcept
{
  close();
}
// 重载函数
RC BplusTreeIndex::create(const char *file_name, const IndexMeta &index_meta, const FieldMeta &field_meta, int is_unique){
  if (inited_) {
    LOG_INFO("BplusTreeIndex::create - RC::RECORD_OPENNED");
    return RC::RECORD_OPENNED;
  }
  RC rc = Index::init(index_meta, field_meta);
  if (rc != RC::SUCCESS)
  {
    return rc;
  }

  rc = index_handler_.create(file_name, field_meta.type(), field_meta.len(),is_unique);
  if (RC::SUCCESS == rc) {
    inited_ = true;
  }
  return rc;
}
RC BplusTreeIndex::create(const char *file_name, const IndexMeta &index_meta, const FieldMeta *field_meta[], int field_num, int is_unique) 
{
  if (inited_) {
    LOG_INFO("BplusTreeIndex::create - RC::RECORD_OPENNED");
    return RC::RECORD_OPENNED;
  }
  AttrType attr_type[field_num];
  int attr_length[field_num]; 
  for(int i = 0; i < field_num; i++){
    attr_type[i] = field_meta[i]->type();
    attr_length[i] = field_meta[i]->len();
  }
  RC rc = Index::init(index_meta, field_meta, field_num);
  if (rc != RC::SUCCESS)
  {
    return rc;
  }
       // const char *file_name, AttrType attr_type[], int attr_length[],int field_num, int is_unique
  rc = index_handler_.create(file_name, attr_type, attr_length, field_num, is_unique);
  if (RC::SUCCESS == rc) {
    inited_ = true;
  }
  return rc;
}
RC BplusTreeIndex::open(const char *file_name, const IndexMeta &index_meta, const FieldMeta &field_meta)
{
  if (inited_)
  {
    return RC::RECORD_OPENNED;
  }
  RC rc = Index::init(index_meta, field_meta);
  if (rc != RC::SUCCESS)
  {
    return rc;
  }

  rc = index_handler_.open(file_name);
  if (RC::SUCCESS == rc)
  {
    inited_ = true;
  }
  return rc;
}

RC BplusTreeIndex::open(const char *file_name, const IndexMeta &index_meta, const FieldMeta *field_meta[], int field_num)
{
  if (inited_)
  {
    return RC::RECORD_OPENNED;
  }
  // RC rc = Index::init(index_meta, field_meta);
  RC rc = Index::init(index_meta, field_meta, field_num);
  if (rc != RC::SUCCESS)
  {
    return rc;
  }

  rc = index_handler_.open(file_name);
  if (RC::SUCCESS == rc)
  {
    inited_ = true;
  }
  return rc;
}

RC BplusTreeIndex::close()
{
  if (inited_)
  {
    index_handler_.close();
    inited_ = false;
  }
  return RC::SUCCESS;
}
int BplusTreeIndex::Get_Field_Num() const
{ 
  return field_num_;
}
int BplusTreeIndex::Get_Key_length() const
{
  return index_handler_.get_key_total_length();
}
RC BplusTreeIndex::insert_entry(const char *record, const RID *rid)
{
  // 这里的field_meta_为index类中的内容
  // 根据offset确定初始位置 然后根据attr_length确定取内容的长度 这里存在顺序的问题
  // 在这里即可构造Key
  LOG_INFO("调用bplustree index中的insert_entry");
  int total_length = Get_Key_length();
  char *key = (char *)malloc(total_length);
  int acc_len=0;
  for(int i=0;i<field_num_;i++){
    memcpy(key + acc_len,record + fields_meta_[i].offset() , fields_meta_[i].len());
    acc_len += fields_meta_[i].len();
  }
  memcpy(key + acc_len, rid, sizeof(*rid));
  return index_handler_.insert_entry(key, rid);
}

RC BplusTreeIndex::delete_entry(const char *record, const RID *rid)
{
  //组合 key
  
  char *pdata = (char *)malloc(index_handler_.get_key_total_length());
  int offset = 0;
  for ( int i = 0; i < fields_meta_.size(); i++){
    memcpy(pdata + offset,record + fields_meta_[i].offset(),fields_meta_[i].len());
    offset += fields_meta_[i].len();
  }
  return index_handler_.delete_entry(pdata, rid);
}
// 这里和multi_index的差别主要是将null值分开
IndexScanner *BplusTreeIndex::create_single_index_scanner(CompOp comp_op, const char *value, int null_field_index)
{
  
  BplusTreeScanner *bplus_tree_scanner = new BplusTreeScanner(index_handler_,1);
  RC rc = bplus_tree_scanner->open_single_index(comp_op, value, null_field_index);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to open index scanner. rc=%d:%s", rc, strrc(rc));
    delete bplus_tree_scanner;
    return nullptr;
  }

  BplusTreeIndexScanner *index_scanner = new BplusTreeIndexScanner(bplus_tree_scanner);
  return index_scanner;
}

IndexScanner *BplusTreeIndex::create_multi_index_scanner(const std::vector<CompOp> &comp_ops, const std::vector<const char *> &values, int &match_num)
{
  // 当前的BplusTreeIndex就是已经匹配上当前condition的index.
  BplusTreeScanner *bplus_tree_scanner = new BplusTreeScanner(index_handler_,match_num);
  RC rc = bplus_tree_scanner->open_multi_index(comp_ops, values);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to open index scanner. rc=%d:%s", rc, strrc(rc));
    delete bplus_tree_scanner;
    return nullptr;
  }

  BplusTreeIndexScanner *index_scanner = new BplusTreeIndexScanner(bplus_tree_scanner);
  return index_scanner;
}

RC BplusTreeIndex::sync()
{
  return index_handler_.sync();
}
BplusTreeIndexScanner::BplusTreeIndexScanner(BplusTreeScanner *tree_scanner) : tree_scanner_(tree_scanner)
{
}

BplusTreeIndexScanner::~BplusTreeIndexScanner() noexcept
{
  tree_scanner_->close();
  delete tree_scanner_;
}

RC BplusTreeIndexScanner::next_entry(RID *rid)
{
  return tree_scanner_->next_entry(rid);
}

RC BplusTreeIndexScanner::destroy()
{
  delete this;
  return RC::SUCCESS;
}