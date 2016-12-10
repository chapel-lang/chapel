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
    TTHROWS = 327,
    TTRY = 328,
    TTYPE = 329,
    TUNDERSCORE = 330,
    TUNION = 331,
    TUSE = 332,
    TVAR = 333,
    TWHEN = 334,
    TWHERE = 335,
    TWHILE = 336,
    TWITH = 337,
    TYIELD = 338,
    TZIP = 339,
    TALIAS = 340,
    TAND = 341,
    TASSIGN = 342,
    TASSIGNBAND = 343,
    TASSIGNBOR = 344,
    TASSIGNBXOR = 345,
    TASSIGNDIVIDE = 346,
    TASSIGNEXP = 347,
    TASSIGNLAND = 348,
    TASSIGNLOR = 349,
    TASSIGNMINUS = 350,
    TASSIGNMOD = 351,
    TASSIGNMULTIPLY = 352,
    TASSIGNPLUS = 353,
    TASSIGNSL = 354,
    TASSIGNSR = 355,
    TBAND = 356,
    TBNOT = 357,
    TBOR = 358,
    TBXOR = 359,
    TCOLON = 360,
    TCOMMA = 361,
    TDIVIDE = 362,
    TDOT = 363,
    TDOTDOT = 364,
    TDOTDOTDOT = 365,
    TEQUAL = 366,
    TEXP = 367,
    TGREATER = 368,
    TGREATEREQUAL = 369,
    THASH = 370,
    TLESS = 371,
    TLESSEQUAL = 372,
    TMINUS = 373,
    TMOD = 374,
    TNOT = 375,
    TNOTEQUAL = 376,
    TOR = 377,
    TPLUS = 378,
    TQUESTION = 379,
    TSEMI = 380,
    TSHIFTLEFT = 381,
    TSHIFTRIGHT = 382,
    TSTAR = 383,
    TSWAP = 384,
    TASSIGNREDUCE = 385,
    TIO = 386,
    TLCBR = 387,
    TRCBR = 388,
    TLP = 389,
    TRP = 390,
    TLSBR = 391,
    TRSBR = 392,
    TNOELSE = 393,
    TUPLUS = 394,
    TUMINUS = 395
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

#line 412 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 418 "bison-chapel.cpp" /* yacc.c:358  */
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

#line 485 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   12911

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  141
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  114
/* YYNRULES -- Number of rules.  */
#define YYNRULES  455
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  862

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   395

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
     135,   136,   137,   138,   139,   140
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   424,   424,   429,   430,   436,   437,   442,   443,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   478,   480,   485,   486,
     487,   502,   503,   508,   509,   514,   519,   524,   528,   534,
     539,   543,   548,   552,   553,   554,   558,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   581,   582,   586,   590,   591,   595,   596,
     600,   601,   605,   606,   607,   608,   609,   610,   611,   615,
     616,   620,   635,   636,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,   651,   657,
     663,   669,   675,   682,   692,   696,   697,   698,   699,   703,
     707,   712,   713,   717,   719,   721,   728,   733,   741,   746,
     751,   759,   760,   765,   766,   768,   773,   783,   792,   796,
     804,   805,   810,   815,   809,   840,   846,   853,   861,   872,
     878,   871,   906,   910,   915,   919,   927,   928,   929,   930,
     931,   932,   933,   934,   935,   936,   937,   938,   939,   940,
     941,   942,   943,   944,   945,   946,   947,   948,   949,   950,
     951,   952,   956,   957,   958,   959,   960,   961,   962,   963,
     964,   965,   966,   967,   971,   972,   976,   980,   981,   982,
     986,   988,   990,   992,   997,   998,  1002,  1003,  1004,  1005,
    1006,  1007,  1008,  1009,  1010,  1014,  1015,  1016,  1017,  1021,
    1022,  1026,  1027,  1028,  1029,  1030,  1031,  1035,  1036,  1039,
    1040,  1044,  1045,  1049,  1051,  1056,  1057,  1061,  1062,  1066,
    1067,  1071,  1073,  1075,  1080,  1093,  1110,  1111,  1113,  1118,
    1126,  1134,  1142,  1151,  1161,  1162,  1163,  1167,  1168,  1176,
    1178,  1184,  1189,  1191,  1193,  1198,  1200,  1202,  1209,  1210,
    1211,  1215,  1216,  1221,  1222,  1223,  1224,  1244,  1248,  1252,
    1260,  1264,  1265,  1266,  1270,  1272,  1278,  1280,  1282,  1287,
    1288,  1289,  1290,  1291,  1292,  1293,  1299,  1300,  1301,  1302,
    1306,  1307,  1308,  1312,  1313,  1317,  1318,  1322,  1323,  1327,
    1328,  1329,  1330,  1331,  1335,  1346,  1347,  1348,  1349,  1350,
    1351,  1353,  1355,  1357,  1359,  1361,  1363,  1368,  1370,  1372,
    1374,  1376,  1378,  1380,  1382,  1384,  1386,  1388,  1390,  1392,
    1399,  1405,  1411,  1417,  1426,  1431,  1439,  1440,  1441,  1442,
    1443,  1444,  1445,  1446,  1451,  1452,  1456,  1461,  1464,  1469,
    1474,  1477,  1482,  1486,  1487,  1491,  1492,  1497,  1502,  1510,
    1511,  1512,  1513,  1514,  1515,  1516,  1517,  1518,  1520,  1522,
    1524,  1526,  1528,  1533,  1534,  1535,  1536,  1547,  1548,  1552,
    1553,  1554,  1558,  1559,  1560,  1568,  1569,  1570,  1571,  1575,
    1576,  1577,  1578,  1579,  1580,  1581,  1582,  1583,  1584,  1588,
    1596,  1597,  1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,
    1609,  1610,  1611,  1612,  1613,  1614,  1615,  1616,  1617,  1618,
    1619,  1620,  1621,  1622,  1623,  1627,  1628,  1629,  1630,  1631,
    1632,  1636,  1637,  1638,  1639,  1643,  1644,  1645,  1646,  1651,
    1652,  1653,  1654,  1655,  1656,  1657
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
  "TSERIAL", "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN",
  "TTHROWS", "TTRY", "TTYPE", "TUNDERSCORE", "TUNION", "TUSE", "TVAR",
  "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND",
  "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
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
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
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
     395
};
# endif

#define YYPACT_NINF -760

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-760)))

#define YYTABLE_NINF -407

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -760,    77,  2575,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,  3647,    21,    90,  -760,    21,  7536,    95,    90,  7536,
    3647,     0,    90,   213,   324,  6196,  7536,  7536,    30,  -760,
      90,  -760,    29,  3647,  7536,  7536,  -760,  7536,  7536,   180,
      76,   505,   635,  -760,  6464,  6598,  7536,  6732,  7536,   131,
     140,  3647,   148,    90,  -760,  6464,  7536,  7536,  -760,  -760,
    7536,  -760,  -760,  2203,  7536,  7536,  -760,  7536,  -760,  -760,
    2843,  5794,  6464,  -760,  3513,  -760,  -760,   174,  -760,  -760,
    -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,  -760,    90,  -760,   278,   165,  -760,  -760,  -760,   265,
     158,  -760,  -760,  -760,   164,   170,   187,   176,   205, 12723,
    2402,    71,   214,   225,  -760,  -760,  -760,  -760,  -760,  -760,
     252,  -760, 12723,   204,  3647,  -760,   249,  -760,   148,  7536,
    7536,  7536,  7536,  7536,  6464,  6464,   154,  -760,  -760,  -760,
    -760, 10257,   162,  -760,  -760,    90,   274, 10477,   329,  6464,
     279,  -760,  -760,  -760,  -760,    90,   125,    90,   283,    26,
    1943,  9334,  9449,  6464,  3647,   286,    14,    66,    34,  -760,
    -760,   699,   314,  9487,   699,  -760,  6464,  -760,   179,  -760,
    -760,    90,  -760,   185, 12723,  -760, 10549, 10137,  3647,  -760,
    -760,  9487, 12723,   298,  6464,  -760, 12723,  2977,  -760,   341,
     308,   223, 10367,  9487, 10609,   456,  2074,   699,   456,   699,
    -760,  -760,  3111,   -25,  -760,  7536,  -760,   -37,   -36, 12723,
      31, 10669,   -49,   428,  -760,    90,   339,  -760,  -760,  -760,
      15,    29,    29,    29,  -760,  7536,  7536,  7536,  7536,  6866,
    6866,  7536,  7536,  7536,  7536,  7536,  7536,    55,  2203,  7536,
    7536,  7536,  7536,  7536,  7536,  7536,  7536,  7536,  7536,  7536,
    7536,  7536,  7536,  7536,  7536,  7536,  7536,  7536,  7536,  7536,
    7536,  7536,  7536,  7536,  7536,  7536,  7536,  7536,  7536,  7536,
    6464,  6464,  6866,  6866,  5660,  -760,  -760,  -760, 10405, 10439,
   10729,     9,  6866,    26,   321,  -760,  -760,  7536,   285,  -760,
     320,   342,  -760, 12723,    90,   326,    90,   422,  6464,  -760,
    3781,  6866,  -760,  3915,  6866,   325,  -760,    26,  3647,   446,
     349,  -760,   332,  -760,  -760,    14,  -760,   374,   355,  -760,
    1314,   406,   411,  7536,    29,  -760,   367,  -760,  -760,  6464,
    -760,  -760,  -760,  -760,  -760,  6464,   369,  5121,   400,  -760,
    -760,  7670,  7536,  -760,  -760,  -760,  -760,  6330,  -760,  1472,
    5928,  -760,  6062,  -760,  6866,  5255,  2709,   377,  7536,  5525,
    -760,  -760,   379,  6464,   385,   145,    29,   236,   254,   276,
     282, 10197,  8935,  8935,   313,  -760,   313,  -760,   313,  9522,
     649,   366,  1267,   314,   456,  -760,  -760,  -760,  2074, 10329,
     313,   749,   749,  8935,   749,   749,  1007,   456, 10329,  9190,
    1007,   699,   699,   456, 10789, 10849, 10909, 10969, 11029, 11089,
   11149, 11209, 11269, 11329, 11389, 11449, 11509, 11569, 11629, 11689,
     383,   386,  -760,   313,  -760,   313,    50,  -760,  -760,  -760,
    -760,  -760,  -760,    90,   -21,  -760, 12783,   307,  7804,  6866,
    7938,  6866,  7536,  6866,  8948,    21, 11749,  -760,  -760, 11809,
    7536,  6464,  -760,  6464,   435,    82,  -760,  -760,   339,  7536,
      54,  7536, 12723,    49,  9597,  7536, 12723,    23,  9372,  5660,
    -760,   499, 11878,  3647,  -760,    73,  -760,    36,  -760,   262,
     391,    14,    70,  -760,  6464,  -760,   944,  7536,  7000,  -760,
   12723,  -760,  -760,  -760, 12723,    60,   394,  -760,  6464,  -760,
     288,    90,   413,   425,   415, 11938,   425,   421,  -760,  -760,
    -760,  -760,  -760,  -760,    -4,  9114,  -760,  -760, 12723,  3647,
   12723,  -760, 11998,  3245,   430,  -760,  -760,  -760,  -760,  -760,
    8999,   291,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,  -760,  -760,  -760,  -760,  -760,  -760,  5660,  -760,  6866,
    6866,  7536,   523, 12058,  7536,   527, 12118,   418,  9152,    26,
      26,  -760, 12723,  -760, 12723,  -760,  7536,    90,  -760,   420,
    9487,  -760,  9657,  4049,  -760,  4183,  -760,  9717,  4317,  -760,
      26,  4451,  -760,    26,    74,  -760,  3647,  7536,  -760,   476,
    -760,    14,   460,   506,  -760,  -760,  -760,    12,  -760,  -760,
    5121,   411,    48, 12723,  -760, 12723,  4585,  6464,  -760,  -760,
    -760,    53,  -760,  7536,  -760,  7536,  -760,  4719,   434,  4853,
     436,  -760,  7536,  -760,  3379,   999,   262,  -760,  -760,  -760,
    -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,  -760,    90,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,  -760,  -760,   464,   442,   442,  -760,  -760,    86,    88,
   12178,  8072,  8206, 12238,  8340,  8474,  8608,  8742,  -760,  -760,
   12723,  -760,  -760,  -760,  3647,  7536, 12723,  7536, 12723,  3647,
    7536, 12723,  -760,  7536, 12723,  -760,  5660,  -760,  -760, 12723,
    -760,   443,  7134,    -1,  -760,   497,  -760,  6866,  2352,  3647,
    -760,    24,  4987, 12303, 12723,  7536,  -760, 12723,  3647,  7536,
    -760, 12723,  3647, 12723,  -760,   492,   275,   275,  -760,   583,
    -760,    11,  -760,  8999,   332,  -760,  -760,  7536,  7536,  7536,
    7536,  7536,  7536,  7536,  7536,  1802, 11878,  9777,  9837, 11878,
    9897,  9957,  -760,   460,     0,  7536,  7536,  5390,  -760,  -760,
     294,  6464,  -760,  -760,  7536,    64,  9224,  -760,  1066,   329,
    -760,  7536, 10017,  -760, 10077,  -760,  -760,   507,  -760,   442,
     442,   173, 12363, 12423, 12483, 12543, 12603, 12663,  -760,  3647,
    3647,  3647,  3647,    -1,  7268,    61,  -760,  -760, 12723, 12723,
    -760,  -760,  -760,  8742, 12723,  3647,  3647,  -760,   497,  -760,
    -760,  -760,  7536,  7536,  7536,  7536,  7536,  7536, 11878, 11878,
   11878, 11878,  -760,  -760,  -760,  -760,  -760,   299,  6866,  8876,
    1117, 11878, 11878,     3,  9262,  -760,  -760,  -760,  -760,  -760,
    7402,  -760
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    35,    75,   399,   400,   401,   402,
     403,     0,   354,    73,   128,   354,     0,   255,    73,     0,
       0,     0,     0,    73,    73,     0,     0,     0,     0,   146,
       0,   142,     0,     0,     0,     0,   345,     0,     0,     0,
       0,   254,   254,   129,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   130,     0,     0,     0,   451,   453,
       0,   454,   455,   382,     0,     0,   452,   449,    82,   450,
       0,     0,     0,     4,     0,     5,     9,     0,    10,    11,
      12,    14,   314,    20,    13,    83,    15,    17,    16,    19,
      18,    88,     0,    86,   374,     0,    89,    87,    90,     0,
     383,   370,   371,   317,   315,     0,     0,   375,   376,     0,
     316,     0,   384,   385,   386,   369,   319,   318,   372,   373,
       0,    22,   325,     0,     0,   355,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,   374,   383,   315,   375,
     376,   354,   316,   384,   385,     0,     0,     0,     0,   305,
       0,    77,    76,   147,    91,     0,   148,     0,     0,     0,
       0,     0,     0,   305,     0,     0,     0,     0,     0,   257,
      29,   437,   367,     0,   438,     7,   305,   255,   256,    85,
      84,   234,   297,     0,   296,    80,     0,     0,     0,    79,
      32,     0,   320,     0,   305,    33,   326,     0,   119,   246,
       0,     0,   296,     0,     0,   440,   381,   436,   439,   435,
      41,    43,     0,     0,   300,     0,   302,     0,     0,   301,
       0,   296,     0,     0,     6,     0,   131,   220,   219,   149,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,   305,     0,     0,     0,    23,    24,    25,     0,     0,
       0,     0,     0,     0,     0,    26,    27,     0,   314,   312,
       0,   306,   307,   313,     0,     0,     0,     0,     0,   100,
       0,     0,    99,     0,     0,     0,   106,     0,     0,   116,
       0,    28,   197,   143,   262,     0,   263,   265,     0,   276,
       0,     0,   268,     0,     0,    30,     0,   148,   233,     0,
      56,    81,   121,    78,    31,   305,     0,     0,   244,   241,
      53,     0,    49,    93,    34,    42,    44,     0,   404,     0,
       0,   395,     0,   397,     0,     0,     0,     0,     0,     0,
     408,     8,     0,     0,     0,   215,     0,     0,     0,     0,
       0,   353,   432,   431,   434,   442,   441,   446,   445,   428,
     425,   426,   427,   378,   415,   394,   393,   392,   379,   419,
     430,   424,   422,   433,   423,   421,   413,   418,   420,   429,
     412,   416,   417,   414,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   444,   443,   448,   447,   209,   206,   207,   208,
     212,   213,   214,     0,     0,   357,     0,     0,     0,     0,
       0,     0,     0,     0,   406,   354,   354,    96,   242,     0,
       0,     0,   322,     0,   140,     0,   137,   243,   131,     0,
       0,     0,   329,     0,     0,     0,   335,     0,     0,     0,
     107,   115,     0,     0,   321,     0,   198,     0,   205,   221,
       0,   266,     0,   280,     0,   275,   370,     0,     0,   259,
     368,   258,   391,   299,   298,     0,     0,   323,     0,   248,
     370,     0,   450,    52,     0,    45,    50,     0,   405,   377,
     396,   303,   398,   304,     0,     0,   407,   112,   339,     0,
     410,   409,     0,     0,   132,   133,   216,   217,   218,   150,
       0,     0,   250,   249,   251,   253,    57,    64,    65,    66,
      61,    63,    71,    72,    59,    62,    60,    58,    68,    67,
      69,    70,   389,   390,   210,   211,   362,     0,   356,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,   311,   309,   310,   308,     0,   138,   136,     0,
       0,   114,     0,     0,    98,     0,    97,     0,     0,   104,
       0,     0,   102,     0,     0,   360,     0,     0,   118,   204,
     196,     0,   289,   222,   225,   224,   226,     0,   264,   267,
       0,   268,     0,   260,   269,   270,     0,     0,   120,   122,
     324,     0,   245,     0,    54,     0,    55,     0,     0,     0,
       0,   113,     0,    36,     0,   254,   221,   179,   177,   182,
     189,   190,   191,   186,   188,   184,   187,   185,   183,   193,
     192,   158,   161,   159,   160,   171,   162,   175,   167,   165,
     178,   166,   164,   169,   174,   176,   163,   168,   172,   173,
     170,   180,   181,   156,   194,   194,   252,   358,   383,   383,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    94,
     141,   139,   133,   101,     0,     0,   328,     0,   327,     0,
       0,   334,   105,     0,   333,   103,     0,   359,   117,   344,
     199,     0,     0,   268,   223,   239,   261,     0,     0,     0,
     124,     0,     0,    47,    46,     0,   110,   341,     0,     0,
     108,   340,     0,   411,    37,    73,   254,   254,   126,   254,
     134,     0,   157,     0,   197,   152,   153,     0,     0,     0,
       0,     0,     0,     0,     0,   254,   332,     0,     0,   338,
       0,     0,   361,   289,   292,   293,   294,     0,   291,   295,
     370,   235,   201,   200,     0,     0,     0,   278,   370,   125,
     123,     0,     0,   111,     0,   109,   135,   227,   156,   194,
     194,     0,     0,     0,     0,     0,     0,     0,   127,     0,
       0,     0,     0,   268,   281,     0,   236,   238,   237,   240,
     231,   232,   144,     0,    48,     0,     0,   228,   239,   154,
     155,   195,     0,     0,     0,     0,     0,     0,   331,   330,
     337,   336,   203,   202,   283,   284,   286,   370,     0,   406,
     370,   343,   342,     0,     0,   285,   287,   229,   151,   230,
     281,   288
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -760,  -760,  -760,     1,  -603,  1917,  -760,  -760,  1557,    47,
     229,  -760,  -760,  -760,  -760,  -760,    84,  1941,   -12,   316,
    -721,  -604,   -40,  -760,  -760,    81,  -760,  -760,  -760,  -760,
    -760,  -760,   427,   118,  -114,  -760,  -760,     2,   357,  -760,
    -760,  -760,  -760,  -760,  -760,  -760,  -162,  -161,  -622,  -760,
    -160,   -16,  -760,  -300,  -760,  -760,   -51,  -760,  -760,  -256,
     143,  -760,  -215,  -228,  -760,  -136,  -760,  -760,  -760,  -202,
     267,  -760,  -318,  -611,  -760,  -467,  -341,  -759,  -718,  -170,
       7,   103,  -760,   -69,  -760,   142,   345,  -275,  -760,  -760,
     667,  -760,     6,  -760,  -760,  -200,  -760,  -441,  -760,   916,
     928,   -11,  1173,  -760,  1200,  1487,  -760,  -760,  -760,  -760,
    -760,  -760,  -760,  -280
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   211,    74,   481,    76,    77,    78,   212,
     513,   517,   514,    79,    80,    81,   152,    82,   156,   190,
      83,    84,    85,    86,    87,   577,    88,    89,    90,   505,
     629,    91,    92,   374,   645,    93,   465,   466,   136,   165,
     489,    95,    96,   375,   646,   539,   684,   685,   755,   323,
     485,   486,   487,   443,   540,   229,   617,   828,   858,   822,
     182,   817,   782,   785,    97,   200,   348,    98,    99,   168,
     169,   327,   328,   499,   331,   332,   495,   845,   779,   723,
     213,   217,   218,   300,   301,   302,   137,   101,   102,   103,
     138,   105,   124,   125,   444,   317,   604,   445,   106,   139,
     140,   109,   142,   111,   143,   144,   114,   115,   222,   116,
     117,   118,   119,   120
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     122,   179,   180,    73,   447,   141,   509,   490,   147,   294,
     726,   153,   329,   329,   160,   161,   162,     5,     5,   305,
     367,   128,   488,   171,   172,   621,   173,   174,   377,   378,
     379,   380,     5,   184,   186,   187,   191,   192,   605,     5,
     196,   750,   749,   453,   202,   203,   204,   598,   188,   205,
     188,   183,   206,   207,   208,   496,   209,   369,     5,   184,
     219,   221,   201,   756,   821,   364,    45,   329,   333,   360,
     362,   329,   510,   593,  -273,  -273,   376,     3,   315,   220,
     395,   357,   727,  -273,   564,   567,   498,   727,   370,   324,
     855,  -273,  -273,     5,   320,   848,   846,   126,   361,   363,
    -273,   861,   146,   123,  -273,   315,   159,   336,   358,   781,
     626,   565,   783,   315,   568,   365,   620,   620,   122,   288,
     289,   290,   196,   184,   221,   346,   687,    45,   857,   396,
     339,   856,   821,   637,   149,   197,  -273,   365,   303,   627,
     334,    14,   291,  -273,  -273,   796,   454,   293,   325,   166,
     725,  -271,   303,  -273,   365,   197,   197,  -273,   197,   365,
     339,   750,   749,   166,   163,   303,   337,   365,   366,   145,
     611,   330,  -273,   619,   541,   620,  -273,   829,   830,   609,
     716,   197,     5,   303,    43,   728,   151,   175,   587,   591,
     732,  -273,  -363,   628,  -364,  -273,   197,   536,   849,   447,
     193,    54,   843,   227,   359,   280,   537,   281,   610,   717,
     176,   430,   431,   356,   216,   588,     5,   -73,   225,   538,
     151,  -363,   228,  -364,   381,   382,   383,   384,   386,   388,
     389,   390,   391,   392,   393,   394,   -73,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   429,   303,
     303,   433,   435,   446,   194,   772,   506,   600,   603,   609,
     197,   456,   613,  -347,   797,   230,   459,   447,  -388,  -346,
    -388,   339,   299,   721,   177,   234,  -387,   184,  -387,   472,
     474,  -351,   476,   478,    23,   178,   299,   482,   831,   488,
     340,    29,   282,  -145,   614,   470,   283,   231,   235,   299,
     385,   387,   500,   615,   638,   640,   232,     5,   504,   339,
    -352,   151,  -145,   154,   303,  -204,   616,   299,   284,  -349,
     515,   515,   334,   233,  -256,   496,   504,   100,   350,   219,
    -348,   219,   436,   525,   504,   528,   100,   530,   532,    94,
     334,   542,   184,   432,   434,   100,   437,   570,    94,   438,
     460,   283,   461,   455,   286,   632,  -256,    94,   100,   543,
     534,  -290,   334,   439,   440,  -256,  -282,   787,   334,   238,
      94,   787,   473,   441,  -247,   477,   100,   334,   155,   295,
    -290,   544,  -256,  -350,  -290,  -282,   442,   545,    94,  -282,
     297,   304,  -388,  -247,  -388,   100,   686,   308,   245,   100,
     322,   247,   247,   299,   299,   250,   239,    94,   347,  -290,
     240,    94,   345,   349,  -282,   371,   447,   472,   573,   476,
     576,   482,   578,   528,   373,   524,   458,   780,   463,   582,
     584,   467,   303,   788,   488,   462,   469,   788,   590,   479,
     592,   579,   580,   521,   597,   523,  -204,   242,   446,   100,
     244,   245,   483,   246,   247,   309,   312,   316,   250,   238,
     491,    94,   503,   221,   484,   257,   623,   625,   299,   335,
     492,   497,   261,   262,   263,     5,   436,   221,   498,   151,
     503,   622,   502,   216,   507,   216,   511,   344,   503,   100,
     437,   533,  -256,   438,   529,   631,   239,   535,   562,   353,
     240,    94,   586,   563,   177,   606,   618,   439,   440,   630,
     572,   633,   575,   100,    23,   178,   339,   441,   -51,   847,
     634,    29,   100,  -145,  -256,    94,   636,   691,   850,   -40,
     442,   694,   702,  -256,    94,   696,   446,   100,   386,   433,
     690,   245,  -145,   693,   247,   722,   155,   724,   250,    94,
    -256,   738,   753,   742,   847,   700,   754,   784,   773,   827,
     644,   516,   706,   306,   708,   847,   589,   711,   765,   701,
     714,   799,   800,   720,   801,   751,   719,   859,   842,    14,
     853,   501,    17,   813,   583,   585,   299,     0,     0,   457,
      22,     0,    23,   745,     0,     0,   184,     0,     0,    29,
       0,  -145,   733,     0,   734,     0,   737,     0,   741,     0,
       0,   743,     0,   480,   731,     0,     0,   223,     0,   746,
    -145,   747,    43,     0,     0,   356,     0,     0,     0,     0,
     385,   432,     0,     0,   177,   100,     0,    53,   100,    54,
       0,     0,     0,   100,    23,   178,     0,    94,     0,   104,
      94,    29,   238,  -145,     0,    94,     0,     0,   104,   -39,
     706,   708,     0,   711,   714,   737,   741,   104,     0,     0,
       0,     0,  -145,   766,   767,     0,   768,     0,   769,   770,
     104,     0,   771,     0,     0,   446,   179,   180,    68,   239,
       0,   100,     0,   240,     0,     0,   786,   528,   104,     0,
       0,   528,   238,    94,   792,     0,     0,     0,   794,     0,
       0,     0,     0,     0,     0,     0,     0,   104,     0,     0,
       0,   104,     0,     0,     0,     0,   766,   802,   803,   769,
     804,   805,   806,   807,   245,     0,   246,   247,     0,   239,
       0,   250,     0,   240,   192,   196,   221,     0,   257,     0,
     818,     0,   238,   819,     0,   261,   262,   263,     0,     0,
     824,     0,     0,     0,   815,     0,     0,     0,   566,   594,
     596,   104,     0,   599,   602,     0,     0,     0,   838,   839,
     840,   841,     0,     0,   245,     0,   246,   247,     0,   239,
       0,   250,   741,   240,   851,   852,     0,     0,   257,     0,
       0,   838,   839,   840,   841,   851,   852,   263,   100,     0,
       0,   104,     0,     0,     0,     0,     0,   854,   528,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,   741,
     242,     0,   243,   244,   245,   104,   246,   247,   248,     0,
       0,   250,     0,     0,   104,   778,     0,   256,   257,     0,
       0,     0,   260,     0,   100,   261,   262,   263,   100,   104,
       0,     0,     0,     0,     0,     0,    94,     0,     0,     0,
      94,     0,     0,     0,     0,   698,   699,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   703,     0,     0,     0,
       0,     0,     0,     0,   688,   689,   712,     0,   107,   715,
       0,     0,     0,     0,   816,     0,     0,   107,     0,     0,
     108,     0,     0,     0,     0,     0,   107,     0,   100,   108,
     100,     0,     0,   100,     0,     0,   100,     0,   108,   107,
      94,   100,    94,     0,     0,    94,     0,   844,    94,     0,
       0,   108,     0,    94,     0,     0,     0,   107,     0,     0,
       0,   100,     0,     0,     0,     0,     0,   104,  -274,   108,
     104,     0,   100,    94,   100,   104,   107,     0,     0,   100,
     107,     0,   844,     0,    94,     0,    94,     0,   108,     0,
       0,    94,   108,   844,     0,     0,     0,  -274,     0,     0,
       0,     0,     0,     0,     0,    14,  -274,     0,    17,     0,
       0,     0,     0,     0,  -274,     0,    22,     0,    23,   745,
     238,  -274,     0,   104,     0,    29,     0,  -145,     0,     0,
     107,     0,     0,     0,     0,     0,     0,   790,     0,   100,
    -274,     0,   108,    39,   100,   746,  -145,   747,    43,     0,
       0,    94,     0,     0,     0,     0,    94,   239,     0,  -274,
       0,   240,     0,    53,   100,    54,  -274,     0,     0,     0,
     107,     0,     0,   100,     0,     0,    94,   100,     0,     0,
       0,     0,   108,     0,     0,    94,     0,     0,     0,    94,
    -277,     0,     0,     0,   107,     0,     0,     0,   242,     0,
     243,   244,   245,   107,   246,   247,   108,     0,     0,   250,
       0,     0,     0,     0,    68,   108,   257,     0,   107,  -277,
       0,     0,   748,   261,   262,   263,     0,     0,  -277,     0,
     108,     0,     0,     0,     0,     0,  -277,     0,     0,     0,
     104,  -279,     0,  -277,   100,   100,   100,   100,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    94,    94,    94,
     100,   100,  -277,     0,     0,   110,     0,     0,     0,     0,
    -279,     0,    94,    94,   110,     0,     0,     0,     0,  -279,
       0,  -277,     0,   110,     0,     0,   104,  -279,  -277,     0,
     104,     0,   112,     0,  -279,     0,   110,     0,     0,     0,
       0,   112,     0,     0,     0,     0,     0,     0,     0,     0,
     112,     0,     0,  -279,   110,     0,   107,     0,     0,   107,
       0,     0,     0,   112,   107,     0,     0,     0,   108,     0,
       0,   108,  -279,   110,     0,     0,   108,   110,     0,  -279,
       0,   112,     0,     0,     0,     0,     0,     0,     0,     0,
     104,     0,   104,     0,     0,   104,     0,     0,   104,     0,
     112,     0,     0,   104,   112,     0,     0,     0,     0,     0,
       0,     0,   107,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   104,   108,     0,     0,   110,     0,     0,
       0,     0,     0,     0,   104,     0,   104,     0,     0,     0,
       0,   104,     0,     0,     0,   493,     0,     5,     6,     7,
       8,     9,    10,     0,   112,     0,   129,   239,     0,     0,
       0,   240,     0,     0,     0,     0,     0,   110,     0,    21,
       0,     0,     0,     0,     0,   130,   131,   132,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,   110,     0,     0,   112,     0,     0,    38,   242,    40,
     110,   104,   245,     0,   246,   247,   104,     0,     0,   250,
       0,    48,    49,    50,   133,   110,   257,     0,   112,     0,
       0,     0,     0,   261,   262,   263,   104,   112,     0,   107,
      58,     0,     0,     0,     0,   104,     0,     0,     0,   104,
       0,   108,   112,     0,     0,    59,    60,    61,    62,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,    65,     0,    66,    67,     0,     0,
       0,     0,    69,     0,     0,   107,   134,     0,    71,   107,
     494,     0,     0,     0,     0,     0,     0,   108,     0,     0,
       0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   104,   104,   104,   104,
       0,     0,   236,   110,     0,     0,   110,   237,     0,   113,
       0,   110,   104,   104,     0,   238,     0,     0,   113,     0,
       0,     0,     0,     0,     0,     0,     0,   113,     0,   107,
     112,   107,     0,   112,   107,     0,     0,   107,   112,     0,
     113,   108,   107,   108,     0,     0,   108,     0,     0,   108,
       0,     0,   239,     0,   108,     0,   240,     0,   113,   110,
       0,     0,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   107,   108,   107,     0,   113,   241,     0,
     107,   113,     0,     0,     0,   108,   112,   108,     0,     0,
       0,     0,   108,   242,     0,   243,   244,   245,     0,   246,
     247,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,     0,   258,   259,   260,     0,     0,   261,   262,
     263,     0,     0,     0,   189,     0,     0,   519,     0,   198,
       0,   113,     0,     0,     0,     0,     0,     0,     0,     0,
     107,     0,     0,     0,     0,   107,     0,     0,     0,     0,
       0,     0,   108,     0,     0,     0,     0,   108,     0,     0,
       0,     0,     0,     0,     0,   107,     0,     0,     0,     0,
       0,   113,     0,     0,   107,     0,   110,   108,   107,     0,
       0,     0,     0,     0,     0,     0,   108,     0,     0,     0,
     108,     0,     0,     0,     0,   113,     0,     0,     0,     0,
       0,     0,     0,   112,   113,   287,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   113,
       0,     0,   110,     0,     0,     0,   110,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,   189,   189,   319,
       0,     0,     0,     0,     0,   107,   107,   107,   107,   112,
     189,     0,     0,   112,     0,     0,     0,   108,   108,   108,
     108,   107,   107,     0,     0,     0,     0,     0,   189,     0,
       0,     0,     0,   108,   108,     0,     0,     0,     0,     0,
     189,     0,     0,     0,     0,     0,   110,     0,   110,     0,
       0,   110,     0,     0,   110,     0,     0,     0,     0,   110,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   112,     0,   112,     0,   113,   112,   110,
     113,   112,     0,     0,     0,   113,   112,     0,     0,     0,
     110,     0,   110,     0,     0,     0,     0,   110,    14,     0,
       0,    17,     0,     0,     0,     0,   112,     0,     0,    22,
       0,    23,   745,     0,     0,     0,     0,   112,    29,   112,
    -145,     0,     0,     0,   112,     0,     0,     0,     0,     0,
     189,     0,     0,   113,     0,     0,    39,     0,   746,  -145,
     747,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   189,     0,    53,   110,    54,     0,
       0,     0,   110,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,     0,   112,     0,     0,     0,     0,   112,
       0,   110,     0,     0,     0,   110,     0,     0,     0,    75,
       0,     0,     0,     0,     0,     0,     0,    68,   121,   112,
       0,     0,     0,     0,     0,   808,     0,   148,   112,     0,
       0,     0,   112,     0,     0,     0,     0,     0,     0,     0,
     170,     0,     0,   236,   127,     0,     0,     0,   237,   127,
       0,     0,     0,   150,   127,   127,   238,   310,   195,     0,
     113,   164,     0,   167,     0,     0,     0,   311,     0,     0,
       0,     0,   110,   110,   110,   110,     0,    75,     0,     0,
       0,   224,     0,     0,   199,     0,     0,     0,   110,   110,
       0,     0,     0,   239,     0,     0,     0,   240,     0,   112,
     112,   112,   112,     0,     0,     0,   113,     0,     0,     0,
     113,     0,     0,     0,     0,   112,   112,     0,     0,   241,
     189,   189,     0,   226,   189,   189,     0,     0,     0,     0,
       0,   285,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,     0,   258,   259,   260,     0,     0,   261,
     262,   263,     0,     0,     0,   197,     0,     0,     0,     0,
     113,   321,   113,     0,     0,   113,   199,     0,   113,     0,
     298,     0,     0,   113,     0,     0,   199,   238,   307,     0,
       0,     0,     0,     0,   298,   343,     0,   326,     0,     0,
       0,     0,     0,   113,    75,     0,     0,   298,     0,   127,
       0,     0,   338,     0,   113,     0,   113,     0,     0,    75,
       0,   113,     0,     0,   239,   298,   189,   189,   240,     0,
       0,     0,     0,     0,     0,     0,     0,   189,     0,   319,
       0,     0,     0,     0,   319,     0,     0,   189,     0,     0,
     189,     0,     0,     0,     0,     0,   372,     0,     0,     0,
       0,   167,   167,   167,   167,   242,     0,   243,   244,   245,
       0,   246,   247,     0,     0,     0,   250,     0,   397,     0,
       0,   113,   256,   257,     0,     0,   113,   260,     0,     0,
     261,   262,   263,     0,     0,     0,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   129,   113,     0,     0,     0,
       0,   298,   298,     0,     0,   113,     0,   343,    21,   113,
     343,     0,     0,     0,   130,   131,   132,     0,    28,     0,
       0,     0,     0,    31,    32,   464,    34,   468,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,   326,     0,     0,     0,
      48,    49,    50,   133,     0,   167,     0,     0,     0,     0,
       0,     0,     0,   527,     0,     0,   298,     0,   189,     0,
       0,     0,     0,     0,     0,     0,   113,   113,   113,   113,
       0,     0,     0,     0,    59,    60,    61,    62,     0,     0,
       0,     0,   113,   113,     0,     0,     0,   167,     0,     0,
       0,    64,     0,    65,   319,   319,    67,   319,   319,     0,
       0,    69,     0,     0,     0,   134,     0,    71,     0,   135,
       0,     0,   820,     0,     0,     0,     0,     0,     0,   319,
       0,   319,     0,   493,     0,     5,     6,     7,     8,     9,
      10,     0,  -406,     0,   129,     0,     0,  -406,     0,     0,
       0,     0,     0,     0,     0,  -406,     0,    21,     0,     0,
       0,     0,     0,   130,   131,   132,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
     608,     0,     0,     0,   298,    38,     0,    40,     0,     0,
     820,     0,  -406,     0,     0,     0,  -406,     0,     0,    48,
      49,    50,   133,     0,     0,     0,     0,     0,   612,     0,
       0,     0,   326,     0,     0,     0,     0,  -272,  -406,     0,
       0,     0,     0,     0,     0,     0,   641,     0,     0,     0,
      75,     0,   199,  -406,    60,  -406,  -406,  -406,     0,  -406,
    -406,  -406,     0,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,    65,  -406,  -406,  -406,     0,     0,  -406,  -406,
    -406,   683,     0,     0,   134,     0,    71,     0,   508,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,     0,     0,     0,     0,     0,     0,     0,
     343,     0,   343,     0,     0,   343,     0,     0,   343,     0,
       0,     0,     0,   718,     0,     0,     0,     0,   464,     0,
       0,   278,   279,  -365,     0,     0,  -387,     0,  -387,     0,
       0,     0,     0,   730,     0,     0,     0,     0,     0,     0,
       0,     0,   326,     0,   736,     0,   740,     0,     0,     0,
       0,    75,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    -2,     4,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -254,    18,    19,     0,    20,
      21,     0,    22,   752,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -145,    30,    31,    32,    33,    34,   -38,
      35,    36,     0,    37,     0,     0,     0,  -254,    38,    39,
      40,    41,  -145,    42,    43,     0,  -254,    44,    45,     0,
      46,    47,    48,    49,    50,    51,   789,     0,    52,    53,
       0,    54,    55,  -254,     0,   793,    56,     0,    57,   795,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
       0,     0,     0,     0,    63,     0,   127,     0,     0,     0,
       0,     0,     0,    64,   798,    65,     0,    66,    67,     0,
      68,     0,     0,    69,     0,     0,     0,    70,     0,    71,
       4,    72,     5,     6,     7,     8,     9,    10,     0,  -406,
       0,    11,    12,    13,  -406,    14,    15,    16,    17,  -254,
      18,    19,  -406,    20,    21,  -406,    22,     0,    23,    24,
      25,    26,    27,     0,    28,    29,     0,  -145,    30,    31,
      32,    33,    34,   -38,    35,    36,     0,    37,     0,     0,
       0,  -254,    38,     0,    40,    41,  -145,    42,    43,  -406,
    -254,    44,    45,  -406,    46,    47,    48,    49,    50,    51,
       0,     0,    52,    53,     0,    54,    55,  -254,     0,     0,
      56,     0,    57,     0,     0,  -406,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -406,    60,  -406,  -406,  -406,  -406,  -406,  -406,  -406,     0,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,    65,
    -406,  -406,  -406,     0,    68,  -406,  -406,  -406,     0,     0,
       0,    70,  -406,    71,     4,    72,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -254,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,    27,     0,    28,    29,
       0,  -145,    30,    31,    32,    33,    34,   -38,    35,    36,
       0,    37,     0,     0,     0,  -254,    38,    39,    40,    41,
    -145,    42,    43,     0,  -254,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,     0,    52,    53,     0,    54,
      55,  -254,     0,     0,    56,     0,    57,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,    65,     0,    66,    67,   181,    68,     0,
       0,    69,     0,     0,     0,    70,   210,    71,     4,    72,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -254,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,     0,    28,    29,     0,  -145,    30,    31,    32,    33,
      34,   -38,    35,    36,     0,    37,     0,     0,     0,  -254,
      38,    39,    40,    41,  -145,    42,    43,     0,  -254,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,     0,
      52,    53,     0,    54,    55,  -254,     0,     0,    56,     0,
      57,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,    62,     0,     0,     0,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,    65,     0,    66,
      67,     0,    68,     0,     0,    69,     0,     0,     0,    70,
     210,    71,     4,    72,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -254,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,     0,    28,    29,     0,  -145,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -254,    38,    39,    40,    41,  -145,    42,
      43,     0,  -254,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,     0,    52,    53,     0,    54,    55,  -254,
       0,     0,    56,     0,    57,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    60,    61,    62,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,    65,     0,    66,    67,     0,    68,     0,     0,    69,
       0,     0,     0,    70,   355,    71,     4,    72,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -254,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -145,    30,    31,    32,    33,    34,   -38,
      35,    36,     0,    37,     0,     0,     0,  -254,    38,    39,
      40,    41,  -145,    42,    43,     0,  -254,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,     0,    52,    53,
       0,    54,    55,  -254,     0,     0,    56,     0,    57,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,    65,     0,    66,    67,     0,
      68,     0,     0,    69,     0,     0,     0,    70,   643,    71,
       4,    72,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -254,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,    27,     0,    28,    29,     0,  -145,    30,    31,
      32,    33,    34,   -38,    35,    36,     0,    37,     0,     0,
       0,  -254,    38,    39,    40,    41,  -145,    42,    43,     0,
    -254,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,     0,    52,    53,     0,    54,    55,  -254,     0,     0,
      56,     0,    57,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,    65,
       0,    66,    67,     0,    68,     0,     0,    69,     0,     0,
       0,    70,   744,    71,     4,    72,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -254,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,    27,     0,    28,    29,
       0,  -145,    30,    31,    32,    33,    34,   -38,    35,    36,
       0,    37,     0,     0,     0,  -254,    38,   223,    40,    41,
    -145,    42,    43,     0,  -254,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,     0,    52,    53,     0,    54,
      55,  -254,     0,     0,    56,     0,    57,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,    65,     0,    66,    67,     0,    68,     0,
       0,    69,     0,     0,     0,    70,     0,    71,     4,    72,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -254,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,     0,    28,    29,     0,  -145,    30,    31,    32,    33,
      34,   -38,    35,    36,     0,    37,     0,     0,     0,  -254,
      38,     0,    40,    41,  -145,    42,    43,     0,  -254,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,     0,
      52,    53,     0,    54,    55,  -254,     0,     0,    56,     0,
      57,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,    62,     0,     0,     0,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,    65,     0,    66,
      67,     0,    68,     0,     0,    69,     0,     0,     0,    70,
       0,    71,     4,    72,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -254,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,   471,     0,    28,    29,     0,  -145,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -254,    38,     0,    40,    41,  -145,    42,
      43,     0,  -254,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,     0,    52,    53,     0,    54,    55,  -254,
       0,     0,    56,     0,    57,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    60,    61,    62,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,    65,     0,    66,    67,     0,    68,     0,     0,    69,
       0,     0,     0,    70,     0,    71,     4,    72,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -254,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,   475,     0,
      28,    29,     0,  -145,    30,    31,    32,    33,    34,   -38,
      35,    36,     0,    37,     0,     0,     0,  -254,    38,     0,
      40,    41,  -145,    42,    43,     0,  -254,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,     0,    52,    53,
       0,    54,    55,  -254,     0,     0,    56,     0,    57,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,    65,     0,    66,    67,     0,
      68,     0,     0,    69,     0,     0,     0,    70,     0,    71,
       4,    72,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -254,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,   705,     0,    28,    29,     0,  -145,    30,    31,
      32,    33,    34,   -38,    35,    36,     0,    37,     0,     0,
       0,  -254,    38,     0,    40,    41,  -145,    42,    43,     0,
    -254,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,     0,    52,    53,     0,    54,    55,  -254,     0,     0,
      56,     0,    57,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,    65,
       0,    66,    67,     0,    68,     0,     0,    69,     0,     0,
       0,    70,     0,    71,     4,    72,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -254,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,   707,     0,    28,    29,
       0,  -145,    30,    31,    32,    33,    34,   -38,    35,    36,
       0,    37,     0,     0,     0,  -254,    38,     0,    40,    41,
    -145,    42,    43,     0,  -254,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,     0,    52,    53,     0,    54,
      55,  -254,     0,     0,    56,     0,    57,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,    65,     0,    66,    67,     0,    68,     0,
       0,    69,     0,     0,     0,    70,     0,    71,     4,    72,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -254,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
     710,     0,    28,    29,     0,  -145,    30,    31,    32,    33,
      34,   -38,    35,    36,     0,    37,     0,     0,     0,  -254,
      38,     0,    40,    41,  -145,    42,    43,     0,  -254,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,     0,
      52,    53,     0,    54,    55,  -254,     0,     0,    56,     0,
      57,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,    62,     0,     0,     0,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,    65,     0,    66,
      67,     0,    68,     0,     0,    69,     0,     0,     0,    70,
       0,    71,     4,    72,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -254,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,   713,     0,    28,    29,     0,  -145,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -254,    38,     0,    40,    41,  -145,    42,
      43,     0,  -254,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,     0,    52,    53,     0,    54,    55,  -254,
       0,     0,    56,     0,    57,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    60,    61,    62,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,    65,     0,    66,    67,     0,    68,     0,     0,    69,
       0,     0,     0,    70,     0,    71,     4,    72,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -254,    18,    19,     0,   729,
      21,     0,    22,     0,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -145,    30,    31,    32,    33,    34,   -38,
      35,    36,     0,    37,     0,     0,     0,  -254,    38,     0,
      40,    41,  -145,    42,    43,     0,  -254,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,     0,    52,    53,
       0,    54,    55,  -254,     0,     0,    56,     0,    57,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,    65,     0,    66,    67,     0,
      68,     0,     0,    69,     0,     0,     0,    70,     0,    71,
       4,    72,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -254,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,   735,     0,    28,    29,     0,  -145,    30,    31,
      32,    33,    34,   -38,    35,    36,     0,    37,     0,     0,
       0,  -254,    38,     0,    40,    41,  -145,    42,    43,     0,
    -254,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,     0,    52,    53,     0,    54,    55,  -254,     0,     0,
      56,     0,    57,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,    65,
       0,    66,    67,     0,    68,     0,     0,    69,     0,     0,
       0,    70,     0,    71,     4,    72,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -254,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,   739,     0,    28,    29,
       0,  -145,    30,    31,    32,    33,    34,   -38,    35,    36,
       0,    37,     0,     0,     0,  -254,    38,     0,    40,    41,
    -145,    42,    43,     0,  -254,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,     0,    52,    53,     0,    54,
      55,  -254,     0,     0,    56,     0,    57,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,    62,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,    65,     0,    66,    67,     0,    68,     0,
       0,    69,     0,     0,     0,    70,     0,    71,   493,    72,
       5,     6,     7,     8,     9,    10,     0,  -406,     0,   129,
       0,     0,  -406,     0,     0,     0,     0,     0,     0,     0,
    -406,     0,    21,     0,     0,     0,     0,     0,   130,   131,
     132,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,  -406,     0,     0,
       0,  -406,     0,     0,    48,    49,    50,   133,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -406,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -406,    60,
    -406,  -406,  -406,     0,  -406,  -406,  -406,     0,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,    65,  -406,  -406,
    -406,     0,     0,  -406,  -406,  -406,     0,     0,     0,   134,
       0,    71,   493,   508,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   129,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   130,   131,   132,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   133,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    60,    61,    62,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,    65,     0,    66,    67,     0,     0,     0,     0,    69,
       0,     0,     0,   134,     0,    71,     0,   508,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   129,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   130,   131,   132,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,    65,     0,    66,    67,   181,
       0,     0,     0,    69,     0,     0,     0,   134,     0,    71,
       0,   135,   526,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   130,   131,   132,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     133,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
      65,     0,    66,    67,   181,     0,     0,     0,    69,     0,
       0,     0,   134,     0,    71,     0,   135,   814,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   129,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   130,   131,   132,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   133,     0,     0,     0,     0,
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
     133,     0,     0,     0,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
      65,     0,    66,    67,     0,     0,     0,     0,    69,     0,
       0,     0,   134,     0,    71,     0,   135,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   129,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   130,   131,   132,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   133,     0,     0,     0,     0,   214,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,     0,
       0,     0,     0,    63,   215,     0,     0,     0,     0,     0,
       0,     0,    64,     0,    65,     0,    66,    67,   181,     0,
       0,     0,    69,     0,     0,     0,   134,     0,    71,     0,
     135,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   130,
     131,   132,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   133,     0,
       0,     0,     0,   214,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,    65,     0,
      66,    67,   181,     0,     0,     0,    69,     0,     0,     0,
     134,     0,    71,   520,   135,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   130,   131,   132,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   133,     0,     0,     0,     0,   214,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,    65,     0,    66,    67,   181,     0,     0,     0,
      69,     0,     0,     0,   134,     0,    71,   522,   135,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   130,   131,   132,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,   157,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   133,     0,     0,     0,
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
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     133,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
      65,     0,    66,    67,   181,     0,     0,     0,    69,     0,
       0,     0,   134,   518,    71,     0,   135,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   129,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   130,   131,   132,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,    65,     0,    66,    67,   181,     0,
       0,     0,    69,     0,     0,     0,   134,     0,    71,     0,
     135,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   130,
     131,   132,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   133,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,    65,     0,
      66,    67,     0,   185,     0,     0,    69,     0,     0,     0,
     134,     0,    71,     0,   135,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   188,    21,     0,     0,
       0,     0,     0,   130,   131,   132,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,    65,     0,    66,    67,     0,     0,     0,     0,
      69,     0,     0,     0,    70,     0,    71,     0,   135,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   130,   131,   132,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   133,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
      65,     0,    66,    67,     0,     0,     0,     0,    69,     0,
       0,     0,   134,     0,    71,     0,   135,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   129,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   774,
       0,     0,     0,     0,     0,   130,   131,   132,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   775,    49,    50,   776,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,    65,     0,    66,    67,   181,     0,
       0,     0,    69,     0,     0,     0,   134,     0,    71,     0,
     777,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   130,
     131,   132,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   133,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,    65,     0,
      66,    67,   181,     0,     0,     0,    69,     0,     0,     0,
     134,     0,    71,     0,   777,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   130,   131,   764,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,    65,     0,    66,    67,   181,     0,     0,     0,
      69,     0,     0,     0,   134,     0,    71,     0,   135,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   130,   131,   132,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   133,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,    61,
      62,     0,     0,     0,     0,    63,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,    65,     0,    66,    67,
       0,     0,     0,     0,    69,     0,     0,     0,   134,     0,
      71,     0,   135,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   130,   131,   132,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     133,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
      65,     0,    66,    67,     0,     0,     0,     0,   512,     0,
       0,     0,   134,     0,    71,     0,   135,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   129,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   130,   131,   571,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,    65,     0,    66,    67,     0,     0,
       0,     0,    69,     0,     0,     0,   134,     0,    71,     0,
     135,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   130,
     131,   574,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   133,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,    65,     0,
      66,    67,     0,     0,     0,     0,    69,     0,     0,     0,
     134,     0,    71,     0,   135,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   130,   131,   758,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,    65,     0,    66,    67,     0,     0,     0,     0,
      69,     0,     0,     0,   134,     0,    71,     0,   135,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   130,   131,   759,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   133,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
      65,     0,    66,    67,     0,     0,     0,     0,    69,     0,
       0,     0,   134,     0,    71,     0,   135,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   129,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   130,   131,   762,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    62,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,    65,     0,    66,    67,     0,     0,
       0,     0,    69,     0,     0,     0,   134,     0,    71,     0,
     135,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   130,
     131,   763,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   133,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,    65,     0,
      66,    67,     0,     0,     0,     0,    69,     0,     0,     0,
     134,     0,    71,     0,   135,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   130,   131,   764,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,    61,    62,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,    65,     0,    66,    67,     0,     0,     0,     0,
      69,     0,     0,     0,   134,     0,    71,     0,   135,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   130,   131,   132,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   133,     0,     0,     0,
       0,     5,     6,     7,     8,     9,    10,     0,   238,     0,
     129,     0,     0,  -281,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,    60,   130,
     131,   132,  -281,    28,     0,     0,  -281,     0,    31,    32,
       0,    34,     0,    35,    36,   239,    65,     0,     0,   240,
     181,    38,     5,    40,     0,     0,     0,     0,   134,   647,
      71,  -281,   777,     0,   648,    48,    49,    50,   133,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,   243,   244,
     245,     0,   246,   247,   248,     0,   249,   250,   251,   252,
      60,   254,   255,   256,   257,     0,   258,   259,   260,     0,
       0,   261,   262,   263,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,   135,     0,   649,   650,   651,   652,
     653,   654,     0,     0,   655,   656,   657,   658,   659,   660,
     661,   662,   663,   664,     0,     0,   665,     0,     0,     0,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,   677,     0,   236,   678,   679,   680,   681,   237,
     682,     0,     0,     0,     0,     0,     0,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   236,     0,     0,     0,     0,   237,     0,     0,
       0,     0,     0,     0,   239,   238,     0,     0,   240,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,   238,     0,   242,   240,   243,   244,   245,
       0,   246,   247,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   236,   258,   259,   260,   241,   237,
     261,   262,   263,     0,     0,     0,     0,   238,     0,     0,
     239,   639,     0,   242,   240,   243,   244,   245,     0,   246,
     247,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   236,   258,   259,   260,   241,   237,   261,   262,
     263,     0,     0,     0,   239,   238,     0,     0,   240,   697,
       0,   242,     0,   243,   244,   245,     0,   246,   247,   248,
       0,   249,   250,   251,   252,     0,   254,   255,   256,   257,
     241,   258,     0,   260,     0,     0,   261,   262,   263,     0,
       0,     0,   239,     0,     0,   242,   240,   243,   244,   245,
       0,   246,   247,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   236,   258,   259,   260,   241,   237,
     261,   262,   263,     0,     0,     0,     0,   238,   313,     0,
       0,   823,     0,   242,     0,   243,   244,   245,   314,   246,
     247,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   236,   258,   259,   260,     0,   237,   261,   262,
     263,     0,     0,     0,   239,   238,   601,     0,   240,   860,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,     0,     0,   242,   240,   243,   244,   245,
       0,   246,   247,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   315,   258,   259,   260,   241,   236,
     261,   262,   263,     0,   237,     0,   197,     0,     0,     0,
       0,     0,   238,   242,     0,   243,   244,   245,     0,   246,
     247,   248,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,     0,   258,   259,   260,     0,   236,   261,   262,
     263,     0,   237,     0,   197,     0,     0,     0,     0,   239,
     238,   188,     0,   240,     0,     0,     0,     0,     0,     0,
     318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,     0,     0,
     242,   240,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,     0,
     258,   259,   260,   241,     0,   261,   262,   263,     0,     0,
       0,   197,   239,     0,     0,     0,   240,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   236,   258,   259,
     260,     0,   237,   261,   262,   263,     0,     0,     0,   197,
     238,   595,     0,   242,     0,   243,   244,   245,     0,   246,
     247,   248,     0,   249,   250,   251,   252,     0,   254,   255,
     256,   257,     0,   258,     0,   260,     0,     0,   261,   262,
     263,     0,     0,     0,     0,     0,     0,   239,     0,     0,
       0,   240,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,     0,   236,   704,   197,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,     0,   236,   709,   197,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,     0,   236,   809,   197,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,     0,   236,   810,   197,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,     0,   236,   811,   197,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,     0,   236,   812,   197,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,     0,   236,   825,   197,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,     0,   236,   826,   197,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,     0,   236,     0,   197,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,     0,   236,     0,   342,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,   292,     0,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   239,   258,   259,
     260,   240,     0,   261,   262,   263,     0,     0,  -366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   123,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   236,   258,   259,
     260,     0,   237,   261,   262,   263,     0,     0,     0,   239,
     238,     0,     0,   240,     0,   351,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,   352,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,   239,   238,   448,
     242,   240,   243,   244,   245,     0,   246,   247,   248,   449,
       0,   250,   251,   252,     0,   254,   255,   256,   257,   236,
       0,     0,   260,   241,   237,   261,   262,   263,     0,     0,
       0,     0,   238,   450,     0,   239,     0,     0,   242,   240,
     243,   244,   245,   451,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   236,   258,   259,
     260,   241,   237,   261,   262,   263,     0,     0,     0,   239,
     238,     0,     0,   240,     0,     0,   242,     0,   243,   244,
     245,     0,   246,   247,   248,     0,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   241,   258,   259,   260,     0,
       0,   261,   262,   263,     0,     0,     0,   239,     0,     0,
     242,   240,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   236,
     258,   259,   260,   241,   237,   261,   262,   263,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,   242,     0,
     243,   244,   245,     0,   246,   247,   248,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,     0,   258,   259,
     260,     0,   296,   261,   262,   263,     0,     0,     0,   239,
       0,     0,     0,   240,     0,     0,     0,     0,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   341,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   354,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   368,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,     0,   261,   262,   263,     0,   236,
     452,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,     0,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   546,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   547,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   548,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   549,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   550,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   551,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   552,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   553,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   554,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   555,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   556,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   557,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   558,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   559,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   560,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,   561,   261,   262,   263,     0,   236,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,   123,   238,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   239,
     258,   259,   260,   240,     0,   261,   262,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   236,     0,
       0,     0,     0,   237,     0,   241,     0,     0,     0,     0,
       0,   238,     0,     0,   607,     0,     0,     0,     0,     0,
     242,     0,   243,   244,   245,     0,   246,   247,   248,     0,
     249,   250,   251,   252,   253,   254,   255,   256,   257,     0,
     258,   259,   260,     0,   581,   261,   262,   263,   239,     0,
       0,     0,   240,     0,     0,     0,     0,     0,   236,   635,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,   238,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   239,   258,
     259,   260,   240,     0,   261,   262,   263,     0,   236,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,   238,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   239,   258,
     259,   260,   240,     0,   261,   262,   263,     0,   236,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,   238,   692,   642,   241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   239,   258,
     259,   260,   240,     0,   261,   262,   263,     0,   236,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,   238,   695,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   239,   258,
     259,   260,   240,     0,   261,   262,   263,     0,   236,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,   238,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   239,   258,
     259,   260,   240,     0,   261,   262,   263,     0,   236,   757,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,   238,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   239,   258,
     259,   260,   240,     0,   261,   262,   263,     0,     0,   760,
       0,     0,     0,   236,   791,     0,     0,     0,   237,     0,
       0,     0,     0,     0,   241,     0,   238,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,   243,   244,   245,     0,   246,   247,   248,     0,   249,
     250,   251,   252,   253,   254,   255,   256,   257,     0,   258,
     259,   260,     0,   239,   261,   262,   263,   240,     0,     0,
       0,     0,     0,   236,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,     0,   261,
     262,   263,     0,   236,   832,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,     0,   261,
     262,   263,     0,   236,   833,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,     0,   261,
     262,   263,     0,   236,   834,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,     0,   261,
     262,   263,     0,   236,   835,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,     0,   261,
     262,   263,     0,   236,   836,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,     0,   261,
     262,   263,     0,   236,   837,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   239,   258,   259,   260,   240,     0,   261,
     262,   263,     0,   236,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   569,   258,   259,   260,   240,     0,   261,
     262,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,   244,   245,     0,
     246,   247,   248,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,     0,   258,   259,   260,     0,     0,   261,
     262,   263
};

static const yytype_int16 yycheck[] =
{
      11,    41,    42,     2,   284,    16,   347,   325,    19,   145,
     621,    23,     1,     1,    25,    26,    27,     3,     3,   155,
     220,    15,   322,    34,    35,   492,    37,    38,   230,   231,
     232,   233,     3,    44,    45,    46,    47,    48,   479,     3,
      51,   645,   645,    34,    55,    56,    57,    24,    24,    60,
      24,    44,    63,    64,    65,   330,    67,   106,     3,    70,
      71,    72,    55,   685,   785,    34,    63,     1,    34,   106,
     106,     1,   347,    24,    63,    63,    61,     0,    82,    72,
      25,   106,    34,    72,    34,   106,    87,    34,   137,    75,
     849,    80,    80,     3,   163,    34,   814,    13,   135,   135,
      34,   860,    18,    82,    34,    82,    25,   176,   133,   110,
      50,    61,   723,    82,   135,   106,   105,   105,   129,   130,
     131,   132,   133,   134,   135,   194,   567,    63,   125,    74,
     106,   849,   853,   137,   134,   132,   125,   106,   149,    79,
     106,    16,   135,   132,   132,   749,   137,   141,   134,   134,
     617,    85,   163,    87,   106,   132,   132,    87,   132,   106,
     106,   765,   765,   134,   134,   176,   178,   106,   137,    74,
     134,   105,   106,   491,   376,   105,   106,   799,   800,   106,
     106,   132,     3,   194,    59,   137,     7,     7,   106,   135,
     137,   125,   106,   133,   106,   125,   132,    52,   137,   479,
      69,    76,   813,    38,   215,   134,    61,   136,   135,   135,
     134,   280,   281,   212,    71,   133,     3,    38,    44,    74,
       7,   135,    57,   135,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,    57,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   134,   716,   345,   477,   478,   106,
     132,   292,    20,   125,   751,    20,   297,   567,   134,   125,
     136,   106,   149,   611,    19,   125,   134,   308,   136,   310,
     311,   125,   313,   314,    29,    30,   163,   318,   135,   609,
     125,    36,    60,    38,    52,   308,    64,    52,   131,   176,
     239,   240,   333,    61,   524,   525,    61,     3,   339,   106,
     125,     7,    57,     9,   345,     3,    74,   194,   134,   125,
     351,   352,   106,    78,    20,   620,   357,     2,   125,   360,
     125,   362,    20,   364,   365,   366,    11,   368,   369,     2,
     106,   125,   373,   282,   283,    20,    34,    60,    11,    37,
      85,    64,    87,   292,   125,   511,    52,    20,    33,   125,
     373,    87,   106,    51,    52,    61,    87,   728,   106,    23,
      33,   732,   311,    61,   106,   314,    51,   106,    74,   125,
     106,   125,    78,   125,   110,   106,    74,   125,    51,   110,
      81,   132,   134,   125,   136,    70,   125,   134,   105,    74,
     134,   108,   108,   280,   281,   112,    60,    70,    87,   135,
      64,    74,   134,   125,   135,     7,   716,   448,   449,   450,
     451,   452,   453,   454,   105,   364,   125,   722,   106,   460,
     461,   125,   463,   728,   754,   135,    34,   732,   469,   134,
     471,   455,   456,   360,   475,   362,   134,   101,   479,   124,
     104,   105,    26,   107,   108,   159,   160,   161,   112,    23,
     106,   124,   339,   494,   135,   119,   497,   498,   345,   173,
     135,    85,   126,   127,   128,     3,    20,   508,    87,     7,
     357,   494,   135,   360,   135,   362,   106,   191,   365,   164,
      34,   132,    20,    37,   137,   508,    60,   132,   135,   203,
      64,   164,    87,   137,    19,    26,   135,    51,    52,   135,
     449,   106,   451,   188,    29,    30,   106,    61,   125,   814,
     125,    36,   197,    38,    52,   188,   125,    24,   823,    44,
      74,    24,   132,    61,   197,   137,   567,   212,   569,   570,
     571,   105,    57,   574,   108,   105,    74,    61,   112,   212,
      78,   137,   108,   137,   849,   586,   134,    80,   135,    72,
     533,   352,   593,   156,   595,   860,   468,   598,   702,   587,
     601,   753,   753,   609,   754,   646,   607,   853,   813,    16,
     828,   334,    19,   773,   461,   463,   463,    -1,    -1,   293,
      27,    -1,    29,    30,    -1,    -1,   627,    -1,    -1,    36,
      -1,    38,   633,    -1,   635,    -1,   637,    -1,   639,    -1,
      -1,   642,    -1,   317,   627,    -1,    -1,    54,    -1,    56,
      57,    58,    59,    -1,    -1,   644,    -1,    -1,    -1,    -1,
     569,   570,    -1,    -1,    19,   310,    -1,    74,   313,    76,
      -1,    -1,    -1,   318,    29,    30,    -1,   310,    -1,     2,
     313,    36,    23,    38,    -1,   318,    -1,    -1,    11,    44,
     691,   692,    -1,   694,   695,   696,   697,    20,    -1,    -1,
      -1,    -1,    57,   704,   705,    -1,   707,    -1,   709,   710,
      33,    -1,   713,    -1,    -1,   716,   746,   747,   125,    60,
      -1,   366,    -1,    64,    -1,    -1,   727,   728,    51,    -1,
      -1,   732,    23,   366,   735,    -1,    -1,    -1,   739,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,   757,   758,   759,   760,
     761,   762,   763,   764,   105,    -1,   107,   108,    -1,    60,
      -1,   112,    -1,    64,   775,   776,   777,    -1,   119,    -1,
     781,    -1,    23,   784,    -1,   126,   127,   128,    -1,    -1,
     791,    -1,    -1,    -1,   777,    -1,    -1,    -1,   443,   473,
     474,   124,    -1,   477,   478,    -1,    -1,    -1,   809,   810,
     811,   812,    -1,    -1,   105,    -1,   107,   108,    -1,    60,
      -1,   112,   823,    64,   825,   826,    -1,    -1,   119,    -1,
      -1,   832,   833,   834,   835,   836,   837,   128,   483,    -1,
      -1,   164,    -1,    -1,    -1,    -1,    -1,   848,   849,    -1,
     483,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   860,
     101,    -1,   103,   104,   105,   188,   107,   108,   109,    -1,
      -1,   112,    -1,    -1,   197,   722,    -1,   118,   119,    -1,
      -1,    -1,   123,    -1,   529,   126,   127,   128,   533,   212,
      -1,    -1,    -1,    -1,    -1,    -1,   529,    -1,    -1,    -1,
     533,    -1,    -1,    -1,    -1,   579,   580,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   590,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   569,   570,   600,    -1,     2,   603,
      -1,    -1,    -1,    -1,   781,    -1,    -1,    11,    -1,    -1,
       2,    -1,    -1,    -1,    -1,    -1,    20,    -1,   593,    11,
     595,    -1,    -1,   598,    -1,    -1,   601,    -1,    20,    33,
     593,   606,   595,    -1,    -1,   598,    -1,   814,   601,    -1,
      -1,    33,    -1,   606,    -1,    -1,    -1,    51,    -1,    -1,
      -1,   626,    -1,    -1,    -1,    -1,    -1,   310,    34,    51,
     313,    -1,   637,   626,   639,   318,    70,    -1,    -1,   644,
      74,    -1,   849,    -1,   637,    -1,   639,    -1,    70,    -1,
      -1,   644,    74,   860,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    16,    72,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    27,    -1,    29,    30,
      23,    87,    -1,   366,    -1,    36,    -1,    38,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,   731,    -1,   704,
     106,    -1,   124,    54,   709,    56,    57,    58,    59,    -1,
      -1,   704,    -1,    -1,    -1,    -1,   709,    60,    -1,   125,
      -1,    64,    -1,    74,   729,    76,   132,    -1,    -1,    -1,
     164,    -1,    -1,   738,    -1,    -1,   729,   742,    -1,    -1,
      -1,    -1,   164,    -1,    -1,   738,    -1,    -1,    -1,   742,
      34,    -1,    -1,    -1,   188,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,   197,   107,   108,   188,    -1,    -1,   112,
      -1,    -1,    -1,    -1,   125,   197,   119,    -1,   212,    63,
      -1,    -1,   133,   126,   127,   128,    -1,    -1,    72,    -1,
     212,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
     483,    34,    -1,    87,   809,   810,   811,   812,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   809,   810,   811,   812,
     825,   826,   106,    -1,    -1,     2,    -1,    -1,    -1,    -1,
      63,    -1,   825,   826,    11,    -1,    -1,    -1,    -1,    72,
      -1,   125,    -1,    20,    -1,    -1,   529,    80,   132,    -1,
     533,    -1,     2,    -1,    87,    -1,    33,    -1,    -1,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,   106,    51,    -1,   310,    -1,    -1,   313,
      -1,    -1,    -1,    33,   318,    -1,    -1,    -1,   310,    -1,
      -1,   313,   125,    70,    -1,    -1,   318,    74,    -1,   132,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     593,    -1,   595,    -1,    -1,   598,    -1,    -1,   601,    -1,
      70,    -1,    -1,   606,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   366,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,   626,   366,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,   637,    -1,   639,    -1,    -1,    -1,
      -1,   644,    -1,    -1,    -1,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   124,    -1,    12,    60,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,   164,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,   188,    -1,    -1,   164,    -1,    -1,    53,   101,    55,
     197,   704,   105,    -1,   107,   108,   709,    -1,    -1,   112,
      -1,    67,    68,    69,    70,   212,   119,    -1,   188,    -1,
      -1,    -1,    -1,   126,   127,   128,   729,   197,    -1,   483,
      86,    -1,    -1,    -1,    -1,   738,    -1,    -1,    -1,   742,
      -1,   483,   212,    -1,    -1,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,    -1,   128,    -1,    -1,   529,   132,    -1,   134,   533,
     136,    -1,    -1,    -1,    -1,    -1,    -1,   529,    -1,    -1,
      -1,   533,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   809,   810,   811,   812,
      -1,    -1,    10,   310,    -1,    -1,   313,    15,    -1,     2,
      -1,   318,   825,   826,    -1,    23,    -1,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,   593,
     310,   595,    -1,   313,   598,    -1,    -1,   601,   318,    -1,
      33,   593,   606,   595,    -1,    -1,   598,    -1,    -1,   601,
      -1,    -1,    60,    -1,   606,    -1,    64,    -1,    51,   366,
      -1,    -1,   626,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   637,   626,   639,    -1,    70,    86,    -1,
     644,    74,    -1,    -1,    -1,   637,   366,   639,    -1,    -1,
      -1,    -1,   644,   101,    -1,   103,   104,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    -1,   121,   122,   123,    -1,    -1,   126,   127,
     128,    -1,    -1,    -1,    47,    -1,    -1,   135,    -1,    52,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     704,    -1,    -1,    -1,    -1,   709,    -1,    -1,    -1,    -1,
      -1,    -1,   704,    -1,    -1,    -1,    -1,   709,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   729,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,   738,    -1,   483,   729,   742,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   738,    -1,    -1,    -1,
     742,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   483,   197,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,
      -1,    -1,   529,    -1,    -1,    -1,   533,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,   162,
      -1,    -1,    -1,    -1,    -1,   809,   810,   811,   812,   529,
     173,    -1,    -1,   533,    -1,    -1,    -1,   809,   810,   811,
     812,   825,   826,    -1,    -1,    -1,    -1,    -1,   191,    -1,
      -1,    -1,    -1,   825,   826,    -1,    -1,    -1,    -1,    -1,
     203,    -1,    -1,    -1,    -1,    -1,   593,    -1,   595,    -1,
      -1,   598,    -1,    -1,   601,    -1,    -1,    -1,    -1,   606,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   593,    -1,   595,    -1,   310,   598,   626,
     313,   601,    -1,    -1,    -1,   318,   606,    -1,    -1,    -1,
     637,    -1,   639,    -1,    -1,    -1,    -1,   644,    16,    -1,
      -1,    19,    -1,    -1,    -1,    -1,   626,    -1,    -1,    27,
      -1,    29,    30,    -1,    -1,    -1,    -1,   637,    36,   639,
      38,    -1,    -1,    -1,   644,    -1,    -1,    -1,    -1,    -1,
     293,    -1,    -1,   366,    -1,    -1,    54,    -1,    56,    57,
      58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   317,    -1,    74,   704,    76,    -1,
      -1,    -1,   709,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   729,    -1,   704,    -1,    -1,    -1,    -1,   709,
      -1,   738,    -1,    -1,    -1,   742,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    11,   729,
      -1,    -1,    -1,    -1,    -1,   133,    -1,    20,   738,    -1,
      -1,    -1,   742,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    10,    13,    -1,    -1,    -1,    15,    18,
      -1,    -1,    -1,    22,    23,    24,    23,    24,    51,    -1,
     483,    30,    -1,    32,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,   809,   810,   811,   812,    -1,    70,    -1,    -1,
      -1,    74,    -1,    -1,    53,    -1,    -1,    -1,   825,   826,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,   809,
     810,   811,   812,    -1,    -1,    -1,   529,    -1,    -1,    -1,
     533,    -1,    -1,    -1,    -1,   825,   826,    -1,    -1,    86,
     473,   474,    -1,    92,   477,   478,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,   101,    -1,   103,   104,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    -1,   121,   122,   123,    -1,    -1,   126,
     127,   128,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
     593,   164,   595,    -1,    -1,   598,   145,    -1,   601,    -1,
     149,    -1,    -1,   606,    -1,    -1,   155,    23,   157,    -1,
      -1,    -1,    -1,    -1,   163,   188,    -1,   166,    -1,    -1,
      -1,    -1,    -1,   626,   197,    -1,    -1,   176,    -1,   178,
      -1,    -1,   181,    -1,   637,    -1,   639,    -1,    -1,   212,
      -1,   644,    -1,    -1,    60,   194,   579,   580,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   590,    -1,   592,
      -1,    -1,    -1,    -1,   597,    -1,    -1,   600,    -1,    -1,
     603,    -1,    -1,    -1,    -1,    -1,   225,    -1,    -1,    -1,
      -1,   230,   231,   232,   233,   101,    -1,   103,   104,   105,
      -1,   107,   108,    -1,    -1,    -1,   112,    -1,   247,    -1,
      -1,   704,   118,   119,    -1,    -1,   709,   123,    -1,    -1,
     126,   127,   128,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,   729,    -1,    -1,    -1,
      -1,   280,   281,    -1,    -1,   738,    -1,   310,    25,   742,
     313,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,   304,    43,   306,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   325,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,   334,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   366,    -1,    -1,   345,    -1,   731,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   809,   810,   811,   812,
      -1,    -1,    -1,    -1,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,   825,   826,    -1,    -1,    -1,   376,    -1,    -1,
      -1,   118,    -1,   120,   767,   768,   123,   770,   771,    -1,
      -1,   128,    -1,    -1,    -1,   132,    -1,   134,    -1,   136,
      -1,    -1,   785,    -1,    -1,    -1,    -1,    -1,    -1,   792,
      -1,   794,    -1,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
     483,    -1,    -1,    -1,   463,    53,    -1,    55,    -1,    -1,
     853,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,   487,    -1,
      -1,    -1,   491,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   529,    -1,    -1,    -1,
     533,    -1,   511,   101,   102,   103,   104,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,    -1,   126,   127,
     128,   540,    -1,    -1,   132,    -1,   134,    -1,   136,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     593,    -1,   595,    -1,    -1,   598,    -1,    -1,   601,    -1,
      -1,    -1,    -1,   606,    -1,    -1,    -1,    -1,   587,    -1,
      -1,   129,   130,   131,    -1,    -1,   134,    -1,   136,    -1,
      -1,    -1,    -1,   626,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   611,    -1,   637,    -1,   639,    -1,    -1,    -1,
      -1,   644,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     0,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,   662,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,   729,    -1,    73,    74,
      -1,    76,    77,    78,    -1,   738,    81,    -1,    83,   742,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,   109,    -1,   745,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   753,   120,    -1,   122,   123,    -1,
     125,    -1,    -1,   128,    -1,    -1,    -1,   132,    -1,   134,
       1,   136,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    -1,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    -1,    83,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   102,   103,   104,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,   132,   133,   134,     1,   136,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    -1,    -1,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    -1,    83,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,   120,    -1,   122,   123,   124,   125,    -1,
      -1,   128,    -1,    -1,    -1,   132,   133,   134,     1,   136,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    54,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    -1,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    81,    -1,
      83,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,   122,
     123,    -1,   125,    -1,    -1,   128,    -1,    -1,    -1,   132,
     133,   134,     1,   136,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    -1,    83,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,   120,    -1,   122,   123,    -1,   125,    -1,    -1,   128,
      -1,    -1,    -1,   132,   133,   134,     1,   136,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    -1,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    -1,    83,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,   120,    -1,   122,   123,    -1,
     125,    -1,    -1,   128,    -1,    -1,    -1,   132,   133,   134,
       1,   136,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    54,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    -1,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    -1,    83,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,
      -1,   122,   123,    -1,   125,    -1,    -1,   128,    -1,    -1,
      -1,   132,   133,   134,     1,   136,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    -1,    -1,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    -1,    83,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,   120,    -1,   122,   123,    -1,   125,    -1,
      -1,   128,    -1,    -1,    -1,   132,    -1,   134,     1,   136,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    -1,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    81,    -1,
      83,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,   122,
     123,    -1,   125,    -1,    -1,   128,    -1,    -1,    -1,   132,
      -1,   134,     1,   136,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    -1,    83,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,   120,    -1,   122,   123,    -1,   125,    -1,    -1,   128,
      -1,    -1,    -1,   132,    -1,   134,     1,   136,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    -1,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    -1,    83,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,   120,    -1,   122,   123,    -1,
     125,    -1,    -1,   128,    -1,    -1,    -1,   132,    -1,   134,
       1,   136,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    -1,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    -1,    83,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,
      -1,   122,   123,    -1,   125,    -1,    -1,   128,    -1,    -1,
      -1,   132,    -1,   134,     1,   136,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    -1,    -1,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    -1,    83,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,   120,    -1,   122,   123,    -1,   125,    -1,
      -1,   128,    -1,    -1,    -1,   132,    -1,   134,     1,   136,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    -1,
      73,    74,    -1,    76,    77,    78,    -1,    -1,    81,    -1,
      83,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,   122,
     123,    -1,   125,    -1,    -1,   128,    -1,    -1,    -1,   132,
      -1,   134,     1,   136,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    -1,    76,    77,    78,
      -1,    -1,    81,    -1,    83,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,   120,    -1,   122,   123,    -1,   125,    -1,    -1,   128,
      -1,    -1,    -1,   132,    -1,   134,     1,   136,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    -1,    73,    74,
      -1,    76,    77,    78,    -1,    -1,    81,    -1,    83,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,   120,    -1,   122,   123,    -1,
     125,    -1,    -1,   128,    -1,    -1,    -1,   132,    -1,   134,
       1,   136,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    -1,    73,    74,    -1,    76,    77,    78,    -1,    -1,
      81,    -1,    83,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,
      -1,   122,   123,    -1,   125,    -1,    -1,   128,    -1,    -1,
      -1,   132,    -1,   134,     1,   136,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    -1,    -1,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    81,    -1,    83,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,   120,    -1,   122,   123,    -1,   125,    -1,
      -1,   128,    -1,    -1,    -1,   132,    -1,   134,     1,   136,
       3,     4,     5,     6,     7,     8,    -1,    10,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    60,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    -1,    -1,   126,   127,   128,    -1,    -1,    -1,   132,
      -1,   134,     1,   136,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,   128,
      -1,    -1,    -1,   132,    -1,   134,    -1,   136,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,   120,    -1,   122,   123,   124,
      -1,    -1,    -1,   128,    -1,    -1,    -1,   132,    -1,   134,
      -1,   136,   137,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,   103,   104,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
     120,    -1,   122,   123,   124,    -1,    -1,    -1,   128,    -1,
      -1,    -1,   132,    -1,   134,    -1,   136,   137,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,   120,    -1,   122,   123,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    -1,   132,    -1,   134,
      -1,   136,   137,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    34,    35,    -1,    37,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,   103,   104,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
     120,    -1,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    -1,   132,    -1,   134,    -1,   136,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,   120,    -1,   122,   123,   124,    -1,
      -1,    -1,   128,    -1,    -1,    -1,   132,    -1,   134,    -1,
     136,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,
     122,   123,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,
     132,    -1,   134,   135,   136,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,   120,    -1,   122,   123,   124,    -1,    -1,    -1,
     128,    -1,    -1,    -1,   132,    -1,   134,   135,   136,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,   120,    -1,   122,   123,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,   132,    -1,
     134,    -1,   136,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,   103,   104,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
     120,    -1,   122,   123,   124,    -1,    -1,    -1,   128,    -1,
      -1,    -1,   132,   133,   134,    -1,   136,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,   120,    -1,   122,   123,   124,    -1,
      -1,    -1,   128,    -1,    -1,    -1,   132,    -1,   134,    -1,
     136,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,
     122,   123,    -1,   125,    -1,    -1,   128,    -1,    -1,    -1,
     132,    -1,   134,    -1,   136,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
     128,    -1,    -1,    -1,   132,    -1,   134,    -1,   136,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,   120,    -1,   122,   123,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,   132,    -1,
     134,    -1,   136,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    47,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,   103,   104,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
     120,    -1,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    -1,   132,    -1,   134,    -1,   136,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,   120,    -1,   122,   123,   124,    -1,
      -1,    -1,   128,    -1,    -1,    -1,   132,    -1,   134,    -1,
     136,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,
     122,   123,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,
     132,    -1,   134,    -1,   136,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,   120,    -1,   122,   123,   124,    -1,    -1,    -1,
     128,    -1,    -1,    -1,   132,    -1,   134,    -1,   136,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,   120,    -1,   122,   123,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,   132,    -1,
     134,    -1,   136,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,   103,   104,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
     120,    -1,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    -1,   132,    -1,   134,    -1,   136,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    -1,   132,    -1,   134,    -1,
     136,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,
     122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,
     132,    -1,   134,    -1,   136,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
     128,    -1,    -1,    -1,   132,    -1,   134,    -1,   136,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,   120,    -1,   122,   123,
      -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,   132,    -1,
     134,    -1,   136,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,   103,   104,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
     120,    -1,   122,   123,    -1,    -1,    -1,    -1,   128,    -1,
      -1,    -1,   132,    -1,   134,    -1,   136,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,   120,    -1,   122,   123,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    -1,   132,    -1,   134,    -1,
     136,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,   120,    -1,
     122,   123,    -1,    -1,    -1,    -1,   128,    -1,    -1,    -1,
     132,    -1,   134,    -1,   136,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,   120,    -1,   122,   123,    -1,    -1,    -1,    -1,
     128,    -1,    -1,    -1,   132,    -1,   134,    -1,   136,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    23,    -1,
      12,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,   102,    31,
      32,    33,   106,    35,    -1,    -1,   110,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    60,   120,    -1,    -1,    64,
     124,    53,     3,    55,    -1,    -1,    -1,    -1,   132,    10,
     134,   135,   136,    -1,    15,    67,    68,    69,    70,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,   103,   104,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,   114,
     102,   116,   117,   118,   119,    -1,   121,   122,   123,    -1,
      -1,   126,   127,   128,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,    -1,   136,    -1,    87,    88,    89,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    -1,   123,    -1,    10,   126,   127,   128,   129,    15,
     131,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    23,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    23,    -1,   101,    64,   103,   104,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    10,   121,   122,   123,    86,    15,
     126,   127,   128,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      60,   137,    -1,   101,    64,   103,   104,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    10,   121,   122,   123,    86,    15,   126,   127,
     128,    -1,    -1,    -1,    60,    23,    -1,    -1,    64,   137,
      -1,   101,    -1,   103,   104,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
      86,   121,    -1,   123,    -1,    -1,   126,   127,   128,    -1,
      -1,    -1,    60,    -1,    -1,   101,    64,   103,   104,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    10,   121,   122,   123,    86,    15,
     126,   127,   128,    -1,    -1,    -1,    -1,    23,    24,    -1,
      -1,   137,    -1,   101,    -1,   103,   104,   105,    34,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    10,   121,   122,   123,    -1,    15,   126,   127,
     128,    -1,    -1,    -1,    60,    23,    24,    -1,    64,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    -1,    -1,   101,    64,   103,   104,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    82,   121,   122,   123,    86,    10,
     126,   127,   128,    -1,    15,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    23,   101,    -1,   103,   104,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    -1,   121,   122,   123,    -1,    10,   126,   127,
     128,    -1,    15,    -1,   132,    -1,    -1,    -1,    -1,    60,
      23,    24,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    60,    -1,    -1,
     101,    64,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    -1,
     121,   122,   123,    86,    -1,   126,   127,   128,    -1,    -1,
      -1,   132,    60,    -1,    -1,    -1,    64,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    10,   121,   122,
     123,    -1,    15,   126,   127,   128,    -1,    -1,    -1,   132,
      23,    24,    -1,   101,    -1,   103,   104,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,    -1,   121,    -1,   123,    -1,    -1,   126,   127,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    60,   121,   122,
     123,    64,    -1,   126,   127,   128,    -1,    10,    71,   132,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    60,   121,   122,
     123,    64,    -1,   126,   127,   128,    -1,    10,    71,   132,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    60,   121,   122,
     123,    64,    -1,   126,   127,   128,    -1,    10,    71,   132,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    60,   121,   122,
     123,    64,    -1,   126,   127,   128,    -1,    10,    71,   132,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    60,   121,   122,
     123,    64,    -1,   126,   127,   128,    -1,    10,    71,   132,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    60,   121,   122,
     123,    64,    -1,   126,   127,   128,    -1,    10,    71,   132,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    60,   121,   122,
     123,    64,    -1,   126,   127,   128,    -1,    10,    71,   132,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    60,   121,   122,
     123,    64,    -1,   126,   127,   128,    -1,    10,    71,   132,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    60,   121,   122,
     123,    64,    -1,   126,   127,   128,    -1,    10,    -1,   132,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    60,   121,   122,
     123,    64,    -1,   126,   127,   128,    -1,    10,    -1,   132,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    60,   121,   122,
     123,    64,    -1,   126,   127,   128,    -1,    -1,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    10,   121,   122,
     123,    -1,    15,   126,   127,   128,    -1,    -1,    -1,    60,
      23,    -1,    -1,    64,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    49,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    60,    23,    24,
     101,    64,   103,   104,   105,    -1,   107,   108,   109,    34,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,    10,
      -1,    -1,   123,    86,    15,   126,   127,   128,    -1,    -1,
      -1,    -1,    23,    24,    -1,    60,    -1,    -1,   101,    64,
     103,   104,   105,    34,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    10,   121,   122,
     123,    86,    15,   126,   127,   128,    -1,    -1,    -1,    60,
      23,    -1,    -1,    64,    -1,    -1,   101,    -1,   103,   104,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    86,   121,   122,   123,    -1,
      -1,   126,   127,   128,    -1,    -1,    -1,    60,    -1,    -1,
     101,    64,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    10,
     121,   122,   123,    86,    15,   126,   127,   128,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,    -1,   126,   127,   128,    -1,    10,
      71,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,    -1,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,   125,   126,   127,   128,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    60,
     121,   122,   123,    64,    -1,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,   128,    60,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
      -1,   103,   104,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    60,   121,
     122,   123,    64,    -1,   126,   127,   128,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
      -1,   103,   104,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    60,   121,
     122,   123,    64,    -1,   126,   127,   128,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
      -1,   103,   104,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    60,   121,
     122,   123,    64,    -1,   126,   127,   128,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
      -1,   103,   104,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    60,   121,
     122,   123,    64,    -1,   126,   127,   128,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
      -1,   103,   104,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    60,   121,
     122,   123,    64,    -1,   126,   127,   128,    -1,    10,    71,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
      -1,   103,   104,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    60,   121,
     122,   123,    64,    -1,   126,   127,   128,    -1,    -1,    71,
      -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
      -1,   103,   104,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,   121,
     122,   123,    -1,    60,   126,   127,   128,    64,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,   103,   104,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    60,   121,   122,   123,    64,    -1,   126,
     127,   128,    -1,    10,    71,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,   103,   104,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    60,   121,   122,   123,    64,    -1,   126,
     127,   128,    -1,    10,    71,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,   103,   104,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    60,   121,   122,   123,    64,    -1,   126,
     127,   128,    -1,    10,    71,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,   103,   104,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    60,   121,   122,   123,    64,    -1,   126,
     127,   128,    -1,    10,    71,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,   103,   104,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    60,   121,   122,   123,    64,    -1,   126,
     127,   128,    -1,    10,    71,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,   103,   104,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    60,   121,   122,   123,    64,    -1,   126,
     127,   128,    -1,    10,    71,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,   103,   104,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    60,   121,   122,   123,    64,    -1,   126,
     127,   128,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,   103,   104,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    60,   121,   122,   123,    64,    -1,   126,
     127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,   103,   104,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    -1,   121,   122,   123,    -1,    -1,   126,
     127,   128
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   142,   143,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    16,    17,    18,    19,    21,    22,
      24,    25,    27,    29,    30,    31,    32,    33,    35,    36,
      39,    40,    41,    42,    43,    45,    46,    48,    53,    54,
      55,    56,    58,    59,    62,    63,    65,    66,    67,    68,
      69,    70,    73,    74,    76,    77,    81,    83,    86,   101,
     102,   103,   104,   109,   118,   120,   122,   123,   125,   128,
     132,   134,   136,   144,   145,   146,   147,   148,   149,   154,
     155,   156,   158,   161,   162,   163,   164,   165,   167,   168,
     169,   172,   173,   176,   179,   182,   183,   205,   208,   209,
     227,   228,   229,   230,   231,   232,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   250,   251,   252,   253,
     254,   146,   242,    82,   233,   234,   157,   158,   233,    12,
      31,    32,    33,    70,   132,   136,   179,   227,   231,   240,
     241,   242,   243,   245,   246,    74,   157,   242,   146,   134,
     158,     7,   157,   159,     9,    74,   159,    52,    84,   166,
     242,   242,   242,   134,   158,   180,   134,   158,   210,   211,
     146,   242,   242,   242,   242,     7,   134,    19,    30,   163,
     163,   124,   201,   221,   242,   125,   242,   242,    24,   149,
     160,   242,   242,    69,   134,   146,   242,   132,   149,   158,
     206,   221,   242,   242,   242,   242,   242,   242,   242,   242,
     133,   144,   150,   221,    75,   110,   201,   222,   223,   242,
     221,   242,   249,    54,   146,    44,   158,    38,    57,   196,
      20,    52,    61,    78,   125,   131,    10,    15,    23,    60,
      64,    86,   101,   103,   104,   105,   107,   108,   109,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   121,   122,
     123,   126,   127,   128,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   129,   130,
     134,   136,    60,    64,   134,   146,   125,   149,   242,   242,
     242,   221,    34,   233,   206,   125,   125,    81,   158,   201,
     224,   225,   226,   242,   132,   206,   173,   158,   134,   160,
      24,    34,   160,    24,    34,    82,   160,   236,    71,   149,
     224,   146,   134,   190,    75,   134,   158,   212,   213,     1,
     105,   215,   216,    34,   106,   160,   224,   159,   158,   106,
     125,   125,   132,   146,   160,   134,   224,    87,   207,   125,
     125,    28,    49,   160,   125,   133,   144,   106,   133,   242,
     106,   135,   106,   135,    34,   106,   137,   236,    85,   106,
     137,     7,   158,   105,   174,   184,    61,   210,   210,   210,
     210,   242,   242,   242,   242,   166,   242,   166,   242,   242,
     242,   242,   242,   242,   242,    25,    74,   158,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     224,   224,   166,   242,   166,   242,    20,    34,    37,    51,
      52,    61,    74,   194,   235,   238,   242,   254,    24,    34,
      24,    34,    71,    34,   137,   166,   242,   160,   125,   242,
      85,    87,   135,   106,   158,   177,   178,   125,   158,    34,
     221,    33,   242,   166,   242,    33,   242,   166,   242,   134,
     160,   146,   242,    26,   135,   191,   192,   193,   194,   181,
     213,   106,   135,     1,   136,   217,   228,    85,    87,   214,
     242,   211,   135,   201,   242,   170,   224,   135,   136,   217,
     228,   106,   128,   151,   153,   242,   151,   152,   133,   135,
     135,   222,   135,   222,   166,   242,   137,   146,   242,   137,
     242,   137,   242,   132,   221,   132,    52,    61,    74,   186,
     195,   210,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   135,   137,    34,    61,   227,   106,   135,    60,
      60,    33,   166,   242,    33,   166,   242,   166,   242,   233,
     233,   125,   242,   201,   242,   226,    87,   106,   133,   174,
     242,   135,   242,    24,   160,    24,   160,   242,    24,   160,
     236,    24,   160,   236,   237,   238,    26,    26,   146,   106,
     135,   134,   158,    20,    52,    61,    74,   197,   135,   213,
     105,   216,   221,   242,    47,   242,    50,    79,   133,   171,
     135,   221,   206,   106,   125,    11,   125,   137,   236,   137,
     236,   146,    85,   133,   150,   175,   185,    10,    15,    87,
      88,    89,    90,    91,    92,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   107,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   123,   126,   127,
     128,   129,   131,   158,   187,   188,   125,   238,   227,   227,
     242,    24,    24,   242,    24,    24,   137,   137,   160,   160,
     242,   178,   132,   160,    71,    33,   242,    33,   242,    71,
      33,   242,   160,    33,   242,   160,   106,   135,   146,   242,
     192,   213,   105,   220,    61,   216,   214,    34,   137,    24,
     146,   221,   137,   242,   242,    33,   146,   242,   137,    33,
     146,   242,   137,   242,   133,    30,    56,    58,   133,   145,
     162,   197,   158,   108,   134,   189,   189,    71,    33,    33,
      71,    33,    33,    33,    33,   175,   242,   242,   242,   242,
     242,   242,   238,   135,    25,    67,    70,   136,   201,   219,
     228,   110,   203,   214,    80,   204,   242,   217,   228,   146,
     160,    11,   242,   146,   242,   146,   162,   216,   158,   187,
     188,   191,   242,   242,   242,   242,   242,   242,   133,    71,
      71,    71,    71,   220,   137,   221,   201,   202,   242,   242,
     149,   161,   200,   137,   242,    71,    71,    72,   198,   189,
     189,   135,    71,    71,    71,    71,    71,    71,   242,   242,
     242,   242,   203,   214,   201,   218,   219,   228,    34,   137,
     228,   242,   242,   204,   242,   218,   219,   125,   199,   200,
     137,   218
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   141,   142,   143,   143,   144,   144,   145,   145,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   147,   147,   148,   148,
     148,   149,   149,   150,   150,   151,   151,   151,   151,   152,
     152,   153,   153,   154,   154,   154,   155,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   157,   157,   158,   159,   159,   160,   160,
     161,   161,   162,   162,   162,   162,   162,   162,   162,   163,
     163,   164,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   166,   167,   167,   167,   167,   168,
     169,   170,   170,   171,   171,   171,   172,   172,   173,   173,
     173,   174,   174,   175,   175,   175,   176,   177,   177,   177,
     178,   178,   180,   181,   179,   182,   182,   182,   182,   184,
     185,   183,   186,   186,   186,   186,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   189,   189,   190,   191,   191,   191,
     192,   192,   192,   192,   193,   193,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   195,   195,   195,   195,   196,
     196,   197,   197,   197,   197,   197,   197,   198,   198,   199,
     199,   200,   200,   201,   201,   202,   202,   203,   203,   204,
     204,   205,   205,   205,   206,   206,   207,   207,   207,   208,
     208,   208,   208,   208,   209,   209,   209,   210,   210,   211,
     211,   211,   212,   212,   212,   213,   213,   213,   214,   214,
     214,   215,   215,   216,   216,   216,   216,   217,   217,   217,
     217,   218,   218,   218,   219,   219,   219,   219,   219,   220,
     220,   220,   220,   220,   220,   220,   221,   221,   221,   221,
     222,   222,   222,   223,   223,   224,   224,   225,   225,   226,
     226,   226,   226,   226,   227,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   230,   231,   232,   232,   232,   232,
     232,   232,   232,   232,   233,   233,   234,   235,   235,   236,
     237,   237,   238,   238,   238,   239,   239,   240,   241,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   243,   243,   243,   243,   244,   244,   245,
     245,   245,   246,   246,   246,   247,   247,   247,   247,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     249,   249,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   251,   251,   251,   251,   251,
     251,   252,   252,   252,   252,   253,   253,   253,   253,   254,
     254,   254,   254,   254,   254,   254
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
       0,    10,     3,     3,     5,     5,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     3,     0,     1,     3,
       4,     4,     6,     6,     0,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     0,     1,     1,     1,     1,
       1,     0,     1,     2,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     2,     1,     0,     1,     2,     2,     0,
       2,     3,     4,     4,     2,     4,     0,     2,     2,     4,
       4,     4,     5,     4,     0,     1,     1,     1,     3,     3,
       4,     5,     1,     1,     3,     1,     2,     3,     0,     2,
       2,     0,     4,     0,     2,     2,     1,     4,     4,     6,
       1,     0,     1,     1,     3,     4,     3,     4,     6,     0,
       2,     2,     2,     2,     2,     2,     1,     1,     3,     3,
       1,     1,     1,     3,     3,     0,     1,     1,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     4,     4,     4,     5,     2,     2,     6,     6,     4,
       9,     9,     7,     6,     6,     4,     9,     9,     7,     4,
       6,     6,     9,     9,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     1,     4,     1,     3,     4,
       1,     3,     2,     3,     3,     1,     3,     2,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     3,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     4,     3,     3,     3,     3,     4,     3,     4,     1,
       1,     1,     1,     1,     3,     4,     3,     4,     3,     4,
       3,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1
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
#line 424 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 4766 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 429 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4772 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 430 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4778 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 437 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4784 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 442 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4790 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 443 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4796 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4802 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 461 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4808 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 462 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4814 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 463 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4820 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 464 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4826 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4832 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 466 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4838 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 467 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4844 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 468 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4850 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 469 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4856 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 470 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4862 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4868 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 472 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4874 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 473 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4880 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 474 "chapel.ypp" /* yacc.c:1661  */
    { printf("syntax error"); clean_exit(1); }
#line 4886 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yyvsp[-4].b), (yylsp[-4]).comment))); }
#line 4892 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-5].b), (yylsp[-5]).comment))); }
#line 4898 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4904 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4910 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4916 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 502 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 4922 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 4928 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 509 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4940 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 4950 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 4960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 524 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 4969 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 528 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 4978 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 534 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 4988 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 543 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 4998 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 552 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5004 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5010 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5016 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5022 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 5028 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 5034 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 5040 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 5046 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 5052 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 567 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 5058 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 568 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 5064 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 5070 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 570 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 5076 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 5082 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 5088 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 5094 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5100 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5106 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5112 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5118 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5124 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5130 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 590 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5136 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 591 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5142 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 595 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5148 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5154 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 5160 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 601 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5166 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 605 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5172 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5178 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 608 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5184 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 621 "chapel.ypp" /* yacc.c:1661  */
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
#line 5200 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5206 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 636 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5212 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5218 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5224 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5230 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5236 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5242 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5248 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 643 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5254 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 644 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5260 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 645 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5266 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt)); }
#line 5272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), zipToTuple((yyvsp[-1].pcallexpr)), NULL, (yyvsp[0].pblockstmt), true); }
#line 5278 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 648 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt), true); }
#line 5284 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 649 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5290 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 650 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt)); }
#line 5296 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 652 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5306 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 658 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5316 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 664 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5326 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 670 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-3].pcallexpr)), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5336 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 676 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5347 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 683 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5358 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 692 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5364 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5370 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5376 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5382 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 699 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5388 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 703 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 5394 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 708 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5400 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 712 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5406 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 713 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5412 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 718 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5418 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 720 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5424 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 722 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5430 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 729 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), NULL, (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5439 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 734 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-6].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5448 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 741 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5458 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 746 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5468 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 751 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5478 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 759 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 5484 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 760 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 5490 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 765 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5496 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 767 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 5502 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 769 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 5508 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 774 "chapel.ypp" /* yacc.c:1661  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 5519 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 784 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[0].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[0].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
#line 5532 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 793 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 5540 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 797 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 5549 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 804 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 5555 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 805 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 5561 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 810 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 815 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5579 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 820 "chapel.ypp" /* yacc.c:1661  */
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
#line 5599 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 840 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5610 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 846 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5622 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 853 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5635 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 861 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 872 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 878 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5669 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 885 "chapel.ypp" /* yacc.c:1661  */
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
#line 5692 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 907 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5700 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 911 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5709 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 916 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
    }
#line 5717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 920 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5726 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 928 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5732 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 929 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 5738 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 930 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 5744 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 931 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 5750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 932 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 5756 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 933 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 5762 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 934 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 5768 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 935 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 5774 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 936 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 5780 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 937 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 5786 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 938 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 5792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 939 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 5798 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 940 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 5804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 941 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 5810 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 942 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 5816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 943 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 5822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 944 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 5828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 945 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 5834 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 946 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 5840 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 947 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 5846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 948 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 5852 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 949 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 5858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 950 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 5864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 951 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 5870 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 952 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 5876 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 956 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 5882 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 957 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 5888 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 958 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 5894 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 959 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 5900 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 960 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 5906 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 961 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 5912 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 962 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 5918 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 963 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 5924 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 964 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 5930 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 965 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 5936 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 966 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 5942 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 967 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 5948 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 971 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 5954 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 972 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 976 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5966 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 980 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 5972 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 981 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 5978 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 982 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 5984 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 987 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 5990 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 989 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 5996 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 991 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6002 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 993 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6008 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 997 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6014 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 998 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6020 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1002 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 6026 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1003 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6032 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1004 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 6038 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1005 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 6044 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1006 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6050 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1007 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6056 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1008 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6062 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1009 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 6068 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1010 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6074 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1014 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6080 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1015 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6086 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1016 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 6092 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1017 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6098 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1021 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6104 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1022 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6110 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1026 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6116 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1027 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6122 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1028 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6128 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1029 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 6134 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1030 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 6140 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1031 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 6146 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1035 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 6152 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1036 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 6158 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1039 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 6164 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1045 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 6170 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1050 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6176 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1052 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6182 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1056 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6188 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1061 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6194 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1062 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6200 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1066 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6206 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1067 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6212 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1072 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 6218 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1074 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 6224 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1076 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 6230 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1081 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 6247 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1094 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 6265 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1110 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6271 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1112 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6277 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1114 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 6283 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1119 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6295 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1127 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6307 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1135 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6319 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1143 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 6332 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1152 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6343 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1161 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6349 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1162 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6355 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1163 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6361 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1169 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6370 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1177 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6376 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1179 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 6386 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1185 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6392 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1190 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 6398 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1192 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6404 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 6410 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1199 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 6416 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1201 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6422 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1203 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6428 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1209 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6434 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1210 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 6440 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1211 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6446 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1215 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6452 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1217 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6458 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1222 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6470 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1223 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 6476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1224 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad type specification"); }
#line 6482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1245 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 6490 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1249 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 6498 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1253 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 6510 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1260 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad array type specification"); clean_exit(1); }
#line 6516 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1264 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6522 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1265 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6528 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1266 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6534 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1271 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6540 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1273 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6546 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1279 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6552 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1281 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6558 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1283 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 6564 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1287 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1288 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6576 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1289 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6582 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1290 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 6588 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1291 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6594 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1292 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6600 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1293 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6606 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1299 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6612 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1300 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1301 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1302 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6630 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1306 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6636 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1307 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6642 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1308 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1312 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6654 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1313 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1317 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1322 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6672 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1323 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1327 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6684 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1328 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1329 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1330 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1335 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6708 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1352 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6714 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1354 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6720 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1356 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6726 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6732 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1360 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 6738 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1362 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6744 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1364 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1369 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6756 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1371 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 6762 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1373 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6768 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1375 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6774 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1377 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6780 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1379 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6786 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1381 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1383 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 6798 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1385 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1387 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6810 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1389 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1391 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1393 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 6833 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1400 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 6843 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1406 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 6853 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1412 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 6863 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1418 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 6873 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1427 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 6879 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1431 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 6885 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1447 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6891 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1451 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6897 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1456 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 6903 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1462 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 6910 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1465 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 6916 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1469 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pForallIntents) = (yyvsp[-1].pForallIntents); }
#line 6922 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1475 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pForallIntents) = new ForallIntents();
      addForallIntent((yyval.pForallIntents), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 6929 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1478 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pForallIntents), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 6935 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1483 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[-1].pt); (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
#line 6943 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).riExp = (yyvsp[-2].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); }
#line 6949 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).riExp = (yyvsp[-2].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); }
#line 6955 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6961 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 6967 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 6973 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 6979 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6985 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1523 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6991 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1525 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 6997 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1527 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 7003 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1529 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 7009 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1552 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7015 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7021 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 7027 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7033 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 7039 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1560 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 7045 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1568 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7051 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 7057 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1570 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 7063 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 7069 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 7075 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 7081 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 7087 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 7093 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 7099 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1580 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 7105 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 7111 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1582 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 7117 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1583 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 7123 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1585 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 7131 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1589 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 7139 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7145 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1597 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7151 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1601 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7157 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7163 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1603 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7169 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7175 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1605 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7181 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7187 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7193 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7199 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1609 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7205 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7211 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1611 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7217 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7223 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1613 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7229 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7235 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1615 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7241 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7247 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1617 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7253 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7259 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1619 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7265 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7271 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7277 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7283 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7289 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 7295 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1628 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 7301 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 7307 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1630 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 7313 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 7319 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 7325 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1636 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7331 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7337 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7343 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7349 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1643 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7355 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1644 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7361 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1645 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7367 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7373 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1651 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 7379 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1652 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 7385 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1653 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 7391 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1654 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 7397 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 453:
#line 1655 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 7403 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 454:
#line 1656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 7409 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 455:
#line 1657 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 7415 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 7419 "bison-chapel.cpp" /* yacc.c:1661  */
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
