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
#line 134 "chapel.ypp" /* yacc.c:355  */

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
#line 156 "chapel.ypp" /* yacc.c:355  */

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

#line 226 "bison-chapel.cpp" /* yacc.c:355  */

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
    TTRY = 327,
    TTYPE = 328,
    TUNDERSCORE = 329,
    TUNION = 330,
    TUSE = 331,
    TVAR = 332,
    TWHEN = 333,
    TWHERE = 334,
    TWHILE = 335,
    TWITH = 336,
    TYIELD = 337,
    TZIP = 338,
    TALIAS = 339,
    TAND = 340,
    TASSIGN = 341,
    TASSIGNBAND = 342,
    TASSIGNBOR = 343,
    TASSIGNBXOR = 344,
    TASSIGNDIVIDE = 345,
    TASSIGNEXP = 346,
    TASSIGNLAND = 347,
    TASSIGNLOR = 348,
    TASSIGNMINUS = 349,
    TASSIGNMOD = 350,
    TASSIGNMULTIPLY = 351,
    TASSIGNPLUS = 352,
    TASSIGNSL = 353,
    TASSIGNSR = 354,
    TBAND = 355,
    TBNOT = 356,
    TBOR = 357,
    TBXOR = 358,
    TCOLON = 359,
    TCOMMA = 360,
    TDIVIDE = 361,
    TDOT = 362,
    TDOTDOT = 363,
    TDOTDOTDOT = 364,
    TEQUAL = 365,
    TEXP = 366,
    TGREATER = 367,
    TGREATEREQUAL = 368,
    THASH = 369,
    TLESS = 370,
    TLESSEQUAL = 371,
    TMINUS = 372,
    TMOD = 373,
    TNOT = 374,
    TNOTEQUAL = 375,
    TOR = 376,
    TPLUS = 377,
    TQUESTION = 378,
    TSEMI = 379,
    TSHIFTLEFT = 380,
    TSHIFTRIGHT = 381,
    TSTAR = 382,
    TSWAP = 383,
    TASSIGNREDUCE = 384,
    TIO = 385,
    TLCBR = 386,
    TRCBR = 387,
    TLP = 388,
    TRP = 389,
    TLSBR = 390,
    TRSBR = 391,
    TNOELSE = 392,
    TUPLUS = 393,
    TUMINUS = 394
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
#line 187 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 411 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 417 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 38 "chapel.ypp" /* yacc.c:359  */

  int         captureTokens;
  char        captureString[1024];
#line 195 "chapel.ypp" /* yacc.c:359  */

  #include "build.h"
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

#line 484 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   12927

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  140
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  113
/* YYNRULES -- Number of rules.  */
#define YYNRULES  453
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  860

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   394

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
     135,   136,   137,   138,   139
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   423,   423,   428,   429,   435,   436,   441,   442,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   477,   479,   484,   485,
     486,   501,   502,   507,   508,   513,   518,   523,   527,   533,
     538,   542,   547,   551,   552,   553,   557,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   573,
     574,   575,   576,   580,   581,   585,   589,   590,   594,   595,
     599,   600,   604,   605,   606,   607,   608,   609,   610,   614,
     615,   619,   634,   635,   636,   637,   638,   639,   640,   641,
     642,   643,   644,   645,   646,   647,   648,   649,   650,   656,
     662,   668,   674,   681,   691,   695,   696,   697,   698,   702,
     706,   711,   712,   716,   718,   720,   727,   732,   740,   745,
     750,   758,   759,   764,   765,   767,   772,   782,   791,   795,
     803,   804,   809,   814,   808,   839,   845,   852,   860,   871,
     877,   870,   905,   909,   914,   918,   926,   927,   928,   929,
     930,   931,   932,   933,   934,   935,   936,   937,   938,   939,
     940,   941,   942,   943,   944,   945,   946,   947,   948,   949,
     950,   951,   955,   956,   957,   958,   959,   960,   961,   962,
     963,   964,   965,   966,   970,   971,   975,   979,   980,   981,
     985,   987,   989,   991,   996,   997,  1001,  1002,  1003,  1004,
    1005,  1006,  1007,  1008,  1009,  1013,  1014,  1015,  1016,  1020,
    1021,  1025,  1026,  1027,  1028,  1029,  1030,  1034,  1035,  1039,
    1040,  1044,  1046,  1051,  1052,  1056,  1057,  1061,  1062,  1066,
    1068,  1070,  1075,  1088,  1105,  1106,  1108,  1113,  1121,  1129,
    1137,  1146,  1156,  1157,  1158,  1162,  1163,  1171,  1173,  1179,
    1184,  1186,  1188,  1193,  1195,  1197,  1204,  1205,  1206,  1210,
    1211,  1216,  1217,  1218,  1219,  1239,  1243,  1247,  1255,  1259,
    1260,  1261,  1265,  1267,  1273,  1275,  1277,  1282,  1283,  1284,
    1285,  1286,  1287,  1288,  1294,  1295,  1296,  1297,  1301,  1302,
    1303,  1307,  1308,  1312,  1313,  1317,  1318,  1322,  1323,  1324,
    1325,  1326,  1330,  1341,  1342,  1343,  1344,  1345,  1346,  1348,
    1350,  1352,  1354,  1356,  1358,  1363,  1365,  1367,  1369,  1371,
    1373,  1375,  1377,  1379,  1381,  1383,  1385,  1387,  1394,  1400,
    1406,  1412,  1421,  1426,  1434,  1435,  1436,  1437,  1438,  1439,
    1440,  1441,  1446,  1447,  1451,  1456,  1459,  1464,  1469,  1472,
    1477,  1481,  1482,  1486,  1487,  1492,  1497,  1505,  1506,  1507,
    1508,  1509,  1510,  1511,  1512,  1513,  1515,  1517,  1519,  1521,
    1523,  1528,  1529,  1530,  1531,  1542,  1543,  1547,  1548,  1549,
    1553,  1554,  1555,  1563,  1564,  1565,  1566,  1570,  1571,  1572,
    1573,  1574,  1575,  1576,  1577,  1578,  1579,  1583,  1591,  1592,
    1596,  1597,  1598,  1599,  1600,  1601,  1602,  1603,  1604,  1605,
    1606,  1607,  1608,  1609,  1610,  1611,  1612,  1613,  1614,  1615,
    1616,  1617,  1618,  1622,  1623,  1624,  1625,  1626,  1627,  1631,
    1632,  1633,  1634,  1638,  1639,  1640,  1641,  1646,  1647,  1648,
    1649,  1650,  1651,  1652
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
  "TSERIAL", "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTRY",
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
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "try_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
  "class_decl_stmt", "class_tag", "opt_inherit", "class_level_stmt_ls",
  "enum_decl_stmt", "enum_ls", "enum_item", "lambda_decl_expr", "$@1",
  "$@2", "linkage_spec", "fn_decl_stmt", "$@3", "$@4",
  "fn_decl_stmt_inner", "fn_ident", "assignop_ident", "opt_formal_ls",
  "req_formal_ls", "formal_ls", "formal", "opt_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_or_iter",
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
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394
};
# endif

#define YYPACT_NINF -757

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-757)))

#define YYTABLE_NINF -405

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -757,    69,  2515,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  3609,    -5,   132,  -757,    -5,  7602,    16,   132,  7602,
    3609,    17,   132,   202,   706,  6272,  7602,  7602,    23,  -757,
     132,  -757,    38,  3609,  7602,  7602,  -757,  7602,  7602,   144,
      28,   824,  1027,  -757,  6538,  6671,  7602,  6804,  7602,    97,
      41,  3609,    75,   132,  -757,  6538,  7602,  7602,  -757,  -757,
    7602,  -757,  -757,  8932,  7602,  7602,  -757,  7602,  -757,  -757,
    2811,  5873,  6538,  -757,  3476,  -757,  -757,   163,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,   132,  -757,   297,   129,  -757,  -757,  -757,   271,
      90,  -757,  -757,  -757,   166,   172,    91,   206,   207, 12623,
    5190,   194,   212,   221,  -757,  -757,  -757,  -757,  -757,  -757,
     155,  -757, 12623,   233,  3609,  -757,   228,  -757,    75,  7602,
    7602,  7602,  7602,  7602,  6538,  6538,   248,  -757,  -757,  -757,
    -757, 10145,   279,  -757,  -757,   132,   250, 10431,   309,  6538,
     289,  -757,  -757,  -757,  -757,   132,    39,   132,   292,    33,
    9260,  9201,  9378,  6538,  3609,   301,    14,   112,    29,  -757,
    -757,   399,   319,  9437,   399,  -757,  6538,  -757,   281,  -757,
    -757,   132,  -757,    84, 12623,  -757, 10490, 10027,  3609,  -757,
    -757,  9437, 12623,   321,  6538,  -757, 12623,  2944,  -757,   370,
     338,   190, 10254,  9437, 10549,   275,  2502,   399,   275,   399,
    -757,  -757,  3077,   -31,  -757,  7602,  -757,   -25,    10, 12623,
      49, 10608,   -32,   450,  -757,   132,   361,  -757,  -757,  -757,
      36,    38,    38,    38,  -757,  7602,  7602,  7602,  7602,  6937,
    6937,  7602,  7602,  7602,  7602,  7602,  7602,    68,  8932,  7602,
    7602,  7602,  7602,  7602,  7602,  7602,  7602,  7602,  7602,  7602,
    7602,  7602,  7602,  7602,  7602,  7602,  7602,  7602,  7602,  7602,
    7602,  7602,  7602,  7602,  7602,  7602,  7602,  7602,  7602,  7602,
    6538,  6538,  6937,  6937,  5740,  -757,  -757,  -757, 10313, 10372,
   10667,    48,  6937,    33,   342,  -757,  -757,  7602,   349,  -757,
     337,   373,  -757, 12623,   132,   351,   132,   445,  6538,  -757,
    3742,  6937,  -757,  3875,  6937,   347,  -757,    33,  3609,   456,
     354,  -757,    44,  -757,  -757,    14,  -757,   379,   356,  -757,
    2275,   401,   403,  7602,    38,  -757,   358,  -757,  -757,  6538,
    -757,  -757,  -757,  -757,  -757,  6538,   360,  5205,   386,  -757,
    -757,  7735,  7602,  -757,  -757,  -757,  -757,  6405,  -757,  9137,
    6006,  -757,  6139,  -757,  6937,  5338,  2678,   362,  7602,  5606,
    -757,  -757,   365,  6538,   368,   333,    38,   219,   267,   293,
     294, 10086,  9073,  9073,   363,  -757,   363,  -757,   363,  2309,
     650,  1488,  1561,   319,   275,  -757,  -757,  -757,  2502, 12800,
     363,  2023,  2023,  9073,  2023,  2023,   785,   275, 12800, 12741,
     785,   399,   399,   275, 10726, 10785, 10844, 10903, 10962, 11021,
   11080, 11139, 11198, 11257, 11316, 11375, 11434, 11493, 11552, 11611,
     366,   371,  -757,   363,  -757,   363,   276,  -757,  -757,  -757,
    -757,  -757,  -757,   132,    86,  -757, 12682,   340,  7868,  6937,
    8001,  6937,  7602,  6937,  9086,    -5, 11670,  -757,  -757, 11729,
    7602,  6538,  -757,  6538,   422,   239,  -757,  -757,   361,  7602,
      88,  7602, 12623,    51,  9496,  7602, 12623,    18,  9319,  5740,
    -757,   485, 11797,  3609,  -757,    89,  -757,    45,  -757,   355,
     378,    14,    76,  -757,  6538,  -757,   711,  7602,  7070,  -757,
   12623,  -757,  -757,  -757, 12623,     8,   382,  -757,  6538,  -757,
     300,   132,   389,   414,   396, 11856,   414,   398,  -757,  -757,
    -757,  -757,  -757,  -757,     3,   914,  -757,  -757, 12623,  3609,
   12623,  -757, 11915,  3210,   418,  -757,  -757,  -757,  -757,  -757,
    2099,   305,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  5740,  -757,  6937,
    6937,  7602,   501, 11974,  7602,   504, 12033,   395,  1217,    33,
      33,  -757, 12623,  -757, 12623,  -757,  7602,   132,  -757,   405,
    9437,  -757,  9555,  4008,  -757,  4141,  -757,  9614,  4274,  -757,
      33,  4407,  -757,    33,   175,  -757,  3609,  7602,  -757,   350,
    -757,    14,   430,   479,  -757,  -757,  -757,    66,  -757,  -757,
    5205,   403,    74, 12623,  -757, 12623,  4540,  6538,  -757,  -757,
    -757,    82,  -757,  7602,  -757,  7602,  -757,  4673,   407,  4806,
     411,  -757,  7602,  -757,  3343,   508,   355,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,   132,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,   434,   415,   415,  -757,  -757,   177,   178,
   12092,  8134,  8267, 12151,  8400,  8533,  8666,  8799,  -757,  -757,
   12623,  -757,  -757,  -757,  3609,  7602, 12623,  7602, 12623,  3609,
    7602, 12623,  -757,  7602, 12623,  -757,  5740,  -757,  -757, 12623,
    -757,   416,  7203,   -18,  -757,   470,  -757,  6937,  4939,  3609,
    -757,    26,  5072, 12210, 12623,  7602,  -757, 12623,  3609,  7602,
    -757, 12623,  3609, 12623,  -757,   441,   331,   331,  -757,   579,
    -757,    24,  -757,  2099,    44,  -757,  -757,  7602,  7602,  7602,
    7602,  7602,  7602,  7602,  7602,  1085, 11797,  9673,  9732, 11797,
    9791,  9850,  -757,   430,    17,  7602,  7602,  5472,  -757,  -757,
     199,  6538,  -757,  -757,  7602,     2,  1787,  -757,   771,   309,
    -757,  7602,  9909,  -757,  9968,  -757,  -757,   470,  -757,   415,
     415,   184, 12269, 12328, 12387, 12446, 12505, 12564,  -757,  3609,
    3609,  3609,  3609,   -18,  7336,   169,  -757,  -757, 12623, 12623,
    -757,  -757,  -757,  8799, 12623,  3609,  3609,    64,  -757,  -757,
    -757,  7602,  7602,  7602,  7602,  7602,  7602, 11797, 11797, 11797,
   11797,  -757,  -757,  -757,  -757,  -757,   208,  6937,  9015,   796,
   11797, 11797,  -757,  -757,  -757,  2343,  -757,  -757,  7469,  -757
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    35,    75,   397,   398,   399,   400,
     401,     0,   352,    73,   128,   352,     0,   253,    73,     0,
       0,     0,     0,    73,    73,     0,     0,     0,     0,   146,
       0,   142,     0,     0,     0,     0,   343,     0,     0,     0,
       0,   252,   252,   129,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   130,     0,     0,     0,   449,   451,
       0,   452,   453,   380,     0,     0,   450,   447,    82,   448,
       0,     0,     0,     4,     0,     5,     9,     0,    10,    11,
      12,    14,   312,    20,    13,    83,    15,    17,    16,    19,
      18,    88,     0,    86,   372,     0,    89,    87,    90,     0,
     381,   368,   369,   315,   313,     0,     0,   373,   374,     0,
     314,     0,   382,   383,   384,   367,   317,   316,   370,   371,
       0,    22,   323,     0,     0,   353,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,   372,   381,   313,   373,
     374,   352,   314,   382,   383,     0,     0,     0,     0,   303,
       0,    77,    76,   147,    91,     0,   148,     0,     0,     0,
       0,     0,     0,   303,     0,     0,     0,     0,     0,   255,
      29,   435,   365,     0,   436,     7,   303,   253,   254,    85,
      84,   232,   295,     0,   294,    80,     0,     0,     0,    79,
      32,     0,   318,     0,   303,    33,   324,     0,   119,   244,
       0,     0,   294,     0,     0,   438,   379,   434,   437,   433,
      41,    43,     0,     0,   298,     0,   300,     0,     0,   299,
       0,   294,     0,     0,     6,     0,   131,   220,   219,   149,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     303,   303,     0,     0,     0,    23,    24,    25,     0,     0,
       0,     0,     0,     0,     0,    26,    27,     0,   312,   310,
       0,   304,   305,   311,     0,     0,     0,     0,     0,   100,
       0,     0,    99,     0,     0,     0,   106,     0,     0,   116,
       0,    28,   197,   143,   260,     0,   261,   263,     0,   274,
       0,     0,   266,     0,     0,    30,     0,   148,   231,     0,
      56,    81,   121,    78,    31,   303,     0,     0,   242,   239,
      53,     0,    49,    93,    34,    42,    44,     0,   402,     0,
       0,   393,     0,   395,     0,     0,     0,     0,     0,     0,
     406,     8,     0,     0,     0,   215,     0,     0,     0,     0,
       0,   351,   430,   429,   432,   440,   439,   444,   443,   426,
     423,   424,   425,   376,   413,   392,   391,   390,   377,   417,
     428,   422,   420,   431,   421,   419,   411,   416,   418,   427,
     410,   414,   415,   412,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   442,   441,   446,   445,   209,   206,   207,   208,
     212,   213,   214,     0,     0,   355,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   352,   352,    96,   240,     0,
       0,     0,   320,     0,   140,     0,   137,   241,   131,     0,
       0,     0,   327,     0,     0,     0,   333,     0,     0,     0,
     107,   115,     0,     0,   319,     0,   198,     0,   205,   221,
       0,   264,     0,   278,     0,   273,   368,     0,     0,   257,
     366,   256,   389,   297,   296,     0,     0,   321,     0,   246,
     368,     0,   448,    52,     0,    45,    50,     0,   403,   375,
     394,   301,   396,   302,     0,     0,   405,   112,   337,     0,
     408,   407,     0,     0,   132,   133,   216,   217,   218,   150,
       0,     0,   248,   247,   249,   251,    57,    64,    65,    66,
      61,    63,    71,    72,    59,    62,    60,    58,    68,    67,
      69,    70,   387,   388,   210,   211,   360,     0,   354,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,   309,   307,   308,   306,     0,   138,   136,     0,
       0,   114,     0,     0,    98,     0,    97,     0,     0,   104,
       0,     0,   102,     0,     0,   358,     0,     0,   118,   204,
     196,     0,   287,   222,   225,   224,   226,     0,   262,   265,
       0,   266,     0,   258,   267,   268,     0,     0,   120,   122,
     322,     0,   243,     0,    54,     0,    55,     0,     0,     0,
       0,   113,     0,    36,     0,   252,   221,   179,   177,   182,
     189,   190,   191,   186,   188,   184,   187,   185,   183,   193,
     192,   158,   161,   159,   160,   171,   162,   175,   167,   165,
     178,   166,   164,   169,   174,   176,   163,   168,   172,   173,
     170,   180,   181,   156,   194,   194,   250,   356,   381,   381,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    94,
     141,   139,   133,   101,     0,     0,   326,     0,   325,     0,
       0,   332,   105,     0,   331,   103,     0,   357,   117,   342,
     199,     0,     0,   266,   223,   237,   259,     0,     0,     0,
     124,     0,     0,    47,    46,     0,   110,   339,     0,     0,
     108,   338,     0,   409,    37,    73,   252,   252,   126,   252,
     134,     0,   157,     0,   197,   152,   153,     0,     0,     0,
       0,     0,     0,     0,     0,   252,   330,     0,     0,   336,
       0,     0,   359,   287,   290,   291,   292,     0,   289,   293,
     368,   233,   201,   200,     0,     0,     0,   276,   368,   125,
     123,     0,     0,   111,     0,   109,   135,   237,   156,   194,
     194,     0,     0,     0,     0,     0,     0,     0,   127,     0,
       0,     0,     0,   266,   279,     0,   234,   236,   235,   238,
     229,   230,   144,     0,    48,     0,     0,     0,   154,   155,
     195,     0,     0,     0,     0,     0,     0,   329,   328,   335,
     334,   203,   202,   281,   282,   284,   368,     0,   404,   368,
     341,   340,   227,   151,   228,     0,   283,   285,   279,   286
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -757,  -757,  -757,     1,  -602,  1756,  -757,  -757,  1253,    19,
     201,  -757,  -757,  -757,  -757,  -757,    72,  1927,   -10,    -1,
    -715,  -573,   -40,  -757,  -757,    81,  -757,  -757,  -757,  -757,
    -757,  -757,   412,    83,  -147,  -757,  -757,   -26,   357,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -183,  -182,  -663,  -757,
    -181,   -37,  -757,  -303,  -757,  -757,   -72,  -757,  -251,   731,
    -757,  -236,  -217,  -757,  -135,  -757,  -757,  -757,  -201,   251,
    -757,  -318,  -612,  -757,  -474,  -335,  -756,  -748,  -187,     7,
      93,  -757,   -69,  -757,   125,   345,  -319,  -757,  -757,   667,
    -757,    -9,  -757,  -757,  -199,  -757,  -441,  -757,   793,   928,
     -11,  1057,  -757,  1177,  1434,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,  -280
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   211,    74,   481,    76,    77,    78,   212,
     513,   517,   514,    79,    80,    81,   152,    82,   156,   190,
      83,    84,    85,    86,    87,   577,    88,    89,    90,   505,
     629,    91,    92,   374,   645,    93,   465,   466,   136,   165,
     489,    95,    96,   375,   646,   539,   684,   685,   755,   323,
     485,   486,   487,   443,   540,   229,   617,   853,   822,   182,
     817,   782,   785,    97,   200,   348,    98,    99,   168,   169,
     327,   328,   499,   331,   332,   495,   844,   779,   723,   213,
     217,   218,   300,   301,   302,   137,   101,   102,   103,   138,
     105,   124,   125,   444,   317,   604,   445,   106,   139,   140,
     109,   142,   111,   143,   144,   114,   115,   222,   116,   117,
     118,   119,   120
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     122,   179,   180,    73,   447,   141,   128,   490,   147,   726,
     294,   496,   509,   153,   160,   161,   162,     5,   621,   488,
     305,   367,   756,   171,   172,   329,   173,   174,   510,   377,
     378,   379,   380,   184,   186,   187,   191,   192,   605,     5,
     196,     5,   598,   749,   202,   203,   204,  -204,     5,   205,
     188,   183,   206,   207,   208,    14,   209,   188,   626,   184,
     219,   221,   201,   333,   436,    45,   845,   329,   498,     3,
     821,     5,   750,   369,   357,   593,   123,   329,   437,   220,
     360,   438,   453,   364,   315,   126,   627,  -271,   324,   145,
     146,   781,   856,   395,   320,   439,   440,   376,    43,   315,
     857,   358,   859,  -271,   370,   441,   159,   336,   727,   361,
    -271,   783,   821,   329,    54,   362,   727,   442,   122,   288,
     289,   290,   196,   184,   221,   346,   687,    45,   620,  -271,
     315,   339,   293,   197,   334,     5,   828,   829,   303,   637,
     628,   396,   291,   725,   363,  -271,  -271,   325,  -271,   197,
     149,   175,   303,   365,   365,  -271,   163,   197,   309,   312,
     316,   176,  -271,   749,   197,   303,   193,   227,   337,   166,
     620,   166,   335,   619,   194,   541,   796,  -204,   611,   365,
     620,  -271,   197,   303,   454,   366,   228,   365,   852,   339,
     344,   567,   750,   339,   609,   197,  -269,  -271,  -271,   447,
    -271,   842,   353,   847,   359,     5,   197,   225,   340,   151,
     728,   430,   431,   356,  -345,   282,   330,  -271,   732,   283,
     568,   235,   591,   610,   381,   382,   383,   384,   386,   388,
     389,   390,   391,   392,   393,   394,  -271,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   429,   303,
     303,   433,   435,   446,   365,   772,   506,   797,   600,   603,
     716,   456,  -361,  -362,     5,  -288,   459,   447,   151,   609,
    -344,   230,   457,   721,  -280,   339,   234,   184,   238,   472,
     474,   496,   476,   478,  -288,   848,   488,   482,  -288,   717,
     564,  -361,  -362,  -280,   350,   470,   480,  -280,   830,   -73,
     385,   387,   500,   231,   334,   638,   640,   280,   504,   281,
    -349,  -350,   232,  -288,   303,   239,  -347,   565,   -73,   240,
     515,   515,  -280,   542,   587,  -346,   504,   100,   233,   219,
     177,   219,   286,   525,   504,   528,   100,   530,   532,    94,
      23,   178,   184,   432,   434,   100,   284,    29,    94,  -145,
     436,   588,   334,   455,   295,   613,   632,    94,   100,   245,
     534,  -386,   247,  -386,   437,   536,   250,   438,  -145,   297,
      94,   543,   473,   787,   537,   477,   100,   787,   334,   334,
     570,   439,   440,   780,   283,  -245,   538,   614,    94,   788,
     334,   441,  -385,   788,  -385,   100,   615,   544,   545,   100,
     304,  -348,   238,   442,  -245,   308,   247,    94,   616,   686,
    -386,    94,  -386,   460,   322,   461,   447,   472,   573,   476,
     576,   482,   578,   528,     5,   524,   579,   580,   151,   582,
     584,   488,   303,   521,   345,   523,   347,   371,   590,   239,
     592,  -254,   349,   240,   597,   373,   458,   245,   446,   100,
     247,   462,   594,   596,   250,   467,   599,   602,   463,   469,
     479,    94,   483,   221,   491,   497,   623,   625,   484,   498,
     492,   511,   502,  -254,   507,   846,   533,   221,   529,   535,
     562,   622,  -254,   245,   849,   246,   247,   563,   586,   100,
     250,   606,   618,   -51,   155,   631,   630,   257,  -254,   633,
     634,    94,   636,   339,    14,   691,   263,    17,   694,   846,
     572,   696,   575,   100,   722,    22,   702,    23,   745,   846,
     724,   753,   100,   738,    29,    94,  -145,   742,   754,   784,
     773,   589,   644,   516,    94,   765,   446,   100,   386,   433,
     690,   701,    39,   693,   746,  -145,   747,    43,   306,    94,
     799,   800,   720,   801,   751,   700,   854,   841,   698,   699,
     827,    53,   706,    54,   708,   501,   813,   711,   585,   703,
     714,     0,     0,     0,     0,    14,   719,     0,    17,   712,
       0,     0,   715,     0,     0,     0,    22,     0,    23,   745,
       0,     0,     0,     0,     0,    29,   184,  -145,     0,     0,
       0,     0,   733,     0,   734,     0,   737,     0,   741,     0,
       0,   743,    68,   223,   731,   746,  -145,   747,    43,     0,
     748,     0,     0,     0,     0,   356,     0,     0,     0,     0,
     385,   432,    53,     0,    54,   100,     0,     0,   100,     0,
       0,     0,     0,   100,     0,     0,     0,    94,     0,   104,
      94,     0,     0,   238,     0,    94,     0,     0,   104,     0,
     706,   708,     0,   711,   714,   737,   741,   104,     0,     0,
       0,     0,     0,   766,   767,     0,   768,     0,   769,   770,
     104,     0,   771,    68,     0,   446,   179,   180,     0,     5,
     239,   100,     0,   151,   240,   154,   786,   528,   104,     0,
       0,   528,     0,    94,   792,     0,  -254,     0,   794,     0,
     790,     0,     0,     0,     0,     0,     0,   104,     0,     0,
       0,   104,     0,     0,     0,  -272,   766,   802,   803,   769,
     804,   805,   806,   807,   245,     0,   246,   247,  -254,     0,
       0,   250,     0,     0,   192,   196,   221,  -254,   257,     0,
     818,     0,     0,   819,  -272,   261,   262,   263,     0,   155,
     824,     0,     0,  -254,   815,     0,     0,     0,   566,     0,
    -272,   104,     0,     0,     0,   107,     0,  -272,   837,   838,
     839,   840,   216,     0,   107,  -275,     0,     0,   238,     0,
       0,     0,   741,   107,   850,   851,  -272,     0,     0,     0,
     837,   838,   839,   840,   850,   851,   107,     0,   100,     0,
    -277,   104,     0,     0,  -275,  -272,   855,   528,     0,     0,
      94,     0,  -272,   177,   107,   239,     0,   741,     0,   240,
    -275,     0,     0,    23,   178,   104,     0,  -275,     0,  -277,
      29,     0,  -145,   107,   104,     0,     0,   107,   -40,     0,
       0,     0,     0,     0,   100,  -277,  -275,     0,   100,   104,
     299,  -145,  -277,     0,     0,   242,    94,   243,   244,   245,
      94,   246,   247,     0,   299,  -275,   250,     0,     0,     0,
       0,  -277,  -275,   257,     0,     0,     0,   299,     0,     0,
     261,   262,   263,     0,   688,   689,     0,   107,     0,     0,
    -277,     0,     0,     0,   236,   299,     0,  -277,     0,   237,
     108,     0,     0,     0,     0,     0,     0,   238,   100,   108,
     100,     0,     0,   100,     0,     0,   100,     0,   108,     0,
      94,   100,    94,     0,     0,    94,     0,   107,    94,     0,
       0,   108,     0,    94,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   239,     0,     0,   104,   240,   108,
     104,   107,   100,    94,   100,   104,     0,     0,     0,   100,
     107,     0,     0,     0,    94,   315,    94,     0,   108,   241,
       0,    94,   108,     0,     0,   107,     0,     0,     0,     0,
       0,   299,   299,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   104,   258,   259,   260,     0,     0,   261,
     262,   263,     0,     0,     0,     0,   177,     0,     0,   100,
     639,     0,   108,     0,   100,     0,    23,   178,     0,   110,
       0,    94,     0,    29,     0,  -145,    94,     0,   110,     0,
     503,   -39,     0,     0,   100,     0,   299,   110,     0,     0,
       0,     0,     0,   100,  -145,     0,    94,   100,   503,     0,
     110,   216,   108,   216,     0,    94,   503,     0,     0,    94,
       0,    14,     0,   107,    17,     0,   107,     0,   110,     0,
       0,   107,    22,     0,    23,   745,   108,     0,     0,     0,
       0,    29,     0,  -145,     0,   108,     0,   110,     0,     0,
       0,   110,     0,     0,     0,     0,     0,     0,     0,    39,
     108,   746,  -145,   747,    43,     0,     0,     0,     0,     0,
     104,     0,     0,     0,   100,   100,   100,   100,    53,   107,
      54,     0,     0,     0,     0,     0,    94,    94,    94,    94,
     100,   100,     0,     0,     0,     0,     0,     0,     0,   112,
       0,   110,    94,    94,     0,     0,     0,     0,   112,     0,
       0,     0,   583,     0,   299,     0,   104,   112,     0,     0,
     104,     0,     0,     0,     0,     0,     0,     0,     0,    68,
     112,     0,     0,     0,     0,     0,     0,   808,     0,     0,
       0,   110,     0,     0,     0,     0,     0,   236,   112,     0,
       0,     0,   237,     0,     0,     0,     0,     0,   108,     0,
     238,   108,     0,     0,     0,   110,   108,   112,     0,     0,
       0,   112,     0,     0,   110,     0,     0,     0,     0,     0,
     104,     0,   104,     0,     0,   104,     0,     0,   104,   110,
       0,     0,     0,   104,     0,     0,   107,   239,     0,     0,
       0,   240,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   104,   108,     0,     0,     0,     0,     0,
     189,   112,   241,     0,   104,   198,   104,     0,     0,     0,
       0,   104,     0,     0,     0,     0,     0,   242,     0,   243,
     244,   245,   107,   246,   247,   248,   107,   249,   250,   251,
     252,   253,   254,   255,   256,   257,     0,   258,   259,   260,
       0,   112,   261,   262,   263,     0,     0,     0,     0,     0,
       0,     0,     0,   697,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   112,     0,   110,     0,     0,
     110,   104,     0,     0,   112,   110,   104,     0,     0,     0,
       0,   287,     0,     0,     0,     0,   107,     0,   107,   112,
       0,   107,     0,     0,   107,     0,   104,     0,     0,   107,
       0,     0,     0,     0,     0,   104,     0,     0,     0,   104,
       0,   108,   189,   189,   189,   319,     0,     0,     0,   107,
       0,     0,     0,   110,     0,     0,   189,     0,     0,     0,
     107,     0,   107,     0,     0,     0,   113,   107,     0,     0,
       0,     0,     0,     0,   189,   113,     0,     0,     0,     0,
       0,     0,     0,   778,   113,     0,   189,   108,     0,     0,
       0,   108,     0,     0,     0,     0,     0,   113,     0,     0,
       0,     0,     0,     0,     0,     0,   104,   104,   104,   104,
       0,     0,     0,     0,     0,   113,     0,   112,     0,     0,
     112,     0,   104,   104,     0,   112,     0,   107,     0,     0,
       0,     0,   107,     0,   113,     0,     0,     0,   113,     0,
       0,   238,   816,     0,     0,     0,     0,     0,     0,     0,
       0,   108,   107,   108,     0,     0,   108,     0,     0,   108,
       0,   107,     0,     0,   108,   107,     0,     0,     0,     0,
     110,     0,     0,   112,     0,   843,   189,     0,   239,     0,
       0,     0,   240,     0,   108,     0,     0,     0,   113,     0,
       0,     0,     0,     0,     0,   108,     0,   108,     0,     0,
     189,     0,   108,     0,     0,     0,     0,     0,     0,   843,
       0,     0,     0,     0,   238,     0,   110,     0,   242,   843,
     110,   244,   245,     0,   246,   247,     0,     0,   113,   250,
       0,     0,   107,   107,   107,   107,   257,     0,     0,     0,
       0,     0,     0,   261,   262,   263,     0,     0,   107,   107,
       0,   239,   113,     0,     0,   240,     0,     0,     0,     0,
       0,   113,   108,     0,     0,     0,     0,   108,     0,     0,
       0,     0,     0,     0,     0,     0,   113,     0,     0,     0,
     110,     0,   110,     0,     0,   110,     0,   108,   110,     0,
     112,   242,     0,   110,     0,   245,   108,   246,   247,     0,
     108,     0,   250,     0,     0,     0,     0,     0,     0,   257,
       0,     0,     0,   110,     0,     0,   261,   262,   263,     0,
       0,     0,     0,     0,   110,     0,   110,     0,     0,     0,
       0,   110,     0,     0,     0,     0,   112,     0,     0,     0,
     112,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,   189,     0,     0,
     189,   189,     0,     0,     0,     0,     0,   108,   108,   108,
     108,     0,     0,     0,   113,     0,     0,   113,     0,     0,
       0,     0,   113,   108,   108,     0,     0,     0,    75,     0,
       0,   110,     0,     0,     0,     0,   110,   121,     0,     0,
     112,     0,   112,     0,     0,   112,   148,     0,   112,     0,
       0,     0,     0,   112,     0,     0,   110,     0,     0,   170,
       0,     0,     0,     0,     0,   110,     0,   236,     0,   110,
     113,     0,   237,   112,     0,     0,     0,   195,     0,     0,
     238,     0,     0,     0,   112,     0,   112,     0,     0,     0,
       0,   112,     0,     0,     0,     0,    75,     0,     0,     0,
     224,     0,   189,   189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,     0,   319,     0,   239,     0,     0,
     319,   240,     0,   189,     0,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,   110,   110,   110,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
     285,   112,   110,   110,     0,     0,   112,   242,     0,   243,
     244,   245,     0,   246,   247,   248,     0,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   112,   258,   259,   260,
       0,     0,   261,   262,   263,   112,     0,   113,     0,   112,
     321,     0,     0,   823,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     127,     0,     0,     0,   343,   127,     0,     0,     0,   150,
     127,   127,     0,    75,     0,     0,     0,   164,     0,   167,
       0,     0,     0,   113,     0,     0,     0,   113,    75,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     199,     0,     0,     0,   189,     0,   112,   112,   112,   112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   112,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   226,
     319,   319,     0,   319,   319,     0,     0,   113,     0,   113,
       0,     0,   113,     0,     0,   113,     0,     0,   820,     0,
     113,     0,     0,     0,     0,   319,   238,   319,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,     0,     0,     0,     0,     0,   343,     0,     0,   343,
       0,   113,   199,   113,     0,     0,   298,     0,   113,     0,
     820,     0,   199,   239,   307,     0,     0,   240,     0,     0,
     298,     0,     0,   326,     0,     0,     0,     0,     0,     0,
       0,     0,     5,   298,     0,   127,     0,     0,   338,   647,
       0,     0,     0,     0,   648,     0,     0,     0,     0,     0,
       0,   298,   527,   242,     0,   243,   244,   245,     0,   246,
     247,   248,     0,     0,   250,     0,     0,     0,   113,     0,
     256,   257,     0,   113,     0,   260,     0,     0,   261,   262,
     263,     0,   372,     0,     0,     0,     0,   167,   167,   167,
     167,     0,     0,   113,     0,     0,     0,     0,     0,     0,
       0,     0,   113,     0,   397,     0,   113,     0,     0,     0,
       0,     0,     0,     0,     0,   649,   650,   651,   652,   653,
     654,     0,     0,   655,   656,   657,   658,   659,   660,   661,
     662,   663,   664,     0,     0,   665,     0,   298,   298,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,   677,     0,     0,   678,   679,   680,   681,     0,   682,
       0,   464,     0,   468,     0,     0,     0,     0,     0,   608,
       0,     0,     0,   113,   113,   113,   113,     0,     0,     0,
       0,     0,   326,     0,     0,     0,     0,     0,     0,   113,
     113,   167,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,     0,   493,     0,     5,     6,
       7,     8,     9,    10,     0,   641,     0,   129,     0,    75,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,   167,     0,     0,   130,   131,   132,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   133,     0,     0,     0,   343,
       0,   343,     0,   236,   343,     0,     0,   343,   237,     0,
      58,     0,   718,     0,     0,     0,   238,     0,     0,   239,
       0,     0,     0,   240,     0,    59,    60,    61,    62,     0,
       0,     0,   730,    63,     0,     0,     0,     0,     0,     0,
     298,     0,    64,   736,    65,   740,    66,    67,     0,     0,
      75,     0,    69,   239,     0,     0,   134,   240,    71,   242,
     494,   243,   244,   245,   612,   246,   247,   248,   326,   249,
     250,   251,   252,     0,   254,   255,   256,   257,   241,   258,
       0,   260,     0,     0,   261,   262,   263,     0,   199,     0,
       0,     0,     0,   242,     0,   243,   244,   245,     0,   246,
     247,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,     0,   258,   259,   260,     0,   683,   261,   262,
     263,     0,     0,     0,     0,     0,     0,     0,     0,   858,
       0,     0,     0,     0,     0,   789,     0,     0,     0,     0,
       0,     0,     0,     0,   793,     0,     0,     0,   795,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   464,    -2,     4,     0,     5,     6,
       7,     8,     9,    10,     0,   238,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -252,    18,    19,   326,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -145,    30,    31,    32,    33,    34,   -38,
      35,    36,   239,    37,     0,     0,   240,  -252,    38,    39,
      40,    41,  -145,    42,    43,     0,  -252,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,    53,   752,
      54,    55,  -252,     0,     0,    56,     0,    57,     0,     0,
      58,     0,   242,     0,   243,   244,   245,     0,   246,   247,
       0,     0,     0,   250,     0,    59,    60,    61,    62,   256,
     257,     0,     0,    63,   260,     0,     0,   261,   262,   263,
       0,     0,    64,     0,    65,     0,    66,    67,     0,    68,
       0,     0,    69,     0,     0,     0,    70,     0,    71,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   127,     0,     0,     0,     0,     0,     0,     4,
     798,     5,     6,     7,     8,     9,    10,     0,  -404,     0,
      11,    12,    13,  -404,    14,    15,    16,    17,  -252,    18,
      19,  -404,    20,    21,  -404,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -145,    30,    31,    32,
      33,    34,   -38,    35,    36,     0,    37,     0,     0,     0,
    -252,    38,     0,    40,    41,  -145,    42,    43,  -404,  -252,
      44,    45,  -404,    46,    47,    48,    49,    50,    51,     0,
      52,    53,     0,    54,    55,  -252,     0,     0,    56,     0,
      57,     0,     0,  -404,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -404,    60,
    -404,  -404,  -404,  -404,  -404,  -404,  -404,     0,  -404,  -404,
    -404,  -404,  -404,  -404,  -404,  -404,  -404,    65,  -404,  -404,
    -404,     0,    68,  -404,  -404,  -404,     0,     0,     0,    70,
    -404,    71,     4,    72,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -252,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,     0,    28,    29,     0,  -145,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -252,    38,    39,    40,    41,  -145,    42,
      43,     0,  -252,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,    53,     0,    54,    55,  -252,     0,
       0,    56,     0,    57,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
      65,     0,    66,    67,   181,    68,     0,     0,    69,     0,
       0,     0,    70,   210,    71,     4,    72,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -252,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -145,    30,    31,    32,    33,    34,   -38,    35,
      36,     0,    37,     0,     0,     0,  -252,    38,    39,    40,
      41,  -145,    42,    43,     0,  -252,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,    53,     0,    54,
      55,  -252,     0,     0,    56,     0,    57,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,    65,     0,    66,    67,     0,    68,     0,
       0,    69,     0,     0,     0,    70,   210,    71,     4,    72,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -252,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,     0,    28,    29,     0,  -145,    30,    31,    32,    33,
      34,   -38,    35,    36,     0,    37,     0,     0,     0,  -252,
      38,    39,    40,    41,  -145,    42,    43,     0,  -252,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
      53,     0,    54,    55,  -252,     0,     0,    56,     0,    57,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,    61,
      62,     0,     0,     0,     0,    63,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,    65,     0,    66,    67,
       0,    68,     0,     0,    69,     0,     0,     0,    70,   355,
      71,     4,    72,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -252,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -145,    30,
      31,    32,    33,    34,   -38,    35,    36,     0,    37,     0,
       0,     0,  -252,    38,    39,    40,    41,  -145,    42,    43,
       0,  -252,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,    53,     0,    54,    55,  -252,     0,     0,
      56,     0,    57,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,    65,
       0,    66,    67,     0,    68,     0,     0,    69,     0,     0,
       0,    70,   643,    71,     4,    72,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -252,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,    27,     0,    28,    29,
       0,  -145,    30,    31,    32,    33,    34,   -38,    35,    36,
       0,    37,     0,     0,     0,  -252,    38,    39,    40,    41,
    -145,    42,    43,     0,  -252,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,    53,     0,    54,    55,
    -252,     0,     0,    56,     0,    57,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,    65,     0,    66,    67,     0,    68,     0,     0,
      69,     0,     0,     0,    70,   744,    71,     4,    72,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -252,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -145,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,     0,  -252,    38,
     223,    40,    41,  -145,    42,    43,     0,  -252,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,    53,
       0,    54,    55,  -252,     0,     0,    56,     0,    57,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,    65,     0,    66,    67,     0,
      68,     0,     0,    69,     0,     0,     0,    70,     0,    71,
       4,    72,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -252,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,    27,     0,    28,    29,     0,  -145,    30,    31,
      32,    33,    34,   -38,    35,    36,     0,    37,     0,     0,
       0,  -252,    38,     0,    40,    41,  -145,    42,    43,     0,
    -252,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,    53,     0,    54,    55,  -252,     0,     0,    56,
       0,    57,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,    65,     0,
      66,    67,     0,    68,     0,     0,    69,     0,     0,     0,
      70,     0,    71,     4,    72,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -252,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,   471,     0,    28,    29,     0,
    -145,    30,    31,    32,    33,    34,   -38,    35,    36,     0,
      37,     0,     0,     0,  -252,    38,     0,    40,    41,  -145,
      42,    43,     0,  -252,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,    53,     0,    54,    55,  -252,
       0,     0,    56,     0,    57,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    60,    61,    62,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,    65,     0,    66,    67,     0,    68,     0,     0,    69,
       0,     0,     0,    70,     0,    71,     4,    72,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -252,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,   475,     0,
      28,    29,     0,  -145,    30,    31,    32,    33,    34,   -38,
      35,    36,     0,    37,     0,     0,     0,  -252,    38,     0,
      40,    41,  -145,    42,    43,     0,  -252,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,    53,     0,
      54,    55,  -252,     0,     0,    56,     0,    57,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,    65,     0,    66,    67,     0,    68,
       0,     0,    69,     0,     0,     0,    70,     0,    71,     4,
      72,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -252,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   705,     0,    28,    29,     0,  -145,    30,    31,    32,
      33,    34,   -38,    35,    36,     0,    37,     0,     0,     0,
    -252,    38,     0,    40,    41,  -145,    42,    43,     0,  -252,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,    53,     0,    54,    55,  -252,     0,     0,    56,     0,
      57,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,    62,     0,     0,     0,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,    65,     0,    66,
      67,     0,    68,     0,     0,    69,     0,     0,     0,    70,
       0,    71,     4,    72,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -252,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,   707,     0,    28,    29,     0,  -145,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -252,    38,     0,    40,    41,  -145,    42,
      43,     0,  -252,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,    53,     0,    54,    55,  -252,     0,
       0,    56,     0,    57,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
      65,     0,    66,    67,     0,    68,     0,     0,    69,     0,
       0,     0,    70,     0,    71,     4,    72,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -252,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,   710,     0,    28,
      29,     0,  -145,    30,    31,    32,    33,    34,   -38,    35,
      36,     0,    37,     0,     0,     0,  -252,    38,     0,    40,
      41,  -145,    42,    43,     0,  -252,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,    53,     0,    54,
      55,  -252,     0,     0,    56,     0,    57,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,    65,     0,    66,    67,     0,    68,     0,
       0,    69,     0,     0,     0,    70,     0,    71,     4,    72,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -252,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
     713,     0,    28,    29,     0,  -145,    30,    31,    32,    33,
      34,   -38,    35,    36,     0,    37,     0,     0,     0,  -252,
      38,     0,    40,    41,  -145,    42,    43,     0,  -252,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
      53,     0,    54,    55,  -252,     0,     0,    56,     0,    57,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,    61,
      62,     0,     0,     0,     0,    63,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,    65,     0,    66,    67,
       0,    68,     0,     0,    69,     0,     0,     0,    70,     0,
      71,     4,    72,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -252,    18,    19,     0,   729,    21,     0,    22,     0,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -145,    30,
      31,    32,    33,    34,   -38,    35,    36,     0,    37,     0,
       0,     0,  -252,    38,     0,    40,    41,  -145,    42,    43,
       0,  -252,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,    53,     0,    54,    55,  -252,     0,     0,
      56,     0,    57,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,    65,
       0,    66,    67,     0,    68,     0,     0,    69,     0,     0,
       0,    70,     0,    71,     4,    72,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -252,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,   735,     0,    28,    29,
       0,  -145,    30,    31,    32,    33,    34,   -38,    35,    36,
       0,    37,     0,     0,     0,  -252,    38,     0,    40,    41,
    -145,    42,    43,     0,  -252,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,    53,     0,    54,    55,
    -252,     0,     0,    56,     0,    57,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,    65,     0,    66,    67,     0,    68,     0,     0,
      69,     0,     0,     0,    70,     0,    71,     4,    72,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -252,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,   739,
       0,    28,    29,     0,  -145,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,     0,  -252,    38,
       0,    40,    41,  -145,    42,    43,     0,  -252,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,    53,
       0,    54,    55,  -252,     0,     0,    56,     0,    57,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,    65,     0,    66,    67,     0,
      68,     0,     0,    69,     0,     0,     0,    70,     0,    71,
     493,    72,     5,     6,     7,     8,     9,    10,     0,  -404,
       0,   129,     0,     0,  -404,     0,     0,     0,     0,     0,
       0,     0,  -404,     0,    21,     0,     0,     0,     0,     0,
     130,   131,   132,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,  -404,
       0,     0,     0,  -404,     0,     0,    48,    49,    50,   133,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -270,  -404,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -404,
      60,  -404,  -404,  -404,     0,  -404,  -404,  -404,     0,  -404,
    -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,    65,  -404,
    -404,  -404,     0,     0,  -404,  -404,  -404,     0,     0,     0,
     134,     0,    71,   493,   508,     5,     6,     7,     8,     9,
      10,     0,  -404,     0,   129,     0,     0,  -404,     0,     0,
       0,     0,     0,     0,     0,  -404,     0,    21,     0,     0,
       0,     0,     0,   130,   131,   132,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,  -404,     0,     0,     0,  -404,     0,     0,    48,
      49,    50,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -404,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -404,    60,  -404,  -404,  -404,     0,  -404,  -404,
    -404,     0,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,
    -404,    65,  -404,  -404,  -404,     0,     0,  -404,  -404,  -404,
       0,     0,     0,   134,     0,    71,   493,   508,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   129,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   130,   131,   132,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   133,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,     0,
       0,     0,     0,    63,     0,     0,     0,     0,   278,   279,
    -363,     0,    64,  -385,    65,  -385,    66,    67,     0,     0,
       0,     0,    69,     0,     0,     0,   134,     0,    71,     0,
     508,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   130,
     131,   132,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   133,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,    62,     0,     0,     0,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,    65,     0,    66,
      67,   181,     0,     0,     0,    69,     0,     0,     0,   134,
       0,    71,     0,   135,   526,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   130,   131,   132,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    60,    61,    62,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,    65,     0,    66,    67,   181,     0,     0,     0,    69,
       0,     0,     0,   134,     0,    71,     0,   135,   814,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   130,   131,   132,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   133,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,    65,     0,    66,    67,     0,
       0,     0,     0,    69,     0,     0,     0,   134,     0,    71,
       0,   135,   531,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   129,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   130,   131,   132,   437,    28,     0,   438,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,   439,   440,    38,     0,    40,     0,     0,     0,     0,
       0,   441,     0,     0,     0,     0,     0,    48,    49,    50,
     133,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,    65,
       0,    66,    67,     0,     0,     0,     0,    69,     0,     0,
       0,   134,     0,    71,     0,   135,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   129,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   130,   131,   132,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   133,     0,     0,     0,   214,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,     0,     0,     0,
       0,    63,   215,     0,     0,     0,     0,     0,     0,     0,
      64,     0,    65,     0,    66,    67,   181,     0,     0,     0,
      69,     0,     0,     0,   134,     0,    71,     0,   135,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   130,   131,   132,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   133,     0,     0,     0,
     214,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,    65,     0,    66,    67,   181,
       0,     0,     0,    69,     0,     0,     0,   134,     0,    71,
     520,   135,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   129,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     130,   131,   132,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   133,
       0,     0,     0,   214,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,    65,     0,
      66,    67,   181,     0,     0,     0,    69,     0,     0,     0,
     134,     0,    71,   522,   135,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   130,   131,   132,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,   157,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   158,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    60,    61,    62,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,    65,     0,    66,    67,     0,     0,     0,     0,    69,
       0,     0,     0,   134,     0,    71,     0,   135,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   129,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   130,   131,   132,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,    65,     0,    66,    67,   181,     0,
       0,     0,    69,     0,     0,     0,   134,   518,    71,     0,
     135,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   130,
     131,   132,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   133,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,    62,     0,     0,     0,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,    65,     0,    66,
      67,   181,     0,     0,     0,    69,     0,     0,     0,   134,
       0,    71,     0,   135,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   129,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   130,   131,   132,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   133,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
      65,     0,    66,    67,     0,   185,     0,     0,    69,     0,
       0,     0,   134,     0,    71,     0,   135,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   129,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   188,    21,
       0,     0,     0,     0,     0,   130,   131,   132,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,    65,     0,    66,    67,     0,     0,     0,
       0,    69,     0,     0,     0,    70,     0,    71,     0,   135,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   129,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   130,   131,
     132,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   133,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     158,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,    61,
      62,     0,     0,     0,     0,    63,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,    65,     0,    66,    67,
       0,     0,     0,     0,    69,     0,     0,     0,   134,     0,
      71,     0,   135,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   130,   131,   132,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,   624,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     133,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,    65,
       0,    66,    67,     0,     0,     0,     0,    69,     0,     0,
       0,   134,     0,    71,     0,   135,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   129,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   774,     0,
       0,     0,     0,     0,   130,   131,   132,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     775,    49,    50,   776,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,    65,     0,    66,    67,   181,     0,     0,     0,
      69,     0,     0,     0,   134,     0,    71,     0,   777,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   130,   131,   132,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   133,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,    65,     0,    66,    67,   181,
       0,     0,     0,    69,     0,     0,     0,   134,     0,    71,
       0,   777,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   129,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     130,   131,   764,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   133,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,    65,     0,
      66,    67,   181,     0,     0,     0,    69,     0,     0,     0,
     134,     0,    71,     0,   135,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   130,   131,   132,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    60,    61,    62,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,    65,     0,    66,    67,     0,     0,     0,     0,    69,
       0,     0,     0,   134,     0,    71,     0,   135,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   129,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   130,   131,   132,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,    65,     0,    66,    67,     0,     0,
       0,     0,   512,     0,     0,     0,   134,     0,    71,     0,
     135,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   130,
     131,   571,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   133,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,    62,     0,     0,     0,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,    65,     0,    66,
      67,     0,     0,     0,     0,    69,     0,     0,     0,   134,
       0,    71,     0,   135,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   129,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   130,   131,   574,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   133,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
      65,     0,    66,    67,     0,     0,     0,     0,    69,     0,
       0,     0,   134,     0,    71,     0,   135,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   129,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   130,   131,   758,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,    65,     0,    66,    67,     0,     0,     0,
       0,    69,     0,     0,     0,   134,     0,    71,     0,   135,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   129,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   130,   131,
     759,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   133,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,    61,
      62,     0,     0,     0,     0,    63,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,    65,     0,    66,    67,
       0,     0,     0,     0,    69,     0,     0,     0,   134,     0,
      71,     0,   135,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   130,   131,   761,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     133,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,    65,
       0,    66,    67,     0,     0,     0,     0,    69,     0,     0,
       0,   134,     0,    71,     0,   135,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   129,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   130,   131,   762,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   133,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,    65,     0,    66,    67,     0,     0,     0,     0,
      69,     0,     0,     0,   134,     0,    71,     0,   135,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   130,   131,   763,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   133,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,    65,     0,    66,    67,     0,
       0,     0,     0,    69,     0,     0,     0,   134,     0,    71,
       0,   135,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   129,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     130,   131,   764,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   133,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,    65,     0,
      66,    67,     0,     0,     0,     0,    69,     0,     0,     0,
     134,     0,    71,     0,   135,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   130,   131,   132,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   129,     0,     0,
       0,     0,    59,    60,    61,    62,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   130,   131,   132,    64,
      28,    65,     0,     0,    67,    31,    32,     0,    34,    69,
      35,    36,     0,   134,     0,    71,     0,   135,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   133,     0,     0,     0,     5,
       6,     7,     8,     9,    10,     0,   238,     0,   129,     0,
       0,  -279,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,    60,   130,   131,   132,
    -279,    28,     0,     0,  -279,     0,    31,    32,     0,    34,
       0,    35,    36,   239,    65,     0,     0,   240,   181,    38,
       0,    40,     0,     0,     0,     0,   134,   236,    71,  -279,
     777,     0,   237,    48,    49,    50,   133,     0,   241,     0,
     238,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   242,     0,   243,   244,   245,     0,   246,
     247,   248,     0,   249,   250,   251,   252,    60,   254,   255,
     256,   257,     0,   258,   259,   260,     0,   239,   261,   262,
     263,   240,     0,     0,     0,    65,     0,     0,     0,     0,
       0,   236,     0,     0,     0,     0,   237,     0,     0,    71,
       0,   135,   241,     0,   238,   313,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   314,     0,   242,     0,   243,
     244,   245,     0,   246,   247,   248,     0,   249,   250,   251,
     252,   253,   254,   255,   256,   257,     0,   258,   259,   260,
       0,   239,   261,   262,   263,   240,     0,     0,     0,     0,
     236,   519,     0,     0,     0,   237,     0,     0,     0,     0,
       0,     0,   315,   238,   310,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   311,     0,     0,     0,     0,     0,
       0,   242,     0,   243,   244,   245,     0,   246,   247,   248,
       0,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     239,   258,   259,   260,   240,     0,   261,   262,   263,   236,
       0,     0,   197,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,   601,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,     0,   261,   262,   263,   236,     0,
       0,   197,     0,   237,     0,     0,     0,     0,     0,     0,
     315,   238,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   239,   258,
     259,   260,   240,     0,   261,   262,   263,   236,     0,   318,
     197,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,   188,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,   236,     0,     0,   197,
       0,   237,     0,     0,     0,     0,     0,     0,     0,   238,
     595,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   242,     0,   243,
     244,   245,     0,   246,   247,   248,     0,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   239,   258,   259,   260,
     240,     0,   261,   262,   263,   236,     0,     0,   197,     0,
     237,     0,     0,     0,     0,     0,     0,     0,   238,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,   243,   244,
     245,     0,   246,   247,   248,     0,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   239,   258,   259,   260,   240,
       0,   261,   262,   263,   236,     0,   704,   197,     0,   237,
       0,     0,     0,     0,     0,     0,     0,   238,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   242,     0,   243,   244,   245,
       0,   246,   247,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   239,   258,   259,   260,   240,     0,
     261,   262,   263,   236,     0,   709,   197,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,     0,   261,
     262,   263,   236,     0,   809,   197,     0,   237,     0,     0,
       0,     0,     0,     0,     0,   238,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   242,     0,   243,   244,   245,     0,   246,
     247,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   239,   258,   259,   260,   240,     0,   261,   262,
     263,   236,     0,   810,   197,     0,   237,     0,     0,     0,
       0,     0,     0,     0,   238,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   242,     0,   243,   244,   245,     0,   246,   247,
     248,     0,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   239,   258,   259,   260,   240,     0,   261,   262,   263,
     236,     0,   811,   197,     0,   237,     0,     0,     0,     0,
       0,     0,     0,   238,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   242,     0,   243,   244,   245,     0,   246,   247,   248,
       0,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     239,   258,   259,   260,   240,     0,   261,   262,   263,   236,
       0,   812,   197,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,     0,   261,   262,   263,   236,     0,
     825,   197,     0,   237,     0,     0,     0,     0,     0,     0,
       0,   238,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   239,   258,
     259,   260,   240,     0,   261,   262,   263,   236,     0,   826,
     197,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,   236,     0,     0,   197,
       0,   237,     0,     0,     0,     0,     0,     0,     0,   238,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   242,     0,   243,
     244,   245,     0,   246,   247,   248,     0,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   239,   258,   259,   260,
     240,     0,   261,   262,   263,   236,     0,     0,   342,     0,
     237,     0,     0,     0,     0,     0,     0,     0,   238,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,   292,
       0,     0,     0,     0,     0,     0,   242,     0,   243,   244,
     245,     0,   246,   247,   248,     0,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   239,   258,   259,   260,   240,
       0,   261,   262,   263,     0,     0,  -364,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   123,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   242,     0,   243,   244,   245,
       0,   246,   247,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   236,   258,   259,   260,     0,   237,
     261,   262,   263,     0,     0,     0,     0,   238,     0,     0,
       0,     0,   351,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   352,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,     0,     0,     0,   240,     0,
       0,     0,     0,   236,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,   448,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   449,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,     0,   261,
     262,   263,   236,     0,     0,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,   238,   450,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   451,     0,     0,     0,
       0,     0,     0,   242,     0,   243,   244,   245,     0,   246,
     247,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   239,   258,   259,   260,   240,     0,   261,   262,
     263,   236,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,   238,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   242,     0,   243,   244,   245,     0,   246,   247,
     248,     0,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   239,   258,   259,   260,   240,     0,   261,   262,   263,
     236,     0,     0,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,   238,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   242,     0,   243,   244,   245,     0,   246,   247,   248,
       0,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     239,   258,   259,   260,   240,   296,   261,   262,   263,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   341,   261,   262,   263,   236,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,   238,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   239,   258,
     259,   260,   240,   354,   261,   262,   263,   236,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   368,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,   236,     0,   452,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,   238,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   242,     0,   243,
     244,   245,     0,   246,   247,   248,     0,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   239,   258,   259,   260,
     240,     0,   261,   262,   263,   236,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,   238,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,   243,   244,
     245,     0,   246,   247,   248,     0,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   239,   258,   259,   260,   240,
     546,   261,   262,   263,   236,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,   238,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   242,     0,   243,   244,   245,
       0,   246,   247,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   239,   258,   259,   260,   240,   547,
     261,   262,   263,   236,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,   548,   261,
     262,   263,   236,     0,     0,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,   238,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   242,     0,   243,   244,   245,     0,   246,
     247,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   239,   258,   259,   260,   240,   549,   261,   262,
     263,   236,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,   238,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   242,     0,   243,   244,   245,     0,   246,   247,
     248,     0,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   239,   258,   259,   260,   240,   550,   261,   262,   263,
     236,     0,     0,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,   238,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   242,     0,   243,   244,   245,     0,   246,   247,   248,
       0,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     239,   258,   259,   260,   240,   551,   261,   262,   263,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   552,   261,   262,   263,   236,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,   238,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   239,   258,
     259,   260,   240,   553,   261,   262,   263,   236,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,   554,   261,   262,   263,   236,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,   238,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   242,     0,   243,
     244,   245,     0,   246,   247,   248,     0,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   239,   258,   259,   260,
     240,   555,   261,   262,   263,   236,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,   238,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,   243,   244,
     245,     0,   246,   247,   248,     0,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   239,   258,   259,   260,   240,
     556,   261,   262,   263,   236,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,   238,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   242,     0,   243,   244,   245,
       0,   246,   247,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   239,   258,   259,   260,   240,   557,
     261,   262,   263,   236,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,   558,   261,
     262,   263,   236,     0,     0,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,   238,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   242,     0,   243,   244,   245,     0,   246,
     247,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   239,   258,   259,   260,   240,   559,   261,   262,
     263,   236,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,   238,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   242,     0,   243,   244,   245,     0,   246,   247,
     248,     0,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   239,   258,   259,   260,   240,   560,   261,   262,   263,
     236,     0,     0,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,   238,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   242,     0,   243,   244,   245,     0,   246,   247,   248,
       0,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     239,   258,   259,   260,   240,   561,   261,   262,   263,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,   123,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,     0,   261,   262,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,   237,     0,   241,     0,     0,     0,     0,     0,
     238,     0,     0,   607,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,     0,   258,
     259,   260,     0,   581,   261,   262,   263,   239,     0,     0,
       0,   240,     0,     0,     0,     0,   236,   635,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,   238,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   242,     0,   243,
     244,   245,     0,   246,   247,   248,     0,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   239,   258,   259,   260,
     240,     0,   261,   262,   263,   236,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,   238,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,   243,   244,
     245,     0,   246,   247,   248,     0,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   239,   258,   259,   260,   240,
       0,   261,   262,   263,   236,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,   238,   692,   642,
     241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   242,     0,   243,   244,   245,
       0,   246,   247,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   239,   258,   259,   260,   240,     0,
     261,   262,   263,   236,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,   695,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,     0,   261,
     262,   263,   236,     0,     0,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,   238,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   242,     0,   243,   244,   245,     0,   246,
     247,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   239,   258,   259,   260,   240,     0,   261,   262,
     263,   236,     0,   757,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,   238,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   242,     0,   243,   244,   245,     0,   246,   247,
     248,     0,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   239,   258,   259,   260,   240,     0,   261,   262,   263,
     236,   791,   760,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,   238,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   242,     0,   243,   244,   245,     0,   246,   247,   248,
       0,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     239,   258,   259,   260,   240,     0,   261,   262,   263,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,     0,   261,   262,   263,   236,     0,
     831,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,   238,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   239,   258,
     259,   260,   240,     0,   261,   262,   263,   236,     0,   832,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,   236,     0,   833,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,   238,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   242,     0,   243,
     244,   245,     0,   246,   247,   248,     0,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   239,   258,   259,   260,
     240,     0,   261,   262,   263,   236,     0,   834,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,   238,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,   243,   244,
     245,     0,   246,   247,   248,     0,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   239,   258,   259,   260,   240,
       0,   261,   262,   263,   236,     0,   835,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,   238,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   242,     0,   243,   244,   245,
       0,   246,   247,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   239,   258,   259,   260,   240,     0,
     261,   262,   263,   236,     0,   836,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,     0,   261,
     262,   263,   236,     0,     0,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,   238,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   242,     0,   243,   244,   245,     0,   246,
     247,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   569,   258,   259,   260,   240,     0,   261,   262,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   242,     0,   243,   244,   245,     0,   246,   247,
     248,     0,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   239,   258,   259,   260,   240,     0,   261,   262,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   242,     0,   243,   244,   245,     0,   246,   247,   248,
       0,   249,   250,   251,   252,     0,   254,   255,   256,   257,
     239,   258,     0,   260,   240,     0,   261,   262,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
       0,   250,   251,   252,     0,   254,   255,   256,   257,     0,
       0,     0,   260,     0,     0,   261,   262,   263
};

static const yytype_int16 yycheck[] =
{
      11,    41,    42,     2,   284,    16,    15,   325,    19,   621,
     145,   330,   347,    23,    25,    26,    27,     3,   492,   322,
     155,   220,   685,    34,    35,     1,    37,    38,   347,   230,
     231,   232,   233,    44,    45,    46,    47,    48,   479,     3,
      51,     3,    24,   645,    55,    56,    57,     3,     3,    60,
      24,    44,    63,    64,    65,    16,    67,    24,    50,    70,
      71,    72,    55,    34,    20,    63,   814,     1,    86,     0,
     785,     3,   645,   105,   105,    24,    81,     1,    34,    72,
     105,    37,    34,    34,    81,    13,    78,    63,    74,    73,
      18,   109,   848,    25,   163,    51,    52,    61,    59,    81,
     848,   132,   858,    79,   136,    61,    25,   176,    34,   134,
      34,   723,   827,     1,    75,   105,    34,    73,   129,   130,
     131,   132,   133,   134,   135,   194,   567,    63,   104,    63,
      81,   105,   141,   131,   105,     3,   799,   800,   149,   136,
     132,    73,   135,   617,   134,    79,    34,   133,   124,   131,
     133,     7,   163,   105,   105,   131,   133,   131,   159,   160,
     161,   133,    86,   765,   131,   176,    69,    38,   178,   133,
     104,   133,   173,   491,   133,   376,   749,   133,   133,   105,
     104,   105,   131,   194,   136,   136,    57,   105,   124,   105,
     191,   105,   765,   105,   105,   131,    84,   131,    86,   479,
     124,   813,   203,    34,   215,     3,   131,    44,   124,     7,
     136,   280,   281,   212,   124,    60,   104,   105,   136,    64,
     134,   130,   134,   134,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   124,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   105,   716,   345,   751,   477,   478,
     105,   292,   105,   105,     3,    86,   297,   567,     7,   105,
     124,    20,   293,   611,    86,   105,   124,   308,    23,   310,
     311,   620,   313,   314,   105,   136,   609,   318,   109,   134,
      34,   134,   134,   105,   124,   308,   317,   109,   134,    38,
     239,   240,   333,    52,   105,   524,   525,   133,   339,   135,
     124,   124,    61,   134,   345,    60,   124,    61,    57,    64,
     351,   352,   134,   124,   105,   124,   357,     2,    77,   360,
      19,   362,   124,   364,   365,   366,    11,   368,   369,     2,
      29,    30,   373,   282,   283,    20,   133,    36,    11,    38,
      20,   132,   105,   292,   124,    20,   511,    20,    33,   104,
     373,   133,   107,   135,    34,    52,   111,    37,    57,    80,
      33,   124,   311,   728,    61,   314,    51,   732,   105,   105,
      60,    51,    52,   722,    64,   105,    73,    52,    51,   728,
     105,    61,   133,   732,   135,    70,    61,   124,   124,    74,
     131,   124,    23,    73,   124,   133,   107,    70,    73,   124,
     133,    74,   135,    84,   133,    86,   716,   448,   449,   450,
     451,   452,   453,   454,     3,   364,   455,   456,     7,   460,
     461,   754,   463,   360,   133,   362,    86,     7,   469,    60,
     471,    20,   124,    64,   475,   104,   124,   104,   479,   124,
     107,   134,   473,   474,   111,   124,   477,   478,   105,    34,
     133,   124,    26,   494,   105,    84,   497,   498,   134,    86,
     134,   105,   134,    52,   134,   814,   131,   508,   136,   131,
     134,   494,    61,   104,   823,   106,   107,   136,    86,   164,
     111,    26,   134,   124,    73,   508,   134,   118,    77,   105,
     124,   164,   124,   105,    16,    24,   127,    19,    24,   848,
     449,   136,   451,   188,   104,    27,   131,    29,    30,   858,
      61,   107,   197,   136,    36,   188,    38,   136,   133,    79,
     134,   468,   533,   352,   197,   702,   567,   212,   569,   570,
     571,   587,    54,   574,    56,    57,    58,    59,   156,   212,
     753,   753,   609,   754,   646,   586,   827,   813,   579,   580,
     797,    73,   593,    75,   595,   334,   773,   598,   463,   590,
     601,    -1,    -1,    -1,    -1,    16,   607,    -1,    19,   600,
      -1,    -1,   603,    -1,    -1,    -1,    27,    -1,    29,    30,
      -1,    -1,    -1,    -1,    -1,    36,   627,    38,    -1,    -1,
      -1,    -1,   633,    -1,   635,    -1,   637,    -1,   639,    -1,
      -1,   642,   124,    54,   627,    56,    57,    58,    59,    -1,
     132,    -1,    -1,    -1,    -1,   644,    -1,    -1,    -1,    -1,
     569,   570,    73,    -1,    75,   310,    -1,    -1,   313,    -1,
      -1,    -1,    -1,   318,    -1,    -1,    -1,   310,    -1,     2,
     313,    -1,    -1,    23,    -1,   318,    -1,    -1,    11,    -1,
     691,   692,    -1,   694,   695,   696,   697,    20,    -1,    -1,
      -1,    -1,    -1,   704,   705,    -1,   707,    -1,   709,   710,
      33,    -1,   713,   124,    -1,   716,   746,   747,    -1,     3,
      60,   366,    -1,     7,    64,     9,   727,   728,    51,    -1,
      -1,   732,    -1,   366,   735,    -1,    20,    -1,   739,    -1,
     731,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    34,   757,   758,   759,   760,
     761,   762,   763,   764,   104,    -1,   106,   107,    52,    -1,
      -1,   111,    -1,    -1,   775,   776,   777,    61,   118,    -1,
     781,    -1,    -1,   784,    63,   125,   126,   127,    -1,    73,
     791,    -1,    -1,    77,   777,    -1,    -1,    -1,   443,    -1,
      79,   124,    -1,    -1,    -1,     2,    -1,    86,   809,   810,
     811,   812,    71,    -1,    11,    34,    -1,    -1,    23,    -1,
      -1,    -1,   823,    20,   825,   826,   105,    -1,    -1,    -1,
     831,   832,   833,   834,   835,   836,    33,    -1,   483,    -1,
      34,   164,    -1,    -1,    63,   124,   847,   848,    -1,    -1,
     483,    -1,   131,    19,    51,    60,    -1,   858,    -1,    64,
      79,    -1,    -1,    29,    30,   188,    -1,    86,    -1,    63,
      36,    -1,    38,    70,   197,    -1,    -1,    74,    44,    -1,
      -1,    -1,    -1,    -1,   529,    79,   105,    -1,   533,   212,
     149,    57,    86,    -1,    -1,   100,   529,   102,   103,   104,
     533,   106,   107,    -1,   163,   124,   111,    -1,    -1,    -1,
      -1,   105,   131,   118,    -1,    -1,    -1,   176,    -1,    -1,
     125,   126,   127,    -1,   569,   570,    -1,   124,    -1,    -1,
     124,    -1,    -1,    -1,    10,   194,    -1,   131,    -1,    15,
       2,    -1,    -1,    -1,    -1,    -1,    -1,    23,   593,    11,
     595,    -1,    -1,   598,    -1,    -1,   601,    -1,    20,    -1,
     593,   606,   595,    -1,    -1,   598,    -1,   164,   601,    -1,
      -1,    33,    -1,   606,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   626,    -1,    -1,    60,    -1,    -1,   310,    64,    51,
     313,   188,   637,   626,   639,   318,    -1,    -1,    -1,   644,
     197,    -1,    -1,    -1,   637,    81,   639,    -1,    70,    85,
      -1,   644,    74,    -1,    -1,   212,    -1,    -1,    -1,    -1,
      -1,   280,   281,    -1,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   366,   120,   121,   122,    -1,    -1,   125,
     126,   127,    -1,    -1,    -1,    -1,    19,    -1,    -1,   704,
     136,    -1,   124,    -1,   709,    -1,    29,    30,    -1,     2,
      -1,   704,    -1,    36,    -1,    38,   709,    -1,    11,    -1,
     339,    44,    -1,    -1,   729,    -1,   345,    20,    -1,    -1,
      -1,    -1,    -1,   738,    57,    -1,   729,   742,   357,    -1,
      33,   360,   164,   362,    -1,   738,   365,    -1,    -1,   742,
      -1,    16,    -1,   310,    19,    -1,   313,    -1,    51,    -1,
      -1,   318,    27,    -1,    29,    30,   188,    -1,    -1,    -1,
      -1,    36,    -1,    38,    -1,   197,    -1,    70,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
     212,    56,    57,    58,    59,    -1,    -1,    -1,    -1,    -1,
     483,    -1,    -1,    -1,   809,   810,   811,   812,    73,   366,
      75,    -1,    -1,    -1,    -1,    -1,   809,   810,   811,   812,
     825,   826,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,
      -1,   124,   825,   826,    -1,    -1,    -1,    -1,    11,    -1,
      -1,    -1,   461,    -1,   463,    -1,   529,    20,    -1,    -1,
     533,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      33,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    10,    51,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,   310,    -1,
      23,   313,    -1,    -1,    -1,   188,   318,    70,    -1,    -1,
      -1,    74,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,
     593,    -1,   595,    -1,    -1,   598,    -1,    -1,   601,   212,
      -1,    -1,    -1,   606,    -1,    -1,   483,    60,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   626,   366,    -1,    -1,    -1,    -1,    -1,
      47,   124,    85,    -1,   637,    52,   639,    -1,    -1,    -1,
      -1,   644,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,
     103,   104,   529,   106,   107,   108,   533,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,   120,   121,   122,
      -1,   164,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   310,    -1,    -1,
     313,   704,    -1,    -1,   197,   318,   709,    -1,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,   593,    -1,   595,   212,
      -1,   598,    -1,    -1,   601,    -1,   729,    -1,    -1,   606,
      -1,    -1,    -1,    -1,    -1,   738,    -1,    -1,    -1,   742,
      -1,   483,   159,   160,   161,   162,    -1,    -1,    -1,   626,
      -1,    -1,    -1,   366,    -1,    -1,   173,    -1,    -1,    -1,
     637,    -1,   639,    -1,    -1,    -1,     2,   644,    -1,    -1,
      -1,    -1,    -1,    -1,   191,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   722,    20,    -1,   203,   529,    -1,    -1,
      -1,   533,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   809,   810,   811,   812,
      -1,    -1,    -1,    -1,    -1,    51,    -1,   310,    -1,    -1,
     313,    -1,   825,   826,    -1,   318,    -1,   704,    -1,    -1,
      -1,    -1,   709,    -1,    70,    -1,    -1,    -1,    74,    -1,
      -1,    23,   781,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   593,   729,   595,    -1,    -1,   598,    -1,    -1,   601,
      -1,   738,    -1,    -1,   606,   742,    -1,    -1,    -1,    -1,
     483,    -1,    -1,   366,    -1,   814,   293,    -1,    60,    -1,
      -1,    -1,    64,    -1,   626,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,   637,    -1,   639,    -1,    -1,
     317,    -1,   644,    -1,    -1,    -1,    -1,    -1,    -1,   848,
      -1,    -1,    -1,    -1,    23,    -1,   529,    -1,   100,   858,
     533,   103,   104,    -1,   106,   107,    -1,    -1,   164,   111,
      -1,    -1,   809,   810,   811,   812,   118,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,    -1,    -1,   825,   826,
      -1,    60,   188,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,   197,   704,    -1,    -1,    -1,    -1,   709,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   212,    -1,    -1,    -1,
     593,    -1,   595,    -1,    -1,   598,    -1,   729,   601,    -1,
     483,   100,    -1,   606,    -1,   104,   738,   106,   107,    -1,
     742,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,   626,    -1,    -1,   125,   126,   127,    -1,
      -1,    -1,    -1,    -1,   637,    -1,   639,    -1,    -1,    -1,
      -1,   644,    -1,    -1,    -1,    -1,   529,    -1,    -1,    -1,
     533,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   473,   474,    -1,    -1,
     477,   478,    -1,    -1,    -1,    -1,    -1,   809,   810,   811,
     812,    -1,    -1,    -1,   310,    -1,    -1,   313,    -1,    -1,
      -1,    -1,   318,   825,   826,    -1,    -1,    -1,     2,    -1,
      -1,   704,    -1,    -1,    -1,    -1,   709,    11,    -1,    -1,
     593,    -1,   595,    -1,    -1,   598,    20,    -1,   601,    -1,
      -1,    -1,    -1,   606,    -1,    -1,   729,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,   738,    -1,    10,    -1,   742,
     366,    -1,    15,   626,    -1,    -1,    -1,    51,    -1,    -1,
      23,    -1,    -1,    -1,   637,    -1,   639,    -1,    -1,    -1,
      -1,   644,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      74,    -1,   579,   580,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   590,    -1,   592,    -1,    60,    -1,    -1,
     597,    64,    -1,   600,    -1,    -1,   603,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   809,   810,   811,   812,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   704,   825,   826,    -1,    -1,   709,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   729,   120,   121,   122,
      -1,    -1,   125,   126,   127,   738,    -1,   483,    -1,   742,
     164,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,   188,    18,    -1,    -1,    -1,    22,
      23,    24,    -1,   197,    -1,    -1,    -1,    30,    -1,    32,
      -1,    -1,    -1,   529,    -1,    -1,    -1,   533,   212,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    -1,    -1,   731,    -1,   809,   810,   811,   812,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   825,   826,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
     767,   768,    -1,   770,   771,    -1,    -1,   593,    -1,   595,
      -1,    -1,   598,    -1,    -1,   601,    -1,    -1,   785,    -1,
     606,    -1,    -1,    -1,    -1,   792,    23,   794,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     626,    -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,   313,
      -1,   637,   145,   639,    -1,    -1,   149,    -1,   644,    -1,
     827,    -1,   155,    60,   157,    -1,    -1,    64,    -1,    -1,
     163,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,   176,    -1,   178,    -1,    -1,   181,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   366,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,    -1,    -1,   111,    -1,    -1,    -1,   704,    -1,
     117,   118,    -1,   709,    -1,   122,    -1,    -1,   125,   126,
     127,    -1,   225,    -1,    -1,    -1,    -1,   230,   231,   232,
     233,    -1,    -1,   729,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   738,    -1,   247,    -1,   742,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,   106,    -1,   280,   281,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      -1,   122,    -1,    -1,   125,   126,   127,   128,    -1,   130,
      -1,   304,    -1,   306,    -1,    -1,    -1,    -1,    -1,   483,
      -1,    -1,    -1,   809,   810,   811,   812,    -1,    -1,    -1,
      -1,    -1,   325,    -1,    -1,    -1,    -1,    -1,    -1,   825,
     826,   334,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   345,    -1,    -1,    -1,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,   529,    -1,    12,    -1,   533,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,   376,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,   593,
      -1,   595,    -1,    10,   598,    -1,    -1,   601,    15,    -1,
      85,    -1,   606,    -1,    -1,    -1,    23,    -1,    -1,    60,
      -1,    -1,    -1,    64,    -1,   100,   101,   102,   103,    -1,
      -1,    -1,   626,   108,    -1,    -1,    -1,    -1,    -1,    -1,
     463,    -1,   117,   637,   119,   639,   121,   122,    -1,    -1,
     644,    -1,   127,    60,    -1,    -1,   131,    64,   133,   100,
     135,   102,   103,   104,   487,   106,   107,   108,   491,   110,
     111,   112,   113,    -1,   115,   116,   117,   118,    85,   120,
      -1,   122,    -1,    -1,   125,   126,   127,    -1,   511,    -1,
      -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,   120,   121,   122,    -1,   540,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,    -1,    -1,    -1,    -1,   729,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   738,    -1,    -1,    -1,   742,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   587,     0,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    23,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,   611,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    60,    48,    -1,    -1,    64,    52,    53,    54,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,   662,
      75,    76,    77,    -1,    -1,    80,    -1,    82,    -1,    -1,
      85,    -1,   100,    -1,   102,   103,   104,    -1,   106,   107,
      -1,    -1,    -1,   111,    -1,   100,   101,   102,   103,   117,
     118,    -1,    -1,   108,   122,    -1,    -1,   125,   126,   127,
      -1,    -1,   117,    -1,   119,    -1,   121,   122,    -1,   124,
      -1,    -1,   127,    -1,    -1,    -1,   131,    -1,   133,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   745,    -1,    -1,    -1,    -1,    -1,    -1,     1,
     753,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      72,    73,    -1,    75,    76,    77,    -1,    -1,    80,    -1,
      82,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   124,   125,   126,   127,    -1,    -1,    -1,   131,
     132,   133,     1,   135,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    72,    73,    -1,    75,    76,    77,    -1,
      -1,    80,    -1,    82,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,   102,   103,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
     119,    -1,   121,   122,   123,   124,    -1,    -1,   127,    -1,
      -1,    -1,   131,   132,   133,     1,   135,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    73,    -1,    75,
      76,    77,    -1,    -1,    80,    -1,    82,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,   119,    -1,   121,   122,    -1,   124,    -1,
      -1,   127,    -1,    -1,    -1,   131,   132,   133,     1,   135,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    54,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    -1,    75,    76,    77,    -1,    -1,    80,    -1,    82,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,   119,    -1,   121,   122,
      -1,   124,    -1,    -1,   127,    -1,    -1,    -1,   131,   132,
     133,     1,   135,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    54,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    -1,    75,    76,    77,    -1,    -1,
      80,    -1,    82,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,   119,
      -1,   121,   122,    -1,   124,    -1,    -1,   127,    -1,    -1,
      -1,   131,   132,   133,     1,   135,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    -1,    72,    73,    -1,    75,    76,
      77,    -1,    -1,    80,    -1,    82,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,   119,    -1,   121,   122,    -1,   124,    -1,    -1,
     127,    -1,    -1,    -1,   131,   132,   133,     1,   135,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    73,
      -1,    75,    76,    77,    -1,    -1,    80,    -1,    82,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,   119,    -1,   121,   122,    -1,
     124,    -1,    -1,   127,    -1,    -1,    -1,   131,    -1,   133,
       1,   135,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    -1,    75,    76,    77,    -1,    -1,    80,
      -1,    82,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,   119,    -1,
     121,   122,    -1,   124,    -1,    -1,   127,    -1,    -1,    -1,
     131,    -1,   133,     1,   135,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    -1,    75,    76,    77,
      -1,    -1,    80,    -1,    82,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,   119,    -1,   121,   122,    -1,   124,    -1,    -1,   127,
      -1,    -1,    -1,   131,    -1,   133,     1,   135,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    72,    73,    -1,
      75,    76,    77,    -1,    -1,    80,    -1,    82,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,   119,    -1,   121,   122,    -1,   124,
      -1,    -1,   127,    -1,    -1,    -1,   131,    -1,   133,     1,
     135,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    73,    -1,    75,    76,    77,    -1,    -1,    80,    -1,
      82,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,   119,    -1,   121,
     122,    -1,   124,    -1,    -1,   127,    -1,    -1,    -1,   131,
      -1,   133,     1,   135,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    72,    73,    -1,    75,    76,    77,    -1,
      -1,    80,    -1,    82,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,   102,   103,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
     119,    -1,   121,   122,    -1,   124,    -1,    -1,   127,    -1,
      -1,    -1,   131,    -1,   133,     1,   135,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    73,    -1,    75,
      76,    77,    -1,    -1,    80,    -1,    82,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,   119,    -1,   121,   122,    -1,   124,    -1,
      -1,   127,    -1,    -1,    -1,   131,    -1,   133,     1,   135,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    -1,    75,    76,    77,    -1,    -1,    80,    -1,    82,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,   119,    -1,   121,   122,
      -1,   124,    -1,    -1,   127,    -1,    -1,    -1,   131,    -1,
     133,     1,   135,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    -1,    75,    76,    77,    -1,    -1,
      80,    -1,    82,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,   119,
      -1,   121,   122,    -1,   124,    -1,    -1,   127,    -1,    -1,
      -1,   131,    -1,   133,     1,   135,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    -1,    72,    73,    -1,    75,    76,
      77,    -1,    -1,    80,    -1,    82,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,   119,    -1,   121,   122,    -1,   124,    -1,    -1,
     127,    -1,    -1,    -1,   131,    -1,   133,     1,   135,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    73,
      -1,    75,    76,    77,    -1,    -1,    80,    -1,    82,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,   119,    -1,   121,   122,    -1,
     124,    -1,    -1,   127,    -1,    -1,    -1,   131,    -1,   133,
       1,   135,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,   102,   103,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,    -1,   125,   126,   127,    -1,    -1,    -1,
     131,    -1,   133,     1,   135,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,   103,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    -1,    -1,   125,   126,   127,
      -1,    -1,    -1,   131,    -1,   133,     1,   135,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,   128,   129,
     130,    -1,   117,   133,   119,   135,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,   131,    -1,   133,    -1,
     135,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,   119,    -1,   121,
     122,   123,    -1,    -1,    -1,   127,    -1,    -1,    -1,   131,
      -1,   133,    -1,   135,   136,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,   119,    -1,   121,   122,   123,    -1,    -1,    -1,   127,
      -1,    -1,    -1,   131,    -1,   133,    -1,   135,   136,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,   119,    -1,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,   131,    -1,   133,
      -1,   135,   136,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    34,    35,    -1,    37,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,   119,
      -1,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,   131,    -1,   133,    -1,   135,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,   119,    -1,   121,   122,   123,    -1,    -1,    -1,
     127,    -1,    -1,    -1,   131,    -1,   133,    -1,   135,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,   119,    -1,   121,   122,   123,
      -1,    -1,    -1,   127,    -1,    -1,    -1,   131,    -1,   133,
     134,   135,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,   119,    -1,
     121,   122,   123,    -1,    -1,    -1,   127,    -1,    -1,    -1,
     131,    -1,   133,   134,   135,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,   119,    -1,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,   131,    -1,   133,    -1,   135,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,   119,    -1,   121,   122,   123,    -1,
      -1,    -1,   127,    -1,    -1,    -1,   131,   132,   133,    -1,
     135,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,   119,    -1,   121,
     122,   123,    -1,    -1,    -1,   127,    -1,    -1,    -1,   131,
      -1,   133,    -1,   135,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,   102,   103,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
     119,    -1,   121,   122,    -1,   124,    -1,    -1,   127,    -1,
      -1,    -1,   131,    -1,   133,    -1,   135,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,   119,    -1,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,   131,    -1,   133,    -1,   135,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,   119,    -1,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,   131,    -1,
     133,    -1,   135,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    47,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,   119,
      -1,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,   131,    -1,   133,    -1,   135,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,   119,    -1,   121,   122,   123,    -1,    -1,    -1,
     127,    -1,    -1,    -1,   131,    -1,   133,    -1,   135,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,   119,    -1,   121,   122,   123,
      -1,    -1,    -1,   127,    -1,    -1,    -1,   131,    -1,   133,
      -1,   135,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,   119,    -1,
     121,   122,   123,    -1,    -1,    -1,   127,    -1,    -1,    -1,
     131,    -1,   133,    -1,   135,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,   119,    -1,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,   131,    -1,   133,    -1,   135,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,   119,    -1,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,   131,    -1,   133,    -1,
     135,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,   119,    -1,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,   131,
      -1,   133,    -1,   135,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,   102,   103,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
     119,    -1,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,   131,    -1,   133,    -1,   135,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,   119,    -1,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,   131,    -1,   133,    -1,   135,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,   119,    -1,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,   131,    -1,
     133,    -1,   135,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,   119,
      -1,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,   131,    -1,   133,    -1,   135,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,   119,    -1,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,   131,    -1,   133,    -1,   135,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,   119,    -1,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,   131,    -1,   133,
      -1,   135,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,   119,    -1,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
     131,    -1,   133,    -1,   135,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,   117,
      35,   119,    -1,    -1,   122,    40,    41,    -1,    43,   127,
      45,    46,    -1,   131,    -1,   133,    -1,   135,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    23,    -1,    12,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,   101,    31,    32,    33,
     105,    35,    -1,    -1,   109,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    60,   119,    -1,    -1,    64,   123,    53,
      -1,    55,    -1,    -1,    -1,    -1,   131,    10,   133,   134,
     135,    -1,    15,    67,    68,    69,    70,    -1,    85,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,    -1,   110,   111,   112,   113,   101,   115,   116,
     117,   118,    -1,   120,   121,   122,    -1,    60,   125,   126,
     127,    64,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,   133,
      -1,   135,    85,    -1,    23,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,   120,   121,   122,
      -1,    60,   125,   126,   127,    64,    -1,    -1,    -1,    -1,
      10,   134,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    23,    24,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      60,   120,   121,   122,    64,    -1,   125,   126,   127,    10,
      -1,    -1,   131,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    24,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    60,
     120,   121,   122,    64,    -1,   125,   126,   127,    10,    -1,
      -1,   131,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    23,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    60,   120,
     121,   122,    64,    -1,   125,   126,   127,    10,    -1,    71,
     131,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    24,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    60,   120,   121,
     122,    64,    -1,   125,   126,   127,    10,    -1,    -1,   131,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      24,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    60,   120,   121,   122,
      64,    -1,   125,   126,   127,    10,    -1,    -1,   131,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    60,   120,   121,   122,    64,
      -1,   125,   126,   127,    10,    -1,    71,   131,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    60,   120,   121,   122,    64,    -1,
     125,   126,   127,    10,    -1,    71,   131,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    60,   120,   121,   122,    64,    -1,   125,
     126,   127,    10,    -1,    71,   131,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    60,   120,   121,   122,    64,    -1,   125,   126,
     127,    10,    -1,    71,   131,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    60,   120,   121,   122,    64,    -1,   125,   126,   127,
      10,    -1,    71,   131,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      60,   120,   121,   122,    64,    -1,   125,   126,   127,    10,
      -1,    71,   131,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    60,
     120,   121,   122,    64,    -1,   125,   126,   127,    10,    -1,
      71,   131,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    60,   120,
     121,   122,    64,    -1,   125,   126,   127,    10,    -1,    71,
     131,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    60,   120,   121,
     122,    64,    -1,   125,   126,   127,    10,    -1,    -1,   131,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    60,   120,   121,   122,
      64,    -1,   125,   126,   127,    10,    -1,    -1,   131,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    60,   120,   121,   122,    64,
      -1,   125,   126,   127,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    10,   120,   121,   122,    -1,    15,
     125,   126,   127,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    60,   120,   121,   122,    64,    -1,   125,
     126,   127,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    24,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    60,   120,   121,   122,    64,    -1,   125,   126,
     127,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    60,   120,   121,   122,    64,    -1,   125,   126,   127,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      60,   120,   121,   122,    64,   124,   125,   126,   127,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    60,
     120,   121,   122,    64,   124,   125,   126,   127,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    60,   120,
     121,   122,    64,   124,   125,   126,   127,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    60,   120,   121,
     122,    64,    -1,   125,   126,   127,    10,    -1,    71,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    60,   120,   121,   122,
      64,    -1,   125,   126,   127,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    60,   120,   121,   122,    64,
     124,   125,   126,   127,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    60,   120,   121,   122,    64,   124,
     125,   126,   127,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    60,   120,   121,   122,    64,   124,   125,
     126,   127,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    60,   120,   121,   122,    64,   124,   125,   126,
     127,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    60,   120,   121,   122,    64,   124,   125,   126,   127,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      60,   120,   121,   122,    64,   124,   125,   126,   127,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    60,
     120,   121,   122,    64,   124,   125,   126,   127,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    60,   120,
     121,   122,    64,   124,   125,   126,   127,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    60,   120,   121,
     122,    64,   124,   125,   126,   127,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    60,   120,   121,   122,
      64,   124,   125,   126,   127,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    60,   120,   121,   122,    64,
     124,   125,   126,   127,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    60,   120,   121,   122,    64,   124,
     125,   126,   127,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    60,   120,   121,   122,    64,   124,   125,
     126,   127,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    60,   120,   121,   122,    64,   124,   125,   126,
     127,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    60,   120,   121,   122,    64,   124,   125,   126,   127,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      60,   120,   121,   122,    64,   124,   125,   126,   127,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    23,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    60,
     120,   121,   122,    64,    -1,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    -1,   120,
     121,   122,    -1,   124,   125,   126,   127,    60,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    60,   120,   121,   122,
      64,    -1,   125,   126,   127,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    60,   120,   121,   122,    64,
      -1,   125,   126,   127,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    60,   120,   121,   122,    64,    -1,
     125,   126,   127,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    60,   120,   121,   122,    64,    -1,   125,
     126,   127,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    60,   120,   121,   122,    64,    -1,   125,   126,
     127,    10,    -1,    71,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    60,   120,   121,   122,    64,    -1,   125,   126,   127,
      10,    11,    71,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      60,   120,   121,   122,    64,    -1,   125,   126,   127,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    60,
     120,   121,   122,    64,    -1,   125,   126,   127,    10,    -1,
      71,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    60,   120,
     121,   122,    64,    -1,   125,   126,   127,    10,    -1,    71,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    60,   120,   121,
     122,    64,    -1,   125,   126,   127,    10,    -1,    71,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    60,   120,   121,   122,
      64,    -1,   125,   126,   127,    10,    -1,    71,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    60,   120,   121,   122,    64,
      -1,   125,   126,   127,    10,    -1,    71,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    60,   120,   121,   122,    64,    -1,
     125,   126,   127,    10,    -1,    71,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    60,   120,   121,   122,    64,    -1,   125,
     126,   127,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    60,   120,   121,   122,    64,    -1,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    60,   120,   121,   122,    64,    -1,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,    -1,   115,   116,   117,   118,
      60,   120,    -1,   122,    64,    -1,   125,   126,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,    -1,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,    -1,   125,   126,   127
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   141,   142,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    16,    17,    18,    19,    21,    22,
      24,    25,    27,    29,    30,    31,    32,    33,    35,    36,
      39,    40,    41,    42,    43,    45,    46,    48,    53,    54,
      55,    56,    58,    59,    62,    63,    65,    66,    67,    68,
      69,    70,    72,    73,    75,    76,    80,    82,    85,   100,
     101,   102,   103,   108,   117,   119,   121,   122,   124,   127,
     131,   133,   135,   143,   144,   145,   146,   147,   148,   153,
     154,   155,   157,   160,   161,   162,   163,   164,   166,   167,
     168,   171,   172,   175,   178,   181,   182,   203,   206,   207,
     225,   226,   227,   228,   229,   230,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   248,   249,   250,   251,
     252,   145,   240,    81,   231,   232,   156,   157,   231,    12,
      31,    32,    33,    70,   131,   135,   178,   225,   229,   238,
     239,   240,   241,   243,   244,    73,   156,   240,   145,   133,
     157,     7,   156,   158,     9,    73,   158,    52,    83,   165,
     240,   240,   240,   133,   157,   179,   133,   157,   208,   209,
     145,   240,   240,   240,   240,     7,   133,    19,    30,   162,
     162,   123,   199,   219,   240,   124,   240,   240,    24,   148,
     159,   240,   240,    69,   133,   145,   240,   131,   148,   157,
     204,   219,   240,   240,   240,   240,   240,   240,   240,   240,
     132,   143,   149,   219,    74,   109,   199,   220,   221,   240,
     219,   240,   247,    54,   145,    44,   157,    38,    57,   195,
      20,    52,    61,    77,   124,   130,    10,    15,    23,    60,
      64,    85,   100,   102,   103,   104,   106,   107,   108,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   120,   121,
     122,   125,   126,   127,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   128,   129,
     133,   135,    60,    64,   133,   145,   124,   148,   240,   240,
     240,   219,    34,   231,   204,   124,   124,    80,   157,   199,
     222,   223,   224,   240,   131,   204,   172,   157,   133,   159,
      24,    34,   159,    24,    34,    81,   159,   234,    71,   148,
     222,   145,   133,   189,    74,   133,   157,   210,   211,     1,
     104,   213,   214,    34,   105,   159,   222,   158,   157,   105,
     124,   124,   131,   145,   159,   133,   222,    86,   205,   124,
     124,    28,    49,   159,   124,   132,   143,   105,   132,   240,
     105,   134,   105,   134,    34,   105,   136,   234,    84,   105,
     136,     7,   157,   104,   173,   183,    61,   208,   208,   208,
     208,   240,   240,   240,   240,   165,   240,   165,   240,   240,
     240,   240,   240,   240,   240,    25,    73,   157,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     222,   222,   165,   240,   165,   240,    20,    34,    37,    51,
      52,    61,    73,   193,   233,   236,   240,   252,    24,    34,
      24,    34,    71,    34,   136,   165,   240,   159,   124,   240,
      84,    86,   134,   105,   157,   176,   177,   124,   157,    34,
     219,    33,   240,   165,   240,    33,   240,   165,   240,   133,
     159,   145,   240,    26,   134,   190,   191,   192,   193,   180,
     211,   105,   134,     1,   135,   215,   226,    84,    86,   212,
     240,   209,   134,   199,   240,   169,   222,   134,   135,   215,
     226,   105,   127,   150,   152,   240,   150,   151,   132,   134,
     134,   220,   134,   220,   165,   240,   136,   145,   240,   136,
     240,   136,   240,   131,   219,   131,    52,    61,    73,   185,
     194,   208,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   134,   136,    34,    61,   225,   105,   134,    60,
      60,    33,   165,   240,    33,   165,   240,   165,   240,   231,
     231,   124,   240,   199,   240,   224,    86,   105,   132,   173,
     240,   134,   240,    24,   159,    24,   159,   240,    24,   159,
     234,    24,   159,   234,   235,   236,    26,    26,   145,   105,
     134,   133,   157,    20,    52,    61,    73,   196,   134,   211,
     104,   214,   219,   240,    47,   240,    50,    78,   132,   170,
     134,   219,   204,   105,   124,    11,   124,   136,   234,   136,
     234,   145,    84,   132,   149,   174,   184,    10,    15,    86,
      87,    88,    89,    90,    91,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   106,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   122,   125,   126,
     127,   128,   130,   157,   186,   187,   124,   236,   225,   225,
     240,    24,    24,   240,    24,    24,   136,   136,   159,   159,
     240,   177,   131,   159,    71,    33,   240,    33,   240,    71,
      33,   240,   159,    33,   240,   159,   105,   134,   145,   240,
     191,   211,   104,   218,    61,   214,   212,    34,   136,    24,
     145,   219,   136,   240,   240,    33,   145,   240,   136,    33,
     145,   240,   136,   240,   132,    30,    56,    58,   132,   144,
     161,   196,   157,   107,   133,   188,   188,    71,    33,    33,
      71,    33,    33,    33,    33,   174,   240,   240,   240,   240,
     240,   240,   236,   134,    25,    67,    70,   135,   199,   217,
     226,   109,   201,   212,    79,   202,   240,   215,   226,   145,
     159,    11,   240,   145,   240,   145,   161,   214,   157,   186,
     187,   190,   240,   240,   240,   240,   240,   240,   132,    71,
      71,    71,    71,   218,   136,   219,   199,   200,   240,   240,
     148,   160,   198,   136,   240,    71,    71,   202,   188,   188,
     134,    71,    71,    71,    71,    71,    71,   240,   240,   240,
     240,   201,   212,   199,   216,   217,   226,    34,   136,   226,
     240,   240,   124,   197,   198,   240,   216,   217,   136,   216
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   140,   141,   142,   142,   143,   143,   144,   144,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   146,   146,   147,   147,
     147,   148,   148,   149,   149,   150,   150,   150,   150,   151,
     151,   152,   152,   153,   153,   153,   154,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   156,   156,   157,   158,   158,   159,   159,
     160,   160,   161,   161,   161,   161,   161,   161,   161,   162,
     162,   163,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   165,   166,   166,   166,   166,   167,
     168,   169,   169,   170,   170,   170,   171,   171,   172,   172,
     172,   173,   173,   174,   174,   174,   175,   176,   176,   176,
     177,   177,   179,   180,   178,   181,   181,   181,   181,   183,
     184,   182,   185,   185,   185,   185,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   188,   188,   189,   190,   190,   190,
     191,   191,   191,   191,   192,   192,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   194,   194,   194,   194,   195,
     195,   196,   196,   196,   196,   196,   196,   197,   197,   198,
     198,   199,   199,   200,   200,   201,   201,   202,   202,   203,
     203,   203,   204,   204,   205,   205,   205,   206,   206,   206,
     206,   206,   207,   207,   207,   208,   208,   209,   209,   209,
     210,   210,   210,   211,   211,   211,   212,   212,   212,   213,
     213,   214,   214,   214,   214,   215,   215,   215,   215,   216,
     216,   216,   217,   217,   217,   217,   217,   218,   218,   218,
     218,   218,   218,   218,   219,   219,   219,   219,   220,   220,
     220,   221,   221,   222,   222,   223,   223,   224,   224,   224,
     224,   224,   225,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   228,   229,   230,   230,   230,   230,   230,   230,
     230,   230,   231,   231,   232,   233,   233,   234,   235,   235,
     236,   236,   236,   237,   237,   238,   239,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   241,   241,   241,   241,   242,   242,   243,   243,   243,
     244,   244,   244,   245,   245,   245,   245,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   247,   247,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   249,   249,   249,   249,   249,   249,   250,
     250,   250,   250,   251,   251,   251,   251,   252,   252,   252,
     252,   252,   252,   252
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     3,     3,     3,     3,     3,     2,
       3,     3,     2,     2,     3,     1,     5,     6,     0,     1,
       1,     2,     3,     1,     2,     1,     3,     3,     5,     0,
       1,     1,     1,     3,     5,     5,     3,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     0,     1,     1,     1,     1,     2,     1,
       2,     3,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     2,     5,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     6,     7,
       6,     7,     4,     5,     4,     4,     3,     6,     5,     2,
       5,     0,     2,     3,     2,     3,     6,     8,     1,     1,
       1,     0,     2,     0,     2,     3,     5,     1,     2,     3,
       1,     3,     0,     0,     8,     0,     1,     2,     2,     0,
       0,     9,     3,     3,     5,     5,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     3,     0,     1,     3,
       4,     4,     6,     6,     0,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     0,     1,     1,     1,     1,
       1,     0,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     0,     1,     2,     2,     0,     2,     3,
       4,     4,     2,     4,     0,     2,     2,     4,     4,     4,
       5,     4,     0,     1,     1,     1,     3,     3,     4,     5,
       1,     1,     3,     1,     2,     3,     0,     2,     2,     0,
       4,     0,     2,     2,     1,     4,     4,     6,     1,     0,
       1,     1,     3,     4,     3,     4,     6,     0,     2,     2,
       2,     2,     2,     2,     1,     1,     3,     3,     1,     1,
       1,     3,     3,     0,     1,     1,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     4,
       4,     4,     5,     2,     2,     6,     6,     4,     9,     9,
       7,     6,     6,     4,     9,     9,     7,     4,     6,     6,
       9,     9,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     1,     4,     1,     3,     4,     1,     3,
       2,     3,     3,     1,     3,     2,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     3,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     4,
       3,     3,     3,     3,     4,     3,     4,     1,     1,     1,
       1,     1,     3,     4,     3,     4,     3,     4,     3,     5,
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
#line 423 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 4763 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 428 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4769 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 429 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4775 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 436 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4781 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 441 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4787 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 442 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4793 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 459 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4799 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4805 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 461 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4811 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 462 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4817 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 463 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4823 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 464 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4829 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4835 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 466 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4841 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 467 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4847 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 468 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4853 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 469 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4859 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 470 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4865 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4871 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 472 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4877 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 473 "chapel.ypp" /* yacc.c:1661  */
    { printf("syntax error"); clean_exit(1); }
#line 4883 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yyvsp[-4].b), (yylsp[-4]).comment))); }
#line 4889 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 480 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-5].b), (yylsp[-5]).comment))); }
#line 4895 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4901 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4907 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4913 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 501 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 4919 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 502 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 4925 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 507 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4931 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 508 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4937 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 513 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 4947 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 518 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 4957 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 523 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 4966 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 527 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 4975 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 533 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 4985 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 542 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 4995 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 551 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5001 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 552 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5007 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5013 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 557 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5019 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 561 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 5025 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 5031 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 5037 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 5043 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 5049 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 5055 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 567 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 5061 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 568 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 5067 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 5073 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 570 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 5079 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 5085 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 5091 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5097 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5103 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5109 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5115 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 580 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5121 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 585 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5127 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 589 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5133 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 590 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5139 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 594 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5145 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 595 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5151 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 599 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 5157 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5163 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5169 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5175 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 607 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5181 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 620 "chapel.ypp" /* yacc.c:1661  */
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
#line 5197 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5203 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5209 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 636 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5215 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5221 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5227 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5233 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5239 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5245 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5251 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 643 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5257 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 644 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5263 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 645 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt)); }
#line 5269 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), zipToTuple((yyvsp[-1].pcallexpr)), NULL, (yyvsp[0].pblockstmt), true); }
#line 5275 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt), true); }
#line 5281 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 648 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5287 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 649 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt)); }
#line 5293 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 651 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5303 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 657 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5313 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 663 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5323 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 669 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-3].pcallexpr)), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5333 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 675 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5344 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 682 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5355 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 691 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5361 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 695 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5367 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5373 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5379 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5385 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 702 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 5391 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5397 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 711 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5403 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 712 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5409 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 717 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5415 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 719 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5421 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 721 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5427 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 728 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), NULL, (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5436 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 733 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-6].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5445 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 740 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5455 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 745 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5465 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 750 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5475 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 758 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 5481 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 759 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 5487 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5493 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 766 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 5499 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 768 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 5505 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 773 "chapel.ypp" /* yacc.c:1661  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 5516 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 783 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[0].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[0].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
#line 5529 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 792 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 5537 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 796 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 5546 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 803 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 5552 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 804 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 5558 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 809 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5567 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 814 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5576 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 819 "chapel.ypp" /* yacc.c:1661  */
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
#line 5596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 839 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5607 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 845 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5619 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 852 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5632 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 860 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5645 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 871 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5655 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 877 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 884 "chapel.ypp" /* yacc.c:1661  */
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
#line 5689 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 906 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5697 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 910 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5706 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 915 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
    }
#line 5714 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 919 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5723 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 927 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 928 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 5735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 929 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 5741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 930 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 5747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 931 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 5753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 932 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 5759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 933 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 5765 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 934 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 5771 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 935 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 5777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 936 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 5783 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 937 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 5789 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 938 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 5795 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 939 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 5801 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 940 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 5807 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 941 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 5813 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 942 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 5819 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 943 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 5825 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 944 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 5831 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 945 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 5837 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 946 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 5843 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 947 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 5849 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 948 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 5855 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 949 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 5861 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 950 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 5867 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 951 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 5873 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 955 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 5879 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 956 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 5885 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 957 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 5891 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 958 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 5897 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 959 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 5903 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 960 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 5909 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 961 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 5915 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 962 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 5921 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 963 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 5927 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 964 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 5933 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 965 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 5939 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 966 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 5945 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 970 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 5951 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 971 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5957 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 975 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5963 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 979 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 5969 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 980 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 5975 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 981 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 5981 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 986 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 5987 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 988 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 5993 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 990 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 5999 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 992 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6005 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 996 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6011 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 997 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6017 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1001 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 6023 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1002 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6029 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1003 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 6035 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1004 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 6041 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1005 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6047 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1006 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6053 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1007 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6059 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1008 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 6065 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1009 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6071 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1013 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6077 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1014 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6083 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1015 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 6089 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1016 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6095 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1020 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6101 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1021 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6107 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1025 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6113 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1026 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6119 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1027 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6125 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1028 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 6131 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1029 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 6137 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1030 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 6143 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1034 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 6149 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1040 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 6155 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1045 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6161 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1047 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6167 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1051 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6173 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1056 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6179 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1057 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6185 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1061 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6191 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1062 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6197 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1067 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 6203 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1069 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 6209 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1071 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 6215 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1076 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 6232 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1089 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 6250 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1105 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6256 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1107 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6262 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1109 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 6268 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1114 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6280 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1122 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6292 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1130 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6304 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1138 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 6317 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1147 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6328 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1156 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6334 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1157 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6340 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1158 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6346 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1164 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6355 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1172 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6361 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1174 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 6371 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1180 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6377 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1185 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 6383 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1187 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6389 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1189 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 6395 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 6401 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1196 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6407 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6413 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1204 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6419 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1205 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 6425 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1206 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6431 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1210 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6437 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1212 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6443 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1216 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6449 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1217 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6455 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1218 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 6461 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1219 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad type specification"); }
#line 6467 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1240 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 6475 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1244 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 6483 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1248 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 6495 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1255 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad array type specification"); clean_exit(1); }
#line 6501 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1259 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6507 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1260 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6513 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1261 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6519 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1266 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6525 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1268 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6531 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1274 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6537 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1276 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6543 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1278 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 6549 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1282 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6555 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1283 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6561 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1284 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6567 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1285 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 6573 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1286 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6579 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1287 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6585 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1288 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6591 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1294 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6597 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1295 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6603 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1296 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6609 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1297 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6615 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1301 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6621 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1302 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6627 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1303 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6633 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1307 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6639 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1308 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6645 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1312 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6651 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1317 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6657 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1318 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6663 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1322 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6669 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1323 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6675 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1324 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6681 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1325 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6687 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1330 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6693 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1347 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1349 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6705 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1351 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1353 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1355 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 6723 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1357 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1359 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1364 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1366 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 6747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1368 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1370 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1372 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6765 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1374 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6771 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1376 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1378 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 6783 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1380 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6789 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1382 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6795 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1384 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6801 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1386 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6807 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1388 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 6818 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1395 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 6828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1401 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 6838 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1407 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 6848 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1413 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 6858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1422 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 6864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1426 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 6870 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1442 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6876 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1446 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6882 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1451 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 6888 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1457 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 6895 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1460 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 6901 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1464 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pForallIntents) = (yyvsp[-1].pForallIntents); }
#line 6907 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1470 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pForallIntents) = new ForallIntents();
      addForallIntent((yyval.pForallIntents), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 6914 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1473 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pForallIntents), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 6920 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1478 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[-1].pt); (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
#line 6928 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).riExp = (yyvsp[-2].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); }
#line 6934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).riExp = (yyvsp[-2].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); }
#line 6940 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6946 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 6952 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 6958 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 6964 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6970 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1518 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6976 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1520 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 6982 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 6988 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1524 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 6994 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 387:
#line 1547 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7000 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 388:
#line 1548 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7006 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1549 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 7012 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7018 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 7024 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 7030 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7036 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 7042 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 7048 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 7054 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1570 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 7060 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 7066 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 7072 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 7078 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 7084 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 7090 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 7096 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 7102 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 7108 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1580 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 7116 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1584 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 7124 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1591 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7130 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1592 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7136 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7142 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1597 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7148 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7154 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1599 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7160 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7166 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1601 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7172 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7178 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1603 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7184 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7190 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1605 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7196 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7202 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7208 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7214 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1609 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7220 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7226 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1611 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7232 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7238 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1613 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7244 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7250 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1615 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7256 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7262 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1617 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7268 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7274 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 7280 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 7286 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1624 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 7292 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1625 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 7298 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 7304 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 7310 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7316 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7322 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7328 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7334 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7340 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7346 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7352 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7358 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 7364 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 7370 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1648 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 7376 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1649 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 7382 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1650 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 7388 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1651 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 7394 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 453:
#line 1652 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 7400 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 7404 "bison-chapel.cpp" /* yacc.c:1661  */
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
