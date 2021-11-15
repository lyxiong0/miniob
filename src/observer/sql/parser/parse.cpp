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

  //获取子串
  char *substr(const char *s, int n1, int n2) /*从s中提取下标为n1~n2的字符组成一个新字符串，然后返回这个新串的首地址*/
  {
    // printf("start call substr on s %s with n1 is %d and n2 is %d \n",s,n1,n2);
    char *sp = (char *)malloc(sizeof(char) * (n2 - n1 + 2));
    int i, j = 0;

    // printf("now the substr is going \n");
    for (i = n1; i <= n2; i++)
    {
      sp[j++] = s[i];
    }
    sp[j] = 0;
    // printf("now the substr end and new string is %s \n",sp);
    return sp;
  }

  void init_attr_or_value(RelAttr *attr, Value *value, int *is_attr, const char *s)
  {
    // 左侧是ID.ID / ID / number
    LOG_INFO("init_attr_or_value - %s", s);
    if (s[0] >= '0' && s[0] <= '9')
    {
      // 左侧是数字
      *is_attr = 0;
      int digit = 0;
      int j = 0;
      while (s[j] != '\0' && s[j] != '.')
      {
        digit = digit * 10 + (s[j] - '0');
        ++j;
      }

      if (s[j] == '.')
      {
        // 存在小数
        float digit_f = (float)digit;
        float div_num = 10;
        ++j;
        while (s[j] != '\0')
        {
          digit_f += (s[j++] - '0') / div_num;
          div_num /= 10;
        }
        value_init_float(value, digit_f, false);
      }
      else
      {
        value_init_integer(value, digit, false);
      }
    }
    else if (s[0] == '\'')
    {
      *is_attr = 0;
      // 左侧是字符串
      LOG_INFO("start init string");
      value_init_string_with_text(value, substr(s, 1, strlen(s) - 2), false, strlen(s) - 2);
    } else if (strcmp(s, "NULL") == 0) {
      *is_attr = 0;
      // 左侧为NULL值
      value_init_string(value, "NULL", true);
    } 
    else
    {
      // 左侧是ID或ID.ID
      LOG_INFO("init ID, s = %s", s);
      *is_attr = 1;
      char tmp[20];
      int j = 0;

      while (s[j] != '\0' && s[j] != '.')
      {
        tmp[j] = s[j];
        ++j;
      }

      tmp[j] = '\0';
      char *relation_name = nullptr;
      char *attribute_name = nullptr;

      if (s[j] == '.')
      {
        // ID.ID的形式
        relation_name = strdup(tmp);
        ++j;
        int idx = 0;
        while (s[j] != '\0')
        {
          tmp[idx++] = s[j];
          ++j;
        }

        tmp[idx] = '\0';
      }
      attribute_name = strdup(tmp);
      relation_attr_init(attr, relation_name, attribute_name, NULL, 0);
    }
  }

  void condition_exp(Condition *condition, const char **left_exp_names, CompOp comp, const char **right_exp_names)
  {
    int left_is_attr = 3;
    RelAttr left_attr;
    Value left_value;

    LOG_INFO("left_exp_names[1] = %s", left_exp_names[0]);
    LOG_INFO("right_exp_names[1] = %s", right_exp_names[1]);

    if (strcmp(left_exp_names[1], "NULL") == 0)
    {
      init_attr_or_value(&left_attr, &left_value, &left_is_attr, left_exp_names[0]);
    }
    else
    {
      condition->exp_num = 0;
      condition_init_expression(condition, left_exp_names, 1);
    }

    int right_is_attr = 3;
    RelAttr right_attr;
    Value right_value;

    if (strcmp(right_exp_names[1], "NULL") == 0)
    {
      init_attr_or_value(&right_attr, &right_value, &right_is_attr, right_exp_names[0]);
    }
    else
    {
      condition->right_exp_num = 0;
      condition_init_expression(condition, right_exp_names, 0);
    }

    condition_init(condition, comp, left_is_attr, &left_attr, &left_value, right_is_attr, &right_attr, &right_value, NULL, NULL);
  }

  void relation_rel_attr_init(RelAttr *relation_attr, const char *rel_attr_name, const char *agg_function_name, int _is_desc)
  {
    char tmp[20];
    int j = 0;

    while (rel_attr_name[j] != '\0' && rel_attr_name[j] != '.')
    {
      tmp[j] = rel_attr_name[j];
      ++j;
    }

    tmp[j] = '\0';
    char *relation_name = nullptr;
    char *attribute_name = nullptr;

    if (rel_attr_name[j] == '.')
    {
      // ID.ID的形式
      relation_name = strdup(tmp);
      ++j;
      int idx = 0;
      while (rel_attr_name[j] != '\0')
      {
        tmp[idx++] = rel_attr_name[j];
        ++j;
      }

      tmp[idx] = '\0';
    }
    attribute_name = strdup(tmp);

    // LOG_INFO("condition: rel_name = %s, cond_name = %s", relation_name, attribute_name);

    if (relation_name != nullptr)
    {
      relation_attr->relation_name = strdup(relation_name);
    }
    else
    {
      relation_attr->relation_name = nullptr;
    }

    relation_attr->attribute_name = strdup(attribute_name);

    if (agg_function_name != nullptr)
    {
      relation_attr->agg_function_name = strdup(agg_function_name);
    }
    else
    {
      relation_attr->agg_function_name = nullptr;
    }

    relation_attr->is_desc = _is_desc;
  }

  void relation_attr_init(RelAttr *relation_attr, const char *relation_name, const char *attribute_name, const char *agg_function_name, int _is_desc)
  {
    // LOG_INFO("relation_attr_init - rel_name = %s, attr_name = %s", relation_name, attribute_name);
    if (relation_name != nullptr)
    {
      relation_attr->relation_name = strdup(relation_name);
    }
    else
    {
      relation_attr->relation_name = nullptr;
    }

    relation_attr->attribute_name = strdup(attribute_name);
    if (agg_function_name != nullptr)
    {
      relation_attr->agg_function_name = strdup(agg_function_name);
    }
    else
    {
      relation_attr->agg_function_name = nullptr;
    }

    relation_attr->is_desc = _is_desc;
  }

  void relation_attr_destroy(RelAttr *relation_attr)
  {
    free(relation_attr->relation_name);
    free(relation_attr->attribute_name);
    free(relation_attr->agg_function_name);

    relation_attr->relation_name = nullptr;
    relation_attr->attribute_name = nullptr;
    relation_attr->agg_function_name = nullptr;
  }

  void value_init_integer(Value *value, int v, int is_null)
  {
    value->type = INTS;
    value->data = malloc(sizeof(v));
    value->is_null = is_null;
    memcpy(value->data, &v, sizeof(v));
  }

  void value_init_float(Value *value, float v, int is_null)
  {
    value->type = FLOATS;
    value->data = malloc(sizeof(v));
    value->is_null = is_null;

    memcpy(value->data, &v, sizeof(v));
  }

  void value_destroy(Value *value)
  {
    value->type = UNDEFINED;
    free(value->data);
    value->data = nullptr;
    value->is_null = false;
  }
  // check date 格式
  bool check_date_format(const char *s)
  {
    std::string str = s;
    std::regex pattern("^\\d{4}-\\d{1,2}-\\d{1,2}");
    if (std::regex_match(str, pattern))
    {
      return true;
    }
    return false;
  }

  int date2num(const char *s)
  {
    // 这个函数写的比较丑，先用着（后期可以考虑使用strtok字符串分割函数进行改写）
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
  // 如果过了date格式则设定t为true且返回对应的int,否则设定t为false且返回0
  int check_date_data_convert(const char *s, int &t)
  {
    int num = date2num(s);
    if (num < 19700101 || num > 20380131)
    {
      t = 0;
    }
    // check 天数
    int days = num % 100;
    if (days > 31 || days < 1)
    {
      t = 0;
    }
    int mons = num % 10000 / 100;
    if (mons > 12 || mons < 1)
    {
      t = 0;
    }
    int years = num / 10000;
    // 处理闰年
    if (mons == 2)
    {
      // years(1970~2038)
      if (years % 4 == 0)
      {
        if (days > 29)
        {
          t = 0;
        }
      }
      else
      {
        if (days > 28)
        {
          t = 0;
        }
      }
    }
    // 处理大小月份
    if (mons == 4 || mons == 6 || mons == 9 || mons == 11)
    {
      if (days > 30)
      {
        t = 0;
      }
    }
    else
    {
      if (days > 31)
      {
        t = 0;
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

void value_init_string_with_text(Value *value, const char *v, int is_null, int len)
  {
    if (is_null)
    {
      value->type = NULLS;
      value->data = strdup(v);
    }
    else if (check_date_format(v))
    {
      LOG_INFO("成功匹配日期格式开始检查具体日期");
      // 转换为数字
      int t = 1;
      int date_num = check_date_data_convert(v, t);
      if (t)
      {
        // LOG_INFO("通过具体日期检测，将输入值作为dates处理");
        value->type = DATES;
        value->data = malloc(sizeof(date_num));
        memcpy(value->data, &date_num, sizeof(date_num));
      }
      else
      {
        value->type = CHARS;
        value->data = strdup(v);
      }
    }
    else
    {
      if (len > 4) {
          LOG_INFO("LEN = %d!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", len);
        value->type = TEXTS;

        const int text_size = 4096;
        char *tmp = new char[text_size];
        memset(tmp, 0, text_size);
        memcpy(tmp, v, len);
        value->data = tmp;

        // value->data = strdup(tmp);
      } else {
        LOG_INFO("v = %s", v);
        value->type = CHARS;
        value->data = strdup(v);
      }

      
    }

    value->is_null = is_null;
  }


  void value_init_string(Value *value, const char *v, int is_null)
  {
    if (is_null)
    {
      value->type = NULLS;
      value->data = strdup(v);
    }
    else if (check_date_format(v))
    {
      LOG_INFO("成功匹配日期格式开始检查具体日期");
      // 转换为数字
      int t=1;
      int date_num = check_date_data_convert(v,t);
      if(t){
        // LOG_INFO("通过具体日期检测，将输入值作为dates处理");
        value->type = DATES;
        value->data = malloc(sizeof(date_num));
        memcpy(value->data, &date_num, sizeof(date_num));
      }
      else
      {
        value->type = CHARS;
        value->data = strdup(v);
      }
    }
    else
    {
        value->type = CHARS;
        value->data = strdup(v);
    }

    value->is_null = is_null;
  }

  void condition_init_expression(Condition *condition, const char **exp_names, int is_left)
  {
    const char **exp_name = exp_names;

    if (is_left)
    {
      for (; strcmp(*exp_name, "NULL") != 0; ++exp_name)
      {
        condition->expression[condition->exp_num++] = strdup(*exp_name);
      }
    }
    else
    {
      for (; strcmp(*exp_name, "NULL") != 0; ++exp_name)
      {
        condition->right_expression[condition->right_exp_num++] = strdup(*exp_name);
      }
    }
  }

  void condition_init(Condition *condition, CompOp comp,
                      int left_is_attr, RelAttr *left_attr, Value *left_value,
                      int right_is_attr, RelAttr *right_attr, Value *right_value,
                      Selects *sub_select, Selects *another_sub_select)
  {
    condition->comp = comp;
    condition->left_is_attr = left_is_attr;

    if (another_sub_select != nullptr)
    {
      // 左侧也是子查询
      condition->another_sub_select = (Selects *)malloc(sizeof(Selects));
      memcpy(condition->another_sub_select, another_sub_select, sizeof(Selects));
      free(another_sub_select);
    }
    else
    {
      if (left_is_attr == 1)
      {
        condition->left_attr = *left_attr;
        LOG_INFO("condition_init - rel_name = %s, attr = %s", left_attr->relation_name, left_attr->attribute_name);
      }
      else if (left_is_attr == 0)
      {
        // check the date format
        condition->left_value = *left_value;
      }
    }

    if (sub_select != nullptr)
    {
      // 碰到子查询
      // 如果只有一个子查询，默认解析到右侧
      // 如果有两个子查询，another是左侧的子查询
      condition->right_is_attr = 2;
      condition->sub_select = (Selects *)malloc(sizeof(Selects));
      memcpy(condition->sub_select, sub_select, sizeof(Selects));
      free(sub_select);

      return;
    }

    condition->right_is_attr = right_is_attr;
    if (right_is_attr == 1)
    {
      condition->right_attr = *right_attr;
    }
    else if (right_is_attr == 0)
    {
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

    free(condition->sub_select);
    condition->sub_select = nullptr;

    for (int i = 0; i < condition->exp_num; ++i)
    {
      free(condition->expression[i]);
      condition->expression[i] = nullptr;
    }
    condition->exp_num = 0;

    for (int i = 0; i < condition->right_exp_num; ++i)
    {
      free(condition->right_expression[i]);
      condition->right_expression[i] = nullptr;
    }
    condition->right_exp_num = 0;
  }

  void attr_info_init(AttrInfo *attr_info, const char *name, AttrType type, size_t length, TrueOrFalse is_nullable)
  {
    attr_info->name = strdup(name);
    attr_info->type = type;
    if (type == AttrType::TEXTS) {
        attr_info->length = 4096;
    } else {
        attr_info->length = length;
    }

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

  void selects_append_attributes(Selects *selects, RelAttr *rel_attrs)
  {
    RelAttr *rel_attr = rel_attrs;
    int flag = rel_attr->is_desc;


    while (flag != 2)
    {
      selects->attributes[selects->attr_num++] = *rel_attr;
      LOG_INFO("selects_append_attributes - rel_name = %s, attr_name = %s", rel_attr->relation_name, rel_attr->attribute_name);
      ++rel_attr;
      flag = rel_attr->is_desc;
    }
  }

  void selects_append_relation(Selects *selects, const char *relation_name)
  {
    selects->relations[selects->relation_num++] = strdup(relation_name);
  }

  void selects_append_relations(Selects *selects, const char **relation_names)
  {
    const char **rel_name = relation_names;

    for (; strcmp(*rel_name, "NULL") != 0; ++rel_name)
    {
      LOG_INFO("selects_append_relations - rel_name = %s", *rel_name);
      selects->relations[selects->relation_num++] = strdup(*rel_name);
    }
  }

  void selects_append_expressions(Selects *selects, const char **exp_names)
  {
    const char **exp_name = exp_names;

    for (; strcmp(*exp_name, "NULL") != 0; ++exp_name)
    {
      LOG_INFO("exp_name = %s, total = %d, num = %d", *exp_name, selects->total_exp, selects->exp_num[selects->total_exp]);
      selects->expression[selects->total_exp][selects->exp_num[selects->total_exp]++] = strdup(*exp_name);
    }

    ++selects->total_exp;
  }

  void print_num(int num)
  {
    LOG_INFO("num = %d", num);
  }

  void print_str(const char *s)
  {
    LOG_INFO("str = %s", s);
  }

  void selects_append_order(Selects *selects, RelAttr *rel_attr)
  {
    selects->order_attrs[selects->order_num++] = *rel_attr;
  }

  void selects_append_groups(Selects *selects, RelAttr *rel_attrs)
  {
    RelAttr *rel_attr = rel_attrs;
    int flag = rel_attr->is_desc;

    while (flag != 2)
    {
      selects->group_attrs[selects->group_num++] = *rel_attr;
      ++rel_attr;
      flag = rel_attr->is_desc;
    }
  }

  void selects_append_conditions(Selects *selects, Condition *conditions)
  {
    // assert(condition_num <= sizeof(selects->conditions) / sizeof(selects->conditions[0]));
    Condition *cond = conditions;

    for (; cond->comp != NO_OP; ++cond)
    {
      selects->conditions[selects->condition_num++] = *cond;
    }

  }

  void selects_append_conditions_with_num(Selects *selects, Condition conditions[], size_t condition_num)
  {
    assert(condition_num <= sizeof(selects->conditions) / sizeof(selects->conditions[0]));
    for (size_t i = 0; i < condition_num; i++)
    {
      selects->conditions[selects->condition_num + i] = conditions[i];
    }
    selects->condition_num += condition_num;
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

    for (size_t i = 0; i < selects->group_num; i++)
    {
      relation_attr_destroy(&selects->group_attrs[i]);
    }
    selects->group_num = 0;

    for (size_t j = 0; j < selects->total_exp; ++j)
    {
      for (size_t i = 0; i < selects->exp_num[j]; i++)
      {
        free(selects->expression[j][i]);
        selects->expression[j][i] = NULL;
      }
      selects->exp_num[j] = 0;
    }
    selects->total_exp = 0;
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

  void deletes_set_conditions(Deletes *deletes, Condition *conditions)
  {
    Condition *cond = conditions;

    for (; cond->comp != NO_OP; ++cond)
    {
      deletes->conditions[deletes->condition_num++] = *cond;
    }
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
                    Value *value)
  {
    updates->relation_name = strdup(relation_name);
    updates->attribute_name = strdup(attribute_name);
    updates->value = *value;
  }

  void updates_init_condition(Updates *updates, Condition *conditions)
  {
    Condition *cond = conditions;

    for (; cond->comp != NO_OP; ++cond)
    {
      updates->conditions[updates->condition_num++] = *cond;
    }
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
  /*
    void create_index_init(CreateIndex *create_index, const char *index_name,
                           const char *relation_name, const char *attr_name, int is_unique)
  */
  void create_index_init(CreateIndex *create_index, const char *index_name,
                         const char *relation_name, int is_unique)
  {
    create_index->index_name = strdup(index_name);
    create_index->relation_name = strdup(relation_name);
    // create_index->attribute_name = strdup(attr_name);
    create_index->is_unique = is_unique;
  }
  void create_index_append_attribute(CreateIndex *create_index, const char *attr_name)
  {
    create_index->attribute_name[create_index->attr_num++] = strdup(attr_name);
  }

  void create_index_destroy(CreateIndex *create_index)
  {
    free(create_index->index_name);
    free(create_index->relation_name);
    free(create_index->attribute_name);

    create_index->index_name = nullptr;
    create_index->relation_name = nullptr;
    for (size_t i = 0; i < create_index->attr_num; i++)
    {
      create_index->attribute_name[i] = nullptr;
    }
    create_index->attr_num = 0;
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
  // LOG_INFO(" the parse result sqln->flag is %d",sqln->flag);
  if (sqln->flag == SCF_ERROR)
  {
    LOG_INFO(" the parse function return SQL_SYNTAX");
    return SQL_SYNTAX;
  }
  else
  {
    return SUCCESS;
  }
}