/* A Bison parser, made by GNU Bison 3.5.1.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

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
  size_t exp_length;

  Value values[MAX_NUM];
  Condition conditions[MAX_NUM];
  char id[MAX_NUM];
  const char *rels[MAX_NUM];
  const char *exps[50];
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


#line 147 "yacc_sql.tab.c"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_YACC_SQL_TAB_H_INCLUDED
# define YY_YY_YACC_SQL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SEMICOLON = 258,
    CREATE = 259,
    DROP = 260,
    TABLE = 261,
    TABLES = 262,
    INDEX = 263,
    SELECT = 264,
    DESC = 265,
    SHOW = 266,
    SYNC = 267,
    INSERT = 268,
    DELETE = 269,
    UPDATE = 270,
    LBRACE = 271,
    RBRACE = 272,
    COMMA = 273,
    TRX_BEGIN = 274,
    TRX_COMMIT = 275,
    TRX_ROLLBACK = 276,
    INT_T = 277,
    STRING_T = 278,
    FLOAT_T = 279,
    ORDER = 280,
    ASC = 281,
    BY = 282,
    DATE_T = 283,
    UNIQUE = 284,
    HELP = 285,
    EXIT = 286,
    DOT = 287,
    INTO = 288,
    VALUES = 289,
    FROM = 290,
    WHERE = 291,
    AND = 292,
    SET = 293,
    ON = 294,
    LOAD = 295,
    DATA = 296,
    INFILE = 297,
    NULLABLE = 298,
    GROUP = 299,
    IS = 300,
    NOT = 301,
    EQ = 302,
    LT = 303,
    GT = 304,
    LE = 305,
    GE = 306,
    NE = 307,
    PLUS = 308,
    DIV = 309,
    NULL_T = 310,
    INNER = 311,
    JOIN = 312,
    IN = 313,
    MINUS = 314,
    TEXT_T = 315,
    NUMBER = 316,
    FLOAT = 317,
    ID = 318,
    PATH = 319,
    SSS = 320,
    STAR = 321,
    STRING_V = 322,
    COUNT = 323,
    OTHER_FUNCTION_TYPE = 324,
    Column = 325,
    LOWER_THAN_BRACE = 326,
    GR = 327
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 142 "yacc_sql.y"

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

#line 286 "yacc_sql.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (void *scanner);

#endif /* !YY_YY_YACC_SQL_TAB_H_INCLUDED  */



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

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


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
#define YYLAST   270

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  74
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  60
/* YYNRULES -- Number of rules.  */
#define YYNRULES  147
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  279

#define YYUNDEFTOK  2
#define YYMAXUTOK   327


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    71,     2,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70,    72,    73
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   195,   195,   197,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   221,   226,   231,   237,   243,   249,   255,   261,   267,
     274,   280,   287,   289,   291,   292,   297,   304,   314,   316,
     320,   331,   341,   344,   347,   353,   359,   363,   367,   371,
     374,   379,   388,   405,   412,   420,   422,   428,   434,   440,
     445,   457,   469,   482,   499,   509,   519,   521,   527,   534,
     543,   545,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   562,   565,   571,   574,   580,   586,   590,   594,   595,
     602,   611,   620,   631,   633,   636,   638,   644,   650,   656,
     662,   668,   674,   683,   684,   685,   689,   699,   700,   706,
     710,   727,   729,   739,   741,   747,   769,   778,   793,   809,
     817,   818,   819,   820,   821,   822,   823,   824,   825,   826,
     830,   848,   849,   858,   861,   865,   871,   879,   881,   886,
     889,   892,   897,   902,   907,   913,   915,   918
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEMICOLON", "CREATE", "DROP", "TABLE",
  "TABLES", "INDEX", "SELECT", "DESC", "SHOW", "SYNC", "INSERT", "DELETE",
  "UPDATE", "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN", "TRX_COMMIT",
  "TRX_ROLLBACK", "INT_T", "STRING_T", "FLOAT_T", "ORDER", "ASC", "BY",
  "DATE_T", "UNIQUE", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM",
  "WHERE", "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "NULLABLE",
  "GROUP", "IS", "NOT", "EQ", "LT", "GT", "LE", "GE", "NE", "PLUS", "DIV",
  "NULL_T", "INNER", "JOIN", "IN", "MINUS", "TEXT_T", "NUMBER", "FLOAT",
  "ID", "PATH", "SSS", "STAR", "STRING_V", "COUNT", "OTHER_FUNCTION_TYPE",
  "Column", "'-'", "LOWER_THAN_BRACE", "GR", "$accept", "commands",
  "command", "exit", "help", "sync", "begin", "commit", "rollback",
  "drop_table", "show_tables", "desc_table", "create_index", "Column_list",
  "Column_def", "drop_index", "create_table", "attr_def_list", "attr_def",
  "opt_null", "number", "type", "ID_get", "insert", "multi_values",
  "value_list", "value", "delete", "update", "select", "select_attr",
  "select_param", "expression", "exp_list", "exp", "lbrace_list",
  "rbrace_list", "minus", "op", "id_type", "attr_list", "join_list",
  "window_function", "opt_star", "from_rel", "rel_list", "where", "on",
  "condition_list", "condition", "comOp", "sub_select", "group_by",
  "group_list", "group_attr", "order_by", "sort_list", "sort_attr",
  "opt_asc", "load_data", YY_NULLPTR
};
#endif

# ifdef YYPRINT
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
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,    45,   326,   327
};
# endif

#define YYPACT_NINF (-218)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-65)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -218,   145,  -218,     5,    57,   -12,   -37,    20,    43,    61,
      65,    33,   101,   103,   109,   119,   120,    83,  -218,  -218,
    -218,  -218,  -218,  -218,  -218,  -218,  -218,  -218,  -218,  -218,
    -218,  -218,  -218,  -218,  -218,  -218,    72,    79,   136,    84,
      85,  -218,  -218,  -218,  -218,  -218,  -218,  -218,    96,  -218,
     111,   147,   153,   135,   137,   152,  -218,  -218,   108,    48,
    -218,    71,   135,  -218,   170,   174,  -218,   115,   116,   142,
    -218,  -218,  -218,  -218,  -218,   139,   166,   144,   121,   183,
     184,   -43,   -42,   125,  -218,   172,   127,   138,    14,  -218,
    -218,  -218,  -218,   135,    23,   135,   108,   172,  -218,  -218,
    -218,   157,   156,   130,   131,   115,   132,   158,  -218,  -218,
    -218,  -218,  -218,   167,  -218,   181,    -3,  -218,   182,   146,
     156,   152,   172,   135,   172,  -218,   185,    36,   199,   159,
     171,   187,   -16,   191,   148,   -41,  -218,  -218,    77,   149,
    -218,   150,   164,  -218,   172,    23,    42,   200,    69,   173,
      69,  -218,    23,   208,   115,   198,  -218,  -218,  -218,  -218,
    -218,    12,   154,   202,   203,   204,   205,   206,   182,   177,
     192,   201,   207,  -218,   211,   -12,   178,   175,  -218,  -218,
    -218,  -218,  -218,  -218,  -218,    36,    36,  -218,    76,   156,
     165,   187,   226,   169,  -218,   176,  -218,  -218,   214,   154,
    -218,  -218,  -218,  -218,  -218,    36,   138,   179,   209,   231,
      23,   218,    23,   137,  -218,  -218,  -218,  -218,   173,   200,
    -218,  -218,  -218,   234,   235,  -218,  -218,  -218,   222,  -218,
     154,   223,   214,   173,  -218,   212,   225,  -218,   186,  -218,
     207,  -218,   207,   156,  -218,  -218,  -218,    62,   214,   238,
     228,  -218,   188,   179,     7,   229,  -218,  -218,   233,   164,
    -218,  -218,  -218,   243,  -218,  -218,  -218,  -218,   189,  -218,
     186,  -218,   236,  -218,     6,  -218,  -218,  -218,  -218
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,    20,
      19,    14,    15,    16,    17,     9,    10,    11,    12,    13,
       8,     5,     7,     6,     4,    18,     0,     0,     0,     0,
       0,    81,    87,    89,    59,    85,    57,    58,    90,    60,
      86,     0,     0,    73,     0,    93,    67,    69,    70,     0,
      88,     0,    72,    66,     0,     0,    23,     0,     0,     0,
      24,    25,    26,    22,    21,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    77,     0,    95,    70,    65,
      86,    68,    82,    75,     0,    74,    70,    76,    29,    28,
      51,     0,   109,     0,     0,     0,     0,     0,    27,    36,
      91,    92,   104,   105,   103,     0,     0,    84,   107,     0,
     109,    93,    79,     0,    78,    71,     0,    70,     0,     0,
       0,    38,     0,     0,     0,     0,    97,   100,     0,     0,
     106,     0,   131,    94,    80,     0,     0,    81,     0,   113,
       0,    61,     0,     0,     0,     0,    46,    47,    48,    49,
      50,    42,    34,     0,     0,     0,     0,     0,   107,   111,
       0,   137,    55,    52,     0,    70,   128,     0,   120,   121,
     122,   123,   124,   125,   126,    70,    70,   110,     0,   109,
       0,    38,     0,     0,    44,     0,    41,    35,    32,    34,
      98,    99,   101,   102,   108,    70,    95,     0,     0,     0,
       0,     0,     0,     0,   129,   127,   115,   116,   113,     0,
     117,   118,   119,     0,     0,    39,    37,    45,     0,    43,
      34,     0,    32,   113,    96,   135,   132,   133,     0,    63,
      55,    53,    55,   109,   114,    62,   147,    42,    32,     0,
       0,   112,     0,     0,   145,   138,   139,    56,     0,   131,
      40,    33,    30,     0,   136,   134,   142,   146,     0,   141,
       0,    54,     0,    31,   145,   140,   130,   144,   143
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -218,  -218,  -218,  -218,  -218,  -218,  -218,  -218,  -218,  -218,
    -218,  -218,  -218,  -217,  -190,  -218,  -218,    63,    94,     8,
    -218,  -218,   190,  -218,  -218,  -170,   -59,  -218,  -218,  -218,
      81,   180,  -124,   -48,   197,  -218,   -57,   210,  -218,   -58,
     140,    53,  -218,  -218,    47,    95,  -118,  -218,  -159,  -168,
     112,   -17,    11,  -218,    13,  -218,  -218,    -6,    -9,  -218
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,   231,   198,    29,    30,   155,   131,   196,
     228,   161,   132,    31,   146,   211,    53,    32,    33,    34,
      54,    55,    56,    57,    58,    59,    85,    60,    61,    62,
      89,   120,    63,   115,    87,   140,   128,   206,   187,   149,
     185,   150,   171,   236,   237,   209,   255,   256,   269,    35
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      93,    95,   142,   148,    41,    97,   156,   157,   158,   232,
      91,    36,   159,    37,   137,   250,   277,   266,   218,   112,
     110,   113,   164,   111,   114,   165,    64,    65,   193,   138,
      41,   261,   267,   267,    38,   123,   122,   233,   124,   268,
     248,    42,    43,    44,   160,   173,    66,    45,   125,    46,
      47,    48,   147,    49,    50,   194,    51,    52,   195,   244,
     174,   216,   148,    39,    92,    40,   144,    42,    43,    44,
     257,   223,   258,    45,   251,    46,    47,    48,    44,    49,
      90,   148,    51,    52,    46,    47,   172,    41,    49,    42,
      43,    44,   219,   189,    67,    45,    69,    46,    47,    48,
      68,    49,    90,    44,    70,   194,    71,    45,   195,    46,
      47,    48,    72,    49,   176,   177,   178,   179,   180,   181,
     182,   183,    73,    74,    75,   259,    44,   184,    81,   220,
     221,    44,    46,    47,    48,    76,    49,    46,    47,    48,
     166,    49,    77,   167,    78,     2,   -64,    79,    80,     3,
       4,   240,    84,   242,     5,     6,     7,     8,     9,    10,
      11,    42,    43,    82,    12,    13,    14,    45,   217,    83,
      88,   222,    86,    98,    90,    15,    16,    99,   100,   102,
     103,   104,   105,   106,   107,    17,   108,   109,   116,   117,
     118,   126,   127,   129,   119,   133,   130,   134,   136,   135,
     139,   145,   151,   141,   153,   154,   152,   162,   170,   175,
     186,   163,   168,   169,   190,   192,   205,   197,   199,   207,
     200,   201,   202,   203,   214,   210,   208,   212,   224,   226,
     227,   229,   230,   215,   239,   241,   238,   245,   246,   247,
     249,   262,   235,   253,   252,   263,   273,   270,   191,   254,
     271,   264,   274,   276,   225,   260,   213,   101,    96,   234,
     243,   143,   188,   204,   275,   278,   265,     0,   121,    94,
     272
};

static const yytype_int16 yycheck[] =
{
      59,    59,   120,   127,    16,    62,    22,    23,    24,   199,
      58,     6,    28,     8,    17,   232,    10,    10,   186,    61,
      63,    63,    63,    66,    66,    66,    63,     7,    16,    32,
      16,   248,    26,    26,    29,    94,    93,   205,    95,    32,
     230,    53,    54,    55,    60,     3,     3,    59,    96,    61,
      62,    63,    16,    65,    66,    43,    68,    69,    46,   218,
      18,   185,   186,     6,    16,     8,   123,    53,    54,    55,
     240,   189,   242,    59,   233,    61,    62,    63,    55,    65,
      66,   205,    68,    69,    61,    62,   145,    16,    65,    53,
      54,    55,    16,   152,    33,    59,    63,    61,    62,    63,
      35,    65,    66,    55,     3,    43,     3,    59,    46,    61,
      62,    63,     3,    65,    45,    46,    47,    48,    49,    50,
      51,    52,     3,     3,    41,   243,    55,    58,    32,   188,
     188,    55,    61,    62,    63,    63,    65,    61,    62,    63,
      63,    65,    63,    66,     8,     0,    35,    63,    63,     4,
       5,   210,    17,   212,     9,    10,    11,    12,    13,    14,
      15,    53,    54,    16,    19,    20,    21,    59,   185,    16,
      18,   188,    35,     3,    66,    30,    31,     3,    63,    63,
      38,    42,    16,    39,    63,    40,     3,     3,    63,    17,
      63,    34,    36,    63,    56,    63,    65,    39,    17,    32,
      18,    16,     3,    57,    33,    18,    47,    16,    44,     9,
      37,    63,    63,    63,     6,    17,    39,    63,    16,    27,
      17,    17,    17,    17,    46,    18,    25,    16,    63,     3,
      61,    55,    18,    58,     3,    17,    27,     3,     3,    17,
      17,     3,    63,    18,    32,    17,     3,    18,   154,    63,
      17,    63,    63,    17,   191,   247,   175,    67,    61,   206,
     213,   121,   150,   168,   270,   274,   253,    -1,    88,    59,
     259
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    75,     0,     4,     5,     9,    10,    11,    12,    13,
      14,    15,    19,    20,    21,    30,    31,    40,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    89,
      90,    97,   101,   102,   103,   133,     6,     8,    29,     6,
       8,    16,    53,    54,    55,    59,    61,    62,    63,    65,
      66,    68,    69,   100,   104,   105,   106,   107,   108,   109,
     111,   112,   113,   116,    63,     7,     3,    33,    35,    63,
       3,     3,     3,     3,     3,    41,    63,    63,     8,    63,
      63,    32,    16,    16,    17,   110,    35,   118,    18,   114,
      66,   107,    16,   100,   111,   113,   108,   110,     3,     3,
      63,    96,    63,    38,    42,    16,    39,    63,     3,     3,
      63,    66,    61,    63,    66,   117,    63,    17,    63,    56,
     115,   105,   110,   100,   110,   107,    34,    36,   120,    63,
      65,    92,    96,    63,    39,    32,    17,    17,    32,    18,
     119,    57,   120,   114,   110,    16,    98,    16,   106,   123,
     125,     3,    47,    33,    18,    91,    22,    23,    24,    28,
      60,    95,    16,    63,    63,    66,    63,    66,    63,    63,
      44,   126,   100,     3,    18,     9,    45,    46,    47,    48,
      49,    50,    51,    52,    58,   124,    37,   122,   124,   100,
       6,    92,    17,    16,    43,    46,    93,    63,    88,    16,
      17,    17,    17,    17,   119,    39,   121,    27,    25,   129,
      18,    99,    16,   104,    46,    58,   106,   125,   123,    16,
     100,   113,   125,   120,    63,    91,     3,    61,    94,    55,
      18,    87,    88,   123,   115,    63,   127,   128,    27,     3,
     100,    17,   100,   118,   122,     3,     3,    17,    88,    17,
      87,   122,    32,    18,    63,   130,   131,    99,    99,   120,
      93,    87,     3,    17,    63,   128,    10,    26,    32,   132,
      18,    17,   126,     3,    63,   131,    17,    10,   132
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    74,    75,    75,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    86,    87,    87,    88,    88,    89,    90,    91,    91,
      92,    92,    93,    93,    93,    94,    95,    95,    95,    95,
      95,    96,    97,    98,    98,    99,    99,   100,   100,   100,
     100,   101,   102,   103,   104,   104,   105,   105,   106,   106,
     107,   107,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   109,   109,   110,   110,   111,   112,   112,   112,   112,
     113,   113,   113,   114,   114,   115,   115,   116,   116,   116,
     116,   116,   116,   117,   117,   117,   118,   119,   119,   120,
     120,   121,   121,   122,   122,   123,   123,   123,   123,   123,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     125,   126,   126,   127,   127,   128,   128,   129,   129,   130,
     130,   131,   131,   131,   131,   132,   132,   133
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
       1,     5,     8,     8,     1,     2,     1,     1,     2,     1,
       0,     3,     1,     1,     2,     2,     2,     2,     3,     3,
       4,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     3,     3,     0,     3,     0,     5,     4,     6,     6,
       4,     6,     6,     1,     1,     1,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       7,     0,     3,     1,     3,     1,     3,     0,     3,     1,
       3,     2,     2,     4,     4,     0,     1,     8
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, void *scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, void *scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep, scanner);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule, void *scanner)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              , scanner);
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, void *scanner)
{
  YYUSE (yyvaluep);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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
# undef YYSTACK_RELOCATE
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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
  case 21:
#line 221 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1675 "yacc_sql.tab.c"
    break;

  case 22:
#line 226 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1683 "yacc_sql.tab.c"
    break;

  case 23:
#line 231 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1691 "yacc_sql.tab.c"
    break;

  case 24:
#line 237 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1699 "yacc_sql.tab.c"
    break;

  case 25:
#line 243 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1707 "yacc_sql.tab.c"
    break;

  case 26:
#line 249 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1715 "yacc_sql.tab.c"
    break;

  case 27:
#line 255 "yacc_sql.y"
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1724 "yacc_sql.tab.c"
    break;

  case 28:
#line 261 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1732 "yacc_sql.tab.c"
    break;

  case 29:
#line 267 "yacc_sql.y"
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1741 "yacc_sql.tab.c"
    break;

  case 30:
#line 275 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			// create_index_init(&CONTEXT->ssql->sstr.create_index, $3, $5, $7, 0);
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-7].string), (yyvsp[-5].string), 0);
		}
#line 1751 "yacc_sql.tab.c"
    break;

  case 31:
#line 281 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			// create_index_init(&CONTEXT->ssql->sstr.create_index, $4, $6, $8, 1);
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-7].string), (yyvsp[-5].string), 1);
		}
#line 1761 "yacc_sql.tab.c"
    break;

  case 33:
#line 289 "yacc_sql.y"
                                       { }
#line 1767 "yacc_sql.tab.c"
    break;

  case 35:
#line 292 "yacc_sql.y"
             {
		create_index_append_attribute(&CONTEXT->ssql->sstr.create_index, (yyvsp[0].string));
	}
#line 1775 "yacc_sql.tab.c"
    break;

  case 36:
#line 298 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1784 "yacc_sql.tab.c"
    break;

  case 37:
#line 305 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
#line 1796 "yacc_sql.tab.c"
    break;

  case 39:
#line 316 "yacc_sql.y"
                                   {    }
#line 1802 "yacc_sql.tab.c"
    break;

  case 40:
#line 321 "yacc_sql.y"
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
#line 1817 "yacc_sql.tab.c"
    break;

  case 41:
#line 332 "yacc_sql.y"
                {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-1].number), 4, (yyvsp[0].number));
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			CONTEXT->value_length++;
		}
#line 1828 "yacc_sql.tab.c"
    break;

  case 42:
#line 341 "yacc_sql.y"
                  {
		(yyval.number) = ISFALSE; // 默认允许null
	}
#line 1836 "yacc_sql.tab.c"
    break;

  case 43:
#line 344 "yacc_sql.y"
                     {
		(yyval.number) = ISFALSE;
	}
#line 1844 "yacc_sql.tab.c"
    break;

  case 44:
#line 347 "yacc_sql.y"
                   {
		(yyval.number) = ISTRUE;
	}
#line 1852 "yacc_sql.tab.c"
    break;

  case 45:
#line 353 "yacc_sql.y"
               {
		(yyval.number) = (yyvsp[0].number);
	}
#line 1860 "yacc_sql.tab.c"
    break;

  case 46:
#line 359 "yacc_sql.y"
              { 
		(yyval.number)=INTS; 
		// printf("CREATE 语句语法解析 type 为 INTS\n");
	}
#line 1869 "yacc_sql.tab.c"
    break;

  case 47:
#line 363 "yacc_sql.y"
                  { 
		   (yyval.number)=CHARS;
		// printf("CREATE 语句语法解析 type 为 STRING_T\n");
	}
#line 1878 "yacc_sql.tab.c"
    break;

  case 48:
#line 367 "yacc_sql.y"
                 { 
		   (yyval.number)=FLOATS;
		// printf("CREATE 语句语法解析 type 为 FLOAT_T\n");
	}
#line 1887 "yacc_sql.tab.c"
    break;

  case 49:
#line 371 "yacc_sql.y"
                    { 
		   (yyval.number)=DATES;
		// printf("CREATE 语句语法解析 type 为 DATE_T\n");
	}
#line 1896 "yacc_sql.tab.c"
    break;

  case 50:
#line 374 "yacc_sql.y"
                    {
	    (yyval.number)=TEXTS;
	}
#line 1904 "yacc_sql.tab.c"
    break;

  case 51:
#line 380 "yacc_sql.y"
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1913 "yacc_sql.tab.c"
    break;

  case 52:
#line 389 "yacc_sql.y"
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
#line 1932 "yacc_sql.tab.c"
    break;

  case 53:
#line 405 "yacc_sql.y"
                                       {
		// 到此结束一组的插入：存储该组、增加index、value_length清零
		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
		CONTEXT->insert_index++;
		//临时变量清零
      	CONTEXT->value_length=0;
	}
#line 1944 "yacc_sql.tab.c"
    break;

  case 54:
#line 412 "yacc_sql.y"
                                                           {
		// 到此结束一组的插入：存储该组、增加index、value_length清零
		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
		CONTEXT->insert_index++;
		//临时变量清零
      	CONTEXT->value_length=0;
	}
#line 1956 "yacc_sql.tab.c"
    break;

  case 56:
#line 422 "yacc_sql.y"
                              { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1964 "yacc_sql.tab.c"
    break;

  case 57:
#line 428 "yacc_sql.y"
          {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number), false);
		char exp_name[MAX_NUM];
		sprintf(exp_name, "%d", (yyvsp[0].number));
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
#line 1975 "yacc_sql.tab.c"
    break;

  case 58:
#line 434 "yacc_sql.y"
          {
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].floats), false);
		char exp_name[MAX_NUM];
		sprintf(exp_name, "%f", (yyvsp[0].floats));
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
#line 1986 "yacc_sql.tab.c"
    break;

  case 59:
#line 440 "yacc_sql.y"
                {
		// null不需要加双引号，当作字符串插入
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		CONTEXT->exps[CONTEXT->exp_length++] = "NULL";
	}
#line 1996 "yacc_sql.tab.c"
    break;

  case 60:
#line 445 "yacc_sql.y"
         {
        // 没有末位的"\0"
		(yyvsp[0].string) = substr((yyvsp[0].string), 1, strlen((yyvsp[0].string))-2);
        // 长度大于4就当作tetx来处理
		value_init_string_with_text(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string), false, strlen((yyvsp[0].string)));
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
		CONTEXT->exps[CONTEXT->exp_length++] = strdup((yyvsp[0].string));
	}
#line 2009 "yacc_sql.tab.c"
    break;

  case 61:
#line 458 "yacc_sql.y"
        {
		CONTEXT->ssql->flag = SCF_DELETE;//"delete";
		deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
		// 处理where
		if ((yyvsp[-1].condition1) != NULL) {
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, (yyvsp[-1].condition1)); // where
		}
    }
#line 2022 "yacc_sql.tab.c"
    break;

  case 62:
#line 470 "yacc_sql.y"
        {
		CONTEXT->ssql->flag = SCF_UPDATE;//"update";
		Value *value = &CONTEXT->values[0];
		updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value);
		if ((yyvsp[-1].condition1) != NULL) {
			updates_init_condition(&CONTEXT->ssql->sstr.update, (yyvsp[-1].condition1));
		}
	}
#line 2035 "yacc_sql.tab.c"
    break;

  case 63:
#line 483 "yacc_sql.y"
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
#line 2053 "yacc_sql.tab.c"
    break;

  case 64:
#line 499 "yacc_sql.y"
         {  // select *
		RelAttr attr;
		relation_attr_init(&attr, NULL, "*", NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);

		(yyval.relattr1) = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy((yyval.relattr1), CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
#line 2068 "yacc_sql.tab.c"
    break;

  case 65:
#line 509 "yacc_sql.y"
                             { 
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);

		(yyval.relattr1) = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy((yyval.relattr1), CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
#line 2080 "yacc_sql.tab.c"
    break;

  case 66:
#line 519 "yacc_sql.y"
                        {
	}
#line 2087 "yacc_sql.tab.c"
    break;

  case 67:
#line 521 "yacc_sql.y"
                     {
		selects_append_expressions(&CONTEXT->ssql->sstr.selection, (yyvsp[0].relation));
	}
#line 2095 "yacc_sql.tab.c"
    break;

  case 68:
#line 527 "yacc_sql.y"
                     {
		CONTEXT->exps[CONTEXT->exp_length++] = "NULL";
		(yyval.relation) = ( const char **)malloc(sizeof(const char*) * CONTEXT->exp_length);
		memcpy((yyval.relation), CONTEXT->exps, sizeof(const char*) * CONTEXT->exp_length);
		CONTEXT->exp_length = 0; // 清空
		CONTEXT->value_length = 0;
	}
#line 2107 "yacc_sql.tab.c"
    break;

  case 69:
#line 534 "yacc_sql.y"
                   {
		CONTEXT->exps[CONTEXT->exp_length++] = "NULL";
		(yyval.relation) = ( const char **)malloc(sizeof(const char*) * CONTEXT->exp_length);
		memcpy((yyval.relation), CONTEXT->exps, sizeof(const char*) * CONTEXT->exp_length);
		CONTEXT->exp_length = 0; // 清空
		CONTEXT->value_length = 0;
	}
#line 2119 "yacc_sql.tab.c"
    break;

  case 71:
#line 545 "yacc_sql.y"
                          {}
#line 2125 "yacc_sql.tab.c"
    break;

  case 81:
#line 562 "yacc_sql.y"
                                      {
		CONTEXT->exps[CONTEXT->exp_length++] = "(";
	}
#line 2133 "yacc_sql.tab.c"
    break;

  case 82:
#line 565 "yacc_sql.y"
                             {
		CONTEXT->exps[CONTEXT->exp_length++] = "(";
	}
#line 2141 "yacc_sql.tab.c"
    break;

  case 83:
#line 571 "yacc_sql.y"
                                      {
		CONTEXT->exps[CONTEXT->exp_length++] = ")";
	}
#line 2149 "yacc_sql.tab.c"
    break;

  case 84:
#line 574 "yacc_sql.y"
                             {
		CONTEXT->exps[CONTEXT->exp_length++] = ")";
	}
#line 2157 "yacc_sql.tab.c"
    break;

  case 85:
#line 580 "yacc_sql.y"
              {
		CONTEXT->exps[CONTEXT->exp_length++] = "-";
	}
#line 2165 "yacc_sql.tab.c"
    break;

  case 86:
#line 586 "yacc_sql.y"
             {
		// *
		CONTEXT->exps[CONTEXT->exp_length++] = "*";
	}
#line 2174 "yacc_sql.tab.c"
    break;

  case 87:
#line 590 "yacc_sql.y"
               {
		// +
		CONTEXT->exps[CONTEXT->exp_length++] = "+";
	}
#line 2183 "yacc_sql.tab.c"
    break;

  case 89:
#line 595 "yacc_sql.y"
              {
		// 除法
		CONTEXT->exps[CONTEXT->exp_length++] = "/";
	}
#line 2192 "yacc_sql.tab.c"
    break;

  case 90:
#line 602 "yacc_sql.y"
          { // select age
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;

		char exp_name[MAX_NUM];
		sprintf(exp_name, "%s", (yyvsp[0].string));
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
#line 2206 "yacc_sql.tab.c"
    break;

  case 91:
#line 611 "yacc_sql.y"
                    { // select t1.age
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;

		char exp_name[MAX_NUM];
		sprintf(exp_name, "%s.%s", (yyvsp[-2].string), (yyvsp[0].string));
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
#line 2220 "yacc_sql.tab.c"
    break;

  case 92:
#line 620 "yacc_sql.y"
                     { // select t1.*
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-2].string), "*", NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;

		char exp_name[MAX_NUM];
		sprintf(exp_name, "%s.*", (yyvsp[-2].string));
		CONTEXT->exps[CONTEXT->exp_length++] = strdup(exp_name);
	}
#line 2234 "yacc_sql.tab.c"
    break;

  case 94:
#line 633 "yacc_sql.y"
                                   { }
#line 2240 "yacc_sql.tab.c"
    break;

  case 96:
#line 638 "yacc_sql.y"
                                 {
		selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-2].string));
    }
#line 2248 "yacc_sql.tab.c"
    break;

  case 97:
#line 645 "yacc_sql.y"
        {	// 只有COUNT允许COUNT(*)
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2258 "yacc_sql.tab.c"
    break;

  case 98:
#line 651 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2268 "yacc_sql.tab.c"
    break;

  case 99:
#line 657 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2278 "yacc_sql.tab.c"
    break;

  case 100:
#line 663 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2288 "yacc_sql.tab.c"
    break;

  case 101:
#line 669 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2298 "yacc_sql.tab.c"
    break;

  case 102:
#line 675 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), "*", (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2308 "yacc_sql.tab.c"
    break;

  case 103:
#line 683 "yacc_sql.y"
             { (yyval.string) = (yyvsp[0].string);}
#line 2314 "yacc_sql.tab.c"
    break;

  case 104:
#line 684 "yacc_sql.y"
                 {(yyval.string) = number_to_str((yyvsp[0].number));}
#line 2320 "yacc_sql.tab.c"
    break;

  case 105:
#line 685 "yacc_sql.y"
             {(yyval.string) = (yyvsp[0].string);}
#line 2326 "yacc_sql.tab.c"
    break;

  case 106:
#line 689 "yacc_sql.y"
                         {
		CONTEXT->rels[CONTEXT->rel_length++] = (yyvsp[-1].string);
		CONTEXT->rels[CONTEXT->rel_length++] = "NULL";
		(yyval.relation) = ( const char **)malloc(sizeof(const char*) * CONTEXT->rel_length);
		memcpy((yyval.relation), CONTEXT->rels, sizeof(const char*) * CONTEXT->rel_length);
		CONTEXT->rel_length = 0;
	}
#line 2338 "yacc_sql.tab.c"
    break;

  case 107:
#line 699 "yacc_sql.y"
                {}
#line 2344 "yacc_sql.tab.c"
    break;

  case 108:
#line 700 "yacc_sql.y"
                        {	
		CONTEXT->rels[CONTEXT->rel_length++] = (yyvsp[-1].string);
	}
#line 2352 "yacc_sql.tab.c"
    break;

  case 109:
#line 706 "yacc_sql.y"
                { 
		(yyval.condition1) = NULL; 
		// 这里不能清零，否则多个子查询条件时，子查询没有where会把主查询的condition清零 
	}
#line 2361 "yacc_sql.tab.c"
    break;

  case 110:
#line 710 "yacc_sql.y"
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
#line 2381 "yacc_sql.tab.c"
    break;

  case 112:
#line 729 "yacc_sql.y"
                                  {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
		selects_append_conditions_with_num(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);
		// 每次where结束都清零长度，多个子查询的where不会相互影响
		CONTEXT->condition_length = 0; 
		// 由于select里只有condition涉及到value_length，所以一并在此清零
		CONTEXT->value_length = 0;
	}
#line 2394 "yacc_sql.tab.c"
    break;

  case 114:
#line 741 "yacc_sql.y"
                                   {
		// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
	}
#line 2402 "yacc_sql.tab.c"
    break;

  case 115:
#line 747 "yacc_sql.y"
                                    {
		// 左侧表达式，右侧表达式
		Condition condition;
		condition_exp(&condition, (yyvsp[-2].relation), (yyvsp[-1].number), (yyvsp[0].relation));
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2413 "yacc_sql.tab.c"
    break;

  case 116:
#line 769 "yacc_sql.y"
                                      {
		RelAttr left_attr;
		Value left_value;
		int left_is_attr;

		init_attr_or_value(&left_attr, &left_value, &left_is_attr, (yyvsp[-2].relation)[0]);
		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), left_is_attr, &left_attr, &left_value, 2, NULL, NULL, (yyvsp[0].selnode), NULL);
	}
#line 2427 "yacc_sql.tab.c"
    break;

  case 117:
#line 778 "yacc_sql.y"
                                 {
		// 反过来，当作正的解析
		print_str("sub_select comOp value");
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
#line 2447 "yacc_sql.tab.c"
    break;

  case 118:
#line 793 "yacc_sql.y"
                                  {
		// 反过来，当作正的解析
		// RelAttr left_attr;
		// relation_attr_init(&left_attr, NULL, $3, NULL, 0);
		RelAttr *left_attr = &CONTEXT->rel_attrs[CONTEXT->rel_attr_length - 1];

		Condition condition;
		if ((yyvsp[-1].number) == GREAT_THAN || (yyvsp[-1].number) == GREAT_EQUAL) {
			condition_init(&condition, (yyvsp[-1].number) - 2, 1, left_attr, NULL, 2, NULL, NULL, (yyvsp[-2].selnode), NULL);
		} else if ((yyvsp[-1].number) == LESS_THAN || (yyvsp[-1].number) == LESS_EQUAL) {
			condition_init(&condition, (yyvsp[-1].number) + 2, 1, left_attr, NULL, 2, NULL, NULL, (yyvsp[-2].selnode), NULL);
		} else {
			condition_init(&condition, (yyvsp[-1].number), 1, left_attr, NULL, 2, NULL, NULL, (yyvsp[-2].selnode), NULL);
		}
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2468 "yacc_sql.tab.c"
    break;

  case 119:
#line 809 "yacc_sql.y"
                                      {
		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), 2, NULL, NULL, 2, NULL, NULL, (yyvsp[0].selnode), (yyvsp[-2].selnode));
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2478 "yacc_sql.tab.c"
    break;

  case 120:
#line 817 "yacc_sql.y"
             { (yyval.number) = 0; }
#line 2484 "yacc_sql.tab.c"
    break;

  case 121:
#line 818 "yacc_sql.y"
         { (yyval.number) = 3; }
#line 2490 "yacc_sql.tab.c"
    break;

  case 122:
#line 819 "yacc_sql.y"
         { (yyval.number) = 5; }
#line 2496 "yacc_sql.tab.c"
    break;

  case 123:
#line 820 "yacc_sql.y"
         { (yyval.number) = 2; }
#line 2502 "yacc_sql.tab.c"
    break;

  case 124:
#line 821 "yacc_sql.y"
         { (yyval.number) = 4; }
#line 2508 "yacc_sql.tab.c"
    break;

  case 125:
#line 822 "yacc_sql.y"
         { (yyval.number) = 1; }
#line 2514 "yacc_sql.tab.c"
    break;

  case 126:
#line 823 "yacc_sql.y"
             { (yyval.number) = 8; }
#line 2520 "yacc_sql.tab.c"
    break;

  case 127:
#line 824 "yacc_sql.y"
                 { (yyval.number) = 9; }
#line 2526 "yacc_sql.tab.c"
    break;

  case 128:
#line 825 "yacc_sql.y"
             {(yyval.number) = 6;}
#line 2532 "yacc_sql.tab.c"
    break;

  case 129:
#line 826 "yacc_sql.y"
                 {(yyval.number) = 7;}
#line 2538 "yacc_sql.tab.c"
    break;

  case 130:
#line 830 "yacc_sql.y"
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
#line 2558 "yacc_sql.tab.c"
    break;

  case 131:
#line 848 "yacc_sql.y"
                  {(yyval.relattr1) = NULL;}
#line 2564 "yacc_sql.tab.c"
    break;

  case 132:
#line 849 "yacc_sql.y"
                              {
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);
		(yyval.relattr1) = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy((yyval.relattr1), CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
#line 2575 "yacc_sql.tab.c"
    break;

  case 133:
#line 858 "yacc_sql.y"
                  {
		;
	}
#line 2583 "yacc_sql.tab.c"
    break;

  case 134:
#line 861 "yacc_sql.y"
                                      {}
#line 2589 "yacc_sql.tab.c"
    break;

  case 135:
#line 865 "yacc_sql.y"
           {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL, 0);
		// selects_append_group(&CONTEXT->ssql->sstr.selection, &attr);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2600 "yacc_sql.tab.c"
    break;

  case 136:
#line 871 "yacc_sql.y"
                    {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);
		// selects_append_group(&CONTEXT->ssql->sstr.selection, &attr);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2611 "yacc_sql.tab.c"
    break;

  case 138:
#line 881 "yacc_sql.y"
                             {
	}
#line 2618 "yacc_sql.tab.c"
    break;

  case 139:
#line 886 "yacc_sql.y"
                  {
		// order by A, B, C，实际上加入顺序为C、B、A，方便后面排序
	}
#line 2626 "yacc_sql.tab.c"
    break;

  case 140:
#line 889 "yacc_sql.y"
                                    {}
#line 2632 "yacc_sql.tab.c"
    break;

  case 141:
#line 892 "yacc_sql.y"
                  {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL, 0);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2642 "yacc_sql.tab.c"
    break;

  case 142:
#line 897 "yacc_sql.y"
                  {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL, 1);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2652 "yacc_sql.tab.c"
    break;

  case 143:
#line 902 "yacc_sql.y"
                            {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL, 0);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2662 "yacc_sql.tab.c"
    break;

  case 144:
#line 907 "yacc_sql.y"
                         {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL, 1);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2672 "yacc_sql.tab.c"
    break;

  case 146:
#line 915 "yacc_sql.y"
              {}
#line 2678 "yacc_sql.tab.c"
    break;

  case 147:
#line 919 "yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 2687 "yacc_sql.tab.c"
    break;


#line 2691 "yacc_sql.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (scanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
                  yystos[+*yyssp], yyvsp, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 925 "yacc_sql.y"

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
