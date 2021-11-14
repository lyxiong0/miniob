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
// Created by Wangyunlai on 2021/5/24.
//

#include <atomic>

#include "storage/trx/trx.h"
#include "storage/common/table.h"
#include "storage/common/record_manager.h"
#include "storage/common/field_meta.h"
#include "common/log/log.h"

static const uint32_t DELETED_FLAG_BIT_MASK = 0x80000000;
static const uint32_t TRX_ID_BIT_MASK = 0x7FFFFFFF;

int32_t Trx::default_trx_id()
{
  return 0;
}

int32_t Trx::next_trx_id()
{
  static std::atomic<int32_t> trx_id;
  return ++trx_id;
}

const char *Trx::trx_field_name()
{
  return "__trx";
}

AttrType Trx::trx_field_type()
{
  return INTS;
}

int Trx::trx_field_len()
{
  return sizeof(int32_t);
}

Trx::Trx()
{
}

Trx::~Trx()
{
}

RC Trx::update_record(Table *table, Record *record, char *new_record_data)
{
  LOG_ERROR("Start update");
  start_if_not_started();
  Operation *old_oper = find_operation(table, record->rid);

  if (old_oper != nullptr)
  {
    if (old_oper->type() == Operation::Type::DELETE)
    {
      // 不能更新已经删除的记录
      LOG_ERROR("Can not update record which is already deleted");
      return RC::GENERIC_ERROR;
    }
    else if (old_oper->type() == Operation::Type::UPDATE)
    {
      // 每次更新都会删除上次更新的记录
      // 即整个事务最多一次update
      // 每次更新都会删除上次更新/插入的记录
      // 即整个事务最多一次update或insert
      delete_operation(table, record->rid);
    }
  }

  // digeset：相当于rid的哈希函数
  reocrd_data_map_[digest_(record->rid)] = new_record_data;
  set_record_trx_id(table, *record, trx_id_, false);
  insert_operation(table, Operation::Type::UPDATE, record->rid);
  LOG_ERROR("finish update");
  return RC::SUCCESS;
}

RC Trx::insert_record(Table *table, Record *record)
{
  RC rc = RC::SUCCESS;
  // 先校验是否以前是否存在过(应该不会存在)
  Operation *old_oper = find_operation(table, record->rid);
  if (old_oper != nullptr)
  {
    return RC::GENERIC_ERROR; // error code
  }

  start_if_not_started();

  // 设置record中trx_field为当前的事务号
  // commit insert的时候会将事务号设置为0，所以这里不用设置？
  // set_record_trx_id(table, *record, trx_id_, false);
  // 记录到operations中
  insert_operation(table, Operation::Type::INSERT, record->rid);
  return rc;
}

RC Trx::delete_record(Table *table, Record *record)
{
  // ("delete_record record: %d - %d", record->rid.page_num, record->rid.slot_num);
  RC rc = RC::SUCCESS;
  start_if_not_started();
  Operation *old_oper = find_operation(table, record->rid);
  // 原代码
  if (old_oper != nullptr)
  {
    if (old_oper->type() != Operation::Type::DELETE)
    {
      // 上次操作是插入或更新，删除即为撤销
      delete_operation(table, record->rid);
      return RC::SUCCESS;
    }
    else
    {
      return RC::GENERIC_ERROR;
    }
  }

  set_record_trx_id(table, *record, trx_id_, true);
  insert_operation(table, Operation::Type::DELETE, record->rid);
  return rc;
}

void Trx::set_record_trx_id(Table *table, Record &record, int32_t trx_id, bool deleted) const
{
  const FieldMeta *trx_field = table->table_meta().trx_field();
  int32_t *ptrx_id = (int32_t *)(record.data + trx_field->offset());
  if (deleted)
  {
    trx_id |= DELETED_FLAG_BIT_MASK;
  }
  *ptrx_id = trx_id;
}

void Trx::get_record_trx_id(Table *table, const Record &record, int32_t &trx_id, bool &deleted)
{
  const FieldMeta *trx_field = table->table_meta().trx_field();
  int32_t trx = *(int32_t *)(record.data + trx_field->offset());
  trx_id = trx & TRX_ID_BIT_MASK;
  deleted = (trx & DELETED_FLAG_BIT_MASK) != 0;
}

Operation *Trx::find_operation(Table *table, const RID &rid)
{
  std::unordered_map<Table *, OperationSet>::iterator table_operations_iter = operations_.find(table);
  if (table_operations_iter == operations_.end())
  {
    return nullptr;
  }

  OperationSet &table_operations = table_operations_iter->second;
  Operation tmp(Operation::Type::UNDEFINED, rid);
  OperationSet::iterator operation_iter = table_operations.find(tmp);
  if (operation_iter == table_operations.end())
  {
    return nullptr;
  }
  return const_cast<Operation *>(&(*operation_iter));
}

void Trx::insert_operation(Table *table, Operation::Type type, const RID &rid)
{
  OperationSet &table_operations = operations_[table];
  table_operations.emplace(type, rid);
}

void Trx::delete_operation(Table *table, const RID &rid)
{
  std::unordered_map<Table *, OperationSet>::iterator table_operations_iter = operations_.find(table);
  if (table_operations_iter == operations_.end())
  {
    return;
  }

  Operation tmp(Operation::Type::UNDEFINED, rid);
  int origin_size = table_operations_iter->second.size();
  table_operations_iter->second.erase(tmp);
  int delete_size = table_operations_iter->second.size();
}

RC Trx::commit()
{
  RC rc = RC::SUCCESS;
  for (const auto &table_operations : operations_)
  {
    Table *table = table_operations.first;
    const OperationSet &operation_set = table_operations.second;
    for (const Operation &operation : operation_set)
    {

      RID rid;
      rid.page_num = operation.page_num();
      rid.slot_num = operation.slot_num();

      switch (operation.type())
      {
      case Operation::Type::INSERT:
      {
        rc = table->commit_insert(this, rid);
        if (rc != RC::SUCCESS)
        {
          // handle rc
          LOG_ERROR("Failed to commit insert operation. rid=%d.%d, rc=%d:%s",
                    rid.page_num, rid.slot_num, rc, strrc(rc));
        }
      }
      break;
      case Operation::Type::DELETE:
      {
        rc = table->commit_delete(this, rid);
        if (rc != RC::SUCCESS)
        {
          // handle rc
          LOG_ERROR("Failed to commit delete operation. rid=%d.%d, rc=%d:%s",
                    rid.page_num, rid.slot_num, rc, strrc(rc));
        }
      }
      break;
      case Operation::Type::UPDATE:
      {
        char *new_record_data = reocrd_data_map_[digest_(rid)];
        if (new_record_data == nullptr)
        {
          LOG_ERROR("new_record_data is nullptr, can not update record");
          rc = RC::GENERIC_ERROR;
        }
        else
        {
          rc = table->commit_update(this, rid, new_record_data);
        }
      }
      break;
      default:
      {
        LOG_PANIC("Unknown operation. type=%d", (int)operation.type());
      }
      break;
      }
    }
  }

  operations_.clear();
  trx_id_ = 0;
  return rc;
}

RC Trx::rollback()
{
  RC rc = RC::SUCCESS;
  for (const auto &table_operations : operations_)
  {
    Table *table = table_operations.first;
    const OperationSet &operation_set = table_operations.second;
    for (const Operation &operation : operation_set)
    {

      RID rid;
      rid.page_num = operation.page_num();
      rid.slot_num = operation.slot_num();

      switch (operation.type())
      {
      case Operation::Type::INSERT:
      {
        rc = table->rollback_insert(this, rid);
        if (rc != RC::SUCCESS)
        {
          // handle rc
          LOG_ERROR("Failed to rollback insert operation. rid=%d.%d, rc=%d:%s",
                    rid.page_num, rid.slot_num, rc, strrc(rc));
        }
      }
      break;
      case Operation::Type::DELETE:
      {
        rc = table->rollback_delete(this, rid);
        if (rc != RC::SUCCESS)
        {
          // handle rc
          LOG_ERROR("Failed to rollback delete operation. rid=%d.%d, rc=%d:%s",
                    rid.page_num, rid.slot_num, rc, strrc(rc));
        }
      }
      break;
      // TODO(xiong): 先不写roll back
      // case Operation::Type::UPDATE:
      // {
      //   rc = table->rollback_update(this, rid);
      //   if (rc != RC::SUCCESS)
      //   {
      //     // handle rc
      //     LOG_ERROR("Failed to rollback delete operation. rid=%d.%d, rc=%d:%s",
      //               rid.page_num, rid.slot_num, rc, strrc(rc));
      //   }
      // }
      // break;
      default:
      {
        LOG_PANIC("Unknown operation. type=%d", (int)operation.type());
      }
      break;
      }
    }
  }

  operations_.clear();
  trx_id_ = 0;
  return rc;
}

RC Trx::commit_insert(Table *table, Record &record)
{
  set_record_trx_id(table, record, 0, false);
  return RC::SUCCESS;
}

RC Trx::rollback_delete(Table *table, Record &record)
{
  set_record_trx_id(table, record, 0, false);
  return RC::SUCCESS;
}

bool Trx::is_visible(Table *table, const Record *record)
{
  int32_t record_trx_id;
  bool record_deleted;
  get_record_trx_id(table, *record, record_trx_id, record_deleted);

  // 0 表示这条数据已经提交
  if (0 == record_trx_id || record_trx_id == trx_id_)
  {
    return !record_deleted;
  }

  return record_deleted; // 当前记录上面有事务号，说明是未提交数据，那么如果有删除标记的话，就表示是未提交的删除
}

void Trx::init_trx_info(Table *table, Record &record)
{
  set_record_trx_id(table, record, trx_id_, false);
}

void Trx::start_if_not_started()
{
  if (trx_id_ == 0)
  {
    trx_id_ = next_trx_id();
  }
}