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
#define YYLAST   13302

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  145
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  120
/* YYNRULES -- Number of rules.  */
#define YYNRULES  477
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  903

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
     727,   731,   732,   736,   742,   743,   744,   748,   751,   756,
     757,   761,   763,   765,   772,   777,   785,   790,   795,   803,
     804,   809,   810,   812,   817,   827,   836,   840,   848,   849,
     854,   859,   853,   884,   890,   897,   905,   916,   922,   915,
     950,   954,   959,   963,   971,   972,   976,   977,   978,   979,
     980,   981,   982,   983,   984,   985,   986,   987,   988,   989,
     990,   991,   992,   993,   994,   995,   996,   997,   998,   999,
    1000,  1001,  1005,  1006,  1007,  1008,  1009,  1010,  1011,  1012,
    1013,  1014,  1015,  1016,  1020,  1021,  1025,  1029,  1030,  1031,
    1035,  1037,  1039,  1041,  1046,  1047,  1051,  1052,  1053,  1054,
    1055,  1056,  1057,  1058,  1059,  1063,  1064,  1065,  1066,  1067,
    1068,  1072,  1073,  1077,  1078,  1079,  1080,  1081,  1082,  1086,
    1087,  1090,  1091,  1095,  1096,  1100,  1102,  1107,  1108,  1112,
    1113,  1117,  1118,  1122,  1124,  1126,  1131,  1144,  1161,  1162,
    1164,  1169,  1177,  1185,  1193,  1202,  1212,  1213,  1214,  1218,
    1219,  1227,  1229,  1235,  1240,  1242,  1244,  1249,  1251,  1253,
    1260,  1261,  1262,  1266,  1267,  1272,  1273,  1274,  1275,  1295,
    1299,  1303,  1311,  1315,  1316,  1317,  1321,  1323,  1329,  1331,
    1333,  1338,  1339,  1340,  1341,  1342,  1343,  1344,  1350,  1351,
    1352,  1353,  1357,  1358,  1359,  1363,  1364,  1368,  1369,  1373,
    1374,  1378,  1379,  1380,  1381,  1382,  1386,  1397,  1398,  1399,
    1400,  1401,  1402,  1404,  1406,  1408,  1410,  1412,  1414,  1419,
    1421,  1423,  1425,  1427,  1429,  1431,  1433,  1435,  1437,  1439,
    1441,  1443,  1450,  1456,  1462,  1468,  1477,  1482,  1490,  1491,
    1492,  1493,  1494,  1495,  1496,  1497,  1502,  1503,  1507,  1512,
    1515,  1520,  1525,  1528,  1533,  1537,  1538,  1542,  1543,  1548,
    1553,  1561,  1562,  1563,  1564,  1565,  1566,  1567,  1568,  1569,
    1571,  1573,  1575,  1577,  1579,  1584,  1585,  1586,  1587,  1598,
    1599,  1603,  1604,  1605,  1609,  1610,  1611,  1619,  1620,  1621,
    1622,  1626,  1627,  1628,  1629,  1630,  1631,  1632,  1633,  1634,
    1635,  1639,  1647,  1648,  1652,  1653,  1654,  1655,  1656,  1657,
    1658,  1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,  1667,
    1668,  1669,  1670,  1671,  1672,  1673,  1674,  1678,  1679,  1680,
    1681,  1682,  1683,  1687,  1688,  1689,  1690,  1694,  1695,  1696,
    1697,  1702,  1703,  1704,  1705,  1706,  1707,  1708
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

#define YYPACT_NINF -804

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-804)))

#define YYTABLE_NINF -429

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -804,    71,  2765,  -804,  -804,  -804,  -804,  -804,  -804,  -804,
    -804,  3921,    -8,    90,  -804,    -8,  7667,    56,    90,  7667,
    3921,    51,    90,   350,   568,  6417,  7667,  6528,  7667,    58,
    -804,    90,  -804,    18,  3921,  7667,  7667,  -804,  7667,  7667,
     208,    98,   693,   777,  -804,  6752,  6863,  7667,  7001,  7667,
     242,   203,  3921,  7667,  7778,  7778,    90,  -804,  6752,  7667,
    7667,  -804,  -804,  7667,  -804,  -804,  8888,  7667,  7667,  -804,
    7667,  -804,  -804,  3093,  6082,  6752,  -804,  3783,  -804,  -804,
     337,  -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,
    -804,  -804,  -804,  -804,  -804,  -804,  -804,    90,  -804,   -52,
     135,  -804,  -804,  -804,   149,   262,  -804,  -804,  -804,   275,
     278,   305,   322,   325, 13108,  1582,   258,   327,   345,  -804,
    -804,  -804,  -804,  -804,  -804,    37,  -804, 13108,   338,  3921,
    -804,   349,  -804,   343,  7667,  7667,  7667,  7667,  7667,  6752,
    6752,   282,  -804,  -804,  -804,  -804, 10359,   307,  -804,  -804,
      90,   351, 10618,   401,  6752,   356,  -804,  -804,  -804,  -804,
      90,   119,    90,   355,    47,  9409,  9331,  -804,  -804,  -804,
   10297,  9538,  6752,  3921,   357,    19,    53,    43,  -804,  -804,
     373,   386,  9600,   373,  -804,  6752,   297,  -804,  -804,    90,
    -804,   100, 13108,  -804, 10680,  2171,  3921,  -804,  -804,  9600,
   13108,   361,  6752,  -804, 13108, 10742,  -804,  -804, 10804,  6846,
    -804,  -804, 10866,   409,   372,   101, 10432,  9600, 10928,    86,
    1424,   373,    86,   373,  -804,  -804,  3231,   192,  -804,  7667,
    -804,    62,    69, 13108,    59, 10990,   -29,   495,  -804,    90,
     397,  -804,  -804,  -804,    17,    18,    18,    18,  -804,  7667,
    7667,  7667,  7667,  7112,  7112,  7667,  7667,  7667,  7667,  7667,
    7667,    87,  8888,  7667,  7667,  7667,  7667,  7667,  7667,  7667,
    7667,  7667,  7667,  7667,  7667,  7667,  7667,  7667,  7667,  7667,
    7667,  7667,  7667,  7667,  7667,  7667,  7667,  7667,  7667,  7667,
    7667,  7667,  7667,  7667,  6752,  6752,  7112,  7112,  5971,  -804,
    -804,  3369,  -804, 10494, 10556, 11052,    66,  7112,    47,   378,
    -804,  -804,  7667,   230,  -804,   374,   404,  -804, 13108,    90,
     388,    90,   482,  6752,  -804,  4059,  7112,  -804,  4197,  7112,
     384,  -804,    47,  7889,  7667,  -804,  3921,   497,   391,  -804,
      55,  -804,  -804,    19,  -804,   417,   393,  -804,  5414,   440,
     444,  7667,    18,  -804,   399,  -804,  -804,  6752,  -804,  -804,
    -804,  -804,  -804,  6752,   400,  -804,   524,  -804,   524,  -804,
    5525,   432,  -804,  -804,  7889,  7667,  -804,  -804,  -804,  -804,
    6639,  -804,  2340,  6195,  -804,  6306,  -804,  7112,  5636,  2955,
     406,  7667,  5858,  -804,  -804,   412,  6752,   416,   288,    18,
     198,   229,   277,   317, 10220,  1694,  1694,   116,  -804,   116,
    -804,   116,  8938,   825,  1321,  1459,   386,    86,  -804,  -804,
    -804,  1424,  1905,   116,   934,   934,  1694,   934,   934,  1220,
      86,  1905,  2814,  1220,   373,   373,    86, 11114, 11176, 11238,
   11300, 11362, 11424, 11486, 11548, 11610, 11672, 11734, 11796, 11858,
   11920, 11982, 12044,   414,   413,  -804,   116,  -804,   116,   308,
    -804,  -804,  -804,  -804,  -804,  -804,    90,    77,  -804, 13170,
     376,  8000,  7112,  8111,  7112,  7667,  7112,  1269,    -8, 12106,
    -804,  -804, 12173,  7667,  6752,  -804,  6752,   454,   257,  -804,
    -804,   397,  7667,    82,  7667, 13108,    49,  9662,  7667, 13108,
      35,  9471,  5971,  -804,   427,   447,   429, 12235,   447,   431,
     538, 12297,  3921,  -804,   181,  -804,    26,  -804,   354,   428,
      19,    68,  -804,  6752,  -804,   263,  7667,  7223,  -804, 13108,
    -804,  -804,  -804, 13108,   251,   430,  -804,    90,  -804,  6752,
    -804,   320,    90,   437,   439,  -804,  -804,  -804,  -804,  -804,
    -804,     6,  9067,  -804,  -804, 13108,  3921, 13108,  -804, 12359,
    3507,   460,  -804,   509,  -804,  -804,  -804,  -804,  2548,   323,
    -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,
    -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,
    -804,  -804,  -804,  -804,  -804,  5971,  -804,  7112,  7112,  7667,
     548, 12421,  7667,   551, 12483,   438,  1063,    47,    47,  -804,
   13108,  -804, 13108,  -804,  7667,    90,  -804,   449,  9600,  -804,
    9724,  4335,  -804,  4473,  -804,  9786,  4611,  -804,    47,  4749,
    -804,    47,   196,  -804,  7667,  -804,  7667,  -804,  3921,  7667,
    -804,   390,  -804,    19,   471,   527,  -804,  -804,  -804,    32,
    -804,  -804,  5525,   444,    94, 13108,  -804, 13108,  4887,  6752,
    -804,  -804,  -804,   483,   343,    96,  -804,  -804,  -804,  5025,
     453,  5163,   456,  -804,  7667,  -804,  3645,   503,  -804,   354,
    -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,
    -804,  -804,  -804,  -804,  -804,    90,  -804,  -804,  -804,  -804,
    -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,  -804,
    -804,  -804,  -804,  -804,  -804,  -804,  7667,   486,   487,   457,
     457,  -804,  -804,  -804,   197,   206, 12545,  8222,  8333, 12607,
    8444,  8555,  8666,  8777,  -804,  -804, 13108,  -804,  -804,  -804,
    3921,  7667, 13108,  7667, 13108,  3921,  7667, 13108,  -804,  7667,
   13108,  -804,  5971,  -804, 12674, 13108,  -804, 13108,  -804,   461,
    7334,    99,  -804,   517,  -804,  7112,  2498,  3921,  -804,    48,
    7667,  -804,  5276,  7667,  -804, 13108,  3921,  7667,  -804, 13108,
    3921, 13108,  -804,   682,   576,   576,  -804,   634,  -804,    31,
    -804,  9269,  9019,    55,  -804,  -804,  7667,  7667,  7667,  7667,
    7667,  7667,  7667,  7667,   872, 12297,  9848,  9910, 12297,  9972,
   10034,  -804,  7667,   471,    51,  7667,  7667,  5747,  -804,  -804,
      95,  6752,  -804,  -804,  7667,   -14,  9145,  -804,   295,   401,
    -804, 13108, 10096,  -804, 10158,  -804,  -804,   529,  -804,  -804,
     457,   457,   216, 12736, 12798, 12860, 12922, 12984, 13046,  -804,
    3921,  3921,  3921,  3921, 13108,    99,  7445,   183,  -804,  -804,
   13108, 13108,  -804,  -804,  -804,  8777,  3921,  3921,  -804,   517,
    -804,  -804,  -804,  7667,  7667,  7667,  7667,  7667,  7667, 12297,
   12297, 12297, 12297,  -804,  -804,  -804,  -804,  -804,   380,  7112,
    8966,   546, 12297, 12297,    10,  9207,  -804,  -804,  -804,  -804,
    -804,  7556,  -804
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    36,    76,   421,   422,   423,   424,
     425,     0,   376,    74,   146,   376,     0,   277,    74,     0,
       0,     0,     0,    74,    74,     0,     0,   276,     0,     0,
     164,     0,   160,     0,     0,     0,     0,   367,     0,     0,
       0,     0,   276,   276,   147,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   148,     0,     0,
       0,   473,   475,     0,   476,   477,   404,     0,     0,   474,
     471,    83,   472,     0,     0,     0,     4,     0,     5,     9,
       0,    10,    11,    12,    14,   336,    21,    13,    84,    90,
      15,    17,    16,    19,    20,    18,    89,     0,    87,   396,
       0,    91,    88,    92,     0,   405,   392,   393,   339,   337,
       0,     0,   397,   398,     0,   338,     0,   406,   407,   408,
     391,   341,   340,   394,   395,     0,    23,   347,     0,     0,
     377,     0,    75,     0,     0,     0,     0,     0,     0,     0,
       0,   396,   405,   337,   397,   398,   376,   338,   406,   407,
       0,     0,     0,     0,   327,     0,    78,    77,   165,    97,
       0,   166,     0,     0,     0,     0,     0,   277,   278,    96,
       0,     0,   327,     0,     0,     0,     0,     0,   279,    30,
     459,   389,     0,   460,     7,   327,   278,    86,    85,   256,
     319,     0,   318,    81,     0,     0,     0,    80,    33,     0,
     342,     0,   327,    34,   348,     0,   131,   127,     0,   338,
     131,   128,     0,   268,     0,     0,   318,     0,     0,   462,
     403,   458,   461,   457,    42,    44,     0,     0,   322,     0,
     324,     0,     0,   323,     0,   318,     0,     0,     6,     0,
     149,   242,   241,   167,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   327,   327,     0,     0,     0,    24,
      25,     0,    26,     0,     0,     0,     0,     0,     0,     0,
      27,    28,     0,   336,   334,     0,   328,   329,   335,     0,
       0,     0,     0,     0,   106,     0,     0,   105,     0,     0,
       0,   112,     0,     0,    50,    93,     0,   122,     0,    29,
     217,   161,   284,     0,   285,   287,     0,   298,     0,     0,
     290,     0,     0,    31,     0,   166,   255,     0,    57,    82,
     139,    79,    32,   327,     0,   137,   129,   125,   130,   126,
       0,   266,   263,    54,     0,    50,    99,    35,    43,    45,
       0,   426,     0,     0,   417,     0,   419,     0,     0,     0,
       0,     0,     0,   430,     8,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   375,   454,   453,   456,   464,   463,
     468,   467,   450,   447,   448,   449,   400,   437,   416,   415,
     414,   401,   441,   452,   446,   444,   455,   445,   443,   435,
     440,   442,   451,   434,   438,   439,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   466,   465,   470,   469,   229,
     226,   227,   228,   232,   233,   234,     0,     0,   379,     0,
       0,     0,     0,     0,     0,     0,     0,   428,   376,   376,
     102,   264,     0,     0,     0,   344,     0,   158,     0,   155,
     265,   149,     0,     0,     0,   351,     0,     0,     0,   357,
       0,     0,     0,   113,   472,    53,     0,    46,    51,     0,
     121,     0,     0,   343,     0,   218,     0,   225,   243,     0,
     288,     0,   302,     0,   297,   392,     0,     0,   281,   390,
     280,   413,   321,   320,     0,     0,   345,   134,   132,     0,
     270,   392,     0,     0,     0,   427,   399,   418,   325,   420,
     326,     0,     0,   429,   118,   361,     0,   432,   431,     0,
       0,   150,   151,   239,   236,   237,   240,   168,     0,     0,
     272,   271,   273,   275,    58,    65,    66,    67,    62,    64,
      72,    73,    60,    63,    61,    59,    69,    68,    70,    71,
     411,   412,   230,   231,   384,     0,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
     333,   331,   332,   330,     0,   156,   154,     0,     0,   120,
       0,     0,   104,     0,   103,     0,     0,   110,     0,     0,
     108,     0,     0,   382,     0,    94,     0,    95,     0,     0,
     124,   224,   216,     0,   311,   244,   247,   246,   248,     0,
     286,   289,     0,   290,     0,   282,   291,   292,     0,     0,
     138,   140,   346,   135,     0,     0,   267,    55,    56,     0,
       0,     0,     0,   119,     0,    37,     0,   276,   238,   243,
     199,   197,   202,   209,   210,   211,   206,   208,   204,   207,
     205,   203,   213,   212,   178,   181,   179,   180,   191,   182,
     195,   187,   185,   198,   186,   184,   189,   194,   196,   183,
     188,   192,   193,   190,   200,   201,     0,   176,     0,   214,
     214,   174,   274,   380,   405,   405,     0,     0,     0,     0,
       0,     0,     0,     0,   101,   100,   159,   157,   151,   107,
       0,     0,   350,     0,   349,     0,     0,   356,   111,     0,
     355,   109,     0,   381,    48,    47,   123,   366,   219,     0,
       0,   290,   245,   261,   283,     0,     0,     0,   142,     0,
       0,   133,     0,     0,   116,   363,     0,     0,   114,   362,
       0,   433,    38,    74,   276,   276,   144,   276,   152,     0,
     177,     0,     0,   217,   170,   171,     0,     0,     0,     0,
       0,     0,     0,     0,   276,   354,     0,     0,   360,     0,
       0,   383,     0,   311,   314,   315,   316,     0,   313,   317,
     392,   257,   221,   220,     0,     0,     0,   300,   392,   143,
     141,   136,     0,   117,     0,   115,   153,   249,   175,   176,
     214,   214,     0,     0,     0,     0,     0,     0,     0,   145,
       0,     0,     0,     0,    49,   290,   303,     0,   258,   260,
     259,   262,   253,   254,   162,     0,     0,     0,   250,   261,
     172,   173,   215,     0,     0,     0,     0,     0,     0,   353,
     352,   359,   358,   223,   222,   305,   306,   308,   392,     0,
     428,   392,   365,   364,     0,     0,   307,   309,   251,   169,
     252,   303,   310
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -804,  -804,  -804,     1,  -631,  1921,  -804,  -804,  1294,    45,
    -264,   234,   243,  -804,  -804,   358,   377,  2174,    -5,   -82,
    -775,  -638,   -41,  -804,  -804,  -804,    36,  -804,  -804,   410,
    -804,  -804,  -804,  -804,  -804,  -804,  -804,   465,   138,   -96,
    -804,  -804,    28,   747,  -804,  -804,  -804,  -804,  -804,  -804,
    -804,  -804,  -147,  -145,  -690,  -804,  -140,    11,  -804,  -336,
    -804,  -804,   -24,  -804,  -804,  -237,   256,  -804,  -196,  -208,
    -804,  -141,  -804,   639,  -804,  -179,   315,  -804,  -332,  -641,
    -804,  -495,  -363,  -668,  -803,  -144,   -35,    65,  -804,   -71,
    -804,   184,   348,  -263,  -804,  -804,   841,  -804,    -9,  -804,
    -804,  -203,  -804,  -457,  -804,  1083,  1217,   -11,   382,  -804,
    1464,  1532,  -804,  -804,  -804,  -804,  -804,  -804,  -804,  -285
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   225,    77,   510,    79,    80,    81,   226,
     505,   509,   506,    82,    83,    84,   157,    85,   161,   198,
      86,    87,    88,    89,    90,    91,   605,    92,    93,   366,
     538,   664,    94,    95,   534,   661,    96,    97,   397,   677,
      98,   488,   489,   141,   174,   518,   100,   101,   398,   679,
     567,   718,   719,   720,   794,   341,   514,   515,   516,   466,
     568,   243,   649,   869,   899,   864,   190,   859,   822,   825,
     102,   214,   371,   103,   104,   177,   178,   345,   346,   528,
     349,   350,   524,   886,   819,   761,   227,   231,   232,   315,
     316,   317,   142,   106,   107,   108,   143,   110,   129,   130,
     467,   332,   632,   468,   111,   144,   145,   114,   147,   116,
     148,   149,   119,   120,   236,   121,   122,   123,   124,   125
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     127,   187,   188,    76,   517,   146,   133,   540,   152,   309,
     191,   519,   764,   470,   165,   166,   170,   171,   158,   320,
       5,     5,     5,   215,   180,   181,   653,   182,   183,     5,
     795,   390,   347,   347,   192,   194,   195,   199,   200,   788,
     234,   204,   205,   208,   212,   633,   787,   216,   217,   218,
     863,    46,   219,   887,   347,   220,   221,   222,  -224,   223,
     626,   164,   192,   233,   235,   400,   401,   402,   403,   347,
     508,     3,   196,   196,   621,    46,   459,  -372,   128,   351,
     399,   392,   324,   327,   331,   525,  -410,   897,  -410,  -295,
       5,   460,   330,     5,   461,   387,  -295,  -295,   342,   296,
     353,   338,   476,   297,  -295,   306,  -295,   541,   462,   463,
     252,   508,   393,   418,   354,  -295,  -295,   362,   464,   863,
     823,   330,   301,   127,   303,   304,   305,   204,   192,   235,
     765,   364,   765,   465,   150,   376,    14,   308,   723,   898,
     652,   652,  -293,   318,  -295,   330,   301,   669,   253,   836,
     870,   871,   254,   352,   763,   175,   175,   343,   357,  -295,
    -295,   318,   348,  -295,   643,   419,   788,  -295,  -295,   388,
     244,   301,   383,   787,   318,   241,   388,   652,  -295,   385,
      44,   355,  -295,   301,   301,   301,  -312,   595,   651,   154,
     527,   318,   357,  -224,   242,   259,   172,  -295,   261,    57,
     389,   384,   264,   245,   388,  -312,   388,   477,   386,  -312,
     357,   357,   246,   821,   884,   184,   596,   470,   382,   889,
     569,   619,   896,   453,   454,   259,   480,   379,   261,   358,
     373,   247,   264,   902,  -312,   766,   185,   772,   404,   405,
     406,   407,   409,   411,   412,   413,   414,   415,   416,   417,
     503,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   318,   318,   456,   458,   469,   493,   408,
     410,   641,   535,   388,   837,   811,   479,   628,   631,  -296,
       5,   482,   380,   658,   156,   517,   752,  -385,   352,   563,
     470,   759,   192,   201,   495,   497,  -386,   499,   501,   483,
     642,   484,   507,   507,   890,   511,   641,   570,  -296,   381,
     230,  -299,   455,   457,   659,   753,  -385,   -74,  -296,   352,
     529,   202,   564,   478,   592,  -386,   533,  -296,   670,   672,
     105,   565,   318,     5,  -296,   872,   -74,   156,   571,   105,
    -299,   561,   496,   507,   507,   500,   566,   615,   105,   533,
    -299,   593,   233,  -296,   233,   645,   552,   533,   555,  -299,
     557,   559,   105,   239,   115,   192,  -299,   352,   660,   525,
     131,  -369,  -296,   115,   616,   151,   294,   252,   295,  -296,
     105,   666,   115,   827,  -368,  -299,   572,   248,   646,   827,
     314,   459,   207,   211,   622,   624,   115,   647,   627,   630,
    -410,   105,  -410,   551,  -299,   105,   460,   352,   314,   461,
    -269,  -299,   648,   352,   115,   253,   209,   209,   598,   254,
     249,   314,   297,   462,   463,  -409,   573,  -409,   548,  -269,
     550,  -373,   722,   464,  -374,   115,  -371,   517,   314,   115,
     495,   601,   499,   604,   511,   606,   555,   470,   465,   607,
     608,  -304,   610,   612,  -370,   318,   298,   105,   300,   301,
     310,   618,   259,   620,   260,   261,   312,   625,   654,   264,
    -304,   469,   319,   323,  -304,   340,   271,   820,   261,   363,
     370,   372,   394,   828,   665,   277,   396,   481,   600,   828,
     603,   115,   235,   485,   486,   655,   657,   490,   492,  -304,
      14,   105,   502,    17,   512,   734,   735,   520,   235,   526,
     513,    22,   521,    23,   783,   527,   739,    27,   531,   536,
     537,    30,   542,  -163,   105,   614,   748,   556,   560,   751,
     314,   314,   562,   590,   591,   115,   -52,   634,   635,    40,
     637,   784,  -163,   785,    44,   638,   667,   650,   668,   662,
     357,     5,   678,   727,   105,   156,   730,   159,   115,   732,
     760,    56,  -301,    57,   469,   738,   409,   456,   726,  -278,
     762,   729,   770,   888,   776,   793,   167,   780,  -336,   792,
     813,   824,   891,   736,   868,   676,    23,   186,   115,   544,
     742,  -301,   744,   532,    30,   747,  -163,   543,   750,   314,
     368,  -301,  -278,   754,   769,   755,   321,   888,   757,   617,
    -301,  -278,    71,   408,   455,  -163,   532,  -301,   888,   230,
     786,   230,   804,   737,   532,   840,   160,   841,   192,   105,
    -278,    14,   758,   842,    17,   789,  -301,   900,   775,   883,
     779,   894,    22,   781,    23,   783,   169,   530,    27,   855,
     613,     0,    30,   105,  -163,  -301,   105,   379,     0,     0,
       0,     0,  -301,   115,   105,     5,     0,   830,     0,   156,
     237,     0,   784,  -163,   785,    44,     0,     0,     0,     0,
       0,     0,     0,  -278,     0,   791,     0,   115,     0,     0,
     115,     0,    56,   167,    57,     0,   742,   744,   115,   747,
     750,   775,   779,    23,   186,     0,     0,     0,     0,   805,
     806,    30,   807,  -163,   808,   809,  -278,   105,   810,   -41,
     611,   469,   314,   187,   188,  -278,     0,     0,     0,    99,
       0,     0,  -163,     0,   826,   555,     0,     0,    99,   831,
     160,   555,   832,    71,  -278,     0,   834,    99,     0,     0,
       0,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,   857,     0,     0,   805,   843,   844,   808,   845,
     846,   847,   848,     0,     0,     0,     0,   167,     0,    99,
       0,   854,     0,     0,   200,   204,   235,    23,   186,     0,
     860,     0,     0,   861,   594,    30,     0,  -163,     0,     0,
      99,     0,     0,   -40,    99,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -163,     0,     0,   879,
     880,   881,   882,   109,     0,     0,     0,     0,     0,   252,
       0,     0,   109,     0,   779,   892,   893,     0,     0,     0,
     105,   109,   879,   880,   881,   882,   892,   893,     0,     0,
       0,     0,     0,     0,     0,   109,    99,     0,   895,   555,
       0,     0,     0,     0,     0,     0,     0,   253,     0,    14,
     779,   254,    17,   109,   115,     0,     0,     0,     0,     0,
      22,     0,    23,   783,   105,     0,    27,     0,   105,     0,
      30,     0,  -163,     0,   109,     0,   721,     0,   109,     0,
      99,     0,     0,     0,     0,     0,     0,     0,    40,     0,
     784,  -163,   785,    44,   259,     0,   260,   261,   115,     0,
       0,   264,   115,    99,     0,   724,   725,     0,   271,     0,
      56,     0,    57,     0,     0,   275,   276,   277,   252,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   105,
     109,   105,     0,    99,   105,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,     0,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   253,     0,     0,     0,
     254,    71,     0,   115,     0,   115,   105,     0,   115,   849,
       0,   115,     0,     0,   109,     0,   818,   105,     0,   105,
     115,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   109,     0,   256,
     115,   257,   258,   259,     0,   260,   261,   262,    99,     0,
     264,   115,     0,   115,     0,     0,   270,   271,   115,     0,
       0,   274,     0,     0,   275,   276,   277,   109,     0,     0,
       0,     0,    99,   250,     0,    99,     0,   858,   251,     0,
       0,     0,     0,    99,     0,   112,     0,   252,   105,     0,
       0,     0,     0,   105,   112,     0,     0,     0,     0,     0,
       0,     0,     0,   112,     0,     0,     0,     0,     0,     0,
       0,     0,   885,     0,     0,   105,     0,   112,     0,     0,
       0,     0,   115,     0,   105,   253,     0,   115,   105,   254,
       0,     0,     0,     0,     0,   112,    99,     0,     0,     0,
       0,     0,   109,     0,     0,     0,   885,     0,     0,   115,
       0,     0,     0,   255,     0,     0,   112,   885,   115,     0,
     112,     0,   115,     0,     0,     0,   109,     0,   256,   109,
     257,   258,   259,     0,   260,   261,   262,   109,   263,   264,
     265,   266,   267,   268,   269,   270,   271,     0,   272,   273,
     274,     0,     0,   275,   276,   277,     0,     0,   105,   105,
     105,   105,     0,     0,   733,     0,     0,     0,     0,     0,
       0,     0,   112,     0,   105,   105,     0,     0,     0,   113,
       0,     0,     0,     0,     0,     0,     0,     0,   113,     0,
     109,     0,   115,   115,   115,   115,     0,   113,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   115,   115,
       0,   113,     0,     0,     0,     0,   112,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   113,
       0,     0,     5,     6,     7,     8,     9,    10,     0,   112,
       0,   134,   253,     0,     0,     0,   254,     0,     0,     0,
     113,     0,     0,     0,   113,    21,     0,     0,     0,     0,
       0,   135,   136,    99,   137,     0,    29,    99,     0,   112,
       0,    32,    33,     0,    35,     0,    36,    37,     0,     0,
       0,     0,     0,     0,    39,   256,    41,   257,   258,   259,
       0,   260,   261,     0,     0,     0,   264,     0,    49,    50,
      51,   138,   197,   271,     0,   252,   113,     0,   206,   210,
     275,   276,   277,   109,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
      99,     0,     0,    99,     0,    63,    99,     0,     0,     0,
       0,     0,     0,   253,   112,    99,     0,   254,     0,     0,
     113,     0,     0,    68,     0,     0,     0,   109,     0,     0,
       0,   109,     0,     0,     0,    99,     0,    74,   112,   140,
       0,   112,     0,   113,     0,     0,    99,     0,    99,   112,
       0,     0,     0,    99,     0,     0,   256,   302,     0,   258,
     259,     0,   260,   261,     0,     0,     0,   264,     0,     0,
       0,     0,     0,   113,   271,     0,     0,     0,   252,     0,
       0,   275,   276,   277,     0,     0,     0,     0,   197,   197,
     197,     0,   109,     0,   109,   337,   117,   109,     0,     0,
     109,     0,   112,     0,     0,   117,   197,     0,     0,   109,
       0,     0,     0,   252,   117,     0,   253,    99,     0,     0,
     254,     0,    99,   197,     0,     0,     0,     0,   117,   109,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     109,   197,   109,     0,    99,     0,   117,   109,   113,     0,
       0,   253,     0,    99,     0,   254,     0,    99,     0,   256,
       0,   257,   258,   259,   118,   260,   261,   117,     0,     0,
     264,   117,   113,   118,     0,   113,   270,   271,     0,     0,
       0,   274,   118,   113,   275,   276,   277,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   118,     0,   259,     0,
     260,   261,     0,     0,     0,   264,     0,     0,     0,     0,
       0,   109,   271,     0,   118,     0,   109,     0,     0,   275,
     276,   277,     0,   117,     0,   112,     0,    99,    99,    99,
      99,     0,   197,     0,     0,   118,   113,     0,   109,   118,
       0,     0,     0,    99,    99,     0,     0,   109,     0,     0,
       0,   109,     0,     0,     0,     0,   197,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   117,     0,   112,
       0,     0,     0,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     117,   118,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,     0,     0,     0,
     117,   109,   109,   109,   109,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   112,   118,   112,   109,   109,   112,
       0,     0,   112,     0,     0,   292,   293,  -387,   252,     0,
    -409,   112,  -409,     0,     0,     0,     0,     0,   118,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   112,     0,   112,     0,   253,     0,   118,   112,
     254,     0,     0,     0,     0,   117,     0,     0,     0,     0,
       0,     0,     0,   113,     0,     0,     0,   113,     0,     0,
       0,     0,     0,     0,   255,     0,     0,     0,     0,   117,
     197,   197,   117,     0,   197,   197,     0,     0,     0,   256,
     117,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,     0,   268,   269,   270,   271,     0,   272,
     273,   274,     0,   112,   275,   276,   277,     0,   112,     0,
       0,     0,     0,   118,     0,     0,     0,     0,   113,     0,
     113,     0,     0,   113,     0,     0,   113,     0,     0,     0,
     112,     0,     0,   117,     0,   113,     0,   118,     0,   112,
     118,     0,     0,   112,     0,     0,     0,     0,   118,     0,
       0,     0,     0,     0,     0,   113,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,     0,   113,     0,
       0,     0,     0,   113,     0,     0,     0,     0,     0,     0,
       0,   197,   197,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,     0,   337,     0,     0,     0,     0,   337,
       0,   118,   197,    78,     0,   197,     0,     0,     0,   252,
       0,     0,   126,   112,   112,   112,   112,     0,     0,     0,
       0,   153,     0,     0,     0,     0,     0,     0,     0,   112,
     112,     0,     0,     0,     0,   179,     0,   113,   771,     0,
       0,     0,   113,     0,     0,     0,     0,   253,     0,     0,
       0,   254,     0,   203,     0,     0,   117,     0,     0,     0,
       0,     0,     0,     0,   113,     0,     0,     0,     0,     0,
       0,     0,     0,   113,    78,     0,     0,   113,   238,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     117,   264,   265,   266,   117,   268,   269,   270,   271,     0,
       0,     0,   274,     0,     0,   275,   276,   277,     0,     0,
       0,     0,     0,     0,   118,     0,     0,     0,     0,     0,
     299,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   197,     0,     0,     0,   113,   113,   113,
     113,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   113,   113,   117,     0,   117,   118,     0,
     117,     0,   118,   117,   339,     0,     0,     0,     0,     0,
     337,   337,   117,   337,   337,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   361,     0,   862,
       0,     0,   117,     0,     0,     0,   337,     0,   337,     0,
       0,     0,     0,   117,     0,   117,     0,     0,     0,     0,
     117,     0,     0,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,   118,     0,   118,     0,     0,   118,     0,
       0,   118,     0,     0,     0,     0,     0,     0,     0,     0,
     118,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   250,     0,     0,     0,     0,   251,   132,   862,     0,
     118,     0,   132,     0,     0,   252,   155,   132,   132,     0,
       0,   118,     0,   118,   117,   173,     0,   176,   118,   117,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,     0,     0,     0,     0,     0,     0,     0,
     213,   117,     0,   253,     0,     0,     0,   254,     0,     0,
     117,     0,     0,     0,   117,     0,   361,     0,     0,   361,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   240,   118,     0,     0,     0,   256,   118,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,     0,   272,   273,   274,   118,
       0,   275,   276,   277,     0,     0,     0,   360,   118,     0,
     554,     0,   118,     0,   117,   117,   117,   117,     0,     0,
       0,     0,     0,     0,   213,     0,     0,     0,   313,     0,
     117,   117,     0,     0,   213,     0,   322,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,   344,
     250,     0,     0,     0,     0,   251,     0,     0,     0,   313,
     132,     0,     0,   356,   252,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,     0,
       0,     0,   118,   118,   118,   118,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   118,   118,
       0,     0,   253,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,   176,   176,
     176,   176,     0,     0,     0,     0,     0,     0,     0,     0,
     255,     0,     0,   640,     0,   420,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,     0,   272,   273,   274,   313,   313,
     275,   276,   277,     0,     0,     0,     0,   673,     0,   546,
       0,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   487,     0,   491,     0,     0,     0,   522,
       0,     5,     6,     7,     8,     9,    10,     0,  -428,     0,
     134,     0,     0,  -428,     0,     0,     0,   344,     0,     0,
       0,     0,  -428,     0,    21,     0,   176,     0,     0,     0,
     135,   136,     0,   137,     0,    29,     0,   313,     0,     0,
      32,    33,   361,    35,   361,    36,    37,   361,     0,     0,
     361,     5,     0,    39,     0,    41,     0,     0,   680,   756,
    -428,     0,     0,   681,  -428,     0,     0,    49,    50,    51,
     138,     0,     0,   176,     0,     0,     0,     0,     0,   768,
       0,     0,     0,     0,     0,     0,     0,  -294,  -428,     0,
     774,     0,   778,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,  -428,    63,  -428,  -428,  -428,     0,  -428,
    -428,  -428,     0,  -428,  -428,  -428,  -428,  -428,  -428,  -428,
    -428,  -428,    68,  -428,  -428,  -428,     0,     0,  -428,  -428,
    -428,     0,     0,     0,   139,     0,    74,     0,   539,   682,
     683,   684,   685,   686,   687,     0,     0,   688,   689,   690,
     691,   692,   693,   694,   695,   696,   697,     0,     0,   698,
     313,     0,     0,   699,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,     0,   710,     0,     0,   711,   712,
     713,   714,     0,   715,     0,     0,   716,     0,   829,     0,
     644,     0,     0,     0,   344,     0,     0,   833,     0,     0,
       0,   835,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   663,     0,     0,     0,     0,   213,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   717,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    -2,     4,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -276,    18,    19,   487,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
      28,     0,    29,    30,     0,  -163,    31,    32,    33,    34,
      35,   -39,    36,    37,     0,    38,     0,   344,     0,  -276,
      39,    40,    41,    42,  -163,    43,    44,     0,  -276,    45,
      46,     0,    47,    48,    49,    50,    51,    52,   252,    53,
       0,    54,    55,    56,     0,    57,    58,  -276,     0,     0,
      59,     0,    60,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   790,
      62,    63,    64,    65,     0,     0,   253,     0,    66,     0,
     254,     0,     0,     0,     0,     0,     0,    67,     0,    68,
       0,    69,    70,     0,    71,     0,     0,    72,     0,     0,
       0,    73,     0,    74,   255,    75,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,     0,   268,   269,   270,   271,     0,   272,
       0,   274,     0,     0,   275,   276,   277,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,   132,     5,     6,
       7,     8,     9,    10,     0,  -428,   839,    11,    12,    13,
    -428,     0,    14,    15,    16,    17,  -276,    18,    19,  -428,
      20,    21,  -428,    22,     0,    23,    24,    25,    26,    27,
      28,     0,    29,    30,     0,  -163,    31,    32,    33,    34,
      35,   -39,    36,    37,     0,    38,     0,     0,     0,  -276,
      39,     0,    41,    42,  -163,    43,    44,  -428,  -276,    45,
      46,  -428,    47,    48,    49,    50,    51,    52,     0,    53,
       0,    54,    55,    56,     0,    57,    58,  -276,     0,     0,
      59,     0,    60,     0,     0,  -428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -428,    63,  -428,  -428,  -428,  -428,  -428,  -428,  -428,     0,
    -428,  -428,  -428,  -428,  -428,  -428,  -428,  -428,  -428,    68,
    -428,  -428,  -428,     0,    71,  -428,  -428,  -428,     0,     0,
       0,    73,  -428,    74,     4,    75,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,     0,
      14,    15,    16,    17,  -276,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,    28,     0,
      29,    30,     0,  -163,    31,    32,    33,    34,    35,   -39,
      36,    37,     0,    38,     0,     0,     0,  -276,    39,    40,
      41,    42,  -163,    43,    44,     0,  -276,    45,    46,     0,
      47,    48,    49,    50,    51,    52,     0,    53,     0,    54,
      55,    56,     0,    57,    58,  -276,     0,     0,    59,     0,
      60,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,    68,     0,    69,
      70,   189,    71,     0,     0,    72,     0,     0,     0,    73,
     224,    74,     4,    75,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -276,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,    28,     0,    29,    30,
       0,  -163,    31,    32,    33,    34,    35,   -39,    36,    37,
       0,    38,     0,     0,     0,  -276,    39,    40,    41,    42,
    -163,    43,    44,     0,  -276,    45,    46,     0,    47,    48,
      49,    50,    51,    52,     0,    53,     0,    54,    55,    56,
       0,    57,    58,  -276,     0,     0,    59,     0,    60,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,    64,    65,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,    68,     0,    69,    70,     0,
      71,     0,     0,    72,     0,     0,     0,    73,   378,    74,
       4,    75,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -276,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,    28,     0,    29,    30,     0,  -163,
      31,    32,    33,    34,    35,   -39,    36,    37,     0,    38,
       0,     0,     0,  -276,    39,    40,    41,    42,  -163,    43,
      44,     0,  -276,    45,    46,     0,    47,    48,    49,    50,
      51,    52,     0,    53,     0,    54,    55,    56,     0,    57,
      58,  -276,     0,     0,    59,     0,    60,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,    67,     0,    68,     0,    69,    70,     0,    71,     0,
       0,    72,     0,     0,     0,    73,   224,    74,     4,    75,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -276,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,    28,     0,    29,    30,     0,  -163,    31,    32,
      33,    34,    35,   -39,    36,    37,     0,    38,     0,     0,
       0,  -276,    39,    40,    41,    42,  -163,    43,    44,     0,
    -276,    45,    46,     0,    47,    48,    49,    50,    51,    52,
       0,    53,     0,    54,    55,    56,     0,    57,    58,  -276,
       0,     0,    59,     0,    60,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,    65,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,    68,     0,    69,    70,     0,    71,     0,     0,    72,
       0,     0,     0,    73,   675,    74,     4,    75,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -276,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
      28,     0,    29,    30,     0,  -163,    31,    32,    33,    34,
      35,   -39,    36,    37,     0,    38,     0,     0,     0,  -276,
      39,    40,    41,    42,  -163,    43,    44,     0,  -276,    45,
      46,     0,    47,    48,    49,    50,    51,    52,     0,    53,
       0,    54,    55,    56,     0,    57,    58,  -276,     0,     0,
      59,     0,    60,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,    68,
       0,    69,    70,     0,    71,     0,     0,    72,     0,     0,
       0,    73,   782,    74,     4,    75,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,     0,
      14,    15,    16,    17,  -276,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,    28,     0,
      29,    30,     0,  -163,    31,    32,    33,    34,    35,   -39,
      36,    37,     0,    38,     0,     0,     0,  -276,    39,   237,
      41,    42,  -163,    43,    44,     0,  -276,    45,    46,     0,
      47,    48,    49,    50,    51,    52,     0,    53,     0,    54,
      55,    56,     0,    57,    58,  -276,     0,     0,    59,     0,
      60,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,    68,     0,    69,
      70,     0,    71,     0,     0,    72,     0,     0,     0,    73,
       0,    74,     4,    75,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -276,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,    28,     0,    29,    30,
       0,  -163,    31,    32,    33,    34,    35,   -39,    36,    37,
       0,    38,     0,     0,     0,  -276,    39,     0,    41,    42,
    -163,    43,    44,     0,  -276,    45,    46,     0,    47,    48,
      49,    50,    51,    52,     0,    53,     0,    54,    55,    56,
       0,    57,    58,  -276,     0,     0,    59,     0,    60,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,    64,    65,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,    68,     0,    69,    70,     0,
      71,     0,     0,    72,     0,     0,     0,    73,     0,    74,
       4,    75,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -276,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,   494,     0,    29,    30,     0,  -163,
      31,    32,    33,    34,    35,   -39,    36,    37,     0,    38,
       0,     0,     0,  -276,    39,     0,    41,    42,  -163,    43,
      44,     0,  -276,    45,    46,     0,    47,    48,    49,    50,
      51,    52,     0,    53,     0,    54,    55,    56,     0,    57,
      58,  -276,     0,     0,    59,     0,    60,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,    67,     0,    68,     0,    69,    70,     0,    71,     0,
       0,    72,     0,     0,     0,    73,     0,    74,     4,    75,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -276,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,   498,     0,    29,    30,     0,  -163,    31,    32,
      33,    34,    35,   -39,    36,    37,     0,    38,     0,     0,
       0,  -276,    39,     0,    41,    42,  -163,    43,    44,     0,
    -276,    45,    46,     0,    47,    48,    49,    50,    51,    52,
       0,    53,     0,    54,    55,    56,     0,    57,    58,  -276,
       0,     0,    59,     0,    60,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,    65,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,    68,     0,    69,    70,     0,    71,     0,     0,    72,
       0,     0,     0,    73,     0,    74,     4,    75,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -276,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
     741,     0,    29,    30,     0,  -163,    31,    32,    33,    34,
      35,   -39,    36,    37,     0,    38,     0,     0,     0,  -276,
      39,     0,    41,    42,  -163,    43,    44,     0,  -276,    45,
      46,     0,    47,    48,    49,    50,    51,    52,     0,    53,
       0,    54,    55,    56,     0,    57,    58,  -276,     0,     0,
      59,     0,    60,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,    68,
       0,    69,    70,     0,    71,     0,     0,    72,     0,     0,
       0,    73,     0,    74,     4,    75,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,     0,
      14,    15,    16,    17,  -276,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,   743,     0,
      29,    30,     0,  -163,    31,    32,    33,    34,    35,   -39,
      36,    37,     0,    38,     0,     0,     0,  -276,    39,     0,
      41,    42,  -163,    43,    44,     0,  -276,    45,    46,     0,
      47,    48,    49,    50,    51,    52,     0,    53,     0,    54,
      55,    56,     0,    57,    58,  -276,     0,     0,    59,     0,
      60,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,    68,     0,    69,
      70,     0,    71,     0,     0,    72,     0,     0,     0,    73,
       0,    74,     4,    75,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -276,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,   746,     0,    29,    30,
       0,  -163,    31,    32,    33,    34,    35,   -39,    36,    37,
       0,    38,     0,     0,     0,  -276,    39,     0,    41,    42,
    -163,    43,    44,     0,  -276,    45,    46,     0,    47,    48,
      49,    50,    51,    52,     0,    53,     0,    54,    55,    56,
       0,    57,    58,  -276,     0,     0,    59,     0,    60,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,    64,    65,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,    68,     0,    69,    70,     0,
      71,     0,     0,    72,     0,     0,     0,    73,     0,    74,
       4,    75,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -276,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,   749,     0,    29,    30,     0,  -163,
      31,    32,    33,    34,    35,   -39,    36,    37,     0,    38,
       0,     0,     0,  -276,    39,     0,    41,    42,  -163,    43,
      44,     0,  -276,    45,    46,     0,    47,    48,    49,    50,
      51,    52,     0,    53,     0,    54,    55,    56,     0,    57,
      58,  -276,     0,     0,    59,     0,    60,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,    67,     0,    68,     0,    69,    70,     0,    71,     0,
       0,    72,     0,     0,     0,    73,     0,    74,     4,    75,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -276,    18,
      19,     0,   767,    21,     0,    22,     0,    23,    24,    25,
      26,    27,    28,     0,    29,    30,     0,  -163,    31,    32,
      33,    34,    35,   -39,    36,    37,     0,    38,     0,     0,
       0,  -276,    39,     0,    41,    42,  -163,    43,    44,     0,
    -276,    45,    46,     0,    47,    48,    49,    50,    51,    52,
       0,    53,     0,    54,    55,    56,     0,    57,    58,  -276,
       0,     0,    59,     0,    60,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,    65,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,    68,     0,    69,    70,     0,    71,     0,     0,    72,
       0,     0,     0,    73,     0,    74,     4,    75,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -276,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
     773,     0,    29,    30,     0,  -163,    31,    32,    33,    34,
      35,   -39,    36,    37,     0,    38,     0,     0,     0,  -276,
      39,     0,    41,    42,  -163,    43,    44,     0,  -276,    45,
      46,     0,    47,    48,    49,    50,    51,    52,     0,    53,
       0,    54,    55,    56,     0,    57,    58,  -276,     0,     0,
      59,     0,    60,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,    68,
       0,    69,    70,     0,    71,     0,     0,    72,     0,     0,
       0,    73,     0,    74,     4,    75,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,     0,
      14,    15,    16,    17,  -276,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,   777,     0,
      29,    30,     0,  -163,    31,    32,    33,    34,    35,   -39,
      36,    37,     0,    38,     0,     0,     0,  -276,    39,     0,
      41,    42,  -163,    43,    44,     0,  -276,    45,    46,     0,
      47,    48,    49,    50,    51,    52,     0,    53,     0,    54,
      55,    56,     0,    57,    58,  -276,     0,     0,    59,     0,
      60,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,   522,     0,     5,
       6,     7,     8,     9,    10,    67,  -428,    68,   134,    69,
      70,  -428,    71,     0,     0,    72,     0,     0,     0,    73,
    -428,    74,    21,    75,     0,     0,     0,     0,   135,   136,
       0,   137,     0,    29,     0,     0,     0,     0,    32,    33,
       0,    35,     0,    36,    37,     0,     0,     0,     0,     0,
       0,    39,     0,    41,     0,     0,     0,     0,  -428,     0,
       0,     0,  -428,     0,     0,    49,    50,    51,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -428,    63,  -428,  -428,  -428,     0,  -428,  -428,  -428,
       0,  -428,  -428,  -428,  -428,  -428,  -428,  -428,  -428,  -428,
      68,  -428,  -428,  -428,     0,     0,  -428,  -428,  -428,     0,
       0,     0,   139,     0,    74,   522,   539,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   135,   136,     0,   137,
       0,    29,     0,     0,     0,     0,    32,    33,     0,    35,
       0,    36,    37,     0,     0,     0,     0,     0,     0,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,   522,    66,     5,     6,
       7,     8,     9,    10,     0,     0,    67,   134,    68,     0,
      69,    70,     0,     0,     0,     0,    72,     0,     0,     0,
     139,    21,    74,     0,   523,     0,     0,   135,   136,     0,
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
       0,   139,    21,    74,     0,   539,     0,     0,   135,   136,
       0,   137,     0,    29,     0,     0,     0,     0,    32,    33,
       0,    35,     0,    36,    37,     0,     0,     0,     0,     0,
       0,    39,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       5,     6,     7,     8,     9,    10,     0,     0,    67,   134,
      68,     0,    69,    70,   189,     0,     0,     0,    72,     0,
       0,     0,   139,    21,    74,     0,   140,   553,     0,   135,
     136,     0,   137,     0,    29,     0,     0,     0,     0,    32,
      33,     0,    35,     0,    36,    37,     0,     0,     0,     0,
       0,     0,    39,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,    65,     0,     0,     0,     0,
      66,     5,     6,     7,     8,     9,    10,     0,     0,    67,
     134,    68,     0,    69,    70,   189,     0,     0,     0,    72,
       0,     0,     0,   139,    21,    74,     0,   140,   856,     0,
     135,   136,     0,   137,     0,    29,     0,     0,     0,     0,
      32,    33,     0,    35,     0,    36,    37,     0,     0,     0,
       0,     0,     0,    39,     0,    41,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,    51,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,    63,    64,    65,     0,     0,     0,
       0,    66,     0,     0,     5,     6,     7,     8,     9,    10,
      67,     0,    68,   134,    69,    70,     0,     0,     0,     0,
      72,     0,   459,     0,   139,     0,    74,    21,   140,   558,
       0,     0,     0,   135,   136,     0,   137,   460,    29,     0,
     461,     0,     0,    32,    33,     0,    35,     0,    36,    37,
       0,     0,     0,     0,   462,   463,    39,     0,    41,     0,
       0,     0,     0,     0,   464,     0,     0,     0,     0,     0,
      49,    50,    51,   138,     0,     0,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,    64,    65,
       0,     0,     0,     0,    66,     5,     6,     7,     8,     9,
      10,     0,     0,    67,   134,    68,     0,    69,    70,     0,
       0,     0,     0,    72,     0,     0,     0,   139,    21,    74,
       0,   140,     0,     0,   135,   136,     0,   137,     0,    29,
       0,     0,     0,     0,    32,    33,     0,    35,     0,    36,
      37,     0,     0,     0,     0,     0,     0,    39,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,   138,     0,     0,     0,     0,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,   229,     0,     5,     6,
       7,     8,     9,    10,    67,     0,    68,   134,    69,    70,
     189,     0,     0,     0,    72,     0,     0,     0,   139,     0,
      74,    21,   140,     0,     0,     0,     0,   135,   136,     0,
     137,     0,    29,     0,     0,     0,     0,    32,    33,     0,
      35,     0,    36,    37,     0,     0,     0,     0,     0,     0,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,   138,     0,     0,
       0,     0,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     5,
       6,     7,     8,     9,    10,     0,     0,    67,   134,    68,
       0,    69,    70,   189,     0,     0,     0,    72,     0,     0,
       0,   139,    21,    74,   547,   140,     0,     0,   135,   136,
       0,   137,     0,    29,     0,     0,     0,     0,    32,    33,
       0,    35,     0,    36,    37,     0,     0,     0,     0,     0,
       0,    39,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,   138,     0,
       0,     0,     0,     0,     0,   228,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       5,     6,     7,     8,     9,    10,     0,     0,    67,   134,
      68,     0,    69,    70,   189,     0,     0,     0,    72,     0,
       0,     0,   139,    21,    74,   549,   140,     0,     0,   135,
     136,     0,   137,     0,    29,     0,     0,     0,     0,    32,
      33,     0,    35,     0,    36,    37,     0,     0,     0,     0,
       0,   162,    39,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   163,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,    65,     0,     0,     0,     0,
      66,     5,     6,     7,     8,     9,    10,     0,     0,    67,
     134,    68,     0,    69,    70,     0,     0,     0,   167,    72,
       0,     0,     0,   139,    21,    74,     0,   140,     0,   168,
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
      72,     0,     0,     0,   139,    21,    74,     0,   140,     0,
       0,   135,   136,     0,   137,     0,    29,     0,     0,     0,
       0,    32,    33,     0,    35,     0,    36,    37,     0,     0,
       0,     0,     0,     0,    39,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,   138,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     0,     0,     5,     6,     7,     8,     9,
      10,    67,     0,    68,   134,    69,    70,   189,     0,     0,
       0,    72,     0,     0,     0,   139,   545,    74,    21,   140,
       0,     0,     0,     0,   135,   136,     0,   137,     0,    29,
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
      74,     0,   140,     0,     0,   135,   136,     0,   137,     0,
      29,     0,     0,     0,     0,    32,    33,     0,    35,     0,
      36,    37,     0,     0,     0,     0,     0,     0,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,   138,     0,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     0,     0,   292,
     293,     0,     0,     0,  -409,    67,  -409,    68,     0,    69,
      70,     0,   193,     0,     0,    72,     0,     0,     0,   139,
       0,    74,     0,   140,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   196,    21,     0,     0,
       0,     0,     0,   135,   136,     0,   137,     0,    29,     0,
       0,     0,     0,    32,    33,     0,    35,     0,    36,    37,
       0,     0,     0,     0,     0,     0,    39,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      49,    50,    51,   138,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,    64,    65,
       0,     0,     0,     0,    66,     5,     6,     7,     8,     9,
      10,     0,     0,    67,   134,    68,     0,    69,    70,     0,
       0,     0,     0,    72,     0,     0,     0,    73,    21,    74,
       0,   140,     0,     0,   135,   136,     0,   137,     0,    29,
       0,     0,     0,     0,    32,    33,     0,    35,     0,    36,
      37,     0,     0,     0,     0,     0,     0,    39,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     163,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,     5,     6,     7,     8,
       9,    10,     0,     0,    67,   134,    68,     0,    69,    70,
       0,     0,     0,     0,    72,     0,     0,     0,   139,    21,
      74,     0,   140,     0,     0,   135,   136,     0,   137,     0,
      29,     0,     0,     0,     0,    32,    33,     0,    35,     0,
      36,    37,   656,     0,     0,     0,     0,     0,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,   138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     5,     6,     7,
       8,     9,    10,     0,     0,    67,   134,    68,     0,    69,
      70,     0,     0,     0,     0,    72,     0,     0,     0,   139,
     814,    74,     0,   140,     0,     0,   135,   136,     0,   137,
       0,    29,     0,     0,     0,     0,    32,    33,     0,    35,
       0,    36,    37,     0,     0,     0,     0,     0,     0,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   815,    50,    51,   816,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,     5,     6,
       7,     8,     9,    10,     0,     0,    67,   134,    68,     0,
      69,    70,   189,     0,     0,     0,    72,     0,     0,     0,
     139,    21,    74,     0,   817,     0,     0,   135,   136,     0,
     137,     0,    29,     0,     0,     0,     0,    32,    33,     0,
      35,     0,    36,    37,     0,     0,     0,     0,     0,     0,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     5,
       6,     7,     8,     9,    10,     0,     0,    67,   134,    68,
       0,    69,    70,   189,     0,     0,     0,    72,     0,     0,
       0,   139,    21,    74,     0,   817,     0,     0,   135,   136,
       0,   803,     0,    29,     0,     0,     0,     0,    32,    33,
       0,    35,     0,    36,    37,     0,     0,     0,     0,     0,
       0,    39,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       5,     6,     7,     8,     9,    10,     0,     0,    67,   134,
      68,     0,    69,    70,   189,     0,     0,     0,    72,     0,
       0,     0,   139,    21,    74,     0,   140,     0,     0,   135,
     136,     0,   137,     0,    29,     0,     0,     0,     0,    32,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     5,     6,     7,     8,     9,    10,     0,
       0,    67,   134,    68,     0,    69,    70,     0,     0,     0,
       0,   504,     0,     0,     0,   139,    21,    74,     0,   140,
       0,     0,   135,   136,     0,   599,     0,    29,     0,     0,
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
     140,     0,     0,   135,   136,     0,   602,     0,    29,     0,
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
       0,   140,     0,     0,   135,   136,     0,   797,     0,    29,
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
      74,     0,   140,     0,     0,   135,   136,     0,   798,     0,
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
      21,    74,     0,   140,     0,     0,   135,   136,     0,   800,
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
     801,     0,    29,     0,     0,     0,     0,    32,    33,     0,
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
       0,   802,     0,    29,     0,     0,     0,     0,    32,    33,
       0,    35,     0,    36,    37,     0,     0,     0,     0,     0,
       0,    39,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       5,     6,     7,     8,     9,    10,     0,     0,    67,   134,
      68,     0,    69,    70,     0,     0,     0,     0,    72,     0,
       0,     0,   139,    21,    74,     0,   140,     0,     0,   135,
     136,     0,   803,     0,    29,     0,     0,     0,     0,    32,
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
     135,   136,     0,   137,     0,    29,     0,     0,     0,     0,
      32,    33,     0,    35,     0,    36,    37,     0,     0,     0,
       0,     0,     0,    39,     0,    41,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,    51,
     138,     0,   252,     0,     0,     0,     0,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   134,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,    62,    63,    64,    65,     0,   135,   136,
     253,   137,     0,    29,   254,     0,     0,     0,    32,    33,
      67,    35,    68,    36,    37,    70,     0,     0,     0,     0,
      72,    39,     5,    41,   139,     0,    74,     0,   140,   680,
       0,     0,     0,     0,   681,    49,    50,    51,   138,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,  -303,   268,   269,
     270,   271,     0,   272,     0,   274,     0,     0,   275,   276,
     277,     0,    63,     0,     0,     0,  -303,   250,     0,     0,
    -303,     0,   251,     0,     0,     0,     0,     0,     0,     0,
      68,   252,     0,     0,   189,     0,     0,     0,     0,     0,
       0,     0,   139,     0,    74,  -303,   817,     0,     0,     0,
     682,   683,   684,   685,   686,   687,     0,     0,   688,   689,
     690,   691,   692,   693,   694,   695,   696,   697,     0,   253,
     698,     0,     0,   254,   699,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   709,     0,   710,     0,     0,   711,
     712,   713,   714,   330,   715,   250,     0,   255,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,     0,   272,   273,   274,     0,     0,   275,   276,   277,
       0,     0,     0,     0,     0,     0,     0,   253,   671,     0,
       0,   254,     0,     0,     0,     0,     0,   250,     0,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   253,
     272,   273,   274,   254,     0,   275,   276,   277,     0,   250,
       0,     0,     0,     0,   251,     0,   865,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,     0,   275,   276,   277,
       0,   250,     0,     0,     0,     0,   251,     0,   901,     0,
       0,     0,     0,     0,     0,   252,   328,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,   329,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,     0,   275,
     276,   277,     0,     0,     0,     0,     0,     0,   838,     0,
       0,     0,     0,     0,     0,     0,     0,   330,     0,   250,
       0,   255,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,   325,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,   326,   263,   264,   265,   266,
     267,   268,   269,   270,   271,     0,   272,   273,   274,     0,
       0,   275,   276,   277,     0,     0,     0,   301,     0,     0,
       0,   253,     0,     0,     0,   254,     0,     0,     0,     0,
       0,   250,     0,     0,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,   629,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,     0,   275,
     276,   277,     0,     0,     0,   301,     0,     0,   250,     0,
       0,     0,     0,   251,     0,     0,     0,   330,     0,     0,
       0,   255,   252,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,     0,   272,   273,   274,     0,
     253,   275,   276,   277,   254,     0,     0,   301,     0,     0,
     250,   336,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,   196,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   253,   272,   273,   274,   254,     0,   275,   276,
     277,     0,   250,     0,   301,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   252,   623,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   253,   272,   273,   274,   254,     0,
     275,   276,   277,     0,   250,     0,   301,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,   257,
     258,   259,     0,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   253,   272,   273,   274,
     254,     0,   275,   276,   277,     0,   250,   740,   301,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   253,   272,
     273,   274,   254,     0,   275,   276,   277,     0,   250,   745,
     301,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,   257,   258,   259,     0,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     253,   272,   273,   274,   254,     0,   275,   276,   277,     0,
     250,   850,   301,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   253,   272,   273,   274,   254,     0,   275,   276,
     277,     0,   250,   851,   301,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   252,     0,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   253,   272,   273,   274,   254,     0,
     275,   276,   277,     0,   250,   852,   301,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,   257,
     258,   259,     0,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   253,   272,   273,   274,
     254,     0,   275,   276,   277,     0,   250,   853,   301,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   253,   272,
     273,   274,   254,     0,   275,   276,   277,     0,   250,   866,
     301,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,   257,   258,   259,     0,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     253,   272,   273,   274,   254,     0,   275,   276,   277,     0,
     250,   867,   301,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   253,   272,   273,   274,   254,     0,   275,   276,
     277,     0,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   250,     0,     0,
     255,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,   256,   333,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,     0,   272,   273,   274,   334,     0,
     275,   276,   277,     0,     0,  -388,     0,     0,     0,   253,
       0,     0,     0,   254,     0,     0,     0,     0,     0,   250,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,   307,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,   335,   275,   276,   277,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   250,     0,     0,   128,     0,   251,     0,   255,
       0,     0,     0,     0,     0,     0,   252,     0,     0,     0,
       0,   374,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   375,   272,   273,   274,     0,     0,   275,
     276,   277,     0,     0,   253,     0,     0,     0,   254,     0,
       0,     0,     0,     0,   250,     0,     0,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,   252,   471,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
     472,     0,     0,     0,     0,     0,     0,   256,     0,   257,
     258,   259,     0,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   253,   272,   273,   274,
     254,     0,   275,   276,   277,     0,   250,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,   473,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,   474,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   253,   272,
     273,   274,   254,     0,   275,   276,   277,     0,   250,     0,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,   257,   258,   259,     0,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     253,   272,   273,   274,   254,     0,   275,   276,   277,     0,
     250,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   253,   272,   273,   274,   254,   311,   275,   276,
     277,     0,   250,     0,     0,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   252,     0,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   253,   272,   273,   274,   254,   359,
     275,   276,   277,     0,   250,     0,     0,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,   257,
     258,   259,     0,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   253,   272,   273,   274,
     254,   365,   275,   276,   277,     0,   250,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   253,   272,
     273,   274,   254,   367,   275,   276,   277,     0,   250,     0,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,   257,   258,   259,     0,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     253,   272,   273,   274,   254,   369,   275,   276,   277,     0,
     250,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   253,   272,   273,   274,   254,   377,   275,   276,
     277,     0,   250,     0,     0,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   252,     0,     0,   391,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   253,   272,   273,   274,   254,     0,
     275,   276,   277,     0,   250,   475,     0,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,   257,
     258,   259,     0,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   253,   272,   273,   274,
     254,     0,   275,   276,   277,     0,   250,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   253,   272,
     273,   274,   254,   574,   275,   276,   277,     0,   250,     0,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,   257,   258,   259,     0,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     253,   272,   273,   274,   254,   575,   275,   276,   277,     0,
     250,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   253,   272,   273,   274,   254,   576,   275,   276,
     277,     0,   250,     0,     0,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   252,     0,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   253,   272,   273,   274,   254,   577,
     275,   276,   277,     0,   250,     0,     0,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,   257,
     258,   259,     0,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   253,   272,   273,   274,
     254,   578,   275,   276,   277,     0,   250,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   253,   272,
     273,   274,   254,   579,   275,   276,   277,     0,   250,     0,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,   257,   258,   259,     0,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     253,   272,   273,   274,   254,   580,   275,   276,   277,     0,
     250,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   253,   272,   273,   274,   254,   581,   275,   276,
     277,     0,   250,     0,     0,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   252,     0,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   253,   272,   273,   274,   254,   582,
     275,   276,   277,     0,   250,     0,     0,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,   257,
     258,   259,     0,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   253,   272,   273,   274,
     254,   583,   275,   276,   277,     0,   250,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   253,   272,
     273,   274,   254,   584,   275,   276,   277,     0,   250,     0,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,   257,   258,   259,     0,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     253,   272,   273,   274,   254,   585,   275,   276,   277,     0,
     250,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   253,   272,   273,   274,   254,   586,   275,   276,
     277,     0,   250,     0,     0,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   252,     0,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   253,   272,   273,   274,   254,   587,
     275,   276,   277,     0,   250,     0,     0,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,   257,
     258,   259,     0,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   253,   272,   273,   274,
     254,   588,   275,   276,   277,     0,   250,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   253,   272,
     273,   274,   254,   589,   275,   276,   277,     0,     0,     0,
       0,     0,     0,   250,     0,     0,     0,     0,   251,     0,
       0,     0,   128,     0,     0,     0,   255,   252,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,   257,   258,   259,     0,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
       0,   272,   273,   274,     0,   253,   275,   276,   277,   254,
       0,     0,     0,     0,     0,   250,   636,     0,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
     257,   258,   259,     0,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   253,   272,   273,
     274,   254,   609,   275,   276,   277,     0,   250,     0,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,   639,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   253,
     272,   273,   274,   254,     0,   275,   276,   277,     0,   250,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   253,   272,   273,   274,   254,     0,   275,   276,   277,
       0,   250,     0,     0,     0,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,   252,   728,     0,   674,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   257,   258,   259,     0,
     260,   261,   262,     0,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   253,   272,   273,   274,   254,     0,   275,
     276,   277,     0,   250,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   252,   731,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,     0,   257,   258,
     259,     0,   260,   261,   262,     0,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   253,   272,   273,   274,   254,
       0,   275,   276,   277,     0,   250,     0,     0,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   256,     0,
     257,   258,   259,     0,   260,   261,   262,     0,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   253,   272,   273,
     274,   254,     0,   275,   276,   277,     0,   250,   796,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     256,     0,   257,   258,   259,     0,   260,   261,   262,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   253,
     272,   273,   274,   254,     0,   275,   276,   277,     0,     0,
     799,     0,     0,     0,   250,   812,     0,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,   255,   252,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,   257,   258,   259,     0,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,     0,   272,   273,   274,     0,   253,   275,   276,   277,
     254,     0,     0,     0,     0,     0,   250,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   253,   272,
     273,   274,   254,     0,   275,   276,   277,     0,   250,   873,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,   257,   258,   259,     0,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     253,   272,   273,   274,   254,     0,   275,   276,   277,     0,
     250,   874,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   253,   272,   273,   274,   254,     0,   275,   276,
     277,     0,   250,   875,     0,     0,     0,   251,     0,     0,
       0,     0,     0,     0,     0,     0,   252,     0,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   253,   272,   273,   274,   254,     0,
     275,   276,   277,     0,   250,   876,     0,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   256,     0,   257,
     258,   259,     0,   260,   261,   262,     0,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   253,   272,   273,   274,
     254,     0,   275,   276,   277,     0,   250,   877,     0,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,   255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
       0,   257,   258,   259,     0,   260,   261,   262,     0,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   253,   272,
     273,   274,   254,     0,   275,   276,   277,     0,   250,   878,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   252,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   256,     0,   257,   258,   259,     0,   260,   261,   262,
       0,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     253,   272,   273,   274,   254,     0,   275,   276,   277,     0,
     250,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,   257,   258,   259,     0,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   597,   272,   273,   274,   254,     0,   275,   276,
     277,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     255,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,   257,   258,   259,
       0,   260,   261,   262,     0,   263,   264,   265,   266,   267,
     268,   269,   270,   271,     0,   272,   273,   274,     0,     0,
     275,   276,   277
};

static const yytype_int16 yycheck[] =
{
      11,    42,    43,     2,   340,    16,    15,   370,    19,   150,
      45,   343,   653,   298,    25,    26,    27,    28,    23,   160,
       3,     3,     3,    58,    35,    36,   521,    38,    39,     3,
     720,   234,     1,     1,    45,    46,    47,    48,    49,   677,
      75,    52,    53,    54,    55,   502,   677,    58,    59,    60,
     825,    65,    63,   856,     1,    66,    67,    68,     3,    70,
      25,    25,    73,    74,    75,   244,   245,   246,   247,     1,
     334,     0,    25,    25,    25,    65,    21,   129,    86,    36,
      63,   110,   164,   165,   166,   348,   138,   890,   140,    36,
       3,    36,    86,     3,    39,    36,    65,    65,    79,    62,
     182,   172,    36,    66,    36,   140,    75,   370,    53,    54,
      24,   375,   141,    26,   185,    84,    84,   199,    63,   894,
     761,    86,   136,   134,   135,   136,   137,   138,   139,   140,
      36,   202,    36,    78,    78,   217,    17,   146,   595,   129,
     109,   109,    89,   154,    91,    86,   136,   141,    62,   787,
     840,   841,    66,   110,   649,   138,   138,   138,   110,    91,
     129,   172,   109,   110,   138,    78,   804,   136,   136,   110,
      21,   136,   110,   804,   185,    40,   110,   109,   110,   110,
      61,   186,   129,   136,   136,   136,    91,   110,   520,   138,
      91,   202,   110,   138,    59,   109,   138,   129,   112,    80,
     141,   139,   116,    54,   110,   110,   110,   141,   139,   114,
     110,   110,    63,   114,   855,     7,   139,   502,   229,    36,
     399,   139,   890,   294,   295,   109,   308,   226,   112,   129,
     129,    82,   116,   901,   139,   141,   138,   141,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     332,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   323,   253,
     254,   110,   363,   110,   789,   752,   307,   500,   501,    36,
       3,   312,   110,    52,     7,   641,   110,   110,   110,    21,
     595,   643,   323,    71,   325,   326,   110,   328,   329,    89,
     139,    91,   333,   334,   141,   336,   110,   129,    65,   137,
      74,    36,   296,   297,    83,   139,   139,    40,    75,   110,
     351,   138,    54,   307,    36,   139,   357,    84,   551,   552,
       2,    63,   363,     3,    91,   139,    59,     7,   129,    11,
      65,   396,   326,   374,   375,   329,    78,   110,    20,   380,
      75,    63,   383,   110,   385,    21,   387,   388,   389,    84,
     391,   392,    34,    46,     2,   396,    91,   110,   137,   652,
      13,   129,   129,    11,   137,    18,   138,    24,   140,   136,
      52,   542,    20,   766,   129,   110,   129,   129,    54,   772,
     154,    21,    54,    55,   496,   497,    34,    63,   500,   501,
     138,    73,   140,   387,   129,    77,    36,   110,   172,    39,
     110,   136,    78,   110,    52,    62,    54,    55,    62,    66,
     135,   185,    66,    53,    54,   138,   129,   140,   383,   129,
     385,   129,   129,    63,   129,    73,   129,   793,   202,    77,
     471,   472,   473,   474,   475,   476,   477,   752,    78,   478,
     479,    91,   483,   484,   129,   486,   138,   129,   129,   136,
     129,   492,   109,   494,   111,   112,    85,   498,   523,   116,
     110,   502,   136,   138,   114,   138,   123,   760,   112,   138,
      91,   129,     7,   766,   539,   132,   109,   129,   472,   772,
     474,   129,   523,   139,   110,   526,   527,   129,    36,   139,
      17,   173,   138,    20,    27,   607,   608,   110,   539,    89,
     139,    28,   139,    30,    31,    91,   618,    34,   139,   139,
      16,    38,   110,    40,   196,    91,   628,   141,   136,   631,
     294,   295,   136,   139,   141,   173,   129,   110,   129,    56,
     129,    58,    59,    60,    61,    27,   129,   139,   129,   139,
     110,     3,    63,    25,   226,     7,    25,     9,   196,   141,
     109,    78,    36,    80,   595,   136,   597,   598,   599,    21,
      63,   602,   109,   856,   141,   138,    20,   141,   112,   112,
     139,    84,   865,   614,    75,   560,    30,    31,   226,   375,
     621,    65,   623,   357,    38,   626,    40,   374,   629,   363,
     210,    75,    54,   634,   659,   636,   161,   890,   639,   491,
      84,    63,   129,   597,   598,    59,   380,    91,   901,   383,
     137,   385,   738,   615,   388,   792,    78,   792,   659,   301,
      82,    17,   641,   793,    20,   679,   110,   894,   669,   855,
     671,   869,    28,   674,    30,    31,    27,   352,    34,   813,
     486,    -1,    38,   325,    40,   129,   328,   676,    -1,    -1,
      -1,    -1,   136,   301,   336,     3,    -1,   769,    -1,     7,
      56,    -1,    58,    59,    60,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,   716,    -1,   325,    -1,    -1,
     328,    -1,    78,    20,    80,    -1,   727,   728,   336,   730,
     731,   732,   733,    30,    31,    -1,    -1,    -1,    -1,   740,
     741,    38,   743,    40,   745,   746,    54,   389,   749,    46,
     484,   752,   486,   784,   785,    63,    -1,    -1,    -1,     2,
      -1,    -1,    59,    -1,   765,   766,    -1,    -1,    11,   770,
      78,   772,   773,   129,    82,    -1,   777,    20,    -1,    -1,
      -1,   389,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,   817,    -1,    -1,   796,   797,   798,   799,   800,
     801,   802,   803,    -1,    -1,    -1,    -1,    20,    -1,    52,
      -1,   812,    -1,    -1,   815,   816,   817,    30,    31,    -1,
     821,    -1,    -1,   824,   466,    38,    -1,    40,    -1,    -1,
      73,    -1,    -1,    46,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,   850,
     851,   852,   853,     2,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    11,    -1,   865,   866,   867,    -1,    -1,    -1,
     512,    20,   873,   874,   875,   876,   877,   878,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,   129,    -1,   889,   890,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    17,
     901,    66,    20,    52,   512,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    30,    31,   556,    -1,    34,    -1,   560,    -1,
      38,    -1,    40,    -1,    73,    -1,   568,    -1,    77,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      58,    59,    60,    61,   109,    -1,   111,   112,   556,    -1,
      -1,   116,   560,   196,    -1,   597,   598,    -1,   123,    -1,
      78,    -1,    80,    -1,    -1,   130,   131,   132,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   621,
     129,   623,    -1,   226,   626,    -1,    -1,   629,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   638,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      66,   129,    -1,   621,    -1,   623,   658,    -1,   626,   137,
      -1,   629,    -1,    -1,   173,    -1,   760,   669,    -1,   671,
     638,    -1,    -1,    -1,   676,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,    -1,   105,
     658,   107,   108,   109,    -1,   111,   112,   113,   301,    -1,
     116,   669,    -1,   671,    -1,    -1,   122,   123,   676,    -1,
      -1,   127,    -1,    -1,   130,   131,   132,   226,    -1,    -1,
      -1,    -1,   325,    10,    -1,   328,    -1,   821,    15,    -1,
      -1,    -1,    -1,   336,    -1,     2,    -1,    24,   740,    -1,
      -1,    -1,    -1,   745,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   856,    -1,    -1,   767,    -1,    34,    -1,    -1,
      -1,    -1,   740,    -1,   776,    62,    -1,   745,   780,    66,
      -1,    -1,    -1,    -1,    -1,    52,   389,    -1,    -1,    -1,
      -1,    -1,   301,    -1,    -1,    -1,   890,    -1,    -1,   767,
      -1,    -1,    -1,    90,    -1,    -1,    73,   901,   776,    -1,
      77,    -1,   780,    -1,    -1,    -1,   325,    -1,   105,   328,
     107,   108,   109,    -1,   111,   112,   113,   336,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    -1,   125,   126,
     127,    -1,    -1,   130,   131,   132,    -1,    -1,   850,   851,
     852,   853,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,   866,   867,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,
     389,    -1,   850,   851,   852,   853,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,   866,   867,
      -1,    34,    -1,    -1,    -1,    -1,   173,    -1,    -1,   512,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   196,
      -1,    12,    62,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    77,    26,    -1,    -1,    -1,    -1,
      -1,    32,    33,   556,    35,    -1,    37,   560,    -1,   226,
      -1,    42,    43,    -1,    45,    -1,    47,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    55,   105,    57,   107,   108,   109,
      -1,   111,   112,    -1,    -1,    -1,   116,    -1,    69,    70,
      71,    72,    48,   123,    -1,    24,   129,    -1,    54,    55,
     130,   131,   132,   512,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   621,    -1,
     623,    -1,    -1,   626,    -1,   106,   629,    -1,    -1,    -1,
      -1,    -1,    -1,    62,   301,   638,    -1,    66,    -1,    -1,
     173,    -1,    -1,   124,    -1,    -1,    -1,   556,    -1,    -1,
      -1,   560,    -1,    -1,    -1,   658,    -1,   138,   325,   140,
      -1,   328,    -1,   196,    -1,    -1,   669,    -1,   671,   336,
      -1,    -1,    -1,   676,    -1,    -1,   105,   133,    -1,   108,
     109,    -1,   111,   112,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,   226,   123,    -1,    -1,    -1,    24,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,   164,   165,
     166,    -1,   621,    -1,   623,   171,     2,   626,    -1,    -1,
     629,    -1,   389,    -1,    -1,    11,   182,    -1,    -1,   638,
      -1,    -1,    -1,    24,    20,    -1,    62,   740,    -1,    -1,
      66,    -1,   745,   199,    -1,    -1,    -1,    -1,    34,   658,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     669,   217,   671,    -1,   767,    -1,    52,   676,   301,    -1,
      -1,    62,    -1,   776,    -1,    66,    -1,   780,    -1,   105,
      -1,   107,   108,   109,     2,   111,   112,    73,    -1,    -1,
     116,    77,   325,    11,    -1,   328,   122,   123,    -1,    -1,
      -1,   127,    20,   336,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,    34,    -1,   109,    -1,
     111,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,   740,   123,    -1,    52,    -1,   745,    -1,    -1,   130,
     131,   132,    -1,   129,    -1,   512,    -1,   850,   851,   852,
     853,    -1,   308,    -1,    -1,    73,   389,    -1,   767,    77,
      -1,    -1,    -1,   866,   867,    -1,    -1,   776,    -1,    -1,
      -1,   780,    -1,    -1,    -1,    -1,   332,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   556,
      -1,    -1,    -1,   560,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
     226,   850,   851,   852,   853,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   621,   173,   623,   866,   867,   626,
      -1,    -1,   629,    -1,    -1,   133,   134,   135,    24,    -1,
     138,   638,   140,    -1,    -1,    -1,    -1,    -1,   196,   512,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   658,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   669,    -1,   671,    -1,    62,    -1,   226,   676,
      66,    -1,    -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   556,    -1,    -1,    -1,   560,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,   325,
     496,   497,   328,    -1,   500,   501,    -1,    -1,    -1,   105,
     336,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,    -1,   740,   130,   131,   132,    -1,   745,    -1,
      -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,   621,    -1,
     623,    -1,    -1,   626,    -1,    -1,   629,    -1,    -1,    -1,
     767,    -1,    -1,   389,    -1,   638,    -1,   325,    -1,   776,
     328,    -1,    -1,   780,    -1,    -1,    -1,    -1,   336,    -1,
      -1,    -1,    -1,    -1,    -1,   658,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   669,    -1,   671,    -1,
      -1,    -1,    -1,   676,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   607,   608,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   618,    -1,   620,    -1,    -1,    -1,    -1,   625,
      -1,   389,   628,     2,    -1,   631,    -1,    -1,    -1,    24,
      -1,    -1,    11,   850,   851,   852,   853,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   866,
     867,    -1,    -1,    -1,    -1,    34,    -1,   740,   664,    -1,
      -1,    -1,   745,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    66,    -1,    52,    -1,    -1,   512,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   767,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   776,    73,    -1,    -1,   780,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     556,   116,   117,   118,   560,   120,   121,   122,   123,    -1,
      -1,    -1,   127,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   769,    -1,    -1,    -1,   850,   851,   852,
     853,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   866,   867,   621,    -1,   623,   556,    -1,
     626,    -1,   560,   629,   173,    -1,    -1,    -1,    -1,    -1,
     806,   807,   638,   809,   810,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,    -1,   825,
      -1,    -1,   658,    -1,    -1,    -1,   832,    -1,   834,    -1,
      -1,    -1,    -1,   669,    -1,   671,    -1,    -1,    -1,    -1,
     676,    -1,    -1,    -1,    -1,    -1,    -1,   226,    -1,    -1,
      -1,    -1,    -1,   621,    -1,   623,    -1,    -1,   626,    -1,
      -1,   629,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     638,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    13,   894,    -1,
     658,    -1,    18,    -1,    -1,    24,    22,    23,    24,    -1,
      -1,   669,    -1,   671,   740,    31,    -1,    33,   676,   745,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   301,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,   767,    -1,    62,    -1,    -1,    -1,    66,    -1,    -1,
     776,    -1,    -1,    -1,   780,    -1,   325,    -1,    -1,   328,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,   740,    -1,    -1,    -1,   105,   745,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    -1,   125,   126,   127,   767,
      -1,   130,   131,   132,    -1,    -1,    -1,   136,   776,    -1,
     389,    -1,   780,    -1,   850,   851,   852,   853,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,   154,    -1,
     866,   867,    -1,    -1,   160,    -1,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,   175,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,   185,
     186,    -1,    -1,   189,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,    -1,    -1,    -1,
      -1,    -1,   850,   851,   852,   853,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   866,   867,
      -1,    -1,    62,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,   239,    -1,    -1,    -1,    -1,   244,   245,
     246,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,   512,    -1,   261,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,   125,   126,   127,   294,   295,
     130,   131,   132,    -1,    -1,    -1,    -1,   556,    -1,   139,
      -1,   560,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   319,    -1,   321,    -1,    -1,    -1,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    -1,    -1,    15,    -1,    -1,    -1,   343,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,   352,    -1,    -1,    -1,
      32,    33,    -1,    35,    -1,    37,    -1,   363,    -1,    -1,
      42,    43,   621,    45,   623,    47,    48,   626,    -1,    -1,
     629,     3,    -1,    55,    -1,    57,    -1,    -1,    10,   638,
      62,    -1,    -1,    15,    66,    -1,    -1,    69,    70,    71,
      72,    -1,    -1,   399,    -1,    -1,    -1,    -1,    -1,   658,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,
     669,    -1,   671,    -1,    -1,    -1,    -1,   676,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,    91,
      92,    93,    94,    95,    96,    -1,    -1,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    -1,    -1,   111,
     486,    -1,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,    -1,    -1,   130,   131,
     132,   133,    -1,   135,    -1,    -1,   138,    -1,   767,    -1,
     516,    -1,    -1,    -1,   520,    -1,    -1,   776,    -1,    -1,
      -1,   780,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   537,    -1,    -1,    -1,    -1,   542,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   568,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     0,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,   615,
      25,    26,    -1,    28,    -1,    30,    31,    32,    33,    34,
      35,    -1,    37,    38,    -1,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    50,    -1,   643,    -1,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    -1,    67,    68,    69,    70,    71,    72,    24,    74,
      -1,    76,    77,    78,    -1,    80,    81,    82,    -1,    -1,
      85,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   695,
     105,   106,   107,   108,    -1,    -1,    62,    -1,   113,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,
      -1,   126,   127,    -1,   129,    -1,    -1,   132,    -1,    -1,
      -1,   136,    -1,   138,    90,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,    -1,   120,   121,   122,   123,    -1,   125,
      -1,   127,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,   783,     3,     4,
       5,     6,     7,     8,    -1,    10,   792,    12,    13,    14,
      15,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    -1,    30,    31,    32,    33,    34,
      35,    -1,    37,    38,    -1,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
      -1,    76,    77,    78,    -1,    80,    81,    82,    -1,    -1,
      85,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    -1,   129,   130,   131,   132,    -1,    -1,
      -1,   136,   137,   138,     1,   140,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    -1,    25,    26,
      -1,    28,    -1,    30,    31,    32,    33,    34,    35,    -1,
      37,    38,    -1,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    50,    -1,    -1,    -1,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    63,    64,    65,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    76,
      77,    78,    -1,    80,    81,    82,    -1,    -1,    85,    -1,
      87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
     107,   108,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,   126,
     127,   128,   129,    -1,    -1,   132,    -1,    -1,    -1,   136,
     137,   138,     1,   140,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    -1,    25,    26,    -1,    28,
      -1,    30,    31,    32,    33,    34,    35,    -1,    37,    38,
      -1,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    50,    -1,    -1,    -1,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    -1,    76,    77,    78,
      -1,    80,    81,    82,    -1,    -1,    85,    -1,    87,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,    -1,   126,   127,    -1,
     129,    -1,    -1,   132,    -1,    -1,    -1,   136,   137,   138,
       1,   140,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    -1,    25,    26,    -1,    28,    -1,    30,
      31,    32,    33,    34,    35,    -1,    37,    38,    -1,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,    50,
      -1,    -1,    -1,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    63,    64,    65,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    -1,    76,    77,    78,    -1,    80,
      81,    82,    -1,    -1,    85,    -1,    87,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,    -1,   126,   127,    -1,   129,    -1,
      -1,   132,    -1,    -1,    -1,   136,   137,   138,     1,   140,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    -1,    25,    26,    -1,    28,    -1,    30,    31,    32,
      33,    34,    35,    -1,    37,    38,    -1,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    -1,    50,    -1,    -1,
      -1,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    -1,    67,    68,    69,    70,    71,    72,
      -1,    74,    -1,    76,    77,    78,    -1,    80,    81,    82,
      -1,    -1,    85,    -1,    87,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,   124,    -1,   126,   127,    -1,   129,    -1,    -1,   132,
      -1,    -1,    -1,   136,   137,   138,     1,   140,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    -1,
      25,    26,    -1,    28,    -1,    30,    31,    32,    33,    34,
      35,    -1,    37,    38,    -1,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    50,    -1,    -1,    -1,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      -1,    76,    77,    78,    -1,    80,    81,    82,    -1,    -1,
      85,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,
      -1,   126,   127,    -1,   129,    -1,    -1,   132,    -1,    -1,
      -1,   136,   137,   138,     1,   140,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    -1,    25,    26,
      -1,    28,    -1,    30,    31,    32,    33,    34,    35,    -1,
      37,    38,    -1,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    50,    -1,    -1,    -1,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    63,    64,    65,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    76,
      77,    78,    -1,    80,    81,    82,    -1,    -1,    85,    -1,
      87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
     107,   108,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,   126,
     127,    -1,   129,    -1,    -1,   132,    -1,    -1,    -1,   136,
      -1,   138,     1,   140,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    -1,    25,    26,    -1,    28,
      -1,    30,    31,    32,    33,    34,    35,    -1,    37,    38,
      -1,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    -1,    76,    77,    78,
      -1,    80,    81,    82,    -1,    -1,    85,    -1,    87,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,    -1,   126,   127,    -1,
     129,    -1,    -1,   132,    -1,    -1,    -1,   136,    -1,   138,
       1,   140,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    -1,    25,    26,    -1,    28,    -1,    30,
      31,    32,    33,    34,    35,    -1,    37,    38,    -1,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    57,    58,    59,    60,
      61,    -1,    63,    64,    65,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    -1,    76,    77,    78,    -1,    80,
      81,    82,    -1,    -1,    85,    -1,    87,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,    -1,   126,   127,    -1,   129,    -1,
      -1,   132,    -1,    -1,    -1,   136,    -1,   138,     1,   140,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    -1,    25,    26,    -1,    28,    -1,    30,    31,    32,
      33,    34,    35,    -1,    37,    38,    -1,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    -1,    67,    68,    69,    70,    71,    72,
      -1,    74,    -1,    76,    77,    78,    -1,    80,    81,    82,
      -1,    -1,    85,    -1,    87,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,   124,    -1,   126,   127,    -1,   129,    -1,    -1,   132,
      -1,    -1,    -1,   136,    -1,   138,     1,   140,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    -1,
      25,    26,    -1,    28,    -1,    30,    31,    32,    33,    34,
      35,    -1,    37,    38,    -1,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      -1,    76,    77,    78,    -1,    80,    81,    82,    -1,    -1,
      85,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,
      -1,   126,   127,    -1,   129,    -1,    -1,   132,    -1,    -1,
      -1,   136,    -1,   138,     1,   140,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    -1,    25,    26,
      -1,    28,    -1,    30,    31,    32,    33,    34,    35,    -1,
      37,    38,    -1,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,
      57,    58,    59,    60,    61,    -1,    63,    64,    65,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    76,
      77,    78,    -1,    80,    81,    82,    -1,    -1,    85,    -1,
      87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
     107,   108,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,   126,
     127,    -1,   129,    -1,    -1,   132,    -1,    -1,    -1,   136,
      -1,   138,     1,   140,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    -1,    25,    26,    -1,    28,
      -1,    30,    31,    32,    33,    34,    35,    -1,    37,    38,
      -1,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    -1,    76,    77,    78,
      -1,    80,    81,    82,    -1,    -1,    85,    -1,    87,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,    -1,   126,   127,    -1,
     129,    -1,    -1,   132,    -1,    -1,    -1,   136,    -1,   138,
       1,   140,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    -1,    25,    26,    -1,    28,    -1,    30,
      31,    32,    33,    34,    35,    -1,    37,    38,    -1,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    57,    58,    59,    60,
      61,    -1,    63,    64,    65,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    -1,    76,    77,    78,    -1,    80,
      81,    82,    -1,    -1,    85,    -1,    87,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,    -1,   126,   127,    -1,   129,    -1,
      -1,   132,    -1,    -1,    -1,   136,    -1,   138,     1,   140,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    -1,    25,    26,    -1,    28,    -1,    30,    31,    32,
      33,    34,    35,    -1,    37,    38,    -1,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    -1,    67,    68,    69,    70,    71,    72,
      -1,    74,    -1,    76,    77,    78,    -1,    80,    81,    82,
      -1,    -1,    85,    -1,    87,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,   124,    -1,   126,   127,    -1,   129,    -1,    -1,   132,
      -1,    -1,    -1,   136,    -1,   138,     1,   140,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    -1,
      25,    26,    -1,    28,    -1,    30,    31,    32,    33,    34,
      35,    -1,    37,    38,    -1,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      -1,    76,    77,    78,    -1,    80,    81,    82,    -1,    -1,
      85,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,
      -1,   126,   127,    -1,   129,    -1,    -1,   132,    -1,    -1,
      -1,   136,    -1,   138,     1,   140,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    -1,    25,    26,
      -1,    28,    -1,    30,    31,    32,    33,    34,    35,    -1,
      37,    38,    -1,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,
      57,    58,    59,    60,    61,    -1,    63,    64,    65,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    76,
      77,    78,    -1,    80,    81,    82,    -1,    -1,    85,    -1,
      87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
     107,   108,    -1,    -1,    -1,    -1,   113,     1,    -1,     3,
       4,     5,     6,     7,     8,   122,    10,   124,    12,   126,
     127,    15,   129,    -1,    -1,   132,    -1,    -1,    -1,   136,
      24,   138,    26,   140,    -1,    -1,    -1,    -1,    32,    33,
      -1,    35,    -1,    37,    -1,    -1,    -1,    -1,    42,    43,
      -1,    45,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    66,    -1,    -1,    69,    70,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,   136,    -1,   138,     1,   140,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    35,
      -1,    37,    -1,    -1,    -1,    -1,    42,    43,    -1,    45,
      -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,    -1,    -1,    -1,     1,   113,     3,     4,
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
     124,    -1,   126,   127,   128,    -1,    -1,    -1,   132,    -1,
      -1,    -1,   136,    26,   138,    -1,   140,   141,    -1,    32,
      33,    -1,    35,    -1,    37,    -1,    -1,    -1,    -1,    42,
      43,    -1,    45,    -1,    47,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,    -1,
     113,     3,     4,     5,     6,     7,     8,    -1,    -1,   122,
      12,   124,    -1,   126,   127,   128,    -1,    -1,    -1,   132,
      -1,    -1,    -1,   136,    26,   138,    -1,   140,   141,    -1,
      32,    33,    -1,    35,    -1,    37,    -1,    -1,    -1,    -1,
      42,    43,    -1,    45,    -1,    47,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,
      -1,   113,    -1,    -1,     3,     4,     5,     6,     7,     8,
     122,    -1,   124,    12,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    21,    -1,   136,    -1,   138,    26,   140,   141,
      -1,    -1,    -1,    32,    33,    -1,    35,    36,    37,    -1,
      39,    -1,    -1,    42,    43,    -1,    45,    -1,    47,    48,
      -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,    78,
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
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,    -1,    -1,    -1,    -1,   113,   114,    -1,     3,     4,
       5,     6,     7,     8,   122,    -1,   124,    12,   126,   127,
     128,    -1,    -1,    -1,   132,    -1,    -1,    -1,   136,    -1,
     138,    26,   140,    -1,    -1,    -1,    -1,    32,    33,    -1,
      35,    -1,    37,    -1,    -1,    -1,    -1,    42,    43,    -1,
      45,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,     3,
       4,     5,     6,     7,     8,    -1,    -1,   122,    12,   124,
      -1,   126,   127,   128,    -1,    -1,    -1,   132,    -1,    -1,
      -1,   136,    26,   138,   139,   140,    -1,    -1,    32,    33,
      -1,    35,    -1,    37,    -1,    -1,    -1,    -1,    42,    43,
      -1,    45,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,   107,   108,    -1,    -1,    -1,    -1,   113,
       3,     4,     5,     6,     7,     8,    -1,    -1,   122,    12,
     124,    -1,   126,   127,   128,    -1,    -1,    -1,   132,    -1,
      -1,    -1,   136,    26,   138,   139,   140,    -1,    -1,    32,
      33,    -1,    35,    -1,    37,    -1,    -1,    -1,    -1,    42,
      43,    -1,    45,    -1,    47,    48,    -1,    -1,    -1,    -1,
      -1,    54,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,   107,   108,    -1,    -1,    -1,    -1,
     113,     3,     4,     5,     6,     7,     8,    -1,    -1,   122,
      12,   124,    -1,   126,   127,    -1,    -1,    -1,    20,   132,
      -1,    -1,    -1,   136,    26,   138,    -1,   140,    -1,    31,
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
      -1,    -1,   113,    -1,    -1,     3,     4,     5,     6,     7,
       8,   122,    -1,   124,    12,   126,   127,   128,    -1,    -1,
      -1,   132,    -1,    -1,    -1,   136,   137,   138,    26,   140,
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,    37,
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
      -1,    -1,    69,    70,    71,    72,    -1,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
     107,   108,    -1,    -1,    -1,    -1,   113,    -1,    -1,   133,
     134,    -1,    -1,    -1,   138,   122,   140,   124,    -1,   126,
     127,    -1,   129,    -1,    -1,   132,    -1,    -1,    -1,   136,
      -1,   138,    -1,   140,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    35,    -1,    37,    -1,
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
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,    -1,    -1,    -1,    -1,   113,     3,     4,     5,     6,
       7,     8,    -1,    -1,   122,    12,   124,    -1,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,   136,    26,
     138,    -1,   140,    -1,    -1,    32,    33,    -1,    35,    -1,
      37,    -1,    -1,    -1,    -1,    42,    43,    -1,    45,    -1,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,
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
     126,   127,   128,    -1,    -1,    -1,   132,    -1,    -1,    -1,
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
      -1,   126,   127,   128,    -1,    -1,    -1,   132,    -1,    -1,
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
     124,    -1,   126,   127,   128,    -1,    -1,    -1,   132,    -1,
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
      72,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,   105,   106,   107,   108,    -1,    32,    33,
      62,    35,    -1,    37,    66,    -1,    -1,    -1,    42,    43,
     122,    45,   124,    47,    48,   127,    -1,    -1,    -1,    -1,
     132,    55,     3,    57,   136,    -1,   138,    -1,   140,    10,
      -1,    -1,    -1,    -1,    15,    69,    70,    71,    72,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,    91,   120,   121,
     122,   123,    -1,   125,    -1,   127,    -1,    -1,   130,   131,
     132,    -1,   106,    -1,    -1,    -1,   110,    10,    -1,    -1,
     114,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    24,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,   140,    -1,    -1,    -1,
      91,    92,    93,    94,    95,    96,    -1,    -1,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    -1,    62,
     111,    -1,    -1,    66,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,    -1,    -1,   130,
     131,   132,   133,    86,   135,    10,    -1,    90,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    -1,   125,   126,   127,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,   141,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    62,
     125,   126,   127,    66,    -1,   130,   131,   132,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,    -1,   130,   131,   132,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    10,
      -1,    90,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    25,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    36,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    -1,   125,   126,   127,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,   136,    -1,    -1,
      -1,    62,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,    -1,   130,
     131,   132,    -1,    -1,    -1,   136,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    90,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    -1,   125,   126,   127,    -1,
      62,   130,   131,   132,    66,    -1,    -1,   136,    -1,    -1,
      10,    73,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    25,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    62,   125,   126,   127,    66,    -1,   130,   131,
     132,    -1,    10,    -1,   136,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    62,   125,   126,   127,    66,    -1,
     130,   131,   132,    -1,    10,    -1,   136,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    62,   125,   126,   127,
      66,    -1,   130,   131,   132,    -1,    10,    73,   136,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    62,   125,
     126,   127,    66,    -1,   130,   131,   132,    -1,    10,    73,
     136,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      62,   125,   126,   127,    66,    -1,   130,   131,   132,    -1,
      10,    73,   136,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    62,   125,   126,   127,    66,    -1,   130,   131,
     132,    -1,    10,    73,   136,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    62,   125,   126,   127,    66,    -1,
     130,   131,   132,    -1,    10,    73,   136,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    62,   125,   126,   127,
      66,    -1,   130,   131,   132,    -1,    10,    73,   136,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    62,   125,
     126,   127,    66,    -1,   130,   131,   132,    -1,    10,    73,
     136,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      62,   125,   126,   127,    66,    -1,   130,   131,   132,    -1,
      10,    73,   136,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    62,   125,   126,   127,    66,    -1,   130,   131,
     132,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      90,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,   105,    29,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,   125,   126,   127,    51,    -1,
     130,   131,   132,    -1,    -1,   135,    -1,    -1,    -1,    62,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,   129,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    86,    -1,    15,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    29,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    51,   125,   126,   127,    -1,    -1,   130,
     131,   132,    -1,    -1,    62,    -1,    -1,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    62,   125,   126,   127,
      66,    -1,   130,   131,   132,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    25,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    62,   125,
     126,   127,    66,    -1,   130,   131,   132,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      62,   125,   126,   127,    66,    -1,   130,   131,   132,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    62,   125,   126,   127,    66,   129,   130,   131,
     132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    62,   125,   126,   127,    66,   129,
     130,   131,   132,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    62,   125,   126,   127,
      66,   129,   130,   131,   132,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    62,   125,
     126,   127,    66,   129,   130,   131,   132,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      62,   125,   126,   127,    66,   129,   130,   131,   132,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    62,   125,   126,   127,    66,   129,   130,   131,
     132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    62,   125,   126,   127,    66,    -1,
     130,   131,   132,    -1,    10,    73,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    62,   125,   126,   127,
      66,    -1,   130,   131,   132,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    62,   125,
     126,   127,    66,   129,   130,   131,   132,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      62,   125,   126,   127,    66,   129,   130,   131,   132,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    62,   125,   126,   127,    66,   129,   130,   131,
     132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    62,   125,   126,   127,    66,   129,
     130,   131,   132,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    62,   125,   126,   127,
      66,   129,   130,   131,   132,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    62,   125,
     126,   127,    66,   129,   130,   131,   132,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      62,   125,   126,   127,    66,   129,   130,   131,   132,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    62,   125,   126,   127,    66,   129,   130,   131,
     132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    62,   125,   126,   127,    66,   129,
     130,   131,   132,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    62,   125,   126,   127,
      66,   129,   130,   131,   132,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    62,   125,
     126,   127,    66,   129,   130,   131,   132,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      62,   125,   126,   127,    66,   129,   130,   131,   132,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    62,   125,   126,   127,    66,   129,   130,   131,
     132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    62,   125,   126,   127,    66,   129,
     130,   131,   132,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    62,   125,   126,   127,
      66,   129,   130,   131,   132,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    62,   125,
     126,   127,    66,   129,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    90,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      -1,   125,   126,   127,    -1,    62,   130,   131,   132,    66,
      -1,    -1,    -1,    -1,    -1,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,    62,   125,   126,
     127,    66,   129,   130,   131,   132,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    27,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    62,
     125,   126,   127,    66,    -1,   130,   131,   132,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    62,   125,   126,   127,    66,    -1,   130,   131,   132,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    62,   125,   126,   127,    66,    -1,   130,
     131,   132,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    62,   125,   126,   127,    66,
      -1,   130,   131,   132,    -1,    10,    -1,    -1,    -1,    -1,
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
     125,   126,   127,    66,    -1,   130,   131,   132,    -1,    -1,
      73,    -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    -1,   125,   126,   127,    -1,    62,   130,   131,   132,
      66,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    62,   125,
     126,   127,    66,    -1,   130,   131,   132,    -1,    10,    73,
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
     132,    -1,    10,    73,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    62,   125,   126,   127,    66,    -1,
     130,   131,   132,    -1,    10,    73,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    62,   125,   126,   127,
      66,    -1,   130,   131,   132,    -1,    10,    73,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    62,   125,
     126,   127,    66,    -1,   130,   131,   132,    -1,    10,    73,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      62,   125,   126,   127,    66,    -1,   130,   131,   132,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    62,   125,   126,   127,    66,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,   125,   126,   127,    -1,    -1,
     130,   131,   132
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
     137,   180,   139,   162,   176,   231,   216,   129,   129,   141,
     246,   141,   246,   150,    89,   137,   154,   184,    63,   194,
      10,    15,    91,    92,    93,    94,    95,    96,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   111,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     127,   130,   131,   132,   133,   135,   138,   162,   196,   197,
     198,   237,   129,   248,   237,   237,   252,    25,    25,   252,
      25,    25,   141,   141,   164,   164,   252,   187,   136,   164,
      73,    35,   252,    35,   252,    73,    35,   252,   164,    35,
     252,   164,   110,   139,   252,   252,   150,   252,   202,   223,
     109,   230,    63,   226,   224,    36,   141,    25,   150,   231,
     109,   153,   141,    35,   150,   252,   141,    35,   150,   252,
     141,   252,   137,    31,    58,    60,   137,   149,   166,   207,
     162,   252,   112,   138,   199,   199,    73,    35,    35,    73,
      35,    35,    35,    35,   184,   252,   252,   252,   252,   252,
     252,   248,    11,   139,    26,    69,    72,   140,   211,   229,
     238,   114,   213,   224,    84,   214,   252,   227,   238,   150,
     164,   252,   252,   150,   252,   150,   166,   226,   139,   162,
     197,   198,   201,   252,   252,   252,   252,   252,   252,   137,
      73,    73,    73,    73,   252,   230,   141,   231,   211,   212,
     252,   252,   153,   165,   210,   141,    73,    73,    75,   208,
     199,   199,   139,    73,    73,    73,    73,    73,    73,   252,
     252,   252,   252,   213,   224,   211,   228,   229,   238,    36,
     141,   238,   252,   252,   214,   252,   228,   229,   129,   209,
     210,   141,   228
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
     173,   174,   174,   175,   176,   176,   176,   177,   178,   179,
     179,   180,   180,   180,   181,   181,   182,   182,   182,   183,
     183,   184,   184,   184,   185,   186,   186,   186,   187,   187,
     189,   190,   188,   191,   191,   191,   191,   193,   194,   192,
     195,   195,   195,   195,   196,   196,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   199,   199,   200,   201,   201,   201,
     202,   202,   202,   202,   203,   203,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   205,   205,   205,   205,   205,
     205,   206,   206,   207,   207,   207,   207,   207,   207,   208,
     208,   209,   209,   210,   210,   211,   211,   212,   212,   213,
     213,   214,   214,   215,   215,   215,   216,   216,   217,   217,
     217,   218,   218,   218,   218,   218,   219,   219,   219,   220,
     220,   221,   221,   221,   222,   222,   222,   223,   223,   223,
     224,   224,   224,   225,   225,   226,   226,   226,   226,   227,
     227,   227,   227,   228,   228,   228,   229,   229,   229,   229,
     229,   230,   230,   230,   230,   230,   230,   230,   231,   231,
     231,   231,   232,   232,   232,   233,   233,   234,   234,   235,
     235,   236,   236,   236,   236,   236,   237,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   240,   241,   242,   242,
     242,   242,   242,   242,   242,   242,   243,   243,   244,   245,
     245,   246,   247,   247,   248,   248,   248,   249,   249,   250,
     251,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   253,   253,   253,   253,   254,
     254,   255,   255,   255,   256,   256,   256,   257,   257,   257,
     257,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   259,   259,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   261,   261,   261,
     261,   261,   261,   262,   262,   262,   262,   263,   263,   263,
     263,   264,   264,   264,   264,   264,   264,   264
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
       3,     0,     2,     3,     0,     1,     3,     3,     5,     0,
       2,     3,     2,     3,     6,     8,     1,     1,     1,     0,
       2,     0,     2,     3,     5,     1,     2,     3,     1,     3,
       0,     0,     8,     0,     1,     2,     2,     0,     0,    10,
       3,     3,     5,     5,     1,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     3,     0,     1,     3,
       4,     4,     6,     6,     0,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     0,     1,     2,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     2,     1,     0,     1,     2,
       2,     0,     2,     3,     4,     4,     2,     4,     0,     2,
       2,     4,     4,     4,     5,     4,     0,     1,     1,     1,
       3,     3,     4,     5,     1,     1,     3,     1,     2,     3,
       0,     2,     2,     0,     4,     0,     2,     2,     1,     4,
       4,     6,     1,     0,     1,     1,     3,     4,     3,     4,
       6,     0,     2,     2,     2,     2,     2,     2,     1,     1,
       3,     3,     1,     1,     1,     3,     3,     0,     1,     1,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     4,     4,     4,     5,     2,     2,     6,
       6,     4,     9,     9,     7,     6,     6,     4,     9,     9,
       7,     4,     6,     6,     9,     9,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     1,     4,     1,
       3,     4,     1,     3,     2,     3,     3,     1,     3,     2,
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
#line 434 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 4872 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 439 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4878 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 440 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4884 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 447 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4890 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 452 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4896 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 453 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4902 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4908 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 472 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4914 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 473 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4920 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 474 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4926 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 475 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4932 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4938 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 477 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4944 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4950 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4956 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 480 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4962 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4968 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4974 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 483 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4980 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4986 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 485 "chapel.ypp" /* yacc.c:1661  */
    { printf("syntax error"); clean_exit(1); }
#line 4992 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 490 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yyvsp[-4].b), (yylsp[-4]).comment))); }
#line 4998 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-5].b), (yylsp[-5]).comment))); }
#line 5004 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5010 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5016 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5022 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 513 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5028 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5034 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5040 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 520 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5046 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 525 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5056 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5066 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 535 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5075 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 539 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5084 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 545 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5094 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5104 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5110 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5116 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5122 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5128 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 5134 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 5140 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 5146 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 5152 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 5158 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 5164 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 5170 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 580 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 5176 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 5182 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 582 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 5188 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 583 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 5194 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 584 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 5200 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 585 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5206 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5212 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5218 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5224 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5230 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 597 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5236 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 601 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5242 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5248 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5254 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5260 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 611 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 5266 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5278 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5284 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 619 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5290 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 5296 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5302 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5308 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 5314 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 5330 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 654 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5336 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 655 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5342 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 657 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 658 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 659 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 660 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 661 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 662 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 663 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 664 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 665 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt)); }
#line 5402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 666 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), zipToTuple((yyvsp[-1].pcallexpr)), NULL, (yyvsp[0].pblockstmt), true); }
#line 5408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 667 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt), true); }
#line 5414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 668 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 669 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt)); }
#line 5426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 671 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5436 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 677 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5446 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 683 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 689 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-3].pcallexpr)), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5466 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 695 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5477 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 702 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 711 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 715 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 716 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 717 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5512 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 718 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 722 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 5524 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 723 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 5530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 724 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 5536 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 725 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 5542 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 726 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 727 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5554 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 731 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 732 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5566 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 736 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5572 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 742 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(newTemp(dtVoid)); }
#line 5578 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 743 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 5584 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 744 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 5590 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 5596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5602 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 756 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5608 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 757 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5614 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 762 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5620 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5626 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 766 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5632 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 773 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), NULL, (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5641 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 778 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-6].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5650 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 785 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 790 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 795 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5680 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 803 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 5686 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 804 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 5692 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 809 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5698 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 811 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 5704 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 813 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 5710 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 818 "chapel.ypp" /* yacc.c:1661  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 5721 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 828 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[0].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[0].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
#line 5734 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 837 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 5742 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 841 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 5751 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 848 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 5757 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 849 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 5763 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 854 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 5772 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 859 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5781 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 864 "chapel.ypp" /* yacc.c:1661  */
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
#line 5801 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 884 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5812 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 890 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5824 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 897 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5837 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 905 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5850 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 916 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 5860 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 922 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5871 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 929 "chapel.ypp" /* yacc.c:1661  */
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
#line 5894 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 951 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5902 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 955 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5911 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 960 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 5919 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 964 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5928 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 972 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 5934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 977 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5940 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 978 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 5946 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 979 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 5952 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 980 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 5958 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 981 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 5964 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 982 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 5970 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 983 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 5976 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 984 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 5982 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 985 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 5988 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 986 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 5994 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 987 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 6000 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 988 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 6006 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 989 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 6012 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 990 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 6018 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 991 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 6024 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 992 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 6030 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 993 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 6036 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 994 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 6042 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 995 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 6048 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 996 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 6054 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 997 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 6060 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 998 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 6066 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 999 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 6072 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1000 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 6078 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1001 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 6084 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1005 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 6090 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1006 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 6096 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1007 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 6102 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1008 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 6108 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1009 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 6114 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1010 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 6120 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1011 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 6126 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1012 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 6132 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1013 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 6138 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1014 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 6144 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1015 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 6150 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1016 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 6156 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1020 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 6162 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1021 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6168 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1025 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6174 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1029 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 6180 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1030 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 6186 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1031 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 6192 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1036 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 6198 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1038 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 6204 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1040 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6210 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1042 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6216 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1046 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1047 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1051 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 6234 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1052 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6240 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1053 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 6246 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1054 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 6252 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1055 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6258 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1056 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6264 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1057 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6270 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1058 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 6276 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1059 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6282 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1063 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6288 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1064 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6294 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1065 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 6300 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1066 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 6306 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1067 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST;   }
#line 6312 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1068 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6318 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1072 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6324 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1073 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6330 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1077 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6336 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1078 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6342 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1079 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1080 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 6354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1081 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 6360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1082 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 6366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1086 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 6372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1087 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 6378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1090 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 6384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1096 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 6390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1101 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1103 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1107 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1112 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1113 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1117 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1118 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1123 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 6438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1125 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 6444 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1127 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 6450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1132 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 6467 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1145 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 6485 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1161 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6491 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1163 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6497 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1165 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 6503 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1170 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6515 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1178 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6527 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1186 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6539 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 6552 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1203 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6563 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1212 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6569 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1213 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6575 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1214 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6581 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1220 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6590 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1228 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1230 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 6606 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1236 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6612 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1241 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 6618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1243 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1245 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 6630 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1250 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 6636 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1252 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6642 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1254 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1260 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6654 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1261 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 6660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1262 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1266 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6672 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1268 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1272 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6684 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1273 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1274 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 6696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1275 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad type specification"); }
#line 6702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1296 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 6710 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1300 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 6718 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1304 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 6730 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1311 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad array type specification"); clean_exit(1); }
#line 6736 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1315 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6742 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1316 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6748 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1317 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6754 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1322 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6760 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1324 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6766 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1330 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6772 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1332 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6778 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1334 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 6784 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1338 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6790 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1339 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6796 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1340 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6802 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1341 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 6808 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1342 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6814 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1343 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6820 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1344 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6826 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1350 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6832 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1351 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6838 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1352 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6844 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1353 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6850 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1357 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6856 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6862 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1359 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6868 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1363 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6874 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1364 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6880 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1368 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6886 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1373 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6892 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1374 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6898 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1378 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6904 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1379 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6910 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1380 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6916 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1381 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6922 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1386 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6928 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1403 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1405 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6940 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1407 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6946 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1409 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6952 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1411 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 6958 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1413 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6964 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1415 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6970 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1420 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6976 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1422 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 6982 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1424 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6988 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1426 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6994 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1428 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7000 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1430 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7006 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1432 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7012 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1434 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 7018 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1436 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7024 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1438 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7030 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1440 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7036 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1442 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7042 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1444 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 7053 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1451 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 7063 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1457 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 7073 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1463 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7083 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1469 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 7093 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 7099 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 7105 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7111 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1502 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 7117 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1507 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7123 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1513 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7130 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1516 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7136 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1520 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pForallIntents) = (yyvsp[-1].pForallIntents); }
#line 7142 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1526 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pForallIntents) = new ForallIntents();
      addForallIntent((yyval.pForallIntents), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7149 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1529 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pForallIntents), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7155 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1534 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[-1].pt); (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
#line 7163 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).riExp = (yyvsp[-2].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); }
#line 7169 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 386:
#line 1538 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).riExp = (yyvsp[-2].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); }
#line 7175 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 388:
#line 1544 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7181 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1549 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 7187 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 7193 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1570 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 7199 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7205 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7211 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 7217 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 7223 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1580 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 7229 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1603 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7235 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7241 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1605 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 7247 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1609 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7253 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 7259 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1611 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 7265 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1619 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7271 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 7277 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 7283 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 7289 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 7295 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 7301 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1628 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 7307 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 7313 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1630 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 7319 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 7325 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 7331 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 7337 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 7343 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1636 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 7351 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1640 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 7359 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7365 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1648 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7371 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1652 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7377 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1653 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7383 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1654 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7389 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1655 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7395 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7401 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1657 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7407 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1658 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7413 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1659 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7419 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1660 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7425 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1661 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7431 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1662 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7437 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1663 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7443 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1664 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7449 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1665 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7455 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1666 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7461 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1667 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7467 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1668 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7473 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1669 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7479 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1670 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7485 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 453:
#line 1671 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7491 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 454:
#line 1672 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7497 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 455:
#line 1673 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7503 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 456:
#line 1674 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7509 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 457:
#line 1678 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 7515 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 458:
#line 1679 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 7521 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 459:
#line 1680 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 7527 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 460:
#line 1681 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 7533 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 461:
#line 1682 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 7539 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 462:
#line 1683 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 7545 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 463:
#line 1687 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7551 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 464:
#line 1688 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7557 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 465:
#line 1689 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7563 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 466:
#line 1690 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7569 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 467:
#line 1694 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7575 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 468:
#line 1695 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7581 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 469:
#line 1696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7587 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 470:
#line 1697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7593 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 471:
#line 1702 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 7599 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 472:
#line 1703 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 7605 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 473:
#line 1704 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 7611 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 474:
#line 1705 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 7617 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 475:
#line 1706 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 7623 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 476:
#line 1707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 7629 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 477:
#line 1708 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 7635 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 7639 "bison-chapel.cpp" /* yacc.c:1661  */
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
