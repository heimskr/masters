/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

#ifndef YY_JS_INCLUDE_BISON_H_INCLUDED
# define YY_JS_INCLUDE_BISON_H_INCLUDED
/* Debug traces.  */
#ifndef JSDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define JSDEBUG 1
#  else
#   define JSDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define JSDEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined JSDEBUG */
#if JSDEBUG
extern int jsdebug;
#endif

/* Token kinds.  */
#ifndef JSTOKENTYPE
# define JSTOKENTYPE
  enum jstokentype
  {
    JSEMPTY = -2,
    JSEOF = 0,                     /* "end of file"  */
    JSerror = 256,                 /* error  */
    JSUNDEF = 257,                 /* "invalid token"  */
    JSTOK_ROOT = 258,              /* JSTOK_ROOT  */
    JSTOK_NUMBER = 259,            /* JSTOK_NUMBER  */
    JSTOK_IDENT = 260,             /* JSTOK_IDENT  */
    JSTOK_STRING = 261,            /* JSTOK_STRING  */
    JSTOK_LPAREN = 262,            /* "("  */
    JSTOK_RPAREN = 263,            /* ")"  */
    JSTOK_PLUS = 264,              /* "+"  */
    JSTOK_MINUS = 265,             /* "-"  */
    JSTOK_TIMES = 266,             /* "*"  */
    JSTOK_DIV = 267,               /* "/"  */
    JSTOK_XOR = 268,               /* "^"  */
    JSTOK_TRUE = 269,              /* "true"  */
    JSTOK_FALSE = 270,             /* "false"  */
    JSTOK_IF = 271,                /* "if"  */
    JSTOK_ELSE = 272,              /* "else"  */
    JSTOK_WHILE = 273,             /* "while"  */
    JSTOK_SEMI = 274,              /* ";"  */
    JSTOK_TEQ = 275,               /* "==="  */
    JSTOK_ASSIGN = 276,            /* "="  */
    JSTOK_LAND = 277,              /* "&&"  */
    JSTOK_LOR = 278,               /* "||"  */
    JSTOK_AND = 279,               /* "&"  */
    JSTOK_OR = 280,                /* "|"  */
    JSTOK_LTE = 281,               /* "<="  */
    JSTOK_GTE = 282,               /* ">="  */
    JSTOK_LT = 283,                /* "<"  */
    JSTOK_GT = 284,                /* ">"  */
    JSTOK_NOT = 285,               /* "!"  */
    JSTOK_TILDE = 286,             /* "~"  */
    JSTOK_NTEQ = 287,              /* "!=="  */
    JSTOK_LBRACE = 288,            /* "{"  */
    JSTOK_RBRACE = 289,            /* "}"  */
    JSTOK_LSQUARE = 290,           /* "["  */
    JSTOK_RSQUARE = 291,           /* "]"  */
    JSTOK_COMMA = 292,             /* ","  */
    JSTOK_QUESTION = 293,          /* "?"  */
    JSTOK_COLON = 294,             /* ":"  */
    JSTOK_RETURN = 295,            /* "return"  */
    JSTOK_PERIOD = 296,            /* "."  */
    JSTOK_VOID = 297,              /* "void"  */
    JSTOK_LSHIFT = 298,            /* "<<"  */
    JSTOK_RSHIFTA = 299,           /* ">>"  */
    JSTOK_RSHIFTL = 300,           /* ">>>"  */
    JSTOK_META_NAME = 301,         /* "#name"  */
    JSTOK_META_AUTHOR = 302,       /* "#author"  */
    JSTOK_META_VERSION = 303,      /* "#version"  */
    JSTOK_META_ORCID = 304,        /* "#orcid"  */
    JSTOK_BREAK = 305,             /* "break"  */
    JSTOK_CONTINUE = 306,          /* "continue"  */
    JSTOK_FOR = 307,               /* "for"  */
    JSTOK_MOD = 308,               /* "%"  */
    JSTOK_PLUSEQ = 309,            /* "+="  */
    JSTOK_MINUSEQ = 310,           /* "-="  */
    JSTOK_PLUSPLUS = 311,          /* "++"  */
    JSTOK_MINUSMINUS = 312,        /* "--"  */
    JSTOK_DO = 313,                /* "do"  */
    JSTOK_NULL = 314,              /* "null"  */
    JSTOK_DIVEQ = 315,             /* "/="  */
    JSTOK_TIMESEQ = 316,           /* "*="  */
    JSTOK_MODEQ = 317,             /* "%="  */
    JSTOK_SRAEQ = 318,             /* ">>="  */
    JSTOK_SRLEQ = 319,             /* ">>>="  */
    JSTOK_SLEQ = 320,              /* "<<="  */
    JSTOK_ANDEQ = 321,             /* "&="  */
    JSTOK_LANDEQ = 322,            /* "&&="  */
    JSTOK_OREQ = 323,              /* "|="  */
    JSTOK_LOREQ = 324,             /* "||="  */
    JSTOK_XOREQ = 325,             /* "^="  */
    JSTOK_EXP = 326,               /* "**"  */
    JSTOK_EXPEQ = 327,             /* "**="  */
    JSTOK_NEW = 328,               /* "new"  */
    JSTOK_DELETE = 329,            /* "delete"  */
    JSTOK_UNDEFINED = 330,         /* "undefined"  */
    JSTOK_NAN = 331,               /* "NaN"  */
    JSTOK_LET = 332,               /* "let"  */
    JSTOK_CONST = 333,             /* "const"  */
    JSTOK_INFINITY = 334,          /* "Infinity"  */
    JSTOK_FUNCTION = 335,          /* "function"  */
    JSTOK_TYPEOF = 336,            /* "typeof"  */
    JSTOK_IN = 337,                /* "in"  */
    JSTOK_INSTANCEOF = 338,        /* "instanceof"  */
    JS_LIST = 339,                 /* JS_LIST  */
    JS_BLOCK = 340,                /* JS_BLOCK  */
    JS_EMPTY = 341,                /* JS_EMPTY  */
    JS_POSTPLUS = 342,             /* JS_POSTPLUS  */
    JS_POSTMINUS = 343,            /* JS_POSTMINUS  */
    PREFIX = 346,                  /* PREFIX  */
    UNARY = 347,                   /* UNARY  */
    POSTFIX = 348,                 /* POSTFIX  */
    NEW_NO_ARGS = 349,             /* NEW_NO_ARGS  */
    CALL = 350,                    /* CALL  */
    NEW_ARGS = 351                 /* NEW_ARGS  */
  };
  typedef enum jstokentype jstoken_kind_t;
#endif

/* Value type.  */
#if ! defined JSSTYPE && ! defined JSSTYPE_IS_DECLARED
typedef int JSSTYPE;
# define JSSTYPE_IS_TRIVIAL 1
# define JSSTYPE_IS_DECLARED 1
#endif


extern JSSTYPE jslval;


int jsparse (void);


#endif /* !YY_JS_INCLUDE_BISON_H_INCLUDED  */
