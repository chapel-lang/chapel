/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0




/* Copy the first part of user declarations.  */

#line 67 "bison-chapel.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "bison-chapel.h".  */
#ifndef YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED
# define YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 33 "chapel.ypp" /* yacc.c:355  */

  extern int  captureTokens;
  extern char captureString[1024];
#line 44 "chapel.ypp" /* yacc.c:355  */

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);

  #endif
#line 63 "chapel.ypp" /* yacc.c:355  */

  #ifndef _BISON_CHAPEL_DEFINES_1_
  #define _BISON_CHAPEL_DEFINES_1_

  #include "symbol.h"

  #include <cstdio>

  class ArgSymbol;
  class BlockStmt;
  class CallExpr;
  class DefExpr;
  class EnumType;
  class Expr;
  class FnSymbol;
  class Type;

  enum   ProcIter {
    ProcIter_PROC,
    ProcIter_ITER
  };

  struct IntentExpr {
    Expr*  first;
    Expr*  second;
  };

  // The lexer only uses pch.
  // The remaining types are for parser productions
  union  YYSTYPE {
    const char*       pch;

    Vec<const char*>* vpch;
    RetTag            retTag;
    bool              b;
    IntentTag         pt;
    Expr*             pexpr;
    DefExpr*          pdefexpr;
    CallExpr*         pcallexpr;
    BlockStmt*        pblockstmt;
    Type*             ptype;
    EnumType*         penumtype;
    FnSymbol*         pfnsymbol;
    Flag              flag;
    ProcIter          procIter;
    FlagSet*          flagSet;
    IntentExpr        pIntentExpr;
  };

  #endif
#line 119 "chapel.ypp" /* yacc.c:355  */

  #ifndef _BISON_CHAPEL_DEFINES_2_
  #define _BISON_CHAPEL_DEFINES_2_

  struct YYLTYPE {
    int         first_line;
    int         first_column;
    int         last_line;
    int         last_column;
    const char* comment;
  };

  #define YYLTYPE_IS_DECLARED 1
  #define YYLTYPE_IS_TRIVIAL  1

  #endif
#line 141 "chapel.ypp" /* yacc.c:355  */

  #ifndef _BISON_CHAPEL_DEFINES_3_
  #define _BISON_CHAPEL_DEFINES_3_

  class ParserContext {
  public:
    ParserContext()
    {
      scanner       = 0;
      latestComment = 0;
      generatedStmt = 0;
    }

    ParserContext(yyscan_t scannerIn)
    {
      scanner       = scannerIn;
      latestComment = 0;
      generatedStmt = 0;
    }

    yyscan_t    scanner;
    const char* latestComment;
    BaseAST*    generatedStmt;
  };

  #endif

#line 211 "bison-chapel.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TIDENT = 258,
    INTLITERAL = 259,
    REALLITERAL = 260,
    IMAGLITERAL = 261,
    STRINGLITERAL = 262,
    CSTRINGLITERAL = 263,
    EXTERNCODE = 264,
    TALIGN = 265,
    TATOMIC = 266,
    TBEGIN = 267,
    TBREAK = 268,
    TBY = 269,
    TCLASS = 270,
    TCOBEGIN = 271,
    TCOFORALL = 272,
    TCONFIG = 273,
    TCONST = 274,
    TCONTINUE = 275,
    TDELETE = 276,
    TDMAPPED = 277,
    TDO = 278,
    TDOMAIN = 279,
    TELSE = 280,
    TENUM = 281,
    TEXPORT = 282,
    TEXTERN = 283,
    TFOR = 284,
    TFORALL = 285,
    TIF = 286,
    TIN = 287,
    TINDEX = 288,
    TINLINE = 289,
    TINOUT = 290,
    TITER = 291,
    TLABEL = 292,
    TLAMBDA = 293,
    TLET = 294,
    TLOCAL = 295,
    TMINUSMINUS = 296,
    TMODULE = 297,
    TNEW = 298,
    TNIL = 299,
    TNOINIT = 300,
    TON = 301,
    TOTHERWISE = 302,
    TOUT = 303,
    TPARAM = 304,
    TPLUSPLUS = 305,
    TPRAGMA = 306,
    TPRIMITIVE = 307,
    TPROC = 308,
    TRECORD = 309,
    TREDUCE = 310,
    TREF = 311,
    TRETURN = 312,
    TSCAN = 313,
    TSELECT = 314,
    TSERIAL = 315,
    TSINGLE = 316,
    TSPARSE = 317,
    TSUBDOMAIN = 318,
    TSYNC = 319,
    TTHEN = 320,
    TTYPE = 321,
    TUNDERSCORE = 322,
    TUNION = 323,
    TUSE = 324,
    TVAR = 325,
    TWHEN = 326,
    TWHERE = 327,
    TWHILE = 328,
    TWITH = 329,
    TYIELD = 330,
    TZIP = 331,
    TALIAS = 332,
    TAND = 333,
    TASSIGN = 334,
    TASSIGNBAND = 335,
    TASSIGNBOR = 336,
    TASSIGNBXOR = 337,
    TASSIGNDIVIDE = 338,
    TASSIGNEXP = 339,
    TASSIGNLAND = 340,
    TASSIGNLOR = 341,
    TASSIGNMINUS = 342,
    TASSIGNMOD = 343,
    TASSIGNMULTIPLY = 344,
    TASSIGNPLUS = 345,
    TASSIGNSL = 346,
    TASSIGNSR = 347,
    TBAND = 348,
    TBNOT = 349,
    TBOR = 350,
    TBXOR = 351,
    TCOLON = 352,
    TCOMMA = 353,
    TDIVIDE = 354,
    TDOT = 355,
    TDOTDOT = 356,
    TDOTDOTDOT = 357,
    TEQUAL = 358,
    TEXP = 359,
    TGREATER = 360,
    TGREATEREQUAL = 361,
    THASH = 362,
    TLESS = 363,
    TLESSEQUAL = 364,
    TMINUS = 365,
    TMOD = 366,
    TNOT = 367,
    TNOTEQUAL = 368,
    TOR = 369,
    TPLUS = 370,
    TQUESTION = 371,
    TSEMI = 372,
    TSHIFTLEFT = 373,
    TSHIFTRIGHT = 374,
    TSTAR = 375,
    TSWAP = 376,
    TIO = 377,
    TLCBR = 378,
    TRCBR = 379,
    TLP = 380,
    TRP = 381,
    TLSBR = 382,
    TRSBR = 383,
    TNOELSE = 384,
    TUPLUS = 385,
    TUMINUS = 386
  };
#endif

/* Value type.  */

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct yypstate yypstate;

int yypush_parse (yypstate *ps, int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc, ParserContext* context);

yypstate * yypstate_new (void);
void yypstate_delete (yypstate *ps);
/* "%code provides" blocks.  */
#line 172 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 388 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 394 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 38 "chapel.ypp" /* yacc.c:359  */

  int         captureTokens;
  char        captureString[1024];
#line 180 "chapel.ypp" /* yacc.c:359  */

  #include "build.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForLoop.h"
  #include "parser.h"
  #include "stmt.h"
  #include "stringutil.h"
  #include "vec.h"
  #include "WhileDoStmt.h"

  #include <cstdio>
  #include <cstdlib>
  #include <cstring>
  #include <stdint.h>

  static int query_uid = 1;

  #define YYLLOC_DEFAULT(Current, Rhs, N)                                 \
    if (N) {                                                              \
      (Current).first_line   = (Rhs)[1].first_line;                       \
      (Current).first_column = (Rhs)[1].first_column;                     \
      (Current).last_line    = (Rhs)[N].last_line;                        \
      (Current).last_column  = (Rhs)[N].last_column;                      \
      (Current).comment      = NULL;                                      \
                                                                          \
      if ((Current).first_line)                                           \
        yystartlineno = (Current).first_line;                             \
                                                                          \
    } else  {                                                             \
      (Current) = yylloc;                                                 \
    }

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str) {
    // TODO -- should this begin with error:
    if (!chplParseString) {
      const char* yyText = yyget_text(context->scanner);

      fprintf(stderr, "%s:%d: %s", yyfilename, chplLineno, str);

      if (strlen(yyText) > 0) {
        fprintf(stderr, ": near '%s'", yyText);
      }
    } else {
      fprintf(stderr, "%s: %s", yyfilename, str);

      if (chplParseStringMsg && (strlen(chplParseStringMsg) > 0)) {
        fprintf(stderr, " %s", chplParseStringMsg);
      }
    }

    fprintf(stderr, "\n");

    clean_exit(1);
  }

#line 460 "bison-chapel.cpp" /* yacc.c:359  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
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
#define YYLAST   10979

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  132
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  105
/* YYNRULES -- Number of rules.  */
#define YYNRULES  424
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  803

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   386

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
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
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   401,   401,   406,   407,   413,   414,   419,   420,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   454,   456,   461,   476,   477,
     482,   483,   488,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   510,   511,
     515,   519,   520,   524,   525,   529,   530,   531,   532,   533,
     534,   538,   553,   554,   555,   556,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,   575,
     581,   587,   593,   600,   610,   614,   615,   616,   617,   621,
     626,   627,   631,   633,   638,   645,   650,   651,   655,   660,
     665,   673,   674,   679,   680,   682,   687,   697,   706,   710,
     718,   719,   724,   729,   723,   754,   760,   767,   775,   783,
     791,   802,   808,   801,   836,   840,   845,   849,   857,   858,
     859,   860,   861,   862,   863,   864,   865,   866,   867,   868,
     869,   870,   871,   872,   873,   874,   875,   876,   877,   878,
     879,   880,   881,   882,   886,   887,   888,   889,   890,   891,
     892,   893,   894,   895,   896,   897,   901,   902,   906,   910,
     911,   912,   916,   918,   920,   922,   927,   928,   929,   930,
     931,   932,   933,   934,   935,   936,   940,   941,   942,   946,
     947,   951,   952,   953,   954,   955,   959,   960,   964,   965,
     969,   971,   976,   977,   981,   982,   986,   987,   991,   993,
     995,  1000,  1013,  1030,  1031,  1033,  1038,  1045,  1052,  1059,
    1067,  1076,  1077,  1078,  1082,  1083,  1091,  1093,  1099,  1104,
    1106,  1108,  1113,  1115,  1117,  1124,  1125,  1126,  1130,  1131,
    1136,  1137,  1138,  1139,  1159,  1163,  1167,  1175,  1179,  1180,
    1181,  1185,  1187,  1193,  1195,  1197,  1202,  1203,  1204,  1205,
    1206,  1207,  1208,  1214,  1215,  1216,  1217,  1221,  1222,  1223,
    1227,  1228,  1232,  1233,  1237,  1238,  1242,  1243,  1244,  1245,
    1246,  1250,  1261,  1262,  1263,  1264,  1265,  1266,  1268,  1270,
    1272,  1274,  1276,  1278,  1283,  1285,  1287,  1289,  1291,  1293,
    1295,  1297,  1299,  1301,  1303,  1305,  1307,  1314,  1320,  1326,
    1332,  1341,  1346,  1354,  1355,  1356,  1357,  1358,  1359,  1360,
    1361,  1366,  1367,  1371,  1375,  1377,  1385,  1395,  1399,  1400,
    1405,  1410,  1418,  1419,  1420,  1421,  1422,  1423,  1424,  1425,
    1426,  1428,  1430,  1432,  1434,  1436,  1441,  1442,  1443,  1444,
    1455,  1456,  1460,  1461,  1462,  1466,  1467,  1468,  1476,  1477,
    1478,  1482,  1483,  1484,  1485,  1486,  1487,  1488,  1489,  1496,
    1497,  1501,  1502,  1503,  1504,  1505,  1506,  1507,  1508,  1509,
    1510,  1511,  1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,
    1520,  1521,  1522,  1523,  1527,  1528,  1529,  1530,  1531,  1532,
    1536,  1537,  1538,  1539,  1543,  1544,  1545,  1546,  1551,  1552,
    1553,  1554,  1555,  1556,  1557
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "CSTRINGLITERAL", "EXTERNCODE", "TALIGN",
  "TATOMIC", "TBEGIN", "TBREAK", "TBY", "TCLASS", "TCOBEGIN", "TCOFORALL",
  "TCONFIG", "TCONST", "TCONTINUE", "TDELETE", "TDMAPPED", "TDO",
  "TDOMAIN", "TELSE", "TENUM", "TEXPORT", "TEXTERN", "TFOR", "TFORALL",
  "TIF", "TIN", "TINDEX", "TINLINE", "TINOUT", "TITER", "TLABEL",
  "TLAMBDA", "TLET", "TLOCAL", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL",
  "TNOINIT", "TON", "TOTHERWISE", "TOUT", "TPARAM", "TPLUSPLUS", "TPRAGMA",
  "TPRIMITIVE", "TPROC", "TRECORD", "TREDUCE", "TREF", "TRETURN", "TSCAN",
  "TSELECT", "TSERIAL", "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC",
  "TTHEN", "TTYPE", "TUNDERSCORE", "TUNION", "TUSE", "TVAR", "TWHEN",
  "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND",
  "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNSL", "TASSIGNSR", "TBAND",
  "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL",
  "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT",
  "TSTAR", "TSWAP", "TIO", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR",
  "TRSBR", "TNOELSE", "TUPLUS", "TUMINUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_stmt", "module", "block_stmt", "stmt_ls", "use_stmt",
  "assignment_stmt", "opt_ident", "ident", "do_stmt", "return_stmt",
  "class_level_stmt", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "select_stmt", "when_stmt_ls",
  "when_stmt", "type_select_stmt", "class_decl_stmt", "opt_extern",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@1", "$@2", "linkage_spec",
  "fn_decl_stmt", "$@3", "$@4", "fn_decl_stmt_inner", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls",
  "formal", "opt_intent_tag", "opt_this_intent_tag", "proc_or_iter",
  "opt_ret_tag", "opt_function_body_stmt", "function_body_stmt",
  "query_expr", "opt_query_expr", "var_arg_expr", "opt_where_part",
  "type_alias_decl_stmt", "type_alias_decl_stmt_inner", "opt_init_type",
  "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "opt_reindex_expr",
  "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "opt_formal_type", "expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "for_expr", "cond_expr", "nil_expr",
  "stmt_level_expr", "opt_task_intent_ls", "task_intent_clause",
  "task_intent_ls", "intent_expr", "io_expr", "new_expr", "let_expr",
  "expr", "lhs_expr", "fun_expr", "call_expr", "dot_expr",
  "parenthesized_expr", "literal_expr", "assoc_expr_ls", "binary_op_expr",
  "unary_op_expr", "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386
};
# endif

#define YYPACT_NINF -695

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-695)))

#define YYTABLE_NINF -378

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -695,    89,  2264,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  3279,    46,   122,    46,  6069,   110,   122,  6069,  3279,
      22,   122,   122,   667,  5110,  6069,  6069,    67,  -695,   122,
    -695,    20,  3279,  6069,  -695,  6069,  -695,  6069,  6069,   194,
     149,  5288,  6069,  5413,  6069,   212,   154,  3279,     9,  5491,
    6069,  6069,  -695,  -695,  6069,  -695,  -695,  1993,  6069,  6069,
    -695,  6069,  -695,  -695,  2529,  4910,  5491,  -695,  3154,  -695,
    -695,   122,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,   245,  -695,   262,   223,  -695,  -695,
    -695,   139,   186,  -695,  -695,  -695,   208,   218,   158,   228,
     239, 10676, 10852,   182,   242,   250,  -695,  -695,  -695,  -695,
    -695,  -695,   148,  -695, 10676,   259,  3279,  -695,   268,  -695,
     169,  6069,  6069,  6069,  6069,  6069,  5491,  5491,   283,  -695,
    -695,  -695,  -695,  8234,   284,  -695,  -695,   122,   276,  8432,
     326,  5491,   281,  -695,  -695,   122,  -695,   122,   280,    26,
    7244,  7178,  7376,  5491,  3279,   287,    18,   263,    28,  -695,
    -695,   605,   313,  7442,   605,  -695,  5491,  -695,  8498,  8102,
    3279,  -695,  -695,  7442, 10676,   305,  5491,  -695, 10676,  5491,
     343,   314,   122,  -695,   185, 10676,  7442,  8564,    80,  1258,
     605,    80,   605,  -695,  -695,  2654,    69,  -695,  6069,  -695,
      72,   163, 10676,    63,  8630,   -24,   425,  -695,   315,  -695,
    -695,  -695,   122,  -695,  -695,  -695,    31,    20,    20,    20,
    -695,  6069,  6069,  6069,  6069,  5591,  5591,  6069,  6069,  6069,
    6069,  6069,  6069,    66,  1993,  6069,  6069,  6069,  6069,  6069,
    6069,  6069,  6069,  6069,  6069,  6069,  6069,  6069,  6069,  6069,
    6069,  6069,  6069,  6069,  6069,  6069,  6069,  6069,  6069,  6069,
    6069,  6069,  6069,  6069,  6069,  5491,  5491,  5591,  5591,  1010,
    -695,  -695,  2779,  -695,  8300,  8366,  8696,    50,  5591,    26,
     324,  -695,  -695,  6069,   233,  -695,   311,   344,  -695, 10676,
     122,   328,   411,  5491,  -695,  3404,  5591,  -695,  3529,  5591,
    -695,    26,  3279,   426,   327,  -695,    32,  -695,  -695,    18,
    -695,   352,   329,  -695,  4654,   375,   377,  6069,    20,  -695,
     332,  -695,  -695,  -695,  -695,  5491,   333,   -32,  4732,   364,
    -695,  -695,  5491,  -695,  -695,  -695,  -695,  -695,  -695,  7112,
    5010,  -695,  5188,  -695,  5591,  2404,   337,  6069,  6069,  -695,
    -695,  2904,   370,   125,    20,   198,   206,   207,   240,  8168,
   10710, 10710,   303,  -695,   303,  -695,   303, 10776,   802,  1410,
     460,   313,    80,  -695,  -695,  -695,  1258, 10810,   303,  1088,
    1088, 10710,  1088,  1088,   413,    80, 10810, 10744,   413,   605,
     605,    80,  8762,  8828,  8894,  8960,  9026,  9092,  9158,  9224,
    9290,  9356,  9422,  9488,  9554,  9620,  9686,   350,   341,  -695,
     303,  -695,   303,    30,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,   122,   167,  -695,   430,  6147,  5591,  6225,  5591,  6069,
    5591,  6871,    46,  9752,  -695,  -695,  9818,  6069,  5491,  -695,
    5491,   398,    84,  -695,  -695,  6069,   168,  6069, 10676,    29,
    7508,  6069, 10676,     5,  7310,  -695,   456,  9884,  3279,  -695,
     171,  -695,    39,   297,   361,    18,   285,  -695,  5491,  -695,
     349,  6069,  5691,  -695, 10676,  -695,  -695,    -6,   365,  -695,
    -695,  5491,  -695,   274,   122,  -695, 10676,  -695,  -695,  -695,
    -695,   -29,  6848,  -695, 10676,  3279, 10676,  9950,  -695,  3029,
    5491,   367,  -695,  -695,  -695,  2101,   275,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  1010,  -695,   122,  6069,   469, 10016,  6069,   470, 10082,
     368,  6914,    26,    26,  -695, 10676,  -695, 10676,  -695,  6069,
     122,  -695,  7442,  -695,  7574,  3654,  -695,  3779,  -695,  7640,
    3904,  -695,    26,  4029,  -695,    26,  3279,  6069,  -695,   320,
    -695,    18,   400,  -695,  -695,  -695,  -695,    74,  -695,  -695,
    4732,   377,    68, 10676,  -695, 10676,  3279,  5491,  -695,  -695,
    -695,    12,    71,  -695,  4154,   371,  4279,   374,  -695,  6069,
    -695,   397,  -695,   297,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,   122,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
     403,   379,   379,  -695,  -695,  -695, 10148,  6303,  6381, 10214,
    6459,  6537,  6615,  6693,  -695,  -695, 10676,  -695,  -695,  3279,
    6069, 10676,  6069, 10676,  3279,  6069, 10676,  -695,  6069, 10676,
    -695,  -695, 10676,  -695,   385,  5769,    27,   433,  -695,  5591,
    4404,  -695,    21,  -695,  4529,  6069,  -695, 10676,  3279,  6069,
    -695, 10676,  3279, 10676,   787,    36,  -695,  2101,    32,  -695,
    -695,  6069,  6069,  6069,  6069,  6069,  6069,  6069,  6069,  9884,
    7706,  7772,  9884,  7838,  7904,   400,    22,  6069,  6069,  4810,
    -695,  -695,    81,  5491,  -695,  -695,  6069,     4,  6980,  -695,
     422,  -695,  7970,  -695,  8036,  -695,   136,   122,  -695,   922,
    -695,   433,  -695,   379,   379,   175, 10280, 10346, 10412, 10478,
   10544, 10610,  3279,  3279,  3279,  3279,    27,  5869,    77,  -695,
    -695, 10676, 10676,  -695,  -695,  -695,  6693,  3279,  3279,  -695,
       0,  -695,  -695,  -695,  6069,  6069,  6069,  6069,  6069,  6069,
    9884,  9884,  9884,  9884,  -695,  -695,  -695,  -695,  -695,   299,
    5591,  6771,   729,  9884,  9884,  -695,  -695,  -695,  7046,  -695,
    -695,  5969,  -695
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    34,    60,   371,   372,   373,   374,
     375,     0,   331,    58,   331,     0,   232,    58,     0,     0,
       0,     0,   127,   233,     0,     0,     0,     0,   126,     0,
     122,     0,     0,     0,    37,     0,   322,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   420,   422,     0,   423,   424,   355,     0,     0,
     421,   418,    65,   419,     0,     0,     0,     4,     0,     5,
       9,     0,    10,    11,    13,   291,    19,    12,    14,    16,
      15,    17,    18,    67,     0,    68,   347,     0,    66,    69,
      70,     0,   356,   343,   344,   294,   292,     0,     0,   348,
     349,     0,   293,     0,   357,   358,   359,   342,   296,   295,
     345,   346,     0,    21,   302,     0,     0,   332,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,   347,   356,
     292,   348,   349,   331,   293,   357,   358,     0,     0,     0,
       0,   282,     0,   128,    71,     0,   130,     0,     0,     0,
       0,     0,     0,   282,     0,     0,     0,     0,     0,   234,
      28,   406,   340,     0,   407,     7,   282,    63,     0,     0,
       0,    62,    31,     0,   297,     0,   282,    32,   303,     0,
     223,     0,   211,   274,     0,   273,     0,     0,   409,   354,
     405,   408,   404,    38,    40,     0,     0,   277,     0,   279,
       0,     0,   278,     0,   273,     0,     0,     6,     0,   108,
     109,   110,     0,   200,   199,   131,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   353,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   282,   282,     0,     0,   186,
      22,    23,     0,    24,     0,     0,     0,     0,     0,     0,
       0,    25,    26,     0,   291,   289,     0,   283,   284,   290,
       0,     0,     0,     0,    80,     0,     0,    79,     0,     0,
      86,     0,     0,    96,     0,    27,   179,   123,   239,     0,
     240,   242,     0,   253,     0,     0,   245,     0,     0,    29,
       0,    64,   100,    61,    30,   282,     0,     0,     0,   221,
     218,   210,     0,    42,    73,    33,    39,    41,   376,     0,
       0,   368,     0,   370,     0,     0,     0,     0,     0,   378,
       8,     0,   111,   196,     0,     0,     0,     0,     0,   330,
     401,   400,   403,   411,   410,   415,   414,   397,   394,   395,
     396,   351,   384,   367,   366,   365,   352,   388,   399,   393,
     391,   402,   392,   390,   382,   387,   389,   398,   381,   385,
     386,   383,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   413,
     412,   417,   416,   190,   187,   188,   189,   193,   194,   195,
     418,     0,     0,   334,     0,     0,     0,     0,     0,     0,
       0,   377,   331,   331,    76,   219,     0,     0,     0,   299,
       0,   120,     0,   117,   220,     0,     0,     0,   306,     0,
       0,     0,   312,     0,     0,    87,    95,     0,     0,   298,
       0,   180,     0,   201,     0,   243,     0,   257,     0,   252,
     343,     0,     0,   236,   341,   235,   364,     0,     0,   300,
     100,     0,   225,   343,     0,   276,   275,   350,   369,   280,
     281,     0,     0,    92,   316,     0,   379,     0,    35,     0,
       0,     0,   197,   198,   132,     0,     0,   227,   226,   228,
     230,    43,    50,    51,    52,    47,    49,    56,    57,    45,
      48,    46,    44,    54,    53,    55,   362,   363,   191,   192,
     336,   186,   333,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    72,   288,   286,   287,   285,     0,
     118,   116,     0,    94,     0,     0,    78,     0,    77,     0,
       0,    84,     0,     0,    82,     0,     0,     0,    98,   186,
     178,     0,   266,   202,   204,   203,   205,     0,   241,   244,
       0,   245,     0,   237,   246,   247,     0,     0,    99,   101,
     301,     0,     0,   222,     0,     0,     0,     0,    93,     0,
      36,   112,   113,   201,   161,   159,   164,   171,   172,   173,
     168,   170,   166,   169,   167,   165,   175,   174,   140,   143,
     141,   142,   153,   144,   157,   149,   147,   160,   148,   146,
     151,   156,   158,   145,   150,   154,   155,   152,   162,   163,
     138,   176,   176,   229,   335,   337,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    74,   121,   119,    81,     0,
       0,   305,     0,   304,     0,     0,   311,    85,     0,   310,
      83,    97,   321,   181,     0,     0,   245,   216,   238,     0,
       0,   103,     0,   104,     0,     0,    90,   318,     0,     0,
      88,   317,     0,   380,   231,     0,   139,     0,   179,   134,
     135,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,     0,   315,     0,     0,   266,   269,   270,   271,     0,
     268,   272,   343,   212,   183,   182,     0,     0,     0,   255,
     343,   102,     0,    91,     0,    89,   233,     0,   105,   231,
     114,   216,   138,   176,   176,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   245,   258,     0,   213,
     215,   214,   217,   208,   209,   124,     0,     0,     0,   115,
       0,   136,   137,   177,     0,     0,     0,     0,     0,     0,
     308,   307,   314,   313,   185,   184,   260,   261,   263,   343,
       0,   377,   343,   320,   319,   206,   133,   207,     0,   262,
     264,   258,   265
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -695,  -695,  -695,    -1,  -180,  1570,  -695,  -695,  1276,   165,
    -695,  -695,   502,  1667,   -79,  -669,  -598,  -695,  -695,    52,
    -695,  -695,    47,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,   -22,   330,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -168,  -163,  -622,  -695,  -162,   -34,  -261,  -695,  -695,
     -66,  -695,  -230,   457,  -695,  -215,  -199,  -695,  -134,  -695,
    -695,  -695,  -200,   225,  -695,  -303,  -571,  -695,  -437,  -326,
    -483,  -694,  -167,     7,    48,  -695,    -3,  -695,   107,   279,
    -236,  -695,  -695,   582,  -695,    -9,  -126,  -695,    19,  -695,
     728,   898,   -11,   980,  -695,  1213,  1296,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -260
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   194,    68,   456,    70,    71,    72,   195,
      73,    74,   118,    75,   172,    76,    77,    78,    79,   540,
      80,    81,   477,   589,    82,    83,    84,   212,   501,   694,
      85,   442,   443,   128,   155,   463,    87,    88,   353,   603,
     504,   641,   642,   699,   307,   460,   461,   462,   505,   215,
     577,   796,   765,   183,   760,   724,   727,    89,   181,   329,
      90,    91,   158,   159,   311,   312,   473,   315,   316,   469,
     787,   721,   676,   196,   200,   201,   286,   287,   288,   129,
      93,    94,    95,   130,    97,   116,   117,   422,   423,    98,
     131,   132,   101,   134,   103,   135,   136,   106,   107,   205,
     108,   109,   110,   111,   112
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     114,    67,   482,   280,   133,   120,   464,   139,   421,   424,
     678,   291,     5,   150,   151,   152,   355,   356,   357,   358,
     700,     5,   161,     5,   162,   301,   163,   164,   560,   581,
     168,   169,   173,   174,     5,  -186,   178,   313,   185,   186,
     187,   586,     5,   188,   170,   115,   189,   190,   191,   170,
     192,   413,   555,   185,   202,   204,   184,    41,   764,   586,
     317,    41,   528,   788,   414,   587,   332,   415,   179,     5,
     294,   297,   300,   203,   348,   313,   149,   346,   470,   115,
     416,   417,   430,   587,   319,   308,   529,   354,   418,     3,
     373,   480,   483,  -250,   324,   344,   740,   800,   419,   594,
     679,   764,   224,   679,   349,   725,   472,   334,  -250,   790,
     114,   274,   275,   276,   178,   185,   204,   795,   588,   332,
     115,   771,   772,   272,   279,     5,   318,   272,   272,   723,
     289,  -250,   374,   580,   277,   225,   683,   115,   226,     5,
     677,   769,   289,   309,   272,   156,  -250,   141,   332,   272,
     304,  -107,   272,  -250,   506,   289,   156,  -186,   216,  -250,
    -267,   332,   579,   320,   571,   289,   332,   332,   185,   332,
     340,   580,  -129,   326,   502,   332,   137,   231,   431,  -267,
     233,   503,   550,  -267,   236,   785,   327,   339,   217,  -129,
    -107,   345,   153,   338,   337,   218,   680,  -250,   341,   684,
     434,   165,   145,   267,  -107,   791,   268,  -267,   551,   219,
     359,   360,   361,   362,   364,   366,   367,   368,   369,   370,
     371,   372,   455,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   404,   405,   406,   289,   289,   410,   412,   741,   213,
     209,   342,   407,   408,   313,   531,   332,   433,   674,   569,
     421,   424,   436,   569,   166,   175,   214,   363,   365,   176,
     221,    92,   185,   332,   448,   450,   313,   452,   454,   343,
      92,   457,   272,   532,   553,  -250,   318,   570,    92,   210,
     446,   773,   333,  -324,   318,   318,   474,   265,   799,   266,
     437,    92,   438,   211,   289,   507,   573,  -250,   802,   409,
     411,   486,   478,   508,   509,  -323,    92,   562,   565,   202,
     432,   202,    86,   492,   494,   220,   496,   497,   318,   413,
    -248,    86,  -250,    92,   470,  -328,   574,    92,   449,    86,
     593,   453,   414,   575,   729,   415,  -329,   510,   729,  -326,
     314,  -250,    86,   576,  -250,   595,   597,  -325,   416,   417,
     556,   558,  -224,   318,   561,   564,   418,    86,  -259,  -327,
    -250,  -251,   580,  -250,   269,   271,   419,  -361,   489,  -361,
     490,  -224,   643,   281,    86,    92,   491,  -259,    86,   283,
     231,  -259,  -250,   233,   290,   293,  -251,   236,  -361,  -360,
    -361,  -360,   306,   233,   448,   536,   452,   539,   457,   541,
     494,  -251,   328,   542,   543,  -259,   545,   547,  -251,   289,
     325,   330,   350,    92,   552,   224,   554,   439,   351,   722,
     559,   435,   440,   445,   730,   444,    86,  -251,   730,    92,
     465,   458,   471,   459,  -254,   466,   472,   204,   476,   479,
     583,   585,   484,   654,   655,   495,  -251,   500,   225,   527,
     204,   226,  -251,   658,    92,   582,   526,   549,   535,  -254,
     538,   566,   224,   667,    86,   533,   670,   578,   592,   185,
     602,   590,   647,   650,  -254,   332,   652,   675,   337,   688,
      86,  -254,   692,   697,   698,   726,   228,   601,   229,   230,
     231,   715,   232,   233,   739,   225,   499,   236,   226,   138,
    -254,   789,   199,   646,   243,    86,   649,   591,   657,   743,
     792,   247,   248,   249,   744,   673,   745,   695,   656,  -254,
     797,   784,   770,   475,   661,  -254,   663,   548,   756,   666,
     644,    92,   669,   228,     0,   789,   672,   231,     0,   232,
     233,     0,     0,     0,   236,   789,     0,     0,     0,     0,
       0,   243,     0,     0,    92,     0,   185,    92,   247,   248,
     249,    92,     0,   687,    96,   691,     0,     0,   693,     0,
       0,     0,     0,    96,   682,     0,     0,     0,   285,     0,
       0,    96,    86,   731,     0,     0,     0,     0,     0,     0,
     285,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,   285,    92,    86,     0,   224,    86,    96,
      92,     0,    86,   285,     0,     0,   661,   663,     0,   666,
     669,   687,   691,     0,     0,     0,    96,     0,   709,   710,
      96,   711,     0,   712,   713,     0,     0,   714,     0,     0,
     225,     0,     0,   226,     0,     0,     0,     0,   728,   494,
       5,     0,     0,   494,   732,    86,   144,     0,   734,     0,
       0,    86,  -107,     0,     0,     0,     0,     0,     0,     0,
     709,   746,   747,   712,   748,   749,   750,   751,    96,     0,
     530,     0,   231,  -129,   232,   233,   174,   178,   204,   236,
       0,     0,   761,     0,     0,   762,   243,     0,     0,     0,
    -129,  -107,   285,   285,     0,   249,   758,     0,     0,     0,
      99,     0,     0,   145,     0,  -107,    96,    92,     0,    99,
       0,   780,   781,   782,   783,     0,     0,    99,     0,     0,
       0,     0,    96,     0,     0,   691,   793,   794,     0,     0,
      99,  -256,     0,   780,   781,   782,   783,   793,   794,     0,
       0,     0,     0,     0,    92,    99,     0,    96,    92,   798,
     494,     0,   285,     0,     0,     0,  -256,     0,    86,   485,
     691,     0,    99,     0,     0,     0,    99,   199,     0,   199,
       0,  -256,  -106,     0,     0,    16,     0,     0,  -256,     0,
       0,     0,   645,    21,    22,   736,     0,     0,     0,     0,
       0,    28,     0,  -125,   224,    86,     0,  -256,     0,    86,
       0,     0,     0,     0,    92,     0,    92,     0,    39,    92,
    -125,  -106,    92,     0,    99,    92,  -256,     0,     0,     0,
       0,     0,  -256,   737,    96,  -106,     0,   225,     0,     0,
     226,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,    92,     0,    92,     0,    96,     0,     0,
      96,     0,    99,     0,    96,    86,     0,    86,     0,     0,
      86,     0,     0,    86,     0,   546,    86,   285,    99,   231,
     100,   232,   233,     0,    62,     0,   236,     0,     0,   100,
       0,   738,     0,   243,     0,     0,    86,   100,     0,     0,
     247,   248,   249,    99,    86,     0,    86,    96,     0,     0,
     100,     0,     0,    96,     0,     0,     0,  -106,    92,     0,
      16,     0,     0,    92,     0,   100,     0,     0,    21,    22,
     736,     0,     0,     0,     0,     0,    28,     0,  -125,     0,
       0,     0,   100,     0,     0,     0,   100,    92,     0,     0,
       0,    92,     0,   206,     0,  -125,  -106,     0,     0,     0,
       0,     0,   102,     0,     0,     0,     0,     0,   737,    86,
    -106,   102,     0,     0,    86,     0,     0,     0,     0,   102,
      99,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   102,     0,   100,     0,     0,     0,    86,     0,
       0,     0,    86,    99,     0,     0,    99,   102,     0,   413,
      99,    92,    92,    92,    92,     0,     0,     0,     0,    62,
      96,     0,   414,     0,   102,   415,    92,    92,   102,     0,
       0,     0,   100,     0,     0,     0,     0,     0,   416,   417,
       0,     0,     0,     0,     0,     0,   418,     0,   100,     0,
       0,     0,     0,    99,     0,     0,   419,    96,     0,    99,
       0,    96,    86,    86,    86,    86,     0,     0,    52,     0,
       0,     0,     0,   100,     0,     0,   102,    86,    86,     0,
       0,     0,     0,    53,     0,    55,    56,     0,     0,     0,
     224,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    60,   420,     0,     0,     0,     0,
      63,     0,   720,     0,   102,     0,     0,    96,     0,    96,
       0,     0,    96,   225,     0,    96,   226,     0,    96,     0,
     102,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
     100,     0,     0,     0,     0,   102,    96,     0,    96,     0,
     759,   228,     0,   229,   230,   231,    99,   232,   233,   234,
       0,     0,   236,   100,     0,     0,   100,     0,   242,   243,
     100,     0,     0,   246,     0,     0,   247,   248,   249,     0,
       0,     0,     0,     0,   786,   104,     0,     0,     0,     0,
       0,     0,     0,    99,   104,     0,     0,    99,     0,     0,
       0,     0,   104,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,   100,     0,   104,    96,     0,   786,   100,
       0,     0,   102,     0,     0,     0,     0,     0,   786,     0,
     104,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,     0,    96,   102,     0,   104,   102,     0,
     224,   104,   102,    99,     0,    99,     0,     0,    99,     0,
       0,    99,     0,     0,    99,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,   105,     0,     0,
       0,     0,     0,   225,    99,   105,   226,     0,     0,   171,
       0,     0,    99,     0,    99,   102,     0,     0,   105,   104,
       0,   102,     0,     0,    96,    96,    96,    96,     0,     0,
       0,     0,     0,   105,     0,     0,     0,     0,     0,    96,
      96,   228,     0,   229,   230,   231,   100,   232,   233,     0,
     105,     0,   236,     0,   105,     0,     0,   104,   242,   243,
       0,     0,     0,   246,     0,     0,   247,   248,   249,     0,
       0,     0,     0,   104,     0,     0,     0,    99,     0,     0,
       0,     0,    99,   100,     0,     0,   273,   100,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,   105,     0,     0,     0,    99,     0,     0,     0,
      99,     0,     0,     0,     0,   171,   171,   171,   303,     0,
       0,     0,   224,     0,     0,     0,     0,     0,   102,   171,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
     105,     0,     0,   100,     0,   100,     0,     0,   100,     0,
       0,   100,   171,     0,   100,   225,   105,     0,   226,     0,
       0,     0,     0,     0,     0,   102,     0,     0,     0,   102,
      99,    99,    99,    99,   100,   104,     0,     0,     0,     0,
       0,   105,   100,     0,   100,    99,    99,     0,     0,     0,
       0,     0,     0,   228,     0,     0,   230,   231,   104,   232,
     233,   104,     0,     0,   236,   104,     0,     0,     0,     0,
       0,   243,     0,     0,     0,     0,     0,     0,   247,   248,
     249,     0,     0,     0,     0,   102,     0,   102,     0,     0,
     102,     0,     0,   102,     0,     0,   102,     0,     0,     0,
       0,     0,     0,     0,     0,   171,     0,   100,   104,     0,
       0,     0,   100,     0,   104,     0,   102,     0,   105,     0,
       0,     0,    69,     0,   102,     0,   102,   171,     0,     0,
       0,   113,     0,     0,     0,     0,   100,     0,     0,   140,
     100,   105,     0,     0,   105,     0,     0,     0,   105,     0,
       0,     0,   160,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,     0,     0,     0,   207,   102,
       0,   105,     0,     0,   102,     0,     0,   105,     0,     0,
     100,   100,   100,   100,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,   100,     0,   102,     0,
       0,   104,   102,     0,     0,     0,     0,     0,     0,     0,
     119,     0,     0,     0,   119,     0,   270,     0,   142,   143,
     146,     0,     0,     0,     0,     0,   154,     0,   157,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,   104,     0,     0,   180,     0,     0,     0,     0,
       0,     0,     0,     0,   305,   171,   171,     0,     0,   171,
     171,     0,   102,   102,   102,   102,     0,     0,   208,     0,
     323,     0,     0,     0,     0,     0,     0,   102,   102,     0,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,     0,     0,   104,     0,
     104,     0,     0,   104,     0,     0,   104,     0,     0,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,     0,     0,     0,   105,     0,     0,     0,   104,
       0,     0,     0,     0,   180,     0,     0,   104,   284,   104,
       0,     0,   180,     0,   292,     0,     0,     0,   171,   171,
     284,     0,     0,   310,     0,     0,     0,     0,   171,     0,
     303,     0,     0,   284,     0,   303,     0,     0,   171,     0,
       0,   171,    69,   284,     0,     0,     0,     0,     0,   331,
       0,   105,     0,   105,     0,     0,   105,     0,     0,   105,
       0,     0,   105,     0,     0,   323,     0,     0,   323,     0,
       0,     0,   104,     0,     0,     0,     0,   104,     0,   352,
       0,     0,   105,   157,   157,   157,   157,     0,     0,     0,
     105,     0,   105,     0,     0,     0,     0,     0,     0,     0,
     375,   104,     0,     0,     0,   104,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   284,   284,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   105,     0,   441,   171,     0,
     105,     0,     0,     0,     0,   104,   104,   104,   104,     0,
       0,     0,     0,     0,     0,     0,   310,     0,     0,     0,
     104,   104,     0,     0,   105,   157,   303,   303,   105,   303,
     303,     0,   284,     0,     0,     0,     5,     6,     7,     8,
       9,    10,     0,   763,   121,     0,     0,     0,   303,     0,
     303,     0,     0,     0,     0,     0,     0,    20,     0,     0,
       0,   157,   122,   123,   124,     0,    27,     0,   568,     0,
       0,    30,    31,     0,    33,     0,    35,    36,     0,     0,
       0,     0,     0,    38,     0,    40,   763,     0,   105,   105,
     105,   105,     0,     0,    44,    45,    46,   125,     0,     0,
       0,     0,     0,   105,   105,   598,     0,     0,     0,    69,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     5,    59,     0,   284,    61,     0,
       0,   604,     0,    63,     0,   605,   126,     0,    65,     0,
     127,     0,     0,     0,     0,   323,     0,   323,     0,   572,
     323,     0,   310,   323,     0,     0,   671,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,     0,     0,     0,     0,   681,     0,     0,     0,
       0,     0,     0,     0,   686,     0,   690,     0,     0,     0,
       0,     0,   640,     0,     0,     0,     0,     0,     0,     0,
     606,   607,   608,   609,   610,   611,     0,     0,   612,   613,
     614,   615,   616,   617,   618,   619,   620,   621,     0,     0,
     622,     0,     0,     0,   623,   624,   625,   626,   627,   628,
     629,   630,   631,   632,   633,     0,   634,   441,     0,   635,
     636,   637,   638,   639,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   310,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   733,     0,
       0,     0,   735,     0,    -2,     4,     0,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,  -106,
      14,    15,    16,  -231,    17,    18,   696,    19,    20,     0,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
    -125,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,     0,     0,  -231,    38,    39,    40,  -125,  -106,     0,
    -231,    41,     0,    42,    43,    44,    45,    46,    47,     0,
      48,     0,  -106,    49,  -231,     0,     0,    50,     0,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    55,
      56,     0,     0,     0,   742,    57,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    59,     0,    60,    61,
       0,    62,     0,     0,    63,     0,     0,    64,     0,    65,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   146,   180,     4,     0,     5,     6,     7,
       8,     9,    10,     0,  -377,    11,    12,    13,  -377,  -106,
      14,    15,    16,  -231,    17,    18,  -377,    19,    20,  -377,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
    -125,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,     0,     0,  -231,    38,     0,    40,  -125,  -106,  -377,
    -231,    41,  -377,    42,    43,    44,    45,    46,    47,     0,
      48,     0,  -106,    49,  -231,     0,     0,    50,     0,    51,
       0,     0,  -377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -377,    54,  -377,
    -377,  -377,  -377,  -377,  -377,  -377,     0,  -377,  -377,  -377,
    -377,  -377,  -377,  -377,  -377,  -377,    59,  -377,  -377,  -377,
       0,    62,  -377,  -377,  -377,     0,     0,    64,  -377,    65,
       4,    66,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,  -106,    14,    15,    16,  -231,    17,
      18,     0,    19,    20,     0,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,  -125,    29,    30,    31,    32,
      33,    34,    35,    36,     0,    37,     0,     0,  -231,    38,
      39,    40,  -125,  -106,     0,  -231,    41,     0,    42,    43,
      44,    45,    46,    47,     0,    48,     0,  -106,    49,  -231,
       0,     0,    50,     0,    51,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    59,     0,    60,    61,   182,    62,     0,     0,    63,
       0,     0,    64,   193,    65,     4,    66,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,  -106,
      14,    15,    16,  -231,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
    -125,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,     0,     0,  -231,    38,    39,    40,  -125,  -106,     0,
    -231,    41,     0,    42,    43,    44,    45,    46,    47,     0,
      48,     0,  -106,    49,  -231,     0,     0,    50,     0,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    59,     0,    60,    61,
       0,    62,     0,     0,    63,     0,     0,    64,   336,    65,
       4,    66,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,  -106,    14,    15,    16,  -231,    17,
      18,     0,    19,    20,     0,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,  -125,    29,    30,    31,    32,
      33,    34,    35,    36,     0,    37,     0,     0,  -231,    38,
      39,    40,  -125,  -106,     0,  -231,    41,     0,    42,    43,
      44,    45,    46,    47,     0,    48,     0,  -106,    49,  -231,
       0,     0,    50,     0,    51,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    59,     0,    60,    61,     0,    62,     0,     0,    63,
       0,     0,    64,   193,    65,     4,    66,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,  -106,
      14,    15,    16,  -231,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
    -125,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,     0,     0,  -231,    38,    39,    40,  -125,  -106,     0,
    -231,    41,     0,    42,    43,    44,    45,    46,    47,     0,
      48,     0,  -106,    49,  -231,     0,     0,    50,     0,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    59,     0,    60,    61,
       0,    62,     0,     0,    63,     0,     0,    64,   498,    65,
       4,    66,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,  -106,    14,    15,    16,  -231,    17,
      18,     0,    19,    20,     0,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,  -125,    29,    30,    31,    32,
      33,    34,    35,    36,     0,    37,     0,     0,  -231,    38,
      39,    40,  -125,  -106,     0,  -231,    41,     0,    42,    43,
      44,    45,    46,    47,     0,    48,     0,  -106,    49,  -231,
       0,     0,    50,     0,    51,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    59,     0,    60,    61,     0,    62,     0,     0,    63,
       0,     0,    64,   600,    65,     4,    66,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,  -106,
      14,    15,    16,  -231,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
    -125,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,     0,     0,  -231,    38,   206,    40,  -125,  -106,     0,
    -231,    41,     0,    42,    43,    44,    45,    46,    47,     0,
      48,     0,  -106,    49,  -231,     0,     0,    50,     0,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    59,     0,    60,    61,
       0,    62,     0,     0,    63,     0,     0,    64,     0,    65,
       4,    66,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,  -106,    14,    15,    16,  -231,    17,
      18,     0,    19,    20,     0,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,  -125,    29,    30,    31,    32,
      33,    34,    35,    36,     0,    37,     0,     0,  -231,    38,
       0,    40,  -125,  -106,     0,  -231,    41,     0,    42,    43,
      44,    45,    46,    47,     0,    48,     0,  -106,    49,  -231,
       0,     0,    50,     0,    51,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    59,     0,    60,    61,     0,    62,     0,     0,    63,
       0,     0,    64,     0,    65,     4,    66,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,  -106,
      14,    15,    16,  -231,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,   447,     0,    27,    28,     0,
    -125,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,     0,     0,  -231,    38,     0,    40,  -125,  -106,     0,
    -231,    41,     0,    42,    43,    44,    45,    46,    47,     0,
      48,     0,  -106,    49,  -231,     0,     0,    50,     0,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    59,     0,    60,    61,
       0,    62,     0,     0,    63,     0,     0,    64,     0,    65,
       4,    66,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,  -106,    14,    15,    16,  -231,    17,
      18,     0,    19,    20,     0,    21,    22,    23,    24,    25,
     451,     0,    27,    28,     0,  -125,    29,    30,    31,    32,
      33,    34,    35,    36,     0,    37,     0,     0,  -231,    38,
       0,    40,  -125,  -106,     0,  -231,    41,     0,    42,    43,
      44,    45,    46,    47,     0,    48,     0,  -106,    49,  -231,
       0,     0,    50,     0,    51,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    59,     0,    60,    61,     0,    62,     0,     0,    63,
       0,     0,    64,     0,    65,     4,    66,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,  -106,
      14,    15,    16,  -231,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,   660,     0,    27,    28,     0,
    -125,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,     0,     0,  -231,    38,     0,    40,  -125,  -106,     0,
    -231,    41,     0,    42,    43,    44,    45,    46,    47,     0,
      48,     0,  -106,    49,  -231,     0,     0,    50,     0,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    59,     0,    60,    61,
       0,    62,     0,     0,    63,     0,     0,    64,     0,    65,
       4,    66,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,  -106,    14,    15,    16,  -231,    17,
      18,     0,    19,    20,     0,    21,    22,    23,    24,    25,
     662,     0,    27,    28,     0,  -125,    29,    30,    31,    32,
      33,    34,    35,    36,     0,    37,     0,     0,  -231,    38,
       0,    40,  -125,  -106,     0,  -231,    41,     0,    42,    43,
      44,    45,    46,    47,     0,    48,     0,  -106,    49,  -231,
       0,     0,    50,     0,    51,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    59,     0,    60,    61,     0,    62,     0,     0,    63,
       0,     0,    64,     0,    65,     4,    66,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,  -106,
      14,    15,    16,  -231,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,   665,     0,    27,    28,     0,
    -125,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,     0,     0,  -231,    38,     0,    40,  -125,  -106,     0,
    -231,    41,     0,    42,    43,    44,    45,    46,    47,     0,
      48,     0,  -106,    49,  -231,     0,     0,    50,     0,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    59,     0,    60,    61,
       0,    62,     0,     0,    63,     0,     0,    64,     0,    65,
       4,    66,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,  -106,    14,    15,    16,  -231,    17,
      18,     0,    19,    20,     0,    21,    22,    23,    24,    25,
     668,     0,    27,    28,     0,  -125,    29,    30,    31,    32,
      33,    34,    35,    36,     0,    37,     0,     0,  -231,    38,
       0,    40,  -125,  -106,     0,  -231,    41,     0,    42,    43,
      44,    45,    46,    47,     0,    48,     0,  -106,    49,  -231,
       0,     0,    50,     0,    51,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    59,     0,    60,    61,     0,    62,     0,     0,    63,
       0,     0,    64,     0,    65,     4,    66,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,  -106,
      14,    15,    16,  -231,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,   685,     0,    27,    28,     0,
    -125,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,     0,     0,  -231,    38,     0,    40,  -125,  -106,     0,
    -231,    41,     0,    42,    43,    44,    45,    46,    47,     0,
      48,     0,  -106,    49,  -231,     0,     0,    50,     0,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    59,     0,    60,    61,
       0,    62,     0,     0,    63,     0,     0,    64,     0,    65,
       4,    66,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,  -106,    14,    15,    16,  -231,    17,
      18,     0,    19,    20,     0,    21,    22,    23,    24,    25,
     689,     0,    27,    28,     0,  -125,    29,    30,    31,    32,
      33,    34,    35,    36,     0,    37,     0,     0,  -231,    38,
       0,    40,  -125,  -106,     0,  -231,    41,     0,    42,    43,
      44,    45,    46,    47,     0,    48,     0,  -106,    49,  -231,
       0,     0,    50,     0,    51,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    59,     0,    60,    61,     0,    62,     0,     0,    63,
       0,     0,    64,     0,    65,   467,    66,     5,     6,     7,
       8,     9,    10,     0,  -377,   121,     0,     0,  -377,     0,
       0,     0,     0,     0,     0,     0,  -377,     0,    20,     0,
       0,     0,     0,   122,   123,   124,     0,    27,     0,     0,
       0,     0,    30,    31,     0,    33,     0,    35,    36,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,  -377,
       0,     0,  -377,     0,     0,    44,    45,    46,   125,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -249,  -377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -377,    54,  -377,
    -377,  -377,     0,  -377,  -377,  -377,     0,  -377,  -377,  -377,
    -377,  -377,  -377,  -377,  -377,  -377,    59,  -377,  -377,  -377,
       0,     0,  -377,  -377,  -377,     0,     0,   126,     0,    65,
     467,   481,     5,     6,     7,     8,     9,    10,     0,  -377,
     121,     0,     0,  -377,     0,     0,     0,     0,     0,     0,
       0,  -377,     0,    20,     0,     0,     0,     0,   122,   123,
     124,     0,    27,     0,     0,     0,     0,    30,    31,     0,
      33,     0,    35,    36,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,  -377,     0,     0,  -377,     0,     0,
      44,    45,    46,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -377,    54,  -377,  -377,  -377,     0,  -377,  -377,
    -377,     0,  -377,  -377,  -377,  -377,  -377,  -377,  -377,  -377,
    -377,    59,  -377,  -377,  -377,     0,     0,  -377,  -377,  -377,
       0,     0,   126,     0,    65,   467,   481,     5,     6,     7,
       8,     9,    10,     0,     0,   121,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    20,     0,
       0,     0,     0,   122,   123,   124,     0,    27,     0,     0,
       0,     0,    30,    31,     0,    33,     0,    35,    36,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,    44,    45,    46,   125,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,   467,     0,     5,     6,     7,     8,     9,
      10,     0,     0,   121,     0,     0,     0,    53,    54,    55,
      56,     0,     0,     0,     0,    57,    20,     0,     0,     0,
       0,   122,   123,   124,    58,    27,    59,     0,    60,    61,
      30,    31,     0,    33,    63,    35,    36,   126,     0,    65,
       0,   468,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,    44,    45,    46,   125,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     5,     6,     7,     8,     9,    10,     0,
       0,   121,     0,     0,     0,    53,    54,    55,    56,     0,
       0,     0,     0,    57,    20,     0,     0,     0,     0,   122,
     123,   124,    58,    27,    59,     0,    60,    61,    30,    31,
       0,    33,    63,    35,    36,   126,     0,    65,     0,   481,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,    44,    45,    46,   125,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,    54,    55,    56,     0,     0,     0,
       0,    57,     0,     5,     6,     7,     8,     9,    10,     0,
      58,   121,    59,     0,    60,    61,   182,     0,     0,     0,
      63,     0,     0,   126,    20,    65,     0,   127,   757,   122,
     123,   124,     0,    27,     0,     0,     0,     0,    30,    31,
       0,    33,     0,    35,    36,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,    44,    45,    46,   125,     0,     0,   197,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,    54,    55,    56,     0,     0,     0,
       0,    57,   198,     5,     6,     7,     8,     9,    10,     0,
      58,   121,    59,     0,    60,    61,   182,     0,     0,     0,
      63,     0,     0,   126,    20,    65,     0,   127,     0,   122,
     123,   124,     0,    27,     0,     0,     0,     0,    30,    31,
       0,    33,     0,    35,    36,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,    44,    45,    46,   125,     0,     0,   197,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,    54,    55,    56,     0,     0,     0,
       0,    57,     0,     5,     6,     7,     8,     9,    10,     0,
      58,   121,    59,     0,    60,    61,   182,     0,     0,     0,
      63,     0,     0,   126,    20,    65,   488,   127,     0,   122,
     123,   124,     0,    27,     0,     0,     0,     0,    30,    31,
       0,    33,     0,    35,    36,     0,     0,     0,     0,   147,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,    44,    45,    46,   125,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   148,     0,    52,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,   121,
       0,     0,     0,    53,    54,    55,    56,     0,     0,     0,
       0,    57,    20,     0,     0,     0,     0,   122,   123,   124,
      58,    27,    59,     0,    60,    61,    30,    31,     0,    33,
      63,    35,    36,   126,     0,    65,     0,   127,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,    44,
      45,    46,   125,     0,     0,   197,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,     0,     0,     0,     0,    57,
       0,     5,     6,     7,     8,     9,    10,     0,    58,   121,
      59,     0,    60,    61,   182,     0,     0,     0,    63,     0,
       0,   126,    20,    65,     0,   127,     0,   122,   123,   124,
       0,    27,     0,     0,     0,     0,    30,    31,     0,    33,
       0,    35,    36,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,    44,
      45,    46,   125,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      59,     0,    60,    61,     0,   167,     0,     0,    63,     0,
       0,   126,     0,    65,     0,   127,     5,     6,     7,     8,
       9,    10,     0,     0,   121,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   170,    20,     0,     0,
       0,     0,   122,   123,   124,     0,    27,     0,     0,     0,
       0,    30,    31,     0,    33,     0,    35,    36,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,    44,    45,    46,   125,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     5,     6,     7,     8,     9,    10,
       0,     0,   121,     0,     0,     0,    53,    54,    55,    56,
       0,     0,     0,     0,    57,    20,     0,     0,     0,     0,
     122,   123,   124,    58,    27,    59,     0,    60,    61,    30,
      31,     0,    33,    63,    35,    36,    64,     0,    65,     0,
     127,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,    44,    45,    46,   125,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,    54,    55,    56,     0,     0,
       0,     0,    57,     0,     5,     6,     7,     8,     9,    10,
       0,    58,   121,    59,     0,    60,    61,   182,     0,     0,
       0,    63,     0,     0,   126,    20,    65,     0,   127,     0,
     122,   123,   124,     0,    27,     0,     0,     0,     0,    30,
      31,     0,    33,     0,    35,    36,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,    44,    45,    46,   125,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   148,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,    54,    55,    56,     0,     0,
       0,     0,    57,     0,     5,     6,     7,     8,     9,    10,
       0,    58,   121,    59,     0,    60,    61,     0,     0,     0,
       0,    63,     0,     0,   126,    20,    65,     0,   127,     0,
     122,   123,   124,     0,    27,     0,     0,     0,     0,    30,
      31,     0,    33,     0,    35,    36,   584,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,    44,    45,    46,   125,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     5,     6,     7,     8,     9,    10,     0,     0,
     121,     0,     0,     0,    53,    54,    55,    56,     0,     0,
       0,     0,    57,   716,     0,     0,     0,     0,   122,   123,
     124,    58,    27,    59,     0,    60,    61,    30,    31,     0,
      33,    63,    35,    36,   126,     0,    65,     0,   127,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
     717,    45,    46,   718,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,     0,     5,     6,     7,     8,     9,    10,     0,    58,
     121,    59,     0,    60,    61,   182,     0,     0,     0,    63,
       0,     0,   126,    20,    65,     0,   719,     0,   122,   123,
     124,     0,    27,     0,     0,     0,     0,    30,    31,     0,
      33,     0,    35,    36,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
      44,    45,    46,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,     0,     5,     6,     7,     8,     9,    10,     0,    58,
     121,    59,     0,    60,    61,   182,     0,     0,     0,    63,
       0,     0,   126,    20,    65,     0,   719,     0,   122,   123,
     708,     0,    27,     0,     0,     0,     0,    30,    31,     0,
      33,     0,    35,    36,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
      44,    45,    46,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,     0,     5,     6,     7,     8,     9,    10,     0,    58,
     121,    59,     0,    60,    61,   182,     0,     0,     0,    63,
       0,     0,   126,    20,    65,     0,   127,     0,   122,   123,
     124,     0,    27,     0,     0,     0,     0,    30,    31,     0,
      33,     0,    35,    36,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
      44,    45,    46,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,   121,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,    20,     0,     0,     0,     0,   122,   123,   534,    58,
      27,    59,     0,    60,    61,    30,    31,     0,    33,    63,
      35,    36,   126,     0,    65,     0,   127,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,    44,    45,
      46,   125,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,   121,     0,     0,     0,
      53,    54,    55,    56,     0,     0,     0,     0,    57,    20,
       0,     0,     0,     0,   122,   123,   537,    58,    27,    59,
       0,    60,    61,    30,    31,     0,    33,    63,    35,    36,
     126,     0,    65,     0,   127,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,    44,    45,    46,   125,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     5,     6,     7,     8,
       9,    10,     0,     0,   121,     0,     0,     0,    53,    54,
      55,    56,     0,     0,     0,     0,    57,    20,     0,     0,
       0,     0,   122,   123,   702,    58,    27,    59,     0,    60,
      61,    30,    31,     0,    33,    63,    35,    36,   126,     0,
      65,     0,   127,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,    44,    45,    46,   125,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     5,     6,     7,     8,     9,    10,
       0,     0,   121,     0,     0,     0,    53,    54,    55,    56,
       0,     0,     0,     0,    57,    20,     0,     0,     0,     0,
     122,   123,   703,    58,    27,    59,     0,    60,    61,    30,
      31,     0,    33,    63,    35,    36,   126,     0,    65,     0,
     127,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,    44,    45,    46,   125,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     5,     6,     7,     8,     9,    10,     0,     0,
     121,     0,     0,     0,    53,    54,    55,    56,     0,     0,
       0,     0,    57,    20,     0,     0,     0,     0,   122,   123,
     705,    58,    27,    59,     0,    60,    61,    30,    31,     0,
      33,    63,    35,    36,   126,     0,    65,     0,   127,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
      44,    45,    46,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,   121,     0,
       0,     0,    53,    54,    55,    56,     0,     0,     0,     0,
      57,    20,     0,     0,     0,     0,   122,   123,   706,    58,
      27,    59,     0,    60,    61,    30,    31,     0,    33,    63,
      35,    36,   126,     0,    65,     0,   127,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,    44,    45,
      46,   125,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,   121,     0,     0,     0,
      53,    54,    55,    56,     0,     0,     0,     0,    57,    20,
       0,     0,     0,     0,   122,   123,   707,    58,    27,    59,
       0,    60,    61,    30,    31,     0,    33,    63,    35,    36,
     126,     0,    65,     0,   127,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,    44,    45,    46,   125,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     5,     6,     7,     8,
       9,    10,     0,     0,   121,     0,     0,     0,    53,    54,
      55,    56,     0,     0,     0,     0,    57,    20,     0,     0,
       0,     0,   122,   123,   708,    58,    27,    59,     0,    60,
      61,    30,    31,     0,    33,    63,    35,    36,   126,     0,
      65,     0,   127,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,    44,    45,    46,   125,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     5,     6,     7,     8,     9,    10,
       0,     0,   121,     0,     0,     0,    53,    54,    55,    56,
       0,     0,     0,     0,    57,    20,     0,     0,     0,     0,
     122,   123,   124,    58,    27,    59,     0,    60,    61,    30,
      31,     0,    33,    63,    35,    36,   126,     0,    65,     0,
     127,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,    44,    45,    46,   125,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -258,     0,     0,     0,     0,     0,     0,     0,   222,     0,
       0,     0,   223,     0,     0,    54,     0,     0,     0,  -258,
     224,     0,     0,  -258,     5,     6,     7,     8,     9,    10,
       0,     0,   121,    59,     0,     0,     0,   182,     0,     0,
       0,     0,     0,     0,   126,    20,    65,  -258,   719,     0,
     122,   123,   124,   225,    27,     0,   226,     0,     0,    30,
      31,     0,    33,     0,    35,    36,     0,     0,     0,     0,
       0,    38,   115,    40,   222,     0,   227,     0,   223,     0,
       0,     0,    44,    45,    46,   125,   224,     0,     0,     0,
       0,   228,     0,   229,   230,   231,     0,   232,   233,   234,
       0,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   246,     0,    54,   247,   248,   249,   225,
       0,     0,   226,     0,     0,     0,   596,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
     222,     0,   227,     0,   223,     0,    65,     0,   127,     0,
       0,     0,   224,     0,     0,     0,     0,   228,     0,   229,
     230,   231,     0,   232,   233,   234,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,   246,
       0,     0,   247,   248,   249,   225,     0,     0,   226,     0,
       0,     0,   653,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,     0,   227,     0,
     223,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,   246,     0,     0,   247,   248,
     249,   225,     0,     0,   226,     0,     0,     0,   766,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,     0,   227,     0,   223,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,     0,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,   246,     0,     0,   247,   248,   249,   225,     0,     0,
     226,     0,     0,     0,   801,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,     0,
     227,     0,   223,     0,     0,     0,     0,     0,     0,     0,
     224,   298,     0,     0,     0,   228,     0,   229,   230,   231,
     299,   232,   233,   234,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,   246,     0,     0,
     247,   248,   249,   225,     0,     0,   226,     0,   487,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   115,     0,   222,     0,   227,     0,   223,     0,
       0,     0,     0,     0,     0,     0,   224,   295,     0,     0,
       0,   228,     0,   229,   230,   231,   296,   232,   233,   234,
       0,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   246,     0,     0,   247,   248,   249,   225,
       0,   272,   226,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,   227,     0,   223,     0,     0,     0,     0,     0,
       0,     0,   224,   563,     0,     0,     0,   228,     0,   229,
     230,   231,     0,   232,   233,   234,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,   246,
       0,     0,   247,   248,   249,   225,     0,   272,   226,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   115,     0,   222,     0,   227,     0,
     223,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,   246,     0,     0,   247,   248,
     249,   225,     0,   272,   226,     0,     0,     0,     0,     0,
       0,   302,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,     0,   227,     0,   223,     0,     0,     0,
       0,     0,     0,     0,   224,   170,     0,     0,     0,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,   246,     0,     0,   247,   248,   249,   225,     0,   272,
     226,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,     0,
     227,     0,   223,     0,     0,     0,     0,     0,     0,     0,
     224,   557,     0,     0,     0,   228,     0,   229,   230,   231,
       0,   232,   233,   234,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,   246,     0,     0,
     247,   248,   249,   225,     0,   272,   226,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,   227,     0,   223,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,   228,     0,   229,   230,   231,     0,   232,   233,   234,
       0,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   246,     0,     0,   247,   248,   249,   225,
       0,   272,   226,     0,     0,     0,     0,     0,     0,   659,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,   227,     0,   223,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,     0,     0,   228,     0,   229,
     230,   231,     0,   232,   233,   234,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,   246,
       0,     0,   247,   248,   249,   225,     0,   272,   226,     0,
       0,     0,     0,     0,     0,   664,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,     0,   227,     0,
     223,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,   246,     0,     0,   247,   248,
     249,   225,     0,   272,   226,     0,     0,     0,     0,     0,
       0,   752,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,     0,   227,     0,   223,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,     0,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,   246,     0,     0,   247,   248,   249,   225,     0,   272,
     226,     0,     0,     0,     0,     0,     0,   753,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,     0,
     227,     0,   223,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,     0,   228,     0,   229,   230,   231,
       0,   232,   233,   234,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,   246,     0,     0,
     247,   248,   249,   225,     0,   272,   226,     0,     0,     0,
       0,     0,     0,   754,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,   227,     0,   223,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,   228,     0,   229,   230,   231,     0,   232,   233,   234,
       0,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   246,     0,     0,   247,   248,   249,   225,
       0,   272,   226,     0,     0,     0,     0,     0,     0,   755,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,   227,     0,   223,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,     0,     0,   228,     0,   229,
     230,   231,     0,   232,   233,   234,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,   246,
       0,     0,   247,   248,   249,   225,     0,   272,   226,     0,
       0,     0,     0,     0,     0,   767,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,     0,   227,     0,
     223,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,   246,     0,     0,   247,   248,
     249,   225,     0,   272,   226,     0,     0,     0,     0,     0,
       0,   768,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,     0,   227,     0,   223,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,     0,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,   246,     0,     0,   247,   248,   249,   225,     0,   272,
     226,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,     0,
     227,     0,   223,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,     0,   228,     0,   229,   230,   231,
       0,   232,   233,   234,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,   246,     0,     0,
     247,   248,   249,   225,     0,   322,   226,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,   227,     0,   223,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,   228,     0,   229,   230,   231,   278,   232,   233,   234,
       0,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   246,     0,     0,   247,   248,   249,   225,
    -339,     0,   226,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,     0,
     222,     0,   227,     0,   223,     0,     0,     0,     0,     0,
       0,     0,   224,   425,     0,     0,     0,   228,     0,   229,
     230,   231,   426,   232,   233,   234,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,   246,
       0,     0,   247,   248,   249,   225,     0,     0,   226,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,     0,   227,     0,
     223,     0,     0,     0,     0,     0,     0,     0,   224,   427,
       0,     0,     0,   228,     0,   229,   230,   231,   428,   232,
     233,   234,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,   246,     0,     0,   247,   248,
     249,   225,     0,     0,   226,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,     0,   227,     0,   223,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,     0,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,   246,     0,     0,   247,   248,   249,   225,     0,     0,
     226,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,     0,
     227,     0,   223,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,     0,   228,     0,   229,   230,   231,
       0,   232,   233,   234,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,   246,     0,   282,
     247,   248,   249,   225,     0,     0,   226,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,   227,     0,   223,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,   228,     0,   229,   230,   231,     0,   232,   233,   234,
       0,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   246,     0,   321,   247,   248,   249,   225,
       0,     0,   226,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,   227,     0,   223,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,     0,     0,   228,     0,   229,
     230,   231,     0,   232,   233,   234,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,   246,
       0,   335,   247,   248,   249,   225,     0,     0,   226,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,   347,   227,     0,
     223,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,   246,     0,     0,   247,   248,
     249,   225,     0,     0,   226,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,     0,   227,     0,   223,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,     0,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,   246,     0,     0,   247,   248,   249,   225,     0,     0,
     226,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,     0,
     227,     0,   223,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,     0,   228,     0,   229,   230,   231,
       0,   232,   233,   234,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,   246,     0,   511,
     247,   248,   249,   225,     0,     0,   226,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,   227,     0,   223,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,   228,     0,   229,   230,   231,     0,   232,   233,   234,
       0,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   246,     0,   512,   247,   248,   249,   225,
       0,     0,   226,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,   227,     0,   223,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,     0,     0,   228,     0,   229,
     230,   231,     0,   232,   233,   234,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,   246,
       0,   513,   247,   248,   249,   225,     0,     0,   226,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,     0,   227,     0,
     223,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,   246,     0,   514,   247,   248,
     249,   225,     0,     0,   226,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,     0,   227,     0,   223,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,     0,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,   246,     0,   515,   247,   248,   249,   225,     0,     0,
     226,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,     0,
     227,     0,   223,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,     0,   228,     0,   229,   230,   231,
       0,   232,   233,   234,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,   246,     0,   516,
     247,   248,   249,   225,     0,     0,   226,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,   227,     0,   223,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,   228,     0,   229,   230,   231,     0,   232,   233,   234,
       0,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   246,     0,   517,   247,   248,   249,   225,
       0,     0,   226,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,   227,     0,   223,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,     0,     0,   228,     0,   229,
     230,   231,     0,   232,   233,   234,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,   246,
       0,   518,   247,   248,   249,   225,     0,     0,   226,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,     0,   227,     0,
     223,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,   246,     0,   519,   247,   248,
     249,   225,     0,     0,   226,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,     0,   227,     0,   223,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,     0,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,   246,     0,   520,   247,   248,   249,   225,     0,     0,
     226,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,     0,
     227,     0,   223,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,     0,   228,     0,   229,   230,   231,
       0,   232,   233,   234,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,   246,     0,   521,
     247,   248,   249,   225,     0,     0,   226,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,   227,     0,   223,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,   228,     0,   229,   230,   231,     0,   232,   233,   234,
       0,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   246,     0,   522,   247,   248,   249,   225,
       0,     0,   226,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,   227,     0,   223,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,     0,     0,   228,     0,   229,
     230,   231,     0,   232,   233,   234,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,   246,
       0,   523,   247,   248,   249,   225,     0,     0,   226,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,     0,   227,     0,
     223,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,   246,     0,   524,   247,   248,
     249,   225,     0,     0,   226,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,     0,   227,     0,   223,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,     0,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,   246,     0,   525,   247,   248,   249,   225,     0,     0,
     226,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   115,     0,   222,     0,
     227,     0,   223,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,     0,   228,     0,   229,   230,   231,
       0,   232,   233,   234,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,   246,     0,     0,
     247,   248,   249,   225,     0,     0,   226,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,   227,     0,   223,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,   567,
       0,   228,     0,   229,   230,   231,     0,   232,   233,   234,
       0,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   246,     0,   544,   247,   248,   249,   225,
       0,     0,   226,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,   227,     0,   223,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,     0,     0,   228,     0,   229,
     230,   231,     0,   232,   233,   234,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,   246,
       0,     0,   247,   248,   249,   225,     0,     0,   226,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,   599,   227,     0,
     223,     0,     0,     0,     0,     0,     0,     0,   224,   648,
       0,     0,     0,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,   246,     0,     0,   247,   248,
     249,   225,     0,     0,   226,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,     0,   227,     0,   223,     0,     0,     0,
       0,     0,     0,     0,   224,   651,     0,     0,     0,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,   246,     0,     0,   247,   248,   249,   225,     0,     0,
     226,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,     0,
     227,     0,   223,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,     0,   228,     0,   229,   230,   231,
       0,   232,   233,   234,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,   246,     0,     0,
     247,   248,   249,   225,     0,     0,   226,     0,     0,     0,
       0,     0,     0,   701,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,   227,     0,   223,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,   228,     0,   229,   230,   231,     0,   232,   233,   234,
       0,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   246,     0,     0,   247,   248,   249,   225,
       0,     0,   226,     0,     0,     0,     0,     0,     0,   704,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,   227,     0,   223,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,     0,     0,   228,     0,   229,
     230,   231,     0,   232,   233,   234,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,   246,
       0,     0,   247,   248,   249,   225,     0,     0,   226,     0,
       0,     0,     0,     0,     0,   774,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,     0,   227,     0,
     223,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,   246,     0,     0,   247,   248,
     249,   225,     0,     0,   226,     0,     0,     0,     0,     0,
       0,   775,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,     0,   227,     0,   223,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,     0,     0,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,   246,     0,     0,   247,   248,   249,   225,     0,     0,
     226,     0,     0,     0,     0,     0,     0,   776,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,     0,
     227,     0,   223,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,     0,   228,     0,   229,   230,   231,
       0,   232,   233,   234,     0,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,   246,     0,     0,
     247,   248,   249,   225,     0,     0,   226,     0,     0,     0,
       0,     0,     0,   777,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,   227,     0,   223,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,   228,     0,   229,   230,   231,     0,   232,   233,   234,
       0,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   246,     0,     0,   247,   248,   249,   225,
       0,     0,   226,     0,     0,     0,     0,     0,     0,   778,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,     0,   227,     0,   223,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,     0,     0,   228,     0,   229,
     230,   231,     0,   232,   233,   234,     0,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,   246,
       0,     0,   247,   248,   249,   225,     0,     0,   226,     0,
       0,     0,     0,     0,     0,   779,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,     0,   227,     0,
     223,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,   246,     0,     0,   247,   248,
     249,   225,   224,     0,   226,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   227,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   225,   224,     0,   226,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   227,   244,
     245,   246,     0,     0,   247,   248,   249,     0,   224,   225,
       0,     0,   226,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,   235,   236,   237,   238,     0,   240,   241,
     242,   243,   227,   244,   245,   246,     0,     0,   247,   248,
     249,   225,   224,     0,   226,     0,     0,   228,     0,   229,
     230,   231,     0,   232,   233,   234,     0,   235,   236,   237,
     238,     0,   240,   241,   242,   243,     0,   244,     0,   246,
       0,     0,   247,   248,   249,   225,     0,     0,   226,   228,
       0,   229,   230,   231,     0,   232,   233,   234,     0,   235,
     236,   237,   238,     0,   240,   241,   242,   243,     0,   244,
       0,   246,     0,     0,   247,   248,   249,     0,     0,     0,
       0,     0,     0,   228,     0,   229,   230,   231,     0,   232,
     233,   234,     0,     0,   236,   237,   238,     0,   240,   241,
     242,   243,     0,     0,     0,   246,     0,     0,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,  -338,     0,     0,  -360,     0,  -360
};

static const yytype_int16 yycheck[] =
{
      11,     2,   328,   137,    15,    14,   309,    18,   269,   269,
     581,   145,     3,    24,    25,    26,   216,   217,   218,   219,
     642,     3,    33,     3,    35,   151,    37,    38,    23,   466,
      41,    42,    43,    44,     3,     3,    47,     1,    49,    50,
      51,    47,     3,    54,    23,    74,    57,    58,    59,    23,
      61,    19,    23,    64,    65,    66,    49,    57,   727,    47,
      32,    57,    32,   757,    32,    71,    98,    35,    59,     3,
     149,   150,   151,    66,    98,     1,    24,   203,   314,    74,
      48,    49,    32,    71,   163,    67,    56,    56,    56,     0,
      24,   123,   328,    57,   173,    32,   694,   791,    66,   128,
      32,   770,    22,    32,   128,   676,    79,   186,    72,    32,
     121,   122,   123,   124,   125,   126,   127,   117,   124,    98,
      74,   743,   744,   123,   133,     3,    98,   123,   123,   102,
     141,    57,    66,    97,   127,    55,   124,    74,    58,     3,
     577,   739,   153,   125,   123,   125,    72,   125,    98,   123,
     153,    15,   123,   117,   354,   166,   125,   125,    19,   123,
      79,    98,   465,   166,   125,   176,    98,    98,   179,    98,
      98,    97,    36,   176,    49,    98,    66,    97,   128,    98,
     100,    56,    98,   102,   104,   756,   179,   198,    49,    53,
      54,   128,   125,   124,   195,    56,   128,   123,   126,   128,
     279,     7,    66,    55,    68,   128,    58,   126,   124,    70,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   301,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   695,    36,
      15,    98,   265,   266,     1,    98,    98,   278,   571,    98,
     531,   531,   283,    98,   125,    63,    53,   225,   226,   125,
     122,     2,   293,    98,   295,   296,     1,   298,   299,   126,
      11,   302,   123,   126,   126,    32,    98,   126,    19,    54,
     293,   126,   117,   117,    98,    98,   317,   125,   791,   127,
      77,    32,    79,    68,   325,   117,    19,    32,   801,   267,
     268,   332,   325,   117,   117,   117,    47,   453,   454,   340,
     278,   342,     2,   344,   345,   117,   347,   348,    98,    19,
      77,    11,    79,    64,   580,   117,    49,    68,   296,    19,
     484,   299,    32,    56,   680,    35,   117,   117,   684,   117,
      97,    98,    32,    66,    79,   491,   492,   117,    48,    49,
     449,   450,    98,    98,   453,   454,    56,    47,    79,   117,
     117,    32,    97,    98,   125,   117,    66,   125,   340,   127,
     342,   117,   117,   117,    64,   116,   344,    98,    68,    73,
      97,   102,   117,   100,   123,   125,    57,   104,   125,   125,
     127,   127,   125,   100,   425,   426,   427,   428,   429,   430,
     431,    72,    79,   432,   433,   126,   437,   438,    79,   440,
     125,   117,     7,   154,   445,    22,   447,   126,   123,   675,
     451,   117,    98,    32,   680,   117,   116,    98,   684,   170,
      98,    25,    77,   126,    32,   126,    79,   468,   126,   126,
     471,   472,    98,   542,   543,   128,   117,    97,    55,   128,
     481,    58,   123,   552,   195,   468,   126,    79,   426,    57,
     428,    25,    22,   562,   154,    55,   565,   126,   481,   500,
     123,   126,    23,    23,    72,    98,   128,    97,   499,   128,
     170,    79,   128,   100,   125,    72,    93,   500,    95,    96,
      97,   126,    99,   100,   694,    55,   351,   104,    58,    17,
      98,   757,    65,   534,   111,   195,   537,   480,   550,   697,
     766,   118,   119,   120,   697,   569,   698,   603,   549,   117,
     770,   756,   741,   318,   555,   123,   557,   440,   715,   560,
     531,   272,   563,    93,    -1,   791,   567,    97,    -1,    99,
     100,    -1,    -1,    -1,   104,   801,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,   295,    -1,   587,   298,   118,   119,
     120,   302,    -1,   594,     2,   596,    -1,    -1,   599,    -1,
      -1,    -1,    -1,    11,   587,    -1,    -1,    -1,   141,    -1,
      -1,    19,   272,   682,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,   345,   295,    -1,    22,   298,    47,
     351,    -1,   302,   176,    -1,    -1,   647,   648,    -1,   650,
     651,   652,   653,    -1,    -1,    -1,    64,    -1,   659,   660,
      68,   662,    -1,   664,   665,    -1,    -1,   668,    -1,    -1,
      55,    -1,    -1,    58,    -1,    -1,    -1,    -1,   679,   680,
       3,    -1,    -1,   684,   685,   345,     9,    -1,   689,    -1,
      -1,   351,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     701,   702,   703,   704,   705,   706,   707,   708,   116,    -1,
     421,    -1,    97,    36,    99,   100,   717,   718,   719,   104,
      -1,    -1,   723,    -1,    -1,   726,   111,    -1,    -1,    -1,
      53,    54,   265,   266,    -1,   120,   719,    -1,    -1,    -1,
       2,    -1,    -1,    66,    -1,    68,   154,   458,    -1,    11,
      -1,   752,   753,   754,   755,    -1,    -1,    19,    -1,    -1,
      -1,    -1,   170,    -1,    -1,   766,   767,   768,    -1,    -1,
      32,    32,    -1,   774,   775,   776,   777,   778,   779,    -1,
      -1,    -1,    -1,    -1,   495,    47,    -1,   195,   499,   790,
     791,    -1,   325,    -1,    -1,    -1,    57,    -1,   458,   332,
     801,    -1,    64,    -1,    -1,    -1,    68,   340,    -1,   342,
      -1,    72,    15,    -1,    -1,    18,    -1,    -1,    79,    -1,
      -1,    -1,   533,    26,    27,    28,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    22,   495,    -1,    98,    -1,   499,
      -1,    -1,    -1,    -1,   555,    -1,   557,    -1,    51,   560,
      53,    54,   563,    -1,   116,   566,   117,    -1,    -1,    -1,
      -1,    -1,   123,    66,   272,    68,    -1,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,   586,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   594,    -1,   596,    -1,   295,    -1,    -1,
     298,    -1,   154,    -1,   302,   555,    -1,   557,    -1,    -1,
     560,    -1,    -1,   563,    -1,   438,   566,   440,   170,    97,
       2,    99,   100,    -1,   117,    -1,   104,    -1,    -1,    11,
      -1,   124,    -1,   111,    -1,    -1,   586,    19,    -1,    -1,
     118,   119,   120,   195,   594,    -1,   596,   345,    -1,    -1,
      32,    -1,    -1,   351,    -1,    -1,    -1,    15,   659,    -1,
      18,    -1,    -1,   664,    -1,    47,    -1,    -1,    26,    27,
      28,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    68,   688,    -1,    -1,
      -1,   692,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,    66,   659,
      68,    11,    -1,    -1,   664,    -1,    -1,    -1,    -1,    19,
     272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,   116,    -1,    -1,    -1,   688,    -1,
      -1,    -1,   692,   295,    -1,    -1,   298,    47,    -1,    19,
     302,   752,   753,   754,   755,    -1,    -1,    -1,    -1,   117,
     458,    -1,    32,    -1,    64,    35,   767,   768,    68,    -1,
      -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,   170,    -1,
      -1,    -1,    -1,   345,    -1,    -1,    66,   495,    -1,   351,
      -1,   499,   752,   753,   754,   755,    -1,    -1,    78,    -1,
      -1,    -1,    -1,   195,    -1,    -1,   116,   767,   768,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,
     120,    -1,   675,    -1,   154,    -1,    -1,   555,    -1,   557,
      -1,    -1,   560,    55,    -1,   563,    58,    -1,   566,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   586,    -1,
     272,    -1,    -1,    -1,    -1,   195,   594,    -1,   596,    -1,
     723,    93,    -1,    95,    96,    97,   458,    99,   100,   101,
      -1,    -1,   104,   295,    -1,    -1,   298,    -1,   110,   111,
     302,    -1,    -1,   115,    -1,    -1,   118,   119,   120,    -1,
      -1,    -1,    -1,    -1,   757,     2,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   495,    11,    -1,    -1,   499,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   659,    -1,   345,    -1,    32,   664,    -1,   791,   351,
      -1,    -1,   272,    -1,    -1,    -1,    -1,    -1,   801,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     688,    -1,    -1,    -1,   692,   295,    -1,    64,   298,    -1,
      22,    68,   302,   555,    -1,   557,    -1,    -1,   560,    -1,
      -1,   563,    -1,    -1,   566,    -1,    -1,    -1,     2,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    55,   586,    19,    58,    -1,    -1,    43,
      -1,    -1,   594,    -1,   596,   345,    -1,    -1,    32,   116,
      -1,   351,    -1,    -1,   752,   753,   754,   755,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,   767,
     768,    93,    -1,    95,    96,    97,   458,    99,   100,    -1,
      64,    -1,   104,    -1,    68,    -1,    -1,   154,   110,   111,
      -1,    -1,    -1,   115,    -1,    -1,   118,   119,   120,    -1,
      -1,    -1,    -1,   170,    -1,    -1,    -1,   659,    -1,    -1,
      -1,    -1,   664,   495,    -1,    -1,   120,   499,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,
      -1,    -1,   116,    -1,    -1,    -1,   688,    -1,    -1,    -1,
     692,    -1,    -1,    -1,    -1,   149,   150,   151,   152,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,   458,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
     154,    -1,    -1,   555,    -1,   557,    -1,    -1,   560,    -1,
      -1,   563,   186,    -1,   566,    55,   170,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,   495,    -1,    -1,    -1,   499,
     752,   753,   754,   755,   586,   272,    -1,    -1,    -1,    -1,
      -1,   195,   594,    -1,   596,   767,   768,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    -1,    96,    97,   295,    99,
     100,   298,    -1,    -1,   104,   302,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
     120,    -1,    -1,    -1,    -1,   555,    -1,   557,    -1,    -1,
     560,    -1,    -1,   563,    -1,    -1,   566,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   279,    -1,   659,   345,    -1,
      -1,    -1,   664,    -1,   351,    -1,   586,    -1,   272,    -1,
      -1,    -1,     2,    -1,   594,    -1,   596,   301,    -1,    -1,
      -1,    11,    -1,    -1,    -1,    -1,   688,    -1,    -1,    19,
     692,   295,    -1,    -1,   298,    -1,    -1,    -1,   302,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,   659,
      -1,   345,    -1,    -1,   664,    -1,    -1,   351,    -1,    -1,
     752,   753,   754,   755,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   767,   768,    -1,   688,    -1,
      -1,   458,   692,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    17,    -1,   116,    -1,    21,    22,
      23,    -1,    -1,    -1,    -1,    -1,    29,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   495,    -1,
      -1,    -1,   499,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   449,   450,    -1,    -1,   453,
     454,    -1,   752,   753,   754,   755,    -1,    -1,    71,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   767,   768,    -1,
      -1,    -1,    -1,    -1,   458,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,   555,    -1,
     557,    -1,    -1,   560,    -1,    -1,   563,    -1,    -1,   566,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   495,    -1,    -1,    -1,   499,    -1,    -1,    -1,   586,
      -1,    -1,    -1,    -1,   137,    -1,    -1,   594,   141,   596,
      -1,    -1,   145,    -1,   147,    -1,    -1,    -1,   542,   543,
     153,    -1,    -1,   156,    -1,    -1,    -1,    -1,   552,    -1,
     554,    -1,    -1,   166,    -1,   559,    -1,    -1,   562,    -1,
      -1,   565,   272,   176,    -1,    -1,    -1,    -1,    -1,   182,
      -1,   555,    -1,   557,    -1,    -1,   560,    -1,    -1,   563,
      -1,    -1,   566,    -1,    -1,   295,    -1,    -1,   298,    -1,
      -1,    -1,   659,    -1,    -1,    -1,    -1,   664,    -1,   212,
      -1,    -1,   586,   216,   217,   218,   219,    -1,    -1,    -1,
     594,    -1,   596,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     233,   688,    -1,    -1,    -1,   692,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   345,    -1,    -1,    -1,    -1,
      -1,   351,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   265,   266,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   659,    -1,   290,   682,    -1,
     664,    -1,    -1,    -1,    -1,   752,   753,   754,   755,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   309,    -1,    -1,    -1,
     767,   768,    -1,    -1,   688,   318,   710,   711,   692,   713,
     714,    -1,   325,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,   727,    11,    -1,    -1,    -1,   732,    -1,
     734,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,   354,    29,    30,    31,    -1,    33,    -1,   458,    -1,
      -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    52,   770,    -1,   752,   753,
     754,   755,    -1,    -1,    61,    62,    63,    64,    -1,    -1,
      -1,    -1,    -1,   767,   768,   495,    -1,    -1,    -1,   499,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,     3,   112,    -1,   440,   115,    -1,
      -1,    10,    -1,   120,    -1,    14,   123,    -1,   125,    -1,
     127,    -1,    -1,    -1,    -1,   555,    -1,   557,    -1,   462,
     560,    -1,   465,   563,    -1,    -1,   566,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   484,    -1,    -1,    -1,    -1,   586,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   594,    -1,   596,    -1,    -1,    -1,
      -1,    -1,   505,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
      99,    -1,    -1,    -1,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   550,    -1,   118,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   571,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   688,    -1,
      -1,    -1,   692,    -1,     0,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,   619,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    -1,
      66,    -1,    68,    69,    70,    -1,    -1,    73,    -1,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,   697,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,   112,    -1,   114,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,   123,    -1,   125,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   736,   737,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    -1,    68,    69,    70,    -1,    -1,    73,    -1,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
       1,   127,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    -1,    66,    -1,    68,    69,    70,
      -1,    -1,    73,    -1,    75,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,    -1,   114,   115,   116,   117,    -1,    -1,   120,
      -1,    -1,   123,   124,   125,     1,   127,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    -1,
      66,    -1,    68,    69,    70,    -1,    -1,    73,    -1,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,   112,    -1,   114,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,   123,   124,   125,
       1,   127,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    -1,    66,    -1,    68,    69,    70,
      -1,    -1,    73,    -1,    75,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,    -1,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,   123,   124,   125,     1,   127,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    -1,
      66,    -1,    68,    69,    70,    -1,    -1,    73,    -1,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,   112,    -1,   114,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,   123,   124,   125,
       1,   127,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    -1,    66,    -1,    68,    69,    70,
      -1,    -1,    73,    -1,    75,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,    -1,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,   123,   124,   125,     1,   127,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    -1,
      66,    -1,    68,    69,    70,    -1,    -1,    73,    -1,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,   112,    -1,   114,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,   123,    -1,   125,
       1,   127,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    -1,    66,    -1,    68,    69,    70,
      -1,    -1,    73,    -1,    75,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,    -1,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,   123,    -1,   125,     1,   127,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    -1,
      66,    -1,    68,    69,    70,    -1,    -1,    73,    -1,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,   112,    -1,   114,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,   123,    -1,   125,
       1,   127,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    -1,    66,    -1,    68,    69,    70,
      -1,    -1,    73,    -1,    75,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,    -1,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,   123,    -1,   125,     1,   127,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    -1,
      66,    -1,    68,    69,    70,    -1,    -1,    73,    -1,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,   112,    -1,   114,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,   123,    -1,   125,
       1,   127,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    -1,    66,    -1,    68,    69,    70,
      -1,    -1,    73,    -1,    75,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,    -1,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,   123,    -1,   125,     1,   127,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    -1,
      66,    -1,    68,    69,    70,    -1,    -1,    73,    -1,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,   112,    -1,   114,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,   123,    -1,   125,
       1,   127,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    -1,    66,    -1,    68,    69,    70,
      -1,    -1,    73,    -1,    75,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,    -1,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,   123,    -1,   125,     1,   127,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    -1,
      66,    -1,    68,    69,    70,    -1,    -1,    73,    -1,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,   112,    -1,   114,   115,
      -1,   117,    -1,    -1,   120,    -1,    -1,   123,    -1,   125,
       1,   127,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    -1,    66,    -1,    68,    69,    70,
      -1,    -1,    73,    -1,    75,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,    -1,   114,   115,    -1,   117,    -1,    -1,   120,
      -1,    -1,   123,    -1,   125,     1,   127,     3,     4,     5,
       6,     7,     8,    -1,    10,    11,    -1,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,    -1,    33,    -1,    -1,
      -1,    -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    -1,    -1,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    -1,   118,   119,   120,    -1,    -1,   123,    -1,   125,
       1,   127,     3,     4,     5,     6,     7,     8,    -1,    10,
      11,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    -1,    -1,
      61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    97,    -1,    99,   100,
     101,    -1,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,   118,   119,   120,
      -1,    -1,   123,    -1,   125,     1,   127,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,    -1,    33,    -1,    -1,
      -1,    -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    11,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,   101,    24,    -1,    -1,    -1,
      -1,    29,    30,    31,   110,    33,   112,    -1,   114,   115,
      38,    39,    -1,    41,   120,    43,    44,   123,    -1,   125,
      -1,   127,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    11,    -1,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    -1,    -1,   101,    24,    -1,    -1,    -1,    -1,    29,
      30,    31,   110,    33,   112,    -1,   114,   115,    38,    39,
      -1,    41,   120,    43,    44,   123,    -1,   125,    -1,   127,
      50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      -1,   101,    -1,     3,     4,     5,     6,     7,     8,    -1,
     110,    11,   112,    -1,   114,   115,   116,    -1,    -1,    -1,
     120,    -1,    -1,   123,    24,   125,    -1,   127,   128,    29,
      30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,
      -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      -1,   101,   102,     3,     4,     5,     6,     7,     8,    -1,
     110,    11,   112,    -1,   114,   115,   116,    -1,    -1,    -1,
     120,    -1,    -1,   123,    24,   125,    -1,   127,    -1,    29,
      30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,
      -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      -1,   101,    -1,     3,     4,     5,     6,     7,     8,    -1,
     110,    11,   112,    -1,   114,   115,   116,    -1,    -1,    -1,
     120,    -1,    -1,   123,    24,   125,   126,   127,    -1,    29,
      30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,
      -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,    49,
      50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      -1,   101,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
     110,    33,   112,    -1,   114,   115,    38,    39,    -1,    41,
     120,    43,    44,   123,    -1,   125,    -1,   127,    50,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,
      -1,     3,     4,     5,     6,     7,     8,    -1,   110,    11,
     112,    -1,   114,   115,   116,    -1,    -1,    -1,   120,    -1,
      -1,   123,    24,   125,    -1,   127,    -1,    29,    30,    31,
      -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,
      -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
     112,    -1,   114,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,   123,    -1,   125,    -1,   127,     3,     4,     5,     6,
       7,     8,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    -1,    -1,
      -1,    -1,    29,    30,    31,    -1,    33,    -1,    -1,    -1,
      -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    -1,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,   101,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,   110,    33,   112,    -1,   114,   115,    38,
      39,    -1,    41,   120,    43,    44,   123,    -1,   125,    -1,
     127,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,   101,    -1,     3,     4,     5,     6,     7,     8,
      -1,   110,    11,   112,    -1,   114,   115,   116,    -1,    -1,
      -1,   120,    -1,    -1,   123,    24,   125,    -1,   127,    -1,
      29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,
      39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,   101,    -1,     3,     4,     5,     6,     7,     8,
      -1,   110,    11,   112,    -1,   114,   115,    -1,    -1,    -1,
      -1,   120,    -1,    -1,   123,    24,   125,    -1,   127,    -1,
      29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,
      39,    -1,    41,    -1,    43,    44,    45,    -1,    -1,    -1,
      -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,   101,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,   110,    33,   112,    -1,   114,   115,    38,    39,    -1,
      41,   120,    43,    44,   123,    -1,   125,    -1,   127,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,     3,     4,     5,     6,     7,     8,    -1,   110,
      11,   112,    -1,   114,   115,   116,    -1,    -1,    -1,   120,
      -1,    -1,   123,    24,   125,    -1,   127,    -1,    29,    30,
      31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,     3,     4,     5,     6,     7,     8,    -1,   110,
      11,   112,    -1,   114,   115,   116,    -1,    -1,    -1,   120,
      -1,    -1,   123,    24,   125,    -1,   127,    -1,    29,    30,
      31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,     3,     4,     5,     6,     7,     8,    -1,   110,
      11,   112,    -1,   114,   115,   116,    -1,    -1,    -1,   120,
      -1,    -1,   123,    24,   125,    -1,   127,    -1,    29,    30,
      31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    11,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    24,    -1,    -1,    -1,    -1,    29,    30,    31,   110,
      33,   112,    -1,   114,   115,    38,    39,    -1,    41,   120,
      43,    44,   123,    -1,   125,    -1,   127,    50,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    24,
      -1,    -1,    -1,    -1,    29,    30,    31,   110,    33,   112,
      -1,   114,   115,    38,    39,    -1,    41,   120,    43,    44,
     123,    -1,   125,    -1,   127,    50,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    11,    -1,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    -1,    -1,   101,    24,    -1,    -1,
      -1,    -1,    29,    30,    31,   110,    33,   112,    -1,   114,
     115,    38,    39,    -1,    41,   120,    43,    44,   123,    -1,
     125,    -1,   127,    50,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    -1,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,   101,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,   110,    33,   112,    -1,   114,   115,    38,
      39,    -1,    41,   120,    43,    44,   123,    -1,   125,    -1,
     127,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,   101,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,   110,    33,   112,    -1,   114,   115,    38,    39,    -1,
      41,   120,    43,    44,   123,    -1,   125,    -1,   127,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    11,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    24,    -1,    -1,    -1,    -1,    29,    30,    31,   110,
      33,   112,    -1,   114,   115,    38,    39,    -1,    41,   120,
      43,    44,   123,    -1,   125,    -1,   127,    50,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    24,
      -1,    -1,    -1,    -1,    29,    30,    31,   110,    33,   112,
      -1,   114,   115,    38,    39,    -1,    41,   120,    43,    44,
     123,    -1,   125,    -1,   127,    50,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    11,    -1,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    -1,    -1,   101,    24,    -1,    -1,
      -1,    -1,    29,    30,    31,   110,    33,   112,    -1,   114,
     115,    38,    39,    -1,    41,   120,    43,    44,   123,    -1,
     125,    -1,   127,    50,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    -1,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,   101,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,   110,    33,   112,    -1,   114,   115,    38,
      39,    -1,    41,   120,    43,    44,   123,    -1,   125,    -1,
     127,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    14,    -1,    -1,    94,    -1,    -1,    -1,    98,
      22,    -1,    -1,   102,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,   112,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    24,   125,   126,   127,    -1,
      29,    30,    31,    55,    33,    -1,    58,    -1,    -1,    38,
      39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,
      -1,    50,    74,    52,    10,    -1,    78,    -1,    14,    -1,
      -1,    -1,    61,    62,    63,    64,    22,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,    94,   118,   119,   120,    55,
      -1,    -1,    58,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    78,    -1,    14,    -1,   125,    -1,   127,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    55,    -1,    -1,    58,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    78,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,    -1,   118,   119,
     120,    55,    -1,    -1,    58,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    78,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,    -1,   118,   119,   120,    55,    -1,    -1,
      58,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      78,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      32,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,    -1,
     118,   119,   120,    55,    -1,    -1,    58,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    10,    -1,    78,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    32,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,    -1,   118,   119,   120,    55,
      -1,   123,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    78,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    55,    -1,   123,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    10,    -1,    78,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,    -1,   118,   119,
     120,    55,    -1,   123,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    78,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,    -1,   118,   119,   120,    55,    -1,   123,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      78,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,    -1,
     118,   119,   120,    55,    -1,   123,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    78,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,    -1,   118,   119,   120,    55,
      -1,   123,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    78,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    55,    -1,   123,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    78,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,    -1,   118,   119,
     120,    55,    -1,   123,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    78,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,    -1,   118,   119,   120,    55,    -1,   123,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      78,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,    -1,
     118,   119,   120,    55,    -1,   123,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    78,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,    -1,   118,   119,   120,    55,
      -1,   123,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    78,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    55,    -1,   123,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    78,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,    -1,   118,   119,
     120,    55,    -1,   123,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    78,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,    -1,   118,   119,   120,    55,    -1,   123,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      78,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,    -1,
     118,   119,   120,    55,    -1,   123,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    78,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    32,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,    -1,   118,   119,   120,    55,
     122,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      10,    -1,    78,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    32,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    55,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    78,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    32,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,    -1,   118,   119,
     120,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    78,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,    -1,   118,   119,   120,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      78,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,    55,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    78,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,    55,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    78,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,    55,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    77,    78,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,    -1,   118,   119,
     120,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    78,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,    -1,   118,   119,   120,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      78,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,    55,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    78,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,    55,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    78,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,    55,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    78,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    78,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      78,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,    55,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    78,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,    55,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    78,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,    55,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    78,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    78,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      78,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,    55,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    78,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,    55,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    78,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,    55,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    78,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    78,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    10,    -1,
      78,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,    -1,
     118,   119,   120,    55,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    78,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    25,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,    55,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    78,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    55,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    77,    78,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,    -1,   118,   119,
     120,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    78,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,    -1,   118,   119,   120,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      78,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,    -1,
     118,   119,   120,    55,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    78,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,    -1,   118,   119,   120,    55,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    78,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    55,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    78,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,    -1,   118,   119,
     120,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    78,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,    -1,   118,   119,   120,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      78,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,    -1,
     118,   119,   120,    55,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    78,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,    -1,   118,   119,   120,    55,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    78,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    55,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    78,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,    -1,   118,   119,
     120,    55,    22,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    22,    -1,    58,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    78,   113,
     114,   115,    -1,    -1,   118,   119,   120,    -1,    22,    55,
      -1,    -1,    58,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,    -1,   108,   109,
     110,   111,    78,   113,   114,   115,    -1,    -1,   118,   119,
     120,    55,    22,    -1,    58,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,    -1,   108,   109,   110,   111,    -1,   113,    -1,   115,
      -1,    -1,   118,   119,   120,    55,    -1,    -1,    58,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,    -1,   108,   109,   110,   111,    -1,   113,
      -1,   115,    -1,    -1,   118,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,    -1,   104,   105,   106,    -1,   108,   109,
     110,   111,    -1,    -1,    -1,   115,    -1,    -1,   118,   119,
     120,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,   122,    -1,    -1,   125,    -1,   127
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   133,   134,     0,     1,     3,     4,     5,     6,     7,
       8,    11,    12,    13,    16,    17,    18,    20,    21,    23,
      24,    26,    27,    28,    29,    30,    31,    33,    34,    37,
      38,    39,    40,    41,    42,    43,    44,    46,    50,    51,
      52,    57,    59,    60,    61,    62,    63,    64,    66,    69,
      73,    75,    78,    93,    94,    95,    96,   101,   110,   112,
     114,   115,   117,   120,   123,   125,   127,   135,   136,   137,
     138,   139,   140,   142,   143,   145,   147,   148,   149,   150,
     152,   153,   156,   157,   158,   162,   165,   168,   169,   189,
     192,   193,   211,   212,   213,   214,   215,   216,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   232,   233,
     234,   235,   236,   137,   224,    74,   217,   218,   144,   145,
     217,    11,    29,    30,    31,    64,   123,   127,   165,   211,
     215,   222,   223,   224,   225,   227,   228,    66,   144,   224,
     137,   125,   145,   145,     9,    66,   145,    49,    76,   151,
     224,   224,   224,   125,   145,   166,   125,   145,   194,   195,
     137,   224,   224,   224,   224,     7,   125,   117,   224,   224,
      23,   140,   146,   224,   224,    63,   125,   137,   224,    59,
     145,   190,   116,   185,   205,   224,   224,   224,   224,   224,
     224,   224,   224,   124,   135,   141,   205,    67,   102,   185,
     206,   207,   224,   205,   224,   231,    51,   137,   145,    15,
      54,    68,   159,    36,    53,   181,    19,    49,    56,    70,
     117,   122,    10,    14,    22,    55,    58,    78,    93,    95,
      96,    97,    99,   100,   101,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   113,   114,   115,   118,   119,   120,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,   121,   125,   127,    55,    58,   125,
     137,   117,   123,   140,   224,   224,   224,   205,    32,   217,
     190,   117,   117,    73,   145,   185,   208,   209,   210,   224,
     123,   190,   145,   125,   146,    23,    32,   146,    23,    32,
     146,   218,    65,   140,   208,   137,   125,   176,    67,   125,
     145,   196,   197,     1,    97,   199,   200,    32,    98,   146,
     208,   117,   123,   137,   146,   125,   208,   205,    79,   191,
     117,   145,    98,   117,   146,   117,   124,   135,   124,   224,
      98,   126,    98,   126,    32,   128,   218,    77,    98,   128,
       7,   123,   145,   170,    56,   194,   194,   194,   194,   224,
     224,   224,   224,   151,   224,   151,   224,   224,   224,   224,
     224,   224,   224,    24,    66,   145,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   208,   208,   151,
     224,   151,   224,    19,    32,    35,    48,    49,    56,    66,
     115,   179,   219,   220,   236,    23,    32,    23,    32,    65,
      32,   128,   151,   224,   146,   117,   224,    77,    79,   126,
      98,   145,   163,   164,   117,    32,   205,    31,   224,   151,
     224,    31,   224,   151,   224,   146,   137,   224,    25,   126,
     177,   178,   179,   167,   197,    98,   126,     1,   127,   201,
     212,    77,    79,   198,   224,   195,   126,   154,   208,   126,
     123,   127,   201,   212,    98,   185,   224,   126,   126,   206,
     206,   151,   224,   137,   224,   128,   224,   224,   124,   141,
      97,   160,    49,    56,   172,   180,   194,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   126,   128,    32,    56,
     211,    98,   126,    55,    31,   151,   224,    31,   151,   224,
     151,   224,   217,   217,   117,   224,   185,   224,   210,    79,
      98,   124,   224,   126,   224,    23,   146,    23,   146,   224,
      23,   146,   218,    23,   146,   218,    25,    25,   137,    98,
     126,   125,   145,    19,    49,    56,    66,   182,   126,   197,
      97,   200,   205,   224,    45,   224,    47,    71,   124,   155,
     126,   154,   205,   190,   128,   218,   128,   218,   137,    77,
     124,   205,   123,   171,    10,    14,    79,    80,    81,    82,
      83,    84,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    99,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   115,   118,   119,   120,   121,   122,
     145,   173,   174,   117,   220,   211,   224,    23,    23,   224,
      23,    23,   128,   128,   146,   146,   224,   164,   146,    65,
      31,   224,    31,   224,    65,    31,   224,   146,    31,   224,
     146,   137,   224,   178,   197,    97,   204,   200,   198,    32,
     128,   137,   205,   124,   128,    31,   137,   224,   128,    31,
     137,   224,   128,   224,   161,   182,   145,   100,   125,   175,
     175,    65,    31,    31,    65,    31,    31,    31,    31,   224,
     224,   224,   224,   224,   224,   126,    24,    61,    64,   127,
     185,   203,   212,   102,   187,   198,    72,   188,   224,   201,
     212,   146,   224,   137,   224,   137,    28,    66,   124,   136,
     148,   200,   145,   173,   174,   177,   224,   224,   224,   224,
     224,   224,    65,    65,    65,    65,   204,   128,   205,   185,
     186,   224,   224,   140,   147,   184,   128,    65,    65,   148,
     188,   175,   175,   126,    65,    65,    65,    65,    65,    65,
     224,   224,   224,   224,   187,   198,   185,   202,   203,   212,
      32,   128,   212,   224,   224,   117,   183,   184,   224,   202,
     203,   128,   202
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   132,   133,   134,   134,   135,   135,   136,   136,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   138,   138,   139,   140,   140,
     141,   141,   142,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   144,   144,
     145,   146,   146,   147,   147,   148,   148,   148,   148,   148,
     148,   149,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   151,   152,   152,   152,   152,   153,
     154,   154,   155,   155,   156,   157,   158,   158,   159,   159,
     159,   160,   160,   161,   161,   161,   162,   163,   163,   163,
     164,   164,   166,   167,   165,   168,   168,   168,   168,   168,
     168,   170,   171,   169,   172,   172,   172,   172,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   175,   175,   176,   177,
     177,   177,   178,   178,   178,   178,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   180,   180,   180,   181,
     181,   182,   182,   182,   182,   182,   183,   183,   184,   184,
     185,   185,   186,   186,   187,   187,   188,   188,   189,   189,
     189,   190,   190,   191,   191,   191,   192,   192,   192,   192,
     192,   193,   193,   193,   194,   194,   195,   195,   195,   196,
     196,   196,   197,   197,   197,   198,   198,   198,   199,   199,
     200,   200,   200,   200,   201,   201,   201,   201,   202,   202,
     202,   203,   203,   203,   203,   203,   204,   204,   204,   204,
     204,   204,   204,   205,   205,   205,   205,   206,   206,   206,
     207,   207,   208,   208,   209,   209,   210,   210,   210,   210,
     210,   211,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   214,   215,   216,   216,   216,   216,   216,   216,   216,
     216,   217,   217,   218,   219,   219,   220,   220,   221,   221,
     222,   223,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   225,   225,   225,   225,
     226,   226,   227,   227,   227,   228,   228,   228,   229,   229,
     229,   230,   230,   230,   230,   230,   230,   230,   230,   231,
     231,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   233,   233,   233,   233,   233,   233,
     234,   234,   234,   234,   235,   235,   235,   235,   236,   236,
     236,   236,   236,   236,   236
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     2,     2,     3,     1,     4,     5,     1,     2,     3,
       1,     2,     3,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     0,     1,
       1,     2,     1,     2,     3,     1,     1,     1,     1,     1,
       1,     2,     5,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     6,     7,
       6,     7,     4,     5,     4,     4,     3,     6,     5,     5,
       0,     2,     3,     2,     6,     7,     0,     1,     1,     1,
       1,     0,     2,     0,     2,     3,     5,     1,     2,     3,
       1,     3,     0,     0,     8,     0,     1,     1,     2,     1,
       2,     0,     0,     9,     3,     3,     5,     5,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     3,     0,
       1,     3,     4,     4,     6,     6,     0,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     0,     1,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     0,     1,     2,     2,     0,     2,     3,     4,
       4,     2,     4,     0,     2,     2,     4,     4,     4,     5,
       4,     0,     1,     1,     1,     3,     3,     4,     5,     1,
       1,     3,     1,     2,     3,     0,     2,     2,     0,     4,
       0,     2,     2,     1,     4,     4,     6,     1,     0,     1,
       1,     3,     4,     3,     4,     6,     0,     2,     2,     2,
       2,     2,     2,     1,     1,     3,     3,     1,     1,     1,
       3,     3,     0,     1,     1,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     4,     4,
       4,     5,     2,     2,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     2,     3,     1,     3,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     3,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     4,     3,     3,     3,     3,     4,
       3,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      yyerror (&yylloc, context, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, context); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* context)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (context);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* context)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, context);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, ParserContext* context)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , context);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, context); \
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
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
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ParserContext* context)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (context);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}



struct yypstate
  {
    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;
    /* Used to determine if this is the first time this instance has
       been used.  */
    int yynew;
  };

/* Initialize the parser data structure.  */
yypstate *
yypstate_new (void)
{
  yypstate *yyps;
  yyps = (yypstate *) malloc (sizeof *yyps);
  if (!yyps)
    return YY_NULLPTR;
  yyps->yynew = 1;
  return yyps;
}

void
yypstate_delete (yypstate *yyps)
{
#ifndef yyoverflow
  /* If the stack was reallocated but the parse did not complete, then the
     stack still needs to be freed.  */
  if (!yyps->yynew && yyps->yyss != yyps->yyssa)
    YYSTACK_FREE (yyps->yyss);
#endif
  free (yyps);
}

#define yynerrs yyps->yynerrs
#define yystate yyps->yystate
#define yyerrstatus yyps->yyerrstatus
#define yyssa yyps->yyssa
#define yyss yyps->yyss
#define yyssp yyps->yyssp
#define yyvsa yyps->yyvsa
#define yyvs yyps->yyvs
#define yyvsp yyps->yyvsp
#define yylsa yyps->yylsa
#define yyls yyps->yyls
#define yylsp yyps->yylsp
#define yyerror_range yyps->yyerror_range
#define yystacksize yyps->yystacksize


/*---------------.
| yypush_parse.  |
`---------------*/

int
yypush_parse (yypstate *yyps, int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE *yypushed_loc, ParserContext* context)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  if (!yyps->yynew)
    {
      yyn = yypact[yystate];
      goto yyread_pushed_token;
    }

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = *yypushed_loc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
      if (!yyps->yynew)
        {
          YYDPRINTF ((stderr, "Return for a new token:\n"));
          yyresult = YYPUSH_MORE;
          goto yypushreturn;
        }
      yyps->yynew = 0;
yyread_pushed_token:
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yypushed_char;
      if (yypushed_val)
        yylval = *yypushed_val;
      if (yypushed_loc)
        yylloc = *yypushed_loc;
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
| yyreduce -- Do a reduction.  |
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 401 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 4318 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 406 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4324 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 407 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4330 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 414 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4336 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 419 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4342 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 420 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 436 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 437 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 438 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 439 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 440 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 441 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 442 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 443 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 444 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 445 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 446 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 447 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 448 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 449 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 450 "chapel.ypp" /* yacc.c:1661  */
    { printf("syntax error"); clean_exit(1); }
#line 4438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 455 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yylsp[-3]).comment))); }
#line 4444 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 457 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yylsp[-4]).comment))); }
#line 4450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 461 "chapel.ypp" /* yacc.c:1661  */
    { (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 4462 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 477 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 4468 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4474 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 483 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4480 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 4486 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 4492 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 4498 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 4504 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 495 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 4510 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 4516 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 4522 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 4528 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 4534 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 500 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 4540 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 501 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 4546 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 502 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 4552 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 4558 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 504 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 4564 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 4570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 506 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 4576 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 4582 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4588 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4594 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 520 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4600 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 524 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 4606 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 525 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 4612 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 529 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 4618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 539 "chapel.ypp" /* yacc.c:1661  */
    {
#ifdef HAVE_LLVM
      if (externC) {
        (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
      } else {
        USR_FATAL(new BlockStmt(), "extern block syntax is turned off. Use --extern-c flag to turn on.");
      }
#else
      USR_FATAL(new BlockStmt(), "Chapel must be built with llvm in order to use the extern block syntax");
#endif
    }
#line 4634 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 4640 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4646 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4652 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 556 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 4658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 557 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4664 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 4670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 4676 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 560 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 4682 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 561 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 4688 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4694 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 4700 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 4706 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 4712 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 4718 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 567 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 4724 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 568 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 4730 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 570 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 4740 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 576 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 4750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 582 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 4760 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 588 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 4770 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 594 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 4781 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 601 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 4792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[-1].pcallexpr)); }
#line 4798 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 4804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 615 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4810 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 4816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 617 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 4822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 4828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 4834 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 627 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 4840 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 4846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 4852 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 639 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL((yyvsp[-3].pcallexpr), "'type select' is no longer supported. Use 'select'"); }
#line 4858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), (yyvsp[-6].flag), (yylsp[-6]).comment)); }
#line 4864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 650 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4870 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 651 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4876 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 655 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 4886 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 660 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 4896 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 665 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 4906 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 673 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 4912 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 674 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 4918 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 679 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 4924 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 681 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 4930 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 683 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 4936 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 688 "chapel.ypp" /* yacc.c:1661  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 4947 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[0].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[0].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
#line 4960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 707 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 4968 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 711 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 4977 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 718 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 4983 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 719 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 4989 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 724 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 4998 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 729 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5007 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 734 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-5].pfnsymbol)->retTag = (yyvsp[-3].retTag);
      if ((yyvsp[-3].retTag) == RET_REF)
        USR_FATAL("'ref' return types are not allowed in lambdas");
      if ((yyvsp[-3].retTag) == RET_PARAM)
        USR_FATAL("'param' return types are not allowed in lambdas");
      if ((yyvsp[-3].retTag) == RET_TYPE)
        USR_FATAL("'type' return types are not allowed in lambdas");
      if ((yyvsp[-2].pexpr))
        (yyvsp[-5].pfnsymbol)->retExprType = new BlockStmt((yyvsp[-2].pexpr), BLOCK_SCOPELESS);
      if ((yyvsp[-1].pexpr))
        (yyvsp[-5].pfnsymbol)->where = new BlockStmt((yyvsp[-1].pexpr));
      (yyvsp[-5].pfnsymbol)->insertAtTail((yyvsp[0].pblockstmt));
      (yyval.pexpr) = new DefExpr(buildLambda((yyvsp[-5].pfnsymbol)));
    }
#line 5027 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 754 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5038 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 760 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5050 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 767 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5063 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 775 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5076 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 783 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5089 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 791 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5102 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 802 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5112 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 808 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5123 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 815 "chapel.ypp" /* yacc.c:1661  */
    {
      FnSymbol* fn = (yyvsp[-5].pfnsymbol);

      fn->copyFlags((yyvsp[-8].pfnsymbol));
      // The user explicitly named this function (controls mangling).
      if (*(yyvsp[-8].pfnsymbol)->name)
        fn->cname = (yyvsp[-8].pfnsymbol)->name;

      if ((yyvsp[-7].procIter) == ProcIter_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[-5].pfnsymbol), (yyvsp[-3].retTag), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), (yylsp[-8]).comment);
      context->latestComment = NULL;
    }
#line 5146 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 837 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5154 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 841 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5163 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 846 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
    }
#line 5171 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 850 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5180 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 858 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5186 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 859 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 5192 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 860 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 5198 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 861 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 5204 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 862 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 5210 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 863 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 5216 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 864 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 5222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 865 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 5228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 866 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 5234 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 867 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 5240 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 868 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 5246 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 869 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 5252 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 870 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 5258 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 871 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 5264 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 872 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 5270 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 873 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 5276 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 874 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 5282 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 875 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 5288 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 876 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 5294 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 877 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 5300 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 878 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 5306 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 879 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 5312 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 880 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "align"; }
#line 5318 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 881 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 5324 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 882 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 5330 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 886 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 5336 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 887 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 5342 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 888 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 5348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 889 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 5354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 890 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 5360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 891 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 5366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 892 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 5372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 893 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 5378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 894 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 5384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 895 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 5390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 896 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 5396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 897 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 5402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 901 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 5408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 902 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 906 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 910 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 5426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 911 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 5432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 912 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 5438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 917 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 5444 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 919 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 5450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 921 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 5456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 923 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 5462 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 927 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5468 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 928 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 5474 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 929 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 5480 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 930 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 5486 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 931 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 5492 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 932 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 5498 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 933 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 5504 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 934 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5510 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 935 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 5516 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 936 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 5522 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 940 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5528 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 941 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5534 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 942 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 5540 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 946 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 5546 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 947 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 5552 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 951 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 5558 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 952 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 5564 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 953 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 5570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 954 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 5576 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 955 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 5582 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 959 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 5588 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 965 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 5594 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 970 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 5600 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 972 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5606 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 976 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5612 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 981 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 982 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 5624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 986 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5630 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 987 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5636 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 992 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 5642 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 994 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 5648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 996 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 5654 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1001 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 5671 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1014 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 5689 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1030 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5695 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1032 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5701 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1034 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 5707 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1039 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
    }
#line 5718 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1046 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
    }
#line 5729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1053 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
    }
#line 5740 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1060 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
    }
#line 5752 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1068 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
    }
#line 5762 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1076 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5768 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1077 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5774 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1078 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5780 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1084 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 5789 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1092 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 5795 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1094 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 5805 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1100 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 5811 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1105 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 5817 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1107 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 5823 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1109 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 5829 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1114 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 5835 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1116 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5841 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1118 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5847 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1124 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5853 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1125 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 5859 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1126 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5865 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1130 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5871 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1132 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 5877 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1136 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5883 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1137 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5889 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1138 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 5895 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1139 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad type specification"); }
#line 5901 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1160 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 5909 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1164 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 5917 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1168 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 5929 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1175 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad array type specification"); clean_exit(1); }
#line 5935 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1179 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5941 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1180 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5947 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1181 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 5953 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1186 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 5959 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1188 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 5965 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 5971 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1196 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 5977 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 5983 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1202 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5989 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1203 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5995 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1204 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6001 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1205 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 6007 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1206 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6013 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1207 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6019 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1208 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6025 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1214 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6031 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1215 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6037 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1216 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6043 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1217 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6049 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6055 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1222 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6061 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1223 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6067 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1227 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6073 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1228 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6079 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1232 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6085 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1237 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6091 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1238 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6097 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1242 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6103 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1243 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6109 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1244 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6115 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1245 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6121 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1250 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6127 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1267 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6133 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1269 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6139 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1271 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6145 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1273 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6151 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1275 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6157 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1277 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6163 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1279 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6169 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1284 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6175 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1286 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6181 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1288 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6187 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1290 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6193 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1292 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6199 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1294 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6205 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1296 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6211 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1298 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6217 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1300 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6223 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1302 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6229 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1304 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6235 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1306 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6241 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1308 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 6252 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1315 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 6262 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1321 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 6272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1327 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 6282 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1333 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 6292 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1342 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 6298 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1346 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 6304 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1362 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6310 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1366 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6316 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1371 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 6322 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1376 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pIntentExpr).first, (yyvsp[0].pIntentExpr).second); }
#line 6328 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1378 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).first);
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).second);
    }
#line 6337 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1386 "chapel.ypp" /* yacc.c:1661  */
    {
      ArgSymbol* tiMark = tiMarkForIntent((yyvsp[-1].pt));
      if (!tiMark)
        USR_FATAL_CONT((yyvsp[0].pexpr), "Unsupported intent in with clause");
      (yyval.pIntentExpr).first = new SymExpr(tiMark);
      (yyval.pIntentExpr).second = (yyvsp[0].pexpr);
    }
#line 6349 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1395 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).first = (yyvsp[-2].pexpr), (yyval.pIntentExpr).second = (yyvsp[0].pexpr); }
#line 6355 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1401 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6361 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1406 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 6367 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1411 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 6373 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1427 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 6379 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1429 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6385 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1431 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6391 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1433 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedLow"), (yyvsp[-1].pexpr)); }
#line 6397 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1435 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedHigh"), (yyvsp[0].pexpr)); }
#line 6403 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1437 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range", buildDotExpr("BoundedRangeType", "boundedNone")); }
#line 6409 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6415 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1461 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6421 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1462 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 6427 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1466 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6433 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1467 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 6439 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1468 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 6445 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6451 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1477 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 6457 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 6463 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 6469 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1483 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 6475 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 6481 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 6487 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 6493 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6499 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 6505 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1490 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 6513 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6519 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1497 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6525 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1501 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6531 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1502 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6537 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6543 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1504 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6549 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6555 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 386:
#line 1506 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6561 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 387:
#line 1507 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6567 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 388:
#line 1508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6573 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1509 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6579 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6585 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1511 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6591 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1512 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6597 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1513 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6603 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6609 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6615 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6621 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1517 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6627 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1518 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6633 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6639 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1520 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6645 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6651 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6657 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1523 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6663 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1527 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 6669 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1528 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 6675 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1529 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 6681 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 6687 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 6693 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1532 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 6699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6705 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1538 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1539 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6723 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1543 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1544 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1545 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1546 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1551 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 6753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1552 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 6759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 6765 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 6771 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 6777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1556 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 6783 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1557 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 6789 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 6793 "bison-chapel.cpp" /* yacc.c:1661  */
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
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
      yyerror (&yylloc, context, YY_("syntax error"));
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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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
        yyerror (&yylloc, context, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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
                      yytoken, &yylval, &yylloc, context);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, context);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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
  yyerror (&yylloc, context, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, context);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, context);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  yyps->yynew = 1;

yypushreturn:
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
