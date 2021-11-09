/* A Bison parser, made by GNU Bison 3.7.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "yacc_sql.y"

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

  Value values[MAX_NUM];
  Condition conditions[MAX_NUM];
  char id[MAX_NUM];
  const char *rels[MAX_NUM];
  RelAttr rel_attrs[MAX_NUM];
} ParserContext;

//获取子串
char *substr(const char *s,int n1,int n2)/*从s中提取下标为n1~n2的字符组成一个新字符串，然后返回这个新串的首地址*/
{
  // printf("start call substr on s %s with n1 is %d and n2 is %d \n",s,n1,n2);
  char *sp = malloc(sizeof(char) * (n2 - n1 + 2));
  int i, j = 0;
  
  // printf("now the substr is going \n");
  for (i = n1; i <= n2; i++) {
    sp[j++] = s[i];
  }
  sp[j] = 0;
  // printf("now the substr end and new string is %s \n",sp);
  return sp;
}

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


#line 142 "yacc_sql.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "yacc_sql.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
<<<<<<< HEAD
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_CREATE = 4,                     /* CREATE  */
  YYSYMBOL_DROP = 5,                       /* DROP  */
  YYSYMBOL_TABLE = 6,                      /* TABLE  */
  YYSYMBOL_TABLES = 7,                     /* TABLES  */
  YYSYMBOL_INDEX = 8,                      /* INDEX  */
  YYSYMBOL_SELECT = 9,                     /* SELECT  */
  YYSYMBOL_DESC = 10,                      /* DESC  */
  YYSYMBOL_SHOW = 11,                      /* SHOW  */
  YYSYMBOL_SYNC = 12,                      /* SYNC  */
  YYSYMBOL_INSERT = 13,                    /* INSERT  */
  YYSYMBOL_DELETE = 14,                    /* DELETE  */
  YYSYMBOL_UPDATE = 15,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 16,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 17,                    /* RBRACE  */
  YYSYMBOL_COMMA = 18,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 19,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 20,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 21,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 22,                     /* INT_T  */
  YYSYMBOL_STRING_T = 23,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 24,                   /* FLOAT_T  */
  YYSYMBOL_ORDER = 25,                     /* ORDER  */
  YYSYMBOL_ASC = 26,                       /* ASC  */
  YYSYMBOL_BY = 27,                        /* BY  */
  YYSYMBOL_DATE_T = 28,                    /* DATE_T  */
  YYSYMBOL_UNIQUE = 29,                    /* UNIQUE  */
  YYSYMBOL_HELP = 30,                      /* HELP  */
  YYSYMBOL_EXIT = 31,                      /* EXIT  */
  YYSYMBOL_DOT = 32,                       /* DOT  */
  YYSYMBOL_INTO = 33,                      /* INTO  */
  YYSYMBOL_VALUES = 34,                    /* VALUES  */
  YYSYMBOL_FROM = 35,                      /* FROM  */
  YYSYMBOL_WHERE = 36,                     /* WHERE  */
  YYSYMBOL_AND = 37,                       /* AND  */
  YYSYMBOL_SET = 38,                       /* SET  */
  YYSYMBOL_ON = 39,                        /* ON  */
  YYSYMBOL_LOAD = 40,                      /* LOAD  */
  YYSYMBOL_DATA = 41,                      /* DATA  */
  YYSYMBOL_INFILE = 42,                    /* INFILE  */
  YYSYMBOL_NULLABLE = 43,                  /* NULLABLE  */
  YYSYMBOL_GROUP = 44,                     /* GROUP  */
  YYSYMBOL_IS = 45,                        /* IS  */
  YYSYMBOL_NOT = 46,                       /* NOT  */
  YYSYMBOL_EQ = 47,                        /* EQ  */
  YYSYMBOL_LT = 48,                        /* LT  */
  YYSYMBOL_GT = 49,                        /* GT  */
  YYSYMBOL_LE = 50,                        /* LE  */
  YYSYMBOL_GE = 51,                        /* GE  */
  YYSYMBOL_NE = 52,                        /* NE  */
  YYSYMBOL_NULL_T = 53,                    /* NULL_T  */
  YYSYMBOL_INNER = 54,                     /* INNER  */
  YYSYMBOL_JOIN = 55,                      /* JOIN  */
  YYSYMBOL_IN = 56,                        /* IN  */
  YYSYMBOL_NUMBER = 57,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 58,                     /* FLOAT  */
  YYSYMBOL_ID = 59,                        /* ID  */
  YYSYMBOL_PATH = 60,                      /* PATH  */
  YYSYMBOL_SSS = 61,                       /* SSS  */
  YYSYMBOL_STAR = 62,                      /* STAR  */
  YYSYMBOL_STRING_V = 63,                  /* STRING_V  */
  YYSYMBOL_COUNT = 64,                     /* COUNT  */
  YYSYMBOL_OTHER_FUNCTION_TYPE = 65,       /* OTHER_FUNCTION_TYPE  */
  YYSYMBOL_Column = 66,                    /* Column  */
  YYSYMBOL_YYACCEPT = 67,                  /* $accept  */
  YYSYMBOL_commands = 68,                  /* commands  */
  YYSYMBOL_command = 69,                   /* command  */
  YYSYMBOL_exit = 70,                      /* exit  */
  YYSYMBOL_help = 71,                      /* help  */
  YYSYMBOL_sync = 72,                      /* sync  */
  YYSYMBOL_begin = 73,                     /* begin  */
  YYSYMBOL_commit = 74,                    /* commit  */
  YYSYMBOL_rollback = 75,                  /* rollback  */
  YYSYMBOL_drop_table = 76,                /* drop_table  */
  YYSYMBOL_show_tables = 77,               /* show_tables  */
  YYSYMBOL_desc_table = 78,                /* desc_table  */
  YYSYMBOL_create_index = 79,              /* create_index  */
  YYSYMBOL_Column_list = 80,               /* Column_list  */
  YYSYMBOL_Column_def = 81,                /* Column_def  */
  YYSYMBOL_drop_index = 82,                /* drop_index  */
  YYSYMBOL_create_table = 83,              /* create_table  */
  YYSYMBOL_attr_def_list = 84,             /* attr_def_list  */
  YYSYMBOL_attr_def = 85,                  /* attr_def  */
  YYSYMBOL_opt_null = 86,                  /* opt_null  */
  YYSYMBOL_number = 87,                    /* number  */
  YYSYMBOL_type = 88,                      /* type  */
  YYSYMBOL_ID_get = 89,                    /* ID_get  */
  YYSYMBOL_insert = 90,                    /* insert  */
  YYSYMBOL_multi_values = 91,              /* multi_values  */
  YYSYMBOL_value_list = 92,                /* value_list  */
  YYSYMBOL_value = 93,                     /* value  */
  YYSYMBOL_delete = 94,                    /* delete  */
  YYSYMBOL_update = 95,                    /* update  */
  YYSYMBOL_select = 96,                    /* select  */
  YYSYMBOL_select_attr = 97,               /* select_attr  */
  YYSYMBOL_id_type = 98,                   /* id_type  */
  YYSYMBOL_attr_list = 99,                 /* attr_list  */
  YYSYMBOL_join_list = 100,                /* join_list  */
  YYSYMBOL_window_function = 101,          /* window_function  */
  YYSYMBOL_opt_star = 102,                 /* opt_star  */
  YYSYMBOL_rel_list = 103,                 /* rel_list  */
  YYSYMBOL_where = 104,                    /* where  */
  YYSYMBOL_on = 105,                       /* on  */
  YYSYMBOL_condition_list = 106,           /* condition_list  */
  YYSYMBOL_condition = 107,                /* condition  */
  YYSYMBOL_comOp = 108,                    /* comOp  */
  YYSYMBOL_group_by = 109,                 /* group_by  */
  YYSYMBOL_group_list = 110,               /* group_list  */
  YYSYMBOL_group_attr = 111,               /* group_attr  */
  YYSYMBOL_order_by = 112,                 /* order_by  */
  YYSYMBOL_sort_list = 113,                /* sort_list  */
  YYSYMBOL_sort_attr = 114,                /* sort_attr  */
  YYSYMBOL_opt_asc = 115,                  /* opt_asc  */
  YYSYMBOL_load_data = 116                 /* load_data  */
=======
#line 133 "yacc_sql.y"

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

#line 274 "yacc_sql.tab.c"

>>>>>>> main
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
<<<<<<< HEAD
#define YYLAST   263
=======
#define YYLAST   277
>>>>>>> main

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
<<<<<<< HEAD
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  126
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  260
=======
#define YYNNTS  51
/* YYNRULES -- Number of rules.  */
#define YYNRULES  133
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  272
>>>>>>> main

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   321


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
<<<<<<< HEAD
       0,   168,   168,   170,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   194,   199,   204,   210,   216,   222,   228,   234,   240,
     247,   253,   260,   262,   264,   265,   270,   277,   286,   288,
     292,   303,   317,   320,   323,   329,   332,   336,   340,   344,
     350,   359,   376,   383,   391,   393,   398,   401,   404,   408,
     416,   426,   436,   456,   461,   463,   467,   472,   477,   484,
     486,   487,   490,   492,   498,   504,   510,   516,   522,   529,
     535,   544,   545,   548,   550,   555,   557,   562,   564,   569,
     571,   576,   597,   617,   637,   659,   681,   702,   721,   733,
     745,   756,   767,   776,   788,   789,   790,   791,   792,   793,
     796,   798,   804,   807,   811,   816,   823,   825,   830,   833,
     836,   841,   846,   851,   857,   859,   862
=======
       0,   174,   174,   176,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   200,   205,   210,   216,   222,   228,   234,   240,   246,
     253,   258,   266,   273,   283,   285,   289,   300,   314,   317,
     320,   326,   329,   333,   337,   341,   347,   356,   373,   380,
     388,   390,   395,   398,   401,   405,   413,   425,   438,   455,
     465,   475,   476,   480,   485,   490,   497,   499,   502,   504,
     510,   516,   522,   528,   534,   540,   549,   550,   551,   555,
     565,   566,   572,   576,   593,   595,   605,   607,   613,   625,
     635,   647,   657,   668,   679,   690,   702,   714,   725,   736,
     745,   754,   762,   770,   777,   791,   806,   821,   829,   830,
     831,   832,   833,   834,   835,   836,   840,   858,   859,   868,
     871,   875,   881,   889,   891,   896,   899,   902,   907,   912,
     917,   923,   925,   928
>>>>>>> main
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
<<<<<<< HEAD
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "CREATE",
  "DROP", "TABLE", "TABLES", "INDEX", "SELECT", "DESC", "SHOW", "SYNC",
  "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN",
  "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T", "FLOAT_T", "ORDER",
  "ASC", "BY", "DATE_T", "UNIQUE", "HELP", "EXIT", "DOT", "INTO", "VALUES",
  "FROM", "WHERE", "AND", "SET", "ON", "LOAD", "DATA", "INFILE",
  "NULLABLE", "GROUP", "IS", "NOT", "EQ", "LT", "GT", "LE", "GE", "NE",
  "NULL_T", "INNER", "JOIN", "IN", "NUMBER", "FLOAT", "ID", "PATH", "SSS",
  "STAR", "STRING_V", "COUNT", "OTHER_FUNCTION_TYPE", "Column", "$accept",
  "commands", "command", "exit", "help", "sync", "begin", "commit",
  "rollback", "drop_table", "show_tables", "desc_table", "create_index",
  "Column_list", "Column_def", "drop_index", "create_table",
  "attr_def_list", "attr_def", "opt_null", "number", "type", "ID_get",
  "insert", "multi_values", "value_list", "value", "delete", "update",
  "select", "select_attr", "id_type", "attr_list", "join_list",
  "window_function", "opt_star", "rel_list", "where", "on",
  "condition_list", "condition", "comOp", "group_by", "group_list",
  "group_attr", "order_by", "sort_list", "sort_attr", "opt_asc",
  "load_data", YY_NULLPTR
=======
  "$end", "error", "$undefined", "SEMICOLON", "CREATE", "DROP", "TABLE",
  "TABLES", "INDEX", "SELECT", "DESC", "SHOW", "SYNC", "INSERT", "DELETE",
  "UPDATE", "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN", "TRX_COMMIT",
  "TRX_ROLLBACK", "INT_T", "STRING_T", "FLOAT_T", "ORDER", "ASC", "BY",
  "DATE_T", "UNIQUE", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM",
  "WHERE", "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "NULLABLE",
  "GROUP", "IS", "NOT", "EQ", "LT", "GT", "LE", "GE", "NE", "NULL_T",
  "INNER", "JOIN", "IN", "NUMBER", "FLOAT", "ID", "PATH", "SSS", "STAR",
  "STRING_V", "COUNT", "OTHER_FUNCTION_TYPE", "$accept", "commands",
  "command", "exit", "help", "sync", "begin", "commit", "rollback",
  "drop_table", "show_tables", "desc_table", "create_index", "drop_index",
  "create_table", "attr_def_list", "attr_def", "opt_null", "number",
  "type", "ID_get", "insert", "multi_values", "value_list", "value",
  "delete", "update", "select", "select_attr", "select_param", "id_type",
  "attr_list", "join_list", "window_function", "opt_star", "from_rel",
  "rel_list", "where", "on", "condition_list", "condition", "comOp",
  "sub_select", "group_by", "group_list", "group_attr", "order_by",
  "sort_list", "sort_attr", "opt_asc", "load_data", YY_NULLPTR
>>>>>>> main
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321
};
#endif

#define YYPACT_NINF (-182)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
<<<<<<< HEAD
    -182,     4,  -182,    14,   132,    67,   -52,    59,    71,    58,
      63,    40,   102,   116,   125,   134,   140,   103,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,    86,    87,   139,    89,
      90,   118,  -182,   135,   136,   119,   137,   137,   150,   153,
    -182,    98,    99,   121,  -182,  -182,  -182,  -182,  -182,   120,
     144,   122,   104,   161,   162,   -32,    -2,   107,   108,    77,
    -182,  -182,  -182,  -182,  -182,   138,   133,   109,   110,    98,
     114,   131,  -182,  -182,  -182,  -182,  -182,    15,  -182,   157,
      21,   158,   137,   137,   159,    22,   174,   141,   145,   163,
     111,   164,   123,  -182,    47,  -182,  -182,    68,   124,   130,
    -182,  -182,    50,    28,  -182,  -182,  -182,    20,  -182,    65,
     142,  -182,    50,   179,    98,   169,  -182,  -182,  -182,  -182,
     -10,   128,   173,   175,   176,   177,   178,   158,   143,   133,
     172,  -182,   180,   146,     8,  -182,  -182,  -182,  -182,  -182,
    -182,    29,    31,    36,    22,  -182,   133,   147,   163,   188,
     151,  -182,   148,  -182,  -182,   181,   128,  -182,  -182,  -182,
    -182,  -182,   152,   156,    50,   185,    50,    73,   154,  -182,
    -182,  -182,   160,  -182,   165,  -182,   142,   200,   201,  -182,
    -182,  -182,   192,  -182,   128,   193,   181,   182,   187,   190,
     172,  -182,   172,    32,    43,  -182,  -182,   166,  -182,  -182,
    -182,    30,   181,   209,   199,    22,   130,   167,   191,   214,
    -182,   202,   170,  -182,   195,  -182,  -182,  -182,  -182,  -182,
     217,   142,  -182,   196,   204,  -182,   171,  -182,  -182,  -182,
     183,  -182,  -182,   184,   167,    19,   206,  -182,  -182,  -182,
    -182,  -182,  -182,   186,  -182,   171,    11,  -182,  -182,  -182
=======
    -182,    96,  -182,     8,   106,    69,   -53,    13,     7,    -3,
       6,     9,    72,    79,    87,    91,   101,    35,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,    43,    86,   138,    88,
      97,   128,  -182,   157,   158,   140,   159,  -182,  -182,   173,
     175,  -182,   120,   121,   143,  -182,  -182,  -182,  -182,  -182,
     141,   166,   145,   126,   183,   184,     4,   109,   129,   130,
     136,    99,  -182,  -182,  -182,  -182,   160,   155,   133,   132,
     120,   137,   156,  -182,  -182,  -182,  -182,  -182,   165,  -182,
     181,     0,   182,   144,   155,   159,   185,    -4,   199,   161,
     170,   186,    75,   189,   147,   108,  -182,  -182,   110,   148,
    -182,   150,   167,  -182,   104,    41,   201,  -182,  -182,  -182,
     -23,  -182,    73,   176,   103,  -182,   104,   206,   120,   197,
    -182,  -182,  -182,  -182,    -8,   162,   200,   198,   202,   203,
     205,   182,   178,   191,   204,   207,  -182,   208,    69,   164,
     -35,   171,  -182,  -182,  -182,  -182,  -182,  -182,  -182,     3,
      -1,    20,    -4,  -182,    27,   155,   169,   186,   223,   174,
    -182,   177,  -182,   215,   179,  -182,  -182,  -182,  -182,  -182,
      -4,   136,   180,   209,   230,   104,   217,   104,   140,    92,
     187,  -182,  -182,  -182,  -182,  -182,   188,  -182,   210,  -182,
    -182,   176,   211,  -182,  -182,   232,   234,  -182,  -182,  -182,
     227,  -182,   242,   229,   176,  -182,   216,   231,  -182,   192,
    -182,   207,  -182,   207,   155,    12,    34,  -182,  -182,   193,
    -182,   194,  -182,  -182,    89,  -182,   244,  -182,   195,   180,
      57,   237,  -182,  -182,   233,   167,   212,  -182,   224,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,   213,
    -182,   192,  -182,   240,  -182,   214,     5,  -182,  -182,  -182,
    -182,  -182
>>>>>>> main
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,    20,
      19,    14,    15,    16,    17,     9,    10,    11,    12,    13,
       8,     5,     7,     6,     4,    18,     0,     0,     0,     0,
<<<<<<< HEAD
       0,    66,    63,     0,     0,     0,    69,    69,     0,     0,
      23,     0,     0,     0,    24,    25,    26,    22,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,    29,    28,    50,     0,    85,     0,     0,     0,
       0,     0,    27,    36,    67,    68,    82,     0,    81,     0,
       0,    83,    69,    69,     0,     0,     0,     0,     0,    38,
       0,     0,     0,    75,     0,    74,    78,     0,     0,    72,
      70,    71,     0,     0,    58,    56,    57,     0,    59,     0,
      89,    60,     0,     0,     0,     0,    46,    47,    48,    49,
      42,    34,     0,     0,     0,     0,     0,    83,     0,    85,
      54,    51,     0,     0,     0,   104,   105,   106,   107,   108,
     109,     0,     0,     0,     0,    86,    85,     0,    38,     0,
       0,    44,     0,    41,    35,    32,    34,    76,    77,    79,
      80,    84,     0,   110,     0,     0,     0,     0,     0,    98,
      93,    91,     0,   103,    94,    92,    89,     0,     0,    39,
      37,    45,     0,    43,    34,     0,    32,    87,     0,   116,
      54,    52,    54,     0,     0,    99,   102,     0,    90,    61,
     126,    42,    32,     0,     0,     0,    72,     0,     0,     0,
      55,     0,     0,   100,     0,    95,    96,    40,    33,    30,
       0,    89,    73,   114,   111,   112,     0,    62,    53,   101,
       0,    31,    88,     0,     0,   124,   117,   118,    97,   115,
     113,   121,   125,     0,   120,     0,   124,   119,   123,   122
=======
       0,    63,    59,     0,     0,     0,    66,    61,    62,     0,
       0,    23,     0,     0,     0,    24,    25,    26,    22,    21,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    60,    29,    28,    46,     0,    82,     0,     0,
       0,     0,     0,    27,    32,    64,    65,    77,    78,    76,
       0,     0,    80,     0,    82,    66,     0,     0,     0,     0,
       0,    34,     0,     0,     0,     0,    70,    73,     0,     0,
      79,     0,   117,    67,     0,     0,     0,    54,    52,    53,
       0,    55,     0,    86,     0,    56,     0,     0,     0,     0,
      42,    43,    44,    45,    38,     0,     0,     0,     0,     0,
       0,    80,    84,     0,   123,    50,    47,     0,     0,     0,
       0,     0,   108,   109,   110,   111,   112,   113,   114,     0,
       0,     0,     0,    83,     0,    82,     0,    34,     0,     0,
      40,     0,    37,     0,     0,    71,    72,    74,    75,    81,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,   115,    90,    88,   101,     0,   100,    91,    89,
     103,    86,   105,   104,   107,     0,     0,    35,    33,    41,
       0,    39,     0,     0,    86,    69,   121,   118,   119,     0,
      58,    50,    48,    50,    82,     0,     0,    96,    99,     0,
      87,     0,    57,   133,    38,    30,     0,    85,     0,     0,
     131,   124,   125,    51,     0,   117,     0,    97,     0,    92,
     102,    93,   106,    36,    31,   122,   120,   128,   132,     0,
     127,     0,    49,     0,    98,     0,   131,   126,   116,    94,
     130,   129
>>>>>>> main
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
<<<<<<< HEAD
    -182,  -182,  -182,  -170,  -154,  -182,  -182,    74,   105,    23,
    -182,  -182,   189,  -182,  -182,  -144,  -112,  -182,  -182,  -182,
    -182,   168,   -44,    17,   194,  -182,    94,  -128,  -182,  -181,
    -152,  -118,  -182,  -182,    -9,  -182,  -182,   -19,   -18,  -182
=======
    -182,  -182,  -182,  -182,  -182,    93,   131,    24,  -182,  -182,
     218,  -182,  -182,  -181,  -113,  -182,  -182,  -182,   113,   196,
    -182,   168,    81,  -182,  -182,    76,   125,   -94,  -182,  -167,
    -141,  -119,  -157,    23,  -182,    30,  -182,  -182,    10,    11,
    -182
>>>>>>> main
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,    19,    20,    21,    22,    23,    24,    25,
<<<<<<< HEAD
      26,    27,    28,   195,   165,    29,    30,   125,    99,   163,
     192,   130,   100,    31,   113,   175,   119,    32,    33,    34,
      45,    46,    70,   139,    47,    89,   109,    96,   216,   155,
     120,   151,   199,   234,   235,   219,   246,   247,   254,    35
=======
      26,    27,    28,    29,    30,   129,   101,   172,   210,   134,
     102,    31,   115,   186,   122,    32,    33,    34,    45,    46,
      47,    72,    94,    48,    90,    70,   110,    98,   181,   163,
     123,   159,   124,   144,   217,   218,   184,   241,   242,   260,
      35
>>>>>>> main
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
<<<<<<< HEAD
     140,   153,   186,    71,     2,   208,   160,    48,     3,     4,
     156,   173,   196,     5,     6,     7,     8,     9,    10,    11,
      36,   258,    37,    12,    13,    14,   214,    84,   187,   251,
      85,   141,   103,   161,    15,    16,   162,   252,   106,   181,
     212,   185,   228,    38,    17,   252,   142,   104,   110,   111,
     242,   253,   143,   107,   178,    86,   220,    87,   221,   204,
      88,   179,   200,   231,   202,   144,    49,   145,   146,   147,
     148,   149,   150,   161,    50,   114,   162,   182,   222,   115,
     116,   117,   114,   118,   183,   223,   115,   116,   180,   114,
     118,    51,   225,   115,   116,   184,   114,   118,    52,    53,
     115,   116,   224,   114,   118,    54,   133,   115,   116,   134,
     152,   118,   145,   146,   147,   148,   149,   150,   203,    55,
     145,   146,   147,   148,   149,   150,    41,   135,    56,    42,
     136,    43,    44,   126,   127,   128,    41,    57,    39,   129,
      40,    43,    44,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    72,    68,    69,    73,    74,    76,    77,
      79,    80,    78,    81,    82,    83,    90,    91,    97,    95,
     102,    98,    94,   101,   105,   112,   108,   121,   123,   154,
     131,   124,   132,   137,   138,   157,   159,   164,   122,   166,
     174,   190,   167,   168,   169,   170,   176,   207,   172,   194,
     198,   193,   201,   209,   210,   177,   188,   205,   191,   211,
     213,   197,   229,   206,   217,   218,   230,   237,   236,   238,
     241,   215,   244,   239,   255,   226,   233,   240,   243,   158,
     245,   171,   189,   232,   227,   250,   257,    92,   259,     0,
      75,     0,   248,   249,     0,   256,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93
=======
     112,   145,   195,   161,   200,   164,    49,   204,   169,   149,
      51,   190,   116,   165,    36,   270,    37,   107,   191,   116,
      50,   201,   150,   151,   152,   153,   154,   155,   156,   157,
      52,   258,   108,   158,   230,   170,   116,    38,   171,   214,
     243,    53,   244,   116,   146,   196,   194,   237,   199,   117,
     116,   203,   197,   118,   119,   120,   117,   121,   246,   147,
     118,   119,   193,    85,   121,   247,    86,   257,    54,   250,
     226,   205,   221,   117,   223,    55,    60,   118,   119,   198,
     117,   121,    56,   258,   118,   119,   202,   117,   121,   259,
      57,   118,   119,   248,    58,   121,     2,   130,   131,   132,
       3,     4,    61,   133,    59,     5,     6,     7,     8,     9,
      10,    11,    39,   249,    40,    12,    13,    14,   160,   151,
     152,   153,   154,   155,   156,   157,    15,    16,    41,   158,
     245,    42,   170,    43,    44,   171,    17,   225,   151,   152,
     153,   154,   155,   156,   157,    62,    63,    64,   158,   151,
     152,   153,   154,   155,   156,   157,    65,   117,    41,   158,
      66,   118,   119,    43,    44,   121,    87,   137,    88,   139,
     138,    89,   140,    67,    68,    69,    73,    71,    74,    75,
      77,    78,    80,    79,    81,    82,    83,    84,    91,    92,
      93,    97,    99,   100,    96,   104,   103,   105,   106,   111,
     109,   114,   125,   127,   128,   135,   136,   141,   126,   142,
     148,   143,   166,   162,   168,   175,   174,   180,   182,   176,
     177,   173,   178,   189,   187,   185,   208,   192,   206,   183,
     211,   209,   212,   220,   222,   232,   219,   233,   213,   216,
     227,   228,   229,   231,   234,   235,   236,   254,   238,   239,
     262,   240,   251,   252,   255,   261,   265,   268,   253,   167,
     207,   188,   215,   113,   224,   264,   179,    95,   263,   256,
      76,   267,   266,   269,     0,     0,     0,   271
>>>>>>> main
};

static const yytype_int16 yycheck[] =
{
<<<<<<< HEAD
     112,   119,   154,    47,     0,   186,    16,    59,     4,     5,
     122,   139,   166,     9,    10,    11,    12,    13,    14,    15,
       6,    10,     8,    19,    20,    21,   196,    59,   156,    10,
      62,     3,    17,    43,    30,    31,    46,    26,    17,   151,
     194,   153,   212,    29,    40,    26,    18,    32,    92,    93,
     231,    32,    32,    32,    46,    57,   200,    59,   202,   177,
      62,    53,   174,   215,   176,    45,     7,    47,    48,    49,
      50,    51,    52,    43,     3,    53,    46,    46,    46,    57,
      58,    59,    53,    61,    53,    53,    57,    58,    59,    53,
      61,    33,   204,    57,    58,    59,    53,    61,    35,    59,
      57,    58,    59,    53,    61,     3,    59,    57,    58,    62,
      45,    61,    47,    48,    49,    50,    51,    52,    45,     3,
      47,    48,    49,    50,    51,    52,    59,    59,     3,    62,
      62,    64,    65,    22,    23,    24,    59,     3,     6,    28,
       8,    64,    65,     3,    41,    59,    59,     8,    59,    59,
      32,    16,    16,     3,    35,    18,     3,    59,    59,    38,
      16,    39,    42,    59,     3,     3,    59,    59,    59,    36,
      39,    61,    34,    59,    17,    16,    18,     3,    33,    37,
      16,    18,    59,    59,    54,     6,    17,    59,    47,    16,
      18,     3,    17,    17,    17,    17,    16,    32,    55,    18,
      44,    53,    17,     3,     3,    59,    59,    53,    57,    17,
      17,    59,     3,    53,    27,    25,    17,     3,    27,    17,
       3,    39,    18,    53,    18,    59,    59,    32,    32,   124,
      59,   137,   158,   216,   211,   244,   255,    69,   256,    -1,
      51,    -1,    59,    59,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69
=======
      94,   114,   159,   122,   161,   124,    59,   164,    16,    32,
       3,    46,    16,   126,     6,    10,     8,    17,    53,    16,
       7,   162,    45,    46,    47,    48,    49,    50,    51,    52,
      33,    26,    32,    56,   201,    43,    16,    29,    46,   180,
     221,    35,   223,    16,     3,    46,   159,   214,   161,    53,
      16,   164,    53,    57,    58,    59,    53,    61,    46,    18,
      57,    58,    59,    59,    61,    53,    62,    10,    59,   226,
     189,   165,   185,    53,   187,     3,    41,    57,    58,    59,
      53,    61,     3,    26,    57,    58,    59,    53,    61,    32,
       3,    57,    58,    59,     3,    61,     0,    22,    23,    24,
       4,     5,    59,    28,     3,     9,    10,    11,    12,    13,
      14,    15,     6,   226,     8,    19,    20,    21,    45,    46,
      47,    48,    49,    50,    51,    52,    30,    31,    59,    56,
     224,    62,    43,    64,    65,    46,    40,    45,    46,    47,
      48,    49,    50,    51,    52,    59,     8,    59,    56,    46,
      47,    48,    49,    50,    51,    52,    59,    53,    59,    56,
      32,    57,    58,    64,    65,    61,    57,    59,    59,    59,
      62,    62,    62,    16,    16,    35,     3,    18,     3,    59,
      59,    38,    16,    42,    39,    59,     3,     3,    59,    59,
      54,    36,    59,    61,    34,    39,    59,    32,    17,    55,
      18,    16,     3,    33,    18,    16,    59,    59,    47,    59,
       9,    44,     6,    37,    17,    17,    16,    39,    27,    17,
      17,    59,    17,    59,    16,    18,     3,    56,    59,    25,
      53,    57,    17,     3,    17,     3,    27,     3,    59,    59,
      53,    53,    32,    32,    17,     3,    17,     3,    32,    18,
      17,    59,    59,    59,    59,    18,    32,    17,   234,   128,
     167,   148,   181,    95,   188,    53,   141,    71,   245,   239,
      52,   261,    59,    59,    -1,    -1,    -1,   266
>>>>>>> main
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
<<<<<<< HEAD
       0,    68,     0,     4,     5,     9,    10,    11,    12,    13,
      14,    15,    19,    20,    21,    30,    31,    40,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    82,
      83,    90,    94,    95,    96,   116,     6,     8,    29,     6,
       8,    59,    62,    64,    65,    97,    98,   101,    59,     7,
       3,    33,    35,    59,     3,     3,     3,     3,     3,    41,
      59,    59,     8,    59,    59,    32,    16,    16,    35,    18,
      99,    99,     3,     3,    59,    89,    59,    38,    42,    16,
      39,    59,     3,     3,    59,    62,    57,    59,    62,   102,
      59,    59,    98,   101,    34,    36,   104,    59,    61,    85,
      89,    59,    39,    17,    32,    17,    17,    32,    18,   103,
      99,    99,    16,    91,    53,    57,    58,    59,    61,    93,
     107,     3,    47,    33,    18,    84,    22,    23,    24,    28,
      88,    16,    59,    59,    62,    59,    62,    59,    54,   100,
      93,     3,    18,    32,    45,    47,    48,    49,    50,    51,
      52,   108,    45,   108,    37,   106,    93,     6,    85,    17,
      16,    43,    46,    86,    59,    81,    16,    17,    17,    17,
      17,   103,    55,   104,    18,    92,    16,    59,    46,    53,
      59,    93,    46,    53,    59,    93,   107,   104,    59,    84,
       3,    57,    87,    53,    18,    80,    81,    59,    44,   109,
      93,    17,    93,    45,   108,    53,    53,    32,   106,     3,
       3,    17,    81,    17,    80,    39,   105,    27,    25,   112,
      92,    92,    46,    53,    59,    93,    59,    86,    80,     3,
      17,   107,   100,    59,   110,   111,    27,     3,    17,    53,
      32,     3,   106,    32,    18,    59,   113,   114,    59,    59,
     111,    10,    26,    32,   115,    18,    59,   114,    10,   115
=======
       0,    67,     0,     4,     5,     9,    10,    11,    12,    13,
      14,    15,    19,    20,    21,    30,    31,    40,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    87,    91,    92,    93,   116,     6,     8,    29,     6,
       8,    59,    62,    64,    65,    94,    95,    96,    99,    59,
       7,     3,    33,    35,    59,     3,     3,     3,     3,     3,
      41,    59,    59,     8,    59,    59,    32,    16,    16,    35,
     101,    18,    97,     3,     3,    59,    86,    59,    38,    42,
      16,    39,    59,     3,     3,    59,    62,    57,    59,    62,
     100,    59,    59,    54,    98,    95,    34,    36,   103,    59,
      61,    82,    86,    59,    39,    32,    17,    17,    32,    18,
     102,    55,   103,    97,    16,    88,    16,    53,    57,    58,
      59,    61,    90,   106,   108,     3,    47,    33,    18,    81,
      22,    23,    24,    28,    85,    16,    59,    59,    62,    59,
      62,    59,    59,    44,   109,    90,     3,    18,     9,    32,
      45,    46,    47,    48,    49,    50,    51,    52,    56,   107,
      45,   107,    37,   105,   107,    90,     6,    82,    17,    16,
      43,    46,    83,    59,    16,    17,    17,    17,    17,   102,
      39,   104,    27,    25,   112,    18,    89,    16,    94,    59,
      46,    53,    56,    59,    90,   108,    46,    53,    59,    90,
     108,   106,    59,    90,   108,   103,    59,    81,     3,    57,
      84,    53,    17,    59,   106,    98,    59,   110,   111,    27,
       3,    90,    17,    90,   101,    45,   107,    53,    53,    32,
     105,    32,     3,     3,    17,     3,    17,   105,    32,    18,
      59,   113,   114,    89,    89,   103,    46,    53,    59,    90,
     108,    59,    59,    83,     3,    59,   111,    10,    26,    32,
     115,    18,    17,   109,    53,    32,    59,   114,    17,    59,
      10,   115
>>>>>>> main
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
<<<<<<< HEAD
       0,    67,    68,    68,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    79,    80,    80,    81,    81,    82,    83,    84,    84,
      85,    85,    86,    86,    86,    87,    88,    88,    88,    88,
      89,    90,    91,    91,    92,    92,    93,    93,    93,    93,
      94,    95,    96,    97,    97,    97,    98,    98,    98,    99,
      99,    99,   100,   100,   101,   101,   101,   101,   101,   101,
     101,   102,   102,   103,   103,   104,   104,   105,   105,   106,
     106,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   108,   108,   108,   108,   108,   108,
     109,   109,   110,   110,   111,   111,   112,   112,   113,   113,
     114,   114,   114,   114,   115,   115,   116
=======
       0,    66,    67,    67,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    78,    79,    80,    81,    81,    82,    82,    83,    83,
      83,    84,    85,    85,    85,    85,    86,    87,    88,    88,
      89,    89,    90,    90,    90,    90,    91,    92,    93,    94,
      94,    95,    95,    96,    96,    96,    97,    97,    98,    98,
      99,    99,    99,    99,    99,    99,   100,   100,   100,   101,
     102,   102,   103,   103,   104,   104,   105,   105,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   107,   107,
     107,   107,   107,   107,   107,   107,   108,   109,   109,   110,
     110,   111,   111,   112,   112,   113,   113,   114,   114,   114,
     114,   115,   115,   116
>>>>>>> main
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
<<<<<<< HEAD
      10,    11,     0,     3,     0,     1,     4,     8,     0,     3,
       6,     3,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     6,     4,     6,     0,     3,     1,     1,     1,     1,
       5,     8,    10,     1,     2,     2,     1,     3,     3,     0,
       3,     3,     0,     5,     4,     4,     6,     6,     4,     6,
       6,     1,     1,     0,     3,     0,     3,     0,     3,     0,
       3,     3,     3,     3,     3,     5,     5,     7,     3,     4,
       5,     6,     4,     3,     1,     1,     1,     1,     1,     1,
       0,     3,     1,     3,     1,     3,     0,     3,     1,     3,
       2,     2,     4,     4,     0,     1,     8
=======
       9,    10,     4,     8,     0,     3,     6,     3,     0,     2,
       1,     1,     1,     1,     1,     1,     1,     6,     4,     6,
       0,     3,     1,     1,     1,     1,     5,     8,     8,     1,
       2,     1,     1,     1,     3,     3,     0,     3,     0,     5,
       4,     6,     6,     4,     6,     6,     1,     1,     1,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     3,     3,
       3,     3,     5,     5,     7,     3,     4,     5,     6,     4,
       3,     3,     5,     3,     3,     3,     5,     3,     1,     1,
       1,     1,     1,     1,     1,     2,     7,     0,     3,     1,
       3,     1,     3,     0,     3,     1,     3,     2,     2,     4,
       4,     0,     1,     8
>>>>>>> main
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void *scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void *scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, void *scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, void *scanner)
{
  YYUSE (yyvaluep);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
<<<<<<< HEAD
  case 21: /* exit: EXIT SEMICOLON  */
#line 194 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1437 "yacc_sql.tab.c"
    break;

  case 22: /* help: HELP SEMICOLON  */
#line 199 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1445 "yacc_sql.tab.c"
    break;

  case 23: /* sync: SYNC SEMICOLON  */
#line 204 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1453 "yacc_sql.tab.c"
    break;

  case 24: /* begin: TRX_BEGIN SEMICOLON  */
#line 210 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1461 "yacc_sql.tab.c"
    break;

  case 25: /* commit: TRX_COMMIT SEMICOLON  */
#line 216 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1469 "yacc_sql.tab.c"
    break;

  case 26: /* rollback: TRX_ROLLBACK SEMICOLON  */
#line 222 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1477 "yacc_sql.tab.c"
    break;

  case 27: /* drop_table: DROP TABLE ID SEMICOLON  */
#line 228 "yacc_sql.y"
=======
  case 21:
#line 200 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1656 "yacc_sql.tab.c"
    break;

  case 22:
#line 205 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1664 "yacc_sql.tab.c"
    break;

  case 23:
#line 210 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1672 "yacc_sql.tab.c"
    break;

  case 24:
#line 216 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1680 "yacc_sql.tab.c"
    break;

  case 25:
#line 222 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1688 "yacc_sql.tab.c"
    break;

  case 26:
#line 228 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1696 "yacc_sql.tab.c"
    break;

  case 27:
#line 234 "yacc_sql.y"
>>>>>>> main
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
<<<<<<< HEAD
#line 1486 "yacc_sql.tab.c"
    break;

  case 28: /* show_tables: SHOW TABLES SEMICOLON  */
#line 234 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1494 "yacc_sql.tab.c"
    break;

  case 29: /* desc_table: DESC ID SEMICOLON  */
#line 240 "yacc_sql.y"
=======
#line 1705 "yacc_sql.tab.c"
    break;

  case 28:
#line 240 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1713 "yacc_sql.tab.c"
    break;

  case 29:
#line 246 "yacc_sql.y"
>>>>>>> main
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
<<<<<<< HEAD
#line 1503 "yacc_sql.tab.c"
    break;

  case 30: /* create_index: CREATE INDEX ID ON ID LBRACE Column_def Column_list RBRACE SEMICOLON  */
#line 248 "yacc_sql.y"
=======
#line 1722 "yacc_sql.tab.c"
    break;

  case 30:
#line 254 "yacc_sql.y"
>>>>>>> main
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			// create_index_init(&CONTEXT->ssql->sstr.create_index, $3, $5, $7, 0);
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-7].string), (yyvsp[-5].string), 0);
		}
<<<<<<< HEAD
#line 1513 "yacc_sql.tab.c"
    break;

  case 31: /* create_index: CREATE UNIQUE INDEX ID ON ID LBRACE Column_def Column_list RBRACE SEMICOLON  */
#line 254 "yacc_sql.y"
=======
#line 1731 "yacc_sql.tab.c"
    break;

  case 31:
#line 259 "yacc_sql.y"
>>>>>>> main
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			// create_index_init(&CONTEXT->ssql->sstr.create_index, $4, $6, $8, 1);
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-7].string), (yyvsp[-5].string), 1);
		}
<<<<<<< HEAD
#line 1523 "yacc_sql.tab.c"
    break;

  case 33: /* Column_list: COMMA Column_def Column_list  */
#line 262 "yacc_sql.y"
                                       { }
#line 1529 "yacc_sql.tab.c"
    break;

  case 35: /* Column_def: ID  */
#line 265 "yacc_sql.y"
             {
		create_index_append_attribute(&CONTEXT->ssql->sstr.create_index, (yyvsp[0].string));
	}
#line 1537 "yacc_sql.tab.c"
    break;

  case 36: /* drop_index: DROP INDEX ID SEMICOLON  */
#line 271 "yacc_sql.y"
=======
#line 1740 "yacc_sql.tab.c"
    break;

  case 32:
#line 267 "yacc_sql.y"
>>>>>>> main
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
<<<<<<< HEAD
#line 1546 "yacc_sql.tab.c"
    break;

  case 37: /* create_table: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON  */
#line 278 "yacc_sql.y"
=======
#line 1749 "yacc_sql.tab.c"
    break;

  case 33:
#line 274 "yacc_sql.y"
>>>>>>> main
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
<<<<<<< HEAD
#line 1558 "yacc_sql.tab.c"
    break;

  case 39: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 288 "yacc_sql.y"
                                   {    }
#line 1564 "yacc_sql.tab.c"
    break;

  case 40: /* attr_def: ID_get type LBRACE number RBRACE opt_null  */
#line 293 "yacc_sql.y"
=======
#line 1761 "yacc_sql.tab.c"
    break;

  case 35:
#line 285 "yacc_sql.y"
                                   {    }
#line 1767 "yacc_sql.tab.c"
    break;

  case 36:
#line 290 "yacc_sql.y"
>>>>>>> main
                {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-4].number), (yyvsp[-2].number), (yyvsp[0].number));
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name =(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type = $2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length = $4;
			CONTEXT->value_length++;
		}
<<<<<<< HEAD
#line 1579 "yacc_sql.tab.c"
    break;

  case 41: /* attr_def: ID_get type opt_null  */
#line 304 "yacc_sql.y"
=======
#line 1782 "yacc_sql.tab.c"
    break;

  case 37:
#line 301 "yacc_sql.y"
>>>>>>> main
                {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-1].number), 4, (yyvsp[0].number));
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name=(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type=$2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length=4; // default attribute length
			CONTEXT->value_length++;
		}
<<<<<<< HEAD
#line 1594 "yacc_sql.tab.c"
    break;

  case 42: /* opt_null: %empty  */
#line 317 "yacc_sql.y"
                  {
		(yyval.number) = ISFALSE; // 默认允许null
	}
#line 1602 "yacc_sql.tab.c"
    break;

  case 43: /* opt_null: NOT NULL_T  */
#line 320 "yacc_sql.y"
                     {
		(yyval.number) = ISFALSE;
	}
#line 1610 "yacc_sql.tab.c"
    break;

  case 44: /* opt_null: NULLABLE  */
#line 323 "yacc_sql.y"
                   {
		(yyval.number) = ISTRUE;
	}
#line 1618 "yacc_sql.tab.c"
    break;

  case 45: /* number: NUMBER  */
#line 329 "yacc_sql.y"
                       {(yyval.number) = (yyvsp[0].number);}
#line 1624 "yacc_sql.tab.c"
    break;

  case 46: /* type: INT_T  */
#line 332 "yacc_sql.y"
=======
#line 1797 "yacc_sql.tab.c"
    break;

  case 38:
#line 314 "yacc_sql.y"
                  {
		(yyval.number) = ISFALSE; // 默认允许null
	}
#line 1805 "yacc_sql.tab.c"
    break;

  case 39:
#line 317 "yacc_sql.y"
                     {
		(yyval.number) = ISFALSE;
	}
#line 1813 "yacc_sql.tab.c"
    break;

  case 40:
#line 320 "yacc_sql.y"
                   {
		(yyval.number) = ISTRUE;
	}
#line 1821 "yacc_sql.tab.c"
    break;

  case 41:
#line 326 "yacc_sql.y"
                       {(yyval.number) = (yyvsp[0].number);}
#line 1827 "yacc_sql.tab.c"
    break;

  case 42:
#line 329 "yacc_sql.y"
>>>>>>> main
              { 
		(yyval.number)=INTS; 
		// printf("CREATE 语句语法解析 type 为 INTS\n");
	}
<<<<<<< HEAD
#line 1633 "yacc_sql.tab.c"
    break;

  case 47: /* type: STRING_T  */
#line 336 "yacc_sql.y"
=======
#line 1836 "yacc_sql.tab.c"
    break;

  case 43:
#line 333 "yacc_sql.y"
>>>>>>> main
                  { 
		   (yyval.number)=CHARS;
		// printf("CREATE 语句语法解析 type 为 STRING_T\n");
	}
<<<<<<< HEAD
#line 1642 "yacc_sql.tab.c"
    break;

  case 48: /* type: FLOAT_T  */
#line 340 "yacc_sql.y"
=======
#line 1845 "yacc_sql.tab.c"
    break;

  case 44:
#line 337 "yacc_sql.y"
>>>>>>> main
                 { 
		   (yyval.number)=FLOATS;
		// printf("CREATE 语句语法解析 type 为 FLOAT_T\n");
	}
<<<<<<< HEAD
#line 1651 "yacc_sql.tab.c"
    break;

  case 49: /* type: DATE_T  */
#line 344 "yacc_sql.y"
=======
#line 1854 "yacc_sql.tab.c"
    break;

  case 45:
#line 341 "yacc_sql.y"
>>>>>>> main
                    { 
		   (yyval.number)=DATES;
		// printf("CREATE 语句语法解析 type 为 DATE_T\n");
	}
<<<<<<< HEAD
#line 1660 "yacc_sql.tab.c"
    break;

  case 50: /* ID_get: ID  */
#line 351 "yacc_sql.y"
=======
#line 1863 "yacc_sql.tab.c"
    break;

  case 46:
#line 348 "yacc_sql.y"
>>>>>>> main
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
<<<<<<< HEAD
#line 1669 "yacc_sql.tab.c"
    break;

  case 51: /* insert: INSERT INTO ID_get VALUES multi_values SEMICOLON  */
#line 360 "yacc_sql.y"
=======
#line 1872 "yacc_sql.tab.c"
    break;

  case 47:
#line 357 "yacc_sql.y"
>>>>>>> main
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
<<<<<<< HEAD
#line 1688 "yacc_sql.tab.c"
    break;

  case 52: /* multi_values: LBRACE value value_list RBRACE  */
#line 376 "yacc_sql.y"
=======
#line 1891 "yacc_sql.tab.c"
    break;

  case 48:
#line 373 "yacc_sql.y"
>>>>>>> main
                                       {
		// 到此结束一组的插入：存储该组、增加index、value_length清零
		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
		CONTEXT->insert_index++;
		//临时变量清零
      	CONTEXT->value_length=0;
	}
<<<<<<< HEAD
#line 1700 "yacc_sql.tab.c"
    break;

  case 53: /* multi_values: multi_values COMMA LBRACE value value_list RBRACE  */
#line 383 "yacc_sql.y"
=======
#line 1903 "yacc_sql.tab.c"
    break;

  case 49:
#line 380 "yacc_sql.y"
>>>>>>> main
                                                           {
		// 到此结束一组的插入：存储该组、增加index、value_length清零
		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
		CONTEXT->insert_index++;
		//临时变量清零
      	CONTEXT->value_length=0;
	}
<<<<<<< HEAD
#line 1712 "yacc_sql.tab.c"
    break;

  case 55: /* value_list: COMMA value value_list  */
#line 393 "yacc_sql.y"
                              { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1720 "yacc_sql.tab.c"
    break;

  case 56: /* value: NUMBER  */
#line 398 "yacc_sql.y"
          {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number), false);
	}
#line 1728 "yacc_sql.tab.c"
    break;

  case 57: /* value: FLOAT  */
#line 401 "yacc_sql.y"
          {
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].floats), false);
	}
#line 1736 "yacc_sql.tab.c"
    break;

  case 58: /* value: NULL_T  */
#line 404 "yacc_sql.y"
=======
#line 1915 "yacc_sql.tab.c"
    break;

  case 51:
#line 390 "yacc_sql.y"
                              { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1923 "yacc_sql.tab.c"
    break;

  case 52:
#line 395 "yacc_sql.y"
          {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number), false);
	}
#line 1931 "yacc_sql.tab.c"
    break;

  case 53:
#line 398 "yacc_sql.y"
          {
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].floats), false);
	}
#line 1939 "yacc_sql.tab.c"
    break;

  case 54:
#line 401 "yacc_sql.y"
>>>>>>> main
                {
		// null不需要加双引号，当作字符串插入
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
	}
<<<<<<< HEAD
#line 1745 "yacc_sql.tab.c"
    break;

  case 59: /* value: SSS  */
#line 408 "yacc_sql.y"
=======
#line 1948 "yacc_sql.tab.c"
    break;

  case 55:
#line 405 "yacc_sql.y"
>>>>>>> main
         {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string), false);
		}
<<<<<<< HEAD
#line 1754 "yacc_sql.tab.c"
    break;

  case 60: /* delete: DELETE FROM ID where SEMICOLON  */
#line 417 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;	
    }
#line 1766 "yacc_sql.tab.c"
    break;

  case 61: /* update: UPDATE ID SET ID EQ value where SEMICOLON  */
#line 427 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
		}
#line 1778 "yacc_sql.tab.c"
    break;

  case 62: /* select: SELECT select_attr FROM ID rel_list join_list where group_by order_by SEMICOLON  */
#line 437 "yacc_sql.y"
=======
#line 1957 "yacc_sql.tab.c"
    break;

  case 56:
#line 414 "yacc_sql.y"
        {
		CONTEXT->ssql->flag = SCF_DELETE;//"delete";
		deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
		// 处理where
		if ((yyvsp[-1].condition1) != NULL) {
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, (yyvsp[-1].condition1)); // where
		}
    }
#line 1970 "yacc_sql.tab.c"
    break;

  case 57:
#line 426 "yacc_sql.y"
        {
		CONTEXT->ssql->flag = SCF_UPDATE;//"update";
		Value *value = &CONTEXT->values[0];
		updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value);
		if ((yyvsp[-1].condition1) != NULL) {
			updates_init_condition(&CONTEXT->ssql->sstr.update, (yyvsp[-1].condition1));
		}
	}
#line 1983 "yacc_sql.tab.c"
    break;

  case 58:
#line 439 "yacc_sql.y"
>>>>>>> main
            {
			CONTEXT->ssql->flag=SCF_SELECT;//"select";

			selects_append_relations(&CONTEXT->ssql->sstr.selection, (yyvsp[-5].relation)); // from_rel
			if ((yyvsp[-3].condition1) != NULL) {
				selects_append_conditions(&CONTEXT->ssql->sstr.selection, (yyvsp[-3].condition1)); // where
			}
			selects_append_attributes(&CONTEXT->ssql->sstr.selection, (yyvsp[-6].relattr1)); // select_attr
			// group by
			if ((yyvsp[-2].relattr1) != NULL) {
				selects_append_groups(&CONTEXT->ssql->sstr.selection, (yyvsp[-2].relattr1)); 
			}
	    }
<<<<<<< HEAD
#line 1800 "yacc_sql.tab.c"
    break;

  case 63: /* select_attr: STAR  */
#line 456 "yacc_sql.y"
         {  // select *
			RelAttr attr;
			relation_attr_init(&attr, NULL, "*", NULL, 0);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1810 "yacc_sql.tab.c"
    break;

  case 64: /* select_attr: id_type attr_list  */
#line 461 "yacc_sql.y"
                        { 
	}
#line 1817 "yacc_sql.tab.c"
    break;

  case 65: /* select_attr: window_function attr_list  */
#line 463 "yacc_sql.y"
                                    { }
#line 1823 "yacc_sql.tab.c"
    break;

  case 66: /* id_type: ID  */
#line 467 "yacc_sql.y"
          { // select age
			RelAttr attr;
			relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL, 0);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1833 "yacc_sql.tab.c"
    break;

  case 67: /* id_type: ID DOT ID  */
#line 472 "yacc_sql.y"
                    { // select t1.age
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1843 "yacc_sql.tab.c"
    break;

  case 68: /* id_type: ID DOT STAR  */
#line 477 "yacc_sql.y"
                     { // select t1.*
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-2].string), "*", NULL, 0);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1853 "yacc_sql.tab.c"
    break;

  case 70: /* attr_list: COMMA id_type attr_list  */
#line 486 "yacc_sql.y"
                              { }
#line 1859 "yacc_sql.tab.c"
    break;

  case 71: /* attr_list: COMMA window_function attr_list  */
#line 487 "yacc_sql.y"
                                          { }
#line 1865 "yacc_sql.tab.c"
    break;

  case 73: /* join_list: INNER JOIN ID on join_list  */
#line 492 "yacc_sql.y"
                                {
        selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-2].string));
    }
#line 1873 "yacc_sql.tab.c"
    break;

  case 74: /* window_function: COUNT LBRACE opt_star RBRACE  */
#line 499 "yacc_sql.y"
=======
#line 2001 "yacc_sql.tab.c"
    break;

  case 59:
#line 455 "yacc_sql.y"
         {  // select *
		RelAttr attr;
		relation_attr_init(&attr, NULL, "*", NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);

		(yyval.relattr1) = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy((yyval.relattr1), CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
#line 2016 "yacc_sql.tab.c"
    break;

  case 60:
#line 465 "yacc_sql.y"
                             { 
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);

		(yyval.relattr1) = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy((yyval.relattr1), CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
#line 2028 "yacc_sql.tab.c"
    break;

  case 61:
#line 475 "yacc_sql.y"
                {}
#line 2034 "yacc_sql.tab.c"
    break;

  case 62:
#line 476 "yacc_sql.y"
                          {}
#line 2040 "yacc_sql.tab.c"
    break;

  case 63:
#line 480 "yacc_sql.y"
          { // select age
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2050 "yacc_sql.tab.c"
    break;

  case 64:
#line 485 "yacc_sql.y"
                    { // select t1.age
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2060 "yacc_sql.tab.c"
    break;

  case 65:
#line 490 "yacc_sql.y"
                     { // select t1.*
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-2].string), "*", NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2070 "yacc_sql.tab.c"
    break;

  case 67:
#line 499 "yacc_sql.y"
                                   { }
#line 2076 "yacc_sql.tab.c"
    break;

  case 69:
#line 504 "yacc_sql.y"
                                 {
		selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-2].string));
    }
#line 2084 "yacc_sql.tab.c"
    break;

  case 70:
#line 511 "yacc_sql.y"
>>>>>>> main
        {	// 只有COUNT允许COUNT(*)
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
<<<<<<< HEAD
#line 1883 "yacc_sql.tab.c"
    break;

  case 75: /* window_function: COUNT LBRACE ID RBRACE  */
#line 505 "yacc_sql.y"
=======
#line 2094 "yacc_sql.tab.c"
    break;

  case 71:
#line 517 "yacc_sql.y"
>>>>>>> main
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
<<<<<<< HEAD
#line 1893 "yacc_sql.tab.c"
    break;

  case 76: /* window_function: COUNT LBRACE ID DOT ID RBRACE  */
#line 511 "yacc_sql.y"
=======
#line 2104 "yacc_sql.tab.c"
    break;

  case 72:
#line 523 "yacc_sql.y"
>>>>>>> main
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
<<<<<<< HEAD
#line 1903 "yacc_sql.tab.c"
    break;

  case 77: /* window_function: COUNT LBRACE ID DOT STAR RBRACE  */
#line 517 "yacc_sql.y"
=======
#line 2114 "yacc_sql.tab.c"
    break;

  case 73:
#line 529 "yacc_sql.y"
>>>>>>> main
        {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
<<<<<<< HEAD
#line 1913 "yacc_sql.tab.c"
    break;

  case 78: /* window_function: OTHER_FUNCTION_TYPE LBRACE ID RBRACE  */
#line 523 "yacc_sql.y"
=======
#line 2124 "yacc_sql.tab.c"
    break;

  case 74:
#line 535 "yacc_sql.y"
>>>>>>> main
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
<<<<<<< HEAD
#line 1924 "yacc_sql.tab.c"
    break;

  case 79: /* window_function: OTHER_FUNCTION_TYPE LBRACE ID DOT ID RBRACE  */
#line 530 "yacc_sql.y"
=======
#line 2134 "yacc_sql.tab.c"
    break;

  case 75:
#line 541 "yacc_sql.y"
>>>>>>> main
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), "*", (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
<<<<<<< HEAD
#line 1934 "yacc_sql.tab.c"
    break;

  case 80: /* window_function: OTHER_FUNCTION_TYPE LBRACE ID DOT STAR RBRACE  */
#line 536 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), "*", (yyvsp[-5].string), 0);
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 1944 "yacc_sql.tab.c"
    break;

  case 81: /* opt_star: STAR  */
#line 544 "yacc_sql.y"
             { (yyval.string) = (yyvsp[0].string);}
#line 1950 "yacc_sql.tab.c"
    break;

  case 82: /* opt_star: NUMBER  */
#line 545 "yacc_sql.y"
                 {(yyval.string) = number_to_str((yyvsp[0].number));}
#line 1956 "yacc_sql.tab.c"
    break;

  case 84: /* rel_list: COMMA ID rel_list  */
#line 550 "yacc_sql.y"
                        {	
				selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-1].string));
		  }
#line 1964 "yacc_sql.tab.c"
    break;

  case 86: /* where: WHERE condition condition_list  */
#line 557 "yacc_sql.y"
                                     {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 1972 "yacc_sql.tab.c"
    break;

  case 88: /* on: ON condition condition_list  */
#line 564 "yacc_sql.y"
                                  {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 1980 "yacc_sql.tab.c"
    break;

  case 90: /* condition_list: AND condition condition_list  */
#line 571 "yacc_sql.y"
                                   {
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 1988 "yacc_sql.tab.c"
    break;

  case 91: /* condition: ID comOp value  */
#line 577 "yacc_sql.y"
=======
#line 2144 "yacc_sql.tab.c"
    break;

  case 76:
#line 549 "yacc_sql.y"
             { (yyval.string) = (yyvsp[0].string);}
#line 2150 "yacc_sql.tab.c"
    break;

  case 77:
#line 550 "yacc_sql.y"
                 {(yyval.string) = number_to_str((yyvsp[0].number));}
#line 2156 "yacc_sql.tab.c"
    break;

  case 78:
#line 551 "yacc_sql.y"
             {(yyval.string) = (yyvsp[0].string);}
#line 2162 "yacc_sql.tab.c"
    break;

  case 79:
#line 555 "yacc_sql.y"
                         {
		CONTEXT->rels[CONTEXT->rel_length++] = (yyvsp[-1].string);
		CONTEXT->rels[CONTEXT->rel_length++] = "NULL";
		(yyval.relation) = ( const char **)malloc(sizeof(const char*) * CONTEXT->rel_length);
		memcpy((yyval.relation), CONTEXT->rels, sizeof(const char*) * CONTEXT->rel_length);
		CONTEXT->rel_length = 0;
	}
#line 2174 "yacc_sql.tab.c"
    break;

  case 80:
#line 565 "yacc_sql.y"
                {}
#line 2180 "yacc_sql.tab.c"
    break;

  case 81:
#line 566 "yacc_sql.y"
                        {	
		CONTEXT->rels[CONTEXT->rel_length++] = (yyvsp[-1].string);
	}
#line 2188 "yacc_sql.tab.c"
    break;

  case 82:
#line 572 "yacc_sql.y"
                { 
		(yyval.condition1) = NULL; 
		// 这里不能清零，否则多个子查询条件时，子查询没有where会把主查询的condition清零 
	}
#line 2197 "yacc_sql.tab.c"
    break;

  case 83:
#line 576 "yacc_sql.y"
                                     {	
		// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
		RelAttr left_attr;
		relation_attr_init(&left_attr, NULL, "NULL", NULL, 0);
		RelAttr right_attr;
		relation_attr_init(&right_attr, NULL, "NULL", NULL, 0);
		condition_init(&CONTEXT->conditions[CONTEXT->condition_length++], NO_OP, 1, &left_attr, NULL, 1, &right_attr, NULL, NULL, NULL);

		(yyval.condition1) = ( Condition *)malloc(sizeof( Condition) * CONTEXT->condition_length);
		memcpy((yyval.condition1), CONTEXT->conditions, sizeof( Condition) * CONTEXT->condition_length);
		// 每次where结束都清零长度，多个子查询的where不会相互影响
		CONTEXT->condition_length = 0; 
		// 由于select里只有condition涉及到value_length，所以一并在此清零
		CONTEXT->value_length = 0;
	}
#line 2217 "yacc_sql.tab.c"
    break;

  case 85:
#line 595 "yacc_sql.y"
                                  {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
		selects_append_conditions_with_num(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);
		// 每次where结束都清零长度，多个子查询的where不会相互影响
		CONTEXT->condition_length = 0; 
		// 由于select里只有condition涉及到value_length，所以一并在此清零
		CONTEXT->value_length = 0;
	}
#line 2230 "yacc_sql.tab.c"
    break;

  case 87:
#line 607 "yacc_sql.y"
                                   {
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 2238 "yacc_sql.tab.c"
    break;

  case 88:
#line 614 "yacc_sql.y"
>>>>>>> main
                {
			RelAttr left_attr;
			// $1 为属性名称
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);

			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 0, NULL, right_value, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
<<<<<<< HEAD
#line 2013 "yacc_sql.tab.c"
    break;

  case 92: /* condition: value comOp value  */
#line 598 "yacc_sql.y"
=======
#line 2254 "yacc_sql.tab.c"
    break;

  case 89:
#line 626 "yacc_sql.y"
>>>>>>> main
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 0, NULL, left_value, 0, NULL, right_value, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

		}
<<<<<<< HEAD
#line 2037 "yacc_sql.tab.c"
    break;

  case 93: /* condition: ID comOp ID  */
#line 618 "yacc_sql.y"
=======
#line 2268 "yacc_sql.tab.c"
    break;

  case 90:
#line 636 "yacc_sql.y"
>>>>>>> main
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 1, &right_attr, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

		}
<<<<<<< HEAD
#line 2061 "yacc_sql.tab.c"
    break;

  case 94: /* condition: value comOp ID  */
#line 638 "yacc_sql.y"
=======
#line 2284 "yacc_sql.tab.c"
    break;

  case 91:
#line 648 "yacc_sql.y"
>>>>>>> main
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 0, NULL, left_value, 1, &right_attr, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
<<<<<<< HEAD
#line 2087 "yacc_sql.tab.c"
    break;

  case 95: /* condition: ID DOT ID comOp value  */
#line 660 "yacc_sql.y"
=======
#line 2298 "yacc_sql.tab.c"
    break;

  case 92:
#line 658 "yacc_sql.y"
>>>>>>> main
                {
			RelAttr left_attr;
			// $1为表名，$3为属性名
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string), NULL, 0);
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 0, NULL, right_value, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
    }
<<<<<<< HEAD
#line 2113 "yacc_sql.tab.c"
    break;

  case 96: /* condition: value comOp ID DOT ID  */
#line 682 "yacc_sql.y"
=======
#line 2313 "yacc_sql.tab.c"
    break;

  case 93:
#line 669 "yacc_sql.y"
>>>>>>> main
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, (yyvsp[-3].number), 0, NULL, left_value, 1, &right_attr, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
    }
<<<<<<< HEAD
#line 2138 "yacc_sql.tab.c"
    break;

  case 97: /* condition: ID DOT ID comOp ID DOT ID  */
#line 703 "yacc_sql.y"
=======
#line 2328 "yacc_sql.tab.c"
    break;

  case 94:
#line 680 "yacc_sql.y"
>>>>>>> main
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-6].string), (yyvsp[-4].string), NULL, 0);
			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, (yyvsp[-3].number), 1, &left_attr, NULL, 1, &right_attr, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
    }
#line 2161 "yacc_sql.tab.c"
    break;

<<<<<<< HEAD
  case 98: /* condition: ID IS NULL_T  */
#line 721 "yacc_sql.y"
=======
  case 95:
#line 690 "yacc_sql.y"
>>>>>>> main
                      {
		RelAttr left_attr;
		// $1 为属性名称
		relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);

		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NULL, 1, &left_attr, NULL, 0, NULL, right_value, NULL, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2178 "yacc_sql.tab.c"
    break;

<<<<<<< HEAD
  case 99: /* condition: ID IS NOT NULL_T  */
#line 733 "yacc_sql.y"
=======
  case 96:
#line 702 "yacc_sql.y"
>>>>>>> main
                          { // id is not null
		RelAttr left_attr;
		// $1 为属性名称
		relation_attr_init(&left_attr, NULL, (yyvsp[-3].string), NULL, 0);

		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NOT_NULL, 1, &left_attr, NULL, 0, NULL, right_value, NULL, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2195 "yacc_sql.tab.c"
    break;

<<<<<<< HEAD
  case 100: /* condition: ID DOT ID IS NULL_T  */
#line 745 "yacc_sql.y"
=======
  case 97:
#line 714 "yacc_sql.y"
>>>>>>> main
                             {
		RelAttr left_attr;
		// $1为表名，$3为属性名
		relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string), NULL, 0);
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NULL, 1, &left_attr, NULL, 0, NULL, right_value, NULL, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2211 "yacc_sql.tab.c"
    break;

<<<<<<< HEAD
  case 101: /* condition: ID DOT ID IS NOT NULL_T  */
#line 756 "yacc_sql.y"
=======
  case 98:
#line 725 "yacc_sql.y"
>>>>>>> main
                                 {
		RelAttr left_attr;
		// $1为表名，$3为属性名
		relation_attr_init(&left_attr, (yyvsp[-5].string), (yyvsp[-3].string), NULL, 0);
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NOT_NULL, 1, &left_attr, NULL, 0, NULL, right_value, NULL, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2227 "yacc_sql.tab.c"
    break;

<<<<<<< HEAD
  case 102: /* condition: value IS NOT NULL_T  */
#line 767 "yacc_sql.y"
=======
  case 99:
#line 736 "yacc_sql.y"
>>>>>>> main
                             { // null is null/value is not null
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NOT_NULL, 0, NULL, left_value, 0, NULL, right_value, NULL, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2241 "yacc_sql.tab.c"
    break;

<<<<<<< HEAD
  case 103: /* condition: value IS NULL_T  */
#line 776 "yacc_sql.y"
=======
  case 100:
#line 745 "yacc_sql.y"
>>>>>>> main
                         { //  null is not null/value is null
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NULL, 0, NULL, left_value, 0, NULL, right_value, NULL, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2255 "yacc_sql.tab.c"
    break;

<<<<<<< HEAD
  case 104: /* comOp: EQ  */
#line 788 "yacc_sql.y"
             { CONTEXT->comp = EQUAL_TO; }
#line 2261 "yacc_sql.tab.c"
    break;

  case 105: /* comOp: LT  */
#line 789 "yacc_sql.y"
         { CONTEXT->comp = LESS_THAN; }
#line 2267 "yacc_sql.tab.c"
    break;

  case 106: /* comOp: GT  */
#line 790 "yacc_sql.y"
         { CONTEXT->comp = GREAT_THAN; }
#line 2273 "yacc_sql.tab.c"
    break;

  case 107: /* comOp: LE  */
#line 791 "yacc_sql.y"
         { CONTEXT->comp = LESS_EQUAL; }
#line 2279 "yacc_sql.tab.c"
    break;

  case 108: /* comOp: GE  */
#line 792 "yacc_sql.y"
         { CONTEXT->comp = GREAT_EQUAL; }
#line 2285 "yacc_sql.tab.c"
    break;

  case 109: /* comOp: NE  */
#line 793 "yacc_sql.y"
         { CONTEXT->comp = NOT_EQUAL; }
#line 2291 "yacc_sql.tab.c"
    break;

  case 111: /* group_by: GROUP BY group_list  */
#line 798 "yacc_sql.y"
                              {
		;
	}
#line 2299 "yacc_sql.tab.c"
    break;

  case 112: /* group_list: group_attr  */
#line 804 "yacc_sql.y"
                  {
		;
	}
#line 2307 "yacc_sql.tab.c"
    break;

  case 113: /* group_list: group_list COMMA group_attr  */
#line 807 "yacc_sql.y"
                                      {}
#line 2313 "yacc_sql.tab.c"
    break;

  case 114: /* group_attr: ID  */
#line 811 "yacc_sql.y"
=======
  case 101:
#line 754 "yacc_sql.y"
                             {
		RelAttr left_attr;
		relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);

		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[0].selnode), NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2450 "yacc_sql.tab.c"
    break;

  case 102:
#line 762 "yacc_sql.y"
                                     {
		RelAttr left_attr;
		relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string), NULL, 0);

		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[0].selnode), NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2463 "yacc_sql.tab.c"
    break;

  case 103:
#line 770 "yacc_sql.y"
                                 {
		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), 0, NULL, left_value, 2, NULL, NULL, (yyvsp[0].selnode), NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2475 "yacc_sql.tab.c"
    break;

  case 104:
#line 777 "yacc_sql.y"
                                 {
		// 反过来，当作正的解析
		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		if ((yyvsp[-1].number) == GREAT_THAN || (yyvsp[-1].number) == GREAT_EQUAL) {
			condition_init(&condition, (yyvsp[-1].number) - 2, 0, NULL, left_value, 2, NULL, NULL, (yyvsp[-2].selnode), NULL);
		} else if ((yyvsp[-1].number) == LESS_THAN || (yyvsp[-1].number) == LESS_EQUAL) {
			condition_init(&condition, (yyvsp[-1].number) + 2, 0, NULL, left_value, 2, NULL, NULL, (yyvsp[-2].selnode), NULL);
		} else {
			condition_init(&condition, (yyvsp[-1].number), 0, NULL, left_value, 2, NULL, NULL, (yyvsp[-2].selnode), NULL);
		}
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2494 "yacc_sql.tab.c"
    break;

  case 105:
#line 791 "yacc_sql.y"
                             {
		// 反过来，当作正的解析
		RelAttr left_attr;
		relation_attr_init(&left_attr, NULL, (yyvsp[0].string), NULL, 0);

		Condition condition;
		if ((yyvsp[-1].number) == GREAT_THAN || (yyvsp[-1].number) == GREAT_EQUAL) {
			condition_init(&condition, (yyvsp[-1].number) - 2, 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[-2].selnode), NULL);
		} else if ((yyvsp[-1].number) == LESS_THAN || (yyvsp[-1].number) == LESS_EQUAL) {
			condition_init(&condition, (yyvsp[-1].number) + 2, 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[-2].selnode), NULL);
		} else {
			condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[-2].selnode), NULL);
		}
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2514 "yacc_sql.tab.c"
    break;

  case 106:
#line 806 "yacc_sql.y"
                                     {
		// 反过来，当作正的解析
		RelAttr left_attr;
		relation_attr_init(&left_attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);

		Condition condition;
		if ((yyvsp[-3].number) == GREAT_THAN || (yyvsp[-3].number) == GREAT_EQUAL) {
			condition_init(&condition, (yyvsp[-3].number) - 2, 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[-4].selnode), NULL);
		} else if ((yyvsp[-3].number) == LESS_THAN || (yyvsp[-3].number) == LESS_EQUAL) {
			condition_init(&condition, (yyvsp[-3].number) + 2, 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[-4].selnode), NULL);
		} else {
			condition_init(&condition, (yyvsp[-3].number), 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[-4].selnode), NULL);
		}
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2534 "yacc_sql.tab.c"
    break;

  case 107:
#line 821 "yacc_sql.y"
                                      {
		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), 2, NULL, NULL, 2, NULL, NULL, (yyvsp[0].selnode), (yyvsp[-2].selnode));
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2544 "yacc_sql.tab.c"
    break;

  case 108:
#line 829 "yacc_sql.y"
             { (yyval.number) = 0; }
#line 2550 "yacc_sql.tab.c"
    break;

  case 109:
#line 830 "yacc_sql.y"
         { (yyval.number) = 3; }
#line 2556 "yacc_sql.tab.c"
    break;

  case 110:
#line 831 "yacc_sql.y"
         { (yyval.number) = 5; }
#line 2562 "yacc_sql.tab.c"
    break;

  case 111:
#line 832 "yacc_sql.y"
         { (yyval.number) = 2; }
#line 2568 "yacc_sql.tab.c"
    break;

  case 112:
#line 833 "yacc_sql.y"
         { (yyval.number) = 4; }
#line 2574 "yacc_sql.tab.c"
    break;

  case 113:
#line 834 "yacc_sql.y"
         { (yyval.number) = 1; }
#line 2580 "yacc_sql.tab.c"
    break;

  case 114:
#line 835 "yacc_sql.y"
             { (yyval.number) = 8; }
#line 2586 "yacc_sql.tab.c"
    break;

  case 115:
#line 836 "yacc_sql.y"
                 { (yyval.number) = 9; }
#line 2592 "yacc_sql.tab.c"
    break;

  case 116:
#line 840 "yacc_sql.y"
                                                                 {
		(yyval.selnode) = (Selects*)malloc(sizeof(Selects));
		// 结构体malloc，后面要不跟上memcpy要不用memset全部默认初始化
		memset((yyval.selnode), 0, sizeof(Selects));

		selects_append_relations((yyval.selnode), (yyvsp[-3].relation)); // from_rel
		if ((yyvsp[-2].condition1) != NULL) {
			selects_append_conditions((yyval.selnode), (yyvsp[-2].condition1)); // where
		}
		selects_append_attributes((yyval.selnode), (yyvsp[-4].relattr1)); // select_attr
		// group by
		if ((yyvsp[-1].relattr1) != NULL) {
			selects_append_groups((yyval.selnode), (yyvsp[-1].relattr1)); 
		}
	}
#line 2612 "yacc_sql.tab.c"
    break;

  case 117:
#line 858 "yacc_sql.y"
                  {(yyval.relattr1) = NULL;}
#line 2618 "yacc_sql.tab.c"
    break;

  case 118:
#line 859 "yacc_sql.y"
                              {
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);
		(yyval.relattr1) = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy((yyval.relattr1), CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
#line 2629 "yacc_sql.tab.c"
    break;

  case 119:
#line 868 "yacc_sql.y"
                  {
		;
	}
#line 2637 "yacc_sql.tab.c"
    break;

  case 120:
#line 871 "yacc_sql.y"
                                      {}
#line 2643 "yacc_sql.tab.c"
    break;

  case 121:
#line 875 "yacc_sql.y"
>>>>>>> main
           {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL, 0);
		// selects_append_group(&CONTEXT->ssql->sstr.selection, &attr);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
<<<<<<< HEAD
#line 2323 "yacc_sql.tab.c"
    break;

  case 115: /* group_attr: ID DOT ID  */
#line 816 "yacc_sql.y"
=======
#line 2654 "yacc_sql.tab.c"
    break;

  case 122:
#line 881 "yacc_sql.y"
>>>>>>> main
                    {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);
		// selects_append_group(&CONTEXT->ssql->sstr.selection, &attr);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
<<<<<<< HEAD
#line 2333 "yacc_sql.tab.c"
    break;

  case 117: /* order_by: ORDER BY sort_list  */
#line 825 "yacc_sql.y"
                             {
	}
#line 2340 "yacc_sql.tab.c"
    break;

  case 118: /* sort_list: sort_attr  */
#line 830 "yacc_sql.y"
                  {
		// order by A, B, C，实际上加入顺序为C、B、A，方便后面排序
	}
#line 2348 "yacc_sql.tab.c"
    break;

  case 119: /* sort_list: sort_list COMMA sort_attr  */
#line 833 "yacc_sql.y"
                                    {}
#line 2354 "yacc_sql.tab.c"
    break;

  case 120: /* sort_attr: ID opt_asc  */
#line 836 "yacc_sql.y"
=======
#line 2665 "yacc_sql.tab.c"
    break;

  case 124:
#line 891 "yacc_sql.y"
                             {
	}
#line 2672 "yacc_sql.tab.c"
    break;

  case 125:
#line 896 "yacc_sql.y"
                  {
		// order by A, B, C，实际上加入顺序为C、B、A，方便后面排序
	}
#line 2680 "yacc_sql.tab.c"
    break;

  case 126:
#line 899 "yacc_sql.y"
                                    {}
#line 2686 "yacc_sql.tab.c"
    break;

  case 127:
#line 902 "yacc_sql.y"
>>>>>>> main
                  {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL, 0);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
<<<<<<< HEAD
#line 2364 "yacc_sql.tab.c"
    break;

  case 121: /* sort_attr: ID DESC  */
#line 841 "yacc_sql.y"
=======
#line 2696 "yacc_sql.tab.c"
    break;

  case 128:
#line 907 "yacc_sql.y"
>>>>>>> main
                  {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL, 1);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
<<<<<<< HEAD
#line 2374 "yacc_sql.tab.c"
    break;

  case 122: /* sort_attr: ID DOT ID opt_asc  */
#line 846 "yacc_sql.y"
=======
#line 2706 "yacc_sql.tab.c"
    break;

  case 129:
#line 912 "yacc_sql.y"
>>>>>>> main
                            {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL, 0);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
<<<<<<< HEAD
#line 2384 "yacc_sql.tab.c"
    break;

  case 123: /* sort_attr: ID DOT ID DESC  */
#line 851 "yacc_sql.y"
=======
#line 2716 "yacc_sql.tab.c"
    break;

  case 130:
#line 917 "yacc_sql.y"
>>>>>>> main
                         {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL, 1);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
<<<<<<< HEAD
#line 2394 "yacc_sql.tab.c"
    break;

  case 125: /* opt_asc: ASC  */
#line 859 "yacc_sql.y"
              {}
#line 2400 "yacc_sql.tab.c"
    break;

  case 126: /* load_data: LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON  */
#line 863 "yacc_sql.y"
=======
#line 2726 "yacc_sql.tab.c"
    break;

  case 132:
#line 925 "yacc_sql.y"
              {}
#line 2732 "yacc_sql.tab.c"
    break;

  case 133:
#line 929 "yacc_sql.y"
>>>>>>> main
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
<<<<<<< HEAD
#line 2409 "yacc_sql.tab.c"
    break;


#line 2413 "yacc_sql.tab.c"
=======
#line 2741 "yacc_sql.tab.c"
    break;


#line 2745 "yacc_sql.tab.c"
>>>>>>> main

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (scanner, YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}
<<<<<<< HEAD

#line 868 "yacc_sql.y"
=======
#line 935 "yacc_sql.y"
>>>>>>> main

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
