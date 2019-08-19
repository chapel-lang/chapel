/* A Bison parser, made by GNU Bison 3.0.5.  */

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
#define YYBISON_VERSION "3.0.5"

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
    TBYTES = 273,
    TCATCH = 274,
    TCLASS = 275,
    TCOBEGIN = 276,
    TCOFORALL = 277,
    TCOMPLEX = 278,
    TCONFIG = 279,
    TCONST = 280,
    TCONTINUE = 281,
    TDEFER = 282,
    TDELETE = 283,
    TDMAPPED = 284,
    TDO = 285,
    TDOMAIN = 286,
    TELSE = 287,
    TENUM = 288,
    TEXCEPT = 289,
    TEXPORT = 290,
    TEXTERN = 291,
    TFALSE = 292,
    TFOR = 293,
    TFORALL = 294,
    TFORWARDING = 295,
    TIF = 296,
    TIMAG = 297,
    TIN = 298,
    TINDEX = 299,
    TINLINE = 300,
    TINOUT = 301,
    TINT = 302,
    TITER = 303,
    TINITEQUALS = 304,
    TLABEL = 305,
    TLAMBDA = 306,
    TLET = 307,
    TLIFETIME = 308,
    TLOCAL = 309,
    TLOCALE = 310,
    TMINUSMINUS = 311,
    TMODULE = 312,
    TNEW = 313,
    TNIL = 314,
    TNOINIT = 315,
    TNONE = 316,
    TNOTHING = 317,
    TON = 318,
    TONLY = 319,
    TOTHERWISE = 320,
    TOUT = 321,
    TOVERRIDE = 322,
    TOWNED = 323,
    TPARAM = 324,
    TPLUSPLUS = 325,
    TPRAGMA = 326,
    TPRIMITIVE = 327,
    TPRIVATE = 328,
    TPROC = 329,
    TPROTOTYPE = 330,
    TPUBLIC = 331,
    TREAL = 332,
    TRECORD = 333,
    TREDUCE = 334,
    TREF = 335,
    TREQUIRE = 336,
    TRETURN = 337,
    TSCAN = 338,
    TSELECT = 339,
    TSERIAL = 340,
    TSHARED = 341,
    TSINGLE = 342,
    TSPARSE = 343,
    TSTRING = 344,
    TSUBDOMAIN = 345,
    TSYNC = 346,
    TTHEN = 347,
    TTHIS = 348,
    TTHROW = 349,
    TTHROWS = 350,
    TTRUE = 351,
    TTRY = 352,
    TTRYBANG = 353,
    TTYPE = 354,
    TUINT = 355,
    TUNDERSCORE = 356,
    TUNION = 357,
    TUNMANAGED = 358,
    TUSE = 359,
    TVAR = 360,
    TVOID = 361,
    TWHEN = 362,
    TWHERE = 363,
    TWHILE = 364,
    TWITH = 365,
    TYIELD = 366,
    TZIP = 367,
    TALIAS = 368,
    TAND = 369,
    TASSIGN = 370,
    TASSIGNBAND = 371,
    TASSIGNBOR = 372,
    TASSIGNBXOR = 373,
    TASSIGNDIVIDE = 374,
    TASSIGNEXP = 375,
    TASSIGNLAND = 376,
    TASSIGNLOR = 377,
    TASSIGNMINUS = 378,
    TASSIGNMOD = 379,
    TASSIGNMULTIPLY = 380,
    TASSIGNPLUS = 381,
    TASSIGNREDUCE = 382,
    TASSIGNSL = 383,
    TASSIGNSR = 384,
    TBANG = 385,
    TBAND = 386,
    TBNOT = 387,
    TBOR = 388,
    TBXOR = 389,
    TCOLON = 390,
    TCOMMA = 391,
    TDIVIDE = 392,
    TDOT = 393,
    TDOTDOT = 394,
    TDOTDOTDOT = 395,
    TEQUAL = 396,
    TEXP = 397,
    TGREATER = 398,
    TGREATEREQUAL = 399,
    THASH = 400,
    TIO = 401,
    TLESS = 402,
    TLESSEQUAL = 403,
    TMINUS = 404,
    TMOD = 405,
    TNOTEQUAL = 406,
    TOR = 407,
    TPLUS = 408,
    TQUESTION = 409,
    TSEMI = 410,
    TSHIFTLEFT = 411,
    TSHIFTRIGHT = 412,
    TSTAR = 413,
    TSWAP = 414,
    TLCBR = 415,
    TRCBR = 416,
    TLP = 417,
    TRP = 418,
    TLSBR = 419,
    TRSBR = 420,
    TNOELSE = 421,
    TUPLUS = 422,
    TUMINUS = 423
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

#line 452 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 458 "bison-chapel.cpp" /* yacc.c:358  */
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

#line 532 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   17441

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  169
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  140
/* YYNRULES -- Number of rules.  */
#define YYNRULES  596
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1054

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   423

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
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   488,   488,   493,   494,   500,   501,   506,   507,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,   545,
     558,   563,   568,   576,   577,   578,   582,   583,   596,   597,
     598,   603,   604,   609,   614,   619,   623,   630,   635,   639,
     644,   648,   649,   650,   654,   655,   656,   660,   664,   666,
     668,   670,   672,   679,   680,   684,   685,   686,   687,   688,
     689,   692,   693,   694,   695,   696,   697,   709,   710,   721,
     722,   723,   724,   725,   726,   727,   728,   729,   730,   731,
     732,   733,   734,   735,   736,   737,   738,   739,   743,   744,
     745,   746,   747,   748,   749,   750,   751,   752,   753,   760,
     761,   766,   767,   772,   773,   777,   778,   782,   783,   787,
     788,   789,   790,   791,   792,   793,   794,   798,   799,   803,
     804,   805,   806,   810,   817,   818,   819,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,   830,   831,   832,
     833,   834,   835,   841,   847,   853,   859,   866,   873,   877,
     884,   888,   889,   890,   891,   892,   894,   896,   898,   903,
     906,   907,   908,   909,   910,   911,   915,   916,   920,   921,
     922,   926,   927,   931,   934,   936,   941,   942,   946,   948,
     950,   957,   967,   977,   987,  1000,  1005,  1010,  1018,  1019,
    1024,  1025,  1027,  1032,  1048,  1055,  1064,  1072,  1076,  1083,
    1084,  1089,  1094,  1088,  1121,  1127,  1134,  1140,  1146,  1157,
    1163,  1156,  1195,  1199,  1204,  1208,  1213,  1220,  1221,  1225,
    1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,
    1236,  1237,  1238,  1239,  1240,  1241,  1242,  1243,  1244,  1245,
    1246,  1247,  1248,  1249,  1250,  1251,  1252,  1256,  1257,  1258,
    1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1271,
    1272,  1276,  1280,  1281,  1282,  1286,  1288,  1290,  1292,  1294,
    1299,  1300,  1304,  1305,  1306,  1307,  1308,  1309,  1310,  1311,
    1312,  1316,  1317,  1318,  1319,  1320,  1321,  1325,  1326,  1330,
    1331,  1332,  1333,  1334,  1335,  1339,  1340,  1343,  1344,  1348,
    1349,  1353,  1355,  1360,  1361,  1365,  1366,  1370,  1371,  1373,
    1375,  1377,  1382,  1383,  1386,  1387,  1388,  1389,  1390,  1391,
    1392,  1395,  1396,  1399,  1401,  1403,  1408,  1421,  1438,  1439,
    1441,  1446,  1447,  1448,  1449,  1450,  1454,  1460,  1466,  1474,
    1475,  1483,  1485,  1490,  1492,  1494,  1499,  1501,  1503,  1510,
    1511,  1512,  1517,  1519,  1521,  1525,  1529,  1531,  1535,  1543,
    1544,  1545,  1546,  1547,  1552,  1553,  1554,  1555,  1556,  1576,
    1580,  1584,  1592,  1599,  1600,  1601,  1605,  1607,  1613,  1615,
    1617,  1622,  1623,  1624,  1625,  1626,  1632,  1633,  1634,  1635,
    1639,  1640,  1644,  1645,  1646,  1650,  1651,  1655,  1656,  1660,
    1661,  1665,  1666,  1667,  1668,  1672,  1673,  1684,  1686,  1691,
    1692,  1693,  1694,  1695,  1696,  1698,  1700,  1702,  1704,  1706,
    1708,  1711,  1713,  1715,  1717,  1719,  1721,  1723,  1725,  1730,
    1732,  1734,  1736,  1738,  1740,  1742,  1744,  1746,  1748,  1750,
    1752,  1754,  1761,  1767,  1773,  1779,  1788,  1798,  1806,  1807,
    1808,  1809,  1810,  1811,  1812,  1813,  1818,  1819,  1823,  1827,
    1828,  1832,  1836,  1837,  1841,  1845,  1849,  1856,  1857,  1858,
    1859,  1860,  1861,  1865,  1866,  1871,  1876,  1884,  1885,  1886,
    1887,  1888,  1889,  1890,  1891,  1892,  1894,  1896,  1898,  1900,
    1902,  1907,  1908,  1911,  1912,  1913,  1916,  1917,  1918,  1919,
    1930,  1931,  1932,  1936,  1937,  1938,  1942,  1943,  1944,  1945,
    1946,  1954,  1955,  1956,  1957,  1961,  1962,  1966,  1967,  1968,
    1969,  1970,  1971,  1972,  1973,  1974,  1975,  1976,  1977,  1981,
    1989,  1990,  1994,  1995,  1996,  1997,  1998,  1999,  2000,  2001,
    2002,  2003,  2004,  2005,  2006,  2007,  2008,  2009,  2010,  2011,
    2012,  2013,  2014,  2015,  2016,  2020,  2021,  2022,  2023,  2024,
    2025,  2026,  2030,  2031,  2032,  2033,  2037,  2038,  2039,  2040,
    2045,  2046,  2047,  2048,  2049,  2050,  2051
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
  "TBYTES", "TCATCH", "TCLASS", "TCOBEGIN", "TCOFORALL", "TCOMPLEX",
  "TCONFIG", "TCONST", "TCONTINUE", "TDEFER", "TDELETE", "TDMAPPED", "TDO",
  "TDOMAIN", "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE",
  "TFOR", "TFORALL", "TFORWARDING", "TIF", "TIMAG", "TIN", "TINDEX",
  "TINLINE", "TINOUT", "TINT", "TITER", "TINITEQUALS", "TLABEL", "TLAMBDA",
  "TLET", "TLIFETIME", "TLOCAL", "TLOCALE", "TMINUSMINUS", "TMODULE",
  "TNEW", "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY",
  "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS",
  "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC",
  "TREAL", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN",
  "TSELECT", "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSTRING",
  "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS", "TTRUE",
  "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE", "TUNION",
  "TUNMANAGED", "TUSE", "TVAR", "TVOID", "TWHEN", "TWHERE", "TWHILE",
  "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND",
  "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP",
  "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL",
  "TASSIGNSR", "TBANG", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON",
  "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP",
  "TGREATER", "TGREATEREQUAL", "THASH", "TIO", "TLESS", "TLESSEQUAL",
  "TMINUS", "TMOD", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP",
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TUPLUS", "TUMINUS", "$accept",
  "program", "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_start", "module_decl_stmt", "access_control",
  "opt_prototype", "block_stmt", "stmt_ls", "only_ls", "opt_only_ls",
  "except_ls", "use_access_control", "use_stmt", "require_stmt",
  "assignment_stmt", "opt_label_ident", "ident_fn_def", "ident_def",
  "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "opt_string", "do_stmt", "return_stmt",
  "class_level_stmt", "private_decl", "forwarding_stmt",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "defer_stmt", "try_stmt", "catch_stmt_ls", "catch_stmt", "catch_expr",
  "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
  "class_decl_stmt", "class_tag", "opt_inherit", "class_level_stmt_ls",
  "enum_decl_stmt", "enum_header", "enum_ls", "enum_item",
  "lambda_decl_expr", "$@1", "$@2", "linkage_spec", "fn_decl_stmt", "$@3",
  "$@4", "fn_decl_stmt_inner", "fn_decl_receiver_expr", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls",
  "formal", "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_or_iter", "opt_ret_tag", "opt_throws_error",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "opt_query_expr", "var_arg_expr", "opt_lifetime_where",
  "lifetime_components_expr", "lifetime_expr", "lifetime_ident",
  "type_alias_decl_stmt", "type_alias_decl_stmt_inner", "opt_init_type",
  "var_decl_type", "var_decl_stmt", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "ret_array_type",
  "opt_ret_type", "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "opt_formal_type", "expr_ls", "simple_expr_ls",
  "tuple_component", "tuple_expr_ls", "opt_actual_ls", "actual_ls",
  "actual_expr", "ident_expr", "type_level_expr", "sub_type_level_expr",
  "for_expr", "cond_expr", "nil_expr", "stmt_level_expr",
  "opt_task_intent_ls", "task_intent_clause", "task_intent_ls",
  "forall_intent_clause", "forall_intent_ls", "intent_expr",
  "shadow_var_prefix", "io_expr", "new_expr", "let_expr", "expr",
  "opt_expr", "opt_try_expr", "lhs_expr", "fun_expr", "call_expr",
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
     415,   416,   417,   418,   419,   420,   421,   422,   423
};
# endif

#define YYPACT_NINF -903

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-903)))

#define YYTABLE_NINF -547

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -903,    97,  2827,  -903,   -92,  -903,  -903,  -903,  -903,  -903,
    -903,  4288,     7,   185,  -903, 12784,  -903,  -903,     7,  9949,
    -903,   201,   141,   185,  4288,  9949,  4288,    73,  -903,  9949,
    6871,  -903,  8329,  8977,  6709,  9949,  -903,   127,  -903,  -903,
   14960,  -903, 13202,  9139,  -903,  9949,  9949,  -903,  -903,  -903,
    9949,  -903, 12784,  -903,  9949,   358,   224,  1305,  2115,  -903,
    -903,  -903,  9301,  7843,  9949,  9139, 12784,  9949,   310,  -903,
     241,  4288,  -903,  9949,  -903, 10111, 10111, 14960,  -903,  -903,
   12784,  -903,  -903,  9949,  9949,  -903,  9949,  -903,  9949,  -903,
    -903, 12379,  9949,  -903,  9949,  -903,  -903,  3316,  7033,  8491,
    -903,  4126,  -903,   251,  -903,   342,  -903,   308,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903, 14960,  -903, 14960,   317,    59,
    -903,  -903, 13202,  -903,   265,  -903,   281,  -903,  -903,   270,
     284,   322,   291,   327, 17060,  2390,   316,   352,   353,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,   155,  -903,  -903, 17060,
     301,  4288,  -903,  -903,   357,  9949,  9949,  9949,  9949,  9949,
    9301,  9301,   334,  -903,  -903,  -903,  -903,   351,   337,  -903,
    -903,   335, 15033, 14960, 13202,  -903,   359,  -903,    11, 17060,
     401,  8005, 17060,  -903,   159,  -903, 14960,   162,   271, 14960,
     360,    46, 14567,    10, 14535,   271,  9949,  -903, 14976, 13783,
    8005,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  4288,  -903,   363,  2556,    56,
     -13,  -903,  4288,  -903,  -903, 14711,   355,   351, 14711,   351,
     355,  -903,  8005,  9949,  -903,  -903, 14960,  -903,    31, 17060,
    9949,  9949,  -903, 17060,   362, 14944,  -903, 14711,   351, 17060,
     365,  8005,  -903, 17060, 15120,  -903,  -903, 15385,  2630,  -903,
    -903, 15434,   400,   364,   351, 14711, 15529,   475,   475,  1294,
     355,   355,   -60,  -903,  -903,  3478,   122,  -903,  9949,  -903,
     -51,    92,  -903,   -65,     3, 15578,   -46,   522,  -903,  3640,
    -903,   473,  9301,   403,   386,  -903,  -903,  -903,    84,  -903,
    -903,  9949,  9949,  9949,  9949,  8977,  8977,  9949,   338,  9949,
    9949,  9949,  9949,  9949,   485, 12379,  9949,  9949,  9949,  9949,
    9949,  9949,  9949,  9949,  9949,  9949,  9949,  9949,  9949,  9949,
    9949,  -903,  -903,  -903,  -903,  -903,  -903,  8653,  8653,  -903,
    -903,  -903,  -903,  8653,  -903,  -903,  8653,  8653,  8005,  8005,
    8977,  8977,  7519,  -903,  -903, 15177, 15209, 15610,    48,  3802,
    -903,  8977,    46,   393,   244,  -903,  9949,  -903,  9949,   437,
    -903,   392,   426,  -903,  -903,   412, 14960, 13202,   527,  9301,
    -903,  4450,  8977,  -903,   409,  -903,    46,  4612,  8977,  -903,
      46,   271, 10273,  9949,  -903,  4288,   540,  9949,   411,  -903,
     440,  -903,  -903,  2556,  -903,   439,   414,  -903, 10435,   465,
    9949, 13202,  -903,  -903,  -903,   418,  -903,  9301,  -903, 17060,
   17060,  -903,    28,  -903,  8005,   419,  -903,   567,  -903,   567,
    -903, 10597,   454,  -903,  -903,  -903,  -903,  -903,  -903,  8815,
    -903, 13639,  7195,  -903,  7357,  -903,  4288,   427,  8977,  6070,
    3154,   431,  9949,  6396,  -903,  -903,   138,  -903,  3964, 14960,
     273, 15353,  9301,   442, 17335,   344,  -903, 15754, 17177, 17177,
     349,  -903,   349,  -903,   349, 13449,  1442,  1370,  1411,   351,
     475,   452,  -903,  -903,  -903,  -903,  1294,  2586,   349,  1554,
    1554, 17177,  1554,  1554,  1988,   475,  2586,  1157,  1988,   355,
     355,   475,   464,   468,   471,   476,   477,   467,   463,  -903,
     349,  -903,   349,    18,  -903,  -903,  -903,   132,  -903,  1750,
   17136,   285, 10759,  8977, 10921,  8977,  9949,  8977, 13044,     7,
   15799,  -903,  -903,  -903, 17060, 15840,  8005,  -903,  8005,  -903,
     403,   274,  9949,   146,  9949, 17060,    52, 14768,  7519,  -903,
    9949, 17060,    54, 14624,  -903,   478,   498,   480, 15951,   498,
     484,   609, 15991,  4288, 14800,  -903,    61,  -903,  -903,  -903,
    -903,  -903,  -903,   558,   154,  -903, 13259,  -903,   367,   481,
    2556,    56,    73,   127,  9949,  9949,  5908,  -903,  -903,   507,
    8167,  -903, 17060,  -903,  -903,  -903, 17060,   486,    41,   482,
    -903, 13123,  -903,  -903,   282, 14960,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  4288,   -43, 13407,  -903,  -903, 17060,  4288,
   17060,  -903, 16032,  -903,  -903,  -903,  -903, 10273,  9949,   510,
    2211,   487,   534,   144,  -903,   572,  -903,  -903,  -903,  -903,
    6549,   491,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  7519,  -903,    37,  8977,  8977,  9949,   625, 16176,  9949,
     628, 16208,   494, 13495,    46,    46,  -903,  -903,  -903,  -903,
     501,  -903, 14711,  -903, 13859,  4774,  -903,  4936,  -903,   218,
    -903, 13935,  5098,  -903,    46,  5260,  -903,    46,  9949,  -903,
    9949,  -903,  4288,  9949,  -903,  4288,   637,  -903,  -903, 14960,
     776,  -903,  2556,   537,   593,  -903,  -903,  -903,   128,  -903,
    -903,   465,   512,    81,  -903,  -903,  -903,  5422,  9301,  -903,
    -903,  -903, 14960,  -903,   544,   335,  -903,  -903,  5584,   516,
    5746,   520,  -903,  9949,   535,   538,   514,  1037,  -903,  9949,
   14960,  -903,  -903,   367,   536,   -36,  -903,   551,  -903,   562,
     563,   575,   565,   569,  -903,   570,   576,   574,   582,   586,
     233,   596,   591,   592,  -903,  -903, 14960,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  9949,  -903,   602,   604,
     597,   536,   536,  -903,  -903,  -903,   465,   226,   230, 16352,
   11083, 11245, 16384, 11407, 11569, 11731, 11893,  -903,  -903,  2325,
    -903,  4288,  9949, 17060,  9949, 17060,  7519,  -903,  4288,  9949,
   17060,  -903,  9949, 17060,  -903, 16441, 17060,  -903, 17060,   704,
    4288,   537,  -903,   581,  9463,   208,  -903,    26,  -903,  -903,
    8977, 12914,  4288,  -903,    17,   590,  9949,  -903,  9949,  -903,
   17060,  4288,  9949,  -903, 17060,  4288, 17060,  -903,  -903,  -903,
    7681,   573,   573,  -903,  2030,  -903, 17060,  -903,    43,   440,
    -903,  -903, 13696,  -903, 13351,  -903,  -903,  -903,  9949,  9949,
    9949,  9949,  9949,  9949,  9949,  9949,   577,  1852, 15991, 14011,
   14087,  -903, 15991, 14163, 14239,  9949,  4288,  -903,   465,   537,
    6233,  -903,  -903,  -903,   112,  9301,  -903,  -903,   140,  9949,
     -34, 13552,  -903,   568,   401,  -903,   335, 17060, 14315,  -903,
   14391,  -903,  -903,  -903, 12055,   650,   231,  -903,   619,   627,
     536,   536, 16528, 16604, 16680, 16756, 16832, 16908,  -903,  -903,
    4288,  4288,  4288,  4288, 17060,  -903,  -903,   208,  9625,    88,
    -903,  -903, 17060,  -903,    68,  -903,    29,  -903,   130, 16984,
    -903,  -903,  -903, 11893,  -903,  4288,  4288,  2989,  -903,  -903,
     190,  -903,    26,  -903,  -903,  -903,  9949,  9949,  9949,  9949,
    9949,  9949, 15991, 15991, 15991, 15991,  -903,  -903,  -903,  -903,
    -903,   129,  8977, 12511,  -903,  9949,   140,    68,    68,    68,
      68,    68,    68,   140,   639, 15991, 15991,   594, 12217,    89,
      63, 13593,  -903,  -903, 17060,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,   624,  -903,  -903,   290, 12654,  -903,  -903,  -903,
    9787,  -903,   300,  -903
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    87,   538,   539,   540,   541,
     542,     0,   476,    73,   108,   447,   114,   205,   476,     0,
     113,     0,   352,    73,     0,     0,     0,     0,   215,   511,
     511,   535,     0,     0,     0,     0,   112,     0,   225,   109,
       0,   221,     0,     0,   116,     0,     0,   467,   543,   117,
       0,   228,   441,   351,     0,     0,     0,    45,    44,   111,
     206,   353,     0,     0,     0,     0,   445,     0,     0,   115,
       0,     0,    88,     0,   536,     0,     0,     0,   110,   207,
     443,   355,   118,     0,     0,   592,     0,   594,     0,   595,
     596,   510,     0,   593,   590,   129,   591,     0,     0,     0,
       4,     0,     5,     0,     9,    46,    10,     0,    11,    12,
      14,   425,   426,    22,    13,   130,   136,    15,    17,    16,
      19,    20,    21,    18,   135,     0,   133,     0,   502,     0,
     137,   134,     0,   138,   516,   498,   427,   499,   431,   429,
       0,     0,   503,   504,     0,   430,     0,   517,   518,   519,
     537,   497,   433,   432,   500,   501,     0,    38,    24,   439,
       0,     0,   477,    74,     0,     0,     0,     0,     0,     0,
       0,     0,   502,   516,   429,   503,   504,   448,   430,   517,
     518,     0,   476,     0,     0,   354,     0,   179,     0,   410,
       0,   417,   512,   226,   541,   143,     0,     0,   227,     0,
       0,     0,     0,     0,     0,     0,   511,   142,     0,     0,
     417,    81,    89,   101,    95,    94,   103,    84,    93,   104,
      90,   105,    82,   106,    99,    92,   100,    98,    96,    97,
      83,    85,    91,   102,   107,     0,    86,     0,     0,     0,
       0,   359,     0,   126,    32,     0,   577,   495,     0,   442,
     578,     7,   417,   511,   132,   131,   322,   407,     0,   406,
       0,     0,   127,   515,     0,     0,    35,     0,   446,   434,
       0,   417,    36,   440,     0,   186,   182,     0,   430,   186,
     183,     0,   348,     0,   444,     0,     0,   579,   581,   509,
     576,   575,     0,    48,    51,     0,     0,   412,     0,   414,
       0,     0,   413,     0,     0,   406,     0,     0,     6,     0,
      47,     0,     0,   208,     0,   308,   307,   229,     0,   428,
      23,     0,     0,     0,     0,     0,     0,     0,   580,     0,
       0,     0,     0,     0,     0,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   267,   274,   275,   276,   271,   273,     0,     0,   269,
     272,   270,   268,     0,   278,   277,     0,     0,   417,   417,
       0,     0,     0,    25,    26,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,    28,     0,    29,     0,   425,
     423,     0,   418,   419,   424,     0,     0,     0,     0,     0,
     152,     0,     0,   151,     0,   160,     0,     0,     0,   158,
       0,     0,     0,    57,   139,     0,   172,     0,     0,    30,
     290,   222,   363,     0,   364,   366,     0,   388,     0,   369,
       0,     0,   125,    31,    33,     0,   321,     0,    67,   513,
     514,   128,     0,    34,   417,     0,   193,   184,   180,   185,
     181,     0,   346,   343,   145,    37,    50,    49,    52,     0,
     544,     0,     0,   531,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,   548,     8,     0,    40,     0,     0,
       0,   406,     0,     0,     0,   301,   358,   475,   572,   571,
     574,   583,   582,   587,   586,   568,   565,   566,   567,   506,
     555,     0,   528,   529,   527,   526,   507,   559,   570,   564,
     562,   573,   563,   561,   553,   558,   560,   569,   552,   556,
     557,   554,     0,     0,     0,     0,     0,     0,     0,   585,
     584,   589,   588,   487,   488,   490,   492,     0,   479,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   546,   476,
     476,   148,   344,   356,   411,     0,     0,   436,     0,   345,
     208,     0,     0,     0,     0,   451,     0,     0,     0,   161,
       0,   457,     0,     0,   159,   591,    60,     0,    53,    58,
       0,   171,     0,     0,     0,   435,   295,   292,   293,   294,
     298,   299,   300,   290,     0,   283,     0,   291,   309,     0,
     367,     0,   121,   122,   120,   119,     0,   387,   386,   498,
       0,   361,   496,   360,   525,   409,   408,     0,     0,     0,
     437,     0,   187,   350,   498,     0,   545,   505,   532,   415,
     534,   416,   168,     0,     0,     0,   547,   166,   461,     0,
     550,   549,     0,    42,    41,    39,    64,     0,    57,   209,
       0,     0,   219,     0,   216,   305,   302,   303,   306,   230,
       0,     0,    71,    72,    70,    69,    68,   523,   524,   489,
     491,     0,   478,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   144,   421,   422,   420,
       0,   357,     0,   170,     0,     0,   150,     0,   149,     0,
     482,     0,     0,   156,     0,     0,   154,     0,     0,   140,
       0,   141,     0,     0,   174,     0,   176,   296,   297,     0,
     290,   281,     0,   401,   310,   313,   312,   314,     0,   365,
     368,   369,     0,     0,   370,   371,   195,     0,     0,   194,
     197,   438,     0,   188,   191,     0,   347,   169,     0,     0,
       0,     0,   167,     0,     0,     0,     0,   224,   214,     0,
     217,   213,   304,   309,   279,    75,   262,    89,   260,    95,
      94,    78,    93,    90,   265,   105,    76,   106,    92,    96,
      77,    79,    91,   107,   259,   241,   244,   242,   243,   254,
     245,   258,   250,   248,   261,   264,   249,   247,   252,   257,
     246,   251,   255,   256,   253,   263,     0,   239,     0,    80,
       0,   279,   279,   237,   530,   480,   369,   516,   516,     0,
       0,     0,     0,     0,     0,     0,     0,   147,   146,     0,
     153,     0,     0,   450,     0,   449,     0,   481,     0,     0,
     456,   157,     0,   455,   155,    55,    54,   173,   466,   175,
       0,   401,   284,     0,     0,   369,   311,   327,   362,   392,
       0,   546,     0,   199,     0,     0,     0,   189,     0,   164,
     463,     0,     0,   162,   462,     0,   551,    65,    66,   203,
     511,   224,   224,   201,   224,   211,   220,   218,     0,   290,
     236,   240,     0,   266,     0,   232,   233,   484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   224,   454,     0,
       0,   483,   460,     0,     0,     0,     0,   178,   369,   401,
       0,   404,   403,   405,   498,   323,   287,   285,     0,     0,
       0,     0,   390,   498,   200,   198,     0,   192,     0,   165,
       0,   163,   212,   383,     0,   315,     0,   238,    75,    77,
     279,   279,     0,     0,     0,     0,     0,     0,   204,   202,
       0,     0,     0,     0,    56,   177,   286,   369,   393,     0,
     324,   326,   325,   341,     0,   342,   329,   332,     0,   328,
     319,   320,   223,     0,   190,     0,     0,     0,   382,   381,
     498,   316,   327,   280,   234,   235,     0,     0,     0,     0,
       0,     0,   453,   452,   459,   458,   289,   288,   395,   396,
     398,   498,     0,   546,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   498,   465,   464,     0,   373,     0,
       0,     0,   397,   399,   331,   333,   334,   337,   338,   339,
     335,   336,   330,   378,   376,   498,   546,   317,   231,   318,
     393,   377,   498,   400
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -903,  -903,  -903,    -1,  -413,  2072,  -903,  -903,  -903,  -903,
     340,   446,  -397,   113,   116,  -903,  -903,  -903,   227,   741,
    -903,    -9,  -102,  -635,  -903,  -795,  -903,  1473,  -902,  -691,
     -52,  -903,  -903,  -903,   198,  -903,  -903,  -903,   489,  -903,
      23,  -903,  -903,  -903,  -903,  -903,   583,   206,   -59,  -903,
    -903,  -903,     9,   781,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -123,  -142,  -775,  -903,  -117,    53,   181,  -903,
    -903,  -903,    16,  -903,  -903,  -252,     5,  -903,  -186,  -208,
    -238,  -229,  -303,  -903,  -181,  -903,    -4,   754,  -106,   366,
    -903,  -381,  -711,  -742,  -903,  -543,  -438,  -867,  -898,  -777,
     -50,  -903,   -10,  -903,  -160,  -903,   232,   685,  -288,  -903,
    -903,  -903,  1067,  -903,    15,  -903,  -903,  -194,  -903,  -557,
    -903,  -903,  1096,  1571,   -11,    -2,    64,   765,  -903,  1713,
    1752,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -363
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   294,   101,   581,   103,   104,   105,   311,
     106,   295,   576,   580,   577,   107,   108,   109,   110,   164,
     807,   239,   111,   236,   112,   607,   197,   244,   113,   114,
     115,   116,   117,   118,   682,   119,   120,   121,   447,   622,
     745,   122,   123,   618,   740,   124,   125,   483,   757,   126,
     127,   653,   654,   172,   237,   598,   129,   130,   485,   763,
     659,   810,   811,   417,   890,   421,   594,   595,   596,   597,
     660,   317,   728,   992,  1048,   982,   257,   971,   926,   930,
     976,   977,   978,   131,   283,   452,   132,   133,   240,   241,
     425,   426,   611,   989,   945,   429,   608,  1009,   923,   855,
     296,   188,   300,   301,   391,   392,   393,   173,   135,   136,
     137,   138,   174,   140,   161,   162,   537,   406,   699,   538,
     539,   141,   175,   176,   144,   198,   394,   178,   146,   179,
     180,   149,   150,   151,   306,   152,   153,   154,   155,   156
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     159,   100,   383,   367,   177,   254,   255,   593,   182,   541,
     410,   700,   258,   623,   189,   395,   579,   184,   192,   192,
     858,   202,   204,   208,   209,   809,   318,   193,   981,   617,
     430,   235,   245,   181,   246,   247,   895,   896,   427,   248,
     242,   249,   599,   250,   943,   404,   468,   242,    63,   304,
     418,   259,   263,   265,   267,   268,   269,   427,   731,   921,
     273,   669,   274,   157,   277,   281,   885,   404,   282,   284,
    1010,   973,   285,   286,   918,   287,   242,   288,   384,   928,
     289,   290,   695,   291,   702,   462,   259,   263,   305,   389,
     473,   547,   435,  -196,   -81,   157,  -379,     3,   670,  -384,
     466,   456,   -87,   299,   717,   897,   737,   315,   389,   467,
     471,   445,   463,   404,   815,  1033,   313,   160,   314,   474,
     404,   378,   748,   431,   860,  -379,   379,   264,   981,   427,
     816,  1012,   547,   316,   929,  -196,   367,  1015,  -379,   469,
     609,   718,   967,   973,   927,    63,  1032,   386,   738,   988,
     389,  -379,  -384,   437,   159,   375,   376,   377,   273,   259,
     305,   975,   302,   624,   404,  1016,   387,   437,   470,   389,
     379,  -384,   428,  -384,   282,   994,   995,   379,   944,  -124,
     263,  -384,    17,  1053,   469,   857,   438,   282,   163,  -196,
     398,   428,  -384,   942,   397,   192,   390,   382,  -379,   263,
    -384,   184,   739,  -379,   411,   541,   379,   966,   527,   528,
    -384,  -384,   379,   548,   379,   390,   885,   469,  1047,   730,
     431,   185,   974,   379,   469,   469,    22,  -402,   464,   424,
     201,   203,   505,   975,   370,   191,  -384,  -124,   371,   486,
      60,   263,   192,  -380,  -394,  1017,   861,   436,  -402,   439,
     440,   579,  -402,  1013,  1046,   465,  1007,   390,   459,   809,
     263,  -124,   480,   428,    79,  -394,   389,   389,   671,  -394,
      53,  1018,  -380,  1019,  1020,  -402,   390,  1021,  1022,   911,
     760,    61,   437,   460,   619,  -380,  1044,   461,  -384,   210,
     720,   561,  -394,   157,   458,   672,    22,   303,  -380,   643,
     183,   481,   276,   280,  1051,   761,    81,   593,   541,   693,
     487,   488,   489,   490,   492,   494,   495,   721,   496,   497,
     498,   499,   500,   610,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,   521,
      53,   853,   389,  -372,   884,  -380,   263,   263,   925,   563,
    -380,    61,   263,  -374,   836,   263,   263,   263,   263,   530,
     532,   540,  -486,   -83,   675,   251,  -485,   720,   371,   655,
     550,   -88,  -372,   390,   390,   554,    81,   555,   704,   707,
     431,   837,  -374,   243,   324,  -372,   252,   560,   259,  -486,
     565,   567,   724,  -485,   993,  -374,   571,   573,  -372,   553,
     270,   578,   578,   271,   582,   243,   584,   397,  -374,   437,
     431,   309,   312,   656,   424,   275,   279,   310,  -349,   612,
    -469,   522,   523,   932,   657,  -468,   616,   524,   646,   691,
     525,   526,   649,   263,   325,   319,   725,  -349,   326,   320,
     749,   751,   615,   658,   746,  -372,  -473,   726,   616,   390,
    -372,   263,   629,   263,   631,  -374,   389,   635,   616,   638,
    -374,   640,   642,   372,   615,   586,   727,   299,   321,   299,
     645,   259,  -472,   541,   615,   652,   593,   458,   368,  -522,
     369,  -522,  -474,   587,   332,   328,   588,   334,     5,   334,
     332,   337,   333,   334,   884,   379,  -522,   337,  -522,  -520,
    -521,  -520,  -521,   501,   324,   344,   589,  -471,  -470,   590,
     388,    55,   374,   350,   385,   451,   502,   441,   812,   453,
     591,   380,   399,   491,   493,   420,   302,   444,   302,   475,
     479,   565,   678,   571,   681,   582,   683,   638,   482,   592,
     503,   243,   243,   243,   243,   263,   484,   263,   552,   416,
    -385,   692,   556,   694,   325,   557,   733,   540,   326,   701,
    -385,   687,   558,   390,   684,   685,   924,   559,   529,   531,
     562,   568,   583,   933,   585,   600,  -282,   601,    72,   549,
     610,   614,   620,   586,   504,   243,   621,   723,   243,  -385,
     625,   424,   633,   269,   273,   305,   639,   205,    22,   263,
     566,   587,   650,  -282,   588,   328,   572,   243,    29,   253,
     332,  -389,   744,   334,   661,  -385,   282,   337,    38,   662,
     688,  -389,  -385,   663,   589,   243,   664,   590,   668,   307,
     667,   665,   666,   -59,   708,   709,   578,   578,   591,   711,
      51,   712,    53,  -385,   729,   741,   437,   736,   758,   759,
    -389,   808,   762,    61,   814,   820,   990,   592,   823,   825,
     540,   829,  -385,   492,   530,   819,   634,  -385,   822,   850,
    -385,  1014,   854,   856,   735,   879,  -389,   859,    81,   866,
    1011,   871,  -391,  -389,   833,   875,   835,   134,   864,  -108,
     877,   840,  -391,   878,   843,  1024,   134,   845,   889,   846,
    -114,  -113,   848,  -112,  -389,   -84,   -82,  -109,  -116,   134,
     851,   134,  -117,   424,  1036,  1037,  1038,  1039,  1040,  1041,
    -111,  -391,   243,  -389,  -115,  1011,   -85,   259,  -389,  -110,
    -118,  -389,   893,   744,   -86,   894,   916,   870,   958,   874,
    1045,   677,   876,   680,   919,   991,   243,  -391,   886,   -81,
     243,   652,   951,   936,  -391,   478,   134,   -83,  1052,  1043,
    1016,   755,  1011,   754,   186,   865,   690,   145,   449,   887,
     907,   950,   946,   852,   719,  -391,   145,   891,  1049,   888,
     396,  1006,   134,   128,  1030,  1042,   134,  1035,   207,   145,
     689,   145,   128,     0,  -391,   892,     0,   613,     0,  -391,
       0,   586,  -391,     0,     0,   128,     0,   128,     0,   833,
     835,     0,   840,   843,   870,   874,     0,     0,     0,   587,
     908,   909,   588,   910,     0,   540,     0,   912,   913,   254,
     255,   914,     0,     0,     0,     0,   145,     0,     0,     0,
     278,   278,   589,     0,     0,   590,   134,    55,     0,   931,
     638,     0,   128,     0,     0,   937,   591,   938,     0,   922,
       0,   940,   145,     0,     0,     0,   145,     0,     0,   192,
     969,     0,   491,   529,     0,   592,     0,     0,   128,     0,
       0,     0,   128,     0,     0,   808,     0,   908,   952,   953,
     912,   954,   955,   956,   957,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   964,     0,   243,   243,     0,   305,
       0,     0,   243,   243,   972,     0,     0,     0,   979,     0,
     134,     0,     0,     0,   716,     0,   145,   134,     0,     0,
     970,     0,     0,     0,     0,     0,     0,  1029,     0,     0,
       0,     0,   128,     0,     0,     0,     0,     0,     0,  1002,
    1003,  1004,  1005,     0,     0,     0,     0,     0,     0,     0,
       0,   743,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   874,  1008,  1025,  1026,   305,     0,     0,     0,
     134,     0,     0,     0,     0,  1002,  1003,  1004,  1005,  1025,
    1026,     0,     0,     0,   134,     0,     0,     0,     0,     0,
     145,  1031,   638,     0,  1034,     0,     0,   145,     0,     0,
       0,     0,     0,     0,     0,     0,   128,     0,  1008,     0,
       0,     0,     0,   128,   243,   243,     0,     0,     0,     0,
       0,     0,   243,     0,   416,   638,     0,     0,     0,   874,
       0,   416,     0,     0,   243,     0,     0,   243,     0,     0,
       0,     0,     0,     0,     0,  1008,     0,    17,     0,     0,
     145,    21,    22,     0,   134,     0,     0,     0,     0,   139,
      28,     0,    29,   880,   145,     0,   128,    34,   139,     0,
       0,     0,    38,     0,     0,   867,   134,     0,     0,     0,
     128,   139,   134,   139,     0,     0,     0,     0,   142,     0,
     134,     0,     0,     0,    51,     0,    53,   142,    55,     0,
     881,     0,     0,   882,     0,    60,     0,    61,     0,     0,
     142,     0,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,     0,   139,    79,
       0,     0,    81,     0,   145,     0,     0,     0,     0,     0,
       0,   134,     0,     0,     0,   134,     0,     0,     0,     0,
     128,     0,     0,   134,   139,     0,   145,   142,   139,     0,
       0,     0,   145,     0,     0,     0,     0,     0,     0,     0,
     145,     0,   128,     0,     0,     0,   324,     0,   128,     0,
       0,     0,    95,   142,     0,     0,   128,   142,   883,     0,
       0,     0,     0,     0,   243,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   673,     0,     0,     0,   139,     0,
       0,   145,     0,     0,     0,   145,   325,     0,     0,     0,
     326,     0,     0,   145,     0,     0,     0,   128,     0,   416,
     416,   128,     0,   416,   416,     0,     0,   142,     0,   128,
       0,     0,     0,     0,     0,     0,     0,     0,   134,     0,
     980,   327,     0,     0,     0,     0,   984,     0,   416,     0,
     416,     0,     0,     0,     0,     0,     0,   328,   329,     0,
     330,   331,   332,     0,   333,   334,   335,     0,   336,   337,
     338,   339,   139,     0,   341,   342,   343,   344,   345,   139,
     347,     0,     0,   348,   349,   350,     0,     0,   134,     0,
       0,     0,     0,   324,   134,     0,     0,     0,     0,   205,
      22,   142,     0,     0,     0,     0,     0,     0,   142,     0,
      29,   253,     0,     0,     0,   813,     0,     0,   145,     0,
      38,     0,     0,  -224,     0,     0,     0,     0,     0,   817,
     818,     0,   139,     0,   128,     0,     0,     0,     0,     0,
     980,     0,    51,   325,    53,     0,   139,   326,     0,  -224,
     134,     0,   134,     0,     0,    61,     0,   134,     0,     0,
     134,   142,     0,     0,     0,     0,     0,   134,   145,   324,
     134,     0,     0,     0,   145,   142,     0,     0,     0,   -63,
      81,     0,     0,     0,   128,     0,     0,     0,     0,     0,
     128,     0,   134,     0,   328,   329,     0,   330,   331,   332,
       0,   333,   334,   134,     0,   134,   337,     0,     0,     0,
     324,     0,     0,   343,   344,     0,   139,   347,     0,   325,
     348,   349,   350,   326,     0,     0,     0,     0,     0,     0,
     145,     0,   145,     0,     0,     0,     0,   145,   139,     0,
     145,   324,     0,     0,   139,   142,   128,   145,   128,     0,
     145,     0,   139,   128,     0,     0,   128,     0,     0,     0,
     325,     0,     0,   128,   326,     0,   128,   142,     0,     0,
     328,   329,   145,   142,   331,   332,     0,   333,   334,     0,
       0,   142,   337,   145,     0,   145,   134,     0,   128,     0,
     344,   325,     0,   134,     0,   326,   348,   349,   350,   128,
       0,   128,     0,   139,     0,   134,     0,   139,   266,     0,
       0,   328,   329,     0,     0,   139,   332,   134,   333,   334,
       0,     0,     0,   337,     0,     0,   134,     0,     0,     0,
     134,   344,   142,     0,     0,     0,   142,   348,   349,   350,
       0,     0,   328,   143,   142,     0,     0,   332,     0,   333,
     334,     0,   143,   324,   337,     0,     0,     0,     0,     0,
       0,     0,   344,     0,     0,   143,   145,   143,   348,   349,
     350,   134,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,   128,     0,     0,   145,     0,     0,     0,   128,
       0,     0,     0,     0,     0,     0,     0,   145,     0,     0,
       0,   128,     0,   325,     0,     0,   145,   326,     0,     0,
     145,     0,   143,   128,     0,   134,   134,   134,   134,     0,
     139,     0,   128,     0,     0,     0,   128,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
     134,   134,   143,     0,   400,   403,   405,   409,     0,   142,
       0,   145,     0,     0,   328,   329,     0,   330,   331,   332,
       0,   333,   334,   335,     0,     0,   337,   128,     0,     0,
     139,     0,     0,   343,   344,     0,   139,   347,     0,     0,
     348,   349,   350,     0,     0,   147,     0,     0,   433,     0,
       0,   434,     0,     0,   147,   145,   145,   145,   145,   142,
       0,     0,   143,     0,     0,   142,     0,   147,     0,   147,
     443,   128,   128,   128,   128,     0,     0,     0,     0,     0,
     145,   145,     0,     5,   148,     0,     0,     0,   454,     0,
       0,     0,   139,   148,   139,    14,   128,   128,    16,   139,
       0,     0,   139,    20,     0,     0,   148,     0,   148,   139,
       0,     0,   139,     0,   147,     0,     0,     0,     0,     0,
       0,   142,    36,   142,     0,     0,     0,    39,   142,     0,
       0,   142,     0,     0,   139,    44,   143,     0,   142,     0,
     147,   142,    49,   143,   147,   139,     0,   139,     0,     0,
       0,     0,     0,   148,     0,     0,     0,    59,     0,     0,
       0,     0,     0,   142,     0,     0,     0,     0,     0,    69,
       0,     0,     0,    72,   142,     0,   142,     0,     0,   148,
      78,     0,     0,   148,     0,   551,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   143,     0,     0,     0,
       0,     0,    17,     0,   147,     0,    21,    22,     0,   569,
     143,     0,     0,   574,     0,    28,     0,    29,   880,     0,
       0,     0,    34,     0,     0,     0,     0,    38,   139,     0,
       0,     0,     0,     0,     0,   139,     0,     0,     0,     0,
       0,     0,     0,   148,     0,     0,     0,   139,     0,    51,
       0,    53,     0,    55,     0,   881,     0,   142,   882,   139,
      60,     0,    61,     0,   142,     0,     0,     0,   139,     0,
       0,     0,   139,     0,     0,     0,   142,     0,   147,     0,
     143,    77,     0,     0,    79,   147,     0,    81,   142,     0,
       0,     0,     0,     0,     0,     0,     0,   142,     0,     0,
       0,   142,   143,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,   139,     0,     0,   143,   148,     0,     0,
       0,     0,     0,     0,   148,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,   147,     0,
       0,     0,   142,   959,     0,     0,     0,   324,     0,     0,
       0,     0,   147,     0,     0,     0,     0,   139,   139,   139,
     139,     0,     0,     0,     0,     0,     0,   143,     0,   696,
     698,   143,     0,     0,     0,   703,   706,   148,     0,   143,
      17,     0,   139,   139,    21,    22,   142,   142,   142,   142,
       0,   148,     0,    28,     0,    29,   880,   325,     0,     0,
      34,   326,     0,     0,   102,    38,     0,     0,     0,     0,
       0,   142,   142,   158,     0,     0,     0,     0,     0,     0,
       0,     0,   147,     0,     0,     0,   187,    51,   190,    53,
       0,   307,     0,   881,     0,     0,   882,     0,    60,     0,
      61,     0,     0,     0,   147,     0,     0,     0,   328,   329,
     147,   330,   331,   332,     0,   333,   334,     0,   147,    77,
     337,   148,    79,     0,     0,    81,     0,     0,   344,   205,
      22,     0,     0,   272,   348,   349,   350,     0,     0,     0,
      29,   253,     0,   148,   143,     0,     0,   827,   828,   148,
      38,     0,     0,  -224,     0,   830,     0,   148,     0,   102,
       0,     0,     0,   308,     0,     0,     0,   841,     0,   147,
     844,     0,    51,   147,    53,    95,     0,     0,     0,  -224,
       0,   147,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,   143,     0,     0,     0,     0,     0,
     143,     0,   756,     0,     0,     0,     0,     0,   148,   -62,
      81,     0,   148,     0,     0,     0,     0,     0,     0,     0,
     148,  -210,     0,   373,     0,  -210,  -210,     0,     0,     0,
       0,     0,     0,     0,  -210,     0,  -210,  -210,     0,     0,
       0,  -210,     0,     0,     0,     0,  -210,     0,     0,  -210,
       0,     0,     0,     0,     0,     0,   143,     0,   143,     0,
       0,     0,     0,   143,     0,     0,   143,     0,  -210,     0,
    -210,     0,  -210,   143,  -210,  -210,   143,  -210,     0,  -210,
       0,  -210,     0,     0,     0,     0,   147,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   419,   143,     0,
    -210,     0,     0,  -210,   432,     0,  -210,     0,     0,   143,
       0,   143,     0,     0,     0,     0,   906,     0,     0,     0,
       0,     0,     0,     0,     0,   148,     0,   935,     0,     0,
       0,     0,     0,     0,     0,  -210,   147,     0,     0,  -210,
    -210,     0,   147,     0,     0,     0,     0,     0,  -210,     0,
    -210,  -210,     0,     0,     0,  -210,  -210,   102,     0,     0,
    -210,     0,  -210,  -210,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,     0,   148,     0,     0,     0,     0,
       0,   148,  -210,     0,  -210,     0,  -210,     0,  -210,  -210,
       0,  -210,   143,  -210,     0,  -210,     0,     0,   147,   143,
     147,     0,     0,     0,     0,   147,     0,     0,   147,     0,
       0,   143,     0,     0,  -210,   147,     0,  -210,   147,     0,
    -210,     0,     0,   143,     0,     0,     0,     0,     0,     0,
       0,     0,   143,     0,     0,     0,   143,   148,     0,   148,
     147,   102,     0,     0,   148,     0,     0,   148,     0,     0,
       0,   147,     0,   147,   148,     0,     0,   148,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,   432,
    -210,     0,     0,     0,     0,     0,  -210,   143,     0,   148,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     148,     0,   148,     0,     0,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,   143,   143,   143,     0,  -493,     0,   632,     0,
       0,     0,   637,     0,   147,     0,     0,     0,     0,   366,
     102,   147,  -520,     0,  -520,     0,   143,   143,     0,   211,
       0,     0,     0,   147,     0,     0,     0,     0,     0,     0,
       0,   212,   213,     0,   214,   147,     0,     0,     0,   215,
       0,     0,     0,   148,   147,     0,     0,   216,   147,     0,
     148,     0,     0,   217,     0,     0,     0,     0,   218,     0,
     219,     0,   148,   220,     0,     0,     0,     0,     0,     0,
       0,   221,     0,     0,   148,   324,     0,   222,   223,     0,
       0,     0,     0,   148,   224,     0,     0,   148,     0,   147,
       0,     0,     0,   225,     0,     0,     0,     0,     0,     0,
       0,     0,   226,   227,     0,   228,     0,   229,     0,   230,
       0,     0,   231,     0,     0,   714,   232,   422,     0,   233,
       0,     0,   234,     0,     0,   325,     0,     0,   148,   326,
       0,     0,     0,   147,   147,   147,   147,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   147,   147,
       0,     0,     0,     0,     0,   747,     0,     0,     0,     0,
       0,   752,   148,   148,   148,   148,   328,   329,   423,   330,
     331,   332,     0,   333,   334,   335,     0,     0,   337,   338,
     339,     0,     0,   341,   342,   343,   344,   148,   148,   347,
       0,     0,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
       0,     0,     0,     0,     0,     0,     0,   432,     0,   432,
       0,     0,     0,     0,   432,     0,     0,   432,     0,     0,
       0,     0,     0,     0,   847,     0,     0,   849,     0,   366,
       0,     0,  -520,     0,  -520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   863,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     869,     0,   873,     0,     0,     0,     0,    -2,     4,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,    16,     0,    17,    18,    19,
      20,    21,    22,    23,    24,    25,     0,    26,    27,     0,
      28,     0,    29,    30,    31,    32,    33,    34,    35,    36,
       0,    37,    38,     0,    39,  -224,     0,    40,    41,    42,
       0,    43,    44,    45,   -43,    46,    47,     0,    48,    49,
      50,     0,     0,     0,    51,    52,    53,    54,    55,    56,
      57,  -224,   -43,    58,    59,    60,     0,    61,    62,    63,
       0,    64,    65,    66,    67,    68,    69,    70,    71,     0,
      72,    73,   917,    74,    75,    76,    77,    78,     0,    79,
      80,   -61,    81,    82,   934,     0,    83,     0,    84,     0,
       0,    85,     0,   939,     0,     0,     0,   941,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,     0,    95,     0,     0,    96,     0,    97,   965,    98,
    1027,    99,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   165,     0,     0,    14,    15,     0,    16,     0,     0,
       0,     0,    20,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,     0,    31,   166,   167,     0,
     168,    36,     0,    37,     0,     0,    39,     0,     0,     0,
      41,    42,     0,     0,    44,    45,     0,    46,    47,     0,
      48,    49,     0,     0,     0,     0,     0,    52,     0,    54,
       0,    56,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,    70,
     169,     0,    72,     0,     0,    74,     0,     0,     0,    78,
       0,     0,    80,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,   256,     0,     0,     0,    96,     0,   170,
       0,    98,     0,   171,  1028,     4,     0,     5,     6,     7,
       8,     9,    10,     0,  -546,     0,    11,    12,    13,    14,
      15,  -546,    16,     0,    17,    18,    19,    20,    21,    22,
      23,    24,    25,  -546,    26,    27,  -546,    28,     0,    29,
      30,    31,    32,    33,    34,    35,    36,     0,    37,    38,
       0,    39,  -224,     0,    40,    41,    42,     0,    43,    44,
      45,   -43,    46,    47,     0,    48,    49,    50,     0,     0,
       0,    51,    52,    53,    54,     0,    56,    57,  -224,   -43,
      58,    59,    60,  -546,    61,    62,    63,  -546,    64,    65,
      66,    67,    68,    69,    70,    71,     0,    72,    73,     0,
      74,    75,    76,    77,    78,     0,    79,    80,   -61,    81,
      82,     0,     0,    83,     0,    84,     0,     0,  -546,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -546,  -546,    88,  -546,  -546,  -546,
    -546,  -546,  -546,  -546,     0,  -546,  -546,  -546,  -546,  -546,
       0,  -546,  -546,  -546,  -546,  -546,  -546,  -546,  -546,    95,
    -546,  -546,  -546,     0,    97,  -546,    98,   292,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,    16,     0,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     0,    26,    27,     0,    28,
       0,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,    38,     0,    39,  -224,     0,    40,    41,    42,     0,
      43,    44,    45,   -43,    46,    47,     0,    48,    49,    50,
       0,     0,     0,    51,    52,    53,    54,    55,    56,    57,
    -224,   -43,    58,    59,    60,     0,    61,    62,    63,     0,
      64,    65,    66,    67,    68,    69,    70,    71,     0,    72,
      73,     0,    74,    75,    76,    77,    78,     0,    79,    80,
     -61,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
     256,    95,     0,     0,    96,     0,    97,   293,    98,     4,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,    16,     0,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     0,    26,    27,
       0,    28,     0,    29,    30,    31,    32,    33,    34,    35,
      36,     0,    37,    38,     0,    39,  -224,     0,    40,    41,
      42,     0,    43,    44,    45,   -43,    46,    47,     0,    48,
      49,    50,     0,     0,     0,    51,    52,    53,    54,    55,
      56,    57,  -224,   -43,    58,    59,    60,     0,    61,    62,
      63,     0,    64,    65,    66,    67,    68,    69,    70,    71,
       0,    72,    73,     0,    74,    75,    76,    77,    78,     0,
      79,    80,   -61,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,    95,     0,     0,    96,     0,    97,   457,
      98,   476,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,    16,     0,
      17,    18,    19,    20,    21,    22,    23,    24,    25,     0,
      26,    27,     0,    28,     0,    29,    30,    31,    32,    33,
      34,    35,    36,     0,    37,    38,     0,    39,  -224,     0,
      40,    41,    42,     0,    43,    44,    45,   -43,    46,    47,
       0,    48,    49,    50,     0,     0,     0,    51,    52,    53,
      54,    55,    56,    57,  -224,   -43,    58,    59,    60,     0,
      61,    62,    63,     0,    64,    65,    66,    67,    68,    69,
      70,    71,     0,    72,    73,     0,    74,    75,    76,    77,
      78,     0,    79,    80,   -61,    81,    82,     0,     0,    83,
       0,    84,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,    95,     0,     0,    96,     0,
      97,   477,    98,   292,    99,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
      16,     0,    17,    18,    19,    20,    21,    22,    23,    24,
      25,     0,    26,    27,     0,    28,     0,    29,    30,    31,
      32,    33,    34,    35,    36,     0,    37,    38,     0,    39,
    -224,     0,    40,    41,    42,     0,    43,    44,    45,   -43,
      46,    47,     0,    48,    49,    50,     0,     0,     0,    51,
      52,    53,    54,    55,    56,    57,  -224,   -43,    58,    59,
      60,     0,    61,    62,    63,     0,    64,    65,    66,    67,
      68,    69,    70,    71,     0,    72,    73,     0,    74,    75,
      76,    77,    78,     0,    79,    80,   -61,    81,    82,     0,
       0,    83,     0,    84,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,    95,     0,     0,
      96,     0,    97,   293,    98,     4,    99,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,    16,     0,    17,    18,    19,    20,    21,    22,
      23,    24,    25,     0,    26,    27,     0,    28,     0,    29,
      30,    31,    32,    33,    34,    35,    36,     0,    37,    38,
       0,    39,  -224,     0,    40,    41,    42,     0,    43,    44,
      45,   -43,    46,    47,     0,    48,    49,    50,     0,     0,
       0,    51,    52,    53,    54,    55,    56,    57,  -224,   -43,
      58,    59,    60,     0,    61,    62,    63,     0,    64,    65,
      66,    67,    68,    69,    70,    71,     0,    72,    73,     0,
      74,    75,    76,    77,    78,     0,    79,    80,   -61,    81,
      82,     0,     0,    83,     0,    84,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,    95,
       0,     0,    96,     0,    97,   644,    98,     4,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,    16,     0,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     0,    26,    27,     0,    28,
       0,    29,    30,    31,    32,    33,    34,    35,    36,     0,
      37,    38,     0,    39,  -224,     0,    40,    41,    42,     0,
      43,    44,    45,   -43,    46,    47,     0,    48,    49,    50,
       0,     0,     0,    51,    52,    53,    54,   307,    56,    57,
    -224,   -43,    58,    59,    60,     0,    61,    62,    63,     0,
      64,    65,    66,    67,    68,    69,    70,    71,     0,    72,
      73,     0,    74,    75,    76,    77,    78,     0,    79,    80,
     -61,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,    95,     0,     0,    96,     0,    97,     0,    98,     4,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,    16,     0,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     0,    26,    27,
       0,    28,     0,    29,    30,    31,    32,    33,    34,    35,
      36,     0,    37,    38,     0,    39,  -224,     0,    40,    41,
      42,     0,    43,    44,    45,   -43,    46,    47,     0,    48,
      49,    50,     0,     0,     0,    51,    52,    53,    54,     0,
      56,    57,  -224,   -43,    58,    59,    60,     0,    61,    62,
      63,     0,    64,    65,    66,    67,    68,    69,    70,    71,
       0,    72,    73,     0,    74,    75,    76,    77,    78,     0,
      79,    80,   -61,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,    95,     0,     0,    96,     0,    97,     0,
      98,     4,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,    16,     0,
      17,    18,    19,    20,    21,    22,    23,    24,    25,     0,
      26,    27,     0,    28,     0,    29,    30,    31,    32,    33,
      34,   564,    36,     0,    37,    38,     0,    39,  -224,     0,
      40,    41,    42,     0,    43,    44,    45,   -43,    46,    47,
       0,    48,    49,    50,     0,     0,     0,    51,    52,    53,
      54,     0,    56,    57,  -224,   -43,    58,    59,    60,     0,
      61,    62,    63,     0,    64,    65,    66,    67,    68,    69,
      70,    71,     0,    72,    73,     0,    74,    75,    76,    77,
      78,     0,    79,    80,   -61,    81,    82,     0,     0,    83,
       0,    84,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,    95,     0,     0,    96,     0,
      97,     0,    98,     4,    99,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
      16,     0,    17,    18,    19,    20,    21,    22,    23,    24,
      25,     0,    26,    27,     0,    28,     0,    29,    30,    31,
      32,    33,    34,   570,    36,     0,    37,    38,     0,    39,
    -224,     0,    40,    41,    42,     0,    43,    44,    45,   -43,
      46,    47,     0,    48,    49,    50,     0,     0,     0,    51,
      52,    53,    54,     0,    56,    57,  -224,   -43,    58,    59,
      60,     0,    61,    62,    63,     0,    64,    65,    66,    67,
      68,    69,    70,    71,     0,    72,    73,     0,    74,    75,
      76,    77,    78,     0,    79,    80,   -61,    81,    82,     0,
       0,    83,     0,    84,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,    95,     0,     0,
      96,     0,    97,     0,    98,     4,    99,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,    16,     0,    17,    18,    19,    20,    21,    22,
      23,    24,    25,     0,    26,    27,     0,    28,     0,    29,
      30,    31,    32,    33,    34,   832,    36,     0,    37,    38,
       0,    39,  -224,     0,    40,    41,    42,     0,    43,    44,
      45,   -43,    46,    47,     0,    48,    49,    50,     0,     0,
       0,    51,    52,    53,    54,     0,    56,    57,  -224,   -43,
      58,    59,    60,     0,    61,    62,    63,     0,    64,    65,
      66,    67,    68,    69,    70,    71,     0,    72,    73,     0,
      74,    75,    76,    77,    78,     0,    79,    80,   -61,    81,
      82,     0,     0,    83,     0,    84,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,    95,
       0,     0,    96,     0,    97,     0,    98,     4,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,    16,     0,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     0,    26,    27,     0,    28,
       0,    29,    30,    31,    32,    33,    34,   834,    36,     0,
      37,    38,     0,    39,  -224,     0,    40,    41,    42,     0,
      43,    44,    45,   -43,    46,    47,     0,    48,    49,    50,
       0,     0,     0,    51,    52,    53,    54,     0,    56,    57,
    -224,   -43,    58,    59,    60,     0,    61,    62,    63,     0,
      64,    65,    66,    67,    68,    69,    70,    71,     0,    72,
      73,     0,    74,    75,    76,    77,    78,     0,    79,    80,
     -61,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,    95,     0,     0,    96,     0,    97,     0,    98,     4,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,    16,     0,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     0,    26,    27,
       0,    28,     0,    29,    30,    31,    32,    33,    34,   839,
      36,     0,    37,    38,     0,    39,  -224,     0,    40,    41,
      42,     0,    43,    44,    45,   -43,    46,    47,     0,    48,
      49,    50,     0,     0,     0,    51,    52,    53,    54,     0,
      56,    57,  -224,   -43,    58,    59,    60,     0,    61,    62,
      63,     0,    64,    65,    66,    67,    68,    69,    70,    71,
       0,    72,    73,     0,    74,    75,    76,    77,    78,     0,
      79,    80,   -61,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,    95,     0,     0,    96,     0,    97,     0,
      98,     4,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,    16,     0,
      17,    18,    19,    20,    21,    22,    23,    24,    25,     0,
      26,    27,     0,    28,     0,    29,    30,    31,    32,    33,
      34,   842,    36,     0,    37,    38,     0,    39,  -224,     0,
      40,    41,    42,     0,    43,    44,    45,   -43,    46,    47,
       0,    48,    49,    50,     0,     0,     0,    51,    52,    53,
      54,     0,    56,    57,  -224,   -43,    58,    59,    60,     0,
      61,    62,    63,     0,    64,    65,    66,    67,    68,    69,
      70,    71,     0,    72,    73,     0,    74,    75,    76,    77,
      78,     0,    79,    80,   -61,    81,    82,     0,     0,    83,
       0,    84,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,    95,     0,     0,    96,     0,
      97,     0,    98,     4,    99,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
      16,     0,    17,    18,    19,    20,    21,    22,    23,    24,
      25,     0,   862,    27,     0,    28,     0,    29,    30,    31,
      32,    33,    34,    35,    36,     0,    37,    38,     0,    39,
    -224,     0,    40,    41,    42,     0,    43,    44,    45,   -43,
      46,    47,     0,    48,    49,    50,     0,     0,     0,    51,
      52,    53,    54,     0,    56,    57,  -224,   -43,    58,    59,
      60,     0,    61,    62,    63,     0,    64,    65,    66,    67,
      68,    69,    70,    71,     0,    72,    73,     0,    74,    75,
      76,    77,    78,     0,    79,    80,   -61,    81,    82,     0,
       0,    83,     0,    84,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,    95,     0,     0,
      96,     0,    97,     0,    98,     4,    99,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,    16,     0,    17,    18,    19,    20,    21,    22,
      23,    24,    25,     0,    26,    27,     0,    28,     0,    29,
      30,    31,    32,    33,    34,   868,    36,     0,    37,    38,
       0,    39,  -224,     0,    40,    41,    42,     0,    43,    44,
      45,   -43,    46,    47,     0,    48,    49,    50,     0,     0,
       0,    51,    52,    53,    54,     0,    56,    57,  -224,   -43,
      58,    59,    60,     0,    61,    62,    63,     0,    64,    65,
      66,    67,    68,    69,    70,    71,     0,    72,    73,     0,
      74,    75,    76,    77,    78,     0,    79,    80,   -61,    81,
      82,     0,     0,    83,     0,    84,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,    95,
       0,     0,    96,     0,    97,     0,    98,     4,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,    16,     0,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     0,    26,    27,     0,    28,
       0,    29,    30,    31,    32,    33,    34,   872,    36,     0,
      37,    38,     0,    39,  -224,     0,    40,    41,    42,     0,
      43,    44,    45,   -43,    46,    47,     0,    48,    49,    50,
       0,     0,     0,    51,    52,    53,    54,     0,    56,    57,
    -224,   -43,    58,    59,    60,     0,    61,    62,    63,     0,
      64,    65,    66,    67,    68,    69,    70,    71,     0,    72,
      73,     0,    74,    75,    76,    77,    78,     0,    79,    80,
     -61,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,    95,     0,     0,    96,     0,    97,     0,    98,   732,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     165,     0,     0,    14,    15,     0,    16,     0,     0,     0,
       0,    20,     0,     0,     0,     0,     0,     0,     0,    27,
       0,     0,     0,     0,     0,    31,   166,   167,     0,   168,
      36,     0,    37,     0,     0,    39,     0,     0,     0,    41,
      42,     0,     0,    44,    45,     0,    46,    47,     0,    48,
      49,     0,     0,     0,     0,     0,    52,     0,    54,     0,
      56,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,    70,   169,
       0,    72,     0,     0,    74,     0,     0,     0,    78,     0,
       0,    80,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,   256,     0,     0,     0,    96,     0,   170,     0,
      98,     0,   171,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   165,     0,     0,    14,    15,     0,    16,     0,
       0,     0,     0,    20,     0,     0,     0,     0,     0,     0,
       0,    27,     0,     0,     0,     0,     0,    31,   166,   167,
       0,   168,    36,     0,    37,     0,     0,    39,     0,     0,
       0,    41,    42,     0,     0,    44,    45,     0,    46,    47,
       0,    48,    49,     0,     0,     0,     0,     0,    52,     0,
      54,     0,    56,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
      70,   169,     0,    72,     0,     0,    74,     0,     0,     0,
      78,     0,     0,    80,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,   256,     0,     0,     0,    96,     0,
     170,     0,    98,     0,   171,   636,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   165,     0,     0,    14,    15,
       0,    16,     0,     0,     0,     0,    20,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,     0,     0,     0,
      31,   166,   167,     0,   168,    36,     0,    37,     0,     0,
      39,     0,     0,     0,    41,    42,     0,     0,    44,    45,
       0,    46,    47,     0,    48,    49,     0,     0,     0,     0,
       0,    52,     0,    54,     0,    56,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,    70,   169,     0,    72,     0,     0,    74,
       0,     0,     0,    78,     0,     0,    80,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,   256,     0,     0,
       0,    96,     0,   170,     0,    98,     0,   171,   968,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   165,     0,
       0,    14,    15,     0,    16,     0,     0,     0,     0,    20,
       0,     0,     0,     0,     0,     0,     0,    27,     0,     0,
       0,     0,     0,    31,   166,   167,     0,   168,    36,     0,
      37,     0,     0,    39,     0,     0,     0,    41,    42,     0,
       0,    44,    45,     0,    46,    47,     0,    48,    49,     0,
       0,     0,     0,     0,    52,     0,    54,     0,    56,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,    70,   169,     0,    72,
       0,     0,    74,     0,     0,     0,    78,     0,     0,    80,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
     764,     0,   765,     0,    96,     0,   170,     0,    98,   766,
     171,   641,     0,     0,   767,   213,   768,   769,     0,     0,
       0,     0,   770,     0,     0,     0,     0,     0,     0,     0,
     216,     0,     0,     0,     0,     0,   771,     0,     0,     0,
       0,   772,     0,   219,     0,     0,   773,     0,   774,     0,
       0,     0,     0,     0,   775,     0,     0,     0,     0,     0,
     776,   777,     0,     0,     0,     0,     0,   224,     0,     0,
       0,     0,     0,     0,     0,     0,   778,     0,     0,     0,
       0,     0,     0,     0,     0,   226,   227,     0,   779,     0,
     229,     0,   780,     0,     0,   781,     0,     0,     0,   782,
       0,     0,   233,     0,     0,   783,     0,     0,     0,     0,
       0,     0,     0,     0,   351,   352,   353,   354,   355,   356,
       0,     0,   359,   360,   361,   362,     0,   364,   365,   784,
     785,   786,   787,   788,     0,     0,   789,     0,     0,     0,
     790,   791,   792,   793,   794,   795,   796,   797,   798,   799,
     800,     0,   801,     0,     0,   802,   803,   804,   805,     0,
       0,   806,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   165,     0,     0,    14,    15,     0,    16,     0,     0,
       0,     0,    20,   205,    22,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,   206,    31,   166,   167,     0,
     168,    36,     0,    37,     0,     0,    39,     0,     0,     0,
      41,    42,     0,     0,    44,    45,     0,    46,    47,     0,
      48,    49,     0,     0,     0,     0,     0,    52,    53,    54,
       0,    56,     0,     0,     0,     0,    59,     0,     0,    61,
       0,     0,     0,     0,     0,    66,    67,    68,    69,    70,
     169,     0,    72,     0,     0,    74,     0,     0,     0,    78,
       0,     0,    80,     0,    81,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,     0,     0,     0,     0,    96,     0,   170,
       0,    98,     0,   171,     5,     6,     7,     8,   194,    10,
     195,     0,     0,   165,     0,     0,    14,    15,     0,    16,
       0,  -123,     0,     0,    20,     0,     0,     0,     0,     0,
       0,     0,    27,     0,     0,     0,     0,     0,    31,   166,
     167,     0,   168,    36,     0,    37,     0,     0,    39,     0,
       0,     0,    41,    42,     0,     0,    44,    45,     0,    46,
      47,     0,    48,    49,     0,     0,     0,     0,     0,    52,
       0,    54,     0,    56,     0,     0,     0,     0,    59,  -123,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,    70,   169,     0,    72,     0,     0,    74,     0,     0,
     196,    78,     0,  -123,    80,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,     0,     0,     0,     0,    96,
       0,   170,     0,    98,     0,   171,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   165,     0,     0,    14,    15,
       0,    16,     0,     0,     0,     0,    20,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,     0,     0,     0,
      31,   166,   167,     0,   168,    36,     0,    37,     0,     0,
      39,     0,     0,     0,    41,    42,     0,     0,    44,    45,
       0,    46,    47,     0,    48,    49,     0,     0,     0,     0,
       0,    52,     0,    54,     0,    56,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,    70,   169,     0,    72,     0,     0,    74,
     260,   261,     0,    78,   297,     0,    80,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,   298,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,   256,     0,     0,
       0,    96,     0,   170,     0,    98,     0,   171,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   165,     0,     0,
      14,    15,     0,    16,     0,     0,     0,     0,    20,     0,
       0,     0,     0,     0,     0,     0,    27,     0,     0,     0,
       0,     0,    31,   166,   167,     0,   168,    36,     0,    37,
       0,     0,    39,     0,     0,     0,    41,    42,     0,     0,
      44,    45,     0,    46,    47,     0,    48,    49,     0,     0,
       0,     0,     0,    52,     0,    54,     0,    56,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,    70,   169,     0,    72,     0,
       0,    74,   260,   261,     0,    78,   297,     0,    80,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,   256,
       0,     0,     0,    96,     0,   170,     0,    98,   628,   171,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   165,
       0,     0,    14,    15,     0,    16,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,     0,    31,   166,   167,     0,   168,    36,
       0,    37,     0,     0,    39,     0,     0,     0,    41,    42,
       0,     0,    44,    45,     0,    46,    47,     0,    48,    49,
       0,     0,     0,     0,     0,    52,     0,    54,     0,    56,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,    70,   169,     0,
      72,     0,     0,    74,   260,   261,     0,    78,   297,     0,
      80,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,   256,     0,     0,     0,    96,     0,   170,     0,    98,
     630,   171,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   165,     0,     0,    14,    15,     0,    16,     0,     0,
       0,     0,    20,     0,   533,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,     0,    31,   166,   167,     0,
     168,    36,   534,    37,     0,     0,    39,     0,     0,     0,
      41,    42,     0,     0,    44,    45,     0,    46,    47,     0,
      48,    49,     0,     0,     0,     0,     0,    52,     0,    54,
       0,    56,     0,     0,     0,     0,    59,     0,     0,   535,
       0,     0,     0,     0,     0,    66,    67,    68,    69,    70,
     169,     0,    72,     0,     0,    74,     0,     0,     0,    78,
       0,     0,    80,     0,   536,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,     0,     0,     0,     0,    96,     0,   170,
       0,    98,     0,   171,     5,     6,     7,     8,   194,    10,
       0,     0,     0,   165,     0,     0,    14,    15,     0,    16,
       0,  -123,     0,     0,    20,     0,     0,     0,     0,     0,
       0,     0,    27,     0,     0,     0,     0,     0,    31,   166,
     167,     0,   168,    36,     0,    37,     0,     0,    39,     0,
       0,     0,    41,    42,     0,     0,    44,    45,     0,    46,
      47,     0,    48,    49,     0,     0,     0,     0,     0,    52,
       0,    54,     0,    56,     0,     0,     0,     0,    59,  -123,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,    70,   169,     0,    72,     0,     0,    74,     0,     0,
     196,    78,     0,  -123,    80,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,     0,     0,     0,     0,    96,
       0,   170,     0,    98,     0,   171,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   165,     0,     0,    14,    15,
       0,    16,     0,     0,     0,     0,    20,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,     0,     0,     0,
      31,   166,   167,     0,   168,    36,     0,    37,     0,     0,
      39,     0,     0,     0,    41,    42,     0,     0,    44,    45,
       0,    46,    47,     0,    48,    49,     0,     0,     0,     0,
       0,    52,     0,    54,     0,    56,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,    70,   169,     0,    72,     0,     0,    74,
     260,   261,     0,    78,     0,     0,    80,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,     0,   262,     0,
       0,    96,     0,   170,     0,    98,     0,   171,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   165,     0,     0,
      14,    15,     0,    16,     0,     0,     0,     0,    20,     0,
       0,     0,     0,     0,     0,     0,    27,     0,     0,     0,
       0,     0,    31,   166,   167,     0,   168,    36,     0,    37,
       0,     0,    39,     0,     0,     0,    41,    42,     0,     0,
      44,    45,     0,    46,    47,     0,    48,    49,     0,     0,
       0,     0,     0,    52,     0,    54,     0,    56,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,    70,   169,     0,    72,     0,
       0,    74,   260,   261,     0,    78,     0,     0,    80,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,   256,
       0,     0,     0,    96,     0,   170,     0,    98,     0,   171,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   165,
       0,     0,    14,    15,     0,    16,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,     0,    31,   166,   167,     0,   168,    36,
       0,    37,     0,     0,    39,     0,     0,     0,    41,    42,
       0,     0,    44,    45,     0,    46,    47,   734,    48,    49,
       0,     0,     0,     0,     0,    52,     0,    54,     0,    56,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,    70,   169,     0,
      72,     0,     0,    74,   260,   261,     0,    78,     0,     0,
      80,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,     0,     0,     0,     0,    96,     0,   170,     0,    98,
       0,   171,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   165,     0,     0,    14,    15,     0,    16,     0,     0,
       0,     0,    20,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,     0,    31,   166,   167,     0,
     168,    36,     0,    37,     0,     0,    39,     0,     0,     0,
      41,    42,     0,     0,    44,    45,     0,    46,    47,     0,
      48,    49,     0,     0,     0,     0,     0,    52,   199,    54,
       0,    56,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,    70,
     169,     0,    72,     0,     0,    74,     0,     0,     0,    78,
       0,     0,    80,     0,     0,    82,     0,     0,     0,     0,
       0,   200,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,     0,     0,     0,     0,    96,     0,   170,
       0,    98,     0,   171,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   165,     0,     0,    14,    15,     0,    16,
       0,     0,     0,     0,    20,     0,     0,     0,     0,     0,
       0,     0,    27,     0,     0,     0,     0,     0,    31,   166,
     167,     0,   168,    36,     0,    37,     0,     0,    39,     0,
       0,     0,    41,    42,     0,     0,    44,    45,     0,    46,
      47,     0,    48,    49,     0,     0,     0,     0,     0,    52,
       0,    54,     0,    56,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,    70,   169,     0,    72,     0,     0,    74,     0,     0,
       0,    78,     0,     0,    80,     0,     0,    82,     0,     0,
       0,     0,     0,   200,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,   256,     0,     0,     0,    96,
       0,   170,     0,    98,     0,   171,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   165,     0,     0,    14,    15,
       0,    16,     0,     0,     0,     0,    20,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,     0,     0,     0,
      31,   166,   167,     0,   168,    36,     0,    37,     0,     0,
      39,     0,     0,     0,    41,    42,     0,     0,    44,    45,
       0,    46,    47,     0,    48,    49,     0,     0,     0,     0,
       0,    52,     0,    54,     0,    56,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,    70,   169,     0,    72,     0,     0,    74,
     260,   261,     0,    78,     0,     0,    80,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,     0,     0,     0,
       0,    96,     0,   170,     0,    98,     0,   171,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   165,     0,     0,
      14,    15,     0,    16,     0,     0,     0,     0,    20,     0,
       0,     0,     0,     0,     0,     0,    27,     0,     0,     0,
       0,     0,    31,   166,   167,     0,   168,    36,     0,    37,
       0,     0,    39,     0,     0,     0,    41,    42,     0,     0,
      44,    45,     0,    46,    47,     0,    48,    49,     0,     0,
       0,     0,     0,    52,     0,    54,     0,    56,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,    70,   169,     0,    72,     0,
       0,    74,     0,     0,     0,    78,     0,     0,    80,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,   256,
       0,     0,     0,    96,     0,   170,   626,    98,     0,   171,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   165,
       0,     0,    14,    15,     0,    16,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,     0,    31,   166,   167,     0,   168,    36,
       0,    37,     0,     0,    39,     0,     0,     0,    41,    42,
       0,     0,    44,    45,     0,    46,    47,     0,    48,    49,
       0,     0,     0,     0,     0,    52,     0,    54,     0,    56,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,    70,   169,     0,
      72,     0,     0,    74,     0,     0,     0,    78,     0,     0,
      80,     0,     0,    82,     0,     0,     0,     0,     0,   200,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,     0,     0,     0,     0,    96,     0,   170,     0,    98,
       0,   171,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   165,     0,     0,    14,    15,     0,    16,     0,     0,
       0,     0,    20,     0,     0,     0,     0,     0,     0,   242,
      27,     0,     0,     0,     0,     0,    31,   166,   167,     0,
     168,    36,     0,    37,     0,     0,    39,     0,     0,     0,
      41,    42,     0,     0,    44,    45,     0,    46,    47,     0,
      48,    49,     0,     0,     0,     0,     0,    52,     0,    54,
       0,    56,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,    70,
     169,     0,    72,     0,     0,    74,     0,     0,     0,    78,
       0,     0,    80,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,     0,     0,     0,     0,    96,     0,    97,
       0,    98,     0,   171,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   165,     0,     0,    14,    15,     0,    16,
       0,     0,     0,     0,    20,     0,     0,     0,     0,     0,
       0,     0,    27,     0,     0,     0,     0,     0,    31,   166,
     167,     0,   168,    36,     0,    37,     0,     0,    39,     0,
       0,     0,    41,    42,     0,     0,    44,    45,     0,    46,
      47,     0,    48,    49,     0,     0,     0,     0,     0,    52,
       0,    54,     0,    56,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,    70,   169,     0,    72,     0,     0,    74,     0,     0,
       0,    78,     0,     0,    80,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,   256,     0,     0,     0,    96,
       0,   170,     0,    98,     0,   171,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   165,     0,     0,    14,    15,
       0,    16,     0,     0,     0,     0,    20,     0,     0,     0,
       0,     0,     0,     0,   602,     0,     0,     0,     0,     0,
      31,   166,   167,     0,   168,    36,     0,   603,     0,     0,
      39,     0,     0,     0,    41,    42,     0,     0,    44,    45,
       0,    46,    47,     0,    48,    49,     0,     0,     0,     0,
       0,    52,     0,    54,     0,    56,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,    66,
     604,    68,    69,    70,   605,     0,    72,     0,     0,    74,
       0,     0,     0,    78,     0,     0,    80,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,   256,     0,     0,
       0,    96,     0,   170,     0,    98,     0,   920,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   165,     0,     0,
      14,    15,     0,    16,     0,     0,     0,     0,    20,     0,
       0,     0,     0,     0,     0,     0,    27,     0,     0,     0,
       0,     0,    31,   166,   167,     0,   168,    36,     0,    37,
       0,     0,    39,     0,     0,     0,    41,    42,     0,     0,
      44,    45,     0,    46,    47,     0,    48,    49,     0,     0,
       0,     0,     0,    52,     0,    54,     0,    56,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,    70,   169,     0,    72,     0,
       0,    74,     0,     0,     0,    78,     0,     0,    80,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,   256,
       0,     0,     0,    96,     0,   170,     0,    98,     0,   920,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   165,
       0,     0,    14,    15,     0,    16,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,     0,    31,   166,   167,     0,   905,    36,
       0,    37,     0,     0,    39,     0,     0,     0,    41,    42,
       0,     0,    44,    45,     0,    46,    47,     0,    48,    49,
       0,     0,     0,     0,     0,    52,     0,    54,     0,    56,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,    70,   169,     0,
      72,     0,     0,    74,     0,     0,     0,    78,     0,     0,
      80,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,   256,     0,     0,     0,    96,     0,   170,     0,    98,
       0,   171,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   165,     0,     0,    14,    15,     0,    16,     0,     0,
       0,     0,    20,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,     0,    31,   166,   167,     0,
     168,    36,     0,    37,     0,     0,    39,     0,     0,     0,
      41,    42,     0,     0,    44,    45,     0,    46,    47,     0,
      48,    49,     0,     0,     0,     0,     0,    52,     0,    54,
       0,    56,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,    70,
     169,     0,    72,     0,     0,    74,     0,     0,     0,    78,
       0,     0,    80,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,     0,     0,     0,     0,    96,     0,   170,
       0,    98,     0,   171,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   165,     0,     0,    14,    15,     0,    16,
       0,     0,     0,     0,    20,     0,     0,     0,     0,     0,
       0,     0,    27,     0,     0,     0,     0,     0,    31,   166,
     167,     0,   168,    36,     0,    37,     0,     0,    39,     0,
       0,     0,    41,    42,     0,     0,    44,    45,     0,    46,
      47,     0,    48,    49,     0,     0,     0,     0,     0,    52,
       0,    54,     0,    56,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,    70,   169,     0,    72,     0,     0,    74,     0,     0,
       0,    78,     0,     0,    80,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,     0,     0,     0,     0,    96,
       0,    97,     0,    98,     0,   171,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   165,     0,     0,    14,    15,
       0,    16,     0,     0,     0,     0,    20,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,     0,     0,     0,
      31,   166,   167,     0,   168,    36,     0,    37,     0,     0,
      39,     0,     0,     0,    41,    42,     0,     0,    44,    45,
       0,    46,    47,     0,    48,    49,     0,     0,     0,     0,
       0,    52,     0,    54,     0,    56,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,    70,   169,     0,    72,     0,     0,    74,
       0,     0,     0,    78,     0,     0,    80,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,     0,     0,     0,
       0,   575,     0,   170,     0,    98,     0,   171,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   165,     0,     0,
      14,    15,     0,    16,     0,     0,     0,     0,    20,     0,
       0,     0,     0,     0,     0,     0,   602,     0,     0,     0,
       0,     0,    31,   166,   167,     0,   168,    36,     0,   603,
       0,     0,    39,     0,     0,     0,    41,    42,     0,     0,
      44,    45,     0,    46,    47,     0,    48,    49,     0,     0,
       0,     0,     0,    52,     0,    54,     0,    56,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,    66,   604,    68,    69,    70,   605,     0,    72,     0,
       0,    74,     0,     0,     0,    78,     0,     0,    80,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,     0,
       0,     0,     0,    96,     0,   170,     0,    98,     0,   606,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   165,
       0,     0,    14,    15,     0,    16,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,     0,    31,   166,   167,     0,   168,    36,
       0,    37,     0,     0,    39,     0,     0,     0,    41,    42,
       0,     0,    44,    45,     0,    46,    47,     0,    48,    49,
       0,     0,     0,     0,     0,    52,     0,    54,     0,    56,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,    70,   169,     0,
      72,     0,     0,    74,     0,     0,     0,    78,     0,     0,
      80,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,     0,     0,     0,     0,    96,     0,   170,     0,    98,
       0,   606,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   165,     0,     0,    14,    15,     0,    16,     0,     0,
       0,     0,    20,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,     0,    31,   166,   167,     0,
     676,    36,     0,    37,     0,     0,    39,     0,     0,     0,
      41,    42,     0,     0,    44,    45,     0,    46,    47,     0,
      48,    49,     0,     0,     0,     0,     0,    52,     0,    54,
       0,    56,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,    70,
     169,     0,    72,     0,     0,    74,     0,     0,     0,    78,
       0,     0,    80,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,     0,     0,     0,     0,    96,     0,   170,
       0,    98,     0,   171,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   165,     0,     0,    14,    15,     0,    16,
       0,     0,     0,     0,    20,     0,     0,     0,     0,     0,
       0,     0,    27,     0,     0,     0,     0,     0,    31,   166,
     167,     0,   679,    36,     0,    37,     0,     0,    39,     0,
       0,     0,    41,    42,     0,     0,    44,    45,     0,    46,
      47,     0,    48,    49,     0,     0,     0,     0,     0,    52,
       0,    54,     0,    56,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,    70,   169,     0,    72,     0,     0,    74,     0,     0,
       0,    78,     0,     0,    80,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,     0,     0,     0,     0,    96,
       0,   170,     0,    98,     0,   171,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   165,     0,     0,    14,    15,
       0,    16,     0,     0,     0,     0,    20,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,     0,     0,     0,
      31,   166,   167,     0,   899,    36,     0,    37,     0,     0,
      39,     0,     0,     0,    41,    42,     0,     0,    44,    45,
       0,    46,    47,     0,    48,    49,     0,     0,     0,     0,
       0,    52,     0,    54,     0,    56,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,    70,   169,     0,    72,     0,     0,    74,
       0,     0,     0,    78,     0,     0,    80,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,     0,     0,     0,
       0,    96,     0,   170,     0,    98,     0,   171,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   165,     0,     0,
      14,    15,     0,    16,     0,     0,     0,     0,    20,     0,
       0,     0,     0,     0,     0,     0,    27,     0,     0,     0,
       0,     0,    31,   166,   167,     0,   900,    36,     0,    37,
       0,     0,    39,     0,     0,     0,    41,    42,     0,     0,
      44,    45,     0,    46,    47,     0,    48,    49,     0,     0,
       0,     0,     0,    52,     0,    54,     0,    56,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,    70,   169,     0,    72,     0,
       0,    74,     0,     0,     0,    78,     0,     0,    80,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,     0,
       0,     0,     0,    96,     0,   170,     0,    98,     0,   171,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   165,
       0,     0,    14,    15,     0,    16,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,     0,    31,   166,   167,     0,   902,    36,
       0,    37,     0,     0,    39,     0,     0,     0,    41,    42,
       0,     0,    44,    45,     0,    46,    47,     0,    48,    49,
       0,     0,     0,     0,     0,    52,     0,    54,     0,    56,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,    70,   169,     0,
      72,     0,     0,    74,     0,     0,     0,    78,     0,     0,
      80,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,     0,     0,     0,     0,    96,     0,   170,     0,    98,
       0,   171,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   165,     0,     0,    14,    15,     0,    16,     0,     0,
       0,     0,    20,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,     0,    31,   166,   167,     0,
     903,    36,     0,    37,     0,     0,    39,     0,     0,     0,
      41,    42,     0,     0,    44,    45,     0,    46,    47,     0,
      48,    49,     0,     0,     0,     0,     0,    52,     0,    54,
       0,    56,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,    70,
     169,     0,    72,     0,     0,    74,     0,     0,     0,    78,
       0,     0,    80,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,     0,     0,     0,     0,    96,     0,   170,
       0,    98,     0,   171,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   165,     0,     0,    14,    15,     0,    16,
       0,     0,     0,     0,    20,     0,     0,     0,     0,     0,
       0,     0,    27,     0,     0,     0,     0,     0,    31,   166,
     167,     0,   904,    36,     0,    37,     0,     0,    39,     0,
       0,     0,    41,    42,     0,     0,    44,    45,     0,    46,
      47,     0,    48,    49,     0,     0,     0,     0,     0,    52,
       0,    54,     0,    56,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,    70,   169,     0,    72,     0,     0,    74,     0,     0,
       0,    78,     0,     0,    80,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,     0,     0,     0,     0,    96,
       0,   170,     0,    98,     0,   171,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   165,     0,     0,    14,    15,
       0,    16,     0,     0,     0,     0,    20,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,     0,     0,     0,
      31,   166,   167,     0,   905,    36,     0,    37,     0,     0,
      39,     0,     0,     0,    41,    42,     0,     0,    44,    45,
       0,    46,    47,     0,    48,    49,     0,     0,     0,     0,
       0,    52,     0,    54,     0,    56,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,    70,   169,     0,    72,     0,     0,    74,
       0,     0,     0,    78,     0,     0,    80,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,     0,     0,     0,
       0,    96,     0,   170,     0,    98,     0,   171,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   165,     0,     0,
      14,    15,     0,    16,     0,     0,     0,     0,    20,     0,
       0,     0,     0,     0,     0,     0,   602,     0,     0,     0,
       0,     0,    31,   166,   167,     0,   168,    36,     0,   603,
       0,     0,    39,     0,     0,     0,    41,    42,     0,     0,
      44,    45,     0,    46,    47,     0,    48,    49,     0,     0,
       0,     0,     0,    52,     0,    54,     0,    56,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,    66,   604,    68,    69,    70,   605,     0,    72,     0,
       0,    74,     0,     0,     0,    78,     0,     0,    80,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,     0,
       0,     0,     0,    96,     0,   170,     0,    98,     0,   987,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   165,
       0,     0,    14,    15,     0,    16,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,     0,    31,   166,   167,     0,   168,    36,
       0,    37,     0,     0,    39,     0,     0,     0,    41,    42,
       0,     0,    44,    45,     0,    46,    47,     0,    48,    49,
       0,     0,     0,     0,     0,    52,     0,    54,     0,    56,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,    70,   169,     0,
      72,     0,     0,    74,     0,     0,     0,    78,     0,     0,
      80,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,     0,     0,     0,     0,    96,     0,     0,     0,    98,
       0,   987,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   165,     0,     0,    14,    15,     0,    16,     0,     0,
       0,     0,    20,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,     0,    31,   166,   167,     0,
     168,    36,     0,    37,     0,     0,    39,     0,     0,     0,
      41,    42,     0,     0,    44,    45,     0,    46,    47,     0,
      48,    49,     0,     0,     0,     0,     0,    52,     0,    54,
       0,    56,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,    70,
     169,     0,    72,     0,     0,    74,     0,     0,     0,    78,
       0,     0,    80,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   165,     0,     0,    14,    15,    92,    16,
       0,     0,    94,     0,    20,     0,     0,    96,     0,   170,
       0,    98,    27,   171,     0,     0,     0,     0,    31,   166,
     167,     0,   168,    36,     0,    37,     0,     0,    39,     0,
       0,     0,    41,    42,     0,     0,    44,    45,     0,    46,
      47,     0,    48,    49,     0,     0,     0,     0,     0,    52,
       0,    54,     0,    56,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,    70,   169,     0,    72,     0,     0,    74,     0,     0,
       0,    78,     0,     0,    80,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,  -393,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,  -393,     0,     0,
       0,  -393,     0,     0,     0,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   256,   165,     0,     0,    14,
      15,   170,    16,    98,  -393,   920,     0,    20,     0,     0,
       0,     0,     0,     0,     0,    27,     0,     0,     0,     0,
       0,    31,   166,   167,     0,   168,    36,     0,    37,     0,
       0,    39,     0,     0,     0,    41,    42,  -375,     0,    44,
      45,     0,    46,    47,     0,    48,    49,     0,     0,     0,
       0,     0,    52,     0,    54,     0,    56,     0,     0,     0,
       0,    59,     0,     0,     0,     0,  -375,     0,     0,     0,
      66,    67,    68,    69,    70,   169,     0,    72,     0,  -375,
      74,     0,     0,     0,    78,     0,     0,    80,     0,     0,
      82,     0,  -375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   165,     0,     0,    14,
      15,     0,    16,     0,     0,     0,     0,    20,     0,  -375,
       0,     0,     0,     0,  -375,    27,    98,     0,   987,     0,
       0,    31,   166,   167,     0,   168,    36,     0,    37,     0,
       0,    39,     0,     0,     0,    41,    42,     0,     0,    44,
      45,     0,    46,    47,     0,    48,    49,     0,     0,     0,
       0,     0,    52,     0,    54,     0,    56,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,    70,   169,     0,    72,     0,     0,
      74,     0,     0,     0,    78,     0,     0,    80,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   165,     0,     0,    14,
      15,     0,    16,     0,     0,     0,     0,    20,     0,     0,
       0,     0,     0,     0,   170,    27,    98,     0,   171,     0,
       0,    31,   166,   167,     0,   168,    36,     0,    37,     0,
       0,    39,     0,     0,     0,    41,    42,     0,     0,    44,
      45,     0,    46,    47,     0,    48,    49,     0,     0,     0,
       0,     0,    52,     0,    54,     0,    56,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,    70,   169,     0,    72,     0,     0,
      74,     0,     0,     0,    78,     0,     0,    80,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   165,     0,     0,    14,
      15,     0,    16,     0,     0,     0,     0,    20,     0,     0,
       0,     0,     0,     0,   170,    27,    98,     0,   606,     0,
       0,    31,   166,   167,     0,   168,    36,     0,    37,     0,
       0,    39,     0,     0,     0,    41,    42,     0,     0,    44,
      45,     0,    46,    47,     0,    48,    49,     0,     0,     0,
       0,     0,    52,     0,    54,     0,    56,     0,     0,     0,
       0,    59,     0,     0,     0,     0,   211,     0,     0,     0,
      66,    67,    68,    69,    70,   169,     0,    72,   212,   213,
      74,   214,     0,     0,    78,     0,   215,    80,     0,     0,
      82,     0,     0,     0,   216,     0,     0,     0,     0,     0,
     217,     0,     0,     0,     0,   218,     0,   219,     0,     0,
     220,     0,     0,     0,     0,     0,    88,     0,   221,     0,
       0,     0,     0,     0,   222,   223,     0,     0,     0,     0,
       0,   224,     0,     0,     0,     0,     0,     0,     0,     0,
     225,     0,     0,     0,     0,   211,    98,     0,   171,   226,
     227,     0,   228,     0,   229,     0,   230,   212,   213,   231,
     214,     0,     0,   232,     0,   215,   233,     0,     0,   234,
       0,     0,     0,   216,     0,     0,     0,     0,     0,   217,
       0,     0,     0,     0,   218,     0,   219,     0,     0,   220,
       0,     0,     0,     0,     0,     0,     0,   221,     0,     0,
       0,     0,   211,   222,   223,     0,     0,     0,     0,     0,
     224,     0,     0,     0,   212,   213,     0,   214,     0,   225,
       0,     0,   215,   379,     0,   742,     0,     0,   226,   227,
     216,   228,     0,   229,     0,   230,   217,     0,   231,     0,
       0,   218,   232,   219,     0,   233,   220,     0,   234,     0,
       0,     0,     0,     0,   221,     0,     0,     0,     0,     0,
     222,   223,     0,     0,     0,     0,     0,   224,     0,     0,
       0,     0,     0,     0,     0,     0,   225,     0,     0,     0,
       0,     0,     0,     0,     0,   226,   227,     0,   228,     0,
     229,     0,   230,     0,   948,   231,     0,     0,     0,   232,
       0,   766,   233,     0,   238,   234,   212,   213,   768,   214,
       0,     0,     0,     0,   215,     0,     0,     0,     0,     0,
       0,     0,   216,     0,     0,     0,     0,     0,   771,     0,
       0,     0,     0,   218,     0,   219,     0,     0,   220,     0,
     774,     0,     0,     0,     0,     0,   221,     0,     0,     0,
       0,     0,   776,   223,     0,     0,     0,   322,     0,   224,
       0,   722,     0,     0,   323,     0,     0,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   324,   226,   227,     0,
     228,     0,   229,     0,   949,     0,     0,   781,     0,     0,
       0,   232,     0,     0,   233,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   352,   353,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   324,   364,
     365,   784,   785,   786,   787,   788,   325,     0,   789,     0,
     326,     0,   790,   791,   792,   793,   794,   795,   796,   797,
     798,   799,   800,     0,   801,   322,     0,   802,   803,   804,
     805,     0,   323,     0,     0,     0,     0,   404,     0,     0,
       0,   327,     0,     0,   324,     0,     0,     0,   325,     0,
       0,     0,   326,     0,     0,     0,     0,   328,   329,     0,
     330,   331,   332,     0,   333,   334,   335,     0,   336,   337,
     338,   339,   340,     0,   341,   342,   343,   344,   345,   346,
     347,     0,   322,   348,   349,   350,     0,     0,     0,   323,
       0,     0,   750,     0,   325,     0,     0,     0,   326,   328,
     329,   324,   330,   331,   332,     0,   333,   334,   335,     0,
     336,   337,   338,   339,     0,     0,   341,   342,   343,   344,
     345,     0,   347,   322,     0,   348,   349,   350,     0,   327,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,     0,     0,   328,   329,     0,   330,   331,
     332,   325,   333,   334,   335,   326,   336,   337,   338,   339,
     340,     0,   341,   342,   343,   344,   345,   346,   347,   322,
       0,   348,   349,   350,     0,     0,   323,     0,     0,     0,
     826,     0,     0,     0,     0,     0,   327,     0,   324,     0,
       0,     0,   325,     0,     0,     0,   326,     0,     0,     0,
       0,     0,   328,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,   336,   337,   338,   339,   340,     0,   341,
     342,   343,   344,   345,   346,   347,   322,   327,   348,   349,
     350,     0,     0,   323,     0,     0,     0,   983,   325,     0,
       0,     0,   326,   328,   329,   324,   330,   331,   332,     0,
     333,   334,   335,     0,   336,   337,   338,   339,   340,     0,
     341,   342,   343,   344,   345,   346,   347,     0,     0,   348,
     349,   350,     0,   327,     0,     0,     0,     0,  1050,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   328,
     329,     0,   330,   331,   332,   325,   333,   334,   335,   326,
     336,   337,   338,   339,   340,     0,   341,   342,   343,   344,
     345,   346,   347,   322,     0,   348,   349,   350,     0,     0,
     323,     0,   627,     0,     0,     0,     0,     0,     0,     0,
     327,     0,   324,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   328,   329,     0,   330,
     331,   332,     0,   333,   334,   335,     0,   336,   337,   338,
     339,   340,     0,   341,   342,   343,   344,   345,   346,   347,
       0,     0,   348,   349,   350,     0,     0,     0,     0,   947,
       0,     0,   325,     0,     0,     0,   326,     0,     0,   322,
       0,     0,     0,     0,     0,   415,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,     0,   327,   351,   352,
     353,   354,   355,   356,     0,     0,   359,   360,   361,   362,
       0,   364,   365,   328,   329,     0,   330,   331,   332,     0,
     333,   334,   335,     0,   336,   337,   338,   339,   340,     0,
     341,   342,   343,   344,   345,   346,   347,     0,   325,   348,
     349,   350,   326,   379,     0,   322,     0,     0,     0,     0,
       0,   831,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,     0,     0,     0,     0,     0,
       0,     0,     0,   327,   351,   352,   353,   354,   355,   356,
       0,     0,   359,   360,   361,   362,     0,   364,   365,   328,
     329,     0,   330,   331,   332,     0,   333,   334,   335,     0,
     336,   337,   338,   339,   340,     0,   341,   342,   343,   344,
     345,   346,   347,     0,   325,   348,   349,   350,   326,   379,
       0,   322,     0,     0,     0,     0,     0,   838,   323,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,     0,     0,     0,     0,     0,     0,     0,     0,   327,
     351,   352,   353,   354,   355,   356,     0,     0,   359,   360,
     361,   362,     0,   364,   365,   328,   329,     0,   330,   331,
     332,     0,   333,   334,   335,     0,   336,   337,   338,   339,
     340,     0,   341,   342,   343,   344,   345,   346,   347,     0,
     325,   348,   349,   350,   326,   379,     0,   322,     0,     0,
       0,     0,     0,   960,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,     0,     0,     0,
       0,     0,     0,     0,     0,   327,   351,   352,   353,   354,
     355,   356,     0,     0,   359,   360,   361,   362,     0,   364,
     365,   328,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,   336,   337,   338,   339,   340,     0,   341,   342,
     343,   344,   345,   346,   347,     0,   325,   348,   349,   350,
     326,   379,     0,   322,     0,     0,     0,     0,     0,   961,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,     0,     0,     0,     0,     0,     0,     0,
       0,   327,   351,   352,   353,   354,   355,   356,     0,     0,
     359,   360,   361,   362,     0,   364,   365,   328,   329,     0,
     330,   331,   332,     0,   333,   334,   335,     0,   336,   337,
     338,   339,   340,     0,   341,   342,   343,   344,   345,   346,
     347,     0,   325,   348,   349,   350,   326,   379,     0,   322,
       0,     0,     0,     0,     0,   962,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,     0,   327,   351,   352,
     353,   354,   355,   356,     0,     0,   359,   360,   361,   362,
       0,   364,   365,   328,   329,     0,   330,   331,   332,     0,
     333,   334,   335,     0,   336,   337,   338,   339,   340,     0,
     341,   342,   343,   344,   345,   346,   347,     0,   325,   348,
     349,   350,   326,   379,     0,   322,     0,     0,     0,     0,
       0,   963,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,     0,     0,     0,     0,     0,
       0,     0,     0,   327,   351,   352,   353,   354,   355,   356,
       0,     0,   359,   360,   361,   362,     0,   364,   365,   328,
     329,     0,   330,   331,   332,     0,   333,   334,   335,     0,
     336,   337,   338,   339,   340,     0,   341,   342,   343,   344,
     345,   346,   347,     0,   325,   348,   349,   350,   326,   379,
       0,   322,     0,     0,     0,     0,     0,   985,   323,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,     0,     0,     0,     0,     0,     0,     0,     0,   327,
     351,   352,   353,   354,   355,   356,     0,     0,   359,   360,
     361,   362,     0,   364,   365,   328,   329,     0,   330,   331,
     332,     0,   333,   334,   335,     0,   336,   337,   338,   339,
     340,     0,   341,   342,   343,   344,   345,   346,   347,     0,
     325,   348,   349,   350,   326,   379,     0,     0,     0,     0,
       0,     0,     0,   986,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   327,   351,   352,   353,   354,
     355,   356,     0,     0,   359,   360,   361,   362,     0,   364,
     365,   328,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,   336,   337,   338,   339,   340,     0,   341,   342,
     343,   344,   345,   346,   347,   322,     0,   348,   349,   350,
       0,   379,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,   407,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   322,   408,     0,
       0,     0,     0,     0,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,     0,     0,     0,   325,     0,     0,     0,   326,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   322,     0,     0,     0,     0,     0,
       0,   323,     0,     0,     0,   404,   325,     0,     0,   327,
     326,     0,     0,   324,   705,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   328,   329,     0,   330,   331,
     332,     0,   333,   334,   335,     0,   336,   337,   338,   339,
     340,   327,   341,   342,   343,   344,   345,   346,   347,     0,
       0,   348,   349,   350,     0,   379,     0,   328,   329,     0,
     330,   331,   332,   325,   333,   334,   335,   326,   336,   337,
     338,   339,   340,     0,   341,   342,   343,   344,   345,   346,
     347,   322,     0,   348,   349,   350,     0,   379,   323,     0,
       0,     0,     0,     0,   404,     0,     0,     0,   327,     0,
     324,   242,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   328,   329,     0,   330,   331,   332,
       0,   333,   334,   335,     0,   336,   337,   338,   339,   340,
       0,   341,   342,   343,   344,   345,   346,   347,   322,     0,
     348,   349,   350,     0,   379,   323,     0,     0,     0,     0,
     325,     0,     0,     0,   326,     0,     0,   324,   697,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     322,     0,     0,     0,     0,     0,     0,   323,     0,     0,
       0,     0,     0,     0,     0,   327,     0,     0,     0,   324,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   328,   329,     0,   330,   331,   332,   325,   333,   334,
     335,   326,   336,   337,   338,   339,   340,     0,   341,   342,
     343,   344,   345,   346,   347,     0,     0,   348,   349,   350,
       0,   379,     0,     0,     0,     0,     0,     0,     0,   325,
       0,     0,   327,   326,     0,     0,     0,     0,     0,     0,
       0,     0,   715,     0,     0,     0,     0,     0,   328,   329,
       0,   330,   331,   332,     0,   333,   334,   335,     0,   336,
     337,   338,   339,   340,   327,   341,   342,   343,   344,   345,
     346,   347,     0,     0,   348,   349,   350,     0,   379,     0,
     328,   329,     0,   330,   331,   332,     0,   333,   334,   335,
       0,   336,   337,   338,   339,   340,     0,   341,   342,   343,
     344,   345,   346,   347,   322,     0,   348,   349,   350,     0,
     379,   323,     0,   211,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,     0,   212,   213,     0,   214,     0,
       0,     0,     0,   215,     0,     0,   322,     0,     0,     0,
       0,   216,     0,   323,     0,     0,     0,   217,     0,     0,
       0,     0,   218,     0,   219,   324,     0,   220,     0,     0,
     412,     0,     0,     0,     0,   221,     0,     0,     0,     0,
       0,   222,   223,   325,     0,     0,     0,   326,   224,     0,
       0,     0,     0,     0,     0,     0,     0,   225,     0,     0,
     413,     0,     0,   322,     0,     0,   226,   227,     0,   228,
     323,   229,     0,   230,     0,   325,   231,     0,   327,   326,
     232,     0,   324,   233,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   328,   329,   381,   330,   331,   332,
       0,   333,   334,   335,     0,   336,   337,   338,   339,   340,
     327,   341,   342,   343,   344,   345,   346,   347,     0,     0,
     348,   349,   350,     0,   442,     0,   328,   329,     0,   330,
     331,   332,   325,   333,   334,   335,   326,   336,   337,   338,
     339,   340,     0,   341,   342,   343,   344,   345,   346,   347,
     322,   414,   348,   349,   350,     0,     0,   323,     0,     0,
       0,     0,     0,   160,     0,     0,     0,   327,     0,   324,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   328,   329,     0,   330,   331,   332,     0,
     333,   334,   335,     0,   336,   337,   338,   339,   340,     0,
     341,   342,   343,   344,   345,   346,   347,   322,     0,   348,
     349,   350,     0,     0,   323,     0,     0,     0,     0,   325,
       0,     0,     0,   326,     0,     0,   324,   542,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   322,
     543,     0,     0,     0,     0,     0,   323,     0,     0,     0,
       0,     0,     0,     0,   327,     0,     0,     0,   324,   544,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     328,   329,   545,   330,   331,   332,   325,   333,   334,   335,
     326,   336,   337,   338,   339,   340,     0,   341,   342,   343,
     344,   345,   346,   347,     0,   446,   348,   349,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   325,     0,
       0,   327,   326,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   328,   329,     0,
     330,   331,   332,     0,   333,   334,   335,     0,   336,   337,
     338,   339,   340,   327,   341,   342,   343,   344,   345,   346,
     347,     0,     0,   348,   349,   350,     0,     0,     0,   328,
     329,     0,   330,   331,   332,     0,   333,   334,   335,     0,
     336,   337,   338,   339,   340,     0,   341,   342,   343,   344,
     345,   346,   347,   322,     0,   348,   349,   350,     0,     0,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,     0,     0,     0,     0,   647,     0,     0,
       0,     0,     0,     0,     0,   322,     0,     0,     0,     0,
       0,     0,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,     0,     0,   648,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   325,     0,     0,     0,   326,     0,     0,     0,
       0,     0,     0,     0,   322,     0,     0,     0,     0,     0,
       0,   323,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,   325,     0,     0,   327,   326,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   328,   329,     0,   330,   331,   332,     0,
     333,   334,   335,     0,   336,   337,   338,   339,   340,   327,
     341,   342,   343,   344,   345,   346,   347,     0,     0,   348,
     349,   350,     0,   325,     0,   328,   329,   326,   330,   331,
     332,     0,   333,   334,   335,     0,   336,   337,   338,   339,
     340,     0,   341,   342,   343,   344,   345,   346,   347,   322,
     448,   348,   349,   350,     0,     0,   323,     0,   327,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,     0,
       0,     0,     0,     0,   328,   329,     0,   330,   331,   332,
       0,   333,   334,   335,     0,   336,   337,   338,   339,   340,
       0,   341,   342,   343,   344,   345,   346,   347,   322,   450,
     348,   349,   350,     0,     0,   323,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   324,   325,     0,
       0,     0,   326,     0,     0,     0,     0,     0,     0,     0,
     322,     0,     0,     0,     0,     0,     0,   323,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
       0,     0,     0,   327,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,     0,   328,
     329,   326,   330,   331,   332,     0,   333,   334,   335,     0,
     336,   337,   338,   339,   340,     0,   341,   342,   343,   344,
     345,   346,   347,     0,   455,   348,   349,   350,     0,   325,
       0,   472,   327,   326,     0,     0,     0,     0,     0,     0,
       0,     0,   546,     0,     0,     0,     0,     0,   328,   329,
       0,   330,   331,   332,     0,   333,   334,   335,     0,   336,
     337,   338,   339,   340,   327,   341,   342,   343,   344,   345,
     346,   347,     0,     0,   348,   349,   350,     0,     0,     0,
     328,   329,     0,   330,   331,   332,     0,   333,   334,   335,
       0,   336,   337,   338,   339,   340,     0,   341,   342,   343,
     344,   345,   346,   347,   322,     0,   348,   349,   350,     0,
       0,   323,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   322,
       0,     0,     0,     0,     0,     0,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,     0,
       0,     0,     0,   325,     0,     0,     0,   326,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     322,     0,     0,     0,     0,     0,     0,   323,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   327,   324,
       0,     0,     0,     0,     0,     0,     0,     0,   325,     0,
       0,     0,   326,     0,   328,   329,     0,   330,   331,   332,
       0,   333,   334,   335,     0,   336,   337,   338,   339,   340,
    -494,   341,   342,   343,   344,   345,   346,   347,     0,   160,
     348,   349,   350,   327,     0,     0,     0,     0,     0,   325,
       0,     0,     0,   326,     0,     0,     0,     0,     0,   328,
     329,     0,   330,   331,   332,     0,   333,   334,   335,     0,
     336,   337,   338,   339,   340,     0,   341,   342,   343,   344,
     345,   346,   347,     0,   327,   348,   349,   350,     0,     0,
       0,   322,   710,     0,     0,     0,     0,     0,   323,     0,
     328,   329,     0,   330,   331,   332,     0,   333,   334,   335,
     324,   336,   337,   338,   339,   340,     0,   341,   342,   343,
     344,   345,   346,   347,     0,   686,   348,   349,   350,     0,
       0,   322,     0,     0,     0,     0,     0,     0,   323,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,     0,     0,   713,     0,     0,     0,     0,     0,     0,
     325,     0,     0,     0,   326,     0,     0,     0,     0,     0,
       0,     0,   322,     0,     0,     0,     0,     0,     0,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,     0,     0,     0,   327,     0,     0,     0,     0,
     325,     0,     0,     0,   326,     0,     0,     0,     0,     0,
       0,   328,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,   336,   337,   338,   339,   340,     0,   341,   342,
     343,   344,   345,   346,   347,   327,     0,   348,   349,   350,
       0,   325,     0,     0,     0,   326,     0,     0,     0,     0,
       0,   328,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,   336,   337,   338,   339,   340,     0,   341,   342,
     343,   344,   345,   346,   347,   753,   327,   348,   349,   350,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   328,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,   336,   337,   338,   339,   340,     0,   341,
     342,   343,   344,   345,   346,   347,   322,     0,   348,   349,
     350,     0,     0,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   821,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,   323,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   324,   824,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   325,     0,     0,     0,   326,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,     0,     0,
     327,   326,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   328,   329,     0,   330,
     331,   332,     0,   333,   334,   335,     0,   336,   337,   338,
     339,   340,   327,   341,   342,   343,   344,   345,   346,   347,
       0,     0,   348,   349,   350,     0,     0,     0,   328,   329,
       0,   330,   331,   332,     0,   333,   334,   335,     0,   336,
     337,   338,   339,   340,     0,   341,   342,   343,   344,   345,
     346,   347,   322,     0,   348,   349,   350,     0,     0,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   322,     0,     0,     0,     0,     0,
       0,   323,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   325,     0,     0,     0,   326,     0,     0,     0,     0,
       0,     0,     0,     0,   898,     0,     0,     0,     0,     0,
       0,   322,   915,     0,     0,     0,     0,     0,   323,     0,
       0,     0,     0,   325,     0,     0,   327,   326,     0,     0,
     324,     0,     0,     0,     0,     0,   901,     0,     0,     0,
       0,     0,   328,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,   336,   337,   338,   339,   340,   327,   341,
     342,   343,   344,   345,   346,   347,     0,     0,   348,   349,
     350,     0,     0,     0,   328,   329,     0,   330,   331,   332,
     325,   333,   334,   335,   326,   336,   337,   338,   339,   340,
       0,   341,   342,   343,   344,   345,   346,   347,   322,     0,
     348,   349,   350,     0,     0,   323,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   327,     0,   324,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   328,   329,     0,   330,   331,   332,     0,   333,   334,
     335,     0,   336,   337,   338,   339,   340,     0,   341,   342,
     343,   344,   345,   346,   347,     0,     0,   348,   349,   350,
       0,     0,     0,     0,     0,     0,     0,   325,     0,     0,
       0,   326,     0,     0,   322,     0,     0,     0,     0,     0,
     996,   323,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,     0,   327,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   328,   329,
       0,   330,   331,   332,     0,   333,   334,   335,     0,   336,
     337,   338,   339,   340,     0,   341,   342,   343,   344,   345,
     346,   347,     0,   325,   348,   349,   350,   326,     0,     0,
     322,     0,     0,     0,     0,     0,   997,   323,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
       0,     0,     0,     0,     0,     0,     0,     0,   327,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   328,   329,     0,   330,   331,   332,
       0,   333,   334,   335,     0,   336,   337,   338,   339,   340,
       0,   341,   342,   343,   344,   345,   346,   347,     0,   325,
     348,   349,   350,   326,     0,     0,   322,     0,     0,     0,
       0,     0,   998,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,     0,     0,     0,     0,
       0,     0,     0,     0,   327,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     328,   329,     0,   330,   331,   332,     0,   333,   334,   335,
       0,   336,   337,   338,   339,   340,     0,   341,   342,   343,
     344,   345,   346,   347,     0,   325,   348,   349,   350,   326,
       0,     0,   322,     0,     0,     0,     0,     0,   999,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,     0,     0,     0,     0,     0,     0,     0,     0,
     327,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   328,   329,     0,   330,
     331,   332,     0,   333,   334,   335,     0,   336,   337,   338,
     339,   340,     0,   341,   342,   343,   344,   345,   346,   347,
       0,   325,   348,   349,   350,   326,     0,     0,   322,     0,
       0,     0,     0,     0,  1000,   323,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   324,     0,     0,
       0,     0,     0,     0,     0,     0,   327,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   328,   329,     0,   330,   331,   332,     0,   333,
     334,   335,     0,   336,   337,   338,   339,   340,     0,   341,
     342,   343,   344,   345,   346,   347,     0,   325,   348,   349,
     350,   326,     0,     0,   322,     0,     0,     0,     0,     0,
    1001,   323,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,     0,     0,     0,     0,     0,     0,
       0,     0,   327,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1023,   328,   329,
       0,   330,   331,   332,     0,   333,   334,   335,     0,   336,
     337,   338,   339,   340,     0,   341,   342,   343,   344,   345,
     346,   347,     0,   325,   348,   349,   350,   326,     0,     0,
     322,     0,     0,     0,     0,     0,     0,   323,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
       0,     0,     0,     0,     0,     0,     0,     0,   327,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   328,   329,     0,   330,   331,   332,
       0,   333,   334,   335,     0,   336,   337,   338,   339,   340,
       0,   341,   342,   343,   344,   345,   346,   347,     0,   325,
     348,   349,   350,   326,     0,     0,   322,     0,     0,     0,
       0,     0,     0,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,     0,     0,     0,     0,
       0,     0,     0,     0,   327,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     328,   329,     0,   330,   331,   332,     0,   333,   334,   335,
       0,   336,   337,   338,   339,   340,   324,   341,   342,   343,
     344,   345,   346,   347,     0,   674,   348,   349,   350,   326,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     327,     0,     0,     0,     0,     0,   325,     0,     0,     0,
     326,     0,     0,     0,     0,     0,   328,   329,     0,   330,
     331,   332,     0,   333,   334,   335,     0,   336,   337,   338,
     339,   340,     0,   341,   342,   343,   344,   345,   346,   347,
       0,   327,   348,   349,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   328,   329,     0,
     330,   331,   332,     0,   333,   334,   335,     0,   336,   337,
     338,   339,     0,     0,   341,   342,   343,   344,   345,   346,
     347,     0,     0,   348,   349,   350,   651,     0,   211,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     212,   213,     0,   214,     0,     0,     0,     0,   215,     0,
       0,     0,     0,     0,     0,     0,   216,     0,     0,     0,
       0,     0,   217,     0,     0,     0,     0,   218,     0,   219,
       0,     0,   220,     0,     0,     0,     0,     0,     0,     0,
     221,     0,     0,     0,     0,     0,   222,   223,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,     0,
       0,     0,   225,     0,     0,     0,     0,     0,     0,     0,
       0,   226,   227,     0,   228,     0,   229,     0,   230,     0,
       0,   231,     0,     0,     0,   232,     0,     0,   233,     0,
       0,   234
};

static const yytype_int16 yycheck[] =
{
      11,     2,   183,   145,    15,    57,    58,   420,    19,   372,
     204,   568,    62,   451,    25,   196,   413,    21,    29,    30,
     731,    32,    33,    34,    35,   660,   132,    29,   930,     1,
      43,    40,    43,    18,    45,    46,   811,   812,     1,    50,
      30,    52,   423,    54,     1,   110,    43,    30,    82,    99,
     210,    62,    63,    64,    65,    66,    67,     1,   601,   854,
      71,    43,    73,   155,    75,    76,   757,   110,    77,    80,
     968,     3,    83,    84,   851,    86,    30,    88,   184,    53,
      91,    92,    30,    94,    30,   136,    97,    98,    99,   191,
     136,    43,   252,    65,   130,   155,    53,     0,    80,    43,
     165,   161,   138,    98,    43,   816,    65,    48,   210,   303,
     304,   271,   163,   110,   671,  1013,   125,   110,   127,   165,
     110,   171,   165,   136,    43,    82,   160,    63,  1030,     1,
     673,    43,    43,    74,   108,   107,   278,   108,    95,   136,
     428,    80,   919,     3,   855,    82,  1013,   136,   107,   944,
     252,   108,   115,   136,   165,   166,   167,   168,   169,   170,
     171,    93,    98,   451,   110,   136,   155,   136,   165,   271,
     160,   115,   135,   136,   183,   950,   951,   160,   135,    20,
     191,    53,    20,  1050,   136,   728,   155,   196,     3,   161,
     199,   135,   136,   884,   198,   206,   191,   182,   155,   210,
     163,   205,   161,   160,   206,   568,   160,   918,   368,   369,
      82,   155,   160,   165,   160,   210,   907,   136,   155,   600,
     136,    80,    82,   160,   136,   136,    25,   115,   136,   238,
      32,    33,   334,    93,    79,   162,   108,    78,    83,   155,
      78,   252,   253,    53,   115,   115,   165,   256,   136,   260,
     261,   648,   140,   165,   165,   163,   967,   252,   136,   894,
     271,   102,   312,   135,   102,   136,   368,   369,   136,   140,
      69,   141,    82,   143,   144,   163,   271,   147,   148,   836,
     136,    80,   136,   161,   444,    95,  1028,   298,   160,   162,
     136,   397,   163,   155,   295,   163,    25,    99,   108,   161,
      99,   312,    75,    76,  1046,   161,   105,   720,   671,   163,
     321,   322,   323,   324,   325,   326,   327,   163,   329,   330,
     331,   332,   333,   115,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
      69,   722,   444,    53,   757,   155,   357,   358,   140,   399,
     160,    80,   363,    53,   136,   366,   367,   368,   369,   370,
     371,   372,   136,   130,    79,     7,   136,   136,    83,    25,
     381,   138,    82,   368,   369,   386,   105,   388,   572,   573,
     136,   163,    82,    43,    29,    95,   162,   396,   399,   163,
     401,   402,    25,   163,   163,    95,   407,   408,   108,   155,
      90,   412,   413,   162,   415,    65,   417,   411,   108,   136,
     136,   160,   104,    69,   423,    75,    76,    75,   136,   430,
     155,   357,   358,   861,    80,   155,   437,   363,   155,   155,
     366,   367,   482,   444,    79,   154,    69,   155,    83,   155,
     634,   635,   437,    99,   625,   155,   155,    80,   459,   444,
     160,   462,   462,   464,   464,   155,   558,   468,   469,   470,
     160,   472,   473,   162,   459,    25,    99,   462,   146,   464,
     479,   482,   155,   836,   469,   484,   889,   478,   162,   162,
     164,   164,   155,    43,   135,   130,    46,   138,     3,   138,
     135,   142,   137,   138,   907,   160,   162,   142,   164,   162,
     162,   164,   164,    18,    29,   150,    66,   155,   155,    69,
     109,    71,   155,   158,   155,   115,    31,   155,   660,   155,
      80,   181,   162,   325,   326,   162,   462,   162,   464,     7,
      57,   542,   543,   544,   545,   546,   547,   548,   135,    99,
      55,   201,   202,   203,   204,   556,   160,   558,   155,   209,
      43,   562,   115,   564,    79,   163,   606,   568,    83,   570,
      53,   556,   136,   558,   549,   550,   854,   155,   370,   371,
      43,   162,    32,   861,   163,   136,   136,   163,    93,   381,
     115,   163,   163,    25,    99,   245,    19,   596,   248,    82,
     136,   600,   165,   604,   605,   606,   165,    24,    25,   610,
     402,    43,   160,   163,    46,   130,   408,   267,    35,    36,
     135,    43,   621,   138,   162,   108,   625,   142,    45,   155,
     556,    53,   115,   155,    66,   285,   155,    69,   165,    71,
     163,   155,   155,   155,   136,   155,   647,   648,    80,   155,
      67,    32,    69,   136,   163,   163,   136,   161,   161,   115,
      82,   660,    80,    80,   163,    30,   944,    99,    30,   165,
     671,   160,   155,   674,   675,   676,   468,   160,   679,    32,
     163,   974,   135,    80,   610,   161,   108,   165,   105,   135,
     968,   165,    43,   115,   695,   165,   697,     2,   738,   138,
     155,   702,    53,   155,   705,   983,    11,   708,   162,   710,
     138,   138,   713,   138,   136,   130,   130,   138,   138,    24,
     719,    26,   138,   722,  1017,  1018,  1019,  1020,  1021,  1022,
     138,    82,   382,   155,   138,  1013,   130,   738,   160,   138,
     138,   163,   130,   742,   130,   138,    32,   748,   161,   750,
    1028,   543,   753,   545,   163,    95,   406,   108,   759,   130,
     410,   760,   894,   163,   115,   309,    71,   130,  1046,   165,
     136,   648,  1050,   647,    23,   742,   560,     2,   279,   760,
     829,   894,   889,   720,   593,   136,    11,   786,  1030,   763,
     197,   967,    97,     2,   992,  1023,   101,  1016,    34,    24,
     558,    26,    11,    -1,   155,   806,    -1,   431,    -1,   160,
      -1,    25,   163,    -1,    -1,    24,    -1,    26,    -1,   820,
     821,    -1,   823,   824,   825,   826,    -1,    -1,    -1,    43,
     831,   832,    46,   834,    -1,   836,    -1,   838,   839,   881,
     882,   842,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      75,    76,    66,    -1,    -1,    69,   161,    71,    -1,   860,
     861,    -1,    71,    -1,    -1,   866,    80,   868,    -1,   854,
      -1,   872,    97,    -1,    -1,    -1,   101,    -1,    -1,   880,
     920,    -1,   674,   675,    -1,    99,    -1,    -1,    97,    -1,
      -1,    -1,   101,    -1,    -1,   894,    -1,   898,   899,   900,
     901,   902,   903,   904,   905,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   915,    -1,   566,   567,    -1,   920,
      -1,    -1,   572,   573,   925,    -1,    -1,    -1,   929,    -1,
     235,    -1,    -1,    -1,   584,    -1,   161,   242,    -1,    -1,
     925,    -1,    -1,    -1,    -1,    -1,    -1,   987,    -1,    -1,
      -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,   960,
     961,   962,   963,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   621,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   983,   968,   985,   986,   987,    -1,    -1,    -1,
     295,    -1,    -1,    -1,    -1,   996,   997,   998,   999,  1000,
    1001,    -1,    -1,    -1,   309,    -1,    -1,    -1,    -1,    -1,
     235,  1012,  1013,    -1,  1015,    -1,    -1,   242,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,  1013,    -1,
      -1,    -1,    -1,   242,   684,   685,    -1,    -1,    -1,    -1,
      -1,    -1,   692,    -1,   694,  1046,    -1,    -1,    -1,  1050,
      -1,   701,    -1,    -1,   704,    -1,    -1,   707,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1050,    -1,    20,    -1,    -1,
     295,    24,    25,    -1,   379,    -1,    -1,    -1,    -1,     2,
      33,    -1,    35,    36,   309,    -1,   295,    40,    11,    -1,
      -1,    -1,    45,    -1,    -1,   745,   401,    -1,    -1,    -1,
     309,    24,   407,    26,    -1,    -1,    -1,    -1,     2,    -1,
     415,    -1,    -1,    -1,    67,    -1,    69,    11,    71,    -1,
      73,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    71,   102,
      -1,    -1,   105,    -1,   379,    -1,    -1,    -1,    -1,    -1,
      -1,   466,    -1,    -1,    -1,   470,    -1,    -1,    -1,    -1,
     379,    -1,    -1,   478,    97,    -1,   401,    71,   101,    -1,
      -1,    -1,   407,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     415,    -1,   401,    -1,    -1,    -1,    29,    -1,   407,    -1,
      -1,    -1,   155,    97,    -1,    -1,   415,   101,   161,    -1,
      -1,    -1,    -1,    -1,   864,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   539,    -1,    -1,    -1,   161,    -1,
      -1,   466,    -1,    -1,    -1,   470,    79,    -1,    -1,    -1,
      83,    -1,    -1,   478,    -1,    -1,    -1,   466,    -1,   909,
     910,   470,    -1,   913,   914,    -1,    -1,   161,    -1,   478,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   583,    -1,
     930,   114,    -1,    -1,    -1,    -1,   936,    -1,   938,    -1,
     940,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,   144,   235,    -1,   147,   148,   149,   150,   151,   242,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,   633,    -1,
      -1,    -1,    -1,    29,   639,    -1,    -1,    -1,    -1,    24,
      25,   235,    -1,    -1,    -1,    -1,    -1,    -1,   242,    -1,
      35,    36,    -1,    -1,    -1,   660,    -1,    -1,   583,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,   674,
     675,    -1,   295,    -1,   583,    -1,    -1,    -1,    -1,    -1,
    1030,    -1,    67,    79,    69,    -1,   309,    83,    -1,    74,
     695,    -1,   697,    -1,    -1,    80,    -1,   702,    -1,    -1,
     705,   295,    -1,    -1,    -1,    -1,    -1,   712,   633,    29,
     715,    -1,    -1,    -1,   639,   309,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,   633,    -1,    -1,    -1,    -1,    -1,
     639,    -1,   737,    -1,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   748,    -1,   750,   142,    -1,    -1,    -1,
      29,    -1,    -1,   149,   150,    -1,   379,   153,    -1,    79,
     156,   157,   158,    83,    -1,    -1,    -1,    -1,    -1,    -1,
     695,    -1,   697,    -1,    -1,    -1,    -1,   702,   401,    -1,
     705,    29,    -1,    -1,   407,   379,   695,   712,   697,    -1,
     715,    -1,   415,   702,    -1,    -1,   705,    -1,    -1,    -1,
      79,    -1,    -1,   712,    83,    -1,   715,   401,    -1,    -1,
     130,   131,   737,   407,   134,   135,    -1,   137,   138,    -1,
      -1,   415,   142,   748,    -1,   750,   831,    -1,   737,    -1,
     150,    79,    -1,   838,    -1,    83,   156,   157,   158,   748,
      -1,   750,    -1,   466,    -1,   850,    -1,   470,    65,    -1,
      -1,   130,   131,    -1,    -1,   478,   135,   862,   137,   138,
      -1,    -1,    -1,   142,    -1,    -1,   871,    -1,    -1,    -1,
     875,   150,   466,    -1,    -1,    -1,   470,   156,   157,   158,
      -1,    -1,   130,     2,   478,    -1,    -1,   135,    -1,   137,
     138,    -1,    11,    29,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    24,   831,    26,   156,   157,
     158,   916,    -1,   838,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   831,    -1,    -1,   850,    -1,    -1,    -1,   838,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   862,    -1,    -1,
      -1,   850,    -1,    79,    -1,    -1,   871,    83,    -1,    -1,
     875,    -1,    71,   862,    -1,   960,   961,   962,   963,    -1,
     583,    -1,   871,    -1,    -1,    -1,   875,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
     985,   986,   101,    -1,   201,   202,   203,   204,    -1,   583,
      -1,   916,    -1,    -1,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,    -1,    -1,   142,   916,    -1,    -1,
     633,    -1,    -1,   149,   150,    -1,   639,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,     2,    -1,    -1,   245,    -1,
      -1,   248,    -1,    -1,    11,   960,   961,   962,   963,   633,
      -1,    -1,   161,    -1,    -1,   639,    -1,    24,    -1,    26,
     267,   960,   961,   962,   963,    -1,    -1,    -1,    -1,    -1,
     985,   986,    -1,     3,     2,    -1,    -1,    -1,   285,    -1,
      -1,    -1,   695,    11,   697,    15,   985,   986,    18,   702,
      -1,    -1,   705,    23,    -1,    -1,    24,    -1,    26,   712,
      -1,    -1,   715,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,   695,    42,   697,    -1,    -1,    -1,    47,   702,    -1,
      -1,   705,    -1,    -1,   737,    55,   235,    -1,   712,    -1,
      97,   715,    62,   242,   101,   748,    -1,   750,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,   737,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,   748,    -1,   750,    -1,    -1,    97,
     100,    -1,    -1,   101,    -1,   382,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   295,    -1,    -1,    -1,
      -1,    -1,    20,    -1,   161,    -1,    24,    25,    -1,   406,
     309,    -1,    -1,   410,    -1,    33,    -1,    35,    36,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    45,   831,    -1,
      -1,    -1,    -1,    -1,    -1,   838,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,    -1,   850,    -1,    67,
      -1,    69,    -1,    71,    -1,    73,    -1,   831,    76,   862,
      78,    -1,    80,    -1,   838,    -1,    -1,    -1,   871,    -1,
      -1,    -1,   875,    -1,    -1,    -1,   850,    -1,   235,    -1,
     379,    99,    -1,    -1,   102,   242,    -1,   105,   862,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   871,    -1,    -1,
      -1,   875,   401,    -1,    -1,    -1,    -1,    -1,   407,    -1,
      -1,    -1,    -1,   916,    -1,    -1,   415,   235,    -1,    -1,
      -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,   295,    -1,
      -1,    -1,   916,   161,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,   309,    -1,    -1,    -1,    -1,   960,   961,   962,
     963,    -1,    -1,    -1,    -1,    -1,    -1,   466,    -1,   566,
     567,   470,    -1,    -1,    -1,   572,   573,   295,    -1,   478,
      20,    -1,   985,   986,    24,    25,   960,   961,   962,   963,
      -1,   309,    -1,    33,    -1,    35,    36,    79,    -1,    -1,
      40,    83,    -1,    -1,     2,    45,    -1,    -1,    -1,    -1,
      -1,   985,   986,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   379,    -1,    -1,    -1,    24,    67,    26,    69,
      -1,    71,    -1,    73,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    -1,    -1,   401,    -1,    -1,    -1,   130,   131,
     407,   133,   134,   135,    -1,   137,   138,    -1,   415,    99,
     142,   379,   102,    -1,    -1,   105,    -1,    -1,   150,    24,
      25,    -1,    -1,    71,   156,   157,   158,    -1,    -1,    -1,
      35,    36,    -1,   401,   583,    -1,    -1,   684,   685,   407,
      45,    -1,    -1,    48,    -1,   692,    -1,   415,    -1,    97,
      -1,    -1,    -1,   101,    -1,    -1,    -1,   704,    -1,   466,
     707,    -1,    67,   470,    69,   155,    -1,    -1,    -1,    74,
      -1,   478,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   633,    -1,    -1,    -1,    -1,    -1,
     639,    -1,     1,    -1,    -1,    -1,    -1,    -1,   466,   104,
     105,    -1,   470,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     478,    20,    -1,   161,    -1,    24,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,   695,    -1,   697,    -1,
      -1,    -1,    -1,   702,    -1,    -1,   705,    -1,    67,    -1,
      69,    -1,    71,   712,    73,    74,   715,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,   583,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   737,    -1,
      99,    -1,    -1,   102,   242,    -1,   105,    -1,    -1,   748,
      -1,   750,    -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   583,    -1,   864,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,   633,    -1,    -1,    24,
      25,    -1,   639,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    36,    -1,    -1,    -1,    40,   155,   295,    -1,    -1,
      45,    -1,   161,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   309,    -1,    -1,    -1,   633,    -1,    -1,    -1,    -1,
      -1,   639,    67,    -1,    69,    -1,    71,    -1,    73,    74,
      -1,    76,   831,    78,    -1,    80,    -1,    -1,   695,   838,
     697,    -1,    -1,    -1,    -1,   702,    -1,    -1,   705,    -1,
      -1,   850,    -1,    -1,    99,   712,    -1,   102,   715,    -1,
     105,    -1,    -1,   862,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   871,    -1,    -1,    -1,   875,   695,    -1,   697,
     737,   379,    -1,    -1,   702,    -1,    -1,   705,    -1,    -1,
      -1,   748,    -1,   750,   712,    -1,    -1,   715,    -1,    -1,
      -1,    -1,    -1,   401,    -1,    -1,    -1,    -1,    -1,   407,
     155,    -1,    -1,    -1,    -1,    -1,   161,   916,    -1,   737,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     748,    -1,   750,    -1,    -1,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   960,   961,   962,   963,    -1,   146,    -1,   466,    -1,
      -1,    -1,   470,    -1,   831,    -1,    -1,    -1,    -1,   159,
     478,   838,   162,    -1,   164,    -1,   985,   986,    -1,     3,
      -1,    -1,    -1,   850,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    16,    -1,    18,   862,    -1,    -1,    -1,    23,
      -1,    -1,    -1,   831,   871,    -1,    -1,    31,   875,    -1,
     838,    -1,    -1,    37,    -1,    -1,    -1,    -1,    42,    -1,
      44,    -1,   850,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    -1,   862,    29,    -1,    61,    62,    -1,
      -1,    -1,    -1,   871,    68,    -1,    -1,   875,    -1,   916,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    89,    -1,    91,    -1,    93,
      -1,    -1,    96,    -1,    -1,   583,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    79,    -1,    -1,   916,    83,
      -1,    -1,    -1,   960,   961,   962,   963,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   985,   986,
      -1,    -1,    -1,    -1,    -1,   633,    -1,    -1,    -1,    -1,
      -1,   639,   960,   961,   962,   963,   130,   131,   162,   133,
     134,   135,    -1,   137,   138,   139,    -1,    -1,   142,   143,
     144,    -1,    -1,   147,   148,   149,   150,   985,   986,   153,
      -1,    -1,   156,   157,   158,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   695,    -1,   697,
      -1,    -1,    -1,    -1,   702,    -1,    -1,   705,    -1,    -1,
      -1,    -1,    -1,    -1,   712,    -1,    -1,   715,    -1,   159,
      -1,    -1,   162,    -1,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   737,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     748,    -1,   750,    -1,    -1,    -1,    -1,     0,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    18,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    -1,    30,    31,    -1,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      -1,    44,    45,    -1,    47,    48,    -1,    50,    51,    52,
      -1,    54,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    -1,    -1,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    -1,    80,    81,    82,
      -1,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,   850,    96,    97,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   862,    -1,   109,    -1,   111,    -1,
      -1,   114,    -1,   871,    -1,    -1,    -1,   875,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,
     153,    -1,   155,    -1,    -1,   158,    -1,   160,   916,   162,
       1,   164,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    -1,
      41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,
      -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
      -1,   152,   153,   154,    -1,    -1,    -1,   158,    -1,   160,
      -1,   162,    -1,   164,   165,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    12,    13,    14,    15,
      16,    17,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    44,    45,
      -1,    47,    48,    -1,    50,    51,    52,    -1,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    -1,
      96,    97,    98,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,   160,   161,   162,     1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    30,    31,    -1,    33,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    -1,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    -1,    80,    81,    82,    -1,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    -1,    96,    97,    98,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
     154,   155,    -1,    -1,   158,    -1,   160,   161,   162,     1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    31,
      -1,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    44,    45,    -1,    47,    48,    -1,    50,    51,
      52,    -1,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    -1,    96,    97,    98,    99,   100,    -1,
     102,   103,   104,   105,   106,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,    -1,   155,    -1,    -1,   158,    -1,   160,   161,
     162,     1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    18,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      30,    31,    -1,    33,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    44,    45,    -1,    47,    48,    -1,
      50,    51,    52,    -1,    54,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    -1,
      80,    81,    82,    -1,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    -1,    96,    97,    98,    99,
     100,    -1,   102,   103,   104,   105,   106,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,    -1,   155,    -1,    -1,   158,    -1,
     160,   161,   162,     1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    30,    31,    -1,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    44,    45,    -1,    47,
      48,    -1,    50,    51,    52,    -1,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    -1,    96,    97,
      98,    99,   100,    -1,   102,   103,   104,   105,   106,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,    -1,   155,    -1,    -1,
     158,    -1,   160,   161,   162,     1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    30,    31,    -1,    33,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    44,    45,
      -1,    47,    48,    -1,    50,    51,    52,    -1,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    81,    82,    -1,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    -1,
      96,    97,    98,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,   155,
      -1,    -1,   158,    -1,   160,   161,   162,     1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    30,    31,    -1,    33,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    -1,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    -1,    80,    81,    82,    -1,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    -1,    96,    97,    98,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
      -1,   155,    -1,    -1,   158,    -1,   160,    -1,   162,     1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    31,
      -1,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    44,    45,    -1,    47,    48,    -1,    50,    51,
      52,    -1,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    -1,    96,    97,    98,    99,   100,    -1,
     102,   103,   104,   105,   106,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,    -1,   155,    -1,    -1,   158,    -1,   160,    -1,
     162,     1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    18,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      30,    31,    -1,    33,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    44,    45,    -1,    47,    48,    -1,
      50,    51,    52,    -1,    54,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    78,    -1,
      80,    81,    82,    -1,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    -1,    96,    97,    98,    99,
     100,    -1,   102,   103,   104,   105,   106,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,    -1,   155,    -1,    -1,   158,    -1,
     160,    -1,   162,     1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    30,    31,    -1,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    44,    45,    -1,    47,
      48,    -1,    50,    51,    52,    -1,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    -1,    96,    97,
      98,    99,   100,    -1,   102,   103,   104,   105,   106,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,    -1,   155,    -1,    -1,
     158,    -1,   160,    -1,   162,     1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    30,    31,    -1,    33,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    44,    45,
      -1,    47,    48,    -1,    50,    51,    52,    -1,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    81,    82,    -1,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    -1,
      96,    97,    98,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,   155,
      -1,    -1,   158,    -1,   160,    -1,   162,     1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    30,    31,    -1,    33,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    -1,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    -1,    80,    81,    82,    -1,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    -1,    96,    97,    98,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
      -1,   155,    -1,    -1,   158,    -1,   160,    -1,   162,     1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    31,
      -1,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    44,    45,    -1,    47,    48,    -1,    50,    51,
      52,    -1,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      82,    -1,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    -1,    96,    97,    98,    99,   100,    -1,
     102,   103,   104,   105,   106,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,    -1,   155,    -1,    -1,   158,    -1,   160,    -1,
     162,     1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    18,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      30,    31,    -1,    33,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    44,    45,    -1,    47,    48,    -1,
      50,    51,    52,    -1,    54,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    78,    -1,
      80,    81,    82,    -1,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    -1,    96,    97,    98,    99,
     100,    -1,   102,   103,   104,   105,   106,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,    -1,   155,    -1,    -1,   158,    -1,
     160,    -1,   162,     1,   164,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    30,    31,    -1,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    44,    45,    -1,    47,
      48,    -1,    50,    51,    52,    -1,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      78,    -1,    80,    81,    82,    -1,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    -1,    96,    97,
      98,    99,   100,    -1,   102,   103,   104,   105,   106,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,    -1,   152,   153,    -1,   155,    -1,    -1,
     158,    -1,   160,    -1,   162,     1,   164,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    30,    31,    -1,    33,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    44,    45,
      -1,    47,    48,    -1,    50,    51,    52,    -1,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    81,    82,    -1,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    -1,
      96,    97,    98,    99,   100,    -1,   102,   103,   104,   105,
     106,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,   155,
      -1,    -1,   158,    -1,   160,    -1,   162,     1,   164,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    30,    31,    -1,    33,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    -1,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    72,    73,
      74,    75,    76,    77,    78,    -1,    80,    81,    82,    -1,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    -1,    96,    97,    98,    99,   100,    -1,   102,   103,
     104,   105,   106,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
      -1,   155,    -1,    -1,   158,    -1,   160,    -1,   162,     1,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    18,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    37,    38,    39,    -1,    41,
      42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,
      62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,
      72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,
     152,   153,   154,    -1,    -1,    -1,   158,    -1,   160,    -1,
     162,    -1,   164,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,
      -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,    59,
      -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,
      70,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,
     100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,   152,   153,   154,    -1,    -1,    -1,   158,    -1,
     160,    -1,   162,    -1,   164,   165,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,
      -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,   152,   153,   154,    -1,    -1,
      -1,   158,    -1,   160,    -1,   162,    -1,   164,   165,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    18,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,
      -1,    55,    56,    -1,    58,    59,    -1,    61,    62,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    93,
      -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,
       1,    -1,     3,    -1,   158,    -1,   160,    -1,   162,    10,
     164,   165,    -1,    -1,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    42,    -1,    44,    -1,    -1,    47,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    89,    -1,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,   137,    -1,    -1,    -1,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,    -1,   153,    -1,    -1,   156,   157,   158,   159,    -1,
      -1,   162,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,    -1,
      -1,    -1,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    37,    38,    39,    -1,
      41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,
      -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,
      -1,    -1,   103,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
      -1,   152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,
      -1,   162,    -1,   164,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,
      -1,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,
      59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      99,   100,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,   158,
      -1,   160,    -1,   162,    -1,   164,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,
      -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    93,    -1,    -1,    96,
      97,    98,    -1,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,   152,   153,   154,    -1,    -1,
      -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,
      55,    56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    93,    -1,
      -1,    96,    97,    98,    -1,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,   154,
      -1,    -1,    -1,   158,    -1,   160,    -1,   162,   163,   164,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    18,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,
      -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,    62,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      93,    -1,    -1,    96,    97,    98,    -1,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,
     153,   154,    -1,    -1,    -1,   158,    -1,   160,    -1,   162,
     163,   164,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,    -1,
      -1,    -1,    23,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    -1,
      41,    42,    43,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,
      -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,
      -1,    -1,   103,    -1,   105,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
      -1,   152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,
      -1,   162,    -1,   164,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,
      -1,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,
      59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      99,   100,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,   158,
      -1,   160,    -1,   162,    -1,   164,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,
      -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    93,    -1,    -1,    96,
      97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,   152,   153,    -1,   155,    -1,
      -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,
      55,    56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    93,    -1,
      -1,    96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,   154,
      -1,    -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    18,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,
      -1,    -1,    55,    56,    -1,    58,    59,    60,    61,    62,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      93,    -1,    -1,    96,    97,    98,    -1,   100,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,
     153,    -1,    -1,    -1,    -1,   158,    -1,   160,    -1,   162,
      -1,   164,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    -1,
      41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,
      -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
      -1,   152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,
      -1,   162,    -1,   164,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,
      59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,   112,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,    -1,   152,   153,   154,    -1,    -1,    -1,   158,
      -1,   160,    -1,   162,    -1,   164,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,
      -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    93,    -1,    -1,    96,
      97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,   152,   153,    -1,    -1,    -1,
      -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,
      55,    56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,   154,
      -1,    -1,    -1,   158,    -1,   160,   161,   162,    -1,   164,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    18,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,
      -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,    62,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,
     153,    -1,    -1,    -1,    -1,   158,    -1,   160,    -1,   162,
      -1,   164,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      31,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    -1,
      41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,
      -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
      -1,   152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,
      -1,   162,    -1,   164,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,
      59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,    -1,   152,   153,   154,    -1,    -1,    -1,   158,
      -1,   160,    -1,   162,    -1,   164,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,
      -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,   152,   153,   154,    -1,    -1,
      -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,
      55,    56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,   154,
      -1,    -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    18,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,
      -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,    62,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,
     153,   154,    -1,    -1,    -1,   158,    -1,   160,    -1,   162,
      -1,   164,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    -1,
      41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,
      -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
      -1,   152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,
      -1,   162,    -1,   164,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,
      59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,   158,
      -1,   160,    -1,   162,    -1,   164,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,
      -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,   152,   153,    -1,    -1,    -1,
      -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,
      55,    56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,
      -1,    -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    18,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,
      -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,    62,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,
     153,    -1,    -1,    -1,    -1,   158,    -1,   160,    -1,   162,
      -1,   164,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    -1,
      41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,
      -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
      -1,   152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,
      -1,   162,    -1,   164,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,
      59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,   158,
      -1,   160,    -1,   162,    -1,   164,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,
      -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,   152,   153,    -1,    -1,    -1,
      -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,
      55,    56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,
      -1,    -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    18,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,
      -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,    62,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,
     153,    -1,    -1,    -1,    -1,   158,    -1,   160,    -1,   162,
      -1,   164,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    -1,
      41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,
      -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
      -1,   152,   153,    -1,    -1,    -1,    -1,   158,    -1,   160,
      -1,   162,    -1,   164,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    18,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,
      59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,   158,
      -1,   160,    -1,   162,    -1,   164,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    -1,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,    56,
      -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    93,    -1,    -1,    96,
      -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,   152,   153,    -1,    -1,    -1,
      -1,   158,    -1,   160,    -1,   162,    -1,   164,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    -1,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,
      55,    56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,   152,   153,    -1,
      -1,    -1,    -1,   158,    -1,   160,    -1,   162,    -1,   164,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    18,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    37,    38,    39,    -1,    41,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    52,
      -1,    -1,    55,    56,    -1,    58,    59,    -1,    61,    62,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    72,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      93,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    -1,   152,
     153,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,   162,
      -1,   164,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    18,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    -1,
      41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    -1,    55,    56,    -1,    58,    59,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,
      -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    93,    -1,    -1,    96,    -1,    -1,    -1,   100,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,   132,   133,   134,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,   149,    18,
      -1,    -1,   153,    -1,    23,    -1,    -1,   158,    -1,   160,
      -1,   162,    31,   164,    -1,    -1,    -1,    -1,    37,    38,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    52,    -1,    -1,    55,    56,    -1,    58,
      59,    -1,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,   154,    12,    -1,    -1,    15,
      16,   160,    18,   162,   163,   164,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    37,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    52,    53,    -1,    55,
      56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    93,    -1,    95,
      96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,   155,
      -1,    -1,    -1,    -1,   160,    31,   162,    -1,   164,    -1,
      -1,    37,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,
      56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    31,   162,    -1,   164,    -1,
      -1,    37,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,
      56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    93,    -1,    -1,
      96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    31,   162,    -1,   164,    -1,
      -1,    37,    38,    39,    -1,    41,    42,    -1,    44,    -1,
      -1,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,
      56,    -1,    58,    59,    -1,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    93,    15,    16,
      96,    18,    -1,    -1,   100,    -1,    23,   103,    -1,    -1,
     106,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    -1,   132,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    61,    62,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,     3,   162,    -1,   164,    86,
      87,    -1,    89,    -1,    91,    -1,    93,    15,    16,    96,
      18,    -1,    -1,   100,    -1,    23,   103,    -1,    -1,   106,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,     3,    61,    62,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    15,    16,    -1,    18,    -1,    77,
      -1,    -1,    23,   160,    -1,   162,    -1,    -1,    86,    87,
      31,    89,    -1,    91,    -1,    93,    37,    -1,    96,    -1,
      -1,    42,   100,    44,    -1,   103,    47,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    89,    -1,
      91,    -1,    93,    -1,     3,    96,    -1,    -1,    -1,   100,
      -1,    10,   103,    -1,   162,   106,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    42,    -1,    44,    -1,    -1,    47,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    61,    62,    -1,    -1,    -1,    10,    -1,    68,
      -1,   162,    -1,    -1,    17,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    86,    87,    -1,
      89,    -1,    91,    -1,    93,    -1,    -1,    96,    -1,    -1,
      -1,   100,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    29,   128,
     129,   130,   131,   132,   133,   134,    79,    -1,   137,    -1,
      83,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,   153,    10,    -1,   156,   157,   158,
     159,    -1,    17,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,   114,    -1,    -1,    29,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,   152,
     153,    -1,    10,   156,   157,   158,    -1,    -1,    -1,    17,
      -1,    -1,   165,    -1,    79,    -1,    -1,    -1,    83,   130,
     131,    29,   133,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,   144,    -1,    -1,   147,   148,   149,   150,
     151,    -1,   153,    10,    -1,   156,   157,   158,    -1,   114,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,   130,   131,    -1,   133,   134,
     135,    79,   137,   138,   139,    83,   141,   142,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,   152,   153,    10,
      -1,   156,   157,   158,    -1,    -1,    17,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   114,    -1,    29,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,    10,   114,   156,   157,
     158,    -1,    -1,    17,    -1,    -1,    -1,   165,    79,    -1,
      -1,    -1,    83,   130,   131,    29,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,    -1,    -1,   156,
     157,   158,    -1,   114,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,    -1,   133,   134,   135,    79,   137,   138,   139,    83,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,   152,   153,    10,    -1,   156,   157,   158,    -1,    -1,
      17,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
      -1,    -1,   156,   157,   158,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    92,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,    -1,    79,   156,
     157,   158,    83,   160,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    92,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,   152,   153,    -1,    79,   156,   157,   158,    83,   160,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    92,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
     115,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,   152,   153,    -1,
      79,   156,   157,   158,    83,   160,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    92,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,    -1,    79,   156,   157,   158,
      83,   160,    -1,    10,    -1,    -1,    -1,    -1,    -1,    92,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,   152,
     153,    -1,    79,   156,   157,   158,    83,   160,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    92,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,    -1,    79,   156,
     157,   158,    83,   160,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    92,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,   152,   153,    -1,    79,   156,   157,   158,    83,   160,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    92,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
     115,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,   152,   153,    -1,
      79,   156,   157,   158,    83,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,    10,    -1,   156,   157,   158,
      -1,   160,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    43,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    79,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,   110,    79,    -1,    -1,   114,
      83,    -1,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,   144,
     145,   114,   147,   148,   149,   150,   151,   152,   153,    -1,
      -1,   156,   157,   158,    -1,   160,    -1,   130,   131,    -1,
     133,   134,   135,    79,   137,   138,   139,    83,   141,   142,
     143,   144,   145,    -1,   147,   148,   149,   150,   151,   152,
     153,    10,    -1,   156,   157,   158,    -1,   160,    17,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,   114,    -1,
      29,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,    10,    -1,
     156,   157,   158,    -1,   160,    17,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    83,    -1,    -1,    29,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,   133,   134,   135,    79,   137,   138,
     139,    83,   141,   142,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,    -1,    -1,   156,   157,   158,
      -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,   114,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,   130,   131,
      -1,   133,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,   144,   145,   114,   147,   148,   149,   150,   151,
     152,   153,    -1,    -1,   156,   157,   158,    -1,   160,    -1,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,   144,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,    10,    -1,   156,   157,   158,    -1,
     160,    17,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    15,    16,    -1,    18,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    31,    -1,    17,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    42,    -1,    44,    29,    -1,    47,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    61,    62,    79,    -1,    -1,    -1,    83,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      64,    -1,    -1,    10,    -1,    -1,    86,    87,    -1,    89,
      17,    91,    -1,    93,    -1,    79,    96,    -1,   114,    83,
     100,    -1,    29,   103,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,    43,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,   144,   145,
     114,   147,   148,   149,   150,   151,   152,   153,    -1,    -1,
     156,   157,   158,    -1,   160,    -1,   130,   131,    -1,   133,
     134,   135,    79,   137,   138,   139,    83,   141,   142,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
      10,   155,   156,   157,   158,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,   114,    -1,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,   144,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,    10,    -1,   156,
     157,   158,    -1,    -1,    17,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    83,    -1,    -1,    29,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      43,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    29,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,    43,   133,   134,   135,    79,   137,   138,   139,
      83,   141,   142,   143,   144,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,   114,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,   144,   145,   114,   147,   148,   149,   150,   151,   152,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,   152,   153,    10,    -1,   156,   157,   158,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    79,    -1,    -1,   114,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,   144,   145,   114,
     147,   148,   149,   150,   151,   152,   153,    -1,    -1,   156,
     157,   158,    -1,    79,    -1,   130,   131,    83,   133,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,   144,
     145,    -1,   147,   148,   149,   150,   151,   152,   153,    10,
     155,   156,   157,   158,    -1,    -1,    17,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,    10,   155,
     156,   157,   158,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    79,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,   130,
     131,    83,   133,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,    -1,    79,
      -1,   113,   114,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,   130,   131,
      -1,   133,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,   144,   145,   114,   147,   148,   149,   150,   151,
     152,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,   144,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,    10,    -1,   156,   157,   158,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    83,    -1,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,    -1,   110,
     156,   157,   158,   114,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,   130,
     131,    -1,   133,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,   144,   145,    -1,   147,   148,   149,   150,
     151,   152,   153,    -1,   114,   156,   157,   158,    -1,    -1,
      -1,    10,    11,    -1,    -1,    -1,    -1,    -1,    17,    -1,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
      29,   141,   142,   143,   144,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,   114,    -1,   156,   157,   158,
      -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,   113,   114,   156,   157,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,    10,    -1,   156,   157,
     158,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
     114,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
     144,   145,   114,   147,   148,   149,   150,   151,   152,   153,
      -1,    -1,   156,   157,   158,    -1,    -1,    -1,   130,   131,
      -1,   133,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,   144,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,    10,    -1,   156,   157,   158,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    11,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    79,    -1,    -1,   114,    83,    -1,    -1,
      29,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,   144,   145,   114,   147,
     148,   149,   150,   151,   152,   153,    -1,    -1,   156,   157,
     158,    -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,
      79,   137,   138,   139,    83,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,    10,    -1,
     156,   157,   158,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,   144,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    83,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      92,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
      -1,   133,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,   144,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,    -1,    79,   156,   157,   158,    83,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    92,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,    -1,    79,
     156,   157,   158,    83,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    92,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,   144,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,    -1,    79,   156,   157,   158,    83,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    92,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
      -1,    79,   156,   157,   158,    83,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    92,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,    -1,   133,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,   144,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,    -1,    79,   156,   157,
     158,    83,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      92,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,   130,   131,
      -1,   133,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,   144,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,    -1,    79,   156,   157,   158,    83,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,   144,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,    -1,    79,
     156,   157,   158,    83,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,    -1,   133,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,   144,   145,    29,   147,   148,   149,
     150,   151,   152,   153,    -1,    79,   156,   157,   158,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
     144,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
      -1,   114,   156,   157,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,   144,    -1,    -1,   147,   148,   149,   150,   151,   152,
     153,    -1,    -1,   156,   157,   158,     1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    16,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    61,    62,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    -1,    89,    -1,    91,    -1,    93,    -1,
      -1,    96,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,
      -1,   106
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   170,   171,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    18,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    30,    31,    33,    35,
      36,    37,    38,    39,    40,    41,    42,    44,    45,    47,
      50,    51,    52,    54,    55,    56,    58,    59,    61,    62,
      63,    67,    68,    69,    70,    71,    72,    73,    76,    77,
      78,    80,    81,    82,    84,    85,    86,    87,    88,    89,
      90,    91,    93,    94,    96,    97,    98,    99,   100,   102,
     103,   105,   106,   109,   111,   114,   130,   131,   132,   133,
     134,   139,   149,   152,   153,   155,   158,   160,   162,   164,
     172,   173,   174,   175,   176,   177,   179,   184,   185,   186,
     187,   191,   193,   197,   198,   199,   200,   201,   202,   204,
     205,   206,   210,   211,   214,   215,   218,   219,   222,   225,
     226,   252,   255,   256,   276,   277,   278,   279,   280,   281,
     282,   290,   291,   292,   293,   296,   297,   298,   299,   300,
     301,   302,   304,   305,   306,   307,   308,   155,   174,   293,
     110,   283,   284,     3,   188,    12,    38,    39,    41,    91,
     160,   164,   222,   276,   281,   291,   292,   293,   296,   298,
     299,   283,   293,    99,   255,    80,   188,   174,   270,   293,
     174,   162,   293,   294,     7,     9,    99,   195,   294,    69,
     112,   203,   293,   203,   293,    24,    36,   256,   293,   293,
     162,     3,    15,    16,    18,    23,    31,    37,    42,    44,
      47,    55,    61,    62,    68,    77,    86,    87,    89,    91,
      93,    96,   100,   103,   106,   190,   192,   223,   162,   190,
     257,   258,    30,   179,   196,   293,   293,   293,   293,   293,
     293,     7,   162,    36,   199,   199,   154,   245,   269,   293,
      97,    98,   155,   293,   295,   293,   196,   293,   293,   293,
      90,   162,   174,   293,   293,   179,   187,   293,   296,   179,
     187,   293,   190,   253,   293,   293,   293,   293,   293,   293,
     293,   293,     1,   161,   172,   180,   269,   101,   140,   245,
     271,   272,   295,   203,   269,   293,   303,    71,   174,   160,
      75,   178,   104,   190,   190,    48,    74,   240,   257,   154,
     155,   146,    10,    17,    29,    79,    83,   114,   130,   131,
     133,   134,   135,   137,   138,   139,   141,   142,   143,   144,
     145,   147,   148,   149,   150,   151,   152,   153,   156,   157,
     158,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   159,   232,   162,   164,
      79,    83,   162,   174,   155,   293,   293,   293,   269,   160,
     179,    43,   283,   253,   257,   155,   136,   155,   109,   191,
     245,   273,   274,   275,   295,   253,   215,   255,   190,   162,
     196,    30,    43,   196,   110,   196,   286,    30,    43,   196,
     286,   294,    34,    64,   155,    92,   179,   232,   273,   174,
     162,   234,   101,   162,   190,   259,   260,     1,   135,   264,
      43,   136,   174,   196,   196,   273,   190,   136,   155,   293,
     293,   155,   160,   196,   162,   273,   155,   207,   155,   207,
     155,   115,   254,   155,   196,   155,   161,   161,   172,   136,
     161,   293,   136,   163,   136,   163,   165,   286,    43,   136,
     165,   286,   113,   136,   165,     7,     1,   161,   180,    57,
     269,   293,   135,   216,   160,   227,   155,   293,   293,   293,
     293,   203,   293,   203,   293,   293,   293,   293,   293,   293,
     293,    18,    31,    55,    99,   191,   293,   293,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   295,   295,   295,   295,   295,   273,   273,   203,
     293,   203,   293,    25,    43,    80,   105,   285,   288,   289,
     293,   308,    30,    43,    30,    43,    92,    43,   165,   203,
     293,   196,   155,   155,   293,   293,   115,   163,   136,   155,
     190,   257,    43,   269,    41,   293,   203,   293,   162,   196,
      41,   293,   203,   293,   196,   158,   181,   183,   293,   181,
     182,   174,   293,    32,   293,   163,    25,    43,    46,    66,
      69,    80,    99,   173,   235,   236,   237,   238,   224,   260,
     136,   163,    31,    44,    87,    91,   164,   194,   265,   277,
     115,   261,   293,   258,   163,   245,   293,     1,   212,   273,
     163,    19,   208,   265,   277,   136,   161,   163,   163,   271,
     163,   271,   174,   165,   203,   293,   165,   174,   293,   165,
     293,   165,   293,   161,   161,   190,   155,    34,    64,   269,
     160,     1,   190,   220,   221,    25,    69,    80,    99,   229,
     239,   162,   155,   155,   155,   155,   155,   163,   165,    43,
      80,   136,   163,   276,    79,    79,    41,   203,   293,    41,
     203,   293,   203,   293,   283,   283,   155,   245,   295,   275,
     216,   155,   293,   163,   293,    30,   196,    30,   196,   287,
     288,   293,    30,   196,   286,    30,   196,   286,   136,   155,
      11,   155,    32,    32,   174,    92,   179,    43,    80,   237,
     136,   163,   162,   190,    25,    69,    80,    99,   241,   163,
     260,   264,     1,   269,    60,   295,   161,    65,   107,   161,
     213,   163,   162,   179,   190,   209,   253,   174,   165,   286,
     165,   286,   174,   113,   183,   182,     1,   217,   161,   115,
     136,   161,    80,   228,     1,     3,    10,    15,    17,    18,
      23,    37,    42,    47,    49,    55,    61,    62,    77,    89,
      93,    96,   100,   106,   130,   131,   132,   133,   134,   137,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   153,   156,   157,   158,   159,   162,   189,   190,   192,
     230,   231,   232,   276,   163,   288,   264,   276,   276,   293,
      30,    30,   293,    30,    30,   165,   165,   196,   196,   160,
     196,    92,    41,   293,    41,   293,   136,   163,    92,    41,
     293,   196,    41,   293,   196,   293,   293,   174,   293,   174,
      32,   190,   236,   260,   135,   268,    80,   264,   261,   165,
      43,   165,    30,   174,   269,   209,   135,   179,    41,   174,
     293,   165,    41,   174,   293,   165,   293,   155,   155,   161,
      36,    73,    76,   161,   173,   198,   293,   221,   241,   162,
     233,   190,   293,   130,   138,   233,   233,   261,    92,    41,
      41,    92,    41,    41,    41,    41,     1,   217,   293,   293,
     293,   288,   293,   293,   293,    11,    32,   174,   268,   163,
     164,   194,   245,   267,   277,   140,   247,   261,    53,   108,
     248,   293,   265,   277,   174,   196,   163,   293,   293,   174,
     293,   174,   198,     1,   135,   263,   235,   163,     3,    93,
     231,   232,   293,   293,   293,   293,   293,   293,   161,   161,
      92,    92,    92,    92,   293,   174,   261,   268,   165,   269,
     245,   246,   293,     3,    82,    93,   249,   250,   251,   293,
     179,   197,   244,   165,   179,    92,    92,   164,   194,   262,
     277,    95,   242,   163,   233,   233,    92,    92,    92,    92,
      92,    92,   293,   293,   293,   293,   247,   261,   245,   266,
     267,   277,    43,   165,   251,   108,   136,   115,   141,   143,
     144,   147,   148,    53,   277,   293,   293,     1,   165,   269,
     248,   293,   266,   267,   293,   250,   251,   251,   251,   251,
     251,   251,   249,   165,   262,   277,   165,   155,   243,   244,
     165,   262,   277,   266
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   169,   170,   171,   171,   172,   172,   173,   173,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   175,
     176,   176,   176,   177,   177,   177,   178,   178,   179,   179,
     179,   180,   180,   181,   181,   181,   181,   182,   182,   183,
     183,   184,   184,   184,   185,   185,   185,   186,   187,   187,
     187,   187,   187,   188,   188,   189,   189,   189,   189,   189,
     189,   190,   190,   190,   190,   190,   190,   191,   191,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   194,
     194,   194,   194,   195,   195,   196,   196,   197,   197,   198,
     198,   198,   198,   198,   198,   198,   198,   199,   199,   200,
     200,   200,   200,   201,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     203,   204,   204,   204,   204,   204,   204,   204,   204,   205,
     206,   206,   206,   206,   206,   206,   207,   207,   208,   208,
     208,   209,   209,   210,   211,   211,   212,   212,   213,   213,
     213,   214,   214,   214,   214,   215,   215,   215,   216,   216,
     217,   217,   217,   218,   218,   219,   220,   220,   220,   221,
     221,   223,   224,   222,   225,   225,   225,   225,   225,   227,
     228,   226,   229,   229,   229,   229,   229,   230,   230,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   233,
     233,   234,   235,   235,   235,   236,   236,   236,   236,   236,
     237,   237,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   239,   239,   239,   239,   239,   239,   240,   240,   241,
     241,   241,   241,   241,   241,   242,   242,   243,   243,   244,
     244,   245,   245,   246,   246,   247,   247,   248,   248,   248,
     248,   248,   249,   249,   250,   250,   250,   250,   250,   250,
     250,   251,   251,   252,   252,   252,   253,   253,   254,   254,
     254,   255,   255,   255,   255,   255,   256,   256,   256,   257,
     257,   258,   258,   259,   259,   259,   260,   260,   260,   261,
     261,   261,   262,   262,   262,   262,   262,   262,   262,   263,
     263,   263,   263,   263,   264,   264,   264,   264,   264,   265,
     265,   265,   265,   266,   266,   266,   267,   267,   267,   267,
     267,   268,   268,   268,   268,   268,   269,   269,   269,   269,
     270,   270,   271,   271,   271,   272,   272,   273,   273,   274,
     274,   275,   275,   275,   275,   276,   276,   277,   277,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   280,   281,   282,   282,
     282,   282,   282,   282,   282,   282,   283,   283,   284,   285,
     285,   286,   287,   287,   288,   288,   288,   289,   289,   289,
     289,   289,   289,   290,   290,   291,   292,   293,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   294,   294,   295,   295,   295,   296,   296,   296,   296,
     297,   297,   297,   298,   298,   298,   299,   299,   299,   299,
     299,   300,   300,   300,   300,   301,   301,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     303,   303,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   305,   305,   305,   305,   305,
     305,   305,   306,   306,   306,   306,   307,   307,   307,   307,
     308,   308,   308,   308,   308,   308,   308
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
       1,     0,     1,     1,     4,     6,     6,     3,     4,     4,
       4,     4,     4,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     2,     1,     2,     3,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     3,
       5,     5,     2,     2,     5,     3,     6,     6,     4,     5,
       5,     3,     3,     6,     5,     6,     5,     6,     3,     4,
       3,     4,     6,     7,     6,     7,     4,     5,     4,     5,
       4,     4,     3,     6,     5,     6,     5,     8,     7,     2,
       3,     3,     2,     2,     3,     3,     0,     2,     2,     3,
       5,     1,     3,     3,     5,     5,     0,     2,     3,     2,
       3,     6,     8,     6,     8,     1,     1,     1,     0,     2,
       0,     2,     3,     5,     5,     1,     1,     2,     3,     1,
       3,     0,     0,     8,     0,     1,     2,     2,     1,     0,
       0,    10,     3,     3,     5,     5,     3,     1,     3,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     3,     0,     1,     3,     4,     5,     4,     6,     6,
       0,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     0,     1,     1,     2,     1,     1,     1,     1,     0,
       1,     2,     1,     1,     1,     0,     1,     1,     1,     1,
       1,     2,     1,     0,     1,     2,     2,     0,     2,     2,
       4,     4,     1,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     3,     4,     4,     2,     4,     0,     2,
       2,     1,     1,     1,     2,     1,     4,     5,     3,     1,
       3,     3,     5,     1,     1,     3,     1,     2,     3,     0,
       2,     2,     3,     2,     4,     3,     3,     4,     3,     0,
       2,     2,     2,     1,     0,     2,     2,     2,     1,     4,
       4,     6,     3,     0,     1,     1,     3,     4,     3,     4,
       6,     0,     2,     2,     2,     2,     1,     1,     3,     3,
       1,     3,     1,     1,     1,     3,     3,     0,     1,     1,
       3,     3,     3,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     2,     4,     4,     4,     5,     2,
       2,     1,     2,     1,     2,     1,     2,     1,     2,     6,
       6,     4,     9,     9,     7,     6,     6,     4,     9,     9,
       7,     4,     6,     6,     9,     9,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     1,     4,     1,
       3,     4,     1,     3,     4,     3,     3,     1,     1,     2,
       1,     2,     1,     1,     3,     2,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     3,     2,     2,
       1,     0,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     1,     4,     4,     4,     3,     3,     3,     3,
       5,     3,     4,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     3,     4,     3,     4,
       3,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 488 "chapel.ypp" /* yacc.c:1663  */
    { yyblock = (yyval.pblockstmt); }
#line 5842 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 3:
#line 493 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5848 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 4:
#line 494 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5854 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 6:
#line 501 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5860 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 7:
#line 506 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5866 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 8:
#line 507 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5872 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 23:
#line 526 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5878 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 24:
#line 527 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5884 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 25:
#line 528 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5890 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 26:
#line 529 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5896 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 27:
#line 530 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5902 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 28:
#line 531 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5908 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 29:
#line 532 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5914 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 30:
#line 533 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5920 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 31:
#line 534 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5926 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 32:
#line 535 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5932 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 33:
#line 536 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5938 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 34:
#line 537 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5944 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 35:
#line 538 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5950 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 36:
#line 539 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5956 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 37:
#line 540 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5962 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 38:
#line 541 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5968 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 39:
#line 546 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 5982 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 40:
#line 559 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 5991 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 41:
#line 564 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6000 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 42:
#line 569 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6009 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 43:
#line 576 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6015 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 44:
#line 577 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6021 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 45:
#line 578 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6027 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 46:
#line 582 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6033 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 47:
#line 583 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 6039 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 48:
#line 596 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6045 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 49:
#line 597 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6051 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 50:
#line 598 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6057 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 51:
#line 603 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6063 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 52:
#line 604 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6069 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 53:
#line 609 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6079 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 54:
#line 614 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6089 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 55:
#line 619 "chapel.ypp" /* yacc.c:1663  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6098 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 56:
#line 623 "chapel.ypp" /* yacc.c:1663  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6107 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 57:
#line 630 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6117 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 59:
#line 639 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6127 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 61:
#line 648 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 62:
#line 649 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6139 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 63:
#line 650 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6145 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 64:
#line 654 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr), (yyvsp[-3].b)); }
#line 6151 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 65:
#line 655 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 6157 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 66:
#line 656 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 6163 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 67:
#line 660 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 6169 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 68:
#line 665 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 6175 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 69:
#line 667 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 6181 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 70:
#line 669 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 6187 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 71:
#line 671 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 6193 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 72:
#line 673 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 6199 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 73:
#line 679 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 6205 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 74:
#line 680 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6211 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 75:
#line 684 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6217 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 76:
#line 685 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6223 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 77:
#line 686 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6229 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 78:
#line 687 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6235 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 79:
#line 688 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6241 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 80:
#line 689 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6247 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 81:
#line 692 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6253 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 82:
#line 693 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6259 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 83:
#line 694 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6265 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 84:
#line 695 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6271 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 85:
#line 696 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6277 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 86:
#line 697 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6283 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 87:
#line 709 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6289 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 88:
#line 710 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6295 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 89:
#line 721 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bool"; }
#line 6301 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 90:
#line 722 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "int"; }
#line 6307 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 91:
#line 723 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "uint"; }
#line 6313 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 92:
#line 724 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "real"; }
#line 6319 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 93:
#line 725 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "imag"; }
#line 6325 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 94:
#line 726 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "complex"; }
#line 6331 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 95:
#line 727 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bytes"; }
#line 6337 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 96:
#line 728 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "string"; }
#line 6343 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 97:
#line 729 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "sync"; }
#line 6349 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 98:
#line 730 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "single"; }
#line 6355 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 99:
#line 731 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "owned"; }
#line 6361 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 100:
#line 732 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "shared"; }
#line 6367 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 101:
#line 733 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "borrowed"; }
#line 6373 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 102:
#line 734 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "unmanaged"; }
#line 6379 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 103:
#line 735 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "domain"; }
#line 6385 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 104:
#line 736 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "index"; }
#line 6391 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 105:
#line 737 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "locale"; }
#line 6397 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 106:
#line 738 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "nothing"; }
#line 6403 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 107:
#line 739 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "void"; }
#line 6409 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 108:
#line 743 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6415 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 109:
#line 744 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6421 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 110:
#line 745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6427 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 111:
#line 746 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6433 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 112:
#line 747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6439 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 113:
#line 748 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6445 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 114:
#line 749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 6451 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 115:
#line 750 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6457 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 116:
#line 751 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6463 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 117:
#line 752 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 6469 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 118:
#line 753 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 6475 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 119:
#line 760 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_syncvar"; }
#line 6481 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 120:
#line 761 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_singlevar"; }
#line 6487 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 121:
#line 766 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_domain"; }
#line 6493 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 122:
#line 767 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_index"; }
#line 6499 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 123:
#line 772 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 6505 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 124:
#line 773 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6511 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 125:
#line 777 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6517 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 126:
#line 778 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6523 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 127:
#line 782 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6529 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 128:
#line 783 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6535 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 129:
#line 787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6541 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 131:
#line 789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6547 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 132:
#line 790 "chapel.ypp" /* yacc.c:1663  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6553 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 139:
#line 803 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6559 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 140:
#line 804 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6565 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 141:
#line 805 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6571 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 142:
#line 806 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6577 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 143:
#line 811 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6585 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 144:
#line 817 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6591 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 145:
#line 818 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6597 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 146:
#line 819 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 6603 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 147:
#line 820 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6609 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 148:
#line 821 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 6615 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 149:
#line 822 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6621 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 150:
#line 823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6627 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 151:
#line 824 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6633 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 152:
#line 825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6639 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 153:
#line 826 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6645 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 154:
#line 827 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6651 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 155:
#line 828 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6657 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 156:
#line 829 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6663 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 157:
#line 830 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6669 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 158:
#line 831 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6675 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 159:
#line 832 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6681 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 160:
#line 833 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6687 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 161:
#line 834 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6693 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 162:
#line 836 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6703 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 163:
#line 842 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6713 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 164:
#line 848 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6723 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 165:
#line 854 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6733 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 166:
#line 860 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6744 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 167:
#line 867 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6755 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 168:
#line 874 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6763 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 169:
#line 878 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6771 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 170:
#line 884 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 6777 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 171:
#line 888 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6783 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 172:
#line 889 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6789 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 173:
#line 890 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6795 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 174:
#line 891 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6801 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 175:
#line 892 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6808 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 176:
#line 894 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6815 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 177:
#line 896 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6822 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 178:
#line 898 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6829 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 179:
#line 903 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6835 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 180:
#line 906 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6841 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 181:
#line 907 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6847 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 182:
#line 908 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6853 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 183:
#line 909 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6859 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 184:
#line 910 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6865 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 185:
#line 911 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6871 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 186:
#line 915 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6877 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 187:
#line 916 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6883 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 188:
#line 920 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6889 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 189:
#line 921 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 6895 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 190:
#line 922 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 6901 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 191:
#line 926 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6907 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 192:
#line 927 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6913 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 193:
#line 931 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6919 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 194:
#line 935 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6925 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 195:
#line 937 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6931 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 196:
#line 941 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6937 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 197:
#line 942 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6943 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 198:
#line 947 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6949 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 199:
#line 949 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6955 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 200:
#line 951 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6961 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 201:
#line 958 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6975 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 202:
#line 968 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6989 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 203:
#line 978 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7003 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 204:
#line 988 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 7017 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 205:
#line 1000 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7027 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 206:
#line 1005 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7037 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 207:
#line 1010 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7047 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 208:
#line 1018 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 7053 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 209:
#line 1019 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 7059 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 210:
#line 1024 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 7065 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 211:
#line 1026 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7071 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 212:
#line 1028 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 7077 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 213:
#line 1033 "chapel.ypp" /* yacc.c:1663  */
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
#line 7097 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 214:
#line 1049 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 7105 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 215:
#line 1056 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7115 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 216:
#line 1065 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 7127 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 217:
#line 1073 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 7135 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 218:
#line 1077 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 7143 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 219:
#line 1083 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 7149 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 220:
#line 1084 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 7155 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 221:
#line 1089 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 7164 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 222:
#line 1094 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7173 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 223:
#line 1099 "chapel.ypp" /* yacc.c:1663  */
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
#line 7195 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 224:
#line 1121 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7206 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 225:
#line 1127 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7218 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 226:
#line 1134 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7229 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 227:
#line 1140 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7240 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 228:
#line 1146 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7252 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 229:
#line 1157 "chapel.ypp" /* yacc.c:1663  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 7262 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 230:
#line 1163 "chapel.ypp" /* yacc.c:1663  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7273 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 231:
#line 1170 "chapel.ypp" /* yacc.c:1663  */
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
#line 7300 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 232:
#line 1196 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 7308 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 233:
#line 1200 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7317 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 234:
#line 1205 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 7325 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 235:
#line 1209 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7334 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 236:
#line 1214 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 7342 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 238:
#line 1221 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7348 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 239:
#line 1225 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7354 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 240:
#line 1226 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 7360 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 241:
#line 1227 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&"; }
#line 7366 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 242:
#line 1228 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|"; }
#line 7372 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 243:
#line 1229 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^"; }
#line 7378 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 244:
#line 1230 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "~"; }
#line 7384 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 245:
#line 1231 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "=="; }
#line 7390 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 246:
#line 1232 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!="; }
#line 7396 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 247:
#line 1233 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<="; }
#line 7402 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 248:
#line 1234 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">="; }
#line 7408 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 249:
#line 1235 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<"; }
#line 7414 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 250:
#line 1236 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">"; }
#line 7420 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 251:
#line 1237 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+"; }
#line 7426 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 252:
#line 1238 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-"; }
#line 7432 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 253:
#line 1239 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*"; }
#line 7438 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 254:
#line 1240 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/"; }
#line 7444 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 255:
#line 1241 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<"; }
#line 7450 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 256:
#line 1242 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>"; }
#line 7456 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 257:
#line 1243 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%"; }
#line 7462 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 258:
#line 1244 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**"; }
#line 7468 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 259:
#line 1245 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!"; }
#line 7474 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 260:
#line 1246 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_by"; }
#line 7480 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 261:
#line 1247 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "#"; }
#line 7486 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 262:
#line 1248 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_align"; }
#line 7492 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 263:
#line 1249 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<=>"; }
#line 7498 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 264:
#line 1250 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<~>"; }
#line 7504 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 265:
#line 1251 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "init="; }
#line 7510 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 266:
#line 1252 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 7516 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 267:
#line 1256 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "="; }
#line 7522 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 268:
#line 1257 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+="; }
#line 7528 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 269:
#line 1258 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-="; }
#line 7534 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 270:
#line 1259 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*="; }
#line 7540 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 271:
#line 1260 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/="; }
#line 7546 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 272:
#line 1261 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%="; }
#line 7552 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 273:
#line 1262 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**="; }
#line 7558 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 274:
#line 1263 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&="; }
#line 7564 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 275:
#line 1264 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|="; }
#line 7570 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 276:
#line 1265 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^="; }
#line 7576 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 277:
#line 1266 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>="; }
#line 7582 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 278:
#line 1267 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<="; }
#line 7588 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 279:
#line 1271 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7594 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 280:
#line 1272 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7600 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 281:
#line 1276 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7606 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 282:
#line 1280 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7612 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 283:
#line 1281 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7618 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 284:
#line 1282 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7624 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 285:
#line 1287 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7630 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 286:
#line 1289 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7636 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 287:
#line 1291 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7642 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 288:
#line 1293 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7648 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 289:
#line 1295 "chapel.ypp" /* yacc.c:1663  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7654 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 290:
#line 1299 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7660 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 291:
#line 1300 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7666 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 292:
#line 1304 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_IN; }
#line 7672 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 293:
#line 1305 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7678 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 294:
#line 1306 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_OUT; }
#line 7684 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 295:
#line 1307 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST; }
#line 7690 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 296:
#line 1308 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7696 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 297:
#line 1309 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7702 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 298:
#line 1310 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7708 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 299:
#line 1311 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF; }
#line 7714 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 300:
#line 1312 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7720 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 301:
#line 1316 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7726 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 302:
#line 1317 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7732 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 303:
#line 1318 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF;   }
#line 7738 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 304:
#line 1319 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 7744 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 305:
#line 1320 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST;   }
#line 7750 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 306:
#line 1321 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 7756 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 307:
#line 1325 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 7762 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 308:
#line 1326 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 7768 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 309:
#line 1330 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 7774 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 310:
#line 1331 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 7780 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 311:
#line 1332 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 7786 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 312:
#line 1333 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_REF; }
#line 7792 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 313:
#line 1334 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_PARAM; }
#line 7798 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 314:
#line 1335 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_TYPE; }
#line 7804 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 315:
#line 1339 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 7810 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 316:
#line 1340 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 7816 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 317:
#line 1343 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = NULL; }
#line 7822 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 320:
#line 1349 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7828 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 321:
#line 1354 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7834 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 322:
#line 1356 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7840 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 323:
#line 1360 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7846 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 325:
#line 1365 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7852 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 326:
#line 1366 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7858 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 327:
#line 1370 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 7864 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 328:
#line 1372 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 7870 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 329:
#line 1374 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 7876 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 330:
#line 1376 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7882 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 331:
#line 1378 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7888 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 332:
#line 1382 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7894 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 333:
#line 1383 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7900 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 334:
#line 1386 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7906 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 335:
#line 1387 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7912 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 336:
#line 1388 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7918 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 337:
#line 1389 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7924 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 338:
#line 1390 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7930 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 339:
#line 1391 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7936 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 340:
#line 1392 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 7942 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 341:
#line 1395 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 7948 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 342:
#line 1396 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 7954 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 343:
#line 1400 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7960 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 344:
#line 1402 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7966 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 345:
#line 1404 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7972 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 346:
#line 1409 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7989 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 347:
#line 1422 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 8007 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 348:
#line 1438 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8013 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 349:
#line 1440 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8019 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 350:
#line 1442 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 8025 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 351:
#line 1446 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 8031 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 352:
#line 1447 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 8037 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 353:
#line 1448 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 8043 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 354:
#line 1449 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 8049 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 355:
#line 1450 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 8055 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 356:
#line 1455 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8065 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 357:
#line 1461 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
      context->latestComment = NULL;
    }
#line 8075 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 358:
#line 1467 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8084 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 360:
#line 1476 "chapel.ypp" /* yacc.c:1663  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 8093 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 361:
#line 1484 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 8099 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 362:
#line 1486 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8105 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 363:
#line 1491 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 8111 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 364:
#line 1493 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 8117 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 365:
#line 1495 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 8123 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 366:
#line 1500 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 8129 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 367:
#line 1502 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 8135 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 368:
#line 1504 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8141 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 369:
#line 1510 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8147 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 370:
#line 1511 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 8153 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 371:
#line 1512 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8159 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 372:
#line 1518 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8165 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 373:
#line 1520 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 8171 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 374:
#line 1522 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8179 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 375:
#line 1526 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 8187 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 376:
#line 1530 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8193 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 377:
#line 1532 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8201 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 378:
#line 1536 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8209 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 379:
#line 1543 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8215 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 380:
#line 1544 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8221 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 381:
#line 1545 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8227 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 382:
#line 1546 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8233 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 383:
#line 1547 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8239 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 384:
#line 1552 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8245 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 385:
#line 1553 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8251 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 386:
#line 1554 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 8257 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 387:
#line 1555 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8263 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 388:
#line 1556 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8269 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 389:
#line 1577 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8277 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 390:
#line 1581 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 8285 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 391:
#line 1585 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 8297 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 392:
#line 1593 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8305 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 393:
#line 1599 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8311 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 394:
#line 1600 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8317 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 395:
#line 1601 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8323 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 396:
#line 1606 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8329 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 397:
#line 1608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8335 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 398:
#line 1614 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8341 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 399:
#line 1616 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8347 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 400:
#line 1618 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 8353 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 401:
#line 1622 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8359 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 402:
#line 1623 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8365 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 403:
#line 1624 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8371 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 404:
#line 1625 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8377 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 405:
#line 1626 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8383 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 406:
#line 1632 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8389 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 407:
#line 1633 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8395 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 408:
#line 1634 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8401 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 409:
#line 1635 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8407 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 410:
#line 1639 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 8413 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 411:
#line 1640 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8419 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 412:
#line 1644 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 8425 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 413:
#line 1645 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8431 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 414:
#line 1646 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8437 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 415:
#line 1650 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8443 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 416:
#line 1651 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8449 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 417:
#line 1655 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 8455 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 419:
#line 1660 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8461 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 420:
#line 1661 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8467 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 421:
#line 1665 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 8473 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 422:
#line 1666 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8479 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 423:
#line 1667 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8485 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 424:
#line 1668 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8491 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 425:
#line 1672 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8497 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 426:
#line 1673 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8503 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 427:
#line 1685 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8509 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 428:
#line 1687 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS, (yyvsp[-1].pexpr)); }
#line 8515 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 434:
#line 1697 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 8521 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 435:
#line 1699 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 8527 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 436:
#line 1701 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 8533 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 437:
#line 1703 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 8539 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 438:
#line 1705 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 8545 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 439:
#line 1707 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 8551 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 440:
#line 1709 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 8557 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 441:
#line 1712 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 8563 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 442:
#line 1714 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8569 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 443:
#line 1716 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_unmanaged"); }
#line 8575 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 444:
#line 1718 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 8581 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 445:
#line 1720 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 8587 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 446:
#line 1722 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8593 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 447:
#line 1724 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_borrowed"); }
#line 8599 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 448:
#line 1726 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 8605 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 449:
#line 1731 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8611 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 450:
#line 1733 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8617 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 451:
#line 1735 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8623 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 452:
#line 1737 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8629 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 453:
#line 1739 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8635 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 454:
#line 1741 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8641 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 455:
#line 1743 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8647 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 456:
#line 1745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8653 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 457:
#line 1747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8659 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 458:
#line 1749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8665 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 459:
#line 1751 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8671 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 460:
#line 1753 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8677 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 461:
#line 1755 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8688 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 462:
#line 1762 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8698 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 463:
#line 1768 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8708 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 464:
#line 1774 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8718 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 465:
#line 1780 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 8728 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 466:
#line 1789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8734 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 467:
#line 1798 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 8740 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 475:
#line 1814 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8746 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 476:
#line 1818 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 8752 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 478:
#line 1823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8758 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 479:
#line 1827 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8764 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 480:
#line 1828 "chapel.ypp" /* yacc.c:1663  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8770 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 481:
#line 1832 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8776 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 482:
#line 1836 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8782 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 483:
#line 1837 "chapel.ypp" /* yacc.c:1663  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8788 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 484:
#line 1842 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 8796 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 485:
#line 1846 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8804 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 486:
#line 1850 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8812 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 487:
#line 1856 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 8818 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 488:
#line 1857 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8824 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 489:
#line 1858 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8830 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 490:
#line 1859 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8836 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 491:
#line 1860 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8842 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 492:
#line 1861 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8848 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 494:
#line 1867 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8854 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 495:
#line 1872 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8860 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 496:
#line 1877 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8866 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 505:
#line 1893 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8872 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 506:
#line 1895 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8878 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 507:
#line 1897 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8884 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 508:
#line 1899 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8890 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 509:
#line 1901 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8896 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 510:
#line 1903 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8902 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 511:
#line 1907 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8908 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 512:
#line 1908 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8914 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 513:
#line 1911 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8920 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 514:
#line 1912 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8926 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 515:
#line 1913 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8932 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 520:
#line 1930 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8938 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 521:
#line 1931 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 8944 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 522:
#line 1932 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8950 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 523:
#line 1936 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8956 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 524:
#line 1937 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8962 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 525:
#line 1938 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8968 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 526:
#line 1942 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8974 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 527:
#line 1943 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8980 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 528:
#line 1944 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8986 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 529:
#line 1945 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 8992 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 530:
#line 1946 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 8998 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 531:
#line 1954 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 9004 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 532:
#line 1955 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 9010 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 533:
#line 1956 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 9016 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 534:
#line 1957 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 9022 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 535:
#line 1961 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 9028 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 536:
#line 1962 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 9034 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 538:
#line 1967 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 9040 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 539:
#line 1968 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 9046 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 540:
#line 1969 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 9052 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 541:
#line 1970 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 9058 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 542:
#line 1971 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 9064 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 543:
#line 1972 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 9070 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 544:
#line 1973 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 9076 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 545:
#line 1974 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 9082 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 546:
#line 1975 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 9088 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 547:
#line 1976 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 9094 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 548:
#line 1978 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 9102 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 549:
#line 1982 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 9110 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 550:
#line 1989 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9116 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 551:
#line 1990 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9122 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 552:
#line 1994 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9128 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 553:
#line 1995 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9134 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 554:
#line 1996 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9140 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 555:
#line 1997 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9146 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 556:
#line 1998 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9152 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 557:
#line 1999 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9158 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 558:
#line 2000 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9164 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 559:
#line 2001 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9170 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 560:
#line 2002 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9176 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 561:
#line 2003 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9182 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 562:
#line 2004 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9188 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 563:
#line 2005 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9194 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 564:
#line 2006 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9200 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 565:
#line 2007 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9206 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 566:
#line 2008 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9212 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 567:
#line 2009 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9218 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 568:
#line 2010 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9224 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 569:
#line 2011 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9230 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 570:
#line 2012 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9236 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 571:
#line 2013 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9242 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 572:
#line 2014 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9248 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 573:
#line 2015 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9254 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 574:
#line 2016 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9260 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 575:
#line 2020 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 9266 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 576:
#line 2021 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 9272 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 577:
#line 2022 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 9278 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 578:
#line 2023 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 9284 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 579:
#line 2024 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 9290 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 580:
#line 2025 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9296 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 581:
#line 2026 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 9302 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 582:
#line 2030 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9308 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 583:
#line 2031 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9314 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 584:
#line 2032 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9320 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 585:
#line 2033 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9326 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 586:
#line 2037 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9332 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 587:
#line 2038 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9338 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 588:
#line 2039 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9344 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 589:
#line 2040 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9350 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 590:
#line 2045 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 9356 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 591:
#line 2046 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 9362 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 592:
#line 2047 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 9368 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 593:
#line 2048 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 9374 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 594:
#line 2049 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 9380 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 595:
#line 2050 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 9386 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 596:
#line 2051 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 9392 "bison-chapel.cpp" /* yacc.c:1663  */
    break;


#line 9396 "bison-chapel.cpp" /* yacc.c:1663  */
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
