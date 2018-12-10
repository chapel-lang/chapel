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
    const char*               pch;

    Vec<const char*>*         vpch;
    RetTag                    retTag;
    AggregateTag              aggrTag;
    bool                      b;
    IntentTag                 pt;
    Expr*                     pexpr;
    DefExpr*                  pdefexpr;
    CallExpr*                 pcallexpr;
    BlockStmt*                pblockstmt;
    Type*                     ptype;
    EnumType*                 penumtype;
    std::vector<DefExpr*>*    pvecOfDefs;
    FnSymbol*                 pfnsymbol;
    ModuleSymbol*             pmodsymbol;
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<OnlyRename*>* ponlylist;
    std::set<Flag>*           pflagset;
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
    const char* prevModule;
  };

  #define YYLTYPE_IS_DECLARED 1
  #define YYLTYPE_IS_TRIVIAL  1

  #endif
#line 155 "chapel.ypp" /* yacc.c:355  */

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

#line 225 "bison-chapel.cpp" /* yacc.c:355  */

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
    TBOOL = 270,
    TBORROWED = 271,
    TBY = 272,
    TCATCH = 273,
    TCLASS = 274,
    TCOBEGIN = 275,
    TCOFORALL = 276,
    TCOMPLEX = 277,
    TCONFIG = 278,
    TCONST = 279,
    TCONTINUE = 280,
    TDEFER = 281,
    TDELETE = 282,
    TDMAPPED = 283,
    TDO = 284,
    TDOMAIN = 285,
    TELSE = 286,
    TENUM = 287,
    TEXCEPT = 288,
    TEXPORT = 289,
    TEXTERN = 290,
    TFALSE = 291,
    TFOR = 292,
    TFORALL = 293,
    TFORWARDING = 294,
    TIF = 295,
    TIMAG = 296,
    TIN = 297,
    TINDEX = 298,
    TINLINE = 299,
    TINOUT = 300,
    TINT = 301,
    TITER = 302,
    TLABEL = 303,
    TLAMBDA = 304,
    TLET = 305,
    TLOCAL = 306,
    TLOCALE = 307,
    TMINUSMINUS = 308,
    TMODULE = 309,
    TNEW = 310,
    TNIL = 311,
    TNOINIT = 312,
    TON = 313,
    TONLY = 314,
    TOTHERWISE = 315,
    TOUT = 316,
    TOVERRIDE = 317,
    TOWNED = 318,
    TPARAM = 319,
    TPLUSPLUS = 320,
    TPRAGMA = 321,
    TPRIMITIVE = 322,
    TPRIVATE = 323,
    TPROC = 324,
    TPROTOTYPE = 325,
    TPUBLIC = 326,
    TREAL = 327,
    TRECORD = 328,
    TREDUCE = 329,
    TREF = 330,
    TREQUIRE = 331,
    TRETURN = 332,
    TSCAN = 333,
    TSELECT = 334,
    TSERIAL = 335,
    TSHARED = 336,
    TSINGLE = 337,
    TSPARSE = 338,
    TSTRING = 339,
    TSUBDOMAIN = 340,
    TSYNC = 341,
    TTHEN = 342,
    TTHIS = 343,
    TTHROW = 344,
    TTHROWS = 345,
    TTRUE = 346,
    TTRY = 347,
    TTRYBANG = 348,
    TTYPE = 349,
    TUINT = 350,
    TUNDERSCORE = 351,
    TUNION = 352,
    TUNMANAGED = 353,
    TUSE = 354,
    TVAR = 355,
    TWHEN = 356,
    TWHERE = 357,
    TWHILE = 358,
    TWITH = 359,
    TYIELD = 360,
    TZIP = 361,
    TALIAS = 362,
    TAND = 363,
    TASSIGN = 364,
    TASSIGNBAND = 365,
    TASSIGNBOR = 366,
    TASSIGNBXOR = 367,
    TASSIGNDIVIDE = 368,
    TASSIGNEXP = 369,
    TASSIGNLAND = 370,
    TASSIGNLOR = 371,
    TASSIGNMINUS = 372,
    TASSIGNMOD = 373,
    TASSIGNMULTIPLY = 374,
    TASSIGNPLUS = 375,
    TASSIGNREDUCE = 376,
    TASSIGNSL = 377,
    TASSIGNSR = 378,
    TBAND = 379,
    TBNOT = 380,
    TBOR = 381,
    TBXOR = 382,
    TCOLON = 383,
    TCOMMA = 384,
    TDIVIDE = 385,
    TDOT = 386,
    TDOTDOT = 387,
    TDOTDOTDOT = 388,
    TEQUAL = 389,
    TEXP = 390,
    TGREATER = 391,
    TGREATEREQUAL = 392,
    THASH = 393,
    TIO = 394,
    TLESS = 395,
    TLESSEQUAL = 396,
    TMINUS = 397,
    TMOD = 398,
    TNOT = 399,
    TNOTEQUAL = 400,
    TOR = 401,
    TPLUS = 402,
    TQUESTION = 403,
    TSEMI = 404,
    TSHIFTLEFT = 405,
    TSHIFTRIGHT = 406,
    TSTAR = 407,
    TSWAP = 408,
    TLCBR = 409,
    TRCBR = 410,
    TLP = 411,
    TRP = 412,
    TLSBR = 413,
    TRSBR = 414,
    TNOELSE = 415,
    TUPLUS = 416,
    TUMINUS = 417
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
#line 186 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 433 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 439 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 38 "chapel.ypp" /* yacc.c:359  */

  #include <string>
  int         captureTokens;
  std::string captureString;
#line 194 "chapel.ypp" /* yacc.c:359  */

  #include "build.h"
  #include "CatchStmt.h"
  #include "DeferStmt.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForallStmt.h"
  #include "ForLoop.h"
  #include "IfExpr.h"
  #include "misc.h"
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

    // like USR_FATAL_CONT
    setupError("parser", __FILE__, __LINE__, 3);

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
  }

#line 513 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   15755

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  163
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  135
/* YYNRULES -- Number of rules.  */
#define YYNRULES  561
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1002

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   417

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
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   471,   471,   476,   477,   483,   484,   489,   490,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     516,   517,   518,   519,   520,   521,   522,   523,   524,   528,
     541,   546,   551,   559,   560,   561,   565,   566,   579,   580,
     581,   586,   587,   592,   597,   602,   606,   613,   618,   622,
     627,   631,   632,   633,   637,   641,   643,   645,   647,   649,
     656,   657,   661,   662,   663,   664,   665,   668,   669,   670,
     671,   672,   684,   685,   696,   697,   698,   699,   700,   701,
     702,   703,   704,   705,   706,   707,   708,   709,   710,   711,
     715,   716,   717,   718,   719,   720,   721,   722,   729,   730,
     731,   732,   733,   734,   735,   736,   741,   742,   746,   747,
     751,   752,   756,   757,   758,   759,   760,   761,   762,   763,
     767,   768,   772,   773,   774,   775,   779,   786,   787,   788,
     789,   790,   791,   792,   793,   794,   795,   796,   797,   798,
     799,   800,   801,   802,   808,   814,   820,   826,   833,   843,
     847,   848,   849,   850,   851,   853,   855,   857,   862,   865,
     866,   867,   868,   869,   870,   874,   875,   879,   880,   881,
     885,   886,   890,   893,   895,   900,   901,   905,   907,   909,
     916,   926,   936,   946,   959,   964,   969,   977,   978,   983,
     984,   986,   991,  1007,  1014,  1023,  1031,  1035,  1042,  1043,
    1048,  1053,  1047,  1078,  1084,  1091,  1097,  1103,  1114,  1120,
    1113,  1152,  1156,  1161,  1165,  1170,  1177,  1178,  1182,  1183,
    1184,  1185,  1186,  1187,  1188,  1189,  1190,  1191,  1192,  1193,
    1194,  1195,  1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,
    1204,  1205,  1206,  1207,  1211,  1212,  1213,  1214,  1215,  1216,
    1217,  1218,  1219,  1220,  1221,  1222,  1226,  1227,  1231,  1235,
    1236,  1237,  1241,  1243,  1245,  1247,  1249,  1254,  1255,  1259,
    1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1271,  1272,
    1273,  1274,  1275,  1276,  1280,  1281,  1285,  1286,  1287,  1288,
    1289,  1290,  1294,  1295,  1298,  1299,  1303,  1304,  1308,  1310,
    1315,  1316,  1320,  1321,  1325,  1326,  1330,  1332,  1334,  1339,
    1352,  1369,  1370,  1372,  1377,  1378,  1379,  1380,  1381,  1385,
    1391,  1397,  1405,  1406,  1414,  1416,  1421,  1423,  1425,  1430,
    1432,  1434,  1441,  1442,  1443,  1448,  1450,  1452,  1456,  1460,
    1462,  1466,  1474,  1475,  1476,  1477,  1478,  1483,  1484,  1485,
    1486,  1487,  1507,  1511,  1515,  1523,  1530,  1531,  1532,  1536,
    1538,  1544,  1546,  1548,  1553,  1554,  1555,  1556,  1557,  1563,
    1564,  1565,  1566,  1570,  1571,  1575,  1576,  1577,  1581,  1582,
    1586,  1587,  1591,  1592,  1596,  1597,  1598,  1599,  1603,  1604,
    1615,  1616,  1617,  1618,  1619,  1620,  1622,  1624,  1626,  1628,
    1630,  1632,  1634,  1636,  1638,  1640,  1645,  1647,  1649,  1651,
    1653,  1655,  1657,  1659,  1661,  1663,  1665,  1667,  1669,  1676,
    1682,  1688,  1694,  1703,  1713,  1721,  1722,  1723,  1724,  1725,
    1726,  1727,  1728,  1733,  1734,  1738,  1742,  1743,  1747,  1751,
    1752,  1756,  1760,  1764,  1771,  1772,  1773,  1774,  1775,  1776,
    1780,  1781,  1786,  1791,  1799,  1800,  1801,  1802,  1803,  1804,
    1805,  1806,  1807,  1809,  1811,  1813,  1815,  1817,  1819,  1821,
    1826,  1827,  1830,  1831,  1832,  1835,  1836,  1837,  1838,  1849,
    1850,  1854,  1855,  1856,  1860,  1861,  1862,  1863,  1871,  1872,
    1873,  1874,  1878,  1879,  1883,  1884,  1885,  1886,  1887,  1888,
    1889,  1890,  1891,  1892,  1893,  1897,  1905,  1906,  1910,  1911,
    1912,  1913,  1914,  1915,  1916,  1917,  1918,  1919,  1920,  1921,
    1922,  1923,  1924,  1925,  1926,  1927,  1928,  1929,  1930,  1931,
    1932,  1936,  1937,  1938,  1939,  1940,  1941,  1945,  1946,  1947,
    1948,  1952,  1953,  1954,  1955,  1960,  1961,  1962,  1963,  1964,
    1965,  1966
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "CSTRINGLITERAL", "EXTERNCODE", "TALIGN",
  "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBOOL", "TBORROWED", "TBY",
  "TCATCH", "TCLASS", "TCOBEGIN", "TCOFORALL", "TCOMPLEX", "TCONFIG",
  "TCONST", "TCONTINUE", "TDEFER", "TDELETE", "TDMAPPED", "TDO", "TDOMAIN",
  "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE", "TFOR",
  "TFORALL", "TFORWARDING", "TIF", "TIMAG", "TIN", "TINDEX", "TINLINE",
  "TINOUT", "TINT", "TITER", "TLABEL", "TLAMBDA", "TLET", "TLOCAL",
  "TLOCALE", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TON",
  "TONLY", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM",
  "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE",
  "TPUBLIC", "TREAL", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN",
  "TSCAN", "TSELECT", "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE",
  "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS",
  "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE", "TUNION",
  "TUNMANAGED", "TUSE", "TVAR", "TWHEN", "TWHERE", "TWHILE", "TWITH",
  "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND",
  "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP",
  "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL",
  "TASSIGNSR", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA",
  "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER",
  "TGREATEREQUAL", "THASH", "TIO", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD",
  "TNOT", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT",
  "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR",
  "TRSBR", "TNOELSE", "TUPLUS", "TUMINUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_start", "module_decl_stmt", "access_control",
  "opt_prototype", "block_stmt", "stmt_ls", "only_ls", "opt_only_ls",
  "except_ls", "use_stmt", "require_stmt", "assignment_stmt",
  "opt_label_ident", "ident_fn_def", "ident_def", "ident_use",
  "internal_type_ident_def", "scalar_type", "reserved_type_ident_use",
  "opt_string", "do_stmt", "return_stmt", "class_level_stmt",
  "private_decl", "forwarding_stmt", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@1", "$@2", "linkage_spec",
  "fn_decl_stmt", "$@3", "$@4", "fn_decl_stmt_inner",
  "fn_decl_receiver_expr", "fn_ident", "assignop_ident", "opt_formal_ls",
  "req_formal_ls", "formal_ls", "formal", "opt_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_or_iter",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "opt_query_expr", "var_arg_expr",
  "opt_where_part", "type_alias_decl_stmt", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_stmt",
  "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "ret_array_type", "opt_ret_type", "opt_type",
  "array_type", "opt_formal_array_elt_type", "formal_array_type",
  "opt_formal_type", "expr_ls", "simple_expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "for_expr", "cond_expr", "nil_expr",
  "stmt_level_expr", "opt_task_intent_ls", "task_intent_clause",
  "task_intent_ls", "forall_intent_clause", "forall_intent_ls",
  "intent_expr", "shadow_var_prefix", "io_expr", "new_expr", "let_expr",
  "expr", "opt_expr", "opt_try_expr", "lhs_expr", "fun_expr", "call_expr",
  "dot_expr", "parenthesized_expr", "bool_literal", "literal_expr",
  "assoc_expr_ls", "binary_op_expr", "unary_op_expr", "reduce_expr",
  "scan_expr", "reduce_scan_op_expr", YY_NULLPTR
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
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417
};
# endif

#define YYPACT_NINF -874

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-874)))

#define YYTABLE_NINF -513

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -874,   100,  2650,  -874,   -24,  -874,  -874,  -874,  -874,  -874,
    -874,  4057,    57,   144,  -874,  9164,  -874,    57,  9164,  -874,
     383,   105,   144,  4057,  9164,  4057,    98,  -874,  9164,  6517,
    -874,  7852,  9164,  6244,  9164,  -874,   177,  -874,  -874, 15657,
    -874,  2148,  8274,  -874,  9164,  9164,  -874,  9164,  -874,  9164,
    -874,  9164,   180,   185,  1020,  1020,  -874,  -874,  -874,  8407,
    7407,  9164,  8274,  9164,  9164,   107,  -874,   199,  4057,  -874,
    9164,  -874,  9297,  9297, 15657,  -874,  -874,  9164,  8407,  -874,
    9164,  9164,  -874,  -874,  9164,  -874,  -874, 11292,  9164,  9164,
    -874,  9164,  -874,  -874,  3121,  6650,  8407,  -874,  3901,  -874,
      74,  -874,   286,  -874,  -874,  -874,  -874,  -874,  -874,  -874,
    -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,
    -874, 15657,  -874, 15657,   343,    63,  -874,  -874,  2148,  -874,
     124,  -874,  -874,  -874,   213,   244,   258,   247,   251, 15349,
    1710,   215,   252,   261,  -874,  -874,  -874,  -874,  -874,  -874,
    -874,    61,  -874,  -874, 15349,   287,  4057,  -874,  -874,   265,
    9164,  9164,  9164,  9164,  9164,  8407,  8407,   259,  -874,  -874,
    -874,  -874,   320,   270,  -874,  -874,   299, 13604, 15657,  2148,
    -874,   308,  -874,   -35, 15349,   357,  7563, 15349,  -874,    72,
    -874, 15657,   183,   188, 15657,   311,    25, 13141, 13090,   188,
    9164,  -874, 13525, 12403,  7563,  -874,  -874,  -874,  -874,  -874,
    -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,
    -874,  -874,  -874,  -874,  -874,  4057,  -874,   314,  1367,    55,
      -7,  -874,  4057,  -874,  -874, 13279,   564,   320, 13279,   320,
     564,  -874,  7563,  9164,  -874,  -874, 15657,  -874,   113, 15349,
    9164,  9164,  -874, 15349,   315, 13455,  -874, 13279,   320, 15349,
     316,  7563,  -874, 15349, 13850,  -874,  -874, 13901, 11550,  -874,
    -874, 13988,   367,   329,   320,   147, 13674, 13279, 14039,   324,
     888,   564,   324,   564,    80,  -874,  -874,  3277,   -31,  -874,
    9164,  -874,   -41,   -36,  -874,   -23, 14126,   -57,   481,  -874,
    3433,  -874,   436,   368,   344,  -874,  -874,  -874,   202,  -874,
    9164,  9164,  9164,  9164,  8563,  8563,  9164,  9164,  9164,  9164,
    9430,  9164,   347, 11292,  9164,  9164,  9164,  9164,  9164,  9164,
    9164,  9164,  9164,  9164,  9164,  9164,  9164,  9164,  9164,  -874,
    -874,  -874,  -874,  -874,  -874,  7985,  7985,  -874,  -874,  -874,
    -874,  7985,  -874,  -874,  7985,  7985,  7563,  7563,  8563,  8563,
    7118,  -874,  -874, 13749, 13819, 14075,   -16,  3589,  -874,  8563,
      25,   359,   210,  -874,  9164,  -874,  9164,   397,  -874,   352,
     382,  -874,  -874,   366, 15657,  2148,   482,  8407,  -874,  4213,
    8563,  -874,  4369,  8563,   369,  -874,    25,   188,  9563,  9164,
    -874,  4057,   495,  9164,   378,  -874,   203,  -874,  -874,  1367,
    -874,   411,   384,  -874,  9696,   435,  9164,  2148,  -874,  -874,
    -874,   389,  -874,  8407,  -874, 15349, 15349,  -874,    43,  -874,
    7563,   395,  -874,   538,  -874,   538,  -874,  9829,   429,  -874,
    -874,  9563,  9164,  -874,  -874,  -874,  -874,  -874,  8118,  -874,
    2422,  6806,  -874,  6962,  -874,  8563,  5773,  2965,   401,  9164,
    6087,  -874,  -874,   143,  -874,  3745, 15657,  8407,   407, 15604,
     345,  -874, 14213, 15450, 15450,   239,  -874,   239,  -874,   239,
   12078,   485,  1480,  1914, 11679, 11679,   320,   324,  -874,  -874,
    -874,  -874,   888, 13349,   239,  1030,  1030, 15450,  1030,  1030,
    1446,   324, 13349, 15501,  1446,   564,   564,   324,   413,   417,
     418,   419,   424,   420,   416,  -874,   239,  -874,   239,    37,
    -874,  -874,  -874,    90,  -874,   339, 15419,   216,  9962,  8563,
   10095,  8563,  9164,  8563, 11871,    57, 14264,  -874,  -874,  -874,
   15349, 14295,  7563,  -874,  7563,  -874,   368,   235,  9164,    92,
    9164, 15349,    53, 13310,  9164, 15349,    13, 13228,  7118,  -874,
     441,   449,   448, 14365,   449,   451,   572, 14440,  4057, 13380,
    -874,    67,  -874,  -874,  -874,  -874,  -874,  -874,   885,    96,
    -874,  2356,  -874,   478,   428,  1367,    55,  9164,    98,   177,
    9164,  9164,  9164,  9164,  9164,  5617,  -874,  -874,   128,  7719,
    -874, 15349,  -874,  -874,  -874, 15349,   450,    71,   447,  -874,
    1878,  -874,  -874,   237, 15657,   458,   459,  -874,  -874,  -874,
    -874,  -874,  -874,   -40, 12038,  -874,  -874, 15349,  4057, 15349,
    -874, 14510,  -874,  -874,  -874,   483,  1493,   455,   502,   -21,
    -874,   542,  -874,  -874,  -874,  -874,  6363,  -874,  -874,  -874,
    -874,  -874,  -874,  -874,  -874,  -874,  7118,  -874,    31,  8563,
    8563,  9164,   592, 14585,  9164,   594, 14655,   467, 12108,    25,
      25,  -874,  -874,  -874,  -874,   475,  -874, 13279,  -874, 12478,
    4525,  -874,  4681,  -874, 12553,  4837,  -874,    25,  4993,  -874,
      25,   115,  -874,  9164,  -874,  9164,  -874,  4057,  9164,  -874,
    4057,   601,  -874,  -874, 15657,  1122,  -874,  1367,   505,   559,
    -874,  -874,  -874,    46,  -874,  -874,   435,   488,    -1,  -874,
    -874,  -874,  5149,  8407,  -874,  -874,  -874, 15657,  -874,   527,
     299,  -874,  -874,  -874,  5305,   498,  5461,   499,  -874,  9164,
     504,  1225,  -874,  9164, 15657,  -874,  -874,   478,   508,   529,
    -874,   539,  -874,   547,  -874,   555,   558,   565,   567,   570,
     573,  -874,   575,  -874, 15657,  -874,  -874,  -874,  -874,  -874,
    -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,
    -874,  -874,  -874,  -874,  -874,  9164,  -874,  -874,   578,   508,
     508,  -874,  -874,   435,   117,   129, 14686, 10228, 10361, 14824,
   10494, 10627, 10760, 10893,  -874,  -874,  2221,  -874,  4057,  9164,
   15349,  9164, 15349,  4057,  9164, 15349,  -874,  9164, 15349,  -874,
    7118,  -874, 14737, 15349,  -874, 15349,   642,  4057,   505,  -874,
     553,  8696,   108,  -874,   616,  -874,  -874,  8563, 11775,  4057,
    -874,    36,   563,  9164,  -874,  9164,  -874, 15349,  4057,  9164,
    -874, 15349,  4057, 15349,  -874,  7274,   470,   470,  -874,  2445,
    -874, 15349,  -874,    28,   203,  -874,  -874, 12328, 11958,  -874,
    -874,  -874,  9164,  9164,  9164,  9164,  9164,  9164,  9164,  9164,
     566, 11480, 14440, 12628, 12703, 14440, 12778, 12853,  -874,  9164,
    4057,  -874,   435,   505,  5930,  -874,  -874,  -874,   353,  8407,
    -874,  -874,  9164,    22, 12183,  -874,   554,   357,  -874,   299,
   15349, 12928,  -874, 13003,  -874,  -874,  -874, 11026,   634,   130,
    -874,  -874,  -874,   508,   508, 14899, 14974, 15049, 15124, 15199,
   15274,  -874,  -874,  4057,  4057,  4057,  4057, 15349,  -874,  -874,
     108,  8852,     4,  -874,  -874, 15349, 15349,  -874,  -874,  -874,
   10893,  -874,  4057,  4057,  2806,  -874,  -874,   331,  -874,   616,
    -874,  -874,  -874,  9164,  9164,  9164,  9164,  9164,  9164, 14440,
   14440, 14440, 14440,  -874,  -874,  -874,  -874,  -874,   543,  8563,
   11425,   588, 14440, 14440,   568, 11159,    44,    60, 12253,  -874,
    -874,  -874,  -874,   497, 11556,  -874,  -874,  -874,  9008,  -874,
     589,  -874
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   505,   506,   507,   508,
     509,     0,   443,    70,   100,     0,   194,   443,     0,   105,
       0,   325,    70,     0,     0,     0,     0,   204,   480,   480,
     502,     0,     0,     0,     0,   104,     0,   214,   101,     0,
     210,     0,     0,   107,     0,     0,   434,     0,   217,     0,
     324,     0,     0,     0,    45,    44,   103,   195,   326,     0,
       0,     0,     0,     0,     0,     0,   106,     0,     0,    83,
       0,   503,     0,     0,     0,   102,   196,     0,     0,   328,
       0,     0,   557,   559,     0,   560,   561,   479,     0,     0,
     558,   555,   122,   556,     0,     0,     0,     4,     0,     5,
       0,     9,    46,    10,    11,    12,    14,   398,   399,    22,
      13,   123,   129,    15,    17,    16,    19,    20,    21,    18,
     128,     0,   126,     0,   469,     0,   130,   127,     0,   131,
     485,   465,   466,   402,   400,     0,     0,   470,   471,     0,
     401,     0,   486,   487,   488,   504,   464,   404,   403,   467,
     468,     0,    38,    24,   410,     0,     0,   444,    71,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   485,   400,
     470,   471,   412,   401,   486,   487,     0,   443,     0,     0,
     327,     0,   168,     0,   383,     0,   390,   481,   215,   508,
     136,     0,     0,   216,     0,     0,     0,     0,     0,     0,
     480,   135,     0,     0,   390,    77,    84,    95,    89,    97,
      79,    88,    98,    85,    99,    93,    87,    94,    92,    90,
      91,    78,    80,    86,    96,     0,    81,     0,     0,     0,
       0,   332,     0,   119,    32,     0,   543,   462,     0,   413,
     544,     7,   390,   480,   125,   124,   309,   380,     0,   379,
       0,     0,   120,   484,     0,     0,    35,     0,   415,   405,
       0,   390,    36,   411,     0,   175,   171,     0,   401,   175,
     172,     0,   321,     0,   414,     0,   379,     0,     0,   546,
     478,   542,   545,   541,     0,    48,    51,     0,     0,   385,
       0,   387,     0,     0,   386,     0,   379,     0,     0,     6,
       0,    47,     0,   197,     0,   295,   294,   218,     0,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   477,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   254,
     261,   262,   263,   258,   260,     0,     0,   256,   259,   257,
     255,     0,   265,   264,     0,     0,   390,   390,     0,     0,
       0,    25,    26,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,    28,     0,    29,     0,   398,   396,     0,
     391,   392,   397,     0,     0,     0,     0,     0,   145,     0,
       0,   144,     0,     0,     0,   151,     0,     0,     0,    57,
     132,     0,   161,     0,     0,    30,   277,   211,   336,     0,
     337,   339,     0,   361,     0,   342,     0,     0,   118,    31,
      33,     0,   308,     0,    64,   482,   483,   121,     0,    34,
     390,     0,   182,   173,   169,   174,   170,     0,   319,   316,
      61,     0,    57,   138,    37,    50,    49,    52,     0,   510,
       0,     0,   498,     0,   500,     0,     0,     0,     0,     0,
       0,   514,     8,     0,    40,     0,     0,     0,     0,     0,
     288,   331,   442,   538,   537,   540,   548,   547,   552,   551,
     534,   531,   532,   533,   474,   475,   473,   521,   496,   497,
     495,   494,   476,   525,   536,   530,   528,   539,   529,   527,
     519,   524,   526,   535,   518,   522,   523,   520,     0,     0,
       0,     0,     0,     0,     0,   550,   549,   554,   553,   454,
     455,   457,   459,     0,   446,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   512,   443,   443,   141,   317,   329,
     384,     0,     0,   407,     0,   318,   197,     0,     0,     0,
       0,   418,     0,     0,     0,   424,     0,     0,     0,   152,
     556,    60,     0,    53,    58,     0,   160,     0,     0,     0,
     406,   282,   279,   280,   281,   285,   286,   287,   277,     0,
     270,     0,   278,   296,     0,   340,     0,   112,   114,   115,
     110,   111,   109,   108,   113,     0,   360,   359,   465,     0,
     334,   463,   333,   493,   382,   381,     0,     0,     0,   408,
       0,   176,   323,   465,     0,     0,     0,   511,   472,   499,
     388,   501,   389,     0,     0,   513,   157,   428,     0,   516,
     515,     0,    42,    41,    39,   198,     0,     0,   208,     0,
     205,   292,   289,   290,   293,   219,     0,    68,    69,    67,
      66,    65,   491,   492,   456,   458,     0,   445,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   137,   394,   395,   393,     0,   330,     0,   159,     0,
       0,   143,     0,   142,     0,     0,   149,     0,     0,   147,
       0,     0,   449,     0,   133,     0,   134,     0,     0,   163,
       0,   165,   283,   284,     0,   277,   268,     0,   374,   297,
     300,   299,   301,     0,   338,   341,   342,     0,     0,   343,
     344,   184,     0,     0,   183,   186,   409,     0,   177,   180,
       0,   320,    62,    63,     0,     0,     0,     0,   158,     0,
       0,   213,   203,     0,   206,   202,   291,   296,   266,    72,
     251,    84,   249,    89,    74,    88,    85,    99,    87,    90,
      73,    75,    86,   230,   233,   231,   232,   243,   234,   247,
     239,   237,   250,   253,   238,   236,   241,   246,   248,   235,
     240,   244,   245,   242,   252,     0,   228,    76,     0,   266,
     266,   226,   447,   342,   485,   485,     0,     0,     0,     0,
       0,     0,     0,     0,   140,   139,     0,   146,     0,     0,
     417,     0,   416,     0,     0,   423,   150,     0,   422,   148,
       0,   448,    55,    54,   162,   433,   164,     0,   374,   271,
       0,     0,   342,   298,   314,   335,   365,     0,   512,     0,
     188,     0,     0,     0,   178,     0,   155,   430,     0,     0,
     153,   429,     0,   517,   192,   480,   213,   213,   190,   213,
     200,   209,   207,     0,   277,   225,   229,     0,     0,   221,
     222,   451,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   213,   421,     0,     0,   427,     0,     0,   450,     0,
       0,   167,   342,   374,     0,   377,   376,   378,   465,   310,
     274,   272,     0,     0,     0,   363,   465,   189,   187,     0,
     181,     0,   156,     0,   154,   201,   356,     0,   302,     0,
     227,    72,    73,   266,   266,     0,     0,     0,     0,     0,
       0,   193,   191,     0,     0,     0,     0,    56,   166,   273,
     342,   366,     0,   311,   313,   312,   315,   306,   307,   212,
       0,   179,     0,     0,     0,   355,   354,   465,   303,   314,
     267,   223,   224,     0,     0,     0,     0,     0,     0,   420,
     419,   426,   425,   276,   275,   368,   369,   371,   465,     0,
     512,   465,   432,   431,     0,   346,     0,     0,     0,   370,
     372,   351,   349,   465,   512,   304,   220,   305,   366,   350,
     465,   373
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -874,  -874,  -874,     6,  -369,  1793,  -874,  -874,  -874,  -874,
    1471,   426,  -304,   288,   290,  -874,  -874,   257,   711,  -874,
     415,  -141,  -630,  -874,  -760,  -874,    -2,  -860,  -713,   -52,
    -874,  -874,  -874,   110,  -874,  -874,  -874,   465,  -874,     8,
    -874,  -874,  -874,  -874,  -874,   544,   193,   -66,  -874,  -874,
    -874,     2,   546,  -874,  -874,  -874,  -874,  -874,  -874,  -874,
    -874,  -127,  -139,  -700,  -874,  -117,    45,   170,  -874,  -874,
    -874,     5,  -874,  -874,  -234,   171,  -874,  -189,  -205,  -874,
    -164,  -874,    -8,   722,  -104,   340,  -874,  -379,  -706,  -734,
    -874,  -547,  -432,  -737,  -873,  -610,    -4,  -874,   -15,  -874,
    -146,  -874,   212,   197,  -236,  -874,  -874,   903,  -874,    -6,
    -874,  -874,  -286,  -874,  -552,  -874,  -874,  1049,  1077,   -11,
      -3,   421,   393,  -874,  1389,  1647,  -874,  -874,  -874,  -874,
    -874,  -874,  -874,  -874,  -345
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   286,    98,   566,   100,   101,   102,   302,
     103,   287,   561,   565,   562,   104,   105,   106,   159,   786,
     229,   107,   226,   108,   596,   192,   234,   109,   110,   111,
     112,   113,   114,   667,   115,   116,   117,   433,   611,   730,
     118,   119,   607,   725,   120,   121,   468,   741,   122,   123,
     639,   640,   167,   227,   583,   125,   126,   470,   747,   645,
     788,   789,   403,   865,   407,   579,   580,   581,   582,   646,
     307,   713,   959,   996,   949,   247,   944,   900,   903,   127,
     273,   438,   128,   129,   230,   231,   411,   412,   600,   956,
     918,   415,   597,   976,   897,   832,   288,   183,   292,   293,
     379,   380,   381,   168,   131,   132,   133,   169,   135,   156,
     157,   523,   396,   691,   524,   525,   136,   170,   171,   139,
     193,   382,   173,   141,   174,   175,   144,   145,   146,   297,
     147,   148,   149,   150,   151
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     154,   355,   244,   245,   172,   612,   692,   177,    97,   458,
     835,   176,   179,   184,   371,   527,   787,   187,   187,   455,
     197,   198,   202,   203,   308,   188,   533,   383,   860,   916,
     584,   235,   413,   236,   237,   416,   238,   578,   239,   716,
     240,   837,   685,   948,   606,   377,   979,   413,   249,   253,
     255,   257,   258,   259,   232,   248,   413,   263,   404,   264,
     256,   267,   271,   377,   394,   232,   274,   276,   977,   277,
     278,   895,   460,   279,   275,   372,   280,   281,   282,   654,
     283,   394,   680,   249,   253,   296,   533,   871,   451,   869,
     870,  -117,   295,   453,   374,   564,   421,  -357,   448,    60,
       3,   377,   461,  -185,   792,  -352,   456,   990,   744,   702,
     305,   793,   655,   456,   375,   431,   452,   394,  -352,   734,
     377,   454,   417,  -357,   449,   152,   901,   948,   456,   355,
    -352,   722,   306,   456,   745,   358,   457,    60,   564,   359,
    -357,   196,   703,   534,  -185,  -117,   915,   158,  -357,   154,
     363,   364,   365,   263,   249,   296,   917,   955,   838,   414,
    -357,   155,   366,   980,  -357,   423,   834,   367,   860,  -117,
    -358,   370,   723,   456,   414,   253,   367,  -352,   598,   367,
     180,   491,  -352,   414,  -357,   385,   939,   241,  -357,   187,
     367,   179,   260,   253,   388,   391,   395,   397,  -185,   130,
    -357,   613,    16,   994,  -357,  -358,   715,   367,   130,   995,
     513,   514,    21,   527,   367,   377,   377,   599,   892,   656,
     130,   423,   130,   961,   962,   705,   724,   571,   300,   152,
    -358,   253,   187,   419,   974,   445,   420,  -358,   787,   425,
     426,   899,   423,   989,   820,   572,  -453,   657,   573,   678,
     253,   992,    50,   706,   186,   429,    57,  -358,  -452,   705,
     999,  1001,   424,    58,   574,   130,   291,   575,   888,    52,
     687,   690,   821,  -436,  -453,   443,   423,  -358,   576,   450,
      76,   547,  -358,   940,   608,  -358,  -452,   960,    79,   377,
     660,   130,   152,   447,   359,   130,   440,   577,   632,   472,
     473,   474,   475,   477,   479,   480,   481,   482,   483,   486,
     487,   527,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   830,   266,
     270,   417,  -269,   204,   253,   253,   578,   735,   737,   417,
     253,   242,     5,   253,   253,   253,   253,   516,   518,   526,
       5,   471,   313,   130,    14,   261,   301,   378,   536,   539,
    -269,    19,  -435,   540,   417,   541,  -322,   320,   537,   641,
     322,   356,   859,   357,   325,   378,   249,   488,   551,   553,
      35,   555,   557,   549,   676,    38,  -322,   563,   563,   385,
     567,    43,   569,   309,   559,   140,  -440,   310,   314,   489,
    -441,  -438,   315,   377,   140,   601,   905,    21,  -353,   642,
    -437,    56,   605,   378,   362,  -490,   140,  -490,   140,   253,
     643,  -353,   130,    66,   476,   478,  -489,    69,  -489,   130,
     563,   563,   378,  -353,    75,    69,   620,   605,   622,   644,
     253,   490,   253,   360,   624,   605,   627,    50,   629,   631,
     731,   322,   320,   367,   225,   322,   249,   373,    58,   325,
     376,   140,  -375,   635,   427,   268,   268,   387,   515,   517,
     406,   447,   430,   172,   274,   527,   437,   178,   439,   535,
    -353,   254,  -375,    79,   130,  -353,  -375,   140,   462,   272,
     466,   140,  -439,   199,    21,   578,   467,   130,   469,  -490,
     552,  -490,   709,   556,    28,   243,   542,   790,   538,   543,
    -375,   544,   859,   313,    37,   545,   294,   551,   663,   555,
     666,   567,   668,   627,   548,   558,   568,   378,   378,   669,
     670,   253,    48,   253,    50,   570,   303,   677,   304,   679,
     585,   586,   710,   684,   599,    58,   603,   526,   124,   140,
     681,   683,   609,   711,   686,   689,   610,   124,   614,   314,
     628,   636,   647,   315,   130,   623,   648,   649,   650,   124,
      79,   124,   712,   651,  -345,   653,   172,   652,   693,   239,
     258,   259,   263,   274,   296,   714,   130,  -345,   253,   130,
     -59,   718,   313,   272,   604,   898,  -362,   694,   130,  -345,
     696,   378,   906,   697,   726,   721,   272,   732,   733,   386,
     742,   743,   423,   320,   124,   321,   322,   746,   140,   604,
     325,   797,   291,   800,   291,   140,   802,   604,   332,   806,
    -364,  -362,   827,   831,   833,   336,   337,   338,   314,   662,
     124,   665,   315,   410,   124,   526,  -345,   836,   477,   516,
     796,  -345,  -367,   799,   130,   843,  -362,   848,   852,   854,
     -82,   422,   130,  -362,   864,  -364,  -347,   804,   805,   810,
    -100,   812,  -367,   890,   815,   807,  -367,   818,  -105,  -347,
     140,   957,   822,  -362,   823,   816,  -104,   825,   819,  -101,
    -364,  -347,   320,   140,   321,   322,  -107,  -364,  -103,   325,
    -367,  -106,   124,  -362,   -83,   978,  -102,   332,  -362,   868,
     893,  -362,   249,   672,   981,   378,   338,  -364,   902,   841,
     909,   931,   658,   847,   958,   851,   465,   991,   853,   924,
     616,   615,   861,   181,   435,   842,   384,  -364,  -347,   675,
     881,   923,  -364,  -347,   978,  -364,   862,   919,   704,   993,
     829,   973,   863,   997,   987,   201,   674,   602,  1000,     0,
     140,     0,   978,     0,     0,   130,   508,   509,     0,   476,
     515,   124,   510,     0,   867,   511,   512,     0,   124,     0,
       0,     0,   140,     0,     0,   140,   810,   812,     0,   815,
     818,   847,   851,     0,   140,     0,     0,   882,   883,   546,
     884,     0,   885,   886,   244,   245,   887,     0,     0,   526,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   410,   130,   904,   627,     0,     0,
       0,     0,   910,   124,   911,     0,     0,     0,   913,   908,
       0,     0,     0,   791,   187,     0,   124,     0,     0,     0,
     140,     0,     0,     0,     0,     0,   794,   795,   140,     0,
       0,   882,   925,   926,   885,   927,   928,   929,   930,     0,
       0,     0,   294,     0,   294,     0,     0,   130,   937,   130,
       0,   634,   130,   296,   638,   130,     0,     0,   945,     0,
     942,   946,     0,     0,   130,     0,     0,   130,     0,     0,
       0,     0,     0,     0,     0,   134,     0,     0,     0,   571,
       0,     0,     0,   124,   134,     0,   313,     0,     0,   130,
       0,     0,   969,   970,   971,   972,   134,   572,   134,     0,
     573,   130,     0,   130,     0,   124,     0,     0,   124,   851,
       0,   982,   983,   296,     0,     0,   574,   124,     0,   575,
     986,   298,   969,   970,   971,   972,   982,   983,     0,     0,
     576,   140,   314,   673,     0,     0,   315,     0,   988,   627,
       0,   134,     0,     0,     0,     0,     0,     0,     0,   577,
       0,     0,     0,   627,     0,     0,     0,   851,     0,     0,
       0,     0,     0,     0,     0,     0,   708,   134,     0,     0,
     410,   134,   896,   124,     0,   130,     0,     0,     0,     0,
     130,   124,   317,     0,   318,   319,   320,     0,   321,   322,
     720,   140,     0,   325,   130,   729,     0,     0,     0,   272,
     331,   332,     0,     0,     0,   335,   130,     0,   336,   337,
     338,     0,     0,   199,    21,   130,     0,     0,     0,   130,
       0,   137,     0,     0,    28,   243,     0,     0,   313,   134,
     137,     0,     0,     0,    37,     0,     0,  -213,     0,     0,
     943,     0,   137,   140,   137,   140,     0,     0,   140,   138,
       0,   140,    48,     0,    50,     0,     0,   130,   138,  -213,
     140,     0,     0,   140,     0,    58,     0,     0,     0,     0,
     138,     0,   138,     0,   314,     0,     0,     0,   315,     0,
       0,     0,   975,     0,   124,   140,     0,   137,     0,   828,
      79,     0,   410,     0,     0,     0,     0,   140,   134,   140,
     130,   130,   130,   130,     0,   134,     0,     0,     0,     0,
       0,     0,   729,   137,     0,   138,   571,   137,     0,   130,
     130,   975,     0,     0,   317,     0,   318,   319,   320,   638,
     321,   322,   323,     0,   572,   325,     0,   573,     0,   975,
       0,   138,   331,   332,   124,   138,     0,   335,     0,   866,
     336,   337,   338,   574,     0,     0,   575,     0,    52,     0,
     134,     0,     0,     0,     0,     0,     0,   576,     0,     0,
       0,   140,     0,   134,     0,   137,   140,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   577,     0,     0,     0,
     140,     0,     0,     0,     0,     0,   124,     0,   124,     0,
       0,   124,   140,   138,   124,     0,     0,     0,     0,     0,
       0,   140,     0,   124,    16,   140,   124,     0,    20,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,    28,
     855,     0,     0,     0,    33,     0,     0,     0,   124,    37,
     134,     0,     0,     0,   137,     0,     0,     0,     0,     0,
     124,   137,   124,   140,     0,     0,     0,    48,     0,    50,
       0,    52,   134,   856,     0,   134,   857,     0,    57,     0,
      58,     0,   138,     0,   134,     0,     0,     0,     0,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,    76,     0,     0,    79,   140,   140,   140,   140,
       0,     0,     0,     0,     0,     0,   137,     0,     0,     0,
       0,     0,     0,     0,     0,   140,   140,     0,     0,   137,
       0,     0,     0,     0,   124,     0,     0,     0,     0,   124,
     134,     0,     0,     0,   138,     0,     0,     0,   134,     0,
     205,     0,     0,   124,    92,     0,     0,   138,     0,     0,
     858,     0,   206,   207,     0,   124,     0,     0,     0,   208,
       0,   142,     0,     0,   124,     0,     0,   209,   124,     0,
     142,     0,     0,   210,     0,     0,     0,     0,   211,     0,
     212,     0,   142,   213,   142,     0,   137,     0,     0,   214,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     215,     0,     0,     0,     0,     0,   124,     0,   137,   216,
       0,   137,     0,     0,   138,     0,     0,     0,   217,   218,
     137,   219,     0,   220,     0,   221,     0,   142,   222,     0,
       0,     0,   223,   408,     0,   224,   138,     0,     0,   138,
       0,   134,     0,     0,   313,     0,     0,     0,   138,   124,
     124,   124,   124,   142,     0,     0,     0,   142,     0,     0,
       0,     0,     0,     0,   740,     0,     0,     0,   124,   124,
       0,     0,     0,     0,     0,     0,   137,     0,   313,     0,
       0,     0,  -199,   233,   137,     0,  -199,  -199,     0,     0,
     314,     0,     0,   409,   315,  -199,     0,  -199,  -199,     0,
       0,   134,  -199,   233,   138,     0,     0,  -199,     0,     0,
    -199,     0,   138,   265,   269,   142,     0,     0,     0,     0,
       0,     0,     0,     0,   314,  -199,     0,  -199,   315,  -199,
       0,  -199,  -199,     0,  -199,     0,  -199,     0,  -199,     0,
     317,     0,   318,   319,   320,     0,   321,   322,     0,     0,
       0,   325,     0,   134,     0,   134,     0,  -199,   134,   332,
    -199,   134,     0,  -199,     0,     0,   336,   337,   338,     0,
     134,     0,     0,   134,   317,     0,     0,   319,   320,     0,
     321,   322,     0,     0,   142,   325,     0,   137,     0,     0,
       0,   142,     0,   332,     0,   134,     0,     0,     0,     0,
     336,   337,   338,     0,     0,     0,     0,   134,     0,   134,
       0,     0,  -199,     0,     0,   138,     0,   368,  -199,   143,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,   233,   233,   233,
     143,     0,   143,     0,   402,     0,   142,   137,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   138,   233,     0,     0,   233,
       0,   134,     0,     0,     0,   143,   134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   233,   137,
     134,   137,     0,     0,   137,     0,     0,   137,     0,     0,
       0,   143,   134,     0,     0,   143,   137,     0,   233,   137,
       0,   134,     0,     0,     0,   134,   142,   138,     0,   138,
       0,     0,   138,     0,     0,   138,     0,     0,     0,     0,
       0,   137,     0,     0,   138,     0,     0,   138,   142,     0,
       0,   142,     0,   137,     0,   137,     0,     0,     0,     0,
     142,     0,     0,   134,     0,    99,     0,     0,     0,   138,
       0,     0,     0,   143,   153,     0,     0,     0,     0,     0,
       0,   138,     0,   138,     0,     0,   182,     0,   185,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,     0,     0,   134,   134,   134,   134,
       0,   233,     0,     0,     0,     0,   142,     0,     0,  -460,
       0,     0,     0,     0,   142,   134,   134,   137,     0,     0,
       0,   262,   137,   354,     0,     0,  -489,   233,  -489,     0,
       0,     0,   143,     0,     0,     0,   137,     0,     0,   143,
       0,   205,     0,     0,     0,   138,     0,    99,   137,     0,
     138,   299,     0,   206,   207,     0,     0,   137,     0,     0,
     208,   137,     0,     0,   138,     0,     0,     0,   209,     0,
       0,     0,     0,     0,   210,     0,   138,     0,     0,   211,
       0,   212,     0,     0,   213,   138,     0,     0,     0,   138,
     214,     0,     0,     0,   143,     0,     0,     0,     0,   137,
       0,   215,   313,     0,     0,     0,     0,   143,     0,   361,
     216,     0,     0,     0,     0,     0,     0,   142,     0,   217,
     218,     0,   219,     0,   220,     0,   221,   138,     0,   222,
       0,     0,     0,   223,     0,     0,   224,     0,     0,     0,
       0,     0,   137,   137,   137,   137,     0,     0,   314,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,   137,   137,     0,     0,     0,     0,     0,     0,     0,
     138,   138,   138,   138,   143,     0,     0,   142,   405,     0,
       0,     0,     0,   233,   233,   418,     0,   233,   233,   138,
     138,     0,   367,     0,   727,     0,   143,     0,   317,   143,
     701,     0,   320,     0,   321,   322,     0,     0,   143,   325,
       0,     0,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,   336,   337,   338,     0,     0,   142,
       0,   142,     0,     0,   142,     0,     0,   142,     0,     0,
      99,   728,     0,     0,     0,     0,   142,     0,     0,   142,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   143,     0,     0,     0,     0,     0,
       0,   142,   143,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   142,     0,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     233,   233,     0,     0,     0,     0,     0,     0,   233,     0,
     402,   205,     0,     0,     0,   402,     0,     0,   233,     0,
      99,   233,     0,   206,   207,     0,     0,     0,     0,     0,
     208,     0,     0,     0,     0,     0,     0,     0,   209,     0,
       0,     0,   418,     0,   210,   418,     0,     0,     0,   211,
       0,   212,     0,     0,   213,     0,     0,   142,     0,     0,
     214,   844,   142,     0,     0,     0,     0,     0,     0,     0,
       0,   215,     0,     0,     0,   143,   142,     0,     0,     0,
     216,     0,   880,     0,     0,     0,     0,     0,   142,   217,
     218,     0,   219,     0,   220,     0,   221,   142,     0,   222,
    -199,   142,     0,   223,  -199,  -199,   224,     0,     0,     0,
     626,     0,     0,  -199,     0,  -199,  -199,     0,    99,     0,
    -199,     0,     0,     0,     0,  -199,     0,     0,  -199,     0,
       0,     0,     0,     0,     0,   143,     0,     0,     0,   142,
       0,     0,     0,  -199,     0,  -199,     0,  -199,     0,  -199,
    -199,     0,  -199,     0,  -199,     0,  -199,     0,     0,     0,
       0,     0,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,   233,     0,     0,  -199,     0,     0,  -199,     0,
       0,  -199,   142,   142,   142,   142,     0,   143,     0,   143,
       0,     0,   143,     0,     0,   143,     0,     0,     0,     0,
       0,   142,   142,     0,   143,     0,     0,   143,     0,     0,
       0,     0,     0,     0,   402,   402,     0,   402,   402,   205,
       0,   699,     0,     0,     0,     0,     0,     0,     0,   143,
    -199,   206,   207,     0,   947,     0,  -199,     0,   208,     0,
     951,   143,   402,   143,   402,     0,   209,     0,     0,     0,
       0,     0,   210,     0,     0,     0,     0,   211,     0,   212,
       0,     0,   213,     0,     0,     0,     0,     0,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   215,
       0,   738,     0,     0,     0,     0,     0,     0,   216,     0,
       0,     0,   311,     0,     0,     0,     0,   217,   218,   312,
     219,     0,   220,     0,   221,     0,     0,   222,     0,     0,
     313,   223,     0,     0,   224,   143,     0,     0,   947,     0,
     143,     0,     0,     0,    16,     0,     0,     0,    20,    21,
       0,     0,     0,   418,   143,   418,     0,    27,   418,    28,
     855,   418,     0,     0,    33,     0,   143,     0,     0,    37,
     824,     0,     0,   826,     0,   143,   314,     0,     0,   143,
     315,     0,     0,     0,     0,     0,     0,    48,     0,    50,
       0,   298,   707,   856,     0,   840,   857,     0,    57,     0,
      58,     0,     0,     0,     0,     0,     0,   846,     0,   850,
     316,     0,     0,     0,     0,     0,     0,   143,     0,    74,
       0,     0,    76,     0,     0,    79,   317,     0,   318,   319,
     320,     0,   321,   322,   323,     0,   324,   325,   326,   327,
     328,     0,   329,   330,   331,   332,     0,   333,   334,   335,
       0,     0,   336,   337,   338,     0,     0,     0,     0,   618,
     143,   143,   143,   143,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,   143,
     143,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     891,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   907,     0,     0,     0,     0,     0,     0,     0,
       0,   912,     0,     0,     0,   914,     0,     0,     0,     0,
      -2,     4,     0,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,   938,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,    37,     0,    38,  -213,    39,    40,
      41,    42,    43,    44,   -43,    45,    46,     0,    47,     0,
       0,     0,    48,    49,    50,    51,    52,    53,    54,  -213,
     -43,    55,    56,    57,     0,    58,    59,    60,     0,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    69,    70,
       0,    71,    72,    73,    74,    75,     0,    76,    77,    78,
      79,     0,     0,    80,     0,    81,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,     0,    92,
       0,     0,    93,     0,    94,     0,    95,   984,    96,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,    36,
       0,     0,    38,     0,     0,    40,    41,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,   246,     0,     0,     0,    93,     0,
     165,     0,    95,     0,   166,   985,     4,     0,     5,     6,
       7,     8,     9,    10,     0,  -512,     0,    11,    12,    13,
      14,    15,  -512,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,  -512,    25,    26,  -512,    27,     0,    28,
      29,    30,    31,    32,    33,    34,    35,     0,    36,    37,
       0,    38,  -213,    39,    40,    41,    42,    43,    44,   -43,
      45,    46,     0,    47,     0,     0,     0,    48,    49,    50,
      51,     0,    53,    54,  -213,   -43,    55,    56,    57,  -512,
      58,    59,    60,  -512,    61,    62,    63,    64,    65,    66,
      67,    68,     0,    69,    70,     0,    71,    72,    73,    74,
      75,     0,    76,    77,    78,    79,     0,     0,    80,     0,
      81,     0,     0,  -512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -512,
      84,  -512,  -512,  -512,  -512,  -512,  -512,  -512,     0,  -512,
    -512,  -512,  -512,  -512,     0,  -512,  -512,  -512,  -512,    89,
    -512,  -512,  -512,     0,    92,  -512,  -512,  -512,     0,    94,
    -512,    95,   284,    96,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,    37,     0,    38,  -213,    39,
      40,    41,    42,    43,    44,   -43,    45,    46,     0,    47,
       0,     0,     0,    48,    49,    50,    51,    52,    53,    54,
    -213,   -43,    55,    56,    57,     0,    58,    59,    60,     0,
      61,    62,    63,    64,    65,    66,    67,    68,     0,    69,
      70,     0,    71,    72,    73,    74,    75,     0,    76,    77,
      78,    79,     0,     0,    80,     0,    81,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,   246,
      92,     0,     0,    93,     0,    94,   285,    95,     4,    96,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,    37,     0,    38,  -213,    39,    40,    41,    42,    43,
      44,   -43,    45,    46,     0,    47,     0,     0,     0,    48,
      49,    50,    51,    52,    53,    54,  -213,   -43,    55,    56,
      57,     0,    58,    59,    60,     0,    61,    62,    63,    64,
      65,    66,    67,    68,     0,    69,    70,     0,    71,    72,
      73,    74,    75,     0,    76,    77,    78,    79,     0,     0,
      80,     0,    81,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,     0,    92,     0,     0,    93,
       0,    94,   446,    95,   463,    96,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,    37,     0,    38,
    -213,    39,    40,    41,    42,    43,    44,   -43,    45,    46,
       0,    47,     0,     0,     0,    48,    49,    50,    51,    52,
      53,    54,  -213,   -43,    55,    56,    57,     0,    58,    59,
      60,     0,    61,    62,    63,    64,    65,    66,    67,    68,
       0,    69,    70,     0,    71,    72,    73,    74,    75,     0,
      76,    77,    78,    79,     0,     0,    80,     0,    81,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,     0,    92,     0,     0,    93,     0,    94,   464,    95,
     284,    96,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -213,    39,    40,    41,
      42,    43,    44,   -43,    45,    46,     0,    47,     0,     0,
       0,    48,    49,    50,    51,    52,    53,    54,  -213,   -43,
      55,    56,    57,     0,    58,    59,    60,     0,    61,    62,
      63,    64,    65,    66,    67,    68,     0,    69,    70,     0,
      71,    72,    73,    74,    75,     0,    76,    77,    78,    79,
       0,     0,    80,     0,    81,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,     0,    92,     0,
       0,    93,     0,    94,   285,    95,     4,    96,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,    34,    35,     0,    36,    37,
       0,    38,  -213,    39,    40,    41,    42,    43,    44,   -43,
      45,    46,     0,    47,     0,     0,     0,    48,    49,    50,
      51,    52,    53,    54,  -213,   -43,    55,    56,    57,     0,
      58,    59,    60,     0,    61,    62,    63,    64,    65,    66,
      67,    68,     0,    69,    70,     0,    71,    72,    73,    74,
      75,     0,    76,    77,    78,    79,     0,     0,    80,     0,
      81,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
       0,    90,    91,     0,    92,     0,     0,    93,     0,    94,
     633,    95,     4,    96,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,    37,     0,    38,  -213,    39,
      40,    41,    42,    43,    44,   -43,    45,    46,     0,    47,
       0,     0,     0,    48,    49,    50,    51,   298,    53,    54,
    -213,   -43,    55,    56,    57,     0,    58,    59,    60,     0,
      61,    62,    63,    64,    65,    66,    67,    68,     0,    69,
      70,     0,    71,    72,    73,    74,    75,     0,    76,    77,
      78,    79,     0,     0,    80,     0,    81,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,     0,
      92,     0,     0,    93,     0,    94,     0,    95,     4,    96,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,    37,     0,    38,  -213,    39,    40,    41,    42,    43,
      44,   -43,    45,    46,     0,    47,     0,     0,     0,    48,
      49,    50,    51,     0,    53,    54,  -213,   -43,    55,    56,
      57,     0,    58,    59,    60,     0,    61,    62,    63,    64,
      65,    66,    67,    68,     0,    69,    70,     0,    71,    72,
      73,    74,    75,     0,    76,    77,    78,    79,     0,     0,
      80,     0,    81,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,     0,    92,     0,     0,    93,
       0,    94,     0,    95,     4,    96,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,   550,    35,     0,    36,    37,     0,    38,
    -213,    39,    40,    41,    42,    43,    44,   -43,    45,    46,
       0,    47,     0,     0,     0,    48,    49,    50,    51,     0,
      53,    54,  -213,   -43,    55,    56,    57,     0,    58,    59,
      60,     0,    61,    62,    63,    64,    65,    66,    67,    68,
       0,    69,    70,     0,    71,    72,    73,    74,    75,     0,
      76,    77,    78,    79,     0,     0,    80,     0,    81,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,     0,    92,     0,     0,    93,     0,    94,     0,    95,
       4,    96,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,   554,
      35,     0,    36,    37,     0,    38,  -213,    39,    40,    41,
      42,    43,    44,   -43,    45,    46,     0,    47,     0,     0,
       0,    48,    49,    50,    51,     0,    53,    54,  -213,   -43,
      55,    56,    57,     0,    58,    59,    60,     0,    61,    62,
      63,    64,    65,    66,    67,    68,     0,    69,    70,     0,
      71,    72,    73,    74,    75,     0,    76,    77,    78,    79,
       0,     0,    80,     0,    81,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,     0,    92,     0,
       0,    93,     0,    94,     0,    95,     4,    96,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,   809,    35,     0,    36,    37,
       0,    38,  -213,    39,    40,    41,    42,    43,    44,   -43,
      45,    46,     0,    47,     0,     0,     0,    48,    49,    50,
      51,     0,    53,    54,  -213,   -43,    55,    56,    57,     0,
      58,    59,    60,     0,    61,    62,    63,    64,    65,    66,
      67,    68,     0,    69,    70,     0,    71,    72,    73,    74,
      75,     0,    76,    77,    78,    79,     0,     0,    80,     0,
      81,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
       0,    90,    91,     0,    92,     0,     0,    93,     0,    94,
       0,    95,     4,    96,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,   811,    35,     0,    36,    37,     0,    38,  -213,    39,
      40,    41,    42,    43,    44,   -43,    45,    46,     0,    47,
       0,     0,     0,    48,    49,    50,    51,     0,    53,    54,
    -213,   -43,    55,    56,    57,     0,    58,    59,    60,     0,
      61,    62,    63,    64,    65,    66,    67,    68,     0,    69,
      70,     0,    71,    72,    73,    74,    75,     0,    76,    77,
      78,    79,     0,     0,    80,     0,    81,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,     0,
      92,     0,     0,    93,     0,    94,     0,    95,     4,    96,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,   814,    35,     0,
      36,    37,     0,    38,  -213,    39,    40,    41,    42,    43,
      44,   -43,    45,    46,     0,    47,     0,     0,     0,    48,
      49,    50,    51,     0,    53,    54,  -213,   -43,    55,    56,
      57,     0,    58,    59,    60,     0,    61,    62,    63,    64,
      65,    66,    67,    68,     0,    69,    70,     0,    71,    72,
      73,    74,    75,     0,    76,    77,    78,    79,     0,     0,
      80,     0,    81,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,     0,    92,     0,     0,    93,
       0,    94,     0,    95,     4,    96,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,   817,    35,     0,    36,    37,     0,    38,
    -213,    39,    40,    41,    42,    43,    44,   -43,    45,    46,
       0,    47,     0,     0,     0,    48,    49,    50,    51,     0,
      53,    54,  -213,   -43,    55,    56,    57,     0,    58,    59,
      60,     0,    61,    62,    63,    64,    65,    66,    67,    68,
       0,    69,    70,     0,    71,    72,    73,    74,    75,     0,
      76,    77,    78,    79,     0,     0,    80,     0,    81,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,     0,    92,     0,     0,    93,     0,    94,     0,    95,
       4,    96,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,   839,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -213,    39,    40,    41,
      42,    43,    44,   -43,    45,    46,     0,    47,     0,     0,
       0,    48,    49,    50,    51,     0,    53,    54,  -213,   -43,
      55,    56,    57,     0,    58,    59,    60,     0,    61,    62,
      63,    64,    65,    66,    67,    68,     0,    69,    70,     0,
      71,    72,    73,    74,    75,     0,    76,    77,    78,    79,
       0,     0,    80,     0,    81,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,     0,    92,     0,
       0,    93,     0,    94,     0,    95,     4,    96,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,   845,    35,     0,    36,    37,
       0,    38,  -213,    39,    40,    41,    42,    43,    44,   -43,
      45,    46,     0,    47,     0,     0,     0,    48,    49,    50,
      51,     0,    53,    54,  -213,   -43,    55,    56,    57,     0,
      58,    59,    60,     0,    61,    62,    63,    64,    65,    66,
      67,    68,     0,    69,    70,     0,    71,    72,    73,    74,
      75,     0,    76,    77,    78,    79,     0,     0,    80,     0,
      81,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
       0,    90,    91,     0,    92,     0,     0,    93,     0,    94,
       0,    95,     4,    96,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,   849,    35,     0,    36,    37,     0,    38,  -213,    39,
      40,    41,    42,    43,    44,   -43,    45,    46,     0,    47,
       0,     0,     0,    48,    49,    50,    51,     0,    53,    54,
    -213,   -43,    55,    56,    57,     0,    58,    59,    60,     0,
      61,    62,    63,    64,    65,    66,    67,    68,     0,    69,
      70,     0,    71,    72,    73,    74,    75,     0,    76,    77,
      78,    79,     0,     0,    80,     0,    81,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,     0,
      92,     0,     0,    93,     0,    94,     0,    95,   717,    96,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   160,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,   246,     0,     0,     0,    93,
       0,   165,     0,    95,     0,   166,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   160,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,   246,     0,     0,     0,    93,     0,   165,     0,    95,
       0,   166,   625,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   160,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,   246,     0,
       0,     0,    93,     0,   165,     0,    95,     0,   166,   941,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   160,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,     0,     0,     0,     0,    93,
       0,   165,     0,    95,     0,   166,   630,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,   199,    21,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,   200,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,    50,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,    58,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,    79,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   748,     0,   749,     0,    83,    84,
      85,    86,     0,   750,     0,     0,    87,     0,   751,   207,
     752,     0,     0,     0,     0,   753,    88,     0,    89,     0,
      90,    91,     0,   209,     0,     0,    93,     0,   165,   754,
      95,     0,   166,     0,   755,     0,   212,     0,     0,   756,
       0,     0,     0,     0,     0,   757,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   215,     0,     0,     0,
       0,     0,     0,     0,     0,   758,     0,     0,     0,     0,
       0,     0,     0,     0,   217,   218,     0,   759,     0,   220,
       0,   760,     0,     0,   761,     0,     0,     0,   762,     0,
       0,   224,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   763,   764,   765,
     766,     0,     0,   767,     0,     0,     0,   768,   769,   770,
     771,   772,   773,   774,   775,   776,   777,   778,   779,     0,
     780,     0,     0,   781,   782,   783,   784,     0,     0,   785,
       5,     6,     7,     8,   189,    10,   190,     0,     0,   160,
       0,     0,    14,    15,     0,     0,  -116,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
    -116,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,   191,    75,     0,  -116,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,    15,     0,     0,    93,
       0,   165,    19,    95,     0,   166,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,   250,   251,     0,    75,   289,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,   290,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,   246,     0,
       0,     0,    93,     0,   165,     0,    95,     0,   166,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,    36,
       0,     0,    38,     0,     0,    40,    41,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,   250,   251,
       0,    75,   289,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,   246,     0,     0,     0,    93,     0,
     165,     0,    95,   619,   166,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   160,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
       0,    40,    41,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,   250,   251,     0,    75,   289,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
     246,     0,     0,     0,    93,     0,   165,     0,    95,   621,
     166,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     160,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,   519,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
     520,    36,     0,     0,    38,     0,     0,    40,    41,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,   521,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,   522,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,     0,     0,     0,     0,
      93,     0,   165,     0,    95,     0,   166,     5,     6,     7,
       8,   189,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,  -116,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,  -116,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,   191,    75,
       0,  -116,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   160,
      90,    91,    14,    15,     0,     0,    93,     0,   165,    19,
      95,     0,   166,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,   250,
     251,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,     0,   252,     0,     0,    93,
       0,   165,     0,    95,     0,   166,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   160,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,   250,   251,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,   246,     0,     0,     0,    93,     0,   165,     0,    95,
       0,   166,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   160,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,    43,    44,     0,    45,    46,   719,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,   250,   251,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    88,     0,    89,   160,    90,    91,    14,    15,     0,
       0,    93,     0,   165,    19,    95,     0,   166,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
       0,    40,    41,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,   194,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,   195,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,    90,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     166,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,    40,    41,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,   250,   251,     0,
      75,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    88,     0,    89,
     160,    90,    91,    14,    15,     0,     0,    93,     0,   165,
      19,    95,     0,   166,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,    40,    41,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,   246,     0,     0,     0,
      93,     0,   165,   617,    95,     0,   166,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,   232,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   160,
      90,    91,    14,    15,     0,     0,    93,     0,    94,    19,
      95,     0,   166,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,   246,     0,     0,     0,    93,
       0,   165,     0,    95,     0,   166,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   160,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,   195,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    88,     0,    89,   160,    90,
      91,    14,   587,     0,     0,    93,     0,   165,    19,    95,
       0,   166,     0,     0,     0,     0,   588,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,   589,
       0,     0,    38,     0,     0,    40,    41,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,   590,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,   591,   592,    65,
      66,    67,   593,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,   594,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,   246,     0,     0,     0,    93,     0,
     165,     0,    95,     0,   894,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   160,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
       0,    40,    41,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
     246,     0,     0,     0,    93,     0,   165,     0,    95,     0,
     894,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     160,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   879,    35,
       0,    36,     0,     0,    38,     0,     0,    40,    41,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,   246,     0,     0,     0,
      93,     0,   165,     0,    95,     0,   166,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   160,
      90,    91,    14,    15,     0,     0,    93,     0,   165,    19,
      95,     0,   166,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,   484,     0,     0,    93,
       0,    94,    19,    95,     0,   166,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,   485,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   160,    90,    91,    14,    15,
       0,     0,    93,     0,   165,    19,    95,     0,   166,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    88,     0,    89,   160,    90,
      91,    14,   587,     0,     0,   560,     0,   165,    19,    95,
       0,   166,     0,     0,     0,     0,   588,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,   589,
       0,     0,    38,     0,     0,    40,    41,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,   590,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,   591,   592,    65,
      66,    67,   593,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,   594,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   595,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    88,     0,    89,   160,    90,    91,    14,    15,     0,
       0,    93,     0,   165,    19,    95,     0,   595,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   661,    35,     0,    36,     0,     0,    38,     0,
       0,    40,    41,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,    90,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     166,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   664,    35,     0,    36,     0,
       0,    38,     0,     0,    40,    41,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,     0,     0,     0,
      75,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    88,     0,    89,
     160,    90,    91,    14,    15,     0,     0,    93,     0,   165,
      19,    95,     0,   166,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   873,    35,
       0,    36,     0,     0,    38,     0,     0,    40,    41,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,    15,     0,     0,
      93,     0,   165,    19,    95,     0,   166,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   874,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    88,     0,    89,   160,    90,    91,    14,
      15,     0,     0,    93,     0,   165,    19,    95,     0,   166,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   876,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   160,
      90,    91,    14,    15,     0,     0,    93,     0,   165,    19,
      95,     0,   166,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   877,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   160,    90,    91,    14,    15,     0,     0,    93,
       0,   165,    19,    95,     0,   166,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     878,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   160,    90,    91,    14,    15,
       0,     0,    93,     0,   165,    19,    95,     0,   166,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   879,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    88,     0,    89,   160,    90,
      91,    14,   587,     0,     0,    93,     0,   165,    19,    95,
       0,   166,     0,     0,     0,     0,   588,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,   589,
       0,     0,    38,     0,     0,    40,    41,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,   590,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,   591,   592,    65,
      66,    67,   593,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,   594,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   954,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    88,     0,    89,   160,    90,    91,    14,    15,     0,
       0,    93,     0,     0,    19,    95,     0,   954,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   161,
     162,     0,   163,    35,     0,    36,     0,     0,    38,     0,
       0,    40,    41,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,     0,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     166,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,    40,    41,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,    16,
       0,     0,     0,    20,    21,     0,    63,    64,    65,    66,
      67,   164,    27,    69,    28,   855,    71,     0,     0,    33,
      75,     0,     0,    77,    37,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -366,     0,     0,     0,     0,     0,
       0,     0,    48,     0,    50,     0,    52,     0,   856,     0,
      84,   857,     0,    57,  -366,    58,     0,     0,  -366,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,    89,
       0,    14,    15,   246,    74,     0,     0,    76,    19,   165,
      79,    95,  -366,   894,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,    36,
       0,     0,    38,     0,     0,    40,    41,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,    92,
       0,     0,     0,  -348,     0,   932,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,  -348,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,  -348,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,     0,     0,     0,     0,     0,     0,
       0,    84,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   160,     0,     0,    14,    15,     0,     0,     0,     0,
      89,    19,     0,   354,     0,  -348,  -489,     0,  -489,    26,
    -348,     0,    95,     0,   954,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   160,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,    84,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,    89,    40,    41,     0,    43,    44,     0,
      45,    46,     0,   165,     0,    95,     0,   166,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   164,     0,    69,     0,     0,    71,     0,     0,     0,
      75,     0,     0,    77,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   160,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
      84,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,    89,
      40,    41,     0,    43,    44,     0,    45,    46,     0,   165,
       0,    95,     0,   595,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,   921,    71,     0,     0,     0,    75,     0,   750,    77,
       0,     0,     0,   206,   207,   752,     0,     0,     0,     0,
     208,     0,     0,     0,     0,     0,     0,     0,   209,     0,
       0,     0,     0,     0,   754,     0,    84,     0,     0,   211,
       0,   212,     0,     0,   213,     0,     0,     0,     0,     0,
     214,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,   215,     0,     0,     0,     0,     0,    95,     0,   166,
     216,     0,     0,     0,     0,     0,     0,     0,     0,   217,
     218,     0,   219,     0,   220,     0,   922,     0,   311,   761,
       0,     0,     0,   223,     0,   312,   224,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,   339,   340,   341,
     342,   343,   344,     0,     0,   347,   348,   349,   350,     0,
     352,   353,   763,   764,   765,   766,     0,     0,   767,     0,
       0,     0,   768,   769,   770,   771,   772,   773,   774,   775,
     776,   777,   778,   779,     0,   780,   313,     0,   781,   782,
     783,   784,   314,     0,     0,     0,   315,     0,   311,     0,
       0,     0,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,     0,
       0,     0,   394,     0,     0,     0,   316,     0,     0,     0,
       0,     0,   314,     0,     0,     0,   315,     0,     0,     0,
       0,     0,   317,     0,   318,   319,   320,     0,   321,   322,
     323,     0,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,   314,   333,   334,   335,   315,     0,   336,   337,
     338,     0,     0,   311,     0,     0,     0,   736,     0,     0,
     312,     0,   317,     0,   318,   319,   320,     0,   321,   322,
     323,   313,   324,   325,   326,   327,   316,     0,   329,   330,
     331,   332,     0,   333,     0,   335,     0,     0,   336,   337,
     338,     0,   317,     0,   318,   319,   320,     0,   321,   322,
     323,     0,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,     0,   333,   334,   335,     0,   314,   336,   337,
     338,   315,     0,   311,     0,     0,     0,   803,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,   314,   333,   334,
     335,   315,     0,   336,   337,   338,     0,     0,   311,     0,
       0,     0,   950,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,     0,
       0,   316,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,     0,   333,   334,
     335,     0,   314,   336,   337,   338,   315,     0,     0,     0,
       0,     0,   998,   311,     0,     0,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,   316,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   317,     0,   318,   319,   320,     0,   321,   322,
     323,     0,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,     0,   333,   334,   335,     0,   314,   336,   337,
     338,   315,     0,     0,     0,   920,     0,     0,   311,     0,
     401,     0,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,     0,   333,   334,
     335,     0,   314,   336,   337,   338,   315,   367,     0,     0,
       0,     0,     0,   311,     0,   808,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,   316,   339,   340,   341,
     342,   343,   344,     0,     0,   347,   348,   349,   350,     0,
     352,   353,   317,     0,   318,   319,   320,     0,   321,   322,
     323,     0,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,     0,   333,   334,   335,     0,   314,   336,   337,
     338,   315,   367,     0,     0,     0,     0,     0,   311,     0,
     813,     0,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,     0,   333,   334,
     335,     0,   314,   336,   337,   338,   315,   367,     0,     0,
       0,     0,     0,   311,     0,   933,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,   316,   339,   340,   341,
     342,   343,   344,     0,     0,   347,   348,   349,   350,     0,
     352,   353,   317,     0,   318,   319,   320,     0,   321,   322,
     323,     0,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,     0,   333,   334,   335,     0,   314,   336,   337,
     338,   315,   367,     0,     0,     0,     0,     0,   311,     0,
     934,     0,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,     0,   333,   334,
     335,     0,   314,   336,   337,   338,   315,   367,     0,     0,
       0,     0,     0,   311,     0,   935,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,   316,   339,   340,   341,
     342,   343,   344,     0,     0,   347,   348,   349,   350,     0,
     352,   353,   317,     0,   318,   319,   320,     0,   321,   322,
     323,     0,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,     0,   333,   334,   335,     0,   314,   336,   337,
     338,   315,   367,     0,     0,     0,     0,     0,   311,     0,
     936,     0,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,     0,     0,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,     0,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,     0,   333,   334,
     335,     0,   314,   336,   337,   338,   315,   367,     0,     0,
       0,     0,     0,   311,     0,   952,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,   316,   339,   340,   341,
     342,   343,   344,     0,     0,   347,   348,   349,   350,     0,
     352,   353,   317,     0,   318,   319,   320,     0,   321,   322,
     323,     0,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,     0,   333,   334,   335,     0,   314,   336,   337,
     338,   315,   367,     0,     0,     0,     0,     0,     0,     0,
     953,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     311,     0,     0,     0,     0,     0,     0,   312,     0,     0,
       0,   316,   339,   340,   341,   342,   343,   344,   313,   392,
     347,   348,   349,   350,     0,   352,   353,   317,     0,   318,
     319,   320,   393,   321,   322,   323,     0,   324,   325,   326,
     327,   328,     0,   329,   330,   331,   332,     0,   333,   334,
     335,   311,     0,   336,   337,   338,     0,   367,   312,     0,
       0,     0,     0,     0,   314,     0,     0,     0,   315,   313,
     389,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   390,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   394,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   317,   314,   318,   319,   320,   315,
     321,   322,   323,     0,   324,   325,   326,   327,   328,     0,
     329,   330,   331,   332,     0,   333,   334,   335,   311,     0,
     336,   337,   338,     0,   367,   312,     0,     0,     0,   316,
       0,     0,     0,     0,     0,     0,   313,   688,     0,     0,
       0,     0,     0,     0,     0,   317,     0,   318,   319,   320,
       0,   321,   322,   323,     0,   324,   325,   326,   327,   328,
       0,   329,   330,   331,   332,     0,   333,   334,   335,   311,
       0,   336,   337,   338,     0,   367,   312,     0,     0,     0,
       0,     0,   314,     0,     0,     0,   315,   313,   232,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     311,     0,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,   394,     0,     0,     0,   316,     0,   313,   682,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   317,   314,   318,   319,   320,   315,   321,   322,
     323,     0,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,     0,   333,   334,   335,     0,   313,   336,   337,
     338,     0,   367,     0,   314,     0,     0,   316,   315,     0,
     311,     0,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,     0,   317,     0,   318,   319,   320,   313,   321,
     322,   323,     0,   324,   325,   326,   327,   328,   316,   329,
     330,   331,   332,   314,   333,   334,   335,   315,     0,   336,
     337,   338,     0,   367,   317,     0,   318,   319,   320,     0,
     321,   322,   323,     0,   324,   325,   326,   327,   328,     0,
     329,   330,   331,   332,   314,   333,   334,   335,   315,     0,
     336,   337,   338,     0,   367,   311,     0,   700,     0,     0,
       0,     0,   312,   317,     0,   318,   319,   320,     0,   321,
     322,   323,     0,   313,   325,   326,   327,     0,   316,   329,
     330,   331,   332,     0,     0,     0,   335,     0,     0,   336,
     337,   338,     0,     0,   317,     0,   318,   319,   320,     0,
     321,   322,   323,     0,   324,   325,   326,   327,   328,     0,
     329,   330,   331,   332,     0,   333,   334,   335,     0,   314,
     336,   337,   338,   315,   367,   311,     0,     0,     0,     0,
       0,     0,   312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,     0,     0,     0,     0,   398,     0,
       0,     0,     0,   316,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   317,
       0,   318,   319,   320,   399,   321,   322,   323,     0,   324,
     325,   326,   327,   328,     0,   329,   330,   331,   332,   314,
     333,   334,   335,   315,     0,   336,   337,   338,     0,   428,
       0,     0,     0,     0,   311,     0,     0,     0,     0,     0,
       0,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,   316,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   369,     0,     0,   317,
       0,   318,   319,   320,     0,   321,   322,   323,     0,   324,
     325,   326,   327,   328,     0,   329,   330,   331,   332,     0,
     333,   334,   335,     0,   400,   336,   337,   338,   314,     0,
       0,     0,   315,     0,   311,     0,     0,     0,     0,     0,
       0,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,   441,   155,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   317,     0,
     318,   319,   320,   442,   321,   322,   323,     0,   324,   325,
     326,   327,   328,     0,   329,   330,   331,   332,   314,   333,
     334,   335,   315,     0,   336,   337,   338,     0,     0,   311,
       0,     0,     0,     0,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,   528,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,     0,   317,     0,
     318,   319,   320,     0,   321,   322,   323,     0,   324,   325,
     326,   327,   328,     0,   329,   330,   331,   332,     0,   333,
     334,   335,     0,   314,   336,   337,   338,   315,     0,   311,
       0,     0,     0,     0,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,   530,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
     311,   531,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,     0,   317,     0,   318,   319,   320,   313,   321,
     322,   323,     0,   324,   325,   326,   327,   328,     0,   329,
     330,   331,   332,   314,   333,   334,   335,   315,     0,   336,
     337,   338,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,     0,   312,     0,
       0,     0,     0,     0,   314,     0,     0,   316,   315,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   317,     0,   318,   319,   320,     0,   321,
     322,   323,     0,   324,   325,   326,   327,   328,   316,   329,
     330,   331,   332,     0,   333,   334,   335,     0,     0,   336,
     337,   338,     0,     0,   317,   314,   318,   319,   320,   315,
     321,   322,   323,     0,   324,   325,   326,   327,   328,     0,
     329,   330,   331,   332,     0,   333,   334,   335,   311,   432,
     336,   337,   338,     0,     0,   312,     0,     0,     0,   316,
       0,     0,     0,     0,     0,     0,   313,     0,     0,     0,
       0,     0,     0,     0,     0,   317,     0,   318,   319,   320,
       0,   321,   322,   323,     0,   324,   325,   326,   327,   328,
       0,   329,   330,   331,   332,     0,   333,   334,   335,   311,
     434,   336,   337,   338,     0,     0,   312,     0,     0,     0,
       0,     0,   314,     0,     0,     0,   315,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   311,     0,     0,     0,     0,
       0,     0,   312,     0,     0,     0,   316,     0,     0,     0,
       0,     0,     0,   313,     0,     0,     0,     0,     0,     0,
       0,     0,   317,   314,   318,   319,   320,   315,   321,   322,
     323,     0,   324,   325,   326,   327,   328,     0,   329,   330,
     331,   332,     0,   333,   334,   335,   311,   436,   336,   337,
     338,     0,     0,   312,     0,     0,     0,   316,     0,   314,
       0,     0,     0,   315,   313,     0,     0,     0,     0,     0,
       0,     0,   532,   317,     0,   318,   319,   320,     0,   321,
     322,   323,     0,   324,   325,   326,   327,   328,     0,   329,
     330,   331,   332,   316,   333,   334,   335,     0,   444,   336,
     337,   338,     0,     0,     0,     0,     0,     0,     0,   317,
     314,   318,   319,   320,   315,   321,   322,   323,     0,   324,
     325,   326,   327,   328,     0,   329,   330,   331,   332,     0,
     333,   334,   335,   311,     0,   336,   337,   338,     0,     0,
     312,     0,     0,   459,   316,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,     0,     0,     0,     0,
     317,     0,   318,   319,   320,     0,   321,   322,   323,     0,
     324,   325,   326,   327,   328,     0,   329,   330,   331,   332,
       0,   333,   334,   335,   311,     0,   336,   337,   338,     0,
       0,   312,     0,     0,     0,     0,     0,   314,     0,     0,
       0,   315,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   311,     0,     0,     0,     0,
       0,     0,   312,     0,     0,     0,     0,     0,     0,     0,
       0,   316,     0,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   317,   314,   318,
     319,   320,   315,   321,   322,   323,     0,   324,   325,   326,
     327,   328,  -461,   329,   330,   331,   332,     0,   333,   334,
     335,     0,     0,   336,   337,   338,     0,     0,   155,   314,
       0,     0,   316,   315,     0,   311,   695,     0,     0,     0,
       0,     0,   312,     0,     0,     0,     0,     0,   317,     0,
     318,   319,   320,   313,   321,   322,   323,     0,   324,   325,
     326,   327,   328,   316,   329,   330,   331,   332,     0,   333,
     334,   335,     0,     0,   336,   337,   338,     0,     0,   317,
       0,   318,   319,   320,     0,   321,   322,   323,     0,   324,
     325,   326,   327,   328,     0,   329,   330,   331,   332,   314,
     333,   334,   335,   315,   671,   336,   337,   338,     0,     0,
     311,     0,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   313,     0,
       0,   698,     0,   316,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   317,
       0,   318,   319,   320,     0,   321,   322,   323,     0,   324,
     325,   326,   327,   328,     0,   329,   330,   331,   332,     0,
     333,   334,   335,     0,   314,   336,   337,   338,   315,     0,
     311,     0,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   313,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   317,     0,   318,   319,   320,     0,
     321,   322,   323,     0,   324,   325,   326,   327,   328,     0,
     329,   330,   331,   332,   314,   333,   334,   335,   315,     0,
     336,   337,   338,     0,     0,   311,     0,     0,     0,     0,
       0,     0,   312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,   798,     0,     0,   739,   316,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   317,     0,   318,   319,   320,     0,
     321,   322,   323,     0,   324,   325,   326,   327,   328,     0,
     329,   330,   331,   332,     0,   333,   334,   335,     0,   314,
     336,   337,   338,   315,     0,   311,     0,     0,     0,     0,
       0,     0,   312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,   801,     0,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,   311,     0,     0,     0,
       0,     0,     0,   312,     0,     0,     0,     0,     0,   317,
       0,   318,   319,   320,   313,   321,   322,   323,     0,   324,
     325,   326,   327,   328,     0,   329,   330,   331,   332,   314,
     333,   334,   335,   315,     0,   336,   337,   338,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   311,   889,     0,
       0,     0,     0,     0,   312,     0,     0,     0,     0,     0,
     314,     0,     0,   316,   315,   313,     0,     0,     0,     0,
       0,     0,     0,   872,     0,     0,     0,     0,     0,   317,
       0,   318,   319,   320,     0,   321,   322,   323,     0,   324,
     325,   326,   327,   328,   316,   329,   330,   331,   332,     0,
     333,   334,   335,     0,     0,   336,   337,   338,     0,     0,
     317,   314,   318,   319,   320,   315,   321,   322,   323,     0,
     324,   325,   326,   327,   328,     0,   329,   330,   331,   332,
       0,   333,   334,   335,   311,     0,   336,   337,   338,     0,
       0,   312,     0,     0,     0,   316,     0,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,     0,
       0,   317,     0,   318,   319,   320,     0,   321,   322,   323,
       0,   324,   325,   326,   327,   328,     0,   329,   330,   331,
     332,     0,   333,   334,   335,     0,     0,   336,   337,   338,
       0,     0,     0,     0,     0,     0,     0,     0,   314,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,   311,
       0,   875,     0,     0,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   317,     0,
     318,   319,   320,     0,   321,   322,   323,     0,   324,   325,
     326,   327,   328,     0,   329,   330,   331,   332,     0,   333,
     334,   335,     0,   314,   336,   337,   338,   315,     0,     0,
       0,     0,     0,     0,   311,     0,   963,     0,     0,     0,
       0,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   317,     0,   318,   319,   320,     0,   321,
     322,   323,     0,   324,   325,   326,   327,   328,     0,   329,
     330,   331,   332,     0,   333,   334,   335,     0,   314,   336,
     337,   338,   315,     0,     0,     0,     0,     0,     0,   311,
       0,   964,     0,     0,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   317,     0,
     318,   319,   320,     0,   321,   322,   323,     0,   324,   325,
     326,   327,   328,     0,   329,   330,   331,   332,     0,   333,
     334,   335,     0,   314,   336,   337,   338,   315,     0,     0,
       0,     0,     0,     0,   311,     0,   965,     0,     0,     0,
       0,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   317,     0,   318,   319,   320,     0,   321,
     322,   323,     0,   324,   325,   326,   327,   328,     0,   329,
     330,   331,   332,     0,   333,   334,   335,     0,   314,   336,
     337,   338,   315,     0,     0,     0,     0,     0,     0,   311,
       0,   966,     0,     0,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   317,     0,
     318,   319,   320,     0,   321,   322,   323,     0,   324,   325,
     326,   327,   328,     0,   329,   330,   331,   332,     0,   333,
     334,   335,     0,   314,   336,   337,   338,   315,     0,     0,
       0,     0,     0,     0,   311,     0,   967,     0,     0,     0,
       0,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   317,     0,   318,   319,   320,     0,   321,
     322,   323,     0,   324,   325,   326,   327,   328,     0,   329,
     330,   331,   332,     0,   333,   334,   335,     0,   314,   336,
     337,   338,   315,     0,     0,     0,     0,     0,     0,   311,
       0,   968,     0,     0,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   317,     0,
     318,   319,   320,     0,   321,   322,   323,     0,   324,   325,
     326,   327,   328,     0,   329,   330,   331,   332,     0,   333,
     334,   335,     0,   314,   336,   337,   338,   315,     0,   311,
       0,     0,     0,     0,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   317,     0,   318,   319,   320,   313,   321,
     322,   323,     0,   324,   325,   326,   327,   328,     0,   329,
     330,   331,   332,   659,   333,   334,   335,   315,     0,   336,
     337,   338,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   314,     0,     0,   316,   315,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   317,     0,   318,   319,   320,     0,   321,
     322,   323,     0,   324,   325,   326,   327,   328,   316,   329,
     330,   331,   332,     0,   333,   334,   335,     0,     0,   336,
     337,   338,     0,     0,   317,   314,   318,   319,   320,   315,
     321,   322,   323,     0,   324,   325,   326,   327,     0,     0,
     329,   330,   331,   332,     0,   333,   334,   335,     0,     0,
     336,   337,   338,     0,     0,   637,     0,   205,     0,   316,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   206,
     207,     0,     0,     0,     0,   317,   208,   318,   319,   320,
       0,   321,   322,   323,   209,   324,   325,   326,   327,     0,
     210,   329,   330,   331,   332,   211,   333,   212,   335,     0,
     213,   336,   337,   338,     0,     0,   214,     0,     0,     0,
     205,     0,     0,     0,     0,     0,     0,   215,     0,     0,
       0,     0,   206,   207,     0,     0,   216,     0,     0,   208,
       0,     0,     0,     0,     0,   217,   218,   209,   219,     0,
     220,     0,   221,   210,     0,   222,     0,     0,   211,   223,
     212,     0,   224,   213,     0,     0,     0,     0,     0,   214,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     215,     0,     0,     0,     0,     0,     0,     0,     0,   216,
       0,     0,     0,     0,     0,     0,     0,     0,   217,   218,
       0,   219,     0,   220,     0,   221,     0,     0,   222,     0,
       0,     0,   223,     0,     0,   224
};

static const yytype_int16 yycheck[] =
{
      11,   140,    54,    55,    15,   437,   558,    18,     2,   295,
     716,    17,    20,    24,   178,   360,   646,    28,    29,    42,
      31,    32,    33,    34,   128,    28,    42,   191,   741,     1,
     409,    42,     1,    44,    45,    42,    47,   406,    49,   586,
      51,    42,    29,   903,     1,   186,    42,     1,    59,    60,
      61,    62,    63,    64,    29,    59,     1,    68,   204,    70,
      62,    72,    73,   204,   104,    29,    77,    78,   941,    80,
      81,   831,   129,    84,    78,   179,    87,    88,    89,    42,
      91,   104,    29,    94,    95,    96,    42,   793,   129,   789,
     790,    19,    96,   129,   129,   399,   242,    42,   129,    77,
       0,   242,   159,    60,   656,    77,   129,   980,   129,    42,
      47,   658,    75,   129,   149,   261,   157,   104,    90,   159,
     261,   157,   129,    77,   155,   149,   832,   987,   129,   268,
     102,    60,    69,   129,   155,    74,   159,    77,   442,    78,
     109,    31,    75,   159,   101,    73,   859,     3,   102,   160,
     161,   162,   163,   164,   165,   166,   128,   917,   159,   128,
     129,   104,   166,   159,   109,   129,   713,   154,   881,    97,
      42,   177,   101,   129,   128,   186,   154,   149,   414,   154,
      75,   322,   154,   128,   129,   193,   892,     7,   157,   200,
     154,   199,    85,   204,   196,   197,   198,   200,   155,     2,
     154,   437,    19,   159,   149,    77,   585,   154,    11,   149,
     356,   357,    24,   558,   154,   356,   357,   109,   828,   129,
      23,   129,    25,   923,   924,   129,   155,    24,   154,   149,
     102,   242,   243,   235,   940,   155,   238,   109,   868,   250,
     251,   133,   129,   980,   129,    42,   129,   157,    45,   157,
     261,   985,    64,   157,   156,   257,    73,   129,   129,   129,
     994,   998,   149,    75,    61,    68,    95,    64,   820,    66,
     556,   557,   157,   149,   157,   277,   129,   149,    75,   290,
      97,   385,   154,   893,   430,   157,   157,   157,   100,   430,
      74,    94,   149,   287,    78,    98,   149,    94,   155,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   656,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   707,    72,
      73,   129,   129,   156,   345,   346,   705,   623,   624,   129,
     351,   156,     3,   354,   355,   356,   357,   358,   359,   360,
       3,   149,    28,   156,    15,   156,    70,   186,   369,   149,
     157,    22,   149,   374,   129,   376,   129,   128,   370,    24,
     131,   156,   741,   158,   135,   204,   387,    30,   389,   390,
      41,   392,   393,   387,   149,    46,   149,   398,   399,   397,
     401,    52,   403,   149,   396,     2,   149,   139,    74,    52,
     149,   149,    78,   544,    11,   416,   838,    24,    77,    64,
     149,    72,   423,   242,   149,   156,    23,   158,    25,   430,
      75,    90,   225,    84,   314,   315,   156,    88,   158,   232,
     441,   442,   261,   102,    95,    88,   451,   448,   453,    94,
     451,    94,   453,   156,   455,   456,   457,    64,   459,   460,
     614,   131,   128,   154,    39,   131,   467,   149,    75,   135,
     103,    68,   109,   467,   149,    72,    73,   156,   358,   359,
     156,   465,   156,   484,   485,   820,   109,    94,   149,   369,
     149,    60,   129,   100,   287,   154,   133,    94,     7,    74,
      54,    98,   149,    23,    24,   864,   128,   300,   154,   156,
     390,   158,    24,   393,    34,    35,   109,   646,   149,   157,
     157,   129,   881,    28,    44,   149,    95,   528,   529,   530,
     531,   532,   533,   534,    42,   156,    31,   356,   357,   535,
     536,   542,    62,   544,    64,   157,   121,   548,   123,   550,
     129,   157,    64,   554,   109,    75,   157,   558,     2,   156,
     552,   553,   157,    75,   556,   557,    18,    11,   129,    74,
     159,   154,   149,    78,   367,   455,   149,   149,   149,    23,
     100,    25,    94,   149,    77,   159,   587,   157,   129,   590,
     591,   592,   593,   594,   595,   157,   389,    90,   599,   392,
     149,   595,    28,   178,   423,   831,    42,   149,   401,   102,
     149,   430,   838,    31,   157,   155,   191,   149,   149,   194,
     155,   109,   129,   128,    68,   130,   131,    75,   225,   448,
     135,    29,   451,    29,   453,   232,   159,   456,   143,   154,
      42,    77,    31,   128,    75,   150,   151,   152,    74,   529,
      94,   531,    78,   228,    98,   656,   149,   159,   659,   660,
     661,   154,   109,   664,   457,   128,   102,   159,   159,   155,
     131,   246,   465,   109,   156,    77,    77,   669,   670,   680,
     131,   682,   129,    31,   685,   677,   133,   688,   131,    90,
     287,   917,   693,   129,   695,   687,   131,   698,   690,   131,
     102,   102,   128,   300,   130,   131,   131,   109,   131,   135,
     157,   131,   156,   149,   131,   941,   131,   143,   154,   131,
     157,   157,   723,   542,   950,   544,   152,   129,   102,   723,
     157,   155,   525,   734,    90,   736,   300,   159,   739,   868,
     442,   441,   743,    22,   269,   727,   192,   149,   149,   546,
     806,   868,   154,   154,   980,   157,   744,   864,   578,   985,
     705,   940,   747,   987,   959,    33,   544,   417,   994,    -1,
     367,    -1,   998,    -1,    -1,   568,   345,   346,    -1,   659,
     660,   225,   351,    -1,   785,   354,   355,    -1,   232,    -1,
      -1,    -1,   389,    -1,    -1,   392,   797,   798,    -1,   800,
     801,   802,   803,    -1,   401,    -1,    -1,   808,   809,   384,
     811,    -1,   813,   814,   856,   857,   817,    -1,    -1,   820,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   409,   628,   837,   838,    -1,    -1,
      -1,    -1,   843,   287,   845,    -1,    -1,    -1,   849,   841,
      -1,    -1,    -1,   646,   855,    -1,   300,    -1,    -1,    -1,
     457,    -1,    -1,    -1,    -1,    -1,   659,   660,   465,    -1,
      -1,   872,   873,   874,   875,   876,   877,   878,   879,    -1,
      -1,    -1,   451,    -1,   453,    -1,    -1,   680,   889,   682,
      -1,   466,   685,   894,   469,   688,    -1,    -1,   899,    -1,
     894,   902,    -1,    -1,   697,    -1,    -1,   700,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,    24,
      -1,    -1,    -1,   367,    11,    -1,    28,    -1,    -1,   722,
      -1,    -1,   933,   934,   935,   936,    23,    42,    25,    -1,
      45,   734,    -1,   736,    -1,   389,    -1,    -1,   392,   950,
      -1,   952,   953,   954,    -1,    -1,    61,   401,    -1,    64,
     954,    66,   963,   964,   965,   966,   967,   968,    -1,    -1,
      75,   568,    74,   542,    -1,    -1,    78,    -1,   979,   980,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    -1,    -1,   994,    -1,    -1,    -1,   998,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   581,    94,    -1,    -1,
     585,    98,   831,   457,    -1,   808,    -1,    -1,    -1,    -1,
     813,   465,   124,    -1,   126,   127,   128,    -1,   130,   131,
     599,   628,    -1,   135,   827,   610,    -1,    -1,    -1,   614,
     142,   143,    -1,    -1,    -1,   147,   839,    -1,   150,   151,
     152,    -1,    -1,    23,    24,   848,    -1,    -1,    -1,   852,
      -1,     2,    -1,    -1,    34,    35,    -1,    -1,    28,   156,
      11,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
     899,    -1,    23,   680,    25,   682,    -1,    -1,   685,     2,
      -1,   688,    62,    -1,    64,    -1,    -1,   890,    11,    69,
     697,    -1,    -1,   700,    -1,    75,    -1,    -1,    -1,    -1,
      23,    -1,    25,    -1,    74,    -1,    -1,    -1,    78,    -1,
      -1,    -1,   941,    -1,   568,   722,    -1,    68,    -1,   704,
     100,    -1,   707,    -1,    -1,    -1,    -1,   734,   225,   736,
     933,   934,   935,   936,    -1,   232,    -1,    -1,    -1,    -1,
      -1,    -1,   727,    94,    -1,    68,    24,    98,    -1,   952,
     953,   980,    -1,    -1,   124,    -1,   126,   127,   128,   744,
     130,   131,   132,    -1,    42,   135,    -1,    45,    -1,   998,
      -1,    94,   142,   143,   628,    98,    -1,   147,    -1,   764,
     150,   151,   152,    61,    -1,    -1,    64,    -1,    66,    -1,
     287,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,   808,    -1,   300,    -1,   156,   813,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,
     827,    -1,    -1,    -1,    -1,    -1,   680,    -1,   682,    -1,
      -1,   685,   839,   156,   688,    -1,    -1,    -1,    -1,    -1,
      -1,   848,    -1,   697,    19,   852,   700,    -1,    23,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,   722,    44,
     367,    -1,    -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,
     734,   232,   736,   890,    -1,    -1,    -1,    62,    -1,    64,
      -1,    66,   389,    68,    -1,   392,    71,    -1,    73,    -1,
      75,    -1,   225,    -1,   401,    -1,    -1,    -1,    -1,   232,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    -1,    97,    -1,    -1,   100,   933,   934,   935,   936,
      -1,    -1,    -1,    -1,    -1,    -1,   287,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   952,   953,    -1,    -1,   300,
      -1,    -1,    -1,    -1,   808,    -1,    -1,    -1,    -1,   813,
     457,    -1,    -1,    -1,   287,    -1,    -1,    -1,   465,    -1,
       3,    -1,    -1,   827,   149,    -1,    -1,   300,    -1,    -1,
     155,    -1,    15,    16,    -1,   839,    -1,    -1,    -1,    22,
      -1,     2,    -1,    -1,   848,    -1,    -1,    30,   852,    -1,
      11,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,
      43,    -1,    23,    46,    25,    -1,   367,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,   890,    -1,   389,    72,
      -1,   392,    -1,    -1,   367,    -1,    -1,    -1,    81,    82,
     401,    84,    -1,    86,    -1,    88,    -1,    68,    91,    -1,
      -1,    -1,    95,    96,    -1,    98,   389,    -1,    -1,   392,
      -1,   568,    -1,    -1,    28,    -1,    -1,    -1,   401,   933,
     934,   935,   936,    94,    -1,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,   952,   953,
      -1,    -1,    -1,    -1,    -1,    -1,   457,    -1,    28,    -1,
      -1,    -1,    19,    42,   465,    -1,    23,    24,    -1,    -1,
      74,    -1,    -1,   156,    78,    32,    -1,    34,    35,    -1,
      -1,   628,    39,    62,   457,    -1,    -1,    44,    -1,    -1,
      47,    -1,   465,    72,    73,   156,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    62,    -1,    64,    78,    66,
      -1,    68,    69,    -1,    71,    -1,    73,    -1,    75,    -1,
     124,    -1,   126,   127,   128,    -1,   130,   131,    -1,    -1,
      -1,   135,    -1,   680,    -1,   682,    -1,    94,   685,   143,
      97,   688,    -1,   100,    -1,    -1,   150,   151,   152,    -1,
     697,    -1,    -1,   700,   124,    -1,    -1,   127,   128,    -1,
     130,   131,    -1,    -1,   225,   135,    -1,   568,    -1,    -1,
      -1,   232,    -1,   143,    -1,   722,    -1,    -1,    -1,    -1,
     150,   151,   152,    -1,    -1,    -1,    -1,   734,    -1,   736,
      -1,    -1,   149,    -1,    -1,   568,    -1,   176,   155,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   198,
      23,    -1,    25,    -1,   203,    -1,   287,   628,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   300,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   628,   235,    -1,    -1,   238,
      -1,   808,    -1,    -1,    -1,    68,   813,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   257,   680,
     827,   682,    -1,    -1,   685,    -1,    -1,   688,    -1,    -1,
      -1,    94,   839,    -1,    -1,    98,   697,    -1,   277,   700,
      -1,   848,    -1,    -1,    -1,   852,   367,   680,    -1,   682,
      -1,    -1,   685,    -1,    -1,   688,    -1,    -1,    -1,    -1,
      -1,   722,    -1,    -1,   697,    -1,    -1,   700,   389,    -1,
      -1,   392,    -1,   734,    -1,   736,    -1,    -1,    -1,    -1,
     401,    -1,    -1,   890,    -1,     2,    -1,    -1,    -1,   722,
      -1,    -1,    -1,   156,    11,    -1,    -1,    -1,    -1,    -1,
      -1,   734,    -1,   736,    -1,    -1,    23,    -1,    25,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   933,   934,   935,   936,
      -1,   370,    -1,    -1,    -1,    -1,   457,    -1,    -1,   139,
      -1,    -1,    -1,    -1,   465,   952,   953,   808,    -1,    -1,
      -1,    68,   813,   153,    -1,    -1,   156,   396,   158,    -1,
      -1,    -1,   225,    -1,    -1,    -1,   827,    -1,    -1,   232,
      -1,     3,    -1,    -1,    -1,   808,    -1,    94,   839,    -1,
     813,    98,    -1,    15,    16,    -1,    -1,   848,    -1,    -1,
      22,   852,    -1,    -1,   827,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    -1,   839,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,   848,    -1,    -1,    -1,   852,
      52,    -1,    -1,    -1,   287,    -1,    -1,    -1,    -1,   890,
      -1,    63,    28,    -1,    -1,    -1,    -1,   300,    -1,   156,
      72,    -1,    -1,    -1,    -1,    -1,    -1,   568,    -1,    81,
      82,    -1,    84,    -1,    86,    -1,    88,   890,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,   933,   934,   935,   936,    -1,    -1,    74,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   952,   953,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     933,   934,   935,   936,   367,    -1,    -1,   628,   225,    -1,
      -1,    -1,    -1,   552,   553,   232,    -1,   556,   557,   952,
     953,    -1,   154,    -1,   156,    -1,   389,    -1,   124,   392,
     569,    -1,   128,    -1,   130,   131,    -1,    -1,   401,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,   150,   151,   152,    -1,    -1,   680,
      -1,   682,    -1,    -1,   685,    -1,    -1,   688,    -1,    -1,
     287,   610,    -1,    -1,    -1,    -1,   697,    -1,    -1,   700,
      -1,    -1,    -1,   300,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   457,    -1,    -1,    -1,    -1,    -1,
      -1,   722,   465,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   734,    -1,   736,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     669,   670,    -1,    -1,    -1,    -1,    -1,    -1,   677,    -1,
     679,     3,    -1,    -1,    -1,   684,    -1,    -1,   687,    -1,
     367,   690,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,   389,    -1,    36,   392,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,   808,    -1,    -1,
      52,   730,   813,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,   568,   827,    -1,    -1,    -1,
      72,    -1,     1,    -1,    -1,    -1,    -1,    -1,   839,    81,
      82,    -1,    84,    -1,    86,    -1,    88,   848,    -1,    91,
      19,   852,    -1,    95,    23,    24,    98,    -1,    -1,    -1,
     457,    -1,    -1,    32,    -1,    34,    35,    -1,   465,    -1,
      39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,   628,    -1,    -1,    -1,   890,
      -1,    -1,    -1,    62,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   841,    -1,    -1,    94,    -1,    -1,    97,    -1,
      -1,   100,   933,   934,   935,   936,    -1,   680,    -1,   682,
      -1,    -1,   685,    -1,    -1,   688,    -1,    -1,    -1,    -1,
      -1,   952,   953,    -1,   697,    -1,    -1,   700,    -1,    -1,
      -1,    -1,    -1,    -1,   883,   884,    -1,   886,   887,     3,
      -1,   568,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   722,
     149,    15,    16,    -1,   903,    -1,   155,    -1,    22,    -1,
     909,   734,   911,   736,   913,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,   628,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    81,    82,    17,
      84,    -1,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      28,    95,    -1,    -1,    98,   808,    -1,    -1,   987,    -1,
     813,    -1,    -1,    -1,    19,    -1,    -1,    -1,    23,    24,
      -1,    -1,    -1,   680,   827,   682,    -1,    32,   685,    34,
      35,   688,    -1,    -1,    39,    -1,   839,    -1,    -1,    44,
     697,    -1,    -1,   700,    -1,   848,    74,    -1,    -1,   852,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,
      -1,    66,   156,    68,    -1,   722,    71,    -1,    73,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,   734,    -1,   736,
     108,    -1,    -1,    -1,    -1,    -1,    -1,   890,    -1,    94,
      -1,    -1,    97,    -1,    -1,   100,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    -1,   150,   151,   152,    -1,    -1,    -1,    -1,   157,
     933,   934,   935,   936,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,   952,
     953,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     827,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   839,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   848,    -1,    -1,    -1,   852,    -1,    -1,    -1,    -1,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,   890,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    58,    -1,
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    75,    76,    77,    -1,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      -1,    91,    92,    93,    94,    95,    -1,    97,    98,    99,
     100,    -1,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,   149,
      -1,    -1,   152,    -1,   154,    -1,   156,     1,   158,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
     144,    -1,   146,   147,   148,    -1,    -1,    -1,   152,    -1,
     154,    -1,   156,    -1,   158,   159,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    10,    -1,    12,    13,    14,
      15,    16,    17,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    58,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    -1,    91,    92,    93,    94,
      95,    -1,    97,    98,    99,   100,    -1,    -1,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,    -1,   149,   150,   151,   152,    -1,   154,
     155,   156,     1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    -1,    58,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    76,    77,    -1,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    -1,    91,    92,    93,    94,    95,    -1,    97,    98,
      99,   100,    -1,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,   148,
     149,    -1,    -1,   152,    -1,   154,   155,   156,     1,   158,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    -1,    58,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    75,    76,    77,    -1,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    -1,    91,    92,
      93,    94,    95,    -1,    97,    98,    99,   100,    -1,    -1,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,    -1,   149,    -1,    -1,   152,
      -1,   154,   155,   156,     1,   158,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      -1,    58,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    -1,    75,    76,
      77,    -1,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    -1,    91,    92,    93,    94,    95,    -1,
      97,    98,    99,   100,    -1,    -1,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
     147,    -1,   149,    -1,    -1,   152,    -1,   154,   155,   156,
       1,   158,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    -1,    58,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    75,    76,    77,    -1,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    -1,
      91,    92,    93,    94,    95,    -1,    97,    98,    99,   100,
      -1,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,    -1,   146,   147,    -1,   149,    -1,
      -1,   152,    -1,   154,   155,   156,     1,   158,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    58,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      75,    76,    77,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    -1,    91,    92,    93,    94,
      95,    -1,    97,    98,    99,   100,    -1,    -1,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,    -1,   149,    -1,    -1,   152,    -1,   154,
     155,   156,     1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    -1,    58,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    76,    77,    -1,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    -1,    91,    92,    93,    94,    95,    -1,    97,    98,
      99,   100,    -1,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,
     149,    -1,    -1,   152,    -1,   154,    -1,   156,     1,   158,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    -1,    58,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    67,    68,    69,    70,    71,    72,
      73,    -1,    75,    76,    77,    -1,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    -1,    91,    92,
      93,    94,    95,    -1,    97,    98,    99,   100,    -1,    -1,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,    -1,   149,    -1,    -1,   152,
      -1,   154,    -1,   156,     1,   158,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      -1,    58,    -1,    -1,    -1,    62,    63,    64,    65,    -1,
      67,    68,    69,    70,    71,    72,    73,    -1,    75,    76,
      77,    -1,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    -1,    91,    92,    93,    94,    95,    -1,
      97,    98,    99,   100,    -1,    -1,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
     147,    -1,   149,    -1,    -1,   152,    -1,   154,    -1,   156,
       1,   158,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    -1,    58,    -1,    -1,
      -1,    62,    63,    64,    65,    -1,    67,    68,    69,    70,
      71,    72,    73,    -1,    75,    76,    77,    -1,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    -1,
      91,    92,    93,    94,    95,    -1,    97,    98,    99,   100,
      -1,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,    -1,   146,   147,    -1,   149,    -1,
      -1,   152,    -1,   154,    -1,   156,     1,   158,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    58,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    67,    68,    69,    70,    71,    72,    73,    -1,
      75,    76,    77,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    -1,    91,    92,    93,    94,
      95,    -1,    97,    98,    99,   100,    -1,    -1,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,    -1,   149,    -1,    -1,   152,    -1,   154,
      -1,   156,     1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    -1,    58,
      -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    76,    77,    -1,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    -1,    91,    92,    93,    94,    95,    -1,    97,    98,
      99,   100,    -1,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,
     149,    -1,    -1,   152,    -1,   154,    -1,   156,     1,   158,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    -1,    58,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    67,    68,    69,    70,    71,    72,
      73,    -1,    75,    76,    77,    -1,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    -1,    91,    92,
      93,    94,    95,    -1,    97,    98,    99,   100,    -1,    -1,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,    -1,   149,    -1,    -1,   152,
      -1,   154,    -1,   156,     1,   158,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      -1,    58,    -1,    -1,    -1,    62,    63,    64,    65,    -1,
      67,    68,    69,    70,    71,    72,    73,    -1,    75,    76,
      77,    -1,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    -1,    91,    92,    93,    94,    95,    -1,
      97,    98,    99,   100,    -1,    -1,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
     147,    -1,   149,    -1,    -1,   152,    -1,   154,    -1,   156,
       1,   158,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    -1,    58,    -1,    -1,
      -1,    62,    63,    64,    65,    -1,    67,    68,    69,    70,
      71,    72,    73,    -1,    75,    76,    77,    -1,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    -1,
      91,    92,    93,    94,    95,    -1,    97,    98,    99,   100,
      -1,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,    -1,   146,   147,    -1,   149,    -1,
      -1,   152,    -1,   154,    -1,   156,     1,   158,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    58,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    67,    68,    69,    70,    71,    72,    73,    -1,
      75,    76,    77,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    -1,    91,    92,    93,    94,
      95,    -1,    97,    98,    99,   100,    -1,    -1,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,    -1,   149,    -1,    -1,   152,    -1,   154,
      -1,   156,     1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    -1,    58,
      -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    76,    77,    -1,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    -1,    91,    92,    93,    94,    95,    -1,    97,    98,
      99,   100,    -1,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,
     149,    -1,    -1,   152,    -1,   154,    -1,   156,     1,   158,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,   148,    -1,    -1,    -1,   152,
      -1,   154,    -1,   156,    -1,   158,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
     147,   148,    -1,    -1,    -1,   152,    -1,   154,    -1,   156,
      -1,   158,   159,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    84,    85,    86,    -1,    88,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,   147,   148,    -1,
      -1,    -1,   152,    -1,   154,    -1,   156,    -1,   158,   159,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,    -1,    -1,    -1,    -1,   152,
      -1,   154,    -1,   156,    -1,   158,   159,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,    -1,   124,   125,
     126,   127,    -1,    10,    -1,    -1,   132,    -1,    15,    16,
      17,    -1,    -1,    -1,    -1,    22,   142,    -1,   144,    -1,
     146,   147,    -1,    30,    -1,    -1,   152,    -1,   154,    36,
     156,    -1,   158,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    -1,    84,    -1,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,    -1,    -1,    -1,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
     147,    -1,    -1,   150,   151,   152,   153,    -1,    -1,   156,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    19,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    94,    95,    -1,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   142,
      -1,   144,    12,   146,   147,    15,    16,    -1,    -1,   152,
      -1,   154,    22,   156,    -1,   158,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    84,    85,    86,    -1,    88,    -1,
      -1,    91,    92,    93,    -1,    95,    96,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,   147,   148,    -1,
      -1,    -1,   152,    -1,   154,    -1,   156,    -1,   158,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    92,    93,
      -1,    95,    96,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
     144,    -1,   146,   147,   148,    -1,    -1,    -1,   152,    -1,
     154,    -1,   156,   157,   158,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    52,    53,    -1,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    -1,
      88,    -1,    -1,    91,    92,    93,    -1,    95,    96,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,
     148,    -1,    -1,    -1,   152,    -1,   154,    -1,   156,   157,
     158,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      42,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    -1,    88,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    98,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,   144,    -1,   146,   147,    -1,    -1,    -1,    -1,
     152,    -1,   154,    -1,   156,    -1,   158,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    19,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    94,    95,
      -1,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   142,    -1,   144,    12,
     146,   147,    15,    16,    -1,    -1,   152,    -1,   154,    22,
     156,    -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    92,
      93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,    -1,   149,    -1,    -1,   152,
      -1,   154,    -1,   156,    -1,   158,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    92,    93,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
     147,   148,    -1,    -1,    -1,   152,    -1,   154,    -1,   156,
      -1,   158,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    -1,    88,    -1,    -1,
      91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   142,    -1,   144,    12,   146,   147,    15,    16,    -1,
      -1,   152,    -1,   154,    22,   156,    -1,   158,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    52,    53,    -1,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    -1,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   142,    -1,   144,    12,   146,   147,
      15,    16,    -1,    -1,   152,    -1,   154,    22,   156,    -1,
     158,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      85,    86,    -1,    88,    -1,    -1,    91,    92,    93,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,   142,    -1,   144,
      12,   146,   147,    15,    16,    -1,    -1,   152,    -1,   154,
      22,   156,    -1,   158,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    -1,    88,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,   144,    -1,   146,   147,   148,    -1,    -1,    -1,
     152,    -1,   154,   155,   156,    -1,   158,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   142,    -1,   144,    12,
     146,   147,    15,    16,    -1,    -1,   152,    -1,   154,    22,
     156,    -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,   148,    -1,    -1,    -1,   152,
      -1,   154,    -1,   156,    -1,   158,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   142,    -1,   144,    12,   146,
     147,    15,    16,    -1,    -1,   152,    -1,   154,    22,   156,
      -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
     144,    -1,   146,   147,   148,    -1,    -1,    -1,   152,    -1,
     154,    -1,   156,    -1,   158,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    52,    53,    -1,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,
     148,    -1,    -1,    -1,   152,    -1,   154,    -1,   156,    -1,
     158,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    -1,    88,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,   144,    -1,   146,   147,   148,    -1,    -1,    -1,
     152,    -1,   154,    -1,   156,    -1,   158,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   142,    -1,   144,    12,
     146,   147,    15,    16,    -1,    -1,   152,    -1,   154,    22,
     156,    -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   142,
      -1,   144,    12,   146,   147,    15,    16,    -1,    -1,   152,
      -1,   154,    22,   156,    -1,   158,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    84,    85,    86,    -1,    88,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   142,    -1,   144,    12,   146,   147,    15,    16,
      -1,    -1,   152,    -1,   154,    22,   156,    -1,   158,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   142,    -1,   144,    12,   146,
     147,    15,    16,    -1,    -1,   152,    -1,   154,    22,   156,
      -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   142,    -1,
     144,    12,   146,   147,    15,    16,    -1,    -1,   152,    -1,
     154,    22,   156,    -1,   158,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    -1,    88,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   142,    -1,   144,    12,   146,   147,    15,    16,    -1,
      -1,   152,    -1,   154,    22,   156,    -1,   158,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    52,    53,    -1,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   142,    -1,   144,    12,   146,   147,
      15,    16,    -1,    -1,   152,    -1,   154,    22,   156,    -1,
     158,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,   142,    -1,   144,
      12,   146,   147,    15,    16,    -1,    -1,   152,    -1,   154,
      22,   156,    -1,   158,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    -1,    88,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     142,    -1,   144,    12,   146,   147,    15,    16,    -1,    -1,
     152,    -1,   154,    22,   156,    -1,   158,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    -1,    88,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   142,    -1,   144,    12,   146,   147,    15,
      16,    -1,    -1,   152,    -1,   154,    22,   156,    -1,   158,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   142,    -1,   144,    12,
     146,   147,    15,    16,    -1,    -1,   152,    -1,   154,    22,
     156,    -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   142,
      -1,   144,    12,   146,   147,    15,    16,    -1,    -1,   152,
      -1,   154,    22,   156,    -1,   158,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    84,    85,    86,    -1,    88,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   142,    -1,   144,    12,   146,   147,    15,    16,
      -1,    -1,   152,    -1,   154,    22,   156,    -1,   158,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   142,    -1,   144,    12,   146,
     147,    15,    16,    -1,    -1,   152,    -1,   154,    22,   156,
      -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   142,    -1,
     144,    12,   146,   147,    15,    16,    -1,    -1,   152,    -1,
     154,    22,   156,    -1,   158,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    -1,    88,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   142,    -1,   144,    12,   146,   147,    15,    16,    -1,
      -1,   152,    -1,    -1,    22,   156,    -1,   158,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    52,    53,    -1,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   142,    -1,   144,    12,    -1,   147,
      15,    16,    -1,    -1,   152,    -1,   154,    22,   156,    -1,
     158,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    19,
      -1,    -1,    -1,    23,    24,    -1,    81,    82,    83,    84,
      85,    86,    32,    88,    34,    35,    91,    -1,    -1,    39,
      95,    -1,    -1,    98,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    64,    -1,    66,    -1,    68,    -1,
     125,    71,    -1,    73,   129,    75,    -1,    -1,   133,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,   144,
      -1,    15,    16,   148,    94,    -1,    -1,    97,    22,   154,
     100,   156,   157,   158,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,   149,
      -1,    -1,    -1,    77,    -1,   155,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    90,    91,    -1,    -1,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
     144,    22,    -1,   153,    -1,   149,   156,    -1,   158,    30,
     154,    -1,   156,    -1,   158,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    -1,    88,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    98,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,   144,    49,    50,    -1,    52,    53,    -1,
      55,    56,    -1,   154,    -1,   156,    -1,   158,    63,    -1,
      65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,
      95,    -1,    -1,    98,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,   144,
      49,    50,    -1,    52,    53,    -1,    55,    56,    -1,   154,
      -1,   156,    -1,   158,    63,    -1,    65,    -1,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    -1,    88,
      -1,     3,    91,    -1,    -1,    -1,    95,    -1,    10,    98,
      -1,    -1,    -1,    15,    16,    17,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    -1,   125,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,   156,    -1,   158,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    -1,    84,    -1,    86,    -1,    88,    -1,    10,    91,
      -1,    -1,    -1,    95,    -1,    17,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,   125,   126,   127,    -1,    -1,   130,    -1,
      -1,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,    -1,   147,    28,    -1,   150,   151,
     152,   153,    74,    -1,    -1,    -1,    78,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    74,   145,   146,   147,    78,    -1,   150,   151,
     152,    -1,    -1,    10,    -1,    -1,    -1,   159,    -1,    -1,
      17,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    28,   134,   135,   136,   137,   108,    -1,   140,   141,
     142,   143,    -1,   145,    -1,   147,    -1,    -1,   150,   151,
     152,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,    -1,    10,    -1,    -1,    -1,   159,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    74,   145,   146,
     147,    78,    -1,   150,   151,   152,    -1,    -1,    10,    -1,
      -1,    -1,   159,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,    -1,    -1,    -1,
      -1,    -1,   159,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,    -1,    -1,    -1,   157,    -1,    -1,    10,    -1,
      87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,   154,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,   154,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,   154,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,   154,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,   154,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,   154,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,   154,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,    28,    29,
     117,   118,   119,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    42,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    10,    -1,   150,   151,   152,    -1,   154,    17,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    74,   126,   127,   128,    78,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    10,    -1,
     150,   151,   152,    -1,   154,    17,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    10,
      -1,   150,   151,   152,    -1,   154,    17,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    78,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    74,   126,   127,   128,    78,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    28,   150,   151,
     152,    -1,   154,    -1,    74,    -1,    -1,   108,    78,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    28,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,   108,   140,
     141,   142,   143,    74,   145,   146,   147,    78,    -1,   150,
     151,   152,    -1,   154,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    74,   145,   146,   147,    78,    -1,
     150,   151,   152,    -1,   154,    10,    -1,    87,    -1,    -1,
      -1,    -1,    17,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,    28,   135,   136,   137,    -1,   108,   140,
     141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,   150,
     151,   152,    -1,    -1,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    -1,    74,
     150,   151,   152,    78,   154,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,    59,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    74,
     145,   146,   147,    78,    -1,   150,   151,   152,    -1,   154,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,    74,    -1,
      -1,    -1,    78,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    33,   104,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    59,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    74,   145,
     146,   147,    78,    -1,   150,   151,   152,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    74,   150,   151,   152,    78,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      10,    42,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    28,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    74,   145,   146,   147,    78,    -1,   150,
     151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,   108,    78,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,   108,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    -1,   150,
     151,   152,    -1,    -1,   124,    74,   126,   127,   128,    78,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    10,   149,
     150,   151,   152,    -1,    -1,    17,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    10,
     149,   150,   151,   152,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    78,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    74,   126,   127,   128,    78,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    10,   149,   150,   151,
     152,    -1,    -1,    17,    -1,    -1,    -1,   108,    -1,    74,
      -1,    -1,    -1,    78,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,   108,   145,   146,   147,    -1,   149,   150,
     151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      74,   126,   127,   128,    78,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    10,    -1,   150,   151,   152,    -1,    -1,
      17,    -1,    -1,   107,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,    10,    -1,   150,   151,   152,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    78,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    74,   126,
     127,   128,    78,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    -1,   150,   151,   152,    -1,    -1,   104,    74,
      -1,    -1,   108,    78,    -1,    10,    11,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    28,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,   108,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    -1,   150,   151,   152,    -1,    -1,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    74,
     145,   146,   147,    78,   149,   150,   151,   152,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    31,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    -1,    74,   150,   151,   152,    78,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    74,   145,   146,   147,    78,    -1,
     150,   151,   152,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    -1,    74,
     150,   151,   152,    78,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,    28,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    74,
     145,   146,   147,    78,    -1,   150,   151,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    11,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,   108,    78,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,   108,   140,   141,   142,   143,    -1,
     145,   146,   147,    -1,    -1,   150,   151,   152,    -1,    -1,
     124,    74,   126,   127,   128,    78,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,    10,    -1,   150,   151,   152,    -1,
      -1,    17,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    -1,   150,   151,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    74,   150,   151,   152,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    74,   150,
     151,   152,    78,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    74,   150,   151,   152,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    74,   150,
     151,   152,    78,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    74,   150,   151,   152,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    74,   150,
     151,   152,    78,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    74,   150,   151,   152,    78,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    28,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    74,   145,   146,   147,    78,    -1,   150,
     151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,   108,    78,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,   108,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    -1,   150,
     151,   152,    -1,    -1,   124,    74,   126,   127,   128,    78,
     130,   131,   132,    -1,   134,   135,   136,   137,    -1,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    -1,    -1,
     150,   151,   152,    -1,    -1,     1,    -1,     3,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,   124,    22,   126,   127,   128,
      -1,   130,   131,   132,    30,   134,   135,   136,   137,    -1,
      36,   140,   141,   142,   143,    41,   145,    43,   147,    -1,
      46,   150,   151,   152,    -1,    -1,    52,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    15,    16,    -1,    -1,    72,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    81,    82,    30,    84,    -1,
      86,    -1,    88,    36,    -1,    91,    -1,    -1,    41,    95,
      43,    -1,    98,    46,    -1,    -1,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      -1,    84,    -1,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   164,   165,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    29,    30,    32,    34,    35,
      36,    37,    38,    39,    40,    41,    43,    44,    46,    48,
      49,    50,    51,    52,    53,    55,    56,    58,    62,    63,
      64,    65,    66,    67,    68,    71,    72,    73,    75,    76,
      77,    79,    80,    81,    82,    83,    84,    85,    86,    88,
      89,    91,    92,    93,    94,    95,    97,    98,    99,   100,
     103,   105,   108,   124,   125,   126,   127,   132,   142,   144,
     146,   147,   149,   152,   154,   156,   158,   166,   167,   168,
     169,   170,   171,   173,   178,   179,   180,   184,   186,   190,
     191,   192,   193,   194,   195,   197,   198,   199,   203,   204,
     207,   208,   211,   212,   215,   218,   219,   242,   245,   246,
     266,   267,   268,   269,   270,   271,   279,   280,   281,   282,
     285,   286,   287,   288,   289,   290,   291,   293,   294,   295,
     296,   297,   149,   168,   282,   104,   272,   273,     3,   181,
      12,    37,    38,    40,    86,   154,   158,   215,   266,   270,
     280,   281,   282,   285,   287,   288,   272,   282,    94,   245,
      75,   181,   168,   260,   282,   168,   156,   282,   283,     7,
       9,    94,   188,   283,    64,   106,   196,   282,   282,    23,
      35,   246,   282,   282,   156,     3,    15,    16,    22,    30,
      36,    41,    43,    46,    52,    63,    72,    81,    82,    84,
      86,    88,    91,    95,    98,   183,   185,   216,   156,   183,
     247,   248,    29,   173,   189,   282,   282,   282,   282,   282,
     282,     7,   156,    35,   192,   192,   148,   238,   259,   282,
      92,    93,   149,   282,   284,   282,   189,   282,   282,   282,
      85,   156,   168,   282,   282,   173,   180,   282,   285,   173,
     180,   282,   183,   243,   282,   259,   282,   282,   282,   282,
     282,   282,   282,   282,     1,   155,   166,   174,   259,    96,
     133,   238,   261,   262,   284,   259,   282,   292,    66,   168,
     154,    70,   172,   183,   183,    47,    69,   233,   247,   149,
     139,    10,    17,    28,    74,    78,   108,   124,   126,   127,
     128,   130,   131,   132,   134,   135,   136,   137,   138,   140,
     141,   142,   143,   145,   146,   147,   150,   151,   152,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   153,   225,   156,   158,    74,    78,
     156,   168,   149,   282,   282,   282,   259,   154,   173,    42,
     272,   243,   247,   149,   129,   149,   103,   184,   238,   263,
     264,   265,   284,   243,   208,   245,   183,   156,   189,    29,
      42,   189,    29,    42,   104,   189,   275,   283,    33,    59,
     149,    87,   173,   225,   263,   168,   156,   227,    96,   156,
     183,   249,   250,     1,   128,   254,    42,   129,   168,   189,
     189,   263,   183,   129,   149,   282,   282,   149,   154,   189,
     156,   263,   149,   200,   149,   200,   149,   109,   244,   149,
     149,    33,    59,   189,   149,   155,   155,   166,   129,   155,
     282,   129,   157,   129,   157,    42,   129,   159,   275,   107,
     129,   159,     7,     1,   155,   174,    54,   128,   209,   154,
     220,   149,   282,   282,   282,   282,   196,   282,   196,   282,
     282,   282,   282,   282,    16,    98,   282,   282,    30,    52,
      94,   184,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   284,   284,
     284,   284,   284,   263,   263,   196,   282,   196,   282,    24,
      42,    75,   100,   274,   277,   278,   282,   297,    29,    42,
      29,    42,    87,    42,   159,   196,   282,   189,   149,   149,
     282,   282,   109,   157,   129,   149,   183,   247,    42,   259,
      40,   282,   196,   282,    40,   282,   196,   282,   156,   189,
     152,   175,   177,   282,   175,   176,   168,   282,    31,   282,
     157,    24,    42,    45,    61,    64,    75,    94,   167,   228,
     229,   230,   231,   217,   250,   129,   157,    16,    30,    43,
      63,    81,    82,    86,    98,   158,   187,   255,   267,   109,
     251,   282,   248,   157,   238,   282,     1,   205,   263,   157,
      18,   201,   255,   267,   129,   177,   176,   155,   157,   157,
     261,   157,   261,   196,   282,   159,   168,   282,   159,   282,
     159,   282,   155,   155,   183,   259,   154,     1,   183,   213,
     214,    24,    64,    75,    94,   222,   232,   149,   149,   149,
     149,   149,   157,   159,    42,    75,   129,   157,   266,    74,
      74,    40,   196,   282,    40,   196,   282,   196,   282,   272,
     272,   149,   238,   284,   265,   209,   149,   282,   157,   282,
      29,   189,    29,   189,   282,    29,   189,   275,    29,   189,
     275,   276,   277,   129,   149,    11,   149,    31,    31,   168,
      87,   173,    42,    75,   230,   129,   157,   156,   183,    24,
      64,    75,    94,   234,   157,   250,   254,     1,   259,    57,
     284,   155,    60,   101,   155,   206,   157,   156,   173,   183,
     202,   243,   149,   149,   159,   275,   159,   275,   168,   107,
       1,   210,   155,   109,   129,   155,    75,   221,     1,     3,
      10,    15,    17,    22,    36,    41,    46,    52,    72,    84,
      88,    91,    95,   124,   125,   126,   127,   130,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     147,   150,   151,   152,   153,   156,   182,   185,   223,   224,
     225,   266,   277,   254,   266,   266,   282,    29,    29,   282,
      29,    29,   159,   159,   189,   189,   154,   189,    87,    40,
     282,    40,   282,    87,    40,   282,   189,    40,   282,   189,
     129,   157,   282,   282,   168,   282,   168,    31,   183,   229,
     250,   128,   258,    75,   254,   251,   159,    42,   159,    29,
     168,   259,   202,   128,   173,    40,   168,   282,   159,    40,
     168,   282,   159,   282,   155,    35,    68,    71,   155,   167,
     191,   282,   214,   234,   156,   226,   183,   282,   131,   226,
     226,   251,    87,    40,    40,    87,    40,    40,    40,    40,
       1,   210,   282,   282,   282,   282,   282,   282,   277,    11,
      31,   168,   258,   157,   158,   187,   238,   257,   267,   133,
     240,   251,   102,   241,   282,   255,   267,   168,   189,   157,
     282,   282,   168,   282,   168,   191,     1,   128,   253,   228,
     157,     3,    88,   224,   225,   282,   282,   282,   282,   282,
     282,   155,   155,    87,    87,    87,    87,   282,   168,   251,
     258,   159,   259,   238,   239,   282,   282,   173,   190,   237,
     159,   173,    87,    87,   158,   187,   252,   267,    90,   235,
     157,   226,   226,    87,    87,    87,    87,    87,    87,   282,
     282,   282,   282,   240,   251,   238,   256,   257,   267,    42,
     159,   267,   282,   282,     1,   159,   259,   241,   282,   256,
     257,   159,   252,   267,   159,   149,   236,   237,   159,   252,
     267,   256
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   163,   164,   165,   165,   166,   166,   167,   167,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   169,
     170,   170,   170,   171,   171,   171,   172,   172,   173,   173,
     173,   174,   174,   175,   175,   175,   175,   176,   176,   177,
     177,   178,   178,   178,   179,   180,   180,   180,   180,   180,
     181,   181,   182,   182,   182,   182,   182,   183,   183,   183,
     183,   183,   184,   184,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     186,   186,   186,   186,   186,   186,   186,   186,   187,   187,
     187,   187,   187,   187,   187,   187,   188,   188,   189,   189,
     190,   190,   191,   191,   191,   191,   191,   191,   191,   191,
     192,   192,   193,   193,   193,   193,   194,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   196,
     197,   197,   197,   197,   197,   197,   197,   197,   198,   199,
     199,   199,   199,   199,   199,   200,   200,   201,   201,   201,
     202,   202,   203,   204,   204,   205,   205,   206,   206,   206,
     207,   207,   207,   207,   208,   208,   208,   209,   209,   210,
     210,   210,   211,   211,   212,   213,   213,   213,   214,   214,
     216,   217,   215,   218,   218,   218,   218,   218,   220,   221,
     219,   222,   222,   222,   222,   222,   223,   223,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   226,   226,   227,   228,
     228,   228,   229,   229,   229,   229,   229,   230,   230,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   232,   232,
     232,   232,   232,   232,   233,   233,   234,   234,   234,   234,
     234,   234,   235,   235,   236,   236,   237,   237,   238,   238,
     239,   239,   240,   240,   241,   241,   242,   242,   242,   243,
     243,   244,   244,   244,   245,   245,   245,   245,   245,   246,
     246,   246,   247,   247,   248,   248,   249,   249,   249,   250,
     250,   250,   251,   251,   251,   252,   252,   252,   252,   252,
     252,   252,   253,   253,   253,   253,   253,   254,   254,   254,
     254,   254,   255,   255,   255,   255,   256,   256,   256,   257,
     257,   257,   257,   257,   258,   258,   258,   258,   258,   259,
     259,   259,   259,   260,   260,   261,   261,   261,   262,   262,
     263,   263,   264,   264,   265,   265,   265,   265,   266,   266,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   269,   270,   271,   271,   271,   271,   271,
     271,   271,   271,   272,   272,   273,   274,   274,   275,   276,
     276,   277,   277,   277,   278,   278,   278,   278,   278,   278,
     279,   279,   280,   281,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     283,   283,   284,   284,   284,   285,   285,   285,   285,   286,
     286,   287,   287,   287,   288,   288,   288,   288,   289,   289,
     289,   289,   290,   290,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   292,   292,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   294,   294,   294,   294,   294,   294,   295,   295,   295,
     295,   296,   296,   296,   296,   297,   297,   297,   297,   297,
     297,   297
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     2,     2,     3,     2,     4,
       3,     4,     4,     0,     1,     1,     0,     1,     2,     3,
       3,     1,     2,     1,     3,     3,     5,     0,     1,     1,
       1,     3,     5,     5,     3,     4,     4,     4,     4,     4,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     2,     1,
       2,     3,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     3,     5,     5,     2,     2,     5,     3,     6,
       6,     4,     5,     5,     3,     3,     6,     5,     6,     5,
       6,     3,     4,     6,     7,     6,     7,     4,     5,     4,
       4,     3,     6,     5,     6,     5,     8,     7,     2,     3,
       3,     2,     2,     3,     3,     0,     2,     2,     3,     5,
       1,     3,     3,     5,     5,     0,     2,     3,     2,     3,
       6,     8,     6,     8,     1,     1,     1,     0,     2,     0,
       2,     3,     5,     5,     1,     1,     2,     3,     1,     3,
       0,     0,     8,     0,     1,     2,     2,     1,     0,     0,
      10,     3,     3,     5,     5,     3,     1,     3,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     3,     0,
       1,     3,     4,     5,     4,     6,     6,     0,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     0,     1,
       1,     2,     1,     1,     1,     1,     0,     1,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     2,     1,
       0,     1,     2,     2,     0,     2,     3,     4,     4,     2,
       4,     0,     2,     2,     1,     1,     1,     2,     1,     4,
       5,     3,     1,     3,     3,     5,     1,     1,     3,     1,
       2,     3,     0,     2,     2,     3,     2,     4,     3,     3,
       4,     3,     0,     2,     2,     2,     1,     0,     2,     2,
       2,     1,     4,     4,     6,     3,     0,     1,     1,     3,
       4,     3,     4,     6,     0,     2,     2,     2,     2,     1,
       1,     3,     3,     1,     3,     1,     1,     1,     3,     3,
       0,     1,     1,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     4,     4,     4,     5,
       2,     2,     2,     2,     2,     2,     6,     6,     4,     9,
       9,     7,     6,     6,     4,     9,     9,     7,     4,     6,
       6,     9,     9,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     1,     4,     1,     3,     4,     1,
       3,     4,     3,     3,     1,     1,     2,     1,     2,     1,
       1,     3,     2,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     3,     3,     3,     2,     2,     1,
       0,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     4,     3,     3,     3,     3,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     3,     4,     3,     4,     3,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1
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
#line 471 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 5455 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5461 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 477 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5467 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5473 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 489 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5479 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 490 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5485 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 509 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5491 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5497 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 511 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5503 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 512 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5509 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 513 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5515 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5521 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5527 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5533 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 517 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5539 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 518 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5545 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5551 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 520 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5557 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5563 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5569 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 523 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5575 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 524 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5581 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 529 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 5595 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 542 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 5604 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 547 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 5613 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 552 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 5622 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5628 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 560 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5634 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 561 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5640 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 5646 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 5652 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 580 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5664 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5676 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 587 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5682 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5692 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 597 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 602 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 606 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5720 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 613 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5730 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5740 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5746 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5752 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5758 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5764 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 5770 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 644 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5776 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5782 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 648 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5788 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 650 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5794 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5800 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 657 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5806 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 661 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5812 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 662 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "this"; }
#line 5818 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 663 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 5824 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 664 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 5830 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 665 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 5836 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 668 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5842 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 669 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 5848 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 670 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 5854 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 671 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 5860 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 672 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 5866 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 684 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5872 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 685 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "this"; }
#line 5878 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "bool"; }
#line 5884 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "int"; }
#line 5890 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "uint"; }
#line 5896 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 699 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "real"; }
#line 5902 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 700 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "imag"; }
#line 5908 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 701 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "complex"; }
#line 5914 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 702 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "string"; }
#line 5920 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 703 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "sync"; }
#line 5926 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 704 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "single"; }
#line 5932 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 705 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "owned"; }
#line 5938 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 706 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "shared"; }
#line 5944 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "borrowed"; }
#line 5950 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 708 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "unmanaged"; }
#line 5956 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 709 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "domain"; }
#line 5962 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 710 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "index"; }
#line 5968 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 711 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "locale"; }
#line 5974 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 715 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 5980 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 716 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 5986 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 717 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 5992 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 718 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 5998 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 719 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6004 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 720 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6010 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 721 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6016 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 722 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6022 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 729 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_syncvar"; }
#line 6028 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 730 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_singlevar"; }
#line 6034 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 731 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_owned"; }
#line 6040 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 732 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_shared"; }
#line 6046 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 733 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_borrowed"; }
#line 6052 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 734 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_unmanaged"; }
#line 6058 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 735 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_domain"; }
#line 6064 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 736 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_index"; }
#line 6070 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 741 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 6076 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 742 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6082 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 746 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6088 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 747 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6094 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 751 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6100 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6106 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 756 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6112 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 758 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6118 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 759 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6124 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6130 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 773 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6136 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 774 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6142 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 775 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6148 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 780 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6156 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 786 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6162 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 787 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6168 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 788 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6174 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 789 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6180 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 790 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6186 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 791 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6192 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 792 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6198 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 793 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6204 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 794 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6210 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 795 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6216 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 796 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 6222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 797 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 6228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 798 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 6234 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 799 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 6240 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 800 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 6246 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 801 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 6252 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 803 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6262 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 809 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 815 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 6282 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 821 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 6292 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 827 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6303 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 834 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6314 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 843 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 6320 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 847 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6326 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 848 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6332 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 849 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6338 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 850 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6344 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 851 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6351 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 853 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6358 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 855 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6365 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 857 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 862 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 865 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 866 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 867 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 868 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 869 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 870 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 874 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 875 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 879 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 880 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 881 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6444 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 885 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 886 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 890 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6462 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 894 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6468 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 896 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6474 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 900 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6480 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 901 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6486 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 906 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6492 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 908 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6498 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 910 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6504 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 917 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 927 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6532 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 937 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6546 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 947 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 959 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 964 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6580 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 969 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6590 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 977 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 978 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6602 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 983 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6608 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 985 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6614 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 987 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6620 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 992 "chapel.ypp" /* yacc.c:1661  */
    {
      EnumType* pdt = (yyvsp[-4].penumtype);
      for_vector(DefExpr, ec, *(yyvsp[-1].pvecOfDefs)) {
        ec->sym->type = pdt;
        pdt->constants.insertAtTail(ec);
        if (pdt->defaultValue == NULL) {
          pdt->defaultValue = ec->sym;
        }
      }
      delete (yyvsp[-1].pvecOfDefs);
      pdt->doc = (yylsp[-4]).comment;
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-4].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 6640 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1008 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1015 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1024 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1032 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1036 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6686 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1042 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6692 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1043 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6698 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1048 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6707 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1053 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6716 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1058 "chapel.ypp" /* yacc.c:1661  */
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
#line 6736 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1078 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1084 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1091 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6770 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1097 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6781 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1103 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6793 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1114 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 6803 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1120 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6814 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1127 "chapel.ypp" /* yacc.c:1661  */
    {
      FnSymbol* fn = (yyvsp[-6].pfnsymbol);
      FnSymbol* linkageFn = (yyvsp[-9].pfnsymbol);

      fn->copyFlags((yyvsp[-9].pfnsymbol));
      if (*linkageFn->name)
        // The user explicitly named this function (controls mangling).
        fn->cname = linkageFn->name;
      else if (linkageFn->numFormals() == 1)
        // cname should be set based upon param
        fn->insertFormalAtTail(linkageFn->getFormal(1));

      if ((yyvsp[-8].procIter) == ProcIter_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[-6].pfnsymbol), (yyvsp[-4].retTag), (yyvsp[-3].pexpr), (yyvsp[-2].b), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), (yylsp[-9]).comment);
      context->latestComment = NULL;
    }
#line 6841 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1153 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 6849 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1157 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1162 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 6866 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1166 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6875 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1171 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 6883 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1178 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6889 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1182 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6895 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1183 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 6901 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1184 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 6907 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1185 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 6913 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1186 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 6919 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1187 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 6925 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1188 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 6931 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1189 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 6937 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1190 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 6943 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1191 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 6949 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1192 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 6955 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1193 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 6961 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 6967 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1195 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 6973 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1196 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 6979 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1197 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 6985 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 6991 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1199 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 6997 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1200 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 7003 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1201 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 7009 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1202 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 7015 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1203 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 7021 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1204 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 7027 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1205 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 7033 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1206 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 7039 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1207 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 7045 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1211 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 7051 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1212 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 7057 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1213 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 7063 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1214 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 7069 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1215 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 7075 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1216 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 7081 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1217 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 7087 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1218 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 7093 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1219 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 7099 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1220 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 7105 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 7111 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1222 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 7117 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1226 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7123 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1227 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7129 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1231 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7135 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1235 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7141 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1236 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7147 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1237 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7153 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1242 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7159 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1244 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7165 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1246 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7171 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1248 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7177 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1250 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7183 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1254 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7189 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1255 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7195 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1259 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 7201 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1260 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7207 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1261 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 7213 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1262 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 7219 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1263 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7225 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1264 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7231 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1265 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7237 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1266 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 7243 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1267 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7249 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1271 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7255 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1272 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7261 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1273 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 7267 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1274 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 7273 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1275 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST;   }
#line 7279 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1276 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 7285 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1280 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 7291 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1281 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 7297 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1285 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 7303 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1286 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 7309 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1287 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 7315 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1288 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 7321 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1289 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 7327 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1290 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 7333 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1294 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 7339 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1295 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 7345 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1298 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 7351 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1304 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7357 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1309 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7363 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1311 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7369 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1315 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7375 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1320 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7381 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1321 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7387 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1325 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7393 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1326 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7399 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1331 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7405 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1333 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7411 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1335 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7417 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1340 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7434 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1353 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7452 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1369 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7458 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1371 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1373 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7470 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1377 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 7476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1378 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 7482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1379 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 7488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1380 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 7494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1381 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 7500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1386 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7510 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1392 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
      context->latestComment = NULL;
    }
#line 7520 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1398 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7529 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1407 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7538 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1415 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7544 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1417 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7550 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1422 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7556 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1424 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7562 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1426 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7568 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1431 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7574 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1433 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7580 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1435 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7586 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1441 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7592 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1442 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7598 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1443 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7604 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1449 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7610 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1451 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7616 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1453 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1457 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7632 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1461 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7638 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1463 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7646 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1467 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7654 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1474 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1475 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7672 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1477 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7684 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1483 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 7702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7708 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7714 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7722 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1512 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 7730 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1516 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 7742 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1524 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7756 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7762 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1532 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7768 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7774 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1539 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7780 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1545 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7786 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1547 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1549 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 7798 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7810 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1556 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1557 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7834 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7840 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1565 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1566 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7852 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1570 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 7858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1571 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 7870 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 386:
#line 1576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7876 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 387:
#line 1577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7882 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 388:
#line 1581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7888 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1582 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7894 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 7900 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1591 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7906 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1592 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7912 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 7918 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1597 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 7924 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7930 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1599 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7936 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1603 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7942 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7948 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 7954 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 7960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1625 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 7966 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 7972 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 7978 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 7984 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 7990 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 7996 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8002 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 8008 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8014 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8020 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1648 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8026 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1650 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8032 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1652 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8038 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1654 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8044 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8050 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1658 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8056 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1660 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8062 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1662 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8068 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1664 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8074 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1666 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8080 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1668 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8086 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1670 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8097 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1677 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8107 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1683 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8117 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1689 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8127 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1695 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 8137 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1704 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8143 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1713 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 8149 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1729 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8155 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1733 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 8161 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1738 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8167 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1742 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8173 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1743 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8179 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1747 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8185 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1751 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8191 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1752 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8197 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1757 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 8205 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1761 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8213 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 453:
#line 1765 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8221 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 454:
#line 1771 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 8227 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 455:
#line 1772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8233 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 456:
#line 1773 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8239 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 457:
#line 1774 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8245 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 458:
#line 1775 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8251 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 459:
#line 1776 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8257 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 461:
#line 1782 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8263 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 462:
#line 1787 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8269 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 463:
#line 1792 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8275 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 472:
#line 1808 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8281 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 473:
#line 1810 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8287 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 474:
#line 1812 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 8293 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 475:
#line 1814 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8299 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 476:
#line 1816 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8305 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 477:
#line 1818 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8311 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 478:
#line 1820 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8317 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 479:
#line 1822 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8323 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 480:
#line 1826 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 8329 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 481:
#line 1827 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8335 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 482:
#line 1830 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8341 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 483:
#line 1831 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8347 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 484:
#line 1832 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8353 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 491:
#line 1854 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8359 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 492:
#line 1855 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8365 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 493:
#line 1856 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8371 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 494:
#line 1860 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8377 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 495:
#line 1861 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8383 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 496:
#line 1862 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8389 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 497:
#line 1863 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 8395 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 498:
#line 1871 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8401 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 499:
#line 1872 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8407 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 500:
#line 1873 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8413 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 501:
#line 1874 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8419 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 502:
#line 1878 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 8425 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 503:
#line 1879 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 8431 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 505:
#line 1884 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 8437 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 506:
#line 1885 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8443 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 507:
#line 1886 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8449 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 508:
#line 1887 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8455 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 509:
#line 1888 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8461 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 510:
#line 1889 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8467 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 511:
#line 1890 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8473 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 512:
#line 1891 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8479 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 513:
#line 1892 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8485 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 514:
#line 1894 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8493 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 515:
#line 1898 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8501 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 516:
#line 1905 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8507 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 517:
#line 1906 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8513 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 518:
#line 1910 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8519 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 519:
#line 1911 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8525 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 520:
#line 1912 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8531 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 521:
#line 1913 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8537 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 522:
#line 1914 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8543 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 523:
#line 1915 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8549 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 524:
#line 1916 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8555 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 525:
#line 1917 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8561 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 526:
#line 1918 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8567 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 527:
#line 1919 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8573 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 528:
#line 1920 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8579 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 529:
#line 1921 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8585 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 530:
#line 1922 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8591 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 531:
#line 1923 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8597 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 532:
#line 1924 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8603 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 533:
#line 1925 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8609 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 534:
#line 1926 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8615 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 535:
#line 1927 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8621 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 536:
#line 1928 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8627 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 537:
#line 1929 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8633 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 538:
#line 1930 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8639 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 539:
#line 1931 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8645 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 540:
#line 1932 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8651 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 541:
#line 1936 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 8657 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 542:
#line 1937 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 8663 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 543:
#line 1938 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 8669 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 544:
#line 1939 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 8675 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 545:
#line 1940 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 8681 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 546:
#line 1941 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 8687 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 547:
#line 1945 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8693 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 548:
#line 1946 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 549:
#line 1947 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8705 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 550:
#line 1948 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 551:
#line 1952 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 552:
#line 1953 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8723 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 553:
#line 1954 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 554:
#line 1955 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 555:
#line 1960 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 8741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 556:
#line 1961 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 8747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 557:
#line 1962 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 8753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 558:
#line 1963 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 8759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 559:
#line 1964 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 8765 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 560:
#line 1965 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 8771 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 561:
#line 1966 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 8777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 8781 "bison-chapel.cpp" /* yacc.c:1661  */
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
