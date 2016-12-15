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
    TTHROW = 327,
    TTHROWS = 328,
    TTRY = 329,
    TTRYBANG = 330,
    TTYPE = 331,
    TUNDERSCORE = 332,
    TUNION = 333,
    TUSE = 334,
    TVAR = 335,
    TWHEN = 336,
    TWHERE = 337,
    TWHILE = 338,
    TWITH = 339,
    TYIELD = 340,
    TZIP = 341,
    TALIAS = 342,
    TAND = 343,
    TASSIGN = 344,
    TASSIGNBAND = 345,
    TASSIGNBOR = 346,
    TASSIGNBXOR = 347,
    TASSIGNDIVIDE = 348,
    TASSIGNEXP = 349,
    TASSIGNLAND = 350,
    TASSIGNLOR = 351,
    TASSIGNMINUS = 352,
    TASSIGNMOD = 353,
    TASSIGNMULTIPLY = 354,
    TASSIGNPLUS = 355,
    TASSIGNSL = 356,
    TASSIGNSR = 357,
    TBAND = 358,
    TBNOT = 359,
    TBOR = 360,
    TBXOR = 361,
    TCOLON = 362,
    TCOMMA = 363,
    TDIVIDE = 364,
    TDOT = 365,
    TDOTDOT = 366,
    TDOTDOTDOT = 367,
    TEQUAL = 368,
    TEXP = 369,
    TGREATER = 370,
    TGREATEREQUAL = 371,
    THASH = 372,
    TLESS = 373,
    TLESSEQUAL = 374,
    TMINUS = 375,
    TMOD = 376,
    TNOT = 377,
    TNOTEQUAL = 378,
    TOR = 379,
    TPLUS = 380,
    TQUESTION = 381,
    TSEMI = 382,
    TSHIFTLEFT = 383,
    TSHIFTRIGHT = 384,
    TSTAR = 385,
    TSWAP = 386,
    TASSIGNREDUCE = 387,
    TIO = 388,
    TLCBR = 389,
    TRCBR = 390,
    TLP = 391,
    TRP = 392,
    TLSBR = 393,
    TRSBR = 394,
    TNOELSE = 395,
    TUPLUS = 396,
    TUMINUS = 397
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

#line 414 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 420 "bison-chapel.cpp" /* yacc.c:358  */
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

#line 487 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   13395

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  143
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  115
/* YYNRULES -- Number of rules.  */
#define YYNRULES  458
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  868

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   397

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
     135,   136,   137,   138,   139,   140,   141,   142
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   425,   425,   430,   431,   437,   438,   443,   444,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   480,   482,   487,
     488,   489,   504,   505,   510,   511,   516,   521,   526,   530,
     536,   541,   545,   550,   554,   555,   556,   560,   564,   565,
     566,   567,   568,   569,   570,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   583,   584,   588,   592,   593,   597,
     598,   602,   603,   607,   608,   609,   610,   611,   612,   613,
     617,   618,   622,   637,   638,   639,   640,   641,   642,   643,
     644,   645,   646,   647,   648,   649,   650,   651,   652,   653,
     659,   665,   671,   677,   684,   694,   698,   699,   700,   701,
     705,   706,   710,   713,   718,   719,   723,   725,   727,   734,
     739,   747,   752,   757,   765,   766,   771,   772,   774,   779,
     789,   798,   802,   810,   811,   816,   821,   815,   846,   852,
     859,   867,   878,   884,   877,   912,   916,   921,   925,   933,
     934,   935,   936,   937,   938,   939,   940,   941,   942,   943,
     944,   945,   946,   947,   948,   949,   950,   951,   952,   953,
     954,   955,   956,   957,   958,   962,   963,   964,   965,   966,
     967,   968,   969,   970,   971,   972,   973,   977,   978,   982,
     986,   987,   988,   992,   994,   996,   998,  1003,  1004,  1008,
    1009,  1010,  1011,  1012,  1013,  1014,  1015,  1016,  1020,  1021,
    1022,  1023,  1027,  1028,  1032,  1033,  1034,  1035,  1036,  1037,
    1041,  1042,  1045,  1046,  1050,  1051,  1055,  1057,  1062,  1063,
    1067,  1068,  1072,  1073,  1077,  1079,  1081,  1086,  1099,  1116,
    1117,  1119,  1124,  1132,  1140,  1148,  1157,  1167,  1168,  1169,
    1173,  1174,  1182,  1184,  1190,  1195,  1197,  1199,  1204,  1206,
    1208,  1215,  1216,  1217,  1221,  1222,  1227,  1228,  1229,  1230,
    1250,  1254,  1258,  1266,  1270,  1271,  1272,  1276,  1278,  1284,
    1286,  1288,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1305,
    1306,  1307,  1308,  1312,  1313,  1314,  1318,  1319,  1323,  1324,
    1328,  1329,  1333,  1334,  1335,  1336,  1337,  1341,  1352,  1353,
    1354,  1355,  1356,  1357,  1359,  1361,  1363,  1365,  1367,  1369,
    1374,  1376,  1378,  1380,  1382,  1384,  1386,  1388,  1390,  1392,
    1394,  1396,  1398,  1405,  1411,  1417,  1423,  1432,  1437,  1445,
    1446,  1447,  1448,  1449,  1450,  1451,  1452,  1457,  1458,  1462,
    1467,  1470,  1475,  1480,  1483,  1488,  1492,  1493,  1497,  1498,
    1503,  1508,  1516,  1517,  1518,  1519,  1520,  1521,  1522,  1523,
    1524,  1526,  1528,  1530,  1532,  1534,  1539,  1540,  1541,  1542,
    1553,  1554,  1558,  1559,  1560,  1564,  1565,  1566,  1574,  1575,
    1576,  1577,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,
    1589,  1590,  1594,  1602,  1603,  1607,  1608,  1609,  1610,  1611,
    1612,  1613,  1614,  1615,  1616,  1617,  1618,  1619,  1620,  1621,
    1622,  1623,  1624,  1625,  1626,  1627,  1628,  1629,  1633,  1634,
    1635,  1636,  1637,  1638,  1642,  1643,  1644,  1645,  1649,  1650,
    1651,  1652,  1657,  1658,  1659,  1660,  1661,  1662,  1663
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
  "TTHROW", "TTHROWS", "TTRY", "TTRYBANG", "TTYPE", "TUNDERSCORE",
  "TUNION", "TUSE", "TVAR", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD",
  "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR",
  "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND",
  "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY",
  "TASSIGNPLUS", "TASSIGNSL", "TASSIGNSR", "TBAND", "TBNOT", "TBOR",
  "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT",
  "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL", "THASH", "TLESS",
  "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL", "TOR", "TPLUS",
  "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP",
  "TASSIGNREDUCE", "TIO", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR",
  "TNOELSE", "TUPLUS", "TUMINUS", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "module_decl_stmt",
  "access_control", "block_stmt", "stmt_ls", "only_ls", "opt_only_ls",
  "except_ls", "use_stmt", "require_stmt", "assignment_stmt", "opt_ident",
  "ident", "opt_ident_or_string", "do_stmt", "return_stmt",
  "class_level_stmt", "private_decl", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "try_stmt", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_ls",
  "enum_item", "lambda_decl_expr", "$@1", "$@2", "linkage_spec",
  "fn_decl_stmt", "$@3", "$@4", "fn_decl_stmt_inner", "fn_ident",
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
     395,   396,   397
};
# endif

#define YYPACT_NINF -769

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-769)))

#define YYTABLE_NINF -410

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -769,    12,  2578,  -769,  -769,  -769,  -769,  -769,  -769,  -769,
    -769,  3666,     5,    94,  -769,     5,  7749,     6,    94,  7749,
    3666,    24,    94,   363,   664,  6389,  7749,  7749,    77,  -769,
      94,  -769,    27,  3666,  7749,  7749,  -769,  7749,  7749,    97,
     149,   433,   744,  -769,  6661,  6797,  7749,  6933,  7749,    70,
     163,  3666,  7749,  3666,  3666,    94,  -769,  6661,  7749,  7749,
    -769,  -769,  7749,  -769,  -769,  9109,  7749,  7749,  -769,  7749,
    -769,  -769,  2850,  5981,  6661,  -769,  3530,  -769,  -769,   104,
    -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,
    -769,  -769,  -769,  -769,  -769,    94,  -769,    79,    96,  -769,
    -769,  -769,    32,    13,  -769,  -769,  -769,    92,   100,   167,
     190,   204, 13093,  5145,   261,   213,   226,  -769,  -769,  -769,
    -769,  -769,  -769,   150,  -769, 13093,   233,  3666,  -769,   259,
    -769,   262,  7749,  7749,  7749,  7749,  7749,  6661,  6661,   287,
    -769,  -769,  -769,  -769, 10468,   291,  -769,  -769,    94,   275,
   10730,   326,  6661,   271,  -769,  -769,  -769,  -769,    94,    52,
      94,   281,    18,  9545,  9511,  9662,  6661,  3666,   298,    26,
      80,    39,  -769,  -769,   503,   302,  9700,   503,  -769,  6661,
    -769,   286,  -769,  -769,    94,  -769,    91, 13093,  -769, 10764,
   10339,  3666,  -769,  -769,  9700, 13093,   306,  6661,  -769, 13093,
   10853,  -769,  -769,   341,   311,   179, 10580,  9700, 10887,   312,
    1616,   503,   312,   503,  -769,  -769,  2986,    90,  -769,  7749,
    -769,   -49,   -32, 13093,   250, 10976,    43,   444,  -769,    94,
     347,  -769,  -769,  -769,    25,    27,    27,    27,  -769,  7749,
    7749,  7749,  7749,  7069,  7069,  7749,  7749,  7749,  7749,  7749,
    7749,   289,  9109,  7749,  7749,  7749,  7749,  7749,  7749,  7749,
    7749,  7749,  7749,  7749,  7749,  7749,  7749,  7749,  7749,  7749,
    7749,  7749,  7749,  7749,  7749,  7749,  7749,  7749,  7749,  7749,
    7749,  7749,  7749,  7749,  6661,  6661,  7069,  7069,  5845,  -769,
    -769,  3122,  -769, 10618, 10692, 11038,    41,  7069,    18,   330,
    -769,  -769,  7749,   346,  -769,   328,   359,  -769, 13093,    94,
     345,    94,   434,  6661,  -769,  3802,  7069,  -769,  3938,  7069,
     337,  -769,    18,  3666,   452,   339,  -769,    57,  -769,  -769,
      26,  -769,   371,   343,  -769,  5162,   395,   394,  7749,    27,
    -769,   348,  -769,  -769,  6661,  -769,  -769,  -769,  -769,  -769,
    6661,   349,  -769,  5298,   380,  -769,  -769,  7885,  7749,  -769,
    -769,  -769,  -769,  6525,  -769,  9399,  6117,  -769,  6253,  -769,
    7069,  5434,  2714,   352,  7749,  5708,  -769,  -769,   362,  6661,
     366,   242,    27,   193,   221,   224,   265, 10401,  1311,  1311,
     294,  -769,   294,  -769,   294, 13232,   996,   787,  1136,   302,
     312,  -769,  -769,  -769,  1616, 13265,   294,  1165,  1165,  1311,
    1165,  1165,  1692,   312, 13265, 13203,  1692,   503,   503,   312,
   11100, 11162, 11224, 11286, 11348, 11410, 11472, 11534, 11596, 11658,
   11720, 11782, 11844, 11906, 11968, 12030,   368,   372,  -769,   294,
    -769,   294,   288,  -769,  -769,  -769,  -769,  -769,  -769,    94,
      -9,  -769, 13155,   331,  8021,  7069,  8157,  7069,  7749,  7069,
    2206,     5, 12092,  -769,  -769, 12154,  7749,  6661,  -769,  6661,
     418,   215,  -769,  -769,   347,  7749,    46,  7749, 13093,    56,
    9734,  7749, 13093,    47,  9583,  5845,  -769,   486, 12225,  3666,
    -769,    66,  -769,    29,  -769,   322,   377,    26,    49,  -769,
    6661,  -769,   248,  7749,  7205,  -769, 13093,  -769,  -769,  -769,
   13093,   -28,   379,  -769,  6661,  -769,   273,    94,   391,   412,
     397, 12287,   412,   402,  -769,  -769,  -769,  -769,  -769,  -769,
     -10,  9234,  -769,  -769, 13093,  3666, 13093,  -769, 12349,  3258,
     413,  -769,  -769,  -769,  -769,  -769,  2072,   276,  -769,  -769,
    -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,
    -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,
    -769,  -769,  -769,  5845,  -769,  7069,  7069,  7749,   498, 12411,
    7749,   507, 12473,   404,  1445,    18,    18,  -769, 13093,  -769,
   13093,  -769,  7749,    94,  -769,   400,  9700,  -769,  9823,  4074,
    -769,  4210,  -769,  9885,  4346,  -769,    18,  4482,  -769,    18,
      84,  -769,  3666,  7749,  -769,   584,  -769,    26,   437,   487,
    -769,  -769,  -769,    38,  -769,  -769,  5298,   394,    62, 13093,
    -769, 13093,  4618,  6661,  -769,  -769,  -769,    83,  -769,  7749,
    -769,  7749,  -769,  4754,   414,  4890,   415,  -769,  7749,  -769,
    3394,   479,   322,  -769,  -769,  -769,  -769,  -769,  -769,  -769,
    -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,    94,  -769,
    -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,
    -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,   442,
     423,   423,  -769,  -769,    86,    87, 12535,  8293,  8429, 12597,
    8565,  8701,  8837,  8973,  -769,  -769, 13093,  -769,  -769,  -769,
    3666,  7749, 13093,  7749, 13093,  3666,  7749, 13093,  -769,  7749,
   13093,  -769,  5845,  -769,  -769, 13093,  -769,   424,  7341,     3,
    -769,   478,  -769,  7069,  2380,  3666,  -769,    71,  5026, 12659,
   13093,  7749,  -769, 13093,  3666,  7749,  -769, 13093,  3666, 13093,
    -769,   579,   511,   511,  -769,  1204,  -769,    37,  -769,  2072,
      57,  -769,  -769,  7749,  7749,  7749,  7749,  7749,  7749,  7749,
    7749,   964, 12225,  9952, 10014, 12225, 10081, 10143,  -769,   437,
      24,  7749,  7749,  5571,  -769,  -769,   201,  6661,  -769,  -769,
    7749,   -18,  9278,  -769,   450,   326,  -769,  7749, 10210,  -769,
   10272,  -769,  -769,   497,  -769,   423,   423,    89, 12721, 12783,
   12845, 12907, 12969, 13031,  -769,  3666,  3666,  3666,  3666,     3,
    7477,   101,  -769,  -769, 13093, 13093,  -769,  -769,  -769,  8973,
   13093,  3666,  3666,  -769,   478,  -769,  -769,  -769,  7749,  7749,
    7749,  7749,  7749,  7749, 12225, 12225, 12225, 12225,  -769,  -769,
    -769,  -769,  -769,   207,  7069,  9195,   512, 12225, 12225,    16,
    9355,  -769,  -769,  -769,  -769,  -769,  7613,  -769
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    36,    76,   402,   403,   404,   405,
     406,     0,   357,    74,   131,   357,     0,   258,    74,     0,
       0,     0,     0,    74,    74,     0,     0,     0,     0,   149,
       0,   145,     0,     0,     0,     0,   348,     0,     0,     0,
       0,   257,   257,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   133,     0,     0,     0,
     454,   456,     0,   457,   458,   385,     0,     0,   455,   452,
      83,   453,     0,     0,     0,     4,     0,     5,     9,     0,
      10,    11,    12,    14,   317,    21,    13,    84,    15,    17,
      16,    19,    20,    18,    89,     0,    87,   377,     0,    90,
      88,    91,     0,   386,   373,   374,   320,   318,     0,     0,
     378,   379,     0,   319,     0,   387,   388,   389,   372,   322,
     321,   375,   376,     0,    23,   328,     0,     0,   358,     0,
      75,     0,     0,     0,     0,     0,     0,     0,     0,   377,
     386,   318,   378,   379,   357,   319,   387,   388,     0,     0,
       0,     0,   308,     0,    78,    77,   150,    92,     0,   151,
       0,     0,     0,     0,     0,     0,   308,     0,     0,     0,
       0,     0,   260,    30,   440,   370,     0,   441,     7,   308,
     258,   259,    86,    85,   237,   300,     0,   299,    81,     0,
       0,     0,    80,    33,     0,   323,     0,   308,    34,   329,
       0,   120,   121,   249,     0,     0,   299,     0,     0,   443,
     384,   439,   442,   438,    42,    44,     0,     0,   303,     0,
     305,     0,     0,   304,     0,   299,     0,     0,     6,     0,
     134,   223,   222,   152,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   383,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   308,   308,     0,     0,     0,    24,
      25,     0,    26,     0,     0,     0,     0,     0,     0,     0,
      27,    28,     0,   317,   315,     0,   309,   310,   316,     0,
       0,     0,     0,     0,   101,     0,     0,   100,     0,     0,
       0,   107,     0,     0,   117,     0,    29,   200,   146,   265,
       0,   266,   268,     0,   279,     0,     0,   271,     0,     0,
      31,     0,   151,   236,     0,    57,    82,   124,    79,    32,
     308,     0,   122,     0,   247,   244,    54,     0,    50,    94,
      35,    43,    45,     0,   407,     0,     0,   398,     0,   400,
       0,     0,     0,     0,     0,     0,   411,     8,     0,     0,
       0,   218,     0,     0,     0,     0,     0,   356,   435,   434,
     437,   445,   444,   449,   448,   431,   428,   429,   430,   381,
     418,   397,   396,   395,   382,   422,   433,   427,   425,   436,
     426,   424,   416,   421,   423,   432,   415,   419,   420,   417,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   447,   446,
     451,   450,   212,   209,   210,   211,   215,   216,   217,     0,
       0,   360,     0,     0,     0,     0,     0,     0,     0,     0,
     409,   357,   357,    97,   245,     0,     0,     0,   325,     0,
     143,     0,   140,   246,   134,     0,     0,     0,   332,     0,
       0,     0,   338,     0,     0,     0,   108,   116,     0,     0,
     324,     0,   201,     0,   208,   224,     0,   269,     0,   283,
       0,   278,   373,     0,     0,   262,   371,   261,   394,   302,
     301,     0,     0,   326,     0,   251,   373,     0,   453,    53,
       0,    46,    51,     0,   408,   380,   399,   306,   401,   307,
       0,     0,   410,   113,   342,     0,   413,   412,     0,     0,
     135,   136,   219,   220,   221,   153,     0,     0,   253,   252,
     254,   256,    58,    65,    66,    67,    62,    64,    72,    73,
      60,    63,    61,    59,    69,    68,    70,    71,   392,   393,
     213,   214,   365,     0,   359,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,   314,   312,
     313,   311,     0,   141,   139,     0,     0,   115,     0,     0,
      99,     0,    98,     0,     0,   105,     0,     0,   103,     0,
       0,   363,     0,     0,   119,   207,   199,     0,   292,   225,
     228,   227,   229,     0,   267,   270,     0,   271,     0,   263,
     272,   273,     0,     0,   123,   125,   327,     0,   248,     0,
      55,     0,    56,     0,     0,     0,     0,   114,     0,    37,
       0,   257,   224,   182,   180,   185,   192,   193,   194,   189,
     191,   187,   190,   188,   186,   196,   195,   161,   164,   162,
     163,   174,   165,   178,   170,   168,   181,   169,   167,   172,
     177,   179,   166,   171,   175,   176,   173,   183,   184,   159,
     197,   197,   255,   361,   386,   386,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    95,   144,   142,   136,   102,
       0,     0,   331,     0,   330,     0,     0,   337,   106,     0,
     336,   104,     0,   362,   118,   347,   202,     0,     0,   271,
     226,   242,   264,     0,     0,     0,   127,     0,     0,    48,
      47,     0,   111,   344,     0,     0,   109,   343,     0,   414,
      38,    74,   257,   257,   129,   257,   137,     0,   160,     0,
     200,   155,   156,     0,     0,     0,     0,     0,     0,     0,
       0,   257,   335,     0,     0,   341,     0,     0,   364,   292,
     295,   296,   297,     0,   294,   298,   373,   238,   204,   203,
       0,     0,     0,   281,   373,   128,   126,     0,     0,   112,
       0,   110,   138,   230,   159,   197,   197,     0,     0,     0,
       0,     0,     0,     0,   130,     0,     0,     0,     0,   271,
     284,     0,   239,   241,   240,   243,   234,   235,   147,     0,
      49,     0,     0,   231,   242,   157,   158,   198,     0,     0,
       0,     0,     0,     0,   334,   333,   340,   339,   206,   205,
     286,   287,   289,   373,     0,   409,   373,   346,   345,     0,
       0,   288,   290,   232,   154,   233,   284,   291
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -769,  -769,  -769,     4,  -594,  1930,  -769,  -769,  1520,    33,
     216,  -769,  -769,  -769,  -769,  -769,   198,  1761,    -6,   632,
    -701,  -582,   -40,  -769,  -769,   134,  -769,  -769,  -769,  -769,
    -769,  -769,  -769,   419,    99,  -137,  -769,  -769,   -14,   360,
    -769,  -769,  -769,  -769,  -769,  -769,  -769,  -179,  -176,  -621,
    -769,  -173,   -23,  -769,  -320,  -769,  -769,   -57,  -769,  -769,
    -262,   131,  -769,  -216,  -229,  -769,  -138,  -769,  -769,  -769,
    -170,   268,  -769,  -319,  -623,  -769,  -477,  -344,  -768,  -742,
    -171,    28,    73,  -769,  -148,  -769,   140,   334,  -310,  -769,
    -769,   713,  -769,    -2,  -769,  -769,  -205,  -769,  -441,  -769,
     872,  1014,   -11,  1160,  -769,  1319,  1621,  -769,  -769,  -769,
    -769,  -769,  -769,  -769,  -285
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   215,    76,   487,    78,    79,    80,   216,
     519,   523,   520,    81,    82,    83,   155,    84,   159,   193,
      85,    86,    87,    88,    89,   583,    90,    91,    92,    93,
     511,   635,    94,    95,   380,   651,    96,   471,   472,   139,
     168,   495,    98,    99,   381,   652,   545,   690,   691,   761,
     328,   491,   492,   493,   449,   546,   233,   623,   834,   864,
     828,   185,   823,   788,   791,   100,   204,   354,   101,   102,
     171,   172,   332,   333,   505,   336,   337,   501,   851,   785,
     729,   217,   221,   222,   305,   306,   307,   140,   104,   105,
     106,   141,   108,   127,   128,   450,   322,   610,   451,   109,
     142,   143,   112,   145,   114,   146,   147,   117,   118,   226,
     119,   120,   121,   122,   123
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     125,   182,   183,   453,   732,   144,    75,   494,   150,   515,
     299,   496,     3,   131,   163,   164,   165,   156,   325,   373,
     310,   627,   632,   174,   175,   502,   176,   177,     5,     5,
       5,   341,     5,   187,   189,   190,   194,   195,   334,   334,
     199,   200,   191,   516,   611,    45,   206,   207,   208,   351,
     334,   209,   234,   633,   210,   211,   212,   755,   213,   366,
    -207,   187,   223,   225,   383,   384,   385,   386,    14,   756,
     762,   604,   186,   338,   320,   459,   368,   442,   852,    45,
     599,   334,   148,  -276,   235,   205,   382,   861,   367,   126,
     827,   443,   504,   236,   444,   191,   733,     5,   867,   573,
    -276,  -276,   224,   329,   178,   369,   789,   634,   445,   446,
    -276,    43,   237,   862,  -276,   787,   291,   733,   447,  -276,
    -276,   125,   293,   294,   295,   199,   187,   225,   574,   643,
      56,   320,   693,   448,   231,   854,   436,   437,  -276,   196,
    -350,   308,   298,   863,   626,   626,   731,   339,   229,   371,
     291,   375,   291,   232,   344,   308,   626,  -276,   827,   162,
     152,   169,   330,   169,  -276,   617,   296,  -274,   308,  -276,
     371,  -276,  -276,   802,   615,   342,  -276,   755,   625,   344,
     460,   291,   376,   597,   835,   836,   308,   335,  -276,   756,
     291,   371,   722,  -207,  -366,  -367,   849,   615,   363,   344,
     453,   734,   512,   616,   220,   291,  -353,  -276,   365,   371,
     286,   129,   547,   166,   287,  -391,   149,  -391,   345,  -349,
     362,   723,   738,  -366,  -367,   364,   837,   238,   387,   388,
     389,   390,   392,   394,   395,   396,   397,   398,   399,   400,
     855,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   435,   308,   308,   439,   441,   452,   606,   609,
     803,   778,  -277,   304,   370,   179,   462,   344,   453,     5,
    -293,   465,     5,   154,   542,   494,  -285,   304,   727,   197,
     239,   339,   187,   543,   478,   480,   356,   482,   484,  -293,
     304,  -277,   488,  -293,   401,  -285,   502,  -354,   544,  -285,
     548,  -277,   570,   593,   -74,   644,   646,   506,   304,   339,
    -277,  -355,   339,   510,   320,   242,   103,  -277,  -293,   308,
    -352,   476,   619,   -74,  -285,   103,   521,   521,   549,   571,
     594,   550,   510,  -351,   103,   223,  -277,   223,   371,   531,
     510,   534,    97,   536,   538,   402,     5,   103,   187,   288,
     154,    97,   243,   339,   620,  -277,   244,   391,   393,   638,
      97,  -250,  -277,   621,   339,   103,   290,   103,   103,   372,
     793,   576,   551,    97,   793,   287,   291,   284,   622,   285,
    -250,   249,   300,   692,   251,   309,   103,   540,   254,   302,
     103,    97,   251,    97,    97,   304,   304,   313,   786,   249,
     438,   440,   251,  -391,   794,  -391,   254,  -390,   794,  -390,
     353,   461,    97,   466,   327,   467,    97,   453,   355,   527,
     494,   529,   350,   478,   579,   482,   582,   488,   584,   534,
     479,   377,   180,   483,   379,   588,   590,   464,   308,   585,
     586,   103,    23,   181,   596,   468,   598,   469,   475,    29,
     603,  -148,   473,   485,   452,   509,   490,   -41,   489,   497,
     498,   304,   503,   504,  -280,   508,   513,    97,   517,   225,
    -148,   535,   629,   631,   509,    14,   539,   220,    17,   220,
     541,   103,   509,   225,   530,   568,    22,   592,    23,   751,
     853,   569,   612,  -280,   624,    29,   636,  -148,   -52,   856,
     639,   344,   697,  -280,   640,   103,   242,    97,   628,   642,
     180,   700,  -280,    39,   708,   752,  -148,   753,    43,  -280,
      23,   181,   637,   702,   728,   853,  -282,    29,   730,  -148,
     103,    97,   759,   744,   748,    55,   853,    56,  -280,   760,
     790,   779,   452,   243,   392,   439,   696,   244,  -148,   699,
     833,   771,   650,   595,   522,  -282,    97,  -280,   311,   707,
     805,   706,     5,   806,  -280,  -282,   154,   807,   712,   578,
     714,   581,   726,   717,  -282,   757,   720,   865,   589,  -259,
     304,  -282,   725,   848,   442,   859,    70,   507,   819,   591,
     249,     0,   250,   251,   754,     0,     0,   254,   443,     0,
    -282,   444,   187,     0,   261,   103,     0,     0,   739,     0,
     740,  -259,   743,   267,   747,   445,   446,   749,     0,  -282,
    -259,     0,     0,     0,     0,   447,  -282,     0,     0,   103,
       0,    97,   103,     0,   362,   158,     0,   103,     0,  -259,
     448,   737,     0,     0,     0,     0,     0,     5,     0,     0,
       0,   154,     0,   157,     0,    97,     0,     0,    97,     0,
       0,     0,     0,    97,  -259,     0,   712,   714,     0,   717,
     720,   743,   747,     0,     0,     0,     0,     0,     0,   772,
     773,     0,   774,     0,   775,   776,   103,     0,   777,   391,
     438,   452,   182,   183,     0,   107,  -259,     0,     0,     0,
       0,     0,   792,   534,   107,  -259,     0,   534,     0,     0,
     798,     0,    97,   107,   800,     0,     0,     0,     0,     0,
     158,     0,     0,     0,  -259,     0,   107,     0,     0,     0,
       0,     0,   772,   808,   809,   775,   810,   811,   812,   813,
       0,     0,     0,   180,   107,     0,   107,   107,     0,     0,
     195,   199,   225,    23,   181,     0,   824,     0,     0,   825,
      29,     0,  -148,   572,     0,   107,   830,     0,   -40,   107,
       0,     0,     0,     0,   314,   317,   321,     0,     0,     0,
       0,  -148,     0,     0,   844,   845,   846,   847,   340,     0,
     242,   821,     0,     0,     0,     0,     0,     0,   747,     0,
     857,   858,     0,   103,     0,     0,   349,   844,   845,   846,
     847,   857,   858,     0,     0,     0,     0,     0,     0,   359,
     107,     0,     0,   860,   534,     0,     0,   243,     0,    97,
       0,   244,     0,     0,     0,   747,     0,     0,     0,   784,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   103,
       0,     0,     0,   103,   110,     0,     0,     0,     0,     0,
     107,     0,     0,   110,     0,     0,     0,     0,     0,     0,
     246,     0,   110,   248,   249,    97,   250,   251,     0,    97,
       0,   254,     0,     0,   107,   110,     0,     0,   261,   694,
     695,     0,     0,     0,     0,   265,   266,   267,   822,     0,
       0,     0,     0,   110,     0,   110,   110,     0,     0,   107,
     463,     0,     0,   103,     0,   103,     0,     0,   103,     0,
       0,   103,     0,     0,   110,     0,   103,     0,   110,     0,
       0,   850,     0,     0,   486,     0,     0,     0,     0,    97,
       0,    97,     0,     0,    97,     0,   103,    97,     0,     0,
       0,     0,    97,     0,     0,     0,     0,   103,     0,   103,
      14,     0,     0,    17,   103,     0,   850,     0,     0,     0,
       0,    22,    97,    23,   751,     0,     0,   850,     0,   110,
      29,     0,  -148,    97,   107,    97,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,   111,     0,    39,   242,
     752,  -148,   753,    43,     0,   111,     0,     0,   107,     0,
       0,   107,     0,     0,   111,     0,   107,     0,     0,   110,
      55,     0,    56,     0,   103,     0,     0,   111,     0,   103,
       0,     0,     0,     0,     0,     0,   243,     0,     0,     0,
     244,     0,     0,   110,     0,   111,     0,   111,   111,   103,
      97,     0,     0,     0,     0,    97,     0,     0,   103,     0,
       0,     0,   103,     0,     0,   107,   111,     0,   110,     0,
     111,    70,     0,     0,     0,    97,     0,     0,     0,   814,
       0,     0,     0,   249,    97,   250,   251,     0,    97,     0,
     254,   600,   602,     0,     0,   605,   608,   261,     0,     0,
       0,     0,     0,     0,   265,   266,   267,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,     0,     0,     0,     0,     0,   103,
     103,   103,   103,     0,     0,     0,     0,     0,     0,   242,
       0,     0,   113,   110,     0,   103,   103,     0,     0,     0,
       0,   113,     0,     0,     0,    97,    97,    97,    97,     0,
     113,   111,     0,     0,     0,     0,     0,   110,   242,     0,
     110,    97,    97,   113,     0,   110,   243,     0,     0,     0,
     244,     0,   107,     0,     0,   111,     0,     0,     0,     0,
       0,   113,     0,   113,   113,     0,     0,   704,   705,     0,
      14,     0,     0,    17,     0,   243,     0,     0,   709,   244,
     111,    22,   113,    23,   751,     0,   113,     0,   718,   246,
      29,   721,  -148,   249,   110,   250,   251,     0,   107,     0,
     254,     0,   107,     0,     0,     0,     0,   261,   227,     0,
     752,  -148,   753,    43,   265,   266,   267,     0,   246,     0,
     247,   248,   249,     0,   250,   251,   252,     0,     0,   254,
      55,     0,    56,     0,     0,   260,   261,   113,     0,     0,
     264,     0,     0,   265,   266,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,     0,     0,     0,     0,
       0,     0,   107,     0,   107,     0,     0,   107,     0,     0,
     107,   115,     0,     0,     0,   107,     0,   113,     0,   111,
     115,    70,   111,     0,   242,     0,     0,   111,     0,   115,
       0,     0,     0,     0,     0,   107,     0,     0,     0,     0,
       0,   113,   115,     0,     0,     0,   107,     0,   107,     0,
       0,   110,     0,   107,     0,     0,     0,     0,     0,   796,
     115,   243,   115,   115,     0,   244,   113,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   111,     0,     0,     0,
       0,   115,     0,     0,     0,   115,     0,     0,     0,   245,
       0,     0,     0,     0,     0,     0,     0,   110,     0,     0,
       0,   110,     0,     0,   246,     0,   247,   248,   249,     0,
     250,   251,   252,   107,   253,   254,   255,   256,   107,   258,
     259,   260,   261,     0,   262,   263,   264,     0,     0,   265,
     266,   267,     0,     0,     0,     0,   115,     0,   107,     0,
       0,   113,     0,     0,     0,   240,     0,   107,     0,     0,
     241,   107,     0,     0,     0,     0,     0,     0,   242,     0,
       0,   110,     0,   110,     0,   113,   110,     0,   113,   110,
       0,     0,     0,   113,   110,     0,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   111,   110,   243,     0,     0,     0,   244,
     115,     0,     0,     0,     0,   110,     0,   110,     0,     0,
       0,     0,   110,     0,     0,     0,     0,     0,   107,   107,
     107,   107,   113,   245,     0,   115,     0,     0,     0,     0,
       0,     0,     0,     0,   107,   107,     0,     0,   246,   111,
     247,   248,   249,   111,   250,   251,   252,     0,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   192,   262,   263,
     264,     0,     0,   265,   266,   267,     0,     0,     0,     0,
       0,     0,   110,     0,   703,     0,     0,   110,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,     0,     0,
     115,     0,     0,   111,     0,   111,   110,     0,   111,     0,
     110,   111,     0,   116,     0,     0,   111,     0,     0,     0,
       0,     0,   116,     0,   115,     0,     0,   115,     0,   242,
       0,   116,   115,     0,     0,     0,   111,     0,     0,   113,
       0,   292,     0,     0,   116,     0,     0,   111,     0,   111,
       0,     0,     0,     0,   111,     0,     0,     0,     0,     0,
       0,     0,   116,     0,   116,   116,   243,     0,     0,     0,
     244,     0,   192,   192,   192,   324,     0,   110,   110,   110,
     110,   115,     0,   116,     0,   113,   192,   116,     0,   113,
       0,     0,     0,   110,   110,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   192,   242,     0,     0,     0,   246,
       0,   247,   248,   249,   111,   250,   251,   192,     0,   111,
     254,     0,     0,     0,     0,     0,   260,   261,     0,     0,
       0,   264,     0,     0,   265,   266,   267,     0,   116,   111,
       0,     0,   243,     0,     0,     0,   244,     0,   111,   113,
       0,   113,   111,     0,   113,     0,     0,   113,     0,     0,
       0,     0,   113,     0,   130,     0,     0,     0,     0,   130,
       0,     0,     0,   153,   130,   130,     0,     0,   116,     0,
       0,   167,   113,   170,     0,   246,     0,   247,   248,   249,
       0,   250,   251,   113,     0,   113,   254,     0,   115,     0,
     113,     0,   116,   261,     0,     0,   203,     0,   192,     0,
     265,   266,   267,     0,     0,     0,     0,     0,     0,   111,
     111,   111,   111,     0,     0,     0,     0,   116,     0,     0,
       0,     0,   192,     0,     0,   111,   111,     0,     0,     0,
       0,     0,     0,     0,   115,     0,   230,     0,   115,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,     0,     0,     0,     0,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   113,     0,     0,     0,     0,
       0,     0,     0,     0,   113,     0,     0,     0,   113,   203,
       0,     0,   116,   303,     0,     0,     0,     0,   115,   203,
     115,   312,     0,   115,     0,     0,   115,   303,     0,     0,
     331,   115,    77,     0,     0,     0,   116,     0,     0,   116,
     303,   124,   130,     0,   116,   343,     0,     0,     0,     0,
     151,   115,     0,     0,     0,     0,     0,     0,   303,     0,
       0,     0,   115,   173,   115,     0,     0,     0,     0,   115,
       0,     0,     0,     0,     0,   113,   113,   113,   113,     0,
       0,   198,     0,   201,   202,     0,     0,     0,     0,     0,
     378,   113,   113,   116,     0,   170,   170,   170,   170,   192,
     192,     0,    77,   192,   192,     0,   228,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   115,
       0,     0,     0,     0,   115,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,   303,     0,     0,     0,
       0,     0,     0,     0,   115,     0,     0,   289,     0,     0,
       0,     0,     0,   115,     0,     0,     0,   115,     0,     0,
     470,     0,   474,     0,     0,     5,     0,     0,     0,     0,
       0,     0,   653,     0,     0,     0,     0,   654,     0,     0,
       0,   331,     0,     0,     0,     0,     0,   326,     0,     0,
     170,     0,     0,     0,     0,   192,   192,     0,     0,     0,
     116,   303,     0,     0,     0,     0,   192,     0,   324,     0,
       0,   348,     0,   324,     0,     0,   192,     0,     0,   192,
       0,     0,     0,     0,   115,   115,   115,   115,     0,     0,
       0,     0,     0,   170,     0,     0,    77,     0,     0,     0,
     115,   115,     0,     0,     0,     0,   116,     0,     0,     0,
     116,   655,   656,   657,   658,   659,   660,     0,     0,   661,
     662,   663,   664,   665,   666,   667,   668,   669,   670,     0,
       0,   671,     0,     0,     0,   672,   673,   674,   675,   676,
     677,   678,   679,   680,   681,   682,     0,   683,     0,     0,
     684,   685,   686,   687,     0,   688,     0,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   132,     0,
     116,    77,   116,     0,     0,   116,     0,     0,   116,     0,
     303,    21,     0,   116,     0,     0,     0,   133,   134,   135,
       0,    28,     0,     0,     0,   348,    31,    32,   348,    34,
       0,    35,    36,   116,   618,     0,     0,   192,   331,    38,
       0,    40,     0,     0,   116,     0,   116,     0,     0,     0,
       0,   116,     0,    48,    49,    50,   136,     0,   203,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,   324,     0,   324,   324,     0,     0,
       0,     0,   533,     0,     0,     0,     0,   689,     0,     0,
      62,   826,     0,     0,     0,     0,     0,     0,   324,     0,
     324,     0,     0,     0,     0,     0,     0,     0,    67,     0,
       0,   116,     0,     0,     0,     0,   116,     0,     0,     0,
       0,     0,    73,     0,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   470,     0,   116,     0,     0,     0,
       0,     0,     0,     0,     0,   116,     0,     0,     0,   116,
       0,     0,     0,     0,     0,     0,     0,     0,   331,   826,
       0,   499,     0,     5,     6,     7,     8,     9,    10,     0,
    -409,     0,   132,     0,     0,  -409,     0,     0,     0,     0,
       0,     0,     0,  -409,     0,    21,     0,     0,     0,     0,
       0,   133,   134,   135,     0,    28,     0,     0,     0,   614,
      31,    32,     0,    34,     0,    35,    36,     0,     0,   758,
       0,     0,     0,    38,     0,    40,   116,   116,   116,   116,
    -409,     0,     0,     0,  -409,     0,     0,    48,    49,    50,
     136,     0,   116,   116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   647,     0,  -275,  -409,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -409,    62,  -409,  -409,  -409,     0,  -409,
    -409,  -409,     0,  -409,  -409,  -409,  -409,  -409,  -409,  -409,
    -409,  -409,    67,  -409,  -409,  -409,     0,     0,  -409,  -409,
    -409,     0,   130,     0,   137,     0,    73,     0,   514,     0,
     804,     0,     0,     0,     0,     0,     0,     0,     0,   348,
       0,   348,     0,     0,   348,     0,     0,   348,     0,     0,
       0,     0,   724,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   736,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   742,     0,   746,     0,     0,    -2,     4,
      77,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -257,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -148,    30,    31,    32,
      33,    34,   -39,    35,    36,     0,    37,     0,     0,     0,
    -257,    38,    39,    40,    41,  -148,    42,    43,     0,  -257,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,    55,     0,    56,    57,  -257,     0,
       0,    58,     0,    59,     0,   795,    60,     0,     0,     0,
       0,     0,     0,     0,   799,     0,     0,     0,   801,     0,
       0,    61,    62,    63,    64,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      67,     0,    68,    69,     0,    70,     0,     0,    71,     0,
       0,     0,    72,     0,    73,     4,    74,     5,     6,     7,
       8,     9,    10,     0,  -409,     0,    11,    12,    13,  -409,
      14,    15,    16,    17,  -257,    18,    19,  -409,    20,    21,
    -409,    22,     0,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -148,    30,    31,    32,    33,    34,   -39,    35,
      36,     0,    37,     0,     0,     0,  -257,    38,     0,    40,
      41,  -148,    42,    43,  -409,  -257,    44,    45,  -409,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
      55,     0,    56,    57,  -257,     0,     0,    58,     0,    59,
       0,     0,  -409,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -409,    62,  -409,
    -409,  -409,  -409,  -409,  -409,  -409,     0,  -409,  -409,  -409,
    -409,  -409,  -409,  -409,  -409,  -409,    67,  -409,  -409,  -409,
       0,    70,  -409,  -409,  -409,     0,     0,     0,    72,  -409,
      73,     4,    74,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -257,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -148,    30,
      31,    32,    33,    34,   -39,    35,    36,     0,    37,     0,
       0,     0,  -257,    38,    39,    40,    41,  -148,    42,    43,
       0,  -257,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,    55,     0,    56,    57,
    -257,     0,     0,    58,     0,    59,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,    64,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    67,     0,    68,    69,   184,    70,     0,     0,
      71,     0,     0,     0,    72,   214,    73,     4,    74,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -257,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -148,    30,    31,    32,    33,    34,
     -39,    35,    36,     0,    37,     0,     0,     0,  -257,    38,
      39,    40,    41,  -148,    42,    43,     0,  -257,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,    55,     0,    56,    57,  -257,     0,     0,    58,
       0,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      62,    63,    64,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    67,     0,
      68,    69,     0,    70,     0,     0,    71,     0,     0,     0,
      72,   361,    73,     4,    74,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -257,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -148,    30,    31,    32,    33,    34,   -39,    35,    36,     0,
      37,     0,     0,     0,  -257,    38,    39,    40,    41,  -148,
      42,    43,     0,  -257,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,    55,     0,
      56,    57,  -257,     0,     0,    58,     0,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,    63,    64,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    67,     0,    68,    69,     0,    70,
       0,     0,    71,     0,     0,     0,    72,   214,    73,     4,
      74,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -257,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -148,    30,    31,    32,
      33,    34,   -39,    35,    36,     0,    37,     0,     0,     0,
    -257,    38,    39,    40,    41,  -148,    42,    43,     0,  -257,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,    55,     0,    56,    57,  -257,     0,
       0,    58,     0,    59,     0,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,    64,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      67,     0,    68,    69,     0,    70,     0,     0,    71,     0,
       0,     0,    72,   649,    73,     4,    74,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -257,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -148,    30,    31,    32,    33,    34,   -39,    35,
      36,     0,    37,     0,     0,     0,  -257,    38,    39,    40,
      41,  -148,    42,    43,     0,  -257,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
      55,     0,    56,    57,  -257,     0,     0,    58,     0,    59,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,    63,
      64,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    67,     0,    68,    69,
       0,    70,     0,     0,    71,     0,     0,     0,    72,   750,
      73,     4,    74,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -257,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -148,    30,
      31,    32,    33,    34,   -39,    35,    36,     0,    37,     0,
       0,     0,  -257,    38,   227,    40,    41,  -148,    42,    43,
       0,  -257,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,    55,     0,    56,    57,
    -257,     0,     0,    58,     0,    59,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,    64,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    67,     0,    68,    69,     0,    70,     0,     0,
      71,     0,     0,     0,    72,     0,    73,     4,    74,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -257,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -148,    30,    31,    32,    33,    34,
     -39,    35,    36,     0,    37,     0,     0,     0,  -257,    38,
       0,    40,    41,  -148,    42,    43,     0,  -257,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,    55,     0,    56,    57,  -257,     0,     0,    58,
       0,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      62,    63,    64,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    67,     0,
      68,    69,     0,    70,     0,     0,    71,     0,     0,     0,
      72,     0,    73,     4,    74,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -257,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,   477,     0,    28,    29,     0,
    -148,    30,    31,    32,    33,    34,   -39,    35,    36,     0,
      37,     0,     0,     0,  -257,    38,     0,    40,    41,  -148,
      42,    43,     0,  -257,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,    55,     0,
      56,    57,  -257,     0,     0,    58,     0,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,    63,    64,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    67,     0,    68,    69,     0,    70,
       0,     0,    71,     0,     0,     0,    72,     0,    73,     4,
      74,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -257,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   481,     0,    28,    29,     0,  -148,    30,    31,    32,
      33,    34,   -39,    35,    36,     0,    37,     0,     0,     0,
    -257,    38,     0,    40,    41,  -148,    42,    43,     0,  -257,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,    55,     0,    56,    57,  -257,     0,
       0,    58,     0,    59,     0,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,    64,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      67,     0,    68,    69,     0,    70,     0,     0,    71,     0,
       0,     0,    72,     0,    73,     4,    74,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -257,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,   711,     0,    28,
      29,     0,  -148,    30,    31,    32,    33,    34,   -39,    35,
      36,     0,    37,     0,     0,     0,  -257,    38,     0,    40,
      41,  -148,    42,    43,     0,  -257,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
      55,     0,    56,    57,  -257,     0,     0,    58,     0,    59,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,    63,
      64,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    67,     0,    68,    69,
       0,    70,     0,     0,    71,     0,     0,     0,    72,     0,
      73,     4,    74,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -257,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,   713,     0,    28,    29,     0,  -148,    30,
      31,    32,    33,    34,   -39,    35,    36,     0,    37,     0,
       0,     0,  -257,    38,     0,    40,    41,  -148,    42,    43,
       0,  -257,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,    55,     0,    56,    57,
    -257,     0,     0,    58,     0,    59,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,    64,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    67,     0,    68,    69,     0,    70,     0,     0,
      71,     0,     0,     0,    72,     0,    73,     4,    74,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -257,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,   716,
       0,    28,    29,     0,  -148,    30,    31,    32,    33,    34,
     -39,    35,    36,     0,    37,     0,     0,     0,  -257,    38,
       0,    40,    41,  -148,    42,    43,     0,  -257,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,    55,     0,    56,    57,  -257,     0,     0,    58,
       0,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      62,    63,    64,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    67,     0,
      68,    69,     0,    70,     0,     0,    71,     0,     0,     0,
      72,     0,    73,     4,    74,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -257,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,   719,     0,    28,    29,     0,
    -148,    30,    31,    32,    33,    34,   -39,    35,    36,     0,
      37,     0,     0,     0,  -257,    38,     0,    40,    41,  -148,
      42,    43,     0,  -257,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,    55,     0,
      56,    57,  -257,     0,     0,    58,     0,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,    63,    64,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    67,     0,    68,    69,     0,    70,
       0,     0,    71,     0,     0,     0,    72,     0,    73,     4,
      74,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -257,    18,
      19,     0,   735,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -148,    30,    31,    32,
      33,    34,   -39,    35,    36,     0,    37,     0,     0,     0,
    -257,    38,     0,    40,    41,  -148,    42,    43,     0,  -257,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,    55,     0,    56,    57,  -257,     0,
       0,    58,     0,    59,     0,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,    64,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      67,     0,    68,    69,     0,    70,     0,     0,    71,     0,
       0,     0,    72,     0,    73,     4,    74,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -257,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,   741,     0,    28,
      29,     0,  -148,    30,    31,    32,    33,    34,   -39,    35,
      36,     0,    37,     0,     0,     0,  -257,    38,     0,    40,
      41,  -148,    42,    43,     0,  -257,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
      55,     0,    56,    57,  -257,     0,     0,    58,     0,    59,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,    63,
      64,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    67,     0,    68,    69,
       0,    70,     0,     0,    71,     0,     0,     0,    72,     0,
      73,     4,    74,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -257,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,   745,     0,    28,    29,     0,  -148,    30,
      31,    32,    33,    34,   -39,    35,    36,     0,    37,     0,
       0,     0,  -257,    38,     0,    40,    41,  -148,    42,    43,
       0,  -257,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,    55,     0,    56,    57,
    -257,     0,     0,    58,     0,    59,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,    64,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    67,     0,    68,    69,     0,    70,     0,     0,
      71,     0,     0,     0,    72,     0,    73,   499,    74,     5,
       6,     7,     8,     9,    10,     0,  -409,     0,   132,     0,
       0,  -409,     0,     0,     0,     0,     0,     0,     0,  -409,
       0,    21,     0,     0,     0,     0,     0,   133,   134,   135,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,  -409,     0,     0,     0,
    -409,     0,     0,    48,    49,    50,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -409,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -409,
      62,  -409,  -409,  -409,     0,  -409,  -409,  -409,     0,  -409,
    -409,  -409,  -409,  -409,  -409,  -409,  -409,  -409,    67,  -409,
    -409,  -409,     0,     0,  -409,  -409,  -409,     0,     0,     0,
     137,     0,    73,   499,   514,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   132,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   133,   134,   135,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   136,     0,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,    63,    64,     0,
       0,     0,     0,    65,     0,     0,   282,   283,  -368,     0,
       0,  -390,    66,  -390,    67,     0,    68,    69,     0,     0,
       0,     0,    71,     0,     0,     0,   137,     0,    73,   499,
     500,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   133,
     134,   135,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   136,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,    64,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      67,     0,    68,    69,     0,     0,     0,     0,    71,     0,
       0,     0,   137,     0,    73,     0,   514,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   132,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   133,   134,   135,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   136,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,    63,
      64,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    67,     0,    68,    69,
     184,     0,     0,     0,    71,     0,     0,     0,   137,     0,
      73,     0,   138,   532,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   133,   134,   135,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    67,     0,    68,    69,   184,     0,     0,
       0,    71,     0,     0,     0,   137,     0,    73,     0,   138,
     820,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   133,
     134,   135,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   136,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,    64,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      67,     0,    68,    69,     0,     0,     0,     0,    71,     0,
       0,     0,   137,     0,    73,     0,   138,   537,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   132,     0,     0,
       0,     0,     0,     0,     0,   442,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   133,   134,   135,   443,
      28,     0,   444,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,   445,   446,    38,     0,
      40,     0,     0,     0,     0,     0,   447,     0,     0,     0,
       0,     0,    48,    49,    50,   136,     0,     0,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,    62,
      63,    64,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    67,     0,    68,
      69,     0,     0,     0,     0,    71,     0,     0,     0,   137,
       0,    73,     0,   138,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   133,   134,   135,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   136,     0,     0,     0,     0,     0,     0,   218,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,    65,   219,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    67,     0,    68,    69,   184,     0,     0,
       0,    71,     0,     0,     0,   137,     0,    73,     0,   138,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   133,   134,
     135,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   136,     0,     0,
       0,     0,     0,     0,   218,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,    64,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    67,
       0,    68,    69,   184,     0,     0,     0,    71,     0,     0,
       0,   137,     0,    73,   526,   138,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   133,   134,   135,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   136,     0,     0,     0,     0,     0,     0,
     218,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,    63,    64,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    67,     0,    68,    69,   184,
       0,     0,     0,    71,     0,     0,     0,   137,     0,    73,
     528,   138,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     133,   134,   135,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,   160,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   161,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,    64,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    67,     0,    68,    69,     0,     0,     0,     0,    71,
       0,     0,     0,   137,     0,    73,     0,   138,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   133,   134,   135,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,    62,
      63,    64,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    67,     0,    68,
      69,   184,     0,     0,     0,    71,     0,     0,     0,   137,
     524,    73,     0,   138,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   133,   134,   135,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    67,     0,    68,    69,   184,     0,     0,
       0,    71,     0,     0,     0,   137,     0,    73,     0,   138,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   133,   134,
     135,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,    64,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    67,
       0,    68,    69,     0,   188,     0,     0,    71,     0,     0,
       0,   137,     0,    73,     0,   138,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   191,    21,     0,
       0,     0,     0,     0,   133,   134,   135,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,    63,    64,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    67,     0,    68,    69,     0,
       0,     0,     0,    71,     0,     0,     0,    72,     0,    73,
       0,   138,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     133,   134,   135,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   161,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,    64,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    67,     0,    68,    69,     0,     0,     0,     0,    71,
       0,     0,     0,   137,     0,    73,     0,   138,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   133,   134,   135,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,   630,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,    62,
      63,    64,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    67,     0,    68,
      69,     0,     0,     0,     0,    71,     0,     0,     0,   137,
       0,    73,     0,   138,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   780,     0,     0,     0,
       0,     0,   133,   134,   135,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   781,    49,
      50,   782,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    67,     0,    68,    69,   184,     0,     0,
       0,    71,     0,     0,     0,   137,     0,    73,     0,   783,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   133,   134,
     135,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,    64,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    67,
       0,    68,    69,   184,     0,     0,     0,    71,     0,     0,
       0,   137,     0,    73,     0,   783,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   133,   134,   770,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,    63,    64,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    67,     0,    68,    69,   184,
       0,     0,     0,    71,     0,     0,     0,   137,     0,    73,
       0,   138,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     133,   134,   135,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,    64,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    67,     0,    68,    69,     0,     0,     0,     0,    71,
       0,     0,     0,   137,     0,    73,     0,   138,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   133,   134,   135,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,    62,
      63,    64,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    67,     0,    68,
      69,     0,     0,     0,     0,   518,     0,     0,     0,   137,
       0,    73,     0,   138,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   133,   134,   577,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    67,     0,    68,    69,     0,     0,     0,
       0,    71,     0,     0,     0,   137,     0,    73,     0,   138,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   133,   134,
     580,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,    64,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    67,
       0,    68,    69,     0,     0,     0,     0,    71,     0,     0,
       0,   137,     0,    73,     0,   138,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   133,   134,   764,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,    63,    64,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    67,     0,    68,    69,     0,
       0,     0,     0,    71,     0,     0,     0,   137,     0,    73,
       0,   138,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     133,   134,   765,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,    64,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    67,     0,    68,    69,     0,     0,     0,     0,    71,
       0,     0,     0,   137,     0,    73,     0,   138,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   133,   134,   767,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,    62,
      63,    64,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    67,     0,    68,
      69,     0,     0,     0,     0,    71,     0,     0,     0,   137,
       0,    73,     0,   138,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   133,   134,   768,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    67,     0,    68,    69,     0,     0,     0,
       0,    71,     0,     0,     0,   137,     0,    73,     0,   138,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   133,   134,
     769,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,    64,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    67,
       0,    68,    69,     0,     0,     0,     0,    71,     0,     0,
       0,   137,     0,    73,     0,   138,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   133,   134,   770,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,    63,    64,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    67,     0,    68,    69,     0,
       0,     0,     0,    71,     0,     0,     0,   137,     0,    73,
       0,   138,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     133,   134,   135,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   132,     0,     0,
       0,     0,    61,    62,    63,    64,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   133,   134,   135,    66,
      28,    67,     0,     0,    69,    31,    32,     0,    34,    71,
      35,    36,     0,   137,   240,    73,     0,   138,    38,   241,
      40,     0,     0,     0,     0,     0,     0,   242,     0,     0,
       0,     0,    48,    49,    50,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -284,     0,     0,     0,   240,     0,
       0,     0,     0,   241,   243,     0,     0,     0,   244,    62,
       0,   242,     0,  -284,     0,     0,     0,  -284,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    67,   320,     0,
       0,   184,   245,     0,     0,     0,     0,     0,     0,   137,
       0,    73,  -284,   783,     0,     0,     0,   246,   243,   247,
     248,   249,   244,   250,   251,   252,     0,   253,   254,   255,
     256,   257,   258,   259,   260,   261,     0,   262,   263,   264,
       0,     0,   265,   266,   267,   240,   245,     0,     0,     0,
     241,     0,     0,   645,     0,     0,     0,     0,   242,     0,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
       0,   262,   263,   264,     0,     0,   265,   266,   267,   240,
       0,     0,     0,     0,   241,   243,     0,   829,     0,   244,
       0,     0,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   246,   243,
     247,   248,   249,   244,   250,   251,   252,     0,   253,   254,
     255,   256,   257,   258,   259,   260,   261,     0,   262,   263,
     264,     0,     0,   265,   266,   267,     0,   245,     0,     0,
       0,     0,     0,     0,   866,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   240,   262,   263,   264,     0,   241,   265,   266,   267,
       0,     0,     0,     0,   242,   318,   525,     0,     0,     0,
       0,     0,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   240,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,   242,   315,
       0,   243,     0,     0,     0,   244,     0,     0,     0,   316,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   240,     0,   320,     0,     0,   241,   245,
       0,     0,     0,     0,     0,   243,   242,   607,     0,   244,
       0,     0,     0,     0,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   245,   262,   263,   264,     0,     0,   265,
     266,   267,     0,   243,     0,   291,     0,   244,   246,     0,
     247,   248,   249,     0,   250,   251,   252,     0,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   320,   262,   263,
     264,   245,   240,   265,   266,   267,     0,   241,     0,   291,
       0,     0,     0,     0,     0,   242,   246,     0,   247,   248,
     249,     0,   250,   251,   252,     0,   253,   254,   255,   256,
     257,   258,   259,   260,   261,     0,   262,   263,   264,     0,
     240,   265,   266,   267,     0,   241,     0,   291,     0,     0,
       0,     0,   243,   242,   191,     0,   244,     0,     0,     0,
       0,     0,     0,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   240,     0,     0,     0,     0,   241,
     245,     0,     0,     0,     0,     0,     0,   242,   601,     0,
     243,     0,     0,     0,   244,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,     0,   262,   263,   264,   245,     0,
     265,   266,   267,     0,   243,     0,   291,     0,   244,     0,
       0,     0,     0,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   245,   262,   263,   264,     0,     0,   265,   266,
     267,     0,     0,   240,   291,     0,     0,   246,   241,   247,
     248,   249,     0,   250,   251,   252,   242,   253,   254,   255,
     256,   257,   258,   259,   260,   261,     0,   262,   263,   264,
       0,     0,   265,   266,   267,     0,     0,     0,   291,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,   710,   240,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,   242,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   246,     0,   247,   248,
     249,     0,   250,   251,   252,     0,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   243,   262,   263,   264,   244,
       0,   265,   266,   267,     0,     0,   715,   291,     0,     0,
       0,     0,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,   245,     0,   242,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   246,     0,
     247,   248,   249,     0,   250,   251,   252,     0,   253,   254,
     255,   256,   257,   258,   259,   260,   261,     0,   262,   263,
     264,     0,   243,   265,   266,   267,   244,     0,     0,   291,
       0,     0,     0,   815,   240,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
     245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   243,   262,   263,   264,   244,     0,
     265,   266,   267,     0,     0,   816,   291,     0,     0,     0,
       0,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,   245,     0,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,     0,   247,
     248,   249,     0,   250,   251,   252,     0,   253,   254,   255,
     256,   257,   258,   259,   260,   261,     0,   262,   263,   264,
       0,   243,   265,   266,   267,   244,     0,     0,   291,     0,
       0,     0,   817,   240,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   243,   262,   263,   264,   244,     0,   265,
     266,   267,     0,     0,   818,   291,     0,     0,     0,     0,
     240,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,   245,     0,   242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   246,     0,   247,   248,
     249,     0,   250,   251,   252,     0,   253,   254,   255,   256,
     257,   258,   259,   260,   261,     0,   262,   263,   264,     0,
     243,   265,   266,   267,   244,     0,     0,   291,     0,     0,
       0,   831,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   243,   262,   263,   264,   244,     0,   265,   266,
     267,     0,     0,   832,   291,     0,     0,     0,     0,   240,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
     245,     0,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,     0,   262,   263,   264,     0,   243,
     265,   266,   267,   244,     0,     0,   291,     0,     0,     0,
       0,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   243,   262,   263,   264,   244,     0,   265,   266,   267,
       0,     0,     0,   347,     0,     0,     0,     0,   240,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,   245,
       0,   242,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   297,     0,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,     0,   262,   263,   264,     0,   243,   265,
     266,   267,   244,     0,  -369,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   126,     0,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     240,   262,   263,   264,     0,   241,   265,   266,   267,     0,
       0,     0,     0,   242,     0,     0,     0,     0,   357,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   240,   358,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
     243,   242,   454,     0,   244,     0,     0,     0,     0,     0,
       0,     0,   455,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   243,     0,
       0,     0,   244,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   240,   262,   263,   264,   245,   241,   265,   266,
     267,     0,     0,     0,     0,   242,   456,     0,     0,     0,
       0,   246,     0,   247,   248,   249,   457,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     240,   262,   263,   264,     0,   241,   265,   266,   267,     0,
       0,     0,   243,   242,     0,     0,   244,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   240,     0,     0,     0,     0,   241,
     245,     0,     0,     0,     0,     0,     0,   242,     0,     0,
     243,     0,     0,     0,   244,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,     0,   262,   263,   264,   245,     0,
     265,   266,   267,     0,   243,     0,     0,     0,   244,     0,
       0,     0,     0,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   245,   262,   263,   264,     0,   301,   265,   266,
     267,     0,     0,   240,     0,     0,     0,   246,   241,   247,
     248,   249,     0,   250,   251,   252,   242,   253,   254,   255,
     256,   257,   258,   259,   260,   261,     0,   262,   263,   264,
       0,   346,   265,   266,   267,     0,     0,   240,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,   243,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   245,     0,     0,     0,     0,     0,   243,     0,     0,
       0,   244,     0,     0,     0,     0,   246,     0,   247,   248,
     249,     0,   250,   251,   252,     0,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   245,   262,   263,   264,     0,
     352,   265,   266,   267,     0,     0,   240,     0,     0,     0,
     246,   241,   247,   248,   249,     0,   250,   251,   252,   242,
     253,   254,   255,   256,   257,   258,   259,   260,   261,     0,
     262,   263,   264,     0,   360,   265,   266,   267,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,   240,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,   242,     0,   374,   245,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   243,   262,
     263,   264,   244,     0,   265,   266,   267,     0,     0,   458,
     240,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     243,   262,   263,   264,   244,     0,   265,   266,   267,     0,
       0,     0,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   243,   262,   263,   264,   244,   552,   265,   266,
     267,     0,     0,     0,   240,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
     245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   243,   262,   263,   264,   244,   553,
     265,   266,   267,     0,     0,     0,   240,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,     0,   247,
     248,   249,     0,   250,   251,   252,     0,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   243,   262,   263,   264,
     244,   554,   265,   266,   267,     0,     0,     0,   240,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   243,   262,
     263,   264,   244,   555,   265,   266,   267,     0,     0,     0,
     240,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     243,   262,   263,   264,   244,   556,   265,   266,   267,     0,
       0,     0,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   243,   262,   263,   264,   244,   557,   265,   266,
     267,     0,     0,     0,   240,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
     245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   243,   262,   263,   264,   244,   558,
     265,   266,   267,     0,     0,     0,   240,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,     0,   247,
     248,   249,     0,   250,   251,   252,     0,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   243,   262,   263,   264,
     244,   559,   265,   266,   267,     0,     0,     0,   240,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   243,   262,
     263,   264,   244,   560,   265,   266,   267,     0,     0,     0,
     240,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     243,   262,   263,   264,   244,   561,   265,   266,   267,     0,
       0,     0,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   243,   262,   263,   264,   244,   562,   265,   266,
     267,     0,     0,     0,   240,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
     245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   243,   262,   263,   264,   244,   563,
     265,   266,   267,     0,     0,     0,   240,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,     0,   247,
     248,   249,     0,   250,   251,   252,     0,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   243,   262,   263,   264,
     244,   564,   265,   266,   267,     0,     0,     0,   240,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   243,   262,
     263,   264,   244,   565,   265,   266,   267,     0,     0,     0,
     240,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     243,   262,   263,   264,   244,   566,   265,   266,   267,     0,
       0,     0,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   243,   262,   263,   264,   244,   567,   265,   266,
     267,     0,     0,     0,   240,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,   126,   242,     0,     0,
     245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   243,   262,   263,   264,   244,     0,
     265,   266,   267,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   240,     0,     0,     0,     0,
     241,     0,   245,     0,     0,     0,     0,     0,   242,     0,
       0,   613,     0,     0,     0,     0,     0,   246,     0,   247,
     248,   249,     0,   250,   251,   252,     0,   253,   254,   255,
     256,   257,   258,   259,   260,   261,     0,   262,   263,   264,
       0,   587,   265,   266,   267,   243,     0,     0,     0,   244,
       0,     0,     0,     0,     0,     0,     0,   240,   641,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   246,     0,
     247,   248,   249,     0,   250,   251,   252,     0,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   243,   262,   263,
     264,   244,     0,   265,   266,   267,     0,     0,     0,   240,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,   242,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     246,     0,   247,   248,   249,     0,   250,   251,   252,     0,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   243,
     262,   263,   264,   244,     0,   265,   266,   267,     0,     0,
       0,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,   698,   648,   245,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   243,   262,   263,   264,   244,     0,   265,   266,   267,
       0,     0,     0,   240,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   242,   701,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   243,   262,   263,   264,   244,     0,   265,
     266,   267,     0,     0,     0,   240,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,   242,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   246,     0,   247,   248,
     249,     0,   250,   251,   252,     0,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   243,   262,   263,   264,   244,
       0,   265,   266,   267,     0,     0,   763,   240,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   246,     0,
     247,   248,   249,     0,   250,   251,   252,     0,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   243,   262,   263,
     264,   244,     0,   265,   266,   267,     0,     0,   766,   240,
     797,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,   242,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     246,     0,   247,   248,   249,     0,   250,   251,   252,     0,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   243,
     262,   263,   264,   244,     0,   265,   266,   267,     0,     0,
       0,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   243,   262,   263,   264,   244,     0,   265,   266,   267,
       0,     0,   838,   240,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   243,   262,   263,   264,   244,     0,   265,
     266,   267,     0,     0,   839,   240,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,   242,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   246,     0,   247,   248,
     249,     0,   250,   251,   252,     0,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   243,   262,   263,   264,   244,
       0,   265,   266,   267,     0,     0,   840,   240,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   246,     0,
     247,   248,   249,     0,   250,   251,   252,     0,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   243,   262,   263,
     264,   244,     0,   265,   266,   267,     0,     0,   841,   240,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,   242,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     246,     0,   247,   248,   249,     0,   250,   251,   252,     0,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   243,
     262,   263,   264,   244,     0,   265,   266,   267,     0,     0,
     842,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   243,   262,   263,   264,   244,     0,   265,   266,   267,
       0,     0,   843,   240,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   243,   262,   263,   264,   244,     0,   265,
     266,   267,     0,     0,     0,   240,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,   242,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   246,     0,   247,   248,
     249,     0,   250,   251,   252,     0,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   575,   262,   263,   264,   244,
       0,   265,   266,   267,     0,     0,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,   246,     0,
     247,   248,   249,   243,   250,   251,   252,   244,   253,   254,
     255,   256,   257,   258,   259,   260,   261,     0,   262,   263,
     264,     0,     0,   265,   266,   267,     0,     0,   242,     0,
       0,   245,   243,     0,     0,     0,   244,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   246,     0,   247,   248,
     249,     0,   250,   251,   252,     0,   253,   254,   255,   256,
       0,   258,   259,   260,   261,   243,   262,     0,   264,   244,
       0,   265,   266,   267,     0,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,     0,
     258,   259,   260,   261,     0,   262,     0,   264,     0,     0,
     265,   266,   267,     0,     0,     0,     0,     0,   246,     0,
     247,   248,   249,     0,   250,   251,   252,     0,     0,   254,
     255,   256,     0,   258,   259,   260,   261,     0,     0,     0,
     264,     0,     0,   265,   266,   267
};

static const yytype_int16 yycheck[] =
{
      11,    41,    42,   288,   627,    16,     2,   327,    19,   353,
     148,   330,     0,    15,    25,    26,    27,    23,   166,   224,
     158,   498,    50,    34,    35,   335,    37,    38,     3,     3,
       3,   179,     3,    44,    45,    46,    47,    48,     1,     1,
      51,    52,    24,   353,   485,    63,    57,    58,    59,   197,
       1,    62,    20,    81,    65,    66,    67,   651,    69,   108,
       3,    72,    73,    74,   234,   235,   236,   237,    16,   651,
     691,    24,    44,    34,    84,    34,   108,    20,   820,    63,
      24,     1,    76,    34,    52,    57,    61,   855,   137,    84,
     791,    34,    89,    61,    37,    24,    34,     3,   866,   108,
      63,    63,    74,    77,     7,   137,   729,   135,    51,    52,
      73,    59,    80,   855,    34,   112,   134,    34,    61,    82,
      82,   132,   133,   134,   135,   136,   137,   138,   137,   139,
      78,    84,   573,    76,    38,    34,   284,   285,    89,    69,
     127,   152,   144,   127,   107,   107,   623,   108,    44,   108,
     134,   108,   134,    57,   108,   166,   107,   108,   859,    25,
     136,   136,   136,   136,   127,   136,   138,    87,   179,    89,
     108,   134,   134,   755,   108,   181,   127,   771,   497,   108,
     139,   134,   139,   137,   805,   806,   197,   107,   108,   771,
     134,   108,   108,   136,   108,   108,   819,   108,   108,   108,
     485,   139,   350,   137,    73,   134,   127,   127,   219,   108,
      60,    13,   382,   136,    64,   136,    18,   138,   127,   127,
     216,   137,   139,   137,   137,   135,   137,   127,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     139,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   483,   484,
     757,   722,    34,   152,    34,   136,   297,   108,   573,     3,
      89,   302,     3,     7,    52,   615,    89,   166,   617,   136,
     133,   108,   313,    61,   315,   316,   127,   318,   319,   108,
     179,    63,   323,   112,    25,   108,   626,   127,    76,   112,
     127,    73,    34,   108,    38,   530,   531,   338,   197,   108,
      82,   127,   108,   344,    84,    23,     2,    89,   137,   350,
     127,   313,    20,    57,   137,    11,   357,   358,   127,    61,
     135,   127,   363,   127,    20,   366,   108,   368,   108,   370,
     371,   372,     2,   374,   375,    76,     3,    33,   379,   136,
       7,    11,    60,   108,    52,   127,    64,   243,   244,   517,
      20,   108,   134,    61,   108,    51,   127,    53,    54,   139,
     734,    60,   127,    33,   738,    64,   134,   136,    76,   138,
     127,   107,   127,   127,   110,   134,    72,   379,   114,    83,
      76,    51,   110,    53,    54,   284,   285,   136,   728,   107,
     286,   287,   110,   136,   734,   138,   114,   136,   738,   138,
      89,   297,    72,    87,   136,    89,    76,   722,   127,   366,
     760,   368,   136,   454,   455,   456,   457,   458,   459,   460,
     316,     7,    19,   319,   107,   466,   467,   127,   469,   461,
     462,   127,    29,    30,   475,   137,   477,   108,    34,    36,
     481,    38,   127,   136,   485,   344,   137,    44,    26,   108,
     137,   350,    87,    89,    34,   137,   137,   127,   108,   500,
      57,   139,   503,   504,   363,    16,   134,   366,    19,   368,
     134,   167,   371,   514,   370,   137,    27,    89,    29,    30,
     820,   139,    26,    63,   137,    36,   137,    38,   127,   829,
     108,   108,    24,    73,   127,   191,    23,   167,   500,   127,
      19,    24,    82,    54,   134,    56,    57,    58,    59,    89,
      29,    30,   514,   139,   107,   855,    34,    36,    61,    38,
     216,   191,   110,   139,   139,    76,   866,    78,   108,   136,
      82,   137,   573,    60,   575,   576,   577,    64,    57,   580,
      73,   708,   539,   474,   358,    63,   216,   127,   159,   593,
     759,   592,     3,   759,   134,    73,     7,   760,   599,   455,
     601,   457,   615,   604,    82,   652,   607,   859,   467,    20,
     469,    89,   613,   819,    20,   834,   127,   339,   779,   469,
     107,    -1,   109,   110,   135,    -1,    -1,   114,    34,    -1,
     108,    37,   633,    -1,   121,   291,    -1,    -1,   639,    -1,
     641,    52,   643,   130,   645,    51,    52,   648,    -1,   127,
      61,    -1,    -1,    -1,    -1,    61,   134,    -1,    -1,   315,
      -1,   291,   318,    -1,   650,    76,    -1,   323,    -1,    80,
      76,   633,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
      -1,     7,    -1,     9,    -1,   315,    -1,    -1,   318,    -1,
      -1,    -1,    -1,   323,    20,    -1,   697,   698,    -1,   700,
     701,   702,   703,    -1,    -1,    -1,    -1,    -1,    -1,   710,
     711,    -1,   713,    -1,   715,   716,   372,    -1,   719,   575,
     576,   722,   752,   753,    -1,     2,    52,    -1,    -1,    -1,
      -1,    -1,   733,   734,    11,    61,    -1,   738,    -1,    -1,
     741,    -1,   372,    20,   745,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    80,    -1,    33,    -1,    -1,    -1,
      -1,    -1,   763,   764,   765,   766,   767,   768,   769,   770,
      -1,    -1,    -1,    19,    51,    -1,    53,    54,    -1,    -1,
     781,   782,   783,    29,    30,    -1,   787,    -1,    -1,   790,
      36,    -1,    38,   449,    -1,    72,   797,    -1,    44,    76,
      -1,    -1,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
      -1,    57,    -1,    -1,   815,   816,   817,   818,   176,    -1,
      23,   783,    -1,    -1,    -1,    -1,    -1,    -1,   829,    -1,
     831,   832,    -1,   489,    -1,    -1,   194,   838,   839,   840,
     841,   842,   843,    -1,    -1,    -1,    -1,    -1,    -1,   207,
     127,    -1,    -1,   854,   855,    -1,    -1,    60,    -1,   489,
      -1,    64,    -1,    -1,    -1,   866,    -1,    -1,    -1,   728,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   535,
      -1,    -1,    -1,   539,     2,    -1,    -1,    -1,    -1,    -1,
     167,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,    20,   106,   107,   535,   109,   110,    -1,   539,
      -1,   114,    -1,    -1,   191,    33,    -1,    -1,   121,   575,
     576,    -1,    -1,    -1,    -1,   128,   129,   130,   787,    -1,
      -1,    -1,    -1,    51,    -1,    53,    54,    -1,    -1,   216,
     298,    -1,    -1,   599,    -1,   601,    -1,    -1,   604,    -1,
      -1,   607,    -1,    -1,    72,    -1,   612,    -1,    76,    -1,
      -1,   820,    -1,    -1,   322,    -1,    -1,    -1,    -1,   599,
      -1,   601,    -1,    -1,   604,    -1,   632,   607,    -1,    -1,
      -1,    -1,   612,    -1,    -1,    -1,    -1,   643,    -1,   645,
      16,    -1,    -1,    19,   650,    -1,   855,    -1,    -1,    -1,
      -1,    27,   632,    29,    30,    -1,    -1,   866,    -1,   127,
      36,    -1,    38,   643,   291,   645,    -1,    -1,    -1,    -1,
     650,    -1,    -1,    -1,    -1,    -1,     2,    -1,    54,    23,
      56,    57,    58,    59,    -1,    11,    -1,    -1,   315,    -1,
      -1,   318,    -1,    -1,    20,    -1,   323,    -1,    -1,   167,
      76,    -1,    78,    -1,   710,    -1,    -1,    33,    -1,   715,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,   191,    -1,    51,    -1,    53,    54,   735,
     710,    -1,    -1,    -1,    -1,   715,    -1,    -1,   744,    -1,
      -1,    -1,   748,    -1,    -1,   372,    72,    -1,   216,    -1,
      76,   127,    -1,    -1,    -1,   735,    -1,    -1,    -1,   135,
      -1,    -1,    -1,   107,   744,   109,   110,    -1,   748,    -1,
     114,   479,   480,    -1,    -1,   483,   484,   121,    -1,    -1,
      -1,    -1,    -1,    -1,   128,   129,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   815,
     816,   817,   818,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,     2,   291,    -1,   831,   832,    -1,    -1,    -1,
      -1,    11,    -1,    -1,    -1,   815,   816,   817,   818,    -1,
      20,   167,    -1,    -1,    -1,    -1,    -1,   315,    23,    -1,
     318,   831,   832,    33,    -1,   323,    60,    -1,    -1,    -1,
      64,    -1,   489,    -1,    -1,   191,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    53,    54,    -1,    -1,   585,   586,    -1,
      16,    -1,    -1,    19,    -1,    60,    -1,    -1,   596,    64,
     216,    27,    72,    29,    30,    -1,    76,    -1,   606,   103,
      36,   609,    38,   107,   372,   109,   110,    -1,   535,    -1,
     114,    -1,   539,    -1,    -1,    -1,    -1,   121,    54,    -1,
      56,    57,    58,    59,   128,   129,   130,    -1,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,    -1,   114,
      76,    -1,    78,    -1,    -1,   120,   121,   127,    -1,    -1,
     125,    -1,    -1,   128,   129,   130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,
      -1,    -1,   599,    -1,   601,    -1,    -1,   604,    -1,    -1,
     607,     2,    -1,    -1,    -1,   612,    -1,   167,    -1,   315,
      11,   127,   318,    -1,    23,    -1,    -1,   323,    -1,    20,
      -1,    -1,    -1,    -1,    -1,   632,    -1,    -1,    -1,    -1,
      -1,   191,    33,    -1,    -1,    -1,   643,    -1,   645,    -1,
      -1,   489,    -1,   650,    -1,    -1,    -1,    -1,    -1,   737,
      51,    60,    53,    54,    -1,    64,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   372,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   535,    -1,    -1,
      -1,   539,    -1,    -1,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   710,   113,   114,   115,   116,   715,   118,
     119,   120,   121,    -1,   123,   124,   125,    -1,    -1,   128,
     129,   130,    -1,    -1,    -1,    -1,   127,    -1,   735,    -1,
      -1,   291,    -1,    -1,    -1,    10,    -1,   744,    -1,    -1,
      15,   748,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,   599,    -1,   601,    -1,   315,   604,    -1,   318,   607,
      -1,    -1,    -1,   323,   612,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   489,   632,    60,    -1,    -1,    -1,    64,
     191,    -1,    -1,    -1,    -1,   643,    -1,   645,    -1,    -1,
      -1,    -1,   650,    -1,    -1,    -1,    -1,    -1,   815,   816,
     817,   818,   372,    88,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   831,   832,    -1,    -1,   103,   535,
     105,   106,   107,   539,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    47,   123,   124,
     125,    -1,    -1,   128,   129,   130,    -1,    -1,    -1,    -1,
      -1,    -1,   710,    -1,   139,    -1,    -1,   715,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   735,    -1,    -1,
     291,    -1,    -1,   599,    -1,   601,   744,    -1,   604,    -1,
     748,   607,    -1,     2,    -1,    -1,   612,    -1,    -1,    -1,
      -1,    -1,    11,    -1,   315,    -1,    -1,   318,    -1,    23,
      -1,    20,   323,    -1,    -1,    -1,   632,    -1,    -1,   489,
      -1,   131,    -1,    -1,    33,    -1,    -1,   643,    -1,   645,
      -1,    -1,    -1,    -1,   650,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    53,    54,    60,    -1,    -1,    -1,
      64,    -1,   162,   163,   164,   165,    -1,   815,   816,   817,
     818,   372,    -1,    72,    -1,   535,   176,    76,    -1,   539,
      -1,    -1,    -1,   831,   832,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,    23,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,   710,   109,   110,   207,    -1,   715,
     114,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,    -1,
      -1,   125,    -1,    -1,   128,   129,   130,    -1,   127,   735,
      -1,    -1,    60,    -1,    -1,    -1,    64,    -1,   744,   599,
      -1,   601,   748,    -1,   604,    -1,    -1,   607,    -1,    -1,
      -1,    -1,   612,    -1,    13,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    22,    23,    24,    -1,    -1,   167,    -1,
      -1,    30,   632,    32,    -1,   103,    -1,   105,   106,   107,
      -1,   109,   110,   643,    -1,   645,   114,    -1,   489,    -1,
     650,    -1,   191,   121,    -1,    -1,    55,    -1,   298,    -1,
     128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,   815,
     816,   817,   818,    -1,    -1,    -1,    -1,   216,    -1,    -1,
      -1,    -1,   322,    -1,    -1,   831,   832,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   535,    -1,    95,    -1,   539,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     710,    -1,    -1,    -1,    -1,   715,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   735,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   744,    -1,    -1,    -1,   748,   148,
      -1,    -1,   291,   152,    -1,    -1,    -1,    -1,   599,   158,
     601,   160,    -1,   604,    -1,    -1,   607,   166,    -1,    -1,
     169,   612,     2,    -1,    -1,    -1,   315,    -1,    -1,   318,
     179,    11,   181,    -1,   323,   184,    -1,    -1,    -1,    -1,
      20,   632,    -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,
      -1,    -1,   643,    33,   645,    -1,    -1,    -1,    -1,   650,
      -1,    -1,    -1,    -1,    -1,   815,   816,   817,   818,    -1,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
     229,   831,   832,   372,    -1,   234,   235,   236,   237,   479,
     480,    -1,    72,   483,   484,    -1,    76,    -1,    -1,    -1,
      -1,    -1,   251,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   710,
      -1,    -1,    -1,    -1,   715,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   284,   285,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   735,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   744,    -1,    -1,    -1,   748,    -1,    -1,
     309,    -1,   311,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,   330,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
     339,    -1,    -1,    -1,    -1,   585,   586,    -1,    -1,    -1,
     489,   350,    -1,    -1,    -1,    -1,   596,    -1,   598,    -1,
      -1,   191,    -1,   603,    -1,    -1,   606,    -1,    -1,   609,
      -1,    -1,    -1,    -1,   815,   816,   817,   818,    -1,    -1,
      -1,    -1,    -1,   382,    -1,    -1,   216,    -1,    -1,    -1,
     831,   832,    -1,    -1,    -1,    -1,   535,    -1,    -1,    -1,
     539,    89,    90,    91,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,    -1,
      -1,   109,    -1,    -1,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,   125,    -1,    -1,
     128,   129,   130,   131,    -1,   133,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
     599,   291,   601,    -1,    -1,   604,    -1,    -1,   607,    -1,
     469,    25,    -1,   612,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,   315,    40,    41,   318,    43,
      -1,    45,    46,   632,   493,    -1,    -1,   737,   497,    53,
      -1,    55,    -1,    -1,   643,    -1,   645,    -1,    -1,    -1,
      -1,   650,    -1,    67,    68,    69,    70,    -1,   517,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   773,   774,    -1,   776,   777,    -1,    -1,
      -1,    -1,   372,    -1,    -1,    -1,    -1,   546,    -1,    -1,
     104,   791,    -1,    -1,    -1,    -1,    -1,    -1,   798,    -1,
     800,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,   710,    -1,    -1,    -1,    -1,   715,    -1,    -1,    -1,
      -1,    -1,   136,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   593,    -1,   735,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   744,    -1,    -1,    -1,   748,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   617,   859,
      -1,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      10,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,   489,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,   668,
      -1,    -1,    -1,    53,    -1,    55,   815,   816,   817,   818,
      60,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,
      70,    -1,   831,   832,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   535,    -1,    87,    88,   539,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,    -1,   128,   129,
     130,    -1,   751,    -1,   134,    -1,   136,    -1,   138,    -1,
     759,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   599,
      -1,   601,    -1,    -1,   604,    -1,    -1,   607,    -1,    -1,
      -1,    -1,   612,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   632,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   643,    -1,   645,    -1,    -1,     0,     1,
     650,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    -1,    85,    -1,   735,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   744,    -1,    -1,    -1,   748,    -1,
      -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
     122,    -1,   124,   125,    -1,   127,    -1,    -1,   130,    -1,
      -1,    -1,   134,    -1,   136,     1,   138,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    -1,    85,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,    -1,    -1,    -1,   134,   135,
     136,     1,   138,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    54,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    78,    79,
      80,    -1,    -1,    83,    -1,    85,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,    -1,   124,   125,   126,   127,    -1,    -1,
     130,    -1,    -1,    -1,   134,   135,   136,     1,   138,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      -1,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,
     124,   125,    -1,   127,    -1,    -1,   130,    -1,    -1,    -1,
     134,   135,   136,     1,   138,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    -1,    85,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,   127,
      -1,    -1,   130,    -1,    -1,    -1,   134,   135,   136,     1,
     138,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    -1,    85,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
     122,    -1,   124,   125,    -1,   127,    -1,    -1,   130,    -1,
      -1,    -1,   134,   135,   136,     1,   138,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    -1,    85,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,
      -1,   127,    -1,    -1,   130,    -1,    -1,    -1,   134,   135,
     136,     1,   138,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    54,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    78,    79,
      80,    -1,    -1,    83,    -1,    85,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,    -1,   124,   125,    -1,   127,    -1,    -1,
     130,    -1,    -1,    -1,   134,    -1,   136,     1,   138,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      -1,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,
     124,   125,    -1,   127,    -1,    -1,   130,    -1,    -1,    -1,
     134,    -1,   136,     1,   138,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    -1,    85,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,   127,
      -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,     1,
     138,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    -1,    85,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
     122,    -1,   124,   125,    -1,   127,    -1,    -1,   130,    -1,
      -1,    -1,   134,    -1,   136,     1,   138,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    -1,    85,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,
      -1,   127,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,
     136,     1,   138,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    78,    79,
      80,    -1,    -1,    83,    -1,    85,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,    -1,   124,   125,    -1,   127,    -1,    -1,
     130,    -1,    -1,    -1,   134,    -1,   136,     1,   138,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      -1,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,
     124,   125,    -1,   127,    -1,    -1,   130,    -1,    -1,    -1,
     134,    -1,   136,     1,   138,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    -1,    85,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,   127,
      -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,     1,
     138,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    -1,    85,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
     122,    -1,   124,   125,    -1,   127,    -1,    -1,   130,    -1,
      -1,    -1,   134,    -1,   136,     1,   138,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    -1,    85,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,
      -1,   127,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,
     136,     1,   138,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    78,    79,
      80,    -1,    -1,    83,    -1,    85,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,    -1,   124,   125,    -1,   127,    -1,    -1,
     130,    -1,    -1,    -1,   134,    -1,   136,     1,   138,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,    -1,   128,   129,   130,    -1,    -1,    -1,
     134,    -1,   136,     1,   138,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,
      -1,    -1,    -1,   111,    -1,    -1,   131,   132,   133,    -1,
      -1,   136,   120,   138,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,     1,
     138,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,   134,    -1,   136,    -1,   138,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,
     126,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,
     136,    -1,   138,   139,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,    -1,   124,   125,   126,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,    -1,   138,
     139,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
     122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
      -1,    -1,   134,    -1,   136,    -1,   138,   139,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    34,
      35,    -1,    37,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    51,    52,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,    -1,   138,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,    -1,   124,   125,   126,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,    -1,   138,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
      -1,   124,   125,   126,    -1,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,   136,   137,   138,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,   126,
      -1,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,
     137,   138,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,   134,    -1,   136,    -1,   138,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,
     125,   126,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
     135,   136,    -1,   138,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,    -1,   124,   125,   126,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,    -1,   138,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
      -1,   124,   125,    -1,   127,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,   136,    -1,   138,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,
      -1,   138,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,   134,    -1,   136,    -1,   138,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    47,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,    -1,   138,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,    -1,   124,   125,   126,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,    -1,   138,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
      -1,   124,   125,   126,    -1,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,   136,    -1,   138,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,   126,
      -1,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,
      -1,   138,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,   134,    -1,   136,    -1,   138,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,    -1,   138,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,    -1,   138,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,   136,    -1,   138,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,
      -1,   138,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,   134,    -1,   136,    -1,   138,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,    -1,   138,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,    -1,   138,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,   136,    -1,   138,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,
      -1,   138,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,   120,
      35,   122,    -1,    -1,   125,    40,    41,    -1,    43,   130,
      45,    46,    -1,   134,    10,   136,    -1,   138,    53,    15,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    60,    -1,    -1,    -1,    64,   104,
      -1,    23,    -1,   108,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    84,    -1,
      -1,   126,    88,    -1,    -1,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,   138,    -1,    -1,    -1,   103,    60,   105,
     106,   107,    64,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,   123,   124,   125,
      -1,    -1,   128,   129,   130,    10,    88,    -1,    -1,    -1,
      15,    -1,    -1,   139,    -1,    -1,    -1,    -1,    23,    -1,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,   123,   124,   125,    -1,    -1,   128,   129,   130,    10,
      -1,    -1,    -1,    -1,    15,    60,    -1,   139,    -1,    64,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    60,
     105,   106,   107,    64,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,   123,   124,
     125,    -1,    -1,   128,   129,   130,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    10,   123,   124,   125,    -1,    15,   128,   129,   130,
      -1,    -1,    -1,    -1,    23,    24,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,
      -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    84,    -1,    -1,    15,    88,
      -1,    -1,    -1,    -1,    -1,    60,    23,    24,    -1,    64,
      -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    88,   123,   124,   125,    -1,    -1,   128,
     129,   130,    -1,    60,    -1,   134,    -1,    64,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    84,   123,   124,
     125,    88,    10,   128,   129,   130,    -1,    15,    -1,   134,
      -1,    -1,    -1,    -1,    -1,    23,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,   123,   124,   125,    -1,
      10,   128,   129,   130,    -1,    15,    -1,   134,    -1,    -1,
      -1,    -1,    60,    23,    24,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    -1,
      60,    -1,    -1,    -1,    64,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,   123,   124,   125,    88,    -1,
     128,   129,   130,    -1,    60,    -1,   134,    -1,    64,    -1,
      -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    88,   123,   124,   125,    -1,    -1,   128,   129,
     130,    -1,    -1,    10,   134,    -1,    -1,   103,    15,   105,
     106,   107,    -1,   109,   110,   111,    23,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,   123,   124,   125,
      -1,    -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    60,   123,   124,   125,    64,
      -1,   128,   129,   130,    -1,    -1,    71,   134,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,   123,   124,
     125,    -1,    60,   128,   129,   130,    64,    -1,    -1,   134,
      -1,    -1,    -1,    71,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    60,   123,   124,   125,    64,    -1,
     128,   129,   130,    -1,    -1,    71,   134,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,   123,   124,   125,
      -1,    60,   128,   129,   130,    64,    -1,    -1,   134,    -1,
      -1,    -1,    71,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    60,   123,   124,   125,    64,    -1,   128,
     129,   130,    -1,    -1,    71,   134,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,   123,   124,   125,    -1,
      60,   128,   129,   130,    64,    -1,    -1,   134,    -1,    -1,
      -1,    71,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    60,   123,   124,   125,    64,    -1,   128,   129,
     130,    -1,    -1,    71,   134,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,   123,   124,   125,    -1,    60,
     128,   129,   130,    64,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    60,   123,   124,   125,    64,    -1,   128,   129,   130,
      -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    -1,   123,   124,   125,    -1,    60,   128,
     129,   130,    64,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      10,   123,   124,   125,    -1,    15,   128,   129,   130,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    49,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    23,    24,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    64,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    10,   123,   124,   125,    88,    15,   128,   129,
     130,    -1,    -1,    -1,    -1,    23,    24,    -1,    -1,    -1,
      -1,   103,    -1,   105,   106,   107,    34,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      10,   123,   124,   125,    -1,    15,   128,   129,   130,    -1,
      -1,    -1,    60,    23,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      60,    -1,    -1,    -1,    64,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,   123,   124,   125,    88,    -1,
     128,   129,   130,    -1,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    88,   123,   124,   125,    -1,   127,   128,   129,
     130,    -1,    -1,    10,    -1,    -1,    -1,   103,    15,   105,
     106,   107,    -1,   109,   110,   111,    23,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,   123,   124,   125,
      -1,   127,   128,   129,   130,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    88,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,    -1,    10,    -1,    -1,    -1,
     103,    15,   105,   106,   107,    -1,   109,   110,   111,    23,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    60,   123,
     124,   125,    64,    -1,   128,   129,   130,    -1,    -1,    71,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      60,   123,   124,   125,    64,    -1,   128,   129,   130,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    60,   123,   124,   125,    64,   127,   128,   129,
     130,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    60,   123,   124,   125,    64,   127,
     128,   129,   130,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    60,   123,   124,   125,
      64,   127,   128,   129,   130,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    60,   123,
     124,   125,    64,   127,   128,   129,   130,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      60,   123,   124,   125,    64,   127,   128,   129,   130,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    60,   123,   124,   125,    64,   127,   128,   129,
     130,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    60,   123,   124,   125,    64,   127,
     128,   129,   130,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    60,   123,   124,   125,
      64,   127,   128,   129,   130,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    60,   123,
     124,   125,    64,   127,   128,   129,   130,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      60,   123,   124,   125,    64,   127,   128,   129,   130,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    60,   123,   124,   125,    64,   127,   128,   129,
     130,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    60,   123,   124,   125,    64,   127,
     128,   129,   130,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    60,   123,   124,   125,
      64,   127,   128,   129,   130,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    60,   123,
     124,   125,    64,   127,   128,   129,   130,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      60,   123,   124,   125,    64,   127,   128,   129,   130,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    60,   123,   124,   125,    64,   127,   128,   129,
     130,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    23,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    60,   123,   124,   125,    64,    -1,
     128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    88,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,   123,   124,   125,
      -1,   127,   128,   129,   130,    60,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    60,   123,   124,
     125,    64,    -1,   128,   129,   130,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    60,
     123,   124,   125,    64,    -1,   128,   129,   130,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    24,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    60,   123,   124,   125,    64,    -1,   128,   129,   130,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    60,   123,   124,   125,    64,    -1,   128,
     129,   130,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    60,   123,   124,   125,    64,
      -1,   128,   129,   130,    -1,    -1,    71,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    60,   123,   124,
     125,    64,    -1,   128,   129,   130,    -1,    -1,    71,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    60,
     123,   124,   125,    64,    -1,   128,   129,   130,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    60,   123,   124,   125,    64,    -1,   128,   129,   130,
      -1,    -1,    71,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    60,   123,   124,   125,    64,    -1,   128,
     129,   130,    -1,    -1,    71,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    60,   123,   124,   125,    64,
      -1,   128,   129,   130,    -1,    -1,    71,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    60,   123,   124,
     125,    64,    -1,   128,   129,   130,    -1,    -1,    71,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    60,
     123,   124,   125,    64,    -1,   128,   129,   130,    -1,    -1,
      71,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    60,   123,   124,   125,    64,    -1,   128,   129,   130,
      -1,    -1,    71,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    60,   123,   124,   125,    64,    -1,   128,
     129,   130,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    60,   123,   124,   125,    64,
      -1,   128,   129,   130,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,   103,    -1,
     105,   106,   107,    60,   109,   110,   111,    64,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,   123,   124,
     125,    -1,    -1,   128,   129,   130,    -1,    -1,    23,    -1,
      -1,    88,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    60,   123,    -1,   125,    64,
      -1,   128,   129,   130,    -1,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,   123,    -1,   125,    -1,    -1,
     128,   129,   130,    -1,    -1,    -1,    -1,    -1,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,    -1,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,    -1,   128,   129,   130
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   144,   145,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    16,    17,    18,    19,    21,    22,
      24,    25,    27,    29,    30,    31,    32,    33,    35,    36,
      39,    40,    41,    42,    43,    45,    46,    48,    53,    54,
      55,    56,    58,    59,    62,    63,    65,    66,    67,    68,
      69,    70,    72,    74,    75,    76,    78,    79,    83,    85,
      88,   103,   104,   105,   106,   111,   120,   122,   124,   125,
     127,   130,   134,   136,   138,   146,   147,   148,   149,   150,
     151,   156,   157,   158,   160,   163,   164,   165,   166,   167,
     169,   170,   171,   172,   175,   176,   179,   182,   185,   186,
     208,   211,   212,   230,   231,   232,   233,   234,   235,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   253,
     254,   255,   256,   257,   148,   245,    84,   236,   237,   159,
     160,   236,    12,    31,    32,    33,    70,   134,   138,   182,
     230,   234,   243,   244,   245,   246,   248,   249,    76,   159,
     245,   148,   136,   160,     7,   159,   161,     9,    76,   161,
      52,    86,   168,   245,   245,   245,   136,   160,   183,   136,
     160,   213,   214,   148,   245,   245,   245,   245,     7,   136,
      19,    30,   165,   165,   126,   204,   224,   245,   127,   245,
     245,    24,   151,   162,   245,   245,    69,   136,   148,   245,
     245,   148,   148,   160,   209,   224,   245,   245,   245,   245,
     245,   245,   245,   245,   135,   146,   152,   224,    77,   112,
     204,   225,   226,   245,   224,   245,   252,    54,   148,    44,
     160,    38,    57,   199,    20,    52,    61,    80,   127,   133,
      10,    15,    23,    60,    64,    88,   103,   105,   106,   107,
     109,   110,   111,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   123,   124,   125,   128,   129,   130,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   131,   132,   136,   138,    60,    64,   136,   148,
     127,   134,   151,   245,   245,   245,   224,    34,   236,   209,
     127,   127,    83,   160,   204,   227,   228,   229,   245,   134,
     209,   176,   160,   136,   162,    24,    34,   162,    24,    34,
      84,   162,   239,    71,   151,   227,   148,   136,   193,    77,
     136,   160,   215,   216,     1,   107,   218,   219,    34,   108,
     162,   227,   161,   160,   108,   127,   127,   134,   148,   162,
     136,   227,   127,    89,   210,   127,   127,    28,    49,   162,
     127,   135,   146,   108,   135,   245,   108,   137,   108,   137,
      34,   108,   139,   239,    87,   108,   139,     7,   160,   107,
     177,   187,    61,   213,   213,   213,   213,   245,   245,   245,
     245,   168,   245,   168,   245,   245,   245,   245,   245,   245,
     245,    25,    76,   160,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   227,   227,   168,   245,
     168,   245,    20,    34,    37,    51,    52,    61,    76,   197,
     238,   241,   245,   257,    24,    34,    24,    34,    71,    34,
     139,   168,   245,   162,   127,   245,    87,    89,   137,   108,
     160,   180,   181,   127,   160,    34,   224,    33,   245,   168,
     245,    33,   245,   168,   245,   136,   162,   148,   245,    26,
     137,   194,   195,   196,   197,   184,   216,   108,   137,     1,
     138,   220,   231,    87,    89,   217,   245,   214,   137,   204,
     245,   173,   227,   137,   138,   220,   231,   108,   130,   153,
     155,   245,   153,   154,   135,   137,   137,   225,   137,   225,
     168,   245,   139,   148,   245,   139,   245,   139,   245,   134,
     224,   134,    52,    61,    76,   189,   198,   213,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   137,   139,
      34,    61,   230,   108,   137,    60,    60,    33,   168,   245,
      33,   168,   245,   168,   245,   236,   236,   127,   245,   204,
     245,   229,    89,   108,   135,   177,   245,   137,   245,    24,
     162,    24,   162,   245,    24,   162,   239,    24,   162,   239,
     240,   241,    26,    26,   148,   108,   137,   136,   160,    20,
      52,    61,    76,   200,   137,   216,   107,   219,   224,   245,
      47,   245,    50,    81,   135,   174,   137,   224,   209,   108,
     127,    11,   127,   139,   239,   139,   239,   148,    87,   135,
     152,   178,   188,    10,    15,    89,    90,    91,    92,    93,
      94,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   109,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   125,   128,   129,   130,   131,   133,   160,
     190,   191,   127,   241,   230,   230,   245,    24,    24,   245,
      24,    24,   139,   139,   162,   162,   245,   181,   134,   162,
      71,    33,   245,    33,   245,    71,    33,   245,   162,    33,
     245,   162,   108,   137,   148,   245,   195,   216,   107,   223,
      61,   219,   217,    34,   139,    24,   148,   224,   139,   245,
     245,    33,   148,   245,   139,    33,   148,   245,   139,   245,
     135,    30,    56,    58,   135,   147,   164,   200,   160,   110,
     136,   192,   192,    71,    33,    33,    71,    33,    33,    33,
      33,   178,   245,   245,   245,   245,   245,   245,   241,   137,
      25,    67,    70,   138,   204,   222,   231,   112,   206,   217,
      82,   207,   245,   220,   231,   148,   162,    11,   245,   148,
     245,   148,   164,   219,   160,   190,   191,   194,   245,   245,
     245,   245,   245,   245,   135,    71,    71,    71,    71,   223,
     139,   224,   204,   205,   245,   245,   151,   163,   203,   139,
     245,    71,    71,    73,   201,   192,   192,   137,    71,    71,
      71,    71,    71,    71,   245,   245,   245,   245,   206,   217,
     204,   221,   222,   231,    34,   139,   231,   245,   245,   207,
     245,   221,   222,   127,   202,   203,   139,   221
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   143,   144,   145,   145,   146,   146,   147,   147,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   149,   149,   150,
     150,   150,   151,   151,   152,   152,   153,   153,   153,   153,
     154,   154,   155,   155,   156,   156,   156,   157,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   159,   159,   160,   161,   161,   162,
     162,   163,   163,   164,   164,   164,   164,   164,   164,   164,
     165,   165,   166,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   168,   169,   169,   169,   169,
     170,   170,   171,   172,   173,   173,   174,   174,   174,   175,
     175,   176,   176,   176,   177,   177,   178,   178,   178,   179,
     180,   180,   180,   181,   181,   183,   184,   182,   185,   185,
     185,   185,   187,   188,   186,   189,   189,   189,   189,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   192,   192,   193,
     194,   194,   194,   195,   195,   195,   195,   196,   196,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   198,   198,
     198,   198,   199,   199,   200,   200,   200,   200,   200,   200,
     201,   201,   202,   202,   203,   203,   204,   204,   205,   205,
     206,   206,   207,   207,   208,   208,   208,   209,   209,   210,
     210,   210,   211,   211,   211,   211,   211,   212,   212,   212,
     213,   213,   214,   214,   214,   215,   215,   215,   216,   216,
     216,   217,   217,   217,   218,   218,   219,   219,   219,   219,
     220,   220,   220,   220,   221,   221,   221,   222,   222,   222,
     222,   222,   223,   223,   223,   223,   223,   223,   223,   224,
     224,   224,   224,   225,   225,   225,   226,   226,   227,   227,
     228,   228,   229,   229,   229,   229,   229,   230,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   233,   234,   235,
     235,   235,   235,   235,   235,   235,   235,   236,   236,   237,
     238,   238,   239,   240,   240,   241,   241,   241,   242,   242,
     243,   244,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   246,   246,   246,   246,
     247,   247,   248,   248,   248,   249,   249,   249,   250,   250,
     250,   250,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   252,   252,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   254,   254,
     254,   254,   254,   254,   255,   255,   255,   255,   256,   256,
     256,   256,   257,   257,   257,   257,   257,   257,   257
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
       1,     1,     2,     5,     3,     6,     6,     4,     5,     5,
       3,     3,     6,     5,     6,     5,     6,     3,     4,     6,
       7,     6,     7,     4,     5,     4,     4,     3,     6,     5,
       2,     2,     3,     5,     0,     2,     3,     2,     3,     6,
       8,     1,     1,     1,     0,     2,     0,     2,     3,     5,
       1,     2,     3,     1,     3,     0,     0,     8,     0,     1,
       2,     2,     0,     0,    10,     3,     3,     5,     5,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     3,
       0,     1,     3,     4,     4,     6,     6,     0,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     0,     1,
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
#line 425 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 4865 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 430 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4871 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 431 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4877 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 438 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4883 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 443 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4889 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 444 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4895 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 462 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4901 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 463 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4907 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 464 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4913 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4919 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 466 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4925 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 467 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4931 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 468 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4937 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 469 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4943 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 470 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4949 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4955 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 472 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4961 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 473 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4967 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 474 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4973 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 475 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4979 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 476 "chapel.ypp" /* yacc.c:1661  */
    { printf("syntax error"); clean_exit(1); }
#line 4985 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yyvsp[-4].b), (yylsp[-4]).comment))); }
#line 4991 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 483 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-5].b), (yylsp[-5]).comment))); }
#line 4997 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5003 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5009 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 489 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5015 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 504 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5021 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5027 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5033 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 511 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5039 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5049 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5059 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 526 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5068 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 530 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5077 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5087 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 545 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5097 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5103 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5109 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 556 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5115 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 560 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5121 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 5127 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 5133 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 5139 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 567 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 5145 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 568 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 5151 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 5157 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 570 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 5163 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 5169 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 5175 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 5181 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 5187 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 5193 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5199 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5205 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5211 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5217 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 583 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5223 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5229 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5235 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 593 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5241 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 597 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5247 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5253 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 5259 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 603 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5265 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5271 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 609 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5277 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 610 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5283 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 623 "chapel.ypp" /* yacc.c:1661  */
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
#line 5299 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5305 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5311 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5317 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5323 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5329 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5335 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 643 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5341 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 644 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5347 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 645 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5353 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5359 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5365 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 648 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt)); }
#line 5371 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 649 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), zipToTuple((yyvsp[-1].pcallexpr)), NULL, (yyvsp[0].pblockstmt), true); }
#line 5377 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 650 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt), true); }
#line 5383 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 651 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5389 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 652 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt)); }
#line 5395 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 654 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5405 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 660 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5415 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 666 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5425 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 672 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-3].pcallexpr)), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5435 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 678 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5446 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 685 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5457 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 694 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5463 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5469 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 699 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5475 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 700 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5481 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 701 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5487 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 705 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 5493 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 706 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 5499 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 710 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 5505 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 714 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5511 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 718 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5517 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 719 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5523 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 724 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5529 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 726 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5535 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 728 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5541 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 735 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), NULL, (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5550 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 740 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-6].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5559 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 747 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5569 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 752 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5579 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 757 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5589 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 765 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 5595 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 766 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 5601 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 771 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5607 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 773 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 5613 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 775 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 5619 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 780 "chapel.ypp" /* yacc.c:1661  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 5630 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 790 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[0].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[0].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
#line 5643 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 799 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 5651 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 803 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 5660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 810 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 5666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 811 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 5672 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 816 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5681 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 821 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 826 "chapel.ypp" /* yacc.c:1661  */
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
#line 5710 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 846 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5721 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 852 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5733 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 859 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5746 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 867 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 878 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5769 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 884 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5780 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 891 "chapel.ypp" /* yacc.c:1661  */
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
#line 5803 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 913 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5811 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 917 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5820 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 922 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
    }
#line 5828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 926 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5837 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 934 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5843 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 935 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 5849 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 936 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 5855 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 937 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 5861 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 938 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 5867 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 939 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 5873 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 940 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 5879 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 941 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 5885 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 942 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 5891 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 943 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 5897 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 944 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 5903 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 945 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 5909 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 946 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 5915 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 947 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 5921 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 948 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 5927 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 949 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 5933 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 950 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 5939 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 951 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 5945 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 952 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 5951 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 953 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 5957 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 954 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 5963 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 955 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 5969 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 956 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 5975 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 957 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 5981 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 958 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 5987 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 962 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 5993 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 963 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 5999 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 964 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 6005 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 965 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 6011 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 966 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 6017 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 967 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 6023 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 968 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 6029 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 969 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 6035 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 970 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 6041 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 971 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 6047 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 972 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 6053 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 973 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 6059 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 977 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 6065 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 978 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6071 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 982 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6077 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 986 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 6083 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 987 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 6089 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 988 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 6095 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 993 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 6101 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 995 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 6107 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 997 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6113 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 999 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6119 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1003 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6125 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1004 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6131 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1008 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 6137 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1009 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6143 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1010 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 6149 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1011 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 6155 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1012 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6161 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1013 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6167 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1014 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6173 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1015 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 6179 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1016 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6185 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1020 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6191 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1021 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6197 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1022 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 6203 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1023 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6209 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1027 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6215 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1028 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6221 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1032 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6227 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1033 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6233 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1034 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6239 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1035 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 6245 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1036 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 6251 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1037 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 6257 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1041 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 6263 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1042 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 6269 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1045 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 6275 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1051 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 6281 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1056 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6287 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1058 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6293 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1062 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6299 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1067 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6305 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1068 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6311 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1072 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6317 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1073 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6323 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1078 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 6329 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1080 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 6335 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1082 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 6341 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1087 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 6358 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1100 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 6376 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1116 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6382 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1118 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6388 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1120 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 6394 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1125 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6406 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1133 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6418 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1141 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6430 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1149 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 6443 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1158 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6454 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1167 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6460 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1168 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6466 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1169 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6472 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1175 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6481 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1183 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6487 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1185 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 6497 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1191 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6503 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1196 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 6509 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6515 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1200 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 6521 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1205 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 6527 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1207 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6533 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1209 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6539 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1215 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6545 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1216 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 6551 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1217 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6557 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6563 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1223 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6569 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1227 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6575 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1228 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6581 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1229 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 6587 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1230 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad type specification"); }
#line 6593 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1251 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 6601 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1255 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 6609 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1259 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 6621 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1266 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad array type specification"); clean_exit(1); }
#line 6627 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1270 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6633 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1271 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6639 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1272 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6645 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1277 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6651 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1279 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6657 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1285 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6663 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1287 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6669 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1289 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 6675 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1293 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6681 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1294 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6687 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1295 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6693 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1296 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 6699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1297 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6705 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1298 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1299 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1305 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6723 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1306 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1307 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1308 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1312 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1313 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1314 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1318 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6765 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1319 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6771 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1323 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1328 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6783 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1329 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6789 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1333 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6795 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1334 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6801 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1335 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6807 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1336 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6813 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1341 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6819 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6825 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1360 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6831 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1362 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6837 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1364 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6843 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1366 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 6849 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1368 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6855 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1370 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6861 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1375 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6867 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1377 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 6873 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1379 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6879 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1381 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6885 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1383 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6891 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1385 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6897 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1387 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6903 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1389 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 6909 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1391 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6915 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1393 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6921 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1395 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6927 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1397 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6933 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1399 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 6944 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1406 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 6954 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1412 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 6964 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1418 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 6974 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1424 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 6984 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1433 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 6990 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1437 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 6996 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1453 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7002 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1457 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 7008 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1462 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7014 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1468 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7021 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1471 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7027 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1475 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pForallIntents) = (yyvsp[-1].pForallIntents); }
#line 7033 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pForallIntents) = new ForallIntents();
      addForallIntent((yyval.pForallIntents), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7040 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1484 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pForallIntents), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7046 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1489 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[-1].pt); (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
#line 7054 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).riExp = (yyvsp[-2].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); }
#line 7060 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).riExp = (yyvsp[-2].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); }
#line 7066 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7072 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1504 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 7078 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1509 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 7084 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1525 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 7090 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1527 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7096 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1529 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7102 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 7108 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1533 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 7114 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1535 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 7120 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7126 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7132 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1560 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 7138 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7144 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 7150 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 7156 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7162 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 7168 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 7174 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 7180 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 7186 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1582 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 7192 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1583 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 7198 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1584 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 7204 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1585 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 7210 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 7216 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 7222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 7228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1589 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 7234 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1591 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 7242 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1595 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 7250 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7256 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1603 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7262 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7268 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7274 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1609 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7280 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7286 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1611 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7292 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7298 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1613 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7304 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7310 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1615 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7316 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7322 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1617 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7328 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7334 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1619 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7340 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7346 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7352 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7358 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7364 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1624 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7370 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1625 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7376 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7382 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7388 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1628 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7394 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7400 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 7406 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 7412 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 7418 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1636 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 7424 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 7430 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 7436 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7442 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1643 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7448 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1644 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7454 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1645 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7460 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1649 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7466 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1650 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7472 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1651 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7478 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1652 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7484 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1657 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 7490 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 453:
#line 1658 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 7496 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 454:
#line 1659 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 7502 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 455:
#line 1660 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 7508 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 456:
#line 1661 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 7514 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 457:
#line 1662 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 7520 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 458:
#line 1663 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 7526 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 7530 "bison-chapel.cpp" /* yacc.c:1661  */
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
