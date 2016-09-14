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
  void stringBufferInit();

  #endif
#line 64 "chapel.ypp" /* yacc.c:355  */

  #ifndef _BISON_CHAPEL_DEFINES_1_
  #define _BISON_CHAPEL_DEFINES_1_

  #include "symbol.h"

  #include <cstdio>
  #include <utility>
  #include <vector>

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

  struct OnlyRename {
    enum{SINGLE, DOUBLE} tag;

    union {
      Expr*  elem;
      std::pair<Expr*, Expr*>* renamed;
    };
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
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif
#line 132 "chapel.ypp" /* yacc.c:355  */

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
#line 154 "chapel.ypp" /* yacc.c:355  */

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

#line 224 "bison-chapel.cpp" /* yacc.c:355  */

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
    TAS = 266,
    TATOMIC = 267,
    TBEGIN = 268,
    TBREAK = 269,
    TBY = 270,
    TCLASS = 271,
    TCOBEGIN = 272,
    TCOFORALL = 273,
    TCONFIG = 274,
    TCONST = 275,
    TCONTINUE = 276,
    TDELETE = 277,
    TDMAPPED = 278,
    TDO = 279,
    TDOMAIN = 280,
    TELSE = 281,
    TENUM = 282,
    TEXCEPT = 283,
    TEXPORT = 284,
    TEXTERN = 285,
    TFOR = 286,
    TFORALL = 287,
    TIF = 288,
    TIN = 289,
    TINDEX = 290,
    TINLINE = 291,
    TINOUT = 292,
    TITER = 293,
    TLABEL = 294,
    TLAMBDA = 295,
    TLET = 296,
    TLOCAL = 297,
    TMINUSMINUS = 298,
    TMODULE = 299,
    TNEW = 300,
    TNIL = 301,
    TNOINIT = 302,
    TON = 303,
    TONLY = 304,
    TOTHERWISE = 305,
    TOUT = 306,
    TPARAM = 307,
    TPLUSPLUS = 308,
    TPRAGMA = 309,
    TPRIMITIVE = 310,
    TPRIVATE = 311,
    TPROC = 312,
    TPUBLIC = 313,
    TRECORD = 314,
    TREDUCE = 315,
    TREF = 316,
    TREQUIRE = 317,
    TRETURN = 318,
    TSCAN = 319,
    TSELECT = 320,
    TSERIAL = 321,
    TSINGLE = 322,
    TSPARSE = 323,
    TSUBDOMAIN = 324,
    TSYNC = 325,
    TTHEN = 326,
    TTYPE = 327,
    TUNDERSCORE = 328,
    TUNION = 329,
    TUSE = 330,
    TVAR = 331,
    TWHEN = 332,
    TWHERE = 333,
    TWHILE = 334,
    TWITH = 335,
    TYIELD = 336,
    TZIP = 337,
    TALIAS = 338,
    TAND = 339,
    TASSIGN = 340,
    TASSIGNBAND = 341,
    TASSIGNBOR = 342,
    TASSIGNBXOR = 343,
    TASSIGNDIVIDE = 344,
    TASSIGNEXP = 345,
    TASSIGNLAND = 346,
    TASSIGNLOR = 347,
    TASSIGNMINUS = 348,
    TASSIGNMOD = 349,
    TASSIGNMULTIPLY = 350,
    TASSIGNPLUS = 351,
    TASSIGNSL = 352,
    TASSIGNSR = 353,
    TBAND = 354,
    TBNOT = 355,
    TBOR = 356,
    TBXOR = 357,
    TCOLON = 358,
    TCOMMA = 359,
    TDIVIDE = 360,
    TDOT = 361,
    TDOTDOT = 362,
    TDOTDOTDOT = 363,
    TEQUAL = 364,
    TEXP = 365,
    TGREATER = 366,
    TGREATEREQUAL = 367,
    THASH = 368,
    TLESS = 369,
    TLESSEQUAL = 370,
    TMINUS = 371,
    TMOD = 372,
    TNOT = 373,
    TNOTEQUAL = 374,
    TOR = 375,
    TPLUS = 376,
    TQUESTION = 377,
    TSEMI = 378,
    TSHIFTLEFT = 379,
    TSHIFTRIGHT = 380,
    TSTAR = 381,
    TSWAP = 382,
    TIO = 383,
    TLCBR = 384,
    TRCBR = 385,
    TLP = 386,
    TRP = 387,
    TLSBR = 388,
    TRSBR = 389,
    TNOELSE = 390,
    TUPLUS = 391,
    TUMINUS = 392
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
#line 185 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 407 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 413 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 38 "chapel.ypp" /* yacc.c:359  */

  int         captureTokens;
  char        captureString[1024];
#line 193 "chapel.ypp" /* yacc.c:359  */

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

#line 479 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   12105

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  138
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  110
/* YYNRULES -- Number of rules.  */
#define YYNRULES  447
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  847

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   392

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
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   417,   417,   422,   423,   429,   430,   435,   436,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   470,   472,   477,   478,   479,
     494,   495,   500,   501,   506,   511,   516,   520,   526,   531,
     535,   540,   544,   545,   546,   550,   554,   555,   556,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   572,   573,   577,   581,   582,   586,   587,   591,   592,
     596,   597,   598,   599,   600,   601,   602,   606,   607,   611,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   648,   654,   660,
     666,   673,   683,   687,   688,   689,   690,   694,   699,   700,
     704,   706,   708,   715,   720,   728,   733,   738,   746,   747,
     752,   753,   755,   760,   770,   779,   783,   791,   792,   797,
     802,   796,   827,   833,   840,   848,   859,   865,   858,   893,
     897,   902,   906,   914,   915,   916,   917,   918,   919,   920,
     921,   922,   923,   924,   925,   926,   927,   928,   929,   930,
     931,   932,   933,   934,   935,   936,   937,   938,   939,   943,
     944,   945,   946,   947,   948,   949,   950,   951,   952,   953,
     954,   958,   959,   963,   967,   968,   969,   973,   975,   977,
     979,   984,   985,   989,   990,   991,   992,   993,   994,   995,
     996,   997,  1001,  1002,  1003,  1004,  1008,  1009,  1013,  1014,
    1015,  1016,  1017,  1018,  1022,  1023,  1027,  1028,  1032,  1034,
    1039,  1040,  1044,  1045,  1049,  1050,  1054,  1056,  1058,  1063,
    1076,  1093,  1094,  1096,  1101,  1109,  1117,  1125,  1134,  1144,
    1145,  1146,  1150,  1151,  1159,  1161,  1167,  1172,  1174,  1176,
    1181,  1183,  1185,  1192,  1193,  1194,  1198,  1199,  1204,  1205,
    1206,  1207,  1227,  1231,  1235,  1243,  1247,  1248,  1249,  1253,
    1255,  1261,  1263,  1265,  1270,  1271,  1272,  1273,  1274,  1275,
    1276,  1282,  1283,  1284,  1285,  1289,  1290,  1291,  1295,  1296,
    1300,  1301,  1305,  1306,  1310,  1311,  1312,  1313,  1314,  1318,
    1329,  1330,  1331,  1332,  1333,  1334,  1336,  1338,  1340,  1342,
    1344,  1346,  1351,  1353,  1355,  1357,  1359,  1361,  1363,  1365,
    1367,  1369,  1371,  1373,  1375,  1382,  1388,  1394,  1400,  1409,
    1414,  1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,  1434,
    1435,  1439,  1443,  1445,  1453,  1464,  1465,  1469,  1470,  1475,
    1480,  1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,  1496,
    1498,  1500,  1502,  1504,  1506,  1511,  1512,  1513,  1514,  1525,
    1526,  1530,  1531,  1532,  1536,  1537,  1538,  1546,  1547,  1548,
    1549,  1553,  1554,  1555,  1556,  1557,  1558,  1559,  1560,  1561,
    1562,  1566,  1574,  1575,  1579,  1580,  1581,  1582,  1583,  1584,
    1585,  1586,  1587,  1588,  1589,  1590,  1591,  1592,  1593,  1594,
    1595,  1596,  1597,  1598,  1599,  1600,  1601,  1605,  1606,  1607,
    1608,  1609,  1610,  1614,  1615,  1616,  1617,  1621,  1622,  1623,
    1624,  1629,  1630,  1631,  1632,  1633,  1634,  1635
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "CSTRINGLITERAL", "EXTERNCODE", "TALIGN",
  "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBY", "TCLASS", "TCOBEGIN",
  "TCOFORALL", "TCONFIG", "TCONST", "TCONTINUE", "TDELETE", "TDMAPPED",
  "TDO", "TDOMAIN", "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN",
  "TFOR", "TFORALL", "TIF", "TIN", "TINDEX", "TINLINE", "TINOUT", "TITER",
  "TLABEL", "TLAMBDA", "TLET", "TLOCAL", "TMINUSMINUS", "TMODULE", "TNEW",
  "TNIL", "TNOINIT", "TON", "TONLY", "TOTHERWISE", "TOUT", "TPARAM",
  "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPUBLIC",
  "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT",
  "TSERIAL", "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTYPE",
  "TUNDERSCORE", "TUNION", "TUSE", "TVAR", "TWHEN", "TWHERE", "TWHILE",
  "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND",
  "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP",
  "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNSL", "TASSIGNSR", "TBAND",
  "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL",
  "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT",
  "TSTAR", "TSWAP", "TIO", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR",
  "TRSBR", "TNOELSE", "TUPLUS", "TUMINUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_stmt", "access_control", "block_stmt", "stmt_ls", "only_ls",
  "opt_only_ls", "except_ls", "use_stmt", "require_stmt",
  "assignment_stmt", "opt_ident", "ident", "opt_ident_or_string",
  "do_stmt", "return_stmt", "class_level_stmt", "private_decl",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "select_stmt", "when_stmt_ls", "when_stmt", "class_decl_stmt",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@1", "$@2", "linkage_spec",
  "fn_decl_stmt", "$@3", "$@4", "fn_decl_stmt_inner", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls",
  "formal", "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_or_iter", "opt_ret_tag", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "opt_query_expr", "var_arg_expr",
  "opt_where_part", "type_alias_decl_stmt", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
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
     385,   386,   387,   388,   389,   390,   391,   392
};
# endif

#define YYPACT_NINF -711

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-711)))

#define YYTABLE_NINF -399

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -711,    99,  2482,  -711,  -711,  -711,  -711,  -711,  -711,  -711,
    -711,  3530,    10,   175,  -711,    10,  6998,    21,   175,  6998,
    3530,    55,   175,    89,   838,  5920,  6998,  6998,    72,  -711,
     175,  -711,    27,  3530,  6998,  6998,  -711,  6998,  6998,   131,
      81,   482,   739,  -711,  6132,  6237,  6998,  6368,  6998,   146,
     101,  3530,   175,  -711,  6132,  6998,  6998,  -711,  -711,  6998,
    -711,  -711,  8048,  6998,  6998,  -711,  6998,  -711,  -711,  2744,
    5603,  6132,  -711,  3399,  -711,  -711,   174,  -711,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,
     175,  -711,   269,    49,  -711,  -711,  -711,   266,   180,  -711,
    -711,  -711,   202,   217,   166,   240,   249, 11863,  1387,   147,
     265,   283,  -711,  -711,  -711,  -711,  -711,  -711,   344,  -711,
   11863,   178,  3530,  -711,   294,  -711,   262,  6998,  6998,  6998,
    6998,  6998,  6132,  6132,   156,  -711,  -711,  -711,  -711,  9476,
     199,  -711,  -711,   175,   307,  9770,   300,  6132,   310,  -711,
    -711,  -711,  -711,   175,   260,   175,   314,    46,  8572,  8538,
    1945,  6132,  3530,   320,    22,    41,    43,  -711,  -711,    82,
     349,  8714,    82,  -711,  6132,  -711,   348,  -711,  -711,   175,
    -711,   -25, 11863,  -711,  9804,  9355,  3530,  -711,  -711,  8714,
   11863,   326,  6132,  -711, 11863,   374,   339,    90,  9584,  8714,
    9889,   256,   981,    82,   256,    82,  -711,  -711,  2875,    53,
    -711,  6998,  -711,   -24,   -21, 11863,   235,  9933,   -37,   459,
    -711,   175,   365,  -711,  -711,  -711,    25,    27,    27,    27,
    -711,  6998,  6998,  6998,  6998,  6473,  6473,  6998,  6998,  6998,
    6998,  6998,  6998,    79,  8048,  6998,  6998,  6998,  6998,  6998,
    6998,  6998,  6998,  6998,  6998,  6998,  6998,  6998,  6998,  6998,
    6998,  6998,  6998,  6998,  6998,  6998,  6998,  6998,  6998,  6998,
    6998,  6998,  6998,  6998,  6998,  6132,  6132,  6473,  6473,  5498,
    -711,  -711,  3006,  -711,  9628,  9736, 10008,    76,  6473,    46,
     347,  -711,  -711,  6998,   342,  -711,   340,   367,  -711, 11863,
     175,   350,   175,   441,  6132,  -711,  3661,  6473,  -711,  3792,
    6473,  -711,    46,  3530,   451,   351,  -711,    51,  -711,  -711,
      22,  -711,   377,   352,  -711,  2149,   399,   400,  6998,    27,
    -711,   359,  -711,  -711,  6132,  -711,  -711,  -711,  -711,  -711,
    6132,   366,  5078,   396,  -711,  -711,  7103,  6998,  -711,  -711,
    -711,  -711,  6025,  -711,  8426,  5710,  -711,  5815,  -711,  6473,
    5183,  2613,   368,  6998,  5393,  -711,  -711,   375,  6132,   379,
     337,    27,    94,   184,   220,   270,  9418, 11979, 11979,   313,
    -711,   313,  -711,   313,  8606,   389,  1100,  1289,   349,   256,
    -711,  -711,  -711,   981,  8784,   313,   540,   540, 11979,   540,
     540,   463,   256,  8784,  8356,   463,    82,    82,   256, 10052,
   10120, 10164, 10232, 10276, 10344, 10388, 10456, 10500, 10568, 10612,
   10680, 10724, 10792, 10836,   384,   383,  -711,   313,  -711,   313,
     135,  -711,  -711,  -711,  -711,  -711,  -711,   175,    44,  -711,
   11921,   364,  7208,  6473,  7313,  6473,  6998,  6473,  1627,    10,
   10904,  -711,  -711, 10948,  6998,  6132,  -711,  6132,   439,   250,
    -711,  -711,   365,  6998,    69,  6998, 11863,    48,  8748,  6998,
   11863,    -2,  8680,  -711,   495, 11016,  3530,  -711,    75,  -711,
      36,  -711,   338,   393,    22,    67,  -711,  6132,  -711,   297,
    6998,  6578,  -711, 11863,  -711,  -711,  -711, 11863,     7,   397,
    -711,  6132,  -711,   272,   175,   405,   429,   412, 11074,   429,
     413,  -711,  -711,  -711,  -711,  -711,  -711,    -6,  8216,  -711,
    -711, 11863,  3530, 11863,  -711, 11137,  3137,   433,  -711,  -711,
    -711,  -711,  -711,  8097,   290,  -711,  -711,  -711,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  5498,
    -711,  6473,  6473,  6998,   514, 11195,  6998,   516, 11258,   408,
    8250,    46,    46,  -711, 11863,  -711, 11863,  -711,  6998,   175,
    -711,   417,  8714,  -711,  8856,  3923,  -711,  4054,  -711,  8919,
    4185,  -711,    46,  4316,  -711,    46,  3530,  6998,  -711,   852,
    -711,    22,   444,   488,  -711,  -711,  -711,    37,  -711,  -711,
    5078,   400,    80, 11863,  -711, 11863,  4447,  6132,  -711,  -711,
    -711,   170,  -711,  6998,  -711,  6998,  -711,  4578,   420,  4709,
     422,  -711,  6998,  -711,  3268,   254,   338,  -711,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,
    -711,  -711,   175,  -711,  -711,  -711,  -711,  -711,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,
    -711,  -711,  -711,   453,   426,   426,  -711,  -711,    87,   168,
   11316,  7418,  7523, 11379,  7628,  7733,  7838,  7943,  -711,  -711,
   11863,  -711,  -711,  -711,  3530,  6998, 11863,  6998, 11863,  3530,
    6998, 11863,  -711,  6998, 11863,  -711,  -711, 11863,  -711,   428,
    6683,   108,  -711,   483,  -711,  6473,  4816,  3530,  -711,    45,
    4947, 11437, 11863,  6998,  -711, 11863,  3530,  6998,  -711, 11863,
    3530, 11863,  -711,   458,   907,   907,  -711,  1155,  -711,    31,
    -711,  8097,    51,  -711,  -711,  6998,  6998,  6998,  6998,  6998,
    6998,  6998,  6998,   819, 11016,  8982,  9045, 11016,  9108,  9171,
     444,    55,  6998,  6998,  5288,  -711,  -711,   229,  6132,  -711,
    -711,  6998,    26,  8284,  -711,   507,   300,  -711,  6998,  9234,
    -711,  9297,  -711,  -711,   483,  -711,   426,   426,   241, 11500,
   11558, 11621, 11679, 11742, 11800,  -711,  3530,  3530,  3530,  3530,
     108,  6788,   171,  -711,  -711, 11863, 11863,  -711,  -711,  -711,
    7943, 11863,  3530,  3530,    66,  -711,  -711,  -711,  6998,  6998,
    6998,  6998,  6998,  6998, 11016, 11016, 11016, 11016,  -711,  -711,
    -711,  -711,  -711,   279,  6473,  2329,   796, 11016, 11016,  -711,
    -711,  -711,  8392,  -711,  -711,  6893,  -711
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    34,    73,   391,   392,   393,   394,
     395,     0,   349,    71,   125,   349,     0,   250,    71,     0,
       0,     0,     0,    71,    71,     0,     0,     0,     0,   143,
       0,   139,     0,     0,     0,     0,   340,     0,     0,     0,
       0,   249,   249,   126,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   127,     0,     0,     0,   443,   445,     0,
     446,   447,   374,     0,     0,   444,   441,    80,   442,     0,
       0,     0,     4,     0,     5,     9,     0,    10,    11,    12,
      14,   309,    19,    13,    81,    15,    17,    16,    18,    86,
       0,    84,   366,     0,    87,    85,    88,     0,   375,   362,
     363,   312,   310,     0,     0,   367,   368,     0,   311,     0,
     376,   377,   378,   361,   314,   313,   364,   365,     0,    21,
     320,     0,     0,   350,     0,    72,     0,     0,     0,     0,
       0,     0,     0,     0,   366,   375,   310,   367,   368,   349,
     311,   376,   377,     0,     0,     0,     0,   300,     0,    75,
      74,   144,    89,     0,   145,     0,     0,     0,     0,     0,
       0,   300,     0,     0,     0,     0,     0,   252,    28,   429,
     359,     0,   430,     7,   300,   250,   251,    83,    82,   229,
     292,     0,   291,    78,     0,     0,     0,    77,    31,     0,
     315,     0,   300,    32,   321,   241,     0,     0,   291,     0,
       0,   432,   373,   428,   431,   427,    40,    42,     0,     0,
     295,     0,   297,     0,     0,   296,     0,   291,     0,     0,
       6,     0,   128,   217,   216,   146,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   372,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   300,   300,     0,     0,     0,
      22,    23,     0,    24,     0,     0,     0,     0,     0,     0,
       0,    25,    26,     0,   309,   307,     0,   301,   302,   308,
       0,     0,     0,     0,     0,    98,     0,     0,    97,     0,
       0,   104,     0,     0,   114,     0,    27,   194,   140,   257,
       0,   258,   260,     0,   271,     0,     0,   263,     0,     0,
      29,     0,   145,   228,     0,    55,    79,   118,    76,    30,
     300,     0,     0,   239,   236,    52,     0,    48,    91,    33,
      41,    43,     0,   396,     0,     0,   387,     0,   389,     0,
       0,     0,     0,     0,     0,   400,     8,     0,     0,     0,
     212,     0,     0,     0,     0,     0,   348,   424,   423,   426,
     434,   433,   438,   437,   420,   417,   418,   419,   370,   407,
     386,   385,   384,   371,   411,   422,   416,   414,   425,   415,
     413,   405,   410,   412,   421,   404,   408,   409,   406,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,   435,   440,   439,
     206,   203,   204,   205,   209,   210,   211,     0,     0,   352,
       0,     0,     0,     0,     0,     0,     0,     0,   398,   349,
     349,    94,   237,     0,     0,     0,   317,     0,   137,     0,
     134,   238,   128,     0,     0,     0,   324,     0,     0,     0,
     330,     0,     0,   105,   113,     0,     0,   316,     0,   195,
       0,   202,   218,     0,   261,     0,   275,     0,   270,   362,
       0,     0,   254,   360,   253,   383,   294,   293,     0,     0,
     318,     0,   243,   362,     0,   442,    51,     0,    44,    49,
       0,   397,   369,   388,   298,   390,   299,     0,     0,   399,
     110,   334,     0,   402,   401,     0,     0,   129,   130,   213,
     214,   215,   147,     0,     0,   245,   244,   246,   248,    56,
      63,    64,    65,    60,    62,    69,    70,    58,    61,    59,
      57,    67,    66,    68,   381,   382,   207,   208,   354,     0,
     351,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,   306,   304,   305,   303,     0,   135,
     133,     0,     0,   112,     0,     0,    96,     0,    95,     0,
       0,   102,     0,     0,   100,     0,     0,     0,   116,   201,
     193,     0,   284,   219,   222,   221,   223,     0,   259,   262,
       0,   263,     0,   255,   264,   265,     0,     0,   117,   119,
     319,     0,   240,     0,    53,     0,    54,     0,     0,     0,
       0,   111,     0,    35,     0,   249,   218,   176,   174,   179,
     186,   187,   188,   183,   185,   181,   184,   182,   180,   190,
     189,   155,   158,   156,   157,   168,   159,   172,   164,   162,
     175,   163,   161,   166,   171,   173,   160,   165,   169,   170,
     167,   177,   178,   153,   191,   191,   247,   353,   375,   375,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    92,
     138,   136,   130,    99,     0,     0,   323,     0,   322,     0,
       0,   329,   103,     0,   328,   101,   115,   339,   196,     0,
       0,   263,   220,   234,   256,     0,     0,     0,   121,     0,
       0,    46,    45,     0,   108,   336,     0,     0,   106,   335,
       0,   403,    36,    71,   249,   249,   123,   249,   131,     0,
     154,     0,   194,   149,   150,     0,     0,     0,     0,     0,
       0,     0,     0,   249,   327,     0,     0,   333,     0,     0,
     284,   287,   288,   289,     0,   286,   290,   362,   230,   198,
     197,     0,     0,     0,   273,   362,   122,   120,     0,     0,
     109,     0,   107,   132,   234,   153,   191,   191,     0,     0,
       0,     0,     0,     0,     0,   124,     0,     0,     0,     0,
     263,   276,     0,   231,   233,   232,   235,   226,   227,   141,
       0,    47,     0,     0,     0,   151,   152,   192,     0,     0,
       0,     0,     0,     0,   326,   325,   332,   331,   200,   199,
     278,   279,   281,   362,     0,   398,   362,   338,   337,   224,
     148,   225,     0,   280,   282,   276,   283
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -711,  -711,  -711,     1,  -588,  1816,  -711,  -711,   605,    52,
     224,  -711,  -711,  -711,  -711,  -711,    63,  1743,    -4,   544,
    -707,  -594,   -40,  -711,  -711,   186,  -711,  -711,  -711,  -711,
    -711,   418,   113,  -115,  -711,  -711,     4,   436,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,  -160,  -157,  -625,  -711,  -152,
      -8,  -711,  -268,  -711,  -711,   -43,  -711,  -220,   268,  -711,
    -205,  -188,  -711,  -133,  -711,  -711,  -711,  -165,   274,  -711,
    -316,  -598,  -711,  -468,  -335,  -704,  -710,  -163,     2,    93,
    -711,  -143,  -711,   142,   345,  -313,  -711,  -711,   657,  -711,
      -9,  -150,  -711,    42,  -711,   811,   987,   -11,  1135,  -711,
    1281,  1515,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -258
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   207,    73,   474,    75,    76,    77,   208,
     506,   510,   507,    78,    79,    80,   150,    81,   154,   188,
      82,    83,    84,    85,    86,   569,    87,    88,   498,   619,
      89,    90,   369,   635,    91,   459,   460,   134,   163,   482,
      93,    94,   370,   636,   532,   674,   675,   743,   318,   478,
     479,   480,   481,   533,   225,   607,   840,   809,   180,   804,
     769,   772,    95,   196,   343,    96,    97,   166,   167,   322,
     323,   492,   326,   327,   488,   831,   766,   711,   209,   213,
     214,   296,   297,   298,   135,    99,   100,   101,   136,   103,
     122,   123,   438,   439,   104,   137,   138,   107,   140,   109,
     141,   142,   112,   113,   218,   114,   115,   116,   117,   118
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     120,   177,   178,    72,   483,   139,   126,   502,   145,   312,
     290,   437,   489,   714,   158,   159,   160,   611,   315,   151,
     301,   441,   590,   169,   170,     5,   171,   172,     5,   503,
       5,   331,   324,   182,   184,   185,   189,   190,   324,     5,
     194,   738,   324,   198,   199,   200,   181,   737,   201,   341,
     744,   202,   203,   204,  -201,   205,   197,   616,   182,   215,
     217,   372,   373,   374,   375,   808,   362,   364,   324,   186,
     186,   430,   585,   216,   121,  -268,   124,   328,   121,   334,
     355,   144,     5,   357,   617,   431,   371,   223,   432,    45,
     121,   832,     5,   143,  -268,   319,   149,   365,   335,     3,
    -268,  -268,   433,   434,   390,   234,   224,   808,   356,  -268,
     447,   358,   435,   770,   715,  -268,   120,   284,   285,   286,
     194,   182,   217,   436,  -266,   844,  -268,   282,   627,    45,
     289,   843,   424,   425,   610,   287,   299,   618,   173,   713,
     610,   846,   235,   783,   325,  -268,   236,   329,   559,   334,
     299,   391,  -268,   320,  -268,   282,   164,   352,   164,   738,
    -268,   815,   816,   299,  -268,   737,  -268,   601,   609,   556,
     610,  -268,   332,   334,   282,   282,   560,   282,     5,   599,
     360,   299,  -201,   353,   360,   241,   147,   242,   243,   839,
    -268,  -355,   246,   491,   334,   282,   557,   499,   329,   253,
     354,   583,   829,   161,   715,   834,   534,   600,   259,   351,
     448,   157,   174,   345,   716,   191,   768,   535,   221,  -355,
     376,   377,   378,   379,   381,   383,   384,   385,   386,   387,
     388,   389,   192,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   299,   299,   427,   429,   440,   359,
      14,   784,  -356,    17,   360,   360,    14,   450,   275,   234,
     276,    22,   453,    23,   733,   709,   226,  -380,   329,  -380,
      29,   437,  -142,   182,   231,   466,   468,   489,   470,   472,
    -356,   441,   475,  -342,   720,   835,   464,   536,    39,   279,
     734,  -142,   735,    43,  -285,   121,   235,   493,   227,    43,
     236,   592,   595,   497,   329,  -341,    52,   228,    53,   299,
    -379,  -269,  -379,  -285,    53,   508,   508,  -285,   212,   360,
     230,   497,   229,   537,   215,   599,   215,    98,   518,   497,
     521,     5,   523,   525,   579,   149,    98,   182,   603,   241,
    -269,  -285,   243,  -346,  -277,    98,   246,   628,   630,   361,
     527,   622,  -347,   817,   329,  -269,  -242,    67,    98,   293,
     580,   774,  -269,  -277,   736,   774,   -71,  -277,  -344,   529,
     604,   282,  -345,   538,   329,  -242,    98,   767,   530,   605,
    -380,  -269,  -380,   775,   277,   -71,  -343,   775,   278,   531,
     606,  -277,   234,   676,    98,   295,   241,   281,    98,   243,
    -269,   380,   382,   246,   562,   454,  -269,   455,   278,   295,
     291,   466,   565,   470,   568,   475,   570,   521,    92,   300,
     571,   572,   295,   574,   576,   304,   299,    92,   514,   235,
     516,   317,   582,   236,   584,   243,    92,   340,   589,   342,
     295,     5,   344,   426,   428,   149,   366,    98,   368,    92,
     452,   457,   456,   461,   449,   463,   217,   476,  -251,   613,
     615,   484,   490,   477,   485,   491,   234,    92,   833,   612,
     217,   495,   241,   467,   242,   243,   471,   836,   500,   246,
     504,   175,   522,   621,   526,    92,   253,    98,   528,    92,
    -251,    23,   176,   257,   258,   259,   554,   555,    29,  -251,
    -142,   596,   833,   235,   578,   608,   -39,   236,   -50,   620,
     153,    98,   833,   623,  -251,   624,   626,   334,   681,  -142,
     684,  -272,   686,   295,   295,   517,   692,   710,   440,   712,
     381,   427,   680,    98,   726,   683,   730,   742,    92,   741,
     760,   771,   238,   234,   239,   240,   241,   690,   242,   243,
    -272,   509,   302,   246,   696,   581,   698,   753,   634,   701,
     253,   786,   704,   691,   787,  -272,   707,   257,   258,   259,
     788,   708,  -272,   739,   841,   828,   814,   800,    92,   577,
     235,   677,   496,   494,   236,     0,   182,     0,   295,     0,
       0,  -272,   721,     0,   722,     0,   725,     0,   729,   719,
     496,   731,    92,   212,     0,   212,     0,    98,   496,   564,
    -272,   567,     0,     0,     0,   351,  -272,     0,     0,   238,
       0,   239,   240,   241,    92,   242,   243,   244,     0,     0,
     246,    98,   187,     0,    98,     0,   252,   253,    98,   102,
       0,   256,     0,     0,   257,   258,   259,     0,   102,     0,
     696,   698,     0,   701,   704,   725,   729,   102,     0,     0,
       0,     0,     0,   754,   755,     0,   756,     0,   757,   758,
     102,     0,   759,     0,   177,   178,     0,     0,     0,     0,
       0,   305,   308,   311,   773,   521,    98,     0,   102,   521,
       0,     0,   779,     0,     0,   330,   781,     0,    92,     0,
       0,     0,     0,   575,     0,   295,   102,     0,     0,     0,
     102,   283,     0,   339,   754,   789,   790,   757,   791,   792,
     793,   794,    92,   348,     0,    92,     0,   380,   426,    92,
       0,   190,   194,   217,     0,     0,     0,   805,   175,     0,
     806,     0,   187,   187,   187,   314,   802,   811,    23,   176,
       0,     0,     0,     0,     0,    29,   187,  -142,     0,   102,
       0,     0,   558,   -38,     0,   824,   825,   826,   827,     0,
       0,     0,     0,     0,   187,     0,  -142,    92,     0,   729,
       0,   837,   838,     0,   187,     0,     0,   824,   825,   826,
     827,   837,   838,   105,     0,     0,     0,     0,     0,   102,
       0,    98,   105,   842,   521,     0,     0,     0,     0,     0,
    -274,   105,     0,   451,   729,    14,     0,     0,    17,     0,
       0,     5,     0,   102,   105,   149,    22,   152,    23,   733,
       0,     0,     0,     0,     0,    29,   473,  -142,  -251,  -274,
       0,     0,   105,     0,     0,   102,     0,    98,     0,     0,
       0,    98,   430,    39,  -274,   734,  -142,   735,    43,     0,
     105,  -274,     0,     0,   105,     0,   431,     0,     0,   432,
    -251,    52,     0,    53,   187,     0,     0,     0,     0,  -251,
    -274,     0,     0,   433,   434,     0,   678,   679,     0,     0,
     153,     0,    92,   435,  -251,     0,     0,   187,     0,  -274,
       0,     0,     0,     0,   436,  -274,   175,     0,     0,     0,
      98,     0,    98,   105,     0,    98,    23,   176,    98,   102,
       0,    98,    67,    29,     0,  -142,     0,     0,     0,   795,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    98,    92,   102,  -142,     0,   102,     0,     0,     0,
     102,     0,    98,   105,    98,     0,     0,     0,   765,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,     0,   105,   106,     0,
       0,     0,     0,     0,   234,     0,     0,   106,     0,     0,
       0,   586,   588,     0,     0,   591,   594,     0,   102,   105,
     106,    92,     0,    92,     0,     0,    92,     0,     0,    92,
       0,     0,    92,     0,     0,     0,   803,     0,   106,    98,
       0,   235,     0,     0,    98,   236,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,   106,     0,     0,     0,
     106,     0,    98,    92,     0,    92,     0,     0,     0,   830,
      92,    98,   187,   187,     0,    98,   187,   187,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,     0,     0,
       0,   246,     0,   105,     0,     0,     0,   252,   253,     0,
       0,     0,   256,   830,     0,   257,   258,   259,     0,   106,
       0,     0,     0,   830,     0,   688,   689,   105,     0,     0,
     105,     0,     0,   234,   105,     0,   693,     0,     0,     0,
      92,     0,     0,   102,     0,    92,   702,   108,     0,   705,
       0,    98,    98,    98,    98,     0,   108,     0,     0,   106,
       0,     0,     0,    92,     0,   108,     0,    98,    98,     0,
     235,     0,    92,     0,   236,     0,    92,     0,   108,     0,
       0,    14,   105,   106,    17,     0,   187,   187,     0,   102,
       0,     0,    22,   102,    23,   733,   108,   187,     0,   314,
       0,    29,     0,  -142,   314,   106,     0,   187,     0,   238,
     187,     0,   240,   241,   108,   242,   243,     0,   108,   219,
     246,   734,  -142,   735,    43,     0,     0,   253,     0,     0,
       0,     0,     0,     0,   257,   258,   259,    52,     0,    53,
       0,     0,    92,    92,    92,    92,     0,     0,     0,     0,
       0,     0,   102,     0,   102,     0,     0,   102,    92,    92,
     102,     0,     0,   102,     0,     0,     0,   108,     0,     0,
       0,     0,     0,   777,     0,     0,     0,     0,     0,   106,
       0,     0,     0,   102,     0,     0,     0,     0,    67,     0,
       0,     0,     0,   110,   102,     0,   102,   105,     0,     0,
       0,   102,   110,   106,     0,     0,   106,   108,     0,     0,
     106,   110,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   234,     0,   110,     0,     0,     0,     0,     0,
       0,   108,     0,     0,   187,     0,     0,     0,     0,     0,
       0,     0,   110,   105,     0,     0,     0,   105,     0,     0,
       0,     0,     0,   108,     0,     0,     0,     0,   106,   235,
     110,   102,     0,   236,   110,     0,   102,     0,     0,     0,
     314,   314,     0,   314,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,   807,     0,     0,
       0,     0,     0,   102,   314,     0,   314,   102,   238,     0,
       0,     0,   241,     0,   242,   243,   105,     0,   105,   246,
       0,   105,     0,   110,   105,     0,   253,   105,     0,     0,
       0,     0,     0,   257,   258,   259,     0,   108,     0,   807,
       0,     0,     0,     0,     0,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   105,     0,
     105,   108,     0,   110,   108,   105,     0,     0,   108,     0,
       0,     0,     0,   102,   102,   102,   102,     0,     0,     0,
       0,     0,     0,   106,     0,     0,     0,   110,     0,   102,
     102,     0,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,     0,     0,     0,   110,
       0,     0,     0,     0,     0,     0,   108,     0,     0,     0,
       0,     0,     0,     0,     0,   105,     0,     0,     0,   106,
     105,     0,     0,   106,   274,  -357,     0,   111,  -379,     0,
    -379,     0,     0,     0,     0,     0,   111,     0,   105,     0,
       0,     0,     0,     0,     0,   111,     0,   105,     0,     0,
       0,   105,     0,     0,     0,     0,     0,     0,   111,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,     0,     0,   111,     0,     0,     0,
       0,     0,   106,     0,   106,     0,     0,   106,     0,     0,
     106,     0,     0,   106,   111,     0,     0,   110,   111,     0,
     110,     0,     0,     0,   110,     0,     0,     0,     0,     0,
       0,     0,     0,   106,     0,     0,     0,   105,   105,   105,
     105,   108,     0,     0,   106,     0,   106,     0,     0,     0,
       0,   106,     0,   105,   105,     0,     0,     0,     0,     0,
       5,     6,     7,     8,     9,    10,     0,   111,     0,   127,
       0,     0,   110,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,   108,   128,   129,
     130,   108,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,   111,     0,     0,
      38,   106,    40,     0,     0,     0,   106,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,   111,     0,     0,   106,     0,     0,     0,     0,     0,
       0,     0,     0,   106,     0,     0,     0,   106,     0,     0,
     108,     0,   108,   111,     0,   108,     0,    59,   108,     0,
       0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,   108,     0,     0,     0,     0,   125,   110,    70,     0,
     133,   125,   108,     0,   108,   148,   125,   125,     0,   108,
       0,     0,     0,   162,     0,   165,     0,     0,     0,     0,
       0,     0,     0,   106,   106,   106,   106,     0,     0,     0,
       0,     0,     0,     0,     0,   195,     0,   111,     0,   106,
     106,     0,     0,   110,     0,     0,     0,   110,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,   111,     0,     0,   111,     0,     0,   119,   111,   108,
       0,     0,     0,   222,   108,     0,   146,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   168,
       0,     0,   108,     0,     0,     0,     0,     0,     0,     0,
       0,   108,     0,     0,     0,   108,   110,   193,   110,     0,
       0,   110,     0,     0,   110,     0,   111,   110,     0,     0,
       0,     0,     0,     0,     0,    74,   195,     0,     0,   220,
     294,     0,     0,     0,     0,     0,   195,   110,   303,     0,
       0,     0,     0,     0,   294,     0,     0,   321,   110,     0,
     110,     0,     0,     0,     0,   110,     0,   294,     0,   125,
       0,     0,   333,     0,     0,     0,     0,     0,     0,     0,
       0,   108,   108,   108,   108,   294,     0,     0,   280,     0,
       0,     0,     0,     0,     0,     0,     0,   108,   108,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,   367,     0,     0,     0,   234,   165,
     165,   165,   165,     0,     0,   110,     0,     0,   316,     0,
     110,     0,     0,     0,     0,     0,   392,     0,     0,     0,
       0,   111,     0,     0,     0,     0,     0,     0,   110,     0,
       0,     0,   338,     0,     0,   235,     0,   110,     0,   236,
       0,   110,     0,     0,     0,     0,   313,     0,   294,   294,
       0,     0,     0,     0,    74,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,   111,     0,     0,
       0,   111,     0,   458,   238,   462,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   321,   254,   255,   256,     0,     0,   257,
     258,   259,   165,     0,   282,     0,     0,   110,   110,   110,
     110,     0,     0,   294,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,   110,     0,     0,     0,    74,     0,
     111,     0,   111,     0,     0,   111,     0,     0,   111,     0,
       0,   111,     0,     0,   165,     0,     0,     0,     0,     0,
       0,     0,   338,     0,     0,   338,     0,     0,     0,     0,
       0,   111,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   111,     0,   111,     0,     0,     0,     0,   111,
     486,     0,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   127,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,   520,     0,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
     294,     0,    38,     0,    40,     0,     0,     0,     0,   111,
       0,     0,     0,     0,   111,     0,    48,    49,    50,   131,
       0,     0,     0,   602,     0,     0,     0,   321,     0,     0,
       0,     0,   111,    57,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,     0,   111,     0,   195,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,     0,     0,     0,    68,   673,     0,   132,     0,
      70,     0,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   598,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,   111,   111,   111,     0,     0,     0,     0,     0,
       0,     0,   458,     0,     0,     0,     0,   111,   111,     0,
       0,     0,     5,     6,     7,     8,     9,    10,   631,     0,
       0,   127,    74,     0,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   740,    48,    49,    50,   131,
       0,   338,     0,   338,     0,     0,   338,     0,     0,   338,
       0,     0,   706,     0,  -276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,   718,  -276,     0,     0,     0,  -276,     0,     0,
       0,     0,     0,   724,     0,   728,     0,    64,     0,     0,
      74,   179,     0,     0,     0,     0,     0,     0,   132,     0,
      70,  -276,   764,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   125,     0,     0,     0,
       0,     0,    -2,     4,   785,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -249,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -142,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,   776,  -249,    38,    39,    40,    41,  -142,
      42,    43,   780,  -249,    44,    45,   782,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,  -249,     0,
       0,    55,     0,    56,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,    67,     0,     0,    68,     0,
       0,    69,     0,    70,     4,    71,     5,     6,     7,     8,
       9,    10,     0,  -398,     0,    11,    12,    13,  -398,    14,
      15,    16,    17,  -249,    18,    19,  -398,    20,    21,  -398,
      22,     0,    23,    24,    25,    26,    27,     0,    28,    29,
       0,  -142,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,     0,  -249,    38,     0,    40,    41,
    -142,    42,    43,  -398,  -249,    44,    45,  -398,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -249,
       0,     0,    55,     0,    56,     0,     0,  -398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -398,    59,  -398,  -398,  -398,  -398,  -398,  -398,
    -398,     0,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,
    -398,    64,  -398,  -398,  -398,     0,    67,  -398,  -398,  -398,
       0,     0,    69,  -398,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -249,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -142,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,     0,  -249,    38,    39,    40,
      41,  -142,    42,    43,     0,  -249,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
    -249,     0,     0,    55,     0,    56,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,   179,    67,     0,     0,
      68,     0,     0,    69,   206,    70,     4,    71,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -249,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -142,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,     0,  -249,    38,    39,
      40,    41,  -142,    42,    43,     0,  -249,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -249,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,    69,   350,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -249,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -142,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,     0,  -249,    38,
      39,    40,    41,  -142,    42,    43,     0,  -249,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,  -249,     0,     0,    55,     0,    56,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,    67,
       0,     0,    68,     0,     0,    69,   206,    70,     4,    71,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -249,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,     0,    28,    29,     0,  -142,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,     0,     0,     0,  -249,
      38,    39,    40,    41,  -142,    42,    43,     0,  -249,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -249,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,    69,   633,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -249,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -142,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -249,    38,    39,    40,    41,  -142,    42,    43,     0,  -249,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -249,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,   732,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -249,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,    27,     0,    28,    29,     0,  -142,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
       0,  -249,    38,   219,    40,    41,  -142,    42,    43,     0,
    -249,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -249,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,    67,     0,     0,    68,     0,     0,    69,     0,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -249,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -142,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,     0,  -249,    38,     0,    40,    41,  -142,    42,    43,
       0,  -249,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,  -249,     0,     0,    55,
       0,    56,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,    67,     0,     0,    68,     0,     0,    69,
       0,    70,     4,    71,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -249,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,   465,     0,    28,    29,     0,  -142,
      30,    31,    32,    33,    34,   -37,    35,    36,     0,    37,
       0,     0,     0,  -249,    38,     0,    40,    41,  -142,    42,
      43,     0,  -249,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -249,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
      69,     0,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -249,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,   469,     0,    28,    29,     0,
    -142,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,     0,  -249,    38,     0,    40,    41,  -142,
      42,    43,     0,  -249,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,  -249,     0,
       0,    55,     0,    56,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,    67,     0,     0,    68,     0,
       0,    69,     0,    70,     4,    71,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -249,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,   695,     0,    28,    29,
       0,  -142,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,     0,  -249,    38,     0,    40,    41,
    -142,    42,    43,     0,  -249,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -249,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,    69,     0,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -249,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,   697,     0,    28,
      29,     0,  -142,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,     0,  -249,    38,     0,    40,
      41,  -142,    42,    43,     0,  -249,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
    -249,     0,     0,    55,     0,    56,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,    67,     0,     0,
      68,     0,     0,    69,     0,    70,     4,    71,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -249,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,   700,     0,
      28,    29,     0,  -142,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,     0,  -249,    38,     0,
      40,    41,  -142,    42,    43,     0,  -249,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -249,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,    69,     0,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -249,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,   703,
       0,    28,    29,     0,  -142,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,     0,  -249,    38,
       0,    40,    41,  -142,    42,    43,     0,  -249,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,  -249,     0,     0,    55,     0,    56,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,    67,
       0,     0,    68,     0,     0,    69,     0,    70,     4,    71,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -249,    18,    19,
       0,   717,    21,     0,    22,     0,    23,    24,    25,    26,
      27,     0,    28,    29,     0,  -142,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,     0,     0,     0,  -249,
      38,     0,    40,    41,  -142,    42,    43,     0,  -249,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -249,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,    69,     0,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -249,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   723,     0,    28,    29,     0,  -142,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -249,    38,     0,    40,    41,  -142,    42,    43,     0,  -249,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -249,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -249,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,   727,     0,    28,    29,     0,  -142,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
       0,  -249,    38,     0,    40,    41,  -142,    42,    43,     0,
    -249,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -249,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,   486,     0,     5,
       6,     7,     8,     9,    10,    63,  -398,    64,   127,    65,
      66,  -398,    67,     0,     0,    68,     0,     0,    69,  -398,
      70,    21,    71,     0,     0,     0,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,  -398,     0,     0,     0,
    -398,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -267,
    -398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -398,    59,  -398,  -398,  -398,
       0,  -398,  -398,  -398,     0,  -398,  -398,  -398,  -398,  -398,
    -398,  -398,  -398,  -398,    64,  -398,  -398,  -398,     0,     0,
    -398,  -398,  -398,     0,     0,   132,     0,    70,   486,   501,
       5,     6,     7,     8,     9,    10,     0,  -398,     0,   127,
       0,     0,  -398,     0,     0,     0,     0,     0,     0,     0,
    -398,     0,    21,     0,     0,     0,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,  -398,     0,     0,
       0,  -398,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -398,    59,  -398,  -398,
    -398,     0,  -398,  -398,  -398,     0,  -398,  -398,  -398,  -398,
    -398,  -398,  -398,  -398,  -398,    64,  -398,  -398,  -398,     0,
       0,  -398,  -398,  -398,     0,     0,   132,     0,    70,   486,
     501,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     5,     6,     7,     8,
       9,    10,     0,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   501,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     5,     6,     7,     8,     9,    10,     0,     0,    63,
     127,    64,     0,    65,    66,   179,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,   519,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     5,     6,     7,     8,
       9,    10,     0,     0,    63,   127,    64,     0,    65,    66,
     179,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,   801,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     5,     6,     7,     8,     9,    10,     0,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,   430,    68,
       0,     0,   132,    21,    70,     0,   133,   524,     0,   128,
     129,   130,   431,    28,     0,   432,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,   433,
     434,    38,     0,    40,     0,     0,     0,     0,     0,   435,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     5,     6,     7,     8,
       9,    10,     0,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,   210,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,   211,     0,     5,     6,     7,     8,     9,    10,    63,
       0,    64,   127,    65,    66,   179,     0,     0,     0,    68,
       0,     0,   132,     0,    70,    21,   133,     0,     0,     0,
       0,   128,   129,   130,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,   210,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     5,     6,
       7,     8,     9,    10,     0,     0,    63,   127,    64,     0,
      65,    66,   179,     0,     0,     0,    68,     0,     0,   132,
      21,    70,   513,   133,     0,     0,   128,   129,   130,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,   210,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     5,     6,     7,     8,     9,    10,     0,
       0,    63,   127,    64,     0,    65,    66,   179,     0,     0,
       0,    68,     0,     0,   132,    21,    70,   515,   133,     0,
       0,   128,   129,   130,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,   155,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   156,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     5,     6,
       7,     8,     9,    10,     0,     0,    63,   127,    64,     0,
      65,    66,     0,     0,     0,     0,    68,     0,     0,   132,
      21,    70,     0,   133,     0,     0,   128,   129,   130,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     5,     6,     7,     8,     9,
      10,    63,     0,    64,   127,    65,    66,   179,     0,     0,
       0,    68,     0,     0,   132,   511,    70,    21,   133,     0,
       0,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,   179,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   133,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
     183,     0,     0,    68,     0,     0,   132,     0,    70,     0,
     133,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   186,    21,     0,     0,     0,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     5,     6,     7,     8,
       9,    10,     0,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,    69,    21,    70,
       0,   133,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   156,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     5,     6,     7,     8,     9,    10,     0,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,   614,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     5,     6,     7,     8,
       9,    10,     0,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,   761,    70,
       0,   133,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     762,    49,    50,   763,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     5,     6,     7,     8,     9,    10,     0,     0,    63,
     127,    64,     0,    65,    66,   179,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   764,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     5,     6,     7,     8,
       9,    10,     0,     0,    63,   127,    64,     0,    65,    66,
     179,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   764,     0,     0,   128,   129,   752,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     5,     6,     7,     8,     9,    10,     0,     0,    63,
     127,    64,     0,    65,    66,   179,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     5,     6,     7,     8,
       9,    10,     0,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     5,     6,     7,     8,     9,    10,     0,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,     0,   505,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   563,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     5,     6,     7,     8,
       9,    10,     0,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,     0,     0,   128,   129,   566,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     5,     6,     7,     8,     9,    10,     0,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   746,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     5,     6,     7,     8,
       9,    10,     0,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,     0,     0,   128,   129,   747,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     5,     6,     7,     8,     9,    10,     0,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   749,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     5,     6,     7,     8,
       9,    10,     0,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,     0,     0,   128,   129,   750,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     5,     6,     7,     8,     9,    10,     0,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   751,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     5,     6,     7,     8,
       9,    10,     0,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,     0,     0,   128,   129,   752,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     5,     6,     7,     8,     9,    10,     0,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       5,    38,     0,    40,     0,     0,     0,   637,     0,     0,
       0,     0,   638,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,     0,    66,
       0,     0,     0,     0,    68,     0,     0,   132,     0,    70,
       0,   133,   639,   640,   641,   642,   643,   644,     0,     0,
     645,   646,   647,   648,   649,   650,   651,   652,   653,   654,
       0,     0,   655,     0,     0,     0,   656,   657,   658,   659,
     660,   661,   662,   663,   664,   665,   666,     0,   667,     0,
       0,   668,   669,   670,   671,   672,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   235,     0,     0,     0,
     236,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   232,     0,   121,     0,     0,   233,
     237,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     235,     0,     0,     0,   236,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   237,   254,   255,   256,     0,     0,
     257,   258,   259,     0,   235,     0,     0,     0,   236,   238,
     629,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   237,   254,
     255,   256,     0,     0,   257,   258,   259,     0,     0,   234,
       0,     0,     0,   238,   687,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   232,   254,   255,   256,     0,   233,   257,   258,
     259,     0,     0,     0,     0,   234,   235,     0,   810,     0,
     236,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
     237,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   235,     0,     0,   238,   236,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,     0,
     250,   251,   252,   253,     0,   254,   237,   256,     0,     0,
     257,   258,   259,     0,     0,     0,   235,     0,     0,     0,
     236,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     237,   254,   255,   256,     0,     0,   257,   258,   259,     0,
       0,     0,     0,     0,     0,   238,   845,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,     0,
     257,   258,   259,   233,     0,     0,     0,     0,   512,     0,
       0,   234,   309,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   310,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,   306,     0,   235,     0,
       0,     0,   236,     0,     0,     0,   307,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   121,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   235,     0,     0,     0,   236,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   237,   254,   255,   256,
       0,     0,   257,   258,   259,     0,   235,   282,     0,     0,
     236,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     232,   254,   255,   256,     0,   233,   257,   258,   259,     0,
       0,   282,     0,   234,   593,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,     0,
     250,   251,   252,   253,   232,   254,     0,   256,     0,   233,
     257,   258,   259,     0,     0,     0,     0,   234,   186,     0,
     235,     0,     0,     0,   236,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   232,     0,
     121,     0,     0,   233,   237,     0,     0,     0,     0,     0,
       0,   234,   587,     0,   235,     0,     0,     0,   236,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   237,   254,
     255,   256,     0,     0,   257,   258,   259,   234,   235,   282,
       0,     0,   236,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   237,   254,   255,   256,     0,     0,   257,   258,
     259,     0,     0,   282,   235,     0,     0,   238,   236,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   232,   254,   255,   256,
       0,   233,   257,   258,   259,     0,     0,   282,     0,   234,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,     0,   246,   247,   248,     0,   250,   251,
     252,   253,     0,     0,     0,   256,     0,     0,   257,   258,
     259,     0,     0,     0,     0,     0,   235,     0,     0,     0,
     236,     0,     0,     0,     0,     0,     0,   694,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     237,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,   282,     0,     0,     0,     0,
     699,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   237,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,   282,     0,
       0,     0,     0,   796,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   237,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,   282,     0,     0,     0,     0,   797,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,   237,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,   282,     0,     0,     0,     0,   798,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,   237,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,   282,     0,     0,
       0,     0,   799,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,   237,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   235,   257,   258,   259,   236,     0,
     282,     0,     0,     0,     0,   812,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   237,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,   282,     0,   232,     0,     0,   813,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
       0,   257,   258,   259,     0,     0,   282,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,   237,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,   337,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     288,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,     0,   257,   258,   259,     0,  -358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   121,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   232,   254,   255,   256,     0,   233,
     257,   258,   259,     0,     0,     0,     0,   234,     0,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,   232,     0,
       0,     0,     0,   233,   235,     0,     0,     0,   236,     0,
       0,   234,   442,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   443,     0,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,   235,   242,
     243,   244,   236,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,     0,   257,   258,
     259,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   232,   254,   255,   256,
       0,   233,   257,   258,   259,     0,     0,     0,     0,   234,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     445,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   235,     0,     0,     0,
     236,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,   233,
     237,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     235,     0,     0,     0,   236,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   237,   254,   255,   256,     0,     0,
     257,   258,   259,     0,   235,     0,     0,     0,   236,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   237,   254,
     255,   256,     0,   292,   257,   258,   259,     0,     0,   232,
       0,     0,     0,   238,   233,   239,   240,   241,     0,   242,
     243,   244,   234,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   336,   257,   258,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,     0,   233,   235,
       0,     0,     0,   236,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,   235,   242,   243,   244,   236,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   349,   257,   258,   259,   363,   237,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,     0,   257,   258,   259,
       0,     0,   232,     0,     0,     0,     0,   233,   235,     0,
       0,     0,   236,     0,     0,   234,     0,     0,     0,   446,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,   235,   242,   243,   244,   236,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
     232,     0,   257,   258,   259,   233,   237,     0,     0,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,   539,   257,   258,   259,   233,
     235,     0,     0,     0,   236,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,   235,   242,   243,   244,   236,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,   540,   257,   258,   259,   233,   237,     0,
       0,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   541,   257,   258,
     259,   233,   235,     0,     0,     0,   236,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,   235,   242,   243,   244,
     236,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,   542,   257,   258,   259,   233,
     237,     0,     0,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,   543,
     257,   258,   259,   233,   235,     0,     0,     0,   236,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,   235,   242,
     243,   244,   236,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   544,   257,   258,
     259,   233,   237,     0,     0,     0,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
     232,   545,   257,   258,   259,   233,   235,     0,     0,     0,
     236,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
     235,   242,   243,   244,   236,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,   546,
     257,   258,   259,   233,   237,     0,     0,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,   547,   257,   258,   259,   233,   235,     0,
       0,     0,   236,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,   235,   242,   243,   244,   236,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
     232,   548,   257,   258,   259,   233,   237,     0,     0,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,   549,   257,   258,   259,   233,
     235,     0,     0,     0,   236,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,   235,   242,   243,   244,   236,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,   550,   257,   258,   259,   233,   237,     0,
       0,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   551,   257,   258,
     259,   233,   235,     0,     0,     0,   236,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,   235,   242,   243,   244,
     236,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,   552,   257,   258,   259,   233,
     237,     0,     0,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,   553,
     257,   258,   259,   233,   235,     0,     0,     0,   236,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   121,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,   235,   242,
     243,   244,   236,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,     0,   257,   258,
     259,   233,   237,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   597,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   573,   257,   258,   259,     0,   235,     0,     0,     0,
     236,     0,     0,     0,   232,   625,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,     0,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   237,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,     0,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,   682,
     632,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
       0,   257,   258,   259,     0,     0,     0,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,   237,
       0,   234,   685,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,     0,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,     0,   257,   258,   259,     0,     0,   745,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     237,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,     0,     0,   232,   778,     0,
     748,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,     0,     0,     0,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,   237,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,     0,     0,   232,     0,
       0,   818,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,     0,   257,   258,   259,     0,     0,   819,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,   237,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,     0,     0,   232,
       0,     0,   820,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,     0,   257,   258,   259,     0,     0,
     821,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   237,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,     0,     0,
     232,     0,     0,   822,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,     0,   257,   258,   259,     0,
       0,   823,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,   237,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   235,   257,   258,   259,   236,     0,     0,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   561,   254,   255,   256,   236,     0,   257,   258,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,     0,   257,   258,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,     0,   250,   251,   252,   253,     0,   254,   255,
     256,     0,     0,   257,   258,   259
};

static const yytype_int16 yycheck[] =
{
      11,    41,    42,     2,   320,    16,    15,   342,    19,   159,
     143,   279,   325,   611,    25,    26,    27,   485,   161,    23,
     153,   279,    24,    34,    35,     3,    37,    38,     3,   342,
       3,   174,     1,    44,    45,    46,    47,    48,     1,     3,
      51,   635,     1,    54,    55,    56,    44,   635,    59,   192,
     675,    62,    63,    64,     3,    66,    54,    50,    69,    70,
      71,   226,   227,   228,   229,   772,   216,   104,     1,    24,
      24,    20,    24,    71,    80,    34,    13,    34,    80,   104,
     104,    18,     3,   104,    77,    34,    61,    38,    37,    63,
      80,   801,     3,    72,    63,    73,     7,   134,   123,     0,
      63,    34,    51,    52,    25,    23,    57,   814,   132,    78,
      34,   132,    61,   711,    34,    78,   127,   128,   129,   130,
     131,   132,   133,    72,    83,   835,    85,   129,   134,    63,
     139,   835,   275,   276,   103,   133,   147,   130,     7,   607,
     103,   845,    60,   737,   103,   104,    64,   104,   104,   104,
     161,    72,    85,   131,   123,   129,   131,   104,   131,   753,
     129,   786,   787,   174,   123,   753,   129,   131,   484,    34,
     103,   104,   176,   104,   129,   129,   132,   129,     3,   104,
     104,   192,   131,   130,   104,   103,   131,   105,   106,   123,
     123,   104,   110,    85,   104,   129,    61,   340,   104,   117,
     211,   132,   800,   131,    34,    34,   371,   132,   126,   208,
     134,    25,   131,   123,   134,    69,   108,   123,    44,   132,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   131,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,    34,
      16,   739,   104,    19,   104,   104,    16,   288,   131,    23,
     133,    27,   293,    29,    30,   601,    20,   131,   104,   133,
      36,   559,    38,   304,   128,   306,   307,   610,   309,   310,
     132,   559,   313,   123,   134,   134,   304,   123,    54,   131,
      56,    57,    58,    59,    85,    80,    60,   328,    52,    59,
      64,   471,   472,   334,   104,   123,    72,    61,    74,   340,
     131,    34,   133,   104,    74,   346,   347,   108,    70,   104,
     123,   352,    76,   123,   355,   104,   357,     2,   359,   360,
     361,     3,   363,   364,   104,     7,    11,   368,    20,   103,
      63,   132,   106,   123,    85,    20,   110,   517,   518,   134,
     368,   504,   123,   132,   104,    78,   104,   123,    33,    79,
     130,   716,    85,   104,   130,   720,    38,   108,   123,    52,
      52,   129,   123,   123,   104,   123,    51,   710,    61,    61,
     131,   104,   133,   716,    60,    57,   123,   720,    64,    72,
      72,   132,    23,   123,    69,   147,   103,   123,    73,   106,
     123,   235,   236,   110,    60,    83,   129,    85,    64,   161,
     123,   442,   443,   444,   445,   446,   447,   448,     2,   129,
     449,   450,   174,   454,   455,   131,   457,    11,   355,    60,
     357,   131,   463,    64,   465,   106,    20,   131,   469,    85,
     192,     3,   123,   277,   278,     7,     7,   122,   103,    33,
     123,   104,   132,   123,   288,    34,   487,    26,    20,   490,
     491,   104,    83,   132,   132,    85,    23,    51,   801,   487,
     501,   132,   103,   307,   105,   106,   310,   810,   132,   110,
     104,    19,   134,   501,   129,    69,   117,   162,   129,    73,
      52,    29,    30,   124,   125,   126,   132,   134,    36,    61,
      38,    26,   835,    60,    85,   132,    44,    64,   123,   132,
      72,   186,   845,   104,    76,   123,   123,   104,    24,    57,
      24,    34,   134,   275,   276,   359,   129,   103,   559,    61,
     561,   562,   563,   208,   134,   566,   134,   131,   122,   106,
     132,    78,    99,    23,   101,   102,   103,   578,   105,   106,
      63,   347,   154,   110,   585,   462,   587,   692,   526,   590,
     117,   741,   593,   579,   741,    78,   597,   124,   125,   126,
     742,   599,    85,   636,   814,   800,   784,   760,   162,   457,
      60,   559,   334,   329,    64,    -1,   617,    -1,   340,    -1,
      -1,   104,   623,    -1,   625,    -1,   627,    -1,   629,   617,
     352,   632,   186,   355,    -1,   357,    -1,   282,   360,   443,
     123,   445,    -1,    -1,    -1,   634,   129,    -1,    -1,    99,
      -1,   101,   102,   103,   208,   105,   106,   107,    -1,    -1,
     110,   306,    47,    -1,   309,    -1,   116,   117,   313,     2,
      -1,   121,    -1,    -1,   124,   125,   126,    -1,    11,    -1,
     681,   682,    -1,   684,   685,   686,   687,    20,    -1,    -1,
      -1,    -1,    -1,   694,   695,    -1,   697,    -1,   699,   700,
      33,    -1,   703,    -1,   734,   735,    -1,    -1,    -1,    -1,
      -1,   157,   158,   159,   715,   716,   361,    -1,    51,   720,
      -1,    -1,   723,    -1,    -1,   171,   727,    -1,   282,    -1,
      -1,    -1,    -1,   455,    -1,   457,    69,    -1,    -1,    -1,
      73,   126,    -1,   189,   745,   746,   747,   748,   749,   750,
     751,   752,   306,   199,    -1,   309,    -1,   561,   562,   313,
      -1,   762,   763,   764,    -1,    -1,    -1,   768,    19,    -1,
     771,    -1,   157,   158,   159,   160,   764,   778,    29,    30,
      -1,    -1,    -1,    -1,    -1,    36,   171,    38,    -1,   122,
      -1,    -1,   437,    44,    -1,   796,   797,   798,   799,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    57,   361,    -1,   810,
      -1,   812,   813,    -1,   199,    -1,    -1,   818,   819,   820,
     821,   822,   823,     2,    -1,    -1,    -1,    -1,    -1,   162,
      -1,   476,    11,   834,   835,    -1,    -1,    -1,    -1,    -1,
      34,    20,    -1,   289,   845,    16,    -1,    -1,    19,    -1,
      -1,     3,    -1,   186,    33,     7,    27,     9,    29,    30,
      -1,    -1,    -1,    -1,    -1,    36,   312,    38,    20,    63,
      -1,    -1,    51,    -1,    -1,   208,    -1,   522,    -1,    -1,
      -1,   526,    20,    54,    78,    56,    57,    58,    59,    -1,
      69,    85,    -1,    -1,    73,    -1,    34,    -1,    -1,    37,
      52,    72,    -1,    74,   289,    -1,    -1,    -1,    -1,    61,
     104,    -1,    -1,    51,    52,    -1,   561,   562,    -1,    -1,
      72,    -1,   476,    61,    76,    -1,    -1,   312,    -1,   123,
      -1,    -1,    -1,    -1,    72,   129,    19,    -1,    -1,    -1,
     585,    -1,   587,   122,    -1,   590,    29,    30,   593,   282,
      -1,   596,   123,    36,    -1,    38,    -1,    -1,    -1,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   522,    -1,
      -1,   616,   526,   306,    57,    -1,   309,    -1,    -1,    -1,
     313,    -1,   627,   162,   629,    -1,    -1,    -1,   710,   634,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    11,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    20,    -1,    -1,
      -1,   467,   468,    -1,    -1,   471,   472,    -1,   361,   208,
      33,   585,    -1,   587,    -1,    -1,   590,    -1,    -1,   593,
      -1,    -1,   596,    -1,    -1,    -1,   768,    -1,    51,   694,
      -1,    60,    -1,    -1,   699,    64,    -1,    -1,    -1,    -1,
      -1,    -1,   616,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      73,    -1,   717,   627,    -1,   629,    -1,    -1,    -1,   801,
     634,   726,   467,   468,    -1,   730,   471,   472,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,    -1,    -1,
      -1,   110,    -1,   282,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,   121,   835,    -1,   124,   125,   126,    -1,   122,
      -1,    -1,    -1,   845,    -1,   571,   572,   306,    -1,    -1,
     309,    -1,    -1,    23,   313,    -1,   582,    -1,    -1,    -1,
     694,    -1,    -1,   476,    -1,   699,   592,     2,    -1,   595,
      -1,   796,   797,   798,   799,    -1,    11,    -1,    -1,   162,
      -1,    -1,    -1,   717,    -1,    20,    -1,   812,   813,    -1,
      60,    -1,   726,    -1,    64,    -1,   730,    -1,    33,    -1,
      -1,    16,   361,   186,    19,    -1,   571,   572,    -1,   522,
      -1,    -1,    27,   526,    29,    30,    51,   582,    -1,   584,
      -1,    36,    -1,    38,   589,   208,    -1,   592,    -1,    99,
     595,    -1,   102,   103,    69,   105,   106,    -1,    73,    54,
     110,    56,    57,    58,    59,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,    72,    -1,    74,
      -1,    -1,   796,   797,   798,   799,    -1,    -1,    -1,    -1,
      -1,    -1,   585,    -1,   587,    -1,    -1,   590,   812,   813,
     593,    -1,    -1,   596,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,   719,    -1,    -1,    -1,    -1,    -1,   282,
      -1,    -1,    -1,   616,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,     2,   627,    -1,   629,   476,    -1,    -1,
      -1,   634,    11,   306,    -1,    -1,   309,   162,    -1,    -1,
     313,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,   719,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,   522,    -1,    -1,    -1,   526,    -1,    -1,
      -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,   361,    60,
      69,   694,    -1,    64,    73,    -1,   699,    -1,    -1,    -1,
     755,   756,    -1,   758,   759,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   717,    -1,    -1,   772,    -1,    -1,
      -1,    -1,    -1,   726,   779,    -1,   781,   730,    99,    -1,
      -1,    -1,   103,    -1,   105,   106,   585,    -1,   587,   110,
      -1,   590,    -1,   122,   593,    -1,   117,   596,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,    -1,   282,    -1,   814,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   616,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   627,    -1,
     629,   306,    -1,   162,   309,   634,    -1,    -1,   313,    -1,
      -1,    -1,    -1,   796,   797,   798,   799,    -1,    -1,    -1,
      -1,    -1,    -1,   476,    -1,    -1,    -1,   186,    -1,   812,
     813,    -1,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,    -1,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,   361,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   694,    -1,    -1,    -1,   522,
     699,    -1,    -1,   526,   127,   128,    -1,     2,   131,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    11,    -1,   717,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    -1,   726,    -1,    -1,
      -1,   730,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   282,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,   585,    -1,   587,    -1,    -1,   590,    -1,    -1,
     593,    -1,    -1,   596,    69,    -1,    -1,   306,    73,    -1,
     309,    -1,    -1,    -1,   313,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   616,    -1,    -1,    -1,   796,   797,   798,
     799,   476,    -1,    -1,   627,    -1,   629,    -1,    -1,    -1,
      -1,   634,    -1,   812,   813,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,   122,    -1,    12,
      -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,   522,    31,    32,
      33,   526,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,   162,    -1,    -1,
      53,   694,    55,    -1,    -1,    -1,   699,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,   186,    -1,    -1,   717,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   726,    -1,    -1,    -1,   730,    -1,    -1,
     585,    -1,   587,   208,    -1,   590,    -1,   100,   593,    -1,
      -1,   596,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,   616,    -1,    -1,    -1,    -1,    13,   476,   131,    -1,
     133,    18,   627,    -1,   629,    22,    23,    24,    -1,   634,
      -1,    -1,    -1,    30,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   796,   797,   798,   799,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    -1,   282,    -1,   812,
     813,    -1,    -1,   522,    -1,    -1,    -1,   526,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,
      -1,   306,    -1,    -1,   309,    -1,    -1,    11,   313,   694,
      -1,    -1,    -1,    90,   699,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,   717,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   726,    -1,    -1,    -1,   730,   585,    51,   587,    -1,
      -1,   590,    -1,    -1,   593,    -1,   361,   596,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,   143,    -1,    -1,    73,
     147,    -1,    -1,    -1,    -1,    -1,   153,   616,   155,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   627,    -1,
     629,    -1,    -1,    -1,    -1,   634,    -1,   174,    -1,   176,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   796,   797,   798,   799,   192,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   812,   813,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,   221,    -1,    -1,    -1,    23,   226,
     227,   228,   229,    -1,    -1,   694,    -1,    -1,   162,    -1,
     699,    -1,    -1,    -1,    -1,    -1,   243,    -1,    -1,    -1,
      -1,   476,    -1,    -1,    -1,    -1,    -1,    -1,   717,    -1,
      -1,    -1,   186,    -1,    -1,    60,    -1,   726,    -1,    64,
      -1,   730,    -1,    -1,    -1,    -1,    71,    -1,   275,   276,
      -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   522,    -1,    -1,
      -1,   526,    -1,   300,    99,   302,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   320,   119,   120,   121,    -1,    -1,   124,
     125,   126,   329,    -1,   129,    -1,    -1,   796,   797,   798,
     799,    -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   812,   813,    -1,    -1,    -1,   282,    -1,
     585,    -1,   587,    -1,    -1,   590,    -1,    -1,   593,    -1,
      -1,   596,    -1,    -1,   371,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   306,    -1,    -1,   309,    -1,    -1,    -1,    -1,
      -1,   616,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   627,    -1,   629,    -1,    -1,    -1,    -1,   634,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,   361,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
     457,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,   694,
      -1,    -1,    -1,    -1,   699,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,   480,    -1,    -1,    -1,   484,    -1,    -1,
      -1,    -1,   717,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   726,    -1,    -1,    -1,   730,    -1,   504,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,    -1,    -1,    -1,    -1,   126,   533,    -1,   129,    -1,
     131,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   476,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   796,   797,   798,   799,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   579,    -1,    -1,    -1,    -1,   812,   813,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   522,    -1,
      -1,    12,   526,    -1,   601,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   652,    67,    68,    69,    70,
      -1,   585,    -1,   587,    -1,    -1,   590,    -1,    -1,   593,
      -1,    -1,   596,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,    -1,   616,   104,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,   627,    -1,   629,    -1,   118,    -1,    -1,
     634,   122,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   733,    -1,    -1,    -1,
      -1,    -1,     0,     1,   741,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,   717,    52,    53,    54,    55,    56,    57,
      58,    59,   726,    61,    62,    63,   730,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
     118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,   129,    -1,   131,     1,   133,     3,     4,     5,     6,
       7,     8,    -1,    10,    -1,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    -1,    74,    75,    76,
      -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,   103,   104,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,   123,   124,   125,   126,
      -1,    -1,   129,   130,   131,     1,   133,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,   122,   123,    -1,    -1,
     126,    -1,    -1,   129,   130,   131,     1,   133,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,   129,   130,   131,     1,   133,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,   129,   130,   131,     1,   133,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    54,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,   129,   130,   131,     1,
     133,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,   129,   130,   131,
       1,   133,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    54,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,
      81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,   129,    -1,
     131,     1,   133,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,
      -1,    81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,   129,
      -1,   131,     1,   133,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    72,    -1,    74,    75,    76,    -1,    -1,
      79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
     129,    -1,   131,     1,   133,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
     118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,   129,    -1,   131,     1,   133,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    -1,    72,    -1,    74,    75,    76,
      -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,   129,    -1,   131,     1,   133,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,   129,    -1,   131,     1,   133,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,   129,    -1,   131,     1,   133,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,   129,    -1,   131,     1,   133,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,   129,    -1,   131,     1,
     133,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,   129,    -1,   131,
       1,   133,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,
      81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,     1,    -1,     3,
       4,     5,     6,     7,     8,   116,    10,   118,    12,   120,
     121,    15,   123,    -1,    -1,   126,    -1,    -1,   129,    23,
     131,    25,   133,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,    -1,
     124,   125,   126,    -1,    -1,   129,    -1,   131,     1,   133,
       3,     4,     5,     6,     7,     8,    -1,    10,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    60,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,   124,   125,   126,    -1,    -1,   129,    -1,   131,     1,
     133,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,
       7,     8,    -1,    -1,   116,    12,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,
      -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,     3,     4,     5,     6,     7,     8,    -1,    -1,   116,
      12,   118,    -1,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,    -1,   129,    25,   131,    -1,   133,   134,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,
       7,     8,    -1,    -1,   116,    12,   118,    -1,   120,   121,
     122,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,
      -1,   133,   134,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,     3,     4,     5,     6,     7,     8,    -1,    -1,   116,
      12,   118,    -1,   120,   121,    -1,    -1,    -1,    20,   126,
      -1,    -1,   129,    25,   131,    -1,   133,   134,    -1,    31,
      32,    33,    34,    35,    -1,    37,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    51,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,
       7,     8,    -1,    -1,   116,    12,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,
      -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,   108,    -1,     3,     4,     5,     6,     7,     8,   116,
      -1,   118,    12,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,    -1,   129,    -1,   131,    25,   133,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,     4,
       5,     6,     7,     8,    -1,    -1,   116,    12,   118,    -1,
     120,   121,   122,    -1,    -1,    -1,   126,    -1,    -1,   129,
      25,   131,   132,   133,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,     3,     4,     5,     6,     7,     8,    -1,
      -1,   116,    12,   118,    -1,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,    -1,   129,    25,   131,   132,   133,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,     4,
       5,     6,     7,     8,    -1,    -1,   116,    12,   118,    -1,
     120,   121,    -1,    -1,    -1,    -1,   126,    -1,    -1,   129,
      25,   131,    -1,   133,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,    -1,     3,     4,     5,     6,     7,
       8,   116,    -1,   118,    12,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,    -1,   129,   130,   131,    25,   133,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,   122,    -1,    -1,    -1,   126,    -1,
      -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,   129,    -1,   131,    -1,
     133,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,
       7,     8,    -1,    -1,   116,    12,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,
      -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,     3,     4,     5,     6,     7,     8,    -1,    -1,   116,
      12,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,
      -1,    -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    47,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,
       7,     8,    -1,    -1,   116,    12,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,
      -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,     3,     4,     5,     6,     7,     8,    -1,    -1,   116,
      12,   118,    -1,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,    -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,
       7,     8,    -1,    -1,   116,    12,   118,    -1,   120,   121,
     122,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,
      -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,     3,     4,     5,     6,     7,     8,    -1,    -1,   116,
      12,   118,    -1,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,    -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,
       7,     8,    -1,    -1,   116,    12,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,
      -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,     3,     4,     5,     6,     7,     8,    -1,    -1,   116,
      12,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,
      -1,    -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,
       7,     8,    -1,    -1,   116,    12,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,
      -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,     3,     4,     5,     6,     7,     8,    -1,    -1,   116,
      12,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,
      -1,    -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,
       7,     8,    -1,    -1,   116,    12,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,
      -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,     3,     4,     5,     6,     7,     8,    -1,    -1,   116,
      12,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,
      -1,    -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,
       7,     8,    -1,    -1,   116,    12,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,
      -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,     3,     4,     5,     6,     7,     8,    -1,    -1,   116,
      12,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,
      -1,    -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,
       7,     8,    -1,    -1,   116,    12,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,
      -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,     3,     4,     5,     6,     7,     8,    -1,    -1,   116,
      12,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,
      -1,    -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
       3,    53,    -1,    55,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    -1,   131,
      -1,   133,    85,    86,    87,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    -1,   121,    -1,
      -1,   124,   125,   126,   127,   128,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    80,    -1,    -1,    15,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      60,    -1,    -1,    -1,    64,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    84,   119,   120,   121,    -1,    -1,
     124,   125,   126,    -1,    60,    -1,    -1,    -1,    64,    99,
     134,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    84,   119,
     120,   121,    -1,    -1,   124,   125,   126,    -1,    -1,    23,
      -1,    -1,    -1,    99,   134,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    10,   119,   120,   121,    -1,    15,   124,   125,
     126,    -1,    -1,    -1,    -1,    23,    60,    -1,   134,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      84,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    60,    -1,    -1,    99,    64,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,    -1,
     114,   115,   116,   117,    -1,   119,    84,   121,    -1,    -1,
     124,   125,   126,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      64,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      84,   119,   120,   121,    -1,    -1,   124,   125,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   134,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    10,    -1,
     124,   125,   126,    15,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    24,    -1,    60,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    60,    -1,    -1,    -1,    64,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    84,   119,   120,   121,
      -1,    -1,   124,   125,   126,    -1,    60,   129,    -1,    -1,
      64,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      10,   119,   120,   121,    -1,    15,   124,   125,   126,    -1,
      -1,   129,    -1,    23,    24,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,    -1,
     114,   115,   116,   117,    10,   119,    -1,   121,    -1,    15,
     124,   125,   126,    -1,    -1,    -1,    -1,    23,    24,    -1,
      60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      80,    -1,    -1,    15,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    -1,    60,    -1,    -1,    -1,    64,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    84,   119,
     120,   121,    -1,    -1,   124,   125,   126,    23,    60,   129,
      -1,    -1,    64,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    84,   119,   120,   121,    -1,    -1,   124,   125,
     126,    -1,    -1,   129,    60,    -1,    -1,    99,    64,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    10,   119,   120,   121,
      -1,    15,   124,   125,   126,    -1,    -1,   129,    -1,    23,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,    -1,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,    -1,    -1,   121,    -1,    -1,   124,   125,
     126,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,    60,
     124,   125,   126,    64,    -1,   129,    -1,    -1,    -1,    -1,
      71,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,    60,   124,   125,   126,    64,    -1,   129,    -1,
      -1,    -1,    -1,    71,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,    60,   124,   125,   126,    64,
      -1,   129,    -1,    -1,    -1,    -1,    71,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,    60,   124,
     125,   126,    64,    -1,   129,    -1,    -1,    -1,    -1,    71,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,    60,   124,   125,   126,    64,    -1,   129,    -1,    -1,
      -1,    -1,    71,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,    60,   124,   125,   126,    64,    -1,
     129,    -1,    -1,    -1,    -1,    71,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    60,   124,   125,
     126,    64,    -1,   129,    -1,    10,    -1,    -1,    71,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    60,   119,   120,   121,    64,
      -1,   124,   125,   126,    -1,    -1,   129,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,    60,   124,
     125,   126,    64,    -1,   129,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    60,   119,   120,   121,
      64,    -1,   124,   125,   126,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    10,   119,   120,   121,    -1,    15,
     124,   125,   126,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    60,    -1,    -1,    -1,    64,    -1,
      -1,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    60,   105,
     106,   107,    64,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    -1,   124,   125,
     126,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    10,   119,   120,   121,
      -1,    15,   124,   125,   126,    -1,    -1,    -1,    -1,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      60,    -1,    -1,    -1,    64,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    84,   119,   120,   121,    -1,    -1,
     124,   125,   126,    -1,    60,    -1,    -1,    -1,    64,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    84,   119,
     120,   121,    -1,   123,   124,   125,   126,    -1,    -1,    10,
      -1,    -1,    -1,    99,    15,   101,   102,   103,    -1,   105,
     106,   107,    23,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,   123,   124,   125,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    60,
      -1,    -1,    -1,    64,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    60,   105,   106,   107,    64,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,   123,   124,   125,   126,    83,    84,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,    -1,   124,   125,   126,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    60,    -1,
      -1,    -1,    64,    -1,    -1,    23,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    60,   105,   106,   107,    64,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      10,    -1,   124,   125,   126,    15,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    10,   123,   124,   125,   126,    15,
      60,    -1,    -1,    -1,    64,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    60,   105,   106,   107,    64,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    10,   123,   124,   125,   126,    15,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    10,   123,   124,   125,
     126,    15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    60,   105,   106,   107,
      64,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    10,   123,   124,   125,   126,    15,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    10,   123,
     124,   125,   126,    15,    60,    -1,    -1,    -1,    64,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    60,   105,
     106,   107,    64,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    10,   123,   124,   125,
     126,    15,    84,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      10,   123,   124,   125,   126,    15,    60,    -1,    -1,    -1,
      64,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      60,   105,   106,   107,    64,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    10,   123,
     124,   125,   126,    15,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    10,   123,   124,   125,   126,    15,    60,    -1,
      -1,    -1,    64,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    60,   105,   106,   107,    64,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      10,   123,   124,   125,   126,    15,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    10,   123,   124,   125,   126,    15,
      60,    -1,    -1,    -1,    64,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    60,   105,   106,   107,    64,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    10,   123,   124,   125,   126,    15,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    10,   123,   124,   125,
     126,    15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    60,   105,   106,   107,
      64,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    10,   123,   124,   125,   126,    15,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    10,   123,
     124,   125,   126,    15,    60,    -1,    -1,    -1,    64,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    60,   105,
     106,   107,    64,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    10,    -1,   124,   125,
     126,    15,    84,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,   123,   124,   125,   126,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,    -1,
     124,   125,   126,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    60,   124,   125,
     126,    64,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    60,   119,   120,   121,    64,
      -1,   124,   125,   126,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,    60,   124,
     125,   126,    64,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    60,   119,   120,   121,
      64,    -1,   124,   125,   126,    -1,    -1,    71,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,    60,
     124,   125,   126,    64,    -1,    -1,    -1,    10,    11,    -1,
      71,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    60,   119,   120,
     121,    64,    -1,   124,   125,   126,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
      60,   124,   125,   126,    64,    -1,    -1,    -1,    10,    -1,
      -1,    71,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    60,   119,
     120,   121,    64,    -1,   124,   125,   126,    -1,    -1,    71,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,    60,   124,   125,   126,    64,    -1,    -1,    -1,    10,
      -1,    -1,    71,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,    -1,   124,   125,   126,    -1,    -1,
      71,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,    60,   124,   125,   126,    64,    -1,    -1,    -1,
      10,    -1,    -1,    71,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      60,   119,   120,   121,    64,    -1,   124,   125,   126,    -1,
      -1,    71,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    -1,    60,   124,   125,   126,    64,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    60,   119,   120,   121,    64,    -1,   124,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,    -1,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,    -1,   124,   125,   126
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   139,   140,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    16,    17,    18,    19,    21,    22,
      24,    25,    27,    29,    30,    31,    32,    33,    35,    36,
      39,    40,    41,    42,    43,    45,    46,    48,    53,    54,
      55,    56,    58,    59,    62,    63,    65,    66,    67,    68,
      69,    70,    72,    74,    75,    79,    81,    84,    99,   100,
     101,   102,   107,   116,   118,   120,   121,   123,   126,   129,
     131,   133,   141,   142,   143,   144,   145,   146,   151,   152,
     153,   155,   158,   159,   160,   161,   162,   164,   165,   168,
     169,   172,   175,   178,   179,   200,   203,   204,   222,   223,
     224,   225,   226,   227,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   243,   244,   245,   246,   247,   143,
     235,    80,   228,   229,   154,   155,   228,    12,    31,    32,
      33,    70,   129,   133,   175,   222,   226,   233,   234,   235,
     236,   238,   239,    72,   154,   235,   143,   131,   155,     7,
     154,   156,     9,    72,   156,    52,    82,   163,   235,   235,
     235,   131,   155,   176,   131,   155,   205,   206,   143,   235,
     235,   235,   235,     7,   131,    19,    30,   160,   160,   122,
     196,   216,   235,   123,   235,   235,    24,   146,   157,   235,
     235,    69,   131,   143,   235,   155,   201,   216,   235,   235,
     235,   235,   235,   235,   235,   235,   130,   141,   147,   216,
      73,   108,   196,   217,   218,   235,   216,   235,   242,    54,
     143,    44,   155,    38,    57,   192,    20,    52,    61,    76,
     123,   128,    10,    15,    23,    60,    64,    84,    99,   101,
     102,   103,   105,   106,   107,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   119,   120,   121,   124,   125,   126,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,   127,   131,   133,    60,    64,   131,
     143,   123,   129,   146,   235,   235,   235,   216,    34,   228,
     201,   123,   123,    79,   155,   196,   219,   220,   221,   235,
     129,   201,   169,   155,   131,   157,    24,    34,   157,    24,
      34,   157,   229,    71,   146,   219,   143,   131,   186,    73,
     131,   155,   207,   208,     1,   103,   210,   211,    34,   104,
     157,   219,   156,   155,   104,   123,   123,   129,   143,   157,
     131,   219,    85,   202,   123,   123,    28,    49,   157,   123,
     130,   141,   104,   130,   235,   104,   132,   104,   132,    34,
     104,   134,   229,    83,   104,   134,     7,   155,   103,   170,
     180,    61,   205,   205,   205,   205,   235,   235,   235,   235,
     163,   235,   163,   235,   235,   235,   235,   235,   235,   235,
      25,    72,   155,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   219,   219,   163,   235,   163,   235,
      20,    34,    37,    51,    52,    61,    72,   190,   230,   231,
     235,   247,    24,    34,    24,    34,    71,    34,   134,   163,
     235,   157,   123,   235,    83,    85,   132,   104,   155,   173,
     174,   123,   155,    34,   216,    33,   235,   163,   235,    33,
     235,   163,   235,   157,   143,   235,    26,   132,   187,   188,
     189,   190,   177,   208,   104,   132,     1,   133,   212,   223,
      83,    85,   209,   235,   206,   132,   196,   235,   166,   219,
     132,   133,   212,   223,   104,   126,   148,   150,   235,   148,
     149,   130,   132,   132,   217,   132,   217,   163,   235,   134,
     143,   235,   134,   235,   134,   235,   129,   216,   129,    52,
      61,    72,   182,   191,   205,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   132,   134,    34,    61,   222,   104,
     132,    60,    60,    33,   163,   235,    33,   163,   235,   163,
     235,   228,   228,   123,   235,   196,   235,   221,    85,   104,
     130,   170,   235,   132,   235,    24,   157,    24,   157,   235,
      24,   157,   229,    24,   157,   229,    26,    26,   143,   104,
     132,   131,   155,    20,    52,    61,    72,   193,   132,   208,
     103,   211,   216,   235,    47,   235,    50,    77,   130,   167,
     132,   216,   201,   104,   123,    11,   123,   134,   229,   134,
     229,   143,    83,   130,   147,   171,   181,    10,    15,    85,
      86,    87,    88,    89,    90,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   105,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   121,   124,   125,
     126,   127,   128,   155,   183,   184,   123,   231,   222,   222,
     235,    24,    24,   235,    24,    24,   134,   134,   157,   157,
     235,   174,   129,   157,    71,    33,   235,    33,   235,    71,
      33,   235,   157,    33,   235,   157,   143,   235,   188,   208,
     103,   215,    61,   211,   209,    34,   134,    24,   143,   216,
     134,   235,   235,    33,   143,   235,   134,    33,   143,   235,
     134,   235,   130,    30,    56,    58,   130,   142,   159,   193,
     155,   106,   131,   185,   185,    71,    33,    33,    71,    33,
      33,    33,    33,   171,   235,   235,   235,   235,   235,   235,
     132,    25,    67,    70,   133,   196,   214,   223,   108,   198,
     209,    78,   199,   235,   212,   223,   143,   157,    11,   235,
     143,   235,   143,   159,   211,   155,   183,   184,   187,   235,
     235,   235,   235,   235,   235,   130,    71,    71,    71,    71,
     215,   134,   216,   196,   197,   235,   235,   146,   158,   195,
     134,   235,    71,    71,   199,   185,   185,   132,    71,    71,
      71,    71,    71,    71,   235,   235,   235,   235,   198,   209,
     196,   213,   214,   223,    34,   134,   223,   235,   235,   123,
     194,   195,   235,   213,   214,   134,   213
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   138,   139,   140,   140,   141,   141,   142,   142,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   144,   144,   145,   145,   145,
     146,   146,   147,   147,   148,   148,   148,   148,   149,   149,
     150,   150,   151,   151,   151,   152,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   154,   154,   155,   156,   156,   157,   157,   158,   158,
     159,   159,   159,   159,   159,   159,   159,   160,   160,   161,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   163,   164,   164,   164,   164,   165,   166,   166,
     167,   167,   167,   168,   168,   169,   169,   169,   170,   170,
     171,   171,   171,   172,   173,   173,   173,   174,   174,   176,
     177,   175,   178,   178,   178,   178,   180,   181,   179,   182,
     182,   182,   182,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   185,   185,   186,   187,   187,   187,   188,   188,   188,
     188,   189,   189,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   191,   191,   191,   191,   192,   192,   193,   193,
     193,   193,   193,   193,   194,   194,   195,   195,   196,   196,
     197,   197,   198,   198,   199,   199,   200,   200,   200,   201,
     201,   202,   202,   202,   203,   203,   203,   203,   203,   204,
     204,   204,   205,   205,   206,   206,   206,   207,   207,   207,
     208,   208,   208,   209,   209,   209,   210,   210,   211,   211,
     211,   211,   212,   212,   212,   212,   213,   213,   213,   214,
     214,   214,   214,   214,   215,   215,   215,   215,   215,   215,
     215,   216,   216,   216,   216,   217,   217,   217,   218,   218,
     219,   219,   220,   220,   221,   221,   221,   221,   221,   222,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   225,
     226,   227,   227,   227,   227,   227,   227,   227,   227,   228,
     228,   229,   230,   230,   231,   231,   231,   232,   232,   233,
     234,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   236,   236,   236,   236,   237,
     237,   238,   238,   238,   239,   239,   239,   240,   240,   240,
     240,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   242,   242,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   244,   244,   244,
     244,   244,   244,   245,   245,   245,   245,   246,   246,   246,
     246,   247,   247,   247,   247,   247,   247,   247
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     2,     2,     3,     1,     5,     6,     0,     1,     1,
       2,     3,     1,     2,     1,     3,     3,     5,     0,     1,
       1,     1,     3,     5,     5,     3,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     0,     1,     1,     1,     1,     2,     1,     2,     3,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     2,
       5,     3,     6,     6,     4,     5,     5,     3,     3,     6,
       5,     6,     5,     6,     3,     4,     6,     7,     6,     7,
       4,     5,     4,     4,     3,     6,     5,     5,     0,     2,
       3,     2,     3,     6,     8,     1,     1,     1,     0,     2,
       0,     2,     3,     5,     1,     2,     3,     1,     3,     0,
       0,     8,     0,     1,     2,     2,     0,     0,     9,     3,
       3,     5,     5,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     0,     1,     3,     4,     4,     6,
       6,     0,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     0,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       0,     1,     2,     2,     0,     2,     3,     4,     4,     2,
       4,     0,     2,     2,     4,     4,     4,     5,     4,     0,
       1,     1,     1,     3,     3,     4,     5,     1,     1,     3,
       1,     2,     3,     0,     2,     2,     0,     4,     0,     2,
       2,     1,     4,     4,     6,     1,     0,     1,     1,     3,
       4,     3,     4,     6,     0,     2,     2,     2,     2,     2,
       2,     1,     1,     3,     3,     1,     1,     1,     3,     3,
       0,     1,     1,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     4,     4,     4,     5,
       2,     2,     6,     6,     4,     9,     9,     7,     6,     6,
       4,     9,     9,     7,     4,     6,     6,     9,     9,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     4,     1,     3,     2,     3,     3,     1,     3,     2,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       3,     3,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     4,     3,     3,     3,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     3,     4,     3,     4,
       3,     4,     3,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1
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
#line 417 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 4584 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 422 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4590 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 423 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 430 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4602 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 435 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4608 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 436 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4614 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 452 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4620 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 453 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4626 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 454 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4632 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 455 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4638 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 456 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4644 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 457 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4650 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 458 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4656 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 459 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4662 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4668 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 461 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4674 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 462 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4680 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 463 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4686 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 464 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4692 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4698 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 466 "chapel.ypp" /* yacc.c:1661  */
    { printf("syntax error"); clean_exit(1); }
#line 4704 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yyvsp[-4].b), (yylsp[-4]).comment))); }
#line 4710 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 473 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-5].b), (yylsp[-5]).comment))); }
#line 4716 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 477 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4722 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4728 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4734 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 4740 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 495 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 4746 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 500 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4752 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 501 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4758 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 506 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 4768 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 511 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 4778 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 4787 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 520 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 4796 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 526 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 4806 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 535 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 4816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 544 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 4822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 545 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 4828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 546 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 4834 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 550 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 4840 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 4846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 4852 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 556 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 4858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 557 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 4864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 4870 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 4876 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 560 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 4882 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 561 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 4888 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 4894 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 4900 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 4906 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 4912 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 4918 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 567 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 4924 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 568 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 4930 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 4936 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4942 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4948 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 582 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4954 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4966 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 591 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 4972 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 4978 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 4984 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4990 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 599 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4996 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 612 "chapel.ypp" /* yacc.c:1661  */
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
#line 5012 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5018 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5024 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 628 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5030 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5036 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 630 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5042 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5048 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5054 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5060 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5066 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5072 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 636 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5078 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5084 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 5090 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 5096 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5102 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5108 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 643 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5118 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 649 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5128 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 655 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5138 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 661 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5148 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 667 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5159 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 674 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5170 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 683 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[-1].pcallexpr)); }
#line 5176 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 687 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5182 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 688 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5188 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 689 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5194 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 690 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5200 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 695 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5206 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 699 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5212 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 700 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5218 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 705 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5224 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5230 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 709 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5236 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 716 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), NULL, (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5245 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 721 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-6].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5254 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 728 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5264 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 733 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5274 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 738 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5284 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 746 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 5290 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 747 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 5296 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5302 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 754 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 5308 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 756 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 5314 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 761 "chapel.ypp" /* yacc.c:1661  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 5325 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 771 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[0].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[0].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
#line 5338 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 780 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 5346 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 784 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 5355 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 791 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 5361 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 792 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 5367 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 797 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5376 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 802 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5385 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 807 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-5].pfnsymbol)->retTag = (yyvsp[-3].retTag);
      if ((yyvsp[-3].retTag) == RET_REF || (yyvsp[-3].retTag) == RET_CONST_REF)
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
#line 5405 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 827 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5416 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 833 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5428 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 840 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5441 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 848 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5454 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 859 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 865 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5475 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 872 "chapel.ypp" /* yacc.c:1661  */
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
#line 5498 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 894 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 898 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5515 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 903 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
    }
#line 5523 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 907 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5532 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 915 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5538 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 916 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 5544 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 917 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 5550 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 918 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 5556 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 919 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 5562 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 920 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 5568 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 921 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 5574 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 922 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 5580 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 923 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 5586 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 924 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 5592 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 925 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 5598 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 926 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 5604 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 927 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 5610 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 928 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 5616 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 929 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 5622 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 930 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 5628 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 931 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 5634 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 932 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 5640 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 933 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 5646 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 934 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 5652 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 935 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 5658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 936 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 5664 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 937 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 5670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 938 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 5676 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 939 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 5682 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 943 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 5688 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 944 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 5694 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 945 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 5700 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 946 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 5706 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 947 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 5712 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 948 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 5718 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 949 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 5724 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 950 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 5730 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 951 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 5736 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 952 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 5742 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 953 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 5748 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 954 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 5754 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 958 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 5760 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 959 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5766 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 963 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5772 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 967 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 5778 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 968 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 5784 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 969 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 5790 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 974 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 5796 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 976 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 5802 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 978 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 5808 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 980 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 5814 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 984 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5820 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 985 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 5826 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 989 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 5832 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 990 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 5838 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 991 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 5844 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 992 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 5850 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 993 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 5856 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 994 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 5862 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 995 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5868 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 996 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 5874 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 997 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 5880 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1001 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5886 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1002 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5892 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1003 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 5898 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1004 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 5904 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1008 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 5910 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1009 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 5916 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1013 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 5922 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1014 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 5928 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1015 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 5934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1016 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 5940 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1017 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 5946 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1018 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 5952 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1022 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 5958 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1028 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 5964 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1033 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 5970 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1035 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5976 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1039 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5982 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1044 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5988 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1045 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 5994 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1049 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6000 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1050 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6006 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1055 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 6012 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1057 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 6018 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1059 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 6024 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1064 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 6041 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1077 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 6059 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1093 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6065 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1095 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6071 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1097 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 6077 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1102 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6089 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1110 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6101 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1118 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6113 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1126 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 6126 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1135 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6137 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1144 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6143 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1145 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6149 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1146 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6155 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1152 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6164 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1160 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6170 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1162 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 6180 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1168 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6186 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1173 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 6192 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1175 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6198 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1177 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 6204 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1182 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 6210 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1184 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6216 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1186 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1192 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1193 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 6234 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6240 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6246 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1200 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6252 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1204 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6258 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1205 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6264 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1206 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 6270 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1207 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad type specification"); }
#line 6276 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1228 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 6284 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1232 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 6292 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1236 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 6304 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1243 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad array type specification"); clean_exit(1); }
#line 6310 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1247 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6316 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1248 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6322 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1249 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6328 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1254 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6334 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1256 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6340 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1262 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6346 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1264 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6352 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1266 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 6358 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1270 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6364 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1271 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6370 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1272 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6376 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1273 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 6382 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1274 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6388 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1275 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6394 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1276 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6400 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1282 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6406 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1283 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6412 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1284 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6418 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1285 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6424 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1289 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6430 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1290 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6436 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1291 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6442 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1295 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6448 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1296 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6454 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1300 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6460 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1305 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6466 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1306 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6472 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1310 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6478 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1311 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6484 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1312 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6490 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1313 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6496 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1318 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6502 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1335 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6508 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1337 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6514 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1339 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6520 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1341 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6526 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1343 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 6532 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1345 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6538 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1347 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6544 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1352 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6550 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1354 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6556 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1356 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6562 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6568 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1360 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6574 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1362 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6580 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1364 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6586 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1366 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6592 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1368 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6598 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1370 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6604 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1372 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6610 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1374 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6616 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1376 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 6627 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1383 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 6637 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1389 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 6647 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1395 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 6657 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1401 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 6667 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1410 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 6673 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1414 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 6679 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1430 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6685 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1434 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6691 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1439 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 6697 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1444 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pIntentExpr).first, (yyvsp[0].pIntentExpr).second); }
#line 6703 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1446 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).first);
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).second);
    }
#line 6712 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1454 "chapel.ypp" /* yacc.c:1661  */
    {
      ArgSymbol* tiMark = tiMarkForIntent((yyvsp[-1].pt));
      if (!tiMark) {
        USR_FATAL_CONT((yyvsp[0].pexpr), "%s is not supported in a 'with' clause",
                           intentDescrString((yyvsp[-1].pt)));
        tiMark = tiMarkForIntent(INTENT_IN); //dummy, so parser can continue
      }
      (yyval.pIntentExpr).first = new SymExpr(tiMark);
      (yyval.pIntentExpr).second = (yyvsp[0].pexpr);
    }
#line 6727 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1464 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).first = (yyvsp[-2].pexpr), (yyval.pIntentExpr).second = (yyvsp[0].pexpr); }
#line 6733 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).first = (yyvsp[-2].pexpr), (yyval.pIntentExpr).second = (yyvsp[0].pexpr); }
#line 6739 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6745 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 6751 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 6757 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 6763 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6769 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1501 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6775 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 6781 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 6787 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1507 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 6793 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6799 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6805 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1532 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 6811 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6817 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 6823 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 386:
#line 1538 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 6829 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 387:
#line 1546 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6835 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 388:
#line 1547 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 6841 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1548 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 6847 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1549 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 6853 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 6859 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 6865 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 6871 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1556 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 6877 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1557 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 6883 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6889 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 6895 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1560 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 6901 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1561 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 6907 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1563 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 6915 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1567 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 6923 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6929 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6935 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6941 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1580 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6947 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6953 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1582 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6959 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1583 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6965 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1584 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6971 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1585 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6977 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6983 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6989 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6995 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1589 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7001 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1590 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7007 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1591 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7013 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7019 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1593 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7025 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1594 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7031 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1595 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7037 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7043 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1597 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7049 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7055 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1599 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7061 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7067 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1601 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7073 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1605 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 7079 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 7085 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 7091 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 7097 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1609 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 7103 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 7109 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7115 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1615 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7121 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7127 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1617 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7133 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7139 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7145 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7151 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1624 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7157 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 7163 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1630 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 7169 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 7175 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 7181 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 7187 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 7193 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 7199 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 7203 "bison-chapel.cpp" /* yacc.c:1661  */
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
