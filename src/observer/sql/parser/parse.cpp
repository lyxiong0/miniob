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
// Created by Longda on 2021/4/13.
//

#include <mutex>
#include <regex>
#include <string>
#include <vector>
#include "sql/parser/parse.h"
#include "rc.h"
#include "common/log/log.h"

RC parse(char *st, Query *sqln);

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
  void relation_attr_init(RelAttr *relation_attr, const char *relation_name, const char *attribute_name, const char *window_function_name, int _is_desc)
  {
    if (relation_name != nullptr)
    {
      relation_attr->relation_name = strdup(relation_name);
    }
    else
    {
      relation_attr->relation_name = nullptr;
    }

    relation_attr->attribute_name = strdup(attribute_name);
      LOG_ERROR("%s", attribute_name);

    if (window_function_name != nullptr)
    {
      relation_attr->window_function_name = strdup(window_function_name);
    }
    else
    {
      relation_attr->window_function_name = nullptr;
    }

    relation_attr->is_desc = _is_desc;
  }

  void relation_attr_destroy(RelAttr *relation_attr)
  {
    free(relation_attr->relation_name);
    free(relation_attr->attribute_name);
    free(relation_attr->window_function_name);

    relation_attr->relation_name = nullptr;
    relation_attr->attribute_name = nullptr;
    relation_attr->window_function_name = nullptr;
  }

  void value_init_integer(Value *value, int v)
  {
    value->type = INTS;
    value->data = malloc(sizeof(v));
    memcpy(value->data, &v, sizeof(v));
  }

  void value_init_float(Value *value, float v)
  {
    value->type = FLOATS;
    value->data = malloc(sizeof(v));
    memcpy(value->data, &v, sizeof(v));
  }

  void value_destroy(Value *value)
  {
    value->type = UNDEFINED;
    free(value->data);
    value->data = nullptr;
  }
  // check date 格式
  bool check_date_data(const char *s)
  {
    std::string str = s;
    std::regex pattern("((19[7-9][0-9]|20[0-2][0-9]|203[0-7])-(((0?[13578]|1[02])-([12][0-9]|3[01]|0?[1-9]))|((0?[469]|11)-([12][0-9]|30|0?[1-9]))|(0?2-([1][0-9]|2[0-8]|0?[1-9]))))|(19(8[048]|[79][26])-0?2-29)|(2038-((0?1-([1-2][0-9]|3[0-1]|0?[1-9]))|(0?2-(1[0-9]|2[0-8]|0?[1-9]))))");
    if (std::regex_match(str, pattern))
    {
      return true;
    }
    return false;
  }

  int convert_date(const char *s)
  {
    // 设定格式为yyyy-mm-dd/yyyy-m-dd/yyyy-mm-d/yyyy-m-d
    std::string str = s;
    int len = str.length();
    int num = 0;
    int mul = 1;
    for (int i = len - 1; i >= 0; i--)
    {
      if (s[i] != '-')
      {
        num += mul * (s[i] - '0');
        mul *= 10;
      }
      else if (i == len - 2)
      {
        mul *= 10;
      }
      else if (i == len - 5)
      {
        if (mul == 1000)
        {
          mul *= 10;
        }
      }
      else if (i == len - 4)
      {
        mul *= 10;
      }
    }
    return num;
  }

  bool match_null(const char *s)
  {
    std::string str = s;
    std::regex format_("^[Nn][Uu][Ll][Ll]$");

    if (std::regex_match(str, format_))
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  void value_init_string(Value *value, const char *v)
  {
    if (check_date_data(v))
    {
      LOG_INFO("成功匹配日期格式");
      value->type = DATES;
      // 转换为数字
      int date_num = convert_date(v);
      value->data = malloc(sizeof(date_num));
      memcpy(value->data, &date_num, sizeof(date_num));
      // std::cout << "now the insert date in value->data is " << *(int *)(value->data) << std::endl;
    }
    else if (match_null(v))
    {
      std::cout << "满足null格式" << std::endl;
      value->type = NULLS;
      value->data = strdup(v);
    }
    else
    {
      std::cout << "没有成功匹配日期格式" << std::endl;

      value->type = CHARS;
      value->data = strdup(v);
      // std::cout << "now the insert char in value->data is " << (char *)(value->data) << std::endl;
    }
  }

  void condition_init(Condition *condition, CompOp comp,
                      int left_is_attr, RelAttr *left_attr, Value *left_value,
                      int right_is_attr, RelAttr *right_attr, Value *right_value)
  {
    LOG_INFO("condition_init function starts");
    condition->comp = comp;
    condition->left_is_attr = left_is_attr;
    if (left_is_attr)
    {
      LOG_INFO("left_is_attr=true and attr.relation=%s attr.attribute_name=%s ", left_attr->relation_name, left_attr->attribute_name);
      condition->left_attr = *left_attr;
    }
    else
    {
      LOG_INFO("left_is_attr=false and left_value.type=%d and its data=%s", left_value->type, (char *)left_value->data);
      condition->left_value = *left_value;
    }

    condition->right_is_attr = right_is_attr;
    if (right_is_attr)
    {
      LOG_INFO("right_is_attr=true and attr.relation=%s attr.attribute_name=%s ", right_attr->relation_name, right_attr->attribute_name);
      condition->right_attr = *right_attr;
    }
    else
    {
      LOG_INFO("right_is_attr=false and left_value.type=%d and its data=%s", right_value->type, (char *)right_value->data);
      condition->right_value = *right_value;
    }
  }
  void condition_destroy(Condition *condition)
  {
    if (condition->left_is_attr)
    {
      relation_attr_destroy(&condition->left_attr);
    }
    else
    {
      value_destroy(&condition->left_value);
    }
    if (condition->right_is_attr)
    {
      relation_attr_destroy(&condition->right_attr);
    }
    else
    {
      value_destroy(&condition->right_value);
    }
  }

  void attr_info_init(AttrInfo *attr_info, const char *name, AttrType type, size_t length, TrueOrFalse is_nullable)
  {
    attr_info->name = strdup(name);
    attr_info->type = type;
    attr_info->length = length;

    if (is_nullable == ISTRUE)
    {
      attr_info->is_nullable = 1;
    }
    else
    {
      attr_info->is_nullable = 0;
    }
  }
  void attr_info_destroy(AttrInfo *attr_info)
  {
    free(attr_info->name);
    attr_info->name = nullptr;
  }

  void selects_init(Selects *selects, ...);
  void selects_append_attribute(Selects *selects, RelAttr *rel_attr)
  {
    selects->attributes[selects->attr_num++] = *rel_attr;
  }
  void selects_append_relation(Selects *selects, const char *relation_name)
  {
    selects->relations[selects->relation_num++] = strdup(relation_name);
  }
  void selects_append_order(Selects *selects, RelAttr *rel_attr)
  {
    selects->order_attrs[selects->order_num++] = *rel_attr;
  }

  void selects_append_conditions(Selects *selects, Condition conditions[], size_t condition_num)
  {
    assert(condition_num <= sizeof(selects->conditions) / sizeof(selects->conditions[0]));
    for (size_t i = 0; i < condition_num; i++)
    {
      selects->conditions[i] = conditions[i];
    }
    selects->condition_num = condition_num;
  }

  void selects_destroy(Selects *selects)
  {
    for (size_t i = 0; i < selects->attr_num; i++)
    {
      relation_attr_destroy(&selects->attributes[i]);
    }
    selects->attr_num = 0;

    for (size_t i = 0; i < selects->relation_num; i++)
    {
      free(selects->relations[i]);
      selects->relations[i] = NULL;
    }
    selects->relation_num = 0;

    for (size_t i = 0; i < selects->condition_num; i++)
    {
      condition_destroy(&selects->conditions[i]);
    }
    selects->condition_num = 0;

    for (size_t i = 0; i < selects->order_num; i++)
    {
      relation_attr_destroy(&selects->order_attrs[i]);
    }
    selects->order_num = 0;
  }

  void inserts_init(Inserts *inserts, const char *relation_name, Value values[], size_t value_num, size_t index)
  {
    assert(value_num <= sizeof(inserts->values[index]) / sizeof(inserts->values[index][0]));

    inserts->relation_name = strdup(relation_name);
    for (size_t i = 0; i < value_num; i++)
    {
      inserts->values[index][i] = values[i];
    }
    inserts->value_num[index] = value_num;
    inserts->group_num = index;
  }

  void inserts_destroy(Inserts *inserts)
  {
    free(inserts->relation_name);
    inserts->relation_name = nullptr;

    for (size_t i = 0; i < inserts->group_num; i++)
    {
      for (size_t j = 0; j < inserts->value_num[i]; j++)
      {
        value_destroy(&inserts->values[i][j]);
      }
      inserts->value_num[i] = 0;
    }

    inserts->group_num = 0;
  }

  void deletes_init_relation(Deletes *deletes, const char *relation_name)
  {
    deletes->relation_name = strdup(relation_name);
  }

  void deletes_set_conditions(Deletes *deletes, Condition conditions[], size_t condition_num)
  {
    assert(condition_num <= sizeof(deletes->conditions) / sizeof(deletes->conditions[0]));
    for (size_t i = 0; i < condition_num; i++)
    {
      deletes->conditions[i] = conditions[i];
    }
    deletes->condition_num = condition_num;
  }
  void deletes_destroy(Deletes *deletes)
  {
    for (size_t i = 0; i < deletes->condition_num; i++)
    {
      condition_destroy(&deletes->conditions[i]);
    }
    deletes->condition_num = 0;
    free(deletes->relation_name);
    deletes->relation_name = nullptr;
  }

  void updates_init(Updates *updates, const char *relation_name, const char *attribute_name,
                    Value *value, Condition conditions[], size_t condition_num)
  {
    updates->relation_name = strdup(relation_name);
    updates->attribute_name = strdup(attribute_name);
    updates->value = *value;

    assert(condition_num <= sizeof(updates->conditions) / sizeof(updates->conditions[0]));
    for (size_t i = 0; i < condition_num; i++)
    {
      updates->conditions[i] = conditions[i];
    }
    updates->condition_num = condition_num;
  }

  void updates_destroy(Updates *updates)
  {
    free(updates->relation_name);
    free(updates->attribute_name);
    updates->relation_name = nullptr;
    updates->attribute_name = nullptr;

    value_destroy(&updates->value);

    for (size_t i = 0; i < updates->condition_num; i++)
    {
      condition_destroy(&updates->conditions[i]);
    }
    updates->condition_num = 0;
  }

  void create_table_append_attribute(CreateTable *create_table, AttrInfo *attr_info)
  {
    create_table->attributes[create_table->attribute_count++] = *attr_info;
  }
  void create_table_init_name(CreateTable *create_table, const char *relation_name)
  {
    create_table->relation_name = strdup(relation_name);
  }
  void create_table_destroy(CreateTable *create_table)
  {
    for (size_t i = 0; i < create_table->attribute_count; i++)
    {
      attr_info_destroy(&create_table->attributes[i]);
    }
    create_table->attribute_count = 0;
    free(create_table->relation_name);
    create_table->relation_name = nullptr;
  }

  void drop_table_init(DropTable *drop_table, const char *relation_name)
  {
    drop_table->relation_name = strdup(relation_name);
  }
  void drop_table_destroy(DropTable *drop_table)
  {
    free(drop_table->relation_name);
    drop_table->relation_name = nullptr;
  }

  void create_index_init(CreateIndex *create_index, const char *index_name,
                         const char *relation_name, const char *attr_name)
  {
    create_index->index_name = strdup(index_name);
    create_index->relation_name = strdup(relation_name);
    create_index->attribute_name = strdup(attr_name);
  }
  void create_index_destroy(CreateIndex *create_index)
  {
    free(create_index->index_name);
    free(create_index->relation_name);
    free(create_index->attribute_name);

    create_index->index_name = nullptr;
    create_index->relation_name = nullptr;
    create_index->attribute_name = nullptr;
  }

  void drop_index_init(DropIndex *drop_index, const char *index_name)
  {
    drop_index->index_name = strdup(index_name);
  }
  void drop_index_destroy(DropIndex *drop_index)
  {
    free((char *)drop_index->index_name);
    drop_index->index_name = nullptr;
  }

  void desc_table_init(DescTable *desc_table, const char *relation_name)
  {
    desc_table->relation_name = strdup(relation_name);
  }

  void desc_table_destroy(DescTable *desc_table)
  {
    free((char *)desc_table->relation_name);
    desc_table->relation_name = nullptr;
  }

  void load_data_init(LoadData *load_data, const char *relation_name, const char *file_name)
  {
    load_data->relation_name = strdup(relation_name);

    if (file_name[0] == '\'' || file_name[0] == '\"')
    {
      file_name++;
    }
    char *dup_file_name = strdup(file_name);
    int len = strlen(dup_file_name);
    if (dup_file_name[len - 1] == '\'' || dup_file_name[len - 1] == '\"')
    {
      dup_file_name[len - 1] = 0;
    }
    load_data->file_name = dup_file_name;
  }

  void load_data_destroy(LoadData *load_data)
  {
    free((char *)load_data->relation_name);
    free((char *)load_data->file_name);
    load_data->relation_name = nullptr;
    load_data->file_name = nullptr;
  }

  void query_init(Query *query)
  {
    query->flag = SCF_ERROR;
    memset(&query->sstr, 0, sizeof(query->sstr));
  }

  Query *query_create()
  {
    Query *query = (Query *)malloc(sizeof(Query));
    if (nullptr == query)
    {
      LOG_ERROR("Failed to alloc memroy for query. size=%ld", sizeof(Query));
      return nullptr;
    }

    query_init(query);
    return query;
  }

  void query_reset(Query *query)
  {
    switch (query->flag)
    {
    case SCF_SELECT:
    {

      selects_destroy(&query->sstr.selection);
    }
    break;
    case SCF_INSERT:
    {
      inserts_destroy(&query->sstr.insertion);
    }
    break;
    case SCF_DELETE:
    {
      deletes_destroy(&query->sstr.deletion);
    }
    break;
    case SCF_UPDATE:
    {
      updates_destroy(&query->sstr.update);
    }
    break;
    case SCF_CREATE_TABLE:
    {
      create_table_destroy(&query->sstr.create_table);
    }
    break;
    case SCF_DROP_TABLE:
    {
      drop_table_destroy(&query->sstr.drop_table);
    }
    break;
    case SCF_CREATE_INDEX:
    {
      create_index_destroy(&query->sstr.create_index);
    }
    break;
    case SCF_DROP_INDEX:
    {
      drop_index_destroy(&query->sstr.drop_index);
    }
    break;
    case SCF_SYNC:
    {
    }
    break;
    case SCF_SHOW_TABLES:
      break;

    case SCF_DESC_TABLE:
    {
      desc_table_destroy(&query->sstr.desc_table);
    }
    break;

    case SCF_LOAD_DATA:
    {
      load_data_destroy(&query->sstr.load_data);
    }
    break;
    case SCF_BEGIN:
    case SCF_COMMIT:
    case SCF_ROLLBACK:
    case SCF_HELP:
    case SCF_EXIT:
    case SCF_ERROR:
      break;
    }
  }

  void query_destroy(Query *query)
  {
    query_reset(query);
    free(query);
  }

  void log_err(const char *info)
  {
    LOG_ERROR(info);
  }

  const char *number_to_str(int number)
  {
    char s[25];
    char ret[25];
    int idx = 0;
    int i = 0;

    if (number == 0)
    {
      s[idx++] = '0';
      s[idx] = '\0';
      return strdup(s);
    }

    if (number < 0)
    {
      ret[i++] = '-';
      number = -number;
    }

    while (number != 0)
    {
      s[idx++] = number % 10 + '0';
      number /= 10;
    }

    for (int j = 0; j < idx; ++j)
    {
      ret[i++] = s[j];
    }

    ret[i] = '\0';
    return strdup(ret);
  }

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

////////////////////////////////////////////////////////////////////////////////

extern "C" int sql_parse(const char *st, Query *sqls);

RC parse(const char *st, Query *sqln)
{
  sql_parse(st, sqln);

  if (sqln->flag == SCF_ERROR)
    return SQL_SYNTAX;
  else
    return SUCCESS;
}