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
// Created by wangyunlai.wyl on 2021/6/7.
//

#ifndef __OBSERVER_SQL_PARSER_PARSE_DEFS_H__
#define __OBSERVER_SQL_PARSER_PARSE_DEFS_H__

#include <stddef.h>
#include <memory.h>
#include <stdbool.h>

#define MAX_NUM 20
#define MAX_REL_NAME 20
#define MAX_ATTR_NAME 20
#define MAX_ERROR_MESSAGE 20
#define MAX_DATA 50

//属性结构体
typedef struct _RelAttr
{
  int is_desc;                // 默认采用升序asc，=1降序
  char *relation_name;        // relation name (may be NULL) 表名
  char *attribute_name;       // attribute name              属性名
  char *agg_function_name; // 窗口函数名
} RelAttr;

typedef enum
{
  EQUAL_TO,    //"="     0
  NOT_EQUAL,   //"<>"    1
  LESS_EQUAL,  //"<="    2
  LESS_THAN,   //"<"     3
  GREAT_EQUAL, //">="    4
  GREAT_THAN,  //">"     5
  IS_NULL,
  IS_NOT_NULL,
  IN_SUB,
  NOT_IN,
  NO_OP
} CompOp;

//属性值类型
typedef enum
{
  UNDEFINED,
  CHARS,
  INTS,
  FLOATS,
  DATES,
  TEXTS,
  NULLS
} AttrType;

// true or false
typedef enum
{
  ISTRUE,
  ISFALSE,
  NOTTRUEORFALSE
} TrueOrFalse;

//属性值
typedef struct _Value
{
  AttrType type; // type of value
  void *data;    // value
  int is_null;   // 1:null, 0:not null
} Value;

// Condition conditions[MAX_NUM]需要先定义完整结构体Condition
// 但Selects *sub_select可以只有声明
typedef struct _Selects Selects;

typedef struct _Condition
{
  int left_is_attr;   // 1时，操作符左边是属性名，0时，是属性值
  Value left_value;   // left_is_attr = 0时，操作符左侧的属性值
  RelAttr left_attr;  // left_is_attr = 0时，操作符左侧的属性名
                      
  CompOp comp;        // comparison operator

  int right_is_attr;  // 1时，操作符右边是属性名，0时，是属性值
  RelAttr right_attr; // right-hand side attribute if right_is_attr = TRUE 右边的属性
  Value right_value;  // right-hand side value if right_is_attr = FALSE
  Selects *sub_select; 
  Selects *another_sub_select; // 左侧的复杂子查询
  
  size_t exp_num;
  size_t right_exp_num;
  char *expression[50]; // 表达式
  char *right_expression[50];
} Condition;

// struct of select
// SELECT column_name,column_name
// FROM table_name
// WHERE column_name operator value;
struct _Selects
{
  
  size_t attr_num;               // Length of attrs in Select clause
  RelAttr attributes[MAX_NUM];   // attrs in Select clause

  size_t relation_num;           // Length of relations in For clause
  char *relations[MAX_NUM];      // relations in From clause

  size_t condition_num;          // Length of conditions in Where clause
  Condition conditions[MAX_NUM]; // conditions in Where clause

  size_t order_num;
  RelAttr order_attrs[MAX_NUM]; // order by数组

  size_t group_num;
  RelAttr group_attrs[MAX_NUM]; // group by 数组

  size_t exp_num[MAX_NUM];
  size_t total_exp;
  char *expression[MAX_NUM][50]; // 表达式
};


// struct of insert
typedef struct
{
  char *relation_name;       // Relation to insert into
  size_t value_num[MAX_NUM]; // Length of values
  size_t group_num;
  // Value values[MAX_NUM]; // values to insert
  Value values[MAX_NUM][MAX_NUM]; // values to insert, values[i][j] - 插入的第i组元素第j个值
} Inserts;

// struct of delete
typedef struct
{
  char *relation_name;           // Relation to delete from
  size_t condition_num;          // Length of conditions in Where clause
  Condition conditions[MAX_NUM]; // conditions in Where clause
} Deletes;

// struct of update
typedef struct
{
  char *relation_name;           // Relation to update
  char *attribute_name;          // Attribute to update
  Value value;                   // update value
  size_t condition_num;          // Length of conditions in Where clause
  Condition conditions[MAX_NUM]; // conditions in Where clause
} Updates;

typedef struct
{
  char *name;      // Attribute name
  AttrType type;   // Type of attribute
  size_t length;   // Length of attribute
  int is_nullable; // 是否允许null，默认不允许
} AttrInfo;

// struct of craete_table
typedef struct
{
  char *relation_name;          // Relation name
  size_t attribute_count;       // Length of attribute
  AttrInfo attributes[MAX_NUM]; // attributes
} CreateTable;

// struct of drop_table
typedef struct
{
  char *relation_name; // Relation name
} DropTable;

// struct of create_index
typedef struct
{
  int is_unique;           // unique =1 means unique index
  char *index_name;     // Index name
  char *relation_name;  // Relation name
  // char *attribute_name; // Attribute name
  char *attribute_name[MAX_NUM];  // Attribute name   char指针数组
  size_t attr_num;         // for multi-index 
} CreateIndex;

// struct of  drop_index
typedef struct
{
  const char *index_name; // Index name
} DropIndex;

typedef struct
{
  const char *relation_name;
} DescTable;

typedef struct
{
  const char *relation_name;
  const char *file_name;
} LoadData;

union Queries
{
  Selects selection;
  Inserts insertion;
  Deletes deletion;
  Updates update;
  CreateTable create_table;
  DropTable drop_table;
  CreateIndex create_index;
  DropIndex drop_index;
  DescTable desc_table;
  LoadData load_data;
  char *errors;
};

// 修改yacc中相关数字编码为宏定义
enum SqlCommandFlag
{
  SCF_ERROR = 0,
  SCF_SELECT,
  SCF_INSERT,
  SCF_UPDATE,
  SCF_DELETE,
  SCF_CREATE_TABLE,
  SCF_DROP_TABLE,
  SCF_CREATE_INDEX,
  SCF_DROP_INDEX,
  SCF_SYNC,
  SCF_SHOW_TABLES,
  SCF_DESC_TABLE,
  SCF_BEGIN,
  SCF_COMMIT,
  SCF_ROLLBACK,
  SCF_LOAD_DATA,
  SCF_HELP,
  SCF_EXIT
};
// struct of flag and sql_struct
typedef struct Query
{
  enum SqlCommandFlag flag;
  union Queries sstr;
} Query;

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
  char *substr(const char *s, int n1, int n2);
  void init_attr_or_value(RelAttr *attr, Value *value, int *is_attr, const char *s);
  void condition_exp(Condition *condition, const char **left_exp_names, CompOp comp, const char **right_exp_names);

  void relation_attr_init(RelAttr *relation_attr, const char *relation_name, const char *attribute_name, const char *agg_function_name, int _is_desc);
  void relation_rel_attr_init(RelAttr *relation_attr, const char *rel_attr_name, const char *agg_function_name, int _is_desc);
  void relation_attr_destroy(RelAttr *relation_attr);

  void value_init_integer(Value *value, int v, int is_null);
  void value_init_float(Value *value, float v, int is_null);
  void value_init_string_with_text(Value *value, const char *v, int is_null, int len);
  void value_init_string(Value *value, const char *v, int is_null);
  void value_destroy(Value *value);

  void condition_init(Condition *condition, CompOp comp, int left_is_attr, RelAttr *left_attr, Value *left_value,
                      int right_is_attr, RelAttr *right_attr, Value *right_value, Selects *sub_select, Selects *another_sub_select);
  void condition_destroy(Condition *condition);
  void condition_init_expression(Condition *condition, const char **exp_names, int is_left);

  void attr_info_init(AttrInfo *attr_info, const char *name, AttrType type, size_t length, TrueOrFalse is_nullable);
  void attr_info_destroy(AttrInfo *attr_info);

  void selects_init(Selects *selects, ...);
  void selects_append_attribute(Selects *selects, RelAttr *rel_attr);
  void selects_append_attributes(Selects *selects, RelAttr *rel_attrs);
  void selects_append_relation(Selects *selects, const char *relation_name);
  void selects_append_relations(Selects *selects, const char **relation_names);
  void selects_append_expressions(Selects *selects, const char **exp_names);
  void selects_append_conditions_with_num(Selects *selects, Condition conditions[], size_t condition_num);
  void selects_append_conditions(Selects *selects, Condition *conditions);
  void selects_append_order(Selects *selects, RelAttr *rel_attr);
  void selects_append_groups(Selects *selects, RelAttr *rel_attr);
  void selects_destroy(Selects *selects);
  void print_num(int num);
  void print_str(const char *s);

  void inserts_init(Inserts *inserts, const char *relation_name, Value values[], size_t value_num, size_t index);
  void inserts_destroy(Inserts *inserts);

  void deletes_init_relation(Deletes *deletes, const char *relation_name);
  void deletes_set_conditions(Deletes *deletes, Condition *conditions);
  void deletes_destroy(Deletes *deletes);

  void updates_init(Updates *updates, const char *relation_name, const char *attribute_name, Value *value);
  void updates_init_condition(Updates *updates, Condition *conditions);
  void updates_destroy(Updates *updates);

  void create_table_append_attribute(CreateTable *create_table, AttrInfo *attr_info);
  void create_table_init_name(CreateTable *create_table, const char *relation_name);
  void create_table_destroy(CreateTable *create_table);

  void drop_table_init(DropTable *drop_table, const char *relation_name);
  void drop_table_destroy(DropTable *drop_table);

  void create_index_init(
      CreateIndex *create_index, const char *index_name, const char *relation_name, int is_unique);
  void create_index_append(CreateIndex *create_index,const char *attr_name);
  void create_index_destroy(CreateIndex *create_index);

  void drop_index_init(DropIndex *drop_index, const char *index_name);
  void drop_index_destroy(DropIndex *drop_index);

  void desc_table_init(DescTable *desc_table, const char *relation_name);
  void desc_table_destroy(DescTable *desc_table);

  void load_data_init(LoadData *load_data, const char *relation_name, const char *file_name);
  void load_data_destroy(LoadData *load_data);

  void query_init(Query *query);
  Query *query_create(); // create and init
  void query_reset(Query *query);
  void query_destroy(Query *query); // reset and delete

  void log_err(const char *info);

  const char *number_to_str(int number);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OBSERVER_SQL_PARSER_PARSE_DEFS_H__