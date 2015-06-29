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
#define YYLAST   10930

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  132
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  105
/* YYNRULES -- Number of rules.  */
#define YYNRULES  424
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  811

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
     515,   519,   520,   524,   525,   529,   530,   534,   535,   536,
     537,   538,   539,   543,   558,   559,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   573,
     574,   580,   586,   592,   598,   605,   615,   619,   620,   621,
     622,   626,   631,   632,   636,   638,   643,   650,   655,   663,
     668,   673,   681,   682,   687,   688,   690,   695,   705,   714,
     718,   726,   727,   732,   737,   731,   762,   768,   775,   783,
     794,   800,   793,   828,   832,   837,   841,   849,   850,   851,
     852,   853,   854,   855,   856,   857,   858,   859,   860,   861,
     862,   863,   864,   865,   866,   867,   868,   869,   870,   871,
     872,   873,   874,   878,   879,   880,   881,   882,   883,   884,
     885,   886,   887,   888,   889,   893,   894,   898,   902,   903,
     904,   908,   910,   912,   914,   919,   920,   921,   922,   923,
     924,   925,   926,   927,   928,   932,   933,   934,   935,   939,
     940,   944,   945,   946,   947,   948,   952,   953,   957,   958,
     962,   964,   969,   970,   974,   975,   979,   980,   984,   986,
     988,   993,  1006,  1023,  1024,  1026,  1031,  1038,  1045,  1052,
    1060,  1069,  1070,  1071,  1075,  1076,  1084,  1086,  1092,  1097,
    1099,  1101,  1106,  1108,  1110,  1117,  1118,  1119,  1123,  1124,
    1129,  1130,  1131,  1132,  1152,  1156,  1160,  1168,  1172,  1173,
    1174,  1178,  1180,  1186,  1188,  1190,  1195,  1196,  1197,  1198,
    1199,  1200,  1201,  1207,  1208,  1209,  1210,  1214,  1215,  1216,
    1220,  1221,  1225,  1226,  1230,  1231,  1235,  1236,  1237,  1238,
    1239,  1243,  1254,  1255,  1256,  1257,  1258,  1259,  1261,  1263,
    1265,  1267,  1269,  1271,  1276,  1278,  1280,  1282,  1284,  1286,
    1288,  1290,  1292,  1294,  1296,  1298,  1300,  1307,  1313,  1319,
    1325,  1334,  1339,  1347,  1348,  1349,  1350,  1351,  1352,  1353,
    1354,  1359,  1360,  1364,  1368,  1370,  1378,  1388,  1392,  1393,
    1398,  1403,  1411,  1412,  1413,  1414,  1415,  1416,  1417,  1418,
    1419,  1421,  1423,  1425,  1427,  1429,  1434,  1435,  1436,  1437,
    1448,  1449,  1453,  1454,  1455,  1459,  1460,  1461,  1469,  1470,
    1471,  1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,  1489,
    1490,  1494,  1495,  1496,  1497,  1498,  1499,  1500,  1501,  1502,
    1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,  1512,
    1513,  1514,  1515,  1516,  1520,  1521,  1522,  1523,  1524,  1525,
    1529,  1530,  1531,  1532,  1536,  1537,  1538,  1539,  1544,  1545,
    1546,  1547,  1548,  1549,  1550
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
  "assignment_stmt", "opt_ident", "ident", "opt_ident_or_string",
  "do_stmt", "return_stmt", "class_level_stmt", "extern_block_stmt",
  "loop_stmt", "zippered_iterator", "if_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "type_select_stmt", "class_decl_stmt",
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

#define YYPACT_NINF -693

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-693)))

#define YYTABLE_NINF -378

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -693,    47,  2270,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  3316,     4,   118,  -693,     4,  6028,   125,   118,  6028,
    3316,     7,   118,   263,   569,  5069,  6028,  6028,    59,  -693,
     118,  -693,    18,  3316,  6028,  -693,  6028,  -693,  6028,  6028,
     261,   194,  -693,  5247,  6028,  5372,  6028,   268,   205,  3316,
      91,  -693,  5450,  6028,  6028,  -693,  -693,  6028,  -693,  -693,
    6730,  6028,  6028,  -693,  6028,  -693,  -693,  2566,  4869,  5450,
    -693,  3191,  -693,  -693,   118,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,   118,  -693,   331,
     158,  -693,  -693,  -693,   130,   231,  -693,  -693,  -693,   236,
     239,   253,   242,   251, 10776,  7160,   -36,   278,   291,  -693,
    -693,  -693,  -693,  -693,  -693,   135,  -693, 10776,   275,  3316,
    -693,   317,  -693,   312,  6028,  6028,  6028,  6028,  6028,  5450,
    5450,   152,  -693,  -693,  -693,  -693,  8334,   280,  -693,  -693,
     118,   321,  8532,   371,  5450,   327,  -693,  -693,  -693,  -693,
     118,    68,   118,   329,    43,  7344,  7278,  7476,  5450,  3316,
     334,     9,    58,     5,  -693,  -693,   476,   360,  7542,   476,
    -693,  5450,  -693,  8598,  8202,  3316,  -693,  -693,  7542, 10776,
     338,  5450,  -693, 10776,  5450,   385,   351,   118,  -693,   -17,
   10776,  7542,  8664,   260,  1694,   476,   260,   476,  -693,  -693,
    2691,    74,  -693,  6028,  -693,   -38,    70, 10776,    80,  8730,
     -19,   465,  -693,   354,   376,  -693,  -693,  -693,    17,    18,
      18,    18,  -693,  6028,  6028,  6028,  6028,  5550,  5550,  6028,
    6028,  6028,  6028,  6028,  6028,    72,  6730,  6028,  6028,  6028,
    6028,  6028,  6028,  6028,  6028,  6028,  6028,  6028,  6028,  6028,
    6028,  6028,  6028,  6028,  6028,  6028,  6028,  6028,  6028,  6028,
    6028,  6028,  6028,  6028,  6028,  6028,  6028,  5450,  5450,  5550,
    5550,  1539,  -693,  -693,  2816,  -693,  8400,  8466,  8796,    61,
    5550,    43,   366,  -693,  -693,  6028,   332,  -693,   358,   387,
    -693, 10776,   118,   369,   118,   455,  5450,  -693,  3441,  5550,
    -693,  3566,  5550,  -693,    43,  3316,   463,   363,  -693,    36,
    -693,  -693,     9,  -693,   393,   367,  -693,  2019,   422,   421,
    6028,    18,  -693,   375,  -693,  -693,  -693,  -693,  5450,   379,
     177,  4691,   408,  -693,  -693,  5450,  -693,  -693,  -693,  -693,
    -693,  -693,  7212,  4969,  -693,  5147,  -693,  5550,  2438,   381,
    6028,  6028,  -693,  -693,  2941,  5450,   384,   350,    18,   107,
     166,   182,   237,  8268,   283,   283,   273,  -693,   273,  -693,
     273,  7042,  1192,   629,  1063,   360,   260,  -693,  -693,  -693,
    1694,  1873,   273,  1570,  1570,   283,  1570,  1570,   513,   260,
    1873, 10810,   513,   476,   476,   260,  8862,  8928,  8994,  9060,
    9126,  9192,  9258,  9324,  9390,  9456,  9522,  9588,  9654,  9720,
    9786,   390,   382,  -693,   273,  -693,   273,   228,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,   118,    78,  -693,   456,  6106,
    5550,  6184,  5550,  6028,  5550,  6908,     4,  9852,  -693,  -693,
    9918,  6028,  5450,  -693,  5450,   438,    83,  -693,  -693,   376,
    6028,    84,  6028, 10776,    46,  7608,  6028, 10776,     8,  7410,
    -693,   493,  9984,  3316,  -693,   167,  -693,    19,   277,   394,
       9,    66,  -693,  5450,  -693,   249,  6028,  5650,  -693, 10776,
    -693,  -693,    82,   396,  -693,  -693,  5450,  -693,   248,   118,
    -693, 10776,  -693,  -693,  -693,  -693,    -4,  2144,  -693, 10776,
    3316, 10776, 10050,  -693,  3066,   426,  -693,  -693,  -693,  -693,
    -693,  6963,   257,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  1539,  -693,   118,
    6028,   496, 10116,  6028,   502, 10182,   398,  6885,    43,    43,
    -693, 10776,  -693, 10776,  -693,  6028,   118,  -693,   404,  7542,
    -693,  7674,  3691,  -693,  3816,  -693,  7740,  3941,  -693,    43,
    4066,  -693,    43,  3316,  6028,  -693,   254,  -693,     9,   431,
    -693,  -693,  -693,  -693,    51,  -693,  -693,  4691,   421,    67,
   10776,  -693, 10776,  3316,  5450,  -693,  -693,  -693,   138,    69,
    -693,  4191,   405,  4316,   410,  -693,  6028,  -693,   898,   277,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,   118,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,   440,   416,   416,  -693,
    -693,  -693, 10248,  6262,  6340, 10314,  6418,  6496,  6574,  6652,
    -693,  -693, 10776,  -693,  -693,  -693,  3316,  6028, 10776,  6028,
   10776,  3316,  6028, 10776,  -693,  6028, 10776,  -693,  -693, 10776,
    -693,   417,  5728,   101,   473,  -693,  5550,  4441,  -693,    54,
    -693,  4566,  6028,  -693, 10776,  3316,  6028,  -693, 10776,  3316,
   10776,   735,   118,  -693,  1254,  -693,    23,  -693,  6963,    36,
    -693,  -693,  6028,  6028,  6028,  6028,  6028,  6028,  6028,  6028,
    1119,  9984,  7806,  7872,  9984,  7938,  8004,   431,     7,  6028,
    6028,  4769,  -693,  -693,   199,  5450,  -693,  -693,  6028,   -12,
    7076,  -693,   353,  -693,  8070,  -693,  8136,  -693,  -693,   473,
    -693,   416,   416,   169, 10380, 10446, 10512, 10578, 10644, 10710,
    -693,  3316,  3316,  3316,  3316,   101,  5828,    73,  -693,  -693,
   10776, 10776,  -693,  -693,  -693,  6652,  3316,  3316,   -13,  -693,
    -693,  -693,  6028,  6028,  6028,  6028,  6028,  6028,  9984,  9984,
    9984,  9984,  -693,  -693,  -693,  -693,  -693,   225,  5550,  6808,
     425,  9984,  9984,  -693,  -693,  -693,  7110,  -693,  -693,  5928,
    -693
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    34,    60,   371,   372,   373,   374,
     375,     0,   331,    58,   109,   331,     0,   232,    58,     0,
       0,     0,     0,    58,    58,     0,     0,     0,     0,   127,
       0,   123,     0,     0,     0,    37,     0,   322,     0,     0,
       0,     0,   110,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,     0,   420,   422,     0,   423,   424,
     355,     0,     0,   421,   418,    67,   419,     0,     0,     0,
       4,     0,     5,     9,     0,    10,    11,    13,   291,    19,
      12,    14,    16,    15,    17,    18,    69,     0,    70,   347,
       0,    68,    71,    72,     0,   356,   343,   344,   294,   292,
       0,     0,   348,   349,     0,   293,     0,   357,   358,   359,
     342,   296,   295,   345,   346,     0,    21,   302,     0,     0,
     332,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,   347,   356,   292,   348,   349,   331,   293,   357,   358,
       0,     0,     0,     0,   282,     0,    62,    61,   128,    73,
       0,   129,     0,     0,     0,     0,     0,     0,   282,     0,
       0,     0,     0,     0,   234,    28,   406,   340,     0,   407,
       7,   282,    65,     0,     0,     0,    64,    31,     0,   297,
       0,   282,    32,   303,     0,   223,     0,   211,   274,     0,
     273,     0,     0,   409,   354,   405,   408,   404,    38,    40,
       0,     0,   277,     0,   279,     0,     0,   278,     0,   273,
       0,     0,     6,     0,   112,   200,   199,   130,     0,     0,
       0,     0,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   353,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   282,   282,     0,
       0,   185,    22,    23,     0,    24,     0,     0,     0,     0,
       0,     0,     0,    25,    26,     0,   291,   289,     0,   283,
     284,   290,     0,     0,     0,     0,     0,    82,     0,     0,
      81,     0,     0,    88,     0,     0,    98,     0,    27,   178,
     124,   239,     0,   240,   242,     0,   253,     0,     0,   245,
       0,     0,    29,     0,    66,   102,    63,    30,   282,     0,
       0,     0,   221,   218,   210,     0,    42,    75,    33,    39,
      41,   376,     0,     0,   368,     0,   370,     0,     0,     0,
       0,     0,   378,     8,     0,     0,     0,   195,     0,     0,
       0,     0,     0,   330,   401,   400,   403,   411,   410,   415,
     414,   397,   394,   395,   396,   351,   384,   367,   366,   365,
     352,   388,   399,   393,   391,   402,   392,   390,   382,   387,
     389,   398,   381,   385,   386,   383,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   413,   412,   417,   416,   189,   186,   187,
     188,   192,   193,   194,   418,     0,     0,   334,     0,     0,
       0,     0,     0,     0,     0,   377,   331,   331,    78,   219,
       0,     0,     0,   299,     0,   121,     0,   118,   220,   112,
       0,     0,     0,   306,     0,     0,     0,   312,     0,     0,
      89,    97,     0,     0,   298,     0,   179,     0,   201,     0,
     243,     0,   257,     0,   252,   343,     0,     0,   236,   341,
     235,   364,     0,     0,   300,   102,     0,   225,   343,     0,
     276,   275,   350,   369,   280,   281,     0,     0,    94,   316,
       0,   379,     0,    35,     0,   113,   114,   196,   197,   198,
     131,     0,     0,   227,   226,   228,   230,    43,    50,    51,
      52,    47,    49,    56,    57,    45,    48,    46,    44,    54,
      53,    55,   362,   363,   190,   191,   336,   185,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,   288,   286,   287,   285,     0,   119,   117,     0,     0,
      96,     0,     0,    80,     0,    79,     0,     0,    86,     0,
       0,    84,     0,     0,     0,   100,   185,   177,     0,   266,
     202,   204,   203,   205,     0,   241,   244,     0,   245,     0,
     237,   246,   247,     0,     0,   101,   103,   301,     0,     0,
     222,     0,     0,     0,     0,    95,     0,    36,   231,   201,
     160,   158,   163,   170,   171,   172,   167,   169,   165,   168,
     166,   164,   174,   173,   139,   142,   140,   141,   152,   143,
     156,   148,   146,   159,   147,   145,   150,   155,   157,   144,
     149,   153,   154,   151,   161,   162,   137,   175,   175,   229,
     335,   337,     0,     0,     0,     0,     0,     0,     0,     0,
      77,    76,   122,   120,   114,    83,     0,     0,   305,     0,
     304,     0,     0,   311,    87,     0,   310,    85,    99,   321,
     180,     0,     0,   245,   216,   238,     0,     0,   105,     0,
     106,     0,     0,    92,   318,     0,     0,    90,   317,     0,
     380,    58,     0,   107,   231,   115,     0,   138,     0,   178,
     133,   134,     0,     0,     0,     0,     0,     0,     0,     0,
     231,   309,     0,     0,   315,     0,     0,   266,   269,   270,
     271,     0,   268,   272,   343,   212,   182,   181,     0,     0,
       0,   255,   343,   104,     0,    93,     0,    91,   116,   216,
     137,   175,   175,     0,     0,     0,     0,     0,     0,     0,
     108,     0,     0,     0,     0,   245,   258,     0,   213,   215,
     214,   217,   208,   209,   125,     0,     0,     0,     0,   135,
     136,   176,     0,     0,     0,     0,     0,     0,   308,   307,
     314,   313,   184,   183,   260,   261,   263,   343,     0,   377,
     343,   320,   319,   206,   132,   207,     0,   262,   264,   258,
     265
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -693,  -693,  -693,    -1,  -590,  1537,  -693,  -693,   324,   192,
    -693,  -693,   295,  1858,   526,   258,  -691,  -579,  -693,  -693,
     267,  -693,  -693,    65,  -693,  -693,  -693,   401,   106,  -107,
    -693,  -693,     2,   441,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -143,  -129,  -594,  -693,  -142,     6,  -265,  -693,  -693,
     -25,  -693,  -193,   246,  -693,  -179,  -151,  -693,  -137,  -693,
    -693,  -693,  -156,   272,  -693,  -308,  -577,  -693,  -445,  -329,
    -428,  -692,  -128,   -33,   124,  -693,  -141,  -693,   159,   296,
    -245,  -693,  -693,   779,  -693,    -8,  -147,  -693,    85,  -693,
     800,   821,   -11,  1157,  -693,  1178,  1199,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -261
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   199,    71,   461,    73,    74,    75,   200,
      76,    77,   147,    78,   151,   177,    79,    80,    81,    82,
     546,    83,    84,   482,   596,    85,    86,    87,   356,   608,
      88,   446,   447,   131,   160,   468,    90,    91,   357,   609,
     510,   647,   648,   710,   310,   465,   466,   467,   511,   217,
     584,   804,   774,   188,   769,   736,   739,    92,   186,   332,
      93,    94,   163,   164,   314,   315,   478,   318,   319,   474,
     795,   733,   683,   201,   205,   206,   288,   289,   290,   132,
      96,    97,    98,   133,   100,   119,   120,   426,   427,   101,
     134,   135,   104,   137,   106,   138,   139,   109,   110,   210,
     111,   112,   113,   114,   115
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     117,    70,   487,   282,   469,   136,   425,   123,   142,   304,
     428,   685,     5,   293,   155,   156,   157,   307,   704,   189,
       5,     5,     5,   166,   316,   167,   588,   168,   169,   705,
     323,   567,   173,   174,   178,   179,   208,   320,   183,  -185,
     329,   190,   191,   192,    43,    43,   193,     3,   773,   194,
     195,   196,   316,   197,   711,   417,   190,   207,   209,   316,
     343,   349,   359,   360,   361,   362,   175,   316,   418,   562,
     118,   419,   475,   358,   796,     5,   311,   175,   118,   351,
    -250,   335,   118,    14,   420,   421,   488,   773,   344,   267,
    -250,   268,   422,   434,     5,  -250,   377,   279,  -250,   686,
     336,   686,   423,   321,   803,   798,   737,   808,  -250,   352,
     274,   274,   347,   117,   276,   277,   278,   183,   190,   209,
     587,     5,    42,  -250,   601,   748,   411,   412,   281,   593,
     704,   274,   144,   291,   312,  -248,    51,  -250,   378,   684,
    -250,   705,   161,   161,   578,  -250,  -250,   291,   587,   218,
     184,   330,   335,   594,   118,   317,  -250,   779,   780,   335,
     291,  -185,   586,   587,  -250,   335,   274,   335,   345,   274,
     291,   335,   335,   190,  -250,  -250,   537,   274,   335,   219,
     477,   556,   335,  -250,   158,   593,   220,   483,   793,   435,
     269,   140,   342,   270,   215,   687,   346,   691,   341,   340,
     221,   799,   512,   735,   538,   321,   595,   557,   348,   594,
     560,   216,   363,   364,   365,   366,   368,   370,   371,   372,
     373,   374,   375,   376,   513,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   291,   291,   414,   416,
     534,   749,   690,   451,   321,   576,     5,   576,   170,   437,
     681,   146,   425,   417,   440,   335,   428,  -361,  -267,  -361,
     321,  -251,   226,   514,   535,   190,   418,   453,   455,   419,
     457,   459,   154,   577,   462,   781,   580,  -267,    95,   515,
     485,  -267,   420,   421,  -259,   226,  -251,    95,   121,   479,
     422,   569,   572,   141,   204,   227,    95,   291,   228,   171,
     423,  -251,   505,  -259,   491,  -267,   581,  -259,  -251,    95,
     181,   180,   207,   582,   207,   321,   497,   499,   227,   501,
     502,   228,   475,   583,   190,    95,  -224,  -251,  -324,   602,
     604,  -259,   600,  -323,   516,   321,   222,   233,   741,  -328,
     235,   229,   741,    95,   238,  -224,  -251,    95,  -329,   176,
     233,   807,  -251,   235,   649,   223,   230,   238,   231,   232,
     233,   810,   234,   235,   236,  -254,   237,   238,   239,   240,
     287,   242,   243,   244,   245,  -326,   246,   247,   248,   507,
     271,   249,   250,   251,   287,  -360,   508,  -360,  -325,   441,
    -254,   442,   297,   300,   303,    95,   509,   287,   453,   542,
     457,   545,   462,   547,   499,  -254,   322,   287,   548,   549,
     551,   553,  -254,   291,   273,   274,   327,   734,   283,   559,
     589,   561,   742,    89,   285,   566,   742,   275,  -327,   337,
     292,  -254,    89,   599,   296,    95,  -361,  -256,  -361,   309,
     235,    89,   209,   328,   331,   590,   592,   494,   333,   495,
    -254,    95,   353,   355,    89,   209,  -254,   354,   176,   176,
     176,   306,  -256,   439,   443,   444,   448,   450,   463,   464,
      89,   470,   176,   471,   367,   369,    95,  -256,   226,   476,
     477,   481,   176,   340,  -256,   484,   489,   506,    89,   500,
     533,   539,    89,   287,   287,   176,   532,   555,   573,   653,
     585,   797,   597,  -256,   335,   656,   658,   664,   682,   652,
     800,   227,   655,   695,   228,   226,   413,   415,   699,   438,
     708,   709,  -256,   727,   662,   738,   504,   436,  -256,   148,
     598,   668,   294,   670,   797,   558,   673,   720,   663,   676,
      89,   689,   460,   679,   797,   751,   454,   753,   227,   458,
      95,   228,     5,   233,   287,   234,   235,   146,   149,   752,
     238,   490,   680,   190,   706,   805,   792,   245,  -233,   204,
     694,   204,   698,   480,    95,   700,   251,    95,   778,   765,
      89,    95,     0,   554,     0,   176,   230,     0,   231,   232,
     233,     0,   234,   235,   496,     0,    89,   238,  -233,     0,
       0,     0,   650,     0,   245,  -233,     0,     0,   176,     0,
       0,   249,   250,   251,     0,   150,     0,     0,     0,  -233,
       0,    89,   668,   670,    95,   673,   676,   694,   698,     0,
      95,   226,     0,     0,     0,   721,   722,     0,   723,     0,
     724,   725,     0,     0,   726,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   740,   499,     0,     0,     0,
     499,   744,     0,     0,   227,   746,     0,   228,   552,     0,
     287,     0,     0,     0,     0,     0,     0,   541,   767,   544,
       0,   721,   754,   755,   724,   756,   757,   758,   759,     0,
       0,     0,   563,   565,     0,    89,   568,   571,   179,   183,
     209,   536,   230,     0,   770,   232,   233,   771,   234,   235,
       0,     0,     0,   238,     0,     0,     0,     0,     5,    89,
     245,     0,    89,   146,     0,     0,    89,   249,   250,   251,
     788,   789,   790,   791,  -233,     0,     0,     0,     0,    95,
       0,     0,     0,     0,   698,   801,   802,     0,     0,     0,
       0,   788,   789,   790,   791,   801,   802,     0,   176,   176,
       0,    99,   176,   176,  -233,     0,     0,   806,   499,    89,
      99,  -233,     0,     0,     0,    89,    95,     0,   698,    99,
      95,   150,   102,     0,     0,  -233,   660,   661,     0,     0,
       0,   102,    99,     0,     0,     0,     0,   665,     0,     0,
     102,     0,     0,   103,     0,     0,     0,   674,    99,     0,
     677,     0,   103,   102,     0,   651,     0,     0,     0,     0,
       0,   103,     0,     0,     0,     0,    99,     0,     0,   102,
      99,     0,     0,     0,   103,     0,     0,     0,    95,     0,
      95,     0,     0,    95,     0,     0,    95,   102,     0,    95,
     103,   102,   176,   176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   176,     0,   306,     0,     0,   103,    95,
     306,     0,   103,   176,     0,     0,   176,    95,    99,    95,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,    14,     0,     0,    17,     0,     0,   102,
       0,     0,     0,     0,    22,    23,   701,     0,   732,     0,
       0,     0,    29,     0,  -126,     0,     0,     0,    99,     0,
     103,    89,     0,     0,     0,    89,     0,   743,     0,    40,
       0,  -126,    42,     0,    99,     0,     0,     0,     0,   102,
       0,     0,    95,     0,   702,     0,    51,    95,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,     0,    99,
     103,   768,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,    95,   103,     0,     0,     0,
     102,     0,     0,    89,     0,    89,     0,     0,    89,     0,
       0,    89,   794,   176,    89,    65,     0,     0,     0,     0,
       0,   103,   703,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    89,     0,    89,   794,   306,   306,     0,   306,
     306,     0,     0,    99,     0,   794,     0,    95,    95,    95,
      95,     0,     0,   772,     0,     0,     0,     0,   306,     0,
     306,     0,    95,    95,   102,     0,     0,    99,     0,     0,
      99,     0,     0,     0,    99,   226,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   103,     0,     0,   102,     0,
       0,   102,   772,     0,     0,   102,     0,    89,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,   227,   103,
       0,   228,   103,     0,     0,     0,   103,    99,     0,     0,
       0,     0,     0,    99,    14,     0,    89,    17,     0,     0,
      89,     0,     0,     0,     0,    22,    23,   701,   102,     0,
       0,     0,     0,    29,   102,  -126,   230,     0,     0,   105,
     233,     0,   234,   235,     0,     0,     0,   238,   105,   103,
      40,     0,  -126,    42,   245,   103,     0,   105,     0,     0,
     107,   249,   250,   251,     0,   702,     0,    51,     0,   107,
     105,     0,     0,     0,     0,     0,     0,     0,   107,     0,
       0,   108,    89,    89,    89,    89,   105,     0,     0,     0,
     108,   107,     0,     0,   226,     0,     0,    89,    89,   108,
       0,     0,     0,     0,   105,     0,     0,   107,   105,     0,
       0,     0,   108,     0,     0,     0,    65,     0,     0,     0,
       0,     0,    99,   760,     0,   107,     0,   227,   108,   107,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,   108,     0,     0,    14,
     108,     0,    17,     0,     0,     0,   105,     0,     0,    99,
      22,    23,   701,    99,   103,     0,     0,     0,    29,   233,
    -126,   234,   235,     0,     0,     0,   238,   107,     0,     0,
     102,     0,     0,   245,   102,   211,     0,  -126,    42,     0,
     249,   250,   251,     0,     0,     0,   105,     0,   108,     0,
     702,   103,    51,     0,     0,   103,     0,     0,     0,     0,
       0,     0,   105,     0,     0,     0,     0,   107,     0,     0,
       0,    99,     0,    99,     0,     0,    99,     0,     0,    99,
       0,     0,    99,   107,     0,     0,     0,   105,   108,     0,
       0,     0,   102,     0,   102,     0,     0,   102,     0,     0,
     102,    65,    99,   102,   108,     0,     0,     0,   107,     0,
      99,     0,    99,   103,     0,   103,     0,     0,   103,     0,
       0,   103,     0,   102,   103,     0,     0,     0,     0,   108,
       0,   102,     0,   102,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   103,     0,     0,     0,     0,     0,
       0,     0,   103,     0,   103,     0,     0,     0,     0,     0,
       0,   105,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,     0,     0,
      99,     0,   107,     0,     0,   105,     0,     0,   105,     0,
       0,     0,   105,     0,     0,     0,   102,     0,     0,     0,
       0,   102,     0,   108,    99,     0,   107,     0,    99,   107,
       0,     0,     0,   107,     0,     0,     0,   103,     0,     0,
       0,     0,   103,     0,     0,   102,     0,   108,     0,   102,
     108,     0,     0,     0,   108,   105,     0,     0,     0,     0,
       0,   105,     0,     0,     0,     0,   103,     0,     0,     0,
     103,     0,     0,     0,     0,     0,   107,     0,     0,     0,
       0,     0,   107,     0,     0,     0,     0,     0,     0,    72,
      99,    99,    99,    99,     0,     0,     0,   108,   116,     0,
       0,     0,     0,   108,     0,    99,    99,   143,   417,     0,
       0,   102,   102,   102,   102,     0,     0,     0,     0,     0,
     165,   418,     0,     0,   419,     0,   102,   102,     0,     0,
       0,     0,   103,   103,   103,   103,   182,   420,   421,     0,
       0,     0,   226,     0,     0,   422,     0,   103,   103,     0,
       0,     0,     0,     0,    72,   423,     0,     0,   212,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
     105,     0,     0,     0,     0,   227,     0,     0,   228,     0,
       0,     0,    56,     0,    58,    59,     0,     0,     0,     0,
       0,   107,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,   424,     0,   272,   105,     0,    66,
       0,   105,   108,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,     0,   238,     0,     0,     0,   107,     0,
     244,   245,   107,     0,     0,   248,     0,     0,   249,   250,
     251,     0,     0,     0,     0,     0,   308,     0,     0,   108,
       0,     0,     0,   108,     0,     0,     0,     0,     0,     0,
       0,     0,   326,     0,     0,     0,   226,     0,     0,   105,
       0,   105,     0,     0,   105,     0,     0,   105,     0,     0,
     105,     0,     0,     0,     0,     0,     0,    72,     0,     0,
     107,     0,   107,     0,     0,   107,     0,     0,   107,   227,
     105,   107,   228,     0,     0,     0,     0,     0,   105,     0,
     105,   108,     0,   108,     0,     0,   108,     0,     0,   108,
       0,   107,   108,     0,     0,     0,     0,     0,     0,   107,
       0,   107,     0,     0,     0,     0,     0,   230,     0,   231,
     232,   233,   108,   234,   235,     0,     0,     0,   238,     0,
     108,     0,   108,     0,   244,   245,     0,     0,     0,   248,
       0,    72,   249,   250,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   105,     0,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,   326,     0,     0,   326,     0,
       0,     0,     0,     0,   107,     0,     0,     0,     0,   107,
       0,     0,   105,     0,     0,     0,   105,     0,     0,     0,
       0,     0,     0,     0,     0,   108,     0,     0,     0,     0,
     108,   122,     0,   107,     0,     0,   122,   107,     0,     0,
     145,   122,   122,     0,     0,   498,     0,     0,   159,     0,
     162,    72,     0,     0,   108,   226,     0,     0,   108,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   185,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   105,   105,
     105,   105,     0,     0,     0,     0,     0,     0,   227,     0,
       0,   228,   213,   105,   105,     0,     0,     0,     0,   107,
     107,   107,   107,     0,     0,   214,     0,     0,     0,     0,
       0,     0,     0,     0,   107,   107,     0,     0,     0,     0,
     108,   108,   108,   108,     0,     0,   230,     0,   231,   232,
     233,     0,   234,   235,   236,   108,   108,   238,   239,   240,
       0,   242,   243,   244,   245,     0,     0,     0,   248,     0,
       0,   249,   250,   251,     0,     0,     0,     0,   185,     0,
     575,     0,   286,     0,     0,     0,     0,     0,   185,     0,
     295,     0,     0,     0,     0,     0,   286,     0,     0,   313,
     472,     0,     5,     6,     7,     8,     9,    10,     0,   286,
     124,     0,     0,     0,     0,     0,     0,   605,     0,   286,
       0,    72,     0,    21,     0,   334,     0,     0,   125,   126,
     127,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    36,    37,     0,     0,     0,     0,     0,    39,
       0,    41,     0,     0,     0,     0,   162,   162,   162,   162,
      46,    47,    48,   128,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,     0,     0,     0,    55,     0,   326,
       0,   326,     0,     0,   326,     0,     0,   326,     0,     0,
     678,     0,    56,    57,    58,    59,     0,     0,     0,     0,
      60,     0,     0,     0,     0,   286,   286,     0,     0,    61,
     688,    62,     0,    63,    64,     0,     0,     0,   693,    66,
     697,     0,   129,     0,    68,     0,   473,     0,     0,     0,
     445,     0,   449,     0,   224,     0,     0,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,   162,
       0,     0,     0,     0,     0,     0,   286,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   162,     0,   118,     0,
       0,     0,   229,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   745,     0,     0,     0,   747,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,     0,     0,     0,     0,     0,
      -2,     4,   603,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -231,
      18,    19,     0,    20,    21,     0,    22,    23,    24,    25,
      26,    27,   286,    28,    29,     0,  -126,    30,    31,    32,
      33,    34,    35,    36,    37,     0,    38,     0,     0,  -231,
      39,    40,    41,  -126,    42,   579,  -231,    43,   313,    44,
      45,    46,    47,    48,    49,     0,    50,     0,    51,    52,
    -231,     0,     0,    53,     0,    54,     0,   185,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,   646,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,    62,     0,    63,    64,     0,    65,     0,     0,
      66,     0,     0,    67,     0,    68,     0,    69,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,     4,
       0,     5,     6,     7,     8,     9,    10,     0,  -377,    11,
      12,    13,  -377,    14,    15,    16,    17,  -231,    18,    19,
    -377,    20,    21,  -377,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -126,    30,    31,    32,    33,    34,
      35,    36,    37,   707,    38,     0,     0,  -231,    39,     0,
      41,  -126,    42,  -377,  -231,    43,  -377,    44,    45,    46,
      47,    48,    49,     0,    50,     0,    51,    52,  -231,     0,
       0,    53,     0,    54,     0,     0,  -377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -377,    57,  -377,  -377,  -377,  -377,  -377,  -377,  -377,
       0,  -377,  -377,  -377,  -377,  -377,  -377,  -377,  -377,  -377,
      62,  -377,  -377,  -377,     0,    65,  -377,  -377,  -377,   122,
     185,    67,  -377,    68,     0,    69,   750,     4,     0,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -231,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -126,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,     0,     0,  -231,    39,    40,    41,  -126,
      42,     0,  -231,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -231,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,   187,    65,     0,     0,    66,     0,     0,    67,
     198,    68,     4,    69,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -231,    18,    19,     0,    20,    21,     0,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,  -126,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,     0,     0,
    -231,    39,    40,    41,  -126,    42,     0,  -231,    43,     0,
      44,    45,    46,    47,    48,    49,     0,    50,     0,    51,
      52,  -231,     0,     0,    53,     0,    54,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,    58,    59,     0,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,    62,     0,    63,    64,     0,    65,     0,
       0,    66,     0,     0,    67,   339,    68,     4,    69,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -231,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -126,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,     0,     0,  -231,    39,    40,    41,  -126,
      42,     0,  -231,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -231,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
     198,    68,     4,    69,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -231,    18,    19,     0,    20,    21,     0,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,  -126,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,     0,     0,
    -231,    39,    40,    41,  -126,    42,     0,  -231,    43,     0,
      44,    45,    46,    47,    48,    49,     0,    50,     0,    51,
      52,  -231,     0,     0,    53,     0,    54,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,    58,    59,     0,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,    62,     0,    63,    64,     0,    65,     0,
       0,    66,     0,     0,    67,   503,    68,     4,    69,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -231,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -126,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,     0,     0,  -231,    39,    40,    41,  -126,
      42,     0,  -231,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -231,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
     607,    68,     4,    69,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -231,    18,    19,     0,    20,    21,     0,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,  -126,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,     0,     0,
    -231,    39,   211,    41,  -126,    42,     0,  -231,    43,     0,
      44,    45,    46,    47,    48,    49,     0,    50,     0,    51,
      52,  -231,     0,     0,    53,     0,    54,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,    58,    59,     0,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,    62,     0,    63,    64,     0,    65,     0,
       0,    66,     0,     0,    67,     0,    68,     4,    69,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -231,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -126,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,     0,     0,  -231,    39,     0,    41,  -126,
      42,     0,  -231,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -231,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
       0,    68,     4,    69,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -231,    18,    19,     0,    20,    21,     0,    22,    23,    24,
      25,    26,   452,     0,    28,    29,     0,  -126,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,     0,     0,
    -231,    39,     0,    41,  -126,    42,     0,  -231,    43,     0,
      44,    45,    46,    47,    48,    49,     0,    50,     0,    51,
      52,  -231,     0,     0,    53,     0,    54,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,    58,    59,     0,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,    62,     0,    63,    64,     0,    65,     0,
       0,    66,     0,     0,    67,     0,    68,     4,    69,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -231,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,   456,     0,    28,
      29,     0,  -126,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,     0,     0,  -231,    39,     0,    41,  -126,
      42,     0,  -231,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -231,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
       0,    68,     4,    69,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -231,    18,    19,     0,    20,    21,     0,    22,    23,    24,
      25,    26,   667,     0,    28,    29,     0,  -126,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,     0,     0,
    -231,    39,     0,    41,  -126,    42,     0,  -231,    43,     0,
      44,    45,    46,    47,    48,    49,     0,    50,     0,    51,
      52,  -231,     0,     0,    53,     0,    54,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,    58,    59,     0,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,    62,     0,    63,    64,     0,    65,     0,
       0,    66,     0,     0,    67,     0,    68,     4,    69,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -231,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,   669,     0,    28,
      29,     0,  -126,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,     0,     0,  -231,    39,     0,    41,  -126,
      42,     0,  -231,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -231,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
       0,    68,     4,    69,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -231,    18,    19,     0,    20,    21,     0,    22,    23,    24,
      25,    26,   672,     0,    28,    29,     0,  -126,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,     0,     0,
    -231,    39,     0,    41,  -126,    42,     0,  -231,    43,     0,
      44,    45,    46,    47,    48,    49,     0,    50,     0,    51,
      52,  -231,     0,     0,    53,     0,    54,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,    58,    59,     0,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,    62,     0,    63,    64,     0,    65,     0,
       0,    66,     0,     0,    67,     0,    68,     4,    69,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -231,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,   675,     0,    28,
      29,     0,  -126,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,     0,     0,  -231,    39,     0,    41,  -126,
      42,     0,  -231,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -231,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
       0,    68,     4,    69,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -231,    18,    19,     0,    20,    21,     0,    22,    23,    24,
      25,    26,   692,     0,    28,    29,     0,  -126,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,     0,     0,
    -231,    39,     0,    41,  -126,    42,     0,  -231,    43,     0,
      44,    45,    46,    47,    48,    49,     0,    50,     0,    51,
      52,  -231,     0,     0,    53,     0,    54,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,    58,    59,     0,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,    62,     0,    63,    64,     0,    65,     0,
       0,    66,     0,     0,    67,     0,    68,     4,    69,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -231,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,   696,     0,    28,
      29,     0,  -126,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,     0,     0,  -231,    39,     0,    41,  -126,
      42,     0,  -231,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -231,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
       0,    68,   472,    69,     5,     6,     7,     8,     9,    10,
       0,  -377,   124,     0,     0,  -377,     0,     0,     0,     0,
       0,     0,     0,  -377,     0,    21,     0,     0,     0,     0,
     125,   126,   127,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    36,    37,     0,     0,     0,     0,
       0,    39,     0,    41,     0,     0,  -377,     0,     0,  -377,
       0,     0,    46,    47,    48,   128,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -249,  -377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -377,    57,  -377,  -377,  -377,     0,
    -377,  -377,  -377,     0,  -377,  -377,  -377,  -377,  -377,  -377,
    -377,  -377,  -377,    62,  -377,  -377,  -377,     0,     0,  -377,
    -377,  -377,     0,     0,   129,     0,    68,   472,   486,     5,
       6,     7,     8,     9,    10,     0,  -377,   124,     0,     0,
    -377,     0,     0,     0,     0,     0,     0,     0,  -377,     0,
      21,     0,     0,     0,     0,   125,   126,   127,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    36,
      37,     0,     0,     0,     0,     0,    39,     0,    41,     0,
       0,  -377,     0,     0,  -377,     0,     0,    46,    47,    48,
     128,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -377,
      57,  -377,  -377,  -377,     0,  -377,  -377,  -377,     0,  -377,
    -377,  -377,  -377,  -377,  -377,  -377,  -377,  -377,    62,  -377,
    -377,  -377,     0,     0,  -377,  -377,  -377,     0,     0,   129,
       0,    68,   472,   486,     5,     6,     7,     8,     9,    10,
       0,     0,   124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
     125,   126,   127,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    36,    37,     0,     0,     0,     0,
       0,    39,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,   128,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
       0,     0,     5,     6,     7,     8,     9,    10,     0,     0,
     124,     0,     0,     0,    56,    57,    58,    59,     0,     0,
       0,     0,    60,    21,     0,     0,     0,     0,   125,   126,
     127,    61,    28,    62,     0,    63,    64,    31,    32,     0,
      34,    66,    36,    37,   129,     0,    68,     0,   486,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,   128,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,    57,    58,    59,     0,     0,     0,     0,
      60,     0,     5,     6,     7,     8,     9,    10,     0,    61,
     124,    62,     0,    63,    64,   187,     0,     0,     0,    66,
       0,     0,   129,    21,    68,     0,   130,   766,   125,   126,
     127,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    36,    37,     0,     0,     0,     0,     0,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,   128,     0,     0,   202,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,    57,    58,    59,     0,     0,     0,     0,
      60,   203,     5,     6,     7,     8,     9,    10,     0,    61,
     124,    62,     0,    63,    64,   187,     0,     0,     0,    66,
       0,     0,   129,    21,    68,     0,   130,     0,   125,   126,
     127,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    36,    37,     0,     0,     0,     0,     0,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,   128,     0,     0,   202,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,    57,    58,    59,     0,     0,     0,     0,
      60,     0,     5,     6,     7,     8,     9,    10,     0,    61,
     124,    62,     0,    63,    64,   187,     0,     0,     0,    66,
       0,     0,   129,    21,    68,   493,   130,     0,   125,   126,
     127,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    36,    37,     0,     0,     0,     0,   152,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,   128,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   153,     0,    55,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,   124,     0,
       0,     0,    56,    57,    58,    59,     0,     0,     0,     0,
      60,    21,     0,     0,     0,     0,   125,   126,   127,    61,
      28,    62,     0,    63,    64,    31,    32,     0,    34,    66,
      36,    37,   129,     0,    68,     0,   130,    39,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,   128,     0,     0,   202,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,    57,    58,    59,     0,     0,     0,     0,    60,     0,
       5,     6,     7,     8,     9,    10,     0,    61,   124,    62,
       0,    63,    64,   187,     0,     0,     0,    66,     0,     0,
     129,    21,    68,     0,   130,     0,   125,   126,   127,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      36,    37,     0,     0,     0,     0,     0,    39,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,   128,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,    57,    58,    59,     0,     0,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,    62,
       0,    63,    64,     0,   172,     0,     0,    66,     0,     0,
     129,     0,    68,     0,   130,     5,     6,     7,     8,     9,
      10,     0,     0,   124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,    21,     0,     0,     0,
       0,   125,   126,   127,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    36,    37,     0,     0,     0,
       0,     0,    39,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,   128,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     5,     6,     7,     8,     9,    10,     0,
       0,   124,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,    21,     0,     0,     0,     0,   125,
     126,   127,    61,    28,    62,     0,    63,    64,    31,    32,
       0,    34,    66,    36,    37,    67,     0,    68,     0,   130,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,   128,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     5,     6,     7,     8,     9,    10,     0,
      61,   124,    62,     0,    63,    64,   187,     0,     0,     0,
      66,     0,     0,   129,    21,    68,     0,   130,     0,   125,
     126,   127,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    36,    37,     0,     0,     0,     0,     0,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,   128,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   153,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     5,     6,     7,     8,     9,    10,     0,
      61,   124,    62,     0,    63,    64,     0,     0,     0,     0,
      66,     0,     0,   129,    21,    68,     0,   130,     0,   125,
     126,   127,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    36,    37,   591,     0,     0,     0,     0,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,   128,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,   124,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,   728,     0,     0,     0,     0,   125,   126,   127,
      61,    28,    62,     0,    63,    64,    31,    32,     0,    34,
      66,    36,    37,   129,     0,    68,     0,   130,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,   729,
      47,    48,   730,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     5,     6,     7,     8,     9,    10,     0,    61,   124,
      62,     0,    63,    64,   187,     0,     0,     0,    66,     0,
       0,   129,    21,    68,     0,   731,     0,   125,   126,   127,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    36,    37,     0,     0,     0,     0,     0,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     5,     6,     7,     8,     9,    10,     0,    61,   124,
      62,     0,    63,    64,   187,     0,     0,     0,    66,     0,
       0,   129,    21,    68,     0,   731,     0,   125,   126,   719,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    36,    37,     0,     0,     0,     0,     0,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     5,     6,     7,     8,     9,    10,     0,    61,   124,
      62,     0,    63,    64,   187,     0,     0,     0,    66,     0,
       0,   129,    21,    68,     0,   130,     0,   125,   126,   127,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    36,    37,     0,     0,     0,     0,     0,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      21,     0,     0,     0,     0,   125,   126,   540,    61,    28,
      62,     0,    63,    64,    31,    32,     0,    34,    66,    36,
      37,   129,     0,    68,     0,   130,    39,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     128,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   124,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,    21,     0,
       0,     0,     0,   125,   126,   543,    61,    28,    62,     0,
      63,    64,    31,    32,     0,    34,    66,    36,    37,   129,
       0,    68,     0,   130,    39,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,   128,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     5,     6,     7,     8,     9,
      10,     0,     0,   124,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,    21,     0,     0,     0,
       0,   125,   126,   713,    61,    28,    62,     0,    63,    64,
      31,    32,     0,    34,    66,    36,    37,   129,     0,    68,
       0,   130,    39,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,   128,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     5,     6,     7,     8,     9,    10,     0,
       0,   124,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,    21,     0,     0,     0,     0,   125,
     126,   714,    61,    28,    62,     0,    63,    64,    31,    32,
       0,    34,    66,    36,    37,   129,     0,    68,     0,   130,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,   128,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,   124,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,    21,     0,     0,     0,     0,   125,   126,   716,
      61,    28,    62,     0,    63,    64,    31,    32,     0,    34,
      66,    36,    37,   129,     0,    68,     0,   130,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      21,     0,     0,     0,     0,   125,   126,   717,    61,    28,
      62,     0,    63,    64,    31,    32,     0,    34,    66,    36,
      37,   129,     0,    68,     0,   130,    39,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     128,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   124,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,    21,     0,
       0,     0,     0,   125,   126,   718,    61,    28,    62,     0,
      63,    64,    31,    32,     0,    34,    66,    36,    37,   129,
       0,    68,     0,   130,    39,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,   128,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     5,     6,     7,     8,     9,
      10,     0,     0,   124,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,    21,     0,     0,     0,
       0,   125,   126,   719,    61,    28,    62,     0,    63,    64,
      31,    32,     0,    34,    66,    36,    37,   129,     0,    68,
       0,   130,    39,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,   128,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     5,     6,     7,     8,     9,    10,     0,
       0,   124,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,    21,     0,     0,     0,     0,   125,
     126,   127,    61,    28,    62,     0,    63,    64,    31,    32,
       0,    34,    66,    36,    37,   129,     0,    68,     0,   130,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,   128,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,   124,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,   125,   126,   127,
      61,    28,    62,     0,     0,    64,    31,    32,     0,    34,
      66,    36,    37,   129,     0,    68,     0,   130,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -258,     0,     0,
       0,     0,     0,     0,     0,   224,     0,     0,     0,   225,
       0,     0,    57,     0,     0,     0,  -258,   226,     0,     0,
    -258,     5,     6,     7,     8,     9,    10,     0,     0,   124,
      62,     0,     0,     0,   187,     0,     0,     0,     0,     0,
       0,   129,    21,    68,  -258,   731,     0,   125,   126,   127,
     227,    28,     0,   228,     0,     0,    31,    32,     0,    34,
       0,    36,    37,     0,     0,     0,     0,     0,    39,     0,
      41,     0,     0,   229,     0,     0,     5,     0,     0,    46,
      47,    48,   128,   610,     0,     0,     0,   611,   230,     0,
     231,   232,   233,     0,   234,   235,   236,     0,   237,   238,
     239,   240,   241,   242,   243,   244,   245,     0,   246,   247,
     248,     0,    57,   249,   250,   251,     0,     0,     0,     0,
       0,     0,     0,   659,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,   130,     0,     0,     0,     0,
       0,     0,   612,   613,   614,   615,   616,   617,     0,     0,
     618,   619,   620,   621,   622,   623,   624,   625,   626,   627,
       0,     0,   628,     0,   226,     0,   629,   630,   631,   632,
     633,   634,   635,   636,   637,   638,   639,     0,   640,     0,
       0,   641,   642,   643,   644,   645,   224,     0,     0,     0,
     225,     0,     0,     0,     0,     0,     0,   227,   226,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,   225,     0,     0,     0,     0,     0,
       0,   227,   226,     0,   228,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,     0,
     242,   243,   244,   245,   229,   246,     0,   248,     0,     0,
     249,   250,   251,     0,     0,   227,     0,     0,   228,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   229,   246,
     247,   248,     0,     0,   249,   250,   251,     0,     0,     0,
       0,     0,     0,   230,   775,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   224,   246,   247,   248,   225,     0,   249,   250,
     251,     0,     0,     0,   226,     0,     0,     0,   809,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   227,     0,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   266,  -338,     0,     0,  -360,     0,  -360,   224,     0,
     229,     0,   225,     0,     0,     0,     0,     0,     0,     0,
     226,   301,     0,     0,     0,   230,     0,   231,   232,   233,
     302,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,     0,
     249,   250,   251,   227,     0,     0,   228,     0,   492,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   118,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,   298,     0,     0,
       0,   230,     0,   231,   232,   233,   299,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,     0,   249,   250,   251,   227,
       0,   274,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,   229,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,   570,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,   227,     0,   274,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   118,     0,   224,     0,   229,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,     0,   249,   250,
     251,   227,     0,   274,   228,     0,     0,     0,     0,     0,
       0,   305,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,   229,     0,   225,     0,     0,     0,
       0,     0,     0,     0,   226,   175,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,     0,   249,   250,   251,   227,     0,   274,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
     229,     0,   225,     0,     0,     0,     0,     0,     0,     0,
     226,   564,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,     0,
     249,   250,   251,   227,     0,   274,   228,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,     0,   249,   250,   251,   227,
       0,   274,   228,     0,     0,     0,     0,     0,     0,   666,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,   229,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,   227,     0,   274,   228,     0,
       0,     0,     0,     0,     0,   671,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,   229,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,     0,   249,   250,
     251,   227,     0,   274,   228,     0,     0,     0,     0,     0,
       0,   761,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,   229,     0,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,     0,   249,   250,   251,   227,     0,   274,
     228,     0,     0,     0,     0,     0,     0,   762,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
     229,     0,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,     0,
     249,   250,   251,   227,     0,   274,   228,     0,     0,     0,
       0,     0,     0,   763,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,     0,   249,   250,   251,   227,
       0,   274,   228,     0,     0,     0,     0,     0,     0,   764,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,   229,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,   227,     0,   274,   228,     0,
       0,     0,     0,     0,     0,   776,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,   229,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,     0,   249,   250,
     251,   227,     0,   274,   228,     0,     0,     0,     0,     0,
       0,   777,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,   229,     0,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,     0,   249,   250,   251,   227,     0,   274,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
     229,     0,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,     0,
     249,   250,   251,   227,     0,   325,   228,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,   230,     0,   231,   232,   233,   280,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,     0,   249,   250,   251,   227,
    -339,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   118,     0,
     224,     0,   229,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,   429,     0,     0,     0,   230,     0,   231,
     232,   233,   430,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,   229,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   226,   431,
       0,     0,     0,   230,     0,   231,   232,   233,   432,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,     0,   249,   250,
     251,   227,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,   229,     0,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,     0,   249,   250,   251,   227,     0,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
     229,     0,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,   284,
     249,   250,   251,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,   324,   249,   250,   251,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,   229,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,   338,   249,   250,   251,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,   350,   229,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,     0,   249,   250,
     251,   227,     0,     0,   228,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,   229,     0,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,     0,   249,   250,   251,   227,     0,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
     229,     0,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,   517,
     249,   250,   251,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,   518,   249,   250,   251,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,   229,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,   519,   249,   250,   251,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,   229,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,   520,   249,   250,
     251,   227,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,   229,     0,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,   521,   249,   250,   251,   227,     0,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
     229,     0,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,   522,
     249,   250,   251,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,   523,   249,   250,   251,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,   229,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,   524,   249,   250,   251,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,   229,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,   525,   249,   250,
     251,   227,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,   229,     0,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,   526,   249,   250,   251,   227,     0,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
     229,     0,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,   527,
     249,   250,   251,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,   528,   249,   250,   251,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,   229,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,   529,   249,   250,   251,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,   229,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,   530,   249,   250,
     251,   227,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,   229,     0,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,   531,   249,   250,   251,   227,     0,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   118,     0,   224,     0,
     229,     0,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,     0,
     249,   250,   251,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,   574,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,   550,   249,   250,   251,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,   229,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,   606,   229,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   226,   654,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,     0,   249,   250,
     251,   227,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,   229,     0,   225,     0,     0,     0,
       0,     0,     0,     0,   226,   657,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,     0,   249,   250,   251,   227,     0,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
     229,     0,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,     0,
     249,   250,   251,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,   712,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,     0,   249,   250,   251,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,   715,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,   229,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,   782,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,   229,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,     0,   249,   250,
     251,   227,     0,     0,   228,     0,     0,     0,     0,     0,
       0,   783,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,   229,     0,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,     0,   249,   250,   251,   227,     0,     0,
     228,     0,     0,     0,     0,     0,     0,   784,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
     229,     0,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,     0,
     249,   250,   251,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,   785,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,     0,   249,   250,   251,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,   786,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,   229,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,   787,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,   229,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,     0,   249,   250,
     251,   227,   226,     0,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   227,     0,     0,   228,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   229,   246,
     247,   248,     0,     0,   249,   250,   251,     0,     0,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,     0,   242,   243,
     244,   245,     0,   246,     0,   248,     0,     0,   249,   250,
     251
};

static const yytype_int16 yycheck[] =
{
      11,     2,   331,   140,   312,    16,   271,    15,    19,   156,
     271,   588,     3,   150,    25,    26,    27,   158,   608,    52,
       3,     3,     3,    34,     1,    36,   471,    38,    39,   608,
     171,    23,    43,    44,    45,    46,    69,    32,    49,     3,
     181,    52,    53,    54,    57,    57,    57,     0,   739,    60,
      61,    62,     1,    64,   648,    19,    67,    68,    69,     1,
      98,   208,   218,   219,   220,   221,    23,     1,    32,    23,
      74,    35,   317,    56,   766,     3,    67,    23,    74,    98,
      57,    98,    74,    15,    48,    49,   331,   778,   126,   125,
      32,   127,    56,    32,     3,    72,    24,   130,    32,    32,
     117,    32,    66,    98,   117,    32,   683,   799,    57,   128,
     123,   123,    32,   124,   125,   126,   127,   128,   129,   130,
      97,     3,    54,    72,   128,   704,   267,   268,   136,    47,
     720,   123,   125,   144,   125,    77,    68,    79,    66,   584,
     117,   720,   125,   125,   125,    79,   123,   158,    97,    19,
      59,   184,    98,    71,    74,    97,    98,   751,   752,    98,
     171,   125,   470,    97,    98,    98,   123,    98,    98,   123,
     181,    98,    98,   184,   123,   117,    98,   123,    98,    49,
      79,    98,    98,   117,   125,    47,    56,   328,   765,   128,
      55,    66,   203,    58,    36,   128,   126,   128,   124,   200,
      70,   128,   358,   102,   126,    98,   124,   124,   128,    71,
     126,    53,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   117,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
      32,   706,   124,   296,    98,    98,     3,    98,     7,   280,
     578,     8,   537,    19,   285,    98,   537,   125,    79,   127,
      98,    32,    22,   117,    56,   296,    32,   298,   299,    35,
     301,   302,    25,   126,   305,   126,    19,    98,     2,   117,
     123,   102,    48,    49,    79,    22,    57,    11,    13,   320,
      56,   458,   459,    18,    68,    55,    20,   328,    58,   125,
      66,    72,   355,    98,   335,   126,    49,   102,    79,    33,
     125,    63,   343,    56,   345,    98,   347,   348,    55,   350,
     351,    58,   587,    66,   355,    49,    98,    98,   117,   496,
     497,   126,   489,   117,   117,    98,   117,    97,   687,   117,
     100,    78,   691,    67,   104,   117,   117,    71,   117,    45,
      97,   799,   123,   100,   117,   122,    93,   104,    95,    96,
      97,   809,    99,   100,   101,    32,   103,   104,   105,   106,
     144,   108,   109,   110,   111,   117,   113,   114,   115,    49,
     125,   118,   119,   120,   158,   125,    56,   127,   117,    77,
      57,    79,   154,   155,   156,   119,    66,   171,   429,   430,
     431,   432,   433,   434,   435,    72,   168,   181,   436,   437,
     441,   442,    79,   444,   117,   123,   178,   682,   117,   450,
     473,   452,   687,     2,    73,   456,   691,   123,   117,   191,
     123,    98,    11,   486,   125,   159,   125,    32,   127,   125,
     100,    20,   473,   125,    79,   476,   477,   343,   117,   345,
     117,   175,     7,    97,    33,   486,   123,   123,   154,   155,
     156,   157,    57,   117,   126,    98,   117,    32,    25,   126,
      49,    98,   168,   126,   227,   228,   200,    72,    22,    77,
      79,   126,   178,   504,    79,   126,    98,   123,    67,   128,
     128,    55,    71,   267,   268,   191,   126,    79,    25,    23,
     126,   766,   126,    98,    98,    23,   128,   123,    97,   540,
     775,    55,   543,   128,    58,    22,   269,   270,   128,   281,
     100,   125,   117,   126,   555,    72,   354,   280,   123,    23,
     485,   562,   151,   564,   799,   449,   567,   664,   556,   570,
     119,   594,   304,   574,   809,   708,   299,   709,    55,   302,
     274,    58,     3,    97,   328,    99,   100,     8,     9,   708,
     104,   335,   576,   594,   609,   778,   765,   111,    19,   343,
     601,   345,   603,   321,   298,   606,   120,   301,   749,   727,
     159,   305,    -1,   444,    -1,   281,    93,    -1,    95,    96,
      97,    -1,    99,   100,   347,    -1,   175,   104,    49,    -1,
      -1,    -1,   537,    -1,   111,    56,    -1,    -1,   304,    -1,
      -1,   118,   119,   120,    -1,    66,    -1,    -1,    -1,    70,
      -1,   200,   653,   654,   348,   656,   657,   658,   659,    -1,
     354,    22,    -1,    -1,    -1,   666,   667,    -1,   669,    -1,
     671,   672,    -1,    -1,   675,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   686,   687,    -1,    -1,    -1,
     691,   692,    -1,    -1,    55,   696,    -1,    58,   442,    -1,
     444,    -1,    -1,    -1,    -1,    -1,    -1,   430,   731,   432,
      -1,   712,   713,   714,   715,   716,   717,   718,   719,    -1,
      -1,    -1,   454,   455,    -1,   274,   458,   459,   729,   730,
     731,   425,    93,    -1,   735,    96,    97,   738,    99,   100,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,     3,   298,
     111,    -1,   301,     8,    -1,    -1,   305,   118,   119,   120,
     761,   762,   763,   764,    19,    -1,    -1,    -1,    -1,   463,
      -1,    -1,    -1,    -1,   775,   776,   777,    -1,    -1,    -1,
      -1,   782,   783,   784,   785,   786,   787,    -1,   454,   455,
      -1,     2,   458,   459,    49,    -1,    -1,   798,   799,   348,
      11,    56,    -1,    -1,    -1,   354,   500,    -1,   809,    20,
     504,    66,     2,    -1,    -1,    70,   548,   549,    -1,    -1,
      -1,    11,    33,    -1,    -1,    -1,    -1,   559,    -1,    -1,
      20,    -1,    -1,     2,    -1,    -1,    -1,   569,    49,    -1,
     572,    -1,    11,    33,    -1,   539,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    67,    -1,    -1,    49,
      71,    -1,    -1,    -1,    33,    -1,    -1,    -1,   562,    -1,
     564,    -1,    -1,   567,    -1,    -1,   570,    67,    -1,   573,
      49,    71,   548,   549,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   559,    -1,   561,    -1,    -1,    67,   593,
     566,    -1,    71,   569,    -1,    -1,   572,   601,   119,   603,
      -1,    -1,    -1,    -1,   463,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    18,    -1,    -1,   119,
      -1,    -1,    -1,    -1,    26,    27,    28,    -1,   682,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,   159,    -1,
     119,   500,    -1,    -1,    -1,   504,    -1,   689,    -1,    51,
      -1,    53,    54,    -1,   175,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   666,    -1,    66,    -1,    68,   671,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,   200,
     159,   735,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   695,    -1,    -1,    -1,   699,   175,    -1,    -1,    -1,
     200,    -1,    -1,   562,    -1,   564,    -1,    -1,   567,    -1,
      -1,   570,   766,   689,   573,   117,    -1,    -1,    -1,    -1,
      -1,   200,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   593,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   601,    -1,   603,   799,   722,   723,    -1,   725,
     726,    -1,    -1,   274,    -1,   809,    -1,   761,   762,   763,
     764,    -1,    -1,   739,    -1,    -1,    -1,    -1,   744,    -1,
     746,    -1,   776,   777,   274,    -1,    -1,   298,    -1,    -1,
     301,    -1,    -1,    -1,   305,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,   298,    -1,
      -1,   301,   778,    -1,    -1,   305,    -1,   666,    -1,    -1,
      -1,    -1,   671,    -1,    -1,    -1,    -1,    -1,    55,   298,
      -1,    58,   301,    -1,    -1,    -1,   305,   348,    -1,    -1,
      -1,    -1,    -1,   354,    15,    -1,   695,    18,    -1,    -1,
     699,    -1,    -1,    -1,    -1,    26,    27,    28,   348,    -1,
      -1,    -1,    -1,    34,   354,    36,    93,    -1,    -1,     2,
      97,    -1,    99,   100,    -1,    -1,    -1,   104,    11,   348,
      51,    -1,    53,    54,   111,   354,    -1,    20,    -1,    -1,
       2,   118,   119,   120,    -1,    66,    -1,    68,    -1,    11,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,     2,   761,   762,   763,   764,    49,    -1,    -1,    -1,
      11,    33,    -1,    -1,    22,    -1,    -1,   776,   777,    20,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    49,    71,    -1,
      -1,    -1,    33,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,   463,   124,    -1,    67,    -1,    55,    49,    71,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   463,    -1,    -1,    67,    -1,    -1,    15,
      71,    -1,    18,    -1,    -1,    -1,   119,    -1,    -1,   500,
      26,    27,    28,   504,   463,    -1,    -1,    -1,    34,    97,
      36,    99,   100,    -1,    -1,    -1,   104,   119,    -1,    -1,
     500,    -1,    -1,   111,   504,    51,    -1,    53,    54,    -1,
     118,   119,   120,    -1,    -1,    -1,   159,    -1,   119,    -1,
      66,   500,    68,    -1,    -1,   504,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,    -1,    -1,   159,    -1,    -1,
      -1,   562,    -1,   564,    -1,    -1,   567,    -1,    -1,   570,
      -1,    -1,   573,   175,    -1,    -1,    -1,   200,   159,    -1,
      -1,    -1,   562,    -1,   564,    -1,    -1,   567,    -1,    -1,
     570,   117,   593,   573,   175,    -1,    -1,    -1,   200,    -1,
     601,    -1,   603,   562,    -1,   564,    -1,    -1,   567,    -1,
      -1,   570,    -1,   593,   573,    -1,    -1,    -1,    -1,   200,
      -1,   601,    -1,   603,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   593,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   601,    -1,   603,    -1,    -1,    -1,    -1,    -1,
      -1,   274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   666,    -1,    -1,    -1,    -1,
     671,    -1,   274,    -1,    -1,   298,    -1,    -1,   301,    -1,
      -1,    -1,   305,    -1,    -1,    -1,   666,    -1,    -1,    -1,
      -1,   671,    -1,   274,   695,    -1,   298,    -1,   699,   301,
      -1,    -1,    -1,   305,    -1,    -1,    -1,   666,    -1,    -1,
      -1,    -1,   671,    -1,    -1,   695,    -1,   298,    -1,   699,
     301,    -1,    -1,    -1,   305,   348,    -1,    -1,    -1,    -1,
      -1,   354,    -1,    -1,    -1,    -1,   695,    -1,    -1,    -1,
     699,    -1,    -1,    -1,    -1,    -1,   348,    -1,    -1,    -1,
      -1,    -1,   354,    -1,    -1,    -1,    -1,    -1,    -1,     2,
     761,   762,   763,   764,    -1,    -1,    -1,   348,    11,    -1,
      -1,    -1,    -1,   354,    -1,   776,   777,    20,    19,    -1,
      -1,   761,   762,   763,   764,    -1,    -1,    -1,    -1,    -1,
      33,    32,    -1,    -1,    35,    -1,   776,   777,    -1,    -1,
      -1,    -1,   761,   762,   763,   764,    49,    48,    49,    -1,
      -1,    -1,    22,    -1,    -1,    56,    -1,   776,   777,    -1,
      -1,    -1,    -1,    -1,    67,    66,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
     463,    -1,    -1,    -1,    -1,    55,    -1,    -1,    58,    -1,
      -1,    -1,    93,    -1,    95,    96,    -1,    -1,    -1,    -1,
      -1,   463,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,    -1,   119,   500,    -1,   120,
      -1,   504,   463,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,    -1,   104,    -1,    -1,    -1,   500,    -1,
     110,   111,   504,    -1,    -1,   115,    -1,    -1,   118,   119,
     120,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   500,
      -1,    -1,    -1,   504,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,    -1,    22,    -1,    -1,   562,
      -1,   564,    -1,    -1,   567,    -1,    -1,   570,    -1,    -1,
     573,    -1,    -1,    -1,    -1,    -1,    -1,   200,    -1,    -1,
     562,    -1,   564,    -1,    -1,   567,    -1,    -1,   570,    55,
     593,   573,    58,    -1,    -1,    -1,    -1,    -1,   601,    -1,
     603,   562,    -1,   564,    -1,    -1,   567,    -1,    -1,   570,
      -1,   593,   573,    -1,    -1,    -1,    -1,    -1,    -1,   601,
      -1,   603,    -1,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,   593,    99,   100,    -1,    -1,    -1,   104,    -1,
     601,    -1,   603,    -1,   110,   111,    -1,    -1,    -1,   115,
      -1,   274,   118,   119,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   666,    -1,    -1,    -1,    -1,   671,    -1,
      -1,    -1,    -1,    -1,    -1,   298,    -1,    -1,   301,    -1,
      -1,    -1,    -1,    -1,   666,    -1,    -1,    -1,    -1,   671,
      -1,    -1,   695,    -1,    -1,    -1,   699,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   666,    -1,    -1,    -1,    -1,
     671,    13,    -1,   695,    -1,    -1,    18,   699,    -1,    -1,
      22,    23,    24,    -1,    -1,   348,    -1,    -1,    30,    -1,
      32,   354,    -1,    -1,   695,    22,    -1,    -1,   699,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   761,   762,
     763,   764,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      -1,    58,    74,   776,   777,    -1,    -1,    -1,    -1,   761,
     762,   763,   764,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   776,   777,    -1,    -1,    -1,    -1,
     761,   762,   763,   764,    -1,    -1,    93,    -1,    95,    96,
      97,    -1,    99,   100,   101,   776,   777,   104,   105,   106,
      -1,   108,   109,   110,   111,    -1,    -1,    -1,   115,    -1,
      -1,   118,   119,   120,    -1,    -1,    -1,    -1,   140,    -1,
     463,    -1,   144,    -1,    -1,    -1,    -1,    -1,   150,    -1,
     152,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,   171,
      11,    -1,    -1,    -1,    -1,    -1,    -1,   500,    -1,   181,
      -1,   504,    -1,    24,    -1,   187,    -1,    -1,    29,    30,
      31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    52,    -1,    -1,    -1,    -1,   218,   219,   220,   221,
      61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   235,    -1,    -1,    -1,    78,    -1,   562,
      -1,   564,    -1,    -1,   567,    -1,    -1,   570,    -1,    -1,
     573,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,   267,   268,    -1,    -1,   110,
     593,   112,    -1,   114,   115,    -1,    -1,    -1,   601,   120,
     603,    -1,   123,    -1,   125,    -1,   127,    -1,    -1,    -1,
     292,    -1,   294,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
     312,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   321,
      -1,    -1,    -1,    -1,    -1,    -1,   328,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,    74,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   695,    -1,    -1,    -1,   699,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    -1,    -1,    -1,    -1,    -1,
       0,     1,   128,     3,     4,     5,     6,     7,     8,    -1,
      -1,    11,    12,    13,    -1,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    24,    -1,    26,    27,    28,    29,
      30,    31,   444,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    46,    -1,    -1,    49,
      50,    51,    52,    53,    54,   467,    56,    57,   470,    59,
      60,    61,    62,    63,    64,    -1,    66,    -1,    68,    69,
      70,    -1,    -1,    73,    -1,    75,    -1,   489,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,   511,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,   112,    -1,   114,   115,    -1,   117,    -1,    -1,
     120,    -1,    -1,   123,    -1,   125,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   556,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   578,    -1,    -1,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,   625,    46,    -1,    -1,    49,    50,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    66,    -1,    68,    69,    70,    -1,
      -1,    73,    -1,    75,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,   117,   118,   119,   120,   701,
     702,   123,   124,   125,    -1,   127,   708,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    -1,    -1,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    -1,    66,    -1,    68,    69,    70,    -1,    -1,    73,
      -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,    -1,
     114,   115,   116,   117,    -1,    -1,   120,    -1,    -1,   123,
     124,   125,     1,   127,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    12,    13,    -1,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    -1,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    -1,    66,    -1,    68,
      69,    70,    -1,    -1,    73,    -1,    75,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,   123,   124,   125,     1,   127,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    -1,    -1,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    -1,    66,    -1,    68,    69,    70,    -1,    -1,    73,
      -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,    -1,
     114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,   123,
     124,   125,     1,   127,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    12,    13,    -1,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    -1,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    -1,    66,    -1,    68,
      69,    70,    -1,    -1,    73,    -1,    75,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,   123,   124,   125,     1,   127,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    -1,    -1,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    -1,    66,    -1,    68,    69,    70,    -1,    -1,    73,
      -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,    -1,
     114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,   123,
     124,   125,     1,   127,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    12,    13,    -1,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    -1,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    -1,    66,    -1,    68,
      69,    70,    -1,    -1,    73,    -1,    75,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,   123,    -1,   125,     1,   127,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    -1,    66,    -1,    68,    69,    70,    -1,    -1,    73,
      -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,    -1,
     114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,   123,
      -1,   125,     1,   127,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    12,    13,    -1,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    -1,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    -1,    66,    -1,    68,
      69,    70,    -1,    -1,    73,    -1,    75,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,   123,    -1,   125,     1,   127,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    -1,    66,    -1,    68,    69,    70,    -1,    -1,    73,
      -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,    -1,
     114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,   123,
      -1,   125,     1,   127,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    12,    13,    -1,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    -1,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    -1,    66,    -1,    68,
      69,    70,    -1,    -1,    73,    -1,    75,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,   123,    -1,   125,     1,   127,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    -1,    66,    -1,    68,    69,    70,    -1,    -1,    73,
      -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,    -1,
     114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,   123,
      -1,   125,     1,   127,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    12,    13,    -1,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    -1,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    -1,    66,    -1,    68,
      69,    70,    -1,    -1,    73,    -1,    75,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,   123,    -1,   125,     1,   127,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    -1,    66,    -1,    68,    69,    70,    -1,    -1,    73,
      -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,    -1,
     114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,   123,
      -1,   125,     1,   127,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    12,    13,    -1,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    -1,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    -1,    66,    -1,    68,
      69,    70,    -1,    -1,    73,    -1,    75,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,    -1,
      -1,   120,    -1,    -1,   123,    -1,   125,     1,   127,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    -1,    66,    -1,    68,    69,    70,    -1,    -1,    73,
      -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,    -1,
     114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,   123,
      -1,   125,     1,   127,     3,     4,     5,     6,     7,     8,
      -1,    10,    11,    -1,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,
      39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      -1,    -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    -1,
      99,   100,   101,    -1,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,    -1,   118,
     119,   120,    -1,    -1,   123,    -1,   125,     1,   127,     3,
       4,     5,     6,     7,     8,    -1,    10,    11,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      24,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,    33,
      -1,    -1,    -1,    -1,    38,    39,    -1,    41,    -1,    43,
      44,    -1,    -1,    -1,    -1,    -1,    50,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    -1,    -1,    61,    62,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,    -1,    -1,   118,   119,   120,    -1,    -1,   123,
      -1,   125,     1,   127,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,
      39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,
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
      -1,    -1,   123,    24,   125,    -1,   127,   128,    29,    30,
      31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,   102,     3,     4,     5,     6,     7,     8,    -1,   110,
      11,   112,    -1,   114,   115,   116,    -1,    -1,    -1,   120,
      -1,    -1,   123,    24,   125,    -1,   127,    -1,    29,    30,
      31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,     3,     4,     5,     6,     7,     8,    -1,   110,
      11,   112,    -1,   114,   115,   116,    -1,    -1,    -1,   120,
      -1,    -1,   123,    24,   125,   126,   127,    -1,    29,    30,
      31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    49,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    11,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    24,    -1,    -1,    -1,    -1,    29,    30,    31,   110,
      33,   112,    -1,   114,   115,    38,    39,    -1,    41,   120,
      43,    44,   123,    -1,   125,    -1,   127,    50,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,
       3,     4,     5,     6,     7,     8,    -1,   110,    11,   112,
      -1,   114,   115,   116,    -1,    -1,    -1,   120,    -1,    -1,
     123,    24,   125,    -1,   127,    -1,    29,    30,    31,    -1,
      33,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    50,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
      -1,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
     123,    -1,   125,    -1,   127,     3,     4,     5,     6,     7,
       8,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    24,    -1,    -1,    -1,
      -1,    29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,
      38,    39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,
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
     120,    -1,    -1,   123,    24,   125,    -1,   127,    -1,    29,
      30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,
      -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      -1,   101,    -1,     3,     4,     5,     6,     7,     8,    -1,
     110,    11,   112,    -1,   114,   115,    -1,    -1,    -1,    -1,
     120,    -1,    -1,   123,    24,   125,    -1,   127,    -1,    29,
      30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,
      -1,    41,    -1,    43,    44,    45,    -1,    -1,    -1,    -1,
      50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      -1,   101,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
     110,    33,   112,    -1,   114,   115,    38,    39,    -1,    41,
     120,    43,    44,   123,    -1,   125,    -1,   127,    50,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,     3,     4,     5,     6,     7,     8,    -1,   110,    11,
     112,    -1,   114,   115,   116,    -1,    -1,    -1,   120,    -1,
      -1,   123,    24,   125,    -1,   127,    -1,    29,    30,    31,
      -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,
      -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    -1,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,
      24,    -1,    -1,    -1,    -1,    29,    30,    31,   110,    33,
     112,    -1,   114,   115,    38,    39,    -1,    41,   120,    43,
      44,   123,    -1,   125,    -1,   127,    50,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   110,    33,   112,    -1,
     114,   115,    38,    39,    -1,    41,   120,    43,    44,   123,
      -1,   125,    -1,   127,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,     3,     4,     5,     6,     7,
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
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      -1,   101,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
     110,    33,   112,    -1,   114,   115,    38,    39,    -1,    41,
     120,    43,    44,   123,    -1,   125,    -1,   127,    50,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    -1,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,
      24,    -1,    -1,    -1,    -1,    29,    30,    31,   110,    33,
     112,    -1,   114,   115,    38,    39,    -1,    41,   120,    43,
      44,   123,    -1,   125,    -1,   127,    50,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   110,    33,   112,    -1,
     114,   115,    38,    39,    -1,    41,   120,    43,    44,   123,
      -1,   125,    -1,   127,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,     3,     4,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
     110,    33,   112,    -1,    -1,   115,    38,    39,    -1,    41,
     120,    43,    44,   123,    -1,   125,    -1,   127,    50,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,
      -1,    -1,    94,    -1,    -1,    -1,    98,    22,    -1,    -1,
     102,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
     112,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    24,   125,   126,   127,    -1,    29,    30,    31,
      55,    33,    -1,    58,    -1,    -1,    38,    39,    -1,    41,
      -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      52,    -1,    -1,    78,    -1,    -1,     3,    -1,    -1,    61,
      62,    63,    64,    10,    -1,    -1,    -1,    14,    93,    -1,
      95,    96,    97,    -1,    99,   100,   101,    -1,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,    -1,    94,   118,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      -1,    -1,    99,    -1,    22,    -1,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,   115,    -1,
      -1,   118,   119,   120,   121,   122,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    55,    22,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    22,    -1,    58,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,    -1,
     108,   109,   110,   111,    78,   113,    -1,   115,    -1,    -1,
     118,   119,   120,    -1,    -1,    55,    -1,    -1,    58,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    78,   113,
     114,   115,    -1,    -1,   118,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    93,   128,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    10,   113,   114,   115,    14,    -1,   118,   119,
     120,    -1,    -1,    -1,    22,    -1,    -1,    -1,   128,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,   122,    -1,    -1,   125,    -1,   127,    10,    -1,
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
      -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    58,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    78,   113,
     114,   115,    -1,    -1,   118,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,    -1,   108,   109,
     110,   111,    -1,   113,    -1,   115,    -1,    -1,   118,   119,
     120
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   133,   134,     0,     1,     3,     4,     5,     6,     7,
       8,    11,    12,    13,    15,    16,    17,    18,    20,    21,
      23,    24,    26,    27,    28,    29,    30,    31,    33,    34,
      37,    38,    39,    40,    41,    42,    43,    44,    46,    50,
      51,    52,    54,    57,    59,    60,    61,    62,    63,    64,
      66,    68,    69,    73,    75,    78,    93,    94,    95,    96,
     101,   110,   112,   114,   115,   117,   120,   123,   125,   127,
     135,   136,   137,   138,   139,   140,   142,   143,   145,   148,
     149,   150,   151,   153,   154,   157,   158,   159,   162,   165,
     168,   169,   189,   192,   193,   211,   212,   213,   214,   215,
     216,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   232,   233,   234,   235,   236,   137,   224,    74,   217,
     218,   144,   145,   217,    11,    29,    30,    31,    64,   123,
     127,   165,   211,   215,   222,   223,   224,   225,   227,   228,
      66,   144,   224,   137,   125,   145,     8,   144,   146,     9,
      66,   146,    49,    76,   152,   224,   224,   224,   125,   145,
     166,   125,   145,   194,   195,   137,   224,   224,   224,   224,
       7,   125,   117,   224,   224,    23,   140,   147,   224,   224,
      63,   125,   137,   224,    59,   145,   190,   116,   185,   205,
     224,   224,   224,   224,   224,   224,   224,   224,   124,   135,
     141,   205,    67,   102,   185,   206,   207,   224,   205,   224,
     231,    51,   137,   145,   145,    36,    53,   181,    19,    49,
      56,    70,   117,   122,    10,    14,    22,    55,    58,    78,
      93,    95,    96,    97,    99,   100,   101,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   113,   114,   115,   118,
     119,   120,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,   121,   125,   127,    55,
      58,   125,   137,   117,   123,   140,   224,   224,   224,   205,
      32,   217,   190,   117,   117,    73,   145,   185,   208,   209,
     210,   224,   123,   190,   159,   145,   125,   147,    23,    32,
     147,    23,    32,   147,   218,    65,   140,   208,   137,   125,
     176,    67,   125,   145,   196,   197,     1,    97,   199,   200,
      32,    98,   147,   208,   117,   123,   137,   147,   125,   208,
     205,    79,   191,   117,   145,    98,   117,   147,   117,   124,
     135,   124,   224,    98,   126,    98,   126,    32,   128,   218,
      77,    98,   128,     7,   123,    97,   160,   170,    56,   194,
     194,   194,   194,   224,   224,   224,   224,   152,   224,   152,
     224,   224,   224,   224,   224,   224,   224,    24,    66,   145,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   208,   208,   152,   224,   152,   224,    19,    32,    35,
      48,    49,    56,    66,   115,   179,   219,   220,   236,    23,
      32,    23,    32,    65,    32,   128,   152,   224,   147,   117,
     224,    77,    79,   126,    98,   145,   163,   164,   117,   145,
      32,   205,    31,   224,   152,   224,    31,   224,   152,   224,
     147,   137,   224,    25,   126,   177,   178,   179,   167,   197,
      98,   126,     1,   127,   201,   212,    77,    79,   198,   224,
     195,   126,   155,   208,   126,   123,   127,   201,   212,    98,
     185,   224,   126,   126,   206,   206,   152,   224,   137,   224,
     128,   224,   224,   124,   141,   205,   123,    49,    56,    66,
     172,   180,   194,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   126,   128,    32,    56,   211,    98,   126,    55,
      31,   152,   224,    31,   152,   224,   152,   224,   217,   217,
     117,   224,   185,   224,   210,    79,    98,   124,   160,   224,
     126,   224,    23,   147,    23,   147,   224,    23,   147,   218,
      23,   147,   218,    25,    25,   137,    98,   126,   125,   145,
      19,    49,    56,    66,   182,   126,   197,    97,   200,   205,
     224,    45,   224,    47,    71,   124,   156,   126,   155,   205,
     190,   128,   218,   128,   218,   137,    77,   124,   161,   171,
      10,    14,    79,    80,    81,    82,    83,    84,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    99,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     115,   118,   119,   120,   121,   122,   145,   173,   174,   117,
     220,   211,   224,    23,    23,   224,    23,    23,   128,   128,
     147,   147,   224,   164,   123,   147,    65,    31,   224,    31,
     224,    65,    31,   224,   147,    31,   224,   147,   137,   224,
     178,   197,    97,   204,   200,   198,    32,   128,   137,   205,
     124,   128,    31,   137,   224,   128,    31,   137,   224,   128,
     224,    28,    66,   124,   136,   149,   182,   145,   100,   125,
     175,   175,    65,    31,    31,    65,    31,    31,    31,    31,
     161,   224,   224,   224,   224,   224,   224,   126,    24,    61,
      64,   127,   185,   203,   212,   102,   187,   198,    72,   188,
     224,   201,   212,   147,   224,   137,   224,   137,   149,   200,
     145,   173,   174,   177,   224,   224,   224,   224,   224,   224,
     124,    65,    65,    65,    65,   204,   128,   205,   185,   186,
     224,   224,   140,   148,   184,   128,    65,    65,   188,   175,
     175,   126,    65,    65,    65,    65,    65,    65,   224,   224,
     224,   224,   187,   198,   185,   202,   203,   212,    32,   128,
     212,   224,   224,   117,   183,   184,   224,   202,   203,   128,
     202
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
     145,   146,   146,   147,   147,   148,   148,   149,   149,   149,
     149,   149,   149,   150,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   152,   153,   153,   153,
     153,   154,   155,   155,   156,   156,   157,   158,   158,   159,
     159,   159,   160,   160,   161,   161,   161,   162,   163,   163,
     163,   164,   164,   166,   167,   165,   168,   168,   168,   168,
     170,   171,   169,   172,   172,   172,   172,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   175,   175,   176,   177,   177,
     177,   178,   178,   178,   178,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   180,   180,   180,   180,   181,
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
       1,     1,     1,     2,     1,     2,     3,     1,     1,     1,
       1,     1,     1,     2,     5,     3,     6,     6,     4,     5,
       5,     3,     3,     6,     5,     6,     5,     6,     3,     4,
       6,     7,     6,     7,     4,     5,     4,     4,     3,     6,
       5,     5,     0,     2,     3,     2,     6,     6,     8,     1,
       1,     1,     0,     2,     0,     2,     3,     5,     1,     2,
       3,     1,     3,     0,     0,     8,     0,     1,     2,     2,
       0,     0,     9,     3,     3,     5,     5,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     3,     3,     0,     1,
       3,     4,     4,     6,     6,     0,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     0,     1,     1,     1,     1,
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
#line 4313 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 406 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4319 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 407 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4325 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 414 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4331 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 419 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4337 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 420 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4343 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 436 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4349 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 437 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4355 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 438 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4361 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 439 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4367 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 440 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4373 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 441 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4379 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 442 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4385 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 443 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4391 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 444 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4397 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 445 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4403 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 446 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4409 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 447 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4415 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 448 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4421 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 449 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4427 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 450 "chapel.ypp" /* yacc.c:1661  */
    { printf("syntax error"); clean_exit(1); }
#line 4433 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 455 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yylsp[-3]).comment))); }
#line 4439 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 457 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yylsp[-4]).comment))); }
#line 4445 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 461 "chapel.ypp" /* yacc.c:1661  */
    { (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4451 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 4457 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 477 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 4463 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4469 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 483 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4475 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 4481 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 4487 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 4493 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 4499 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 495 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 4505 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 4511 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 4517 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 4523 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 4529 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 500 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 4535 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 501 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 4541 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 502 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 4547 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 4553 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 504 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 4559 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 4565 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 506 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 4571 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 4577 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4583 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4589 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 520 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4595 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 524 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4601 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 525 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4607 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 529 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 4613 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 4619 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 534 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 4625 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 544 "chapel.ypp" /* yacc.c:1661  */
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
#line 4641 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 4647 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4653 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 560 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4659 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 561 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 4665 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4671 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 4677 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 4683 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 4689 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 4695 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 567 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4701 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 568 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 4707 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 4713 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 570 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 4719 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 4725 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 4731 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 4737 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 575 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 4747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 581 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 4757 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 587 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 4767 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 593 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 4777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 599 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 4788 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 606 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 4799 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 615 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[-1].pcallexpr)); }
#line 4805 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 619 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 4811 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4817 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 4823 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 4829 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 4835 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 4841 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 632 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 4847 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 4853 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 4859 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 644 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL((yyvsp[-3].pcallexpr), "'type select' is no longer supported. Use 'select'"); }
#line 4865 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 651 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), NULL, (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 4874 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 656 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-6].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 4883 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 663 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 4893 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 668 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 4903 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 673 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 4913 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 681 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 4919 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 682 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 4925 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 687 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 4931 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 689 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 4937 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 691 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 4943 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 696 "chapel.ypp" /* yacc.c:1661  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 4954 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 706 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[0].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[0].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
#line 4967 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 715 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 4975 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 719 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 4984 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 726 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 4990 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 727 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 4996 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 732 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5005 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 737 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5014 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 742 "chapel.ypp" /* yacc.c:1661  */
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
#line 5034 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 762 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5045 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 768 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5057 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 5070 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 783 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5083 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 794 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5093 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 800 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5104 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 807 "chapel.ypp" /* yacc.c:1661  */
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
#line 5127 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 829 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5135 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 833 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5144 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 838 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
    }
#line 5152 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 842 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5161 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 850 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5167 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 851 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 5173 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 852 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 5179 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 853 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 5185 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 854 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 5191 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 855 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 5197 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 856 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 5203 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 857 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 5209 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 858 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 5215 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 859 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 5221 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 860 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 5227 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 861 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 5233 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 862 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 5239 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 863 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 5245 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 864 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 5251 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 865 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 5257 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 866 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 5263 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 867 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 5269 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 868 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 5275 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 869 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 5281 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 870 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 5287 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 871 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 5293 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 872 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "align"; }
#line 5299 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 873 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 5305 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 874 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 5311 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 878 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 5317 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 879 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 5323 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 880 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 5329 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 881 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 5335 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 882 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 5341 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 883 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 5347 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 884 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 5353 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 885 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 5359 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 886 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 5365 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 887 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 5371 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 888 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 5377 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 889 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 5383 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 893 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 5389 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 894 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5395 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 898 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5401 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 902 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 5407 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 903 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 5413 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 904 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 5419 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 909 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 5425 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 911 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 5431 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 913 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 5437 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 915 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 5443 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 919 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5449 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 920 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 5455 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 921 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 5461 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 922 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 5467 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 923 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 5473 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 924 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 5479 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 925 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 5485 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 926 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5491 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 927 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 5497 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 928 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 5503 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 932 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5509 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 933 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5515 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 934 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 5521 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 935 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 5527 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 939 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 5533 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 940 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 5539 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 944 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 5545 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 945 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 5551 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 946 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 5557 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 947 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 5563 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 948 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 5569 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 952 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 5575 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 958 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 5581 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 963 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 5587 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 965 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5593 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 969 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5599 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 974 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5605 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 975 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 5611 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 979 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5617 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 980 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5623 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 985 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 5629 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 987 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 5635 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 989 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 5641 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 994 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 5658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1007 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 5676 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1023 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5682 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1025 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5688 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1027 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 5694 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1032 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
    }
#line 5705 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1039 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
    }
#line 5716 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1046 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
    }
#line 5727 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1053 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
    }
#line 5739 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1061 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
    }
#line 5749 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1069 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5755 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1070 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5761 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1071 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5767 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1077 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 5776 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1085 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 5782 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1087 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 5792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1093 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 5798 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1098 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 5804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1100 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 5810 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1102 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 5816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1107 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 5822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1109 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1111 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5834 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1117 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5840 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1118 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 5846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1119 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5852 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1123 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1125 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 5864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1129 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5870 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1130 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5876 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1131 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 5882 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1132 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad type specification"); }
#line 5888 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1153 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 5896 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1157 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 5904 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1161 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 5916 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1168 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad array type specification"); clean_exit(1); }
#line 5922 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1172 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5928 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1173 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1174 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 5940 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1179 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 5946 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1181 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 5952 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1187 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 5958 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1189 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 5964 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1191 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 5970 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1195 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5976 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1196 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5982 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1197 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 5988 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 5994 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1199 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6000 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1200 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6006 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1201 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6012 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1207 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6018 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1208 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6024 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1209 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6030 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1210 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6036 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1214 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6042 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1215 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6048 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1216 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6054 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1220 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6060 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6066 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1225 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6072 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1230 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6078 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1231 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6084 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1235 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6090 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1236 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6096 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1237 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6102 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1238 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6108 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1243 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6114 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1260 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6120 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1262 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6126 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1264 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6132 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1266 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6138 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1268 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6144 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1270 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6150 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1272 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6156 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1277 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6162 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1279 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6168 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1281 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6174 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1283 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6180 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1285 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6186 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1287 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6192 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1289 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6198 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1291 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6204 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1293 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6210 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1295 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6216 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1297 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1299 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1301 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 6239 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1308 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 6249 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1314 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 6259 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1320 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 6269 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1326 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 6279 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1335 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 6285 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1339 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 6291 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1355 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6297 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1359 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6303 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1364 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 6309 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1369 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pIntentExpr).first, (yyvsp[0].pIntentExpr).second); }
#line 6315 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1371 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).first);
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).second);
    }
#line 6324 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1379 "chapel.ypp" /* yacc.c:1661  */
    {
      ArgSymbol* tiMark = tiMarkForIntent((yyvsp[-1].pt));
      if (!tiMark)
        USR_FATAL_CONT((yyvsp[0].pexpr), "Unsupported intent in with clause");
      (yyval.pIntentExpr).first = new SymExpr(tiMark);
      (yyval.pIntentExpr).second = (yyvsp[0].pexpr);
    }
#line 6336 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1388 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).first = (yyvsp[-2].pexpr), (yyval.pIntentExpr).second = (yyvsp[0].pexpr); }
#line 6342 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1394 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1399 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 6354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1404 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 6360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1420 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 6366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1422 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1424 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1426 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedLow"), (yyvsp[-1].pexpr)); }
#line 6384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1428 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedHigh"), (yyvsp[0].pexpr)); }
#line 6390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1430 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range", buildDotExpr("BoundedRangeType", "boundedNone")); }
#line 6396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1453 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1454 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1455 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 6414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1459 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 6426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1461 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 6432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1469 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1470 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 6444 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 6450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1475 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 6456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 6462 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1477 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 6468 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 6474 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 6480 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1480 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6486 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 6492 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1483 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 6500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1489 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1490 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6512 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1495 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6524 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6536 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6542 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 386:
#line 1499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 387:
#line 1500 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6554 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 388:
#line 1501 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1502 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6566 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6572 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1504 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6578 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6584 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1506 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6590 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1507 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6602 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1509 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6608 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6614 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1511 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6620 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1512 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6626 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1513 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6632 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6638 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6644 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6650 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1520 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 6656 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 6662 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 6668 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1523 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 6674 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1524 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 6680 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1525 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 6686 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1529 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6692 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6698 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6704 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1532 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6710 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6716 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6722 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1538 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6728 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1539 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6734 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1544 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 6740 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1545 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 6746 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1546 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 6752 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1547 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 6758 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1548 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 6764 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1549 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 6770 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1550 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 6776 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 6780 "bison-chapel.cpp" /* yacc.c:1661  */
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
