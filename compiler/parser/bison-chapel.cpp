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
    TNONE = 315,
    TNOTHING = 316,
    TON = 317,
    TONLY = 318,
    TOTHERWISE = 319,
    TOUT = 320,
    TOVERRIDE = 321,
    TOWNED = 322,
    TPARAM = 323,
    TPLUSPLUS = 324,
    TPRAGMA = 325,
    TPRIMITIVE = 326,
    TPRIVATE = 327,
    TPROC = 328,
    TPROTOTYPE = 329,
    TPUBLIC = 330,
    TREAL = 331,
    TRECORD = 332,
    TREDUCE = 333,
    TREF = 334,
    TREQUIRE = 335,
    TRETURN = 336,
    TSCAN = 337,
    TSELECT = 338,
    TSERIAL = 339,
    TSHARED = 340,
    TSINGLE = 341,
    TSPARSE = 342,
    TSTRING = 343,
    TSUBDOMAIN = 344,
    TSYNC = 345,
    TTHEN = 346,
    TTHIS = 347,
    TTHROW = 348,
    TTHROWS = 349,
    TTRUE = 350,
    TTRY = 351,
    TTRYBANG = 352,
    TTYPE = 353,
    TUINT = 354,
    TUNDERSCORE = 355,
    TUNION = 356,
    TUNMANAGED = 357,
    TUSE = 358,
    TVAR = 359,
    TVOID = 360,
    TWHEN = 361,
    TWHERE = 362,
    TWHILE = 363,
    TWITH = 364,
    TYIELD = 365,
    TZIP = 366,
    TALIAS = 367,
    TAND = 368,
    TASSIGN = 369,
    TASSIGNBAND = 370,
    TASSIGNBOR = 371,
    TASSIGNBXOR = 372,
    TASSIGNDIVIDE = 373,
    TASSIGNEXP = 374,
    TASSIGNLAND = 375,
    TASSIGNLOR = 376,
    TASSIGNMINUS = 377,
    TASSIGNMOD = 378,
    TASSIGNMULTIPLY = 379,
    TASSIGNPLUS = 380,
    TASSIGNREDUCE = 381,
    TASSIGNSL = 382,
    TASSIGNSR = 383,
    TBANG = 384,
    TBAND = 385,
    TBNOT = 386,
    TBOR = 387,
    TBXOR = 388,
    TCOLON = 389,
    TCOMMA = 390,
    TDIVIDE = 391,
    TDOT = 392,
    TDOTDOT = 393,
    TDOTDOTDOT = 394,
    TEQUAL = 395,
    TEXP = 396,
    TGREATER = 397,
    TGREATEREQUAL = 398,
    THASH = 399,
    TIO = 400,
    TLESS = 401,
    TLESSEQUAL = 402,
    TMINUS = 403,
    TMOD = 404,
    TNOTEQUAL = 405,
    TOR = 406,
    TPLUS = 407,
    TQUESTION = 408,
    TSEMI = 409,
    TSHIFTLEFT = 410,
    TSHIFTRIGHT = 411,
    TSTAR = 412,
    TSWAP = 413,
    TLCBR = 414,
    TRCBR = 415,
    TLP = 416,
    TRP = 417,
    TLSBR = 418,
    TRSBR = 419,
    TNOELSE = 420,
    TUPLUS = 421,
    TUMINUS = 422
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

#line 451 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 457 "bison-chapel.cpp" /* yacc.c:358  */
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

#line 531 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   16598

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  168
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  138
/* YYNRULES -- Number of rules.  */
#define YYNRULES  588
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1050

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   422

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
     165,   166,   167
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
     728,   729,   733,   734,   735,   736,   737,   738,   739,   740,
     741,   742,   749,   750,   751,   752,   753,   754,   755,   756,
     761,   762,   766,   767,   771,   772,   776,   777,   778,   779,
     780,   781,   782,   783,   787,   788,   792,   793,   794,   795,
     799,   806,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,   817,   818,   819,   820,   821,   822,   823,   824,
     830,   836,   842,   848,   855,   862,   866,   873,   877,   878,
     879,   880,   881,   883,   885,   887,   892,   895,   896,   897,
     898,   899,   900,   904,   905,   909,   910,   911,   915,   916,
     920,   923,   925,   930,   931,   935,   937,   939,   946,   956,
     966,   976,   989,   994,   999,  1007,  1008,  1013,  1014,  1016,
    1021,  1037,  1044,  1053,  1061,  1065,  1072,  1073,  1078,  1083,
    1077,  1110,  1116,  1123,  1129,  1135,  1146,  1152,  1145,  1184,
    1188,  1193,  1197,  1202,  1209,  1210,  1214,  1215,  1216,  1217,
    1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,
    1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,
    1238,  1239,  1240,  1241,  1245,  1246,  1247,  1248,  1249,  1250,
    1251,  1252,  1253,  1254,  1255,  1256,  1260,  1261,  1265,  1269,
    1270,  1271,  1275,  1277,  1279,  1281,  1283,  1288,  1289,  1293,
    1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,  1305,  1306,
    1307,  1308,  1309,  1310,  1314,  1315,  1319,  1320,  1321,  1322,
    1323,  1324,  1328,  1329,  1332,  1333,  1337,  1338,  1342,  1344,
    1349,  1350,  1354,  1355,  1359,  1360,  1362,  1364,  1366,  1371,
    1372,  1375,  1376,  1377,  1378,  1379,  1380,  1381,  1384,  1385,
    1388,  1390,  1392,  1397,  1410,  1427,  1428,  1430,  1435,  1436,
    1437,  1438,  1439,  1443,  1449,  1455,  1463,  1464,  1472,  1474,
    1479,  1481,  1483,  1488,  1490,  1492,  1499,  1500,  1501,  1506,
    1508,  1510,  1514,  1518,  1520,  1524,  1532,  1533,  1534,  1535,
    1536,  1541,  1542,  1543,  1544,  1545,  1565,  1569,  1573,  1581,
    1588,  1589,  1590,  1594,  1596,  1602,  1604,  1606,  1611,  1612,
    1613,  1614,  1615,  1621,  1622,  1623,  1624,  1628,  1629,  1633,
    1634,  1635,  1639,  1640,  1644,  1645,  1649,  1650,  1654,  1655,
    1656,  1657,  1661,  1662,  1673,  1674,  1675,  1676,  1677,  1678,
    1680,  1682,  1684,  1686,  1688,  1690,  1692,  1694,  1696,  1698,
    1700,  1705,  1707,  1709,  1711,  1713,  1715,  1717,  1719,  1721,
    1723,  1725,  1727,  1729,  1736,  1742,  1748,  1754,  1763,  1773,
    1781,  1782,  1783,  1784,  1785,  1786,  1787,  1788,  1793,  1794,
    1798,  1802,  1803,  1807,  1811,  1812,  1816,  1820,  1824,  1831,
    1832,  1833,  1834,  1835,  1836,  1840,  1841,  1846,  1851,  1859,
    1860,  1861,  1862,  1863,  1864,  1865,  1866,  1867,  1869,  1871,
    1873,  1875,  1877,  1879,  1881,  1886,  1887,  1890,  1891,  1892,
    1895,  1896,  1897,  1898,  1909,  1910,  1914,  1915,  1916,  1920,
    1921,  1922,  1923,  1931,  1932,  1933,  1934,  1938,  1939,  1943,
    1944,  1945,  1946,  1947,  1948,  1949,  1950,  1951,  1952,  1953,
    1954,  1958,  1966,  1967,  1971,  1972,  1973,  1974,  1975,  1976,
    1977,  1978,  1979,  1980,  1981,  1982,  1983,  1984,  1985,  1986,
    1987,  1988,  1989,  1990,  1991,  1992,  1993,  1997,  1998,  1999,
    2000,  2001,  2002,  2003,  2007,  2008,  2009,  2010,  2014,  2015,
    2016,  2017,  2022,  2023,  2024,  2025,  2026,  2027,  2028
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
  "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY", "TOTHERWISE",
  "TOUT", "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS", "TPRAGMA",
  "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC", "TREAL",
  "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT",
  "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN",
  "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS", "TTRUE", "TTRY",
  "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE", "TUNION", "TUNMANAGED",
  "TUSE", "TVAR", "TVOID", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD",
  "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR",
  "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND",
  "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY",
  "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL", "TASSIGNSR", "TBANG",
  "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TIO", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOTEQUAL",
  "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT",
  "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR",
  "TNOELSE", "TUPLUS", "TUMINUS", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "module_decl_start",
  "module_decl_stmt", "access_control", "opt_prototype", "block_stmt",
  "stmt_ls", "only_ls", "opt_only_ls", "except_ls", "use_stmt",
  "require_stmt", "assignment_stmt", "opt_label_ident", "ident_fn_def",
  "ident_def", "ident_use", "internal_type_ident_def", "scalar_type",
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
  "actual_expr", "ident_expr", "type_level_expr", "for_expr", "cond_expr",
  "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "shadow_var_prefix", "io_expr",
  "new_expr", "let_expr", "expr", "opt_expr", "opt_try_expr", "lhs_expr",
  "fun_expr", "call_expr", "dot_expr", "parenthesized_expr",
  "bool_literal", "literal_expr", "assoc_expr_ls", "binary_op_expr",
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
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422
};
# endif

#define YYPACT_NINF -911

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-911)))

#define YYTABLE_NINF -539

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -911,   143,  2694,  -911,    -6,  -911,  -911,  -911,  -911,  -911,
    -911,  4146,   151,   285,  -911,  9584,  -911,   151,  9584,  -911,
     271,   221,   285,  4146,  9584,  4146,   187,  -911,  9584,  6686,
    -911,  8066,  8664,  6403,  9584,  -911,   205,  -911,  -911, 16493,
    -911, 12568,  8802,  -911,  9584,  9584,  -911,  -911,  -911,  9584,
    -911,  9584,  -911,  9584,   383,   277,   833,   833,  -911,  -911,
    -911,  8940,  7606,  9584,  8802,  9584,  9584,   360,  -911,   295,
    4146,  -911,  9584,  -911,  9722,  9722, 16493,  -911,  -911,  9584,
    8940,  -911,  -911,  9584,  9584,  -911,  9584,  -911,  9584,  -911,
    -911, 11792,  9584,  -911,  9584,  -911,  -911,  3180,  6824,  8204,
    -911,  3985,  -911,   321,  -911,   408,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911, 16493,  -911, 16493,   259,    51,  -911,
    -911, 12568,  -911,   272,  -911,  -911,  -911,   329,   342,   352,
     347,   349, 16230, 12035,    73,   350,   356,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,   184,  -911,  -911, 16230,   345,  4146,
    -911,  -911,   357,  9584,  9584,  9584,  9584,  9584,  8940,  8940,
     202,   354,  -911,  -911,  -911,   376,   218,  -911,  -911,   355,
   14428, 16493, 12568,  -911,   362,  -911,   207, 16230,   407,  7767,
   16230,  -911,    93,  -911, 16493,   154,   193, 16493,   358,    49,
   13947,    50, 13906,   193,  9584,  -911, 14331, 13151,  7767,  -911,
    -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,
    -911,  4146,  -911,   359,  2503,    90,     7,  -911,  4146,  -911,
    -911, 14090,   510,   376, 14090,   376,   510,  -911,  7767,  9584,
    -911,  -911, 16493,  -911,   254, 16230,  9584,  9584,  -911, 16230,
     363, 14278,  -911, 14090,   376, 16230,   365,  7767,  -911, 16230,
   14664,  -911,  -911, 14721,  2564,  -911,  -911, 14807,   409,   368,
     376,   256, 14481, 14090, 14864,   157,   157, 13026,   510,   510,
     -44,  -911,  -911,  3341,   -32,  -911,  9584,  -911,   -31,    64,
    -911,   -19,    65, 14950,   -53,   530,  -911,  3502,  -911,   486,
     411,   390,  -911,  -911,  -911,   282,  -911,  -911,  9584,  9584,
    9584,  9584,  8664,  8664,  9584,  -911,  9584,  9584,  9584,  9860,
    9584,   249, 11792,  9584,  9584,  9584,  9584,  9584,  9584,  9584,
    9584,  9584,  9584,  9584,  9584,  9584,  9584,  9584,  -911,  -911,
    -911,  -911,  -911,  -911,  8365,  8365,  -911,  -911,  -911,  -911,
    8365,  -911,  -911,  8365,  8365,  7767,  7767,  8664,  8664,  7307,
    -911,  -911, 14578, 14624, 14904,     4,  3663,  -911,  8664,    49,
     397,   283,  -911,  9584,  -911,  9584,   441,  -911,   395,   424,
    -911,  -911,   406, 16493, 12568,   519,  8940,  -911,  4307,  8664,
    -911,   401,  -911,    49,  4468,  8664,  -911,    49,   193,  9998,
    9584,  -911,  4146,   532,  9584,   403,  -911,   142,  -911,  -911,
    2503,  -911,   431,   410,  -911, 10136,   464,  9584, 12568,  -911,
    -911,  -911,   412,  -911,  8940,  -911, 16230, 16230,  -911,    38,
    -911,  7767,   417,  -911,   563,  -911,   563,  -911, 10274,   447,
    -911,  -911,  9998,  9584,  -911,  -911,  -911,  -911,  -911,  8503,
    -911, 12985,  6985,  -911,  7146,  -911,  4146,   420,  8664,  5917,
    3019,   421,  9584,  6241,  -911,  -911,   145,  -911,  3824, 16493,
    8940,   427, 16436,   203,  -911, 15047,  2256,  2256,   361,  -911,
     361,  -911,   361, 14045,  1579,  1374,   975, 12170, 12170,   376,
     157,  -911,  -911,  -911,  -911, 13026, 14180,   361,  1539,  1539,
    2256,  1539,  1539,  1222,   157, 14180,  2493,  1222,   510,   510,
     157,   433,   435,   437,   445,   446,   442,   443,  -911,   361,
    -911,   361,    29,  -911,  -911,  -911,   106,  -911,  1494, 16279,
     353, 10412,  8664, 10550,  8664,  9584,  8664, 12398,   151, 15100,
    -911,  -911,  -911, 16230, 15140,  7767,  -911,  7767,  -911,   411,
     292,  9584,   108,  9584, 16230,    60, 14135,  7307,  -911,  9584,
   16230,    56, 13992,  -911,   452,   473,   457, 15197,   473,   458,
     585, 15283,  4146, 14233,  -911,    55,  -911,  -911,  -911,  -911,
    -911,  -911,   629,   118,  -911, 12625,  -911,   208,   455,  2503,
      90,  9584,   187,   205,  9584,  9584,  9584,  9584,  9584,  5756,
    -911,  -911,   466,  7928,  -911, 16230,  -911,  -911,  -911, 16230,
     461,    12,   456,  -911,  1358,  -911,  -911,   317, 16493,   469,
     470,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  4146,   111,
   12748,  -911,  -911, 16230,  4146, 16230,  -911, 15340,  -911,  -911,
    -911,   491, 12472,   467,   515,   114,  -911,   551,  -911,  -911,
    -911,  -911,  6527,  -911,  -911,  -911,  -911,  -911,  -911,  -911,
    -911,  -911,  7307,  -911,    27,  8664,  8664,  9584,   602, 15372,
    9584,   606, 15515,   472, 12835,    49,    49,  -911,  -911,  -911,
    -911,   478,  -911, 14090,  -911, 13231,  4629,  -911,  4790,  -911,
     122,  -911, 13307,  4951,  -911,    49,  5112,  -911,    49,  9584,
    -911,  9584,  -911,  4146,  9584,  -911,  4146,   607,  -911,  -911,
   16493,   796,  -911,  2503,   506,   562,  -911,  -911,  -911,    57,
    -911,  -911,   464,   481,    32,  -911,  -911,  -911,  5273,  8940,
    -911,  -911,  -911, 16493,  -911,   508,   355,  -911,  -911,  -911,
    -911,  5434,   485,  5595,   490,  -911,  9584,   496,   686,  -911,
    9584, 16493,  -911,  -911,   208,   489,   239,  -911,   521,  -911,
     523,   533,   526,   535,  -911,   536,   538,   539,   542,   326,
     541,   543,   547,  -911,  -911, 16493,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,  -911,  9584,  -911,   557,   559,   552,
     489,   489,  -911,  -911,   464,   130,   211, 15483, 10688, 10826,
   15626, 10964, 11102, 11240, 11378,  -911,  -911, 12533,  -911,  4146,
    9584, 16230,  9584, 16230,  7307,  -911,  4146,  9584, 16230,  -911,
    9584, 16230,  -911, 15658, 16230,  -911, 16230,   659,  4146,   506,
    -911,   534,  9101,   223,  -911,    95,  -911,  -911,  8664, 12284,
    4146,  -911,    71,   550,  9584,  -911,  9584,  -911, 16230,  4146,
    9584,  -911, 16230,  4146, 16230,  -911,  7468,   336,   336,  -911,
    2037,  -911, 16230,  -911,    69,   142,  -911,  -911, 13075,  -911,
   12692,  -911,  -911,  -911,  9584,  9584,  9584,  9584,  9584,  9584,
    9584,  9584,   544,  1117, 15283, 13383, 13459,  -911, 15283, 13535,
   13611,  9584,  4146,  -911,   464,   506,  6079,  -911,  -911,  -911,
     128,  8940,  -911,  -911,    54,  9584,   -45, 12892,  -911,   762,
     407,  -911,   355, 16230, 13687,  -911, 13763,  -911,  -911,  -911,
   11516,   619,   216,  -911,   572,   587,   489,   489, 15769, 15801,
   15912, 15944, 16055, 16087,  -911,  -911,  4146,  4146,  4146,  4146,
   16230,  -911,  -911,   223,  9262,    83,  -911,  -911, 16230,  -911,
      59,  -911,   -12,  -911,   624, 16198,  -911,  -911,  -911, 11378,
    -911,  4146,  4146,  2855,  -911,  -911,   325,  -911,    95,  -911,
    -911,  -911,  9584,  9584,  9584,  9584,  9584,  9584, 15283, 15283,
   15283, 15283,  -911,  -911,  -911,  -911,  -911,   134,  8664, 11926,
    -911,  9584,    54,    59,    59,    59,    59,    59,    59,    54,
     797, 15283, 15283,   553, 11654,    87,   105, 12932,  -911,  -911,
   16230,  -911,  -911,  -911,  -911,  -911,  -911,  -911,   579,  -911,
    -911,   340, 12040,  -911,  -911,  -911,  9423,  -911,   460,  -911
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   530,   531,   532,   533,
     534,     0,   468,    70,   102,     0,   202,   468,     0,   107,
       0,   349,    70,     0,     0,     0,     0,   212,   505,   505,
     527,     0,     0,     0,     0,   106,     0,   222,   103,     0,
     218,     0,     0,   109,     0,     0,   459,   535,   110,     0,
     225,     0,   348,     0,     0,     0,    45,    44,   105,   203,
     350,     0,     0,     0,     0,     0,     0,     0,   108,     0,
       0,    83,     0,   528,     0,     0,     0,   104,   204,     0,
       0,   352,   111,     0,     0,   584,     0,   586,     0,   587,
     588,   504,     0,   585,   582,   126,   583,     0,     0,     0,
       4,     0,     5,     0,     9,    46,    10,    11,    12,    14,
     422,   423,    22,    13,   127,   133,    15,    17,    16,    19,
      20,    21,    18,   132,     0,   130,     0,   494,     0,   134,
     131,     0,   135,   510,   490,   491,   426,   424,     0,     0,
     495,   496,     0,   425,     0,   511,   512,   513,   529,   489,
     428,   427,   492,   493,     0,    38,    24,   434,     0,     0,
     469,    71,     0,     0,     0,     0,     0,     0,     0,     0,
     494,   510,   424,   495,   496,   436,   425,   511,   512,     0,
     468,     0,     0,   351,     0,   176,     0,   407,     0,   414,
     506,   223,   533,   140,     0,     0,   224,     0,     0,     0,
       0,     0,     0,     0,   505,   139,     0,     0,   414,    77,
      84,    95,    89,    97,    79,    88,    98,    85,    99,   100,
      93,    87,    94,    92,    90,    91,    78,    80,    86,    96,
     101,     0,    81,     0,     0,     0,     0,   356,     0,   123,
      32,     0,   569,   487,     0,   437,   570,     7,   414,   505,
     129,   128,   319,   404,     0,   403,     0,     0,   124,   509,
       0,     0,    35,     0,   439,   429,     0,   414,    36,   435,
       0,   183,   179,     0,   425,   183,   180,     0,   345,     0,
     438,     0,   403,     0,     0,   571,   573,   503,   568,   567,
       0,    48,    51,     0,     0,   409,     0,   411,     0,     0,
     410,     0,     0,   403,     0,     0,     6,     0,    47,     0,
     205,     0,   305,   304,   226,     0,   440,    23,     0,     0,
       0,     0,     0,     0,     0,   572,     0,     0,     0,     0,
       0,     0,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,   271,
     272,   273,   268,   270,     0,     0,   266,   269,   267,   265,
       0,   275,   274,     0,     0,   414,   414,     0,     0,     0,
      25,    26,     0,     0,     0,     0,     0,    27,     0,     0,
       0,     0,    28,     0,    29,     0,   422,   420,     0,   415,
     416,   421,     0,     0,     0,     0,     0,   149,     0,     0,
     148,     0,   157,     0,     0,     0,   155,     0,     0,     0,
      57,   136,     0,   169,     0,     0,    30,   287,   219,   360,
       0,   361,   363,     0,   385,     0,   366,     0,     0,   122,
      31,    33,     0,   318,     0,    64,   507,   508,   125,     0,
      34,   414,     0,   190,   181,   177,   182,   178,     0,   343,
     340,    61,     0,    57,   142,    37,    50,    49,    52,     0,
     536,     0,     0,   523,     0,   525,     0,     0,     0,     0,
       0,     0,     0,     0,   540,     8,     0,    40,     0,     0,
       0,     0,     0,   298,   355,   467,   564,   563,   566,   575,
     574,   579,   578,   560,   557,   558,   559,   499,   500,   498,
     547,   521,   522,   520,   519,   501,   551,   562,   556,   554,
     565,   555,   553,   545,   550,   552,   561,   544,   548,   549,
     546,     0,     0,     0,     0,     0,     0,     0,   577,   576,
     581,   580,   479,   480,   482,   484,     0,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   538,   468,   468,
     145,   341,   353,   408,     0,     0,   431,     0,   342,   205,
       0,     0,     0,     0,   443,     0,     0,     0,   158,     0,
     449,     0,     0,   156,   583,    60,     0,    53,    58,     0,
     168,     0,     0,     0,   430,   292,   289,   290,   291,   295,
     296,   297,   287,     0,   280,     0,   288,   306,     0,   364,
       0,   116,   118,   119,   114,   115,   113,   112,   117,     0,
     384,   383,   490,     0,   358,   488,   357,   518,   406,   405,
       0,     0,     0,   432,     0,   184,   347,   490,     0,     0,
       0,   537,   497,   524,   412,   526,   413,   165,     0,     0,
       0,   539,   163,   453,     0,   542,   541,     0,    42,    41,
      39,   206,     0,     0,   216,     0,   213,   302,   299,   300,
     303,   227,     0,    68,    69,    67,    66,    65,   516,   517,
     481,   483,     0,   470,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   141,   418,   419,
     417,     0,   354,     0,   167,     0,     0,   147,     0,   146,
       0,   474,     0,     0,   153,     0,     0,   151,     0,     0,
     137,     0,   138,     0,     0,   171,     0,   173,   293,   294,
       0,   287,   278,     0,   398,   307,   310,   309,   311,     0,
     362,   365,   366,     0,     0,   367,   368,   192,     0,     0,
     191,   194,   433,     0,   185,   188,     0,   344,    62,    63,
     166,     0,     0,     0,     0,   164,     0,     0,   221,   211,
       0,   214,   210,   301,   306,   276,    72,   259,    84,   257,
      89,    74,    88,    85,   262,    99,   100,    87,    90,    73,
      75,    86,   101,   256,   238,   241,   239,   240,   251,   242,
     255,   247,   245,   258,   261,   246,   244,   249,   254,   243,
     248,   252,   253,   250,   260,     0,   236,     0,    76,     0,
     276,   276,   234,   472,   366,   510,   510,     0,     0,     0,
       0,     0,     0,     0,     0,   144,   143,     0,   150,     0,
       0,   442,     0,   441,     0,   473,     0,     0,   448,   154,
       0,   447,   152,    55,    54,   170,   458,   172,     0,   398,
     281,     0,     0,   366,   308,   324,   359,   389,     0,   538,
       0,   196,     0,     0,     0,   186,     0,   161,   455,     0,
       0,   159,   454,     0,   543,   200,   505,   221,   221,   198,
     221,   208,   217,   215,     0,   287,   233,   237,     0,   263,
       0,   229,   230,   476,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   221,   446,     0,     0,   475,   452,     0,
       0,     0,     0,   175,   366,   398,     0,   401,   400,   402,
     490,   320,   284,   282,     0,     0,     0,     0,   387,   490,
     197,   195,     0,   189,     0,   162,     0,   160,   209,   380,
       0,   312,     0,   235,    72,    73,   276,   276,     0,     0,
       0,     0,     0,     0,   201,   199,     0,     0,     0,     0,
      56,   174,   283,   366,   390,     0,   321,   323,   322,   338,
       0,   339,   326,   329,     0,   325,   316,   317,   220,     0,
     187,     0,     0,     0,   379,   378,   490,   313,   324,   277,
     231,   232,     0,     0,     0,     0,     0,     0,   445,   444,
     451,   450,   286,   285,   392,   393,   395,   490,     0,   538,
     337,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   457,   456,     0,   370,     0,     0,     0,   394,   396,
     328,   330,   331,   334,   335,   336,   332,   333,   327,   375,
     373,   490,   538,   314,   228,   315,   390,   374,   490,   397
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -911,  -911,  -911,     0,  -376,  1799,  -911,  -911,  -911,  -911,
     369,   415,  -304,   266,   279,  -911,  -911,   400,   701,  -911,
    1163,  -152,  -636,  -911,  -820,  -911,  1850,  -896,  -711,   -47,
    -911,  -911,  -911,    61,  -911,  -911,  -911,   449,  -911,    -4,
    -911,  -911,  -911,  -911,  -911,   537,   175,   -92,  -911,  -911,
    -911,   -20,  1016,  -911,  -911,  -911,  -911,  -911,  -911,  -911,
    -911,  -147,  -137,  -766,  -911,  -141,    25,   155,  -911,  -911,
    -911,   -13,  -911,  -911,  -271,   316,  -911,  -204,  -226,  -251,
    -243,  -525,  -911,  -178,  -911,    -8,   740,  -116,   346,  -911,
    -409,  -713,  -722,  -911,  -552,  -447,  -910,  -904,  -782,   -56,
    -911,     8,  -911,  -183,  -911,   220,   384,  -331,  -911,  -911,
    1083,  -911,    -9,  -911,  -911,  -175,  -911,  -553,  -911,  -911,
    1165,  1511,   -11,     1,   113,   632,  -911,  1522,  1604,  -911,
    -911,  -911,  -911,  -911,  -911,  -911,  -911,  -334
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   292,   101,   580,   103,   104,   105,   309,
     106,   293,   575,   579,   576,   107,   108,   109,   162,   806,
     235,   110,   232,   111,   610,   195,   240,   112,   113,   114,
     115,   116,   117,   683,   118,   119,   120,   444,   625,   746,
     121,   122,   621,   741,   123,   124,   481,   758,   125,   126,
     655,   656,   170,   233,   597,   128,   129,   483,   764,   661,
     809,   810,   414,   886,   418,   593,   594,   595,   596,   662,
     314,   729,   988,  1044,   978,   253,   967,   922,   926,   972,
     973,   974,   130,   279,   449,   131,   132,   236,   237,   422,
     423,   614,   985,   941,   426,   611,  1005,   919,   853,   294,
     186,   298,   299,   388,   389,   390,   171,   134,   135,   136,
     172,   138,   159,   160,   536,   403,   700,   537,   538,   139,
     173,   174,   142,   196,   391,   176,   144,   177,   178,   147,
     148,   149,   304,   150,   151,   152,   153,   154
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     157,   626,   100,   380,   175,   254,   364,   180,   179,   250,
     251,   598,   182,   187,   701,   315,   392,   190,   190,   856,
     200,   202,   206,   207,   281,   415,   808,   407,   424,   191,
     977,   241,   917,   242,   243,   540,    62,   386,   244,   620,
     245,   592,   246,   302,   891,   892,   546,   881,   732,   427,
     255,   259,   261,   263,   264,   265,   386,   969,   424,   269,
    1006,   270,   969,   273,   277,   432,   381,   914,   280,   282,
     939,   670,   283,   284,   858,   285,   738,   286,   238,   238,
     287,   288,   473,   289,   442,   703,   255,   259,   303,   696,
     401,   424,   199,   201,   612,  1011,   386,   718,   312,  1028,
     238,   893,  -193,   459,   462,  1029,   578,   468,   671,  -381,
     155,   474,  -121,   375,   376,   386,   456,   627,   739,   813,
     984,  -376,   814,  1012,   313,  1008,   467,   471,   460,   546,
     977,   463,  -381,   963,   719,   970,  1049,   364,  -381,   469,
     923,  -381,   428,     3,  -193,   466,   971,   924,   155,   578,
    -376,   971,   157,   372,   373,   374,   269,   255,   303,   401,
     301,   425,  -381,  -376,  -381,   401,   585,   469,   547,   938,
    -121,   379,   740,    16,   401,   260,  -376,   855,   259,   504,
     990,   991,   526,   527,   586,   321,    62,   587,   394,  -381,
     731,   425,   881,   190,  -121,   182,   859,   259,  -193,   464,
     469,   962,   925,   940,  -381,   408,   434,   588,   376,   376,
     589,   300,    54,   386,   386,   376,  -381,    21,   469,   376,
     401,   590,   469,  -376,   425,  -381,   465,   657,  -376,   470,
     376,    59,   725,   540,   365,   322,   366,   259,   190,   323,
     591,   672,  -399,   434,  -381,   436,   437,  1009,  -391,   761,
    1003,  1042,     5,   721,   808,    78,   259,   834,   622,  1043,
     158,    52,   367,  -399,   376,  -478,   368,  -399,   673,  -391,
     694,   658,    60,  -391,   762,   751,   726,  -279,   560,   501,
     722,   907,   659,   316,   835,   461,   325,   727,   161,   386,
    -399,   329,  -478,   458,   331,    21,  -391,    81,   334,   155,
     183,   660,  1040,   502,  -279,   648,   728,   485,   486,   487,
     488,   490,   492,   493,   851,   494,   495,   496,   499,   500,
    1047,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   517,   518,   519,   520,   613,   540,    52,
     562,    71,   383,   259,   259,   592,  -477,   503,   189,   259,
      60,   721,   259,   259,   259,   259,   529,   531,   539,   203,
      21,   384,   921,  -515,   316,  -515,   208,   549,   -77,   181,
      28,   249,   553,  -477,   554,    81,   -82,  -377,   989,  -514,
      37,  -514,   880,   489,   491,   255,   133,   564,   566,   434,
     247,   434,  -369,   570,   572,   133,   705,   708,   577,   577,
     394,   581,    50,   583,    52,   386,  -377,   133,   435,   133,
     451,   239,   928,  -464,   297,    60,   615,   428,   428,  -377,
    -515,  -369,  -515,   619,   651,   316,  -461,   428,   528,   530,
     259,   676,  -377,   239,  -369,   368,   484,   552,   248,   548,
      81,   577,   577,   271,   275,  1010,   692,  -369,   619,   266,
     747,   259,  -346,   259,   133,   -78,   267,   640,   619,   643,
     565,   645,   647,   -83,   752,   754,   571,   521,   522,   255,
     634,  -346,   636,   523,   272,   276,   524,   525,   458,  -377,
     307,   133,   308,  -460,  -377,   133,   175,   280,  1032,  1033,
    1034,  1035,  1036,  1037,  -369,   329,   317,   318,   331,  -369,
     540,  -465,   334,  -466,  -463,   387,   369,   316,  -382,   592,
    -462,   371,  -371,   331,   376,   385,   382,   438,  -382,   396,
     417,   920,   450,   448,   387,   811,   441,   880,   929,   639,
     564,   679,   570,   682,   581,   684,   643,   475,   321,   685,
     686,  -371,   479,   133,   259,   480,   259,  -382,   377,   482,
     693,   551,   695,   734,  -371,   555,   539,   556,   702,   557,
     558,   561,   567,   582,   387,   584,   599,  -371,   239,   239,
     239,   239,   600,  -382,   617,   300,   413,   300,   613,   623,
    -382,   624,   628,   387,   638,   644,   652,   663,   322,   664,
     175,   665,   323,   245,   264,   265,   269,   280,   303,   666,
     667,  -382,   259,   678,   668,   681,   -59,   669,   709,   986,
     239,   710,   712,   239,  -371,   133,   713,   730,   742,  -371,
    -382,   737,   133,   748,   749,  -382,   434,   759,  -382,   760,
     763,   818,   239,  1007,   143,   821,   823,   827,   848,   325,
     852,   854,   864,   143,   329,   857,   330,   331,  1020,   869,
     885,   334,   239,   585,   873,   143,   875,   143,  -102,   341,
    -107,   539,   -79,  -106,   490,   529,   817,   347,   689,   820,
     -80,   586,  -103,  -109,   587,  -110,  -105,   133,  1007,  -108,
    -104,   387,   387,   862,  -111,   831,   889,   833,   -81,   890,
     912,   133,   838,  1041,   588,   841,   915,   589,   843,   305,
     844,   -77,   143,   846,   954,    16,   274,   274,   590,    20,
      21,  1048,   932,   987,  1012,  1007,   -78,  1039,    27,   630,
      28,   876,   478,   184,   446,    33,   736,   591,   255,   143,
      37,   629,   393,   143,   691,   903,   489,   528,  1013,   863,
     868,   883,   872,   946,   942,   874,   850,   720,   239,   882,
     618,   884,    50,   947,    52,  1045,    54,   387,   877,  1002,
     133,   878,  1026,    59,  1014,    60,  1015,  1016,  1038,  1031,
    1017,  1018,   239,   205,   616,   618,   239,   690,   297,     0,
     297,     0,   133,     0,    76,   618,     0,    78,   133,     0,
      81,   143,     0,     0,   888,     0,   133,     0,     0,     0,
       0,     0,     0,     0,  -386,     0,     0,   831,   833,     0,
     838,   841,   868,   872,  -386,     0,     0,     0,   904,   905,
     585,   906,     0,   539,     0,   908,   909,     0,     0,   910,
     250,   251,     0,     0,     0,     0,     0,     0,   586,  -388,
      95,   587,     0,  -386,     0,     0,   879,   927,   643,  -388,
     133,     0,     0,   933,   133,   934,   203,    21,     0,   936,
     965,   588,   133,   143,   589,   190,    54,    28,   249,  -386,
     143,   688,     0,   387,     0,   590,  -386,    37,  -388,     0,
    -221,     0,     0,   904,   948,   949,   908,   950,   951,   952,
     953,     0,     0,     0,   591,     0,     0,  -386,     0,    50,
     960,    52,     0,     0,  -388,   303,  -221,     0,     0,     0,
     968,  -388,    60,     0,   975,     0,  -386,     0,     0,     0,
       0,  -386,   674,     0,  -386,   143,     0,  1025,     0,     0,
       0,     0,  -388,     0,   239,   239,     0,    81,     0,   143,
     239,   239,     0,     0,     0,   998,   999,  1000,  1001,     0,
       0,  -388,   717,     0,     0,     0,  -388,     0,     0,  -388,
       0,     0,     0,     0,     0,     0,   133,     0,   872,     0,
    1021,  1022,   303,     0,     0,     0,     0,     0,     0,     0,
       0,   998,   999,  1000,  1001,  1021,  1022,     0,     0,     0,
       0,     0,     0,   744,     0,     0,     0,  1027,   643,     0,
    1030,     0,     0,   321,     0,     0,     0,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   127,     0,
       0,     0,   133,     0,     0,     0,     0,   127,   133,     0,
     143,   643,     0,     0,     0,   872,   143,     0,     0,   127,
       0,   127,     0,     0,   143,     0,   812,     0,     0,     0,
       0,     0,     0,   322,   239,   239,     0,   323,     0,   815,
     816,     0,   239,     0,   413,     0,     0,     0,     0,     0,
       0,   413,     0,     0,   239,     0,     0,   239,     0,     0,
     133,     0,   133,     0,     0,   137,   127,   133,     0,     0,
     133,     0,     0,     0,   137,     0,     0,   133,   143,     0,
     133,     0,   143,     0,   325,   326,   137,     0,   137,   329,
     143,   330,   331,   127,     0,   865,   334,   127,     0,     0,
       0,     0,   133,     0,   341,     0,     0,     0,     0,     0,
     345,   346,   347,     0,     0,   133,    16,   133,     0,     0,
      20,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,    28,   876,   137,     0,     0,    33,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,   140,   918,     0,
       0,     0,     0,     0,     0,   127,   140,     0,     0,     0,
     137,     0,     0,    50,   137,    52,     0,    54,   140,   877,
     140,     0,   878,     0,    59,     0,    60,     0,     0,     0,
       0,     0,   231,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   133,   143,    76,     0,     0,    78,     0,
     133,    81,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   239,   133,     0,     0,   140,     0,   966,     0,   278,
       0,     0,   137,     0,   133,     0,     0,   127,     0,     0,
     321,     0,     0,   133,   127,     0,     0,   133,     0,     0,
       0,     0,   140,     0,     0,     0,   140,     0,     0,     0,
     143,    95,     0,     0,   413,   413,   143,   955,   413,   413,
    1004,     0,     0,     0,     0,     0,     0,   310,     0,   311,
       0,     0,     0,     0,     0,   976,   133,     0,     0,     0,
     322,   980,     0,   413,   323,   413,     0,     0,     0,   127,
       0,     0,     0,     0,   137,     0,     0,     0,     0,     0,
       0,   137,     0,   127,   140,  1004,     0,     0,   143,     0,
     143,     0,     0,     0,     0,   143,     0,     0,   143,     0,
     133,   133,   133,   133,   278,   143,     0,     0,   143,     0,
       0,   325,   326,     0,   327,   328,   329,   278,   330,   331,
     395,   209,  1004,   334,     0,   133,   133,     0,     0,     0,
     143,   341,     0,   210,   211,     0,   137,   345,   346,   347,
     212,     0,     0,   143,     0,   143,     0,     0,   213,     0,
     137,     0,   127,     0,   214,   976,   140,   421,     0,   215,
       0,   216,   321,   140,   217,     0,     0,     0,     0,     0,
       0,     0,   218,     0,   127,   433,     0,     0,     0,   219,
     127,     0,     0,     0,     0,   220,     0,     0,   127,     0,
       0,     0,     0,     0,   221,     0,     0,     0,     0,     0,
       0,     0,     0,   222,   223,     0,   224,     0,   225,     0,
     226,     0,   322,   227,     0,     0,   323,   228,   140,   137,
     229,   143,     0,   230,     0,     0,     0,     0,   143,     0,
       0,     0,   140,     0,     0,     0,     0,     0,     0,     0,
     143,   137,   127,     0,     0,     0,   127,   137,     0,     0,
       0,     0,   143,     0,   127,   137,     0,     5,     0,     0,
       0,   143,     0,   325,   326,   143,     0,   328,   329,    14,
     330,   331,     0,   141,     0,   334,    19,   376,     0,   743,
       0,     0,   141,   341,   145,     0,     0,     0,     0,   345,
     346,   347,     0,   145,   141,    35,   141,     0,     0,     0,
      38,   140,     0,     0,   143,   145,     0,   145,    43,   137,
       0,     0,     0,   137,     0,    48,   559,     0,     0,     0,
       0,   137,     0,   140,     0,     0,     0,   321,     0,   140,
      58,     0,     0,     0,     0,     0,     0,   140,     0,     0,
       0,   141,    68,   421,     0,     0,    71,     0,   143,   143,
     143,   143,   145,    77,     0,     0,     0,     0,   127,    82,
       0,     0,     0,     0,     0,     0,   146,   321,   141,     0,
       0,     0,   141,   143,   143,   146,     0,   322,     0,   145,
       0,   323,     0,   145,     0,     0,     0,   146,     0,   146,
       0,   140,     0,     0,     0,   140,     0,     0,     0,     0,
       0,     0,   650,   140,     0,   654,     0,     0,     0,     0,
       0,     0,     0,     0,   127,     0,     0,   322,     0,     0,
     127,   323,     0,     0,     0,   137,     0,     0,   325,   326,
     141,   327,   328,   329,   146,   330,   331,   332,     0,     0,
     334,   145,     0,     0,     0,     0,     0,   340,   341,     0,
       0,   344,     0,     0,   345,   346,   347,     0,     0,     0,
       0,   146,     0,     0,     0,   146,     0,     0,   325,     0,
       0,     0,   127,   329,   127,   330,   331,     0,     0,   127,
     334,   137,   127,     0,     0,     0,     0,   137,   341,   127,
       0,     0,   127,     0,   345,   346,   347,     0,     0,     0,
       0,     0,   141,     0,     0,     0,     0,   140,     0,   141,
       0,     0,     0,   145,   127,     0,     0,     0,   724,     0,
     145,     0,   421,   146,     0,     0,     0,   127,     0,   127,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   137,
       0,   137,     0,     0,     0,     0,   137,   745,     0,   137,
       0,   278,     0,     0,     0,     0,   137,     0,     0,   137,
       0,   102,     0,   140,   141,     0,     0,     0,     0,   140,
     156,     0,     0,     0,     0,   145,     0,     0,   141,     0,
       0,   137,   185,     0,   188,   807,     0,     0,     0,   145,
       0,     0,     0,     0,   137,   146,   137,     0,     0,     0,
       0,     0,   146,     0,     0,   127,     0,     0,     0,     0,
       0,     0,   127,     0,     0,     0,     0,     0,     0,     0,
       0,   140,     0,   140,   127,     0,     0,     0,   140,   268,
       0,   140,     0,     0,     0,     0,   127,     0,   140,     0,
       0,   140,     0,   849,     0,   127,   421,   141,     0,   127,
       0,     0,     0,     0,     0,     0,   102,   146,   145,     0,
     306,     0,     0,   140,     0,     0,   745,     0,     0,   141,
       0,   146,   137,     0,   262,   141,   140,     0,   140,   137,
     145,     0,     0,   141,   654,     0,   145,     0,   127,     0,
       0,   137,     0,     0,   145,     0,     0,     0,     0,     0,
       0,     0,     0,   137,     0,     0,     0,     0,   887,     0,
       0,     0,   137,     0,     0,     0,   137,     0,   370,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   127,   127,   127,   127,     0,   141,     0,     0,
     146,   141,     0,     0,     0,     0,     0,     0,   145,   141,
       0,     0,   145,     0,   140,   137,     0,   127,   127,     0,
     145,   140,   146,     0,     0,     0,     0,     0,   146,     0,
       0,     0,     0,   140,     0,     0,   146,     0,     0,     0,
       0,     0,     0,     0,     0,   140,     0,     0,     0,     0,
     416,     0,     0,     0,   140,     0,     0,   429,   140,   137,
     137,   137,   137,     0,     0,     0,     0,     0,     0,   397,
     400,   402,   406,   807,     0,     0,    16,     0,     0,     0,
      20,    21,     0,     0,   137,   137,     0,     0,     0,    27,
     146,    28,   876,     0,   146,     0,    33,   140,     0,     0,
       0,    37,   146,     0,     0,     0,     0,     0,     0,     0,
       0,   430,   102,   141,   431,     0,     0,     0,     0,     0,
       0,     0,     0,    50,   145,    52,   102,   305,     0,   877,
       0,     0,   878,   440,    59,     0,    60,     0,     0,     0,
       0,   140,   140,   140,   140,     0,     0,     0,     0,     0,
       0,     0,     0,   454,     0,    76,     0,     0,    78,     0,
       0,    81,     0,     0,     0,     0,   140,   140,     0,   141,
       0,     0,     0,     0,     0,   141,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,   145,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   146,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,   429,     0,     0,
       0,     0,     0,   429,     0,     0,     0,   141,     0,   141,
       0,     0,     0,     0,   141,     0,     0,   141,   145,     0,
     145,     0,     0,     0,   141,   145,     0,   141,   145,   550,
       0,     0,     0,     0,     0,   145,     0,     0,   145,     0,
       0,     0,   146,     0,     0,     0,     0,     0,   146,   141,
       0,     0,     0,   568,     0,     0,     0,   573,     0,     0,
     145,     0,   141,     0,   141,   637,     0,     0,     0,   642,
       0,     0,     0,   145,     0,   145,     0,   102,     0,     0,
       0,     0,     0,     0,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     146,     0,   146,     0,     0,     0,     0,   146,     0,     0,
     146,     0,     0,     0,     0,     0,     0,   146,     0,     0,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   322,     0,     0,     0,   323,     0,
     141,     0,   146,     0,     0,     0,     0,   141,     0,     0,
       0,   145,     0,     0,     0,   146,     0,   146,   145,   141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
     145,   141,     0,     0,     0,     0,     0,     0,     0,     0,
     141,   715,   145,     0,   141,   325,   326,     0,   327,   328,
     329,   145,   330,   331,   332,   145,   333,   334,   335,   336,
       0,     0,   338,   339,   340,   341,   342,   343,   344,     0,
       0,   345,   346,   347,     0,   697,   699,     0,     0,     0,
       0,   704,   707,   141,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   146,   145,     0,     0,   750,     0,     0,
     146,     0,     0,   755,     0,     0,     0,     0,     0,     0,
       0,     0,   146,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   146,     0,     0,   141,   141,   141,
     141,     0,     0,   146,     0,     0,     0,   146,   145,   145,
     145,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   141,   141,     0,   429,     0,   429,     0,     0,
       0,     0,   429,   145,   145,   429,   209,     0,     0,     0,
       0,     0,   845,     0,     0,   847,   146,     0,   210,   211,
       0,   321,     0,     0,     0,   212,     0,     0,     0,     0,
       0,     0,     0,   213,     0,   825,   826,   861,     0,   214,
       0,     0,     0,   828,   215,     0,   216,     0,     0,   217,
     867,     0,   871,     0,     0,   839,     0,   218,   842,     0,
     146,   146,   146,   146,   219,     0,     0,     0,     0,     0,
     220,   322,     0,     0,     0,   323,     0,     0,     0,   221,
       0,     0,     0,     0,     0,   146,   146,     0,   222,   223,
       0,   224,     0,   225,     0,   226,     0,     0,   227,     0,
       0,     0,   228,   419,     0,   229,   324,     0,   230,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   325,   326,     0,   327,   328,   329,     0,   330,
     331,   332,     0,   333,   334,   335,   336,     0,     0,   338,
     339,   340,   341,   342,     0,   344,     0,   913,   345,   346,
     347,     0,     0,     0,     0,     0,     0,     0,     0,   930,
       0,     0,     0,     0,   420,     0,     0,     0,   935,     0,
       0,     0,   937,     0,     0,     0,     0,     0,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,     0,    -2,     4,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,   961,   931,    16,    17,    18,    19,    20,    21,    22,
      23,    24,   363,    25,    26,  -514,    27,  -514,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -221,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,    48,    49,     0,     0,     0,
      50,    51,    52,    53,    54,    55,    56,  -221,   -43,    57,
      58,    59,     0,    60,    61,    62,     0,    63,    64,    65,
      66,    67,    68,    69,    70,     0,    71,    72,     0,    73,
      74,    75,    76,    77,     0,    78,    79,    80,    81,    82,
       0,     0,    83,     0,    84,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,     0,    95,     0,
       0,    96,     0,    97,     0,    98,  1023,    99,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   163,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   164,   165,     0,   166,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   167,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,   252,     0,
       0,     0,    96,     0,   168,     0,    98,     0,   169,  1024,
       4,     0,     5,     6,     7,     8,     9,    10,     0,  -538,
       0,    11,    12,    13,    14,    15,  -538,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,  -538,    25,    26,
    -538,    27,     0,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -221,     0,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
      48,    49,     0,     0,     0,    50,    51,    52,    53,     0,
      55,    56,  -221,   -43,    57,    58,    59,  -538,    60,    61,
      62,  -538,    63,    64,    65,    66,    67,    68,    69,    70,
       0,    71,    72,     0,    73,    74,    75,    76,    77,     0,
      78,    79,    80,    81,    82,     0,     0,    83,     0,    84,
       0,     0,  -538,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -538,  -538,
      88,  -538,  -538,  -538,  -538,  -538,  -538,  -538,     0,  -538,
    -538,  -538,  -538,  -538,     0,  -538,  -538,  -538,  -538,  -538,
    -538,  -538,     0,    95,  -538,  -538,  -538,     0,    97,  -538,
      98,   290,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,    37,     0,    38,  -221,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,    48,    49,     0,     0,     0,    50,    51,    52,    53,
      54,    55,    56,  -221,   -43,    57,    58,    59,     0,    60,
      61,    62,     0,    63,    64,    65,    66,    67,    68,    69,
      70,     0,    71,    72,     0,    73,    74,    75,    76,    77,
       0,    78,    79,    80,    81,    82,     0,     0,    83,     0,
      84,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,   252,    95,     0,     0,    96,     0,    97,
     291,    98,     4,    99,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,    37,     0,    38,  -221,     0,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,    47,    48,    49,     0,     0,     0,    50,    51,    52,
      53,    54,    55,    56,  -221,   -43,    57,    58,    59,     0,
      60,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,     0,    71,    72,     0,    73,    74,    75,    76,
      77,     0,    78,    79,    80,    81,    82,     0,     0,    83,
       0,    84,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,    95,     0,     0,    96,     0,
      97,   457,    98,   476,    99,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,    37,     0,    38,  -221,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,    48,    49,     0,     0,     0,    50,    51,
      52,    53,    54,    55,    56,  -221,   -43,    57,    58,    59,
       0,    60,    61,    62,     0,    63,    64,    65,    66,    67,
      68,    69,    70,     0,    71,    72,     0,    73,    74,    75,
      76,    77,     0,    78,    79,    80,    81,    82,     0,     0,
      83,     0,    84,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,     0,    95,     0,     0,    96,
       0,    97,   477,    98,   290,    99,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,    37,     0,    38,
    -221,     0,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,    47,    48,    49,     0,     0,     0,    50,
      51,    52,    53,    54,    55,    56,  -221,   -43,    57,    58,
      59,     0,    60,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,     0,    71,    72,     0,    73,    74,
      75,    76,    77,     0,    78,    79,    80,    81,    82,     0,
       0,    83,     0,    84,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,    95,     0,     0,
      96,     0,    97,   291,    98,     4,    99,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -221,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,    48,    49,     0,     0,     0,
      50,    51,    52,    53,    54,    55,    56,  -221,   -43,    57,
      58,    59,     0,    60,    61,    62,     0,    63,    64,    65,
      66,    67,    68,    69,    70,     0,    71,    72,     0,    73,
      74,    75,    76,    77,     0,    78,    79,    80,    81,    82,
       0,     0,    83,     0,    84,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,     0,    95,     0,
       0,    96,     0,    97,   649,    98,     4,    99,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,    34,    35,     0,    36,    37,
       0,    38,  -221,     0,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,    48,    49,     0,     0,
       0,    50,    51,    52,    53,   305,    55,    56,  -221,   -43,
      57,    58,    59,     0,    60,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,     0,    71,    72,     0,
      73,    74,    75,    76,    77,     0,    78,    79,    80,    81,
      82,     0,     0,    83,     0,    84,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,    95,
       0,     0,    96,     0,    97,     0,    98,     4,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -221,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,    48,    49,     0,
       0,     0,    50,    51,    52,    53,     0,    55,    56,  -221,
     -43,    57,    58,    59,     0,    60,    61,    62,     0,    63,
      64,    65,    66,    67,    68,    69,    70,     0,    71,    72,
       0,    73,    74,    75,    76,    77,     0,    78,    79,    80,
      81,    82,     0,     0,    83,     0,    84,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,     0,
      95,     0,     0,    96,     0,    97,     0,    98,     4,    99,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,   563,    35,     0,
      36,    37,     0,    38,  -221,     0,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,    48,    49,
       0,     0,     0,    50,    51,    52,    53,     0,    55,    56,
    -221,   -43,    57,    58,    59,     0,    60,    61,    62,     0,
      63,    64,    65,    66,    67,    68,    69,    70,     0,    71,
      72,     0,    73,    74,    75,    76,    77,     0,    78,    79,
      80,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,    95,     0,     0,    96,     0,    97,     0,    98,     4,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,   569,    35,
       0,    36,    37,     0,    38,  -221,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,    48,
      49,     0,     0,     0,    50,    51,    52,    53,     0,    55,
      56,  -221,   -43,    57,    58,    59,     0,    60,    61,    62,
       0,    63,    64,    65,    66,    67,    68,    69,    70,     0,
      71,    72,     0,    73,    74,    75,    76,    77,     0,    78,
      79,    80,    81,    82,     0,     0,    83,     0,    84,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,     0,    95,     0,     0,    96,     0,    97,     0,    98,
       4,    99,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,   830,
      35,     0,    36,    37,     0,    38,  -221,     0,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
      48,    49,     0,     0,     0,    50,    51,    52,    53,     0,
      55,    56,  -221,   -43,    57,    58,    59,     0,    60,    61,
      62,     0,    63,    64,    65,    66,    67,    68,    69,    70,
       0,    71,    72,     0,    73,    74,    75,    76,    77,     0,
      78,    79,    80,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,    95,     0,     0,    96,     0,    97,     0,
      98,     4,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
     832,    35,     0,    36,    37,     0,    38,  -221,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,    48,    49,     0,     0,     0,    50,    51,    52,    53,
       0,    55,    56,  -221,   -43,    57,    58,    59,     0,    60,
      61,    62,     0,    63,    64,    65,    66,    67,    68,    69,
      70,     0,    71,    72,     0,    73,    74,    75,    76,    77,
       0,    78,    79,    80,    81,    82,     0,     0,    83,     0,
      84,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,     0,    95,     0,     0,    96,     0,    97,
       0,    98,     4,    99,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,   837,    35,     0,    36,    37,     0,    38,  -221,     0,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,    47,    48,    49,     0,     0,     0,    50,    51,    52,
      53,     0,    55,    56,  -221,   -43,    57,    58,    59,     0,
      60,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,     0,    71,    72,     0,    73,    74,    75,    76,
      77,     0,    78,    79,    80,    81,    82,     0,     0,    83,
       0,    84,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,    95,     0,     0,    96,     0,
      97,     0,    98,     4,    99,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,   840,    35,     0,    36,    37,     0,    38,  -221,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,    48,    49,     0,     0,     0,    50,    51,
      52,    53,     0,    55,    56,  -221,   -43,    57,    58,    59,
       0,    60,    61,    62,     0,    63,    64,    65,    66,    67,
      68,    69,    70,     0,    71,    72,     0,    73,    74,    75,
      76,    77,     0,    78,    79,    80,    81,    82,     0,     0,
      83,     0,    84,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,     0,    95,     0,     0,    96,
       0,    97,     0,    98,     4,    99,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,   860,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,    37,     0,    38,
    -221,     0,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,    47,    48,    49,     0,     0,     0,    50,
      51,    52,    53,     0,    55,    56,  -221,   -43,    57,    58,
      59,     0,    60,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,     0,    71,    72,     0,    73,    74,
      75,    76,    77,     0,    78,    79,    80,    81,    82,     0,
       0,    83,     0,    84,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,     0,    95,     0,     0,
      96,     0,    97,     0,    98,     4,    99,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,   866,    35,     0,    36,    37,     0,
      38,  -221,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,    48,    49,     0,     0,     0,
      50,    51,    52,    53,     0,    55,    56,  -221,   -43,    57,
      58,    59,     0,    60,    61,    62,     0,    63,    64,    65,
      66,    67,    68,    69,    70,     0,    71,    72,     0,    73,
      74,    75,    76,    77,     0,    78,    79,    80,    81,    82,
       0,     0,    83,     0,    84,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,     0,    95,     0,
       0,    96,     0,    97,     0,    98,     4,    99,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,   870,    35,     0,    36,    37,
       0,    38,  -221,     0,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,    48,    49,     0,     0,
       0,    50,    51,    52,    53,     0,    55,    56,  -221,   -43,
      57,    58,    59,     0,    60,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,     0,    71,    72,     0,
      73,    74,    75,    76,    77,     0,    78,    79,    80,    81,
      82,     0,     0,    83,     0,    84,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,    95,
       0,     0,    96,     0,    97,     0,    98,   733,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   163,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   164,   165,     0,   166,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   167,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,   252,
       0,     0,     0,    96,     0,   168,     0,    98,     0,   169,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   163,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   164,   165,     0,   166,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   167,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
     252,     0,     0,     0,    96,     0,   168,     0,    98,     0,
     169,   641,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   163,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   164,   165,     0,   166,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   167,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,   252,     0,     0,     0,    96,     0,   168,     0,
      98,     0,   169,   964,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   163,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   164,   165,
       0,   166,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   167,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,     0,     0,     0,     0,    96,     0,
     168,     0,    98,     0,   169,   646,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   163,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,   203,    21,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,   204,    30,
     164,   165,     0,   166,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,    52,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,    60,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   167,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,    81,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   765,     0,
     766,     0,    86,    87,    88,    89,    90,   767,     0,     0,
       0,    91,   768,   211,   769,     0,     0,     0,     0,   770,
       0,    92,     0,     0,    93,    94,     0,   213,     0,     0,
      96,     0,   168,   771,    98,     0,   169,     0,   772,     0,
     216,     0,     0,   773,     0,   774,     0,     0,     0,     0,
       0,   775,     0,     0,     0,     0,     0,     0,   776,     0,
       0,     0,     0,     0,   220,     0,     0,     0,     0,     0,
       0,     0,     0,   777,     0,     0,     0,     0,     0,     0,
       0,     0,   222,   223,     0,   778,     0,   225,     0,   779,
       0,     0,   780,     0,     0,     0,   781,     0,     0,   229,
       0,     0,   782,     0,     0,     0,     0,     0,     0,     0,
       0,   348,   349,   350,   351,   352,   353,     0,     0,   356,
     357,   358,   359,     0,   361,   362,   783,   784,   785,   786,
     787,     0,     0,   788,     0,     0,     0,   789,   790,   791,
     792,   793,   794,   795,   796,   797,   798,   799,     0,   800,
       0,     0,   801,   802,   803,   804,     0,     0,   805,     5,
       6,     7,     8,   192,    10,   193,     0,     0,   163,     0,
       0,    14,    15,     0,     0,  -120,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   164,   165,     0,   166,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,  -120,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   167,     0,    71,     0,
       0,    73,     0,     0,   194,    77,     0,  -120,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    92,     0,   163,    93,    94,    14,
      15,     0,     0,    96,     0,   168,    19,    98,     0,   169,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   164,   165,     0,   166,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   167,     0,    71,     0,     0,    73,
     256,   257,     0,    77,   295,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,   296,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,   252,     0,     0,
       0,    96,     0,   168,     0,    98,     0,   169,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   163,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   164,   165,     0,   166,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   167,     0,    71,     0,     0,
      73,   256,   257,     0,    77,   295,     0,    79,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,   252,     0,
       0,     0,    96,     0,   168,     0,    98,   633,   169,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   163,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   164,   165,     0,   166,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   167,     0,    71,     0,
       0,    73,   256,   257,     0,    77,   295,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,   252,
       0,     0,     0,    96,     0,   168,     0,    98,   635,   169,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   163,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,   532,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   164,   165,     0,   166,    35,   533,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,   534,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   167,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,   535,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,     0,     0,     0,    96,     0,   168,     0,    98,     0,
     169,     5,     6,     7,     8,   192,    10,     0,     0,     0,
     163,     0,     0,    14,    15,     0,     0,  -120,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   164,   165,     0,   166,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,  -120,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   167,     0,
      71,     0,     0,    73,     0,     0,   194,    77,     0,  -120,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   163,    93,
      94,    14,    15,     0,     0,    96,     0,   168,    19,    98,
       0,   169,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   164,   165,     0,   166,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   167,     0,    71,     0,
       0,    73,   256,   257,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,     0,
     258,     0,     0,    96,     0,   168,     0,    98,     0,   169,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   163,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   164,   165,     0,   166,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   167,     0,    71,
       0,     0,    73,   256,   257,     0,    77,     0,     0,    79,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
     252,     0,     0,     0,    96,     0,   168,     0,    98,     0,
     169,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     163,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   164,   165,     0,   166,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,   735,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   167,     0,
      71,     0,     0,    73,   256,   257,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   163,    93,
      94,    14,    15,     0,     0,    96,     0,   168,    19,    98,
       0,   169,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   164,   165,     0,   166,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,   197,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   167,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,   198,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    92,     0,   163,    93,    94,    14,
      15,     0,     0,    96,     0,   168,    19,    98,     0,   169,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   164,   165,     0,   166,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   167,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,   198,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,   252,     0,     0,
       0,    96,     0,   168,     0,    98,     0,   169,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   163,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   164,   165,     0,   166,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   167,     0,    71,     0,     0,
      73,   256,   257,     0,    77,     0,     0,    79,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     5,     6,     7,     8,
       9,    10,     0,    92,     0,   163,    93,    94,    14,    15,
       0,     0,    96,     0,   168,    19,    98,     0,   169,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     164,   165,     0,   166,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   167,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,   252,     0,     0,     0,
      96,     0,   168,   631,    98,     0,   169,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   163,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   164,   165,     0,   166,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   167,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,   198,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   163,    93,    94,    14,    15,     0,
       0,    96,     0,   168,    19,    98,     0,   169,     0,     0,
       0,   238,    26,     0,     0,     0,     0,     0,    30,   164,
     165,     0,   166,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   167,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     5,     6,     7,     8,     9,    10,     0,
      92,     0,   163,    93,    94,    14,    15,     0,     0,    96,
       0,    97,    19,    98,     0,   169,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   164,   165,     0,
     166,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     167,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,   252,     0,     0,     0,    96,     0,   168,
       0,    98,     0,   169,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   163,     0,     0,    14,   601,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,   602,     0,     0,     0,     0,     0,    30,   164,   165,
       0,   166,    35,     0,   603,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,   604,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,   605,   606,    67,    68,
      69,   607,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,   608,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    93,    94,   252,     0,     0,     0,    96,     0,
     168,     0,    98,     0,   916,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   163,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   164,
     165,     0,   166,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   167,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,   252,     0,     0,     0,    96,
       0,   168,     0,    98,     0,   916,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   163,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     164,   165,     0,   901,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   167,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,   252,     0,     0,     0,
      96,     0,   168,     0,    98,     0,   169,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   163,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   164,   165,     0,   166,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   167,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   163,    93,    94,    14,    15,     0,
       0,    96,     0,   168,    19,    98,     0,   169,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   164,
     165,     0,   166,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   167,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     5,     6,     7,     8,     9,    10,     0,
      92,     0,   163,    93,    94,    14,   497,     0,     0,    96,
       0,    97,    19,    98,     0,   169,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   164,   165,     0,
     166,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     167,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,   498,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    92,     0,
     163,    93,    94,    14,    15,     0,     0,    96,     0,   168,
      19,    98,     0,   169,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   164,   165,     0,   166,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   167,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   163,    93,
      94,    14,   601,     0,     0,   574,     0,   168,    19,    98,
       0,   169,     0,     0,     0,     0,   602,     0,     0,     0,
       0,     0,    30,   164,   165,     0,   166,    35,     0,   603,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,   604,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,   605,   606,    67,    68,    69,   607,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,   608,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    92,     0,   163,    93,    94,    14,
      15,     0,     0,    96,     0,   168,    19,    98,     0,   609,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   164,   165,     0,   166,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   167,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   163,    93,    94,    14,    15,     0,
       0,    96,     0,   168,    19,    98,     0,   609,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   164,
     165,     0,   677,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   167,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     5,     6,     7,     8,     9,    10,     0,
      92,     0,   163,    93,    94,    14,    15,     0,     0,    96,
       0,   168,    19,    98,     0,   169,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   164,   165,     0,
     680,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     167,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    92,     0,
     163,    93,    94,    14,    15,     0,     0,    96,     0,   168,
      19,    98,     0,   169,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   164,   165,     0,   895,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   167,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   163,    93,
      94,    14,    15,     0,     0,    96,     0,   168,    19,    98,
       0,   169,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   164,   165,     0,   896,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   167,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    92,     0,   163,    93,    94,    14,
      15,     0,     0,    96,     0,   168,    19,    98,     0,   169,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   164,   165,     0,   898,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   167,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   163,    93,    94,    14,    15,     0,
       0,    96,     0,   168,    19,    98,     0,   169,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   164,
     165,     0,   899,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   167,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     5,     6,     7,     8,     9,    10,     0,
      92,     0,   163,    93,    94,    14,    15,     0,     0,    96,
       0,   168,    19,    98,     0,   169,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   164,   165,     0,
     900,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     167,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    92,     0,
     163,    93,    94,    14,    15,     0,     0,    96,     0,   168,
      19,    98,     0,   169,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   164,   165,     0,   901,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   167,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   163,    93,
      94,    14,   601,     0,     0,    96,     0,   168,    19,    98,
       0,   169,     0,     0,     0,     0,   602,     0,     0,     0,
       0,     0,    30,   164,   165,     0,   166,    35,     0,   603,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,   604,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,   605,   606,    67,    68,    69,   607,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,   608,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    92,     0,   163,    93,    94,    14,
      15,     0,     0,    96,     0,   168,    19,    98,     0,   983,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   164,   165,     0,   166,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   167,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   163,    93,    94,    14,    15,     0,
       0,    96,     0,     0,    19,    98,     0,   983,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   164,
     165,     0,   166,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   167,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   163,     0,
      92,    14,    15,     0,    94,     0,     0,     0,    19,    96,
       0,   168,     0,    98,     0,   169,    26,     0,     0,     0,
       0,     0,    30,   164,   165,     0,   166,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   167,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
    -390,     0,     0,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   163,     0,     0,    14,    15,    88,     0,     0,
       0,  -390,    19,     0,     0,  -390,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   164,   165,   252,
     166,    35,     0,    36,     0,   168,    38,    98,  -390,   916,
      40,    41,  -372,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,  -372,     0,     0,     0,    65,    66,    67,    68,    69,
     167,     0,    71,     0,  -372,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,  -372,     0,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     5,     6,     7,     8,     9,    10,     0,
    -485,     0,   163,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,   363,  -372,     0,  -514,     0,  -514,  -372,
      26,    98,     0,   983,     0,     0,    30,   164,   165,     0,
     166,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     167,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   163,     0,     0,    14,
      15,    88,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   164,   165,     0,   166,    35,     0,    36,     0,   168,
      38,    98,     0,   169,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   167,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     163,     0,     0,    14,    15,    88,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   164,   165,     0,   166,    35,
       0,    36,     0,   168,    38,    98,     0,   609,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,   757,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   167,     0,
      71,  -207,     0,    73,     0,  -207,  -207,    77,     0,     0,
      79,     0,     0,    82,  -207,     0,  -207,  -207,     0,     0,
       0,  -207,     0,     0,     0,     0,  -207,     0,     0,  -207,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,   902,     0,     0,     0,  -207,     0,
    -207,     0,  -207,     0,  -207,  -207,     0,  -207,     0,  -207,
       0,  -207,  -207,     0,     0,     0,  -207,  -207,     0,    98,
       0,   169,     0,     0,     0,  -207,     0,  -207,  -207,     0,
    -207,   209,  -207,  -207,     0,     0,  -207,  -207,     0,     0,
    -207,     0,     0,   210,   211,     0,     0,     0,     0,     0,
     212,     0,     0,     0,     0,     0,     0,     0,   213,  -207,
       0,  -207,     0,  -207,   214,  -207,  -207,     0,  -207,   215,
    -207,   216,  -207,     0,   217,     0,     0,     0,     0,     0,
       0,     0,   218,     0,     0,     0,  -207,     0,   209,   219,
       0,  -207,  -207,     0,  -207,   220,     0,  -207,     0,     0,
     210,   211,     0,     0,   221,     0,     0,   212,     0,     0,
       0,     0,     0,   222,   223,   213,   224,     0,   225,     0,
     226,   214,     0,   227,     0,     0,   215,   228,   216,     0,
     229,   217,     0,   230,     0,     0,     0,     0,     0,   218,
       0,     0,     0,     0,     0,     0,   219,  -207,     0,     0,
       0,     0,   220,  -207,     0,   944,     0,     0,     0,     0,
       0,   221,   767,     0,     0,     0,     0,   210,   211,   769,
     222,   223,     0,   224,   212,   225,     0,   226,     0,     0,
     227,     0,   213,     0,   228,     0,     0,   229,   771,   234,
     230,     0,     0,   215,     0,   216,     0,     0,   217,     0,
     774,     0,     0,     0,     0,     0,   218,     0,     0,     0,
       0,     0,     0,   219,     0,     0,     0,     0,   319,   220,
       0,     0,     0,     0,     0,   320,     0,     0,   221,     0,
       0,     0,     0,     0,     0,     0,   321,   222,   223,     0,
     224,     0,   225,     0,   945,     0,   723,   780,     0,     0,
       0,   228,     0,     0,   229,     0,     0,   230,     0,     0,
       0,     0,     0,     0,     0,     0,   348,   349,   350,   351,
     352,   353,     0,     0,   356,   357,   358,   359,     0,   361,
     362,   783,   784,   785,   786,   787,   322,     0,   788,     0,
     323,     0,   789,   790,   791,   792,   793,   794,   795,   796,
     797,   798,   799,     0,   800,   319,     0,   801,   802,   803,
     804,     0,   320,     0,     0,     0,     0,   401,     0,     0,
       0,   324,     0,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,   326,     0,
     327,   328,   329,     0,   330,   331,   332,     0,   333,   334,
     335,   336,   337,     0,   338,   339,   340,   341,   342,   343,
     344,     0,   319,   345,   346,   347,     0,     0,     0,   320,
       0,     0,   753,   322,     0,     0,     0,   323,     0,     0,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   319,     0,     0,     0,     0,     0,   324,   320,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     321,     0,     0,     0,   325,   326,     0,   327,   328,   329,
     322,   330,   331,   332,   323,   333,   334,   335,   336,   337,
       0,   338,   339,   340,   341,   342,   343,   344,     0,     0,
     345,   346,   347,     0,     0,   319,     0,     0,     0,   824,
       0,     0,   320,     0,     0,   324,     0,     0,     0,     0,
     322,     0,     0,   321,   323,     0,     0,     0,     0,     0,
       0,   325,   326,     0,   327,   328,   329,     0,   330,   331,
     332,     0,   333,   334,   335,   336,   337,     0,   338,   339,
     340,   341,   342,   343,   344,   324,     0,   345,   346,   347,
       0,     0,     0,     0,   321,     0,   979,     0,     0,     0,
       0,   325,   326,   322,   327,   328,   329,   323,   330,   331,
     332,     0,   333,   334,   335,   336,   337,     0,   338,   339,
     340,   341,   342,   343,   344,   319,     0,   345,   346,   347,
       0,     0,   320,     0,     0,     0,  1046,     0,   324,     0,
       0,     0,     0,   321,   322,     0,     0,     0,   323,     0,
       0,     0,     0,     0,   325,   326,     0,   327,   328,   329,
       0,   330,   331,   332,     0,   333,   334,   335,   336,   337,
       0,   338,   339,   340,   341,   342,   343,   344,     0,     0,
     345,   346,   347,     0,     0,     0,     0,   632,     0,     0,
       0,     0,     0,   322,     0,   325,   326,   323,   327,   328,
     329,   319,   330,   331,     0,     0,     0,   334,   320,     0,
       0,     0,     0,     0,   340,   341,     0,     0,   344,   321,
       0,   345,   346,   347,     0,     0,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   325,   326,     0,   327,   328,   329,
       0,   330,   331,   332,     0,   333,   334,   335,   336,   337,
       0,   338,   339,   340,   341,   342,   343,   344,     0,   322,
     345,   346,   347,   323,     0,     0,     0,   943,     0,     0,
       0,   319,   412,     0,     0,     0,     0,     0,   320,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   321,
       0,     0,     0,     0,   324,   348,   349,   350,   351,   352,
     353,     0,     0,   356,   357,   358,   359,     0,   361,   362,
     325,   326,     0,   327,   328,   329,     0,   330,   331,   332,
       0,   333,   334,   335,   336,   337,     0,   338,   339,   340,
     341,   342,   343,   344,     0,     0,   345,   346,   347,   322,
     376,     0,     0,   323,     0,     0,     0,   319,     0,     0,
       0,     0,   829,     0,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   321,     0,     0,     0,     0,
       0,     0,     0,     0,   324,   348,   349,   350,   351,   352,
     353,     0,     0,   356,   357,   358,   359,     0,   361,   362,
     325,   326,     0,   327,   328,   329,     0,   330,   331,   332,
       0,   333,   334,   335,   336,   337,     0,   338,   339,   340,
     341,   342,   343,   344,     0,   322,   345,   346,   347,   323,
     376,     0,     0,   319,     0,     0,     0,     0,   836,     0,
     320,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   321,     0,     0,     0,     0,     0,     0,     0,     0,
     324,   348,   349,   350,   351,   352,   353,     0,     0,   356,
     357,   358,   359,     0,   361,   362,   325,   326,     0,   327,
     328,   329,     0,   330,   331,   332,     0,   333,   334,   335,
     336,   337,     0,   338,   339,   340,   341,   342,   343,   344,
       0,   322,   345,   346,   347,   323,   376,     0,     0,   319,
       0,     0,     0,     0,   956,     0,   320,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   321,     0,     0,
       0,     0,     0,     0,     0,     0,   324,   348,   349,   350,
     351,   352,   353,     0,     0,   356,   357,   358,   359,     0,
     361,   362,   325,   326,     0,   327,   328,   329,     0,   330,
     331,   332,     0,   333,   334,   335,   336,   337,     0,   338,
     339,   340,   341,   342,   343,   344,     0,   322,   345,   346,
     347,   323,   376,     0,     0,   319,     0,     0,     0,     0,
     957,     0,   320,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   321,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   348,   349,   350,   351,   352,   353,     0,
       0,   356,   357,   358,   359,     0,   361,   362,   325,   326,
       0,   327,   328,   329,     0,   330,   331,   332,     0,   333,
     334,   335,   336,   337,     0,   338,   339,   340,   341,   342,
     343,   344,     0,   322,   345,   346,   347,   323,   376,     0,
       0,   319,     0,     0,     0,     0,   958,     0,   320,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   321,
       0,     0,     0,     0,     0,     0,     0,     0,   324,   348,
     349,   350,   351,   352,   353,     0,     0,   356,   357,   358,
     359,     0,   361,   362,   325,   326,     0,   327,   328,   329,
       0,   330,   331,   332,     0,   333,   334,   335,   336,   337,
       0,   338,   339,   340,   341,   342,   343,   344,     0,   322,
     345,   346,   347,   323,   376,     0,     0,   319,     0,     0,
       0,     0,   959,     0,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   321,     0,     0,     0,     0,
       0,     0,     0,     0,   324,   348,   349,   350,   351,   352,
     353,     0,     0,   356,   357,   358,   359,     0,   361,   362,
     325,   326,     0,   327,   328,   329,     0,   330,   331,   332,
       0,   333,   334,   335,   336,   337,     0,   338,   339,   340,
     341,   342,   343,   344,     0,   322,   345,   346,   347,   323,
     376,     0,     0,   319,     0,     0,     0,     0,   981,     0,
     320,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   321,     0,     0,     0,     0,     0,     0,     0,     0,
     324,   348,   349,   350,   351,   352,   353,     0,     0,   356,
     357,   358,   359,     0,   361,   362,   325,   326,     0,   327,
     328,   329,     0,   330,   331,   332,     0,   333,   334,   335,
     336,   337,     0,   338,   339,   340,   341,   342,   343,   344,
       0,   322,   345,   346,   347,   323,   376,     0,     0,     0,
       0,     0,     0,     0,   982,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,   348,   349,   350,
     351,   352,   353,     0,     0,   356,   357,   358,   359,     0,
     361,   362,   325,   326,     0,   327,   328,   329,     0,   330,
     331,   332,     0,   333,   334,   335,   336,   337,     0,   338,
     339,   340,   341,   342,   343,   344,   319,     0,   345,   346,
     347,     0,   376,   320,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   321,   404,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   405,     0,
       0,     0,     0,     0,     0,     0,     0,   319,     0,     0,
       0,     0,     0,     0,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   321,   398,     0,     0,     0,
       0,     0,     0,     0,   322,     0,     0,     0,   323,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   319,     0,     0,     0,     0,     0,     0,   320,
       0,     0,     0,     0,     0,   401,     0,     0,     0,   324,
     321,   706,     0,     0,     0,   322,     0,     0,     0,   323,
       0,     0,     0,     0,     0,   325,   326,     0,   327,   328,
     329,     0,   330,   331,   332,     0,   333,   334,   335,   336,
     337,     0,   338,   339,   340,   341,   342,   343,   344,     0,
     324,   345,   346,   347,     0,   376,     0,     0,     0,     0,
     322,     0,     0,   321,   323,     0,   325,   326,     0,   327,
     328,   329,     0,   330,   331,   332,     0,   333,   334,   335,
     336,   337,     0,   338,   339,   340,   341,   342,   343,   344,
     319,   401,   345,   346,   347,   324,   376,   320,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   321,   238,
       0,   325,   326,   322,   327,   328,   329,   323,   330,   331,
     332,     0,   333,   334,   335,   336,   337,     0,   338,   339,
     340,   341,   342,   343,   344,   319,     0,   345,   346,   347,
       0,   376,   320,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   321,   698,     0,     0,     0,   322,     0,
       0,     0,   323,     0,   325,   326,     0,   327,   328,   329,
       0,   330,   331,   332,     0,   333,   334,   335,   336,     0,
       0,   338,   339,   340,   341,   342,     0,   344,     0,     0,
     345,   346,   347,   324,     0,     0,     0,     0,   321,     0,
       0,     0,     0,   322,     0,     0,     0,   323,     0,   325,
     326,     0,   327,   328,   329,     0,   330,   331,   332,     0,
     333,   334,   335,   336,   337,     0,   338,   339,   340,   341,
     342,   343,   344,   319,     0,   345,   346,   347,   324,   376,
     320,     0,     0,     0,     0,     0,     0,     0,   322,     0,
       0,   321,   323,     0,   325,   326,     0,   327,   328,   329,
       0,   330,   331,   332,     0,   333,   334,   335,   336,   337,
       0,   338,   339,   340,   341,   342,   343,   344,   319,     0,
     345,   346,   347,     0,   376,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   321,     0,     0,   325,
     326,   322,   327,   328,   329,   323,   330,   331,   332,     0,
       0,   334,   335,   336,   716,     0,   338,   339,   340,   341,
       0,     0,   344,     0,     0,   345,   346,   347,     0,     0,
       0,   319,     0,     0,     0,     0,   324,     0,   320,     0,
       0,     0,     0,     0,     0,     0,   322,     0,     0,   321,
     323,     0,   325,   326,   409,   327,   328,   329,     0,   330,
     331,   332,     0,   333,   334,   335,   336,   337,     0,   338,
     339,   340,   341,   342,   343,   344,     0,     0,   345,   346,
     347,   324,   376,     0,   410,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,   326,   322,
     327,   328,   329,   323,   330,   331,   332,     0,   333,   334,
     335,   336,   337,     0,   338,   339,   340,   341,   342,   343,
     344,     0,     0,   345,   346,   347,     0,   439,   319,     0,
       0,     0,     0,     0,   324,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   321,     0,     0,     0,
     325,   326,     0,   327,   328,   329,     0,   330,   331,   332,
     378,   333,   334,   335,   336,   337,     0,   338,   339,   340,
     341,   342,   343,   344,     0,   411,   345,   346,   347,     0,
       0,   319,     0,     0,     0,     0,     0,     0,   320,     0,
       0,     0,     0,     0,     0,     0,   322,     0,     0,   321,
     323,     0,     0,     0,   452,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   158,     0,     0,
       0,   324,     0,     0,   453,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,   326,   322,
     327,   328,   329,   323,   330,   331,   332,     0,   333,   334,
     335,   336,   337,     0,   338,   339,   340,   341,   342,   343,
     344,     0,     0,   345,   346,   347,     0,     0,   319,     0,
       0,     0,     0,     0,   324,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   321,   541,     0,     0,
     325,   326,     0,   327,   328,   329,     0,   330,   331,   332,
     542,   333,   334,   335,   336,   337,     0,   338,   339,   340,
     341,   342,   343,   344,   319,     0,   345,   346,   347,     0,
       0,   320,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   321,   543,     0,     0,   322,     0,     0,     0,
     323,     0,     0,     0,     0,     0,   544,     0,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,   320,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,     0,     0,     0,   323,   325,   326,     0,
     327,   328,   329,     0,   330,   331,   332,     0,   333,   334,
     335,   336,   337,     0,   338,   339,   340,   341,   342,   343,
     344,   319,     0,   345,   346,   347,     0,   324,   320,     0,
       0,     0,   322,     0,     0,     0,   323,     0,     0,   321,
       0,     0,     0,   325,   326,     0,   327,   328,   329,     0,
     330,   331,   332,     0,   333,   334,   335,   336,   337,     0,
     338,   339,   340,   341,   342,   343,   344,   324,     0,   345,
     346,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   325,   326,     0,   327,   328,   329,   322,
     330,   331,   332,   323,   333,   334,   335,   336,   337,     0,
     338,   339,   340,   341,   342,   343,   344,   319,   443,   345,
     346,   347,     0,     0,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,   321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     325,   326,     0,   327,   328,   329,     0,   330,   331,   332,
       0,   333,   334,   335,   336,   337,     0,   338,   339,   340,
     341,   342,   343,   344,   319,   445,   345,   346,   347,     0,
       0,   320,     0,     0,     0,   322,     0,     0,     0,   323,
       0,     0,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
     324,   320,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   321,     0,     0,     0,   325,   326,     0,   327,
     328,   329,   322,   330,   331,   332,   323,   333,   334,   335,
     336,   337,     0,   338,   339,   340,   341,   342,   343,   344,
     319,   447,   345,   346,   347,     0,     0,   320,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   324,   321,     0,
       0,     0,   322,     0,     0,     0,   323,     0,     0,     0,
       0,     0,     0,   325,   326,   545,   327,   328,   329,     0,
     330,   331,   332,     0,   333,   334,   335,   336,   337,     0,
     338,   339,   340,   341,   342,   343,   344,   324,   455,   345,
     346,   347,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,   323,   325,   326,     0,   327,   328,   329,     0,
     330,   331,   332,     0,   333,   334,   335,   336,   337,     0,
     338,   339,   340,   341,   342,   343,   344,   319,     0,   345,
     346,   347,   472,   324,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   321,     0,     0,     0,   325,
     326,     0,   327,   328,   329,     0,   330,   331,   332,     0,
     333,   334,   335,   336,   337,     0,   338,   339,   340,   341,
     342,   343,   344,     0,     0,   345,   346,   347,     0,     0,
     319,     0,     0,     0,     0,     0,     0,   320,     0,     0,
       0,     0,     0,     0,     0,   322,     0,     0,   321,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     319,     0,     0,     0,     0,     0,     0,   320,     0,     0,
     324,     0,     0,     0,     0,     0,     0,     0,   321,     0,
       0,     0,     0,     0,     0,     0,   325,   326,   322,   327,
     328,   329,   323,   330,   331,   332,     0,   333,   334,   335,
     336,   337,  -486,   338,   339,   340,   341,   342,   343,   344,
       0,     0,   345,   346,   347,     0,     0,   319,   711,   158,
       0,     0,     0,   324,   320,     0,     0,     0,   322,     0,
       0,     0,   323,     0,     0,   321,     0,     0,     0,   325,
     326,     0,   327,   328,   329,     0,   330,   331,   332,     0,
     333,   334,   335,   336,   337,     0,   338,   339,   340,   341,
     342,   343,   344,   324,     0,   345,   346,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   325,
     326,     0,   327,   328,   329,   322,   330,   331,   332,   323,
     333,   334,   335,   336,   337,     0,   338,   339,   340,   341,
     342,   343,   344,   319,   687,   345,   346,   347,     0,     0,
     320,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,   321,     0,     0,   714,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   325,   326,     0,   327,
     328,   329,     0,   330,   331,   332,     0,   333,   334,   335,
     336,   337,     0,   338,   339,   340,   341,   342,   343,   344,
     319,     0,   345,   346,   347,     0,     0,   320,     0,     0,
       0,   322,     0,     0,     0,   323,     0,     0,   321,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   319,     0,     0,     0,     0,     0,     0,   320,
       0,     0,     0,     0,     0,     0,   324,     0,     0,     0,
     321,   819,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   325,   326,     0,   327,   328,   329,   322,   330,
     331,   332,   323,   333,   334,   335,   336,   337,     0,   338,
     339,   340,   341,   342,   343,   344,     0,     0,   345,   346,
     347,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     322,     0,   756,   324,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   325,
     326,     0,   327,   328,   329,     0,   330,   331,   332,     0,
     333,   334,   335,   336,   337,   324,   338,   339,   340,   341,
     342,   343,   344,   319,     0,   345,   346,   347,     0,     0,
     320,   325,   326,     0,   327,   328,   329,     0,   330,   331,
     332,   321,   333,   334,   335,   336,   337,     0,   338,   339,
     340,   341,   342,   343,   344,   319,     0,   345,   346,   347,
       0,     0,   320,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   321,   822,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   322,     0,     0,     0,   323,     0,     0,     0,     0,
       0,     0,     0,     0,   894,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   322,     0,     0,   324,   323,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   325,   326,     0,   327,   328,   329,     0,   330,
     331,   332,     0,   333,   334,   335,   336,   337,   324,   338,
     339,   340,   341,   342,   343,   344,   319,     0,   345,   346,
     347,     0,     0,   320,   325,   326,     0,   327,   328,   329,
       0,   330,   331,   332,   321,   333,   334,   335,   336,   337,
       0,   338,   339,   340,   341,   342,   343,   344,   319,   911,
     345,   346,   347,     0,     0,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   321,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   322,     0,     0,     0,   323,     0,
       0,     0,     0,     0,     0,     0,     0,   897,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,     0,     0,   324,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   325,   326,     0,   327,   328,
     329,     0,   330,   331,   332,     0,   333,   334,   335,   336,
     337,   324,   338,   339,   340,   341,   342,   343,   344,   319,
       0,   345,   346,   347,     0,     0,   320,   325,   326,     0,
     327,   328,   329,     0,   330,   331,   332,   321,   333,   334,
     335,   336,   337,     0,   338,   339,   340,   341,   342,   343,
     344,   319,     0,   345,   346,   347,     0,     0,   320,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   321,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   322,     0,     0,
       0,   323,     0,     0,     0,     0,     0,     0,     0,     0,
     992,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   322,
       0,     0,   324,   323,     0,     0,     0,     0,     0,     0,
       0,     0,   993,     0,     0,     0,     0,     0,   325,   326,
       0,   327,   328,   329,     0,   330,   331,   332,     0,   333,
     334,   335,   336,   337,   324,   338,   339,   340,   341,   342,
     343,   344,   319,     0,   345,   346,   347,     0,     0,   320,
     325,   326,     0,   327,   328,   329,     0,   330,   331,   332,
     321,   333,   334,   335,   336,   337,     0,   338,   339,   340,
     341,   342,   343,   344,   319,     0,   345,   346,   347,     0,
       0,   320,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     322,     0,     0,     0,   323,     0,     0,     0,     0,     0,
       0,     0,     0,   994,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,     0,     0,   324,   323,     0,     0,     0,
       0,     0,     0,     0,     0,   995,     0,     0,     0,     0,
       0,   325,   326,     0,   327,   328,   329,     0,   330,   331,
     332,     0,   333,   334,   335,   336,   337,   324,   338,   339,
     340,   341,   342,   343,   344,   319,     0,   345,   346,   347,
       0,     0,   320,   325,   326,     0,   327,   328,   329,     0,
     330,   331,   332,   321,   333,   334,   335,   336,   337,     0,
     338,   339,   340,   341,   342,   343,   344,   319,     0,   345,
     346,   347,     0,     0,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   322,     0,     0,     0,   323,     0,     0,
       0,     0,     0,     0,     0,     0,   996,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   322,     0,     0,   324,   323,
       0,     0,     0,     0,     0,     0,     0,     0,   997,     0,
       0,     0,     0,     0,   325,   326,     0,   327,   328,   329,
       0,   330,   331,   332,     0,   333,   334,   335,   336,   337,
     324,   338,   339,   340,   341,   342,   343,   344,   319,     0,
     345,   346,   347,     0,     0,   320,   325,   326,     0,   327,
     328,   329,     0,   330,   331,   332,   321,   333,   334,   335,
     336,   337,     0,   338,   339,   340,   341,   342,   343,   344,
     319,     0,   345,   346,   347,     0,     0,   320,     0,     0,
    1019,     0,     0,     0,     0,     0,     0,     0,   321,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,     0,     0,     0,
     323,     0,     0,     0,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,   320,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   321,   322,     0,
       0,   324,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,   326,     0,
     327,   328,   329,     0,   330,   331,   332,     0,   333,   334,
     335,   336,   337,   324,   338,   339,   340,   341,   342,   343,
     344,     0,     0,   345,   346,   347,     0,   675,     0,   325,
     326,   323,   327,   328,   329,     0,   330,   331,   332,     0,
     333,   334,   335,   336,   337,     0,   338,   339,   340,   341,
     342,   343,   344,     0,     0,   345,   346,   347,     0,     0,
       0,     0,   324,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   325,   326,
       0,   327,   328,   329,     0,   330,   331,   332,     0,   333,
     334,   335,   336,   337,     0,   338,   339,   340,   341,   342,
     343,   344,     0,     0,   345,   346,   347,   653,     0,   209,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   210,   211,     0,     0,     0,     0,     0,   212,     0,
       0,     0,     0,     0,     0,     0,   213,     0,     0,     0,
       0,     0,   214,     0,     0,     0,     0,   215,     0,   216,
       0,     0,   217,     0,     0,     0,     0,     0,     0,     0,
     218,     0,     0,     0,     0,     0,   209,   219,     0,     0,
       0,     0,     0,   220,     0,     0,     0,     0,   210,   211,
       0,     0,   221,     0,     0,   212,     0,     0,     0,     0,
       0,   222,   223,   213,   224,     0,   225,     0,   226,   214,
       0,   227,     0,     0,   215,   228,   216,     0,   229,   217,
       0,   230,     0,     0,     0,     0,     0,   218,     0,     0,
       0,     0,     0,     0,   219,     0,     0,     0,     0,     0,
     220,     0,     0,     0,     0,     0,     0,     0,     0,   221,
       0,     0,     0,     0,     0,     0,     0,     0,   222,   223,
       0,   224,     0,   225,     0,   226,     0,     0,   227,     0,
       0,     0,   228,     0,     0,   229,     0,     0,   230
};

static const yytype_int16 yycheck[] =
{
      11,   448,     2,   181,    15,    61,   143,    18,    17,    56,
      57,   420,    20,    24,   567,   131,   194,    28,    29,   732,
      31,    32,    33,    34,    80,   208,   662,   202,     1,    28,
     926,    42,   852,    44,    45,   369,    81,   189,    49,     1,
      51,   417,    53,    99,   810,   811,    42,   758,   600,    42,
      61,    62,    63,    64,    65,    66,   208,     3,     1,    70,
     964,    72,     3,    74,    75,   248,   182,   849,    79,    80,
       1,    42,    83,    84,    42,    86,    64,    88,    29,    29,
      91,    92,   135,    94,   267,    29,    97,    98,    99,    29,
     109,     1,    31,    32,   425,   107,   248,    42,    47,  1009,
      29,   814,    64,   135,   135,  1009,   410,    42,    79,    52,
     154,   164,    19,   169,   159,   267,   160,   448,   106,   672,
     940,    52,   674,   135,    73,    42,   301,   302,   160,    42,
    1026,   162,    42,   915,    79,    81,  1046,   274,    81,   135,
     853,   114,   135,     0,   106,   164,    92,    52,   154,   453,
      81,    92,   163,   164,   165,   166,   167,   168,   169,   109,
      99,   134,   135,    94,   107,   109,    24,   135,   164,   880,
      77,   180,   160,    19,   109,    62,   107,   729,   189,   331,
     946,   947,   365,   366,    42,    28,    81,    45,   196,   162,
     599,   134,   903,   204,   101,   203,   164,   208,   160,   135,
     135,   914,   107,   134,   114,   204,   135,    65,   159,   159,
      68,    98,    70,   365,   366,   159,   159,    24,   135,   159,
     109,    79,   135,   154,   134,   135,   162,    24,   159,   164,
     159,    77,    24,   567,   161,    78,   163,   248,   249,    82,
      98,   135,   114,   135,   154,   256,   257,   164,   114,   135,
     963,   164,     3,   135,   890,   101,   267,   135,   441,   154,
     109,    68,    78,   135,   159,   135,    82,   139,   162,   135,
     162,    68,    79,   139,   160,   164,    68,   135,   394,    30,
     162,   834,    79,   153,   162,   296,   129,    79,     3,   441,
     162,   134,   162,   293,   137,    24,   162,   104,   141,   154,
      79,    98,  1024,    54,   162,   160,    98,   318,   319,   320,
     321,   322,   323,   324,   723,   326,   327,   328,   329,   330,
    1042,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   114,   672,    68,
     396,    92,   135,   354,   355,   721,   135,    98,   161,   360,
      79,   135,   363,   364,   365,   366,   367,   368,   369,    23,
      24,   154,   139,   161,   153,   163,   161,   378,   129,    98,
      34,    35,   383,   162,   385,   104,   137,    52,   162,   161,
      44,   163,   758,   322,   323,   396,     2,   398,   399,   135,
       7,   135,    52,   404,   405,    11,   571,   572,   409,   410,
     408,   412,    66,   414,    68,   557,    81,    23,   154,    25,
     154,    42,   859,   154,    98,    79,   427,   135,   135,    94,
     161,    81,   163,   434,   480,   153,   154,   135,   367,   368,
     441,    78,   107,    64,    94,    82,   154,   154,   161,   378,
     104,   452,   453,    74,    75,   970,   154,   107,   459,    89,
     628,   462,   135,   464,    70,   129,   161,   468,   469,   470,
     399,   472,   473,   137,   639,   640,   405,   354,   355,   480,
     462,   154,   464,   360,    74,    75,   363,   364,   478,   154,
     159,    97,    74,   154,   159,   101,   497,   498,  1013,  1014,
    1015,  1016,  1017,  1018,   154,   134,   154,   145,   137,   159,
     834,   154,   141,   154,   154,   189,   161,   153,    42,   885,
     154,   154,    52,   137,   159,   108,   154,   154,    52,   161,
     161,   852,   154,   114,   208,   662,   161,   903,   859,   468,
     541,   542,   543,   544,   545,   546,   547,     7,    28,   548,
     549,    81,    56,   159,   555,   134,   557,    81,   179,   159,
     561,   154,   563,   609,    94,   114,   567,   162,   569,   135,
     154,    42,   161,    31,   248,   162,   135,   107,   199,   200,
     201,   202,   162,   107,   162,   462,   207,   464,   114,   162,
     114,    18,   135,   267,   164,   164,   159,   154,    78,   154,
     601,   154,    82,   604,   605,   606,   607,   608,   609,   154,
     154,   135,   613,   542,   162,   544,   154,   164,   135,   940,
     241,   154,   154,   244,   154,   231,    31,   162,   162,   159,
     154,   160,   238,   154,   154,   159,   135,   160,   162,   114,
      79,    29,   263,   964,     2,    29,   164,   159,    31,   129,
     134,    79,   134,    11,   134,   164,   136,   137,   979,   164,
     161,   141,   283,    24,   164,    23,   160,    25,   137,   149,
     137,   672,   129,   137,   675,   676,   677,   157,   555,   680,
     129,    42,   137,   137,    45,   137,   137,   293,  1009,   137,
     137,   365,   366,   739,   137,   696,   129,   698,   129,   137,
      31,   307,   703,  1024,    65,   706,   162,    68,   709,    70,
     711,   129,    70,   714,   160,    19,    74,    75,    79,    23,
      24,  1042,   162,    94,   135,  1046,   129,   164,    32,   453,
      34,    35,   307,    22,   275,    39,   613,    98,   739,    97,
      44,   452,   195,   101,   559,   827,   675,   676,   114,   743,
     751,   761,   753,   890,   885,   756,   721,   592,   379,   760,
     434,   764,    66,   890,    68,  1026,    70,   441,    72,   963,
     376,    75,   988,    77,   140,    79,   142,   143,  1019,  1012,
     146,   147,   403,    33,   428,   459,   407,   557,   462,    -1,
     464,    -1,   398,    -1,    98,   469,    -1,   101,   404,    -1,
     104,   159,    -1,    -1,   805,    -1,   412,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,   818,   819,    -1,
     821,   822,   823,   824,    52,    -1,    -1,    -1,   829,   830,
      24,   832,    -1,   834,    -1,   836,   837,    -1,    -1,   840,
     877,   878,    -1,    -1,    -1,    -1,    -1,    -1,    42,    42,
     154,    45,    -1,    81,    -1,    -1,   160,   858,   859,    52,
     466,    -1,    -1,   864,   470,   866,    23,    24,    -1,   870,
     916,    65,   478,   231,    68,   876,    70,    34,    35,   107,
     238,   555,    -1,   557,    -1,    79,   114,    44,    81,    -1,
      47,    -1,    -1,   894,   895,   896,   897,   898,   899,   900,
     901,    -1,    -1,    -1,    98,    -1,    -1,   135,    -1,    66,
     911,    68,    -1,    -1,   107,   916,    73,    -1,    -1,    -1,
     921,   114,    79,    -1,   925,    -1,   154,    -1,    -1,    -1,
      -1,   159,   538,    -1,   162,   293,    -1,   983,    -1,    -1,
      -1,    -1,   135,    -1,   565,   566,    -1,   104,    -1,   307,
     571,   572,    -1,    -1,    -1,   956,   957,   958,   959,    -1,
      -1,   154,   583,    -1,    -1,    -1,   159,    -1,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,   582,    -1,   979,    -1,
     981,   982,   983,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   992,   993,   994,   995,   996,   997,    -1,    -1,    -1,
      -1,    -1,    -1,   624,    -1,    -1,    -1,  1008,  1009,    -1,
    1011,    -1,    -1,    28,    -1,    -1,    -1,    -1,   376,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,
      -1,    -1,   638,    -1,    -1,    -1,    -1,    11,   644,    -1,
     398,  1042,    -1,    -1,    -1,  1046,   404,    -1,    -1,    23,
      -1,    25,    -1,    -1,   412,    -1,   662,    -1,    -1,    -1,
      -1,    -1,    -1,    78,   685,   686,    -1,    82,    -1,   675,
     676,    -1,   693,    -1,   695,    -1,    -1,    -1,    -1,    -1,
      -1,   702,    -1,    -1,   705,    -1,    -1,   708,    -1,    -1,
     696,    -1,   698,    -1,    -1,     2,    70,   703,    -1,    -1,
     706,    -1,    -1,    -1,    11,    -1,    -1,   713,   466,    -1,
     716,    -1,   470,    -1,   129,   130,    23,    -1,    25,   134,
     478,   136,   137,    97,    -1,   746,   141,   101,    -1,    -1,
      -1,    -1,   738,    -1,   149,    -1,    -1,    -1,    -1,    -1,
     155,   156,   157,    -1,    -1,   751,    19,   753,    -1,    -1,
      23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    34,    35,    70,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,     2,   852,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    11,    -1,    -1,    -1,
      97,    -1,    -1,    66,   101,    68,    -1,    70,    23,    72,
      25,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   829,   582,    98,    -1,    -1,   101,    -1,
     836,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   862,   848,    -1,    -1,    70,    -1,   921,    -1,    76,
      -1,    -1,   159,    -1,   860,    -1,    -1,   231,    -1,    -1,
      28,    -1,    -1,   869,   238,    -1,    -1,   873,    -1,    -1,
      -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,    -1,
     638,   154,    -1,    -1,   905,   906,   644,   160,   909,   910,
     964,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,    -1,    -1,    -1,    -1,   926,   912,    -1,    -1,    -1,
      78,   932,    -1,   934,    82,   936,    -1,    -1,    -1,   293,
      -1,    -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,    -1,
      -1,   238,    -1,   307,   159,  1009,    -1,    -1,   696,    -1,
     698,    -1,    -1,    -1,    -1,   703,    -1,    -1,   706,    -1,
     956,   957,   958,   959,   181,   713,    -1,    -1,   716,    -1,
      -1,   129,   130,    -1,   132,   133,   134,   194,   136,   137,
     197,     3,  1046,   141,    -1,   981,   982,    -1,    -1,    -1,
     738,   149,    -1,    15,    16,    -1,   293,   155,   156,   157,
      22,    -1,    -1,   751,    -1,   753,    -1,    -1,    30,    -1,
     307,    -1,   376,    -1,    36,  1026,   231,   234,    -1,    41,
      -1,    43,    28,   238,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,   398,   252,    -1,    -1,    -1,    61,
     404,    -1,    -1,    -1,    -1,    67,    -1,    -1,   412,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    88,    -1,    90,    -1,
      92,    -1,    78,    95,    -1,    -1,    82,    99,   293,   376,
     102,   829,    -1,   105,    -1,    -1,    -1,    -1,   836,    -1,
      -1,    -1,   307,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     848,   398,   466,    -1,    -1,    -1,   470,   404,    -1,    -1,
      -1,    -1,   860,    -1,   478,   412,    -1,     3,    -1,    -1,
      -1,   869,    -1,   129,   130,   873,    -1,   133,   134,    15,
     136,   137,    -1,     2,    -1,   141,    22,   159,    -1,   161,
      -1,    -1,    11,   149,     2,    -1,    -1,    -1,    -1,   155,
     156,   157,    -1,    11,    23,    41,    25,    -1,    -1,    -1,
      46,   376,    -1,    -1,   912,    23,    -1,    25,    54,   466,
      -1,    -1,    -1,   470,    -1,    61,   393,    -1,    -1,    -1,
      -1,   478,    -1,   398,    -1,    -1,    -1,    28,    -1,   404,
      76,    -1,    -1,    -1,    -1,    -1,    -1,   412,    -1,    -1,
      -1,    70,    88,   420,    -1,    -1,    92,    -1,   956,   957,
     958,   959,    70,    99,    -1,    -1,    -1,    -1,   582,   105,
      -1,    -1,    -1,    -1,    -1,    -1,     2,    28,    97,    -1,
      -1,    -1,   101,   981,   982,    11,    -1,    78,    -1,    97,
      -1,    82,    -1,   101,    -1,    -1,    -1,    23,    -1,    25,
      -1,   466,    -1,    -1,    -1,   470,    -1,    -1,    -1,    -1,
      -1,    -1,   479,   478,    -1,   482,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   638,    -1,    -1,    78,    -1,    -1,
     644,    82,    -1,    -1,    -1,   582,    -1,    -1,   129,   130,
     159,   132,   133,   134,    70,   136,   137,   138,    -1,    -1,
     141,   159,    -1,    -1,    -1,    -1,    -1,   148,   149,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,
      -1,    97,    -1,    -1,    -1,   101,    -1,    -1,   129,    -1,
      -1,    -1,   696,   134,   698,   136,   137,    -1,    -1,   703,
     141,   638,   706,    -1,    -1,    -1,    -1,   644,   149,   713,
      -1,    -1,   716,    -1,   155,   156,   157,    -1,    -1,    -1,
      -1,    -1,   231,    -1,    -1,    -1,    -1,   582,    -1,   238,
      -1,    -1,    -1,   231,   738,    -1,    -1,    -1,   595,    -1,
     238,    -1,   599,   159,    -1,    -1,    -1,   751,    -1,   753,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   696,
      -1,   698,    -1,    -1,    -1,    -1,   703,   624,    -1,   706,
      -1,   628,    -1,    -1,    -1,    -1,   713,    -1,    -1,   716,
      -1,     2,    -1,   638,   293,    -1,    -1,    -1,    -1,   644,
      11,    -1,    -1,    -1,    -1,   293,    -1,    -1,   307,    -1,
      -1,   738,    23,    -1,    25,   662,    -1,    -1,    -1,   307,
      -1,    -1,    -1,    -1,   751,   231,   753,    -1,    -1,    -1,
      -1,    -1,   238,    -1,    -1,   829,    -1,    -1,    -1,    -1,
      -1,    -1,   836,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   696,    -1,   698,   848,    -1,    -1,    -1,   703,    70,
      -1,   706,    -1,    -1,    -1,    -1,   860,    -1,   713,    -1,
      -1,   716,    -1,   720,    -1,   869,   723,   376,    -1,   873,
      -1,    -1,    -1,    -1,    -1,    -1,    97,   293,   376,    -1,
     101,    -1,    -1,   738,    -1,    -1,   743,    -1,    -1,   398,
      -1,   307,   829,    -1,    64,   404,   751,    -1,   753,   836,
     398,    -1,    -1,   412,   761,    -1,   404,    -1,   912,    -1,
      -1,   848,    -1,    -1,   412,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   860,    -1,    -1,    -1,    -1,   785,    -1,
      -1,    -1,   869,    -1,    -1,    -1,   873,    -1,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   956,   957,   958,   959,    -1,   466,    -1,    -1,
     376,   470,    -1,    -1,    -1,    -1,    -1,    -1,   466,   478,
      -1,    -1,   470,    -1,   829,   912,    -1,   981,   982,    -1,
     478,   836,   398,    -1,    -1,    -1,    -1,    -1,   404,    -1,
      -1,    -1,    -1,   848,    -1,    -1,   412,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   860,    -1,    -1,    -1,    -1,
     231,    -1,    -1,    -1,   869,    -1,    -1,   238,   873,   956,
     957,   958,   959,    -1,    -1,    -1,    -1,    -1,    -1,   199,
     200,   201,   202,   890,    -1,    -1,    19,    -1,    -1,    -1,
      23,    24,    -1,    -1,   981,   982,    -1,    -1,    -1,    32,
     466,    34,    35,    -1,   470,    -1,    39,   912,    -1,    -1,
      -1,    44,   478,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   241,   293,   582,   244,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,   582,    68,   307,    70,    -1,    72,
      -1,    -1,    75,   263,    77,    -1,    79,    -1,    -1,    -1,
      -1,   956,   957,   958,   959,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   283,    -1,    98,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,    -1,   981,   982,    -1,   638,
      -1,    -1,    -1,    -1,    -1,   644,    -1,    -1,    -1,    -1,
     638,    -1,    -1,    -1,    -1,    -1,   644,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   582,    -1,    -1,    -1,
      -1,   154,    -1,    -1,    -1,    -1,    -1,   398,    -1,    -1,
      -1,    -1,    -1,   404,    -1,    -1,    -1,   696,    -1,   698,
      -1,    -1,    -1,    -1,   703,    -1,    -1,   706,   696,    -1,
     698,    -1,    -1,    -1,   713,   703,    -1,   716,   706,   379,
      -1,    -1,    -1,    -1,    -1,   713,    -1,    -1,   716,    -1,
      -1,    -1,   638,    -1,    -1,    -1,    -1,    -1,   644,   738,
      -1,    -1,    -1,   403,    -1,    -1,    -1,   407,    -1,    -1,
     738,    -1,   751,    -1,   753,   466,    -1,    -1,    -1,   470,
      -1,    -1,    -1,   751,    -1,   753,    -1,   478,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     696,    -1,   698,    -1,    -1,    -1,    -1,   703,    -1,    -1,
     706,    -1,    -1,    -1,    -1,    -1,    -1,   713,    -1,    -1,
     716,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,
     829,    -1,   738,    -1,    -1,    -1,    -1,   836,    -1,    -1,
      -1,   829,    -1,    -1,    -1,   751,    -1,   753,   836,   848,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     848,   860,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     869,   582,   860,    -1,   873,   129,   130,    -1,   132,   133,
     134,   869,   136,   137,   138,   873,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,   150,   151,   152,    -1,
      -1,   155,   156,   157,    -1,   565,   566,    -1,    -1,    -1,
      -1,   571,   572,   912,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   829,   912,    -1,    -1,   638,    -1,    -1,
     836,    -1,    -1,   644,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   848,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   860,    -1,    -1,   956,   957,   958,
     959,    -1,    -1,   869,    -1,    -1,    -1,   873,   956,   957,
     958,   959,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   981,   982,    -1,   696,    -1,   698,    -1,    -1,
      -1,    -1,   703,   981,   982,   706,     3,    -1,    -1,    -1,
      -1,    -1,   713,    -1,    -1,   716,   912,    -1,    15,    16,
      -1,    28,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,   685,   686,   738,    -1,    36,
      -1,    -1,    -1,   693,    41,    -1,    43,    -1,    -1,    46,
     751,    -1,   753,    -1,    -1,   705,    -1,    54,   708,    -1,
     956,   957,   958,   959,    61,    -1,    -1,    -1,    -1,    -1,
      67,    78,    -1,    -1,    -1,    82,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,   981,   982,    -1,    85,    86,
      -1,    88,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    99,   100,    -1,   102,   113,    -1,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,    -1,   132,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   848,   155,   156,
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   860,
      -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,   869,    -1,
      -1,    -1,   873,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,     0,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,   912,   862,    19,    20,    21,    22,    23,    24,    25,
      26,    27,   158,    29,    30,   161,    32,   163,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    -1,    -1,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    -1,    95,
      96,    97,    98,    99,    -1,   101,   102,   103,   104,   105,
      -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,    -1,    -1,   151,   152,    -1,   154,    -1,
      -1,   157,    -1,   159,    -1,   161,     1,   163,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,
      -1,    -1,   157,    -1,   159,    -1,   161,    -1,   163,   164,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    12,    13,    14,    15,    16,    17,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    -1,    95,    96,    97,    98,    99,    -1,
     101,   102,   103,   104,   105,    -1,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,    -1,   159,   160,
     161,     1,   163,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    -1,    95,    96,    97,    98,    99,
      -1,   101,   102,   103,   104,   105,    -1,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,
      -1,   151,   152,   153,   154,    -1,    -1,   157,    -1,   159,
     160,   161,     1,   163,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    80,    81,    -1,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    -1,    95,    96,    97,    98,
      99,    -1,   101,   102,   103,   104,   105,    -1,    -1,   108,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
      -1,    -1,   151,   152,    -1,   154,    -1,    -1,   157,    -1,
     159,   160,   161,     1,   163,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    -1,    95,    96,    97,
      98,    99,    -1,   101,   102,   103,   104,   105,    -1,    -1,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,    -1,    -1,   151,   152,    -1,   154,    -1,    -1,   157,
      -1,   159,   160,   161,     1,   163,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    -1,    -1,    -1,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    80,    81,    -1,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    -1,    95,    96,
      97,    98,    99,    -1,   101,   102,   103,   104,   105,    -1,
      -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,    -1,    -1,   151,   152,    -1,   154,    -1,    -1,
     157,    -1,   159,   160,   161,     1,   163,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    -1,    -1,    -1,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    -1,    95,
      96,    97,    98,    99,    -1,   101,   102,   103,   104,   105,
      -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,    -1,    -1,   151,   152,    -1,   154,    -1,
      -1,   157,    -1,   159,   160,   161,     1,   163,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    80,    81,    -1,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    -1,
      95,    96,    97,    98,    99,    -1,   101,   102,   103,   104,
     105,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,    -1,    -1,   151,   152,    -1,   154,
      -1,    -1,   157,    -1,   159,    -1,   161,     1,   163,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    -1,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      -1,    95,    96,    97,    98,    99,    -1,   101,   102,   103,
     104,   105,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,    -1,
     154,    -1,    -1,   157,    -1,   159,    -1,   161,     1,   163,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      -1,    -1,    -1,    66,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    -1,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    -1,    95,    96,    97,    98,    99,    -1,   101,   102,
     103,   104,   105,    -1,    -1,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
      -1,   154,    -1,    -1,   157,    -1,   159,    -1,   161,     1,
     163,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    -1,    60,    61,
      62,    -1,    -1,    -1,    66,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      -1,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    -1,    95,    96,    97,    98,    99,    -1,   101,
     102,   103,   104,   105,    -1,    -1,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,
     152,    -1,   154,    -1,    -1,   157,    -1,   159,    -1,   161,
       1,   163,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    -1,    95,    96,    97,    98,    99,    -1,
     101,   102,   103,   104,   105,    -1,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,
     151,   152,    -1,   154,    -1,    -1,   157,    -1,   159,    -1,
     161,     1,   163,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    -1,    -1,    -1,    66,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    -1,    95,    96,    97,    98,    99,
      -1,   101,   102,   103,   104,   105,    -1,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,
      -1,   151,   152,    -1,   154,    -1,    -1,   157,    -1,   159,
      -1,   161,     1,   163,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    -1,    -1,    -1,    66,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    80,    81,    -1,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    -1,    95,    96,    97,    98,
      99,    -1,   101,   102,   103,   104,   105,    -1,    -1,   108,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
      -1,    -1,   151,   152,    -1,   154,    -1,    -1,   157,    -1,
     159,    -1,   161,     1,   163,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      -1,    79,    80,    81,    -1,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    -1,    95,    96,    97,
      98,    99,    -1,   101,   102,   103,   104,   105,    -1,    -1,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,    -1,    -1,   151,   152,    -1,   154,    -1,    -1,   157,
      -1,   159,    -1,   161,     1,   163,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    -1,    -1,    -1,    66,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    80,    81,    -1,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    -1,    95,    96,
      97,    98,    99,    -1,   101,   102,   103,   104,   105,    -1,
      -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,    -1,    -1,   151,   152,    -1,   154,    -1,    -1,
     157,    -1,   159,    -1,   161,     1,   163,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    -1,    79,    80,    81,    -1,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    -1,    95,
      96,    97,    98,    99,    -1,   101,   102,   103,   104,   105,
      -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,    -1,    -1,   151,   152,    -1,   154,    -1,
      -1,   157,    -1,   159,    -1,   161,     1,   163,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    56,    57,    58,    -1,    60,    61,    62,    -1,    -1,
      -1,    66,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,    80,    81,    -1,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    -1,
      95,    96,    97,    98,    99,    -1,   101,   102,   103,   104,
     105,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,    -1,    -1,   151,   152,    -1,   154,
      -1,    -1,   157,    -1,   159,    -1,   161,     1,   163,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,   153,
      -1,    -1,    -1,   157,    -1,   159,    -1,   161,    -1,   163,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    87,    88,    89,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,    -1,   157,    -1,   159,    -1,   161,    -1,
     163,   164,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    90,
      -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,
     161,    -1,   163,   164,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,
      99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
      -1,    -1,   151,   152,    -1,    -1,    -1,    -1,   157,    -1,
     159,    -1,   161,    -1,   163,   164,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    99,    -1,    -1,   102,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,    -1,   129,   130,   131,   132,   133,    10,    -1,    -1,
      -1,   138,    15,    16,    17,    -1,    -1,    -1,    -1,    22,
      -1,   148,    -1,    -1,   151,   152,    -1,    30,    -1,    -1,
     157,    -1,   159,    36,   161,    -1,   163,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    -1,    88,    -1,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,    -1,    -1,   136,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,   152,
      -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    19,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    98,    99,    -1,   101,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   148,    -1,    12,   151,   152,    15,
      16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,   163,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      96,    97,    -1,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,
      -1,   157,    -1,   159,    -1,   161,    -1,   163,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    92,    -1,    -1,
      95,    96,    97,    -1,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,
      -1,    -1,   157,    -1,   159,    -1,   161,   162,   163,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    96,    97,    -1,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,   153,
      -1,    -1,    -1,   157,    -1,   159,    -1,   161,   162,   163,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    42,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    85,    86,    87,    88,    89,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
      -1,    -1,    -1,    -1,   157,    -1,   159,    -1,   161,    -1,
     163,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    98,    99,    -1,   101,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,   148,    -1,    12,   151,
     152,    15,    16,    -1,    -1,   157,    -1,   159,    22,   161,
      -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,    -1,
     154,    -1,    -1,   157,    -1,   159,    -1,   161,    -1,   163,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    87,    88,    89,    90,    -1,    92,
      -1,    -1,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,    -1,   157,    -1,   159,    -1,   161,    -1,
     163,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    59,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,   148,    -1,    12,   151,
     152,    15,    16,    -1,    -1,   157,    -1,   159,    22,   161,
      -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   148,    -1,    12,   151,   152,    15,
      16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,   163,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,
      -1,   157,    -1,   159,    -1,   161,    -1,   163,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    92,    -1,    -1,
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,   138,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,   148,    -1,    12,   151,   152,    15,    16,
      -1,    -1,   157,    -1,   159,    22,   161,    -1,   163,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,    -1,   159,   160,   161,    -1,   163,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,   148,    -1,    12,   151,   152,    15,    16,    -1,
      -1,   157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,
      -1,    29,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
     148,    -1,    12,   151,   152,    15,    16,    -1,    -1,   157,
      -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,   161,    -1,   163,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,
      99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,    -1,   157,    -1,
     159,    -1,   161,    -1,   163,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
      -1,   159,    -1,   161,    -1,   163,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,
     157,    -1,   159,    -1,   161,    -1,   163,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,   148,    -1,    12,   151,   152,    15,    16,    -1,
      -1,   157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
     148,    -1,    12,   151,   152,    15,    16,    -1,    -1,   157,
      -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,   148,    -1,
      12,   151,   152,    15,    16,    -1,    -1,   157,    -1,   159,
      22,   161,    -1,   163,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,   148,    -1,    12,   151,
     152,    15,    16,    -1,    -1,   157,    -1,   159,    22,   161,
      -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   148,    -1,    12,   151,   152,    15,
      16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,   163,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,   148,    -1,    12,   151,   152,    15,    16,    -1,
      -1,   157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
     148,    -1,    12,   151,   152,    15,    16,    -1,    -1,   157,
      -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,   148,    -1,
      12,   151,   152,    15,    16,    -1,    -1,   157,    -1,   159,
      22,   161,    -1,   163,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,   148,    -1,    12,   151,
     152,    15,    16,    -1,    -1,   157,    -1,   159,    22,   161,
      -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   148,    -1,    12,   151,   152,    15,
      16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,   163,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,   148,    -1,    12,   151,   152,    15,    16,    -1,
      -1,   157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,
     138,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
     148,    -1,    12,   151,   152,    15,    16,    -1,    -1,   157,
      -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,   148,    -1,
      12,   151,   152,    15,    16,    -1,    -1,   157,    -1,   159,
      22,   161,    -1,   163,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,   148,    -1,    12,   151,
     152,    15,    16,    -1,    -1,   157,    -1,   159,    22,   161,
      -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   148,    -1,    12,   151,   152,    15,
      16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,   163,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,   148,    -1,    12,   151,   152,    15,    16,    -1,
      -1,   157,    -1,    -1,    22,   161,    -1,   163,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
     148,    15,    16,    -1,   152,    -1,    -1,    -1,    22,   157,
      -1,   159,    -1,   161,    -1,   163,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,   131,    -1,    -1,
      -1,   135,    22,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,   153,
      40,    41,    -1,    43,    -1,   159,    46,   161,   162,   163,
      50,    51,    52,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    94,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,   107,    -1,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,    -1,     3,     4,     5,     6,     7,     8,    -1,
     145,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,   158,   154,    -1,   161,    -1,   163,   159,
      30,   161,    -1,   163,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,   131,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,   159,
      46,   161,    -1,   163,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,   131,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,   159,    46,   161,    -1,   163,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,     1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    19,    -1,    95,    -1,    23,    24,    99,    -1,    -1,
     102,    -1,    -1,   105,    32,    -1,    34,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
      -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,    66,    -1,
      68,    -1,    70,    -1,    72,    73,    -1,    75,    -1,    77,
      -1,    79,    19,    -1,    -1,    -1,    23,    24,    -1,   161,
      -1,   163,    -1,    -1,    -1,    32,    -1,    34,    35,    -1,
      98,     3,    39,   101,    -1,    -1,   104,    44,    -1,    -1,
      47,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    66,
      -1,    68,    -1,    70,    36,    72,    73,    -1,    75,    41,
      77,    43,    79,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    -1,   154,    -1,     3,    61,
      -1,    98,   160,    -1,   101,    67,    -1,   104,    -1,    -1,
      15,    16,    -1,    -1,    76,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    85,    86,    30,    88,    -1,    90,    -1,
      92,    36,    -1,    95,    -1,    -1,    41,    99,    43,    -1,
     102,    46,    -1,   105,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,   154,    -1,    -1,
      -1,    -1,    67,   160,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    76,    10,    -1,    -1,    -1,    -1,    15,    16,    17,
      85,    86,    -1,    88,    22,    90,    -1,    92,    -1,    -1,
      95,    -1,    30,    -1,    99,    -1,    -1,   102,    36,   161,
     105,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    10,    67,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    85,    86,    -1,
      88,    -1,    90,    -1,    92,    -1,   161,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,   132,   133,    78,    -1,   136,    -1,
      82,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,    -1,   152,    10,    -1,   155,   156,   157,
     158,    -1,    17,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,   113,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,    -1,    10,   155,   156,   157,    -1,    -1,    -1,    17,
      -1,    -1,   164,    78,    -1,    -1,    -1,    82,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,   113,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,
      78,   136,   137,   138,    82,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    10,    -1,    -1,    -1,   164,
      -1,    -1,    17,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    28,    82,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,   113,    -1,   155,   156,   157,
      -1,    -1,    -1,    -1,    28,    -1,   164,    -1,    -1,    -1,
      -1,   129,   130,    78,   132,   133,   134,    82,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,    10,    -1,   155,   156,   157,
      -1,    -1,    17,    -1,    -1,    -1,   164,    -1,   113,    -1,
      -1,    -1,    -1,    28,    78,    -1,    -1,    -1,    82,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,    -1,   162,    -1,    -1,
      -1,    -1,    -1,    78,    -1,   129,   130,    82,   132,   133,
     134,    10,   136,   137,    -1,    -1,    -1,   141,    17,    -1,
      -1,    -1,    -1,    -1,   148,   149,    -1,    -1,   152,    28,
      -1,   155,   156,   157,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    -1,    78,
     155,   156,   157,    82,    -1,    -1,    -1,   162,    -1,    -1,
      -1,    10,    91,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    -1,    -1,   155,   156,   157,    78,
     159,    -1,    -1,    82,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    91,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    -1,    78,   155,   156,   157,    82,
     159,    -1,    -1,    10,    -1,    -1,    -1,    -1,    91,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      -1,    78,   155,   156,   157,    82,   159,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    91,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,    -1,    78,   155,   156,
     157,    82,   159,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      91,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,    -1,    78,   155,   156,   157,    82,   159,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    91,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    -1,    78,
     155,   156,   157,    82,   159,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    91,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    -1,    78,   155,   156,   157,    82,
     159,    -1,    -1,    10,    -1,    -1,    -1,    -1,    91,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      -1,    78,   155,   156,   157,    82,   159,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,    10,    -1,   155,   156,
     157,    -1,   159,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,   113,
      28,    29,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,   143,
     144,    -1,   146,   147,   148,   149,   150,   151,   152,    -1,
     113,   155,   156,   157,    -1,   159,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    28,    82,    -1,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      10,   109,   155,   156,   157,   113,   159,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,   129,   130,    78,   132,   133,   134,    82,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,    10,    -1,   155,   156,   157,
      -1,   159,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    82,    -1,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,   150,    -1,   152,    -1,    -1,
     155,   156,   157,   113,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,   129,
     130,    -1,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,    10,    -1,   155,   156,   157,   113,   159,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    28,    82,    -1,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    10,    -1,
     155,   156,   157,    -1,   159,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,   129,
     130,    78,   132,   133,   134,    82,   136,   137,   138,    -1,
      -1,   141,   142,   143,    91,    -1,   146,   147,   148,   149,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,   113,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    28,
      82,    -1,   129,   130,    33,   132,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,    -1,    -1,   155,   156,
     157,   113,   159,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    78,
     132,   133,   134,    82,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,    -1,    -1,   155,   156,   157,    -1,   159,    10,    -1,
      -1,    -1,    -1,    -1,   113,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      42,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    28,
      82,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,   113,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    78,
     132,   133,   134,    82,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,   113,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      42,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    10,    -1,   155,   156,   157,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    -1,    -1,    78,    -1,    -1,    -1,
      82,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    82,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,    10,    -1,   155,   156,   157,    -1,   113,    17,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    82,    -1,    -1,    28,
      -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,   113,    -1,   155,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,    78,
     136,   137,   138,    82,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,    10,   154,   155,
     156,   157,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    10,   154,   155,   156,   157,    -1,
      -1,    17,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
     113,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,   129,   130,    -1,   132,
     133,   134,    78,   136,   137,   138,    82,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      10,   154,   155,   156,   157,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    28,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,    91,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,   113,   154,   155,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    82,   129,   130,    -1,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,    10,    -1,   155,
     156,   157,   112,   113,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,   129,
     130,    -1,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    28,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    78,   132,
     133,   134,    82,   136,   137,   138,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    10,    11,   109,
      -1,    -1,    -1,   113,    17,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    82,    -1,    -1,    28,    -1,    -1,    -1,   129,
     130,    -1,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,   113,    -1,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,    -1,   132,   133,   134,    78,   136,   137,   138,    82,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,    10,   154,   155,   156,   157,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      10,    -1,   155,   156,   157,    -1,    -1,    17,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    82,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,    -1,   132,   133,   134,    78,   136,
     137,   138,    82,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,    -1,    -1,   155,   156,
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,   112,   113,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,    -1,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,   113,   146,   147,   148,   149,
     150,   151,   152,    10,    -1,   155,   156,   157,    -1,    -1,
      17,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,    28,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,    10,    -1,   155,   156,   157,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,   113,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,    -1,   132,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,   143,   144,   113,   146,
     147,   148,   149,   150,   151,   152,    10,    -1,   155,   156,
     157,    -1,    -1,    17,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    28,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    10,    11,
     155,   156,   157,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,   113,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,   143,
     144,   113,   146,   147,   148,   149,   150,   151,   152,    10,
      -1,   155,   156,   157,    -1,    -1,    17,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,    28,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,    10,    -1,   155,   156,   157,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,   113,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,   143,   144,   113,   146,   147,   148,   149,   150,
     151,   152,    10,    -1,   155,   156,   157,    -1,    -1,    17,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      28,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    10,    -1,   155,   156,   157,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,   113,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,   113,   146,   147,
     148,   149,   150,   151,   152,    10,    -1,   155,   156,   157,
      -1,    -1,    17,   129,   130,    -1,   132,   133,   134,    -1,
     136,   137,   138,    28,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,    10,    -1,   155,
     156,   157,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,   113,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
     113,   146,   147,   148,   149,   150,   151,   152,    10,    -1,
     155,   156,   157,    -1,    -1,    17,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,    28,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      10,    -1,   155,   156,   157,    -1,    -1,    17,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    78,    -1,
      -1,   113,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,   113,   146,   147,   148,   149,   150,   151,
     152,    -1,    -1,   155,   156,   157,    -1,    78,    -1,   129,
     130,    82,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,    -1,    -1,   155,   156,   157,     1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,     3,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    15,    16,
      -1,    -1,    76,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    85,    86,    30,    88,    -1,    90,    -1,    92,    36,
      -1,    95,    -1,    -1,    41,    99,    43,    -1,   102,    46,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      -1,    88,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   169,   170,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    29,    30,    32,    34,    35,
      36,    37,    38,    39,    40,    41,    43,    44,    46,    49,
      50,    51,    53,    54,    55,    57,    58,    60,    61,    62,
      66,    67,    68,    69,    70,    71,    72,    75,    76,    77,
      79,    80,    81,    83,    84,    85,    86,    87,    88,    89,
      90,    92,    93,    95,    96,    97,    98,    99,   101,   102,
     103,   104,   105,   108,   110,   113,   129,   130,   131,   132,
     133,   138,   148,   151,   152,   154,   157,   159,   161,   163,
     171,   172,   173,   174,   175,   176,   178,   183,   184,   185,
     189,   191,   195,   196,   197,   198,   199,   200,   202,   203,
     204,   208,   209,   212,   213,   216,   217,   220,   223,   224,
     250,   253,   254,   274,   275,   276,   277,   278,   279,   287,
     288,   289,   290,   293,   294,   295,   296,   297,   298,   299,
     301,   302,   303,   304,   305,   154,   173,   290,   109,   280,
     281,     3,   186,    12,    37,    38,    40,    90,   159,   163,
     220,   274,   278,   288,   289,   290,   293,   295,   296,   280,
     290,    98,   253,    79,   186,   173,   268,   290,   173,   161,
     290,   291,     7,     9,    98,   193,   291,    68,   111,   201,
     290,   201,   290,    23,    35,   254,   290,   290,   161,     3,
      15,    16,    22,    30,    36,    41,    43,    46,    54,    61,
      67,    76,    85,    86,    88,    90,    92,    95,    99,   102,
     105,   188,   190,   221,   161,   188,   255,   256,    29,   178,
     194,   290,   290,   290,   290,   290,   290,     7,   161,    35,
     197,   197,   153,   243,   267,   290,    96,    97,   154,   290,
     292,   290,   194,   290,   290,   290,    89,   161,   173,   290,
     290,   178,   185,   290,   293,   178,   185,   290,   188,   251,
     290,   267,   290,   290,   290,   290,   290,   290,   290,   290,
       1,   160,   171,   179,   267,   100,   139,   243,   269,   270,
     292,   201,   267,   290,   300,    70,   173,   159,    74,   177,
     188,   188,    47,    73,   238,   255,   153,   154,   145,    10,
      17,    28,    78,    82,   113,   129,   130,   132,   133,   134,
     136,   137,   138,   140,   141,   142,   143,   144,   146,   147,
     148,   149,   150,   151,   152,   155,   156,   157,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   158,   230,   161,   163,    78,    82,   161,
     173,   154,   290,   290,   290,   267,   159,   178,    42,   280,
     251,   255,   154,   135,   154,   108,   189,   243,   271,   272,
     273,   292,   251,   213,   253,   188,   161,   194,    29,    42,
     194,   109,   194,   283,    29,    42,   194,   283,   291,    33,
      63,   154,    91,   178,   230,   271,   173,   161,   232,   100,
     161,   188,   257,   258,     1,   134,   262,    42,   135,   173,
     194,   194,   271,   188,   135,   154,   290,   290,   154,   159,
     194,   161,   271,   154,   205,   154,   205,   154,   114,   252,
     154,   154,    33,    63,   194,   154,   160,   160,   171,   135,
     160,   290,   135,   162,   135,   162,   164,   283,    42,   135,
     164,   283,   112,   135,   164,     7,     1,   160,   179,    56,
     134,   214,   159,   225,   154,   290,   290,   290,   290,   201,
     290,   201,   290,   290,   290,   290,   290,    16,   102,   290,
     290,    30,    54,    98,   189,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   292,   292,   292,   292,   292,   271,   271,   201,   290,
     201,   290,    24,    42,    79,   104,   282,   285,   286,   290,
     305,    29,    42,    29,    42,    91,    42,   164,   201,   290,
     194,   154,   154,   290,   290,   114,   162,   135,   154,   188,
     255,    42,   267,    40,   290,   201,   290,   161,   194,    40,
     290,   201,   290,   194,   157,   180,   182,   290,   180,   181,
     173,   290,    31,   290,   162,    24,    42,    45,    65,    68,
      79,    98,   172,   233,   234,   235,   236,   222,   258,   135,
     162,    16,    30,    43,    67,    85,    86,    90,   102,   163,
     192,   263,   275,   114,   259,   290,   256,   162,   243,   290,
       1,   210,   271,   162,    18,   206,   263,   275,   135,   182,
     181,   160,   162,   162,   269,   162,   269,   173,   164,   201,
     290,   164,   173,   290,   164,   290,   164,   290,   160,   160,
     188,   267,   159,     1,   188,   218,   219,    24,    68,    79,
      98,   227,   237,   154,   154,   154,   154,   154,   162,   164,
      42,    79,   135,   162,   274,    78,    78,    40,   201,   290,
      40,   201,   290,   201,   290,   280,   280,   154,   243,   292,
     273,   214,   154,   290,   162,   290,    29,   194,    29,   194,
     284,   285,   290,    29,   194,   283,    29,   194,   283,   135,
     154,    11,   154,    31,    31,   173,    91,   178,    42,    79,
     235,   135,   162,   161,   188,    24,    68,    79,    98,   239,
     162,   258,   262,     1,   267,    59,   292,   160,    64,   106,
     160,   211,   162,   161,   178,   188,   207,   251,   154,   154,
     173,   164,   283,   164,   283,   173,   112,     1,   215,   160,
     114,   135,   160,    79,   226,     1,     3,    10,    15,    17,
      22,    36,    41,    46,    48,    54,    61,    76,    88,    92,
      95,    99,   105,   129,   130,   131,   132,   133,   136,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     152,   155,   156,   157,   158,   161,   187,   188,   190,   228,
     229,   230,   274,   285,   262,   274,   274,   290,    29,    29,
     290,    29,    29,   164,   164,   194,   194,   159,   194,    91,
      40,   290,    40,   290,   135,   162,    91,    40,   290,   194,
      40,   290,   194,   290,   290,   173,   290,   173,    31,   188,
     234,   258,   134,   266,    79,   262,   259,   164,    42,   164,
      29,   173,   267,   207,   134,   178,    40,   173,   290,   164,
      40,   173,   290,   164,   290,   160,    35,    72,    75,   160,
     172,   196,   290,   219,   239,   161,   231,   188,   290,   129,
     137,   231,   231,   259,    91,    40,    40,    91,    40,    40,
      40,    40,     1,   215,   290,   290,   290,   285,   290,   290,
     290,    11,    31,   173,   266,   162,   163,   192,   243,   265,
     275,   139,   245,   259,    52,   107,   246,   290,   263,   275,
     173,   194,   162,   290,   290,   173,   290,   173,   196,     1,
     134,   261,   233,   162,     3,    92,   229,   230,   290,   290,
     290,   290,   290,   290,   160,   160,    91,    91,    91,    91,
     290,   173,   259,   266,   164,   267,   243,   244,   290,     3,
      81,    92,   247,   248,   249,   290,   178,   195,   242,   164,
     178,    91,    91,   163,   192,   260,   275,    94,   240,   162,
     231,   231,    91,    91,    91,    91,    91,    91,   290,   290,
     290,   290,   245,   259,   243,   264,   265,   275,    42,   164,
     249,   107,   135,   114,   140,   142,   143,   146,   147,    52,
     275,   290,   290,     1,   164,   267,   246,   290,   264,   265,
     290,   248,   249,   249,   249,   249,   249,   249,   247,   164,
     260,   275,   164,   154,   241,   242,   164,   260,   275,   264
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   168,   169,   170,   170,   171,   171,   172,   172,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   174,
     175,   175,   175,   176,   176,   176,   177,   177,   178,   178,
     178,   179,   179,   180,   180,   180,   180,   181,   181,   182,
     182,   183,   183,   183,   184,   185,   185,   185,   185,   185,
     186,   186,   187,   187,   187,   187,   187,   188,   188,   188,
     188,   188,   189,   189,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   192,   192,   192,   192,   192,   192,   192,   192,
     193,   193,   194,   194,   195,   195,   196,   196,   196,   196,
     196,   196,   196,   196,   197,   197,   198,   198,   198,   198,
     199,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   201,   202,   202,
     202,   202,   202,   202,   202,   202,   203,   204,   204,   204,
     204,   204,   204,   205,   205,   206,   206,   206,   207,   207,
     208,   209,   209,   210,   210,   211,   211,   211,   212,   212,
     212,   212,   213,   213,   213,   214,   214,   215,   215,   215,
     216,   216,   217,   218,   218,   218,   219,   219,   221,   222,
     220,   223,   223,   223,   223,   223,   225,   226,   224,   227,
     227,   227,   227,   227,   228,   228,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   231,   231,   232,   233,
     233,   233,   234,   234,   234,   234,   234,   235,   235,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   237,   237,
     237,   237,   237,   237,   238,   238,   239,   239,   239,   239,
     239,   239,   240,   240,   241,   241,   242,   242,   243,   243,
     244,   244,   245,   245,   246,   246,   246,   246,   246,   247,
     247,   248,   248,   248,   248,   248,   248,   248,   249,   249,
     250,   250,   250,   251,   251,   252,   252,   252,   253,   253,
     253,   253,   253,   254,   254,   254,   255,   255,   256,   256,
     257,   257,   257,   258,   258,   258,   259,   259,   259,   260,
     260,   260,   260,   260,   260,   260,   261,   261,   261,   261,
     261,   262,   262,   262,   262,   262,   263,   263,   263,   263,
     264,   264,   264,   265,   265,   265,   265,   265,   266,   266,
     266,   266,   266,   267,   267,   267,   267,   268,   268,   269,
     269,   269,   270,   270,   271,   271,   272,   272,   273,   273,
     273,   273,   274,   274,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   277,   278,
     279,   279,   279,   279,   279,   279,   279,   279,   280,   280,
     281,   282,   282,   283,   284,   284,   285,   285,   285,   286,
     286,   286,   286,   286,   286,   287,   287,   288,   289,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   291,   291,   292,   292,   292,
     293,   293,   293,   293,   294,   294,   295,   295,   295,   296,
     296,   296,   296,   297,   297,   297,   297,   298,   298,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   300,   300,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   302,   302,   302,
     302,   302,   302,   302,   303,   303,   303,   303,   304,   304,
     304,   304,   305,   305,   305,   305,   305,   305,   305
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     2,     1,     2,     3,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     3,     5,     5,     2,
       2,     5,     3,     6,     6,     4,     5,     5,     3,     3,
       6,     5,     6,     5,     6,     3,     4,     3,     4,     6,
       7,     6,     7,     4,     5,     4,     5,     4,     4,     3,
       6,     5,     6,     5,     8,     7,     2,     3,     3,     2,
       2,     3,     3,     0,     2,     2,     3,     5,     1,     3,
       3,     5,     5,     0,     2,     3,     2,     3,     6,     8,
       6,     8,     1,     1,     1,     0,     2,     0,     2,     3,
       5,     5,     1,     1,     2,     3,     1,     3,     0,     0,
       8,     0,     1,     2,     2,     1,     0,     0,    10,     3,
       3,     5,     5,     3,     1,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     3,     0,
       1,     3,     4,     5,     4,     6,     6,     0,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     0,     1,
       1,     2,     1,     1,     1,     1,     0,     1,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     2,     1,
       0,     1,     2,     2,     0,     2,     2,     4,     4,     1,
       3,     3,     3,     3,     3,     3,     3,     2,     1,     1,
       3,     4,     4,     2,     4,     0,     2,     2,     1,     1,
       1,     2,     1,     4,     5,     3,     1,     3,     3,     5,
       1,     1,     3,     1,     2,     3,     0,     2,     2,     3,
       2,     4,     3,     3,     4,     3,     0,     2,     2,     2,
       1,     0,     2,     2,     2,     1,     4,     4,     6,     3,
       0,     1,     1,     3,     4,     3,     4,     6,     0,     2,
       2,     2,     2,     1,     1,     3,     3,     1,     3,     1,
       1,     1,     3,     3,     0,     1,     1,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       4,     4,     4,     5,     2,     2,     2,     2,     2,     2,
       2,     6,     6,     4,     9,     9,     7,     6,     6,     4,
       9,     9,     7,     4,     6,     6,     9,     9,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       4,     1,     3,     4,     1,     3,     4,     3,     3,     1,
       1,     2,     1,     2,     1,     1,     3,     2,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     3,
       3,     3,     2,     2,     1,     0,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     4,     3,
       3,     3,     3,     3,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     4,     3,     4,
       3,     4,     3,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
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
#line 5666 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 3:
#line 492 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5672 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 4:
#line 493 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5678 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 6:
#line 500 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5684 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 7:
#line 505 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5690 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 8:
#line 506 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5696 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 23:
#line 525 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5702 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 24:
#line 526 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5708 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 25:
#line 527 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5714 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 26:
#line 528 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5720 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 27:
#line 529 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5726 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 28:
#line 530 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5732 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 29:
#line 531 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5738 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 30:
#line 532 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5744 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 31:
#line 533 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5750 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 32:
#line 534 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5756 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 33:
#line 535 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5762 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 34:
#line 536 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5768 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 35:
#line 537 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5774 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 36:
#line 538 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5780 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 37:
#line 539 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5786 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 38:
#line 540 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5792 "bison-chapel.cpp" /* yacc.c:1666  */
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
#line 5806 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 40:
#line 558 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 5815 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 41:
#line 563 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 5824 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 42:
#line 568 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 5833 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 43:
#line 575 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5839 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 44:
#line 576 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5845 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 45:
#line 577 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5851 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 46:
#line 581 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; }
#line 5857 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 47:
#line 582 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true;  }
#line 5863 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 48:
#line 595 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5869 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 49:
#line 596 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5875 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 50:
#line 597 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5881 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 51:
#line 602 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5887 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 52:
#line 603 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5893 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 53:
#line 608 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5903 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 54:
#line 613 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5913 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 55:
#line 618 "chapel.ypp" /* yacc.c:1666  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5922 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 56:
#line 622 "chapel.ypp" /* yacc.c:1666  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5931 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 57:
#line 629 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5941 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 59:
#line 638 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5951 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 61:
#line 647 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5957 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 62:
#line 648 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5963 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 63:
#line 649 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5969 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 64:
#line 653 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5975 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 65:
#line 658 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 5981 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 66:
#line 660 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5987 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 67:
#line 662 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5993 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 68:
#line 664 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5999 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 69:
#line 666 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 6005 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 70:
#line 672 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = NULL; }
#line 6011 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 71:
#line 673 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6017 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 72:
#line 677 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6023 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 73:
#line 678 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "this"; }
#line 6029 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 74:
#line 679 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6035 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 75:
#line 680 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6041 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 76:
#line 681 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6047 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 77:
#line 684 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6053 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 78:
#line 685 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6059 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 79:
#line 686 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6065 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 80:
#line 687 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6071 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 81:
#line 688 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6077 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 82:
#line 700 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6083 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 83:
#line 701 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "this"; }
#line 6089 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 84:
#line 712 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "bool"; }
#line 6095 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 85:
#line 713 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "int"; }
#line 6101 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 86:
#line 714 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "uint"; }
#line 6107 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 87:
#line 715 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "real"; }
#line 6113 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 88:
#line 716 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "imag"; }
#line 6119 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 89:
#line 717 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "complex"; }
#line 6125 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 90:
#line 718 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "string"; }
#line 6131 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 91:
#line 719 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "sync"; }
#line 6137 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 92:
#line 720 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "single"; }
#line 6143 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 93:
#line 721 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "owned"; }
#line 6149 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 94:
#line 722 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "shared"; }
#line 6155 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 95:
#line 723 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "borrowed"; }
#line 6161 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 96:
#line 724 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "unmanaged"; }
#line 6167 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 97:
#line 725 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "domain"; }
#line 6173 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 98:
#line 726 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "index"; }
#line 6179 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 99:
#line 727 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "locale"; }
#line 6185 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 100:
#line 728 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "nothing"; }
#line 6191 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 101:
#line 729 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "void"; }
#line 6197 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 102:
#line 733 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6203 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 103:
#line 734 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6209 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 104:
#line 735 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6215 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 105:
#line 736 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6221 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 106:
#line 737 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6227 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 107:
#line 738 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6233 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 108:
#line 739 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6239 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 109:
#line 740 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6245 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 110:
#line 741 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 6251 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 111:
#line 742 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 6257 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 112:
#line 749 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_syncvar"; }
#line 6263 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 113:
#line 750 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_singlevar"; }
#line 6269 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 114:
#line 751 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_owned"; }
#line 6275 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 115:
#line 752 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_shared"; }
#line 6281 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 116:
#line 753 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_borrowed"; }
#line 6287 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 117:
#line 754 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_unmanaged"; }
#line 6293 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 118:
#line 755 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_domain"; }
#line 6299 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 119:
#line 756 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_index"; }
#line 6305 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 120:
#line 761 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = NULL; }
#line 6311 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 121:
#line 762 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6317 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 122:
#line 766 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6323 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 123:
#line 767 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6329 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 124:
#line 771 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6335 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 125:
#line 772 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6341 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 126:
#line 776 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6347 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 128:
#line 778 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6353 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 129:
#line 779 "chapel.ypp" /* yacc.c:1666  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6359 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 136:
#line 792 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6365 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 137:
#line 793 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6371 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 138:
#line 794 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6377 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 139:
#line 795 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6383 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 140:
#line 800 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6391 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 141:
#line 806 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6397 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 142:
#line 807 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6403 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 143:
#line 808 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 6409 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 144:
#line 809 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6415 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 145:
#line 810 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 6421 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 146:
#line 811 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6427 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 147:
#line 812 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6433 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 148:
#line 813 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6439 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 149:
#line 814 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6445 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 150:
#line 815 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6451 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 151:
#line 816 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6457 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 152:
#line 817 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6463 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 153:
#line 818 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6469 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 154:
#line 819 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6475 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 155:
#line 820 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6481 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 156:
#line 821 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6487 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 157:
#line 822 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6493 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 158:
#line 823 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6499 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 159:
#line 825 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6509 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 160:
#line 831 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6519 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 161:
#line 837 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6529 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 162:
#line 843 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6539 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 163:
#line 849 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6550 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 164:
#line 856 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6561 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 165:
#line 863 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6569 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 166:
#line 867 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6577 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 167:
#line 873 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 6583 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 168:
#line 877 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6589 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 169:
#line 878 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6595 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 170:
#line 879 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6601 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 171:
#line 880 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6607 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 172:
#line 881 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6614 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 173:
#line 883 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6621 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 174:
#line 885 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6628 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 175:
#line 887 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6635 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 176:
#line 892 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6641 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 177:
#line 895 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6647 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 178:
#line 896 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6653 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 179:
#line 897 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6659 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 180:
#line 898 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6665 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 181:
#line 899 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6671 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 182:
#line 900 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6677 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 183:
#line 904 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6683 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 184:
#line 905 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6689 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 185:
#line 909 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6695 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 186:
#line 910 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 6701 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 187:
#line 911 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 6707 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 188:
#line 915 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6713 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 189:
#line 916 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6719 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 190:
#line 920 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6725 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 191:
#line 924 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6731 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 192:
#line 926 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6737 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 193:
#line 930 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6743 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 194:
#line 931 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6749 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 195:
#line 936 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6755 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 196:
#line 938 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6761 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 197:
#line 940 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6767 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 198:
#line 947 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6781 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 199:
#line 957 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6795 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 200:
#line 967 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6809 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 201:
#line 977 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6823 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 202:
#line 989 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6833 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 203:
#line 994 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6843 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 204:
#line 999 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6853 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 205:
#line 1007 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = NULL; }
#line 6859 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 206:
#line 1008 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6865 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 207:
#line 1013 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6871 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 208:
#line 1015 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6877 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 209:
#line 1017 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6883 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 210:
#line 1022 "chapel.ypp" /* yacc.c:1666  */
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
#line 6903 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 211:
#line 1038 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6911 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 212:
#line 1045 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6921 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 213:
#line 1054 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6933 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 214:
#line 1062 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6941 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 215:
#line 1066 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6949 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 216:
#line 1072 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6955 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 217:
#line 1073 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6961 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 218:
#line 1078 "chapel.ypp" /* yacc.c:1666  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6970 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 219:
#line 1083 "chapel.ypp" /* yacc.c:1666  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6979 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 220:
#line 1088 "chapel.ypp" /* yacc.c:1666  */
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
#line 7001 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 221:
#line 1110 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7012 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 222:
#line 1116 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7024 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 223:
#line 1123 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7035 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 224:
#line 1129 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7046 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 225:
#line 1135 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7058 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 226:
#line 1146 "chapel.ypp" /* yacc.c:1666  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 7068 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 227:
#line 1152 "chapel.ypp" /* yacc.c:1666  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7079 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 228:
#line 1159 "chapel.ypp" /* yacc.c:1666  */
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
#line 7106 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 229:
#line 1185 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 7114 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 230:
#line 1189 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7123 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 231:
#line 1194 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 7131 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 232:
#line 1198 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7140 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 233:
#line 1203 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 7148 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 235:
#line 1210 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7154 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 236:
#line 1214 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7160 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 237:
#line 1215 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 7166 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 238:
#line 1216 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "&"; }
#line 7172 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 239:
#line 1217 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "|"; }
#line 7178 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 240:
#line 1218 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "^"; }
#line 7184 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 241:
#line 1219 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "~"; }
#line 7190 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 242:
#line 1220 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "=="; }
#line 7196 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 243:
#line 1221 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "!="; }
#line 7202 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 244:
#line 1222 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<="; }
#line 7208 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 245:
#line 1223 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">="; }
#line 7214 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 246:
#line 1224 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<"; }
#line 7220 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 247:
#line 1225 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">"; }
#line 7226 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 248:
#line 1226 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "+"; }
#line 7232 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 249:
#line 1227 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "-"; }
#line 7238 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 250:
#line 1228 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "*"; }
#line 7244 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 251:
#line 1229 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "/"; }
#line 7250 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 252:
#line 1230 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<<"; }
#line 7256 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 253:
#line 1231 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">>"; }
#line 7262 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 254:
#line 1232 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "%"; }
#line 7268 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 255:
#line 1233 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "**"; }
#line 7274 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 256:
#line 1234 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "!"; }
#line 7280 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 257:
#line 1235 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "chpl_by"; }
#line 7286 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 258:
#line 1236 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "#"; }
#line 7292 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 259:
#line 1237 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "chpl_align"; }
#line 7298 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 260:
#line 1238 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<=>"; }
#line 7304 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 261:
#line 1239 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<~>"; }
#line 7310 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 262:
#line 1240 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "init="; }
#line 7316 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 263:
#line 1241 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 7322 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 264:
#line 1245 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "="; }
#line 7328 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 265:
#line 1246 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "+="; }
#line 7334 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 266:
#line 1247 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "-="; }
#line 7340 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 267:
#line 1248 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "*="; }
#line 7346 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 268:
#line 1249 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "/="; }
#line 7352 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 269:
#line 1250 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "%="; }
#line 7358 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 270:
#line 1251 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "**="; }
#line 7364 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 271:
#line 1252 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "&="; }
#line 7370 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 272:
#line 1253 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "|="; }
#line 7376 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 273:
#line 1254 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "^="; }
#line 7382 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 274:
#line 1255 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">>="; }
#line 7388 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 275:
#line 1256 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<<="; }
#line 7394 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 276:
#line 1260 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7400 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 277:
#line 1261 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7406 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 278:
#line 1265 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7412 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 279:
#line 1269 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7418 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 280:
#line 1270 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7424 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 281:
#line 1271 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7430 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 282:
#line 1276 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7436 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 283:
#line 1278 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7442 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 284:
#line 1280 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7448 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 285:
#line 1282 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7454 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 286:
#line 1284 "chapel.ypp" /* yacc.c:1666  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7460 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 287:
#line 1288 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7466 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 288:
#line 1289 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7472 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 289:
#line 1293 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_IN; }
#line 7478 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 290:
#line 1294 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7484 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 291:
#line 1295 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_OUT; }
#line 7490 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 292:
#line 1296 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST; }
#line 7496 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 293:
#line 1297 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7502 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 294:
#line 1298 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7508 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 295:
#line 1299 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7514 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 296:
#line 1300 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_REF; }
#line 7520 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 297:
#line 1301 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7526 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 298:
#line 1305 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7532 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 299:
#line 1306 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7538 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 300:
#line 1307 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_REF;   }
#line 7544 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 301:
#line 1308 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 7550 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 302:
#line 1309 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST;   }
#line 7556 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 303:
#line 1310 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 7562 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 304:
#line 1314 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 7568 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 305:
#line 1315 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 7574 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 306:
#line 1319 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_VALUE; }
#line 7580 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 307:
#line 1320 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_VALUE; }
#line 7586 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 308:
#line 1321 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 7592 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 309:
#line 1322 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_REF; }
#line 7598 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 310:
#line 1323 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_PARAM; }
#line 7604 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 311:
#line 1324 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_TYPE; }
#line 7610 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 312:
#line 1328 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; }
#line 7616 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 313:
#line 1329 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true;  }
#line 7622 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 314:
#line 1332 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = NULL; }
#line 7628 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 317:
#line 1338 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7634 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 318:
#line 1343 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7640 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 319:
#line 1345 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7646 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 320:
#line 1349 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7652 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 322:
#line 1354 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7658 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 323:
#line 1355 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7664 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 324:
#line 1359 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 7670 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 325:
#line 1361 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 7676 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 326:
#line 1363 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 7682 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 327:
#line 1365 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7688 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 328:
#line 1367 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7694 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 329:
#line 1371 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7700 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 330:
#line 1372 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7706 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 331:
#line 1375 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7712 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 332:
#line 1376 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7718 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 333:
#line 1377 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7724 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 334:
#line 1378 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7730 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 335:
#line 1379 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7736 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 336:
#line 1380 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7742 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 337:
#line 1381 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 7748 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 338:
#line 1384 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 7754 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 339:
#line 1385 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 7760 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 340:
#line 1389 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7766 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 341:
#line 1391 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7772 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 342:
#line 1393 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7778 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 343:
#line 1398 "chapel.ypp" /* yacc.c:1666  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7795 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 344:
#line 1411 "chapel.ypp" /* yacc.c:1666  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7813 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 345:
#line 1427 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7819 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 346:
#line 1429 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7825 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 347:
#line 1431 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7831 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 348:
#line 1435 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 7837 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 349:
#line 1436 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 7843 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 350:
#line 1437 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 7849 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 351:
#line 1438 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 7855 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 352:
#line 1439 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 7861 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 353:
#line 1444 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7871 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 354:
#line 1450 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
      context->latestComment = NULL;
    }
#line 7881 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 355:
#line 1456 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7890 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 357:
#line 1465 "chapel.ypp" /* yacc.c:1666  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7899 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 358:
#line 1473 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7905 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 359:
#line 1475 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7911 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 360:
#line 1480 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7917 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 361:
#line 1482 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7923 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 362:
#line 1484 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7929 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 363:
#line 1489 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7935 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 364:
#line 1491 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7941 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 365:
#line 1493 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7947 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 366:
#line 1499 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7953 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 367:
#line 1500 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7959 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 368:
#line 1501 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7965 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 369:
#line 1507 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7971 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 370:
#line 1509 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7977 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 371:
#line 1511 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7985 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 372:
#line 1515 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7993 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 373:
#line 1519 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7999 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 374:
#line 1521 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8007 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 375:
#line 1525 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8015 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 376:
#line 1532 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8021 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 377:
#line 1533 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8027 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 378:
#line 1534 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8033 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 379:
#line 1535 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8039 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 380:
#line 1536 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8045 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 381:
#line 1541 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8051 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 382:
#line 1542 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8057 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 383:
#line 1543 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 8063 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 384:
#line 1544 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8069 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 385:
#line 1545 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8075 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 386:
#line 1566 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8083 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 387:
#line 1570 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 8091 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 388:
#line 1574 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 8103 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 389:
#line 1582 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8111 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 390:
#line 1588 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8117 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 391:
#line 1589 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8123 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 392:
#line 1590 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8129 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 393:
#line 1595 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8135 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 394:
#line 1597 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8141 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 395:
#line 1603 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8147 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 396:
#line 1605 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8153 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 397:
#line 1607 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 8159 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 398:
#line 1611 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8165 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 399:
#line 1612 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8171 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 400:
#line 1613 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8177 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 401:
#line 1614 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8183 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 402:
#line 1615 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8189 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 403:
#line 1621 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8195 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 404:
#line 1622 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8201 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 405:
#line 1623 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8207 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 406:
#line 1624 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8213 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 407:
#line 1628 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 8219 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 408:
#line 1629 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8225 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 409:
#line 1633 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 8231 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 410:
#line 1634 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8237 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 411:
#line 1635 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8243 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 412:
#line 1639 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8249 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 413:
#line 1640 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8255 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 414:
#line 1644 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 8261 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 416:
#line 1649 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8267 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 417:
#line 1650 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8273 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 418:
#line 1654 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 8279 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 419:
#line 1655 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8285 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 420:
#line 1656 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8291 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 421:
#line 1657 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8297 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 422:
#line 1661 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8303 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 423:
#line 1662 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8309 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 429:
#line 1679 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 8315 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 430:
#line 1681 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 8321 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 431:
#line 1683 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 8327 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 432:
#line 1685 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 8333 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 433:
#line 1687 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 8339 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 434:
#line 1689 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 8345 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 435:
#line 1691 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 8351 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 436:
#line 1693 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 8357 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 437:
#line 1695 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8363 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 438:
#line 1697 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 8369 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 439:
#line 1699 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8375 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 440:
#line 1701 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS, (yyvsp[-1].pexpr)); }
#line 8381 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 441:
#line 1706 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8387 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 442:
#line 1708 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8393 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 443:
#line 1710 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8399 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 444:
#line 1712 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8405 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 445:
#line 1714 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8411 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 446:
#line 1716 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8417 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 447:
#line 1718 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8423 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 448:
#line 1720 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8429 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 449:
#line 1722 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8435 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 450:
#line 1724 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8441 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 451:
#line 1726 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8447 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 452:
#line 1728 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8453 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 453:
#line 1730 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8464 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 454:
#line 1737 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8474 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 455:
#line 1743 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8484 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 456:
#line 1749 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8494 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 457:
#line 1755 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 8504 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 458:
#line 1764 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8510 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 459:
#line 1773 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 8516 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 467:
#line 1789 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8522 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 468:
#line 1793 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = NULL; }
#line 8528 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 470:
#line 1798 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8534 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 471:
#line 1802 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8540 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 472:
#line 1803 "chapel.ypp" /* yacc.c:1666  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8546 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 473:
#line 1807 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8552 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 474:
#line 1811 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8558 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 475:
#line 1812 "chapel.ypp" /* yacc.c:1666  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8564 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 476:
#line 1817 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 8572 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 477:
#line 1821 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8580 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 478:
#line 1825 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8588 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 479:
#line 1831 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 8594 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 480:
#line 1832 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8600 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 481:
#line 1833 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8606 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 482:
#line 1834 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8612 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 483:
#line 1835 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8618 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 484:
#line 1836 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8624 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 486:
#line 1842 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8630 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 487:
#line 1847 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8636 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 488:
#line 1852 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8642 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 497:
#line 1868 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8648 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 498:
#line 1870 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8654 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 499:
#line 1872 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 8660 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 500:
#line 1874 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8666 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 501:
#line 1876 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8672 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 502:
#line 1878 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8678 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 503:
#line 1880 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8684 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 504:
#line 1882 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8690 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 505:
#line 1886 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8696 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 506:
#line 1887 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8702 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 507:
#line 1890 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8708 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 508:
#line 1891 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8714 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 509:
#line 1892 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8720 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 516:
#line 1914 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8726 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 517:
#line 1915 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8732 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 518:
#line 1916 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8738 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 519:
#line 1920 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8744 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 520:
#line 1921 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8750 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 521:
#line 1922 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8756 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 522:
#line 1923 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 8762 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 523:
#line 1931 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8768 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 524:
#line 1932 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8774 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 525:
#line 1933 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8780 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 526:
#line 1934 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8786 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 527:
#line 1938 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 8792 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 528:
#line 1939 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 8798 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 530:
#line 1944 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 8804 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 531:
#line 1945 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8810 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 532:
#line 1946 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8816 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 533:
#line 1947 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8822 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 534:
#line 1948 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8828 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 535:
#line 1949 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 8834 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 536:
#line 1950 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8840 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 537:
#line 1951 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8846 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 538:
#line 1952 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8852 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 539:
#line 1953 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8858 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 540:
#line 1955 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8866 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 541:
#line 1959 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8874 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 542:
#line 1966 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8880 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 543:
#line 1967 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8886 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 544:
#line 1971 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8892 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 545:
#line 1972 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8898 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 546:
#line 1973 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8904 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 547:
#line 1974 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8910 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 548:
#line 1975 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8916 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 549:
#line 1976 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8922 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 550:
#line 1977 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8928 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 551:
#line 1978 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8934 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 552:
#line 1979 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8940 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 553:
#line 1980 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8946 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 554:
#line 1981 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8952 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 555:
#line 1982 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8958 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 556:
#line 1983 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8964 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 557:
#line 1984 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8970 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 558:
#line 1985 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8976 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 559:
#line 1986 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8982 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 560:
#line 1987 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8988 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 561:
#line 1988 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8994 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 562:
#line 1989 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9000 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 563:
#line 1990 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9006 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 564:
#line 1991 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9012 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 565:
#line 1992 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9018 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 566:
#line 1993 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9024 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 567:
#line 1997 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 9030 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 568:
#line 1998 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 9036 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 569:
#line 1999 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 9042 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 570:
#line 2000 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 9048 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 571:
#line 2001 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 9054 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 572:
#line 2002 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9060 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 573:
#line 2003 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 9066 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 574:
#line 2007 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9072 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 575:
#line 2008 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9078 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 576:
#line 2009 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9084 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 577:
#line 2010 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9090 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 578:
#line 2014 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9096 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 579:
#line 2015 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9102 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 580:
#line 2016 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9108 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 581:
#line 2017 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9114 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 582:
#line 2022 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 9120 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 583:
#line 2023 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 9126 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 584:
#line 2024 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 9132 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 585:
#line 2025 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 9138 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 586:
#line 2026 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 9144 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 587:
#line 2027 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 9150 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 588:
#line 2028 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 9156 "bison-chapel.cpp" /* yacc.c:1666  */
    break;


#line 9160 "bison-chapel.cpp" /* yacc.c:1666  */
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
