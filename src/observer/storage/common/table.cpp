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
// Created by Wangyunlai on 2021/5/13.
//

#include <limits.h>
#include <string.h>
#include <algorithm>

#include "storage/common/table.h"
#include "storage/common/table_meta.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "storage/default/disk_buffer_pool.h"
#include "storage/common/record_manager.h"
#include "storage/common/condition_filter.h"
#include "storage/common/meta_util.h"
#include "storage/common/index.h"
#include "storage/common/bplus_tree_index.h"
#include "storage/trx/trx.h"

Table::Table() : data_buffer_pool_(nullptr),
                 file_id_(-1),
                 record_handler_(nullptr)
{
}

Table::~Table()
{
  delete record_handler_;
  record_handler_ = nullptr;

  if (data_buffer_pool_ != nullptr && file_id_ >= 0)
  {
    data_buffer_pool_->close_file(file_id_);
    data_buffer_pool_ = nullptr;
  }

  LOG_INFO("Table has been closed: %s", name());
}

RC Table::create(const char *path, const char *name, const char *base_dir, int attribute_count, const AttrInfo attributes[])
{
  // 检查表名参数
  if (nullptr == name || common::is_blank(name))
  {
    LOG_WARN("Name cannot be empty");
    return RC::INVALID_ARGUMENT;
  }
  LOG_INFO("Begin to create table %s:%s", base_dir, name);

  // 检查属性参数
  if (attribute_count <= 0 || nullptr == attributes)
  {
    LOG_WARN("Invalid arguments. table_name=%s, attribute_count=%d, attributes=%p",
             name, attribute_count, attributes);
    return RC::INVALID_ARGUMENT;
  }

  RC rc = RC::SUCCESS;

  // 使用 table_name.table记录一个表的元数据
  // 判断表文件是否已经存在
  int fd = ::open(path, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0600);
  if (-1 == fd)
  {
    if (EEXIST == errno)
    {
      LOG_ERROR("Failed to create table file, it has been created. %s, EEXIST, %s",
                path, strerror(errno));
      return RC::SCHEMA_TABLE_EXIST;
    }
    LOG_ERROR("Create table file failed. filename=%s, errmsg=%d:%s",
              path, errno, strerror(errno));
    return RC::IOERR;
  }

  close(fd);

  // 创建文件
  if ((rc = table_meta_.init(name, attribute_count, attributes)) != RC::SUCCESS)
  {
    LOG_ERROR("Failed to init table meta. name:%s, ret:%d", name, rc);
    return rc; // delete table file
  }

  std::fstream fs;
  fs.open(path, std::ios_base::out | std::ios_base::binary);
  if (!fs.is_open())
  {
    LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", path, strerror(errno));
    return RC::IOERR;
  }

  // 记录元数据到文件中
  table_meta_.serialize(fs);
  fs.close();

  std::string data_file = std::string(base_dir) + "/" + name + TABLE_DATA_SUFFIX;
  std::cout << data_file << std::endl;
  data_buffer_pool_ = theGlobalDiskBufferPool();
  rc = data_buffer_pool_->create_file(data_file.c_str());
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to create disk buffer pool of data file. file name=%s", data_file.c_str());
    return rc;
  }

  rc = init_record_handler(base_dir);

  base_dir_ = base_dir;
  LOG_INFO("Successfully create table %s:%s", base_dir, name);
  return rc;
}

RC Table::open(const char *meta_file, const char *base_dir)
{
  // 加载元数据文件
  std::fstream fs;
  std::string meta_file_path = std::string(base_dir) + "/" + meta_file;
  fs.open(meta_file_path, std::ios_base::in | std::ios_base::binary);
  if (!fs.is_open())
  {
    LOG_ERROR("Failed to open meta file for read. file name=%s, errmsg=%s", meta_file, strerror(errno));
    return RC::IOERR;
  }
  if (table_meta_.deserialize(fs) < 0)
  {
    LOG_ERROR("Failed to deserialize table meta. file name=%s", meta_file);
    return RC::GENERIC_ERROR;
  }
  fs.close();

  // 加载数据文件
  RC rc = init_record_handler(base_dir);

  base_dir_ = base_dir;

  const int index_num = table_meta_.index_num();
  for (int i = 0; i < index_num; i++)
  {
    BplusTreeIndex *index = new BplusTreeIndex();
    const IndexMeta *index_meta = table_meta_.index(i);
    int index_fields_num = index_meta->field_num();
    if(index_fields_num>1){
      // multi-index
      const FieldMeta *field_metas[index_fields_num];
      for(int i =0;i<index_fields_num;i++){

        const FieldMeta *field_meta= table_meta_.field(index_meta->field(i));
        if (field_meta == nullptr)
        {
          LOG_PANIC("Found invalid index meta info which has a non-exists field. table=%s, index=%s, field=%s",
                name(), index_meta->name(), index_meta->field(i));
          return RC::GENERIC_ERROR;
        }
        field_metas[i] = field_meta;
      } 
      std::string index_file = index_data_file(base_dir, name(), index_meta->name());
      rc = index->open(index_file.c_str(), *index_meta, field_metas, index_fields_num);
      if (rc != RC::SUCCESS)
      {
        delete index;
        LOG_ERROR("Failed to open index. table=%s, index=%s, file=%s, rc=%d:%s",
                name(), index_meta->name(), index_file.c_str(), rc, strrc(rc));
        return rc;
      }
    }else{
      // single-index
      const FieldMeta *field_meta = table_meta_.field(index_meta->field(0));
      if (field_meta == nullptr)
      {
        LOG_PANIC("Found invalid index meta info which has a non-exists field. table=%s, index=%s, field=%s",
                name(), index_meta->name(), index_meta->field(0));
        return RC::GENERIC_ERROR;
      }
      std::string index_file = index_data_file(base_dir, name(), index_meta->name());
      rc = index->open(index_file.c_str(), *index_meta, *field_meta);
      if (rc != RC::SUCCESS)
      {
        delete index;
        LOG_ERROR("Failed to open index. table=%s, index=%s, file=%s, rc=%d:%s",
                name(), index_meta->name(), index_file.c_str(), rc, strrc(rc));
        return rc;
      }
    }
    indexes_.push_back(index);
  }
  return rc;
}

RC Table::commit_insert(Trx *trx, const RID &rid)
{
  Record record;
  LOG_INFO("RID: %d:%d!!!!!!!!!!!!!!!!!!!!!!!", rid.page_num, rid.slot_num);
  RC rc = record_handler_->get_record(&rid, &record);
  if (rc != RC::SUCCESS)
  {
    return rc;
  }

  return trx->commit_insert(this, record);
}

RC Table::rollback_insert(Trx *trx, const RID &rid)
{

  Record record;
  RC rc = record_handler_->get_record(&rid, &record);
  if (rc != RC::SUCCESS)
  {
    return rc;
  }

  // remove all indexes
  rc = delete_entry_of_indexes(record.data, rid, false);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to delete indexes of record(rid=%d.%d) while rollback insert, rc=%d:%s",
              rid.page_num, rid.slot_num, rc, strrc(rc));
  }
  else
  {
    rc = record_handler_->delete_record(&rid);
  }
  return rc;
}

RC Table::insert_record(Trx *trx, Record *record)
{
  // 首先insert到record中，再将记录insert到index索引中
  RC rc = RC::SUCCESS;

  if (trx != nullptr)
  {
    trx->init_trx_info(this, *record);
  }
  // 插入到record中，并获取对应的rid
  // 这里需要加上分配给null的大小
  rc = record_handler_->insert_record(record->data, table_meta_.record_size() + table_meta_.field_num(), &record->rid);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Insert record failed. table name=%s, rc=%d:%s", table_meta_.name(), rc, strrc(rc));
    return rc;
  }

  if (trx != nullptr)
  {
    rc = trx->insert_record(this, record);
    if (rc != RC::SUCCESS)
    {
      LOG_ERROR("Failed to log operation(insertion) to trx");
      RC rc2 = record_handler_->delete_record(&record->rid);
      if (rc2 != RC::SUCCESS)
      {
        LOG_PANIC("Failed to rollback record data when insert index entries failed. table name=%s, rc=%d:%s",
                  name(), rc2, strrc(rc2));
      }
      return rc;
    }
  }
  // LOG_INFO("INSERT ENTRY OF INDEXES with data %d, rid:page_num :%d,slotnum :%d\n",record->data,record->rid.page_num,record->rid.slot_num);
  // record->data为key,record->rid为value
  rc = insert_entry_of_indexes(record->data, record->rid);
  if (rc != RC::SUCCESS)
  {
    RC rc2 = delete_entry_of_indexes(record->data, record->rid, true);
    if (rc2 != RC::SUCCESS)
    {
      LOG_PANIC("Failed to rollback index data when insert index entries failed. table name=%s, rc=%d:%s",
                name(), rc2, strrc(rc2));
    }
    rc2 = record_handler_->delete_record(&record->rid);
    if (rc2 != RC::SUCCESS)
    {
      LOG_PANIC("Failed to rollback record data when insert index entries failed. table name=%s, rc=%d:%s",
                name(), rc2, strrc(rc2));
    }
    return rc;
  }
  return rc;
}

RC Table::insert_record(Trx *trx, int value_num, const Value *values, Record **ret_record)
{
  // value_num->value的数量,values->value数组，ret_record->用于返回
  if (value_num <= 0 || nullptr == values)
  {
    LOG_ERROR("Invalid argument. value num=%d, values=%p", value_num, values);
    return RC::INVALID_ARGUMENT;
  }

  // record_data只是单纯用来装数据
  char *record_data;            // 对应record.data
  // 依次与table_meta中的内容对比来检验values，并将每项按照table_meta中的offset顺序放在record_data中
  RC rc = make_record(value_num, values, record_data);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to create a record. rc=%d:%s", rc, strrc(rc));
    return rc;
  }
  Record record;
  record.data = record_data;
  rc = insert_record(trx, &record);
  if (ret_record != nullptr)
  {
    *ret_record = &record;
  }
  delete[] record_data;
  return rc;
}

const char *Table::name() const
{
  return table_meta_.name();
}

const TableMeta &Table::table_meta() const
{
  return table_meta_;
}

RC Table::is_legal(const Value &value, const FieldMeta *field)
{

  if (value.type == AttrType::NULLS)
  {
    // field->desc(std::cout);
    if (!field->nullable())
    {
      LOG_ERROR("该列不允许插入null值");
      return RC::SCHEMA_FIELD_NAME_ILLEGAL;
    }
  }
  else if (field->type() != value.type)
  {
    LOG_ERROR("Invalid value type. field name=%s, type=%d, but given=%d",
              field->name(), field->type(), value.type);
    
    return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  }

  if (value.type == AttrType::CHARS)
  {
    // CHARS值需要判断长度
    char *s = (char *)value.data;
    if (strlen(s) > field->len())
    {
      LOG_ERROR("待插入CHARS类型值过长");
      return RC::SCHEMA_FIELD_MISSING;
    }
  }

  return RC::SUCCESS;
}

RC Table::make_record(int value_num, const Value *values, char *&record_out)
{
  // 首先确定插入的数据依次和当前的table属性一样
  // 然后将制作好的record放在record_out中

  // 检查字段类型是否一致
  if (value_num + table_meta_.sys_field_num() != table_meta_.field_num())
  {
    return RC::SCHEMA_FIELD_MISSING;
  }

  Value new_value;
  // 每个record前面有sys_field
  const int normal_field_start_index = table_meta_.sys_field_num();
  // 挨个验证values里的内容是否和table_meta中的field的内容相符合
  for (int i = 0; i < value_num; i++)
  {
    const FieldMeta *field = table_meta_.field(i + normal_field_start_index);
    const Value &value = values[i];

    RC rc = is_legal(value, field);
    if (rc != RC::SUCCESS)
    {
      return rc;
    }
  }

  // 复制所有字段的值
  int record_size = table_meta_.record_size();
  LOG_INFO("record_size: %d", record_size);
  const FieldMeta *field = table_meta_.field(value_num - 1 + normal_field_start_index);
  int null_field_index = field->offset() + field->len();
  // record大小增加value_num个字节，用来存放是否null值
  char *record = new char[record_size + value_num];

  for (int i = 0; i < value_num; i++)
  {
    const FieldMeta *field = table_meta_.field(i + normal_field_start_index);
    const Value &value = values[i];
    bool is_null;

    if (value.is_null)
    {
      // 如果是null值，int/float类型放0，char类型直接放null，date放int类型19700101
      switch (field->type())
      {
      case AttrType::CHARS:
      {
        const char *v = "NULL";
        memcpy(record + field->offset(), v, field->len());
      }
      break;
      case AttrType::DATES:
      {
        int v = 19700101;
        memcpy(record + field->offset(), &v, field->len());
      }
      break;
      case AttrType::FLOATS:
      {
        float v = 0;
        memcpy(record + field->offset(), &v, field->len());
      }
      break;
      case AttrType::INTS:
      {
        int v = 0;
        memcpy(record + field->offset(), &v, field->len());
      }
      break;
      default:
        break;
      }

      // 放入null标志
      is_null = true;
      memcpy(record + null_field_index + i, &is_null, 1);
    }
    else
    {
      memcpy(record + field->offset(), value.data, field->len());
      // 放入null标志
      is_null = false;
      memcpy(record + null_field_index + i, &is_null, 1);
    }
  }

  

  record_out = record;
  return RC::SUCCESS;
}

RC Table::init_record_handler(const char *base_dir)
{
  std::string data_file = std::string(base_dir) + "/" + table_meta_.name() + TABLE_DATA_SUFFIX;
  if (nullptr == data_buffer_pool_)
  {
    data_buffer_pool_ = theGlobalDiskBufferPool();
  }

  int data_buffer_pool_file_id;
  RC rc = data_buffer_pool_->open_file(data_file.c_str(), &data_buffer_pool_file_id);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to open disk buffer pool for file:%s. rc=%d:%s",
              data_file.c_str(), rc, strrc(rc));
    return rc;
  }

  record_handler_ = new RecordFileHandler();
  rc = record_handler_->init(*data_buffer_pool_, data_buffer_pool_file_id);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to init record handler. rc=%d:%s", rc, strrc(rc));
    return rc;
  }

  file_id_ = data_buffer_pool_file_id;
  return rc;
}

/**
 * 为了不把Record暴露出去，封装一下
 */
class RecordReaderScanAdapter
{
public:
  explicit RecordReaderScanAdapter(void (*record_reader)(const char *data, void *context), void *context)
      : record_reader_(record_reader), context_(context)
  {
  }

  void consume(const Record *record)
  {
    record_reader_(record->data, context_);
  }

private:
  void (*record_reader_)(const char *, void *);
  void *context_;
};
static RC scan_record_reader_adapter(Record *record, void *context)
{
  RecordReaderScanAdapter &adapter = *(RecordReaderScanAdapter *)context;
  adapter.consume(record);
  return RC::SUCCESS;
}

RC Table::scan_record(Trx *trx, ConditionFilter *filter, int limit, void *context, void (*record_reader)(const char *data, void *context))
{ //当前scan_record 调用下面的scan_record函数
  RecordReaderScanAdapter adapter(record_reader, context);
  return scan_record(trx, filter, limit, (void *)&adapter, scan_record_reader_adapter);
}

RC Table::scan_record(Trx *trx, ConditionFilter *filter, int limit, void *context, RC (*record_reader)(Record *record, void *context))
{
  if (nullptr == record_reader)
  {
    return RC::INVALID_ARGUMENT;
  }

  if (0 == limit)
  {
    return RC::SUCCESS;
  }

  if (limit < 0)
  {
    limit = INT_MAX;
  }

  // filter == nullptr，则index_scanner也为nullptr
  IndexScanner *index_scanner = find_index_for_scan(filter);
  if (index_scanner != nullptr)
  {
    LOG_INFO("使用index,scan_record_by_index");
    return scan_record_by_index(trx, index_scanner, filter, limit, context, record_reader);
  }
  LOG_INFO("没有使用index");
  // filter == nullptr时，scanner会扫描所有元组
  RC rc = RC::SUCCESS;
  RecordFileScanner scanner;
  rc = scanner.open_scan(*data_buffer_pool_, file_id_, filter);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("failed to open scanner. file id=%d. rc=%d:%s", file_id_, rc, strrc(rc));
    return rc;
  }

  int record_count = 0;
  Record record;

  bool has_next = false;
  rc = scanner.get_first_record(&record, has_next);
  
  for (; RC::SUCCESS == rc && record_count < limit; rc = scanner.get_next_record(&record, has_next))
  {
      // 如果是TEXT，这里拿到的record的RID是第二页的
    if (has_next == true) {
        Record tmp;
        tmp.data = record.data;
        tmp.rid = record.rid;
        tmp.rid.page_num--;
        if (trx == nullptr || trx->is_visible(this, &tmp)) {
            rc = record_reader(&tmp, context);
            record_count++;
            // has_next = false;
        }
    } else {
        if (trx == nullptr || trx->is_visible(this, &record))
        {
            rc = record_reader(&record, context);
            if (rc != RC::SUCCESS)
            {
                break;
            }
            record_count++;
        }
    }
  } // for

  if (RC::RECORD_EOF == rc)
  {
    rc = RC::SUCCESS;
  }
  else
  {
    LOG_ERROR("failed to scan record. file id=%d, rc=%d:%s", file_id_, rc, strrc(rc));
  }
  scanner.close_scan();
  return rc;
}

RC Table::scan_record_by_index(Trx *trx, IndexScanner *scanner, ConditionFilter *filter, int limit, void *context,
                               RC (*record_reader)(Record *, void *))
{
  RC rc = RC::SUCCESS;
  RID rid;
  Record record;
  int record_count = 0;
  while (record_count < limit)
  {
    // 通过bplustree搜索获取rid
    rc = scanner->next_entry(&rid);
    if (rc != RC::SUCCESS)
    {
      if (RC::RECORD_EOF == rc || RC::RECORD_NO_MORE_IDX_IN_MEM == rc)
      {
        rc = RC::SUCCESS;
        break;
      }
      LOG_ERROR("Failed to scan table by index. rc=%d:%s", rc, strrc(rc));
      break;
    }
    // 根据rid获取record
    rc = record_handler_->get_record(&rid, &record);
    if (rc != RC::SUCCESS)
    {
      LOG_ERROR("Failed to fetch record of rid=%d:%d, rc=%d:%s", rid.page_num, rid.slot_num, rc, strrc(rc));
      break;
    }

    if ((trx == nullptr || trx->is_visible(this, &record)) && (filter == nullptr || filter->filter(record)))
    {
      rc = record_reader(&record, context);
      if (rc != RC::SUCCESS)
      {
        LOG_TRACE("Record reader break the table scanning. rc=%d:%s", rc, strrc(rc));
        break;
      }
    }
    record_count++;
  }

  scanner->destroy();
  return rc;
}

class IndexInserter
{
public:
  explicit IndexInserter(Index *index) : index_(index)
  {
  }

  RC insert_index(const Record *record)
  {
    return index_->insert_entry(record->data, &record->rid);
  }

private:
  Index *index_;
};

static RC insert_index_record_reader_adapter(Record *record, void *context)
{
  IndexInserter &inserter = *(IndexInserter *)context;
  return inserter.insert_index(record);
}

std::vector<const char *> Table::get_index_names()
{
  std::vector<const char *> res;
  for (auto idx : indexes_)
  {
    LOG_ERROR("index_name = %s", idx->index_meta().name());
    res.push_back(idx->index_meta().name());
  }

  return res;
}
bool check_attr_name( const int& attr_num, const char *attribute_name[] ){
  for(int i = 0; i < attr_num; i++ ){
    if(attribute_name[i] == nullptr || common::is_blank(attribute_name[i])){
      return true;
    }
  }
  return false;
}
//重载函数
RC Table::create_index(Trx *trx, const char *index_name, const int& attr_num, const char *attribute_name[],int is_unique)
{
  if (index_name == nullptr || common::is_blank(index_name) ||
      check_attr_name(attr_num, attribute_name))
  {
    LOG_ERROR("create_index - INVALID_ARGUMENT");
    return RC::INVALID_ARGUMENT;
  }
  if (table_meta_.index(index_name) != nullptr ||
      table_meta_.find_multi_index_by_fields_for_check(attribute_name,attr_num))
  {
    LOG_ERROR("create_index - SCHEMA_INDEX_EXIST");
    return RC::SCHEMA_INDEX_EXIST;
  }

  const FieldMeta *field_metas[attr_num];
  for(int i=0;i<attr_num;i++){
    const FieldMeta *field_meta = table_meta_.field(attribute_name[i]);
    if (!field_meta)
    {
      LOG_ERROR("create_index - SCHEMA_FIELD_MISSING");
      return RC::SCHEMA_FIELD_MISSING;
    }
    field_metas[i] = field_meta;
  }
  IndexMeta new_index_meta;
  RC rc = new_index_meta.init(index_name, field_metas, attr_num);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("fail to init index meta");
    return rc;
  }

  // 创建索引相关数据
  BplusTreeIndex *index = new BplusTreeIndex();
  std::string index_file = index_data_file(base_dir_.c_str(), name(), index_name);
  // 创建对应文件
  rc = index->create(index_file.c_str(), new_index_meta, field_metas, attr_num, is_unique);
  if (rc != RC::SUCCESS)
  {
    delete index;
    LOG_ERROR("Failed to create bplus tree index. file name=%s, rc=%d:%s", index_file.c_str(), rc, strrc(rc));
    return rc;
  }

  // 遍历当前的所有数据，插入这个索引  就是对之前的创建index前的数据全部建立索引
  IndexInserter index_inserter(index);
  rc = scan_record(trx, nullptr, -1, &index_inserter, insert_index_record_reader_adapter);
  if (rc != RC::SUCCESS)
  {
    // rollback
    delete index;
    LOG_ERROR("Failed to insert index to all records. table=%s, rc=%d:%s", name(), rc, strrc(rc));
    return rc;
  }
  indexes_.push_back(index);

  TableMeta new_table_meta(table_meta_);
  rc = new_table_meta.add_index(new_index_meta);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to add index (%s) on table (%s). error=%d:%s", index_name, name(), rc, strrc(rc));
    return rc;
  }
  // 创建元数据临时文件
  std::string tmp_file = table_meta_file(base_dir_.c_str(), name()) + ".tmp";
  std::fstream fs;
  fs.open(tmp_file, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (!fs.is_open())
  {
    LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", tmp_file.c_str(), strerror(errno));
    return RC::IOERR; // 创建索引中途出错，要做还原操作
  }
  if (new_table_meta.serialize(fs) < 0)
  {
    LOG_ERROR("Failed to dump new table meta to file: %s. sys err=%d:%s", tmp_file.c_str(), errno, strerror(errno));
    return RC::IOERR;
  }
  fs.close();

  // 覆盖原始元数据文件
  std::string meta_file = table_meta_file(base_dir_.c_str(), name());
  int ret = rename(tmp_file.c_str(), meta_file.c_str());
  if (ret != 0)
  {
    LOG_ERROR("Failed to rename tmp meta file (%s) to normal meta file (%s) while creating index (%s) on table (%s). "
              "system error=%d:%s",
              tmp_file.c_str(), meta_file.c_str(), index_name, name(), errno, strerror(errno));
    return RC::IOERR;
  }

  table_meta_.swap(new_table_meta);

  LOG_INFO("successfully add a new index (%s) on the table (%s)", index_name, name());

  return rc;
}
RC Table::create_index(Trx *trx, const char *index_name, const char *attribute_name, int is_unique)
{
  // LOG_INFO("create_index starts");
  if (index_name == nullptr || common::is_blank(index_name) ||
      attribute_name == nullptr || common::is_blank(attribute_name))
  {
    LOG_ERROR("create_index - INVALID_ARGUMENT");
    return RC::INVALID_ARGUMENT;
  }
  if (table_meta_.index(index_name) != nullptr ||
      table_meta_.find_single_index_by_field(attribute_name))
  {
    LOG_ERROR("create_index - SCHEMA_INDEX_EXIST");
    return RC::SCHEMA_INDEX_EXIST;
  }

  const FieldMeta *field_meta = table_meta_.field(attribute_name);
  if (!field_meta)
  {
    LOG_ERROR("create_index - SCHEMA_FIELD_MISSING");
    return RC::SCHEMA_FIELD_MISSING;
  }

  IndexMeta new_index_meta;
  RC rc = new_index_meta.init(index_name, *field_meta);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("fail to init index meta");
    return rc;
  }

  // 创建索引相关数据
  BplusTreeIndex *index = new BplusTreeIndex();
  std::string index_file = index_data_file(base_dir_.c_str(), name(), index_name);
  // 创建对应文件
  rc = index->create(index_file.c_str(), new_index_meta, *field_meta,is_unique);
  if (rc != RC::SUCCESS)
  {
    delete index;
    LOG_ERROR("Failed to create bplus tree index. file name=%s, rc=%d:%s", index_file.c_str(), rc, strrc(rc));
    return rc;
  }

  // 遍历当前的所有数据，插入这个索引  就是对之前的创建index前的数据全部建立索引
  IndexInserter index_inserter(index);
  rc = scan_record(trx, nullptr, -1, &index_inserter, insert_index_record_reader_adapter);
  if (rc != RC::SUCCESS)
  {
    // rollback
    delete index;
    LOG_ERROR("Failed to insert index to all records. table=%s, rc=%d:%s", name(), rc, strrc(rc));
    return rc;
  }
  indexes_.push_back(index);

  TableMeta new_table_meta(table_meta_);
  rc = new_table_meta.add_index(new_index_meta);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to add index (%s) on table (%s). error=%d:%s", index_name, name(), rc, strrc(rc));
    return rc;
  }
  // 创建元数据临时文件
  std::string tmp_file = table_meta_file(base_dir_.c_str(), name()) + ".tmp";
  std::fstream fs;
  fs.open(tmp_file, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (!fs.is_open())
  {
    LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", tmp_file.c_str(), strerror(errno));
    return RC::IOERR; // 创建索引中途出错，要做还原操作
  }
  if (new_table_meta.serialize(fs) < 0)
  {
    LOG_ERROR("Failed to dump new table meta to file: %s. sys err=%d:%s", tmp_file.c_str(), errno, strerror(errno));
    return RC::IOERR;
  }
  fs.close();

  // 覆盖原始元数据文件
  std::string meta_file = table_meta_file(base_dir_.c_str(), name());
  int ret = rename(tmp_file.c_str(), meta_file.c_str());
  if (ret != 0)
  {
    LOG_ERROR("Failed to rename tmp meta file (%s) to normal meta file (%s) while creating index (%s) on table (%s). "
              "system error=%d:%s",
              tmp_file.c_str(), meta_file.c_str(), index_name, name(), errno, strerror(errno));
    return RC::IOERR;
  }

  table_meta_.swap(new_table_meta);

  LOG_INFO("successfully add a new index (%s) on the table (%s)", index_name, name());

  return rc;
}

class RecordUpdater
{
public:
  RecordUpdater(Table &table, Trx *trx, const char *attribute_name, const Value *value) : table_(table), trx_(trx), attribute_name_(attribute_name), value_(value)
  {
  }

  RC update_record(Record *record)
  {
    RC rc = RC::SUCCESS;
    rc = table_.update_record(trx_, record, attribute_name_, value_);
    if (rc == RC::SUCCESS)
    {
      ++updated_count_;
    }
    return rc;
  }

  int updated_count() const
  {
    return updated_count_;
  }

private:
  Table &table_;
  Trx *trx_;
  int updated_count_ = 0;
  const char *attribute_name_;
  const Value *value_;
};

static RC record_reader_update_adapter(Record *record, void *context)
{
  RecordUpdater &record_updater = *(RecordUpdater *)context;
  // std::cout << record->data << std::endl;

  return record_updater.update_record(record);
}

RC Table::update_record(Trx *trx, const char *attribute_name, const Value *value, int condition_num, const Condition conditions[], int *updated_count)
{
  // TODO(xiong): 任务3 实现udpate功能，update单个字段即可。
  if (nullptr == value || nullptr == attribute_name)
  {
    LOG_ERROR("Invalid argument. values=%p, attribute_name=%p", value, attribute_name);
    return RC::INVALID_ARGUMENT;
  }

  // 1.1 有条件，则获取条件过滤器
  RecordUpdater updater(*this, trx, attribute_name, value);
  RC rc = RC::SUCCESS;

  if (condition_num > 0)
  {
    // 元数据检查：判断where中表名是否与要update的表名一致
    const char *rel_name = table_meta_.name();
    for (int i = 0; i < condition_num; i++)
    {
      const Condition &cond = conditions[i];
      if (cond.left_is_attr == 1)
      {
        const char *cond_rel_name = cond.left_attr.relation_name;
        if (cond_rel_name != nullptr && strcmp(cond_rel_name, rel_name) != 0)
        {
          LOG_ERROR("update的表名和where条件中不一致");
          return RC::SCHEMA_TABLE_NAME_ILLEGAL;
        }
      }

      if (cond.right_is_attr == 1)
      {
        const char *cond_rel_name = cond.right_attr.relation_name;
        if (cond_rel_name != nullptr && strcmp(cond_rel_name, rel_name) != 0)
        {
          LOG_ERROR("update的表名和where条件中不一致");
          return RC::SCHEMA_TABLE_NAME_ILLEGAL;
        }
      }
    }

    CompositeConditionFilter condition_filter;
    rc = condition_filter.init(*this, conditions, condition_num);
    if (rc != RC::SUCCESS)
    {
      return rc;
    }
    // 2. 筛选满足所有条件的record，逐条进行更新
    // -1表示不对筛选数量进行限制，
    rc = scan_record(trx, &condition_filter, -1, &updater, record_reader_update_adapter);
  }
  else
  {
    // 1.2 没条件，则遍历所有元组
    rc = scan_record(trx, nullptr, -1, &updater, record_reader_update_adapter);
  }

  if (updated_count != nullptr)
  {
    *updated_count = updater.updated_count();
  }

  if (rc != RC::SUCCESS)
  {
    return rc;
  }

  return rc;
}

RC Table::update_record(Trx *trx, Record *record, const char *attribute_name, const Value *value)
{
  // 这里的参数record是通过scan得到的满足filter条件的record，是原本数据库中的record
  // 1. 获得字段数据
  LOG_INFO("进入table:update_record");
  int i = table_meta_.find_field_index_by_name(attribute_name);
  if (i == -1)
  {
    // 不存在该属性
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }
  const FieldMeta *field_meta = table_meta_.field(i);

  RC rc = RC::SUCCESS;
  // if (trx != nullptr)
  if (false)
  {
    // 更新record
    // 深拷贝record->data
    char *new_record_data = new char[strlen(record->data) + 1];
    strcpy(new_record_data, record->data);
    memcpy(new_record_data + field_meta->offset(), value->data, field_meta->len());

    // 更新事务
    rc = trx->update_record(this, record, new_record_data);
    if (rc != RC::SUCCESS)
    {
      return rc;
    }
  }
  // 根据原始record和value创建一个new_record,此new_record表示一个
  rc = is_legal(*value, field_meta);
  if (rc != RC::SUCCESS)
  {
    return rc;
  }
  // 这里的table_meta_.record_size()包含了sys_field的4个字节 就是根据将各个属性的长度求和 // sizeof(record->rid) = 8
  char *data =(char *)malloc(table_meta_.record_size());
  // memcpy(data + field_meta->offset(), value->data, field_meta->len());
  memcpy(data, record->data, table_meta_.record_size());
  memcpy(data + field_meta->offset(), value->data, field_meta->len());

  rc = update_entry_of_indexes(data, record->rid,record->data, record->rid, false);
  if (rc != RC::SUCCESS)
  {
    free(data);
    LOG_ERROR("update_entry_of_indexes fail");
    return rc;
  }
  // 更新null状态
  auto last_field = table_meta_.field(table_meta_.field_num() - 1);
  int null_field_index = last_field->offset() + last_field->len();
  memcpy(record->data + null_field_index + i - 1, &value->is_null, 1);
  // 修改对应record,将新的record写入进去
  memcpy(record->data + field_meta->offset(), value->data, field_meta->len());
  rc = record_handler_->update_record(record);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Update record failed. table name=%s, rc=%d:%s", table_meta_.name(), rc, strrc(rc));
    free(data);
    return rc;
  }
  free(data);
  
  return rc;
}

RC Table::commit_update(Trx *trx, const RID &rid, char *new_record_data)
{
  RC rc = RC::SUCCESS;
  Record record;
  rc = record_handler_->get_record(&rid, &record);
  if (rc != RC::SUCCESS)
  {
    return rc;
  }

  // 删除索引index
  rc = delete_entry_of_indexes(record.data, record.rid, false); // 重复代码 refer to commit_delete
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to delete indexes of record (rid=%d.%d). rc=%d:%s",
              record.rid.page_num, record.rid.slot_num, rc, strrc(rc));
    return rc;
  }

  // 更新record   
  strcpy(record.data, new_record_data);
  rc = record_handler_->update_record(&record);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Update record failed. table name=%s, rc=%d:%s", table_meta_.name(), rc, strrc(rc));
    return rc;
  }

  // 插入index
  rc = insert_entry_of_indexes(record.data, record.rid);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("insert_entry_of_indexes fail");
    RC rc2 = delete_entry_of_indexes(record.data, record.rid, true);
    if (rc2 != RC::SUCCESS)
    {
      LOG_PANIC("Failed to rollback index data when insert index entries failed. table name=%s, rc=%d:%s",
                name(), rc2, strrc(rc2));
    }
    rc2 = record_handler_->delete_record(&record.rid);
    if (rc2 != RC::SUCCESS)
    {
      LOG_PANIC("Failed to rollback record data when insert index entries failed. table name=%s, rc=%d:%s",
                name(), rc2, strrc(rc2));
    }
    return rc;
  }
  return rc;
}

class RecordDeleter
{
public:
  RecordDeleter(Table &table, Trx *trx) : table_(table), trx_(trx)
  {
  }

  RC delete_record(Record *record)
  {
    RC rc = RC::SUCCESS;
    rc = table_.delete_record(trx_, record);
    if (rc == RC::SUCCESS)
    {
      deleted_count_++;
    }
    return rc;
  }

  int deleted_count() const
  {
    return deleted_count_;
  }

private:
  Table &table_;
  Trx *trx_;
  int deleted_count_ = 0;
};

static RC record_reader_delete_adapter(Record *record, void *context)
{
  RecordDeleter &record_deleter = *(RecordDeleter *)context;
  // std::cout << record->data << std::endl;
  return record_deleter.delete_record(record);
}

RC Table::delete_record(Trx *trx, ConditionFilter *filter, int *deleted_count)
{
  RecordDeleter deleter(*this, trx);
  RC rc = scan_record(trx, filter, -1, &deleter, record_reader_delete_adapter);
  if (deleted_count != nullptr)
  {
    *deleted_count = deleter.deleted_count();
  }
  return rc;
}

RC Table::delete_record(Trx *trx, Record *record)
{
  RC rc = RC::SUCCESS;
  if (trx != nullptr)
  {
    rc = trx->delete_record(this, record);
  }
  else
  {
    rc = delete_entry_of_indexes(record->data, record->rid, false); // 重复代码 refer to commit_delete
    if (rc != RC::SUCCESS)
    {
      LOG_ERROR("Failed to delete indexes of record (rid=%d.%d). rc=%d:%s",
                record->rid.page_num, record->rid.slot_num, rc, strrc(rc));
    }
    else
    {
      rc = record_handler_->delete_record(&record->rid);
    }
  }
  return rc;
}

RC Table::commit_delete(Trx *trx, const RID &rid)
{
  RC rc = RC::SUCCESS;
  Record record;
  rc = record_handler_->get_record(&rid, &record);
  if (rc != RC::SUCCESS)
  {
    return rc;
  }
  rc = delete_entry_of_indexes(record.data, record.rid, false);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to delete indexes of record(rid=%d.%d). rc=%d:%s",
              rid.page_num, rid.slot_num, rc, strrc(rc)); // panic?
  }

  rc = record_handler_->delete_record(&rid);
  if (rc != RC::SUCCESS)
  {
    return rc;
  }

  return rc;
}

RC Table::rollback_delete(Trx *trx, const RID &rid)
{
  RC rc = RC::SUCCESS;
  Record record;
  rc = record_handler_->get_record(&rid, &record);
  if (rc != RC::SUCCESS)
  {
    return rc;
  }

  return trx->rollback_delete(this, record); // update record in place
}
RC Table::update_entry_of_indexes(const char *record_i, const RID &rid_i,
                                  const char *record_d, const RID &rid_d, bool error_on_not_exists)
{
  RC rc = RC::SUCCESS;
  for (Index *index : indexes_)
  {
    rc = index->insert_entry(record_i, &rid_i);
    if (rc == RC::RECORD_DUPLICATE_KEY){
      // 已经当前索引保持原样即可, 后面也不要去删除
      rc = RC::SUCCESS;
      continue;
    }else if (rc != RC::SUCCESS)
    {
      LOG_ERROR("Failed to insert indexes of record (rid=%d.%d). rc=%d:%s",
                rid_i.page_num, rid_i.slot_num, rc, strrc(rc));
      break;
    }
    rc = index->delete_entry(record_d, &rid_d);
    if (rc != RC::SUCCESS)
    {
      if (rc != RC::RECORD_INVALID_KEY || !error_on_not_exists)
      {
        LOG_ERROR("Failed to delete indexes of record (rid=%d.%d). rc=%d:%s",
               rid_d.page_num, rid_d.slot_num, rc, strrc(rc));
        break;
      }
    }
  }
  return rc;
}
RC Table::insert_entry_of_indexes(const char *record, const RID &rid)
{
  RC rc = RC::SUCCESS;
  for (Index *index : indexes_)
  {
    rc = index->insert_entry(record, &rid);
    if (rc != RC::SUCCESS)
    {
      break;
    }
  }
  return rc;
}

RC Table::delete_entry_of_indexes(const char *record, const RID &rid, bool error_on_not_exists)
{
  RC rc = RC::SUCCESS;
  for (Index *index : indexes_)
  {
    rc = index->delete_entry(record, &rid);
    if (rc != RC::SUCCESS)
    {
      if (rc != RC::RECORD_INVALID_KEY || !error_on_not_exists)
      {
        break;
      }
    }
  }
  return rc;
}
void Table::find_index_for_update(std::vector<Index *> &index_cover,const char *attr_name) const{
  for (Index *index : indexes_)
  {
    auto fields = index->index_meta().fields();
    for(const auto &field : fields){
      int tmp =strcmp(field.c_str(), attr_name);  
      if (0 == tmp)
      {
        index_cover.push_back(index);
        break;
      }
    }
  }
}
Index *Table::find_index(const char *index_name) const
{
  // 实际上index_name为attribute_name,传进来的就是attribute_name
  for (Index *index : indexes_)
  {
    int tmp =strcmp(index->index_meta().name(), index_name);
    if (0 == tmp)
    {
      return index;
    }
  }
  return nullptr;
}
IndexScanner *Table::find_multi_index_for_scan(const CompositeConditionFilter &filters)
{
  int filter_num = filters.filter_num();
  // std::vector<const ConDesc *> field_cond_descs;
  const char *field_names[filter_num];
  std::vector<CompOp> comp_ops;
  std::vector<const char *> values;
  std::vector<const ConDesc *> field_cond_descs;   // for encounter the single index case with null
  for(int i = 0; i < filter_num; i++){
    const DefaultConditionFilter *default_condition_filter = dynamic_cast<const DefaultConditionFilter *>(&filters.filter(i));
    const ConDesc *field_cond_desc = nullptr;
    const ConDesc *value_cond_desc = nullptr;
    if (default_condition_filter->left().is_attr && !default_condition_filter->right().is_attr)
    {
      field_cond_desc = &default_condition_filter->left();
      value_cond_desc = &default_condition_filter->right();
    }
    else if (default_condition_filter->right().is_attr && !default_condition_filter->left().is_attr)
    {
      field_cond_desc = &default_condition_filter->right();
      value_cond_desc = &default_condition_filter->left();
    }
    if (field_cond_desc == nullptr || value_cond_desc == nullptr)
    {
      return nullptr;
    }
    const FieldMeta *field_meta = table_meta_.find_field_by_offset(field_cond_desc->attr_offset);
    if (nullptr == field_meta)
    {
      LOG_PANIC("Cannot find field by offset %d. table=%s",
              field_cond_desc->attr_offset, name());
      return nullptr;
    }
    field_names[i] = field_meta->name();
    comp_ops.push_back(default_condition_filter->comp_op());
    values.push_back((char *)value_cond_desc->value);
    field_cond_descs.push_back(field_cond_desc);
  }

  int match_num = 0;
  const IndexMeta *index_meta = table_meta_.find_multi_index_by_fields(field_names,filter_num,match_num);
  if (nullptr == index_meta)
  {
    return nullptr;
  }

  Index *index = find_index(index_meta->name());
  if (nullptr == index)
  {
    return nullptr;
  }
  if (index->Get_Field_Num() == 1){
    //  (filter.comp_op(), (const char *)value_cond_desc->value, field_cond_desc->null_field_index)
    return index->create_single_index_scanner(comp_ops[0], values[0], field_cond_descs[0]->null_field_index);
  }
                        // (const std::vector<CompOp> &comp_ops, const std::vector<const char *> &values)
  return index->create_multi_index_scanner(comp_ops, values, match_num);
}

IndexScanner *Table::find_single_index_for_scan(const DefaultConditionFilter &filter)
{
  const ConDesc *field_cond_desc = nullptr;
  const ConDesc *value_cond_desc = nullptr;
  if (filter.left().is_attr && !filter.right().is_attr)
  {
    field_cond_desc = &filter.left();
    value_cond_desc = &filter.right();
  }
  else if (filter.right().is_attr && !filter.left().is_attr)
  {
    field_cond_desc = &filter.right();
    value_cond_desc = &filter.left();
  }
  if (field_cond_desc == nullptr || value_cond_desc == nullptr)
  {
    return nullptr;
  }

  const FieldMeta *field_meta = table_meta_.find_field_by_offset(field_cond_desc->attr_offset);
  if (nullptr == field_meta)
  {
    LOG_PANIC("Cannot find field by offset %d. table=%s",
              field_cond_desc->attr_offset, name());
    return nullptr;
  }

  const IndexMeta *index_meta = table_meta_.find_single_index_by_field(field_meta->name());
  if (nullptr == index_meta)
  {
    return nullptr;
  }

  Index *index = find_index(index_meta->name());
  if (nullptr == index)
  {
    return nullptr;
  }

  return index->create_single_index_scanner(filter.comp_op(), (const char *)value_cond_desc->value, field_cond_desc->null_field_index);
}

IndexScanner *Table::find_index_for_scan(const ConditionFilter *filter)
{
  if (nullptr == filter)
  {
    return nullptr;
  }
  IndexScanner * index_scanner=nullptr;
  // remove dynamic_cast
  const DefaultConditionFilter *default_condition_filter = dynamic_cast<const DefaultConditionFilter *>(filter);
  if (default_condition_filter != nullptr)
  {
    index_scanner =  find_single_index_for_scan(*default_condition_filter);
  }
  if (index_scanner != nullptr){
    return index_scanner;
  }
  const CompositeConditionFilter *composite_condition_filter = dynamic_cast<const CompositeConditionFilter *>(filter);
  
  if (composite_condition_filter != nullptr)
  {
    index_scanner =  find_multi_index_for_scan(*composite_condition_filter);
  }
  return index_scanner;
}

RC Table::sync()
{
  RC rc = data_buffer_pool_->flush_all_pages(file_id_);
  if (rc != RC::SUCCESS)
  {
    LOG_ERROR("Failed to flush table's data pages. table=%s, rc=%d:%s", name(), rc, strrc(rc));
    return rc;
  }

  for (Index *index : indexes_)
  {
    rc = index->sync();
    if (rc != RC::SUCCESS)
    {
      LOG_ERROR("Failed to flush index's pages. table=%s, index=%s, rc=%d:%s",
                name(), index->index_meta().name(), rc, strrc(rc));
      return rc;
    }
  }
  LOG_INFO("Sync table over. table=%s", name());
  return rc;
}
