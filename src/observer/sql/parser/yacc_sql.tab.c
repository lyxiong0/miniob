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
# define YYDEBUG 0
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
    NULL_T = 308,
    INNER = 309,
    JOIN = 310,
    IN = 311,
    NUMBER = 312,
    FLOAT = 313,
    ID = 314,
    PATH = 315,
    SSS = 316,
    STAR = 317,
    STRING_V = 318,
    COUNT = 319,
    OTHER_FUNCTION_TYPE = 320
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
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
#define YYLAST   286

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  66
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  51
/* YYNRULES -- Number of rules.  */
#define YYNRULES  132
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  270

#define YYUNDEFTOK  2
#define YYMAXUTOK   320


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
      65
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   173,   173,   175,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   199,   204,   209,   215,   221,   227,   233,   239,   245,
     252,   257,   265,   272,   282,   284,   288,   299,   313,   316,
     319,   325,   328,   332,   336,   340,   346,   355,   372,   379,
     387,   389,   394,   397,   400,   404,   412,   424,   437,   450,
     460,   470,   471,   475,   480,   485,   492,   494,   497,   499,
     505,   511,   517,   523,   529,   535,   544,   545,   546,   550,
     560,   561,   567,   572,   589,   591,   601,   603,   609,   621,
     641,   661,   683,   705,   726,   745,   757,   769,   780,   791,
     800,   809,   817,   825,   832,   846,   861,   879,   880,   881,
     882,   883,   884,   885,   886,   890,   903,   905,   911,   914,
     918,   923,   930,   932,   937,   940,   943,   948,   953,   958,
     964,   966,   969
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
     315,   316,   317,   318,   319,   320
};
# endif

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
    -197,    94,  -197,     8,    70,   103,   -53,    11,     7,    32,
       9,   -20,    65,    74,    81,    82,    92,    52,  -197,  -197,
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
    -197,  -197,  -197,  -197,  -197,  -197,    42,    43,   120,    51,
      84,    79,  -197,   117,   128,    95,   127,  -197,  -197,   151,
     153,  -197,    99,   105,   141,  -197,  -197,  -197,  -197,  -197,
     124,   164,   143,   125,   180,   182,     4,   115,   129,   130,
     132,    67,  -197,  -197,  -197,  -197,   156,   155,   133,   126,
      99,   134,   157,  -197,  -197,  -197,  -197,  -197,   162,  -197,
     178,    18,   179,   144,   155,   127,   184,    -4,   195,   154,
     169,   185,   147,   188,   146,   114,  -197,  -197,   119,   148,
    -197,   149,   165,  -197,    39,    40,   197,  -197,  -197,  -197,
     -24,  -197,    71,   173,   101,  -197,    39,   205,    99,   196,
    -197,  -197,  -197,  -197,   -13,   158,   198,   199,   201,   202,
     203,   179,   176,   194,   187,   204,  -197,   207,   103,   166,
     -12,   168,  -197,  -197,  -197,  -197,  -197,  -197,  -197,     3,
      -6,    22,    -4,  -197,   102,   155,   167,   185,   224,   171,
    -197,   177,  -197,   212,   172,  -197,  -197,  -197,  -197,  -197,
      -4,   132,   174,   208,   229,    39,   217,    39,    95,    90,
     183,  -197,  -197,  -197,  -197,  -197,   186,  -197,   206,  -197,
    -197,   173,   209,  -197,   234,   237,  -197,  -197,  -197,   225,
    -197,   240,   227,   173,  -197,   213,   228,  -197,   189,  -197,
     204,  -197,   204,   155,     6,    29,  -197,  -197,   190,  -197,
     191,  -197,  -197,    24,  -197,   244,  -197,   192,   174,    10,
     235,  -197,  -197,   238,   239,   210,  -197,   220,  -197,  -197,
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,   200,  -197,
     189,  -197,  -197,  -197,   211,     5,  -197,  -197,  -197,  -197
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
       0,    63,    59,     0,     0,     0,    66,    61,    62,     0,
       0,    23,     0,     0,     0,    24,    25,    26,    22,    21,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    60,    29,    28,    46,     0,    82,     0,     0,
       0,     0,     0,    27,    32,    64,    65,    77,    78,    76,
       0,     0,    80,     0,    82,    66,     0,     0,     0,     0,
       0,    34,     0,     0,     0,     0,    70,    73,     0,     0,
      79,     0,   116,    67,     0,     0,     0,    54,    52,    53,
       0,    55,     0,    86,     0,    56,     0,     0,     0,     0,
      42,    43,    44,    45,    38,     0,     0,     0,     0,     0,
       0,    80,    84,     0,   122,    50,    47,     0,     0,     0,
       0,     0,   107,   108,   109,   110,   111,   112,   113,     0,
       0,     0,     0,    83,     0,    82,     0,    34,     0,     0,
      40,     0,    37,     0,     0,    71,    72,    74,    75,    81,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,   114,    90,    88,   101,     0,   100,    91,    89,
     103,    86,   105,   104,     0,     0,    35,    33,    41,     0,
      39,     0,     0,    86,    69,   120,   117,   118,     0,    58,
      50,    48,    50,    82,     0,     0,    96,    99,     0,    87,
       0,    57,   132,    38,    30,     0,    85,     0,     0,   130,
     123,   124,    51,     0,     0,     0,    97,     0,    92,   102,
      93,   106,    36,    31,   121,   119,   127,   131,     0,   126,
       0,    49,   115,    98,     0,   130,   125,    94,   129,   128
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
    -197,  -197,  -197,  -197,  -197,    87,   136,    25,  -197,  -197,
     214,  -197,  -197,  -131,  -113,  -197,  -197,  -197,   109,   215,
    -197,   170,    80,  -197,  -197,    72,   121,   -94,  -197,  -196,
    -151,  -120,  -152,  -197,  -197,    30,  -197,  -197,    12,     2,
    -197
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,   129,   101,   172,   209,   134,
     102,    31,   115,   186,   122,    32,    33,    34,    45,    46,
      47,    72,    94,    48,    90,    70,   110,    98,   181,   163,
     123,   159,   124,   144,   216,   217,   184,   240,   241,   259,
      35
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     112,   145,   161,   169,   164,   229,    49,   195,   149,   200,
      51,   201,   116,   165,    36,   268,    37,   236,    50,   116,
     256,   150,   151,   152,   153,   154,   155,   156,   157,   213,
     170,   257,   158,   171,   190,   107,   257,    38,   116,    54,
     196,   191,   258,   146,    53,   116,   194,   197,   199,   117,
     108,   203,   245,   118,   119,   120,   117,   121,   147,   246,
     118,   119,   193,    85,   121,    52,    86,   170,    55,   225,
     171,   204,   220,   249,   222,   117,    39,    56,    40,   118,
     119,   198,   117,   121,    57,    58,   118,   119,   247,   242,
     121,   243,   117,    60,     2,    59,   118,   119,     3,     4,
     121,    61,    62,     5,     6,     7,     8,     9,    10,    11,
      64,    66,   248,    12,    13,    14,   160,   151,   152,   153,
     154,   155,   156,   157,    15,    16,    41,   158,    63,   244,
      69,    43,    44,    67,    17,   224,   151,   152,   153,   154,
     155,   156,   157,    65,    68,    71,   158,   151,   152,   153,
     154,   155,   156,   157,    73,   117,    74,   158,    75,   118,
     119,   202,    41,   121,    77,    42,    79,    43,    44,   130,
     131,   132,    87,   137,    88,   133,   138,    89,   139,    78,
      80,   140,    81,    83,    82,    84,    93,   100,    91,    92,
      96,    97,    99,   103,   105,   106,   104,   109,   125,   111,
     114,   126,   127,   128,   135,   136,   148,   141,   142,   143,
     162,   166,   183,   168,   174,   180,   175,   173,   176,   177,
     178,   182,   185,   187,   192,   189,   205,   207,   208,   211,
     210,   212,   219,   215,   221,   218,   226,   231,   228,   227,
     232,   230,   233,   234,   235,   237,   238,   253,   239,   250,
     251,   254,   264,   260,   206,   261,   262,   188,   252,   265,
     223,   214,   179,   263,   167,   113,    76,   269,   255,     0,
     267,     0,   266,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    95
};

static const yytype_int16 yycheck[] =
{
      94,   114,   122,    16,   124,   201,    59,   159,    32,   161,
       3,   162,    16,   126,     6,    10,     8,   213,     7,    16,
      10,    45,    46,    47,    48,    49,    50,    51,    52,   180,
      43,    26,    56,    46,    46,    17,    26,    29,    16,    59,
      46,    53,    32,     3,    35,    16,   159,    53,   161,    53,
      32,   164,    46,    57,    58,    59,    53,    61,    18,    53,
      57,    58,    59,    59,    61,    33,    62,    43,     3,   189,
      46,   165,   185,   225,   187,    53,     6,     3,     8,    57,
      58,    59,    53,    61,     3,     3,    57,    58,    59,   220,
      61,   222,    53,    41,     0,     3,    57,    58,     4,     5,
      61,    59,    59,     9,    10,    11,    12,    13,    14,    15,
      59,    32,   225,    19,    20,    21,    45,    46,    47,    48,
      49,    50,    51,    52,    30,    31,    59,    56,     8,   223,
      35,    64,    65,    16,    40,    45,    46,    47,    48,    49,
      50,    51,    52,    59,    16,    18,    56,    46,    47,    48,
      49,    50,    51,    52,     3,    53,     3,    56,    59,    57,
      58,    59,    59,    61,    59,    62,    42,    64,    65,    22,
      23,    24,    57,    59,    59,    28,    62,    62,    59,    38,
      16,    62,    39,     3,    59,     3,    54,    61,    59,    59,
      34,    36,    59,    59,    32,    17,    39,    18,     3,    55,
      16,    47,    33,    18,    16,    59,     9,    59,    59,    44,
      37,     6,    25,    17,    16,    39,    17,    59,    17,    17,
      17,    27,    18,    16,    56,    59,    59,     3,    57,    17,
      53,    59,     3,    59,    17,    27,    53,     3,    32,    53,
       3,    32,    17,     3,    17,    32,    18,     3,    59,    59,
      59,    59,    32,    18,   167,    17,    17,   148,   233,    59,
     188,   181,   141,    53,   128,    95,    52,   265,   238,    -1,
      59,    -1,   260,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
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
     108,   106,    59,    90,   103,    59,    81,     3,    57,    84,
      53,    17,    59,   106,    98,    59,   110,   111,    27,     3,
      90,    17,    90,   101,    45,   107,    53,    53,    32,   105,
      32,     3,     3,    17,     3,    17,   105,    32,    18,    59,
     113,   114,    89,    89,   103,    46,    53,    59,    90,   108,
      59,    59,    83,     3,    59,   111,    10,    26,    32,   115,
      18,    17,    17,    53,    32,    59,   114,    59,    10,   115
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
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
     106,   106,   106,   106,   106,   106,   106,   107,   107,   107,
     107,   107,   107,   107,   107,   108,   109,   109,   110,   110,
     111,   111,   112,   112,   113,   113,   114,   114,   114,   114,
     115,   115,   116
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
       9,    10,     4,     8,     0,     3,     6,     3,     0,     2,
       1,     1,     1,     1,     1,     1,     1,     6,     4,     6,
       0,     3,     1,     1,     1,     1,     5,     8,     8,     1,
       2,     1,     1,     1,     3,     3,     0,     3,     0,     5,
       4,     6,     6,     4,     6,     6,     1,     1,     1,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     3,     3,
       3,     3,     5,     5,     7,     3,     4,     5,     6,     4,
       3,     3,     5,     3,     3,     3,     5,     1,     1,     1,
       1,     1,     1,     1,     2,     6,     0,     3,     1,     3,
       1,     3,     0,     3,     1,     3,     2,     2,     4,     4,
       0,     1,     8
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
#line 199 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1655 "yacc_sql.tab.c"
    break;

  case 22:
#line 204 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1663 "yacc_sql.tab.c"
    break;

  case 23:
#line 209 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1671 "yacc_sql.tab.c"
    break;

  case 24:
#line 215 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1679 "yacc_sql.tab.c"
    break;

  case 25:
#line 221 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1687 "yacc_sql.tab.c"
    break;

  case 26:
#line 227 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1695 "yacc_sql.tab.c"
    break;

  case 27:
#line 233 "yacc_sql.y"
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1704 "yacc_sql.tab.c"
    break;

  case 28:
#line 239 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1712 "yacc_sql.tab.c"
    break;

  case 29:
#line 245 "yacc_sql.y"
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1721 "yacc_sql.tab.c"
    break;

  case 30:
#line 253 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string), 0);
		}
#line 1730 "yacc_sql.tab.c"
    break;

  case 31:
#line 258 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string), 1);
		}
#line 1739 "yacc_sql.tab.c"
    break;

  case 32:
#line 266 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1748 "yacc_sql.tab.c"
    break;

  case 33:
#line 273 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
#line 1760 "yacc_sql.tab.c"
    break;

  case 35:
#line 284 "yacc_sql.y"
                                   {    }
#line 1766 "yacc_sql.tab.c"
    break;

  case 36:
#line 289 "yacc_sql.y"
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
#line 1781 "yacc_sql.tab.c"
    break;

  case 37:
#line 300 "yacc_sql.y"
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
#line 1796 "yacc_sql.tab.c"
    break;

  case 38:
#line 313 "yacc_sql.y"
                  {
		(yyval.number) = ISFALSE; // 默认允许null
	}
#line 1804 "yacc_sql.tab.c"
    break;

  case 39:
#line 316 "yacc_sql.y"
                     {
		(yyval.number) = ISFALSE;
	}
#line 1812 "yacc_sql.tab.c"
    break;

  case 40:
#line 319 "yacc_sql.y"
                   {
		(yyval.number) = ISTRUE;
	}
#line 1820 "yacc_sql.tab.c"
    break;

  case 41:
#line 325 "yacc_sql.y"
                       {(yyval.number) = (yyvsp[0].number);}
#line 1826 "yacc_sql.tab.c"
    break;

  case 42:
#line 328 "yacc_sql.y"
              { 
		(yyval.number)=INTS; 
		// printf("CREATE 语句语法解析 type 为 INTS\n");
	}
#line 1835 "yacc_sql.tab.c"
    break;

  case 43:
#line 332 "yacc_sql.y"
                  { 
		   (yyval.number)=CHARS;
		// printf("CREATE 语句语法解析 type 为 STRING_T\n");
	}
#line 1844 "yacc_sql.tab.c"
    break;

  case 44:
#line 336 "yacc_sql.y"
                 { 
		   (yyval.number)=FLOATS;
		// printf("CREATE 语句语法解析 type 为 FLOAT_T\n");
	}
#line 1853 "yacc_sql.tab.c"
    break;

  case 45:
#line 340 "yacc_sql.y"
                    { 
		   (yyval.number)=DATES;
		// printf("CREATE 语句语法解析 type 为 DATE_T\n");
	}
#line 1862 "yacc_sql.tab.c"
    break;

  case 46:
#line 347 "yacc_sql.y"
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1871 "yacc_sql.tab.c"
    break;

  case 47:
#line 356 "yacc_sql.y"
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
#line 1890 "yacc_sql.tab.c"
    break;

  case 48:
#line 372 "yacc_sql.y"
                                       {
		// 到此结束一组的插入：存储该组、增加index、value_length清零
		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
		CONTEXT->insert_index++;
		//临时变量清零
      	CONTEXT->value_length=0;
	}
#line 1902 "yacc_sql.tab.c"
    break;

  case 49:
#line 379 "yacc_sql.y"
                                                           {
		// 到此结束一组的插入：存储该组、增加index、value_length清零
		inserts_init(&CONTEXT->ssql->sstr.insertion, CONTEXT->id, CONTEXT->values, CONTEXT->value_length, CONTEXT->insert_index);
		CONTEXT->insert_index++;
		//临时变量清零
      	CONTEXT->value_length=0;
	}
#line 1914 "yacc_sql.tab.c"
    break;

  case 51:
#line 389 "yacc_sql.y"
                              { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1922 "yacc_sql.tab.c"
    break;

  case 52:
#line 394 "yacc_sql.y"
          {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number), false);
	}
#line 1930 "yacc_sql.tab.c"
    break;

  case 53:
#line 397 "yacc_sql.y"
          {
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].floats), false);
	}
#line 1938 "yacc_sql.tab.c"
    break;

  case 54:
#line 400 "yacc_sql.y"
                {
		// null不需要加双引号，当作字符串插入
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
	}
#line 1947 "yacc_sql.tab.c"
    break;

  case 55:
#line 404 "yacc_sql.y"
         {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string), false);
		}
#line 1956 "yacc_sql.tab.c"
    break;

  case 56:
#line 413 "yacc_sql.y"
        {
		CONTEXT->ssql->flag = SCF_DELETE;//"delete";
		deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
		// 处理where
		if ((yyvsp[-1].condition1) != NULL) {
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, (yyvsp[-1].condition1)); // where
		}
    }
#line 1969 "yacc_sql.tab.c"
    break;

  case 57:
#line 425 "yacc_sql.y"
        {
		CONTEXT->ssql->flag = SCF_UPDATE;//"update";
		Value *value = &CONTEXT->values[0];
		updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value);
		if ((yyvsp[-1].condition1) != NULL) {
			updates_init_condition(&CONTEXT->ssql->sstr.update, (yyvsp[-1].condition1));
		}
	}
#line 1982 "yacc_sql.tab.c"
    break;

  case 58:
#line 438 "yacc_sql.y"
            {
			CONTEXT->ssql->flag=SCF_SELECT;//"select";

			selects_append_relations(&CONTEXT->ssql->sstr.selection, (yyvsp[-5].relation)); // from_rel
			if ((yyvsp[-3].condition1) != NULL) {
				selects_append_conditions(&CONTEXT->ssql->sstr.selection, (yyvsp[-3].condition1)); // where
			}
			selects_append_attributes(&CONTEXT->ssql->sstr.selection, (yyvsp[-6].relattr1)); // select_attr
	    }
#line 1996 "yacc_sql.tab.c"
    break;

  case 59:
#line 450 "yacc_sql.y"
         {  // select *
		RelAttr attr;
		relation_attr_init(&attr, NULL, "*", NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);

		(yyval.relattr1) = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy((yyval.relattr1), CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
#line 2011 "yacc_sql.tab.c"
    break;

  case 60:
#line 460 "yacc_sql.y"
                             { 
		relation_attr_init(&CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] , NULL, "*", NULL, 2);

		(yyval.relattr1) = (RelAttr *)malloc(sizeof(RelAttr) * CONTEXT->rel_attr_length);
		memcpy((yyval.relattr1), CONTEXT->rel_attrs, sizeof(RelAttr) * CONTEXT->rel_attr_length);
		CONTEXT->rel_attr_length = 0; 
	}
#line 2023 "yacc_sql.tab.c"
    break;

  case 61:
#line 470 "yacc_sql.y"
                {}
#line 2029 "yacc_sql.tab.c"
    break;

  case 62:
#line 471 "yacc_sql.y"
                          {}
#line 2035 "yacc_sql.tab.c"
    break;

  case 63:
#line 475 "yacc_sql.y"
          { // select age
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2045 "yacc_sql.tab.c"
    break;

  case 64:
#line 480 "yacc_sql.y"
                    { // select t1.age
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2055 "yacc_sql.tab.c"
    break;

  case 65:
#line 485 "yacc_sql.y"
                     { // select t1.*
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-2].string), "*", NULL, 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2065 "yacc_sql.tab.c"
    break;

  case 67:
#line 494 "yacc_sql.y"
                                   { }
#line 2071 "yacc_sql.tab.c"
    break;

  case 69:
#line 499 "yacc_sql.y"
                                 {
		selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-2].string));
    }
#line 2079 "yacc_sql.tab.c"
    break;

  case 70:
#line 506 "yacc_sql.y"
        {	// 只有COUNT允许COUNT(*)
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2089 "yacc_sql.tab.c"
    break;

  case 71:
#line 512 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2099 "yacc_sql.tab.c"
    break;

  case 72:
#line 518 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2109 "yacc_sql.tab.c"
    break;

  case 73:
#line 524 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), (yyvsp[-3].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2119 "yacc_sql.tab.c"
    break;

  case 74:
#line 530 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2129 "yacc_sql.tab.c"
    break;

  case 75:
#line 536 "yacc_sql.y"
        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), "*", (yyvsp[-5].string), 0);
		CONTEXT->rel_attrs[CONTEXT->rel_attr_length++] = attr;
	}
#line 2139 "yacc_sql.tab.c"
    break;

  case 76:
#line 544 "yacc_sql.y"
             { (yyval.string) = (yyvsp[0].string);}
#line 2145 "yacc_sql.tab.c"
    break;

  case 77:
#line 545 "yacc_sql.y"
                 {(yyval.string) = number_to_str((yyvsp[0].number));}
#line 2151 "yacc_sql.tab.c"
    break;

  case 78:
#line 546 "yacc_sql.y"
             {(yyval.string) = (yyvsp[0].string);}
#line 2157 "yacc_sql.tab.c"
    break;

  case 79:
#line 550 "yacc_sql.y"
                         {
		CONTEXT->rels[CONTEXT->rel_length++] = (yyvsp[-1].string);
		CONTEXT->rels[CONTEXT->rel_length++] = "NULL";
		(yyval.relation) = ( const char **)malloc(sizeof(const char*) * CONTEXT->rel_length);
		memcpy((yyval.relation), CONTEXT->rels, sizeof(const char*) * CONTEXT->rel_length);
		CONTEXT->rel_length = 0;
	}
#line 2169 "yacc_sql.tab.c"
    break;

  case 80:
#line 560 "yacc_sql.y"
                {}
#line 2175 "yacc_sql.tab.c"
    break;

  case 81:
#line 561 "yacc_sql.y"
                        {	
		CONTEXT->rels[CONTEXT->rel_length++] = (yyvsp[-1].string);
	}
#line 2183 "yacc_sql.tab.c"
    break;

  case 82:
#line 567 "yacc_sql.y"
                { 
		(yyval.condition1) = NULL; 
		// 每次where结束都清零长度，多个子查询的where不会相互影响
		CONTEXT->condition_length = 0; 
	}
#line 2193 "yacc_sql.tab.c"
    break;

  case 83:
#line 572 "yacc_sql.y"
                                     {	
		// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
		RelAttr left_attr;
		relation_attr_init(&left_attr, NULL, "NULL", NULL, 0);
		RelAttr right_attr;
		relation_attr_init(&right_attr, NULL, "NULL", NULL, 0);
		condition_init(&CONTEXT->conditions[CONTEXT->condition_length++], NO_OP, 1, &left_attr, NULL, 1, &right_attr, NULL, NULL);

		(yyval.condition1) = ( Condition *)malloc(sizeof( Condition) * CONTEXT->condition_length);
		memcpy((yyval.condition1), CONTEXT->conditions, sizeof( Condition) * CONTEXT->condition_length);
		// 每次where结束都清零长度，多个子查询的where不会相互影响
		CONTEXT->condition_length = 0; 
		// 由于select里只有condition涉及到value_length，所以一并在此清零
		CONTEXT->value_length = 0;
	}
#line 2213 "yacc_sql.tab.c"
    break;

  case 85:
#line 591 "yacc_sql.y"
                                  {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
		selects_append_conditions_with_num(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);
		// 每次where结束都清零长度，多个子查询的where不会相互影响
		CONTEXT->condition_length = 0; 
		// 由于select里只有condition涉及到value_length，所以一并在此清零
		CONTEXT->value_length = 0;
	}
#line 2226 "yacc_sql.tab.c"
    break;

  case 87:
#line 603 "yacc_sql.y"
                                   {
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 2234 "yacc_sql.tab.c"
    break;

  case 88:
#line 610 "yacc_sql.y"
                {
			RelAttr left_attr;
			// $1 为属性名称
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);

			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 0, NULL, right_value, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
#line 2250 "yacc_sql.tab.c"
    break;

  case 89:
#line 622 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 0, NULL, left_value, 0, NULL, right_value, NULL);
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
#line 2274 "yacc_sql.tab.c"
    break;

  case 90:
#line 642 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 1, &right_attr, NULL, NULL);
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
#line 2298 "yacc_sql.tab.c"
    break;

  case 91:
#line 662 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 0, NULL, left_value, 1, &right_attr, NULL, NULL);
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
#line 2324 "yacc_sql.tab.c"
    break;

  case 92:
#line 684 "yacc_sql.y"
                {
			RelAttr left_attr;
			// $1为表名，$3为属性名
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string), NULL, 0);
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 0, NULL, right_value, NULL);
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
#line 2350 "yacc_sql.tab.c"
    break;

  case 93:
#line 706 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, (yyvsp[-3].number), 0, NULL, left_value, 1, &right_attr, NULL, NULL);
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
#line 2375 "yacc_sql.tab.c"
    break;

  case 94:
#line 727 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-6].string), (yyvsp[-4].string), NULL, 0);
			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);

			Condition condition;
			condition_init(&condition, (yyvsp[-3].number), 1, &left_attr, NULL, 1, &right_attr, NULL, NULL);
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
#line 2398 "yacc_sql.tab.c"
    break;

  case 95:
#line 745 "yacc_sql.y"
                      {
		RelAttr left_attr;
		// $1 为属性名称
		relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);

		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NULL, 1, &left_attr, NULL, 0, NULL, right_value, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2415 "yacc_sql.tab.c"
    break;

  case 96:
#line 757 "yacc_sql.y"
                          { // id is not null
		RelAttr left_attr;
		// $1 为属性名称
		relation_attr_init(&left_attr, NULL, (yyvsp[-3].string), NULL, 0);

		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NOT_NULL, 1, &left_attr, NULL, 0, NULL, right_value, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2432 "yacc_sql.tab.c"
    break;

  case 97:
#line 769 "yacc_sql.y"
                             {
		RelAttr left_attr;
		// $1为表名，$3为属性名
		relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string), NULL, 0);
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NULL, 1, &left_attr, NULL, 0, NULL, right_value, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2448 "yacc_sql.tab.c"
    break;

  case 98:
#line 780 "yacc_sql.y"
                                 {
		RelAttr left_attr;
		// $1为表名，$3为属性名
		relation_attr_init(&left_attr, (yyvsp[-5].string), (yyvsp[-3].string), NULL, 0);
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NOT_NULL, 1, &left_attr, NULL, 0, NULL, right_value, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2464 "yacc_sql.tab.c"
    break;

  case 99:
#line 791 "yacc_sql.y"
                             { // null is null/value is not null
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NOT_NULL, 0, NULL, left_value, 0, NULL, right_value, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2478 "yacc_sql.tab.c"
    break;

  case 100:
#line 800 "yacc_sql.y"
                         { //  null is not null/value is null
		value_init_string(&CONTEXT->values[CONTEXT->value_length++], "NULL", true);
		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, IS_NULL, 0, NULL, left_value, 0, NULL, right_value, NULL);
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2492 "yacc_sql.tab.c"
    break;

  case 101:
#line 809 "yacc_sql.y"
                             {
		RelAttr left_attr;
		relation_attr_init(&left_attr, NULL, (yyvsp[-2].string), NULL, 0);

		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[0].selnode));
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2505 "yacc_sql.tab.c"
    break;

  case 102:
#line 817 "yacc_sql.y"
                                     {
		RelAttr left_attr;
		relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string), NULL, 0);

		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[0].selnode));
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2518 "yacc_sql.tab.c"
    break;

  case 103:
#line 825 "yacc_sql.y"
                                 {
		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		condition_init(&condition, (yyvsp[-1].number), 0, NULL, left_value, 2, NULL, NULL, (yyvsp[0].selnode));
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2530 "yacc_sql.tab.c"
    break;

  case 104:
#line 832 "yacc_sql.y"
                                 {
		// 反过来，当作正的解析
		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

		Condition condition;
		if ((yyvsp[-1].number) == GREAT_THAN || (yyvsp[-1].number) == GREAT_EQUAL) {
			condition_init(&condition, (yyvsp[-1].number) - 2, 0, NULL, left_value, 2, NULL, NULL, (yyvsp[-2].selnode));
		} else if ((yyvsp[-1].number) == LESS_THAN || (yyvsp[-1].number) == LESS_EQUAL) {
			condition_init(&condition, (yyvsp[-1].number) + 2, 0, NULL, left_value, 2, NULL, NULL, (yyvsp[-2].selnode));
		} else {
			condition_init(&condition, (yyvsp[-1].number), 0, NULL, left_value, 2, NULL, NULL, (yyvsp[-2].selnode));
		}
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2549 "yacc_sql.tab.c"
    break;

  case 105:
#line 846 "yacc_sql.y"
                             {
		// 反过来，当作正的解析
		RelAttr left_attr;
		relation_attr_init(&left_attr, NULL, (yyvsp[0].string), NULL, 0);

		Condition condition;
		if ((yyvsp[-1].number) == GREAT_THAN || (yyvsp[-1].number) == GREAT_EQUAL) {
			condition_init(&condition, (yyvsp[-1].number) - 2, 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[-2].selnode));
		} else if ((yyvsp[-1].number) == LESS_THAN || (yyvsp[-1].number) == LESS_EQUAL) {
			condition_init(&condition, (yyvsp[-1].number) + 2, 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[-2].selnode));
		} else {
			condition_init(&condition, (yyvsp[-1].number), 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[-2].selnode));
		}
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2569 "yacc_sql.tab.c"
    break;

  case 106:
#line 861 "yacc_sql.y"
                                     {
		// 反过来，当作正的解析
		RelAttr left_attr;
		relation_attr_init(&left_attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);

		Condition condition;
		if ((yyvsp[-3].number) == GREAT_THAN || (yyvsp[-3].number) == GREAT_EQUAL) {
			condition_init(&condition, (yyvsp[-3].number) - 2, 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[-4].selnode));
		} else if ((yyvsp[-3].number) == LESS_THAN || (yyvsp[-3].number) == LESS_EQUAL) {
			condition_init(&condition, (yyvsp[-3].number) + 2, 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[-4].selnode));
		} else {
			condition_init(&condition, (yyvsp[-3].number), 1, &left_attr, NULL, 2, NULL, NULL, (yyvsp[-4].selnode));
		}
		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2589 "yacc_sql.tab.c"
    break;

  case 107:
#line 879 "yacc_sql.y"
             { (yyval.number) = 0; }
#line 2595 "yacc_sql.tab.c"
    break;

  case 108:
#line 880 "yacc_sql.y"
         { (yyval.number) = 3; }
#line 2601 "yacc_sql.tab.c"
    break;

  case 109:
#line 881 "yacc_sql.y"
         { (yyval.number) = 5; }
#line 2607 "yacc_sql.tab.c"
    break;

  case 110:
#line 882 "yacc_sql.y"
         { (yyval.number) = 2; }
#line 2613 "yacc_sql.tab.c"
    break;

  case 111:
#line 883 "yacc_sql.y"
         { (yyval.number) = 4; }
#line 2619 "yacc_sql.tab.c"
    break;

  case 112:
#line 884 "yacc_sql.y"
         { (yyval.number) = 1; }
#line 2625 "yacc_sql.tab.c"
    break;

  case 113:
#line 885 "yacc_sql.y"
             { (yyval.number) = 8; }
#line 2631 "yacc_sql.tab.c"
    break;

  case 114:
#line 886 "yacc_sql.y"
                 { (yyval.number) = 9; }
#line 2637 "yacc_sql.tab.c"
    break;

  case 115:
#line 890 "yacc_sql.y"
                                                        {
		(yyval.selnode) = (Selects*)malloc(sizeof(Selects));
		// 结构体malloc，后面要不跟上memcpy要不用memset全部默认初始化
		memset((yyval.selnode), 0, sizeof(Selects));

		selects_append_relations((yyval.selnode), (yyvsp[-2].relation)); // from_rel
		if ((yyvsp[-1].condition1) != NULL) {
			selects_append_conditions((yyval.selnode), (yyvsp[-1].condition1)); // where
		}
		selects_append_attributes((yyval.selnode), (yyvsp[-3].relattr1)); // select_attr
	}
#line 2653 "yacc_sql.tab.c"
    break;

  case 117:
#line 905 "yacc_sql.y"
                              {
		;
	}
#line 2661 "yacc_sql.tab.c"
    break;

  case 118:
#line 911 "yacc_sql.y"
                  {
		;
	}
#line 2669 "yacc_sql.tab.c"
    break;

  case 119:
#line 914 "yacc_sql.y"
                                      {}
#line 2675 "yacc_sql.tab.c"
    break;

  case 120:
#line 918 "yacc_sql.y"
           {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[0].string), NULL, 0);
		selects_append_group(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2685 "yacc_sql.tab.c"
    break;

  case 121:
#line 923 "yacc_sql.y"
                    {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string), NULL, 0);
		selects_append_group(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2695 "yacc_sql.tab.c"
    break;

  case 123:
#line 932 "yacc_sql.y"
                             {
	}
#line 2702 "yacc_sql.tab.c"
    break;

  case 124:
#line 937 "yacc_sql.y"
                  {
		// order by A, B, C，实际上加入顺序为C、B、A，方便后面排序
	}
#line 2710 "yacc_sql.tab.c"
    break;

  case 125:
#line 940 "yacc_sql.y"
                                    {}
#line 2716 "yacc_sql.tab.c"
    break;

  case 126:
#line 943 "yacc_sql.y"
                  {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL, 0);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2726 "yacc_sql.tab.c"
    break;

  case 127:
#line 948 "yacc_sql.y"
                  {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string), NULL, 1);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2736 "yacc_sql.tab.c"
    break;

  case 128:
#line 953 "yacc_sql.y"
                            {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL, 0);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2746 "yacc_sql.tab.c"
    break;

  case 129:
#line 958 "yacc_sql.y"
                         {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string), NULL, 1);
		selects_append_order(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2756 "yacc_sql.tab.c"
    break;

  case 131:
#line 966 "yacc_sql.y"
              {}
#line 2762 "yacc_sql.tab.c"
    break;

  case 132:
#line 970 "yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 2771 "yacc_sql.tab.c"
    break;


#line 2775 "yacc_sql.tab.c"

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
#line 975 "yacc_sql.y"

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
