/* A Bison parser, made by GNU Bison 3.7.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_YACC_SQL_TAB_H_INCLUDED
# define YY_YY_YACC_SQL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SEMICOLON = 258,               /* SEMICOLON  */
    CREATE = 259,                  /* CREATE  */
    DROP = 260,                    /* DROP  */
    TABLE = 261,                   /* TABLE  */
    TABLES = 262,                  /* TABLES  */
    INDEX = 263,                   /* INDEX  */
    SELECT = 264,                  /* SELECT  */
    DESC = 265,                    /* DESC  */
    SHOW = 266,                    /* SHOW  */
    SYNC = 267,                    /* SYNC  */
    INSERT = 268,                  /* INSERT  */
    DELETE = 269,                  /* DELETE  */
    UPDATE = 270,                  /* UPDATE  */
    LBRACE = 271,                  /* LBRACE  */
    RBRACE = 272,                  /* RBRACE  */
    COMMA = 273,                   /* COMMA  */
    TRX_BEGIN = 274,               /* TRX_BEGIN  */
    TRX_COMMIT = 275,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 276,            /* TRX_ROLLBACK  */
    INT_T = 277,                   /* INT_T  */
    STRING_T = 278,                /* STRING_T  */
    FLOAT_T = 279,                 /* FLOAT_T  */
    ORDER = 280,                   /* ORDER  */
    ASC = 281,                     /* ASC  */
    BY = 282,                      /* BY  */
    DATE_T = 283,                  /* DATE_T  */
    UNIQUE = 284,                  /* UNIQUE  */
    HELP = 285,                    /* HELP  */
    EXIT = 286,                    /* EXIT  */
    DOT = 287,                     /* DOT  */
    INTO = 288,                    /* INTO  */
    VALUES = 289,                  /* VALUES  */
    FROM = 290,                    /* FROM  */
    WHERE = 291,                   /* WHERE  */
    AND = 292,                     /* AND  */
    SET = 293,                     /* SET  */
    ON = 294,                      /* ON  */
    LOAD = 295,                    /* LOAD  */
    DATA = 296,                    /* DATA  */
    INFILE = 297,                  /* INFILE  */
    NULLABLE = 298,                /* NULLABLE  */
    GROUP = 299,                   /* GROUP  */
    IS = 300,                      /* IS  */
    NOT = 301,                     /* NOT  */
    EQ = 302,                      /* EQ  */
    LT = 303,                      /* LT  */
    GT = 304,                      /* GT  */
    LE = 305,                      /* LE  */
    GE = 306,                      /* GE  */
    NE = 307,                      /* NE  */
    NULL_T = 308,                  /* NULL_T  */
    INNER = 309,                   /* INNER  */
    JOIN = 310,                    /* JOIN  */
    IN = 311,                      /* IN  */
    NUMBER = 312,                  /* NUMBER  */
    FLOAT = 313,                   /* FLOAT  */
    ID = 314,                      /* ID  */
    PATH = 315,                    /* PATH  */
    SSS = 316,                     /* SSS  */
    STAR = 317,                    /* STAR  */
    STRING_V = 318,                /* STRING_V  */
    COUNT = 319,                   /* COUNT  */
    OTHER_FUNCTION_TYPE = 320,     /* OTHER_FUNCTION_TYPE  */
    Column = 321                   /* Column  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 135 "yacc_sql.y"

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

#line 144 "yacc_sql.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (void *scanner);

#endif /* !YY_YY_YACC_SQL_TAB_H_INCLUDED  */
