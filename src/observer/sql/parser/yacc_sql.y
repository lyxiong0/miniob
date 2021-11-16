%{
#ifdef YYDEBUG
  yydebug = 1;
#endif
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.tab.h"
#include "sql/parser/lex.yy.h"
// #include "common/log/log.h" // 包含C++中的头文件

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int yydebug = 1;

typedef struct ParserContext {
  Query * ssql;
  size_t condition_length;
  size_t value_length;
  size_t insert_index;
  size_t rel_length;
  size_t rel_attr_length;
  size_t exp_length;
  size_t exps_select_length;
  size_t exps_select_total;
  size_t tmp_len;

  Value values[MAX_NUM];
  Condition conditions[MAX_NUM];
  char id[MAX_NUM];
  const char *rels[MAX_NUM];
  const char *exps[50];
  const char *exps_for_select[MAX_NUM][50];
  RelAttr rel_attrs[MAX_NUM];
} ParserContext;

// //获取子串
// char *substr(const char *s,int n1,int n2)/*从s中提取下标为n1~n2的字符组成一个新字符串，然后返回这个新串的首地址*/
// {
//   // printf("start call substr on s %s with n1 is %d and n2 is %d \n",s,n1,n2);
//   char *sp = malloc(sizeof(char) * (n2 - n1 + 2));
//   int i, j = 0;
  
//   // printf("now the substr is going \n");
//   for (i = n1; i <= n2; i++) {
//     sp[j++] = s[i];
//   }
//   sp[j] = 0;
//   // printf("now the substr end and new string is %s \n",sp);
//   return sp;
// }

void yyerror(yyscan_t scanner, const char *str)
{
	// 初始化
  ParserContext *context = (ParserContext *)(yyget_extra(scanner));
  query_reset(context->ssql);
  context->ssql->flag = SCF_ERROR;
  context->condition_length = 0;
  context->value_length = 0;
  context->insert_index = 0;
  context->rel_length = 0;
  context->rel_attr_length = 0;
  context->exp_length = 0;
  context->exps_select_length = 0;
  context->tmp_len = 0;
  context->exps_select_total = 0;

  for (size_t i = 0; i < MAX_NUM; i++) {
  	context->ssql->sstr.insertion.value_num[i] = 0;
  }
  context->ssql->sstr.insertion.group_num = 0;
  printf("parse sql failed. error=%s", str);
}

ParserContext *get_context(yyscan_t scanner)
{
  return (ParserContext *)yyget_extra(scanner);
}

#define CONTEXT get_context(scanner)

%}

%define api.pure full
%lex-param { yyscan_t scanner }
%parse-param { void *scanner }

//标识tokens
%token  SEMICOLON
        CREATE
        DROP
        TABLE
        TABLES
        INDEX
        SELECT
        DESC
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        INT_T
        STRING_T
        FLOAT_T
		ORDER
		ASC
		BY
		DATE_T
		UNIQUE
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        WHERE
        AND
        SET
        ON
        LOAD
        DATA
        INFILE
		NULLABLE
		GROUP
		IS
		NOT
        EQ
        LT
        GT
        LE
        GE
        NE
		PLUS
		DIV
		NULL_T
        INNER
        JOIN
		IN
		MINUS
		TEXT_T
        
%union {
  struct _Attr *attr;
  struct _Condition *condition1;
  struct _Value *value1;
  char *string;
  //char *date;
  int number;
  float floats;
  char *position;
  const char **relation;
  struct _RelAttr *relattr1;
  struct _Selects *selnode;
}

%token <number> NUMBER
%token <floats> FLOAT 
%token <string> ID
%token <string> PATH
%token <string> SSS
// %token <string> DATE 
%token <string> STAR
%token <string> STRING_V
%token <string> COUNT 
%token <string> OTHER_FUNCTION_TYPE
%token <string> Column
//非终结符

%type <number> type;
%type <condition1> condition;
%type <value1> value;
%type <number> number;
%type <number> opt_null;
%type <string> opt_star;
%type <condition1> where;
%type <relation> from_rel;
%type <relattr1> select_attr;
%type <selnode> sub_select;
%type <number> comOp;
%type <relattr1> group_by;
%type <relation> expression;
%type <number> sub_comOp;

// 解决value / value RBRACE冲突
// 将更高优先级给shift，即value RBRACE
// %nonassoc LOWER_THAN_BRACE
// %nonassoc RBRACE

%left PLUS '-'
%left STAR DIV // 越靠后优先级越高

%right LOWER_THAN_BRACE RBRACE NOT EQ LT GR LE GE NE IN IS GT SELECT

%%

commands:		//commands or sqls. parser starts here.
    /* empty */ 
    | commands command
    ;

command:
	  select  
	| insert
	| update
	| delete
	| create_table
	| drop_table
	| show_tables
	| desc_table
	| create_index	
	| drop_index
	| sync
	| begin
	| commit
	| rollback
	| load_data
	| help
	| exit
    ;

exit:			
    EXIT SEMICOLON {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    };

help:
    HELP SEMICOLON {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    };

sync:
    SYNC SEMICOLON {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
    ;

begin:
    TRX_BEGIN SEMICOLON {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
    ;

commit:
    TRX_COMMIT SEMICOLON {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
    ;

rollback:
    TRX_ROLLBACK SEMICOLON {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
    ;

drop_table:		/*drop table 语句的语法解析树*/
    DROP TABLE ID SEMICOLON {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, $3);
    };

show_tables:
    SHOW TABLES SEMICOLON {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
    ;

desc_table:
    DESC ID SEMICOLON {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, $2);
    }
    ;

create_index:		/*create index 语句的语法解析树*/
    CREATE INDEX ID ON ID LBRACE Column_def Column_list RBRACE SEMICOLON 
		{
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			// create_index_init(&CONTEXT->ssql->sstr.create_index, $3, $5, $7, 0);
			create_index_init(&CONTEXT->ssql->sstr.create_index, $3, $5, 0);
		}
	| CREATE UNIQUE INDEX ID ON ID LBRACE Column_def Column_list RBRACE SEMICOLON 
		{
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			// create_index_init(&CONTEXT->ssql->sstr.create_index, $4, $6, $8, 1);
			create_index_init(&CONTEXT->ssql->sstr.create_index, $4, $6, 1);
		}
    ;
Column_list:
	/* empty */
	| COMMA Column_def Column_list { }      
	;
Column_def:
	| ID {
		create_index_append_attribute(&CONTEXT->ssql->sstr.create_index, $1);
	}
	;
drop_index:			/*drop index 语句的语法解析树*/
    DROP INDEX ID  SEMICOLON 
		{
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, $3);
		}
    ;
create_table:		/*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON 
		{
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, $3);
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
    ;
	
attr_def_list:
    /* empty */
    | COMMA attr_def attr_def_list {    }
    ;
    
attr_def:
    ID_get type LBRACE number RBRACE opt_null
		{
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, $2, $4, $6);
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name =(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type = $2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length = $4;
			CONTEXT->value_length++;
		}
    |ID_get type opt_null
		{
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, $2, 4, $3);
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			CONTEXT->value_length++;
		}
    ;

opt_null:
	/*empty*/ {
		$$ = ISFALSE; // 默认允许null
	}
	| NOT NULL_T {
		$$ = ISFALSE;
	}
	| NULLABLE {
		$$ = ISTRUE;
	}
	;

number:
	NUMBER {
		$$ = $1;
	}
	;

type: 
	INT_T { 
		$$=INTS; 
		// printf("CREATE 语句语法解析 type 为 INTS\n");
	}
       | STRING_T { 
		   $$=CHARS;
		// printf("CREATE 语句语法解析 type 为 STRING_T\n");
	}
       | FLOAT_T { 
		   $$=FLOATS;
		// printf("CREATE 语句语法解析 type 为 FLOAT_T\n");
	}
	   | DATE_T { 
		   $$=DATES;
		// printf("CREATE 语句语法解析 type 为 DATE_T\n");
	}  | TEXT_T {
	    $$=TEXTS;
	}
       ;
ID_get:
	ID 
	{
		char *temp=$1; 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
	;

	
insert:				/*insert   语句的语法解析树*/
    INSERT INTO ID_get VALUES multi_values SEMICOLON 
	{
			// CONTEXT->values[CONTEXT->value_length++] = *$6;

			CONTEXT->ssql->flag=SCF_INSERT;//"insert";
			// CONTEXT->ssql->sstr.insertion.relation_name = $3;
			// CONTEXT->ssql->sstr.insertion.value_num = CONTEXT->value_length;
			// for(i = 0; i < CONTEXT->value_length; i++){
			// 	CONTEXT->ssql->sstr.insertion.values[i] = CONTEXT->values[i];
      // }	// 到此结束所有插入：存储最后一组、index清零、length清零
	  		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
			CONTEXT->insert_index=0;
			//临时变量清零
      		CONTEXT->value_length=0;
    }
	;
multi_values:
	LBRACE value_with_neg value_list RBRACE {
		// 到此结束一组的插入：存储该组、增加index、value_length清零
		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
		CONTEXT->insert_index++;
		//临时变量清零
      	CONTEXT->value_length=0;
	}
	|multi_values COMMA LBRACE value_with_neg value_list RBRACE {
		// 到此结束一组的插入：存储该组、增加index、value_length清零
		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
		CONTEXT->insert_index++;
		//临时变量清零
      	CONTEXT->value_length=0;
	}
	;
value_list:
    /* empty */
    | COMMA value_with_neg value_list  { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
    ;

value_with_neg:
	value {
		CONTEXT->exp_length = 0;
	}
	| minus NUMBER {
		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], $2 * -1, false);
	}
	| minus FLOAT {
		value_init_float(&CONTEXT->values[CONTEXT->value_length++], $2 * -1.0, false);
	}
	;

value:
    NUMBER{	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], $1, false);
		char exp_name[MAX_NUM];
		sprintf(exp_name, "%d", $1);
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
    |FLOAT{
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], $1, false);
		char exp_name[MAX_NUM];
		sprintf(exp_name, "%f", $1);
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
	|NULL_T {
		// null不需要加双引号，当作字符串插入
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		CONTEXT->exps[CONTEXT->exp_length++] = "NULL";
	}
    |SSS {
        // 没有末位的"\0"
		CONTEXT->exps[CONTEXT->exp_length++] = strdup($1);
		$1 = substr($1, 1, strlen($1)-2);
        // 长度大于4就当作tetx来处理
		value_init_string_with_text(&CONTEXT->values[CONTEXT->value_length++], $1, false, strlen($1));
	}
	;

    
delete:		/*  delete 语句的语法解析树*/
    DELETE FROM ID where SEMICOLON 
	{
		CONTEXT->ssql->flag = SCF_DELETE;//"delete";
		deletes_init_relation(&CONTEXT->ssql->sstr.deletion, $3);
		// 处理where
		if ($4 != NULL) {
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, $4); // where
		}
    }
    ;

update:			/*  update 语句的语法解析树*/
    UPDATE ID SET ID EQ value_with_neg where SEMICOLON
	{
		CONTEXT->ssql->flag = SCF_UPDATE;//"update";
		Value *value = &CONTEXT->values[0];
		updates_init(&CONTEXT->ssql->sstr.update, $2, $4, value);
		if ($7 != NULL) {
			updates_init_condition(&CONTEXT->ssql->sstr.update, $7);
		}
	}
    ;


select:				/*  select 语句的语法解析树*/
    SELECT select_attr from_rel join_list where group_by order_by SEMICOLON
	    {
			CONTEXT->ssql->flag=SCF_SELECT;//"select";

			selects_append_relations(&CONTEXT->ssql->sstr.selection, $3); // from_rel
			if ($5 != NULL) {
				selects_append_conditions(&CONTEXT->ssql->sstr.selection, $5); // where
			}
			selects_append_attributes(&CONTEXT->ssql->sstr.selection, $2); // select_attr
			// group by
			if ($6 != NULL) {
				selects_append_groups(&CONTEXT->ssql->sstr.selection, $6); 
			}

			if (CONTEXT->exps_select_length > 0) {
				for (int i = 0; i < CONTEXT->exps_select_length; ++i) {
					selects_append_expressions(&CONTEXT->ssql->sstr.selection, CONTEXT->exps_for_select[i]); // exp
				}
				CONTEXT->exps_select_length = 0;
			}
	    }
	;
	
select_attr:
    STAR {  // select *
		RelAttr attr;
		relation_attr_init(&attr, NULL, "*", NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++], NULL, "*", NULL, 2);

		$$ = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy($$, CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
    | select_param attr_list { 
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);

		$$ = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy($$, CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
    ;

select_param:
	window_function {
		CONTEXT->exps[CONTEXT->exp_length++] = "NULL";
		// selects_append_expressions(&CONTEXT->ssql->sstr.selection, CONTEXT->exps);
		memcpy(CONTEXT->exps_for_select[CONTEXT->exps_select_length++], CONTEXT->exps, sizeof(const char *) * CONTEXT->exp_length);
		CONTEXT->exp_length = 0;
	}
	| expression {
		// selects_append_expressions(&CONTEXT->ssql->sstr.selection, $1);
		memcpy(CONTEXT->exps_for_select[CONTEXT->exps_select_length++], $1, sizeof(const char *) * CONTEXT->tmp_len);
	}
	;


expression:
	exp exp_list {
		// 1+2+...
		CONTEXT->exps[CONTEXT->exp_length++] = "NULL";
		$$ = ( const char **)malloc(sizeof(const char*) * CONTEXT->exp_length);
		memcpy($$, CONTEXT->exps, sizeof(const char*) * CONTEXT->exp_length);
		CONTEXT->tmp_len = CONTEXT->exp_length;
		CONTEXT->exp_length = 0; // 清空
		// CONTEXT->value_length = 0;
	}
	| exp_list {
		// -1+2+...
		CONTEXT->exps[CONTEXT->exp_length++] = "NULL";
		$$ = ( const char **)malloc(sizeof(const char*) * CONTEXT->exp_length);
		memcpy($$, CONTEXT->exps, sizeof(const char*) * CONTEXT->exp_length);
		CONTEXT->tmp_len = CONTEXT->exp_length;
		CONTEXT->exp_length = 0; // 清空
		// CONTEXT->value_length = 0;
	}
	| lbrace exp exp_list rbrace exp_list {
		// (1+2)+...
		CONTEXT->exps[CONTEXT->exp_length++] = "NULL";
		$$ = ( const char **)malloc(sizeof(const char*) * CONTEXT->exp_length);
		memcpy($$, CONTEXT->exps, sizeof(const char*) * CONTEXT->exp_length);
		CONTEXT->tmp_len = CONTEXT->exp_length;
		CONTEXT->exp_length = 0; // 清空
	}	
	// | lbrace exp_list rbrace exp_list {
	// 	// (-1)+...
	// 	CONTEXT->exps[CONTEXT->exp_length++] = "NULL";
	// 	$$ = ( const char **)malloc(sizeof(const char*) * CONTEXT->exp_length);
	// 	memcpy($$, CONTEXT->exps, sizeof(const char*) * CONTEXT->exp_length);
	// 	CONTEXT->tmp_len = CONTEXT->exp_length;
	// 	CONTEXT->exp_length = 0; // 清空
	// }
	;

exp_list:
	/*empty*/ {}
	| op lbrace exp exp_list rbrace exp_list
	| op exp exp_list
	| op lbrace minus exp rbrace exp_list // * (-1) 
	;

lbrace:
	LBRACE {
		CONTEXT->exps[CONTEXT->exp_length++] = "(";
	}
	;

rbrace:
	RBRACE {
		CONTEXT->exps[CONTEXT->exp_length++] = ")";
	}
	;

exp:
	id_type %prec LOWER_THAN_BRACE
	| value %prec LOWER_THAN_BRACE
	// | lbrace_list id_type %prec LOWER_THAN_BRACE
	// | lbrace_list value %prec LOWER_THAN_BRACE
	// | id_type rbrace_list %prec LOWER_THAN_BRACE
	// | value rbrace_list %prec LOWER_THAN_BRACE
	// | lbrace_list id_type rbrace_list %prec LOWER_THAN_BRACE
	// | lbrace_list value rbrace_list %prec LOWER_THAN_BRACE
	// | lbrace_list minus value rbrace_list %prec LOWER_THAN_BRACE
	;

// lbrace_list:
// 	LBRACE %prec LOWER_THAN_BRACE {
// 		CONTEXT->exps[CONTEXT->exp_length++] = "(";
// 	}
// 	| lbrace_list LBRACE {
// 		CONTEXT->exps[CONTEXT->exp_length++] = "(";
// 	}
// 	;

// rbrace_list:
// 	RBRACE %prec LOWER_THAN_BRACE {
// 		CONTEXT->exps[CONTEXT->exp_length++] = ")";
// 	}
// 	| rbrace_list RBRACE {
// 		CONTEXT->exps[CONTEXT->exp_length++] = ")";
// 	}
// 	;

minus:
	MINUS {
		CONTEXT->exps[CONTEXT->exp_length++] = "-";
	}
	;

op:
	STAR {
		// *
		CONTEXT->exps[CONTEXT->exp_length++] = "*";
	}
	| PLUS {
		// +
		CONTEXT->exps[CONTEXT->exp_length++] = "+";
	}
	| minus
	| DIV {
		// 除法
		CONTEXT->exps[CONTEXT->exp_length++] = "/";
	}
	;

id_type:
	ID{ // select age
		RelAttr attr;
		relation_attr_init(&attr, NULL, $1, NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;

		char exp_name[MAX_NUM];
		sprintf(exp_name, "%s", $1);
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
	| ID DOT ID { // select t1.age
		RelAttr attr;
		relation_attr_init(&attr, $1, $3, NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;

		char exp_name[MAX_NUM];
		sprintf(exp_name, "%s.%s", $1, $3);
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
	| ID DOT STAR{ // select t1.*
		RelAttr attr;
		relation_attr_init(&attr, $1, "*", NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;

		char exp_name[MAX_NUM];
		sprintf(exp_name, "%s.*", $1);
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
	;

attr_list:
    /* empty */
    | COMMA select_param attr_list { }
  	;

join_list:
    /* empty */ 
    | INNER JOIN ID on join_list {
		selects_append_relation(&CONTEXT->ssql->sstr.selection, $3);
    }
    ;


window_function:
	COUNT LBRACE opt_star RBRACE 
	{	// 只有COUNT允许COUNT(*)
		RelAttr attr;
		relation_attr_init(&attr, NULL, $3, $1, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;

		CONTEXT->exps[CONTEXT->exp_length++] = strdup($3);
	}
	| COUNT LBRACE ID DOT ID RBRACE 
	{
		RelAttr attr;
		relation_attr_init(&attr, $3, $5, $1, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;

		char exp_name[MAX_NUM];
		sprintf(exp_name, "%s.%s", $3, $5);
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
	| COUNT LBRACE ID DOT STAR RBRACE 
	{
		RelAttr attr;
		relation_attr_init(&attr, $3, $5, $1, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;

		char exp_name[MAX_NUM];
		sprintf(exp_name, "%s.*", $3);
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
	| OTHER_FUNCTION_TYPE LBRACE ID RBRACE 
	{
		RelAttr attr;
		relation_attr_init(&attr, NULL, $3, $1, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;

		char exp_name[MAX_NUM];
		sprintf(exp_name, "%s", $3);
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
	| OTHER_FUNCTION_TYPE LBRACE ID DOT ID RBRACE 
	{
		RelAttr attr;
		relation_attr_init(&attr, $3, $5, $1, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;

		char exp_name[MAX_NUM];
		sprintf(exp_name, "%s.%s", $3, $5);
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
	| OTHER_FUNCTION_TYPE LBRACE ID DOT STAR RBRACE 
	{
		RelAttr attr;
		relation_attr_init(&attr, $3, "*", $1, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;

		char exp_name[MAX_NUM];
		sprintf(exp_name, "%s.*", $3);
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
	;

opt_star:
	STAR { $$ = $1;}
	| NUMBER {$$ = number_to_str($1);}
	| ID {$$ = $1;}
	;

from_rel:
	FROM ID rel_list {
		CONTEXT->rels[CONTEXT->rel_length++] = $2;
		CONTEXT->rels[CONTEXT->rel_length++] = "NULL";
		$$ = ( const char **)malloc(sizeof(const char*) * CONTEXT->rel_length);
		memcpy($$, CONTEXT->rels, sizeof(const char*) * CONTEXT->rel_length);
		CONTEXT->rel_length = 0;
	}
	;

rel_list:
    /* empty */ {}
    | COMMA ID rel_list {	
		CONTEXT->rels[CONTEXT->rel_length++] = $2;
	}
    ;

where:
    /* empty */ { 
		$$ = NULL; 
		CONTEXT->rel_attr_length = 0;
	}
    | WHERE condition condition_list {	
		RelAttr left_attr;
		relation_attr_init(&left_attr, NULL, "NULL", NULL, 0);
		RelAttr right_attr;
		relation_attr_init(&right_attr, NULL, "NULL", NULL, 0);
		condition_init(&CONTEXT->conditions[CONTEXT->condition_length++], NO_OP, 1, &left_attr, NULL, 1, &right_attr, NULL, NULL, NULL);

		$$ = ( Condition *)malloc(sizeof( Condition) * CONTEXT->condition_length);
		memcpy($$, CONTEXT->conditions, sizeof( Condition) * CONTEXT->condition_length);
		// 每次where结束都清零长度，多个子查询的where不会相互影响
		CONTEXT->condition_length = 0; 
		// 由于select里只有condition涉及到value_length，所以一并在此清零
		CONTEXT->value_length = 0;
		CONTEXT->rel_attr_length = 0;
	}
    ;

on:
    /* empty */ 
    | ON condition condition_list {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
		selects_append_conditions_with_num(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);
		// 每次where结束都清零长度，多个子查询的where不会相互影响
		CONTEXT->condition_length = 0; 
		// 由于select里只有condition涉及到value_length，所以一并在此清零
		CONTEXT->value_length = 0;
	}
    ;

condition_list:
    /* empty */
    | AND condition condition_list {
		// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
	}
    ;

condition:
	expression sub_comOp expression {
		// 左侧表达式，右侧表达式
		Condition condition;
		condition_exp(&condition, $1, $2, $3);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
	| expression sub_comOp sub_select {
		RelAttr left_attr;
		Value left_value;
		int left_is_attr;

		init_attr_or_value(&left_attr, &left_value, &left_is_attr, $1[0]);
		Condition condition;
		condition_init(&condition, $2, left_is_attr, &left_attr, &left_value, 2, NULL, NULL, $3, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
	| sub_select sub_comOp value {
		// 反过来，当作正的解析
		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		if ($2 == GREAT_THAN || $2 == GREAT_EQUAL) {
			condition_init(&condition, $2 - 2, 0, NULL, left_value, 2, NULL, NULL, $1, NULL);
		} else if ($2 == LESS_THAN || $2 == LESS_EQUAL) {
			condition_init(&condition, $2 + 2, 0, NULL, left_value, 2, NULL, NULL, $1, NULL);
		} else {
			condition_init(&condition, $2, 0, NULL, left_value, 2, NULL, NULL, $1, NULL);
		}
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
	| sub_select sub_comOp id_type{
		// 反过来，当作正的解析
		// RelAttr left_attr;
		// relation_attr_init(&left_attr, NULL, $3, NULL, 0);
		RelAttr *left_attr = &CONTEXT->rel_attrs[CONTEXT->rel_attr_length - 1];
		CONTEXT->rel_attr_length = 0;
		CONTEXT->exp_length = 0;

		Condition condition;
		if ($2 == GREAT_THAN || $2 == GREAT_EQUAL) {
			condition_init(&condition, $2 - 2, 1, left_attr, NULL, 2, NULL, NULL, $1, NULL);
		} else if ($2 == LESS_THAN || $2 == LESS_EQUAL) {
			condition_init(&condition, $2 + 2, 1, left_attr, NULL, 2, NULL, NULL, $1, NULL);
		} else {
			condition_init(&condition, $2, 1, left_attr, NULL, 2, NULL, NULL, $1, NULL);
		}
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
	| sub_select sub_comOp sub_select {
		Condition condition;
		condition_init(&condition, $2, 2, NULL, NULL, 2, NULL, NULL, $3, $1);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
    ;

sub_comOp:
	comOp {
		CONTEXT->rel_attr_length = 0;
		$$ = $1;
	}
	;

comOp:
  	  EQ { $$ = 0; }
    | LT { $$ = 3; }
    | GT { $$ = 5; }
    | LE { $$ = 2; }
    | GE { $$ = 4; }
    | NE { $$ = 1; }
	| IN { $$ = 8; }
	| NOT IN { $$ = 9; }
	| IS {$$ = 6;}
	| IS NOT {$$ = 7;}
    ;

sub_select: /* 简单子查询，只包含聚合、比较、in/not in */
	LBRACE SELECT select_attr from_rel where RBRACE {
		$$ = (Selects*)malloc(sizeof(Selects));
		// 结构体malloc，后面要不跟上memcpy要不用memset全部默认初始化
		memset($$, 0, sizeof(Selects));

		selects_append_relations($$, $4); // from_rel
		if ($5 != NULL) {
			selects_append_conditions($$, $5); // where
		}
		selects_append_attributes($$, $3); // select_attr

		if (CONTEXT->exps_select_length > 0) {
			for (int i = 0; i < CONTEXT->exps_select_length; ++i) {
				selects_append_expressions($$, CONTEXT->exps_for_select[i]); // exp
			}
			CONTEXT->exps_select_length = 0;
		}
	}
	;

group_by:
	/*empty*/ {$$ = NULL;}
	| GROUP BY group_list {
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);
		$$ = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy($$, CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
	;

group_list:
	expression{
		selects_append_expressions(&CONTEXT->ssql->sstr.selection, $1);
	}
	| group_list COMMA expression {
		selects_append_expressions(&CONTEXT->ssql->sstr.selection, $3);
	}
	;

// group_attr:
// 	ID {
// 		RelAttr attr;
// 		relation_attr_init(&attr, NULL, $1, NULL, 0);
// 		// selects_append_group(&CONTEXT->ssql->sstr.selection, &attr);
// 		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
// 	}
// 	| ID DOT ID {
// 		RelAttr attr;
// 		relation_attr_init(&attr, $1, $3, NULL, 0);
// 		// selects_append_group(&CONTEXT->ssql->sstr.selection, &attr);
// 		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
// 	}
// 	;

order_by:
	/*empty*/ 
	| ORDER BY sort_list {
	}
	;

sort_list:
	sort_attr {
		// order by A, B, C，实际上加入顺序为C、B、A，方便后面排序
	}
	| sort_list COMMA sort_attr {}
	;
sort_attr:
	ID opt_asc{
		RelAttr attr;
		relation_attr_init(&attr, NULL, $1, NULL, 0);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
	| ID DESC {
		RelAttr attr;
		relation_attr_init(&attr, NULL, $1, NULL, 1);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
	| ID DOT ID opt_asc {
		RelAttr attr;
		relation_attr_init(&attr, $1, $3, NULL, 0);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
	| ID DOT ID DESC {
		RelAttr attr;
		relation_attr_init(&attr, $1, $3, NULL, 1);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
	;
opt_asc:
	/* empty */
	| ASC {}
	;
load_data:
		LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON
		{
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, $7, $4);
		}
		;

%%
//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, Query *sqls){
	ParserContext context;
	memset(&context, 0, sizeof(context));

	yyscan_t scanner;
	yylex_init_extra(&context, &scanner);
	context.ssql = sqls;
	scan_string(s, scanner);
	int result = yyparse(scanner);
	yylex_destroy(scanner);
	return result;
}