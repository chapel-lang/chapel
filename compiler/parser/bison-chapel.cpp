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
#define YYLAST   11823

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  138
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  110
/* YYNRULES -- Number of rules.  */
#define YYNRULES  443
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  841

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
    1552,  1553,  1554,  1555,  1556,  1557,  1558,  1559,  1566,  1567,
    1571,  1572,  1573,  1574,  1575,  1576,  1577,  1578,  1579,  1580,
    1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,  1589,  1590,
    1591,  1592,  1593,  1597,  1598,  1599,  1600,  1601,  1602,  1606,
    1607,  1608,  1609,  1613,  1614,  1615,  1616,  1621,  1622,  1623,
    1624,  1625,  1626,  1627
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

#define YYPACT_NINF -758

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-758)))

#define YYTABLE_NINF -397

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -758,    90,  2592,  -758,  -758,  -758,  -758,  -758,  -758,  -758,
    -758,  3640,    -4,    93,  -758,    -4,  6765,    57,    93,  6765,
    3640,   -23,    93,   294,   446,  5689,  6765,  6765,    44,  -758,
      93,  -758,    36,  3640,  6765,  6765,  -758,  6765,  6765,   150,
      79,   754,   846,  -758,  5899,  6004,  6765,  6135,  6765,   103,
      87,  3640,    93,  -758,  5899,  6765,  6765,  -758,  -758,  6765,
    -758,  -758,  7815,  6765,  6765,  -758,  6765,  -758,  -758,  2854,
    5477,  5899,  -758,  3509,  -758,  -758,   171,  -758,  -758,  -758,
    -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,
      93,  -758,   431,    41,  -758,  -758,  -758,   251,   153,  -758,
    -758,  -758,   169,   185,   198,   190,   214, 11581,  7807,   288,
     223,   226,  -758,  -758,  -758,  -758,  -758,  -758,   264,  -758,
   11581,   241,  3640,  -758,   272,  -758,   269,  6765,  6765,  6765,
    6765,  6765,  5899,  5899,   326,  -758,  -758,  -758,  -758,  9194,
     327,  -758,  -758,    93,   297,  9488,   349,  5899,   319,  -758,
    -758,  -758,  -758,    93,   216,    93,   305,    54,  8219,  8185,
    8390,  5899,  3640,   331,    18,    55,   -12,  -758,  -758,   652,
     348,  8448,   652,  -758,  5899,  -758,   281,  -758,  -758,    93,
    -758,   182, 11581,  -758,  9522,  9073,  3640,  -758,  -758,  8448,
   11581,   334,  5899,  -758, 11581,   383,   353,   259,  9302,  8448,
    9607,   307,  1181,   652,   307,   652,  -758,  -758,  2985,   -31,
    -758,  6765,  -758,     7,    64, 11581,     8,  9651,    -9,   465,
    -758,    93,   381,  -758,  -758,  -758,    25,    36,    36,    36,
    -758,  6765,  6765,  6765,  6765,  6240,  6240,  6765,  6765,  6765,
    6765,  6765,  6765,    82,  7815,  6765,  6765,  6765,  6765,  6765,
    6765,  6765,  6765,  6765,  6765,  6765,  6765,  6765,  6765,  6765,
    6765,  6765,  6765,  6765,  6765,  6765,  6765,  6765,  6765,  6765,
    6765,  6765,  6765,  6765,  6765,  5899,  5899,  6240,  6240,  5372,
    -758,  -758,  3116,  -758,  9346,  9454,  9726,    40,  6240,    54,
     373,  -758,  -758,  6765,   378,  -758,   367,   397,  -758, 11581,
      93,   380,    93,   471,  5899,  -758,  3771,  6240,  -758,  3902,
    6240,  -758,    54,  3640,   486,   382,  -758,    63,  -758,  -758,
      18,  -758,   411,   388,  -758,  5057,   441,   440,  6765,    36,
    -758,   395,  -758,  -758,  5899,  -758,  -758,  -758,  -758,  -758,
    5899,   399,  5162,   425,  -758,  -758,  6870,  6765,  -758,  -758,
    -758,  -758,  -758,  8073,  5584,  -758,  5794,  -758,  6240,  2723,
     402,  6765,  6765,  -758,  -758,   403,  5899,   404,   270,    36,
     279,   299,   300,   302,  9136,  2353,  2353,   286,  -758,   286,
    -758,   286,  7931,    81,   535,   851,   348,   307,  -758,  -758,
    -758,  1181,  8253,   286,  1430,  1430,  2353,  1430,  1430,  1321,
     307,  8253, 11697,  1321,   652,   652,   307,  9770,  9838,  9882,
    9950,  9994, 10062, 10106, 10174, 10218, 10286, 10330, 10398, 10442,
   10510, 10554,   409,   413,  -758,   286,  -758,   286,   112,  -758,
    -758,  -758,  -758,  -758,  -758,    93,   177,  -758, 11639,   352,
    6975,  6240,  7080,  6240,  6765,  6240,  1777,    -4, 10622,  -758,
    -758, 10666,  6765,  5899,  -758,  5899,   463,    84,  -758,  -758,
     381,  6765,   179,  6765, 11581,    56,  8511,  6765, 11581,    33,
    8327,  -758,   525, 10734,  3640,  -758,   202,  -758,    38,  -758,
     280,   424,    18,    76,  -758,  5899,  -758,   255,  6765,  6345,
    -758, 11581,  -758,  -758,  -758, 11581,   140,   427,  -758,  5899,
    -758,   314,    93,   434,   459,   437, 10792,   459,   443,  -758,
    -758,  -758,  -758,   139,  7863,  -758, 11581,  3640, 11581, 10855,
    3247,   467,  -758,  -758,  -758,  -758,  -758,  2413,   323,  -758,
    -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,
    -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,
    -758,  -758,  -758,  5372,  -758,  6240,  6240,  6765,   543, 10913,
    6765,   545, 10976,   438,  7897,    54,    54,  -758, 11581,  -758,
   11581,  -758,  6765,    93,  -758,   448,  8448,  -758,  8574,  4033,
    -758,  4164,  -758,  8637,  4295,  -758,    54,  4426,  -758,    54,
    3640,  6765,  -758,   653,  -758,    18,   472,   518,  -758,  -758,
    -758,    31,  -758,  -758,  5162,   440,    48, 11581,  -758, 11581,
    4557,  5899,  -758,  -758,  -758,    67,  -758,  6765,  -758,  6765,
    -758,  4688,   447,  4819,   450,  -758,  6765,  -758,  3378,   481,
     280,  -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,
    -758,  -758,  -758,  -758,  -758,  -758,    93,  -758,  -758,  -758,
    -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,  -758,
    -758,  -758,  -758,  -758,  -758,  -758,  -758,   476,   454,   454,
    -758,  -758,   256,   262, 11034,  7185,  7290, 11097,  7395,  7500,
    7605,  7710,  -758,  -758, 11581,  -758,  -758,  -758,  3640,  6765,
   11581,  6765, 11581,  3640,  6765, 11581,  -758,  6765, 11581,  -758,
    -758, 11581,  -758,   455,  6450,    43,  -758,   511,  -758,  6240,
    2239,  3640,  -758,    26,  4926, 11155, 11581,  6765,  -758, 11581,
    3640,  6765,  -758, 11581,  3640, 11581,  -758,   571,   456,   456,
    -758,  1165,  -758,    24,  -758,  2413,    63,  -758,  -758,  6765,
    6765,  6765,  6765,  6765,  6765,  6765,  6765,   813, 10734,  8700,
    8763, 10734,  8826,  8889,   472,   -23,  6765,  6765,  5267,  -758,
    -758,   339,  5899,  -758,  -758,  6765,    60,  8005,  -758,   276,
     349,  -758,  6765,  8952,  -758,  9015,  -758,  -758,   511,  -758,
     454,   454,   282, 11218, 11276, 11339, 11397, 11460, 11518,  -758,
    3640,  3640,  3640,  3640,    43,  6555,   170,  -758,  -758, 11581,
   11581,  -758,  -758,  -758,  7710, 11581,  3640,  3640,   149,  -758,
    -758,  -758,  6765,  6765,  6765,  6765,  6765,  6765, 10734, 10734,
   10734, 10734,  -758,  -758,  -758,  -758,  -758,   398,  6240,   976,
     322, 10734, 10734,  -758,  -758,  -758,  8039,  -758,  -758,  6660,
    -758
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    34,    73,   390,   391,   392,   393,
     394,     0,   349,    71,   125,   349,     0,   250,    71,     0,
       0,     0,     0,    71,    71,     0,     0,     0,     0,   143,
       0,   139,     0,     0,     0,     0,   340,     0,     0,     0,
       0,   249,   249,   126,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   127,     0,     0,     0,   439,   441,     0,
     442,   443,   374,     0,     0,   440,   437,    80,   438,     0,
       0,     0,     4,     0,     5,     9,     0,    10,    11,    12,
      14,   309,    19,    13,    81,    15,    17,    16,    18,    86,
       0,    84,   366,     0,    87,    85,    88,     0,   375,   362,
     363,   312,   310,     0,     0,   367,   368,     0,   311,     0,
     376,   377,   378,   361,   314,   313,   364,   365,     0,    21,
     320,     0,     0,   350,     0,    72,     0,     0,     0,     0,
       0,     0,     0,     0,   366,   375,   310,   367,   368,   349,
     311,   376,   377,     0,     0,     0,     0,   300,     0,    75,
      74,   144,    89,     0,   145,     0,     0,     0,     0,     0,
       0,   300,     0,     0,     0,     0,     0,   252,    28,   425,
     359,     0,   426,     7,   300,   250,   251,    83,    82,   229,
     292,     0,   291,    78,     0,     0,     0,    77,    31,     0,
     315,     0,   300,    32,   321,   241,     0,     0,   291,     0,
       0,   428,   373,   424,   427,   423,    40,    42,     0,     0,
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
      41,    43,   395,     0,     0,   387,     0,   389,     0,     0,
       0,     0,     0,   397,     8,     0,     0,     0,   212,     0,
       0,     0,     0,     0,   348,   420,   419,   422,   430,   429,
     434,   433,   416,   413,   414,   415,   370,   403,   386,   385,
     384,   371,   407,   418,   412,   410,   421,   411,   409,   401,
     406,   408,   417,   400,   404,   405,   402,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,   431,   436,   435,   206,   203,
     204,   205,   209,   210,   211,     0,     0,   352,     0,     0,
       0,     0,     0,     0,     0,     0,   396,   349,   349,    94,
     237,     0,     0,     0,   317,     0,   137,     0,   134,   238,
     128,     0,     0,     0,   324,     0,     0,     0,   330,     0,
       0,   105,   113,     0,     0,   316,     0,   195,     0,   202,
     218,     0,   261,     0,   275,     0,   270,   362,     0,     0,
     254,   360,   253,   383,   294,   293,     0,     0,   318,     0,
     243,   362,     0,   438,    51,     0,    44,    49,     0,   369,
     388,   298,   299,     0,     0,   110,   334,     0,   398,     0,
       0,   129,   130,   213,   214,   215,   147,     0,     0,   245,
     244,   246,   248,    56,    63,    64,    65,    60,    62,    69,
      70,    58,    61,    59,    57,    67,    66,    68,   381,   382,
     207,   208,   354,     0,   351,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,   306,   304,
     305,   303,     0,   135,   133,     0,     0,   112,     0,     0,
      96,     0,    95,     0,     0,   102,     0,     0,   100,     0,
       0,     0,   116,   201,   193,     0,   284,   219,   222,   221,
     223,     0,   259,   262,     0,   263,     0,   255,   264,   265,
       0,     0,   117,   119,   319,     0,   240,     0,    53,     0,
      54,     0,     0,     0,     0,   111,     0,    35,     0,   249,
     218,   176,   174,   179,   186,   187,   188,   183,   185,   181,
     184,   182,   180,   190,   189,   155,   158,   156,   157,   168,
     159,   172,   164,   162,   175,   163,   161,   166,   171,   173,
     160,   165,   169,   170,   167,   177,   178,   153,   191,   191,
     247,   353,   375,   375,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    92,   138,   136,   130,    99,     0,     0,
     323,     0,   322,     0,     0,   329,   103,     0,   328,   101,
     115,   339,   196,     0,     0,   263,   220,   234,   256,     0,
       0,     0,   121,     0,     0,    46,    45,     0,   108,   336,
       0,     0,   106,   335,     0,   399,    36,    71,   249,   249,
     123,   249,   131,     0,   154,     0,   194,   149,   150,     0,
       0,     0,     0,     0,     0,     0,     0,   249,   327,     0,
       0,   333,     0,     0,   284,   287,   288,   289,     0,   286,
     290,   362,   230,   198,   197,     0,     0,     0,   273,   362,
     122,   120,     0,     0,   109,     0,   107,   132,   234,   153,
     191,   191,     0,     0,     0,     0,     0,     0,     0,   124,
       0,     0,     0,     0,   263,   276,     0,   231,   233,   232,
     235,   226,   227,   141,     0,    47,     0,     0,     0,   151,
     152,   192,     0,     0,     0,     0,     0,     0,   326,   325,
     332,   331,   200,   199,   278,   279,   281,   362,     0,   396,
     362,   338,   337,   224,   148,   225,     0,   280,   282,   276,
     283
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -758,  -758,  -758,     1,  -582,  1861,  -758,  -758,  1431,    70,
     245,  -758,  -758,  -758,  -758,  -758,   195,  1948,    -6,   -96,
    -682,  -583,   -40,  -758,  -758,    80,  -758,  -758,  -758,  -758,
    -758,   439,   134,   -90,  -758,  -758,    29,   532,  -758,  -758,
    -758,  -758,  -758,  -758,  -758,  -138,  -137,  -604,  -758,  -133,
      14,  -758,  -268,  -758,  -758,   -21,  -758,  -195,   133,  -758,
    -180,  -162,  -758,  -123,  -758,  -758,  -758,  -158,   289,  -758,
    -316,  -599,  -758,  -464,  -333,  -637,  -757,  -135,    10,   125,
    -758,  -143,  -758,   165,   342,  -313,  -758,  -758,   691,  -758,
      -8,  -149,  -758,    69,  -758,   920,  1045,   -11,  1167,  -758,
    1341,  1366,  -758,  -758,  -758,  -758,  -758,  -758,  -758,  -266
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   207,    73,   472,    75,    76,    77,   208,
     504,   508,   505,    78,    79,    80,   150,    81,   154,   188,
      82,    83,    84,    85,    86,   563,    87,    88,   496,   613,
      89,    90,   367,   629,    91,   457,   458,   134,   163,   480,
      93,    94,   368,   630,   526,   668,   669,   737,   318,   476,
     477,   478,   479,   527,   225,   601,   834,   803,   180,   798,
     763,   766,    95,   196,   343,    96,    97,   166,   167,   322,
     323,   490,   326,   327,   486,   825,   760,   705,   209,   213,
     214,   296,   297,   298,   135,    99,   100,   101,   136,   103,
     122,   123,   436,   437,   104,   137,   138,   107,   140,   109,
     141,   142,   112,   113,   218,   114,   115,   116,   117,   118
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     120,   177,   178,    72,   481,   139,   708,   126,   145,   500,
     312,   435,   487,   439,   158,   159,   160,   151,   315,   605,
     290,     5,   328,   169,   170,   324,   171,   172,     5,   501,
     301,   331,   324,   182,   184,   185,   189,   190,   826,     5,
     194,     5,   358,   198,   199,   200,   732,   731,   201,   341,
     186,   202,   203,   204,   181,   205,   324,   584,   182,   215,
     217,   305,   308,   311,   197,   738,  -201,   360,   370,   371,
     372,   373,   838,   334,   445,   330,   121,   324,   186,   223,
     579,   216,   709,   428,   802,     5,   369,  -268,   121,  -268,
       3,   319,   329,   339,  -268,   362,     5,   429,   224,   352,
     430,   709,  -268,   348,   234,   157,   764,   388,   147,  -268,
    -268,   354,   334,   121,   431,   432,   120,   284,   285,   286,
     194,   182,   217,    45,   433,   363,   802,   604,   489,   143,
     334,   289,   422,   423,   604,   434,   299,   707,  -266,   355,
    -268,   235,   359,   287,   334,   236,   550,  -268,   777,   320,
     299,   762,   334,  -268,   389,   282,   164,   173,   325,  -268,
    -268,  -268,   282,   299,   732,   731,   603,   164,   356,   595,
     332,   334,   191,   551,   446,   161,   809,   810,  -268,   604,
    -268,   299,   710,   282,   241,   282,   242,   243,   573,   282,
     610,   246,   837,   449,  -201,   823,   357,   497,   253,  -268,
     353,   714,   840,   212,   828,   257,   258,   259,   124,   351,
     174,   528,    45,   144,   574,   221,   471,   611,   192,   121,
     374,   375,   376,   377,   379,   381,   382,   383,   384,   385,
     386,   387,    14,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   299,   299,   425,   427,   438,   778,
     612,   226,   833,   621,   334,    43,  -342,   448,   282,   703,
     295,   553,   451,   334,     5,   435,   334,   439,   149,  -269,
      53,   487,  -341,   182,   295,   464,   466,     5,   468,   470,
     597,   149,   473,   227,   829,   335,   593,   295,   230,   554,
    -272,   577,   228,  -346,   462,   378,   380,   491,  -269,   -71,
     586,   589,   523,   495,   277,   295,   231,   229,   278,   299,
     234,   524,   598,  -269,   594,   506,   506,  -347,   -71,  -272,
    -269,   599,   525,   215,    98,   215,  -344,   514,   516,  -343,
     518,   519,   600,    98,  -272,   182,  -274,   424,   426,  -269,
    -355,  -272,    98,   334,   622,   624,  -356,   235,   447,   580,
     582,   236,   279,   585,   588,    98,   521,   768,  -269,   616,
    -272,   768,   345,   329,  -269,  -274,   593,   465,  -355,   241,
     469,   761,   243,    98,  -356,   281,   246,   769,   282,  -272,
    -274,   769,   529,   329,   329,  -272,   329,  -274,   295,   295,
     241,    98,   556,   243,   811,    98,   278,   246,  -242,   275,
     291,   276,   530,   531,  -285,   532,  -274,   329,   293,   464,
     559,   468,   562,   473,   564,   516,   304,  -242,   513,   565,
     566,   568,   570,  -285,   299,  -274,   670,  -285,   300,     5,
     576,  -274,   578,   149,   243,   152,   583,  -380,  -379,  -380,
    -379,   452,   317,   453,    98,   340,  -251,   494,   342,   682,
     683,  -285,   364,   295,   217,   175,   344,   607,   609,   511,
     687,   512,   827,  -277,   366,    23,   176,   212,   217,   212,
     696,   830,    29,   699,  -142,   606,   450,    14,  -251,   454,
      17,   455,  -277,   459,    98,   461,  -277,  -251,    22,   615,
      23,   727,   474,  -142,   475,   482,   827,    29,   153,  -142,
     483,   558,  -251,   561,   488,   489,   827,   493,    98,   502,
    -277,   498,   520,   522,    92,    39,   517,   728,  -142,   729,
      43,   548,   438,    92,   379,   425,   674,   549,   572,   677,
      98,   590,    92,    52,  -345,    53,   602,   -50,   234,   614,
     618,   684,  -380,   617,  -380,    92,   620,   675,   690,   678,
     692,   334,   680,   695,     5,   704,   698,   686,   149,   706,
     701,   720,   735,    92,   724,   736,   569,   754,   295,   765,
     628,  -251,   507,   302,   575,   235,   747,   780,   781,   236,
     182,    92,   685,   782,    67,    92,   715,   702,   716,   733,
     719,   730,   723,   835,   822,   725,   808,   771,   492,   794,
     571,   713,   671,  -251,    98,     0,     0,     0,     0,   351,
       0,     0,  -251,     0,   238,   378,   424,   240,   241,     0,
     242,   243,     0,   153,     0,   246,     0,  -251,    98,     0,
       0,    98,   253,     0,    92,    98,     0,     0,     0,   257,
     258,   259,     0,     0,   690,   692,     0,   695,   698,   719,
     723,     0,     0,   428,     0,   234,     0,   748,   749,     0,
     750,     0,   751,   752,     0,     0,   753,   429,   177,   178,
     430,     0,     0,   102,    92,     0,     0,     0,   767,   516,
       0,    98,   102,   516,   431,   432,   773,     0,     0,     0,
     775,   102,   235,     0,   433,     0,   236,     0,    92,     0,
       0,     0,     0,     0,   102,   434,     0,     0,   748,   783,
     784,   751,   785,   786,   787,   788,     0,     0,     0,     0,
      92,     0,   102,     0,     0,   190,   194,   217,     0,     0,
       0,   799,     0,     0,   800,   241,     0,   242,   243,     0,
     102,   805,   246,     0,   102,     0,     0,     0,   796,   253,
       0,     0,     0,   175,     0,     0,     0,   552,   259,   818,
     819,   820,   821,    23,   176,     0,     0,     0,     0,     0,
      29,     0,  -142,   723,     0,   831,   832,     0,   -39,     0,
       0,   818,   819,   820,   821,   831,   832,     0,     0,     0,
       0,  -142,     0,   102,    92,     0,    98,   836,   516,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   723,    14,
       0,     0,    17,     0,     0,     0,     0,   759,    92,     0,
      22,    92,    23,   727,     0,    92,     0,     0,     0,    29,
       0,  -142,     0,   102,     0,     0,     0,     0,     0,    98,
       0,     0,    98,     0,     0,   175,     0,    39,     0,   728,
    -142,   729,    43,     0,   234,    23,   176,   102,     0,     0,
       0,     0,    29,     0,  -142,    52,     0,    53,     0,     0,
     -38,    92,     0,     0,     0,   797,     0,   672,   673,   102,
       0,     0,     0,  -142,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,   236,     0,     0,     0,     0,
       0,    98,   105,    98,     0,     0,    98,     0,   824,    98,
       0,   105,    98,     0,     0,     0,    67,     0,     0,     0,
     105,     0,     0,   789,     0,     0,     0,     0,     0,     0,
     238,     0,    98,   105,   241,     0,   242,   243,     0,     0,
       0,   246,   824,    98,     0,    98,     0,     0,   253,     0,
      98,   105,   824,   102,     0,   257,   258,   259,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   127,   105,
       0,     0,     0,   105,     0,     0,     0,   102,     0,     0,
     102,    21,     0,     0,   102,     0,    92,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
      98,    40,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,   105,    48,    49,    50,   131,   106,     0,    92,
     102,     0,    92,    98,     0,     0,   106,     0,     0,     0,
       0,  -276,    98,     0,     0,   106,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,   106,     0,
    -276,     0,   105,     0,  -276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,   106,     0,   179,     0,
       0,     0,     0,     0,     0,   132,   105,    70,  -276,   758,
       0,    92,     0,    92,   106,     0,    92,     0,   106,    92,
       0,     0,    92,     0,     0,     0,     0,     0,   105,     0,
       0,     0,    98,    98,    98,    98,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,    98,    98,
       0,     0,     0,    92,     0,    92,     0,     0,     0,     0,
      92,     0,     0,     0,     0,   102,     0,   106,     0,   108,
       0,     0,     0,     0,     0,     0,     0,     0,   108,     0,
       0,    14,     0,     0,    17,     0,     0,   108,     0,     0,
       0,     0,    22,     0,    23,   727,     0,     0,     0,     0,
     108,    29,   105,  -142,   234,     0,     0,   106,   102,     0,
       0,   102,     0,     0,     0,     0,     0,     0,   108,   219,
      92,   728,  -142,   729,    43,    92,   105,     0,     0,   105,
       0,   106,     0,   105,     0,     0,   108,    52,     0,    53,
     108,   235,     0,    92,     0,   236,     0,     0,     0,     0,
       0,     0,    92,   106,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,   102,     0,     0,   102,     0,     0,   102,   105,
     238,   102,   239,   240,   241,     0,   242,   243,    67,   108,
       0,   246,     0,     0,     0,     0,     0,   252,   253,     0,
       0,   102,   256,     0,     0,   257,   258,   259,     0,     0,
       0,     0,   102,     0,   102,     0,     0,     0,     0,   102,
       0,     0,    92,    92,    92,    92,     0,   106,     0,   108,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    92,
       0,     0,     0,   110,   234,     0,     0,     0,     0,     0,
       0,   106,   110,   108,   106,     0,     0,     0,   106,     0,
       0,   110,     0,     0,     0,     0,     0,     0,   111,     0,
       0,     0,     0,     0,   110,   108,     0,   111,     0,   102,
       0,   235,     0,     0,   102,   236,   111,     0,     0,     0,
       0,     0,   110,     0,   105,     0,     0,     0,     0,   111,
       0,     0,   102,     0,   106,     0,     0,     0,     0,     0,
     110,   102,     0,     0,   110,   102,     0,   111,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,     0,     0,
       0,   246,     0,     0,     0,   111,     0,   105,   253,   111,
     105,     0,     0,     0,     0,   257,   258,   259,     0,   108,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   108,     0,     0,   108,     0,   187,     0,
     108,   102,   102,   102,   102,     0,     0,     0,   111,     0,
     235,     0,     0,     0,   236,     0,     0,   102,   102,   105,
       0,   105,     0,   110,   105,     0,     0,   105,     0,     0,
     105,     0,     0,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,   108,   110,   111,   238,
     105,   239,   240,   241,     0,   242,   243,   244,     0,     0,
     246,   105,     0,   105,     0,     0,   252,   253,   105,   110,
       0,   256,   111,     0,   257,   258,   259,   283,     0,     0,
       0,     0,   106,     0,     0,   106,     0,     0,     0,     0,
       0,     0,     0,     0,   111,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   187,   187,
     187,   314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   187,     0,     0,     0,     0,     0,   105,     0,
       0,     0,     0,   105,     0,     0,     0,     0,     0,     0,
     187,     0,     0,   110,   106,     0,   106,     0,     0,   106,
     187,   105,   106,     0,     0,   106,     0,     0,     0,     0,
     105,   108,     0,     0,   105,     0,     0,   110,   111,     0,
     110,     0,     0,     0,   110,   106,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   106,     0,   106,     0,
       0,     0,   111,   106,     0,   111,     0,     0,     0,   111,
       0,     0,     0,     0,   108,     0,     0,   108,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,   105,   105,   105,     0,     0,     0,     0,     0,     0,
     187,     0,     0,     0,     0,   111,   105,   105,     0,     0,
       0,     0,     0,   106,     0,     0,     0,     0,   106,     0,
       0,     0,     0,   187,     0,     0,   108,     0,   108,     0,
       0,   108,     0,     0,   108,     0,   106,   108,     0,     0,
       0,     0,     0,     0,     0,   106,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,     0,   108,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,   108,   127,
     108,     0,     0,     0,     0,   108,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   128,   129,
     130,     0,    28,     0,     0,   110,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,   106,   106,   106,   106,     0,
     111,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,   106,   106,     0,     0,   108,     0,     0,   110,     0,
     108,   110,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,   119,     0,     0,     0,     0,    59,   108,     0,
       0,   146,     0,   111,     0,     0,   111,   108,     0,     0,
       0,   108,     0,     0,   168,    64,   187,   187,     0,     0,
     187,   187,     0,     0,     0,     0,     0,     0,    70,     0,
     133,     0,   193,     0,     0,     0,     0,     0,     0,     0,
     110,     0,   110,     0,     0,   110,     0,     0,   110,     0,
      74,   110,     0,     0,   220,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,     0,   111,     0,     0,
     111,   110,     0,   111,     0,     0,   111,   108,   108,   108,
     108,   125,   110,     0,   110,     0,   125,     0,     0,   110,
     148,   125,   125,   108,   108,     0,   111,     0,   162,     0,
     165,     0,     0,   280,     0,     0,     0,   111,     0,   111,
       0,     0,     0,     0,   111,     0,   187,   187,     0,     0,
     195,     0,     0,     0,     0,     0,     0,   187,     0,   314,
       0,     0,     0,     0,   314,     0,     0,   187,     0,     0,
     187,     0,     0,   316,     0,     0,     0,     0,     0,   110,
       0,     0,     0,     0,   110,     0,     0,     0,   222,     0,
       0,     0,     0,     0,     0,     0,     0,   338,     0,     0,
       0,     0,   110,     0,   111,     0,     0,     0,     0,   111,
       0,   110,     0,     0,     0,   110,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,   111,     0,     0,
       0,     0,     0,     0,     0,     0,   111,     0,     0,     0,
     111,   195,     0,     0,     0,   294,     0,     0,     0,     0,
       0,   195,     0,   303,     0,     0,     0,     0,     0,   294,
       0,     0,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   294,     0,   125,     0,     0,   333,     0,     0,
       0,   110,   110,   110,   110,     0,     0,     0,     0,     0,
     294,     0,     0,    74,   187,     0,     0,   110,   110,     0,
       0,     0,     0,     0,     0,     0,   111,   111,   111,   111,
       0,     0,     0,     0,     0,     0,     0,   338,     0,   365,
     338,     0,   111,   111,   165,   165,   165,   165,     0,     0,
     314,   314,     0,   314,   314,     0,     0,     0,     0,     0,
       0,   390,     0,     0,     0,     0,     0,   801,     0,     0,
       0,     0,     0,     0,   314,     0,   314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     515,     0,     0,   294,   294,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   801,
     484,     0,     5,     6,     7,     8,     9,    10,   456,  -396,
     460,   127,     0,     0,  -396,     0,     0,     0,     0,     0,
       0,     0,  -396,     0,    21,     0,     0,     0,   321,     0,
     128,   129,   130,     0,    28,     0,     0,   165,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,   294,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,  -396,
       0,     0,     0,  -396,     0,     0,    48,    49,    50,   131,
       0,     0,     0,     0,     0,     0,     0,   165,     0,     0,
       0,     0,  -267,  -396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   592,     0,     0,  -396,    59,
    -396,  -396,  -396,     0,  -396,  -396,  -396,     0,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,    64,  -396,  -396,
    -396,     0,     0,  -396,  -396,  -396,     0,     0,   132,     0,
      70,     0,   499,     0,     0,     0,   234,     0,   625,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   294,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   235,     0,     0,     5,   236,     0,     0,
       0,     0,     0,   631,     0,     0,   596,     0,   632,     0,
     321,     0,     0,     0,     0,     0,     0,   237,     0,     0,
     338,     0,   338,     0,     0,   338,     0,     0,   338,     0,
     195,   700,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,     0,   250,   251,   252,
     253,   712,   254,   255,   256,   667,     0,   257,   258,   259,
       0,     0,   718,     0,   722,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,   633,   634,
     635,   636,   637,   638,     0,     0,   639,   640,   641,   642,
     643,   644,   645,   646,   647,   648,     0,     0,   649,     0,
       0,   456,   650,   651,   652,   653,   654,   655,   656,   657,
     658,   659,   660,     0,   661,     0,     0,   662,   663,   664,
     665,   666,     0,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   770,     0,     0,     0,     0,     0,     0,     0,
       0,   774,     0,     0,     0,   776,     0,     0,     0,     0,
       0,     0,    -2,     4,   734,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -249,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -142,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,     0,  -249,    38,    39,    40,    41,  -142,
      42,    43,     0,  -249,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,  -249,     0,
       0,    55,     0,    56,     0,   125,    57,     0,     0,     0,
       0,     0,     0,   779,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,    67,     0,     0,    68,     0,
       0,    69,     0,    70,     4,    71,     5,     6,     7,     8,
       9,    10,     0,  -396,     0,    11,    12,    13,  -396,    14,
      15,    16,    17,  -249,    18,    19,  -396,    20,    21,  -396,
      22,     0,    23,    24,    25,    26,    27,     0,    28,    29,
       0,  -142,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,     0,  -249,    38,     0,    40,    41,
    -142,    42,    43,  -396,  -249,    44,    45,  -396,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -249,
       0,     0,    55,     0,    56,     0,     0,  -396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -396,    59,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,     0,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,    64,  -396,  -396,  -396,     0,    67,  -396,  -396,  -396,
       0,     0,    69,  -396,    70,     4,    71,     5,     6,     7,
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
      67,     0,     0,    68,     0,     0,    69,   627,    70,     4,
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
       0,    67,     0,     0,    68,     0,     0,    69,   726,    70,
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
      23,    24,    25,    26,   463,     0,    28,    29,     0,  -142,
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
       0,    23,    24,    25,    26,   467,     0,    28,    29,     0,
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
      22,     0,    23,    24,    25,    26,   689,     0,    28,    29,
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
       0,    22,     0,    23,    24,    25,    26,   691,     0,    28,
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
      21,     0,    22,     0,    23,    24,    25,    26,   694,     0,
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
      20,    21,     0,    22,     0,    23,    24,    25,    26,   697,
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
       0,   711,    21,     0,    22,     0,    23,    24,    25,    26,
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
      26,   717,     0,    28,    29,     0,  -142,    30,    31,    32,
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
      25,    26,   721,     0,    28,    29,     0,  -142,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
       0,  -249,    38,     0,    40,    41,  -142,    42,    43,     0,
    -249,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -249,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,   484,     0,     5,
       6,     7,     8,     9,    10,    63,  -396,    64,   127,    65,
      66,  -396,    67,     0,     0,    68,     0,     0,    69,  -396,
      70,    21,    71,     0,     0,     0,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,  -396,     0,     0,     0,
    -396,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -396,    59,  -396,  -396,  -396,
       0,  -396,  -396,  -396,     0,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,    64,  -396,  -396,  -396,     0,     0,
    -396,  -396,  -396,     0,     0,   132,     0,    70,   484,   499,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   127,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,   484,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     485,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   499,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,   179,
       0,     0,   428,    68,     0,     0,   132,    21,    70,     0,
     133,   795,     0,   128,   129,   130,   429,    28,     0,   430,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,   431,   432,    38,     0,    40,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,   434,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   133,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
     210,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,   211,     0,     5,     6,     7,
       8,     9,    10,    63,     0,    64,   127,    65,    66,   179,
       0,     0,     0,    68,     0,     0,   132,     0,    70,    21,
     133,     0,     0,     0,     0,   128,   129,   130,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,   210,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     5,     6,     7,     8,     9,    10,     0,     0,
      63,   127,    64,     0,    65,    66,   179,     0,     0,     0,
      68,     0,     0,   132,    21,    70,   510,   133,     0,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,   155,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   156,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     5,     6,     7,
       8,     9,    10,     0,     0,    63,   127,    64,     0,    65,
      66,     0,     0,     0,     0,    68,     0,     0,   132,    21,
      70,     0,   133,     0,     0,   128,   129,   130,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,   210,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     5,     6,     7,     8,     9,    10,     0,     0,
      63,   127,    64,     0,    65,    66,   179,     0,     0,     0,
      68,     0,     0,   132,    21,    70,     0,   133,     0,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     5,     6,     7,
       8,     9,    10,     0,     0,    63,   127,    64,     0,    65,
      66,   179,     0,     0,     0,    68,     0,     0,   132,    21,
      70,     0,   133,     0,     0,   128,   129,   130,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,   183,     0,     0,
      68,     0,     0,   132,     0,    70,     0,   133,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   127,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   186,
      21,     0,     0,     0,     0,     0,   128,   129,   130,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     5,     6,     7,     8,     9,    10,     0,
       0,    63,   127,    64,     0,    65,    66,     0,     0,     0,
       0,    68,     0,     0,    69,    21,    70,     0,   133,     0,
       0,   128,   129,   130,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   156,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     5,     6,
       7,     8,     9,    10,     0,     0,    63,   127,    64,     0,
      65,    66,     0,     0,     0,     0,    68,     0,     0,   132,
      21,    70,     0,   133,     0,     0,   128,   129,   130,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,   608,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     5,     6,     7,     8,     9,    10,     0,
       0,    63,   127,    64,     0,    65,    66,     0,     0,     0,
       0,    68,     0,     0,   132,   755,    70,     0,   133,     0,
       0,   128,   129,   130,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   756,    49,    50,
     757,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     5,     6,
       7,     8,     9,    10,     0,     0,    63,   127,    64,     0,
      65,    66,   179,     0,     0,     0,    68,     0,     0,   132,
      21,    70,     0,   758,     0,     0,   128,   129,   130,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     5,     6,     7,     8,     9,    10,     0,
       0,    63,   127,    64,     0,    65,    66,   179,     0,     0,
       0,    68,     0,     0,   132,    21,    70,     0,   758,     0,
       0,   128,   129,   746,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     5,     6,
       7,     8,     9,    10,     0,     0,    63,   127,    64,     0,
      65,    66,   179,     0,     0,     0,    68,     0,     0,   132,
      21,    70,     0,   133,     0,     0,   128,   129,   130,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     5,     6,     7,     8,     9,    10,     0,
       0,    63,   127,    64,     0,    65,    66,     0,     0,     0,
       0,    68,     0,     0,   132,    21,    70,     0,   133,     0,
       0,   128,   129,   130,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     5,     6,
       7,     8,     9,    10,     0,     0,    63,   127,    64,     0,
      65,    66,     0,     0,     0,     0,   503,     0,     0,   132,
      21,    70,     0,   133,     0,     0,   128,   129,   557,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     5,     6,     7,     8,     9,    10,     0,
       0,    63,   127,    64,     0,    65,    66,     0,     0,     0,
       0,    68,     0,     0,   132,    21,    70,     0,   133,     0,
       0,   128,   129,   560,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     5,     6,
       7,     8,     9,    10,     0,     0,    63,   127,    64,     0,
      65,    66,     0,     0,     0,     0,    68,     0,     0,   132,
      21,    70,     0,   133,     0,     0,   128,   129,   740,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     5,     6,     7,     8,     9,    10,     0,
       0,    63,   127,    64,     0,    65,    66,     0,     0,     0,
       0,    68,     0,     0,   132,    21,    70,     0,   133,     0,
       0,   128,   129,   741,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     5,     6,
       7,     8,     9,    10,     0,     0,    63,   127,    64,     0,
      65,    66,     0,     0,     0,     0,    68,     0,     0,   132,
      21,    70,     0,   133,     0,     0,   128,   129,   743,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     5,     6,     7,     8,     9,    10,     0,
       0,    63,   127,    64,     0,    65,    66,     0,     0,     0,
       0,    68,     0,     0,   132,    21,    70,     0,   133,     0,
       0,   128,   129,   744,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     5,     6,
       7,     8,     9,    10,     0,     0,    63,   127,    64,     0,
      65,    66,     0,     0,     0,     0,    68,     0,     0,   132,
      21,    70,     0,   133,     0,     0,   128,   129,   745,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     5,     6,     7,     8,     9,    10,     0,
       0,    63,   127,    64,     0,    65,    66,     0,     0,     0,
       0,    68,     0,     0,   132,    21,    70,     0,   133,     0,
       0,   128,   129,   746,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     5,     6,
       7,     8,     9,    10,     0,     0,    63,   127,    64,     0,
      65,    66,     0,     0,     0,     0,    68,     0,     0,   132,
      21,    70,     0,   133,     0,     0,   128,   129,   130,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,    48,    49,    50,   131,   234,     0,     0,     0,
       0,     0,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,     0,   232,     0,     0,
       0,     0,   233,     0,    58,    59,    60,    61,     0,     0,
     234,     0,     0,   235,     0,     0,     0,   236,     0,     0,
       0,    63,     0,    64,   274,  -357,    66,     0,  -379,     0,
    -379,    68,     0,   121,   132,     0,    70,   237,   133,     0,
       0,     0,     0,     0,   234,     0,     0,   235,     0,     0,
       0,   236,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   237,   254,   255,   256,     0,     0,   257,   258,   259,
       0,   235,     0,     0,     0,   236,   238,   623,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   232,   254,   255,   256,     0,
     233,   257,   258,   259,     0,     0,     0,     0,   234,     0,
     238,   681,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,     0,   250,   251,   252,   253,   232,
     254,     0,   256,     0,   233,   257,   258,   259,     0,     0,
       0,     0,   234,     0,     0,   235,     0,     0,     0,   236,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,     0,   233,   237,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   235,
       0,     0,     0,   236,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   237,   254,   255,   256,     0,     0,   257,
     258,   259,     0,   235,     0,     0,     0,   236,   238,   804,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   237,   254,   255,
     256,     0,     0,   257,   258,   259,     0,     0,     0,     0,
       0,     0,   238,   839,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,   232,     0,   257,   258,   259,
     233,     0,     0,     0,     0,   509,     0,     0,   234,   309,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,   306,     0,   235,     0,     0,     0,   236,
       0,     0,     0,   307,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   121,     0,     0,     0,   237,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   235,
       0,     0,     0,   236,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   237,   254,   255,   256,     0,     0,   257,
     258,   259,     0,   235,   282,     0,     0,   236,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   232,   254,   255,
     256,     0,   233,   257,   258,   259,     0,     0,   282,     0,
     234,   587,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,     0,   246,   247,   248,     0,   250,   251,   252,
     253,     0,     0,     0,   256,     0,     0,   257,   258,   259,
       0,     0,     0,     0,     0,     0,     0,   235,     0,     0,
       0,   236,     0,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,   233,     0,   121,     0,     0,
       0,   237,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,   282,     0,   232,     0,
       0,   313,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,   186,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,     0,   257,   258,   259,     0,     0,   282,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,   237,     0,   234,   581,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,   282,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,   237,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   235,   257,   258,   259,   236,     0,
     282,     0,     0,     0,     0,   688,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   237,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,   282,     0,     0,     0,     0,   693,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,   237,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,   282,     0,     0,     0,
       0,   790,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,   237,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   235,   257,   258,   259,   236,     0,   282,
       0,     0,     0,     0,   791,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   237,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,   282,     0,     0,     0,     0,   792,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     237,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,   282,     0,     0,     0,     0,
     793,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   237,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,   282,     0,
       0,     0,     0,   806,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   237,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,   282,     0,   232,     0,     0,   807,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,     0,     0,   282,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   237,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,   337,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,   288,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,  -358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   121,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   232,   254,   255,   256,     0,   233,   257,   258,
     259,     0,     0,     0,     0,   234,     0,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,   232,     0,     0,     0,
       0,   233,   235,     0,     0,     0,   236,     0,     0,   234,
     440,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     441,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,   235,   242,   243,   244,
     236,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,     0,   257,   258,   259,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   232,   254,   255,   256,     0,   233,
     257,   258,   259,     0,     0,     0,     0,   234,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   443,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   235,     0,     0,     0,   236,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,     0,     0,   233,   237,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   235,     0,
       0,     0,   236,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   237,   254,   255,   256,     0,     0,   257,   258,
     259,     0,   235,     0,     0,     0,   236,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   237,   254,   255,   256,
       0,   292,   257,   258,   259,     0,     0,   232,     0,     0,
       0,   238,   233,   239,   240,   241,     0,   242,   243,   244,
     234,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   336,   257,   258,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,   233,   235,     0,     0,
       0,   236,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,   235,   242,   243,   244,   236,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     349,   257,   258,   259,   361,   237,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,     0,   257,   258,   259,     0,     0,
     232,     0,     0,     0,     0,   233,   235,     0,     0,     0,
     236,     0,     0,   234,     0,     0,     0,   444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
     235,   242,   243,   244,   236,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,     0,
     257,   258,   259,   233,   237,     0,     0,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,   533,   257,   258,   259,   233,   235,     0,
       0,     0,   236,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,   235,   242,   243,   244,   236,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
     232,   534,   257,   258,   259,   233,   237,     0,     0,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,   535,   257,   258,   259,   233,
     235,     0,     0,     0,   236,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,   235,   242,   243,   244,   236,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,   536,   257,   258,   259,   233,   237,     0,
       0,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   537,   257,   258,
     259,   233,   235,     0,     0,     0,   236,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,   235,   242,   243,   244,
     236,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,   538,   257,   258,   259,   233,
     237,     0,     0,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,   539,
     257,   258,   259,   233,   235,     0,     0,     0,   236,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,   235,   242,
     243,   244,   236,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   540,   257,   258,
     259,   233,   237,     0,     0,     0,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
     232,   541,   257,   258,   259,   233,   235,     0,     0,     0,
     236,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
     235,   242,   243,   244,   236,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,   542,
     257,   258,   259,   233,   237,     0,     0,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,   543,   257,   258,   259,   233,   235,     0,
       0,     0,   236,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,   235,   242,   243,   244,   236,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
     232,   544,   257,   258,   259,   233,   237,     0,     0,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,   545,   257,   258,   259,   233,
     235,     0,     0,     0,   236,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,   235,   242,   243,   244,   236,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,   546,   257,   258,   259,   233,   237,     0,
       0,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   547,   257,   258,
     259,   233,   235,     0,     0,     0,   236,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   121,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,   235,   242,   243,   244,
     236,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,     0,   257,   258,   259,   233,
     237,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     591,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   567,
     257,   258,   259,     0,   235,     0,     0,     0,   236,     0,
       0,     0,   232,   619,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,     0,   257,   258,
     259,     0,     0,     0,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   237,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,     0,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,   676,   626,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,     0,     0,     0,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   237,     0,   234,
     679,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,   739,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   237,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,     0,     0,   232,   772,     0,   742,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
       0,   257,   258,   259,     0,     0,     0,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,   237,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,     0,     0,   232,     0,     0,   812,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,     0,   257,   258,   259,     0,     0,   813,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     237,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,     0,     0,   232,     0,     0,
     814,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,     0,     0,   815,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,   237,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,     0,     0,   232,     0,
       0,   816,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,     0,   257,   258,   259,     0,     0,   817,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,   237,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,     0,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   555,
     254,   255,   256,   236,     0,   257,   258,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
       0,   250,   251,   252,   253,     0,   254,     0,   256,     0,
       0,   257,   258,   259
};

static const yytype_int16 yycheck[] =
{
      11,    41,    42,     2,   320,    16,   605,    15,    19,   342,
     159,   279,   325,   279,    25,    26,    27,    23,   161,   483,
     143,     3,    34,    34,    35,     1,    37,    38,     3,   342,
     153,   174,     1,    44,    45,    46,    47,    48,   795,     3,
      51,     3,    34,    54,    55,    56,   629,   629,    59,   192,
      24,    62,    63,    64,    44,    66,     1,    24,    69,    70,
      71,   157,   158,   159,    54,   669,     3,   216,   226,   227,
     228,   229,   829,   104,    34,   171,    80,     1,    24,    38,
      24,    71,    34,    20,   766,     3,    61,    63,    80,    34,
       0,    73,   104,   189,    63,   104,     3,    34,    57,   130,
      37,    34,    78,   199,    23,    25,   705,    25,   131,    78,
      34,   104,   104,    80,    51,    52,   127,   128,   129,   130,
     131,   132,   133,    63,    61,   134,   808,   103,    85,    72,
     104,   139,   275,   276,   103,    72,   147,   601,    83,   132,
      85,    60,   134,   133,   104,    64,    34,   123,   731,   131,
     161,   108,   104,   129,    72,   129,   131,     7,   103,   104,
     129,    85,   129,   174,   747,   747,   482,   131,   104,   131,
     176,   104,    69,    61,   134,   131,   780,   781,   123,   103,
     104,   192,   134,   129,   103,   129,   105,   106,   104,   129,
      50,   110,   829,   289,   131,   794,   132,   340,   117,   123,
     211,   134,   839,    70,    34,   124,   125,   126,    13,   208,
     131,   369,    63,    18,   130,    44,   312,    77,   131,    80,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,    16,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   733,
     130,    20,   123,   134,   104,    59,   123,   288,   129,   595,
     147,   104,   293,   104,     3,   553,   104,   553,     7,    34,
      74,   604,   123,   304,   161,   306,   307,     3,   309,   310,
      20,     7,   313,    52,   134,   123,   104,   174,   123,   132,
      34,   132,    61,   123,   304,   235,   236,   328,    63,    38,
     469,   470,    52,   334,    60,   192,   128,    76,    64,   340,
      23,    61,    52,    78,   132,   346,   347,   123,    57,    63,
      85,    61,    72,   354,     2,   356,   123,   358,   359,   123,
     361,   362,    72,    11,    78,   366,    34,   277,   278,   104,
     104,    85,    20,   104,   513,   514,   104,    60,   288,   465,
     466,    64,   131,   469,   470,    33,   366,   710,   123,   502,
     104,   714,   123,   104,   129,    63,   104,   307,   132,   103,
     310,   704,   106,    51,   132,   123,   110,   710,   129,   123,
      78,   714,   123,   104,   104,   129,   104,    85,   275,   276,
     103,    69,    60,   106,   132,    73,    64,   110,   104,   131,
     123,   133,   123,   123,    85,   123,   104,   104,    79,   440,
     441,   442,   443,   444,   445,   446,   131,   123,   358,   447,
     448,   452,   453,   104,   455,   123,   123,   108,   129,     3,
     461,   129,   463,     7,   106,     9,   467,   131,   131,   133,
     133,    83,   131,    85,   122,   131,    20,   334,    85,   565,
     566,   132,     7,   340,   485,    19,   123,   488,   489,   354,
     576,   356,   795,    85,   103,    29,    30,   354,   499,   356,
     586,   804,    36,   589,    38,   485,   123,    16,    52,   132,
      19,   104,   104,   123,   162,    34,   108,    61,    27,   499,
      29,    30,    26,    57,   132,   104,   829,    36,    72,    38,
     132,   441,    76,   443,    83,    85,   839,   132,   186,   104,
     132,   132,   129,   129,     2,    54,   134,    56,    57,    58,
      59,   132,   553,    11,   555,   556,   557,   134,    85,   560,
     208,    26,    20,    72,   123,    74,   132,   123,    23,   132,
     123,   572,   131,   104,   133,    33,   123,    24,   579,    24,
     581,   104,   134,   584,     3,   103,   587,   129,     7,    61,
     591,   134,   106,    51,   134,   131,   453,   132,   455,    78,
     520,    20,   347,   154,   460,    60,   686,   735,   735,    64,
     611,    69,   573,   736,   123,    73,   617,   593,   619,   630,
     621,   130,   623,   808,   794,   626,   778,   713,   329,   754,
     455,   611,   553,    52,   282,    -1,    -1,    -1,    -1,   628,
      -1,    -1,    61,    -1,    99,   555,   556,   102,   103,    -1,
     105,   106,    -1,    72,    -1,   110,    -1,    76,   306,    -1,
      -1,   309,   117,    -1,   122,   313,    -1,    -1,    -1,   124,
     125,   126,    -1,    -1,   675,   676,    -1,   678,   679,   680,
     681,    -1,    -1,    20,    -1,    23,    -1,   688,   689,    -1,
     691,    -1,   693,   694,    -1,    -1,   697,    34,   728,   729,
      37,    -1,    -1,     2,   162,    -1,    -1,    -1,   709,   710,
      -1,   359,    11,   714,    51,    52,   717,    -1,    -1,    -1,
     721,    20,    60,    -1,    61,    -1,    64,    -1,   186,    -1,
      -1,    -1,    -1,    -1,    33,    72,    -1,    -1,   739,   740,
     741,   742,   743,   744,   745,   746,    -1,    -1,    -1,    -1,
     208,    -1,    51,    -1,    -1,   756,   757,   758,    -1,    -1,
      -1,   762,    -1,    -1,   765,   103,    -1,   105,   106,    -1,
      69,   772,   110,    -1,    73,    -1,    -1,    -1,   758,   117,
      -1,    -1,    -1,    19,    -1,    -1,    -1,   435,   126,   790,
     791,   792,   793,    29,    30,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    38,   804,    -1,   806,   807,    -1,    44,    -1,
      -1,   812,   813,   814,   815,   816,   817,    -1,    -1,    -1,
      -1,    57,    -1,   122,   282,    -1,   474,   828,   829,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   839,    16,
      -1,    -1,    19,    -1,    -1,    -1,    -1,   704,   306,    -1,
      27,   309,    29,    30,    -1,   313,    -1,    -1,    -1,    36,
      -1,    38,    -1,   162,    -1,    -1,    -1,    -1,    -1,   517,
      -1,    -1,   520,    -1,    -1,    19,    -1,    54,    -1,    56,
      57,    58,    59,    -1,    23,    29,    30,   186,    -1,    -1,
      -1,    -1,    36,    -1,    38,    72,    -1,    74,    -1,    -1,
      44,   359,    -1,    -1,    -1,   762,    -1,   555,   556,   208,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,   579,     2,   581,    -1,    -1,   584,    -1,   795,   587,
      -1,    11,   590,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      20,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   610,    33,   103,    -1,   105,   106,    -1,    -1,
      -1,   110,   829,   621,    -1,   623,    -1,    -1,   117,    -1,
     628,    51,   839,   282,    -1,   124,   125,   126,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    69,
      -1,    -1,    -1,    73,    -1,    -1,    -1,   306,    -1,    -1,
     309,    25,    -1,    -1,   313,    -1,   474,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
     688,    55,    -1,    -1,    -1,   693,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    67,    68,    69,    70,     2,    -1,   517,
     359,    -1,   520,   711,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    85,   720,    -1,    -1,    20,   724,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,    33,    -1,
     104,    -1,   162,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    51,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   186,   131,   132,   133,
      -1,   579,    -1,   581,    69,    -1,   584,    -1,    73,   587,
      -1,    -1,   590,    -1,    -1,    -1,    -1,    -1,   208,    -1,
      -1,    -1,   790,   791,   792,   793,    -1,    -1,    -1,    -1,
      -1,    -1,   610,    -1,    -1,    -1,    -1,    -1,   806,   807,
      -1,    -1,    -1,   621,    -1,   623,    -1,    -1,    -1,    -1,
     628,    -1,    -1,    -1,    -1,   474,    -1,   122,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,
      -1,    16,    -1,    -1,    19,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    27,    -1,    29,    30,    -1,    -1,    -1,    -1,
      33,    36,   282,    38,    23,    -1,    -1,   162,   517,    -1,
      -1,   520,    -1,    -1,    -1,    -1,    -1,    -1,    51,    54,
     688,    56,    57,    58,    59,   693,   306,    -1,    -1,   309,
      -1,   186,    -1,   313,    -1,    -1,    69,    72,    -1,    74,
      73,    60,    -1,   711,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,   720,   208,    -1,    -1,   724,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     579,    -1,   581,    -1,    -1,   584,    -1,    -1,   587,   359,
      99,   590,   101,   102,   103,    -1,   105,   106,   123,   122,
      -1,   110,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,   610,   121,    -1,    -1,   124,   125,   126,    -1,    -1,
      -1,    -1,   621,    -1,   623,    -1,    -1,    -1,    -1,   628,
      -1,    -1,   790,   791,   792,   793,    -1,   282,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   806,   807,
      -1,    -1,    -1,     2,    23,    -1,    -1,    -1,    -1,    -1,
      -1,   306,    11,   186,   309,    -1,    -1,    -1,   313,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,
      -1,    -1,    -1,    -1,    33,   208,    -1,    11,    -1,   688,
      -1,    60,    -1,    -1,   693,    64,    20,    -1,    -1,    -1,
      -1,    -1,    51,    -1,   474,    -1,    -1,    -1,    -1,    33,
      -1,    -1,   711,    -1,   359,    -1,    -1,    -1,    -1,    -1,
      69,   720,    -1,    -1,    73,   724,    -1,    51,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    69,    -1,   517,   117,    73,
     520,    -1,    -1,    -1,    -1,   124,   125,   126,    -1,   282,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   306,    -1,    -1,   309,    -1,    47,    -1,
     313,   790,   791,   792,   793,    -1,    -1,    -1,   122,    -1,
      60,    -1,    -1,    -1,    64,    -1,    -1,   806,   807,   579,
      -1,   581,    -1,   162,   584,    -1,    -1,   587,    -1,    -1,
     590,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   474,
      -1,    -1,    -1,    -1,    -1,    -1,   359,   186,   162,    99,
     610,   101,   102,   103,    -1,   105,   106,   107,    -1,    -1,
     110,   621,    -1,   623,    -1,    -1,   116,   117,   628,   208,
      -1,   121,   186,    -1,   124,   125,   126,   126,    -1,    -1,
      -1,    -1,   517,    -1,    -1,   520,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,
     159,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,   688,    -1,
      -1,    -1,    -1,   693,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,    -1,   282,   579,    -1,   581,    -1,    -1,   584,
     199,   711,   587,    -1,    -1,   590,    -1,    -1,    -1,    -1,
     720,   474,    -1,    -1,   724,    -1,    -1,   306,   282,    -1,
     309,    -1,    -1,    -1,   313,   610,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   621,    -1,   623,    -1,
      -1,    -1,   306,   628,    -1,   309,    -1,    -1,    -1,   313,
      -1,    -1,    -1,    -1,   517,    -1,    -1,   520,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     359,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     790,   791,   792,   793,    -1,    -1,    -1,    -1,    -1,    -1,
     289,    -1,    -1,    -1,    -1,   359,   806,   807,    -1,    -1,
      -1,    -1,    -1,   688,    -1,    -1,    -1,    -1,   693,    -1,
      -1,    -1,    -1,   312,    -1,    -1,   579,    -1,   581,    -1,
      -1,   584,    -1,    -1,   587,    -1,   711,   590,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   720,    -1,    -1,    -1,   724,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   610,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,   621,    12,
     623,    -1,    -1,    -1,    -1,   628,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,   474,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,   790,   791,   792,   793,    -1,
     474,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,   806,   807,    -1,    -1,   688,    -1,    -1,   517,    -1,
     693,   520,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,    -1,    -1,    -1,   100,   711,    -1,
      -1,    20,    -1,   517,    -1,    -1,   520,   720,    -1,    -1,
      -1,   724,    -1,    -1,    33,   118,   465,   466,    -1,    -1,
     469,   470,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,
     133,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     579,    -1,   581,    -1,    -1,   584,    -1,    -1,   587,    -1,
      69,   590,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   579,    -1,   581,    -1,    -1,
     584,   610,    -1,   587,    -1,    -1,   590,   790,   791,   792,
     793,    13,   621,    -1,   623,    -1,    18,    -1,    -1,   628,
      22,    23,    24,   806,   807,    -1,   610,    -1,    30,    -1,
      32,    -1,    -1,   122,    -1,    -1,    -1,   621,    -1,   623,
      -1,    -1,    -1,    -1,   628,    -1,   565,   566,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,   576,    -1,   578,
      -1,    -1,    -1,    -1,   583,    -1,    -1,   586,    -1,    -1,
     589,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,   688,
      -1,    -1,    -1,    -1,   693,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      -1,    -1,   711,    -1,   688,    -1,    -1,    -1,    -1,   693,
      -1,   720,    -1,    -1,    -1,   724,    -1,    -1,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   711,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   720,    -1,    -1,    -1,
     724,   143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,   153,    -1,   155,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,    -1,    -1,   179,    -1,    -1,
      -1,   790,   791,   792,   793,    -1,    -1,    -1,    -1,    -1,
     192,    -1,    -1,   282,   713,    -1,    -1,   806,   807,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   790,   791,   792,   793,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   306,    -1,   221,
     309,    -1,   806,   807,   226,   227,   228,   229,    -1,    -1,
     749,   750,    -1,   752,   753,    -1,    -1,    -1,    -1,    -1,
      -1,   243,    -1,    -1,    -1,    -1,    -1,   766,    -1,    -1,
      -1,    -1,    -1,    -1,   773,    -1,   775,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     359,    -1,    -1,   275,   276,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   808,
       1,    -1,     3,     4,     5,     6,     7,     8,   300,    10,
     302,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    25,    -1,    -1,    -1,   320,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,   329,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,   340,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   369,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   474,    -1,    -1,    99,   100,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    -1,    -1,   124,   125,   126,    -1,    -1,   129,    -1,
     131,    -1,   133,    -1,    -1,    -1,    23,    -1,   517,    -1,
      -1,   520,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   455,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,     3,    64,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,   478,    -1,    15,    -1,
     482,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
     579,    -1,   581,    -1,    -1,   584,    -1,    -1,   587,    -1,
     502,   590,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,   610,   119,   120,   121,   527,    -1,   124,   125,   126,
      -1,    -1,   621,    -1,   623,    -1,    -1,    -1,    -1,   628,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,   105,    -1,
      -1,   573,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    -1,   121,    -1,    -1,   124,   125,   126,
     127,   128,    -1,   595,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   711,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   720,    -1,    -1,    -1,   724,    -1,    -1,    -1,    -1,
      -1,    -1,     0,     1,   646,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    -1,    81,    -1,   727,    84,    -1,    -1,    -1,
      -1,    -1,    -1,   735,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,    -1,
     124,   125,   126,    -1,    -1,   129,    -1,   131,     1,   133,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,     1,   107,     3,     4,     5,     6,     7,
       8,    -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,    -1,
     133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,     7,
       8,    -1,    -1,   116,    12,   118,    -1,   120,   121,   122,
      -1,    -1,    20,   126,    -1,    -1,   129,    25,   131,    -1,
     133,   134,    -1,    31,    32,    33,    34,    35,    -1,    37,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,   108,    -1,     3,     4,     5,
       6,     7,     8,   116,    -1,   118,    12,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,    -1,   129,    -1,   131,    25,
     133,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,     3,     4,     5,     6,     7,     8,    -1,    -1,
     116,    12,   118,    -1,   120,   121,   122,    -1,    -1,    -1,
     126,    -1,    -1,   129,    25,   131,   132,   133,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,
       6,     7,     8,    -1,    -1,   116,    12,   118,    -1,   120,
     121,    -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,
     131,    -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,     3,     4,     5,     6,     7,     8,    -1,    -1,
     116,    12,   118,    -1,   120,   121,   122,    -1,    -1,    -1,
     126,    -1,    -1,   129,    25,   131,    -1,   133,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,
       6,     7,     8,    -1,    -1,   116,    12,   118,    -1,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,
     131,    -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,   129,    -1,   131,    -1,   133,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,     3,     4,     5,     6,     7,     8,    -1,
      -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,   126,    -1,    -1,   129,    25,   131,    -1,   133,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,     4,
       5,     6,     7,     8,    -1,    -1,   116,    12,   118,    -1,
     120,   121,    -1,    -1,    -1,    -1,   126,    -1,    -1,   129,
      25,   131,    -1,   133,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    47,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,     3,     4,     5,     6,     7,     8,    -1,
      -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,   126,    -1,    -1,   129,    25,   131,    -1,   133,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,     4,
       5,     6,     7,     8,    -1,    -1,   116,    12,   118,    -1,
     120,   121,   122,    -1,    -1,    -1,   126,    -1,    -1,   129,
      25,   131,    -1,   133,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,     3,     4,     5,     6,     7,     8,    -1,
      -1,   116,    12,   118,    -1,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,    -1,   129,    25,   131,    -1,   133,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,     4,
       5,     6,     7,     8,    -1,    -1,   116,    12,   118,    -1,
     120,   121,   122,    -1,    -1,    -1,   126,    -1,    -1,   129,
      25,   131,    -1,   133,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,     3,     4,     5,     6,     7,     8,    -1,
      -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,   126,    -1,    -1,   129,    25,   131,    -1,   133,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   107,     3,     4,     5,     6,     7,     8,    -1,
      -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,   126,    -1,    -1,   129,    25,   131,    -1,   133,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   107,     3,     4,     5,     6,     7,     8,    -1,
      -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,   126,    -1,    -1,   129,    25,   131,    -1,   133,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   107,     3,     4,     5,     6,     7,     8,    -1,
      -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,   126,    -1,    -1,   129,    25,   131,    -1,   133,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   107,     3,     4,     5,     6,     7,     8,    -1,
      -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,   126,    -1,    -1,   129,    25,   131,    -1,   133,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,     4,
       5,     6,     7,     8,    -1,    -1,   116,    12,   118,    -1,
     120,   121,    -1,    -1,    -1,    -1,   126,    -1,    -1,   129,
      25,   131,    -1,   133,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    67,    68,    69,    70,    23,    -1,    -1,    -1,
      -1,    -1,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    99,   100,   101,   102,    -1,    -1,
      23,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      -1,   116,    -1,   118,   127,   128,   121,    -1,   131,    -1,
     133,   126,    -1,    80,   129,    -1,   131,    84,   133,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    60,    -1,    -1,
      -1,    64,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    84,   119,   120,   121,    -1,    -1,   124,   125,   126,
      -1,    60,    -1,    -1,    -1,    64,    99,   134,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    10,   119,   120,   121,    -1,
      15,   124,   125,   126,    -1,    -1,    -1,    -1,    23,    -1,
      99,   134,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,    10,
     119,    -1,   121,    -1,    15,   124,   125,   126,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    60,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    60,
      -1,    -1,    -1,    64,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    84,   119,   120,   121,    -1,    -1,   124,
     125,   126,    -1,    60,    -1,    -1,    -1,    64,    99,   134,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    84,   119,   120,
     121,    -1,    -1,   124,   125,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   134,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    10,    -1,   124,   125,   126,
      15,    -1,    -1,    -1,    -1,   132,    -1,    -1,    23,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    24,    -1,    60,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    60,
      -1,    -1,    -1,    64,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    84,   119,   120,   121,    -1,    -1,   124,
     125,   126,    -1,    60,   129,    -1,    -1,    64,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    10,   119,   120,
     121,    -1,    15,   124,   125,   126,    -1,    -1,   129,    -1,
      23,    24,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,    -1,   110,   111,   112,    -1,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,    -1,   124,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    80,    -1,    -1,
      -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
      60,   124,   125,   126,    64,    -1,   129,    -1,    10,    -1,
      -1,    71,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    60,   119,
     120,   121,    64,    -1,   124,   125,   126,    -1,    -1,   129,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    23,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,    60,   124,   125,   126,    64,    -1,   129,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
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
     126,    64,    -1,   129,    -1,    -1,    -1,    -1,    71,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
      60,   124,   125,   126,    64,    -1,   129,    -1,    -1,    -1,
      -1,    71,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    -1,    60,   124,   125,   126,    64,    -1,   129,
      -1,    -1,    -1,    -1,    71,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,    60,   124,   125,   126,
      64,    -1,   129,    -1,    -1,    -1,    -1,    71,    -1,    10,
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
      -1,   129,    -1,    10,    -1,    -1,    71,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    60,   119,   120,   121,    64,    -1,   124,
     125,   126,    -1,    -1,   129,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,    60,   124,   125,   126,
      64,    -1,   129,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,    -1,
     124,   125,   126,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    10,   119,   120,   121,    -1,    15,   124,   125,
     126,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    60,   105,   106,   107,
      64,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,    -1,   124,   125,   126,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    10,   119,   120,   121,    -1,    15,
     124,   125,   126,    -1,    -1,    -1,    -1,    23,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    60,    -1,
      -1,    -1,    64,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    84,   119,   120,   121,    -1,    -1,   124,   125,
     126,    -1,    60,    -1,    -1,    -1,    64,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    84,   119,   120,   121,
      -1,   123,   124,   125,   126,    -1,    -1,    10,    -1,    -1,
      -1,    99,    15,   101,   102,   103,    -1,   105,   106,   107,
      23,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    60,    -1,    -1,
      -1,    64,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    60,   105,   106,   107,    64,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
     123,   124,   125,   126,    83,    84,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,    -1,   124,   125,   126,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    60,    -1,    -1,    -1,
      64,    -1,    -1,    23,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      60,   105,   106,   107,    64,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    10,    -1,
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
      -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    60,   105,   106,   107,
      64,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    10,    -1,   124,   125,   126,    15,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,   123,
     124,   125,   126,    -1,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    60,   119,   120,   121,    64,    -1,   124,   125,
     126,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,    60,   124,   125,   126,    64,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    60,   119,   120,   121,    64,    -1,   124,
     125,   126,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,    60,   124,   125,   126,
      64,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,    -1,
     124,   125,   126,    -1,    -1,    71,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    60,   124,   125,
     126,    64,    -1,    -1,    -1,    10,    11,    -1,    71,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    60,   119,   120,   121,    64,
      -1,   124,   125,   126,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,    60,   124,
     125,   126,    64,    -1,    -1,    -1,    10,    -1,    -1,    71,
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
     124,   125,   126,    64,    -1,    -1,    -1,    10,    -1,    -1,
      71,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    60,   119,   120,
     121,    64,    -1,   124,   125,   126,    -1,    -1,    71,    -1,
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
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,    -1,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    60,   119,   120,
     121,    64,    -1,   124,   125,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,   119,    -1,   121,    -1,
      -1,   124,   125,   126
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
     130,   141,   130,   235,   104,   132,   104,   132,    34,   134,
     229,    83,   104,   134,     7,   155,   103,   170,   180,    61,
     205,   205,   205,   205,   235,   235,   235,   235,   163,   235,
     163,   235,   235,   235,   235,   235,   235,   235,    25,    72,
     155,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   219,   219,   163,   235,   163,   235,    20,    34,
      37,    51,    52,    61,    72,   190,   230,   231,   235,   247,
      24,    34,    24,    34,    71,    34,   134,   163,   235,   157,
     123,   235,    83,    85,   132,   104,   155,   173,   174,   123,
     155,    34,   216,    33,   235,   163,   235,    33,   235,   163,
     235,   157,   143,   235,    26,   132,   187,   188,   189,   190,
     177,   208,   104,   132,     1,   133,   212,   223,    83,    85,
     209,   235,   206,   132,   196,   235,   166,   219,   132,   133,
     212,   223,   104,   126,   148,   150,   235,   148,   149,   132,
     132,   217,   217,   163,   235,   143,   235,   134,   235,   235,
     129,   216,   129,    52,    61,    72,   182,   191,   205,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   132,   134,
      34,    61,   222,   104,   132,    60,    60,    33,   163,   235,
      33,   163,   235,   163,   235,   228,   228,   123,   235,   196,
     235,   221,    85,   104,   130,   170,   235,   132,   235,    24,
     157,    24,   157,   235,    24,   157,   229,    24,   157,   229,
      26,    26,   143,   104,   132,   131,   155,    20,    52,    61,
      72,   193,   132,   208,   103,   211,   216,   235,    47,   235,
      50,    77,   130,   167,   132,   216,   201,   104,   123,    11,
     123,   134,   229,   134,   229,   143,    83,   130,   147,   171,
     181,    10,    15,    85,    86,    87,    88,    89,    90,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   105,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   121,   124,   125,   126,   127,   128,   155,   183,   184,
     123,   231,   222,   222,   235,    24,    24,   235,    24,    24,
     134,   134,   157,   157,   235,   174,   129,   157,    71,    33,
     235,    33,   235,    71,    33,   235,   157,    33,   235,   157,
     143,   235,   188,   208,   103,   215,    61,   211,   209,    34,
     134,    24,   143,   216,   134,   235,   235,    33,   143,   235,
     134,    33,   143,   235,   134,   235,   130,    30,    56,    58,
     130,   142,   159,   193,   155,   106,   131,   185,   185,    71,
      33,    33,    71,    33,    33,    33,    33,   171,   235,   235,
     235,   235,   235,   235,   132,    25,    67,    70,   133,   196,
     214,   223,   108,   198,   209,    78,   199,   235,   212,   223,
     143,   157,    11,   235,   143,   235,   143,   159,   211,   155,
     183,   184,   187,   235,   235,   235,   235,   235,   235,   130,
      71,    71,    71,    71,   215,   134,   216,   196,   197,   235,
     235,   146,   158,   195,   134,   235,    71,    71,   199,   185,
     185,   132,    71,    71,    71,    71,    71,    71,   235,   235,
     235,   235,   198,   209,   196,   213,   214,   223,    34,   134,
     223,   235,   235,   123,   194,   195,   235,   213,   214,   134,
     213
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
     241,   241,   241,   241,   241,   241,   241,   241,   242,   242,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   244,   244,   244,   244,   244,   244,   245,
     245,   245,   245,   246,   246,   246,   246,   247,   247,   247,
     247,   247,   247,   247
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
       1,     1,     1,     1,     1,     3,     3,     3,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     1
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
#line 4528 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 422 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4534 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 423 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4540 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 430 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4546 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 435 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4552 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 436 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4558 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 452 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4564 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 453 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 454 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4576 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 455 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4582 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 456 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4588 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 457 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4594 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 458 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4600 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 459 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4606 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4612 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 461 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 462 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 463 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4630 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 464 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4636 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4642 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 466 "chapel.ypp" /* yacc.c:1661  */
    { printf("syntax error"); clean_exit(1); }
#line 4648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yyvsp[-4].b), (yylsp[-4]).comment))); }
#line 4654 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 473 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-5].b), (yylsp[-5]).comment))); }
#line 4660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 477 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4672 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 4684 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 495 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 4690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 500 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 501 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 506 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 4712 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 511 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 4722 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 4731 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 520 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 4740 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 526 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 4750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 535 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 4760 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 544 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 4766 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 545 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 4772 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 546 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 4778 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 550 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 4784 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 4790 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 4796 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 556 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 4802 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 557 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 4808 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 4814 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 4820 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 560 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 4826 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 561 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 4832 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 4838 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 4844 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 4850 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 4856 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 4862 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 567 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 4868 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 568 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 4874 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 4880 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4886 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4892 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 582 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4898 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4904 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4910 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 591 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 4916 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 4922 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 4928 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 599 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4940 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 4956 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 4962 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4968 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 628 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4974 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 4980 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 630 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4986 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 4992 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 4998 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5004 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5010 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5016 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 636 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5022 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5028 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 5034 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 5040 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5046 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5052 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 643 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5062 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 649 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5072 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 655 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5082 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 661 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5092 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 667 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5103 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 674 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5114 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 683 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[-1].pcallexpr)); }
#line 5120 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 687 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5126 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 688 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5132 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 689 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5138 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 690 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5144 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 695 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5150 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 699 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5156 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 700 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5162 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 705 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5168 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5174 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 709 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5180 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 716 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), NULL, (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5189 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 721 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-6].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5198 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 728 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5208 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 733 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5218 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 738 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 746 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 5234 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 747 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 5240 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5246 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 754 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 5252 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 756 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 5258 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 761 "chapel.ypp" /* yacc.c:1661  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 5269 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 5282 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 780 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 5290 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 784 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 5299 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 791 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 5305 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 792 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 5311 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 797 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5320 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 802 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5329 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 5349 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 827 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 833 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5372 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 5385 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 5398 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 859 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 865 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5419 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 5442 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 894 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 898 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5459 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 903 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
    }
#line 5467 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 907 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 915 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 916 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 5488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 917 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 5494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 918 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 5500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 919 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 5506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 920 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 5512 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 921 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 5518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 922 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 5524 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 923 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 5530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 924 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 5536 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 925 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 5542 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 926 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 5548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 927 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 5554 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 928 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 5560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 929 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 5566 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 930 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 5572 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 931 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 5578 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 932 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 5584 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 933 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 5590 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 934 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 5596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 935 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 5602 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 936 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 5608 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 937 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 5614 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 938 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 5620 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 939 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 5626 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 943 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 5632 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 944 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 5638 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 945 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 5644 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 946 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 5650 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 947 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 5656 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 948 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 5662 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 949 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 5668 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 950 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 5674 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 951 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 5680 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 952 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 5686 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 953 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 5692 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 954 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 5698 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 958 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 5704 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 959 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5710 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 963 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5716 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 967 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 5722 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 968 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 5728 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 969 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 5734 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 974 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 5740 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 976 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 5746 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 978 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 5752 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 980 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 5758 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 984 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5764 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 985 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 5770 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 989 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 5776 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 990 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 5782 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 991 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 5788 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 992 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 5794 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 993 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 5800 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 994 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 5806 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 995 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5812 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 996 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 5818 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 997 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 5824 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1001 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5830 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1002 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5836 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1003 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 5842 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1004 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 5848 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1008 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 5854 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1009 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 5860 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1013 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 5866 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1014 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 5872 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1015 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 5878 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1016 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 5884 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1017 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 5890 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1018 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 5896 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1022 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 5902 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1028 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 5908 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1033 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 5914 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1035 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5920 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1039 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5926 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1044 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5932 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1045 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 5938 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1049 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 5944 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1050 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5950 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1055 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 5956 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1057 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 5962 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1059 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 5968 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 5985 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6003 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1093 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6009 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1095 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6015 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1097 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 6021 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6033 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6045 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6057 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6070 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1135 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6081 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1144 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6087 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1145 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6093 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1146 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6099 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1152 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6108 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1160 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6114 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1162 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 6124 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1168 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6130 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1173 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 6136 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1175 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6142 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1177 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 6148 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1182 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 6154 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1184 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6160 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1186 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6166 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1192 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6172 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1193 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 6178 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6184 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6190 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1200 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6196 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1204 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6202 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1205 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6208 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1206 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 6214 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1207 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad type specification"); }
#line 6220 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1228 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 6228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1232 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 6236 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6248 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1243 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad array type specification"); clean_exit(1); }
#line 6254 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1247 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6260 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1248 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6266 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1249 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1254 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6278 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1256 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6284 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1262 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6290 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1264 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6296 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1266 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 6302 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1270 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6308 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1271 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6314 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1272 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6320 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1273 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 6326 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1274 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6332 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1275 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6338 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1276 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6344 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1282 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6350 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1283 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6356 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1284 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6362 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1285 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6368 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1289 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6374 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1290 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6380 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1291 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6386 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1295 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6392 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1296 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6398 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1300 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6404 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1305 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6410 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1306 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6416 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1310 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6422 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1311 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6428 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1312 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6434 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1313 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6440 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1318 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6446 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1335 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6452 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1337 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6458 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1339 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1341 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6470 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1343 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 6476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1345 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1347 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1352 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1354 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1356 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6512 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1360 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1362 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6524 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1364 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1366 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6536 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1368 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6542 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1370 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1372 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6554 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1374 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1376 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 6571 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1383 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 6581 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1389 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 6591 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1395 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 6601 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1401 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 6611 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1410 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 6617 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1414 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 6623 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1430 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6629 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1434 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6635 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1439 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 6641 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1444 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pIntentExpr).first, (yyvsp[0].pIntentExpr).second); }
#line 6647 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1446 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).first);
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).second);
    }
#line 6656 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6671 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1464 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).first = (yyvsp[-2].pexpr), (yyval.pIntentExpr).second = (yyvsp[0].pexpr); }
#line 6677 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).first = (yyvsp[-2].pexpr), (yyval.pIntentExpr).second = (yyvsp[0].pexpr); }
#line 6683 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6689 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 6695 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 6701 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 6707 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6713 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1501 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6719 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 6725 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 6731 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1507 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 6737 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6743 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6749 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1532 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 6755 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6761 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 6767 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 386:
#line 1538 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 6773 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 387:
#line 1546 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6779 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 388:
#line 1547 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 6785 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1548 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 6791 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1552 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 6797 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 6803 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 6809 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 6815 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1556 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 6821 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1557 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6827 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 6833 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1560 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 6841 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6847 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1567 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6853 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6859 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6865 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6871 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6877 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6883 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6889 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6895 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6901 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6907 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1580 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6913 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6919 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1582 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6925 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1583 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6931 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1584 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6937 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1585 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6943 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6949 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6955 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6961 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1589 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6967 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1590 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6973 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1591 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6979 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6985 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1593 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6991 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1597 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 6997 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 7003 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1599 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 7009 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 7015 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1601 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 7021 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 7027 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7033 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7039 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7045 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1609 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7051 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1613 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7057 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7063 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1615 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7069 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7075 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 7081 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 7087 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 7093 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1624 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 7099 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1625 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 7105 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 7111 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 7117 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 7121 "bison-chapel.cpp" /* yacc.c:1661  */
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
