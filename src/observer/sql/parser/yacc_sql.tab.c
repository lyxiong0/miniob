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
#line 2 "yacc_sql.y"


#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.tab.h"
#include "sql/parser/lex.yy.h"
// #include "common/log/log.h" // 包含C++中的头文件

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct ParserContext {
  Query * ssql;
  size_t select_length;
  size_t condition_length;
  size_t from_length;
  size_t value_length;
  size_t insert_index;
  Value values[MAX_NUM];
  Condition conditions[MAX_NUM];
  CompOp comp;
	char id[MAX_NUM];
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
  context->from_length = 0;
  context->select_length = 0;
  context->value_length = 0;
  context->insert_index = 0;
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


#line 138 "yacc_sql.tab.c"

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
  YYSYMBOL_HELP = 29,                      /* HELP  */
  YYSYMBOL_EXIT = 30,                      /* EXIT  */
  YYSYMBOL_DOT = 31,                       /* DOT  */
  YYSYMBOL_INTO = 32,                      /* INTO  */
  YYSYMBOL_VALUES = 33,                    /* VALUES  */
  YYSYMBOL_FROM = 34,                      /* FROM  */
  YYSYMBOL_WHERE = 35,                     /* WHERE  */
  YYSYMBOL_AND = 36,                       /* AND  */
  YYSYMBOL_SET = 37,                       /* SET  */
  YYSYMBOL_ON = 38,                        /* ON  */
  YYSYMBOL_LOAD = 39,                      /* LOAD  */
  YYSYMBOL_DATA = 40,                      /* DATA  */
  YYSYMBOL_INFILE = 41,                    /* INFILE  */
  YYSYMBOL_NULLABLE = 42,                  /* NULLABLE  */
  YYSYMBOL_NOT = 43,                       /* NOT  */
  YYSYMBOL_EQ = 44,                        /* EQ  */
  YYSYMBOL_LT = 45,                        /* LT  */
  YYSYMBOL_GT = 46,                        /* GT  */
  YYSYMBOL_LE = 47,                        /* LE  */
  YYSYMBOL_GE = 48,                        /* GE  */
  YYSYMBOL_NE = 49,                        /* NE  */
  YYSYMBOL_NUMBER = 50,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 51,                     /* FLOAT  */
  YYSYMBOL_ID = 52,                        /* ID  */
  YYSYMBOL_PATH = 53,                      /* PATH  */
  YYSYMBOL_SSS = 54,                       /* SSS  */
  YYSYMBOL_STAR = 55,                      /* STAR  */
  YYSYMBOL_STRING_V = 56,                  /* STRING_V  */
  YYSYMBOL_COUNT = 57,                     /* COUNT  */
  YYSYMBOL_OTHER_FUNCTION_TYPE = 58,       /* OTHER_FUNCTION_TYPE  */
  YYSYMBOL_NULL_T = 59,                    /* NULL_T  */
  YYSYMBOL_YYACCEPT = 60,                  /* $accept  */
  YYSYMBOL_commands = 61,                  /* commands  */
  YYSYMBOL_command = 62,                   /* command  */
  YYSYMBOL_exit = 63,                      /* exit  */
  YYSYMBOL_help = 64,                      /* help  */
  YYSYMBOL_sync = 65,                      /* sync  */
  YYSYMBOL_begin = 66,                     /* begin  */
  YYSYMBOL_commit = 67,                    /* commit  */
  YYSYMBOL_rollback = 68,                  /* rollback  */
  YYSYMBOL_drop_table = 69,                /* drop_table  */
  YYSYMBOL_show_tables = 70,               /* show_tables  */
  YYSYMBOL_desc_table = 71,                /* desc_table  */
  YYSYMBOL_create_index = 72,              /* create_index  */
  YYSYMBOL_drop_index = 73,                /* drop_index  */
  YYSYMBOL_create_table = 74,              /* create_table  */
  YYSYMBOL_attr_def_list = 75,             /* attr_def_list  */
  YYSYMBOL_attr_def = 76,                  /* attr_def  */
  YYSYMBOL_opt_null = 77,                  /* opt_null  */
  YYSYMBOL_number = 78,                    /* number  */
  YYSYMBOL_type = 79,                      /* type  */
  YYSYMBOL_ID_get = 80,                    /* ID_get  */
  YYSYMBOL_insert = 81,                    /* insert  */
  YYSYMBOL_multi_values = 82,              /* multi_values  */
  YYSYMBOL_value_list = 83,                /* value_list  */
  YYSYMBOL_value = 84,                     /* value  */
  YYSYMBOL_delete = 85,                    /* delete  */
  YYSYMBOL_update = 86,                    /* update  */
  YYSYMBOL_select = 87,                    /* select  */
  YYSYMBOL_select_attr = 88,               /* select_attr  */
  YYSYMBOL_attr_list = 89,                 /* attr_list  */
  YYSYMBOL_window_function = 90,           /* window_function  */
  YYSYMBOL_opt_star = 91,                  /* opt_star  */
  YYSYMBOL_function_list = 92,             /* function_list  */
  YYSYMBOL_rel_list = 93,                  /* rel_list  */
  YYSYMBOL_where = 94,                     /* where  */
  YYSYMBOL_condition_list = 95,            /* condition_list  */
  YYSYMBOL_condition = 96,                 /* condition  */
  YYSYMBOL_comOp = 97,                     /* comOp  */
  YYSYMBOL_order_by = 98,                  /* order_by  */
  YYSYMBOL_sort_list = 99,                 /* sort_list  */
  YYSYMBOL_sort_attr = 100,                /* sort_attr  */
  YYSYMBOL_opt_asc = 101,                  /* opt_asc  */
  YYSYMBOL_load_data = 102                 /* load_data  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYLAST   204

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  219

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   314


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
      55,    56,    57,    58,    59
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   155,   155,   157,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   181,   186,   191,   197,   203,   209,   215,   221,   227,
     234,   242,   249,   258,   260,   264,   275,   289,   292,   295,
     301,   304,   308,   312,   316,   322,   331,   348,   355,   363,
     365,   370,   373,   376,   381,   389,   399,   409,   429,   434,
     439,   444,   449,   453,   455,   462,   471,   477,   483,   489,
     495,   501,   507,   515,   516,   518,   520,   524,   526,   530,
     532,   536,   538,   543,   565,   585,   605,   627,   649,   670,
     692,   693,   694,   695,   696,   697,   699,   701,   705,   708,
     711,   716,   721,   726,   732,   734,   737
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
  "ASC", "BY", "DATE_T", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM",
  "WHERE", "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "NULLABLE", "NOT",
  "EQ", "LT", "GT", "LE", "GE", "NE", "NUMBER", "FLOAT", "ID", "PATH",
  "SSS", "STAR", "STRING_V", "COUNT", "OTHER_FUNCTION_TYPE", "NULL_T",
  "$accept", "commands", "command", "exit", "help", "sync", "begin",
  "commit", "rollback", "drop_table", "show_tables", "desc_table",
  "create_index", "drop_index", "create_table", "attr_def_list",
  "attr_def", "opt_null", "number", "type", "ID_get", "insert",
  "multi_values", "value_list", "value", "delete", "update", "select",
  "select_attr", "attr_list", "window_function", "opt_star",
  "function_list", "rel_list", "where", "condition_list", "condition",
  "comOp", "order_by", "sort_list", "sort_attr", "opt_asc", "load_data", YY_NULLPTR
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
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314
};
#endif

#define YYPACT_NINF (-118)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -118,    11,  -118,    37,    55,    54,   -34,    40,    59,    47,
      60,    56,    97,   100,   104,   116,   118,    83,  -118,  -118,
    -118,  -118,  -118,  -118,  -118,  -118,  -118,  -118,  -118,  -118,
    -118,  -118,  -118,  -118,  -118,  -118,    72,    74,    75,    76,
     -12,  -118,   113,   114,    98,   115,   128,   131,  -118,    84,
      85,   101,  -118,  -118,  -118,  -118,  -118,    94,   123,   102,
     138,   139,    91,   -38,  -118,    70,    92,    93,   -49,  -118,
    -118,  -118,  -118,   117,   111,    95,    99,    84,    96,  -118,
    -118,    28,   133,   133,  -118,    22,  -118,   132,    25,   134,
     115,   140,    23,   151,   119,   125,   137,    48,   142,   107,
    -118,  -118,  -118,  -118,     2,  -118,  -118,    26,   108,   111,
    -118,    51,     9,  -118,  -118,    20,  -118,  -118,    69,   126,
    -118,    51,   155,    84,   147,  -118,  -118,  -118,  -118,    -9,
     120,   133,   148,   149,   150,   152,   134,   143,   153,  -118,
     154,   121,  -118,  -118,  -118,  -118,  -118,  -118,    33,    39,
      23,  -118,   111,   122,   137,   172,   127,  -118,   124,  -118,
     159,  -118,  -118,  -118,  -118,  -118,  -118,   157,   175,    51,
     162,    51,    69,  -118,  -118,   156,  -118,   126,   177,   178,
    -118,  -118,  -118,   165,  -118,   182,   136,  -118,   153,  -118,
     153,    45,   141,  -118,  -118,  -118,    -7,  -118,    18,   168,
    -118,  -118,   173,   158,  -118,  -118,  -118,  -118,  -118,   144,
    -118,   136,  -118,   145,     3,  -118,  -118,  -118,  -118
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,    20,
      19,    14,    15,    16,    17,     9,    10,    11,    12,    13,
       8,     5,     7,     6,     4,    18,     0,     0,     0,     0,
      63,    58,     0,     0,     0,    75,     0,     0,    23,     0,
       0,     0,    24,    25,    26,    22,    21,     0,     0,     0,
       0,     0,     0,     0,    59,     0,     0,     0,     0,    62,
      29,    28,    45,     0,    79,     0,     0,     0,     0,    27,
      31,    63,    63,    63,    74,     0,    73,     0,     0,    77,
      75,     0,     0,     0,     0,     0,    33,     0,     0,     0,
      64,    60,    61,    67,     0,    66,    70,     0,     0,    79,
      76,     0,     0,    51,    52,     0,    54,    53,     0,    81,
      55,     0,     0,     0,     0,    41,    42,    43,    44,    37,
       0,    63,     0,     0,     0,     0,    77,    96,    49,    46,
       0,     0,    90,    91,    92,    93,    94,    95,     0,     0,
       0,    80,    79,     0,    33,     0,     0,    39,     0,    36,
       0,    65,    68,    69,    71,    72,    78,     0,     0,     0,
       0,     0,     0,    85,    83,    86,    84,    81,     0,     0,
      34,    32,    40,     0,    38,     0,     0,    57,    49,    47,
      49,     0,     0,    82,    56,   106,    37,    30,   104,    97,
      98,    50,     0,     0,    87,    88,    35,   101,   105,     0,
     100,     0,    48,     0,   104,    99,    89,   103,   102
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -118,  -118,  -118,  -118,  -118,  -118,  -118,  -118,  -118,  -118,
    -118,  -118,  -118,  -118,  -118,    38,    68,    -2,  -118,  -118,
     146,  -118,  -118,  -102,  -111,  -118,  -118,  -118,  -118,   -79,
     130,  -118,   109,    64,  -104,    24,    52,  -117,  -118,  -118,
      -8,   -10,  -118
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,   124,    96,   159,   183,   129,
      97,    31,   112,   170,   118,    32,    33,    34,    44,    64,
      45,    87,    69,   109,    93,   151,   119,   148,   168,   199,
     200,   210,    35
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
     138,   149,   100,   101,   102,   137,    62,   156,    42,    43,
     152,     2,   139,   217,    82,     3,     4,    83,    46,    63,
       5,     6,     7,     8,     9,    10,    11,   140,   207,   208,
      12,    13,    14,   157,   158,   157,   158,   174,   176,   103,
      15,    16,   106,    36,   208,    37,    62,    47,   178,   209,
      17,   141,   161,   104,   132,   191,   107,   133,   188,    99,
     190,    38,    48,    39,   142,   143,   144,   145,   146,   147,
     125,   126,   127,   113,   114,   115,   128,   116,   134,    49,
     204,   135,   117,   113,   114,   173,   201,   116,   202,   113,
     114,   175,   117,   116,    50,   113,   114,   203,   117,   116,
      52,   113,   114,    53,   117,   116,    40,    54,    51,    41,
     117,    42,    43,   142,   143,   144,   145,   146,   147,    55,
      84,    56,    85,    57,    58,    86,    59,    60,    61,    65,
      66,    70,    67,    68,    71,    76,    72,    74,    75,    77,
      78,    79,    80,    81,    88,    89,    92,    94,    98,   105,
      91,    62,   108,    95,   120,   123,   111,   122,   130,   131,
     136,   153,   150,   121,   155,   162,   163,   164,   167,   165,
     171,   169,   160,   172,   179,   181,   185,   182,   187,   189,
     194,   195,   196,   184,   186,   197,   211,   192,   198,   213,
     212,   154,   180,   205,   206,    73,   214,   216,    90,   110,
     166,   193,   177,   215,   218
};

static const yytype_uint8 yycheck[] =
{
     111,   118,    81,    82,    83,   109,    18,    16,    57,    58,
     121,     0,     3,    10,    52,     4,     5,    55,    52,    31,
       9,    10,    11,    12,    13,    14,    15,    18,    10,    26,
      19,    20,    21,    42,    43,    42,    43,   148,   149,    17,
      29,    30,    17,     6,    26,     8,    18,     7,   152,    31,
      39,    31,   131,    31,    52,   172,    31,    55,   169,    31,
     171,     6,     3,     8,    44,    45,    46,    47,    48,    49,
      22,    23,    24,    50,    51,    52,    28,    54,    52,    32,
     191,    55,    59,    50,    51,    52,   188,    54,   190,    50,
      51,    52,    59,    54,    34,    50,    51,    52,    59,    54,
       3,    50,    51,     3,    59,    54,    52,     3,    52,    55,
      59,    57,    58,    44,    45,    46,    47,    48,    49,     3,
      50,     3,    52,    40,    52,    55,    52,    52,    52,    16,
      16,     3,    34,    18,     3,    41,    52,    52,    37,    16,
      38,     3,     3,    52,    52,    52,    35,    52,    52,    17,
      33,    18,    18,    54,     3,    18,    16,    32,    16,    52,
      52,     6,    36,    44,    17,    17,    17,    17,    25,    17,
      16,    18,    52,    52,    52,     3,    17,    50,     3,    17,
       3,     3,    17,    59,    27,     3,    18,    31,    52,    31,
      17,   123,   154,    52,   196,    49,    52,    52,    68,    90,
     136,   177,   150,   211,   214
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    61,     0,     4,     5,     9,    10,    11,    12,    13,
      14,    15,    19,    20,    21,    29,    30,    39,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    81,    85,    86,    87,   102,     6,     8,     6,     8,
      52,    55,    57,    58,    88,    90,    52,     7,     3,    32,
      34,    52,     3,     3,     3,     3,     3,    40,    52,    52,
      52,    52,    18,    31,    89,    16,    16,    34,    18,    92,
       3,     3,    52,    80,    52,    37,    41,    16,    38,     3,
       3,    52,    52,    55,    50,    52,    55,    91,    52,    52,
      90,    33,    35,    94,    52,    54,    76,    80,    52,    31,
      89,    89,    89,    17,    31,    17,    17,    31,    18,    93,
      92,    16,    82,    50,    51,    52,    54,    59,    84,    96,
       3,    44,    32,    18,    75,    22,    23,    24,    28,    79,
      16,    52,    52,    55,    52,    55,    52,    94,    84,     3,
      18,    31,    44,    45,    46,    47,    48,    49,    97,    97,
      36,    95,    84,     6,    76,    17,    16,    42,    43,    77,
      52,    89,    17,    17,    17,    17,    93,    25,    98,    18,
      83,    16,    52,    52,    84,    52,    84,    96,    94,    52,
      75,     3,    50,    78,    59,    17,    27,     3,    84,    17,
      84,    97,    31,    95,     3,     3,    17,     3,    52,    99,
     100,    83,    83,    52,    84,    52,    77,    10,    26,    31,
     101,    18,    17,    31,    52,   100,    52,    10,   101
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    60,    61,    61,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    75,    76,    76,    77,    77,    77,
      78,    79,    79,    79,    79,    80,    81,    82,    82,    83,
      83,    84,    84,    84,    84,    85,    86,    87,    88,    88,
      88,    88,    88,    89,    89,    89,    90,    90,    90,    90,
      90,    90,    90,    91,    91,    92,    92,    93,    93,    94,
      94,    95,    95,    96,    96,    96,    96,    96,    96,    96,
      97,    97,    97,    97,    97,    97,    98,    98,    99,    99,
     100,   100,   100,   100,   101,   101,   102
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
       9,     4,     8,     0,     3,     6,     3,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     6,     4,     6,     0,
       3,     1,     1,     1,     1,     5,     8,     8,     1,     2,
       4,     4,     2,     0,     3,     5,     4,     4,     6,     6,
       4,     6,     6,     1,     1,     0,     3,     0,     3,     0,
       3,     0,     3,     3,     3,     3,     3,     5,     5,     7,
       1,     1,     1,     1,     1,     1,     0,     3,     1,     3,
       2,     2,     4,     4,     0,     1,     8
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
#line 181 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1384 "yacc_sql.tab.c"
    break;

  case 22: /* help: HELP SEMICOLON  */
#line 186 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1392 "yacc_sql.tab.c"
    break;

  case 23: /* sync: SYNC SEMICOLON  */
#line 191 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1400 "yacc_sql.tab.c"
    break;

  case 24: /* begin: TRX_BEGIN SEMICOLON  */
#line 197 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1408 "yacc_sql.tab.c"
    break;

  case 25: /* commit: TRX_COMMIT SEMICOLON  */
#line 203 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1416 "yacc_sql.tab.c"
    break;

  case 26: /* rollback: TRX_ROLLBACK SEMICOLON  */
#line 209 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1424 "yacc_sql.tab.c"
    break;

  case 27: /* drop_table: DROP TABLE ID SEMICOLON  */
#line 215 "yacc_sql.y"
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1433 "yacc_sql.tab.c"
    break;

  case 28: /* show_tables: SHOW TABLES SEMICOLON  */
#line 221 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1441 "yacc_sql.tab.c"
    break;

  case 29: /* desc_table: DESC ID SEMICOLON  */
#line 227 "yacc_sql.y"
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1450 "yacc_sql.tab.c"
    break;

  case 30: /* create_index: CREATE INDEX ID ON ID LBRACE ID RBRACE SEMICOLON  */
#line 235 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string));
		}
#line 1459 "yacc_sql.tab.c"
    break;

  case 31: /* drop_index: DROP INDEX ID SEMICOLON  */
#line 243 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1468 "yacc_sql.tab.c"
    break;

  case 32: /* create_table: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON  */
#line 250 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
#line 1480 "yacc_sql.tab.c"
    break;

  case 34: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 260 "yacc_sql.y"
                                   {    }
#line 1486 "yacc_sql.tab.c"
    break;

  case 35: /* attr_def: ID_get type LBRACE number RBRACE opt_null  */
#line 265 "yacc_sql.y"
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
#line 1501 "yacc_sql.tab.c"
    break;

  case 36: /* attr_def: ID_get type opt_null  */
#line 276 "yacc_sql.y"
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
#line 1516 "yacc_sql.tab.c"
    break;

  case 37: /* opt_null: %empty  */
#line 289 "yacc_sql.y"
                  {
		(yyval.number) = ISFALSE; // 默认允许null
	}
#line 1524 "yacc_sql.tab.c"
    break;

  case 38: /* opt_null: NOT NULL_T  */
#line 292 "yacc_sql.y"
                     {
		(yyval.number) = ISFALSE;
	}
#line 1532 "yacc_sql.tab.c"
    break;

  case 39: /* opt_null: NULLABLE  */
#line 295 "yacc_sql.y"
                   {
		(yyval.number) = ISTRUE;
	}
#line 1540 "yacc_sql.tab.c"
    break;

  case 40: /* number: NUMBER  */
#line 301 "yacc_sql.y"
                       {(yyval.number) = (yyvsp[0].number);}
#line 1546 "yacc_sql.tab.c"
    break;

  case 41: /* type: INT_T  */
#line 304 "yacc_sql.y"
              { 
		(yyval.number)=INTS; 
		// printf("CREATE 语句语法解析 type 为 INTS\n");
	}
#line 1555 "yacc_sql.tab.c"
    break;

  case 42: /* type: STRING_T  */
#line 308 "yacc_sql.y"
                  { 
		   (yyval.number)=CHARS;
		// printf("CREATE 语句语法解析 type 为 STRING_T\n");
	}
#line 1564 "yacc_sql.tab.c"
    break;

  case 43: /* type: FLOAT_T  */
#line 312 "yacc_sql.y"
                 { 
		   (yyval.number)=FLOATS;
		// printf("CREATE 语句语法解析 type 为 FLOAT_T\n");
	}
#line 1573 "yacc_sql.tab.c"
    break;

  case 44: /* type: DATE_T  */
#line 316 "yacc_sql.y"
                    { 
		   (yyval.number)=DATES;
		// printf("CREATE 语句语法解析 type 为 DATE_T\n");
	}
#line 1582 "yacc_sql.tab.c"
    break;

  case 45: /* ID_get: ID  */
#line 323 "yacc_sql.y"
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1591 "yacc_sql.tab.c"
    break;

  case 46: /* insert: INSERT INTO ID_get VALUES multi_values SEMICOLON  */
#line 332 "yacc_sql.y"
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
#line 1610 "yacc_sql.tab.c"
    break;

  case 47: /* multi_values: LBRACE value value_list RBRACE  */
#line 348 "yacc_sql.y"
                                       {
		// 到此结束一组的插入：存储该组、增加index、value_length清零
		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
		CONTEXT->insert_index++;
		//临时变量清零
      	CONTEXT->value_length=0;
	}
#line 1622 "yacc_sql.tab.c"
    break;

  case 48: /* multi_values: multi_values COMMA LBRACE value value_list RBRACE  */
#line 355 "yacc_sql.y"
                                                           {
		// 到此结束一组的插入：存储该组、增加index、value_length清零
		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
		CONTEXT->insert_index++;
		//临时变量清零
      	CONTEXT->value_length=0;
	}
#line 1634 "yacc_sql.tab.c"
    break;

  case 50: /* value_list: COMMA value value_list  */
#line 365 "yacc_sql.y"
                              { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1642 "yacc_sql.tab.c"
    break;

  case 51: /* value: NUMBER  */
#line 370 "yacc_sql.y"
          {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number));
	}
#line 1650 "yacc_sql.tab.c"
    break;

  case 52: /* value: FLOAT  */
#line 373 "yacc_sql.y"
          {
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].floats));
	}
#line 1658 "yacc_sql.tab.c"
    break;

  case 53: /* value: NULL_T  */
#line 376 "yacc_sql.y"
                {
		// null不需要加双引号，当作字符串插入
		printf("初始化null值");
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
	}
#line 1668 "yacc_sql.tab.c"
    break;

  case 54: /* value: SSS  */
#line 381 "yacc_sql.y"
         {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
		}
#line 1677 "yacc_sql.tab.c"
    break;

  case 55: /* delete: DELETE FROM ID where SEMICOLON  */
#line 390 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;	
    }
#line 1689 "yacc_sql.tab.c"
    break;

  case 56: /* update: UPDATE ID SET ID EQ value where SEMICOLON  */
#line 400 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
		}
#line 1701 "yacc_sql.tab.c"
    break;

  case 57: /* select: SELECT select_attr FROM ID rel_list where order_by SEMICOLON  */
#line 410 "yacc_sql.y"
        {
			CONTEXT->ssql->flag=SCF_SELECT;//"select";

			// CONTEXT->ssql->sstr.selection.relations[CONTEXT->from_length++]=$4;
			selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-4].string));

			//selects_append_conditions(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);
			selects_append_conditions(CONTEXT->ssql, CONTEXT->conditions, CONTEXT->condition_length);
			
			// CONTEXT->ssql->sstr.selection.attr_num = CONTEXT->select_length;
			
			//临时变量清零
			CONTEXT->condition_length=0;
			CONTEXT->from_length=0;
			CONTEXT->select_length=0;
			CONTEXT->value_length = 0;
	}
#line 1723 "yacc_sql.tab.c"
    break;

  case 58: /* select_attr: STAR  */
#line 429 "yacc_sql.y"
         {  // select *
			RelAttr attr;
			relation_attr_init(&attr, NULL, "*", NULL, 0);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1733 "yacc_sql.tab.c"
    break;

  case 59: /* select_attr: ID attr_list  */
#line 434 "yacc_sql.y"
                   { // select age
			RelAttr attr;
			relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL, 0);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1743 "yacc_sql.tab.c"
    break;

  case 60: /* select_attr: ID DOT ID attr_list  */
#line 439 "yacc_sql.y"
                              { // select t1.age
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL, 0);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1753 "yacc_sql.tab.c"
    break;

  case 61: /* select_attr: ID DOT STAR attr_list  */
#line 444 "yacc_sql.y"
                                { // select t1.age
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-3].string), "*", NULL, 0);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1763 "yacc_sql.tab.c"
    break;

  case 62: /* select_attr: window_function function_list  */
#line 449 "yacc_sql.y"
                                        {
		// 放到window_function里执行
	}
#line 1771 "yacc_sql.tab.c"
    break;

  case 64: /* attr_list: COMMA ID attr_list  */
#line 455 "yacc_sql.y"
                         { // .., id
			RelAttr attr;
			relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL, 0);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
     	  // CONTEXT->ssql->sstr.selection.attributes[CONTEXT->select_length].relation_name = NULL;
        // CONTEXT->ssql->sstr.selection.attributes[CONTEXT->select_length++].attribute_name=$2;
      }
#line 1783 "yacc_sql.tab.c"
    break;

  case 65: /* attr_list: COMMA ID DOT ID attr_list  */
#line 462 "yacc_sql.y"
                                {
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL, 0);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
        // CONTEXT->ssql->sstr.selection.attributes[CONTEXT->select_length].attribute_name=$4;
        // CONTEXT->ssql->sstr.selection.attributes[CONTEXT->select_length++].relation_name=$2;
  	  }
#line 1795 "yacc_sql.tab.c"
    break;

  case 66: /* window_function: COUNT LBRACE opt_star RBRACE  */
#line 472 "yacc_sql.y"
        {	// 只有COUNT允许COUNT(*)
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string), 0);
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 1805 "yacc_sql.tab.c"
    break;

  case 67: /* window_function: COUNT LBRACE ID RBRACE  */
#line 478 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string), 0);
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 1815 "yacc_sql.tab.c"
    break;

  case 68: /* window_function: COUNT LBRACE ID DOT ID RBRACE  */
#line 484 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 1825 "yacc_sql.tab.c"
    break;

  case 69: /* window_function: COUNT LBRACE ID DOT STAR RBRACE  */
#line 490 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), "*", (yyvsp[-5].string), 0);
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 1835 "yacc_sql.tab.c"
    break;

  case 70: /* window_function: OTHER_FUNCTION_TYPE LBRACE ID RBRACE  */
#line 496 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string), 0);
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 1845 "yacc_sql.tab.c"
    break;

  case 71: /* window_function: OTHER_FUNCTION_TYPE LBRACE ID DOT ID RBRACE  */
#line 502 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 1855 "yacc_sql.tab.c"
    break;

  case 72: /* window_function: OTHER_FUNCTION_TYPE LBRACE ID DOT STAR RBRACE  */
#line 508 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), "*", (yyvsp[-5].string), 0);
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 1865 "yacc_sql.tab.c"
    break;

  case 73: /* opt_star: STAR  */
#line 515 "yacc_sql.y"
             { (yyval.string) = (yyvsp[0].string);}
#line 1871 "yacc_sql.tab.c"
    break;

  case 74: /* opt_star: NUMBER  */
#line 516 "yacc_sql.y"
                 {(yyval.string) = number_to_str((yyvsp[0].number));}
#line 1877 "yacc_sql.tab.c"
    break;

  case 76: /* function_list: COMMA window_function function_list  */
#line 520 "yacc_sql.y"
                                          { // .., id
		// 不操作，留给window_function执行
      }
#line 1885 "yacc_sql.tab.c"
    break;

  case 78: /* rel_list: COMMA ID rel_list  */
#line 526 "yacc_sql.y"
                        {	
				selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-1].string));
		  }
#line 1893 "yacc_sql.tab.c"
    break;

  case 80: /* where: WHERE condition condition_list  */
#line 532 "yacc_sql.y"
                                     {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 1901 "yacc_sql.tab.c"
    break;

  case 82: /* condition_list: AND condition condition_list  */
#line 538 "yacc_sql.y"
                                   {
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 1909 "yacc_sql.tab.c"
    break;

  case 83: /* condition: ID comOp value  */
#line 544 "yacc_sql.y"
                {
			RelAttr left_attr;
			// $1 为属性名称
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);

			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$ = ( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name = NULL;
			// $$->left_attr.attribute_name= $1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 0;
			// $$->right_attr.relation_name = NULL;
			// $$->right_attr.attribute_name = NULL;
			// $$->right_value = *$3;

		}
#line 1935 "yacc_sql.tab.c"
    break;

  case 84: /* condition: value comOp value  */
#line 566 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$ = ( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 0;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=NULL;
			// $$->left_value = *$1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 0;
			// $$->right_attr.relation_name = NULL;
			// $$->right_attr.attribute_name = NULL;
			// $$->right_value = *$3;

		}
#line 1959 "yacc_sql.tab.c"
    break;

  case 85: /* condition: ID comOp ID  */
#line 586 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=$1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 1;
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=$3;

		}
#line 1983 "yacc_sql.tab.c"
    break;

  case 86: /* condition: value comOp ID  */
#line 606 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 0;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=NULL;
			// $$->left_value = *$1;
			// $$->comp=CONTEXT->comp;
			
			// $$->right_is_attr = 1;
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=$3;
		
		}
#line 2009 "yacc_sql.tab.c"
    break;

  case 87: /* condition: ID DOT ID comOp value  */
#line 628 "yacc_sql.y"
                {
			RelAttr left_attr;
			// $1为表名，$3为属性名
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string), NULL, 0);
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name=$1;
			// $$->left_attr.attribute_name=$3;
			// $$->comp=CONTEXT->comp;
			// $$->right_is_attr = 0;   //属性值
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=NULL;
			// $$->right_value =*$5;			
							
    }
#line 2035 "yacc_sql.tab.c"
    break;

  case 88: /* condition: value comOp ID DOT ID  */
#line 650 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 0;//属性值
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=NULL;
			// $$->left_value = *$1;
			// $$->comp =CONTEXT->comp;
			// $$->right_is_attr = 1;//属性
			// $$->right_attr.relation_name = $3;
			// $$->right_attr.attribute_name = $5;
									
    }
#line 2060 "yacc_sql.tab.c"
    break;

  case 89: /* condition: ID DOT ID comOp ID DOT ID  */
#line 671 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-6].string), (yyvsp[-4].string), NULL, 0);
			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;		//属性
			// $$->left_attr.relation_name=$1;
			// $$->left_attr.attribute_name=$3;
			// $$->comp =CONTEXT->comp;
			// $$->right_is_attr = 1;		//属性
			// $$->right_attr.relation_name=$5;
			// $$->right_attr.attribute_name=$7;
    }
#line 2083 "yacc_sql.tab.c"
    break;

  case 90: /* comOp: EQ  */
#line 692 "yacc_sql.y"
             { CONTEXT->comp = EQUAL_TO; }
#line 2089 "yacc_sql.tab.c"
    break;

  case 91: /* comOp: LT  */
#line 693 "yacc_sql.y"
         { CONTEXT->comp = LESS_THAN; }
#line 2095 "yacc_sql.tab.c"
    break;

  case 92: /* comOp: GT  */
#line 694 "yacc_sql.y"
         { CONTEXT->comp = GREAT_THAN; }
#line 2101 "yacc_sql.tab.c"
    break;

  case 93: /* comOp: LE  */
#line 695 "yacc_sql.y"
         { CONTEXT->comp = LESS_EQUAL; }
#line 2107 "yacc_sql.tab.c"
    break;

  case 94: /* comOp: GE  */
#line 696 "yacc_sql.y"
         { CONTEXT->comp = GREAT_EQUAL; }
#line 2113 "yacc_sql.tab.c"
    break;

  case 95: /* comOp: NE  */
#line 697 "yacc_sql.y"
         { CONTEXT->comp = NOT_EQUAL; }
#line 2119 "yacc_sql.tab.c"
    break;

  case 97: /* order_by: ORDER BY sort_list  */
#line 701 "yacc_sql.y"
                             {
	}
#line 2126 "yacc_sql.tab.c"
    break;

  case 98: /* sort_list: sort_attr  */
#line 705 "yacc_sql.y"
                  {
		// order by A, B, C，实际上加入顺序为C、B、A，方便后面排序
	}
#line 2134 "yacc_sql.tab.c"
    break;

  case 99: /* sort_list: sort_list COMMA sort_attr  */
#line 708 "yacc_sql.y"
                                    {}
#line 2140 "yacc_sql.tab.c"
    break;

  case 100: /* sort_attr: ID opt_asc  */
#line 711 "yacc_sql.y"
                  {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL, 0);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2150 "yacc_sql.tab.c"
    break;

  case 101: /* sort_attr: ID DESC  */
#line 716 "yacc_sql.y"
                  {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL, 1);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2160 "yacc_sql.tab.c"
    break;

  case 102: /* sort_attr: ID DOT ID opt_asc  */
#line 721 "yacc_sql.y"
                            {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL, 0);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2170 "yacc_sql.tab.c"
    break;

  case 103: /* sort_attr: ID DOT ID DESC  */
#line 726 "yacc_sql.y"
                         {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL, 1);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2180 "yacc_sql.tab.c"
    break;

  case 105: /* opt_asc: ASC  */
#line 734 "yacc_sql.y"
              {}
#line 2186 "yacc_sql.tab.c"
    break;

  case 106: /* load_data: LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON  */
#line 738 "yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 2195 "yacc_sql.tab.c"
    break;


#line 2199 "yacc_sql.tab.c"

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

#line 743 "yacc_sql.y"

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
