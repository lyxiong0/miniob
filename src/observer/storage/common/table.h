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

#ifndef __OBSERVER_STORAGE_COMMON_TABLE_H__
#define __OBSERVER_STORAGE_COMMON_TABLE_H__

#include "storage/common/table_meta.h"
#include "storage/common/condition_filter.h"

#include <cstring>

class DiskBufferPool;
class RecordFileHandler;
class ConditionFilter;
class DefaultConditionFilter;
class CompositeConditionFilter;
struct Record;
struct RID;
class Index;
class IndexScanner;
class RecordDeleter;
class Trx;

class Table
{
  friend class DefaultStorageStage;

public:
  Table();
  ~Table();

  /**
   * @brief 创建一个表
   *
   * @param path 元数据保存的文件(完整路径)
   * @param name 表名
   * @param base_dir 表数据存放的路径
   * @param attribute_count 字段个数
   * @param attributes 字段
   */
  RC create(const char *path, const char *name, const char *base_dir, int attribute_count, const AttrInfo attributes[]);

  /**
   * 打开一个表
   * @param meta_file 保存表元数据的文件完整路径
   * @param base_dir 表所在的文件夹，表记录数据文件、索引数据文件存放位置
   */
  RC open(const char *meta_file, const char *base_dir);

  RC insert_record(Trx *trx, int value_num, const Value *values, Record **ret_record = nullptr);

  /**
   * @brief 该函数用于更新表中所有满足指定条件的元组，
   * 在每一个更新的元组中将属性attrName的值设置为一个新的值。
   * 如果没有指定条件，则此方法更新表中所有元组。
   * 如果要更新一个被索引的属性，应当先删除每个被更新元组对应的索引条目，然后插入一个新的索引条目
   *
   * @param trx 事务
   * @param attribute_name 属性名
   * @param value 新的值
   * @param condition_num 条件个数
   * @param conditions 条件
   * @param updated_count 更新数量
   * @return RC
   */
  RC update_record(Trx *trx, const char *attribute_name, const Value *value, int condition_num, const Condition conditions[], int *updated_count);

  RC delete_record(Trx *trx, ConditionFilter *filter, int *deleted_count);

  RC scan_record(Trx *trx, ConditionFilter *filter, int limit, void *context, void (*record_reader)(const char *data, void *context));

  RC create_index(Trx *trx, const char *index_name, const int& attr_num, const char *attribute_name[],int is_unique);

  RC create_index(Trx *trx, const char *index_name,const char *attribute_name,int is_unique);

  std::vector<const char *> get_index_names();

public:
  const char *name() const;

  const TableMeta &table_meta() const;

  RC sync();

public:
  RC commit_insert(Trx *trx, const RID &rid);
  RC commit_delete(Trx *trx, const RID &rid);
  RC commit_update(Trx *trx, const RID &rid, char *new_record_data);
  RC rollback_insert(Trx *trx, const RID &rid);
  RC rollback_delete(Trx *trx, const RID &rid);

private:
  RC scan_record(Trx *trx, ConditionFilter *filter, int limit, void *context, RC (*record_reader)(Record *record, void *context));
  RC scan_record_by_index(Trx *trx, IndexScanner *scanner, ConditionFilter *filter, int limit, void *context, RC (*record_reader)(Record *record, void *context));
  IndexScanner *find_index_for_scan(const ConditionFilter *filter);
  IndexScanner *find_single_index_for_scan(const DefaultConditionFilter &filter);
  IndexScanner *find_multi_index_for_scan(const CompositeConditionFilter &filter);
  // IndexScanner *find_index_multi_for_scan(std::vector<DefaultConditionFilter> &filters);
  // default return the longest index(multi-index) if not multi-index then return single index
  const IndexMeta *find_multi_index_by_Deaultfields(std::vector<const ConDesc *> &field_cond_descs);
  

  RC insert_record(Trx *trx, Record *record);
  RC delete_record(Trx *trx, Record *record);
  RC update_record(Trx *trx, Record *record, const char *attribute_name, const Value *value);

private:
  friend class RecordUpdater;
  friend class RecordDeleter;
  RC update_entry_of_indexes(const char *record_i, const RID &rid_i,const char *record_d, const RID &rid_d, bool error_on_not_exists);
  RC insert_entry_of_indexes(const char *record, const RID &rid);
  RC delete_entry_of_indexes(const char *record, const RID &rid, bool error_on_not_exists);

private:
  RC init_record_handler(const char *base_dir);
  RC make_record(int value_num, const Value *values, char *&record_out);

private:
  void find_index_for_update(std::vector<Index *> &index_cover, const char *attr_name) const;
  Index *find_index(const char *index_name) const;
  RC is_legal(const Value &value, const FieldMeta *field);

private:
  std::string base_dir_;
  TableMeta table_meta_;
  DiskBufferPool *data_buffer_pool_; /// 数据文件关联的buffer pool
  int file_id_;
  RecordFileHandler *record_handler_; /// 记录操作
  std::vector<Index *> indexes_;
};

#endif // __OBSERVER_STORAGE_COMMON_TABLE_H__