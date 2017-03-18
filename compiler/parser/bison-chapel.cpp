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
#line 32 "chapel.ypp" /* yacc.c:355  */

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;
#line 45 "chapel.ypp" /* yacc.c:355  */

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif
#line 65 "chapel.ypp" /* yacc.c:355  */

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
    Expr*     iVar;
    IntentTag tfIntent; // undefined for a reduce intent
    Expr*     riExp;    // non-NULL for a reduce intent
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
    ForallIntents*    pForallIntents;
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif
#line 135 "chapel.ypp" /* yacc.c:355  */

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
#line 157 "chapel.ypp" /* yacc.c:355  */

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

#line 227 "bison-chapel.cpp" /* yacc.c:355  */

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
    TCATCH = 271,
    TCLASS = 272,
    TCOBEGIN = 273,
    TCOFORALL = 274,
    TCONFIG = 275,
    TCONST = 276,
    TCONTINUE = 277,
    TDELETE = 278,
    TDMAPPED = 279,
    TDO = 280,
    TDOMAIN = 281,
    TELSE = 282,
    TENUM = 283,
    TEXCEPT = 284,
    TEXPORT = 285,
    TEXTERN = 286,
    TFOR = 287,
    TFORALL = 288,
    TFORWARDING = 289,
    TIF = 290,
    TIN = 291,
    TINDEX = 292,
    TINLINE = 293,
    TINOUT = 294,
    TITER = 295,
    TLABEL = 296,
    TLAMBDA = 297,
    TLET = 298,
    TLOCAL = 299,
    TMINUSMINUS = 300,
    TMODULE = 301,
    TNEW = 302,
    TNIL = 303,
    TNOINIT = 304,
    TON = 305,
    TONLY = 306,
    TOTHERWISE = 307,
    TOUT = 308,
    TPARAM = 309,
    TPLUSPLUS = 310,
    TPRAGMA = 311,
    TPRIMITIVE = 312,
    TPRIVATE = 313,
    TPROC = 314,
    TPUBLIC = 315,
    TRECORD = 316,
    TREDUCE = 317,
    TREF = 318,
    TREQUIRE = 319,
    TRETURN = 320,
    TSCAN = 321,
    TSELECT = 322,
    TSERIAL = 323,
    TSINGLE = 324,
    TSPARSE = 325,
    TSUBDOMAIN = 326,
    TSYNC = 327,
    TTHEN = 328,
    TTHROW = 329,
    TTHROWS = 330,
    TTRY = 331,
    TTRYBANG = 332,
    TTYPE = 333,
    TUNDERSCORE = 334,
    TUNION = 335,
    TUSE = 336,
    TVAR = 337,
    TWHEN = 338,
    TWHERE = 339,
    TWHILE = 340,
    TWITH = 341,
    TYIELD = 342,
    TZIP = 343,
    TALIAS = 344,
    TAND = 345,
    TASSIGN = 346,
    TASSIGNBAND = 347,
    TASSIGNBOR = 348,
    TASSIGNBXOR = 349,
    TASSIGNDIVIDE = 350,
    TASSIGNEXP = 351,
    TASSIGNLAND = 352,
    TASSIGNLOR = 353,
    TASSIGNMINUS = 354,
    TASSIGNMOD = 355,
    TASSIGNMULTIPLY = 356,
    TASSIGNPLUS = 357,
    TASSIGNSL = 358,
    TASSIGNSR = 359,
    TBAND = 360,
    TBNOT = 361,
    TBOR = 362,
    TBXOR = 363,
    TCOLON = 364,
    TCOMMA = 365,
    TDIVIDE = 366,
    TDOT = 367,
    TDOTDOT = 368,
    TDOTDOTDOT = 369,
    TEQUAL = 370,
    TEXP = 371,
    TGREATER = 372,
    TGREATEREQUAL = 373,
    THASH = 374,
    TLESS = 375,
    TLESSEQUAL = 376,
    TMINUS = 377,
    TMOD = 378,
    TNOT = 379,
    TNOTEQUAL = 380,
    TOR = 381,
    TPLUS = 382,
    TQUESTION = 383,
    TSEMI = 384,
    TSHIFTLEFT = 385,
    TSHIFTRIGHT = 386,
    TSTAR = 387,
    TSWAP = 388,
    TASSIGNREDUCE = 389,
    TIO = 390,
    TLCBR = 391,
    TRCBR = 392,
    TLP = 393,
    TRP = 394,
    TLSBR = 395,
    TRSBR = 396,
    TNOELSE = 397,
    TUPLUS = 398,
    TUMINUS = 399
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
#line 188 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 417 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 423 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 38 "chapel.ypp" /* yacc.c:359  */

  #include <string>
  int         captureTokens;
  std::string captureString;
#line 196 "chapel.ypp" /* yacc.c:359  */

  #include "build.h"
  #include "CatchStmt.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForLoop.h"
  #include "parser.h"
  #include "stmt.h"
  #include "stringutil.h"
  #include "TryStmt.h"
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

#line 492 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   13163

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  145
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  120
/* YYNRULES -- Number of rules.  */
#define YYNRULES  478
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  907

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   399

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
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   434,   434,   439,   440,   446,   447,   452,   453,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   489,   491,   496,
     497,   498,   513,   514,   519,   520,   525,   530,   535,   539,
     545,   550,   554,   559,   563,   564,   565,   569,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   592,   593,   597,   601,   602,   606,
     607,   611,   612,   616,   617,   618,   619,   620,   621,   622,
     623,   627,   628,   632,   633,   634,   635,   639,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,   664,   665,
     666,   667,   668,   669,   670,   676,   682,   688,   694,   701,
     711,   715,   716,   717,   718,   722,   723,   724,   725,   726,
     727,   731,   732,   736,   737,   743,   744,   745,   749,   752,
     757,   758,   762,   764,   766,   773,   778,   786,   791,   796,
     804,   805,   810,   811,   813,   818,   828,   837,   841,   849,
     850,   855,   860,   854,   885,   891,   898,   906,   917,   923,
     916,   951,   955,   960,   964,   972,   973,   977,   978,   979,
     980,   981,   982,   983,   984,   985,   986,   987,   988,   989,
     990,   991,   992,   993,   994,   995,   996,   997,   998,   999,
    1000,  1001,  1002,  1006,  1007,  1008,  1009,  1010,  1011,  1012,
    1013,  1014,  1015,  1016,  1017,  1021,  1022,  1026,  1030,  1031,
    1032,  1036,  1038,  1040,  1042,  1047,  1048,  1052,  1053,  1054,
    1055,  1056,  1057,  1058,  1059,  1060,  1064,  1065,  1066,  1067,
    1068,  1069,  1073,  1074,  1078,  1079,  1080,  1081,  1082,  1083,
    1087,  1088,  1091,  1092,  1096,  1097,  1101,  1103,  1108,  1109,
    1113,  1114,  1118,  1119,  1123,  1125,  1127,  1132,  1145,  1162,
    1163,  1165,  1170,  1178,  1186,  1194,  1203,  1213,  1214,  1215,
    1219,  1220,  1228,  1230,  1236,  1241,  1243,  1245,  1250,  1252,
    1254,  1261,  1262,  1263,  1267,  1268,  1273,  1274,  1275,  1276,
    1296,  1300,  1304,  1312,  1316,  1317,  1318,  1322,  1324,  1330,
    1332,  1334,  1339,  1340,  1341,  1342,  1343,  1344,  1345,  1351,
    1352,  1353,  1354,  1358,  1359,  1360,  1364,  1365,  1369,  1370,
    1374,  1375,  1379,  1380,  1381,  1382,  1383,  1387,  1398,  1399,
    1400,  1401,  1402,  1403,  1405,  1407,  1409,  1411,  1413,  1415,
    1420,  1422,  1424,  1426,  1428,  1430,  1432,  1434,  1436,  1438,
    1440,  1442,  1444,  1451,  1457,  1463,  1469,  1478,  1483,  1491,
    1492,  1493,  1494,  1495,  1496,  1497,  1498,  1503,  1504,  1508,
    1513,  1516,  1521,  1526,  1529,  1534,  1538,  1539,  1543,  1544,
    1549,  1554,  1562,  1563,  1564,  1565,  1566,  1567,  1568,  1569,
    1570,  1572,  1574,  1576,  1578,  1580,  1585,  1586,  1587,  1588,
    1599,  1600,  1604,  1605,  1606,  1610,  1611,  1612,  1620,  1621,
    1622,  1623,  1627,  1628,  1629,  1630,  1631,  1632,  1633,  1634,
    1635,  1636,  1640,  1648,  1649,  1653,  1654,  1655,  1656,  1657,
    1658,  1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,  1667,
    1668,  1669,  1670,  1671,  1672,  1673,  1674,  1675,  1679,  1680,
    1681,  1682,  1683,  1684,  1688,  1689,  1690,  1691,  1695,  1696,
    1697,  1698,  1703,  1704,  1705,  1706,  1707,  1708,  1709
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "CSTRINGLITERAL", "EXTERNCODE", "TALIGN",
  "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBY", "TCATCH", "TCLASS",
  "TCOBEGIN", "TCOFORALL", "TCONFIG", "TCONST", "TCONTINUE", "TDELETE",
  "TDMAPPED", "TDO", "TDOMAIN", "TELSE", "TENUM", "TEXCEPT", "TEXPORT",
  "TEXTERN", "TFOR", "TFORALL", "TFORWARDING", "TIF", "TIN", "TINDEX",
  "TINLINE", "TINOUT", "TITER", "TLABEL", "TLAMBDA", "TLET", "TLOCAL",
  "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TON", "TONLY",
  "TOTHERWISE", "TOUT", "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE",
  "TPRIVATE", "TPROC", "TPUBLIC", "TRECORD", "TREDUCE", "TREF", "TREQUIRE",
  "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSINGLE", "TSPARSE",
  "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHROW", "TTHROWS", "TTRY", "TTRYBANG",
  "TTYPE", "TUNDERSCORE", "TUNION", "TUSE", "TVAR", "TWHEN", "TWHERE",
  "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN",
  "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNSL", "TASSIGNSR", "TBAND",
  "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL",
  "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT",
  "TSTAR", "TSWAP", "TASSIGNREDUCE", "TIO", "TLCBR", "TRCBR", "TLP", "TRP",
  "TLSBR", "TRSBR", "TNOELSE", "TUPLUS", "TUMINUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_stmt", "access_control", "block_stmt", "stmt_ls", "only_ls",
  "opt_only_ls", "except_ls", "use_stmt", "require_stmt",
  "assignment_stmt", "opt_ident", "ident", "opt_ident_or_string",
  "do_stmt", "return_stmt", "class_level_stmt", "private_decl",
  "forwarding_stmt", "extern_block_stmt", "loop_stmt", "zippered_iterator",
  "if_stmt", "try_stmt", "catch_stmt_ls", "catch_stmt", "catch_expr",
  "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
  "class_decl_stmt", "class_tag", "opt_inherit", "class_level_stmt_ls",
  "enum_decl_stmt", "enum_ls", "enum_item", "lambda_decl_expr", "$@1",
  "$@2", "linkage_spec", "fn_decl_stmt", "$@3", "$@4",
  "fn_decl_stmt_inner", "fn_decl_receiver_expr", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls",
  "formal", "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_or_iter", "opt_ret_tag", "opt_throws_error",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "opt_query_expr", "var_arg_expr", "opt_where_part",
  "type_alias_decl_stmt", "type_alias_decl_stmt_inner", "opt_init_type",
  "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "opt_reindex_expr",
  "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "opt_formal_type", "expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "for_expr", "cond_expr", "nil_expr",
  "stmt_level_expr", "opt_task_intent_ls", "task_intent_clause",
  "task_intent_ls", "forall_intent_clause", "forall_intent_ls",
  "intent_expr", "io_expr", "new_expr", "let_expr", "expr", "lhs_expr",
  "fun_expr", "call_expr", "dot_expr", "parenthesized_expr",
  "literal_expr", "assoc_expr_ls", "binary_op_expr", "unary_op_expr",
  "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULLPTR
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
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399
};
# endif

#define YYPACT_NINF -763

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-763)))

#define YYTABLE_NINF -430

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -763,    11,  2567,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  3682,   -54,   149,  -763,   -54,  7344,    81,   149,  7344,
    3682,   -60,   149,   350,   632,  6094,  7344,  6205,  7344,    31,
    -763,   149,  -763,    19,  3682,  7344,  7344,  -763,  7344,  7344,
     202,    96,   536,   638,  -763,  6429,  6540,  7344,  6678,  7344,
     222,   165,  3682,  7344,  7455,  7455,   149,  -763,  6429,  7344,
    7344,  -763,  -763,  7344,  -763,  -763,  8565,  7344,  7344,  -763,
    7344,  -763,  -763,  2854,  5759,  6429,  -763,  3544,  -763,  -763,
     270,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,  -763,   149,  -763,   342,
      75,  -763,  -763,  -763,   305,   198,  -763,  -763,  -763,   216,
     268,   276,   283,   288, 12921,  6523,   -47,   298,   322,  -763,
    -763,  -763,  -763,  -763,  -763,   363,  -763, 12921,   307,  3682,
    -763,   324,  -763,   319,  7344,  7344,  7344,  7344,  7344,  6429,
    6429,    85,  -763,  -763,  -763,  -763, 10172,   173,  -763,  -763,
     149,   325, 10431,   362,  6429,   332,  -763,  -763,  -763,  -763,
     149,   275,   149,   336,    14,  9143,  9099,  -763,  -763,  -763,
   10110,  9289,  6429,  3682,   339,    15,    73,    -3,  -763,  -763,
     327,   374,  9351,   327,  -763,  6429,   301,  -763,  -763,   149,
    -763,   -34, 12921,  -763, 10493,  9971,  3682,  -763,  -763,  9351,
   12921,   352,  6429,  -763, 12921, 10555,  -763,  -763, 10617,  8628,
    -763,  -763, 10679,   403,   366,   101, 10245,  9351, 10741,   380,
    2243,   327,   380,   327,  -763,  -763,  2992,   -33,  -763,  7344,
    -763,   -21,   -20, 12921,    65, 10803,    88,   491,  -763,   149,
     390,  -763,  -763,  -763,    50,    19,    19,    19,  -763,  7344,
    7344,  7344,  7344,  6789,  6789,  7344,  7344,  7344,  7344,  7344,
    7344,    82,  8565,  7344,  7344,  7344,  7344,  7344,  7344,  7344,
    7344,  7344,  7344,  7344,  7344,  7344,  7344,  7344,  7344,  7344,
    7344,  7344,  7344,  7344,  7344,  7344,  7344,  7344,  7344,  7344,
    7344,  7344,  7344,  7344,  6429,  6429,  6789,  6789,  5648,  -763,
    -763,  3130,  -763, 10307, 10369, 10865,    37,  6789,    14,   371,
    -763,  -763,  7344,   253,  -763,   367,   392,  -763, 12921,   149,
     376,   149,   467,  6429,  -763,  3820,  6789,  -763,  3958,  6789,
     369,  -763,    14,  7566,  7344,  -763,  3682,   486,   378,  -763,
      67,  -763,  -763,    15,  -763,   408,   381,  -763,  2256,   430,
     431,  7344,    19,  -763,   382,  -763,  -763,  6429,  -763,  -763,
    -763,  -763,  -763,  6429,   384,  -763,   511,  -763,   511,  -763,
    2419,   421,  -763,  -763,  7566,  7344,  -763,  -763,  -763,  -763,
    6316,  -763,  8981,  5872,  -763,  5983,  -763,  6789,  5313,  2716,
     391,  7344,  5535,  -763,  -763,   402,  6429,   405,   316,    19,
     102,   221,   257,   261, 10033,  2080,  2080,   340,  -763,   340,
    -763,   340, 13031,   708,  2554,   540,   374,   380,  -763,  -763,
    -763,  2243, 12520,   340,  1464,  1464,  2080,  1464,  1464,  2406,
     380, 12520,  8829,  2406,   327,   327,   380, 10927, 10989, 11051,
   11113, 11175, 11237, 11299, 11361, 11423, 11485, 11547, 11609, 11671,
   11733, 11795, 11857,   404,   409,  -763,   340,  -763,   340,   151,
    -763,  -763,  -763,  -763,  -763,  -763,   149,    94,  -763, 12983,
     368,  7677,  6789,  7788,  6789,  7344,  6789,  1846,   -54, 11919,
    -763,  -763, 11986,  7344,  6429,  -763,  6429,   464,   184,  -763,
    -763,   390,  7344,    98,  7344, 12921,    35,  9413,  7344, 12921,
      54,  9222,  5648,  -763,   428,   448,   432, 12048,   448,   434,
     541, 12110,  3682,  -763,   185,  -763,    43,  -763,   370,   433,
      15,    86,  -763,  6429,  -763,   263,  7344,  6900,  -763, 12921,
    -763,  -763,  -763, 12921,    28,   436,  -763,    51,  -763,  6429,
    -763,   293,   149,   440,   441,  -763,  -763,  -763,  -763,  -763,
    -763,     0,   826,  -763,  -763, 12921,  3682, 12921,  -763, 12172,
    3268,   461,  -763,   516,  -763,  -763,  -763,  -763,  8696,   311,
    -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  -763,  -763,  -763,  -763,  5648,  -763,  6789,  6789,  7344,
     552, 12234,  7344,   556, 12296,   442,  1959,    14,    14,  -763,
   12921,  -763, 12921,  -763,  7344,   149,  -763,   453,  9351,  -763,
    9475,  4096,  -763,  4234,  -763,  9537,  4372,  -763,    14,  4510,
    -763,    14,   190,  -763,  7344,  -763,  7344,  -763,  3682,  7344,
    -763,   938,  -763,    15,   483,   530,  -763,  -763,  -763,    49,
    -763,  -763,  2419,   431,    60, 12921,  -763, 12921,  4648,  6429,
    -763,  -763,  -763,   149,   485,   319,    62,  -763,  -763,  -763,
    4786,   455,  4924,   458,  -763,  7344,  -763,  3406,  1299,  -763,
     370,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,   149,  -763,  -763,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,  -763,  7344,   488,   492,
     470,   470,  -763,  -763,  -763,   195,   196, 12358,  7899,  8010,
   12420,  8121,  8232,  8343,  8454,  -763,  -763, 12921,  -763,  -763,
    -763,  3682,  7344, 12921,  7344, 12921,  3682,  7344, 12921,  -763,
    7344, 12921,  -763,  5648,  -763, 12487, 12921,  -763, 12921,  -763,
     472,  7011,   108,  -763,   525,  -763,  6789,  5037,  3682,  -763,
      56,   474,  7344,  -763,  5175,  7344,  -763, 12921,  3682,  7344,
    -763, 12921,  3682, 12921,  -763,   544,   514,   514,  -763,  1477,
    -763,    71,  -763,  9025,  8744,    67,  -763,  -763,  7344,  7344,
    7344,  7344,  7344,  7344,  7344,  7344,  1383, 12110,  9599,  9661,
   12110,  9723,  9785,  -763,  7344,   483,   -60,  7344,  7344,  5424,
    -763,  -763,   292,  6429,  -763,  -763,  7344,    77,  8863,  -763,
     449,   362,  -763,   319, 12921,  9847,  -763,  9909,  -763,  -763,
     539,  -763,  -763,   470,   470,   200, 12549, 12611, 12673, 12735,
   12797, 12859,  -763,  3682,  3682,  3682,  3682, 12921,   108,  7122,
      76,  -763,  -763, 12921, 12921,  -763,  -763,  -763,  8454,  -763,
    3682,  3682,  -763,   525,  -763,  -763,  -763,  7344,  7344,  7344,
    7344,  7344,  7344, 12110, 12110, 12110, 12110,  -763,  -763,  -763,
    -763,  -763,   318,  6789,  8643,   675, 12110, 12110,    27,  8907,
    -763,  -763,  -763,  -763,  -763,  7233,  -763
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    36,    76,   422,   423,   424,   425,
     426,     0,   377,    74,   147,   377,     0,   278,    74,     0,
       0,     0,     0,    74,    74,     0,     0,   277,     0,     0,
     165,     0,   161,     0,     0,     0,     0,   368,     0,     0,
       0,     0,   277,   277,   148,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   149,     0,     0,
       0,   474,   476,     0,   477,   478,   405,     0,     0,   475,
     472,    83,   473,     0,     0,     0,     4,     0,     5,     9,
       0,    10,    11,    12,    14,   337,    21,    13,    84,    90,
      15,    17,    16,    19,    20,    18,    89,     0,    87,   397,
       0,    91,    88,    92,     0,   406,   393,   394,   340,   338,
       0,     0,   398,   399,     0,   339,     0,   407,   408,   409,
     392,   342,   341,   395,   396,     0,    23,   348,     0,     0,
     378,     0,    75,     0,     0,     0,     0,     0,     0,     0,
       0,   397,   406,   338,   398,   399,   377,   339,   407,   408,
       0,     0,     0,     0,   328,     0,    78,    77,   166,    97,
       0,   167,     0,     0,     0,     0,     0,   278,   279,    96,
       0,     0,   328,     0,     0,     0,     0,     0,   280,    30,
     460,   390,     0,   461,     7,   328,   279,    86,    85,   257,
     320,     0,   319,    81,     0,     0,     0,    80,    33,     0,
     343,     0,   328,    34,   349,     0,   131,   127,     0,   339,
     131,   128,     0,   269,     0,     0,   319,     0,     0,   463,
     404,   459,   462,   458,    42,    44,     0,     0,   323,     0,
     325,     0,     0,   324,     0,   319,     0,     0,     6,     0,
     150,   243,   242,   168,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   328,   328,     0,     0,     0,    24,
      25,     0,    26,     0,     0,     0,     0,     0,     0,     0,
      27,    28,     0,   337,   335,     0,   329,   330,   336,     0,
       0,     0,     0,     0,   106,     0,     0,   105,     0,     0,
       0,   112,     0,     0,    50,    93,     0,   122,     0,    29,
     218,   162,   285,     0,   286,   288,     0,   299,     0,     0,
     291,     0,     0,    31,     0,   167,   256,     0,    57,    82,
     140,    79,    32,   328,     0,   138,   129,   125,   130,   126,
       0,   267,   264,    54,     0,    50,    99,    35,    43,    45,
       0,   427,     0,     0,   418,     0,   420,     0,     0,     0,
       0,     0,     0,   431,     8,     0,     0,     0,   236,     0,
       0,     0,     0,     0,   376,   455,   454,   457,   465,   464,
     469,   468,   451,   448,   449,   450,   401,   438,   417,   416,
     415,   402,   442,   453,   447,   445,   456,   446,   444,   436,
     441,   443,   452,   435,   439,   440,   437,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   467,   466,   471,   470,   230,
     227,   228,   229,   233,   234,   235,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,   429,   377,   377,
     102,   265,     0,     0,     0,   345,     0,   159,     0,   156,
     266,   150,     0,     0,     0,   352,     0,     0,     0,   358,
       0,     0,     0,   113,   473,    53,     0,    46,    51,     0,
     121,     0,     0,   344,     0,   219,     0,   226,   244,     0,
     289,     0,   303,     0,   298,   393,     0,     0,   282,   391,
     281,   414,   322,   321,     0,     0,   346,   135,   132,     0,
     271,   393,     0,     0,     0,   428,   400,   419,   326,   421,
     327,     0,     0,   430,   118,   362,     0,   433,   432,     0,
       0,   151,   152,   240,   237,   238,   241,   169,     0,     0,
     273,   272,   274,   276,    58,    65,    66,    67,    62,    64,
      72,    73,    60,    63,    61,    59,    69,    68,    70,    71,
     412,   413,   231,   232,   385,     0,   379,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
     334,   332,   333,   331,     0,   157,   155,     0,     0,   120,
       0,     0,   104,     0,   103,     0,     0,   110,     0,     0,
     108,     0,     0,   383,     0,    94,     0,    95,     0,     0,
     124,   225,   217,     0,   312,   245,   248,   247,   249,     0,
     287,   290,     0,   291,     0,   283,   292,   293,     0,     0,
     139,   141,   347,   135,   136,     0,     0,   268,    55,    56,
       0,     0,     0,     0,   119,     0,    37,     0,   277,   239,
     244,   200,   198,   203,   210,   211,   212,   207,   209,   205,
     208,   206,   204,   214,   213,   179,   182,   180,   181,   192,
     183,   196,   188,   186,   199,   187,   185,   190,   195,   197,
     184,   189,   193,   194,   191,   201,   202,     0,   177,     0,
     215,   215,   175,   275,   381,   406,   406,     0,     0,     0,
       0,     0,     0,     0,     0,   101,   100,   160,   158,   152,
     107,     0,     0,   351,     0,   350,     0,     0,   357,   111,
       0,   356,   109,     0,   382,    48,    47,   123,   367,   220,
       0,     0,   291,   246,   262,   284,     0,     0,     0,   143,
       0,     0,     0,   133,     0,     0,   116,   364,     0,     0,
     114,   363,     0,   434,    38,    74,   277,   277,   145,   277,
     153,     0,   178,     0,     0,   218,   171,   172,     0,     0,
       0,     0,     0,     0,     0,     0,   277,   355,     0,     0,
     361,     0,     0,   384,     0,   312,   315,   316,   317,     0,
     314,   318,   393,   258,   222,   221,     0,     0,     0,   301,
     393,   144,   142,     0,   137,     0,   117,     0,   115,   154,
     250,   176,   177,   215,   215,     0,     0,     0,     0,     0,
       0,     0,   146,     0,     0,     0,     0,    49,   291,   304,
       0,   259,   261,   260,   263,   254,   255,   163,     0,   134,
       0,     0,   251,   262,   173,   174,   216,     0,     0,     0,
       0,     0,     0,   354,   353,   360,   359,   224,   223,   306,
     307,   309,   393,     0,   429,   393,   366,   365,     0,     0,
     308,   310,   252,   170,   253,   304,   311
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -763,  -763,  -763,     2,  -652,   987,  -763,  -763,  1606,    57,
    -265,   241,   245,  -763,  -763,   341,   206,  1933,   -10,   -82,
    -725,  -658,   -41,  -763,  -763,  -763,    36,  -763,  -763,   411,
    -763,   -32,  -763,  -763,  -763,  -763,  -763,   468,   139,  -107,
    -763,  -763,    22,   528,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  -763,  -156,  -154,  -650,  -763,  -153,     3,  -763,  -311,
    -763,  -763,   -37,  -763,  -763,  -252,    25,  -763,  -211,  -219,
    -763,  -141,  -763,   628,  -763,  -179,   308,  -763,  -336,  -623,
    -763,  -500,  -367,  -673,  -762,  -149,   -35,   -27,  -763,  -127,
    -763,   181,   364,  -232,  -763,  -763,   852,  -763,    -9,  -763,
    -763,  -203,  -763,  -451,  -763,   986,  1066,   -11,   424,  -763,
    1124,  1385,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -286
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   225,    77,   510,    79,    80,    81,   226,
     505,   509,   506,    82,    83,    84,   157,    85,   161,   198,
      86,    87,    88,    89,    90,    91,   605,    92,    93,   366,
     538,   665,    94,    95,   534,   661,    96,    97,   397,   678,
      98,   488,   489,   141,   174,   518,   100,   101,   398,   680,
     567,   719,   720,   721,   796,   341,   514,   515,   516,   466,
     568,   243,   649,   873,   903,   867,   190,   862,   824,   827,
     102,   214,   371,   103,   104,   177,   178,   345,   346,   528,
     349,   350,   524,   890,   821,   762,   227,   231,   232,   315,
     316,   317,   142,   106,   107,   108,   143,   110,   129,   130,
     467,   332,   632,   468,   111,   144,   145,   114,   147,   116,
     148,   149,   119,   120,   236,   121,   122,   123,   124,   125
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     127,   187,   188,   540,    76,   146,   133,   519,   152,   309,
     191,     3,   470,   158,   165,   166,   170,   171,     5,   320,
     790,   653,     5,   215,   180,   181,   789,   182,   183,   517,
     765,   390,   128,   351,   192,   194,   195,   199,   200,   196,
     234,   204,   205,   208,   212,   338,     5,   216,   217,   218,
     347,   633,   219,     5,     5,   220,   221,   222,   354,   223,
     621,   164,   192,   233,   235,   400,   401,   402,   403,   508,
    -225,   797,   347,   476,   347,   364,   357,   380,   154,   626,
     658,   196,   324,   327,   331,     5,   330,   347,   459,   383,
     385,   294,    46,   295,   342,   358,   766,   891,   766,   230,
     353,   387,   866,   460,   381,   306,   461,   352,   418,  -296,
     508,   659,   893,   399,  -296,   241,   525,   362,   384,   386,
     462,   463,  -296,   127,   303,   304,   305,   204,   192,   235,
     464,   839,   901,  -296,   242,   376,  -296,   308,   541,   825,
     330,   670,    46,   318,   724,   465,  -296,   388,   790,   764,
     301,   330,     5,   343,   789,  -296,   902,   175,   652,   150,
     419,   318,  -294,   301,  -296,   660,   357,   453,   454,   172,
     388,   301,   388,   866,   318,   388,   355,  -296,   477,   314,
     652,   643,   348,  -296,   651,  -296,   388,   592,   175,   663,
     301,   318,   301,   874,   875,   652,  -296,   314,   392,   527,
    -296,   767,  -296,   774,   595,  -225,   389,  -296,   357,   184,
     314,   357,   352,   301,   593,  -296,   470,   894,   382,   131,
     569,   900,   823,  -411,   151,  -411,   480,   314,   379,   393,
     373,   570,   906,   596,   185,   888,   535,   619,   404,   405,
     406,   407,   409,   411,   412,   413,   414,   415,   416,   417,
     503,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   318,   318,   456,   458,   469,   493,   408,
     410,   840,    14,   201,   615,   641,   479,   628,   631,  -297,
     753,   482,   813,   202,     5,  -386,  -387,   760,   156,   470,
     641,  -410,   192,  -410,   495,   497,   239,   499,   501,   314,
     314,   616,   507,   507,   642,   511,   244,  -370,  -297,   754,
     517,   352,   455,   457,  -386,  -387,    44,   563,  -297,   876,
     529,   -74,   483,   478,   484,  -369,   533,  -297,   671,   673,
     571,   252,   318,     5,  -297,    57,   548,   156,   550,   245,
     -74,   561,   496,   507,   507,   500,   105,   352,   246,   533,
     564,   352,   233,  -297,   233,   105,   552,   533,   555,   565,
     557,   559,   532,  -313,   105,   192,   572,   247,   314,   253,
     573,   645,  -297,   254,   566,   207,   211,   248,   105,  -297,
     829,   667,  -313,  -270,   252,   532,  -313,   829,   230,  -305,
     230,   249,  -374,   532,   622,   624,   105,  -375,   627,   630,
     525,   352,  -270,   551,   646,   296,   115,  -372,  -305,   297,
     598,  -313,  -305,   647,   297,   115,   259,   105,   260,   261,
     723,   105,   253,   264,   115,   298,   254,   312,   648,   259,
     271,  -371,   261,   300,   310,   301,   264,  -305,   115,   277,
     495,   601,   499,   604,   511,   606,   555,   470,   319,   607,
     608,  -373,   610,   612,   323,   318,   115,   340,   209,   209,
    -411,   618,  -411,   620,   517,  -300,   261,   625,   654,   259,
     363,   469,   261,   105,   370,   372,   264,   115,   394,   396,
     481,   115,   486,   492,   666,   490,   485,   502,   600,   611,
     603,   314,   235,   512,  -300,   655,   657,   513,   520,   526,
     521,   531,   527,   536,  -300,   735,   736,   537,   235,   822,
      99,   542,   556,  -300,   167,   830,   740,   105,   560,    99,
    -300,   562,   830,   590,    23,   186,   749,     5,    99,   752,
     591,   156,    30,   115,  -164,   614,   167,   -52,   634,  -300,
     105,   635,    99,   637,   252,  -279,    23,   186,   638,   668,
     669,   357,   650,  -164,    30,   662,  -164,   728,  -300,   679,
      99,   731,   -41,   733,   469,  -300,   409,   456,   727,   739,
     105,   730,   761,   763,   772,  -164,   778,   115,  -279,   782,
    -337,    99,   253,   737,   794,    99,   254,  -279,   795,   826,
     743,   815,   745,   833,   872,   748,   544,   677,   751,   543,
     115,   368,   160,   755,   770,   756,  -279,   892,   758,   321,
     617,   771,   806,   408,   455,     5,   895,   738,   843,   156,
     844,   159,   845,   791,   759,   256,   904,   887,   192,   259,
     115,   260,   261,  -279,   898,   169,   264,    99,   167,   777,
     530,   781,   892,   271,   783,   105,   858,   613,    23,   186,
     275,   276,   277,   892,     0,     0,    30,     0,  -164,   379,
       0,     0,     0,     0,   -40,     0,  -279,     0,   832,   105,
       0,     0,   105,     0,     0,  -279,     0,  -164,     0,     0,
     105,    99,     0,     0,     0,     0,   793,     0,     0,     0,
     160,  -302,     0,     0,  -279,     0,     0,   743,   745,     0,
     748,   751,   777,   781,    99,   115,     0,     0,     0,     0,
     807,   808,   252,   809,     0,   810,   811,     0,     0,   812,
    -302,     0,   469,     0,     0,   187,   188,     0,     0,   115,
    -302,     0,   115,   105,    99,   828,   555,     0,     0,  -302,
     115,   834,     0,   555,   835,     0,  -302,     0,   837,     0,
     253,     0,     0,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   860,  -302,   820,   807,   846,   847,
     810,   848,   849,   850,   851,     0,     0,     0,     0,     0,
       0,     0,     0,   857,  -302,     0,   200,   204,   235,     0,
       0,  -302,   863,   115,     0,   864,     0,   259,     0,   260,
     261,     0,     0,     0,   264,     0,     0,     0,     0,    99,
     594,   271,     0,     0,     0,     0,   250,     0,   275,   276,
     277,   251,   883,   884,   885,   886,     0,     0,   861,     0,
     252,     0,     0,    99,   109,     0,    99,   781,     0,   896,
     897,     0,     0,   109,    99,     0,   883,   884,   885,   886,
     896,   897,   109,     0,     0,     0,   105,     0,     0,     0,
       0,     0,   899,   555,   889,     0,   109,     0,   253,     0,
       0,     0,   254,     0,   781,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   109,     0,     0,     0,     0,     0,
       0,     0,   330,     0,     0,     0,   255,    99,     0,   889,
     105,     0,     0,     0,   105,   109,     0,     0,     0,   109,
     889,   256,   722,   257,   258,   259,   115,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
       0,   272,   273,   274,     0,     0,   275,   276,   277,   459,
       0,   725,   726,     0,     0,     0,     0,   672,     0,     0,
       0,     0,     0,     0,   460,     0,     0,   461,     0,     0,
     115,   109,     0,     0,   115,   105,     0,   105,   112,    78,
     105,   462,   463,   105,     0,     0,     0,   112,   126,     0,
       0,   464,   105,     0,     0,     0,   112,   153,     0,     0,
       0,     0,     0,     0,     0,     0,   465,     0,     0,     0,
     112,   179,   105,     0,     0,   109,     0,     0,     0,     0,
       0,     0,     0,     0,   105,     0,   105,     0,   112,   203,
      99,   105,     0,     0,     0,   115,     0,   115,   109,     0,
     115,     0,     0,   115,     0,     0,     0,     0,     0,   112,
      78,     0,   115,   112,   238,     0,     0,     0,   113,     0,
       0,     0,     0,     0,     0,     0,     0,   113,   109,     0,
       0,     0,   115,     0,    99,     0,   113,     0,    99,     0,
       0,     0,     0,     0,   115,     0,   115,     0,     0,     0,
     113,   115,     0,     0,     0,   105,     0,     0,     0,     0,
     105,     0,     0,     0,     0,   112,   299,     0,   113,     0,
       0,     0,     0,     0,     0,     0,   117,     0,     0,     0,
       0,     0,   105,     0,     0,   117,     0,     0,     0,   113,
       0,     0,   105,   113,   117,     0,   105,     0,     0,    99,
       0,    99,     0,   109,    99,     0,     0,    99,   117,   112,
     339,     0,     0,     0,     0,   115,    99,     0,     0,     0,
     115,     0,     0,     0,     0,     0,   117,   109,     0,     0,
     109,     0,   112,   361,     0,     0,    99,     0,   109,     0,
       0,     0,   115,     0,     0,   113,     0,   117,    99,     0,
      99,   117,   115,     0,     0,    99,   115,     0,     0,     0,
       0,     0,   112,    78,     0,     0,     0,   105,   105,   105,
     105,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   105,   105,     0,     0,     0,   113,
       0,   109,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   117,     0,     0,     0,     0,     0,     0,
       0,     0,   113,     0,     0,     0,     0,     0,     0,    99,
       0,     0,     0,     0,    99,     0,     0,   115,   115,   115,
     115,     0,     0,     0,     0,     0,     0,   112,    78,     0,
       0,     0,   113,     0,   115,   115,    99,   117,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,     0,
      99,   112,   361,     0,   112,   361,    14,     0,     0,    17,
     117,     0,   112,     0,     0,     0,     0,    22,     0,    23,
     785,     0,     0,    27,     0,     0,     0,    30,     0,  -164,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     117,     0,     0,     0,     0,    40,     0,   786,  -164,   787,
      44,     0,     0,     0,   109,     0,     0,   113,     0,     0,
       0,     0,     0,     0,     0,   112,   554,    56,     0,    57,
       0,    99,    99,    99,    99,     0,     0,   118,     0,     0,
       0,   113,     0,     0,   113,     0,   118,     0,    99,    99,
      14,     0,   113,    17,     0,   118,     0,     0,   109,     0,
       0,    22,   109,    23,   785,     0,     0,    27,     0,   118,
       0,    30,     0,  -164,     0,   117,     0,     0,    71,     0,
       0,     0,     0,     0,     0,     0,   788,   118,     0,    40,
       0,   786,  -164,   787,    44,     0,     0,     0,     0,   117,
       0,     0,   117,     0,     0,   113,     0,     0,   118,     0,
     117,    56,   118,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   109,     0,   109,     0,     0,   109,     0,
       0,   109,     0,     0,     0,     0,     0,     0,   252,     0,
     109,     0,     0,     0,    14,     0,     0,    17,   112,   640,
       0,     0,     0,     0,     0,    22,     0,    23,   785,     0,
     109,    27,    71,   117,   118,    30,     0,  -164,     0,     0,
     852,     0,   109,     0,   109,     0,   253,     0,     0,   109,
     254,     0,     0,   237,     0,   786,  -164,   787,    44,     0,
       0,     0,   112,   674,     0,     0,   112,    78,     0,     0,
       0,     0,     0,     0,     0,    56,     0,    57,   118,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,   113,     0,
     264,   118,     0,     0,     0,     0,   270,   271,     0,     0,
       0,   274,     0,   109,   275,   276,   277,     0,   109,     0,
       0,     0,     0,     0,     0,     0,    71,   112,   361,   112,
     361,   118,   112,   361,     0,   112,   361,     0,     0,     0,
     109,     0,   113,     0,   112,   757,   113,     0,     0,     0,
     109,     0,     0,     0,   109,     0,   117,     0,     0,     0,
       0,     0,     0,     0,   112,   769,     0,     0,     0,     0,
       0,     0,     0,     0,   197,     0,   112,   776,   112,   780,
     206,   210,     0,   112,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     117,     0,     0,     0,   117,     0,   118,   113,     0,   113,
       0,     0,   113,     0,     0,   113,     0,     0,     0,     0,
       0,     0,     0,     0,   113,   109,   109,   109,   109,     0,
     118,     0,     0,   118,     0,     0,     0,     0,     0,     0,
       0,   118,   109,   109,   113,     0,     0,   112,     0,     0,
       0,     0,   112,     0,     0,     0,   113,     0,   113,   302,
       0,     0,     0,   113,     0,   117,     0,   117,     0,     0,
     117,     0,     0,   117,   112,   831,     0,     0,     0,     0,
       0,     0,   117,     0,   112,   836,     0,     0,   112,   838,
     197,   197,   197,     0,   118,     0,     0,   337,     0,     0,
       0,     0,   117,     0,     0,     0,     0,     0,   197,     0,
       0,     0,     0,     0,   117,     0,   117,     0,     0,     0,
       0,   117,     0,     0,     0,   197,     0,   113,     0,     0,
       0,     0,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   197,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,     0,     0,     0,     0,   112,
     112,   112,   112,     0,   113,     0,     0,     0,   113,     5,
       6,     7,     8,     9,    10,     0,   112,   112,   134,     0,
       0,     0,     0,     0,     0,   117,     0,     0,     0,     0,
     117,     0,    21,     0,     0,     0,     0,     0,   135,   136,
       0,   137,     0,    29,     0,     0,     0,     0,    32,    33,
       0,    35,   117,    36,    37,     0,     0,   118,     0,     0,
       0,    39,   117,    41,     0,     0,   117,     0,     0,     0,
       0,     0,     0,     0,   197,    49,    50,    51,   138,   113,
     113,   113,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,   113,   197,     0,
       0,   118,     0,     0,     0,   118,   132,     0,     0,     0,
       0,   132,    63,     0,     0,   155,   132,   132,     0,     0,
       0,     0,     0,     0,   173,     0,   176,     0,     0,   250,
      68,     0,     0,     0,   251,     0,     0,   117,   117,   117,
     117,     0,     0,   252,    74,     0,   140,     0,     0,   213,
       0,     0,     0,     0,   117,   117,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   118,     0,   118,     0,
       0,   118,     0,     0,   118,     0,     0,     0,     0,     0,
       0,   253,     0,   118,     0,   254,     0,     0,     0,     0,
     240,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   118,     0,     0,     0,     0,     0,   255,
       0,     0,     0,     0,     0,   118,     0,   118,     0,     0,
       0,     0,   118,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   213,   272,   273,   274,   313,     0,   275,
     276,   277,     0,   213,     0,   322,     0,     0,     0,     0,
     734,     0,   197,   197,   252,   313,   197,   197,   344,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   313,   132,
       0,     0,   356,     0,     0,     0,   118,     0,     0,     0,
       0,   118,     0,     0,     0,   313,     0,     0,     0,     0,
       0,     0,   253,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,   118,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   118,     0,     0,     0,   118,     0,     0,
     255,     0,   395,     0,     0,     0,     0,   176,   176,   176,
     176,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,   420,   263,   264,   265,   266,     0,
     268,   269,   270,   271,     0,   272,   273,   274,     0,     0,
     275,   276,   277,   197,   197,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   197,     0,   337,   313,   313,     0,
       0,   337,     0,     0,   197,     0,     0,   197,   118,   118,
     118,   118,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   487,     0,   491,   118,   118,   522,     0,     5,
       6,     7,     8,     9,    10,     0,     0,   252,   134,     0,
       0,   773,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,    21,     0,     0,   176,     0,     0,   135,   136,
       0,   137,     0,    29,     0,     0,   313,     0,    32,    33,
       0,    35,     0,    36,    37,   253,     0,     0,     0,   254,
       0,    39,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,   138,     0,
       0,     0,   176,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,   256,     0,
     257,   258,   259,     0,   260,   261,     0,     0,     0,   264,
       0,    62,    63,    64,    65,   270,   271,     0,     0,    66,
     274,     0,     0,   275,   276,   277,   197,     0,    67,     0,
      68,     0,    69,    70,     0,     0,     0,     0,    72,     0,
       0,     0,   139,     0,    74,     0,   523,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   337,   337,     0,   337,   337,   313,
     522,     0,     5,     6,     7,     8,     9,    10,     0,     0,
     252,   134,     0,   865,     0,     0,     0,     0,     0,   869,
       0,   337,     0,   337,     0,    21,     0,     0,     0,   644,
       0,   135,   136,   344,   137,     0,    29,     0,     0,     0,
       0,    32,    33,     0,    35,     0,    36,    37,   253,     0,
     664,     0,   254,     0,    39,   213,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,   138,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   718,     0,     0,   865,     0,     0,     0,     0,    61,
       0,   256,     0,   257,   258,   259,     0,   260,   261,     0,
       0,     0,   264,     0,    62,    63,    64,    65,     0,   271,
       0,     0,    66,     0,     0,     0,   275,   276,   277,     0,
       0,    67,     0,    68,     0,    69,    70,     0,   487,     0,
       0,    72,     0,     0,     0,   139,     0,    74,     0,   539,
       0,     0,     0,     0,     0,     0,     0,    -2,     4,     0,
       5,     6,     7,     8,     9,    10,   344,     0,   252,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -277,    18,
      19,     0,    20,    21,     0,    22,   664,    23,    24,    25,
      26,    27,    28,     0,    29,    30,     0,  -164,    31,    32,
      33,    34,    35,   -39,    36,    37,   253,    38,     0,     0,
     254,  -277,    39,    40,    41,    42,  -164,    43,    44,   792,
    -277,    45,    46,     0,    47,    48,    49,    50,    51,    52,
       0,    53,     0,    54,    55,    56,     0,    57,    58,  -277,
       0,     0,    59,     0,    60,     0,     0,    61,     0,   256,
       0,     0,   258,   259,     0,   260,   261,     0,     0,     0,
     264,     0,    62,    63,    64,    65,     0,   271,     0,     0,
      66,     0,     0,     0,   275,   276,   277,     0,     0,    67,
       0,    68,     0,    69,    70,     0,    71,     0,     0,    72,
       0,     0,     0,    73,     0,    74,     0,    75,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,   132,     5,
       6,     7,     8,     9,    10,     0,  -429,   842,    11,    12,
      13,  -429,     0,    14,    15,    16,    17,  -277,    18,    19,
    -429,    20,    21,  -429,    22,     0,    23,    24,    25,    26,
      27,    28,     0,    29,    30,     0,  -164,    31,    32,    33,
      34,    35,   -39,    36,    37,     0,    38,     0,     0,     0,
    -277,    39,     0,    41,    42,  -164,    43,    44,  -429,  -277,
      45,    46,  -429,    47,    48,    49,    50,    51,    52,     0,
      53,     0,    54,    55,    56,     0,    57,    58,  -277,     0,
       0,    59,     0,    60,     0,     0,  -429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -429,    63,  -429,  -429,  -429,  -429,  -429,  -429,  -429,
       0,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,
      68,  -429,  -429,  -429,     0,    71,  -429,  -429,  -429,     0,
       0,     0,    73,  -429,    74,     4,    75,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
       0,    14,    15,    16,    17,  -277,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,    28,
       0,    29,    30,     0,  -164,    31,    32,    33,    34,    35,
     -39,    36,    37,     0,    38,     0,     0,     0,  -277,    39,
      40,    41,    42,  -164,    43,    44,     0,  -277,    45,    46,
       0,    47,    48,    49,    50,    51,    52,     0,    53,     0,
      54,    55,    56,     0,    57,    58,  -277,     0,     0,    59,
       0,    60,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,    67,     0,    68,     0,
      69,    70,   189,    71,     0,     0,    72,     0,     0,     0,
      73,   224,    74,     4,    75,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,     0,    14,
      15,    16,    17,  -277,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,    27,    28,     0,    29,
      30,     0,  -164,    31,    32,    33,    34,    35,   -39,    36,
      37,     0,    38,     0,     0,     0,  -277,    39,    40,    41,
      42,  -164,    43,    44,     0,  -277,    45,    46,     0,    47,
      48,    49,    50,    51,    52,     0,    53,     0,    54,    55,
      56,     0,    57,    58,  -277,     0,     0,    59,     0,    60,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,    68,     0,    69,    70,
       0,    71,     0,     0,    72,     0,     0,     0,    73,   378,
      74,     4,    75,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -277,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,    28,     0,    29,    30,     0,
    -164,    31,    32,    33,    34,    35,   -39,    36,    37,     0,
      38,     0,     0,     0,  -277,    39,    40,    41,    42,  -164,
      43,    44,     0,  -277,    45,    46,     0,    47,    48,    49,
      50,    51,    52,     0,    53,     0,    54,    55,    56,     0,
      57,    58,  -277,     0,     0,    59,     0,    60,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,    64,    65,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,    67,     0,    68,     0,    69,    70,     0,    71,
       0,     0,    72,     0,     0,     0,    73,   224,    74,     4,
      75,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -277,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,    27,    28,     0,    29,    30,     0,  -164,    31,
      32,    33,    34,    35,   -39,    36,    37,     0,    38,     0,
       0,     0,  -277,    39,    40,    41,    42,  -164,    43,    44,
       0,  -277,    45,    46,     0,    47,    48,    49,    50,    51,
      52,     0,    53,     0,    54,    55,    56,     0,    57,    58,
    -277,     0,     0,    59,     0,    60,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,    63,    64,    65,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,    68,     0,    69,    70,     0,    71,     0,     0,
      72,     0,     0,     0,    73,   676,    74,     4,    75,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,     0,    14,    15,    16,    17,  -277,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,    28,     0,    29,    30,     0,  -164,    31,    32,    33,
      34,    35,   -39,    36,    37,     0,    38,     0,     0,     0,
    -277,    39,    40,    41,    42,  -164,    43,    44,     0,  -277,
      45,    46,     0,    47,    48,    49,    50,    51,    52,     0,
      53,     0,    54,    55,    56,     0,    57,    58,  -277,     0,
       0,    59,     0,    60,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
      68,     0,    69,    70,     0,    71,     0,     0,    72,     0,
       0,     0,    73,   784,    74,     4,    75,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
       0,    14,    15,    16,    17,  -277,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,    28,
       0,    29,    30,     0,  -164,    31,    32,    33,    34,    35,
     -39,    36,    37,     0,    38,     0,     0,     0,  -277,    39,
     237,    41,    42,  -164,    43,    44,     0,  -277,    45,    46,
       0,    47,    48,    49,    50,    51,    52,     0,    53,     0,
      54,    55,    56,     0,    57,    58,  -277,     0,     0,    59,
       0,    60,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,    67,     0,    68,     0,
      69,    70,     0,    71,     0,     0,    72,     0,     0,     0,
      73,     0,    74,     4,    75,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,     0,    14,
      15,    16,    17,  -277,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,    27,    28,     0,    29,
      30,     0,  -164,    31,    32,    33,    34,    35,   -39,    36,
      37,     0,    38,     0,     0,     0,  -277,    39,     0,    41,
      42,  -164,    43,    44,     0,  -277,    45,    46,     0,    47,
      48,    49,    50,    51,    52,     0,    53,     0,    54,    55,
      56,     0,    57,    58,  -277,     0,     0,    59,     0,    60,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,    68,     0,    69,    70,
       0,    71,     0,     0,    72,     0,     0,     0,    73,     0,
      74,     4,    75,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -277,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,   494,     0,    29,    30,     0,
    -164,    31,    32,    33,    34,    35,   -39,    36,    37,     0,
      38,     0,     0,     0,  -277,    39,     0,    41,    42,  -164,
      43,    44,     0,  -277,    45,    46,     0,    47,    48,    49,
      50,    51,    52,     0,    53,     0,    54,    55,    56,     0,
      57,    58,  -277,     0,     0,    59,     0,    60,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,    64,    65,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,    67,     0,    68,     0,    69,    70,     0,    71,
       0,     0,    72,     0,     0,     0,    73,     0,    74,     4,
      75,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -277,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,    27,   498,     0,    29,    30,     0,  -164,    31,
      32,    33,    34,    35,   -39,    36,    37,     0,    38,     0,
       0,     0,  -277,    39,     0,    41,    42,  -164,    43,    44,
       0,  -277,    45,    46,     0,    47,    48,    49,    50,    51,
      52,     0,    53,     0,    54,    55,    56,     0,    57,    58,
    -277,     0,     0,    59,     0,    60,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,    63,    64,    65,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,    68,     0,    69,    70,     0,    71,     0,     0,
      72,     0,     0,     0,    73,     0,    74,     4,    75,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,     0,    14,    15,    16,    17,  -277,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,   742,     0,    29,    30,     0,  -164,    31,    32,    33,
      34,    35,   -39,    36,    37,     0,    38,     0,     0,     0,
    -277,    39,     0,    41,    42,  -164,    43,    44,     0,  -277,
      45,    46,     0,    47,    48,    49,    50,    51,    52,     0,
      53,     0,    54,    55,    56,     0,    57,    58,  -277,     0,
       0,    59,     0,    60,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
      68,     0,    69,    70,     0,    71,     0,     0,    72,     0,
       0,     0,    73,     0,    74,     4,    75,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
       0,    14,    15,    16,    17,  -277,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,   744,
       0,    29,    30,     0,  -164,    31,    32,    33,    34,    35,
     -39,    36,    37,     0,    38,     0,     0,     0,  -277,    39,
       0,    41,    42,  -164,    43,    44,     0,  -277,    45,    46,
       0,    47,    48,    49,    50,    51,    52,     0,    53,     0,
      54,    55,    56,     0,    57,    58,  -277,     0,     0,    59,
       0,    60,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,    67,     0,    68,     0,
      69,    70,     0,    71,     0,     0,    72,     0,     0,     0,
      73,     0,    74,     4,    75,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,     0,    14,
      15,    16,    17,  -277,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,    27,   747,     0,    29,
      30,     0,  -164,    31,    32,    33,    34,    35,   -39,    36,
      37,     0,    38,     0,     0,     0,  -277,    39,     0,    41,
      42,  -164,    43,    44,     0,  -277,    45,    46,     0,    47,
      48,    49,    50,    51,    52,     0,    53,     0,    54,    55,
      56,     0,    57,    58,  -277,     0,     0,    59,     0,    60,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,    68,     0,    69,    70,
       0,    71,     0,     0,    72,     0,     0,     0,    73,     0,
      74,     4,    75,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -277,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,   750,     0,    29,    30,     0,
    -164,    31,    32,    33,    34,    35,   -39,    36,    37,     0,
      38,     0,     0,     0,  -277,    39,     0,    41,    42,  -164,
      43,    44,     0,  -277,    45,    46,     0,    47,    48,    49,
      50,    51,    52,     0,    53,     0,    54,    55,    56,     0,
      57,    58,  -277,     0,     0,    59,     0,    60,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,    64,    65,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,    67,     0,    68,     0,    69,    70,     0,    71,
       0,     0,    72,     0,     0,     0,    73,     0,    74,     4,
      75,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -277,
      18,    19,     0,   768,    21,     0,    22,     0,    23,    24,
      25,    26,    27,    28,     0,    29,    30,     0,  -164,    31,
      32,    33,    34,    35,   -39,    36,    37,     0,    38,     0,
       0,     0,  -277,    39,     0,    41,    42,  -164,    43,    44,
       0,  -277,    45,    46,     0,    47,    48,    49,    50,    51,
      52,     0,    53,     0,    54,    55,    56,     0,    57,    58,
    -277,     0,     0,    59,     0,    60,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,    63,    64,    65,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,    68,     0,    69,    70,     0,    71,     0,     0,
      72,     0,     0,     0,    73,     0,    74,     4,    75,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,     0,    14,    15,    16,    17,  -277,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,   775,     0,    29,    30,     0,  -164,    31,    32,    33,
      34,    35,   -39,    36,    37,     0,    38,     0,     0,     0,
    -277,    39,     0,    41,    42,  -164,    43,    44,     0,  -277,
      45,    46,     0,    47,    48,    49,    50,    51,    52,     0,
      53,     0,    54,    55,    56,     0,    57,    58,  -277,     0,
       0,    59,     0,    60,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
      68,     0,    69,    70,     0,    71,     0,     0,    72,     0,
       0,     0,    73,     0,    74,     4,    75,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
       0,    14,    15,    16,    17,  -277,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,   779,
       0,    29,    30,     0,  -164,    31,    32,    33,    34,    35,
     -39,    36,    37,     0,    38,     0,     0,     0,  -277,    39,
       0,    41,    42,  -164,    43,    44,     0,  -277,    45,    46,
       0,    47,    48,    49,    50,    51,    52,     0,    53,     0,
      54,    55,    56,     0,    57,    58,  -277,     0,     0,    59,
       0,    60,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,   522,     0,
       5,     6,     7,     8,     9,    10,    67,  -429,    68,   134,
      69,    70,  -429,    71,     0,     0,    72,     0,     0,     0,
      73,  -429,    74,    21,    75,     0,     0,     0,     0,   135,
     136,     0,   137,     0,    29,     0,     0,     0,     0,    32,
      33,     0,    35,     0,    36,    37,     0,     0,     0,     0,
       0,     0,    39,     0,    41,     0,     0,     0,     0,  -429,
       0,     0,     0,  -429,     0,     0,    49,    50,    51,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -295,  -429,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -429,    63,  -429,  -429,  -429,     0,  -429,  -429,
    -429,     0,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,
    -429,    68,  -429,  -429,  -429,     0,     0,  -429,  -429,  -429,
       0,     0,     0,   139,     0,    74,   522,   539,     5,     6,
       7,     8,     9,    10,     0,  -429,     0,   134,     0,     0,
    -429,     0,     0,     0,     0,     0,     0,     0,     0,  -429,
       0,    21,     0,     0,     0,     0,     0,   135,   136,     0,
     137,     0,    29,     0,     0,     0,     0,    32,    33,     0,
      35,     0,    36,    37,     0,     0,     0,     0,     0,     0,
      39,     0,    41,     0,     0,     0,     0,  -429,     0,     0,
       0,  -429,     0,     0,    49,    50,    51,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -429,    63,  -429,  -429,  -429,     0,  -429,  -429,  -429,     0,
    -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,    68,
    -429,  -429,  -429,     0,     0,  -429,  -429,  -429,     0,     0,
       0,   139,     0,    74,     0,   539,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   135,   136,     0,   137,     0,
      29,     0,     0,     0,     0,    32,    33,     0,    35,     0,
      36,    37,     0,     0,     0,     0,     0,     0,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,   138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     5,     6,     7,
       8,     9,    10,     0,     0,    67,   134,    68,     0,    69,
      70,   189,     0,     0,     0,    72,     0,     0,     0,   139,
      21,    74,     0,   140,   553,     0,   135,   136,     0,   137,
       0,    29,     0,     0,     0,     0,    32,    33,     0,    35,
       0,    36,    37,     0,     0,     0,     0,     0,     0,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,     5,     6,
       7,     8,     9,    10,     0,     0,    67,   134,    68,     0,
      69,    70,   189,     0,     0,     0,    72,     0,     0,     0,
     139,    21,    74,     0,   140,   859,     0,   135,   136,     0,
     137,     0,    29,     0,     0,     0,     0,    32,    33,     0,
      35,     0,    36,    37,     0,     0,     0,     0,     0,     0,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     0,
       0,     5,     6,     7,     8,     9,    10,    67,     0,    68,
     134,    69,    70,     0,     0,     0,     0,    72,     0,   459,
       0,   139,     0,    74,    21,   140,   558,     0,     0,     0,
     135,   136,     0,   137,   460,    29,     0,   461,     0,     0,
      32,    33,     0,    35,     0,    36,    37,     0,     0,     0,
       0,   462,   463,    39,     0,    41,     0,     0,     0,     0,
       0,   464,     0,     0,     0,     0,     0,    49,    50,    51,
     138,     0,     0,     0,     0,     0,   465,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,    63,    64,    65,     0,     0,     0,
       0,    66,     5,     6,     7,     8,     9,    10,     0,     0,
      67,   134,    68,     0,    69,    70,     0,     0,     0,     0,
      72,     0,     0,     0,   139,    21,    74,     0,   140,     0,
       0,   135,   136,     0,   137,     0,    29,     0,     0,     0,
       0,    32,    33,     0,    35,     0,    36,    37,     0,     0,
       0,     0,     0,     0,    39,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,   138,     0,     0,     0,     0,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,   229,     0,     5,     6,     7,     8,     9,
      10,    67,     0,    68,   134,    69,    70,   189,     0,     0,
       0,    72,     0,     0,     0,   139,     0,    74,    21,   140,
       0,     0,     0,     0,   135,   136,     0,   137,     0,    29,
       0,     0,     0,     0,    32,    33,     0,    35,     0,    36,
      37,     0,     0,     0,     0,     0,     0,    39,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,   138,     0,     0,     0,     0,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,     5,     6,     7,     8,
       9,    10,     0,     0,    67,   134,    68,     0,    69,    70,
     189,     0,     0,     0,    72,     0,     0,     0,   139,    21,
      74,   547,   140,     0,     0,   135,   136,     0,   137,     0,
      29,     0,     0,     0,     0,    32,    33,     0,    35,     0,
      36,    37,     0,     0,     0,     0,     0,     0,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,   138,     0,     0,     0,     0,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     5,     6,     7,
       8,     9,    10,     0,     0,    67,   134,    68,     0,    69,
      70,   189,     0,     0,     0,    72,     0,     0,     0,   139,
      21,    74,   549,   140,     0,     0,   135,   136,     0,   137,
       0,    29,     0,     0,     0,     0,    32,    33,     0,    35,
       0,    36,    37,     0,     0,     0,     0,     0,   162,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   163,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,     5,     6,
       7,     8,     9,    10,     0,     0,    67,   134,    68,     0,
      69,    70,     0,     0,     0,   167,    72,     0,     0,     0,
     139,    21,    74,     0,   140,     0,   168,   135,   136,     0,
     137,     0,    29,     0,     0,     0,     0,    32,    33,     0,
      35,     0,    36,    37,     0,     0,     0,     0,     0,     0,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     5,
       6,     7,     8,     9,    10,     0,     0,    67,   134,    68,
       0,    69,    70,     0,     0,     0,     0,    72,     0,     0,
       0,   139,    21,    74,     0,   140,     0,     0,   135,   136,
       0,   137,     0,    29,     0,     0,     0,     0,    32,    33,
       0,    35,     0,    36,    37,     0,     0,     0,     0,     0,
       0,    39,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       0,     0,     5,     6,     7,     8,     9,    10,    67,     0,
      68,   134,    69,    70,   189,     0,     0,     0,    72,     0,
       0,     0,   139,   545,    74,    21,   140,     0,     0,     0,
       0,   135,   136,     0,   137,     0,    29,     0,     0,     0,
       0,    32,    33,     0,    35,     0,    36,    37,     0,     0,
       0,     0,     0,     0,    39,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,   138,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     5,     6,     7,     8,     9,    10,     0,
       0,    67,   134,    68,     0,    69,    70,   189,     0,     0,
       0,    72,     0,     0,     0,   139,    21,    74,     0,   140,
       0,     0,   135,   136,     0,   137,     0,    29,     0,     0,
       0,     0,    32,    33,     0,    35,     0,    36,    37,     0,
       0,     0,     0,     0,     0,    39,     0,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,   138,     0,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,    64,    65,     0,
       0,     0,     0,    66,     0,     0,   292,   293,  -388,     0,
       0,  -410,    67,  -410,    68,     0,    69,    70,     0,   193,
       0,     0,    72,     0,     0,     0,   139,     0,    74,     0,
     140,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     134,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   196,    21,     0,     0,     0,     0,     0,
     135,   136,     0,   137,     0,    29,     0,     0,     0,     0,
      32,    33,     0,    35,     0,    36,    37,     0,     0,     0,
       0,     0,     0,    39,     0,    41,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,    51,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,    63,    64,    65,     0,     0,     0,
       0,    66,     5,     6,     7,     8,     9,    10,     0,     0,
      67,   134,    68,     0,    69,    70,     0,     0,     0,     0,
      72,     0,     0,     0,    73,    21,    74,     0,   140,     0,
       0,   135,   136,     0,   137,     0,    29,     0,     0,     0,
       0,    32,    33,     0,    35,     0,    36,    37,     0,     0,
       0,     0,     0,     0,    39,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,   138,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   163,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     5,     6,     7,     8,     9,    10,     0,
       0,    67,   134,    68,     0,    69,    70,     0,     0,     0,
       0,    72,     0,     0,     0,   139,    21,    74,     0,   140,
       0,     0,   135,   136,     0,   137,     0,    29,     0,     0,
       0,     0,    32,    33,     0,    35,     0,    36,    37,   656,
       0,     0,     0,     0,     0,    39,     0,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,   138,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,    64,    65,     0,
       0,     0,     0,    66,     5,     6,     7,     8,     9,    10,
       0,     0,    67,   134,    68,     0,    69,    70,     0,     0,
       0,     0,    72,     0,     0,     0,   139,   816,    74,     0,
     140,     0,     0,   135,   136,     0,   137,     0,    29,     0,
       0,     0,     0,    32,    33,     0,    35,     0,    36,    37,
       0,     0,     0,     0,     0,     0,    39,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     817,    50,    51,   818,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,    64,    65,
       0,     0,     0,     0,    66,     5,     6,     7,     8,     9,
      10,     0,     0,    67,   134,    68,     0,    69,    70,   189,
       0,     0,     0,    72,     0,     0,     0,   139,    21,    74,
       0,   819,     0,     0,   135,   136,     0,   137,     0,    29,
       0,     0,     0,     0,    32,    33,     0,    35,     0,    36,
      37,     0,     0,     0,     0,     0,     0,    39,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,     5,     6,     7,     8,
       9,    10,     0,     0,    67,   134,    68,     0,    69,    70,
     189,     0,     0,     0,    72,     0,     0,     0,   139,    21,
      74,     0,   819,     0,     0,   135,   136,     0,   805,     0,
      29,     0,     0,     0,     0,    32,    33,     0,    35,     0,
      36,    37,     0,     0,     0,     0,     0,     0,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,   138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     5,     6,     7,
       8,     9,    10,     0,     0,    67,   134,    68,     0,    69,
      70,   189,     0,     0,     0,    72,     0,     0,     0,   139,
      21,    74,     0,   140,     0,     0,   135,   136,     0,   137,
       0,    29,     0,     0,     0,     0,    32,    33,     0,    35,
       0,    36,    37,     0,     0,     0,     0,     0,     0,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,     5,     6,
       7,     8,     9,    10,     0,     0,    67,   134,    68,     0,
      69,    70,     0,     0,     0,     0,    72,     0,     0,     0,
     139,    21,    74,     0,   140,     0,     0,   135,   136,     0,
     137,     0,    29,     0,     0,     0,     0,    32,    33,     0,
      35,     0,    36,    37,     0,     0,     0,     0,     0,     0,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     5,
       6,     7,     8,     9,    10,     0,     0,    67,   134,    68,
       0,    69,    70,     0,     0,     0,     0,    72,     0,     0,
       0,    73,    21,    74,     0,   140,     0,     0,   135,   136,
       0,   137,     0,    29,     0,     0,     0,     0,    32,    33,
       0,    35,     0,    36,    37,     0,     0,     0,     0,     0,
       0,    39,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       5,     6,     7,     8,     9,    10,     0,     0,    67,   134,
      68,     0,    69,    70,     0,     0,     0,     0,   504,     0,
       0,     0,   139,    21,    74,     0,   140,     0,     0,   135,
     136,     0,   599,     0,    29,     0,     0,     0,     0,    32,
      33,     0,    35,     0,    36,    37,     0,     0,     0,     0,
       0,     0,    39,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,    65,     0,     0,     0,     0,
      66,     5,     6,     7,     8,     9,    10,     0,     0,    67,
     134,    68,     0,    69,    70,     0,     0,     0,     0,    72,
       0,     0,     0,   139,    21,    74,     0,   140,     0,     0,
     135,   136,     0,   602,     0,    29,     0,     0,     0,     0,
      32,    33,     0,    35,     0,    36,    37,     0,     0,     0,
       0,     0,     0,    39,     0,    41,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,    51,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,    63,    64,    65,     0,     0,     0,
       0,    66,     5,     6,     7,     8,     9,    10,     0,     0,
      67,   134,    68,     0,    69,    70,     0,     0,     0,     0,
      72,     0,     0,     0,   139,    21,    74,     0,   140,     0,
       0,   135,   136,     0,   799,     0,    29,     0,     0,     0,
       0,    32,    33,     0,    35,     0,    36,    37,     0,     0,
       0,     0,     0,     0,    39,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,   138,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     5,     6,     7,     8,     9,    10,     0,
       0,    67,   134,    68,     0,    69,    70,     0,     0,     0,
       0,    72,     0,     0,     0,   139,    21,    74,     0,   140,
       0,     0,   135,   136,     0,   800,     0,    29,     0,     0,
       0,     0,    32,    33,     0,    35,     0,    36,    37,     0,
       0,     0,     0,     0,     0,    39,     0,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,   138,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,    64,    65,     0,
       0,     0,     0,    66,     5,     6,     7,     8,     9,    10,
       0,     0,    67,   134,    68,     0,    69,    70,     0,     0,
       0,     0,    72,     0,     0,     0,   139,    21,    74,     0,
     140,     0,     0,   135,   136,     0,   802,     0,    29,     0,
       0,     0,     0,    32,    33,     0,    35,     0,    36,    37,
       0,     0,     0,     0,     0,     0,    39,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      49,    50,    51,   138,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,    64,    65,
       0,     0,     0,     0,    66,     5,     6,     7,     8,     9,
      10,     0,     0,    67,   134,    68,     0,    69,    70,     0,
       0,     0,     0,    72,     0,     0,     0,   139,    21,    74,
       0,   140,     0,     0,   135,   136,     0,   803,     0,    29,
       0,     0,     0,     0,    32,    33,     0,    35,     0,    36,
      37,     0,     0,     0,     0,     0,     0,    39,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,     5,     6,     7,     8,
       9,    10,     0,     0,    67,   134,    68,     0,    69,    70,
       0,     0,     0,     0,    72,     0,     0,     0,   139,    21,
      74,     0,   140,     0,     0,   135,   136,     0,   804,     0,
      29,     0,     0,     0,     0,    32,    33,     0,    35,     0,
      36,    37,     0,     0,     0,     0,     0,     0,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,   138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     5,     6,     7,
       8,     9,    10,     0,     0,    67,   134,    68,     0,    69,
      70,     0,     0,     0,     0,    72,     0,     0,     0,   139,
      21,    74,     0,   140,     0,     0,   135,   136,     0,   805,
       0,    29,     0,     0,     0,     0,    32,    33,     0,    35,
       0,    36,    37,     0,     0,     0,     0,     0,     0,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,     5,     6,
       7,     8,     9,    10,     0,     0,    67,   134,    68,     0,
      69,    70,     0,     0,     0,     0,    72,     0,     0,     0,
     139,    21,    74,     0,   140,     0,     0,   135,   136,     0,
     137,     0,    29,     0,     0,     0,     0,    32,    33,     0,
      35,     0,    36,    37,     0,     0,     0,     0,     0,     0,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
      62,    63,    64,    65,     0,   135,   136,     0,   137,     0,
      29,     0,     0,     0,     0,    32,    33,    67,    35,    68,
      36,    37,    70,     0,     0,     0,     0,    72,    39,     5,
      41,   139,     0,    74,     0,   140,   681,     0,     0,     0,
       0,   682,    49,    50,    51,   138,     0,     0,     0,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,     0,  -304,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     0,    63,
       0,     0,     0,  -304,   681,     0,     0,  -304,     0,   682,
       0,   292,   293,     0,     0,     0,  -410,    68,  -410,     0,
       0,   189,     0,     0,     0,     0,     0,     0,     0,   139,
       0,    74,  -304,   819,     0,     0,     0,   683,   684,   685,
     686,   687,   688,     0,     0,   689,   690,   691,   692,   693,
     694,   695,   696,   697,   698,     0,     0,   699,     0,     0,
       0,   700,   701,   702,   703,   704,   705,   706,   707,   708,
     709,   710,     0,   711,     0,     0,   712,   713,   714,   715,
       0,   716,     0,     0,   717,   683,   684,   685,   686,   687,
     688,     0,     0,   689,   690,   691,   692,   693,   694,   695,
     696,   697,   698,   252,     0,   699,     0,     0,     0,   700,
     701,   702,   703,   704,   705,   706,   707,   708,   709,   710,
       0,   711,     0,   250,   712,   713,   714,   715,   251,   716,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
       0,   253,     0,     0,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   250,     0,   255,
       0,     0,   251,     0,     0,   253,     0,     0,     0,   254,
       0,   252,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,     0,   268,
     269,   270,   271,   255,   272,     0,   274,     0,     0,   275,
     276,   277,     0,     0,     0,     0,     0,     0,   256,   253,
     257,   258,   259,   254,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,     0,   272,   273,
     274,   250,     0,   275,   276,   277,   251,   255,     0,     0,
       0,     0,     0,     0,   868,   252,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,     0,   272,   273,   274,   250,     0,   275,   276,   277,
     251,     0,     0,   253,     0,     0,     0,   254,   905,   252,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,   253,   257,   258,
     259,   254,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,     0,   272,   273,   274,   250,
       0,   275,   276,   277,   251,   255,     0,     0,     0,     0,
     546,     0,     0,   252,   328,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,   329,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,     0,
     272,   273,   274,   250,     0,   275,   276,   277,   251,     0,
       0,   253,     0,     0,   841,   254,     0,   252,   325,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   326,
       0,     0,     0,     0,     0,   330,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,   253,   257,   258,   259,   254,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,     0,   272,   273,   274,     0,     0,   275,
     276,   277,   250,   255,     0,   301,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   252,   629,   256,     0,
     257,   258,   259,     0,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,     0,   272,   273,
     274,     0,     0,   275,   276,   277,     0,     0,     0,   301,
       0,     0,     0,     0,   253,     0,     0,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   250,
       0,     0,     0,     0,   251,     0,     0,     0,   330,     0,
       0,     0,   255,   252,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,   257,
     258,   259,     0,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,     0,   272,   273,   274,
       0,   253,   275,   276,   277,   254,     0,     0,   301,     0,
       0,   250,   336,     0,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,   196,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,     0,   275,
     276,   277,     0,   250,     0,   301,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   252,   623,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   253,   272,   273,   274,   254,
       0,   275,   276,   277,     0,   250,     0,   301,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
     257,   258,   259,     0,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   253,   272,   273,
     274,   254,     0,   275,   276,   277,     0,   250,   741,   301,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   253,
     272,   273,   274,   254,     0,   275,   276,   277,     0,   250,
     746,   301,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,     0,   275,   276,   277,
       0,   250,   853,   301,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,     0,   275,
     276,   277,     0,   250,   854,   301,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   253,   272,   273,   274,   254,
       0,   275,   276,   277,     0,   250,   855,   301,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
     257,   258,   259,     0,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   253,   272,   273,
     274,   254,     0,   275,   276,   277,     0,   250,   856,   301,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   253,
     272,   273,   274,   254,     0,   275,   276,   277,     0,   250,
     870,   301,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,     0,   275,   276,   277,
       0,   250,   871,   301,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,     0,   275,
     276,   277,     0,   250,     0,   301,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   253,   272,   273,   274,   254,
       0,   275,   276,   277,     0,     0,     0,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     250,     0,     0,   255,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   256,   333,
     257,   258,   259,     0,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,     0,   272,   273,
     274,   334,     0,   275,   276,   277,     0,     0,  -389,     0,
       0,     0,   253,     0,     0,     0,   254,     0,     0,     0,
       0,     0,   250,     0,     0,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   252,     0,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,   307,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   253,   272,   273,   274,   254,   335,
     275,   276,   277,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   250,     0,     0,   128,     0,
     251,     0,   255,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,     0,   374,     0,     0,   256,     0,   257,
     258,   259,     0,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   375,   272,   273,   274,
       0,     0,   275,   276,   277,     0,     0,   253,     0,     0,
       0,   254,     0,     0,     0,     0,     0,   250,     0,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,   471,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,   472,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   253,
     272,   273,   274,   254,     0,   275,   276,   277,     0,   250,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,   473,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,   474,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,     0,   275,   276,   277,
       0,   250,     0,     0,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,     0,   275,
     276,   277,     0,   250,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   253,   272,   273,   274,   254,
     311,   275,   276,   277,     0,   250,     0,     0,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
     257,   258,   259,     0,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   253,   272,   273,
     274,   254,   359,   275,   276,   277,     0,   250,     0,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   253,
     272,   273,   274,   254,   365,   275,   276,   277,     0,   250,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,   367,   275,   276,   277,
       0,   250,     0,     0,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,   369,   275,
     276,   277,     0,   250,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   253,   272,   273,   274,   254,
     377,   275,   276,   277,     0,   250,     0,     0,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,   391,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
     257,   258,   259,     0,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   253,   272,   273,
     274,   254,     0,   275,   276,   277,     0,   250,   475,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   253,
     272,   273,   274,   254,     0,   275,   276,   277,     0,   250,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,   574,   275,   276,   277,
       0,   250,     0,     0,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,   575,   275,
     276,   277,     0,   250,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   253,   272,   273,   274,   254,
     576,   275,   276,   277,     0,   250,     0,     0,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
     257,   258,   259,     0,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   253,   272,   273,
     274,   254,   577,   275,   276,   277,     0,   250,     0,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   253,
     272,   273,   274,   254,   578,   275,   276,   277,     0,   250,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,   579,   275,   276,   277,
       0,   250,     0,     0,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,   580,   275,
     276,   277,     0,   250,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   253,   272,   273,   274,   254,
     581,   275,   276,   277,     0,   250,     0,     0,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
     257,   258,   259,     0,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   253,   272,   273,
     274,   254,   582,   275,   276,   277,     0,   250,     0,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   253,
     272,   273,   274,   254,   583,   275,   276,   277,     0,   250,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,   584,   275,   276,   277,
       0,   250,     0,     0,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,   585,   275,
     276,   277,     0,   250,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   253,   272,   273,   274,   254,
     586,   275,   276,   277,     0,   250,     0,     0,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
     257,   258,   259,     0,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   253,   272,   273,
     274,   254,   587,   275,   276,   277,     0,   250,     0,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   253,
     272,   273,   274,   254,   588,   275,   276,   277,     0,   250,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,   589,   275,   276,   277,
       0,     0,     0,     0,     0,     0,   250,     0,     0,     0,
       0,   251,     0,     0,     0,   128,     0,     0,     0,   255,
     252,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,     0,   272,   273,   274,     0,   253,   275,
     276,   277,   254,     0,     0,     0,     0,     0,   250,   636,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,   257,   258,   259,     0,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     253,   272,   273,   274,   254,   609,   275,   276,   277,     0,
     250,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,   639,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   253,   272,   273,   274,   254,     0,   275,   276,
     277,     0,   250,     0,     0,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   252,     0,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   253,   272,   273,   274,   254,     0,
     275,   276,   277,     0,   250,     0,     0,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,   252,   729,
       0,   675,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,   257,
     258,   259,     0,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   253,   272,   273,   274,
     254,     0,   275,   276,   277,     0,   250,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,   732,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   253,   272,
     273,   274,   254,     0,   275,   276,   277,     0,   250,     0,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,   257,   258,   259,     0,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     253,   272,   273,   274,   254,     0,   275,   276,   277,     0,
     250,   798,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   253,   272,   273,   274,   254,     0,   275,   276,
     277,     0,     0,   801,     0,     0,     0,   250,   814,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
     255,   252,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   252,   272,   273,   274,     0,   253,
     275,   276,   277,   254,     0,     0,     0,     0,     0,   250,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,   255,     0,     0,
       0,     0,   253,     0,     0,     0,   254,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,     0,   275,   276,   277,
       0,   250,   877,     0,     0,   256,   251,   257,   258,   259,
       0,   260,   261,   262,     0,   252,   264,   265,   266,   255,
     268,   269,   270,   271,     0,     0,     0,   274,     0,     0,
     275,   276,   277,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,     0,   275,
     276,   277,     0,   250,   878,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   253,   272,   273,   274,   254,
       0,   275,   276,   277,     0,   250,   879,     0,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
     257,   258,   259,     0,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   253,   272,   273,
     274,   254,     0,   275,   276,   277,     0,   250,   880,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   253,
     272,   273,   274,   254,     0,   275,   276,   277,     0,   250,
     881,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,     0,   275,   276,   277,
       0,   250,   882,     0,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,     0,   275,
     276,   277,     0,   250,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   252,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   597,   272,   273,   274,   254,
       0,   275,   276,   277,     0,   252,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
     257,   258,   259,   253,   260,   261,   262,   254,   263,   264,
     265,   266,   267,   268,   269,   270,   271,     0,   272,   273,
     274,     0,     0,   275,   276,   277,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
       0,   268,   269,   270,   271,     0,   272,     0,   274,     0,
       0,   275,   276,   277
};

static const yytype_int16 yycheck[] =
{
      11,    42,    43,   370,     2,    16,    15,   343,    19,   150,
      45,     0,   298,    23,    25,    26,    27,    28,     3,   160,
     678,   521,     3,    58,    35,    36,   678,    38,    39,   340,
     653,   234,    86,    36,    45,    46,    47,    48,    49,    25,
      75,    52,    53,    54,    55,   172,     3,    58,    59,    60,
       1,   502,    63,     3,     3,    66,    67,    68,   185,    70,
      25,    25,    73,    74,    75,   244,   245,   246,   247,   334,
       3,   721,     1,    36,     1,   202,   110,   110,   138,    25,
      52,    25,   164,   165,   166,     3,    86,     1,    21,   110,
     110,   138,    65,   140,    79,   129,    36,   859,    36,    74,
     182,    36,   827,    36,   137,   140,    39,   110,    26,    36,
     375,    83,    36,    63,    65,    40,   348,   199,   139,   139,
      53,    54,    36,   134,   135,   136,   137,   138,   139,   140,
      63,   789,   894,    84,    59,   217,    65,   146,   370,   762,
      86,   141,    65,   154,   595,    78,    75,   110,   806,   649,
     136,    86,     3,   138,   806,    84,   129,   138,   109,    78,
      78,   172,    89,   136,    91,   137,   110,   294,   295,   138,
     110,   136,   110,   898,   185,   110,   186,    91,   141,   154,
     109,   138,   109,   110,   520,   136,   110,    36,   138,   138,
     136,   202,   136,   843,   844,   109,   110,   172,   110,    91,
     129,   141,   129,   141,   110,   138,   141,   136,   110,     7,
     185,   110,   110,   136,    63,   129,   502,   141,   229,    13,
     399,   894,   114,   138,    18,   140,   308,   202,   226,   141,
     129,   129,   905,   139,   138,   858,   363,   139,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     332,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   323,   253,
     254,   791,    17,    71,   110,   110,   307,   500,   501,    36,
     110,   312,   753,   138,     3,   110,   110,   643,     7,   595,
     110,   138,   323,   140,   325,   326,    46,   328,   329,   294,
     295,   137,   333,   334,   139,   336,    21,   129,    65,   139,
     641,   110,   296,   297,   139,   139,    61,    21,    75,   139,
     351,    40,    89,   307,    91,   129,   357,    84,   551,   552,
     129,    24,   363,     3,    91,    80,   383,     7,   385,    54,
      59,   396,   326,   374,   375,   329,     2,   110,    63,   380,
      54,   110,   383,   110,   385,    11,   387,   388,   389,    63,
     391,   392,   357,    91,    20,   396,   129,    82,   363,    62,
     129,    21,   129,    66,    78,    54,    55,   129,    34,   136,
     767,   542,   110,   110,    24,   380,   114,   774,   383,    91,
     385,   135,   129,   388,   496,   497,    52,   129,   500,   501,
     652,   110,   129,   387,    54,    62,     2,   129,   110,    66,
      62,   139,   114,    63,    66,    11,   109,    73,   111,   112,
     129,    77,    62,   116,    20,   138,    66,    85,    78,   109,
     123,   129,   112,   129,   129,   136,   116,   139,    34,   132,
     471,   472,   473,   474,   475,   476,   477,   753,   136,   478,
     479,   129,   483,   484,   138,   486,    52,   138,    54,    55,
     138,   492,   140,   494,   795,    36,   112,   498,   523,   109,
     138,   502,   112,   129,    91,   129,   116,    73,     7,   109,
     129,    77,   110,    36,   539,   129,   139,   138,   472,   484,
     474,   486,   523,    27,    65,   526,   527,   139,   110,    89,
     139,   139,    91,   139,    75,   607,   608,    16,   539,   761,
       2,   110,   141,    84,    20,   767,   618,   173,   136,    11,
      91,   136,   774,   139,    30,    31,   628,     3,    20,   631,
     141,     7,    38,   129,    40,    91,    20,   129,   110,   110,
     196,   129,    34,   129,    24,    21,    30,    31,    27,   129,
     129,   110,   139,    59,    38,   139,    40,    25,   129,    63,
      52,    25,    46,   141,   595,   136,   597,   598,   599,   136,
     226,   602,   109,    63,   109,    59,   141,   173,    54,   141,
     112,    73,    62,   614,   112,    77,    66,    63,   138,    84,
     621,   139,   623,   139,    75,   626,   375,   560,   629,   374,
     196,   210,    78,   634,   659,   636,    82,   859,   639,   161,
     491,   663,   739,   597,   598,     3,   868,   615,   794,     7,
     794,     9,   795,   680,   641,   105,   898,   858,   659,   109,
     226,   111,   112,    21,   873,    27,   116,   129,    20,   670,
     352,   672,   894,   123,   675,   301,   815,   486,    30,    31,
     130,   131,   132,   905,    -1,    -1,    38,    -1,    40,   677,
      -1,    -1,    -1,    -1,    46,    -1,    54,    -1,   770,   325,
      -1,    -1,   328,    -1,    -1,    63,    -1,    59,    -1,    -1,
     336,   173,    -1,    -1,    -1,    -1,   717,    -1,    -1,    -1,
      78,    36,    -1,    -1,    82,    -1,    -1,   728,   729,    -1,
     731,   732,   733,   734,   196,   301,    -1,    -1,    -1,    -1,
     741,   742,    24,   744,    -1,   746,   747,    -1,    -1,   750,
      65,    -1,   753,    -1,    -1,   786,   787,    -1,    -1,   325,
      75,    -1,   328,   389,   226,   766,   767,    -1,    -1,    84,
     336,   772,    -1,   774,   775,    -1,    91,    -1,   779,    -1,
      62,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   819,   110,   761,   798,   799,   800,
     801,   802,   803,   804,   805,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   814,   129,    -1,   817,   818,   819,    -1,
      -1,   136,   823,   389,    -1,   826,    -1,   109,    -1,   111,
     112,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,   301,
     466,   123,    -1,    -1,    -1,    -1,    10,    -1,   130,   131,
     132,    15,   853,   854,   855,   856,    -1,    -1,   823,    -1,
      24,    -1,    -1,   325,     2,    -1,   328,   868,    -1,   870,
     871,    -1,    -1,    11,   336,    -1,   877,   878,   879,   880,
     881,   882,    20,    -1,    -1,    -1,   512,    -1,    -1,    -1,
      -1,    -1,   893,   894,   859,    -1,    34,    -1,    62,    -1,
      -1,    -1,    66,    -1,   905,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    90,   389,    -1,   894,
     556,    -1,    -1,    -1,   560,    73,    -1,    -1,    -1,    77,
     905,   105,   568,   107,   108,   109,   512,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      -1,   125,   126,   127,    -1,    -1,   130,   131,   132,    21,
      -1,   597,   598,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    39,    -1,    -1,
     556,   129,    -1,    -1,   560,   621,    -1,   623,     2,     2,
     626,    53,    54,   629,    -1,    -1,    -1,    11,    11,    -1,
      -1,    63,   638,    -1,    -1,    -1,    20,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      34,    34,   658,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   670,    -1,   672,    -1,    52,    52,
     512,   677,    -1,    -1,    -1,   621,    -1,   623,   196,    -1,
     626,    -1,    -1,   629,    -1,    -1,    -1,    -1,    -1,    73,
      73,    -1,   638,    77,    77,    -1,    -1,    -1,     2,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,   226,    -1,
      -1,    -1,   658,    -1,   556,    -1,    20,    -1,   560,    -1,
      -1,    -1,    -1,    -1,   670,    -1,   672,    -1,    -1,    -1,
      34,   677,    -1,    -1,    -1,   741,    -1,    -1,    -1,    -1,
     746,    -1,    -1,    -1,    -1,   129,   129,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,
      -1,    -1,   768,    -1,    -1,    11,    -1,    -1,    -1,    73,
      -1,    -1,   778,    77,    20,    -1,   782,    -1,    -1,   621,
      -1,   623,    -1,   301,   626,    -1,    -1,   629,    34,   173,
     173,    -1,    -1,    -1,    -1,   741,   638,    -1,    -1,    -1,
     746,    -1,    -1,    -1,    -1,    -1,    52,   325,    -1,    -1,
     328,    -1,   196,   196,    -1,    -1,   658,    -1,   336,    -1,
      -1,    -1,   768,    -1,    -1,   129,    -1,    73,   670,    -1,
     672,    77,   778,    -1,    -1,   677,   782,    -1,    -1,    -1,
      -1,    -1,   226,   226,    -1,    -1,    -1,   853,   854,   855,
     856,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   870,   871,    -1,    -1,    -1,   173,
      -1,   389,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,    -1,    -1,    -1,    -1,    -1,    -1,   741,
      -1,    -1,    -1,    -1,   746,    -1,    -1,   853,   854,   855,
     856,    -1,    -1,    -1,    -1,    -1,    -1,   301,   301,    -1,
      -1,    -1,   226,    -1,   870,   871,   768,   173,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   778,    -1,    -1,    -1,
     782,   325,   325,    -1,   328,   328,    17,    -1,    -1,    20,
     196,    -1,   336,    -1,    -1,    -1,    -1,    28,    -1,    30,
      31,    -1,    -1,    34,    -1,    -1,    -1,    38,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,    -1,    -1,    -1,    -1,    56,    -1,    58,    59,    60,
      61,    -1,    -1,    -1,   512,    -1,    -1,   301,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   389,   389,    78,    -1,    80,
      -1,   853,   854,   855,   856,    -1,    -1,     2,    -1,    -1,
      -1,   325,    -1,    -1,   328,    -1,    11,    -1,   870,   871,
      17,    -1,   336,    20,    -1,    20,    -1,    -1,   556,    -1,
      -1,    28,   560,    30,    31,    -1,    -1,    34,    -1,    34,
      -1,    38,    -1,    40,    -1,   301,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    52,    -1,    56,
      -1,    58,    59,    60,    61,    -1,    -1,    -1,    -1,   325,
      -1,    -1,   328,    -1,    -1,   389,    -1,    -1,    73,    -1,
     336,    78,    77,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   621,    -1,   623,    -1,    -1,   626,    -1,
      -1,   629,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
     638,    -1,    -1,    -1,    17,    -1,    -1,    20,   512,   512,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    30,    31,    -1,
     658,    34,   129,   389,   129,    38,    -1,    40,    -1,    -1,
     137,    -1,   670,    -1,   672,    -1,    62,    -1,    -1,   677,
      66,    -1,    -1,    56,    -1,    58,    59,    60,    61,    -1,
      -1,    -1,   556,   556,    -1,    -1,   560,   560,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,   512,    -1,
     116,   196,    -1,    -1,    -1,    -1,   122,   123,    -1,    -1,
      -1,   127,    -1,   741,   130,   131,   132,    -1,   746,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   621,   621,   623,
     623,   226,   626,   626,    -1,   629,   629,    -1,    -1,    -1,
     768,    -1,   556,    -1,   638,   638,   560,    -1,    -1,    -1,
     778,    -1,    -1,    -1,   782,    -1,   512,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   658,   658,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,   670,   670,   672,   672,
      54,    55,    -1,   677,   677,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     556,    -1,    -1,    -1,   560,    -1,   301,   621,    -1,   623,
      -1,    -1,   626,    -1,    -1,   629,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   638,   853,   854,   855,   856,    -1,
     325,    -1,    -1,   328,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   336,   870,   871,   658,    -1,    -1,   741,    -1,    -1,
      -1,    -1,   746,    -1,    -1,    -1,   670,    -1,   672,   133,
      -1,    -1,    -1,   677,    -1,   621,    -1,   623,    -1,    -1,
     626,    -1,    -1,   629,   768,   768,    -1,    -1,    -1,    -1,
      -1,    -1,   638,    -1,   778,   778,    -1,    -1,   782,   782,
     164,   165,   166,    -1,   389,    -1,    -1,   171,    -1,    -1,
      -1,    -1,   658,    -1,    -1,    -1,    -1,    -1,   182,    -1,
      -1,    -1,    -1,    -1,   670,    -1,   672,    -1,    -1,    -1,
      -1,   677,    -1,    -1,    -1,   199,    -1,   741,    -1,    -1,
      -1,    -1,   746,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   768,    -1,    -1,    -1,    -1,   853,
     854,   855,   856,    -1,   778,    -1,    -1,    -1,   782,     3,
       4,     5,     6,     7,     8,    -1,   870,   871,    12,    -1,
      -1,    -1,    -1,    -1,    -1,   741,    -1,    -1,    -1,    -1,
     746,    -1,    26,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    35,    -1,    37,    -1,    -1,    -1,    -1,    42,    43,
      -1,    45,   768,    47,    48,    -1,    -1,   512,    -1,    -1,
      -1,    55,   778,    57,    -1,    -1,   782,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   308,    69,    70,    71,    72,   853,
     854,   855,   856,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   870,   871,   332,    -1,
      -1,   556,    -1,    -1,    -1,   560,    13,    -1,    -1,    -1,
      -1,    18,   106,    -1,    -1,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    10,
     124,    -1,    -1,    -1,    15,    -1,    -1,   853,   854,   855,
     856,    -1,    -1,    24,   138,    -1,   140,    -1,    -1,    56,
      -1,    -1,    -1,    -1,   870,   871,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   621,    -1,   623,    -1,
      -1,   626,    -1,    -1,   629,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    -1,   638,    -1,    66,    -1,    -1,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   658,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,   670,    -1,   672,    -1,    -1,
      -1,    -1,   677,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   150,   125,   126,   127,   154,    -1,   130,
     131,   132,    -1,   160,    -1,   162,    -1,    -1,    -1,    -1,
     141,    -1,   496,   497,    24,   172,   500,   501,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,   189,    -1,    -1,    -1,   741,    -1,    -1,    -1,
      -1,   746,    -1,    -1,    -1,   202,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,   768,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   778,    -1,    -1,    -1,   782,    -1,    -1,
      90,    -1,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
     247,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,   261,   115,   116,   117,   118,    -1,
     120,   121,   122,   123,    -1,   125,   126,   127,    -1,    -1,
     130,   131,   132,   607,   608,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   618,    -1,   620,   294,   295,    -1,
      -1,   625,    -1,    -1,   628,    -1,    -1,   631,   853,   854,
     855,   856,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   319,    -1,   321,   870,   871,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    24,    12,    -1,
      -1,   665,    -1,    -1,    -1,    -1,   343,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,   352,    -1,    -1,    32,    33,
      -1,    35,    -1,    37,    -1,    -1,   363,    -1,    42,    43,
      -1,    45,    -1,    47,    48,    62,    -1,    -1,    -1,    66,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,    -1,
      -1,    -1,   399,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,    -1,    -1,    -1,   116,
      -1,   105,   106,   107,   108,   122,   123,    -1,    -1,   113,
     127,    -1,    -1,   130,   131,   132,   770,    -1,   122,    -1,
     124,    -1,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,   136,    -1,   138,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   808,   809,    -1,   811,   812,   486,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      24,    12,    -1,   827,    -1,    -1,    -1,    -1,    -1,   833,
      -1,   835,    -1,   837,    -1,    26,    -1,    -1,    -1,   516,
      -1,    32,    33,   520,    35,    -1,    37,    -1,    -1,    -1,
      -1,    42,    43,    -1,    45,    -1,    47,    48,    62,    -1,
     537,    -1,    66,    -1,    55,   542,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   568,    -1,    -1,   898,    -1,    -1,    -1,    -1,    90,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,    -1,
      -1,    -1,   116,    -1,   105,   106,   107,   108,    -1,   123,
      -1,    -1,   113,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,   122,    -1,   124,    -1,   126,   127,    -1,   615,    -1,
      -1,   132,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,
       3,     4,     5,     6,     7,     8,   643,    -1,    24,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    -1,    25,    26,    -1,    28,   663,    30,    31,    32,
      33,    34,    35,    -1,    37,    38,    -1,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    62,    50,    -1,    -1,
      66,    54,    55,    56,    57,    58,    59,    60,    61,   696,
      63,    64,    65,    -1,    67,    68,    69,    70,    71,    72,
      -1,    74,    -1,    76,    77,    78,    -1,    80,    81,    82,
      -1,    -1,    85,    -1,    87,    -1,    -1,    90,    -1,   105,
      -1,    -1,   108,   109,    -1,   111,   112,    -1,    -1,    -1,
     116,    -1,   105,   106,   107,   108,    -1,   123,    -1,    -1,
     113,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,   122,
      -1,   124,    -1,   126,   127,    -1,   129,    -1,    -1,   132,
      -1,    -1,    -1,   136,    -1,   138,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,   785,     3,
       4,     5,     6,     7,     8,    -1,    10,   794,    12,    13,
      14,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    30,    31,    32,    33,
      34,    35,    -1,    37,    38,    -1,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,    76,    77,    78,    -1,    80,    81,    82,    -1,
      -1,    85,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,   129,   130,   131,   132,    -1,
      -1,    -1,   136,   137,   138,     1,   140,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    -1,    25,
      26,    -1,    28,    -1,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    -1,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    -1,    50,    -1,    -1,    -1,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      76,    77,    78,    -1,    80,    81,    82,    -1,    -1,    85,
      -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
     126,   127,   128,   129,    -1,    -1,   132,    -1,    -1,    -1,
     136,   137,   138,     1,   140,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    -1,    25,    26,    -1,
      28,    -1,    30,    31,    32,    33,    34,    35,    -1,    37,
      38,    -1,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    -1,    50,    -1,    -1,    -1,    54,    55,    56,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    -1,    67,
      68,    69,    70,    71,    72,    -1,    74,    -1,    76,    77,
      78,    -1,    80,    81,    82,    -1,    -1,    85,    -1,    87,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,   126,   127,
      -1,   129,    -1,    -1,   132,    -1,    -1,    -1,   136,   137,
     138,     1,   140,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    -1,    25,    26,    -1,    28,    -1,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    -1,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      50,    -1,    -1,    -1,    54,    55,    56,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    -1,    67,    68,    69,
      70,    71,    72,    -1,    74,    -1,    76,    77,    78,    -1,
      80,    81,    82,    -1,    -1,    85,    -1,    87,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,    -1,   126,   127,    -1,   129,
      -1,    -1,   132,    -1,    -1,    -1,   136,   137,   138,     1,
     140,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    -1,    25,    26,    -1,    28,    -1,    30,    31,
      32,    33,    34,    35,    -1,    37,    38,    -1,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    -1,    50,    -1,
      -1,    -1,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    -1,    67,    68,    69,    70,    71,
      72,    -1,    74,    -1,    76,    77,    78,    -1,    80,    81,
      82,    -1,    -1,    85,    -1,    87,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,   124,    -1,   126,   127,    -1,   129,    -1,    -1,
     132,    -1,    -1,    -1,   136,   137,   138,     1,   140,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      -1,    25,    26,    -1,    28,    -1,    30,    31,    32,    33,
      34,    35,    -1,    37,    38,    -1,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    -1,    50,    -1,    -1,    -1,
      54,    55,    56,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,    76,    77,    78,    -1,    80,    81,    82,    -1,
      -1,    85,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,    -1,   126,   127,    -1,   129,    -1,    -1,   132,    -1,
      -1,    -1,   136,   137,   138,     1,   140,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    -1,    25,
      26,    -1,    28,    -1,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    -1,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    -1,    50,    -1,    -1,    -1,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      76,    77,    78,    -1,    80,    81,    82,    -1,    -1,    85,
      -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
     126,   127,    -1,   129,    -1,    -1,   132,    -1,    -1,    -1,
     136,    -1,   138,     1,   140,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    -1,    25,    26,    -1,
      28,    -1,    30,    31,    32,    33,    34,    35,    -1,    37,
      38,    -1,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    -1,    67,
      68,    69,    70,    71,    72,    -1,    74,    -1,    76,    77,
      78,    -1,    80,    81,    82,    -1,    -1,    85,    -1,    87,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,   126,   127,
      -1,   129,    -1,    -1,   132,    -1,    -1,    -1,   136,    -1,
     138,     1,   140,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    -1,    25,    26,    -1,    28,    -1,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    -1,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    -1,    67,    68,    69,
      70,    71,    72,    -1,    74,    -1,    76,    77,    78,    -1,
      80,    81,    82,    -1,    -1,    85,    -1,    87,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,    -1,   126,   127,    -1,   129,
      -1,    -1,   132,    -1,    -1,    -1,   136,    -1,   138,     1,
     140,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    -1,    25,    26,    -1,    28,    -1,    30,    31,
      32,    33,    34,    35,    -1,    37,    38,    -1,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    -1,    67,    68,    69,    70,    71,
      72,    -1,    74,    -1,    76,    77,    78,    -1,    80,    81,
      82,    -1,    -1,    85,    -1,    87,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,   124,    -1,   126,   127,    -1,   129,    -1,    -1,
     132,    -1,    -1,    -1,   136,    -1,   138,     1,   140,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      -1,    25,    26,    -1,    28,    -1,    30,    31,    32,    33,
      34,    35,    -1,    37,    38,    -1,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,    76,    77,    78,    -1,    80,    81,    82,    -1,
      -1,    85,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,    -1,   126,   127,    -1,   129,    -1,    -1,   132,    -1,
      -1,    -1,   136,    -1,   138,     1,   140,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    -1,    25,
      26,    -1,    28,    -1,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    -1,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      76,    77,    78,    -1,    80,    81,    82,    -1,    -1,    85,
      -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
     126,   127,    -1,   129,    -1,    -1,   132,    -1,    -1,    -1,
     136,    -1,   138,     1,   140,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    -1,    25,    26,    -1,
      28,    -1,    30,    31,    32,    33,    34,    35,    -1,    37,
      38,    -1,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    -1,    67,
      68,    69,    70,    71,    72,    -1,    74,    -1,    76,    77,
      78,    -1,    80,    81,    82,    -1,    -1,    85,    -1,    87,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,   126,   127,
      -1,   129,    -1,    -1,   132,    -1,    -1,    -1,   136,    -1,
     138,     1,   140,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    -1,    25,    26,    -1,    28,    -1,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    -1,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    -1,    67,    68,    69,
      70,    71,    72,    -1,    74,    -1,    76,    77,    78,    -1,
      80,    81,    82,    -1,    -1,    85,    -1,    87,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,    -1,   126,   127,    -1,   129,
      -1,    -1,   132,    -1,    -1,    -1,   136,    -1,   138,     1,
     140,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    -1,    25,    26,    -1,    28,    -1,    30,    31,
      32,    33,    34,    35,    -1,    37,    38,    -1,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    -1,    67,    68,    69,    70,    71,
      72,    -1,    74,    -1,    76,    77,    78,    -1,    80,    81,
      82,    -1,    -1,    85,    -1,    87,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,   124,    -1,   126,   127,    -1,   129,    -1,    -1,
     132,    -1,    -1,    -1,   136,    -1,   138,     1,   140,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      -1,    25,    26,    -1,    28,    -1,    30,    31,    32,    33,
      34,    35,    -1,    37,    38,    -1,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,    76,    77,    78,    -1,    80,    81,    82,    -1,
      -1,    85,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,    -1,   126,   127,    -1,   129,    -1,    -1,   132,    -1,
      -1,    -1,   136,    -1,   138,     1,   140,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    -1,    25,
      26,    -1,    28,    -1,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    -1,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      76,    77,    78,    -1,    80,    81,    82,    -1,    -1,    85,
      -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,    -1,    -1,    -1,    -1,   113,     1,    -1,
       3,     4,     5,     6,     7,     8,   122,    10,   124,    12,
     126,   127,    15,   129,    -1,    -1,   132,    -1,    -1,    -1,
     136,    24,   138,    26,   140,    -1,    -1,    -1,    -1,    32,
      33,    -1,    35,    -1,    37,    -1,    -1,    -1,    -1,    42,
      43,    -1,    45,    -1,    47,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    62,
      -1,    -1,    -1,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,   136,    -1,   138,     1,   140,     3,     4,
       5,     6,     7,     8,    -1,    10,    -1,    12,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      35,    -1,    37,    -1,    -1,    -1,    -1,    42,    43,    -1,
      45,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    66,    -1,    -1,    69,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,   136,    -1,   138,    -1,   140,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,
      37,    -1,    -1,    -1,    -1,    42,    43,    -1,    45,    -1,
      47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
     107,   108,    -1,    -1,    -1,    -1,   113,     3,     4,     5,
       6,     7,     8,    -1,    -1,   122,    12,   124,    -1,   126,
     127,   128,    -1,    -1,    -1,   132,    -1,    -1,    -1,   136,
      26,   138,    -1,   140,   141,    -1,    32,    33,    -1,    35,
      -1,    37,    -1,    -1,    -1,    -1,    42,    43,    -1,    45,
      -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,    -1,    -1,    -1,    -1,   113,     3,     4,
       5,     6,     7,     8,    -1,    -1,   122,    12,   124,    -1,
     126,   127,   128,    -1,    -1,    -1,   132,    -1,    -1,    -1,
     136,    26,   138,    -1,   140,   141,    -1,    32,    33,    -1,
      35,    -1,    37,    -1,    -1,    -1,    -1,    42,    43,    -1,
      45,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,    -1,
      -1,     3,     4,     5,     6,     7,     8,   122,    -1,   124,
      12,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    21,
      -1,   136,    -1,   138,    26,   140,   141,    -1,    -1,    -1,
      32,    33,    -1,    35,    36,    37,    -1,    39,    -1,    -1,
      42,    43,    -1,    45,    -1,    47,    48,    -1,    -1,    -1,
      -1,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,
      -1,   113,     3,     4,     5,     6,     7,     8,    -1,    -1,
     122,    12,   124,    -1,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,   136,    26,   138,    -1,   140,    -1,
      -1,    32,    33,    -1,    35,    -1,    37,    -1,    -1,    -1,
      -1,    42,    43,    -1,    45,    -1,    47,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,
      -1,    -1,   113,   114,    -1,     3,     4,     5,     6,     7,
       8,   122,    -1,   124,    12,   126,   127,   128,    -1,    -1,
      -1,   132,    -1,    -1,    -1,   136,    -1,   138,    26,   140,
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,    37,
      -1,    -1,    -1,    -1,    42,    43,    -1,    45,    -1,    47,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,    -1,    -1,    -1,    -1,   113,     3,     4,     5,     6,
       7,     8,    -1,    -1,   122,    12,   124,    -1,   126,   127,
     128,    -1,    -1,    -1,   132,    -1,    -1,    -1,   136,    26,
     138,   139,   140,    -1,    -1,    32,    33,    -1,    35,    -1,
      37,    -1,    -1,    -1,    -1,    42,    43,    -1,    45,    -1,
      47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
     107,   108,    -1,    -1,    -1,    -1,   113,     3,     4,     5,
       6,     7,     8,    -1,    -1,   122,    12,   124,    -1,   126,
     127,   128,    -1,    -1,    -1,   132,    -1,    -1,    -1,   136,
      26,   138,   139,   140,    -1,    -1,    32,    33,    -1,    35,
      -1,    37,    -1,    -1,    -1,    -1,    42,    43,    -1,    45,
      -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    54,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,    -1,    -1,    -1,    -1,   113,     3,     4,
       5,     6,     7,     8,    -1,    -1,   122,    12,   124,    -1,
     126,   127,    -1,    -1,    -1,    20,   132,    -1,    -1,    -1,
     136,    26,   138,    -1,   140,    -1,    31,    32,    33,    -1,
      35,    -1,    37,    -1,    -1,    -1,    -1,    42,    43,    -1,
      45,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,     3,
       4,     5,     6,     7,     8,    -1,    -1,   122,    12,   124,
      -1,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,   136,    26,   138,    -1,   140,    -1,    -1,    32,    33,
      -1,    35,    -1,    37,    -1,    -1,    -1,    -1,    42,    43,
      -1,    45,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,
      -1,    -1,     3,     4,     5,     6,     7,     8,   122,    -1,
     124,    12,   126,   127,   128,    -1,    -1,    -1,   132,    -1,
      -1,    -1,   136,   137,   138,    26,   140,    -1,    -1,    -1,
      -1,    32,    33,    -1,    35,    -1,    37,    -1,    -1,    -1,
      -1,    42,    43,    -1,    45,    -1,    47,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,
      -1,    -1,   113,     3,     4,     5,     6,     7,     8,    -1,
      -1,   122,    12,   124,    -1,   126,   127,   128,    -1,    -1,
      -1,   132,    -1,    -1,    -1,   136,    26,   138,    -1,   140,
      -1,    -1,    32,    33,    -1,    35,    -1,    37,    -1,    -1,
      -1,    -1,    42,    43,    -1,    45,    -1,    47,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    71,    72,    -1,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,
      -1,    -1,    -1,   113,    -1,    -1,   133,   134,   135,    -1,
      -1,   138,   122,   140,   124,    -1,   126,   127,    -1,   129,
      -1,    -1,   132,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    35,    -1,    37,    -1,    -1,    -1,    -1,
      42,    43,    -1,    45,    -1,    47,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,
      -1,   113,     3,     4,     5,     6,     7,     8,    -1,    -1,
     122,    12,   124,    -1,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,   136,    26,   138,    -1,   140,    -1,
      -1,    32,    33,    -1,    35,    -1,    37,    -1,    -1,    -1,
      -1,    42,    43,    -1,    45,    -1,    47,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,
      -1,    -1,   113,     3,     4,     5,     6,     7,     8,    -1,
      -1,   122,    12,   124,    -1,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,   136,    26,   138,    -1,   140,
      -1,    -1,    32,    33,    -1,    35,    -1,    37,    -1,    -1,
      -1,    -1,    42,    43,    -1,    45,    -1,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,
      -1,    -1,    -1,   113,     3,     4,     5,     6,     7,     8,
      -1,    -1,   122,    12,   124,    -1,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,   136,    26,   138,    -1,
     140,    -1,    -1,    32,    33,    -1,    35,    -1,    37,    -1,
      -1,    -1,    -1,    42,    43,    -1,    45,    -1,    47,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,
      -1,    -1,    -1,    -1,   113,     3,     4,     5,     6,     7,
       8,    -1,    -1,   122,    12,   124,    -1,   126,   127,   128,
      -1,    -1,    -1,   132,    -1,    -1,    -1,   136,    26,   138,
      -1,   140,    -1,    -1,    32,    33,    -1,    35,    -1,    37,
      -1,    -1,    -1,    -1,    42,    43,    -1,    45,    -1,    47,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,    -1,    -1,    -1,    -1,   113,     3,     4,     5,     6,
       7,     8,    -1,    -1,   122,    12,   124,    -1,   126,   127,
     128,    -1,    -1,    -1,   132,    -1,    -1,    -1,   136,    26,
     138,    -1,   140,    -1,    -1,    32,    33,    -1,    35,    -1,
      37,    -1,    -1,    -1,    -1,    42,    43,    -1,    45,    -1,
      47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
     107,   108,    -1,    -1,    -1,    -1,   113,     3,     4,     5,
       6,     7,     8,    -1,    -1,   122,    12,   124,    -1,   126,
     127,   128,    -1,    -1,    -1,   132,    -1,    -1,    -1,   136,
      26,   138,    -1,   140,    -1,    -1,    32,    33,    -1,    35,
      -1,    37,    -1,    -1,    -1,    -1,    42,    43,    -1,    45,
      -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,    -1,    -1,    -1,    -1,   113,     3,     4,
       5,     6,     7,     8,    -1,    -1,   122,    12,   124,    -1,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
     136,    26,   138,    -1,   140,    -1,    -1,    32,    33,    -1,
      35,    -1,    37,    -1,    -1,    -1,    -1,    42,    43,    -1,
      45,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,     3,
       4,     5,     6,     7,     8,    -1,    -1,   122,    12,   124,
      -1,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,   136,    26,   138,    -1,   140,    -1,    -1,    32,    33,
      -1,    35,    -1,    37,    -1,    -1,    -1,    -1,    42,    43,
      -1,    45,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,
       3,     4,     5,     6,     7,     8,    -1,    -1,   122,    12,
     124,    -1,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,   136,    26,   138,    -1,   140,    -1,    -1,    32,
      33,    -1,    35,    -1,    37,    -1,    -1,    -1,    -1,    42,
      43,    -1,    45,    -1,    47,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,    -1,
     113,     3,     4,     5,     6,     7,     8,    -1,    -1,   122,
      12,   124,    -1,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,   136,    26,   138,    -1,   140,    -1,    -1,
      32,    33,    -1,    35,    -1,    37,    -1,    -1,    -1,    -1,
      42,    43,    -1,    45,    -1,    47,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,
      -1,   113,     3,     4,     5,     6,     7,     8,    -1,    -1,
     122,    12,   124,    -1,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,   136,    26,   138,    -1,   140,    -1,
      -1,    32,    33,    -1,    35,    -1,    37,    -1,    -1,    -1,
      -1,    42,    43,    -1,    45,    -1,    47,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,
      -1,    -1,   113,     3,     4,     5,     6,     7,     8,    -1,
      -1,   122,    12,   124,    -1,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,   136,    26,   138,    -1,   140,
      -1,    -1,    32,    33,    -1,    35,    -1,    37,    -1,    -1,
      -1,    -1,    42,    43,    -1,    45,    -1,    47,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,
      -1,    -1,    -1,   113,     3,     4,     5,     6,     7,     8,
      -1,    -1,   122,    12,   124,    -1,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,   136,    26,   138,    -1,
     140,    -1,    -1,    32,    33,    -1,    35,    -1,    37,    -1,
      -1,    -1,    -1,    42,    43,    -1,    45,    -1,    47,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,
      -1,    -1,    -1,    -1,   113,     3,     4,     5,     6,     7,
       8,    -1,    -1,   122,    12,   124,    -1,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,   136,    26,   138,
      -1,   140,    -1,    -1,    32,    33,    -1,    35,    -1,    37,
      -1,    -1,    -1,    -1,    42,    43,    -1,    45,    -1,    47,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,    -1,    -1,    -1,    -1,   113,     3,     4,     5,     6,
       7,     8,    -1,    -1,   122,    12,   124,    -1,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,   136,    26,
     138,    -1,   140,    -1,    -1,    32,    33,    -1,    35,    -1,
      37,    -1,    -1,    -1,    -1,    42,    43,    -1,    45,    -1,
      47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
     107,   108,    -1,    -1,    -1,    -1,   113,     3,     4,     5,
       6,     7,     8,    -1,    -1,   122,    12,   124,    -1,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,   136,
      26,   138,    -1,   140,    -1,    -1,    32,    33,    -1,    35,
      -1,    37,    -1,    -1,    -1,    -1,    42,    43,    -1,    45,
      -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,    -1,    -1,    -1,    -1,   113,     3,     4,
       5,     6,     7,     8,    -1,    -1,   122,    12,   124,    -1,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
     136,    26,   138,    -1,   140,    -1,    -1,    32,    33,    -1,
      35,    -1,    37,    -1,    -1,    -1,    -1,    42,    43,    -1,
      45,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
     105,   106,   107,   108,    -1,    32,    33,    -1,    35,    -1,
      37,    -1,    -1,    -1,    -1,    42,    43,   122,    45,   124,
      47,    48,   127,    -1,    -1,    -1,    -1,   132,    55,     3,
      57,   136,    -1,   138,    -1,   140,    10,    -1,    -1,    -1,
      -1,    15,    69,    70,    71,    72,    -1,    -1,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,   106,
      -1,    -1,    -1,   110,    10,    -1,    -1,   114,    -1,    15,
      -1,   133,   134,    -1,    -1,    -1,   138,   124,   140,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,   140,    -1,    -1,    -1,    91,    92,    93,
      94,    95,    96,    -1,    -1,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,    -1,    -1,   111,    -1,    -1,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,    -1,    -1,   130,   131,   132,   133,
      -1,   135,    -1,    -1,   138,    91,    92,    93,    94,    95,
      96,    -1,    -1,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,    24,    -1,   111,    -1,    -1,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,    -1,    10,   130,   131,   132,   133,    15,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    62,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    90,
      -1,    -1,    15,    -1,    -1,    62,    -1,    -1,    -1,    66,
      -1,    24,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,    -1,   120,
     121,   122,   123,    90,   125,    -1,   127,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,   105,    62,
     107,   108,   109,    66,   111,   112,   113,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,   125,   126,
     127,    10,    -1,   130,   131,   132,    15,    90,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    24,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    -1,   125,   126,   127,    10,    -1,   130,   131,   132,
      15,    -1,    -1,    62,    -1,    -1,    -1,    66,   141,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    62,   107,   108,
     109,    66,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    -1,   125,   126,   127,    10,
      -1,   130,   131,   132,    15,    90,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    36,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    -1,
     125,   126,   127,    10,    -1,   130,   131,   132,    15,    -1,
      -1,    62,    -1,    -1,   139,    66,    -1,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    62,   107,   108,   109,    66,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    -1,   125,   126,   127,    -1,    -1,   130,
     131,   132,    10,    90,    -1,   136,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    25,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,   125,   126,
     127,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,   136,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    90,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,   125,   126,   127,
      -1,    62,   130,   131,   132,    66,    -1,    -1,   136,    -1,
      -1,    10,    73,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,    -1,   130,
     131,   132,    -1,    10,    -1,   136,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    62,   125,   126,   127,    66,
      -1,   130,   131,   132,    -1,    10,    -1,   136,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    62,   125,   126,
     127,    66,    -1,   130,   131,   132,    -1,    10,    73,   136,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    62,
     125,   126,   127,    66,    -1,   130,   131,   132,    -1,    10,
      73,   136,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,    -1,   130,   131,   132,
      -1,    10,    73,   136,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,    -1,   130,
     131,   132,    -1,    10,    73,   136,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    62,   125,   126,   127,    66,
      -1,   130,   131,   132,    -1,    10,    73,   136,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    62,   125,   126,
     127,    66,    -1,   130,   131,   132,    -1,    10,    73,   136,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    62,
     125,   126,   127,    66,    -1,   130,   131,   132,    -1,    10,
      73,   136,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,    -1,   130,   131,   132,
      -1,    10,    73,   136,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,    -1,   130,
     131,   132,    -1,    10,    -1,   136,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    62,   125,   126,   127,    66,
      -1,   130,   131,   132,    -1,    -1,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    90,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,   105,    29,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,   125,   126,
     127,    51,    -1,   130,   131,   132,    -1,    -1,   135,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    62,   125,   126,   127,    66,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    86,    -1,
      15,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    29,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    51,   125,   126,   127,
      -1,    -1,   130,   131,   132,    -1,    -1,    62,    -1,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    25,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    62,
     125,   126,   127,    66,    -1,   130,   131,   132,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    25,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,    -1,   130,   131,   132,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,    -1,   130,
     131,   132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    62,   125,   126,   127,    66,
     129,   130,   131,   132,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    62,   125,   126,
     127,    66,   129,   130,   131,   132,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    62,
     125,   126,   127,    66,   129,   130,   131,   132,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,   129,   130,   131,   132,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,   129,   130,
     131,   132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    62,   125,   126,   127,    66,
     129,   130,   131,   132,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    62,   125,   126,
     127,    66,    -1,   130,   131,   132,    -1,    10,    73,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    62,
     125,   126,   127,    66,    -1,   130,   131,   132,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,   129,   130,   131,   132,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,   129,   130,
     131,   132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    62,   125,   126,   127,    66,
     129,   130,   131,   132,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    62,   125,   126,
     127,    66,   129,   130,   131,   132,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    62,
     125,   126,   127,    66,   129,   130,   131,   132,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,   129,   130,   131,   132,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,   129,   130,
     131,   132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    62,   125,   126,   127,    66,
     129,   130,   131,   132,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    62,   125,   126,
     127,    66,   129,   130,   131,   132,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    62,
     125,   126,   127,    66,   129,   130,   131,   132,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,   129,   130,   131,   132,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,   129,   130,
     131,   132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    62,   125,   126,   127,    66,
     129,   130,   131,   132,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    62,   125,   126,
     127,    66,   129,   130,   131,   132,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    62,
     125,   126,   127,    66,   129,   130,   131,   132,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,   129,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    -1,   125,   126,   127,    -1,    62,   130,
     131,   132,    66,    -1,    -1,    -1,    -1,    -1,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      62,   125,   126,   127,    66,   129,   130,   131,   132,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    27,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    62,   125,   126,   127,    66,    -1,   130,   131,
     132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    62,   125,   126,   127,    66,    -1,
     130,   131,   132,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,
      -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    62,   125,   126,   127,
      66,    -1,   130,   131,   132,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    25,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    62,   125,
     126,   127,    66,    -1,   130,   131,   132,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      62,   125,   126,   127,    66,    -1,   130,   131,   132,    -1,
      10,    73,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    62,   125,   126,   127,    66,    -1,   130,   131,
     132,    -1,    -1,    73,    -1,    -1,    -1,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    24,   125,   126,   127,    -1,    62,
     130,   131,   132,    66,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,    -1,   130,   131,   132,
      -1,    10,    73,    -1,    -1,   105,    15,   107,   108,   109,
      -1,   111,   112,   113,    -1,    24,   116,   117,   118,    90,
     120,   121,   122,   123,    -1,    -1,    -1,   127,    -1,    -1,
     130,   131,   132,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,    -1,   130,
     131,   132,    -1,    10,    73,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    62,   125,   126,   127,    66,
      -1,   130,   131,   132,    -1,    10,    73,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    62,   125,   126,
     127,    66,    -1,   130,   131,   132,    -1,    10,    73,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    62,
     125,   126,   127,    66,    -1,   130,   131,   132,    -1,    10,
      73,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,    -1,   130,   131,   132,
      -1,    10,    73,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,    -1,   130,
     131,   132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    62,   125,   126,   127,    66,
      -1,   130,   131,   132,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    62,   111,   112,   113,    66,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,   125,   126,
     127,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
      -1,   120,   121,   122,   123,    -1,   125,    -1,   127,    -1,
      -1,   130,   131,   132
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   146,   147,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    17,    18,    19,    20,    22,    23,
      25,    26,    28,    30,    31,    32,    33,    34,    35,    37,
      38,    41,    42,    43,    44,    45,    47,    48,    50,    55,
      56,    57,    58,    60,    61,    64,    65,    67,    68,    69,
      70,    71,    72,    74,    76,    77,    78,    80,    81,    85,
      87,    90,   105,   106,   107,   108,   113,   122,   124,   126,
     127,   129,   132,   136,   138,   140,   148,   149,   150,   151,
     152,   153,   158,   159,   160,   162,   165,   166,   167,   168,
     169,   170,   172,   173,   177,   178,   181,   182,   185,   188,
     191,   192,   215,   218,   219,   237,   238,   239,   240,   241,
     242,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   260,   261,   262,   263,   264,   150,   252,    86,   243,
     244,   161,   162,   243,    12,    32,    33,    35,    72,   136,
     140,   188,   237,   241,   250,   251,   252,   253,   255,   256,
      78,   161,   252,   150,   138,   162,     7,   161,   163,     9,
      78,   163,    54,    88,   171,   252,   252,    20,    31,   218,
     252,   252,   138,   162,   189,   138,   162,   220,   221,   150,
     252,   252,   252,   252,     7,   138,    31,   167,   167,   128,
     211,   231,   252,   129,   252,   252,    25,   153,   164,   252,
     252,    71,   138,   150,   252,   252,   153,   160,   252,   253,
     153,   160,   252,   162,   216,   231,   252,   252,   252,   252,
     252,   252,   252,   252,   137,   148,   154,   231,    79,   114,
     211,   232,   233,   252,   231,   252,   259,    56,   150,    46,
     162,    40,    59,   206,    21,    54,    63,    82,   129,   135,
      10,    15,    24,    62,    66,    90,   105,   107,   108,   109,
     111,   112,   113,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   125,   126,   127,   130,   131,   132,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   133,   134,   138,   140,    62,    66,   138,   150,
     129,   136,   153,   252,   252,   252,   231,    36,   243,   216,
     129,   129,    85,   162,   211,   234,   235,   236,   252,   136,
     216,   182,   162,   138,   164,    25,    36,   164,    25,    36,
      86,   164,   246,    29,    51,   129,    73,   153,   234,   150,
     138,   200,    79,   138,   162,   222,   223,     1,   109,   225,
     226,    36,   110,   164,   234,   163,   162,   110,   129,   129,
     136,   150,   164,   138,   234,   129,   174,   129,   174,   129,
      91,   217,   129,   129,    29,    51,   164,   129,   137,   148,
     110,   137,   252,   110,   139,   110,   139,    36,   110,   141,
     246,    89,   110,   141,     7,   162,   109,   183,   193,    63,
     220,   220,   220,   220,   252,   252,   252,   252,   171,   252,
     171,   252,   252,   252,   252,   252,   252,   252,    26,    78,
     162,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   234,   234,   171,   252,   171,   252,    21,
      36,    39,    53,    54,    63,    78,   204,   245,   248,   252,
     264,    25,    36,    25,    36,    73,    36,   141,   171,   252,
     164,   129,   252,    89,    91,   139,   110,   162,   186,   187,
     129,   162,    36,   231,    35,   252,   171,   252,    35,   252,
     171,   252,   138,   164,   132,   155,   157,   252,   155,   156,
     150,   252,    27,   139,   201,   202,   203,   204,   190,   223,
     110,   139,     1,   140,   227,   238,    89,    91,   224,   252,
     221,   139,   211,   252,   179,   234,   139,    16,   175,   140,
     227,   238,   110,   157,   156,   137,   139,   139,   232,   139,
     232,   171,   252,   141,   150,   252,   141,   252,   141,   252,
     136,   231,   136,    21,    54,    63,    78,   195,   205,   220,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     139,   141,    36,    63,   237,   110,   139,    62,    62,    35,
     171,   252,    35,   171,   252,   171,   252,   243,   243,   129,
     252,   211,   252,   236,    91,   110,   137,   183,   252,   139,
     252,    25,   164,    25,   164,   252,    25,   164,   246,    25,
     164,   246,   247,   248,   110,   129,    11,   129,    27,    27,
     150,   110,   139,   138,   162,    21,    54,    63,    78,   207,
     139,   223,   109,   226,   231,   252,    49,   252,    52,    83,
     137,   180,   139,   138,   162,   176,   231,   216,   129,   129,
     141,   246,   141,   246,   150,    89,   137,   154,   184,    63,
     194,    10,    15,    91,    92,    93,    94,    95,    96,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   111,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   127,   130,   131,   132,   133,   135,   138,   162,   196,
     197,   198,   237,   129,   248,   237,   237,   252,    25,    25,
     252,    25,    25,   141,   141,   164,   164,   252,   187,   136,
     164,    73,    35,   252,    35,   252,    73,    35,   252,   164,
      35,   252,   164,   110,   139,   252,   252,   150,   252,   202,
     223,   109,   230,    63,   226,   224,    36,   141,    25,   150,
     231,   176,   109,   153,   141,    35,   150,   252,   141,    35,
     150,   252,   141,   252,   137,    31,    58,    60,   137,   149,
     166,   207,   162,   252,   112,   138,   199,   199,    73,    35,
      35,    73,    35,    35,    35,    35,   184,   252,   252,   252,
     252,   252,   252,   248,    11,   139,    26,    69,    72,   140,
     211,   229,   238,   114,   213,   224,    84,   214,   252,   227,
     238,   150,   164,   139,   252,   252,   150,   252,   150,   166,
     226,   139,   162,   197,   198,   201,   252,   252,   252,   252,
     252,   252,   137,    73,    73,    73,    73,   252,   230,   141,
     231,   211,   212,   252,   252,   153,   165,   210,   141,   153,
      73,    73,    75,   208,   199,   199,   139,    73,    73,    73,
      73,    73,    73,   252,   252,   252,   252,   213,   224,   211,
     228,   229,   238,    36,   141,   238,   252,   252,   214,   252,
     228,   229,   129,   209,   210,   141,   228
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   145,   146,   147,   147,   148,   148,   149,   149,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   151,   151,   152,
     152,   152,   153,   153,   154,   154,   155,   155,   155,   155,
     156,   156,   157,   157,   158,   158,   158,   159,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   161,   161,   162,   163,   163,   164,
     164,   165,   165,   166,   166,   166,   166,   166,   166,   166,
     166,   167,   167,   168,   168,   168,   168,   169,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     171,   172,   172,   172,   172,   173,   173,   173,   173,   173,
     173,   174,   174,   175,   175,   176,   176,   176,   177,   178,
     179,   179,   180,   180,   180,   181,   181,   182,   182,   182,
     183,   183,   184,   184,   184,   185,   186,   186,   186,   187,
     187,   189,   190,   188,   191,   191,   191,   191,   193,   194,
     192,   195,   195,   195,   195,   196,   196,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   199,   199,   200,   201,   201,
     201,   202,   202,   202,   202,   203,   203,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   205,   205,   205,   205,
     205,   205,   206,   206,   207,   207,   207,   207,   207,   207,
     208,   208,   209,   209,   210,   210,   211,   211,   212,   212,
     213,   213,   214,   214,   215,   215,   215,   216,   216,   217,
     217,   217,   218,   218,   218,   218,   218,   219,   219,   219,
     220,   220,   221,   221,   221,   222,   222,   222,   223,   223,
     223,   224,   224,   224,   225,   225,   226,   226,   226,   226,
     227,   227,   227,   227,   228,   228,   228,   229,   229,   229,
     229,   229,   230,   230,   230,   230,   230,   230,   230,   231,
     231,   231,   231,   232,   232,   232,   233,   233,   234,   234,
     235,   235,   236,   236,   236,   236,   236,   237,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   240,   241,   242,
     242,   242,   242,   242,   242,   242,   242,   243,   243,   244,
     245,   245,   246,   247,   247,   248,   248,   248,   249,   249,
     250,   251,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   253,   253,   253,   253,
     254,   254,   255,   255,   255,   256,   256,   256,   257,   257,
     257,   257,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   259,   259,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   261,   261,
     261,   261,   261,   261,   262,   262,   262,   262,   263,   263,
     263,   263,   264,   264,   264,   264,   264,   264,   264
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     3,     3,     3,     3,     3,
       2,     3,     3,     2,     2,     3,     1,     5,     6,     0,
       1,     1,     2,     3,     1,     2,     1,     3,     3,     5,
       0,     1,     1,     1,     3,     5,     5,     3,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     0,     1,     1,     1,     1,     2,
       1,     2,     3,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     3,     5,     5,     2,     2,     5,     3,
       6,     6,     4,     5,     5,     3,     3,     6,     5,     6,
       5,     6,     3,     4,     6,     7,     6,     7,     4,     5,
       4,     4,     3,     6,     5,     3,     3,     2,     2,     3,
       3,     0,     2,     3,     5,     0,     1,     3,     3,     5,
       0,     2,     3,     2,     3,     6,     8,     1,     1,     1,
       0,     2,     0,     2,     3,     5,     1,     2,     3,     1,
       3,     0,     0,     8,     0,     1,     2,     2,     0,     0,
      10,     3,     3,     5,     5,     1,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     3,     3,     0,     1,
       3,     4,     4,     6,     6,     0,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     2,     1,     0,     1,
       2,     2,     0,     2,     3,     4,     4,     2,     4,     0,
       2,     2,     4,     4,     4,     5,     4,     0,     1,     1,
       1,     3,     3,     4,     5,     1,     1,     3,     1,     2,
       3,     0,     2,     2,     0,     4,     0,     2,     2,     1,
       4,     4,     6,     1,     0,     1,     1,     3,     4,     3,
       4,     6,     0,     2,     2,     2,     2,     2,     2,     1,
       1,     3,     3,     1,     1,     1,     3,     3,     0,     1,
       1,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     4,     4,     4,     5,     2,     2,
       6,     6,     4,     9,     9,     7,     6,     6,     4,     9,
       9,     7,     4,     6,     6,     9,     9,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     1,     4,
       1,     3,     4,     1,     3,     2,     3,     3,     1,     3,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     3,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     4,     3,     3,     3,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     3,     4,     3,
       4,     3,     4,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1
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
#line 434 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 4844 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 439 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4850 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 440 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4856 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 447 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4862 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 452 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4868 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 453 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4874 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4880 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 472 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4886 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 473 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4892 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 474 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4898 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 475 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4904 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4910 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 477 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4916 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4922 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4928 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 480 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4940 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4946 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 483 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4952 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4958 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 485 "chapel.ypp" /* yacc.c:1661  */
    { printf("syntax error"); clean_exit(1); }
#line 4964 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 490 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yyvsp[-4].b), (yylsp[-4]).comment))); }
#line 4970 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-5].b), (yylsp[-5]).comment))); }
#line 4976 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4982 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4988 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4994 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 513 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5000 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5006 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5012 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 520 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5018 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 525 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5028 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5038 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 535 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5047 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 539 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5056 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 545 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5066 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5076 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5082 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5088 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5094 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5100 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 5106 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 5112 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 5118 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 5124 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 5130 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 5136 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 5142 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 580 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 5148 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 5154 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 582 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 5160 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 583 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 5166 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 584 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 5172 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 585 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5178 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5184 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5190 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5196 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5202 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 597 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5208 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 601 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5214 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5220 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5226 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5232 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 611 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 5238 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5244 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5250 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5256 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 619 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5262 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 5268 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5274 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5280 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 5286 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 640 "chapel.ypp" /* yacc.c:1661  */
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
#line 5302 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 654 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5308 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 655 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5314 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5320 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 657 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5326 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 658 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5332 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 659 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5338 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 660 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5344 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 661 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5350 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 662 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5356 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 663 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5362 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 664 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5368 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 665 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt)); }
#line 5374 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 666 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), zipToTuple((yyvsp[-1].pcallexpr)), NULL, (yyvsp[0].pblockstmt), true); }
#line 5380 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 667 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt), true); }
#line 5386 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 668 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5392 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 669 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt)); }
#line 5398 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 671 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 677 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5418 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 683 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5428 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 689 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-3].pcallexpr)), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 695 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5449 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 702 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5460 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 711 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5466 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 715 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5472 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 716 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5478 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 717 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5484 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 718 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5490 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 722 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 5496 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 723 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 5502 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 724 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 5508 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 725 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 5514 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 726 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5520 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 727 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5526 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 731 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5532 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 732 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5538 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 736 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5544 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 737 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5550 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 743 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(newTemp(dtVoid)); }
#line 5556 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 744 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 5562 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 745 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 5568 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 749 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 5574 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 753 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5580 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 757 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5586 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 758 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5592 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 763 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5598 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 765 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5604 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 767 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5610 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 774 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), NULL, (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5619 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 779 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-6].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5628 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 786 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5638 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 791 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 796 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 804 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 5664 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 805 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 5670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 810 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5676 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 812 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 5682 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 814 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 5688 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 819 "chapel.ypp" /* yacc.c:1661  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 5699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 829 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[0].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[0].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
#line 5712 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 838 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 5720 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 842 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 5729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 849 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 5735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 850 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 5741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 855 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 5750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 860 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 865 "chapel.ypp" /* yacc.c:1661  */
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
#line 5779 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 885 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5790 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 891 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5802 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 898 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5815 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 906 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 917 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 5838 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 923 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5849 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 930 "chapel.ypp" /* yacc.c:1661  */
    {
      FnSymbol* fn = (yyvsp[-6].pfnsymbol);

      fn->copyFlags((yyvsp[-9].pfnsymbol));
      // The user explicitly named this function (controls mangling).
      if (*(yyvsp[-9].pfnsymbol)->name)
        fn->cname = (yyvsp[-9].pfnsymbol)->name;

      if ((yyvsp[-8].procIter) == ProcIter_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[-6].pfnsymbol), (yyvsp[-4].retTag), (yyvsp[-3].pexpr), (yyvsp[-2].b), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), (yylsp[-9]).comment);
      context->latestComment = NULL;
    }
#line 5872 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 952 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5880 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 956 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5889 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 961 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 5897 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 965 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5906 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 973 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 5912 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 978 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5918 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 979 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 5924 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 980 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 5930 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 981 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 5936 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 982 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 5942 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 983 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 5948 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 984 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 5954 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 985 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 5960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 986 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 5966 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 987 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 5972 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 988 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 5978 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 989 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 5984 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 990 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 5990 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 991 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 5996 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 992 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 6002 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 993 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 6008 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 994 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 6014 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 995 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 6020 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 996 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 6026 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 997 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 6032 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 998 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 6038 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 999 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 6044 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1000 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 6050 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1001 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 6056 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1002 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 6062 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1006 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 6068 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1007 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 6074 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1008 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 6080 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1009 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 6086 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1010 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 6092 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1011 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 6098 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1012 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 6104 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1013 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 6110 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1014 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 6116 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1015 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 6122 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1016 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 6128 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1017 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 6134 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1021 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 6140 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1022 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6146 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1026 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6152 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1030 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 6158 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1031 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 6164 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1032 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 6170 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1037 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 6176 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1039 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 6182 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1041 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6188 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1043 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6194 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1047 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6200 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1048 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6206 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1052 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 6212 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1053 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6218 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1054 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 6224 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1055 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 6230 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1056 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6236 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1057 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6242 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1058 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6248 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1059 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 6254 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1060 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6260 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1064 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6266 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1065 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1066 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 6278 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1067 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 6284 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1068 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST;   }
#line 6290 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1069 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6296 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1073 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6302 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1074 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6308 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1078 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6314 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1079 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6320 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1080 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6326 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1081 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 6332 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1082 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 6338 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1083 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 6344 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1087 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 6350 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1088 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 6356 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1091 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 6362 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1097 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 6368 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1102 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6374 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1104 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6380 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1108 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6386 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1113 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6392 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1114 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6398 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1118 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6404 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1119 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6410 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1124 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 6416 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1126 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 6422 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1128 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 6428 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1133 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 6445 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1146 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 6463 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1162 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6469 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1164 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6475 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1166 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 6481 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1171 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6493 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1179 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6505 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1187 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6517 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1195 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 6530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1204 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6541 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1213 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6547 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1214 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6553 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1215 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6559 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6568 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1229 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6574 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1231 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 6584 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1237 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6590 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1242 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 6596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1244 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6602 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1246 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 6608 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1251 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 6614 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1253 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6620 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1255 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6626 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1261 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6632 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1262 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 6638 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1263 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6644 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1267 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6650 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1269 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6656 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1273 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6662 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1274 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6668 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1275 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 6674 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1276 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad type specification"); }
#line 6680 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1297 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 6688 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1301 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 6696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1305 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 6708 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1312 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad array type specification"); clean_exit(1); }
#line 6714 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1316 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6720 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1317 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6726 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1318 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6732 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1323 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6738 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1325 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6744 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1331 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1333 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6756 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1335 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 6762 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1339 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6768 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1340 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6774 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1341 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6780 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1342 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 6786 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1343 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1344 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6798 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1345 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1351 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6810 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1352 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1353 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1354 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6834 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1359 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6840 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1360 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1364 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6852 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1365 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1369 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1374 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6870 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1375 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6876 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1379 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6882 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1380 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6888 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1381 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6894 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1382 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6900 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1387 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6906 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1404 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6912 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1406 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6918 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1408 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6924 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1410 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6930 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1412 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 6936 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1414 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6942 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1416 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6948 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1421 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6954 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1423 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 6960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1425 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6966 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1427 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6972 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1429 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6978 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1431 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6984 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1433 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6990 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1435 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 6996 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1437 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7002 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1439 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7008 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1441 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7014 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1443 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7020 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1445 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 7031 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1452 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 7041 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1458 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 7051 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1464 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7061 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1470 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 7071 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 7077 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1483 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 7083 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7089 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 7095 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7101 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7108 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1517 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7114 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pForallIntents) = (yyvsp[-1].pForallIntents); }
#line 7120 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1527 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pForallIntents) = new ForallIntents();
      addForallIntent((yyval.pForallIntents), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7127 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1530 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pForallIntents), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7133 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1535 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[-1].pt); (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
#line 7141 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 386:
#line 1538 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).riExp = (yyvsp[-2].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); }
#line 7147 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 387:
#line 1539 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).riExp = (yyvsp[-2].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); }
#line 7153 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1545 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7159 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1550 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 7165 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 7171 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 7177 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7183 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7189 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 7195 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 7201 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 7207 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7213 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1605 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7219 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 7225 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7231 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1611 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 7237 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 7243 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7249 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 7255 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 7261 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 7267 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 7273 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1628 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 7279 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 7285 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1630 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 7291 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 7297 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 7303 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 7309 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 7315 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 7321 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1637 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 7329 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1641 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 7337 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1648 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7343 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1649 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7349 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1653 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7355 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1654 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7361 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1655 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7367 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7373 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1657 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7379 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1658 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7385 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1659 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7391 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1660 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7397 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1661 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7403 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1662 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7409 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1663 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7415 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1664 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7421 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1665 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7427 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1666 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7433 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1667 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7439 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1668 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7445 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1669 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7451 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1670 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7457 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 453:
#line 1671 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7463 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 454:
#line 1672 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7469 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 455:
#line 1673 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7475 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 456:
#line 1674 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7481 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 457:
#line 1675 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7487 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 458:
#line 1679 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 7493 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 459:
#line 1680 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 7499 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 460:
#line 1681 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 7505 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 461:
#line 1682 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 7511 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 462:
#line 1683 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 7517 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 463:
#line 1684 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 7523 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 464:
#line 1688 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7529 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 465:
#line 1689 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7535 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 466:
#line 1690 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7541 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 467:
#line 1691 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7547 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 468:
#line 1695 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7553 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 469:
#line 1696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7559 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 470:
#line 1697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7565 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 471:
#line 1698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7571 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 472:
#line 1703 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 7577 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 473:
#line 1704 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 7583 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 474:
#line 1705 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 7589 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 475:
#line 1706 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 7595 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 476:
#line 1707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 7601 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 477:
#line 1708 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 7607 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 478:
#line 1709 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 7613 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 7617 "bison-chapel.cpp" /* yacc.c:1661  */
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
