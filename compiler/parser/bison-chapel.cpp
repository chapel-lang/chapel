/* A Bison parser, made by GNU Bison 3.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.1"

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

  struct WhereAndLifetime {
    Expr* where;
    Expr* lifetime;
  };
  static inline
  WhereAndLifetime makeWhereAndLifetime(Expr* w, Expr* lt) {
    WhereAndLifetime ret;
    ret.where = w;
    ret.lifetime = lt;
    return ret;
  }

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
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif
#line 145 "chapel.ypp" /* yacc.c:355  */

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
#line 168 "chapel.ypp" /* yacc.c:355  */

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

#line 238 "bison-chapel.cpp" /* yacc.c:355  */

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
    TINITEQUALS = 303,
    TLABEL = 304,
    TLAMBDA = 305,
    TLET = 306,
    TLIFETIME = 307,
    TLOCAL = 308,
    TLOCALE = 309,
    TMINUSMINUS = 310,
    TMODULE = 311,
    TNEW = 312,
    TNIL = 313,
    TNOINIT = 314,
    TNOTHING = 315,
    TON = 316,
    TONLY = 317,
    TOTHERWISE = 318,
    TOUT = 319,
    TOVERRIDE = 320,
    TOWNED = 321,
    TPARAM = 322,
    TPLUSPLUS = 323,
    TPRAGMA = 324,
    TPRIMITIVE = 325,
    TPRIVATE = 326,
    TPROC = 327,
    TPROTOTYPE = 328,
    TPUBLIC = 329,
    TREAL = 330,
    TRECORD = 331,
    TREDUCE = 332,
    TREF = 333,
    TREQUIRE = 334,
    TRETURN = 335,
    TSCAN = 336,
    TSELECT = 337,
    TSERIAL = 338,
    TSHARED = 339,
    TSINGLE = 340,
    TSPARSE = 341,
    TSTRING = 342,
    TSUBDOMAIN = 343,
    TSYNC = 344,
    TTHEN = 345,
    TTHIS = 346,
    TTHROW = 347,
    TTHROWS = 348,
    TTRUE = 349,
    TTRY = 350,
    TTRYBANG = 351,
    TTYPE = 352,
    TUINT = 353,
    TUNDERSCORE = 354,
    TUNION = 355,
    TUNMANAGED = 356,
    TUSE = 357,
    TVAR = 358,
    TWHEN = 359,
    TWHERE = 360,
    TWHILE = 361,
    TWITH = 362,
    TYIELD = 363,
    TZIP = 364,
    TALIAS = 365,
    TAND = 366,
    TASSIGN = 367,
    TASSIGNBAND = 368,
    TASSIGNBOR = 369,
    TASSIGNBXOR = 370,
    TASSIGNDIVIDE = 371,
    TASSIGNEXP = 372,
    TASSIGNLAND = 373,
    TASSIGNLOR = 374,
    TASSIGNMINUS = 375,
    TASSIGNMOD = 376,
    TASSIGNMULTIPLY = 377,
    TASSIGNPLUS = 378,
    TASSIGNREDUCE = 379,
    TASSIGNSL = 380,
    TASSIGNSR = 381,
    TBAND = 382,
    TBNOT = 383,
    TBOR = 384,
    TBXOR = 385,
    TCOLON = 386,
    TCOMMA = 387,
    TDIVIDE = 388,
    TDOT = 389,
    TDOTDOT = 390,
    TDOTDOTDOT = 391,
    TEQUAL = 392,
    TEXP = 393,
    TGREATER = 394,
    TGREATEREQUAL = 395,
    THASH = 396,
    TIO = 397,
    TLESS = 398,
    TLESSEQUAL = 399,
    TMINUS = 400,
    TMOD = 401,
    TNOT = 402,
    TNOTEQUAL = 403,
    TOR = 404,
    TPLUS = 405,
    TQUESTION = 406,
    TSEMI = 407,
    TSHIFTLEFT = 408,
    TSHIFTRIGHT = 409,
    TSTAR = 410,
    TSWAP = 411,
    TLCBR = 412,
    TRCBR = 413,
    TLP = 414,
    TRP = 415,
    TLSBR = 416,
    TRSBR = 417,
    TNOELSE = 418,
    TUPLUS = 419,
    TUMINUS = 420
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
#line 199 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 449 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 455 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 38 "chapel.ypp" /* yacc.c:359  */

  #include <string>
  int         captureTokens;
  std::string captureString;
#line 207 "chapel.ypp" /* yacc.c:359  */

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

#line 529 "bison-chapel.cpp" /* yacc.c:359  */

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
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
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
#  define YYSIZE_T unsigned
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

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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
#define YYLAST   16668

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  166
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  138
/* YYNRULES -- Number of rules.  */
#define YYNRULES  581
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1040

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   420

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

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
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   487,   487,   492,   493,   499,   500,   505,   506,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   544,
     557,   562,   567,   575,   576,   577,   581,   582,   595,   596,
     597,   602,   603,   608,   613,   618,   622,   629,   634,   638,
     643,   647,   648,   649,   653,   657,   659,   661,   663,   665,
     672,   673,   677,   678,   679,   680,   681,   684,   685,   686,
     687,   688,   700,   701,   712,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,   723,   724,   725,   726,   727,
     731,   732,   733,   734,   735,   736,   737,   738,   745,   746,
     747,   748,   749,   750,   751,   752,   757,   758,   762,   763,
     767,   768,   772,   773,   774,   775,   776,   777,   778,   779,
     783,   784,   788,   789,   790,   791,   795,   802,   803,   804,
     805,   806,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,   817,   818,   819,   820,   826,   832,   838,   844,
     851,   858,   862,   869,   873,   874,   875,   876,   877,   879,
     881,   883,   888,   891,   892,   893,   894,   895,   896,   900,
     901,   905,   906,   907,   911,   912,   916,   919,   921,   926,
     927,   931,   933,   935,   942,   952,   962,   972,   985,   990,
     995,  1003,  1004,  1009,  1010,  1012,  1017,  1033,  1040,  1049,
    1057,  1061,  1068,  1069,  1074,  1079,  1073,  1106,  1112,  1119,
    1125,  1131,  1142,  1148,  1141,  1180,  1184,  1189,  1193,  1198,
    1205,  1206,  1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,
    1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,
    1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1240,
    1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,
    1251,  1255,  1256,  1260,  1264,  1265,  1266,  1270,  1272,  1274,
    1276,  1278,  1283,  1284,  1288,  1289,  1290,  1291,  1292,  1293,
    1294,  1295,  1296,  1300,  1301,  1302,  1303,  1304,  1305,  1309,
    1310,  1314,  1315,  1316,  1317,  1318,  1319,  1323,  1324,  1327,
    1328,  1332,  1333,  1337,  1339,  1344,  1345,  1349,  1350,  1354,
    1355,  1357,  1359,  1361,  1366,  1367,  1370,  1371,  1372,  1373,
    1374,  1375,  1376,  1379,  1380,  1383,  1385,  1387,  1392,  1405,
    1422,  1423,  1425,  1430,  1431,  1432,  1433,  1434,  1438,  1444,
    1450,  1458,  1459,  1467,  1469,  1474,  1476,  1478,  1483,  1485,
    1487,  1494,  1495,  1496,  1501,  1503,  1505,  1509,  1513,  1515,
    1519,  1527,  1528,  1529,  1530,  1531,  1532,  1537,  1538,  1539,
    1540,  1541,  1561,  1565,  1569,  1577,  1584,  1585,  1586,  1590,
    1592,  1598,  1600,  1602,  1607,  1608,  1609,  1610,  1611,  1617,
    1618,  1619,  1620,  1624,  1625,  1629,  1630,  1631,  1635,  1636,
    1640,  1641,  1645,  1646,  1650,  1651,  1652,  1653,  1657,  1658,
    1669,  1670,  1671,  1672,  1673,  1674,  1676,  1678,  1680,  1682,
    1684,  1686,  1688,  1690,  1692,  1694,  1699,  1701,  1703,  1705,
    1707,  1709,  1711,  1713,  1715,  1717,  1719,  1721,  1723,  1730,
    1736,  1742,  1748,  1757,  1767,  1775,  1776,  1777,  1778,  1779,
    1780,  1781,  1782,  1787,  1788,  1792,  1796,  1797,  1801,  1805,
    1806,  1810,  1814,  1818,  1825,  1826,  1827,  1828,  1829,  1830,
    1834,  1835,  1840,  1845,  1853,  1854,  1855,  1856,  1857,  1858,
    1859,  1860,  1861,  1863,  1865,  1867,  1869,  1871,  1873,  1875,
    1880,  1881,  1884,  1885,  1886,  1889,  1890,  1891,  1892,  1903,
    1904,  1908,  1909,  1910,  1914,  1915,  1916,  1917,  1925,  1926,
    1927,  1928,  1932,  1933,  1937,  1938,  1939,  1940,  1941,  1942,
    1943,  1944,  1945,  1946,  1947,  1951,  1959,  1960,  1964,  1965,
    1966,  1967,  1968,  1969,  1970,  1971,  1972,  1973,  1974,  1975,
    1976,  1977,  1978,  1979,  1980,  1981,  1982,  1983,  1984,  1985,
    1986,  1990,  1991,  1992,  1993,  1994,  1995,  1999,  2000,  2001,
    2002,  2006,  2007,  2008,  2009,  2014,  2015,  2016,  2017,  2018,
    2019,  2020
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
  "TINOUT", "TINT", "TITER", "TINITEQUALS", "TLABEL", "TLAMBDA", "TLET",
  "TLIFETIME", "TLOCAL", "TLOCALE", "TMINUSMINUS", "TMODULE", "TNEW",
  "TNIL", "TNOINIT", "TNOTHING", "TON", "TONLY", "TOTHERWISE", "TOUT",
  "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE",
  "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC", "TREAL", "TRECORD",
  "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL",
  "TSHARED", "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN", "TSYNC",
  "TTHEN", "TTHIS", "TTHROW", "TTHROWS", "TTRUE", "TTRY", "TTRYBANG",
  "TTYPE", "TUINT", "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUSE", "TVAR",
  "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND",
  "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
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
  "opt_lifetime_where", "lifetime_components_expr", "lifetime_expr",
  "lifetime_ident", "type_alias_decl_stmt", "type_alias_decl_stmt_inner",
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
     415,   416,   417,   418,   419,   420
};
# endif

#define YYPACT_NINF -912

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-912)))

#define YYTABLE_NINF -533

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -912,    99,  2951,  -912,   -42,  -912,  -912,  -912,  -912,  -912,
    -912,  4385,    15,   164,  -912,  9769,  -912,    15,  9769,  -912,
     403,   130,   164,  4385,  9769,  4385,    76,  -912,  9769,  6773,
    -912,  8135,  8725,  6614,  9769,  -912,    85,  -912,  -912, 16567,
    -912,  2636,  8861,  -912,  9769,  9769,  -912,  9769,  -912,  9769,
    -912,  9769,   255,   111,  1040,  1040,  -912,  -912,  -912,  8997,
    7681,  9769,  8861,  9769,  9769,   208,  -912,   138,  4385,  -912,
    9769,  -912,  9905,  9905, 16567,  -912,  -912,  9769,  8997,  -912,
    9769,  9769,  -912,  -912,  9769,  -912,  -912, 11809,  9769,  9769,
    -912,  9769,  -912,  -912,  3431,  6909,  8271,  -912,  4226,  -912,
     181,  -912,   266,  -912,  -912,  -912,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,
    -912, 16567,  -912, 16567,   326,   203,  -912,  -912,  2636,  -912,
     200,  -912,  -912,  -912,   216,   248,   222,   296,   297, 16270,
   12073,   271,   305,   319,  -912,  -912,  -912,  -912,  -912,  -912,
    -912,    20,  -912,  -912, 16270,   317,  4385,  -912,  -912,   336,
    9769,  9769,  9769,  9769,  9769,  8997,  8997,   281,  -912,  -912,
    -912,  -912,   273,   333,  -912,  -912,   329, 14170, 16567,  2636,
    -912,   339,  -912,   139, 16270,   390,  7840, 16270,  -912,   155,
    -912, 16567,   257,    40, 16567,   348,    27, 13691,    31, 13640,
      40,  9769,  -912, 14091, 12954,  7840,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  4385,  -912,   350,  2258,
      29,    48,  -912,  4385,  -912,  -912, 13782,   327,   273, 13782,
     273,   327,  -912,  7840,  9769,  -912,  -912, 16567,  -912,   211,
   16270,  9769,  9769,  -912, 16270,   346, 14021,  -912, 13782,   273,
   16270,   352,  7840,  -912, 16270, 14468,  -912,  -912, 14538, 12195,
    -912,  -912, 14617,   400,   361,   273,   221, 14240, 13782, 14687,
      92,  2733,   327,    92,   327,   121,  -912,  -912,  3590,   -34,
    -912,  9769,  -912,   -37,   134,  -912,   -35,    95, 14766,    96,
     507,  -912,  3749,  -912,   461,   388,   365,  -912,  -912,  -912,
     243,  -912,  9769,  9769,  9769,  9769,  8725,  8725,  9769,  9769,
    9769,  9769, 10041,  9769,   331, 11809,  9769,  9769,  9769,  9769,
    9769,  9769,  9769,  9769,  9769,  9769,  9769,  9769,  9769,  9769,
    9769,  -912,  -912,  -912,  -912,  -912,  -912,  8430,  8430,  -912,
    -912,  -912,  -912,  8430,  -912,  -912,  8430,  8430,  7840,  7840,
    8725,  8725,  7386,  -912,  -912, 14319, 14389, 14836,    72,  3908,
    -912,  8725,    27,   369,   293,  -912,  9769,  -912,  9769,   419,
    -912,   373,   402,  -912,  -912,   386, 16567,  2636,   499,  8997,
    -912,  4544,  8725,  -912,   383,  -912,    27,  4703,  8725,  -912,
      27,    40, 10177,  9769,  -912,  4385,   516,  9769,   394,  -912,
     572,  -912,  -912,  2258,  -912,   418,   397,  -912, 10313,   446,
    9769,  2636,  -912,  -912,  -912,   399,  -912,  8997,  -912, 16270,
   16270,  -912,    43,  -912,  7840,   406,  -912,   549,  -912,   549,
    -912, 10449,   441,  -912,  -912, 10177,  9769,  -912,  -912,  -912,
    -912,  -912,  8566,  -912, 12813,  7068,  -912,  7227,  -912,  4385,
     416,  8725,  6134,  3272,   417,  9769,  6454,  -912,  -912,   231,
    -912,  4067, 16567,  8997,   423,  2338,   196,  -912, 14915,  1578,
    1578,   283,  -912,   283,  -912,   283,  1915,   114,  1027,   591,
   12205, 12205,   273,    92,  -912,  -912,  -912,  -912,  2733,  1775,
     283,   903,   903,  1578,   903,   903,   475,    92,  1775, 12732,
     475,   327,   327,    92,   429,   440,   442,   445,   447,   424,
     436,  -912,   283,  -912,   283,    62,  -912,  -912,  -912,   199,
    -912,   766, 16349,   184, 10585,  8725, 10721,  8725,  9769,  8725,
   12457,    15, 14985,  -912,  -912,  -912, 16270, 15065,  7840,  -912,
    7840,  -912,   388,   301,  9769,   212,  9769, 16270,    57, 13872,
    7386,  -912,  9769, 16270,    58, 13731,  -912,   449,   478,   455,
   15144,   478,   460,   584, 15214,  4385, 13942,  -912,    70,  -912,
    -912,  -912,  -912,  -912,  -912,   334,   224,  -912,  2736,  -912,
     405,   464,  2258,    29,  9769,    76,    85,  9769,  9769,  9769,
    9769,  9769,  5975,  -912,  -912,   513,  7999,  -912, 16270,  -912,
    -912,  -912, 16270,   468,     5,   467,  -912,  1499,  -912,  -912,
     315, 16567,   481,   483,  -912,  -912,  -912,  -912,  -912,  -912,
    -912,  4385,     6,  2763,  -912,  -912, 16270,  4385, 16270,  -912,
   15293,  -912,  -912,  -912,   505,  2112,   486,   534,     0,  -912,
     570,  -912,  -912,  -912,  -912,  2449,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  7386,  -912,    45,  8725,  8725,
    9769,   623, 15363,  9769,   626, 15442,   489, 12600,    27,    27,
    -912,  -912,  -912,  -912,   503,  -912, 13782,  -912, 13024,  4862,
    -912,  5021,  -912,   230,  -912, 13103,  5180,  -912,    27,  5339,
    -912,    27,  9769,  -912,  9769,  -912,  4385,  9769,  -912,  4385,
     625,  -912,  -912, 16567,   824,  -912,  2258,   530,   585,  -912,
    -912,  -912,    41,  -912,  -912,   446,   504,    74,  -912,  -912,
    -912,  5498,  8997,  -912,  -912,  -912, 16567,  -912,   536,   329,
    -912,  -912,  -912,  -912,  5657,   514,  5816,   517,  -912,  9769,
     523,  1379,  -912,  9769, 16567,  -912,  -912,   405,   525,   541,
    -912,   548,  -912,   552,  -912,   553,   555,  -912,   556,   558,
     560,   561,  -912,   563,  -912, 16567,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  9769,  -912,  -912,   564,
     525,   525,  -912,  -912,   446,   242,   245, 15512, 10857, 10993,
   15591, 11129, 11265, 11401, 11537,  -912,  -912,  2179,  -912,  4385,
    9769, 16270,  9769, 16270,  7386,  -912,  4385,  9769, 16270,  -912,
    9769, 16270,  -912, 15666, 16270,  -912, 16270,   668,  4385,   530,
    -912,   540,  9156,   141,  -912,    39,  -912,  -912,  8725, 12331,
    4385,  -912,    50,   545,  9769,  -912,  9769,  -912, 16270,  4385,
    9769,  -912, 16270,  4385, 16270,  -912,  7545,   883,   883,  -912,
   16491,  -912, 16270,  -912,   107,   572,  -912,  -912, 12864, 12546,
    -912,  -912,  -912,  9769,  9769,  9769,  9769,  9769,  9769,  9769,
    9769,   543, 16477, 15214, 13173, 13252,  -912, 15214, 13322, 13401,
    9769,  4385,  -912,   446,   530,  6294,  -912,  -912,  -912,   372,
    8997,  -912,  -912,   133,  9769,     9, 12636,  -912,   622,   390,
    -912,   329, 16270, 13471,  -912, 13550,  -912,  -912,  -912,  9315,
     613,   250,  -912,  -912,  -912,   525,   525, 15741, 15816, 15891,
   15966, 16041, 16116,  -912,  -912,  4385,  4385,  4385,  4385, 16270,
    -912,  -912,   141,  9451,    84,  -912,  -912, 16270,  -912,   112,
    -912,   124,  -912,   432, 16200,  -912,  -912,  -912, 11537,  -912,
    4385,  4385,  -912,  3110,  -912,  -912,   261,  -912,    39,  -912,
    -912,  -912,  9769,  9769,  9769,  9769,  9769,  9769, 15214, 15214,
   15214, 15214,  -912,  -912,  -912,  -912,  -912,   511,  8725, 11945,
    -912,  9769,   133,   112,   112,   112,   112,   112,   112,   133,
     753, 15214, 15214,   550, 11673,    87,    65, 12777,  -912,  -912,
   16270,  -912,  -912,  -912,  -912,  -912,  -912,  -912,   581,  -912,
    -912,   363, 12079,  -912,  -912,  -912,  9610,  -912,   425,  -912
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   525,   526,   527,   528,
     529,     0,   463,    70,   100,     0,   198,   463,     0,   105,
       0,   344,    70,     0,     0,     0,     0,   208,   500,   500,
     522,     0,     0,     0,     0,   104,     0,   218,   101,     0,
     214,     0,     0,   107,     0,     0,   454,     0,   221,     0,
     343,     0,     0,     0,    45,    44,   103,   199,   345,     0,
       0,     0,     0,     0,     0,     0,   106,     0,     0,    83,
       0,   523,     0,     0,     0,   102,   200,     0,     0,   347,
       0,     0,   577,   579,     0,   580,   581,   499,     0,     0,
     578,   575,   122,   576,     0,     0,     0,     4,     0,     5,
       0,     9,    46,    10,    11,    12,    14,   418,   419,    22,
      13,   123,   129,    15,    17,    16,    19,    20,    21,    18,
     128,     0,   126,     0,   489,     0,   130,   127,     0,   131,
     505,   485,   486,   422,   420,     0,     0,   490,   491,     0,
     421,     0,   506,   507,   508,   524,   484,   424,   423,   487,
     488,     0,    38,    24,   430,     0,     0,   464,    71,     0,
       0,     0,     0,     0,     0,     0,     0,   489,   505,   420,
     490,   491,   432,   421,   506,   507,     0,   463,     0,     0,
     346,     0,   172,     0,   403,     0,   410,   501,   219,   528,
     136,     0,     0,   220,     0,     0,     0,     0,     0,     0,
       0,   500,   135,     0,     0,   410,    77,    84,    95,    89,
      97,    79,    88,    98,    85,    99,    93,    87,    94,    92,
      90,    91,    78,    80,    86,    96,     0,    81,     0,     0,
       0,     0,   351,     0,   119,    32,     0,   563,   482,     0,
     433,   564,     7,   410,   500,   125,   124,   314,   400,     0,
     399,     0,     0,   120,   504,     0,     0,    35,     0,   435,
     425,     0,   410,    36,   431,     0,   179,   175,     0,   421,
     179,   176,     0,   340,     0,   434,     0,   399,     0,     0,
     566,   498,   562,   565,   561,     0,    48,    51,     0,     0,
     405,     0,   407,     0,     0,   406,     0,     0,   399,     0,
       0,     6,     0,    47,     0,   201,     0,   300,   299,   222,
       0,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   497,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   266,   267,   268,   263,   265,     0,     0,   261,
     264,   262,   260,     0,   270,   269,     0,     0,   410,   410,
       0,     0,     0,    25,    26,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,    28,     0,    29,     0,   418,
     416,     0,   411,   412,   417,     0,     0,     0,     0,     0,
     145,     0,     0,   144,     0,   153,     0,     0,     0,   151,
       0,     0,     0,    57,   132,     0,   165,     0,     0,    30,
     282,   215,   355,     0,   356,   358,     0,   381,     0,   361,
       0,     0,   118,    31,    33,     0,   313,     0,    64,   502,
     503,   121,     0,    34,   410,     0,   186,   177,   173,   178,
     174,     0,   338,   335,    61,     0,    57,   138,    37,    50,
      49,    52,     0,   530,     0,     0,   518,     0,   520,     0,
       0,     0,     0,     0,     0,     0,     0,   534,     8,     0,
      40,     0,     0,     0,     0,     0,   293,   350,   462,   558,
     557,   560,   568,   567,   572,   571,   554,   551,   552,   553,
     494,   495,   493,   541,   516,   517,   515,   514,   496,   545,
     556,   550,   548,   559,   549,   547,   539,   544,   546,   555,
     538,   542,   543,   540,     0,     0,     0,     0,     0,     0,
       0,   570,   569,   574,   573,   474,   475,   477,   479,     0,
     466,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     532,   463,   463,   141,   336,   348,   404,     0,     0,   427,
       0,   337,   201,     0,     0,     0,     0,   438,     0,     0,
       0,   154,     0,   444,     0,     0,   152,   576,    60,     0,
      53,    58,     0,   164,     0,     0,     0,   426,   287,   284,
     285,   286,   290,   291,   292,   282,     0,   275,     0,   283,
     301,     0,   359,     0,   112,   114,   115,   110,   111,   109,
     108,   113,     0,   380,   379,   485,     0,   353,   483,   352,
     513,   402,   401,     0,     0,     0,   428,     0,   180,   342,
     485,     0,     0,     0,   531,   492,   519,   408,   521,   409,
     161,     0,     0,     0,   533,   159,   448,     0,   536,   535,
       0,    42,    41,    39,   202,     0,     0,   212,     0,   209,
     297,   294,   295,   298,   223,     0,    68,    69,    67,    66,
      65,   511,   512,   476,   478,     0,   465,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     137,   414,   415,   413,     0,   349,     0,   163,     0,     0,
     143,     0,   142,     0,   469,     0,     0,   149,     0,     0,
     147,     0,     0,   133,     0,   134,     0,     0,   167,     0,
     169,   288,   289,     0,   282,   273,     0,   394,   302,   305,
     304,   306,     0,   357,   360,   361,     0,     0,   362,   363,
     188,     0,     0,   187,   190,   429,     0,   181,   184,     0,
     339,    62,    63,   162,     0,     0,     0,     0,   160,     0,
       0,   217,   207,     0,   210,   206,   296,   301,   271,    72,
     255,    84,   253,    89,    74,    88,    85,   258,    99,    87,
      90,    73,    75,    86,   234,   237,   235,   236,   247,   238,
     251,   243,   241,   254,   257,   242,   240,   245,   250,   252,
     239,   244,   248,   249,   246,   256,     0,   232,    76,     0,
     271,   271,   230,   467,   361,   505,   505,     0,     0,     0,
       0,     0,     0,     0,     0,   140,   139,     0,   146,     0,
       0,   437,     0,   436,     0,   468,     0,     0,   443,   150,
       0,   442,   148,    55,    54,   166,   453,   168,     0,   394,
     276,     0,     0,   361,   303,   319,   354,   385,     0,   532,
       0,   192,     0,     0,     0,   182,     0,   157,   450,     0,
       0,   155,   449,     0,   537,   196,   500,   217,   217,   194,
     217,   204,   213,   211,     0,   282,   229,   233,     0,     0,
     225,   226,   471,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,   441,     0,     0,   470,   447,     0,     0,
       0,     0,   171,   361,   394,     0,   397,   396,   398,   485,
     315,   279,   277,     0,     0,     0,     0,   383,   485,   193,
     191,     0,   185,     0,   158,     0,   156,   205,   376,     0,
     307,     0,   231,    72,    73,   271,   271,     0,     0,     0,
       0,     0,     0,   197,   195,     0,     0,     0,     0,    56,
     170,   278,   361,   386,     0,   316,   318,   317,   333,     0,
     334,   321,   324,     0,   320,   311,   312,   216,     0,   183,
       0,     0,   375,     0,   374,   373,   485,   308,   319,   272,
     227,   228,     0,     0,     0,     0,     0,     0,   440,   439,
     446,   445,   281,   280,   388,   389,   391,   485,     0,   532,
     332,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     485,   452,   451,     0,   365,     0,     0,     0,   390,   392,
     323,   325,   326,   329,   330,   331,   327,   328,   322,   370,
     368,   485,   532,   309,   224,   310,   386,   369,   485,   393
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -912,  -912,  -912,     4,  -382,  2009,  -912,  -912,  -912,  -912,
     364,   412,  -307,   269,   272,  -912,  -912,   215,   694,  -912,
    1021,  -162,  -636,  -912,  -711,  -912,   512,  -860,  -706,   -46,
    -912,  -912,  -912,   103,  -912,  -912,  -912,   450,  -912,   -17,
    -912,  -912,  -912,  -912,  -912,   531,   174,   -89,  -912,  -912,
    -912,   -24,   941,  -912,  -912,  -912,  -912,  -912,  -912,  -912,
    -912,  -148,  -139,  -726,  -912,  -136,    33,   156,  -912,  -912,
    -912,   -14,  -912,  -912,  -267,   658,  -912,  -202,  -226,  -254,
    -246,  -722,  -912,  -152,  -912,    -8,   724,   -87,   337,  -912,
    -381,  -710,  -793,  -912,  -539,  -438,  -911,  -888,  -734,   -49,
    -912,    42,  -912,  -180,  -912,   209,   609,  -383,  -912,  -912,
    1128,  -912,    -6,  -912,  -912,  -194,  -912,  -546,  -912,  -912,
    1139,  1464,   -11,   -12,   -58,   930,  -912,  1800,  1998,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  -912,  -335
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   287,    98,   573,   100,   101,   102,   304,
     103,   288,   568,   572,   569,   104,   105,   106,   159,   797,
     230,   107,   227,   108,   603,   192,   235,   109,   110,   111,
     112,   113,   114,   676,   115,   116,   117,   437,   618,   739,
     118,   119,   614,   734,   120,   121,   474,   751,   122,   123,
     648,   649,   167,   228,   590,   125,   126,   476,   757,   654,
     799,   800,   407,   876,   411,   586,   587,   588,   589,   655,
     309,   722,   978,  1034,   967,   248,   956,   911,   915,   961,
     962,   963,   127,   274,   442,   128,   129,   231,   232,   415,
     416,   607,   975,   930,   419,   604,   995,   908,   843,   289,
     183,   293,   294,   381,   382,   383,   168,   131,   132,   133,
     169,   135,   156,   157,   529,   396,   693,   530,   531,   136,
     170,   171,   139,   193,   384,   173,   141,   174,   175,   144,
     145,   146,   299,   147,   148,   149,   150,   151
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     154,   357,   255,   619,   172,   400,    97,   177,   245,   246,
     249,   176,   179,   184,   694,   846,   188,   187,   187,   798,
     197,   199,   203,   204,   379,   408,   373,   533,   585,   276,
     417,   236,   591,   237,   238,   605,   239,   295,   240,   385,
     241,   310,   417,   379,   613,   871,   417,   297,   250,   254,
     256,   258,   259,   260,   725,   966,   233,   264,   620,   265,
     233,   268,   272,   425,    21,   996,   275,   277,   731,   278,
     279,  -377,   394,   280,   880,   881,   281,   282,   283,   233,
     284,   379,   435,   250,   254,   298,   689,   696,  1018,    60,
     420,   913,   374,  -377,   882,   455,   571,   360,   452,     3,
     379,   361,   460,   464,   663,   903,  -189,    50,   928,   732,
     152,  1019,   711,   394,   539,   958,   848,   368,    58,   803,
     315,  -377,   155,   456,   453,  1039,   998,   459,   804,   539,
     357,   906,   754,   912,   196,   198,   958,   461,   394,   571,
     664,  -377,   315,    79,   914,    60,  -377,  -189,   712,   154,
     365,   366,   367,   264,   250,   298,   966,  -377,   755,  -371,
     418,  -377,   497,   733,   927,   394,   369,   158,   744,   316,
     952,   372,   418,   317,  -117,   254,   418,  -377,   519,   520,
     421,  -377,   427,   845,   369,   387,   871,  -371,   369,   401,
     187,   316,   179,   951,   254,   317,   379,   379,  -377,   296,
    -371,  -189,   394,   960,   462,  -377,   462,   369,   180,   980,
     981,   724,  -371,   959,   369,   369,   462,  1033,   974,   462,
     650,  1030,   369,   322,   960,   533,   324,   462,   466,  1001,
     327,  -117,   254,   187,   540,   186,   849,  1000,   929,  1037,
     429,   430,   993,   798,   205,   322,   999,   323,   324,  1032,
     307,   254,   327,   606,   615,  -117,  1002,   463,   467,  -371,
     334,   669,   242,   651,  -371,   361,   457,   338,   339,   340,
     243,   376,   379,   152,   652,   308,    16,   910,   896,   449,
     454,  1022,  1023,  1024,  1025,  1026,  1027,   267,   271,   514,
     515,   377,   451,   653,   458,   516,   261,   262,   517,   518,
     553,   478,   479,   480,   481,   483,   485,   486,   487,   488,
     489,   492,   493,  -372,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     533,   665,   585,    57,     5,   841,   254,   254,   302,   303,
     555,  -372,   254,   427,   427,   254,   254,   254,   254,   522,
     524,   532,  -456,   427,  -372,   315,   714,    76,   578,   666,
     542,   494,   824,   428,   312,   546,  -372,   547,  -455,   870,
     698,   701,   687,   444,  -473,   421,   579,  -472,   250,   580,
     557,   559,   714,   152,   715,   495,   563,   565,   379,   641,
     825,   570,   570,   387,   574,   477,   576,   295,   581,   295,
     311,   582,  -473,   300,   316,  -472,   234,   324,   317,   608,
     979,   917,   583,  -372,   322,  -364,   612,   324,  -372,   482,
     484,   327,    69,   254,   644,   421,   234,    21,   496,   718,
     358,   584,   359,   421,   570,   570,   266,   270,   745,   747,
    -510,   612,  -510,  -364,   254,   545,   254,  -341,  -460,  -461,
     633,   612,   636,   685,   638,   640,  -364,  -458,   322,   909,
     323,   324,   250,   521,   523,   327,   918,  -341,  -364,   740,
      50,  -457,   719,   334,   541,   451,   362,  -366,  -459,   172,
     275,    58,   340,   720,  -395,  -510,   369,  -510,   364,   533,
     682,   375,  -509,   585,  -509,   558,   378,   627,   431,   629,
     178,   564,   721,   315,  -395,  -366,    79,   389,  -395,   410,
     870,   434,   441,   443,   468,  -364,   801,   472,  -366,   473,
    -364,   544,   475,   557,   672,   563,   675,   574,   677,   636,
    -366,   548,  -395,   549,   550,   678,   679,   254,   551,   254,
     370,   554,   560,   686,  1003,   688,   976,   575,   729,   532,
     592,   695,   316,   727,   577,  -378,   317,   593,   606,   610,
     234,   234,   234,   234,   632,  -378,   616,   617,   406,  1004,
     997,  1005,  1006,   621,   257,  1007,  1008,  -366,   631,   637,
     645,   656,  -366,   172,   661,  1010,   240,   259,   260,   264,
     275,   298,   657,  -378,   658,   254,   578,   659,   662,   660,
     234,   -59,   319,   234,   320,   321,   322,   703,   323,   324,
     702,   130,   705,   327,   579,   706,   997,   580,  -378,   315,
     130,   334,   234,  -387,   723,  -378,   730,   735,   338,   339,
     340,  1031,   130,   741,   130,   742,   581,   427,   671,   582,
     674,    52,   234,  -387,   752,  -378,   753,  -387,   756,  1038,
     583,   813,   808,   997,   532,   811,   838,   483,   522,   807,
     817,   842,   810,   844,  -382,  -378,   847,   854,   316,   584,
    -378,  -387,   317,  -378,  -382,   -82,   859,   130,   821,   863,
     823,   865,  -100,   852,   875,   828,  -105,  -104,   831,  -101,
    -107,   833,  -103,   834,  -106,   -83,   836,  -102,   879,   901,
     904,   943,  -382,   130,  -274,   921,   977,   130,   390,   393,
     395,   399,  1029,  1002,   471,   623,   181,   622,   319,   853,
     439,   250,   322,   386,   323,   324,   684,  -382,   892,   327,
     873,   935,  -274,   858,  -382,   862,   234,   334,   864,   931,
     936,   713,   872,   874,   338,   339,   340,   840,   423,  1035,
     992,   424,  1016,   292,  -382,  1028,  1021,   202,   609,   683,
     234,     0,     0,     0,   234,   130,     0,     0,     0,     5,
     433,   482,   521,     0,  -382,     0,     0,     0,     0,  -382,
       0,    14,  -382,     0,     0,   878,     0,     0,    19,     0,
     447,     0,     0,     0,     0,  -384,     0,   821,   823,     0,
     828,   831,   858,   862,     0,  -384,     0,    35,   893,   894,
       0,   895,    38,   532,     0,   897,   898,     0,     0,   899,
      43,   245,   246,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -384,     0,   130,     0,   916,   636,     0,
       0,    56,   130,   922,   380,   923,     0,     0,   578,   925,
       0,     0,     0,    66,     0,   187,   954,    69,  -384,     0,
       0,     0,     0,   380,    75,  -384,   579,     0,     0,   580,
       0,     0,   893,   937,   938,   897,   939,   940,   941,   942,
       0,     0,     0,     0,   543,  -384,     0,     0,   581,   949,
       0,   582,     0,    52,   298,     0,     0,   130,     0,   957,
       0,   380,   583,   964,     0,  -384,   200,    21,   561,     0,
    -384,   130,   566,  -384,     0,     0,     0,    28,   244,     0,
     380,   584,   234,   234,  1015,     0,     0,    37,   234,   234,
       0,   315,   140,     0,   988,   989,   990,   991,     0,     0,
     710,   140,     0,   124,     0,     0,     0,     0,    48,     0,
      50,     0,   124,   140,     0,   140,     0,   862,     0,  1011,
    1012,    58,   298,     0,   124,     0,   124,     0,     0,     0,
       0,   988,   989,   990,   991,  1011,  1012,     0,   130,     0,
     316,   737,     0,     0,   317,     0,    79,  1017,   636,     0,
    1020,     0,     0,     0,     0,     0,     0,     0,   140,     0,
     130,     0,   269,   269,     0,     0,   130,     0,     0,   124,
       0,     0,     0,     0,   130,     0,   380,   380,     0,     0,
       0,   636,     0,     0,   140,   862,     0,     0,   140,     0,
     319,     0,   320,   321,   322,   124,   323,   324,   325,   124,
       0,   327,   234,   234,     0,     0,     0,     0,   333,   334,
     234,     0,   406,   337,     0,   315,   338,   339,   340,   406,
     226,     0,   234,   200,    21,   234,     0,     0,   130,     0,
     690,   692,   130,     0,    28,   244,   697,   700,     0,     0,
     130,     0,     0,     0,    37,   611,   140,  -217,     0,     0,
       0,     0,   380,     0,     0,   273,     0,   124,     0,     0,
       0,     0,     0,   855,   316,    48,     0,    50,   317,     0,
     611,     0,  -217,   292,     0,   292,     0,     0,    58,     0,
     611,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     134,     0,     0,     0,     0,     0,     0,     0,     0,   134,
     667,   137,   305,    79,   306,     0,     0,     0,     0,     0,
     137,   134,     0,   134,   319,     0,   140,   321,   322,     0,
     323,   324,   137,   140,   137,   327,     0,   124,     0,     0,
       0,     0,     0,   334,   124,     0,     0,     0,     0,     0,
     338,   339,   340,     0,   130,     0,     0,     0,     0,     0,
     815,   816,     0,     0,     0,     0,   134,     0,   818,   273,
       0,     0,     0,     0,     0,     0,   681,   137,   380,     0,
     829,     0,   273,   832,     0,   388,   234,     0,   140,     0,
       0,     0,   134,     0,     0,     0,   134,     0,     0,   124,
       0,     0,   140,   137,     0,     0,     0,   137,     0,     0,
     130,     0,     0,   124,     0,     0,   130,     0,     0,     0,
     414,     0,     0,     0,     0,     0,     0,     0,   406,   406,
       0,     0,   406,   406,   802,     0,     0,     0,   426,     0,
       0,     0,     0,     0,     0,     0,     0,   805,   806,   965,
       0,     0,     0,     0,   134,   969,     0,   406,     0,   406,
       0,     0,     0,     0,     0,   137,     0,     0,   130,   140,
     130,     0,     0,     0,     0,   130,     0,     0,   130,     0,
     124,     0,     0,     0,     0,   130,     0,     0,   130,     0,
       0,   140,     0,     0,     0,     0,     0,   140,     0,     0,
       0,     0,   124,     0,     0,   140,     0,     0,   124,     0,
     130,     0,     0,     0,     0,     0,   124,     0,     0,     0,
       0,     0,     0,   130,   134,   130,     0,     0,     0,     0,
       0,   134,     0,     0,   920,   137,     0,     0,     0,     0,
       0,     0,   137,     0,     0,     0,     0,     0,     0,     0,
     965,     0,     0,     0,     0,     0,     0,     0,     0,   140,
       0,     0,     0,   140,     0,     0,     0,     0,    16,     0,
     124,   140,    20,    21,   124,     0,     0,   552,     0,     0,
       0,    27,   124,    28,   866,     0,   134,     0,    33,     0,
       0,     0,     0,    37,     0,     0,     0,   137,   130,     0,
     134,     0,     0,     0,   414,   130,     0,     0,     0,     0,
       0,   137,     0,     0,    48,     0,    50,   130,    52,     0,
     867,     0,     0,   868,     0,    57,     0,    58,     0,   130,
       0,     0,     0,     0,     0,     0,   138,     0,   130,     0,
       0,     0,   130,     0,     0,   138,    74,     0,     0,    76,
       0,     0,    79,     0,     0,     0,     0,   138,     0,   138,
       0,     0,     0,   643,     0,     0,   647,   134,     0,     0,
     907,     0,   206,     0,     0,   140,     0,     0,   137,     0,
     130,     0,     0,     0,   207,   208,   124,     0,     0,   134,
       0,   209,     0,     0,     0,   134,     0,     0,     0,   210,
     137,    92,   138,   134,     0,   211,   137,   869,     0,     0,
     212,     0,   213,     0,   137,   214,     0,     0,     0,     0,
       0,     0,     0,   215,   130,   130,   130,   130,   138,     0,
       0,   140,   138,     0,     0,   216,     0,   140,   955,     0,
       0,     0,   124,     0,   217,     0,     0,     0,   124,   130,
     130,     0,     0,   218,   219,     0,   220,   134,   221,     0,
     222,   134,     0,   223,     0,     0,     0,   224,   137,   134,
     225,     0,   137,     0,     0,     0,   315,     0,     0,   717,
     137,   994,     0,   414,     0,     0,     0,     0,     0,   140,
     138,   140,     0,     0,     0,     0,   140,     0,     0,   140,
     124,     0,   124,     0,     0,     0,   140,   124,   738,   140,
     124,     0,   273,     0,     0,     0,     0,   124,     0,     0,
     124,     0,     0,     0,     0,   316,   369,   994,   736,   317,
       0,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   124,     0,   140,     0,   140,     0,     0,     0,
       0,     0,     0,     0,     0,   124,     0,   124,     0,   318,
     138,     0,     0,     0,   994,     0,     0,   138,     0,     0,
       0,     0,     0,   134,     0,   319,     0,   320,   321,   322,
       0,   323,   324,   325,   137,   326,   327,   328,   329,     0,
       0,   331,   332,   333,   334,     0,   335,   336,   337,     0,
       0,   338,   339,   340,   839,     0,     0,   414,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   140,
       0,     0,   138,     0,     0,     0,   140,   738,     0,   134,
     124,     0,     0,     0,     0,   134,   138,   124,   140,     0,
     137,     0,     0,     0,     0,   647,   137,     0,     0,   124,
     140,     0,     0,     0,     0,     0,     0,     0,     0,   140,
       0,   124,     0,   140,     0,     0,   877,     0,     0,     0,
     124,     0,   142,   315,   124,     0,     0,     0,     0,     0,
       0,   142,     0,     0,     0,     0,     0,   134,     0,   134,
       0,     0,     0,   142,   134,   142,     0,   134,   137,     0,
     137,   140,     0,   138,   134,   137,     0,   134,   137,     0,
       0,     0,   124,     0,     0,   137,     0,     0,   137,     0,
       0,     0,   316,     0,     0,   138,   317,     0,     0,   134,
       0,   138,     0,     0,     0,     0,     0,     0,   142,   138,
     137,     0,   134,     0,   134,   140,   140,   140,   140,     0,
       0,     0,     0,   137,     0,   137,   124,   124,   124,   124,
       0,     0,     0,     0,   142,     0,     0,     0,   142,     0,
     140,   140,   319,     0,   320,   321,   322,     0,   323,   324,
     325,   124,   124,   327,   328,   329,     0,     0,   331,   332,
     333,   334,     0,   138,     0,   337,     0,   138,   338,   339,
     340,     0,     0,     0,     0,   138,     0,     0,     0,     0,
       0,     0,     0,   315,     0,     0,     0,   134,     0,     0,
       0,     0,     0,     0,   134,     0,   142,     0,   137,     0,
       0,     0,     0,     0,     0,   137,   134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   137,   134,     0,
       0,     0,     0,     0,     0,     0,     0,   134,     0,   137,
       0,   134,   316,     0,     0,     0,   317,     0,   137,     0,
     143,     0,   137,     0,     0,     0,     0,     0,     0,   143,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
     153,   143,     0,   143,     0,     0,   142,     0,     0,   134,
       0,     0,   182,   142,   185,     0,     0,     0,     0,   138,
     137,     0,   319,     0,   320,   321,   322,     0,   323,   324,
     325,     0,   326,   327,   328,   329,     0,     0,   331,   332,
     333,   334,     0,   335,     0,   337,   143,     0,   338,   339,
     340,     0,     0,   134,   134,   134,   134,   263,     0,     0,
       0,     0,     0,     0,   137,   137,   137,   137,   142,     0,
       0,     0,   143,     0,     0,   138,   143,     0,   134,   134,
       0,   138,   142,    99,     0,     0,     0,   301,     0,   137,
     137,     0,     0,   750,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -203,     0,     0,     0,  -203,  -203,     0,     0,     0,
       0,     0,     0,     0,  -203,     0,  -203,  -203,     0,     0,
       0,  -203,     0,   138,   143,   138,  -203,     0,     0,  -203,
     138,     0,     0,   138,     0,   363,     0,     0,     0,   142,
     138,     0,     0,   138,     0,     0,     0,  -203,     0,  -203,
     891,  -203,     0,  -203,  -203,     0,  -203,     0,  -203,     0,
    -203,   142,     0,     0,     0,   138,     0,   142,  -203,     0,
       0,     0,  -203,  -203,     0,   142,     0,     0,   138,  -203,
     138,  -203,  -203,  -203,  -203,  -203,     0,     0,  -203,     0,
       0,     0,     0,  -203,   143,     0,  -203,     0,     0,     0,
       0,   143,     0,     0,     0,   409,     0,     0,     0,     0,
       0,     0,   422,     0,  -203,     0,  -203,     0,  -203,     0,
    -203,  -203,     0,  -203,     0,  -203,     0,  -203,     0,   142,
       0,   206,     0,   142,  -203,     0,     0,     0,     0,     0,
    -203,   142,     0,   207,   208,     0,  -203,     0,     0,  -203,
     209,     0,  -203,   138,     0,     0,   143,     0,   210,     0,
     138,     0,     0,     0,   211,     0,     0,    99,     0,   212,
     143,   213,   138,     0,   214,     0,     0,     0,     0,     0,
       0,    99,   215,     0,   138,     0,     0,     0,     0,     0,
       0,     0,     0,   138,   216,     0,     0,   138,     0,     0,
       0,  -203,     0,   217,     0,     0,     0,  -203,     0,   646,
       0,   206,   218,   219,     0,   220,     0,   221,     0,   222,
       0,     0,   223,   207,   208,     0,   224,   412,     0,   225,
     209,     0,     0,     0,     0,   138,     0,   143,   210,     0,
       0,     0,     0,     0,   211,   142,     0,     0,    99,   212,
       0,   213,     0,     0,   214,     0,     0,     0,     0,   143,
       0,     0,   215,     0,     0,   143,     0,     0,     0,     0,
     422,     0,     0,   143,   216,     0,   422,     0,     0,   138,
     138,   138,   138,   217,     0,     0,     0,   413,     0,     0,
       0,     0,   218,   219,     0,   220,     0,   221,     0,   222,
       0,   142,   223,     0,   138,   138,   224,   142,     0,   225,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     758,     0,   759,     0,     0,     0,     0,   143,     0,   760,
       0,   143,     0,     0,   761,   208,   762,     0,   630,   143,
       0,   763,   635,     0,     0,     0,     0,     0,     0,   210,
      99,     0,     0,     0,     0,   764,     0,     0,     0,   142,
     765,   142,   213,     0,     0,   766,   142,   767,     0,   142,
       0,     0,     0,   768,     0,     0,   142,     0,     0,   142,
       0,     0,     0,     0,     0,   216,     0,     0,     0,     0,
       0,     0,     0,     0,   769,     0,     0,     0,     0,     0,
       0,   142,     0,   218,   219,     0,   770,     0,   221,     0,
     771,     0,     0,   772,   142,     0,   142,   773,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   341,   342,   343,   344,   345,   346,     0,     0,   349,
     350,   351,   352,   143,   354,   355,   774,   775,   776,   777,
       0,     0,   778,     0,   708,     0,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   789,   790,     0,   791,
       0,     0,   792,   793,   794,   795,     0,     0,   796,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
       0,     0,     0,     0,     0,     0,   142,     0,     0,   143,
       0,     0,     0,     0,     0,   143,     0,     0,   142,   206,
     743,     0,     0,     0,     0,     0,   748,     0,     0,     0,
     142,   207,   208,     0,     0,     0,     0,     0,   209,   142,
       0,     0,     0,   142,     0,     0,   210,     0,     0,     0,
       0,     0,   211,     0,     0,     0,     0,   212,     0,   213,
       0,     0,   214,     0,     0,     0,     0,   143,     0,   143,
     215,     0,     0,     0,   143,     0,     0,   143,   422,     0,
     422,   142,   216,     0,   143,   422,     0,   143,   422,     0,
       0,   217,     0,     0,     0,   835,     0,     0,   837,     0,
     218,   219,     0,   220,     0,   221,     0,   222,     0,   143,
     223,     0,     0,     0,   224,     0,     0,   225,     0,   206,
     851,     0,   143,     0,   143,   142,   142,   142,   142,     0,
       0,   207,   208,   857,     0,   861,     0,     0,   209,     0,
       0,   315,     0,     0,     0,     0,   210,     0,     0,     0,
     142,   142,   211,   313,     0,     0,     0,   212,     0,   213,
     314,     0,   214,     0,     0,     0,     0,     0,     0,     0,
     215,   315,     0,     0,     0,   229,     0,     0,     0,     0,
       0,     0,   216,     0,     0,     0,     0,     0,     0,     0,
     316,   217,     0,     0,   317,     0,     0,   143,     0,     0,
     218,   219,     0,   220,   143,   221,     0,   222,     0,     0,
     223,     0,     0,     0,   224,     0,   143,   225,     0,     0,
     316,     0,     0,     0,   317,     0,     0,   902,   143,     0,
       0,     0,     0,     0,     0,     0,     0,   143,     0,   919,
     319,   143,   320,   321,   322,     0,   323,   324,   924,     0,
     394,   327,   926,     0,   318,     0,     0,     0,   333,   334,
       0,     0,     0,   337,     0,     0,   338,   339,   340,     0,
     319,     0,   320,   321,   322,   716,   323,   324,   325,   143,
     326,   327,   328,   329,   330,     0,   331,   332,   333,   334,
     950,   335,   336,   337,     0,     0,   338,   339,   340,     0,
       0,     0,     0,     0,     0,   746,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   143,   143,   143,   143,     0,     0,     0,
       0,    -2,     4,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,   143,   143,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,    37,     0,    38,  -217,     0,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,     0,    47,     0,     0,     0,    48,    49,    50,    51,
      52,    53,    54,  -217,   -43,    55,    56,    57,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,    92,     0,     0,    93,     0,    94,     0,
      95,  1013,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   160,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,   247,     0,     0,     0,    93,     0,   165,     0,    95,
       0,   166,  1014,     4,     0,     5,     6,     7,     8,     9,
      10,     0,  -532,     0,    11,    12,    13,    14,    15,  -532,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    -532,    25,    26,  -532,    27,     0,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,    37,     0,    38,  -217,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,     0,    47,     0,     0,     0,    48,    49,    50,
      51,     0,    53,    54,  -217,   -43,    55,    56,    57,  -532,
      58,    59,    60,  -532,    61,    62,    63,    64,    65,    66,
      67,    68,     0,    69,    70,     0,    71,    72,    73,    74,
      75,     0,    76,    77,    78,    79,     0,     0,    80,     0,
      81,     0,     0,  -532,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -532,
      84,  -532,  -532,  -532,  -532,  -532,  -532,  -532,     0,  -532,
    -532,  -532,  -532,  -532,     0,  -532,  -532,  -532,  -532,    89,
    -532,  -532,  -532,     0,    92,  -532,  -532,  -532,     0,    94,
    -532,    95,   285,    96,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,    37,     0,    38,  -217,     0,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,     0,    47,     0,     0,     0,    48,    49,    50,    51,
      52,    53,    54,  -217,   -43,    55,    56,    57,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,   247,    92,     0,     0,    93,     0,    94,   286,
      95,     4,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,    37,     0,    38,  -217,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
       0,    47,     0,     0,     0,    48,    49,    50,    51,    52,
      53,    54,  -217,   -43,    55,    56,    57,     0,    58,    59,
      60,     0,    61,    62,    63,    64,    65,    66,    67,    68,
       0,    69,    70,     0,    71,    72,    73,    74,    75,     0,
      76,    77,    78,    79,     0,     0,    80,     0,    81,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,     0,    92,     0,     0,    93,     0,    94,   450,    95,
     469,    96,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -217,     0,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,     0,
      47,     0,     0,     0,    48,    49,    50,    51,    52,    53,
      54,  -217,   -43,    55,    56,    57,     0,    58,    59,    60,
       0,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      69,    70,     0,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,     0,     0,    80,     0,    81,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
       0,    92,     0,     0,    93,     0,    94,   470,    95,   285,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -217,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,     0,    47,
       0,     0,     0,    48,    49,    50,    51,    52,    53,    54,
    -217,   -43,    55,    56,    57,     0,    58,    59,    60,     0,
      61,    62,    63,    64,    65,    66,    67,    68,     0,    69,
      70,     0,    71,    72,    73,    74,    75,     0,    76,    77,
      78,    79,     0,     0,    80,     0,    81,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,     0,
      92,     0,     0,    93,     0,    94,   286,    95,     4,    96,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,    37,     0,    38,  -217,     0,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,     0,    47,     0,
       0,     0,    48,    49,    50,    51,    52,    53,    54,  -217,
     -43,    55,    56,    57,     0,    58,    59,    60,     0,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    69,    70,
       0,    71,    72,    73,    74,    75,     0,    76,    77,    78,
      79,     0,     0,    80,     0,    81,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,     0,    92,
       0,     0,    93,     0,    94,   642,    95,     4,    96,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -217,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,     0,    47,     0,     0,
       0,    48,    49,    50,    51,   300,    53,    54,  -217,   -43,
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
      29,    30,    31,    32,    33,    34,    35,     0,    36,    37,
       0,    38,  -217,     0,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,     0,    47,     0,     0,     0,
      48,    49,    50,    51,     0,    53,    54,  -217,   -43,    55,
      56,    57,     0,    58,    59,    60,     0,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    69,    70,     0,    71,
      72,    73,    74,    75,     0,    76,    77,    78,    79,     0,
       0,    80,     0,    81,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,     0,    92,     0,     0,
      93,     0,    94,     0,    95,     4,    96,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,   556,    35,     0,    36,    37,     0,
      38,  -217,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,     0,    47,     0,     0,     0,    48,
      49,    50,    51,     0,    53,    54,  -217,   -43,    55,    56,
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
      31,    32,    33,   562,    35,     0,    36,    37,     0,    38,
    -217,     0,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,     0,    47,     0,     0,     0,    48,    49,
      50,    51,     0,    53,    54,  -217,   -43,    55,    56,    57,
       0,    58,    59,    60,     0,    61,    62,    63,    64,    65,
      66,    67,    68,     0,    69,    70,     0,    71,    72,    73,
      74,    75,     0,    76,    77,    78,    79,     0,     0,    80,
       0,    81,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,     0,    92,     0,     0,    93,     0,
      94,     0,    95,     4,    96,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,   820,    35,     0,    36,    37,     0,    38,  -217,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,     0,    47,     0,     0,     0,    48,    49,    50,
      51,     0,    53,    54,  -217,   -43,    55,    56,    57,     0,
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
      33,   822,    35,     0,    36,    37,     0,    38,  -217,     0,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,     0,    47,     0,     0,     0,    48,    49,    50,    51,
       0,    53,    54,  -217,   -43,    55,    56,    57,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,    92,     0,     0,    93,     0,    94,     0,
      95,     4,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
     827,    35,     0,    36,    37,     0,    38,  -217,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
       0,    47,     0,     0,     0,    48,    49,    50,    51,     0,
      53,    54,  -217,   -43,    55,    56,    57,     0,    58,    59,
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
       0,    27,     0,    28,    29,    30,    31,    32,    33,   830,
      35,     0,    36,    37,     0,    38,  -217,     0,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,     0,
      47,     0,     0,     0,    48,    49,    50,    51,     0,    53,
      54,  -217,   -43,    55,    56,    57,     0,    58,    59,    60,
       0,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      69,    70,     0,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,     0,     0,    80,     0,    81,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
       0,    92,     0,     0,    93,     0,    94,     0,    95,     4,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,   850,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -217,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,     0,    47,
       0,     0,     0,    48,    49,    50,    51,     0,    53,    54,
    -217,   -43,    55,    56,    57,     0,    58,    59,    60,     0,
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
       0,    28,    29,    30,    31,    32,    33,   856,    35,     0,
      36,    37,     0,    38,  -217,     0,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,     0,    47,     0,
       0,     0,    48,    49,    50,    51,     0,    53,    54,  -217,
     -43,    55,    56,    57,     0,    58,    59,    60,     0,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    69,    70,
       0,    71,    72,    73,    74,    75,     0,    76,    77,    78,
      79,     0,     0,    80,     0,    81,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,     0,    92,
       0,     0,    93,     0,    94,     0,    95,     4,    96,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,   860,    35,     0,    36,
      37,     0,    38,  -217,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,     0,    47,     0,     0,
       0,    48,    49,    50,    51,     0,    53,    54,  -217,   -43,
      55,    56,    57,     0,    58,    59,    60,     0,    61,    62,
      63,    64,    65,    66,    67,    68,     0,    69,    70,     0,
      71,    72,    73,    74,    75,     0,    76,    77,    78,    79,
       0,     0,    80,     0,    81,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,     0,    92,     0,
       0,    93,     0,    94,     0,    95,   726,    96,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   160,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,   247,     0,     0,     0,
      93,     0,   165,     0,    95,     0,   166,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,   247,     0,     0,     0,    93,
       0,   165,     0,    95,     0,   166,   634,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,   247,     0,     0,     0,    93,
       0,   165,     0,    95,     0,   166,   953,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,     0,     0,     0,     0,    93,
       0,   165,     0,    95,     0,   166,   639,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   160,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,   200,    21,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,   201,
      30,   161,   162,     0,   163,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,     0,
      49,    50,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,    58,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   164,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,    79,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,     0,     0,     0,     0,    93,
       0,   165,     0,    95,     0,   166,     5,     6,     7,     8,
     189,    10,   190,     0,     0,   160,     0,     0,    14,    15,
       0,     0,  -116,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,  -116,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
     191,    75,     0,  -116,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   166,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,   251,   252,     0,    75,   290,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,   291,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
     247,     0,     0,     0,    93,     0,   165,     0,    95,     0,
     166,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     160,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   161,   162,     0,   163,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   164,     0,    69,
       0,     0,    71,   251,   252,     0,    75,   290,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    89,     0,    90,    91,   247,
       0,     0,     0,    93,     0,   165,     0,    95,   626,   166,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   160,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,   251,   252,     0,    75,   290,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,   247,     0,
       0,     0,    93,     0,   165,     0,    95,   628,   166,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
     525,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,   526,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,   527,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     0,   528,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,     0,     0,     0,
       0,    93,     0,   165,     0,    95,     0,   166,     5,     6,
       7,     8,   189,    10,     0,     0,     0,   160,     0,     0,
      14,    15,     0,     0,  -116,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,  -116,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,   191,    75,     0,  -116,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,    15,     0,     0,
      93,     0,   165,    19,    95,     0,   166,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,   251,   252,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,   253,     0,     0,    93,     0,   165,     0,
      95,     0,   166,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   160,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,   251,   252,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,   247,     0,     0,     0,    93,     0,   165,     0,    95,
       0,   166,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   160,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,   728,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,     0,    71,   251,   252,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   160,    90,    91,
      14,    15,     0,     0,    93,     0,   165,    19,    95,     0,
     166,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,    49,   194,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,   195,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,    15,     0,     0,
      93,     0,   165,    19,    95,     0,   166,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
     195,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,   247,     0,     0,     0,    93,     0,   165,     0,
      95,     0,   166,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   160,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     163,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,   251,   252,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    88,     0,    89,   160,    90,
      91,    14,    15,     0,     0,    93,     0,   165,    19,    95,
       0,   166,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   164,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,   247,     0,     0,
       0,    93,     0,   165,   624,    95,     0,   166,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   160,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,   195,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,    15,     0,     0,
      93,     0,   165,    19,    95,     0,   166,     0,     0,     0,
     233,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,    49,     0,    51,
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
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,   247,     0,
       0,     0,    93,     0,   165,     0,    95,     0,   166,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   160,     0,
       0,    14,   594,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,   595,     0,     0,     0,
       0,     0,    30,   161,   162,     0,   163,    35,     0,   596,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,     0,   597,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
     598,   599,    65,    66,    67,   600,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,   601,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,   247,     0,     0,
       0,    93,     0,   165,     0,    95,     0,   905,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   160,     0,     0,
      14,   594,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,   595,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   163,    35,     0,   596,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,   972,     0,     0,     0,     0,
       0,   597,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,   598,
     599,    65,    66,    67,   600,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,   601,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,    15,     0,     0,
      93,     0,   165,    19,    95,     0,   973,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,   247,     0,     0,     0,    93,     0,   165,     0,
      95,     0,   905,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   160,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   161,   162,     0,
     890,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   164,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,   247,     0,     0,     0,    93,     0,   165,     0,    95,
       0,   166,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   160,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
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
       0,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   160,    90,    91,    14,   490,     0,     0,
      93,     0,    94,    19,    95,     0,   166,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,   491,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   160,
      90,    91,    14,    15,     0,     0,    93,     0,   165,    19,
      95,     0,   166,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   160,    90,    91,    14,   594,
       0,     0,   567,     0,   165,    19,    95,     0,   166,     0,
       0,     0,     0,   595,     0,     0,     0,     0,     0,    30,
     161,   162,     0,   163,    35,     0,   596,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,     0,   597,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,   598,   599,    65,
      66,    67,   600,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,   601,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   602,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
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
     602,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   161,   162,     0,   670,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
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
       0,   673,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,    49,     0,    51,
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
       0,     0,     0,    30,   161,   162,     0,   884,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
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
     161,   162,     0,   885,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   160,    90,    91,    14,    15,     0,     0,    93,     0,
     165,    19,    95,     0,   166,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   887,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
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
       0,    30,   161,   162,     0,   888,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
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
       0,   889,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,    49,     0,    51,
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
       0,     0,     0,    30,   161,   162,     0,   890,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
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
     161,   162,     0,   163,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   164,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   160,    90,    91,    14,    15,     0,     0,    93,     0,
       0,    19,    95,     0,   973,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,     0,
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
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -386,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,     0,     0,  -386,     0,     0,
       0,  -386,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   160,    89,     0,    14,    15,   247,     0,     0,     0,
       0,    19,   165,     0,    95,  -386,   905,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   161,   162,     0,   163,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,  -367,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,  -367,
       0,     0,     0,    63,    64,    65,    66,    67,   164,     0,
      69,     0,  -367,    71,     0,     0,     0,    75,     0,     0,
      77,     0,     0,     0,  -367,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,     0,     0,     0,     0,     0,     0,    84,     5,     6,
       7,     8,     9,    10,     0,  -480,     0,   160,     0,     0,
      14,    15,     0,     0,     0,     0,    89,    19,     0,   356,
       0,  -367,  -509,     0,  -509,    26,  -367,     0,    95,     0,
     973,    30,   161,   162,     0,   163,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   164,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   160,     0,     0,    14,    15,     0,     0,
       0,   356,    89,    19,  -509,     0,  -509,     0,     0,     0,
       0,    26,   165,     0,    95,     0,   166,    30,   161,   162,
       0,   163,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     164,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    84,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   160,
       0,     0,    14,    15,     0,     0,     0,     0,    89,    19,
       0,     0,     0,     0,     0,     0,     0,    26,   165,     0,
      95,     0,   602,    30,   161,   162,     0,   163,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   164,     0,    69,   933,
       0,    71,     0,     0,     0,    75,   760,     0,    77,     0,
       0,   207,   208,   762,     0,     0,     0,     0,   209,     0,
       0,     0,     0,     0,     0,     0,   210,     0,     0,     0,
       0,     0,   764,     0,     0,    84,     0,   212,     0,   213,
       0,     0,   214,     0,   767,     0,     0,     0,     0,     0,
     215,     0,     0,     0,    89,     0,     0,     0,     0,     0,
     313,     0,   216,     0,     0,     0,    95,   314,   166,     0,
       0,   217,     0,     0,     0,     0,     0,     0,   315,     0,
     218,   219,     0,   220,     0,   221,     0,   934,     0,     0,
     772,     0,     0,     0,   224,     0,   313,   225,     0,     0,
       0,     0,     0,   314,     0,     0,     0,     0,   341,   342,
     343,   344,   345,   346,   315,     0,   349,   350,   351,   352,
       0,   354,   355,   774,   775,   776,   777,   316,     0,   778,
       0,   317,     0,   779,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,   790,     0,   791,     0,     0,   792,
     793,   794,   795,     0,     0,     0,     0,     0,     0,     0,
       0,   318,     0,   316,     0,     0,     0,   317,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   319,     0,   320,
     321,   322,     0,   323,   324,   325,     0,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,   318,   335,   336,
     337,     0,     0,   338,   339,   340,     0,     0,     0,     0,
     315,     0,   814,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,   313,     0,   338,
     339,   340,     0,     0,   314,     0,     0,     0,   968,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,   316,
       0,     0,     0,   317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,     0,     0,     0,     0,     0,     0,
     314,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   315,     0,   318,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   316,     0,     0,     0,   317,   319,
       0,   320,   321,   322,     0,   323,   324,   325,     0,   326,
     327,   328,   329,     0,   313,   331,   332,   333,   334,     0,
     335,   314,   337,     0,     0,   338,   339,   340,   318,     0,
     316,     0,   315,     0,   317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   319,     0,   320,   321,   322,     0,
     323,   324,   325,     0,   326,   327,   328,   329,   330,     0,
     331,   332,   333,   334,   318,   335,   336,   337,     0,     0,
     338,   339,   340,     0,     0,     0,     0,     0,     0,  1036,
     319,   316,   320,   321,   322,   317,   323,   324,   325,     0,
     326,   327,   328,   329,   330,     0,   331,   332,   333,   334,
       0,   335,   336,   337,   313,     0,   338,   339,   340,     0,
       0,   314,     0,   625,     0,   318,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,   319,     0,   320,   321,   322,     0,   323,   324,   325,
       0,   326,   327,   328,   329,   330,     0,   331,   332,   333,
     334,     0,   335,   336,   337,     0,     0,   338,   339,   340,
       0,     0,     0,     0,   932,     0,     0,     0,     0,     0,
       0,   316,     0,     0,   313,   317,     0,     0,     0,     0,
       0,   314,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   318,   341,   342,   343,   344,
     345,   346,     0,     0,   349,   350,   351,   352,     0,   354,
     355,   319,     0,   320,   321,   322,     0,   323,   324,   325,
       0,   326,   327,   328,   329,   330,     0,   331,   332,   333,
     334,   316,   335,   336,   337,   317,     0,   338,   339,   340,
       0,   369,     0,   313,   819,     0,     0,     0,     0,     0,
     314,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,   318,   341,   342,   343,   344,
     345,   346,     0,     0,   349,   350,   351,   352,     0,   354,
     355,   319,     0,   320,   321,   322,     0,   323,   324,   325,
       0,   326,   327,   328,   329,   330,     0,   331,   332,   333,
     334,     0,   335,   336,   337,     0,     0,   338,   339,   340,
     316,   369,     0,   313,   317,     0,     0,     0,     0,     0,
     314,     0,     0,   826,     0,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   318,   341,   342,   343,   344,   345,
     346,     0,     0,   349,   350,   351,   352,     0,   354,   355,
     319,     0,   320,   321,   322,     0,   323,   324,   325,     0,
     326,   327,   328,   329,   330,     0,   331,   332,   333,   334,
     316,   335,   336,   337,   317,     0,   338,   339,   340,     0,
     369,     0,   313,   945,     0,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,   318,   341,   342,   343,   344,   345,
     346,     0,     0,   349,   350,   351,   352,     0,   354,   355,
     319,     0,   320,   321,   322,     0,   323,   324,   325,     0,
     326,   327,   328,   329,   330,     0,   331,   332,   333,   334,
       0,   335,   336,   337,     0,     0,   338,   339,   340,   316,
     369,     0,   313,   317,     0,     0,     0,     0,     0,   314,
       0,     0,   946,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   318,   341,   342,   343,   344,   345,   346,
       0,     0,   349,   350,   351,   352,     0,   354,   355,   319,
       0,   320,   321,   322,     0,   323,   324,   325,     0,   326,
     327,   328,   329,   330,     0,   331,   332,   333,   334,   316,
     335,   336,   337,   317,     0,   338,   339,   340,     0,   369,
       0,   313,   947,     0,     0,     0,     0,     0,   314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,     0,     0,   318,   341,   342,   343,   344,   345,   346,
       0,     0,   349,   350,   351,   352,     0,   354,   355,   319,
       0,   320,   321,   322,     0,   323,   324,   325,     0,   326,
     327,   328,   329,   330,     0,   331,   332,   333,   334,     0,
     335,   336,   337,     0,     0,   338,   339,   340,   316,   369,
       0,   313,   317,     0,     0,     0,     0,     0,   314,     0,
       0,   948,     0,     0,     0,     0,     0,     0,     0,   315,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   318,   341,   342,   343,   344,   345,   346,     0,
       0,   349,   350,   351,   352,     0,   354,   355,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,   316,   335,
     336,   337,   317,     0,   338,   339,   340,     0,   369,     0,
     313,   970,     0,     0,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,   318,   341,   342,   343,   344,   345,   346,     0,
       0,   349,   350,   351,   352,     0,   354,   355,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,     0,   338,   339,   340,   316,   369,     0,
       0,   317,     0,     0,     0,     0,     0,     0,     0,     0,
     971,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,   314,     0,     0,
       0,   318,   341,   342,   343,   344,   345,   346,   315,   397,
     349,   350,   351,   352,     0,   354,   355,   319,     0,   320,
     321,   322,   398,   323,   324,   325,     0,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,     0,   335,   336,
     337,   313,     0,   338,   339,   340,     0,   369,   314,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,   315,
     391,   317,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   392,     0,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,     0,   394,   314,     0,
       0,   318,     0,     0,     0,     0,     0,     0,     0,   315,
     699,     0,     0,     0,     0,     0,     0,   319,   316,   320,
     321,   322,   317,   323,   324,   325,     0,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,     0,   335,   336,
     337,     0,   313,   338,   339,   340,     0,   369,     0,   314,
       0,     0,   318,     0,     0,     0,     0,     0,   316,     0,
     315,   233,   317,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,   394,   335,
     336,   337,   318,     0,   338,   339,   340,     0,   369,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,   316,
     320,   321,   322,   317,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,   313,     0,   338,   339,   340,     0,   369,   314,
       0,     0,     0,   318,     0,     0,     0,     0,     0,     0,
     315,   691,     0,     0,     0,     0,     0,     0,     0,   319,
       0,   320,   321,   322,     0,   323,   324,   325,     0,   326,
     327,   328,   329,   330,     0,   331,   332,   333,   334,     0,
     335,   336,   337,     0,     0,   338,   339,   340,     0,   369,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   316,
       0,     0,   313,   317,     0,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   318,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   319,
       0,   320,   321,   322,     0,   323,   324,   325,     0,   326,
     327,   328,   329,   330,     0,   331,   332,   333,   334,   316,
     335,   336,   337,   317,     0,   338,   339,   340,     0,   369,
       0,   313,   709,     0,     0,     0,     0,     0,   314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,     0,     0,   318,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   319,
       0,   320,   321,   322,     0,   323,   324,   325,     0,   326,
     327,   328,   329,   330,     0,   331,   332,   333,   334,     0,
     335,   336,   337,     0,     0,   338,   339,   340,   316,   369,
       0,   313,   317,     0,     0,     0,     0,     0,   314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,   403,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,   316,   335,
     336,   337,   317,     0,   338,   339,   340,     0,   432,     0,
     313,     0,     0,     0,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   371,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   404,   338,   339,   340,   316,     0,     0,
     313,   317,     0,     0,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,   445,     0,     0,     0,   155,     0,     0,
       0,   318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   319,     0,   320,
     321,   322,   446,   323,   324,   325,     0,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,   316,   335,   336,
     337,   317,     0,   338,   339,   340,     0,     0,     0,   313,
       0,     0,     0,     0,     0,     0,   314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   315,   534,     0,
       0,   318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   535,     0,     0,     0,     0,     0,   319,     0,   320,
     321,   322,     0,   323,   324,   325,     0,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,     0,   335,   336,
     337,     0,     0,   338,   339,   340,   316,     0,     0,   313,
     317,     0,     0,     0,     0,     0,   314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   315,   536,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     318,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   319,     0,   320,   321,
     322,     0,   323,   324,   325,     0,   326,   327,   328,   329,
     330,     0,   331,   332,   333,   334,   316,   335,   336,   337,
     317,     0,   338,   339,   340,     0,     0,     0,   313,     0,
       0,     0,     0,     0,     0,   314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
     318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   319,     0,   320,   321,
     322,     0,   323,   324,   325,     0,   326,   327,   328,   329,
     330,     0,   331,   332,   333,   334,     0,   335,   336,   337,
       0,     0,   338,   339,   340,   316,     0,     0,   313,   317,
       0,     0,     0,     0,     0,   314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   318,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,   316,   335,   336,   337,   317,
     436,   338,   339,   340,     0,     0,     0,   313,     0,     0,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,   318,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   319,     0,   320,   321,   322,
       0,   323,   324,   325,     0,   326,   327,   328,   329,   330,
       0,   331,   332,   333,   334,     0,   335,   336,   337,     0,
     438,   338,   339,   340,   316,     0,     0,   313,   317,     0,
       0,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   319,     0,   320,   321,   322,     0,
     323,   324,   325,     0,   326,   327,   328,   329,   330,     0,
     331,   332,   333,   334,   316,   335,   336,   337,   317,   440,
     338,   339,   340,     0,     0,     0,   313,     0,     0,     0,
       0,     0,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   315,     0,     0,     0,   318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   319,     0,   320,   321,   322,     0,
     323,   324,   325,     0,   326,   327,   328,   329,   330,     0,
     331,   332,   333,   334,     0,   335,   336,   337,     0,   448,
     338,   339,   340,   316,     0,     0,   313,   317,     0,     0,
       0,     0,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   315,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   465,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,   316,   335,   336,   337,   317,     0,   338,
     339,   340,     0,     0,     0,   313,   538,     0,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,     0,   338,
     339,   340,   316,     0,     0,   313,   317,     0,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   318,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   319,     0,   320,   321,   322,     0,   323,   324,
     325,     0,   326,   327,   328,   329,   330,  -481,   331,   332,
     333,   334,   316,   335,   336,   337,   317,     0,   338,   339,
     340,     0,     0,     0,     0,   313,     0,     0,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   155,   315,     0,     0,   318,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   319,     0,   320,   321,   322,     0,   323,   324,
     325,     0,   326,   327,   328,   329,   330,     0,   331,   332,
     333,   334,     0,   335,   336,   337,     0,     0,   338,   339,
     340,     0,   316,     0,     0,     0,   317,     0,     0,     0,
       0,     0,     0,     0,   313,   704,     0,     0,     0,     0,
       0,   314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,   318,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   319,     0,   320,   321,   322,     0,   323,   324,
     325,     0,   326,   327,   328,   329,   330,     0,   331,   332,
     333,   334,     0,   335,   336,   337,     0,   680,   338,   339,
     340,   316,     0,     0,   313,   317,     0,     0,     0,     0,
       0,   314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   315,     0,     0,   707,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   318,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   319,     0,   320,   321,   322,     0,   323,   324,   325,
       0,   326,   327,   328,   329,   330,     0,   331,   332,   333,
     334,   316,   335,   336,   337,   317,     0,   338,   339,   340,
       0,     0,     0,   313,     0,     0,     0,     0,     0,     0,
     314,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,   318,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   319,     0,   320,   321,   322,     0,   323,   324,   325,
       0,   326,   327,   328,   329,   330,     0,   331,   332,   333,
     334,     0,   335,   336,   337,     0,     0,   338,   339,   340,
     316,     0,     0,   313,   317,     0,     0,     0,     0,     0,
     314,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   315,   809,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   749,   318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     319,     0,   320,   321,   322,     0,   323,   324,   325,     0,
     326,   327,   328,   329,   330,     0,   331,   332,   333,   334,
     316,   335,   336,   337,   317,     0,   338,   339,   340,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,   812,     0,     0,   318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     319,     0,   320,   321,   322,     0,   323,   324,   325,     0,
     326,   327,   328,   329,   330,     0,   331,   332,   333,   334,
       0,   335,   336,   337,     0,     0,   338,   339,   340,   316,
       0,     0,   313,   317,     0,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   318,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   319,
       0,   320,   321,   322,     0,   323,   324,   325,     0,   326,
     327,   328,   329,   330,     0,   331,   332,   333,   334,   316,
     335,   336,   337,   317,     0,   338,   339,   340,     0,     0,
       0,   313,   883,     0,     0,     0,     0,     0,   314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,     0,     0,   318,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   319,
       0,   320,   321,   322,     0,   323,   324,   325,     0,   326,
     327,   328,   329,   330,     0,   331,   332,   333,   334,     0,
     335,   336,   337,     0,     0,   338,   339,   340,   316,     0,
       0,     0,   317,     0,     0,     0,   313,   900,     0,     0,
       0,   886,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   315,     0,     0,     0,     0,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,     0,     0,
       0,   313,     0,     0,     0,     0,     0,     0,   314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,     0,     0,     0,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,     0,     0,     0,   313,     0,     0,     0,
       0,   982,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   315,     0,     0,     0,     0,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,     0,     0,
       0,   313,     0,     0,     0,     0,   983,     0,   314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,     0,     0,     0,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,     0,     0,     0,   313,     0,     0,     0,
       0,   984,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   315,     0,     0,     0,     0,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,     0,     0,
       0,   313,     0,     0,     0,     0,   985,     0,   314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,     0,     0,     0,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,     0,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,   316,   338,
     339,   340,   317,     0,     0,     0,   313,     0,     0,     0,
       0,   986,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   315,     0,     0,     0,     0,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     320,   321,   322,     0,   323,   324,   325,     0,   326,   327,
     328,   329,   330,     0,   331,   332,   333,   334,     0,   335,
     336,   337,     0,   316,   338,   339,   340,   317,     0,     0,
       0,     0,     0,     0,     0,     0,   987,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   318,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,   320,   321,   322,     0,   323,
     324,   325,  1009,   326,   327,   328,   329,   330,     0,   331,
     332,   333,   334,     0,   335,   336,   337,     0,     0,   338,
     339,   340,     0,     0,     0,     0,     0,   316,     0,     0,
     313,   317,     0,     0,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   319,     0,   320,
     321,   322,     0,   323,   324,   325,     0,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,   316,   335,   336,
     337,   317,     0,   338,   339,   340,     0,     0,     0,   313,
       0,     0,     0,     0,     0,     0,   314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   315,     0,     0,
       0,   318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   319,     0,   320,
     321,   322,     0,   323,   324,   325,     0,   326,   327,   328,
     329,   330,     0,   331,   332,   333,   334,     0,   335,   336,
     337,     0,     0,   338,   339,   340,   668,     0,     0,     0,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   319,     0,   320,   321,
     322,     0,   323,   324,   325,     0,   326,   327,   328,   329,
     330,     0,   331,   332,   333,   334,    16,   335,   336,   337,
      20,    21,   338,   339,   340,     0,     0,     0,     0,    27,
      16,    28,   866,     0,    20,    21,    33,     0,     0,     0,
       0,    37,     0,    27,     0,    28,   866,     0,     0,     0,
      33,     0,     0,     0,     0,    37,     0,     0,     0,     0,
       0,     0,    48,     0,    50,     0,    52,     0,   867,     0,
       0,   868,     0,    57,     0,    58,    48,     0,    50,     0,
     300,     0,   867,     0,     0,   868,     0,    57,     0,    58,
     206,     0,     0,     0,    74,     0,     0,    76,     0,     0,
      79,     0,   207,   208,     0,     0,     0,     0,    74,   209,
       0,    76,     0,     0,    79,     0,     0,   210,     0,     0,
       0,     0,     0,   211,     0,     0,     0,     0,   212,     0,
     213,     0,     0,   214,     0,     0,     0,     0,     0,     0,
       0,   215,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,   216,     0,   944,     0,     0,     0,     0,
       0,     0,   217,    92,     0,     0,     0,     0,     0,     0,
       0,   218,   219,     0,   220,     0,   221,     0,   222,     0,
       0,   223,     0,     0,     0,   224,     0,     0,   225
};

static const yytype_int16 yycheck[] =
{
      11,   140,    60,   441,    15,   199,     2,    18,    54,    55,
      59,    17,    20,    24,   560,   725,    28,    28,    29,   655,
      31,    32,    33,    34,   186,   205,   178,   362,   410,    78,
       1,    42,   413,    44,    45,   418,    47,    95,    49,   191,
      51,   128,     1,   205,     1,   751,     1,    96,    59,    60,
      61,    62,    63,    64,   593,   915,    29,    68,   441,    70,
      29,    72,    73,   243,    24,   953,    77,    78,    63,    80,
      81,    42,   107,    84,   800,   801,    87,    88,    89,    29,
      91,   243,   262,    94,    95,    96,    29,    29,   999,    80,
      42,    52,   179,    52,   804,   132,   403,    77,   132,     0,
     262,    81,   296,   297,    42,   839,    63,    67,     1,   104,
     152,   999,    42,   107,    42,     3,    42,   166,    78,   665,
      28,    80,   107,   160,   158,  1036,    42,   162,   667,    42,
     269,   842,   132,   843,    31,    32,     3,    42,   107,   446,
      78,   112,    28,   103,   105,    80,   105,   104,    78,   160,
     161,   162,   163,   164,   165,   166,  1016,   112,   158,    52,
     131,   132,   324,   158,   870,   107,   157,     3,   162,    77,
     904,   177,   131,    81,    19,   186,   131,   132,   358,   359,
     132,   152,   132,   722,   157,   193,   892,    80,   157,   201,
     201,    77,   200,   903,   205,    81,   358,   359,   157,    96,
      93,   158,   107,    91,   132,   160,   132,   157,    78,   935,
     936,   592,   105,    80,   157,   157,   132,   152,   929,   132,
      24,  1014,   157,   131,    91,   560,   134,   132,   132,   105,
     138,    76,   243,   244,   162,   159,   162,   959,   131,  1032,
     251,   252,   952,   879,   159,   131,   162,   133,   134,   162,
      47,   262,   138,   112,   434,   100,   132,   162,   162,   152,
     146,    77,     7,    67,   157,    81,   132,   153,   154,   155,
     159,   132,   434,   152,    78,    72,    19,   136,   824,   158,
     291,  1003,  1004,  1005,  1006,  1007,  1008,    72,    73,   347,
     348,   152,   288,    97,   160,   353,    88,   159,   356,   357,
     387,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,    52,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     665,   132,   714,    76,     3,   716,   347,   348,   157,    73,
     389,    80,   353,   132,   132,   356,   357,   358,   359,   360,
     361,   362,   152,   132,    93,    28,   132,   100,    24,   160,
     371,    30,   132,   152,   142,   376,   105,   378,   152,   751,
     564,   565,   160,   152,   132,   132,    42,   132,   389,    45,
     391,   392,   132,   152,   160,    54,   397,   398,   550,   158,
     160,   402,   403,   401,   405,   152,   407,   455,    64,   457,
     152,    67,   160,    69,    77,   160,    42,   134,    81,   420,
     160,   849,    78,   152,   131,    52,   427,   134,   157,   316,
     317,   138,    91,   434,   473,   132,    62,    24,    97,    24,
     159,    97,   161,   132,   445,   446,    72,    73,   632,   633,
     159,   452,   161,    80,   455,   152,   457,   132,   152,   152,
     461,   462,   463,   152,   465,   466,    93,   152,   131,   842,
     133,   134,   473,   360,   361,   138,   849,   152,   105,   621,
      67,   152,    67,   146,   371,   471,   159,    52,   152,   490,
     491,    78,   155,    78,   112,   159,   157,   161,   152,   824,
     548,   152,   159,   875,   161,   392,   106,   455,   152,   457,
      97,   398,    97,    28,   132,    80,   103,   159,   136,   159,
     892,   159,   112,   152,     7,   152,   655,    56,    93,   131,
     157,   152,   157,   534,   535,   536,   537,   538,   539,   540,
     105,   112,   160,   160,   132,   541,   542,   548,   152,   550,
     176,    42,   159,   554,   112,   556,   929,    31,   606,   560,
     132,   562,    77,   602,   160,    42,    81,   160,   112,   160,
     196,   197,   198,   199,   461,    52,   160,    18,   204,   137,
     953,   139,   140,   132,    62,   143,   144,   152,   162,   162,
     157,   152,   157,   594,   160,   968,   597,   598,   599,   600,
     601,   602,   152,    80,   152,   606,    24,   152,   162,   152,
     236,   152,   127,   239,   129,   130,   131,   152,   133,   134,
     132,     2,   152,   138,    42,    31,   999,    45,   105,    28,
      11,   146,   258,   112,   160,   112,   158,   160,   153,   154,
     155,  1014,    23,   152,    25,   152,    64,   132,   535,    67,
     537,    69,   278,   132,   158,   132,   112,   136,    78,  1032,
      78,   162,    29,  1036,   665,    29,    31,   668,   669,   670,
     157,   131,   673,    78,    42,   152,   162,   131,    77,    97,
     157,   160,    81,   160,    52,   134,   162,    68,   689,   162,
     691,   158,   134,   732,   159,   696,   134,   134,   699,   134,
     134,   702,   134,   704,   134,   134,   707,   134,   134,    31,
     160,   158,    80,    94,   132,   160,    93,    98,   196,   197,
     198,   199,   162,   132,   302,   446,    22,   445,   127,   736,
     270,   732,   131,   192,   133,   134,   552,   105,   817,   138,
     754,   879,   160,   744,   112,   746,   372,   146,   749,   875,
     879,   585,   753,   757,   153,   154,   155,   714,   236,  1016,
     952,   239,   978,    95,   132,  1009,  1002,    33,   421,   550,
     396,    -1,    -1,    -1,   400,   156,    -1,    -1,    -1,     3,
     258,   668,   669,    -1,   152,    -1,    -1,    -1,    -1,   157,
      -1,    15,   160,    -1,    -1,   796,    -1,    -1,    22,    -1,
     278,    -1,    -1,    -1,    -1,    42,    -1,   808,   809,    -1,
     811,   812,   813,   814,    -1,    52,    -1,    41,   819,   820,
      -1,   822,    46,   824,    -1,   826,   827,    -1,    -1,   830,
      54,   867,   868,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,   226,    -1,   848,   849,    -1,
      -1,    75,   233,   854,   186,   856,    -1,    -1,    24,   860,
      -1,    -1,    -1,    87,    -1,   866,   905,    91,   105,    -1,
      -1,    -1,    -1,   205,    98,   112,    42,    -1,    -1,    45,
      -1,    -1,   883,   884,   885,   886,   887,   888,   889,   890,
      -1,    -1,    -1,    -1,   372,   132,    -1,    -1,    64,   900,
      -1,    67,    -1,    69,   905,    -1,    -1,   288,    -1,   910,
      -1,   243,    78,   914,    -1,   152,    23,    24,   396,    -1,
     157,   302,   400,   160,    -1,    -1,    -1,    34,    35,    -1,
     262,    97,   558,   559,   973,    -1,    -1,    44,   564,   565,
      -1,    28,     2,    -1,   945,   946,   947,   948,    -1,    -1,
     576,    11,    -1,     2,    -1,    -1,    -1,    -1,    65,    -1,
      67,    -1,    11,    23,    -1,    25,    -1,   968,    -1,   970,
     971,    78,   973,    -1,    23,    -1,    25,    -1,    -1,    -1,
      -1,   982,   983,   984,   985,   986,   987,    -1,   369,    -1,
      77,   617,    -1,    -1,    81,    -1,   103,   998,   999,    -1,
    1001,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,
     391,    -1,    72,    73,    -1,    -1,   397,    -1,    -1,    68,
      -1,    -1,    -1,    -1,   405,    -1,   358,   359,    -1,    -1,
      -1,  1032,    -1,    -1,    94,  1036,    -1,    -1,    98,    -1,
     127,    -1,   129,   130,   131,    94,   133,   134,   135,    98,
      -1,   138,   678,   679,    -1,    -1,    -1,    -1,   145,   146,
     686,    -1,   688,   150,    -1,    28,   153,   154,   155,   695,
      39,    -1,   698,    23,    24,   701,    -1,    -1,   459,    -1,
     558,   559,   463,    -1,    34,    35,   564,   565,    -1,    -1,
     471,    -1,    -1,    -1,    44,   427,   156,    47,    -1,    -1,
      -1,    -1,   434,    -1,    -1,    74,    -1,   156,    -1,    -1,
      -1,    -1,    -1,   739,    77,    65,    -1,    67,    81,    -1,
     452,    -1,    72,   455,    -1,   457,    -1,    -1,    78,    -1,
     462,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,
     531,     2,   121,   103,   123,    -1,    -1,    -1,    -1,    -1,
      11,    23,    -1,    25,   127,    -1,   226,   130,   131,    -1,
     133,   134,    23,   233,    25,   138,    -1,   226,    -1,    -1,
      -1,    -1,    -1,   146,   233,    -1,    -1,    -1,    -1,    -1,
     153,   154,   155,    -1,   575,    -1,    -1,    -1,    -1,    -1,
     678,   679,    -1,    -1,    -1,    -1,    68,    -1,   686,   178,
      -1,    -1,    -1,    -1,    -1,    -1,   548,    68,   550,    -1,
     698,    -1,   191,   701,    -1,   194,   852,    -1,   288,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    98,    -1,    -1,   288,
      -1,    -1,   302,    94,    -1,    -1,    -1,    98,    -1,    -1,
     631,    -1,    -1,   302,    -1,    -1,   637,    -1,    -1,    -1,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   894,   895,
      -1,    -1,   898,   899,   655,    -1,    -1,    -1,   247,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   668,   669,   915,
      -1,    -1,    -1,    -1,   156,   921,    -1,   923,    -1,   925,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   689,   369,
     691,    -1,    -1,    -1,    -1,   696,    -1,    -1,   699,    -1,
     369,    -1,    -1,    -1,    -1,   706,    -1,    -1,   709,    -1,
      -1,   391,    -1,    -1,    -1,    -1,    -1,   397,    -1,    -1,
      -1,    -1,   391,    -1,    -1,   405,    -1,    -1,   397,    -1,
     731,    -1,    -1,    -1,    -1,    -1,   405,    -1,    -1,    -1,
      -1,    -1,    -1,   744,   226,   746,    -1,    -1,    -1,    -1,
      -1,   233,    -1,    -1,   852,   226,    -1,    -1,    -1,    -1,
      -1,    -1,   233,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1016,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   459,
      -1,    -1,    -1,   463,    -1,    -1,    -1,    -1,    19,    -1,
     459,   471,    23,    24,   463,    -1,    -1,   386,    -1,    -1,
      -1,    32,   471,    34,    35,    -1,   288,    -1,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,   288,   819,    -1,
     302,    -1,    -1,    -1,   413,   826,    -1,    -1,    -1,    -1,
      -1,   302,    -1,    -1,    65,    -1,    67,   838,    69,    -1,
      71,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,   850,
      -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,   859,    -1,
      -1,    -1,   863,    -1,    -1,    11,    97,    -1,    -1,   100,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    23,    -1,    25,
      -1,    -1,    -1,   472,    -1,    -1,   475,   369,    -1,    -1,
     842,    -1,     3,    -1,    -1,   575,    -1,    -1,   369,    -1,
     901,    -1,    -1,    -1,    15,    16,   575,    -1,    -1,   391,
      -1,    22,    -1,    -1,    -1,   397,    -1,    -1,    -1,    30,
     391,   152,    68,   405,    -1,    36,   397,   158,    -1,    -1,
      41,    -1,    43,    -1,   405,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,   945,   946,   947,   948,    94,    -1,
      -1,   631,    98,    -1,    -1,    66,    -1,   637,   910,    -1,
      -1,    -1,   631,    -1,    75,    -1,    -1,    -1,   637,   970,
     971,    -1,    -1,    84,    85,    -1,    87,   459,    89,    -1,
      91,   463,    -1,    94,    -1,    -1,    -1,    98,   459,   471,
     101,    -1,   463,    -1,    -1,    -1,    28,    -1,    -1,   588,
     471,   953,    -1,   592,    -1,    -1,    -1,    -1,    -1,   689,
     156,   691,    -1,    -1,    -1,    -1,   696,    -1,    -1,   699,
     689,    -1,   691,    -1,    -1,    -1,   706,   696,   617,   709,
     699,    -1,   621,    -1,    -1,    -1,    -1,   706,    -1,    -1,
     709,    -1,    -1,    -1,    -1,    77,   157,   999,   159,    81,
      -1,   731,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   731,    -1,   744,    -1,   746,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   744,    -1,   746,    -1,   111,
     226,    -1,    -1,    -1,  1036,    -1,    -1,   233,    -1,    -1,
      -1,    -1,    -1,   575,    -1,   127,    -1,   129,   130,   131,
      -1,   133,   134,   135,   575,   137,   138,   139,   140,    -1,
      -1,   143,   144,   145,   146,    -1,   148,   149,   150,    -1,
      -1,   153,   154,   155,   713,    -1,    -1,   716,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   819,
      -1,    -1,   288,    -1,    -1,    -1,   826,   736,    -1,   631,
     819,    -1,    -1,    -1,    -1,   637,   302,   826,   838,    -1,
     631,    -1,    -1,    -1,    -1,   754,   637,    -1,    -1,   838,
     850,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   859,
      -1,   850,    -1,   863,    -1,    -1,   775,    -1,    -1,    -1,
     859,    -1,     2,    28,   863,    -1,    -1,    -1,    -1,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    -1,   689,    -1,   691,
      -1,    -1,    -1,    23,   696,    25,    -1,   699,   689,    -1,
     691,   901,    -1,   369,   706,   696,    -1,   709,   699,    -1,
      -1,    -1,   901,    -1,    -1,   706,    -1,    -1,   709,    -1,
      -1,    -1,    77,    -1,    -1,   391,    81,    -1,    -1,   731,
      -1,   397,    -1,    -1,    -1,    -1,    -1,    -1,    68,   405,
     731,    -1,   744,    -1,   746,   945,   946,   947,   948,    -1,
      -1,    -1,    -1,   744,    -1,   746,   945,   946,   947,   948,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    98,    -1,
     970,   971,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,   970,   971,   138,   139,   140,    -1,    -1,   143,   144,
     145,   146,    -1,   459,    -1,   150,    -1,   463,   153,   154,
     155,    -1,    -1,    -1,    -1,   471,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,   819,    -1,    -1,
      -1,    -1,    -1,    -1,   826,    -1,   156,    -1,   819,    -1,
      -1,    -1,    -1,    -1,    -1,   826,   838,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   838,   850,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   859,    -1,   850,
      -1,   863,    77,    -1,    -1,    -1,    81,    -1,   859,    -1,
       2,    -1,   863,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      11,    23,    -1,    25,    -1,    -1,   226,    -1,    -1,   901,
      -1,    -1,    23,   233,    25,    -1,    -1,    -1,    -1,   575,
     901,    -1,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,    -1,    -1,   143,   144,
     145,   146,    -1,   148,    -1,   150,    68,    -1,   153,   154,
     155,    -1,    -1,   945,   946,   947,   948,    68,    -1,    -1,
      -1,    -1,    -1,    -1,   945,   946,   947,   948,   288,    -1,
      -1,    -1,    94,    -1,    -1,   631,    98,    -1,   970,   971,
      -1,   637,   302,    94,    -1,    -1,    -1,    98,    -1,   970,
     971,    -1,    -1,     1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    23,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    34,    35,    -1,    -1,
      -1,    39,    -1,   689,   156,   691,    44,    -1,    -1,    47,
     696,    -1,    -1,   699,    -1,   156,    -1,    -1,    -1,   369,
     706,    -1,    -1,   709,    -1,    -1,    -1,    65,    -1,    67,
       1,    69,    -1,    71,    72,    -1,    74,    -1,    76,    -1,
      78,   391,    -1,    -1,    -1,   731,    -1,   397,    19,    -1,
      -1,    -1,    23,    24,    -1,   405,    -1,    -1,   744,    97,
     746,    32,   100,    34,    35,   103,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,   226,    -1,    47,    -1,    -1,    -1,
      -1,   233,    -1,    -1,    -1,   226,    -1,    -1,    -1,    -1,
      -1,    -1,   233,    -1,    65,    -1,    67,    -1,    69,    -1,
      71,    72,    -1,    74,    -1,    76,    -1,    78,    -1,   459,
      -1,     3,    -1,   463,   152,    -1,    -1,    -1,    -1,    -1,
     158,   471,    -1,    15,    16,    -1,    97,    -1,    -1,   100,
      22,    -1,   103,   819,    -1,    -1,   288,    -1,    30,    -1,
     826,    -1,    -1,    -1,    36,    -1,    -1,   288,    -1,    41,
     302,    43,   838,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,   302,    54,    -1,   850,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   859,    66,    -1,    -1,   863,    -1,    -1,
      -1,   152,    -1,    75,    -1,    -1,    -1,   158,    -1,     1,
      -1,     3,    84,    85,    -1,    87,    -1,    89,    -1,    91,
      -1,    -1,    94,    15,    16,    -1,    98,    99,    -1,   101,
      22,    -1,    -1,    -1,    -1,   901,    -1,   369,    30,    -1,
      -1,    -1,    -1,    -1,    36,   575,    -1,    -1,   369,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    -1,   391,
      -1,    -1,    54,    -1,    -1,   397,    -1,    -1,    -1,    -1,
     391,    -1,    -1,   405,    66,    -1,   397,    -1,    -1,   945,
     946,   947,   948,    75,    -1,    -1,    -1,   159,    -1,    -1,
      -1,    -1,    84,    85,    -1,    87,    -1,    89,    -1,    91,
      -1,   631,    94,    -1,   970,   971,    98,   637,    -1,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,    -1,    -1,    -1,    -1,   459,    -1,    10,
      -1,   463,    -1,    -1,    15,    16,    17,    -1,   459,   471,
      -1,    22,   463,    -1,    -1,    -1,    -1,    -1,    -1,    30,
     471,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,   689,
      41,   691,    43,    -1,    -1,    46,   696,    48,    -1,   699,
      -1,    -1,    -1,    54,    -1,    -1,   706,    -1,    -1,   709,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,   731,    -1,    84,    85,    -1,    87,    -1,    89,    -1,
      91,    -1,    -1,    94,   744,    -1,   746,    98,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,   116,   117,    -1,    -1,   120,
     121,   122,   123,   575,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,    -1,   575,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    -1,   150,
      -1,    -1,   153,   154,   155,   156,    -1,    -1,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   819,
      -1,    -1,    -1,    -1,    -1,    -1,   826,    -1,    -1,   631,
      -1,    -1,    -1,    -1,    -1,   637,    -1,    -1,   838,     3,
     631,    -1,    -1,    -1,    -1,    -1,   637,    -1,    -1,    -1,
     850,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,   859,
      -1,    -1,    -1,   863,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    -1,   689,    -1,   691,
      54,    -1,    -1,    -1,   696,    -1,    -1,   699,   689,    -1,
     691,   901,    66,    -1,   706,   696,    -1,   709,   699,    -1,
      -1,    75,    -1,    -1,    -1,   706,    -1,    -1,   709,    -1,
      84,    85,    -1,    87,    -1,    89,    -1,    91,    -1,   731,
      94,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,     3,
     731,    -1,   744,    -1,   746,   945,   946,   947,   948,    -1,
      -1,    15,    16,   744,    -1,   746,    -1,    -1,    22,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
     970,   971,    36,    10,    -1,    -1,    -1,    41,    -1,    43,
      17,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    28,    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    75,    -1,    -1,    81,    -1,    -1,   819,    -1,    -1,
      84,    85,    -1,    87,   826,    89,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    98,    -1,   838,   101,    -1,    -1,
      77,    -1,    -1,    -1,    81,    -1,    -1,   838,   850,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   859,    -1,   850,
     127,   863,   129,   130,   131,    -1,   133,   134,   859,    -1,
     107,   138,   863,    -1,   111,    -1,    -1,    -1,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,   153,   154,   155,    -1,
     127,    -1,   129,   130,   131,   159,   133,   134,   135,   901,
     137,   138,   139,   140,   141,    -1,   143,   144,   145,   146,
     901,   148,   149,   150,    -1,    -1,   153,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   945,   946,   947,   948,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,   970,   971,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    57,    58,
      -1,    -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    -1,    94,    95,    96,    97,    98,
      -1,   100,   101,   102,   103,    -1,    -1,   106,    -1,   108,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,
     149,   150,    -1,   152,    -1,    -1,   155,    -1,   157,    -1,
     159,     1,   161,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,    89,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    98,    -1,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,
     150,   151,    -1,    -1,    -1,   155,    -1,   157,    -1,   159,
      -1,   161,   162,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    13,    14,    15,    16,    17,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    -1,    -1,    61,    -1,    -1,    -1,    65,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    -1,    94,    95,    96,    97,
      98,    -1,   100,   101,   102,   103,    -1,    -1,   106,    -1,
     108,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,    -1,   143,   144,   145,   146,   147,
     148,   149,   150,    -1,   152,   153,   154,   155,    -1,   157,
     158,   159,     1,   161,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    57,    58,
      -1,    -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    -1,    94,    95,    96,    97,    98,
      -1,   100,   101,   102,   103,    -1,    -1,   106,    -1,   108,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,
     149,   150,   151,   152,    -1,    -1,   155,    -1,   157,   158,
     159,     1,   161,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    -1,    94,    95,    96,    97,    98,    -1,
     100,   101,   102,   103,    -1,    -1,   106,    -1,   108,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,
     150,    -1,   152,    -1,    -1,   155,    -1,   157,   158,   159,
       1,   161,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    57,    58,    -1,    -1,
      61,    -1,    -1,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    -1,    94,    95,    96,    97,    98,    -1,   100,
     101,   102,   103,    -1,    -1,   106,    -1,   108,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,   150,
      -1,   152,    -1,    -1,   155,    -1,   157,   158,   159,     1,
     161,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    -1,    -1,    61,
      -1,    -1,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    -1,    94,    95,    96,    97,    98,    -1,   100,   101,
     102,   103,    -1,    -1,   106,    -1,   108,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    -1,   147,    -1,   149,   150,    -1,
     152,    -1,    -1,   155,    -1,   157,   158,   159,     1,   161,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    -1,    -1,    61,    -1,
      -1,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      -1,    94,    95,    96,    97,    98,    -1,   100,   101,   102,
     103,    -1,    -1,   106,    -1,   108,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,    -1,   147,    -1,   149,   150,    -1,   152,
      -1,    -1,   155,    -1,   157,   158,   159,     1,   161,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    -1,    -1,    61,    -1,    -1,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    -1,
      94,    95,    96,    97,    98,    -1,   100,   101,   102,   103,
      -1,    -1,   106,    -1,   108,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,   147,    -1,   149,   150,    -1,   152,    -1,
      -1,   155,    -1,   157,    -1,   159,     1,   161,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    56,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,
      65,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    -1,    94,
      95,    96,    97,    98,    -1,   100,   101,   102,   103,    -1,
      -1,   106,    -1,   108,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    -1,   147,    -1,   149,   150,    -1,   152,    -1,    -1,
     155,    -1,   157,    -1,   159,     1,   161,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,    65,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    -1,    94,    95,
      96,    97,    98,    -1,   100,   101,   102,   103,    -1,    -1,
     106,    -1,   108,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,   147,    -1,   149,   150,    -1,   152,    -1,    -1,   155,
      -1,   157,    -1,   159,     1,   161,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    55,    56,
      57,    58,    -1,    -1,    61,    -1,    -1,    -1,    65,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    -1,    94,    95,    96,
      97,    98,    -1,   100,   101,   102,   103,    -1,    -1,   106,
      -1,   108,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,
     147,    -1,   149,   150,    -1,   152,    -1,    -1,   155,    -1,
     157,    -1,   159,     1,   161,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    -1,    -1,    61,    -1,    -1,    -1,    65,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    79,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    -1,    94,    95,    96,    97,
      98,    -1,   100,   101,   102,   103,    -1,    -1,   106,    -1,
     108,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,   147,
      -1,   149,   150,    -1,   152,    -1,    -1,   155,    -1,   157,
      -1,   159,     1,   161,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    57,    58,
      -1,    -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    -1,    94,    95,    96,    97,    98,
      -1,   100,   101,   102,   103,    -1,    -1,   106,    -1,   108,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,
     149,   150,    -1,   152,    -1,    -1,   155,    -1,   157,    -1,
     159,     1,   161,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    -1,    94,    95,    96,    97,    98,    -1,
     100,   101,   102,   103,    -1,    -1,   106,    -1,   108,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,
     150,    -1,   152,    -1,    -1,   155,    -1,   157,    -1,   159,
       1,   161,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    57,    58,    -1,    -1,
      61,    -1,    -1,    -1,    65,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    76,    -1,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    -1,    94,    95,    96,    97,    98,    -1,   100,
     101,   102,   103,    -1,    -1,   106,    -1,   108,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,   150,
      -1,   152,    -1,    -1,   155,    -1,   157,    -1,   159,     1,
     161,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    -1,    -1,    61,
      -1,    -1,    -1,    65,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    -1,    94,    95,    96,    97,    98,    -1,   100,   101,
     102,   103,    -1,    -1,   106,    -1,   108,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    -1,   147,    -1,   149,   150,    -1,
     152,    -1,    -1,   155,    -1,   157,    -1,   159,     1,   161,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    -1,    -1,    61,    -1,
      -1,    -1,    65,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      -1,    94,    95,    96,    97,    98,    -1,   100,   101,   102,
     103,    -1,    -1,   106,    -1,   108,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,    -1,   147,    -1,   149,   150,    -1,   152,
      -1,    -1,   155,    -1,   157,    -1,   159,     1,   161,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    -1,    -1,    61,    -1,    -1,
      -1,    65,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    -1,
      94,    95,    96,    97,    98,    -1,   100,   101,   102,   103,
      -1,    -1,   106,    -1,   108,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,   147,    -1,   149,   150,    -1,   152,    -1,
      -1,   155,    -1,   157,    -1,   159,     1,   161,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    68,    -1,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    -1,   147,    -1,   149,   150,   151,    -1,    -1,    -1,
     155,    -1,   157,    -1,   159,    -1,   161,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    68,    -1,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      86,    87,    88,    89,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    98,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,   147,    -1,   149,   150,   151,    -1,    -1,    -1,   155,
      -1,   157,    -1,   159,    -1,   161,   162,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    68,    -1,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      86,    87,    88,    89,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    98,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,   147,    -1,   149,   150,   151,    -1,    -1,    -1,   155,
      -1,   157,    -1,   159,    -1,   161,   162,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    68,    -1,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      86,    87,    88,    89,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    98,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,   147,    -1,   149,   150,    -1,    -1,    -1,    -1,   155,
      -1,   157,    -1,   159,    -1,   161,   162,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    -1,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    84,    85,
      86,    87,    88,    89,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    98,    -1,    -1,   101,    -1,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,   147,    -1,   149,   150,    -1,    -1,    -1,    -1,   155,
      -1,   157,    -1,   159,    -1,   161,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    19,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    68,    -1,    70,    -1,    -1,    -1,    -1,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    86,
      87,    88,    89,    -1,    91,    -1,    -1,    94,    -1,    -1,
      97,    98,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   145,    -1,
     147,    12,   149,   150,    15,    16,    -1,    -1,   155,    -1,
     157,    22,   159,    -1,   161,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      91,    -1,    -1,    94,    95,    96,    -1,    98,    99,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,   150,
     151,    -1,    -1,    -1,   155,    -1,   157,    -1,   159,    -1,
     161,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    68,    -1,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    86,    87,    88,    89,    -1,    91,
      -1,    -1,    94,    95,    96,    -1,    98,    99,    -1,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    -1,   147,    -1,   149,   150,   151,
      -1,    -1,    -1,   155,    -1,   157,    -1,   159,   160,   161,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    68,    -1,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    91,    -1,
      -1,    94,    95,    96,    -1,    98,    99,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,    -1,   147,    -1,   149,   150,   151,    -1,
      -1,    -1,   155,    -1,   157,    -1,   159,   160,   161,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    42,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    68,    -1,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      84,    85,    86,    87,    88,    89,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,   147,    -1,   149,   150,    -1,    -1,    -1,
      -1,   155,    -1,   157,    -1,   159,    -1,   161,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    19,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    68,    -1,    70,    -1,    -1,    -1,    -1,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    -1,    -1,    94,
      -1,    -1,    97,    98,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     145,    -1,   147,    12,   149,   150,    15,    16,    -1,    -1,
     155,    -1,   157,    22,   159,    -1,   161,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,
      -1,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,
      89,    -1,    91,    -1,    -1,    94,    95,    96,    -1,    98,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,
     149,   150,    -1,   152,    -1,    -1,   155,    -1,   157,    -1,
     159,    -1,   161,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,    89,
      -1,    91,    -1,    -1,    94,    95,    96,    -1,    98,    -1,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,
     150,   151,    -1,    -1,    -1,   155,    -1,   157,    -1,   159,
      -1,   161,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      91,    -1,    -1,    94,    95,    96,    -1,    98,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   145,    -1,   147,    12,   149,   150,
      15,    16,    -1,    -1,   155,    -1,   157,    22,   159,    -1,
     161,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    -1,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     145,    -1,   147,    12,   149,   150,    15,    16,    -1,    -1,
     155,    -1,   157,    22,   159,    -1,   161,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,
      -1,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,
      89,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    98,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,
     149,   150,   151,    -1,    -1,    -1,   155,    -1,   157,    -1,
     159,    -1,   161,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,    89,
      -1,    91,    -1,    -1,    94,    95,    96,    -1,    98,    -1,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   145,    -1,   147,    12,   149,
     150,    15,    16,    -1,    -1,   155,    -1,   157,    22,   159,
      -1,   161,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    68,    -1,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    86,    87,    88,    89,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,   147,    -1,   149,   150,   151,    -1,    -1,
      -1,   155,    -1,   157,   158,   159,    -1,   161,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    68,    -1,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     145,    -1,   147,    12,   149,   150,    15,    16,    -1,    -1,
     155,    -1,   157,    22,   159,    -1,   161,    -1,    -1,    -1,
      29,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,
      -1,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,
      89,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    98,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   145,    -1,   147,    12,
     149,   150,    15,    16,    -1,    -1,   155,    -1,   157,    22,
     159,    -1,   161,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    68,    -1,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,    -1,   147,    -1,   149,   150,   151,    -1,
      -1,    -1,   155,    -1,   157,    -1,   159,    -1,   161,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    68,    -1,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    86,    87,    88,    89,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,   147,    -1,   149,   150,   151,    -1,    -1,
      -1,   155,    -1,   157,    -1,   159,    -1,   161,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    68,    -1,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     145,    -1,   147,    12,   149,   150,    15,    16,    -1,    -1,
     155,    -1,   157,    22,   159,    -1,   161,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,
      -1,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,
      89,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    98,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,
     149,   150,   151,    -1,    -1,    -1,   155,    -1,   157,    -1,
     159,    -1,   161,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,    89,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    98,    -1,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,
     150,   151,    -1,    -1,    -1,   155,    -1,   157,    -1,   159,
      -1,   161,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    98,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   145,    -1,   147,    12,   149,   150,
      15,    16,    -1,    -1,   155,    -1,   157,    22,   159,    -1,
     161,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    68,    -1,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     145,    -1,   147,    12,   149,   150,    15,    16,    -1,    -1,
     155,    -1,   157,    22,   159,    -1,   161,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,
      -1,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,
      89,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    98,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   145,    -1,   147,    12,
     149,   150,    15,    16,    -1,    -1,   155,    -1,   157,    22,
     159,    -1,   161,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    68,    -1,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   145,    -1,   147,    12,   149,   150,    15,    16,
      -1,    -1,   155,    -1,   157,    22,   159,    -1,   161,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    68,    -1,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    86,
      87,    88,    89,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    98,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   145,    -1,
     147,    12,   149,   150,    15,    16,    -1,    -1,   155,    -1,
     157,    22,   159,    -1,   161,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    98,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   145,    -1,   147,    12,   149,   150,
      15,    16,    -1,    -1,   155,    -1,   157,    22,   159,    -1,
     161,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    68,    -1,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     145,    -1,   147,    12,   149,   150,    15,    16,    -1,    -1,
     155,    -1,   157,    22,   159,    -1,   161,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,
      -1,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,
      89,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    98,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   145,    -1,   147,    12,
     149,   150,    15,    16,    -1,    -1,   155,    -1,   157,    22,
     159,    -1,   161,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    68,    -1,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   145,    -1,   147,    12,   149,   150,    15,    16,
      -1,    -1,   155,    -1,   157,    22,   159,    -1,   161,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    68,    -1,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    86,
      87,    88,    89,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    98,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   145,    -1,
     147,    12,   149,   150,    15,    16,    -1,    -1,   155,    -1,
     157,    22,   159,    -1,   161,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    98,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   145,    -1,   147,    12,   149,   150,
      15,    16,    -1,    -1,   155,    -1,   157,    22,   159,    -1,
     161,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    68,    -1,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     145,    -1,   147,    12,   149,   150,    15,    16,    -1,    -1,
     155,    -1,   157,    22,   159,    -1,   161,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,
      -1,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,
      89,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    98,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   145,    -1,   147,    12,
     149,   150,    15,    16,    -1,    -1,   155,    -1,   157,    22,
     159,    -1,   161,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    68,    -1,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   145,    -1,   147,    12,   149,   150,    15,    16,
      -1,    -1,   155,    -1,   157,    22,   159,    -1,   161,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    68,    -1,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    86,
      87,    88,    89,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    98,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   145,    -1,
     147,    12,   149,   150,    15,    16,    -1,    -1,   155,    -1,
      -1,    22,   159,    -1,   161,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    98,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   145,    -1,   147,    12,    -1,   150,
      15,    16,    -1,    -1,   155,    -1,   157,    22,   159,    -1,
     161,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    68,    -1,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    -1,   132,    -1,    -1,
      -1,   136,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,   147,    -1,    15,    16,   151,    -1,    -1,    -1,
      -1,    22,   157,    -1,   159,   160,   161,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    52,    -1,    54,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    84,    85,    86,    87,    88,    89,    -1,
      91,    -1,    93,    94,    -1,    -1,    -1,    98,    -1,    -1,
     101,    -1,    -1,    -1,   105,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,     3,     4,
       5,     6,     7,     8,    -1,   142,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,   147,    22,    -1,   156,
      -1,   152,   159,    -1,   161,    30,   157,    -1,   159,    -1,
     161,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    68,    -1,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,    89,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    98,    -1,    -1,   101,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,   156,   147,    22,   159,    -1,   161,    -1,    -1,    -1,
      -1,    30,   157,    -1,   159,    -1,   161,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68,
      -1,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    86,    87,    88,
      89,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    98,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,   147,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,   157,    -1,
     159,    -1,   161,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    68,    -1,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    89,    -1,    91,     3,
      -1,    94,    -1,    -1,    -1,    98,    10,    -1,   101,    -1,
      -1,    15,    16,    17,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,   128,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      54,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    66,    -1,    -1,    -1,   159,    17,   161,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      84,    85,    -1,    87,    -1,    89,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    98,    -1,    10,   101,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,   116,   117,    28,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,   129,   130,    77,    -1,   133,
      -1,    81,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,   150,    -1,    -1,   153,
     154,   155,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    77,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,   111,   148,   149,
     150,    -1,    -1,   153,   154,   155,    -1,    -1,    -1,    -1,
      28,    -1,   162,   127,    -1,   129,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,   140,   141,    -1,   143,
     144,   145,   146,    -1,   148,   149,   150,    10,    -1,   153,
     154,   155,    -1,    -1,    17,    -1,    -1,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    81,   127,
      -1,   129,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,   140,    -1,    10,   143,   144,   145,   146,    -1,
     148,    17,   150,    -1,    -1,   153,   154,   155,   111,    -1,
      77,    -1,    28,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,   140,   141,    -1,
     143,   144,   145,   146,   111,   148,   149,   150,    -1,    -1,
     153,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     127,    77,   129,   130,   131,    81,   133,   134,   135,    -1,
     137,   138,   139,   140,   141,    -1,   143,   144,   145,   146,
      -1,   148,   149,   150,    10,    -1,   153,   154,   155,    -1,
      -1,    17,    -1,   160,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,    -1,   148,   149,   150,    -1,    -1,   153,   154,   155,
      -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    10,    81,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,    77,   148,   149,   150,    81,    -1,   153,   154,   155,
      -1,   157,    -1,    10,    90,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,   111,   112,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,    -1,   148,   149,   150,    -1,    -1,   153,   154,   155,
      77,   157,    -1,    10,    81,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,   140,   141,    -1,   143,   144,   145,   146,
      77,   148,   149,   150,    81,    -1,   153,   154,   155,    -1,
     157,    -1,    10,    90,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,   111,   112,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,   140,   141,    -1,   143,   144,   145,   146,
      -1,   148,   149,   150,    -1,    -1,   153,   154,   155,    77,
     157,    -1,    10,    81,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,   123,    -1,   125,   126,   127,
      -1,   129,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,    -1,   143,   144,   145,   146,    77,
     148,   149,   150,    81,    -1,   153,   154,   155,    -1,   157,
      -1,    10,    90,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,   111,   112,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,   123,    -1,   125,   126,   127,
      -1,   129,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,    -1,   143,   144,   145,   146,    -1,
     148,   149,   150,    -1,    -1,   153,   154,   155,    77,   157,
      -1,    10,    81,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,   140,   141,    -1,   143,   144,   145,   146,    77,   148,
     149,   150,    81,    -1,   153,   154,   155,    -1,   157,    -1,
      10,    90,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,   111,   112,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,   140,   141,    -1,   143,   144,   145,   146,    -1,   148,
     149,   150,    -1,    -1,   153,   154,   155,    77,   157,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,   111,   112,   113,   114,   115,   116,   117,    28,    29,
     120,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,    42,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,    -1,   148,   149,
     150,    10,    -1,   153,   154,   155,    -1,   157,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    28,
      29,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,   107,    17,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,   127,    77,   129,
     130,   131,    81,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,    -1,   148,   149,
     150,    -1,    10,   153,   154,   155,    -1,   157,    -1,    17,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      28,    29,    81,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,   140,   141,    -1,   143,   144,   145,   146,   107,   148,
     149,   150,   111,    -1,   153,   154,   155,    -1,   157,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    77,
     129,   130,   131,    81,   133,   134,   135,    -1,   137,   138,
     139,   140,   141,    -1,   143,   144,   145,   146,    -1,   148,
     149,   150,    10,    -1,   153,   154,   155,    -1,   157,    17,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,    -1,   143,   144,   145,   146,    -1,
     148,   149,   150,    -1,    -1,   153,   154,   155,    -1,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    10,    81,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,    -1,   143,   144,   145,   146,    77,
     148,   149,   150,    81,    -1,   153,   154,   155,    -1,   157,
      -1,    10,    90,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,    -1,   143,   144,   145,   146,    -1,
     148,   149,   150,    -1,    -1,   153,   154,   155,    77,   157,
      -1,    10,    81,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,   130,   131,    62,   133,   134,   135,    -1,   137,   138,
     139,   140,   141,    -1,   143,   144,   145,   146,    77,   148,
     149,   150,    81,    -1,   153,   154,   155,    -1,   157,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,   140,   141,    -1,   143,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,    77,    -1,    -1,
      10,    81,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
     130,   131,    62,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,    77,   148,   149,
     150,    81,    -1,   153,   154,   155,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,    -1,   148,   149,
     150,    -1,    -1,   153,   154,   155,    77,    -1,    -1,    10,
      81,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,    77,   148,   149,   150,
      81,    -1,   153,   154,   155,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,    -1,   148,   149,   150,
      -1,    -1,   153,   154,   155,    77,    -1,    -1,    10,    81,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,   130,   131,
      -1,   133,   134,   135,    -1,   137,   138,   139,   140,   141,
      -1,   143,   144,   145,   146,    77,   148,   149,   150,    81,
     152,   153,   154,   155,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,   130,   131,
      -1,   133,   134,   135,    -1,   137,   138,   139,   140,   141,
      -1,   143,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,    77,    -1,    -1,    10,    81,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,   140,   141,    -1,
     143,   144,   145,   146,    77,   148,   149,   150,    81,   152,
     153,   154,   155,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,   140,   141,    -1,
     143,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,    77,    -1,    -1,    10,    81,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,   140,   141,    -1,   143,
     144,   145,   146,    77,   148,   149,   150,    81,    -1,   153,
     154,   155,    -1,    -1,    -1,    10,    90,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,   140,   141,    -1,   143,
     144,   145,   146,    -1,   148,   149,   150,    -1,    -1,   153,
     154,   155,    77,    -1,    -1,    10,    81,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    77,   148,   149,   150,    81,    -1,   153,   154,
     155,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    28,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,    -1,   148,   149,   150,    -1,    -1,   153,   154,
     155,    -1,    77,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,    -1,   143,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,    77,    -1,    -1,    10,    81,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,    77,   148,   149,   150,    81,    -1,   153,   154,   155,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,   129,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
     146,    -1,   148,   149,   150,    -1,    -1,   153,   154,   155,
      77,    -1,    -1,    10,    81,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,   140,   141,    -1,   143,   144,   145,   146,
      77,   148,   149,   150,    81,    -1,   153,   154,   155,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,   140,   141,    -1,   143,   144,   145,   146,
      -1,   148,   149,   150,    -1,    -1,   153,   154,   155,    77,
      -1,    -1,    10,    81,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,    -1,   143,   144,   145,   146,    77,
     148,   149,   150,    81,    -1,   153,   154,   155,    -1,    -1,
      -1,    10,    90,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,    -1,   143,   144,   145,   146,    -1,
     148,   149,   150,    -1,    -1,   153,   154,   155,    77,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    10,    11,    -1,    -1,
      -1,    90,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,   140,   141,    -1,   143,   144,   145,   146,    -1,   148,
     149,   150,    -1,    77,   153,   154,   155,    81,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,   140,   141,    -1,   143,
     144,   145,   146,    -1,   148,   149,   150,    -1,    77,   153,
     154,   155,    81,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    90,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,   140,   141,    -1,   143,   144,   145,   146,    -1,   148,
     149,   150,    -1,    77,   153,   154,   155,    81,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    90,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,   140,   141,    -1,   143,
     144,   145,   146,    -1,   148,   149,   150,    -1,    77,   153,
     154,   155,    81,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    90,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,   140,   141,    -1,   143,   144,   145,   146,    -1,   148,
     149,   150,    -1,    77,   153,   154,   155,    81,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    90,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,   140,   141,    -1,   143,
     144,   145,   146,    -1,   148,   149,   150,    -1,    77,   153,
     154,   155,    81,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    90,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,   140,   141,    -1,   143,   144,   145,   146,    -1,   148,
     149,   150,    -1,    77,   153,   154,   155,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,   130,   131,    -1,   133,
     134,   135,    52,   137,   138,   139,   140,   141,    -1,   143,
     144,   145,   146,    -1,   148,   149,   150,    -1,    -1,   153,
     154,   155,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      10,    81,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,    77,   148,   149,
     150,    81,    -1,   153,   154,   155,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,    -1,   143,   144,   145,   146,    -1,   148,   149,
     150,    -1,    -1,   153,   154,   155,    77,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,    19,   148,   149,   150,
      23,    24,   153,   154,   155,    -1,    -1,    -1,    -1,    32,
      19,    34,    35,    -1,    23,    24,    39,    -1,    -1,    -1,
      -1,    44,    -1,    32,    -1,    34,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    67,    -1,    69,    -1,    71,    -1,
      -1,    74,    -1,    76,    -1,    78,    65,    -1,    67,    -1,
      69,    -1,    71,    -1,    -1,    74,    -1,    76,    -1,    78,
       3,    -1,    -1,    -1,    97,    -1,    -1,   100,    -1,    -1,
     103,    -1,    15,    16,    -1,    -1,    -1,    -1,    97,    22,
      -1,   100,    -1,    -1,   103,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    66,    -1,   158,    -1,    -1,    -1,    -1,
      -1,    -1,    75,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    -1,    87,    -1,    89,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    98,    -1,    -1,   101
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   167,   168,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    29,    30,    32,    34,    35,
      36,    37,    38,    39,    40,    41,    43,    44,    46,    49,
      50,    51,    53,    54,    55,    57,    58,    61,    65,    66,
      67,    68,    69,    70,    71,    74,    75,    76,    78,    79,
      80,    82,    83,    84,    85,    86,    87,    88,    89,    91,
      92,    94,    95,    96,    97,    98,   100,   101,   102,   103,
     106,   108,   111,   127,   128,   129,   130,   135,   145,   147,
     149,   150,   152,   155,   157,   159,   161,   169,   170,   171,
     172,   173,   174,   176,   181,   182,   183,   187,   189,   193,
     194,   195,   196,   197,   198,   200,   201,   202,   206,   207,
     210,   211,   214,   215,   218,   221,   222,   248,   251,   252,
     272,   273,   274,   275,   276,   277,   285,   286,   287,   288,
     291,   292,   293,   294,   295,   296,   297,   299,   300,   301,
     302,   303,   152,   171,   288,   107,   278,   279,     3,   184,
      12,    37,    38,    40,    89,   157,   161,   218,   272,   276,
     286,   287,   288,   291,   293,   294,   278,   288,    97,   251,
      78,   184,   171,   266,   288,   171,   159,   288,   289,     7,
       9,    97,   191,   289,    67,   109,   199,   288,   199,   288,
      23,    35,   252,   288,   288,   159,     3,    15,    16,    22,
      30,    36,    41,    43,    46,    54,    66,    75,    84,    85,
      87,    89,    91,    94,    98,   101,   186,   188,   219,   159,
     186,   253,   254,    29,   176,   192,   288,   288,   288,   288,
     288,   288,     7,   159,    35,   195,   195,   151,   241,   265,
     288,    95,    96,   152,   288,   290,   288,   192,   288,   288,
     288,    88,   159,   171,   288,   288,   176,   183,   288,   291,
     176,   183,   288,   186,   249,   288,   265,   288,   288,   288,
     288,   288,   288,   288,   288,     1,   158,   169,   177,   265,
      99,   136,   241,   267,   268,   290,   199,   265,   288,   298,
      69,   171,   157,    73,   175,   186,   186,    47,    72,   236,
     253,   152,   142,    10,    17,    28,    77,    81,   111,   127,
     129,   130,   131,   133,   134,   135,   137,   138,   139,   140,
     141,   143,   144,   145,   146,   148,   149,   150,   153,   154,
     155,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   156,   228,   159,   161,
      77,    81,   159,   171,   152,   288,   288,   288,   265,   157,
     176,    42,   278,   249,   253,   152,   132,   152,   106,   187,
     241,   269,   270,   271,   290,   249,   211,   251,   186,   159,
     192,    29,    42,   192,   107,   192,   281,    29,    42,   192,
     281,   289,    33,    62,   152,    90,   176,   228,   269,   171,
     159,   230,    99,   159,   186,   255,   256,     1,   131,   260,
      42,   132,   171,   192,   192,   269,   186,   132,   152,   288,
     288,   152,   157,   192,   159,   269,   152,   203,   152,   203,
     152,   112,   250,   152,   152,    33,    62,   192,   152,   158,
     158,   169,   132,   158,   288,   132,   160,   132,   160,   162,
     281,    42,   132,   162,   281,   110,   132,   162,     7,     1,
     158,   177,    56,   131,   212,   157,   223,   152,   288,   288,
     288,   288,   199,   288,   199,   288,   288,   288,   288,   288,
      16,   101,   288,   288,    30,    54,    97,   187,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   290,   290,   290,   290,   290,   269,
     269,   199,   288,   199,   288,    24,    42,    78,   103,   280,
     283,   284,   288,   303,    29,    42,    29,    42,    90,    42,
     162,   199,   288,   192,   152,   152,   288,   288,   112,   160,
     132,   152,   186,   253,    42,   265,    40,   288,   199,   288,
     159,   192,    40,   288,   199,   288,   192,   155,   178,   180,
     288,   178,   179,   171,   288,    31,   288,   160,    24,    42,
      45,    64,    67,    78,    97,   170,   231,   232,   233,   234,
     220,   256,   132,   160,    16,    30,    43,    66,    84,    85,
      89,   101,   161,   190,   261,   273,   112,   257,   288,   254,
     160,   241,   288,     1,   208,   269,   160,    18,   204,   261,
     273,   132,   180,   179,   158,   160,   160,   267,   160,   267,
     171,   162,   199,   288,   162,   171,   288,   162,   288,   162,
     288,   158,   158,   186,   265,   157,     1,   186,   216,   217,
      24,    67,    78,    97,   225,   235,   152,   152,   152,   152,
     152,   160,   162,    42,    78,   132,   160,   272,    77,    77,
      40,   199,   288,    40,   199,   288,   199,   288,   278,   278,
     152,   241,   290,   271,   212,   152,   288,   160,   288,    29,
     192,    29,   192,   282,   283,   288,    29,   192,   281,    29,
     192,   281,   132,   152,    11,   152,    31,    31,   171,    90,
     176,    42,    78,   233,   132,   160,   159,   186,    24,    67,
      78,    97,   237,   160,   256,   260,     1,   265,    59,   290,
     158,    63,   104,   158,   209,   160,   159,   176,   186,   205,
     249,   152,   152,   171,   162,   281,   162,   281,   171,   110,
       1,   213,   158,   112,   132,   158,    78,   224,     1,     3,
      10,    15,    17,    22,    36,    41,    46,    48,    54,    75,
      87,    91,    94,    98,   127,   128,   129,   130,   133,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   150,   153,   154,   155,   156,   159,   185,   188,   226,
     227,   228,   272,   283,   260,   272,   272,   288,    29,    29,
     288,    29,    29,   162,   162,   192,   192,   157,   192,    90,
      40,   288,    40,   288,   132,   160,    90,    40,   288,   192,
      40,   288,   192,   288,   288,   171,   288,   171,    31,   186,
     232,   256,   131,   264,    78,   260,   257,   162,    42,   162,
      29,   171,   265,   205,   131,   176,    40,   171,   288,   162,
      40,   171,   288,   162,   288,   158,    35,    71,    74,   158,
     170,   194,   288,   217,   237,   159,   229,   186,   288,   134,
     229,   229,   257,    90,    40,    40,    90,    40,    40,    40,
      40,     1,   213,   288,   288,   288,   283,   288,   288,   288,
      11,    31,   171,   264,   160,   161,   190,   241,   263,   273,
     136,   243,   257,    52,   105,   244,   288,   261,   273,   171,
     192,   160,   288,   288,   171,   288,   171,   194,     1,   131,
     259,   231,   160,     3,    91,   227,   228,   288,   288,   288,
     288,   288,   288,   158,   158,    90,    90,    90,    90,   288,
     171,   257,   264,   162,   265,   241,   242,   288,     3,    80,
      91,   245,   246,   247,   288,   176,   193,   240,   162,   176,
      90,    90,    60,   161,   190,   258,   273,    93,   238,   160,
     229,   229,    90,    90,    90,    90,    90,    90,   288,   288,
     288,   288,   243,   257,   241,   262,   263,   273,    42,   162,
     247,   105,   132,   112,   137,   139,   140,   143,   144,    52,
     273,   288,   288,     1,   162,   265,   244,   288,   262,   263,
     288,   246,   247,   247,   247,   247,   247,   247,   245,   162,
     258,   273,   162,   152,   239,   240,   162,   258,   273,   262
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   166,   167,   168,   168,   169,   169,   170,   170,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   172,
     173,   173,   173,   174,   174,   174,   175,   175,   176,   176,
     176,   177,   177,   178,   178,   178,   178,   179,   179,   180,
     180,   181,   181,   181,   182,   183,   183,   183,   183,   183,
     184,   184,   185,   185,   185,   185,   185,   186,   186,   186,
     186,   186,   187,   187,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     189,   189,   189,   189,   189,   189,   189,   189,   190,   190,
     190,   190,   190,   190,   190,   190,   191,   191,   192,   192,
     193,   193,   194,   194,   194,   194,   194,   194,   194,   194,
     195,   195,   196,   196,   196,   196,   197,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   199,   200,   200,   200,   200,   200,   200,
     200,   200,   201,   202,   202,   202,   202,   202,   202,   203,
     203,   204,   204,   204,   205,   205,   206,   207,   207,   208,
     208,   209,   209,   209,   210,   210,   210,   210,   211,   211,
     211,   212,   212,   213,   213,   213,   214,   214,   215,   216,
     216,   216,   217,   217,   219,   220,   218,   221,   221,   221,
     221,   221,   223,   224,   222,   225,   225,   225,   225,   225,
     226,   226,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   229,   229,   230,   231,   231,   231,   232,   232,   232,
     232,   232,   233,   233,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   235,   235,   235,   235,   235,   235,   236,
     236,   237,   237,   237,   237,   237,   237,   238,   238,   239,
     239,   240,   240,   241,   241,   242,   242,   243,   243,   244,
     244,   244,   244,   244,   245,   245,   246,   246,   246,   246,
     246,   246,   246,   247,   247,   248,   248,   248,   249,   249,
     250,   250,   250,   251,   251,   251,   251,   251,   252,   252,
     252,   253,   253,   254,   254,   255,   255,   255,   256,   256,
     256,   257,   257,   257,   258,   258,   258,   258,   258,   258,
     258,   259,   259,   259,   259,   259,   259,   260,   260,   260,
     260,   260,   261,   261,   261,   261,   262,   262,   262,   263,
     263,   263,   263,   263,   264,   264,   264,   264,   264,   265,
     265,   265,   265,   266,   266,   267,   267,   267,   268,   268,
     269,   269,   270,   270,   271,   271,   271,   271,   272,   272,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   275,   276,   277,   277,   277,   277,   277,
     277,   277,   277,   278,   278,   279,   280,   280,   281,   282,
     282,   283,   283,   283,   284,   284,   284,   284,   284,   284,
     285,   285,   286,   287,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     289,   289,   290,   290,   290,   291,   291,   291,   291,   292,
     292,   293,   293,   293,   294,   294,   294,   294,   295,   295,
     295,   295,   296,   296,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   298,   298,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   300,   300,   300,   300,   300,   300,   301,   301,   301,
     301,   302,   302,   302,   302,   303,   303,   303,   303,   303,
     303,   303
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
       6,     3,     4,     3,     4,     6,     7,     6,     7,     4,
       5,     4,     5,     4,     4,     3,     6,     5,     6,     5,
       8,     7,     2,     3,     3,     2,     2,     3,     3,     0,
       2,     2,     3,     5,     1,     3,     3,     5,     5,     0,
       2,     3,     2,     3,     6,     8,     6,     8,     1,     1,
       1,     0,     2,     0,     2,     3,     5,     5,     1,     1,
       2,     3,     1,     3,     0,     0,     8,     0,     1,     2,
       2,     1,     0,     0,    10,     3,     3,     5,     5,     3,
       1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     0,     1,     3,     4,     5,     4,
       6,     6,     0,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     0,     1,     2,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     2,     1,     0,     1,     2,     2,     0,
       2,     2,     4,     4,     1,     3,     3,     3,     3,     3,
       3,     3,     2,     1,     1,     3,     4,     4,     2,     4,
       0,     2,     2,     1,     1,     1,     2,     1,     4,     5,
       3,     1,     3,     3,     5,     1,     1,     3,     1,     2,
       3,     0,     2,     2,     3,     2,     4,     3,     3,     4,
       3,     0,     2,     2,     2,     2,     1,     0,     2,     2,
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
  unsigned long yylno = yyrline[yyrule];
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
    default: /* Avoid compiler warnings. */
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
  if (yyps)
    {
#ifndef yyoverflow
      /* If the stack was reallocated but the parse did not complete, then the
         stack still needs to be freed.  */
      if (!yyps->yynew && yyps->yyss != yyps->yyssa)
        YYSTACK_FREE (yyps->yyss);
#endif
      free (yyps);
    }
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
                  (unsigned long) yystacksize));

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 487 "chapel.ypp" /* yacc.c:1666  */
    { yyblock = (yyval.pblockstmt); }
#line 5675 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 3:
#line 492 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5681 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 4:
#line 493 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5687 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 6:
#line 500 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5693 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 7:
#line 505 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5699 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 8:
#line 506 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5705 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 23:
#line 525 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5711 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 24:
#line 526 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5717 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 25:
#line 527 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5723 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 26:
#line 528 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5729 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 27:
#line 529 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5735 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 28:
#line 530 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5741 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 29:
#line 531 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5747 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 30:
#line 532 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5753 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 31:
#line 533 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5759 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 32:
#line 534 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5765 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 33:
#line 535 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5771 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 34:
#line 536 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5777 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 35:
#line 537 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5783 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 36:
#line 538 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5789 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 37:
#line 539 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5795 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 38:
#line 540 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5801 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 39:
#line 545 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 5815 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 40:
#line 558 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 5824 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 41:
#line 563 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 5833 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 42:
#line 568 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 5842 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 43:
#line 575 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5848 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 44:
#line 576 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5854 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 45:
#line 577 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5860 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 46:
#line 581 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; }
#line 5866 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 47:
#line 582 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true;  }
#line 5872 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 48:
#line 595 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5878 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 49:
#line 596 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5884 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 50:
#line 597 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5890 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 51:
#line 602 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5896 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 52:
#line 603 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5902 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 53:
#line 608 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5912 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 54:
#line 613 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5922 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 55:
#line 618 "chapel.ypp" /* yacc.c:1666  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5931 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 56:
#line 622 "chapel.ypp" /* yacc.c:1666  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5940 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 57:
#line 629 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5950 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 59:
#line 638 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5960 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 61:
#line 647 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5966 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 62:
#line 648 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5972 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 63:
#line 649 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5978 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 64:
#line 653 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5984 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 65:
#line 658 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 5990 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 66:
#line 660 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5996 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 67:
#line 662 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 6002 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 68:
#line 664 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 6008 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 69:
#line 666 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 6014 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 70:
#line 672 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = NULL; }
#line 6020 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 71:
#line 673 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6026 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 72:
#line 677 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6032 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 73:
#line 678 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "this"; }
#line 6038 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 74:
#line 679 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6044 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 75:
#line 680 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6050 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 76:
#line 681 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6056 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 77:
#line 684 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6062 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 78:
#line 685 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6068 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 79:
#line 686 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6074 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 80:
#line 687 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6080 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 81:
#line 688 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6086 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 82:
#line 700 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6092 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 83:
#line 701 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "this"; }
#line 6098 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 84:
#line 712 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "bool"; }
#line 6104 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 85:
#line 713 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "int"; }
#line 6110 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 86:
#line 714 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "uint"; }
#line 6116 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 87:
#line 715 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "real"; }
#line 6122 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 88:
#line 716 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "imag"; }
#line 6128 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 89:
#line 717 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "complex"; }
#line 6134 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 90:
#line 718 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "string"; }
#line 6140 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 91:
#line 719 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "sync"; }
#line 6146 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 92:
#line 720 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "single"; }
#line 6152 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 93:
#line 721 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "owned"; }
#line 6158 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 94:
#line 722 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "shared"; }
#line 6164 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 95:
#line 723 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "borrowed"; }
#line 6170 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 96:
#line 724 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "unmanaged"; }
#line 6176 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 97:
#line 725 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "domain"; }
#line 6182 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 98:
#line 726 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "index"; }
#line 6188 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 99:
#line 727 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "locale"; }
#line 6194 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 100:
#line 731 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6200 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 101:
#line 732 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6206 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 102:
#line 733 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6212 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 103:
#line 734 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6218 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 104:
#line 735 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6224 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 105:
#line 736 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6230 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 106:
#line 737 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6236 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 107:
#line 738 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6242 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 108:
#line 745 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_syncvar"; }
#line 6248 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 109:
#line 746 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_singlevar"; }
#line 6254 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 110:
#line 747 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_owned"; }
#line 6260 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 111:
#line 748 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_shared"; }
#line 6266 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 112:
#line 749 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_borrowed"; }
#line 6272 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 113:
#line 750 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_unmanaged"; }
#line 6278 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 114:
#line 751 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_domain"; }
#line 6284 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 115:
#line 752 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_index"; }
#line 6290 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 116:
#line 757 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = NULL; }
#line 6296 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 117:
#line 758 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6302 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 118:
#line 762 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6308 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 119:
#line 763 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6314 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 120:
#line 767 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6320 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 121:
#line 768 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6326 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 122:
#line 772 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6332 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 124:
#line 774 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6338 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 125:
#line 775 "chapel.ypp" /* yacc.c:1666  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6344 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 132:
#line 788 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6350 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 133:
#line 789 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6356 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 134:
#line 790 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6362 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 135:
#line 791 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6368 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 136:
#line 796 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6376 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 137:
#line 802 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6382 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 138:
#line 803 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6388 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 139:
#line 804 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 6394 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 140:
#line 805 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6400 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 141:
#line 806 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 6406 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 142:
#line 807 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6412 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 143:
#line 808 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6418 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 144:
#line 809 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6424 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 145:
#line 810 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6430 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 146:
#line 811 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6436 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 147:
#line 812 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6442 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 148:
#line 813 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6448 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 149:
#line 814 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6454 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 150:
#line 815 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6460 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 151:
#line 816 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6466 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 152:
#line 817 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6472 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 153:
#line 818 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6478 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 154:
#line 819 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6484 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 155:
#line 821 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6494 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 156:
#line 827 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6504 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 157:
#line 833 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6514 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 158:
#line 839 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6524 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 159:
#line 845 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6535 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 160:
#line 852 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6546 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 161:
#line 859 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6554 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 162:
#line 863 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6562 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 163:
#line 869 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 6568 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 164:
#line 873 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6574 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 165:
#line 874 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6580 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 166:
#line 875 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6586 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 167:
#line 876 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6592 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 168:
#line 877 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6599 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 169:
#line 879 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6606 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 170:
#line 881 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6613 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 171:
#line 883 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6620 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 172:
#line 888 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6626 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 173:
#line 891 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6632 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 174:
#line 892 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6638 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 175:
#line 893 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6644 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 176:
#line 894 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6650 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 177:
#line 895 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6656 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 178:
#line 896 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6662 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 179:
#line 900 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6668 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 180:
#line 901 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6674 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 181:
#line 905 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6680 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 182:
#line 906 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 6686 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 183:
#line 907 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 6692 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 184:
#line 911 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6698 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 185:
#line 912 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6704 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 186:
#line 916 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6710 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 187:
#line 920 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6716 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 188:
#line 922 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6722 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 189:
#line 926 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6728 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 190:
#line 927 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6734 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 191:
#line 932 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6740 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 192:
#line 934 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6746 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 193:
#line 936 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6752 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 194:
#line 943 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6766 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 195:
#line 953 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6780 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 196:
#line 963 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6794 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 197:
#line 973 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6808 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 198:
#line 985 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6818 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 199:
#line 990 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6828 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 200:
#line 995 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6838 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 201:
#line 1003 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = NULL; }
#line 6844 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 202:
#line 1004 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6850 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 203:
#line 1009 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6856 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 204:
#line 1011 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6862 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 205:
#line 1013 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6868 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 206:
#line 1018 "chapel.ypp" /* yacc.c:1666  */
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
#line 6888 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 207:
#line 1034 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6896 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 208:
#line 1041 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6906 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 209:
#line 1050 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6918 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 210:
#line 1058 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6926 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 211:
#line 1062 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6934 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 212:
#line 1068 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6940 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 213:
#line 1069 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6946 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 214:
#line 1074 "chapel.ypp" /* yacc.c:1666  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6955 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 215:
#line 1079 "chapel.ypp" /* yacc.c:1666  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6964 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 216:
#line 1084 "chapel.ypp" /* yacc.c:1666  */
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
      if ((yyvsp[-1].lifetimeAndWhere).where)
        (yyvsp[-5].pfnsymbol)->where = new BlockStmt((yyvsp[-1].lifetimeAndWhere).where);
      if ((yyvsp[-1].lifetimeAndWhere).lifetime)
        (yyvsp[-5].pfnsymbol)->lifetimeConstraints = new BlockStmt((yyvsp[-1].lifetimeAndWhere).lifetime);
      (yyvsp[-5].pfnsymbol)->insertAtTail((yyvsp[0].pblockstmt));
      (yyval.pexpr) = new DefExpr(buildLambda((yyvsp[-5].pfnsymbol)));
    }
#line 6986 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 217:
#line 1106 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6997 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 218:
#line 1112 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7009 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 219:
#line 1119 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7020 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 220:
#line 1125 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7031 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 221:
#line 1131 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7043 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 222:
#line 1142 "chapel.ypp" /* yacc.c:1666  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 7053 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 223:
#line 1148 "chapel.ypp" /* yacc.c:1666  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7064 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 224:
#line 1155 "chapel.ypp" /* yacc.c:1666  */
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

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[-6].pfnsymbol), (yyvsp[-4].retTag), (yyvsp[-3].pexpr), (yyvsp[-2].b), (yyvsp[-1].lifetimeAndWhere).where, (yyvsp[-1].lifetimeAndWhere).lifetime, (yyvsp[0].pblockstmt), (yylsp[-9]).comment);
      context->latestComment = NULL;
    }
#line 7091 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 225:
#line 1181 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 7099 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 226:
#line 1185 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7108 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 227:
#line 1190 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 7116 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 228:
#line 1194 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7125 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 229:
#line 1199 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 7133 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 231:
#line 1206 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7139 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 232:
#line 1210 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7145 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 233:
#line 1211 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 7151 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 234:
#line 1212 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "&"; }
#line 7157 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 235:
#line 1213 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "|"; }
#line 7163 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 236:
#line 1214 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "^"; }
#line 7169 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 237:
#line 1215 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "~"; }
#line 7175 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 238:
#line 1216 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "=="; }
#line 7181 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 239:
#line 1217 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "!="; }
#line 7187 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 240:
#line 1218 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<="; }
#line 7193 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 241:
#line 1219 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">="; }
#line 7199 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 242:
#line 1220 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<"; }
#line 7205 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 243:
#line 1221 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">"; }
#line 7211 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 244:
#line 1222 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "+"; }
#line 7217 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 245:
#line 1223 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "-"; }
#line 7223 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 246:
#line 1224 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "*"; }
#line 7229 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 247:
#line 1225 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "/"; }
#line 7235 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 248:
#line 1226 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<<"; }
#line 7241 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 249:
#line 1227 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">>"; }
#line 7247 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 250:
#line 1228 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "%"; }
#line 7253 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 251:
#line 1229 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "**"; }
#line 7259 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 252:
#line 1230 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "!"; }
#line 7265 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 253:
#line 1231 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "chpl_by"; }
#line 7271 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 254:
#line 1232 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "#"; }
#line 7277 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 255:
#line 1233 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "chpl_align"; }
#line 7283 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 256:
#line 1234 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<=>"; }
#line 7289 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 257:
#line 1235 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<~>"; }
#line 7295 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 258:
#line 1236 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "init="; }
#line 7301 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 259:
#line 1240 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "="; }
#line 7307 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 260:
#line 1241 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "+="; }
#line 7313 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 261:
#line 1242 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "-="; }
#line 7319 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 262:
#line 1243 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "*="; }
#line 7325 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 263:
#line 1244 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "/="; }
#line 7331 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 264:
#line 1245 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "%="; }
#line 7337 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 265:
#line 1246 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "**="; }
#line 7343 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 266:
#line 1247 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "&="; }
#line 7349 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 267:
#line 1248 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "|="; }
#line 7355 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 268:
#line 1249 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "^="; }
#line 7361 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 269:
#line 1250 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">>="; }
#line 7367 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 270:
#line 1251 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<<="; }
#line 7373 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 271:
#line 1255 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7379 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 272:
#line 1256 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7385 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 273:
#line 1260 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7391 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 274:
#line 1264 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7397 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 275:
#line 1265 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7403 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 276:
#line 1266 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7409 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 277:
#line 1271 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7415 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 278:
#line 1273 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7421 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 279:
#line 1275 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7427 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 280:
#line 1277 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7433 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 281:
#line 1279 "chapel.ypp" /* yacc.c:1666  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7439 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 282:
#line 1283 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7445 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 283:
#line 1284 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7451 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 284:
#line 1288 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_IN; }
#line 7457 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 285:
#line 1289 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7463 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 286:
#line 1290 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_OUT; }
#line 7469 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 287:
#line 1291 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST; }
#line 7475 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 288:
#line 1292 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7481 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 289:
#line 1293 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7487 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 290:
#line 1294 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7493 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 291:
#line 1295 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_REF; }
#line 7499 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 292:
#line 1296 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7505 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 293:
#line 1300 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7511 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 294:
#line 1301 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7517 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 295:
#line 1302 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_REF;   }
#line 7523 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 296:
#line 1303 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 7529 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 297:
#line 1304 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST;   }
#line 7535 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 298:
#line 1305 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 7541 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 299:
#line 1309 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 7547 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 300:
#line 1310 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 7553 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 301:
#line 1314 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_VALUE; }
#line 7559 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 302:
#line 1315 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_VALUE; }
#line 7565 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 303:
#line 1316 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 7571 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 304:
#line 1317 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_REF; }
#line 7577 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 305:
#line 1318 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_PARAM; }
#line 7583 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 306:
#line 1319 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_TYPE; }
#line 7589 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 307:
#line 1323 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; }
#line 7595 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 308:
#line 1324 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true;  }
#line 7601 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 309:
#line 1327 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = NULL; }
#line 7607 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 312:
#line 1333 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7613 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 313:
#line 1338 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7619 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 314:
#line 1340 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7625 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 315:
#line 1344 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7631 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 317:
#line 1349 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7637 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 318:
#line 1350 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7643 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 319:
#line 1354 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 7649 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 320:
#line 1356 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 7655 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 321:
#line 1358 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 7661 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 322:
#line 1360 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7667 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 323:
#line 1362 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7673 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 324:
#line 1366 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7679 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 325:
#line 1367 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7685 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 326:
#line 1370 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7691 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 327:
#line 1371 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7697 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 328:
#line 1372 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7703 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 329:
#line 1373 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7709 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 330:
#line 1374 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7715 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 331:
#line 1375 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7721 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 332:
#line 1376 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 7727 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 333:
#line 1379 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 7733 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 334:
#line 1380 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 7739 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 335:
#line 1384 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7745 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 336:
#line 1386 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7751 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 337:
#line 1388 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7757 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 338:
#line 1393 "chapel.ypp" /* yacc.c:1666  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7774 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 339:
#line 1406 "chapel.ypp" /* yacc.c:1666  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7792 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 340:
#line 1422 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7798 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 341:
#line 1424 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7804 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 342:
#line 1426 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7810 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 343:
#line 1430 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 7816 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 344:
#line 1431 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 7822 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 345:
#line 1432 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 7828 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 346:
#line 1433 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 7834 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 347:
#line 1434 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 7840 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 348:
#line 1439 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7850 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 349:
#line 1445 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
      context->latestComment = NULL;
    }
#line 7860 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 350:
#line 1451 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7869 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 352:
#line 1460 "chapel.ypp" /* yacc.c:1666  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7878 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 353:
#line 1468 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7884 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 354:
#line 1470 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7890 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 355:
#line 1475 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7896 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 356:
#line 1477 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7902 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 357:
#line 1479 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7908 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 358:
#line 1484 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7914 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 359:
#line 1486 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7920 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 360:
#line 1488 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7926 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 361:
#line 1494 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7932 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 362:
#line 1495 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7938 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 363:
#line 1496 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7944 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 364:
#line 1502 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7950 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 365:
#line 1504 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7956 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 366:
#line 1506 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7964 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 367:
#line 1510 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7972 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 368:
#line 1514 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7978 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 369:
#line 1516 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7986 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 370:
#line 1520 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7994 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 371:
#line 1527 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8000 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 372:
#line 1528 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8006 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 373:
#line 1529 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8012 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 374:
#line 1530 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8018 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 375:
#line 1531 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNothing); }
#line 8024 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 376:
#line 1532 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8030 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 377:
#line 1537 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8036 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 378:
#line 1538 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8042 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 379:
#line 1539 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 8048 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 380:
#line 1540 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8054 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 381:
#line 1541 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8060 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 382:
#line 1562 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8068 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 383:
#line 1566 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 8076 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 384:
#line 1570 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 8088 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 385:
#line 1578 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8096 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 386:
#line 1584 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8102 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 387:
#line 1585 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8108 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 388:
#line 1586 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8114 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 389:
#line 1591 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8120 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 390:
#line 1593 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8126 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 391:
#line 1599 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8132 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 392:
#line 1601 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8138 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 393:
#line 1603 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 8144 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 394:
#line 1607 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8150 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 395:
#line 1608 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8156 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 396:
#line 1609 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8162 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 397:
#line 1610 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8168 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 398:
#line 1611 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8174 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 399:
#line 1617 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8180 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 400:
#line 1618 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8186 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 401:
#line 1619 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8192 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 402:
#line 1620 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8198 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 403:
#line 1624 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 8204 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 404:
#line 1625 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8210 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 405:
#line 1629 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 8216 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 406:
#line 1630 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8222 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 407:
#line 1631 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8228 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 408:
#line 1635 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8234 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 409:
#line 1636 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8240 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 410:
#line 1640 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 8246 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 412:
#line 1645 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8252 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 413:
#line 1646 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8258 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 414:
#line 1650 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 8264 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 415:
#line 1651 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8270 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 416:
#line 1652 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8276 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 417:
#line 1653 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8282 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 418:
#line 1657 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8288 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 419:
#line 1658 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8294 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 425:
#line 1675 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 8300 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 426:
#line 1677 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 8306 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 427:
#line 1679 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 8312 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 428:
#line 1681 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 8318 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 429:
#line 1683 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 8324 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 430:
#line 1685 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 8330 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 431:
#line 1687 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 8336 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 432:
#line 1689 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 8342 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 433:
#line 1691 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8348 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 434:
#line 1693 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 8354 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 435:
#line 1695 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8360 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 436:
#line 1700 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8366 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 437:
#line 1702 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8372 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 438:
#line 1704 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8378 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 439:
#line 1706 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8384 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 440:
#line 1708 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8390 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 441:
#line 1710 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8396 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 442:
#line 1712 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8402 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 443:
#line 1714 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8408 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 444:
#line 1716 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8414 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 445:
#line 1718 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8420 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 446:
#line 1720 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8426 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 447:
#line 1722 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8432 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 448:
#line 1724 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8443 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 449:
#line 1731 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8453 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 450:
#line 1737 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8463 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 451:
#line 1743 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8473 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 452:
#line 1749 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 8483 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 453:
#line 1758 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8489 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 454:
#line 1767 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 8495 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 462:
#line 1783 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8501 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 463:
#line 1787 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = NULL; }
#line 8507 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 465:
#line 1792 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8513 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 466:
#line 1796 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8519 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 467:
#line 1797 "chapel.ypp" /* yacc.c:1666  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8525 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 468:
#line 1801 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8531 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 469:
#line 1805 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8537 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 470:
#line 1806 "chapel.ypp" /* yacc.c:1666  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8543 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 471:
#line 1811 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 8551 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 472:
#line 1815 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8559 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 473:
#line 1819 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8567 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 474:
#line 1825 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 8573 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 475:
#line 1826 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8579 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 476:
#line 1827 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8585 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 477:
#line 1828 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8591 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 478:
#line 1829 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8597 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 479:
#line 1830 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8603 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 481:
#line 1836 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8609 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 482:
#line 1841 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8615 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 483:
#line 1846 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8621 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 492:
#line 1862 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8627 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 493:
#line 1864 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8633 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 494:
#line 1866 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 8639 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 495:
#line 1868 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8645 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 496:
#line 1870 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8651 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 497:
#line 1872 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8657 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 498:
#line 1874 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8663 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 499:
#line 1876 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8669 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 500:
#line 1880 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8675 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 501:
#line 1881 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8681 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 502:
#line 1884 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8687 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 503:
#line 1885 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8693 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 504:
#line 1886 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8699 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 511:
#line 1908 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8705 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 512:
#line 1909 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8711 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 513:
#line 1910 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8717 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 514:
#line 1914 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8723 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 515:
#line 1915 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8729 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 516:
#line 1916 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8735 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 517:
#line 1917 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 8741 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 518:
#line 1925 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8747 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 519:
#line 1926 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8753 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 520:
#line 1927 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8759 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 521:
#line 1928 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8765 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 522:
#line 1932 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 8771 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 523:
#line 1933 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 8777 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 525:
#line 1938 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 8783 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 526:
#line 1939 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8789 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 527:
#line 1940 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8795 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 528:
#line 1941 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8801 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 529:
#line 1942 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8807 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 530:
#line 1943 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8813 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 531:
#line 1944 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8819 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 532:
#line 1945 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8825 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 533:
#line 1946 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8831 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 534:
#line 1948 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8839 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 535:
#line 1952 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8847 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 536:
#line 1959 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8853 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 537:
#line 1960 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8859 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 538:
#line 1964 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8865 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 539:
#line 1965 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8871 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 540:
#line 1966 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8877 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 541:
#line 1967 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8883 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 542:
#line 1968 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8889 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 543:
#line 1969 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8895 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 544:
#line 1970 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8901 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 545:
#line 1971 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8907 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 546:
#line 1972 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8913 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 547:
#line 1973 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8919 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 548:
#line 1974 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8925 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 549:
#line 1975 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8931 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 550:
#line 1976 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8937 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 551:
#line 1977 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8943 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 552:
#line 1978 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8949 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 553:
#line 1979 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8955 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 554:
#line 1980 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8961 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 555:
#line 1981 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8967 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 556:
#line 1982 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8973 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 557:
#line 1983 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8979 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 558:
#line 1984 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8985 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 559:
#line 1985 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8991 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 560:
#line 1986 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8997 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 561:
#line 1990 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 9003 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 562:
#line 1991 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 9009 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 563:
#line 1992 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 9015 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 564:
#line 1993 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 9021 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 565:
#line 1994 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 9027 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 566:
#line 1995 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 9033 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 567:
#line 1999 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9039 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 568:
#line 2000 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9045 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 569:
#line 2001 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9051 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 570:
#line 2002 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9057 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 571:
#line 2006 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9063 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 572:
#line 2007 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9069 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 573:
#line 2008 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9075 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 574:
#line 2009 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9081 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 575:
#line 2014 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 9087 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 576:
#line 2015 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 9093 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 577:
#line 2016 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 9099 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 578:
#line 2017 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 9105 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 579:
#line 2018 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 9111 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 580:
#line 2019 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 9117 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 581:
#line 2020 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 9123 "bison-chapel.cpp" /* yacc.c:1666  */
    break;


#line 9127 "bison-chapel.cpp" /* yacc.c:1666  */
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
