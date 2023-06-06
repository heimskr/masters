/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         JSSTYPE
/* Substitute the variable and function names.  */
#define yyparse         jsparse
#define yylex           jslex
#define yyerror         jserror
#define yydebug         jsdebug
#define yynerrs         jsnerrs
#define yylval          jslval
#define yychar          jschar

/* First part of user prologue.  */
#line 1 "src/parser.y"

#include <cassert>
#include <cstdarg>
#include <initializer_list>

#define NO_YYPARSE
#include "Lexer.h"
#include "ASTNode.h"
#include "Parser.h"

// Disable PVS-Studio warnings about branches that do the same thing.
//-V::1037

template <typename ...Args>
void D(Args && ...args) {
	(void) std::initializer_list<int> {
		((void) delete args, 0)...
	};
}

using AN = ASTNode;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#line 105 "src/bison.cpp"

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

#include "bison.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_JSTOK_ROOT = 3,                 /* JSTOK_ROOT  */
  YYSYMBOL_JSTOK_NUMBER = 4,               /* JSTOK_NUMBER  */
  YYSYMBOL_JSTOK_IDENT = 5,                /* JSTOK_IDENT  */
  YYSYMBOL_JSTOK_STRING = 6,               /* JSTOK_STRING  */
  YYSYMBOL_JSTOK_LPAREN = 7,               /* "("  */
  YYSYMBOL_JSTOK_RPAREN = 8,               /* ")"  */
  YYSYMBOL_JSTOK_PLUS = 9,                 /* "+"  */
  YYSYMBOL_JSTOK_MINUS = 10,               /* "-"  */
  YYSYMBOL_JSTOK_TIMES = 11,               /* "*"  */
  YYSYMBOL_JSTOK_DIV = 12,                 /* "/"  */
  YYSYMBOL_JSTOK_XOR = 13,                 /* "^"  */
  YYSYMBOL_JSTOK_TRUE = 14,                /* "true"  */
  YYSYMBOL_JSTOK_FALSE = 15,               /* "false"  */
  YYSYMBOL_JSTOK_IF = 16,                  /* "if"  */
  YYSYMBOL_JSTOK_ELSE = 17,                /* "else"  */
  YYSYMBOL_JSTOK_WHILE = 18,               /* "while"  */
  YYSYMBOL_JSTOK_SEMI = 19,                /* ";"  */
  YYSYMBOL_JSTOK_TEQ = 20,                 /* "==="  */
  YYSYMBOL_JSTOK_ASSIGN = 21,              /* "="  */
  YYSYMBOL_JSTOK_LAND = 22,                /* "&&"  */
  YYSYMBOL_JSTOK_LOR = 23,                 /* "||"  */
  YYSYMBOL_JSTOK_AND = 24,                 /* "&"  */
  YYSYMBOL_JSTOK_OR = 25,                  /* "|"  */
  YYSYMBOL_JSTOK_LTE = 26,                 /* "<="  */
  YYSYMBOL_JSTOK_GTE = 27,                 /* ">="  */
  YYSYMBOL_JSTOK_LT = 28,                  /* "<"  */
  YYSYMBOL_JSTOK_GT = 29,                  /* ">"  */
  YYSYMBOL_JSTOK_NOT = 30,                 /* "!"  */
  YYSYMBOL_JSTOK_TILDE = 31,               /* "~"  */
  YYSYMBOL_JSTOK_NTEQ = 32,                /* "!=="  */
  YYSYMBOL_JSTOK_LBRACE = 33,              /* "{"  */
  YYSYMBOL_JSTOK_RBRACE = 34,              /* "}"  */
  YYSYMBOL_JSTOK_LSQUARE = 35,             /* "["  */
  YYSYMBOL_JSTOK_RSQUARE = 36,             /* "]"  */
  YYSYMBOL_JSTOK_COMMA = 37,               /* ","  */
  YYSYMBOL_JSTOK_QUESTION = 38,            /* "?"  */
  YYSYMBOL_JSTOK_COLON = 39,               /* ":"  */
  YYSYMBOL_JSTOK_RETURN = 40,              /* "return"  */
  YYSYMBOL_JSTOK_PERIOD = 41,              /* "."  */
  YYSYMBOL_JSTOK_VOID = 42,                /* "void"  */
  YYSYMBOL_JSTOK_LSHIFT = 43,              /* "<<"  */
  YYSYMBOL_JSTOK_RSHIFTA = 44,             /* ">>"  */
  YYSYMBOL_JSTOK_RSHIFTL = 45,             /* ">>>"  */
  YYSYMBOL_JSTOK_META_NAME = 46,           /* "#name"  */
  YYSYMBOL_JSTOK_META_AUTHOR = 47,         /* "#author"  */
  YYSYMBOL_JSTOK_META_VERSION = 48,        /* "#version"  */
  YYSYMBOL_JSTOK_META_ORCID = 49,          /* "#orcid"  */
  YYSYMBOL_JSTOK_BREAK = 50,               /* "break"  */
  YYSYMBOL_JSTOK_CONTINUE = 51,            /* "continue"  */
  YYSYMBOL_JSTOK_FOR = 52,                 /* "for"  */
  YYSYMBOL_JSTOK_MOD = 53,                 /* "%"  */
  YYSYMBOL_JSTOK_PLUSEQ = 54,              /* "+="  */
  YYSYMBOL_JSTOK_MINUSEQ = 55,             /* "-="  */
  YYSYMBOL_JSTOK_PLUSPLUS = 56,            /* "++"  */
  YYSYMBOL_JSTOK_MINUSMINUS = 57,          /* "--"  */
  YYSYMBOL_JSTOK_DO = 58,                  /* "do"  */
  YYSYMBOL_JSTOK_NULL = 59,                /* "null"  */
  YYSYMBOL_JSTOK_DIVEQ = 60,               /* "/="  */
  YYSYMBOL_JSTOK_TIMESEQ = 61,             /* "*="  */
  YYSYMBOL_JSTOK_MODEQ = 62,               /* "%="  */
  YYSYMBOL_JSTOK_SRAEQ = 63,               /* ">>="  */
  YYSYMBOL_JSTOK_SRLEQ = 64,               /* ">>>="  */
  YYSYMBOL_JSTOK_SLEQ = 65,                /* "<<="  */
  YYSYMBOL_JSTOK_ANDEQ = 66,               /* "&="  */
  YYSYMBOL_JSTOK_LANDEQ = 67,              /* "&&="  */
  YYSYMBOL_JSTOK_OREQ = 68,                /* "|="  */
  YYSYMBOL_JSTOK_LOREQ = 69,               /* "||="  */
  YYSYMBOL_JSTOK_XOREQ = 70,               /* "^="  */
  YYSYMBOL_JSTOK_EXP = 71,                 /* "**"  */
  YYSYMBOL_JSTOK_EXPEQ = 72,               /* "**="  */
  YYSYMBOL_JSTOK_NEW = 73,                 /* "new"  */
  YYSYMBOL_JSTOK_DELETE = 74,              /* "delete"  */
  YYSYMBOL_JSTOK_UNDEFINED = 75,           /* "undefined"  */
  YYSYMBOL_JSTOK_NAN = 76,                 /* "NaN"  */
  YYSYMBOL_JSTOK_LET = 77,                 /* "let"  */
  YYSYMBOL_JSTOK_CONST = 78,               /* "const"  */
  YYSYMBOL_JSTOK_INFINITY = 79,            /* "Infinity"  */
  YYSYMBOL_JSTOK_FUNCTION = 80,            /* "function"  */
  YYSYMBOL_JSTOK_TYPEOF = 81,              /* "typeof"  */
  YYSYMBOL_JSTOK_IN = 82,                  /* "in"  */
  YYSYMBOL_JSTOK_INSTANCEOF = 83,          /* "instanceof"  */
  YYSYMBOL_JS_LIST = 84,                   /* JS_LIST  */
  YYSYMBOL_JS_BLOCK = 85,                  /* JS_BLOCK  */
  YYSYMBOL_JS_EMPTY = 86,                  /* JS_EMPTY  */
  YYSYMBOL_JS_POSTPLUS = 87,               /* JS_POSTPLUS  */
  YYSYMBOL_JS_POSTMINUS = 88,              /* JS_POSTMINUS  */
  YYSYMBOL_89_ = 89,                       /* "=="  */
  YYSYMBOL_90_ = 90,                       /* "!="  */
  YYSYMBOL_PREFIX = 91,                    /* PREFIX  */
  YYSYMBOL_UNARY = 92,                     /* UNARY  */
  YYSYMBOL_POSTFIX = 93,                   /* POSTFIX  */
  YYSYMBOL_NEW_NO_ARGS = 94,               /* NEW_NO_ARGS  */
  YYSYMBOL_CALL = 95,                      /* CALL  */
  YYSYMBOL_NEW_ARGS = 96,                  /* NEW_ARGS  */
  YYSYMBOL_YYACCEPT = 97,                  /* $accept  */
  YYSYMBOL_start = 98,                     /* start  */
  YYSYMBOL_program = 99,                   /* program  */
  YYSYMBOL_statement = 100,                /* statement  */
  YYSYMBOL_var_def = 101,                  /* var_def  */
  YYSYMBOL_var_decl = 102,                 /* var_decl  */
  YYSYMBOL_block = 103,                    /* block  */
  YYSYMBOL_statements = 104,               /* statements  */
  YYSYMBOL_conditional = 105,              /* conditional  */
  YYSYMBOL_while_loop = 106,               /* while_loop  */
  YYSYMBOL_for_loop = 107,                 /* for_loop  */
  YYSYMBOL_expr_ = 108,                    /* expr_  */
  YYSYMBOL_expr = 109,                     /* expr  */
  YYSYMBOL_string = 110,                   /* string  */
  YYSYMBOL_boolean = 111,                  /* boolean  */
  YYSYMBOL_function_call = 112,            /* function_call  */
  YYSYMBOL_exprlist = 113,                 /* exprlist  */
  YYSYMBOL_exprlist_ = 114,                /* exprlist_  */
  YYSYMBOL_number = 115,                   /* number  */
  YYSYMBOL_ident = 116                     /* ident  */
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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if 1

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
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined JSSTYPE_IS_TRIVIAL && JSSTYPE_IS_TRIVIAL)))

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1706

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  97
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  102
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  197

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   351


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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96
};

#if JSDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   147,   147,   149,   150,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   165,   166,   168,
     169,   174,   176,   177,   179,   180,   182,   183,   185,   187,
     187,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   252,   254,   254,   256,   258,   259,   261,
     262,   270,   271
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "JSTOK_ROOT",
  "JSTOK_NUMBER", "JSTOK_IDENT", "JSTOK_STRING", "\"(\"", "\")\"", "\"+\"",
  "\"-\"", "\"*\"", "\"/\"", "\"^\"", "\"true\"", "\"false\"", "\"if\"",
  "\"else\"", "\"while\"", "\";\"", "\"===\"", "\"=\"", "\"&&\"", "\"||\"",
  "\"&\"", "\"|\"", "\"<=\"", "\">=\"", "\"<\"", "\">\"", "\"!\"", "\"~\"",
  "\"!==\"", "\"{\"", "\"}\"", "\"[\"", "\"]\"", "\",\"", "\"?\"", "\":\"",
  "\"return\"", "\".\"", "\"void\"", "\"<<\"", "\">>\"", "\">>>\"",
  "\"#name\"", "\"#author\"", "\"#version\"", "\"#orcid\"", "\"break\"",
  "\"continue\"", "\"for\"", "\"%\"", "\"+=\"", "\"-=\"", "\"++\"",
  "\"--\"", "\"do\"", "\"null\"", "\"/=\"", "\"*=\"", "\"%=\"", "\">>=\"",
  "\">>>=\"", "\"<<=\"", "\"&=\"", "\"&&=\"", "\"|=\"", "\"||=\"",
  "\"^=\"", "\"**\"", "\"**=\"", "\"new\"", "\"delete\"", "\"undefined\"",
  "\"NaN\"", "\"let\"", "\"const\"", "\"Infinity\"", "\"function\"",
  "\"typeof\"", "\"in\"", "\"instanceof\"", "JS_LIST", "JS_BLOCK",
  "JS_EMPTY", "JS_POSTPLUS", "JS_POSTMINUS", "\"==\"", "\"!=\"", "PREFIX",
  "UNARY", "POSTFIX", "NEW_NO_ARGS", "CALL", "NEW_ARGS", "$accept",
  "start", "program", "statement", "var_def", "var_decl", "block",
  "statements", "conditional", "while_loop", "for_loop", "expr_", "expr",
  "string", "boolean", "function_call", "exprlist", "exprlist_", "number",
  "ident", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-156)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -156,     7,   248,  -156,  -156,  -156,  -156,   356,     1,     1,
    -156,  -156,     3,     8,  -156,   356,   356,  -156,   326,   356,
      -1,     0,    15,   356,   356,   248,  -156,   356,   356,  -156,
    -156,    18,    18,  -156,   356,  -156,  -156,  -156,  -156,  -156,
    -156,  -156,   429,  -156,  -156,  -156,  -156,   495,  -156,  -156,
     356,   356,   101,   101,   170,  -156,   561,   101,  -156,  -156,
     356,   101,   101,     6,   158,   101,  -156,   -18,   -15,   101,
     356,   356,   356,   356,   356,   356,  -156,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
      20,   356,   356,   356,   356,   356,  -156,  -156,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,  -156,   627,   693,  -156,  -156,  -156,   759,
      19,   356,  -156,   356,  -156,   356,  1155,    -9,    21,  1634,
    1634,   332,   332,  1485,  1155,  1221,  1353,  1287,  1543,  1419,
    1596,  1596,  1596,  1596,  1155,   825,   891,  -156,  1623,  1623,
     332,  1221,  1221,  1221,  1221,  1221,  1221,  1221,  1221,  1221,
    1221,  1221,  1221,  1221,   332,  1155,  1596,  1596,   248,   248,
     356,   356,    22,   957,  1023,   356,  -156,  -156,   356,    16,
    -156,    13,  1155,  1089,  -156,  -156,  -156,  1155,  1155,   248,
     356,    17,  -156,    26,  -156,   248,  -156
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,   101,    80,    93,     0,     0,     0,
      94,    95,     0,     0,    16,     0,     0,    23,     0,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,    90,
      91,     0,     0,    92,     0,     3,     9,    10,     5,     8,
       7,     6,     0,    82,    81,    69,    83,     0,    74,    75,
       0,     0,    71,    72,     0,    12,     0,    87,    13,    14,
       0,    76,    78,     0,    85,    86,   102,     0,     0,    88,
     100,     0,     0,     0,     0,     0,    11,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    70,     0,     0,    21,    22,    15,     0,
       0,   100,    19,     0,    20,     0,    98,    99,     0,    44,
      45,    46,    48,    35,    36,    49,    31,    32,    33,    34,
      42,    43,    40,    41,    37,     0,     0,    67,    38,    39,
      50,    51,    52,    55,    53,    56,    58,    59,    57,    60,
      63,    61,    62,    64,    47,    54,    65,    66,     0,     0,
      30,     0,     0,     0,     0,     0,    96,    68,     0,    25,
      26,     0,    29,     0,    84,    17,    18,    97,    73,     0,
      30,     0,    24,     0,    27,     0,    28
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -156,  -156,  -156,   -23,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,  -155,    -7,  -156,  -156,  -156,  -156,   -84,     5,     9
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    35,    36,    37,    38,    54,    39,    40,
      41,   181,    42,    43,    44,    45,   127,   128,    46,    67
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      47,   122,    63,   123,   124,     4,   125,     3,    52,    53,
      50,    56,    57,    48,    49,    51,    61,    62,    58,    59,
      64,    65,    60,    66,   120,   147,   171,    69,   175,   176,
     184,   117,   190,   189,   195,   193,   194,   172,     0,     0,
       0,    68,     0,   114,   115,     0,     0,     0,     0,     0,
       0,     0,     0,   119,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   126,   129,   130,   131,   132,   133,     0,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,     0,   148,   149,   150,   151,   152,     0,
       0,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,     0,     0,    70,     0,
       0,     0,     0,     0,   126,     0,   173,     0,   174,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,    88,     0,     0,     0,
       0,     0,    90,     0,     0,   179,   180,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    97,     0,
       0,     0,     0,   182,   183,   121,   192,     0,   187,     0,
       0,   188,   196,   110,     4,     5,     6,     7,    77,     8,
       9,     0,     0,   182,    10,    11,    12,     0,    13,    14,
      87,     0,     0,    88,     0,     0,     0,     0,     0,    90,
      15,    16,     0,    17,   116,     0,     0,     0,     0,     0,
      18,     0,    19,     0,    96,    97,     0,     0,     0,     0,
      20,    21,    22,     0,     0,     0,    23,    24,    25,    26,
     110,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    27,    28,    29,    30,    31,    32,    33,
       0,    34,     4,     5,     6,     7,     0,     8,     9,     0,
       0,     0,    10,    11,    12,     0,    13,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    15,    16,
       0,    17,     0,     0,     0,     0,     0,     0,    18,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    20,    21,
      22,     0,     0,     0,    23,    24,    25,    26,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    27,    28,    29,    30,    31,    32,    33,     0,    34,
       4,     5,     6,     7,     0,     8,     9,     0,     0,    70,
      10,    11,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,    77,     0,     0,     0,    15,    16,     0,     0,
       4,     5,     6,     7,    87,     8,     9,    88,    19,     0,
      10,    11,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,    23,    24,     0,    26,    15,    16,    96,    97,
       0,     0,     0,     0,     0,     0,     0,     0,    19,    27,
      28,    29,    30,   109,   110,    33,     0,    34,     0,     0,
       0,     0,    23,    24,     0,    26,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    27,
      28,    29,    30,     0,     0,    33,    70,    34,    71,    72,
      73,    74,    75,     0,     0,     0,     0,     0,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,     0,
       0,    87,     0,     0,    88,     0,     0,    89,     0,     0,
      90,     0,    91,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    70,   113,    71,    72,    73,    74,    75,     0,
       0,   111,   112,     0,     0,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,     0,     0,    87,     0,     0,
      88,     0,     0,    89,     0,     0,    90,     0,    91,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    70,     0,
      71,    72,    73,    74,    75,     0,     0,   111,   112,     0,
     118,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,     0,     0,    87,     0,     0,    88,     0,     0,    89,
       0,     0,    90,     0,    91,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    70,   168,    71,    72,    73,    74,
      75,     0,     0,   111,   112,     0,     0,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,     0,     0,    87,
       0,     0,    88,     0,     0,    89,     0,     0,    90,     0,
      91,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      70,   169,    71,    72,    73,    74,    75,     0,     0,   111,
     112,     0,     0,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,     0,     0,    87,     0,     0,    88,     0,
       0,    89,     0,     0,    90,     0,    91,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    70,     0,    71,    72,
      73,    74,    75,     0,     0,   111,   112,     0,   170,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,     0,
       0,    87,     0,     0,    88,     0,     0,    89,     0,     0,
      90,     0,    91,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    70,     0,    71,    72,    73,    74,    75,     0,
       0,   111,   112,     0,     0,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,     0,     0,    87,     0,     0,
      88,   177,     0,    89,     0,     0,    90,     0,    91,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    70,     0,
      71,    72,    73,    74,    75,     0,     0,   111,   112,     0,
       0,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,     0,     0,    87,     0,     0,    88,     0,     0,    89,
     178,     0,    90,     0,    91,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    70,     0,    71,    72,    73,    74,
      75,     0,     0,   111,   112,     0,   185,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,     0,     0,    87,
       0,     0,    88,     0,     0,    89,     0,     0,    90,     0,
      91,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      70,     0,    71,    72,    73,    74,    75,     0,     0,   111,
     112,     0,   186,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,     0,     0,    87,     0,     0,    88,     0,
       0,    89,     0,     0,    90,     0,    91,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    70,   191,    71,    72,
      73,    74,    75,     0,     0,   111,   112,     0,     0,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,     0,
       0,    87,     0,     0,    88,     0,     0,    89,     0,     0,
      90,     0,    91,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    70,     0,    71,    72,    73,    74,    75,     0,
       0,   111,   112,     0,     0,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,     0,     0,    87,     0,     0,
      88,     0,     0,    89,     0,     0,    90,     0,    91,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    70,     0,
      71,    72,    73,    74,    75,     0,     0,   111,   112,     0,
       0,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,     0,     0,    87,     0,     0,    88,     0,     0,     0,
       0,     0,    90,     0,    91,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    70,     0,    71,    72,    73,    74,
      75,     0,     0,   111,   112,     0,     0,    77,     0,    79,
       0,    81,    82,    83,    84,    85,    86,     0,     0,    87,
       0,     0,    88,     0,     0,     0,     0,     0,    90,     0,
      91,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    96,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   109,   110,
      70,     0,    71,    72,    73,    74,    75,     0,     0,   111,
     112,     0,     0,    77,     0,     0,     0,    81,    82,    83,
      84,    85,    86,     0,     0,    87,     0,     0,    88,     0,
       0,     0,     0,     0,    90,     0,    91,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    96,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   109,   110,    70,     0,    71,    72,
      73,    74,    75,     0,     0,   111,   112,     0,     0,    77,
       0,     0,     0,    81,     0,    83,    84,    85,    86,     0,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
      90,     0,    91,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    96,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     109,   110,    70,     0,    71,    72,    73,    74,     0,     0,
       0,   111,   112,     0,     0,    77,     0,     0,     0,    81,
       0,    83,    84,    85,    86,     0,     0,    87,     0,     0,
      88,     0,     0,     0,     0,     0,    90,     0,    91,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    96,    97,     0,     0,     0,     0,     0,     0,     0,
      70,     0,    71,    72,    73,    74,   109,   110,     0,     0,
       0,     0,     0,    77,     0,     0,     0,   111,   112,    83,
      84,    85,    86,     0,     0,    87,     0,     0,    88,     0,
       0,     0,     0,     0,    90,     0,    91,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    96,
      97,     0,     0,    70,     0,    71,    72,    73,    74,     0,
       0,     0,     0,     0,   109,   110,    77,     0,     0,     0,
       0,     0,     0,     0,     0,   111,   112,     0,    87,     0,
      70,    88,    71,    72,    73,    74,     0,    90,     0,    91,
      92,    70,     0,    77,     0,    73,    74,     0,     0,    93,
       0,     0,    96,    97,    77,    87,     0,     0,    88,     0,
       0,     0,     0,     0,    90,     0,    87,   109,   110,    88,
       0,     0,     0,     0,     0,    90,    93,     0,     0,    96,
      97,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      96,    97,     0,     0,   109,   110,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   109,   110
};

static const yytype_int16 yycheck[] =
{
       7,    19,    25,    21,    19,     4,    21,     0,    15,    16,
       7,    18,    19,     8,     9,     7,    23,    24,    19,    19,
      27,    28,     7,     5,    18,     5,     7,    34,    37,     8,
       8,    54,    19,    17,     8,   190,    19,   121,    -1,    -1,
      -1,    32,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    72,    73,    74,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,    -1,     7,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,   170,   171,     7,   189,    -1,   175,    -1,
      -1,   178,   195,    72,     4,     5,     6,     7,    20,     9,
      10,    -1,    -1,   190,    14,    15,    16,    -1,    18,    19,
      32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,
      30,    31,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    42,    -1,    56,    57,    -1,    -1,    -1,    -1,
      50,    51,    52,    -1,    -1,    -1,    56,    57,    58,    59,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    75,    76,    77,    78,    79,
      -1,    81,     4,     5,     6,     7,    -1,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    -1,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    74,    75,    76,    77,    78,    79,    -1,    81,
       4,     5,     6,     7,    -1,     9,    10,    -1,    -1,     7,
      14,    15,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    30,    31,    -1,    -1,
       4,     5,     6,     7,    32,     9,    10,    35,    42,    -1,
      14,    15,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    57,    -1,    59,    30,    31,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    73,
      74,    75,    76,    71,    72,    79,    -1,    81,    -1,    -1,
      -1,    -1,    56,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      74,    75,    76,    -1,    -1,    79,     7,    81,     9,    10,
      11,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      -1,    32,    -1,    -1,    35,    -1,    -1,    38,    -1,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,     7,     8,     9,    10,    11,    12,    13,    -1,
      -1,    82,    83,    -1,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    -1,    32,    -1,    -1,
      35,    -1,    -1,    38,    -1,    -1,    41,    -1,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,
      55,    56,    57,    -1,    -1,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,     7,    -1,
       9,    10,    11,    12,    13,    -1,    -1,    82,    83,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    38,
      -1,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,    -1,
      -1,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,     7,     8,     9,    10,    11,    12,
      13,    -1,    -1,    82,    83,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    -1,    32,
      -1,    -1,    35,    -1,    -1,    38,    -1,    -1,    41,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    54,    55,    56,    57,    -1,    -1,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
       7,     8,     9,    10,    11,    12,    13,    -1,    -1,    82,
      83,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,    -1,
      -1,    38,    -1,    -1,    41,    -1,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    -1,    -1,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,     7,    -1,     9,    10,
      11,    12,    13,    -1,    -1,    82,    83,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      -1,    32,    -1,    -1,    35,    -1,    -1,    38,    -1,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,     7,    -1,     9,    10,    11,    12,    13,    -1,
      -1,    82,    83,    -1,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    -1,    32,    -1,    -1,
      35,    36,    -1,    38,    -1,    -1,    41,    -1,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,
      55,    56,    57,    -1,    -1,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,     7,    -1,
       9,    10,    11,    12,    13,    -1,    -1,    82,    83,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    38,
      39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,    -1,
      -1,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,     7,    -1,     9,    10,    11,    12,
      13,    -1,    -1,    82,    83,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    -1,    32,
      -1,    -1,    35,    -1,    -1,    38,    -1,    -1,    41,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    54,    55,    56,    57,    -1,    -1,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
       7,    -1,     9,    10,    11,    12,    13,    -1,    -1,    82,
      83,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,    -1,
      -1,    38,    -1,    -1,    41,    -1,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    -1,    -1,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,     7,     8,     9,    10,
      11,    12,    13,    -1,    -1,    82,    83,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      -1,    32,    -1,    -1,    35,    -1,    -1,    38,    -1,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,     7,    -1,     9,    10,    11,    12,    13,    -1,
      -1,    82,    83,    -1,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    -1,    32,    -1,    -1,
      35,    -1,    -1,    38,    -1,    -1,    41,    -1,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,
      55,    56,    57,    -1,    -1,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,     7,    -1,
       9,    10,    11,    12,    13,    -1,    -1,    82,    83,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,    -1,
      -1,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,     7,    -1,     9,    10,    11,    12,
      13,    -1,    -1,    82,    83,    -1,    -1,    20,    -1,    22,
      -1,    24,    25,    26,    27,    28,    29,    -1,    -1,    32,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
       7,    -1,     9,    10,    11,    12,    13,    -1,    -1,    82,
      83,    -1,    -1,    20,    -1,    -1,    -1,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,     7,    -1,     9,    10,
      11,    12,    13,    -1,    -1,    82,    83,    -1,    -1,    20,
      -1,    -1,    -1,    24,    -1,    26,    27,    28,    29,    -1,
      -1,    32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,     7,    -1,     9,    10,    11,    12,    -1,    -1,
      -1,    82,    83,    -1,    -1,    20,    -1,    -1,    -1,    24,
      -1,    26,    27,    28,    29,    -1,    -1,    32,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    41,    -1,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       7,    -1,     9,    10,    11,    12,    71,    72,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    82,    83,    26,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    56,
      57,    -1,    -1,     7,    -1,     9,    10,    11,    12,    -1,
      -1,    -1,    -1,    -1,    71,    72,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    32,    -1,
       7,    35,     9,    10,    11,    12,    -1,    41,    -1,    43,
      44,     7,    -1,    20,    -1,    11,    12,    -1,    -1,    53,
      -1,    -1,    56,    57,    20,    32,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    32,    71,    72,    35,
      -1,    -1,    -1,    -1,    -1,    41,    53,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,
      56,    57,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    98,    99,     0,     4,     5,     6,     7,     9,    10,
      14,    15,    16,    18,    19,    30,    31,    33,    40,    42,
      50,    51,    52,    56,    57,    58,    59,    73,    74,    75,
      76,    77,    78,    79,    81,   100,   101,   102,   103,   105,
     106,   107,   109,   110,   111,   112,   115,   109,   115,   115,
       7,     7,   109,   109,   104,    19,   109,   109,    19,    19,
       7,   109,   109,   100,   109,   109,     5,   116,   116,   109,
       7,     9,    10,    11,    12,    13,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    32,    35,    38,
      41,    43,    44,    53,    54,    55,    56,    57,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    82,    83,     8,   109,   109,    34,   100,    19,   109,
      18,     7,    19,    21,    19,    21,   109,   113,   114,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,     5,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,     8,     8,
      19,     7,   114,   109,   109,    37,     8,    36,    39,   100,
     100,   108,   109,   109,     8,    19,    19,   109,   109,    17,
      19,     8,   100,   108,    19,     8,   100
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    97,    98,    99,    99,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   101,   101,   102,
     102,   103,   104,   104,   105,   105,   106,   106,   107,   108,
     108,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   110,   111,   111,   112,   113,   113,   114,
     114,   115,   116
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     3,     1,     5,     5,     3,
       3,     3,     2,     0,     7,     5,     5,     7,     9,     1,
       0,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     1,
       3,     2,     2,     5,     2,     2,     2,     2,     2,     2,
       1,     1,     1,     1,     5,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     1,     1,
       0,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = JSEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == JSEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use JSerror or JSUNDEF. */
#define YYERRCODE JSUNDEF


/* Enable debugging if requested.  */
#if JSDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
                 int yyrule)
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
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !JSDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !JSDEBUG */


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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
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
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
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
# endif
#endif

#ifndef yytnamerr
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
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
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
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
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
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
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
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
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


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
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

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = JSEMPTY; /* Cause a token to be read.  */


/* User initialization code.  */
#line 35 "src/parser.y"
{
    jsParser.root = new ASTNode(jsParser, JSTOK_ROOT, ASTLocation(), "");
}

#line 1727 "src/bison.cpp"

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
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
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
  if (yychar == JSEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= JSEOF)
    {
      yychar = JSEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == JSerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = JSUNDEF;
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
  yychar = JSEMPTY;
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
  case 3: /* program: program statement  */
#line 149 "src/parser.y"
                           { yyval = yyvsp[-1]->adopt(yyvsp[0]); }
#line 1930 "src/bison.cpp"
    break;

  case 4: /* program: %empty  */
#line 150 "src/parser.y"
         { yyval = jsParser.root; }
#line 1936 "src/bison.cpp"
    break;

  case 11: /* statement: expr ";"  */
#line 158 "src/parser.y"
                          { D(yyvsp[0]); }
#line 1942 "src/bison.cpp"
    break;

  case 12: /* statement: "return" ";"  */
#line 159 "src/parser.y"
                          { D(yyvsp[0]); }
#line 1948 "src/bison.cpp"
    break;

  case 13: /* statement: "break" ";"  */
#line 160 "src/parser.y"
                          { D(yyvsp[0]); }
#line 1954 "src/bison.cpp"
    break;

  case 14: /* statement: "continue" ";"  */
#line 161 "src/parser.y"
                          { D(yyvsp[0]); }
#line 1960 "src/bison.cpp"
    break;

  case 15: /* statement: "return" expr ";"  */
#line 162 "src/parser.y"
                             { yyval = yyvsp[-2]->adopt(yyvsp[-1]); D(yyvsp[0]); }
#line 1966 "src/bison.cpp"
    break;

  case 16: /* statement: ";"  */
#line 163 "src/parser.y"
               { yyvsp[0]->symbol = JS_EMPTY; }
#line 1972 "src/bison.cpp"
    break;

  case 17: /* var_def: "let" ident "=" expr ";"  */
#line 165 "src/parser.y"
                                    { yyval = yyvsp[-4]->adopt({yyvsp[-3], yyvsp[-1]}); D(yyvsp[-2], yyvsp[0]); }
#line 1978 "src/bison.cpp"
    break;

  case 18: /* var_def: "const" ident "=" expr ";"  */
#line 166 "src/parser.y"
                                    { yyval = yyvsp[-4]->adopt({yyvsp[-3], yyvsp[-1]}); D(yyvsp[-2], yyvsp[0]); }
#line 1984 "src/bison.cpp"
    break;

  case 19: /* var_decl: "let" ident ";"  */
#line 168 "src/parser.y"
                            { yyval = yyvsp[-2]->adopt(yyvsp[-1]); D(yyvsp[0]); }
#line 1990 "src/bison.cpp"
    break;

  case 20: /* var_decl: "const" ident ";"  */
#line 169 "src/parser.y"
                            { yyval = yyvsp[-2]->adopt(yyvsp[-1]); D(yyvsp[0]); }
#line 1996 "src/bison.cpp"
    break;

  case 21: /* block: "{" statements "}"  */
#line 174 "src/parser.y"
                          { yyval = yyvsp[-1]; D(yyvsp[-2], yyvsp[0]); }
#line 2002 "src/bison.cpp"
    break;

  case 22: /* statements: statements statement  */
#line 176 "src/parser.y"
                                 { yyval = yyvsp[-1]->adopt(yyvsp[0]); }
#line 2008 "src/bison.cpp"
    break;

  case 23: /* statements: %empty  */
#line 177 "src/parser.y"
                                 { yyval = new ASTNode(jsParser, JS_BLOCK); }
#line 2014 "src/bison.cpp"
    break;

  case 24: /* conditional: "if" "(" expr ")" statement "else" statement  */
#line 179 "src/parser.y"
                                                          { yyval = yyvsp[-6]->adopt({yyvsp[-4], yyvsp[-2], yyvsp[0]}); D(yyvsp[-5], yyvsp[-3], yyvsp[-1]); }
#line 2020 "src/bison.cpp"
    break;

  case 25: /* conditional: "if" "(" expr ")" statement  */
#line 180 "src/parser.y"
                                                          { yyval = yyvsp[-4]->adopt({yyvsp[-2], yyvsp[0]});     D(yyvsp[-3], yyvsp[-1]);     }
#line 2026 "src/bison.cpp"
    break;

  case 26: /* while_loop: "while" "(" expr ")" statement  */
#line 182 "src/parser.y"
                                                    { yyval = yyvsp[-4]->adopt({yyvsp[-2], yyvsp[0]}); D(yyvsp[-3], yyvsp[-1]);     }
#line 2032 "src/bison.cpp"
    break;

  case 27: /* while_loop: "do" statement "while" "(" expr ")" ";"  */
#line 183 "src/parser.y"
                                                    { yyval = yyvsp[-6]->adopt({yyvsp[-2], yyvsp[-5]}); D(yyvsp[-3], yyvsp[-1], yyvsp[0]); }
#line 2038 "src/bison.cpp"
    break;

  case 28: /* for_loop: "for" "(" expr ";" expr_ ";" expr_ ")" statement  */
#line 185 "src/parser.y"
                                                           { yyval = yyvsp[-8]->adopt({yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]}); D(yyvsp[-7], yyvsp[-5], yyvsp[-3], yyvsp[-1]); }
#line 2044 "src/bison.cpp"
    break;

  case 30: /* expr_: %empty  */
#line 187 "src/parser.y"
              { yyval = new ASTNode(jsParser, JS_EMPTY); }
#line 2050 "src/bison.cpp"
    break;

  case 31: /* expr: expr "&&" expr  */
#line 189 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2056 "src/bison.cpp"
    break;

  case 32: /* expr: expr "||" expr  */
#line 190 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2062 "src/bison.cpp"
    break;

  case 33: /* expr: expr "&" expr  */
#line 191 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2068 "src/bison.cpp"
    break;

  case 34: /* expr: expr "|" expr  */
#line 192 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2074 "src/bison.cpp"
    break;

  case 35: /* expr: expr "^" expr  */
#line 193 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2080 "src/bison.cpp"
    break;

  case 36: /* expr: expr "===" expr  */
#line 194 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2086 "src/bison.cpp"
    break;

  case 37: /* expr: expr "!==" expr  */
#line 195 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2092 "src/bison.cpp"
    break;

  case 38: /* expr: expr "<<" expr  */
#line 196 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2098 "src/bison.cpp"
    break;

  case 39: /* expr: expr ">>" expr  */
#line 197 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2104 "src/bison.cpp"
    break;

  case 40: /* expr: expr "<" expr  */
#line 198 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2110 "src/bison.cpp"
    break;

  case 41: /* expr: expr ">" expr  */
#line 199 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2116 "src/bison.cpp"
    break;

  case 42: /* expr: expr "<=" expr  */
#line 200 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2122 "src/bison.cpp"
    break;

  case 43: /* expr: expr ">=" expr  */
#line 201 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2128 "src/bison.cpp"
    break;

  case 44: /* expr: expr "+" expr  */
#line 202 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2134 "src/bison.cpp"
    break;

  case 45: /* expr: expr "-" expr  */
#line 203 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2140 "src/bison.cpp"
    break;

  case 46: /* expr: expr "*" expr  */
#line 204 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2146 "src/bison.cpp"
    break;

  case 47: /* expr: expr "**" expr  */
#line 205 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2152 "src/bison.cpp"
    break;

  case 48: /* expr: expr "/" expr  */
#line 206 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2158 "src/bison.cpp"
    break;

  case 49: /* expr: expr "=" expr  */
#line 207 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2164 "src/bison.cpp"
    break;

  case 50: /* expr: expr "%" expr  */
#line 208 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2170 "src/bison.cpp"
    break;

  case 51: /* expr: expr "+=" expr  */
#line 209 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2176 "src/bison.cpp"
    break;

  case 52: /* expr: expr "-=" expr  */
#line 210 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2182 "src/bison.cpp"
    break;

  case 53: /* expr: expr "*=" expr  */
#line 211 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2188 "src/bison.cpp"
    break;

  case 54: /* expr: expr "**=" expr  */
#line 212 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2194 "src/bison.cpp"
    break;

  case 55: /* expr: expr "/=" expr  */
#line 213 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2200 "src/bison.cpp"
    break;

  case 56: /* expr: expr "%=" expr  */
#line 214 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2206 "src/bison.cpp"
    break;

  case 57: /* expr: expr "<<=" expr  */
#line 215 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2212 "src/bison.cpp"
    break;

  case 58: /* expr: expr ">>=" expr  */
#line 216 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2218 "src/bison.cpp"
    break;

  case 59: /* expr: expr ">>>=" expr  */
#line 217 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2224 "src/bison.cpp"
    break;

  case 60: /* expr: expr "&=" expr  */
#line 218 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2230 "src/bison.cpp"
    break;

  case 61: /* expr: expr "|=" expr  */
#line 219 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2236 "src/bison.cpp"
    break;

  case 62: /* expr: expr "||=" expr  */
#line 220 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2242 "src/bison.cpp"
    break;

  case 63: /* expr: expr "&&=" expr  */
#line 221 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2248 "src/bison.cpp"
    break;

  case 64: /* expr: expr "^=" expr  */
#line 222 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2254 "src/bison.cpp"
    break;

  case 65: /* expr: expr "in" expr  */
#line 223 "src/parser.y"
                       { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2260 "src/bison.cpp"
    break;

  case 66: /* expr: expr "instanceof" expr  */
#line 224 "src/parser.y"
                             { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]}); }
#line 2266 "src/bison.cpp"
    break;

  case 67: /* expr: expr "." JSTOK_IDENT  */
#line 225 "src/parser.y"
                                     { yyval = yyvsp[-1]->adopt({yyvsp[-2], yyvsp[0]});        }
#line 2272 "src/bison.cpp"
    break;

  case 68: /* expr: expr "[" expr "]"  */
#line 226 "src/parser.y"
                                     { yyval = yyvsp[-2]->adopt({yyvsp[-3], yyvsp[-1]}); D(yyvsp[0]); }
#line 2278 "src/bison.cpp"
    break;

  case 70: /* expr: "(" expr ")"  */
#line 228 "src/parser.y"
                             { yyval = yyvsp[-1]; D(yyvsp[-2], yyvsp[0]); }
#line 2284 "src/bison.cpp"
    break;

  case 71: /* expr: "!" expr  */
#line 229 "src/parser.y"
               { yyval = yyvsp[-1]->adopt(yyvsp[0]); }
#line 2290 "src/bison.cpp"
    break;

  case 72: /* expr: "~" expr  */
#line 230 "src/parser.y"
               { yyval = yyvsp[-1]->adopt(yyvsp[0]); }
#line 2296 "src/bison.cpp"
    break;

  case 73: /* expr: expr "?" expr ":" expr  */
#line 231 "src/parser.y"
                                       { yyval = yyvsp[-3]->adopt({yyvsp[-4], yyvsp[-2], yyvsp[0]}); D(yyvsp[-1]); }
#line 2302 "src/bison.cpp"
    break;

  case 74: /* expr: "+" number  */
#line 232 "src/parser.y"
                               { yyval = yyvsp[0]->adopt(yyvsp[-1]); }
#line 2308 "src/bison.cpp"
    break;

  case 75: /* expr: "-" number  */
#line 233 "src/parser.y"
                               { yyval = yyvsp[0]->adopt(yyvsp[-1]); }
#line 2314 "src/bison.cpp"
    break;

  case 76: /* expr: "++" expr  */
#line 234 "src/parser.y"
                               { yyval = yyvsp[-1]->adopt(yyvsp[0]); }
#line 2320 "src/bison.cpp"
    break;

  case 77: /* expr: expr "++"  */
#line 235 "src/parser.y"
                               { yyval = yyvsp[0]->adopt(yyvsp[-1]); yyval->symbol = JS_POSTPLUS; }
#line 2326 "src/bison.cpp"
    break;

  case 78: /* expr: "--" expr  */
#line 236 "src/parser.y"
                               { yyval = yyvsp[-1]->adopt(yyvsp[0]); }
#line 2332 "src/bison.cpp"
    break;

  case 79: /* expr: expr "--"  */
#line 237 "src/parser.y"
                               { yyval = yyvsp[0]->adopt(yyvsp[-1]); yyval->symbol = JS_POSTMINUS; }
#line 2338 "src/bison.cpp"
    break;

  case 84: /* expr: "new" expr "(" exprlist_ ")"  */
#line 242 "src/parser.y"
                                                  { yyval = yyvsp[-4]->adopt({yyvsp[-3], yyvsp[-1]}); D(yyvsp[-2], yyvsp[0]); }
#line 2344 "src/bison.cpp"
    break;

  case 85: /* expr: "new" expr  */
#line 243 "src/parser.y"
                                   { yyval = yyvsp[-1]->adopt(yyvsp[0]); }
#line 2350 "src/bison.cpp"
    break;

  case 86: /* expr: "delete" expr  */
#line 244 "src/parser.y"
                        { yyval = yyvsp[-1]->adopt(yyvsp[0]); }
#line 2356 "src/bison.cpp"
    break;

  case 87: /* expr: "void" expr  */
#line 245 "src/parser.y"
                        { yyval = yyvsp[-1]->adopt(yyvsp[0]); }
#line 2362 "src/bison.cpp"
    break;

  case 88: /* expr: "typeof" expr  */
#line 246 "src/parser.y"
                        { yyval = yyvsp[-1]->adopt(yyvsp[0]); }
#line 2368 "src/bison.cpp"
    break;

  case 96: /* function_call: expr "(" exprlist_ ")"  */
#line 256 "src/parser.y"
                                      { yyval = yyvsp[-2]->adopt({yyvsp[-3], yyvsp[-1]}); D(yyvsp[0]); }
#line 2374 "src/bison.cpp"
    break;

  case 97: /* exprlist: exprlist "," expr  */
#line 258 "src/parser.y"
                            { yyval = yyvsp[-2]->adopt(yyvsp[0]); D(yyvsp[-1]); }
#line 2380 "src/bison.cpp"
    break;

  case 98: /* exprlist: expr  */
#line 259 "src/parser.y"
                            { yyval = (new ASTNode(jsParser, JS_LIST))->locate(yyvsp[0])->adopt(yyvsp[0]); }
#line 2386 "src/bison.cpp"
    break;

  case 100: /* exprlist_: %empty  */
#line 262 "src/parser.y"
           { yyval = new ASTNode(jsParser, JS_LIST); }
#line 2392 "src/bison.cpp"
    break;


#line 2396 "src/bison.cpp"

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
  yytoken = yychar == JSEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= JSEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == JSEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = JSEMPTY;
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
  ++yynerrs;

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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
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
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != JSEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 273 "src/parser.y"


#pragma GCC diagnostic pop

const char * Parser::getName(int symbol) {
    return yytname[YYTRANSLATE(symbol)];
}
