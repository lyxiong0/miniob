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


#line 145 "yacc_sql.tab.c"

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
  YYSYMBOL_TEXT_T = 57,                    /* TEXT_T  */
  YYSYMBOL_NUMBER = 58,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 59,                     /* FLOAT  */
  YYSYMBOL_ID = 60,                        /* ID  */
  YYSYMBOL_PATH = 61,                      /* PATH  */
  YYSYMBOL_SSS = 62,                       /* SSS  */
  YYSYMBOL_STAR = 63,                      /* STAR  */
  YYSYMBOL_STRING_V = 64,                  /* STRING_V  */
  YYSYMBOL_COUNT = 65,                     /* COUNT  */
  YYSYMBOL_OTHER_FUNCTION_TYPE = 66,       /* OTHER_FUNCTION_TYPE  */
  YYSYMBOL_Column = 67,                    /* Column  */
  YYSYMBOL_YYACCEPT = 68,                  /* $accept  */
  YYSYMBOL_commands = 69,                  /* commands  */
  YYSYMBOL_command = 70,                   /* command  */
  YYSYMBOL_exit = 71,                      /* exit  */
  YYSYMBOL_help = 72,                      /* help  */
  YYSYMBOL_sync = 73,                      /* sync  */
  YYSYMBOL_begin = 74,                     /* begin  */
  YYSYMBOL_commit = 75,                    /* commit  */
  YYSYMBOL_rollback = 76,                  /* rollback  */
  YYSYMBOL_drop_table = 77,                /* drop_table  */
  YYSYMBOL_show_tables = 78,               /* show_tables  */
  YYSYMBOL_desc_table = 79,                /* desc_table  */
  YYSYMBOL_create_index = 80,              /* create_index  */
  YYSYMBOL_Column_list = 81,               /* Column_list  */
  YYSYMBOL_Column_def = 82,                /* Column_def  */
  YYSYMBOL_drop_index = 83,                /* drop_index  */
  YYSYMBOL_create_table = 84,              /* create_table  */
  YYSYMBOL_attr_def_list = 85,             /* attr_def_list  */
  YYSYMBOL_attr_def = 86,                  /* attr_def  */
  YYSYMBOL_opt_null = 87,                  /* opt_null  */
  YYSYMBOL_number = 88,                    /* number  */
  YYSYMBOL_type = 89,                      /* type  */
  YYSYMBOL_ID_get = 90,                    /* ID_get  */
  YYSYMBOL_insert = 91,                    /* insert  */
  YYSYMBOL_multi_values = 92,              /* multi_values  */
  YYSYMBOL_value_list = 93,                /* value_list  */
  YYSYMBOL_value = 94,                     /* value  */
  YYSYMBOL_delete = 95,                    /* delete  */
  YYSYMBOL_update = 96,                    /* update  */
  YYSYMBOL_select = 97,                    /* select  */
  YYSYMBOL_select_attr = 98,               /* select_attr  */
  YYSYMBOL_select_param = 99,              /* select_param  */
  YYSYMBOL_id_type = 100,                  /* id_type  */
  YYSYMBOL_attr_list = 101,                /* attr_list  */
  YYSYMBOL_join_list = 102,                /* join_list  */
  YYSYMBOL_window_function = 103,          /* window_function  */
  YYSYMBOL_opt_star = 104,                 /* opt_star  */
  YYSYMBOL_from_rel = 105,                 /* from_rel  */
  YYSYMBOL_rel_list = 106,                 /* rel_list  */
  YYSYMBOL_where = 107,                    /* where  */
  YYSYMBOL_on = 108,                       /* on  */
  YYSYMBOL_condition_list = 109,           /* condition_list  */
  YYSYMBOL_condition = 110,                /* condition  */
  YYSYMBOL_comOp = 111,                    /* comOp  */
  YYSYMBOL_sub_select = 112,               /* sub_select  */
  YYSYMBOL_group_by = 113,                 /* group_by  */
  YYSYMBOL_group_list = 114,               /* group_list  */
  YYSYMBOL_group_attr = 115,               /* group_attr  */
  YYSYMBOL_order_by = 116,                 /* order_by  */
  YYSYMBOL_sort_list = 117,                /* sort_list  */
  YYSYMBOL_sort_attr = 118,                /* sort_attr  */
  YYSYMBOL_opt_asc = 119,                  /* opt_asc  */
  YYSYMBOL_load_data = 120                 /* load_data  */
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
#define YYLAST   282

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  53
/* YYNRULES -- Number of rules.  */
#define YYNRULES  138
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  279

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   322


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
      65,    66,    67
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   178,   178,   180,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   204,   209,   214,   220,   226,   232,   238,   244,   250,
     257,   263,   270,   272,   274,   275,   280,   287,   297,   299,
     303,   314,   324,   327,   330,   336,   339,   343,   347,   351,
     354,   359,   368,   385,   392,   400,   402,   407,   410,   413,
     417,   427,   439,   452,   469,   479,   489,   490,   494,   499,
     504,   511,   513,   516,   518,   524,   530,   536,   542,   548,
     554,   563,   564,   565,   569,   579,   580,   586,   590,   607,
     609,   619,   621,   627,   639,   649,   661,   671,   682,   693,
     704,   716,   728,   739,   750,   759,   768,   776,   784,   791,
     805,   820,   835,   843,   844,   845,   846,   847,   848,   849,
     850,   854,   872,   873,   882,   885,   889,   895,   903,   905,
     910,   913,   916,   921,   926,   931,   937,   939,   942
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
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "CREATE",
  "DROP", "TABLE", "TABLES", "INDEX", "SELECT", "DESC", "SHOW", "SYNC",
  "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN",
  "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T", "FLOAT_T", "ORDER",
  "ASC", "BY", "DATE_T", "UNIQUE", "HELP", "EXIT", "DOT", "INTO", "VALUES",
  "FROM", "WHERE", "AND", "SET", "ON", "LOAD", "DATA", "INFILE",
  "NULLABLE", "GROUP", "IS", "NOT", "EQ", "LT", "GT", "LE", "GE", "NE",
  "NULL_T", "INNER", "JOIN", "IN", "TEXT_T", "NUMBER", "FLOAT", "ID",
  "PATH", "SSS", "STAR", "STRING_V", "COUNT", "OTHER_FUNCTION_TYPE",
  "Column", "$accept", "commands", "command", "exit", "help", "sync",
  "begin", "commit", "rollback", "drop_table", "show_tables", "desc_table",
  "create_index", "Column_list", "Column_def", "drop_index",
  "create_table", "attr_def_list", "attr_def", "opt_null", "number",
  "type", "ID_get", "insert", "multi_values", "value_list", "value",
  "delete", "update", "select", "select_attr", "select_param", "id_type",
  "attr_list", "join_list", "window_function", "opt_star", "from_rel",
  "rel_list", "where", "on", "condition_list", "condition", "comOp",
  "sub_select", "group_by", "group_list", "group_attr", "order_by",
  "sort_list", "sort_attr", "opt_asc", "load_data", YY_NULLPTR
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
     315,   316,   317,   318,   319,   320,   321,   322
};
#endif

#define YYPACT_NINF (-197)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -197,   108,  -197,    31,   154,    71,   -50,    62,    56,    44,
      59,    37,    88,   100,   104,   122,   146,    74,  -197,  -197,
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
    -197,  -197,  -197,  -197,  -197,  -197,    90,   101,   163,   113,
     116,   145,  -197,   162,   164,   144,   165,  -197,  -197,   178,
     179,  -197,   124,   125,   148,  -197,  -197,  -197,  -197,  -197,
     147,   171,   149,   130,   188,   189,   -17,    72,   133,   134,
     141,    33,  -197,  -197,  -197,  -197,   166,   160,   137,   136,
     124,   139,   167,  -197,  -197,  -197,  -197,  -197,   169,  -197,
     185,    34,   186,   150,   160,   165,   187,    -5,   204,   161,
     176,   192,    -7,   195,   152,    46,  -197,  -197,   112,   153,
    -197,   155,   170,  -197,    52,    65,   207,  -197,  -197,  -197,
     -23,  -197,    95,   180,   118,  -197,    52,   212,   124,   202,
    -197,  -197,  -197,  -197,  -197,   -11,   168,   205,   203,   206,
     208,   209,   186,   183,   197,   210,   211,  -197,   214,    71,
     172,    -8,   175,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
       3,    39,    20,    -5,  -197,    28,   160,   173,   192,   224,
     181,  -197,   184,  -197,  -197,   216,   168,  -197,  -197,  -197,
    -197,  -197,    -5,   141,   182,   213,   233,    52,   221,    52,
     144,   107,   190,  -197,  -197,  -197,  -197,  -197,   191,  -197,
     215,  -197,  -197,   180,   217,  -197,  -197,   238,   242,  -197,
    -197,  -197,   229,  -197,   168,   231,   216,   180,  -197,   218,
     234,  -197,   193,  -197,   211,  -197,   211,   160,    43,    42,
    -197,  -197,   194,  -197,   196,  -197,  -197,    21,   216,   248,
     240,  -197,   198,   182,     8,   237,  -197,  -197,   243,   170,
     219,  -197,   227,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
     258,  -197,  -197,  -197,  -197,   220,  -197,   193,  -197,   245,
    -197,   222,  -197,     4,  -197,  -197,  -197,  -197,  -197
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
       0,    68,    64,     0,     0,     0,    71,    66,    67,     0,
       0,    23,     0,     0,     0,    24,    25,    26,    22,    21,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      73,     0,    65,    29,    28,    51,     0,    87,     0,     0,
       0,     0,     0,    27,    36,    69,    70,    82,    83,    81,
       0,     0,    85,     0,    87,    71,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,    75,    78,     0,     0,
      84,     0,   122,    72,     0,     0,     0,    59,    57,    58,
       0,    60,     0,    91,     0,    61,     0,     0,     0,     0,
      46,    47,    48,    49,    50,    42,    34,     0,     0,     0,
       0,     0,    85,    89,     0,   128,    55,    52,     0,     0,
       0,     0,     0,   113,   114,   115,   116,   117,   118,   119,
       0,     0,     0,     0,    88,     0,    87,     0,    38,     0,
       0,    44,     0,    41,    35,    32,    34,    76,    77,    79,
      80,    86,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,   120,    95,    93,   106,     0,   105,
      96,    94,   108,    91,   110,   109,   112,     0,     0,    39,
      37,    45,     0,    43,    34,     0,    32,    91,    74,   126,
     123,   124,     0,    63,    55,    53,    55,    87,     0,     0,
     101,   104,     0,    92,     0,    62,   138,    42,    32,     0,
       0,    90,     0,     0,   136,   129,   130,    56,     0,   122,
       0,   102,     0,    97,   107,    98,   111,    40,    33,    30,
       0,   127,   125,   133,   137,     0,   132,     0,    54,     0,
     103,     0,    31,   136,   131,   121,    99,   135,   134
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
    -197,  -197,  -197,  -196,  -173,  -197,  -197,    96,   135,    29,
    -197,  -197,   223,  -197,  -197,  -100,  -113,  -197,  -197,  -197,
     119,   199,  -197,   174,    82,  -197,  -197,    77,   129,   -94,
    -197,  -133,  -151,  -116,  -158,    24,  -197,    35,  -197,  -197,
       7,     6,  -197
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,   215,   175,    29,    30,   129,   101,   173,
     212,   135,   102,    31,   115,   188,   122,    32,    33,    34,
      45,    46,    47,    72,    94,    48,    90,    70,   110,    98,
     183,   164,   123,   160,   124,   145,   220,   221,   186,   245,
     246,   266,    35
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     112,   146,   197,   216,   202,   170,   162,   206,   165,   150,
      49,   116,   203,   166,   277,   130,   131,   132,   263,   116,
     240,   133,   151,   152,   153,   154,   155,   156,   157,   158,
     264,   217,   171,   159,   264,   172,   116,    36,   192,    37,
     265,   238,   258,    85,   116,   193,    86,   196,   117,   201,
     134,   107,   205,   118,   119,   120,   117,   121,   116,    51,
      38,   118,   119,   195,   171,   121,   108,   172,   147,    50,
     233,   254,   207,   117,   224,   229,   226,    52,   118,   119,
     200,   117,   121,   148,   241,   198,   118,   119,   204,   250,
     121,    55,   199,    41,    53,   117,   251,    54,    43,    44,
     118,   119,   252,    56,   121,   117,   138,    57,     2,   139,
     118,   119,     3,     4,   121,    60,   253,     5,     6,     7,
       8,     9,    10,    11,   247,    58,   248,    12,    13,    14,
      87,    41,    88,   249,    42,    89,    43,    44,    15,    16,
     161,   152,   153,   154,   155,   156,   157,   158,    17,    59,
      61,   159,   228,   152,   153,   154,   155,   156,   157,   158,
      39,    62,    40,   159,   152,   153,   154,   155,   156,   157,
     158,    63,   140,    64,   159,   141,    65,    66,    67,    69,
      68,    73,    74,    71,    75,    77,    78,    80,    81,    79,
      82,    83,    84,    91,    92,    93,    97,    99,   100,   103,
      96,   105,   106,   114,   109,   111,   104,   125,   126,   127,
     128,   136,   137,   142,   144,   143,   149,   163,   167,   169,
     177,   176,   182,   178,   184,   179,   180,   210,   174,   187,
     189,   194,   191,   208,   214,   185,   223,   213,   225,   211,
     222,   235,   219,   230,   231,   236,   237,   232,   239,   234,
     242,   259,   243,   244,   255,   267,   256,   260,   261,   271,
     268,   272,   275,   168,   209,   218,   257,   227,   190,   113,
      95,   181,   270,   269,   274,    76,     0,     0,   262,   278,
     273,     0,   276
};

static const yytype_int16 yycheck[] =
{
      94,   114,   160,   176,   162,    16,   122,   165,   124,    32,
      60,    16,   163,   126,    10,    22,    23,    24,    10,    16,
     216,    28,    45,    46,    47,    48,    49,    50,    51,    52,
      26,   182,    43,    56,    26,    46,    16,     6,    46,     8,
      32,   214,   238,    60,    16,    53,    63,   160,    53,   162,
      57,    17,   165,    58,    59,    60,    53,    62,    16,     3,
      29,    58,    59,    60,    43,    62,    32,    46,     3,     7,
     203,   229,   166,    53,   187,   191,   189,    33,    58,    59,
      60,    53,    62,    18,   217,    46,    58,    59,    60,    46,
      62,     3,    53,    60,    35,    53,    53,    60,    65,    66,
      58,    59,    60,     3,    62,    53,    60,     3,     0,    63,
      58,    59,     4,     5,    62,    41,   229,     9,    10,    11,
      12,    13,    14,    15,   224,     3,   226,    19,    20,    21,
      58,    60,    60,   227,    63,    63,    65,    66,    30,    31,
      45,    46,    47,    48,    49,    50,    51,    52,    40,     3,
      60,    56,    45,    46,    47,    48,    49,    50,    51,    52,
       6,    60,     8,    56,    46,    47,    48,    49,    50,    51,
      52,     8,    60,    60,    56,    63,    60,    32,    16,    35,
      16,     3,     3,    18,    60,    60,    38,    16,    39,    42,
      60,     3,     3,    60,    60,    54,    36,    60,    62,    60,
      34,    32,    17,    16,    18,    55,    39,     3,    47,    33,
      18,    16,    60,    60,    44,    60,     9,    37,     6,    17,
      17,    16,    39,    17,    27,    17,    17,     3,    60,    18,
      16,    56,    60,    60,    18,    25,     3,    53,    17,    58,
      27,     3,    60,    53,    53,     3,    17,    32,    17,    32,
      32,     3,    18,    60,    60,    18,    60,    17,    60,    32,
      17,     3,    17,   128,   168,   183,   237,   190,   149,    95,
      71,   142,    53,   249,   267,    52,    -1,    -1,   243,   273,
      60,    -1,    60
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    69,     0,     4,     5,     9,    10,    11,    12,    13,
      14,    15,    19,    20,    21,    30,    31,    40,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    83,
      84,    91,    95,    96,    97,   120,     6,     8,    29,     6,
       8,    60,    63,    65,    66,    98,    99,   100,   103,    60,
       7,     3,    33,    35,    60,     3,     3,     3,     3,     3,
      41,    60,    60,     8,    60,    60,    32,    16,    16,    35,
     105,    18,   101,     3,     3,    60,    90,    60,    38,    42,
      16,    39,    60,     3,     3,    60,    63,    58,    60,    63,
     104,    60,    60,    54,   102,    99,    34,    36,   107,    60,
      62,    86,    90,    60,    39,    32,    17,    17,    32,    18,
     106,    55,   107,   101,    16,    92,    16,    53,    58,    59,
      60,    62,    94,   110,   112,     3,    47,    33,    18,    85,
      22,    23,    24,    28,    57,    89,    16,    60,    60,    63,
      60,    63,    60,    60,    44,   113,    94,     3,    18,     9,
      32,    45,    46,    47,    48,    49,    50,    51,    52,    56,
     111,    45,   111,    37,   109,   111,    94,     6,    86,    17,
      16,    43,    46,    87,    60,    82,    16,    17,    17,    17,
      17,   106,    39,   108,    27,    25,   116,    18,    93,    16,
      98,    60,    46,    53,    56,    60,    94,   112,    46,    53,
      60,    94,   112,   110,    60,    94,   112,   107,    60,    85,
       3,    58,    88,    53,    18,    81,    82,   110,   102,    60,
     114,   115,    27,     3,    94,    17,    94,   105,    45,   111,
      53,    53,    32,   109,    32,     3,     3,    17,    82,    17,
      81,   109,    32,    18,    60,   117,   118,    93,    93,   107,
      46,    53,    60,    94,   112,    60,    60,    87,    81,     3,
      17,    60,   115,    10,    26,    32,   119,    18,    17,   113,
      53,    32,     3,    60,   118,    17,    60,    10,   119
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    68,    69,    69,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    80,    81,    81,    82,    82,    83,    84,    85,    85,
      86,    86,    87,    87,    87,    88,    89,    89,    89,    89,
      89,    90,    91,    92,    92,    93,    93,    94,    94,    94,
      94,    95,    96,    97,    98,    98,    99,    99,   100,   100,
     100,   101,   101,   102,   102,   103,   103,   103,   103,   103,
     103,   104,   104,   104,   105,   106,   106,   107,   107,   108,
     108,   109,   109,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   111,   111,   111,   111,   111,   111,   111,
     111,   112,   113,   113,   114,   114,   115,   115,   116,   116,
     117,   117,   118,   118,   118,   118,   119,   119,   120
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
      10,    11,     0,     3,     0,     1,     4,     8,     0,     3,
       6,     3,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     4,     6,     0,     3,     1,     1,     1,
       1,     5,     8,     8,     1,     2,     1,     1,     1,     3,
       3,     0,     3,     0,     5,     4,     6,     6,     4,     6,
       6,     1,     1,     1,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     3,     3,     3,     3,     5,     5,     7,
       3,     4,     5,     6,     4,     3,     3,     5,     3,     3,
       3,     5,     3,     1,     1,     1,     1,     1,     1,     1,
       2,     7,     0,     3,     1,     3,     1,     3,     0,     3,
       1,     3,     2,     2,     4,     4,     0,     1,     8
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
  case 21: /* exit: EXIT SEMICOLON  */
#line 204 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1459 "yacc_sql.tab.c"
    break;

  case 22: /* help: HELP SEMICOLON  */
#line 209 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1467 "yacc_sql.tab.c"
    break;

  case 23: /* sync: SYNC SEMICOLON  */
#line 214 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1475 "yacc_sql.tab.c"
    break;

  case 24: /* begin: TRX_BEGIN SEMICOLON  */
#line 220 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1483 "yacc_sql.tab.c"
    break;

  case 25: /* commit: TRX_COMMIT SEMICOLON  */
#line 226 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1491 "yacc_sql.tab.c"
    break;

  case 26: /* rollback: TRX_ROLLBACK SEMICOLON  */
#line 232 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1499 "yacc_sql.tab.c"
    break;

  case 27: /* drop_table: DROP TABLE ID SEMICOLON  */
#line 238 "yacc_sql.y"
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1508 "yacc_sql.tab.c"
    break;

  case 28: /* show_tables: SHOW TABLES SEMICOLON  */
#line 244 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1516 "yacc_sql.tab.c"
    break;

  case 29: /* desc_table: DESC ID SEMICOLON  */
#line 250 "yacc_sql.y"
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1525 "yacc_sql.tab.c"
    break;

  case 30: /* create_index: CREATE INDEX ID ON ID LBRACE Column_def Column_list RBRACE SEMICOLON  */
#line 258 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			// create_index_init(&CONTEXT->ssql->sstr.create_index, $3, $5, $7, 0);
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-7].string), (yyvsp[-5].string), 0);
		}
#line 1535 "yacc_sql.tab.c"
    break;

  case 31: /* create_index: CREATE UNIQUE INDEX ID ON ID LBRACE Column_def Column_list RBRACE SEMICOLON  */
#line 264 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			// create_index_init(&CONTEXT->ssql->sstr.create_index, $4, $6, $8, 1);
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-7].string), (yyvsp[-5].string), 1);
		}
#line 1545 "yacc_sql.tab.c"
    break;

  case 33: /* Column_list: COMMA Column_def Column_list  */
#line 272 "yacc_sql.y"
                                       { }
#line 1551 "yacc_sql.tab.c"
    break;

  case 35: /* Column_def: ID  */
#line 275 "yacc_sql.y"
             {
		create_index_append_attribute(&CONTEXT->ssql->sstr.create_index, (yyvsp[0].string));
	}
#line 1559 "yacc_sql.tab.c"
    break;

  case 36: /* drop_index: DROP INDEX ID SEMICOLON  */
#line 281 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1568 "yacc_sql.tab.c"
    break;

  case 37: /* create_table: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON  */
#line 288 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
#line 1580 "yacc_sql.tab.c"
    break;

  case 39: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 299 "yacc_sql.y"
                                   {    }
#line 1586 "yacc_sql.tab.c"
    break;

  case 40: /* attr_def: ID_get type LBRACE number RBRACE opt_null  */
#line 304 "yacc_sql.y"
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
#line 1601 "yacc_sql.tab.c"
    break;

  case 41: /* attr_def: ID_get type opt_null  */
#line 315 "yacc_sql.y"
                {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-1].number), 4, (yyvsp[0].number));
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			CONTEXT->value_length++;
		}
#line 1612 "yacc_sql.tab.c"
    break;

  case 42: /* opt_null: %empty  */
#line 324 "yacc_sql.y"
                  {
		(yyval.number) = ISFALSE; // 默认允许null
	}
#line 1620 "yacc_sql.tab.c"
    break;

  case 43: /* opt_null: NOT NULL_T  */
#line 327 "yacc_sql.y"
                     {
		(yyval.number) = ISFALSE;
	}
#line 1628 "yacc_sql.tab.c"
    break;

  case 44: /* opt_null: NULLABLE  */
#line 330 "yacc_sql.y"
                   {
		(yyval.number) = ISTRUE;
	}
#line 1636 "yacc_sql.tab.c"
    break;

  case 45: /* number: NUMBER  */
#line 336 "yacc_sql.y"
                       {(yyval.number) = (yyvsp[0].number);}
#line 1642 "yacc_sql.tab.c"
    break;

  case 46: /* type: INT_T  */
#line 339 "yacc_sql.y"
              { 
		(yyval.number)=INTS; 
		// printf("CREATE 语句语法解析 type 为 INTS\n");
	}
#line 1651 "yacc_sql.tab.c"
    break;

  case 47: /* type: STRING_T  */
#line 343 "yacc_sql.y"
                  { 
		   (yyval.number)=CHARS;
		// printf("CREATE 语句语法解析 type 为 STRING_T\n");
	}
#line 1660 "yacc_sql.tab.c"
    break;

  case 48: /* type: FLOAT_T  */
#line 347 "yacc_sql.y"
                 { 
		   (yyval.number)=FLOATS;
		// printf("CREATE 语句语法解析 type 为 FLOAT_T\n");
	}
#line 1669 "yacc_sql.tab.c"
    break;

  case 49: /* type: DATE_T  */
#line 351 "yacc_sql.y"
                    { 
		   (yyval.number)=DATES;
		// printf("CREATE 语句语法解析 type 为 DATE_T\n");
	}
#line 1678 "yacc_sql.tab.c"
    break;

  case 50: /* type: TEXT_T  */
#line 354 "yacc_sql.y"
                    {
	    (yyval.number)=TEXTS;
	}
#line 1686 "yacc_sql.tab.c"
    break;

  case 51: /* ID_get: ID  */
#line 360 "yacc_sql.y"
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1695 "yacc_sql.tab.c"
    break;

  case 52: /* insert: INSERT INTO ID_get VALUES multi_values SEMICOLON  */
#line 369 "yacc_sql.y"
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
#line 1714 "yacc_sql.tab.c"
    break;

  case 53: /* multi_values: LBRACE value value_list RBRACE  */
#line 385 "yacc_sql.y"
                                       {
		// 到此结束一组的插入：存储该组、增加index、value_length清零
		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
		CONTEXT->insert_index++;
		//临时变量清零
      	CONTEXT->value_length=0;
	}
#line 1726 "yacc_sql.tab.c"
    break;

  case 54: /* multi_values: multi_values COMMA LBRACE value value_list RBRACE  */
#line 392 "yacc_sql.y"
                                                           {
		// 到此结束一组的插入：存储该组、增加index、value_length清零
		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
		CONTEXT->insert_index++;
		//临时变量清零
      	CONTEXT->value_length=0;
	}
#line 1738 "yacc_sql.tab.c"
    break;

  case 56: /* value_list: COMMA value value_list  */
#line 402 "yacc_sql.y"
                              { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1746 "yacc_sql.tab.c"
    break;

  case 57: /* value: NUMBER  */
#line 407 "yacc_sql.y"
          {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number), false);
	}
#line 1754 "yacc_sql.tab.c"
    break;

  case 58: /* value: FLOAT  */
#line 410 "yacc_sql.y"
          {
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].floats), false);
	}
#line 1762 "yacc_sql.tab.c"
    break;

  case 59: /* value: NULL_T  */
#line 413 "yacc_sql.y"
                {
		// null不需要加双引号，当作字符串插入
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
	}
#line 1771 "yacc_sql.tab.c"
    break;

  case 60: /* value: SSS  */
#line 417 "yacc_sql.y"
         {
        // 没有末位的"\0"
		(yyvsp[0].string) = substr((yyvsp[0].string), 1, strlen((yyvsp[0].string))-2);
        // 长度大于4就当作tetx来处理
		value_init_string_with_text(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string), false, strlen((yyvsp[0].string)));
		}
#line 1782 "yacc_sql.tab.c"
    break;

  case 61: /* delete: DELETE FROM ID where SEMICOLON  */
#line 428 "yacc_sql.y"
        {
		CONTEXT->ssql->flag = SCF_DELETE;//"delete";
		deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
		// 处理where
		if ((yyvsp[-1].condition1) != NULL) {
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, (yyvsp[-1].condition1)); // where
		}
    }
#line 1795 "yacc_sql.tab.c"
    break;

  case 62: /* update: UPDATE ID SET ID EQ value where SEMICOLON  */
#line 440 "yacc_sql.y"
        {
		CONTEXT->ssql->flag = SCF_UPDATE;//"update";
		Value *value = &CONTEXT->values[0];
		updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value);
		if ((yyvsp[-1].condition1) != NULL) {
			updates_init_condition(&CONTEXT->ssql->sstr.update, (yyvsp[-1].condition1));
		}
	}
#line 1808 "yacc_sql.tab.c"
    break;

  case 63: /* select: SELECT select_attr from_rel join_list where group_by order_by SEMICOLON  */
#line 453 "yacc_sql.y"
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
#line 1826 "yacc_sql.tab.c"
    break;

  case 64: /* select_attr: STAR  */
#line 469 "yacc_sql.y"
         {  // select *
		RelAttr attr;
		relation_attr_init(&attr, NULL, "*", NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);

		(yyval.relattr1) = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy((yyval.relattr1), CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
#line 1841 "yacc_sql.tab.c"
    break;

  case 65: /* select_attr: select_param attr_list  */
#line 479 "yacc_sql.y"
                             { 
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);

		(yyval.relattr1) = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy((yyval.relattr1), CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
#line 1853 "yacc_sql.tab.c"
    break;

  case 66: /* select_param: id_type  */
#line 489 "yacc_sql.y"
                {}
#line 1859 "yacc_sql.tab.c"
    break;

  case 67: /* select_param: window_function  */
#line 490 "yacc_sql.y"
                          {}
#line 1865 "yacc_sql.tab.c"
    break;

  case 68: /* id_type: ID  */
#line 494 "yacc_sql.y"
          { // select age
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 1875 "yacc_sql.tab.c"
    break;

  case 69: /* id_type: ID DOT ID  */
#line 499 "yacc_sql.y"
                    { // select t1.age
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 1885 "yacc_sql.tab.c"
    break;

  case 70: /* id_type: ID DOT STAR  */
#line 504 "yacc_sql.y"
                     { // select t1.*
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-2].string), "*", NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 1895 "yacc_sql.tab.c"
    break;

  case 72: /* attr_list: COMMA select_param attr_list  */
#line 513 "yacc_sql.y"
                                   { }
#line 1901 "yacc_sql.tab.c"
    break;

  case 74: /* join_list: INNER JOIN ID on join_list  */
#line 518 "yacc_sql.y"
                                 {
		selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-2].string));
    }
#line 1909 "yacc_sql.tab.c"
    break;

  case 75: /* window_function: COUNT LBRACE opt_star RBRACE  */
#line 525 "yacc_sql.y"
        {	// 只有COUNT允许COUNT(*)
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 1919 "yacc_sql.tab.c"
    break;

  case 76: /* window_function: COUNT LBRACE ID DOT ID RBRACE  */
#line 531 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 1929 "yacc_sql.tab.c"
    break;

  case 77: /* window_function: COUNT LBRACE ID DOT STAR RBRACE  */
#line 537 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 1939 "yacc_sql.tab.c"
    break;

  case 78: /* window_function: OTHER_FUNCTION_TYPE LBRACE ID RBRACE  */
#line 543 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 1949 "yacc_sql.tab.c"
    break;

  case 79: /* window_function: OTHER_FUNCTION_TYPE LBRACE ID DOT ID RBRACE  */
#line 549 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 1959 "yacc_sql.tab.c"
    break;

  case 80: /* window_function: OTHER_FUNCTION_TYPE LBRACE ID DOT STAR RBRACE  */
#line 555 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), "*", (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 1969 "yacc_sql.tab.c"
    break;

  case 81: /* opt_star: STAR  */
#line 563 "yacc_sql.y"
             { (yyval.string) = (yyvsp[0].string);}
#line 1975 "yacc_sql.tab.c"
    break;

  case 82: /* opt_star: NUMBER  */
#line 564 "yacc_sql.y"
                 {(yyval.string) = number_to_str((yyvsp[0].number));}
#line 1981 "yacc_sql.tab.c"
    break;

  case 83: /* opt_star: ID  */
#line 565 "yacc_sql.y"
             {(yyval.string) = (yyvsp[0].string);}
#line 1987 "yacc_sql.tab.c"
    break;

  case 84: /* from_rel: FROM ID rel_list  */
#line 569 "yacc_sql.y"
                         {
		CONTEXT->rels[CONTEXT->rel_length++] = (yyvsp[-1].string);
		CONTEXT->rels[CONTEXT->rel_length++] = "NULL";
		(yyval.relation) = ( const char **)malloc(sizeof(const char*) * CONTEXT->rel_length);
		memcpy((yyval.relation), CONTEXT->rels, sizeof(const char*) * CONTEXT->rel_length);
		CONTEXT->rel_length = 0;
	}
#line 1999 "yacc_sql.tab.c"
    break;

  case 85: /* rel_list: %empty  */
#line 579 "yacc_sql.y"
                {}
#line 2005 "yacc_sql.tab.c"
    break;

  case 86: /* rel_list: COMMA ID rel_list  */
#line 580 "yacc_sql.y"
                        {	
		CONTEXT->rels[CONTEXT->rel_length++] = (yyvsp[-1].string);
	}
#line 2013 "yacc_sql.tab.c"
    break;

  case 87: /* where: %empty  */
#line 586 "yacc_sql.y"
                { 
		(yyval.condition1) = NULL; 
		// 这里不能清零，否则多个子查询条件时，子查询没有where会把主查询的condition清零 
	}
#line 2022 "yacc_sql.tab.c"
    break;

  case 88: /* where: WHERE condition condition_list  */
#line 590 "yacc_sql.y"
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
#line 2042 "yacc_sql.tab.c"
    break;

  case 90: /* on: ON condition condition_list  */
#line 609 "yacc_sql.y"
                                  {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
		selects_append_conditions_with_num(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);
		// 每次where结束都清零长度，多个子查询的where不会相互影响
		CONTEXT->condition_length = 0; 
		// 由于select里只有condition涉及到value_length，所以一并在此清零
		CONTEXT->value_length = 0;
	}
#line 2055 "yacc_sql.tab.c"
    break;

  case 92: /* condition_list: AND condition condition_list  */
#line 621 "yacc_sql.y"
                                   {
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 2063 "yacc_sql.tab.c"
    break;

  case 93: /* condition: ID comOp value  */
#line 628 "yacc_sql.y"
                {
			RelAttr left_attr;
			// $1 为属性名称
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);

			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 0, NULL, right_value, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
#line 2079 "yacc_sql.tab.c"
    break;

  case 94: /* condition: value comOp value  */
#line 640 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 0, NULL, left_value, 0, NULL, right_value, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

		}
#line 2093 "yacc_sql.tab.c"
    break;

  case 95: /* condition: ID comOp ID  */
#line 650 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 1, &right_attr, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

		}
#line 2109 "yacc_sql.tab.c"
    break;

  case 96: /* condition: value comOp ID  */
#line 662 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 0, NULL, left_value, 1, &right_attr, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
#line 2123 "yacc_sql.tab.c"
    break;

  case 97: /* condition: ID DOT ID comOp value  */
#line 672 "yacc_sql.y"
                {
			RelAttr left_attr;
			// $1为表名，$3为属性名
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string), NULL, 0);
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 0, NULL, right_value, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
    }
#line 2138 "yacc_sql.tab.c"
    break;

  case 98: /* condition: value comOp ID DOT ID  */
#line 683 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, (yyvsp[-3].number), 0, NULL, left_value, 1, &right_attr, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
    }
#line 2153 "yacc_sql.tab.c"
    break;

  case 99: /* condition: ID DOT ID comOp ID DOT ID  */
#line 694 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-6].string), (yyvsp[-4].string), NULL, 0);
			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, (yyvsp[-3].number), 1, &left_attr, NULL, 1, &right_attr, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
    }
#line 2168 "yacc_sql.tab.c"
    break;

  case 100: /* condition: ID IS NULL_T  */
#line 704 "yacc_sql.y"
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
#line 2185 "yacc_sql.tab.c"
    break;

  case 101: /* condition: ID IS NOT NULL_T  */
#line 716 "yacc_sql.y"
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
#line 2202 "yacc_sql.tab.c"
    break;

  case 102: /* condition: ID DOT ID IS NULL_T  */
#line 728 "yacc_sql.y"
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
#line 2218 "yacc_sql.tab.c"
    break;

  case 103: /* condition: ID DOT ID IS NOT NULL_T  */
#line 739 "yacc_sql.y"
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
#line 2234 "yacc_sql.tab.c"
    break;

  case 104: /* condition: value IS NOT NULL_T  */
#line 750 "yacc_sql.y"
                             { // null is null/value is not null
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NOT_NULL, 0, NULL, left_value, 0, NULL, right_value, NULL, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2248 "yacc_sql.tab.c"
    break;

  case 105: /* condition: value IS NULL_T  */
#line 759 "yacc_sql.y"
                         { //  null is not null/value is null
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NULL, 0, NULL, left_value, 0, NULL, right_value, NULL, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2262 "yacc_sql.tab.c"
    break;

  case 106: /* condition: ID comOp sub_select  */
#line 768 "yacc_sql.y"
                             {
		RelAttr left_attr;
		relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);

		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[0].selnode), NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2275 "yacc_sql.tab.c"
    break;

  case 107: /* condition: ID DOT ID comOp sub_select  */
#line 776 "yacc_sql.y"
                                     {
		RelAttr left_attr;
		relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string), NULL, 0);

		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[0].selnode), NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2288 "yacc_sql.tab.c"
    break;

  case 108: /* condition: value comOp sub_select  */
#line 784 "yacc_sql.y"
                                 {
		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), 0, NULL, left_value, 2, NULL, NULL, (yyvsp[0].selnode), NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2300 "yacc_sql.tab.c"
    break;

  case 109: /* condition: sub_select comOp value  */
#line 791 "yacc_sql.y"
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
#line 2319 "yacc_sql.tab.c"
    break;

  case 110: /* condition: sub_select comOp ID  */
#line 805 "yacc_sql.y"
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
#line 2339 "yacc_sql.tab.c"
    break;

  case 111: /* condition: sub_select comOp ID DOT ID  */
#line 820 "yacc_sql.y"
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
#line 2359 "yacc_sql.tab.c"
    break;

  case 112: /* condition: sub_select comOp sub_select  */
#line 835 "yacc_sql.y"
                                      {
		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), 2, NULL, NULL, 2, NULL, NULL, (yyvsp[0].selnode), (yyvsp[-2].selnode));
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2369 "yacc_sql.tab.c"
    break;

  case 113: /* comOp: EQ  */
#line 843 "yacc_sql.y"
             { (yyval.number) = 0; }
#line 2375 "yacc_sql.tab.c"
    break;

  case 114: /* comOp: LT  */
#line 844 "yacc_sql.y"
         { (yyval.number) = 3; }
#line 2381 "yacc_sql.tab.c"
    break;

  case 115: /* comOp: GT  */
#line 845 "yacc_sql.y"
         { (yyval.number) = 5; }
#line 2387 "yacc_sql.tab.c"
    break;

  case 116: /* comOp: LE  */
#line 846 "yacc_sql.y"
         { (yyval.number) = 2; }
#line 2393 "yacc_sql.tab.c"
    break;

  case 117: /* comOp: GE  */
#line 847 "yacc_sql.y"
         { (yyval.number) = 4; }
#line 2399 "yacc_sql.tab.c"
    break;

  case 118: /* comOp: NE  */
#line 848 "yacc_sql.y"
         { (yyval.number) = 1; }
#line 2405 "yacc_sql.tab.c"
    break;

  case 119: /* comOp: IN  */
#line 849 "yacc_sql.y"
             { (yyval.number) = 8; }
#line 2411 "yacc_sql.tab.c"
    break;

  case 120: /* comOp: NOT IN  */
#line 850 "yacc_sql.y"
                 { (yyval.number) = 9; }
#line 2417 "yacc_sql.tab.c"
    break;

  case 121: /* sub_select: LBRACE SELECT select_attr from_rel where group_by RBRACE  */
#line 854 "yacc_sql.y"
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
#line 2437 "yacc_sql.tab.c"
    break;

  case 122: /* group_by: %empty  */
#line 872 "yacc_sql.y"
                  {(yyval.relattr1) = NULL;}
#line 2443 "yacc_sql.tab.c"
    break;

  case 123: /* group_by: GROUP BY group_list  */
#line 873 "yacc_sql.y"
                              {
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);
		(yyval.relattr1) = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy((yyval.relattr1), CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
#line 2454 "yacc_sql.tab.c"
    break;

  case 124: /* group_list: group_attr  */
#line 882 "yacc_sql.y"
                  {
		;
	}
#line 2462 "yacc_sql.tab.c"
    break;

  case 125: /* group_list: group_list COMMA group_attr  */
#line 885 "yacc_sql.y"
                                      {}
#line 2468 "yacc_sql.tab.c"
    break;

  case 126: /* group_attr: ID  */
#line 889 "yacc_sql.y"
           {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL, 0);
		// selects_append_group(&CONTEXT->ssql->sstr.selection, &attr);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2479 "yacc_sql.tab.c"
    break;

  case 127: /* group_attr: ID DOT ID  */
#line 895 "yacc_sql.y"
                    {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);
		// selects_append_group(&CONTEXT->ssql->sstr.selection, &attr);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2490 "yacc_sql.tab.c"
    break;

  case 129: /* order_by: ORDER BY sort_list  */
#line 905 "yacc_sql.y"
                             {
	}
#line 2497 "yacc_sql.tab.c"
    break;

  case 130: /* sort_list: sort_attr  */
#line 910 "yacc_sql.y"
                  {
		// order by A, B, C，实际上加入顺序为C、B、A，方便后面排序
	}
#line 2505 "yacc_sql.tab.c"
    break;

  case 131: /* sort_list: sort_list COMMA sort_attr  */
#line 913 "yacc_sql.y"
                                    {}
#line 2511 "yacc_sql.tab.c"
    break;

  case 132: /* sort_attr: ID opt_asc  */
#line 916 "yacc_sql.y"
                  {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL, 0);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2521 "yacc_sql.tab.c"
    break;

  case 133: /* sort_attr: ID DESC  */
#line 921 "yacc_sql.y"
                  {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL, 1);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2531 "yacc_sql.tab.c"
    break;

  case 134: /* sort_attr: ID DOT ID opt_asc  */
#line 926 "yacc_sql.y"
                            {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL, 0);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2541 "yacc_sql.tab.c"
    break;

  case 135: /* sort_attr: ID DOT ID DESC  */
#line 931 "yacc_sql.y"
                         {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL, 1);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2551 "yacc_sql.tab.c"
    break;

  case 137: /* opt_asc: ASC  */
#line 939 "yacc_sql.y"
              {}
#line 2557 "yacc_sql.tab.c"
    break;

  case 138: /* load_data: LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON  */
#line 943 "yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 2566 "yacc_sql.tab.c"
    break;


#line 2570 "yacc_sql.tab.c"

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

#line 949 "yacc_sql.y"

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
