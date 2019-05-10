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
    TBAND = 384,
    TBNOT = 385,
    TBOR = 386,
    TBXOR = 387,
    TCOLON = 388,
    TCOMMA = 389,
    TDIVIDE = 390,
    TDOT = 391,
    TDOTDOT = 392,
    TDOTDOTDOT = 393,
    TEQUAL = 394,
    TEXP = 395,
    TGREATER = 396,
    TGREATEREQUAL = 397,
    THASH = 398,
    TIO = 399,
    TLESS = 400,
    TLESSEQUAL = 401,
    TMINUS = 402,
    TMOD = 403,
    TNOT = 404,
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
#define YYLAST   16537

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  168
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  138
/* YYNRULES -- Number of rules.  */
#define YYNRULES  585
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1046

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
    1238,  1239,  1240,  1244,  1245,  1246,  1247,  1248,  1249,  1250,
    1251,  1252,  1253,  1254,  1255,  1259,  1260,  1264,  1268,  1269,
    1270,  1274,  1276,  1278,  1280,  1282,  1287,  1288,  1292,  1293,
    1294,  1295,  1296,  1297,  1298,  1299,  1300,  1304,  1305,  1306,
    1307,  1308,  1309,  1313,  1314,  1318,  1319,  1320,  1321,  1322,
    1323,  1327,  1328,  1331,  1332,  1336,  1337,  1341,  1343,  1348,
    1349,  1353,  1354,  1358,  1359,  1361,  1363,  1365,  1370,  1371,
    1374,  1375,  1376,  1377,  1378,  1379,  1380,  1383,  1384,  1387,
    1389,  1391,  1396,  1409,  1426,  1427,  1429,  1434,  1435,  1436,
    1437,  1438,  1442,  1448,  1454,  1462,  1463,  1471,  1473,  1478,
    1480,  1482,  1487,  1489,  1491,  1498,  1499,  1500,  1505,  1507,
    1509,  1513,  1517,  1519,  1523,  1531,  1532,  1533,  1534,  1535,
    1540,  1541,  1542,  1543,  1544,  1564,  1568,  1572,  1580,  1587,
    1588,  1589,  1593,  1595,  1601,  1603,  1605,  1610,  1611,  1612,
    1613,  1614,  1620,  1621,  1622,  1623,  1627,  1628,  1632,  1633,
    1634,  1638,  1639,  1643,  1644,  1648,  1649,  1653,  1654,  1655,
    1656,  1660,  1661,  1672,  1673,  1674,  1675,  1676,  1677,  1679,
    1681,  1683,  1685,  1687,  1689,  1691,  1693,  1695,  1697,  1702,
    1704,  1706,  1708,  1710,  1712,  1714,  1716,  1718,  1720,  1722,
    1724,  1726,  1733,  1739,  1745,  1751,  1760,  1770,  1778,  1779,
    1780,  1781,  1782,  1783,  1784,  1785,  1790,  1791,  1795,  1799,
    1800,  1804,  1808,  1809,  1813,  1817,  1821,  1828,  1829,  1830,
    1831,  1832,  1833,  1837,  1838,  1843,  1848,  1856,  1857,  1858,
    1859,  1860,  1861,  1862,  1863,  1864,  1866,  1868,  1870,  1872,
    1874,  1876,  1878,  1883,  1884,  1887,  1888,  1889,  1892,  1893,
    1894,  1895,  1906,  1907,  1911,  1912,  1913,  1917,  1918,  1919,
    1920,  1928,  1929,  1930,  1931,  1935,  1936,  1940,  1941,  1942,
    1943,  1944,  1945,  1946,  1947,  1948,  1949,  1950,  1951,  1955,
    1963,  1964,  1968,  1969,  1970,  1971,  1972,  1973,  1974,  1975,
    1976,  1977,  1978,  1979,  1980,  1981,  1982,  1983,  1984,  1985,
    1986,  1987,  1988,  1989,  1990,  1994,  1995,  1996,  1997,  1998,
    1999,  2003,  2004,  2005,  2006,  2010,  2011,  2012,  2013,  2018,
    2019,  2020,  2021,  2022,  2023,  2024
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
  "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL", "TASSIGNSR", "TBAND",
  "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TIO", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOT",
  "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT",
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
     415,   416,   417,   418,   419,   420,   421,   422
};
# endif

#define YYPACT_NINF -937

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-937)))

#define YYTABLE_NINF -537

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -937,    98,  2909,  -937,   -48,  -937,  -937,  -937,  -937,  -937,
    -937,  4361,     0,   113,  -937,  9799,  -937,     0,  9799,  -937,
     191,    86,   113,  4361,  9799,  4361,    49,  -937,  9799,  6901,
    -937,  8281,  8879,  6618,  9799,  -937,   115,  -937,  -937, 16432,
    -937,  1237,  9017,  -937,  9799,  9799,  -937,  -937,  -937,  9799,
    -937,  9799,  -937,  9799,   272,   143,  1164,  1164,  -937,  -937,
    -937,  9155,  7821,  9799,  9017,  9799,  9799,   214,  -937,   183,
    4361,  -937,  9799,  -937,  9937,  9937, 16432,  -937,  -937,  9799,
    9155,  -937,  -937,  9799,  9799,  -937,  -937,  9799,  -937,  -937,
   12007,  9799,  9799,  -937,  9799,  -937,  -937,  3395,  7039,  8419,
    -937,  4200,  -937,   190,  -937,   284,  -937,  -937,  -937,  -937,
    -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,
    -937,  -937,  -937,  -937, 16432,  -937, 16432,   334,   156,  -937,
    -937,  1237,  -937,   212,  -937,  -937,  -937,   221,   248,   268,
     283,   288, 16147,  2270,   -64,   290,   291,  -937,  -937,  -937,
    -937,  -937,  -937,  -937,   161,  -937,  -937, 16147,   303,  4361,
    -937,  -937,   298,  9799,  9799,  9799,  9799,  9799,  9155,  9155,
     175,  -937,  -937,  -937,  -937,   343,   196,  -937,  -937,   306,
   14322, 16432,  1237,  -937,   326,  -937,   147, 16147,   373,  7982,
   16147,  -937,   164,  -937, 16432,   187,   267, 16432,   321,    12,
   13820,    33, 13033,   267,  9799,  -937, 14270, 13124,  7982,  -937,
    -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,
    -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,
    -937,  4361,  -937,   322,  2130,    35,     7,  -937,  4361,  -937,
    -937, 13908,   521,   343, 13908,   343,   521,  -937,  7982,  9799,
    -937,  -937, 16432,  -937,   206, 16147,  9799,  9799,  -937, 16147,
     337, 14127,  -937, 13908,   343, 16147,   335,  7982,  -937, 16147,
   14501,  -937,  -937, 14644, 12275,  -937,  -937, 14700,   389,   350,
     343,   259, 14413, 13908, 14736,   353,  1750,   521,   353,   521,
     108,  -937,  -937,  3556,    84,  -937,  9799,  -937,   -44,    92,
    -937,   -16,    52, 14792,    41,   499,  -937,  3717,  -937,   451,
     375,   352,  -937,  -937,  -937,   266,  -937,  9799,  9799,  9799,
    9799,  8879,  8879,  9799,  9799,  9799,  9799, 10075,  9799,   204,
   12007,  9799,  9799,  9799,  9799,  9799,  9799,  9799,  9799,  9799,
    9799,  9799,  9799,  9799,  9799,  9799,  -937,  -937,  -937,  -937,
    -937,  -937,  8580,  8580,  -937,  -937,  -937,  -937,  8580,  -937,
    -937,  8580,  8580,  7982,  7982,  8879,  8879,  7522,  -937,  -937,
   14465, 14557, 14879,    83,  3878,  -937,  8879,    12,   355,   271,
    -937,  9799,  -937,  9799,   400,  -937,   358,   383,  -937,  -937,
     367, 16432,  1237,   481,  9155,  -937,  4522,  8879,  -937,   364,
    -937,    12,  4683,  8879,  -937,    12,   267, 10213,  9799,  -937,
    4361,   495,  9799,   374,  -937,   445,  -937,  -937,  2130,  -937,
     393,   385,  -937, 10351,   439,  9799,  1237,  -937,  -937,  -937,
     395,  -937,  9155,  -937, 16147, 16147,  -937,    27,  -937,  7982,
     398,  -937,   544,  -937,   544,  -937, 10489,   430,  -937,  -937,
   10213,  9799,  -937,  -937,  -937,  -937,  -937,  8718,  -937, 12945,
    7200,  -937,  7361,  -937,  4361,   401,  8879,  6132,  3234,   402,
    9799,  6456,  -937,  -937,   126,  -937,  4039, 16432,  9155,   408,
   16375,    24,  -937, 14935, 15735, 15735,   365,  -937,   365,  -937,
     365, 16177,   142,  1920,  1435, 12409, 12409,   343,   353,  -937,
    -937,  -937,  -937,  1750, 13954,   365,  1018,  1018, 15735,  1018,
    1018,   923,   353, 13954, 15886,   923,   521,   521,   353,   414,
     416,   418,   420,   424,   419,   423,  -937,   365,  -937,   365,
      25,  -937,  -937,  -937,    93,  -937,  1563, 16218,   351, 10627,
    8879, 10765,  8879,  9799,  8879, 12665,     0, 15022,  -937,  -937,
    -937, 16147, 15078,  7982,  -937,  7982,  -937,   375,   273,  9799,
      99,  9799, 16147,    45, 14051,  7522,  -937,  9799, 16147,    36,
   13872,  -937,   428,   455,   431, 15165,   455,   443,   567, 15221,
    4361, 14087,  -937,    29,  -937,  -937,  -937,  -937,  -937,  -937,
     319,   101,  -937,  2463,  -937,   269,   440,  2130,    35,  9799,
      49,   115,  9799,  9799,  9799,  9799,  9799,  5971,  -937,  -937,
     470,  8143,  -937, 16147,  -937,  -937,  -937, 16147,   441,   -17,
     446,  -937,  1714,  -937,  -937,   304, 16432,   457,   458,  -937,
    -937,  -937,  -937,  -937,  -937,  -937,  4361,    57,  2681,  -937,
    -937, 16147,  4361, 16147,  -937, 15124,  -937,  -937,  -937,   471,
    1378,   449,   501,   140,  -937,   537,  -937,  -937,  -937,  -937,
    6742,  -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,
    7522,  -937,    26,  8879,  8879,  9799,   588, 15308,  9799,   592,
   15364,   461,  2726,    12,    12,  -937,  -937,  -937,  -937,   463,
    -937, 13908,  -937, 13195,  4844,  -937,  5005,  -937,   211,  -937,
   13275,  5166,  -937,    12,  5327,  -937,    12,  9799,  -937,  9799,
    -937,  4361,  9799,  -937,  4361,   599,  -937,  -937, 16432,  1025,
    -937,  2130,   500,   556,  -937,  -937,  -937,   133,  -937,  -937,
     439,   474,    85,  -937,  -937,  -937,  5488,  9155,  -937,  -937,
    -937, 16432,  -937,   507,   306,  -937,  -937,  -937,  -937,  5649,
     478,  5810,   479,  -937,  9799,   484,  2484,  -937,  9799, 16432,
    -937,  -937,   269,   485,   513,  -937,   517,  -937,   519,  -937,
     522,   524,  -937,   530,   532,   534,   535,   540,  -937,   541,
     543,  -937, 16432,  -937,  -937,  -937,  -937,  -937,  -937,  -937,
    -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,  -937,
    -937,  -937,  -937,  9799,  -937,  -937,   545,   485,   485,  -937,
    -937,   439,   228,   261, 15400, 10903, 11041, 15543, 11179, 11317,
   11455, 11593,  -937,  -937,  1912,  -937,  4361,  9799, 16147,  9799,
   16147,  7522,  -937,  4361,  9799, 16147,  -937,  9799, 16147,  -937,
   15456, 16147,  -937, 16147,   617,  4361,   500,  -937,   511,  9316,
     -19,  -937,   146,  -937,  -937,  8879, 12537,  4361,  -937,     3,
     518,  9799,  -937,  9799,  -937, 16147,  4361,  9799,  -937, 16147,
    4361, 16147,  -937,  7683,   968,   968,  -937,  1476,  -937, 16147,
    -937,    69,   445,  -937,  -937, 12981, 12746,  -937,  -937,  -937,
    9799,  9799,  9799,  9799,  9799,  9799,  9799,  9799,   514,  6011,
   15221, 13346, 13426,  -937, 15221, 13497, 13577,  9799,  4361,  -937,
     439,   500,  6294,  -937,  -937,  -937,   292,  9155,  -937,  -937,
     144,  9799,    50, 12801,  -937,   538,   373,  -937,   306, 16147,
   13648,  -937, 13728,  -937,  -937,  -937, 11731,   590,   270,  -937,
    -937,  -937,   485,   485, 15614, 15694, 15765, 15845, 15916, 15996,
    -937,  -937,  4361,  4361,  4361,  4361, 16147,  -937,  -937,   -19,
    9477,    88,  -937,  -937, 16147,  -937,    72,  -937,   135,  -937,
     329, 16067,  -937,  -937,  -937, 11593,  -937,  4361,  4361,  3070,
    -937,  -937,   464,  -937,   146,  -937,  -937,  -937,  9799,  9799,
    9799,  9799,  9799,  9799, 15221, 15221, 15221, 15221,  -937,  -937,
    -937,  -937,  -937,   421,  8879, 12145,  -937,  9799,   144,    72,
      72,    72,    72,    72,    72,   144,   699, 15221, 15221,   523,
   11869,    96,    54, 12889,  -937,  -937, 16147,  -937,  -937,  -937,
    -937,  -937,  -937,  -937,   548,  -937,  -937,   705, 12281,  -937,
    -937,  -937,  9638,  -937,   717,  -937
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   528,   529,   530,   531,
     532,     0,   466,    70,   102,     0,   202,   466,     0,   107,
       0,   348,    70,     0,     0,     0,     0,   212,   503,   503,
     525,     0,     0,     0,     0,   106,     0,   222,   103,     0,
     218,     0,     0,   109,     0,     0,   457,   533,   110,     0,
     225,     0,   347,     0,     0,     0,    45,    44,   105,   203,
     349,     0,     0,     0,     0,     0,     0,     0,   108,     0,
       0,    83,     0,   526,     0,     0,     0,   104,   204,     0,
       0,   351,   111,     0,     0,   581,   583,     0,   584,   585,
     502,     0,     0,   582,   579,   126,   580,     0,     0,     0,
       4,     0,     5,     0,     9,    46,    10,    11,    12,    14,
     421,   422,    22,    13,   127,   133,    15,    17,    16,    19,
      20,    21,    18,   132,     0,   130,     0,   492,     0,   134,
     131,     0,   135,   508,   488,   489,   425,   423,     0,     0,
     493,   494,     0,   424,     0,   509,   510,   511,   527,   487,
     427,   426,   490,   491,     0,    38,    24,   433,     0,     0,
     467,    71,     0,     0,     0,     0,     0,     0,     0,     0,
     492,   508,   423,   493,   494,   435,   424,   509,   510,     0,
     466,     0,     0,   350,     0,   176,     0,   406,     0,   413,
     504,   223,   531,   140,     0,     0,   224,     0,     0,     0,
       0,     0,     0,     0,   503,   139,     0,     0,   413,    77,
      84,    95,    89,    97,    79,    88,    98,    85,    99,   100,
      93,    87,    94,    92,    90,    91,    78,    80,    86,    96,
     101,     0,    81,     0,     0,     0,     0,   355,     0,   123,
      32,     0,   567,   485,     0,   436,   568,     7,   413,   503,
     129,   128,   318,   403,     0,   402,     0,     0,   124,   507,
       0,     0,    35,     0,   438,   428,     0,   413,    36,   434,
       0,   183,   179,     0,   424,   183,   180,     0,   344,     0,
     437,     0,   402,     0,     0,   570,   501,   566,   569,   565,
       0,    48,    51,     0,     0,   408,     0,   410,     0,     0,
     409,     0,     0,   402,     0,     0,     6,     0,    47,     0,
     205,     0,   304,   303,   226,     0,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     500,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   263,   270,   271,   272,
     267,   269,     0,     0,   265,   268,   266,   264,     0,   274,
     273,     0,     0,   413,   413,     0,     0,     0,    25,    26,
       0,     0,     0,     0,     0,    27,     0,     0,     0,     0,
      28,     0,    29,     0,   421,   419,     0,   414,   415,   420,
       0,     0,     0,     0,     0,   149,     0,     0,   148,     0,
     157,     0,     0,     0,   155,     0,     0,     0,    57,   136,
       0,   169,     0,     0,    30,   286,   219,   359,     0,   360,
     362,     0,   384,     0,   365,     0,     0,   122,    31,    33,
       0,   317,     0,    64,   505,   506,   125,     0,    34,   413,
       0,   190,   181,   177,   182,   178,     0,   342,   339,    61,
       0,    57,   142,    37,    50,    49,    52,     0,   534,     0,
       0,   521,     0,   523,     0,     0,     0,     0,     0,     0,
       0,     0,   538,     8,     0,    40,     0,     0,     0,     0,
       0,   297,   354,   465,   562,   561,   564,   572,   571,   576,
     575,   558,   555,   556,   557,   497,   498,   496,   545,   519,
     520,   518,   517,   499,   549,   560,   554,   552,   563,   553,
     551,   543,   548,   550,   559,   542,   546,   547,   544,     0,
       0,     0,     0,     0,     0,     0,   574,   573,   578,   577,
     477,   478,   480,   482,     0,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   536,   466,   466,   145,   340,
     352,   407,     0,     0,   430,     0,   341,   205,     0,     0,
       0,     0,   441,     0,     0,     0,   158,     0,   447,     0,
       0,   156,   580,    60,     0,    53,    58,     0,   168,     0,
       0,     0,   429,   291,   288,   289,   290,   294,   295,   296,
     286,     0,   279,     0,   287,   305,     0,   363,     0,   116,
     118,   119,   114,   115,   113,   112,   117,     0,   383,   382,
     488,     0,   357,   486,   356,   516,   405,   404,     0,     0,
       0,   431,     0,   184,   346,   488,     0,     0,     0,   535,
     495,   522,   411,   524,   412,   165,     0,     0,     0,   537,
     163,   451,     0,   540,   539,     0,    42,    41,    39,   206,
       0,     0,   216,     0,   213,   301,   298,   299,   302,   227,
       0,    68,    69,    67,    66,    65,   514,   515,   479,   481,
       0,   468,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   141,   417,   418,   416,     0,
     353,     0,   167,     0,     0,   147,     0,   146,     0,   472,
       0,     0,   153,     0,     0,   151,     0,     0,   137,     0,
     138,     0,     0,   171,     0,   173,   292,   293,     0,   286,
     277,     0,   397,   306,   309,   308,   310,     0,   361,   364,
     365,     0,     0,   366,   367,   192,     0,     0,   191,   194,
     432,     0,   185,   188,     0,   343,    62,    63,   166,     0,
       0,     0,     0,   164,     0,     0,   221,   211,     0,   214,
     210,   300,   305,   275,    72,   259,    84,   257,    89,    74,
      88,    85,   262,    99,   100,    87,    90,    73,    75,    86,
     101,   238,   241,   239,   240,   251,   242,   255,   247,   245,
     258,   261,   246,   244,   249,   254,   256,   243,   248,   252,
     253,   250,   260,     0,   236,    76,     0,   275,   275,   234,
     470,   365,   508,   508,     0,     0,     0,     0,     0,     0,
       0,     0,   144,   143,     0,   150,     0,     0,   440,     0,
     439,     0,   471,     0,     0,   446,   154,     0,   445,   152,
      55,    54,   170,   456,   172,     0,   397,   280,     0,     0,
     365,   307,   323,   358,   388,     0,   536,     0,   196,     0,
       0,     0,   186,     0,   161,   453,     0,     0,   159,   452,
       0,   541,   200,   503,   221,   221,   198,   221,   208,   217,
     215,     0,   286,   233,   237,     0,     0,   229,   230,   474,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   221,
     444,     0,     0,   473,   450,     0,     0,     0,     0,   175,
     365,   397,     0,   400,   399,   401,   488,   319,   283,   281,
       0,     0,     0,     0,   386,   488,   197,   195,     0,   189,
       0,   162,     0,   160,   209,   379,     0,   311,     0,   235,
      72,    73,   275,   275,     0,     0,     0,     0,     0,     0,
     201,   199,     0,     0,     0,     0,    56,   174,   282,   365,
     389,     0,   320,   322,   321,   337,     0,   338,   325,   328,
       0,   324,   315,   316,   220,     0,   187,     0,     0,     0,
     378,   377,   488,   312,   323,   276,   231,   232,     0,     0,
       0,     0,     0,     0,   443,   442,   449,   448,   285,   284,
     391,   392,   394,   488,     0,   536,   336,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   488,   455,   454,     0,
     369,     0,     0,     0,   393,   395,   327,   329,   330,   333,
     334,   335,   331,   332,   326,   374,   372,   488,   536,   313,
     228,   314,   389,   373,   488,   396
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -937,  -937,  -937,     1,  -380,  2034,  -937,  -937,  -937,  -937,
     209,   379,  -284,   237,   239,  -937,  -937,   157,   669,  -937,
    1096,  -152,  -636,  -937,  -810,  -937,   512,  -876,  -726,   -47,
    -937,  -937,  -937,    97,  -937,  -937,  -937,   427,  -937,   -42,
    -937,  -937,  -937,  -937,  -937,   509,   148,  -118,  -937,  -937,
    -937,   -52,   914,  -937,  -937,  -937,  -937,  -937,  -937,  -937,
    -937,  -178,  -142,  -763,  -937,  -166,     5,   141,  -937,  -937,
    -937,   -37,  -937,  -937,  -292,   174,  -937,  -226,  -250,  -280,
    -272,  -291,  -937,  -165,  -937,   -12,   704,  -125,   313,  -937,
    -403,  -711,  -936,  -937,  -555,  -432,  -898,  -900,  -788,    21,
    -937,   -26,  -937,  -182,  -937,   193,   550,  -310,  -937,  -937,
     963,  -937,    -6,  -937,  -937,  -190,  -937,  -560,  -937,  -937,
    1329,  1618,   -11,    -3,    16,   692,  -937,  1741,  1941,  -937,
    -937,  -937,  -937,  -937,  -937,  -937,  -937,  -365
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   292,   101,   578,   103,   104,   105,   309,
     106,   293,   573,   577,   574,   107,   108,   109,   162,   804,
     235,   110,   232,   111,   608,   195,   240,   112,   113,   114,
     115,   116,   117,   681,   118,   119,   120,   442,   623,   744,
     121,   122,   619,   739,   123,   124,   479,   756,   125,   126,
     653,   654,   170,   233,   595,   128,   129,   481,   762,   659,
     806,   807,   412,   883,   416,   591,   592,   593,   594,   660,
     314,   727,   984,  1040,   974,   253,   963,   918,   922,   968,
     969,   970,   130,   279,   447,   131,   132,   236,   237,   420,
     421,   612,   981,   937,   424,   609,  1001,   915,   850,   294,
     186,   298,   299,   386,   387,   388,   171,   134,   135,   136,
     172,   138,   159,   160,   534,   401,   698,   535,   536,   139,
     173,   174,   142,   196,   389,   176,   144,   177,   178,   147,
     148,   149,   304,   150,   151,   152,   153,   154
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     157,   362,   538,   100,   175,   699,   315,   180,   182,   250,
     251,   179,   405,   187,   624,   596,   378,   190,   190,   853,
     200,   202,   206,   207,   805,   191,   413,   422,   618,   390,
     878,   241,   238,   242,   243,   590,   422,   384,   244,   913,
     245,   238,   246,   730,   887,   888,   973,   736,   655,   425,
     255,   259,   261,   263,   264,   265,   384,   379,   910,   269,
    1002,   270,   238,   273,   277,   701,   430,   668,   280,   282,
     935,   716,   283,   284,   694,   965,   285,  -380,   260,   286,
     287,   288,   254,   289,  1036,   440,   255,   259,   303,   737,
     460,  -193,   656,   399,   466,   611,   384,   363,     3,   364,
     889,   281,  1043,   657,   669,  1025,   155,  1024,   717,   158,
     810,   465,   469,   610,   300,   384,   161,   811,   461,   917,
     302,  -375,   658,   959,   576,   544,   980,   855,   199,   201,
    1004,    62,   362,  -193,   422,    62,   625,   432,   544,   919,
    -380,   426,   399,   738,  1045,   399,   973,   965,   464,  -380,
    -375,   934,   157,   370,   371,   372,   269,   255,   303,   423,
    -380,   399,   374,  -375,   967,   183,   399,   576,   423,  -380,
     320,   374,   852,   878,   377,   471,  -375,   502,   259,   986,
     987,   524,   525,  -121,   392,  -380,   467,  -193,  -380,  -380,
     373,   182,   374,   190,   729,   374,   301,   259,   920,   958,
     538,   406,   936,   312,   374,   472,    16,     5,  1039,   374,
     189,   384,   384,   374,  -380,    21,   468,   467,   457,   467,
     321,   749,   467,  -375,   322,   966,   462,   670,  -375,   313,
     467,   272,   276,   432,   499,   719,   967,   259,   190,   365,
    -380,  -121,  1007,   366,   458,   434,   435,   545,   999,   856,
     805,   239,  1005,   921,   463,   671,   259,   620,   500,    52,
    1038,   692,   155,   720,    59,  -121,   423,   558,   454,  1008,
      60,   903,   297,   239,   759,   327,   208,   328,   329,   247,
     155,   381,   332,   271,   275,   459,   646,   384,    78,   181,
     339,    21,  -380,   723,   456,    81,    71,   343,   344,   345,
     760,   382,   501,   266,   248,   538,   483,   484,   485,   486,
     488,   490,   491,   492,   493,   494,   497,   498,   848,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   517,   518,    52,  -513,   724,  -513,   590,
     432,   259,   259,   583,   267,   831,    60,   259,   725,   307,
     259,   259,   259,   259,   527,   529,   537,  -512,   308,  -512,
     433,   584,  -476,   385,   585,   547,  -459,   726,   519,   520,
     551,    81,   552,   832,   521,  -458,   877,   522,   523,   703,
     706,   320,   385,   255,   586,   562,   564,   587,   375,   305,
    -476,   568,   570,   432,   392,  -475,   575,   575,   588,   579,
     426,   581,   316,   384,   719,   426,  -398,   426,   239,   239,
     239,   239,   317,   449,   613,   560,   411,   589,   487,   489,
     482,   617,   385,  -475,   924,   550,  -398,   690,   259,   674,
    -398,   321,   985,   366,   632,   322,   634,  -463,  -345,   575,
     575,   385,  -464,  1009,  -461,  -460,   617,   750,   752,   259,
     239,   259,   369,   239,  -398,   638,   617,   641,  -345,   643,
     645,   745,   526,   528,   367,   374,   538,   255,  1010,   583,
    1011,  1012,   239,   546,  1013,  1014,   300,   456,   300,   329,
     380,   383,   394,   415,   175,   280,   327,   584,  -462,   329,
     585,   436,   239,   332,   563,  -513,   439,  -513,   327,   649,
     569,   329,   590,   446,   448,   332,   473,   477,   478,   549,
     586,   480,  -381,   587,   553,    54,  -376,   555,   808,   877,
     554,   556,  -381,   559,   588,   565,   580,   597,   562,   677,
     568,   680,   579,   682,   641,  -390,   582,   385,   385,   916,
     683,   684,   259,   589,   259,  -376,   925,   598,   691,   320,
     693,  -381,   133,   611,   537,  -390,   700,   615,  -376,  -390,
     621,   133,   622,   637,   626,   636,   642,   650,   661,   687,
     662,  -376,   663,   133,   664,   133,   262,  -381,   665,  -278,
    -385,   666,   -59,  -390,  -381,   708,   239,   667,   175,   707,
    -385,   245,   264,   265,   269,   280,   303,   710,   711,   321,
     259,   735,   728,   322,  -381,   432,   616,  -278,   740,   757,
     239,   746,   747,   385,   239,   758,   761,   815,  -376,  -385,
     133,   818,   824,  -376,  -381,   820,   982,   734,   732,  -381,
     845,   616,  -381,   849,   297,   851,   297,   676,   854,   679,
     861,   616,   866,   870,   872,  -385,   882,   133,   908,   -82,
    1003,   133,  -385,  -102,   327,  -107,   328,   329,  -106,   537,
    -103,   332,   488,   527,   814,  1016,  -109,   817,  -110,   339,
    -105,  -108,  -385,   911,   950,  1006,   -83,  -104,   345,  -111,
     928,   886,  1008,   828,   983,   830,   476,  1035,   628,   627,
     835,   184,  -385,   838,   143,  1003,   840,  -385,   841,   860,
    -385,   843,   444,   143,   391,   689,   899,   880,   942,   133,
    1037,   395,   398,   400,   404,   143,   938,   143,  1028,  1029,
    1030,  1031,  1032,  1033,   847,   881,   255,   686,  1044,   385,
    1041,   718,  1003,   998,  1022,  1034,  1027,   205,   865,   614,
     869,  -387,     0,   871,   943,     0,     0,   879,   688,     0,
       0,  -387,     0,   428,     0,     0,   429,  -368,   859,     0,
       0,     0,   143,     0,     0,     0,   274,   274,     0,  -370,
     487,   526,   239,   239,     0,   438,     0,     0,   239,   239,
    -387,   133,     0,     0,     0,     0,  -368,     0,   133,   143,
     715,     0,   885,   143,     0,   452,     0,     0,  -370,  -368,
       0,     0,     0,     0,   828,   830,  -387,   835,   838,   865,
     869,  -370,  -368,  -387,     0,   900,   901,     0,   902,     0,
     537,     0,   904,   905,  -370,     0,   906,   250,   251,     0,
       0,   742,     0,  -387,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   133,   923,   641,     0,     0,     0,     0,
     929,   143,   930,  -387,     0,     0,   932,   133,  -387,  -368,
       0,  -387,   190,     0,  -368,     0,     0,     0,     0,     0,
       0,  -370,     0,     0,     0,     0,  -370,     0,     0,   900,
     944,   945,   904,   946,   947,   948,   949,     0,     0,   548,
       0,     0,   239,   239,     0,     0,   956,     0,     0,     0,
     239,   303,   411,     0,     0,     0,   964,     0,     0,   411,
     971,     0,   239,   566,     0,   239,   127,   571,     0,     0,
       0,     0,     0,   143,   133,   127,     0,     0,     0,     0,
     143,     0,     0,   961,     0,     0,     0,   127,     0,   127,
       0,   994,   995,   996,   997,     0,   133,     0,     0,     0,
       0,   320,   133,   862,     0,     0,     0,     0,     0,     0,
     133,     0,     0,     0,   869,   137,  1017,  1018,   303,     0,
       0,     0,     0,     0,   137,     0,     0,   994,   995,   996,
     997,  1017,  1018,     0,   127,   143,   137,     0,   137,     0,
       0,   203,    21,  1023,   641,     0,  1026,     0,     0,   143,
    1021,   321,    28,   249,     0,   322,     0,     0,     0,     0,
       0,   127,    37,     0,   133,   127,     0,     0,   133,     0,
       0,     0,     0,   914,     0,     0,   133,   641,     0,     0,
       0,   869,     0,   137,    50,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,    60,     0,   583,
       0,     0,   324,     0,   325,   326,   327,     0,   328,   329,
     137,     0,     0,   332,   137,     0,   143,   584,   239,     0,
     585,   339,    81,   127,     0,   695,   697,     0,   343,   344,
     345,   702,   705,     0,     0,     0,   672,     0,   143,     0,
     586,   962,     0,   587,   143,    54,   321,     0,     0,     0,
     322,     0,   143,     0,   588,     0,     0,     0,     0,     0,
     411,   411,     0,     0,   411,   411,     0,     0,     0,     0,
       0,     0,   137,   589,     0,     0,     0,     0,     0,     0,
     133,   972,     0,     0,  1000,   231,     0,   976,     0,   411,
       0,   411,     0,     0,     0,   127,     0,   324,     0,   325,
     326,   327,   127,   328,   329,   330,   143,     0,   332,     0,
     143,     0,     0,     0,     0,   338,   339,     0,   143,     0,
     342,     0,   278,   343,   344,   345,     0,     0,     0,  1000,
       0,     0,     0,     0,     0,     0,   133,   203,    21,     0,
       0,     0,   133,     0,   137,   822,   823,     0,    28,   249,
       0,   137,     0,   825,     0,     0,     0,   127,    37,     0,
     809,  -221,     0,     0,     0,   836,  1000,     0,   839,     0,
     310,   127,   311,   812,   813,     0,     0,     0,     0,     0,
      50,   972,    52,     0,     0,     0,     0,  -221,     0,     0,
     209,     0,     0,    60,   133,     0,   133,     0,     0,     0,
       0,   133,   210,   211,   133,     0,   137,     0,     0,   212,
       0,   133,     0,     0,   133,     0,     0,   213,    81,     0,
     137,     0,   143,   214,     0,     0,     0,   278,   215,     0,
     216,     0,     0,   217,     0,     0,   133,     0,   127,     0,
     278,   218,     0,   393,     0,     0,     0,     0,   219,   133,
       0,   133,     0,     0,   220,     0,     0,     0,     0,     0,
     127,     0,     0,   221,     0,     0,   127,     0,     0,     0,
       0,     0,   222,   223,   127,   224,     0,   225,   143,   226,
     419,   140,   227,     0,   143,     0,   228,   137,     0,   229,
     140,     0,   230,     0,     0,     0,     0,     0,   431,     0,
       0,     0,   140,     0,   140,     0,     0,     0,     0,   137,
       0,     0,     0,     0,     0,   137,     0,     0,     0,     0,
       0,   927,     0,   137,     0,     0,   133,     0,   127,   755,
       0,     0,   127,   133,     0,     0,   143,     0,   143,     0,
     127,     0,     0,   143,     0,   133,   143,  -207,   234,   140,
       0,  -207,  -207,   143,     0,     0,   143,   133,     0,     0,
    -207,     0,  -207,  -207,     0,     0,   133,  -207,     0,     0,
     133,     0,  -207,     0,     0,  -207,   140,   137,   143,     0,
     140,   137,     0,     0,     0,     0,     0,     0,     0,   137,
       0,   143,     0,   143,  -207,     0,  -207,     0,  -207,     0,
    -207,  -207,     0,  -207,     0,  -207,     0,  -207,   133,     0,
       0,     0,     0,   320,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -207,     0,     0,  -207,
       0,     0,  -207,     0,     0,     0,     0,   557,   140,     0,
       0,     0,     0,     0,   127,    16,     0,     0,     0,    20,
      21,     0,   133,   133,   133,   133,     0,     0,    27,     0,
      28,   873,     0,   321,   419,    33,     0,   322,   143,     0,
      37,     0,     0,     0,     0,   143,     0,   133,   133,     0,
       0,     0,  -207,     0,     0,     0,     0,   143,  -207,     0,
       0,     0,    50,   137,    52,     0,   305,     0,   874,   143,
     127,   875,     0,    59,     0,    60,   127,     0,   143,     0,
     140,     0,   143,     0,   324,     0,     5,   140,   327,     0,
     328,   329,     0,   648,    76,   332,   652,    78,    14,     0,
      81,     0,     0,   339,     0,    19,     0,     0,     0,     0,
     343,   344,   345,     0,     0,     0,     0,     0,     0,   137,
     143,     0,     0,     0,    35,   137,     0,     0,   127,    38,
     127,     0,     0,     0,     0,   127,     0,    43,   127,     0,
     141,     0,   140,     0,    48,   127,     0,     0,   127,   141,
      95,     0,     0,     0,     0,     0,   140,     0,     0,    58,
       0,   141,     0,   141,   143,   143,   143,   143,     0,     0,
     127,    68,     0,     0,     0,    71,     0,   137,     0,   137,
       0,     0,    77,   127,   137,   127,     0,   137,    82,   143,
     143,     0,     0,     0,   137,     0,     0,   137,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   141,   722,
       0,     0,     0,   419,     0,     0,     0,     0,     0,   137,
       0,     0,     0,   140,     0,     0,     0,     0,     0,     0,
       0,     0,   137,     0,   137,   141,     0,   209,   743,   141,
       0,     0,   278,     0,     0,   140,     0,     0,     0,   210,
     211,   140,     0,     0,     0,     0,   212,     0,     0,   140,
     127,     0,     0,   145,   213,     0,     0,   127,     0,     0,
     214,     0,   145,     0,     0,   215,     0,   216,     0,   127,
     217,     0,     0,     0,   145,     0,   145,     0,   218,     0,
       0,   127,     0,     0,     0,   219,     0,   141,   320,     0,
     127,   220,     0,     0,   127,     0,     0,     0,     0,   137,
     221,     0,     0,   140,     0,     0,   137,   140,     0,   222,
     223,     0,   224,     0,   225,   140,   226,     0,   137,   227,
       0,   145,     0,   228,   846,     0,   229,   419,     0,   230,
     137,     0,   127,     0,     0,     0,     0,     0,   321,   137,
       0,     0,   322,   137,     0,     0,     0,   743,   145,     0,
       0,     0,   145,     0,     0,     0,     0,     0,     0,   141,
       0,     0,     0,     0,     0,   652,   141,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   127,   127,   127,   127,
       0,   137,     0,   374,     0,   741,     0,     0,   884,   324,
       0,   325,   326,   327,     0,   328,   329,     0,     0,     0,
     332,   127,   127,     0,     0,     0,     0,   338,   339,     0,
     145,     0,   342,     0,     0,   343,   344,   345,     0,   140,
       0,   141,     0,   898,     0,   137,   137,   137,   137,     0,
       0,     0,     0,     0,     0,   141,     0,     0,     0,     0,
       0,  -207,     0,     0,     0,  -207,  -207,     0,     0,     0,
     137,   137,     0,   146,  -207,     0,  -207,  -207,   320,     0,
       0,  -207,   146,     0,     0,     0,  -207,     0,     0,  -207,
       0,     0,     0,     0,   146,   140,   146,     0,     0,     0,
       0,   140,   145,     0,     0,     0,     0,     0,  -207,   145,
    -207,     0,  -207,     0,  -207,  -207,     0,  -207,     0,  -207,
       0,  -207,   141,     0,     0,     0,     0,     0,   321,     0,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
    -207,   146,     0,  -207,   141,     0,  -207,     0,     0,     0,
     141,     0,     0,   140,     0,   140,     0,     0,   141,     0,
     140,     0,     0,   140,   145,     0,   102,     0,   146,     0,
     140,     0,   146,   140,     0,   156,     0,     0,   145,   324,
       0,     0,   326,   327,     0,   328,   329,   185,     0,   188,
     332,     0,     0,     0,     0,   140,  -207,     0,   339,     0,
       0,     0,  -207,     0,     0,   343,   344,   345,   140,     0,
     140,     0,   141,     0,     0,     0,   141,     0,     0,     0,
       0,     0,     0,     0,   141,     0,     0,     0,     0,     0,
     146,     0,     0,     0,   268,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,   209,     0,   306,     0,   145,     0,     0,
       0,     0,     0,   145,     0,   210,   211,     0,     0,     0,
       0,   145,   212,     0,     0,   140,     0,     0,     0,     0,
     213,     0,   140,     0,     0,     0,   214,     0,     0,     0,
       0,   215,   146,   216,   140,     0,   217,     0,     0,   146,
       0,     0,     0,     0,   218,     0,   140,     0,     0,     0,
       0,   219,     0,   368,     0,   140,     0,   220,   141,   140,
       0,     0,     0,     0,     0,   145,   221,     0,     0,   145,
       0,     0,     0,     0,     0,   222,   223,   145,   224,     0,
     225,     0,   226,     0,     0,   227,     0,     0,     0,   228,
     417,     0,   229,     0,   146,   230,     0,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   146,     0,
       0,     0,     0,     0,   141,     0,     0,     0,     0,     0,
     141,     0,     0,     0,     0,   414,     0,     0,     0,     0,
       0,     0,   427,     0,     0,     0,     0,     0,     0,     0,
       0,   140,   140,   140,   140,     0,     0,     0,     0,     0,
       0,   418,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   140,   140,     0,     0,
       0,     0,   141,     0,   141,   146,     0,     0,     0,   141,
       0,   145,   141,     0,     0,     0,     0,   102,     0,   141,
       0,     0,   141,     0,     0,     0,     0,   146,     0,     0,
       0,   102,     0,   146,     0,     0,     0,     0,     0,     0,
       0,   146,     0,     0,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   141,     0,   141,
       0,     0,     0,     0,     0,     0,     0,   145,     0,     0,
       0,     0,     0,   145,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,     0,
       0,     0,     0,     0,     0,   146,     0,     0,   102,   146,
       0,     0,     0,     0,  -483,     0,     0,   146,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   361,     0,
     427,  -512,     0,  -512,     0,   145,   427,   145,     0,     0,
       0,     0,   145,     0,   141,   145,     0,     0,     0,     0,
       0,   141,   145,     0,     0,   145,     0,     0,     0,     0,
       0,     0,     0,   141,     0,     0,   209,     0,     0,     0,
       0,     0,     0,     0,     0,   141,     0,   145,   210,   211,
       0,     0,     0,     0,   141,   212,     0,     0,   141,     0,
     145,     0,   145,   213,     0,     0,     0,     0,   635,   214,
       0,     0,   640,    16,   215,     0,   216,    20,    21,   217,
     102,     0,     0,     0,     0,     0,    27,   218,    28,   873,
       0,   146,     0,    33,   219,     0,   141,     0,    37,     0,
     220,     0,     0,     0,     0,     0,     0,     0,     0,   221,
       0,     0,     0,     0,     0,     0,     0,     0,   222,   223,
      50,   224,    52,   225,    54,   226,   874,     0,   227,   875,
       0,    59,   228,    60,     0,   229,     0,   145,   230,     0,
     141,   141,   141,   141,   145,     0,     0,   146,     0,     0,
       0,     0,    76,   146,     0,    78,   145,     0,    81,     0,
       0,     0,     0,     0,     0,   141,   141,     0,   145,     0,
       0,     0,     0,     0,     0,     0,     0,   145,     0,     0,
       0,   145,     0,     0,   713,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   721,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   146,     0,   146,    95,     0,
       0,     0,   146,     0,   876,   146,     0,     0,     0,   145,
       0,     0,   146,     0,     0,   146,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     748,     0,     0,     0,     0,     0,   753,   146,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     146,   318,   146,   145,   145,   145,   145,     0,   319,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   320,
       0,     0,     0,     0,     0,     0,     0,     0,   145,   145,
       0,     0,     0,     0,     0,     0,     0,     0,   427,     0,
     427,     0,     0,     0,     0,   427,   318,     0,   427,     0,
       0,     0,     0,   319,     0,   842,     0,     0,   844,     0,
       0,     0,     0,     0,   320,     0,     0,     0,     0,   321,
       0,     0,     0,   322,     0,     0,     0,   146,     0,     0,
     858,     0,     0,     0,   146,     0,     0,     0,     0,     0,
       0,     0,     0,   864,     0,   868,   146,     0,     0,     0,
     399,     0,     0,     0,   323,     0,     0,     0,   146,     0,
       0,     0,     0,     0,   321,     0,     0,   146,   322,     0,
     324,   146,   325,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,   334,   335,     0,   336,   337,   338,   339,
       0,   340,   341,   342,     0,     0,   343,   344,   345,   323,
       0,     0,     0,     0,     0,   751,     0,     0,     0,   146,
       0,     0,     0,     0,     0,   324,     0,   325,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,   334,   335,
       0,   336,   337,   338,   339,     0,   340,   341,   342,   909,
       0,   343,   344,   345,     0,     0,     0,     0,     0,     0,
     821,   926,     0,   146,   146,   146,   146,     0,     0,     0,
     931,     0,     0,     0,   933,     0,     0,     0,     0,    -2,
       4,     0,     5,     6,     7,     8,     9,    10,   146,   146,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,    26,
       0,    27,   957,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -221,     0,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
      48,    49,     0,     0,     0,    50,    51,    52,    53,    54,
      55,    56,  -221,   -43,    57,    58,    59,     0,    60,    61,
      62,     0,    63,    64,    65,    66,    67,    68,    69,    70,
       0,    71,    72,     0,    73,    74,    75,    76,    77,     0,
      78,    79,    80,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,     0,    92,     0,
      93,    94,     0,    95,     0,     0,    96,     0,    97,     0,
      98,  1019,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   163,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
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
      87,    88,    89,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,     0,    92,
       0,    93,    94,   252,     0,     0,     0,    96,     0,   168,
       0,    98,     0,   169,  1020,     4,     0,     5,     6,     7,
       8,     9,    10,     0,  -536,     0,    11,    12,    13,    14,
      15,  -536,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,  -536,    25,    26,  -536,    27,     0,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -221,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,    48,    49,     0,     0,     0,
      50,    51,    52,    53,     0,    55,    56,  -221,   -43,    57,
      58,    59,  -536,    60,    61,    62,  -536,    63,    64,    65,
      66,    67,    68,    69,    70,     0,    71,    72,     0,    73,
      74,    75,    76,    77,     0,    78,    79,    80,    81,    82,
       0,     0,    83,     0,    84,     0,     0,  -536,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -536,    87,  -536,  -536,  -536,  -536,  -536,
    -536,  -536,     0,  -536,  -536,  -536,  -536,  -536,     0,  -536,
    -536,  -536,  -536,    92,  -536,  -536,  -536,     0,    95,  -536,
    -536,  -536,     0,    97,  -536,    98,   290,    99,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,    34,    35,     0,    36,    37,
       0,    38,  -221,     0,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,    48,    49,     0,     0,
       0,    50,    51,    52,    53,    54,    55,    56,  -221,   -43,
      57,    58,    59,     0,    60,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,     0,    71,    72,     0,
      73,    74,    75,    76,    77,     0,    78,    79,    80,    81,
      82,     0,     0,    83,     0,    84,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,    92,     0,    93,    94,   252,    95,
       0,     0,    96,     0,    97,   291,    98,     4,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -221,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,    48,    49,     0,
       0,     0,    50,    51,    52,    53,    54,    55,    56,  -221,
     -43,    57,    58,    59,     0,    60,    61,    62,     0,    63,
      64,    65,    66,    67,    68,    69,    70,     0,    71,    72,
       0,    73,    74,    75,    76,    77,     0,    78,    79,    80,
      81,    82,     0,     0,    83,     0,    84,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,     0,    92,     0,    93,    94,     0,
      95,     0,     0,    96,     0,    97,   455,    98,   474,    99,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,    37,     0,    38,  -221,     0,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,    48,    49,
       0,     0,     0,    50,    51,    52,    53,    54,    55,    56,
    -221,   -43,    57,    58,    59,     0,    60,    61,    62,     0,
      63,    64,    65,    66,    67,    68,    69,    70,     0,    71,
      72,     0,    73,    74,    75,    76,    77,     0,    78,    79,
      80,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,     0,    92,     0,    93,    94,
       0,    95,     0,     0,    96,     0,    97,   475,    98,   290,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -221,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,    48,
      49,     0,     0,     0,    50,    51,    52,    53,    54,    55,
      56,  -221,   -43,    57,    58,    59,     0,    60,    61,    62,
       0,    63,    64,    65,    66,    67,    68,    69,    70,     0,
      71,    72,     0,    73,    74,    75,    76,    77,     0,    78,
      79,    80,    81,    82,     0,     0,    83,     0,    84,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,     0,    92,     0,    93,
      94,     0,    95,     0,     0,    96,     0,    97,   291,    98,
       4,    99,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -221,     0,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
      48,    49,     0,     0,     0,    50,    51,    52,    53,    54,
      55,    56,  -221,   -43,    57,    58,    59,     0,    60,    61,
      62,     0,    63,    64,    65,    66,    67,    68,    69,    70,
       0,    71,    72,     0,    73,    74,    75,    76,    77,     0,
      78,    79,    80,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,     0,    92,     0,
      93,    94,     0,    95,     0,     0,    96,     0,    97,   647,
      98,     4,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,    37,     0,    38,  -221,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,    48,    49,     0,     0,     0,    50,    51,    52,    53,
     305,    55,    56,  -221,   -43,    57,    58,    59,     0,    60,
      61,    62,     0,    63,    64,    65,    66,    67,    68,    69,
      70,     0,    71,    72,     0,    73,    74,    75,    76,    77,
       0,    78,    79,    80,    81,    82,     0,     0,    83,     0,
      84,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,     0,    92,
       0,    93,    94,     0,    95,     0,     0,    96,     0,    97,
       0,    98,     4,    99,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,    37,     0,    38,  -221,     0,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,    47,    48,    49,     0,     0,     0,    50,    51,    52,
      53,     0,    55,    56,  -221,   -43,    57,    58,    59,     0,
      60,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,     0,    71,    72,     0,    73,    74,    75,    76,
      77,     0,    78,    79,    80,    81,    82,     0,     0,    83,
       0,    84,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,     0,
      92,     0,    93,    94,     0,    95,     0,     0,    96,     0,
      97,     0,    98,     4,    99,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,   561,    35,     0,    36,    37,     0,    38,  -221,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,    48,    49,     0,     0,     0,    50,    51,
      52,    53,     0,    55,    56,  -221,   -43,    57,    58,    59,
       0,    60,    61,    62,     0,    63,    64,    65,    66,    67,
      68,    69,    70,     0,    71,    72,     0,    73,    74,    75,
      76,    77,     0,    78,    79,    80,    81,    82,     0,     0,
      83,     0,    84,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
       0,    92,     0,    93,    94,     0,    95,     0,     0,    96,
       0,    97,     0,    98,     4,    99,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,   567,    35,     0,    36,    37,     0,    38,
    -221,     0,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,    47,    48,    49,     0,     0,     0,    50,
      51,    52,    53,     0,    55,    56,  -221,   -43,    57,    58,
      59,     0,    60,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,     0,    71,    72,     0,    73,    74,
      75,    76,    77,     0,    78,    79,    80,    81,    82,     0,
       0,    83,     0,    84,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,     0,    92,     0,    93,    94,     0,    95,     0,     0,
      96,     0,    97,     0,    98,     4,    99,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,   827,    35,     0,    36,    37,     0,
      38,  -221,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,    48,    49,     0,     0,     0,
      50,    51,    52,    53,     0,    55,    56,  -221,   -43,    57,
      58,    59,     0,    60,    61,    62,     0,    63,    64,    65,
      66,    67,    68,    69,    70,     0,    71,    72,     0,    73,
      74,    75,    76,    77,     0,    78,    79,    80,    81,    82,
       0,     0,    83,     0,    84,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,     0,    92,     0,    93,    94,     0,    95,     0,
       0,    96,     0,    97,     0,    98,     4,    99,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,   829,    35,     0,    36,    37,
       0,    38,  -221,     0,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,    48,    49,     0,     0,
       0,    50,    51,    52,    53,     0,    55,    56,  -221,   -43,
      57,    58,    59,     0,    60,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,     0,    71,    72,     0,
      73,    74,    75,    76,    77,     0,    78,    79,    80,    81,
      82,     0,     0,    83,     0,    84,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,    92,     0,    93,    94,     0,    95,
       0,     0,    96,     0,    97,     0,    98,     4,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,   834,    35,     0,    36,
      37,     0,    38,  -221,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,    48,    49,     0,
       0,     0,    50,    51,    52,    53,     0,    55,    56,  -221,
     -43,    57,    58,    59,     0,    60,    61,    62,     0,    63,
      64,    65,    66,    67,    68,    69,    70,     0,    71,    72,
       0,    73,    74,    75,    76,    77,     0,    78,    79,    80,
      81,    82,     0,     0,    83,     0,    84,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,     0,    92,     0,    93,    94,     0,
      95,     0,     0,    96,     0,    97,     0,    98,     4,    99,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,   837,    35,     0,
      36,    37,     0,    38,  -221,     0,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,    48,    49,
       0,     0,     0,    50,    51,    52,    53,     0,    55,    56,
    -221,   -43,    57,    58,    59,     0,    60,    61,    62,     0,
      63,    64,    65,    66,    67,    68,    69,    70,     0,    71,
      72,     0,    73,    74,    75,    76,    77,     0,    78,    79,
      80,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,     0,    92,     0,    93,    94,
       0,    95,     0,     0,    96,     0,    97,     0,    98,     4,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,   857,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -221,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,    48,
      49,     0,     0,     0,    50,    51,    52,    53,     0,    55,
      56,  -221,   -43,    57,    58,    59,     0,    60,    61,    62,
       0,    63,    64,    65,    66,    67,    68,    69,    70,     0,
      71,    72,     0,    73,    74,    75,    76,    77,     0,    78,
      79,    80,    81,    82,     0,     0,    83,     0,    84,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,     0,    92,     0,    93,
      94,     0,    95,     0,     0,    96,     0,    97,     0,    98,
       4,    99,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,   863,
      35,     0,    36,    37,     0,    38,  -221,     0,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
      48,    49,     0,     0,     0,    50,    51,    52,    53,     0,
      55,    56,  -221,   -43,    57,    58,    59,     0,    60,    61,
      62,     0,    63,    64,    65,    66,    67,    68,    69,    70,
       0,    71,    72,     0,    73,    74,    75,    76,    77,     0,
      78,    79,    80,    81,    82,     0,     0,    83,     0,    84,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,     0,    92,     0,
      93,    94,     0,    95,     0,     0,    96,     0,    97,     0,
      98,     4,    99,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
     867,    35,     0,    36,    37,     0,    38,  -221,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,    48,    49,     0,     0,     0,    50,    51,    52,    53,
       0,    55,    56,  -221,   -43,    57,    58,    59,     0,    60,
      61,    62,     0,    63,    64,    65,    66,    67,    68,    69,
      70,     0,    71,    72,     0,    73,    74,    75,    76,    77,
       0,    78,    79,    80,    81,    82,     0,     0,    83,     0,
      84,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,     0,    92,
       0,    93,    94,     0,    95,     0,     0,    96,     0,    97,
       0,    98,   731,    99,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   163,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   164,   165,
       0,   166,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
      16,    47,    48,     0,    20,    21,     0,     0,    51,     0,
      53,     0,    55,    27,     0,    28,   873,    58,     0,     0,
      33,     0,     0,     0,     0,    37,    65,    66,    67,    68,
      69,   167,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    82,    50,     0,    52,
       0,    54,     0,   874,    85,     0,   875,     0,    59,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,     0,     0,     0,     0,    90,    76,
       0,     0,    78,     0,     0,    81,     0,     0,    91,     0,
      92,     0,    93,    94,   252,     0,     0,     0,    96,     0,
     168,     0,    98,     0,   169,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   163,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,    95,     0,     0,    30,   164,
     165,   951,   166,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   167,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
       0,    92,     0,    93,    94,   252,     0,     0,     0,    96,
       0,   168,     0,    98,     0,   169,   639,     5,     6,     7,
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
       0,     0,     0,    86,    87,    88,    89,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,     0,    92,     0,    93,    94,   252,     0,     0,
       0,    96,     0,   168,     0,    98,     0,   169,   960,     5,
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
       0,     0,     0,     0,     0,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,     0,    92,     0,    93,    94,     0,
       0,     0,     0,    96,     0,   168,     0,    98,     0,   169,
     644,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     163,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,   203,    21,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,   204,    30,   164,   165,     0,   166,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,    52,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,    60,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   167,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,    81,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   763,     0,   764,     0,    86,    87,    88,
      89,     0,   765,     0,     0,    90,     0,   766,   211,   767,
       0,     0,     0,     0,   768,    91,     0,    92,     0,    93,
      94,     0,   213,     0,     0,    96,     0,   168,   769,    98,
       0,   169,     0,   770,     0,   216,     0,     0,   771,     0,
     772,     0,     0,     0,     0,     0,   773,     0,     0,     0,
       0,     0,     0,   774,     0,     0,     0,     0,     0,   220,
       0,     0,     0,     0,     0,     0,     0,     0,   775,     0,
       0,     0,     0,     0,     0,     0,     0,   222,   223,     0,
     776,     0,   225,     0,   777,     0,     0,   778,     0,     0,
       0,   779,     0,     0,   229,     0,     0,   780,     0,     0,
       0,     0,     0,     0,     0,     0,   346,   347,   348,   349,
     350,   351,     0,     0,   354,   355,   356,   357,     0,   359,
     360,   781,   782,   783,   784,     0,     0,   785,     0,     0,
       0,   786,   787,   788,   789,   790,   791,   792,   793,   794,
     795,   796,   797,     0,   798,     0,     0,   799,   800,   801,
     802,     0,     0,   803,     5,     6,     7,     8,   192,    10,
     193,     0,     0,   163,     0,     0,    14,    15,     0,     0,
    -120,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   164,   165,
       0,   166,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,  -120,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   167,     0,    71,     0,     0,    73,     0,     0,   194,
      77,     0,  -120,    79,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,     0,     0,     0,     0,    90,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    91,     0,
      92,   163,    93,    94,    14,    15,     0,     0,    96,     0,
     168,    19,    98,     0,   169,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   164,   165,     0,   166,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   167,
       0,    71,     0,     0,    73,   256,   257,     0,    77,   295,
       0,    79,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,     0,     0,     0,     0,    90,   296,     0,     0,
       0,     0,     0,     0,     0,     0,    91,     0,    92,     0,
      93,    94,   252,     0,     0,     0,    96,     0,   168,     0,
      98,     0,   169,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   163,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   164,   165,     0,
     166,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     167,     0,    71,     0,     0,    73,   256,   257,     0,    77,
     295,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,     0,    92,
       0,    93,    94,   252,     0,     0,     0,    96,     0,   168,
       0,    98,   631,   169,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   163,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   164,   165,
       0,   166,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   167,     0,    71,     0,     0,    73,   256,   257,     0,
      77,   295,     0,    79,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,     0,
      92,     0,    93,    94,   252,     0,     0,     0,    96,     0,
     168,     0,    98,   633,   169,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   163,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,   530,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   164,
     165,     0,   166,    35,   531,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,   532,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   167,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,   533,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
       0,    92,     0,    93,    94,     0,     0,     0,     0,    96,
       0,   168,     0,    98,     0,   169,     5,     6,     7,     8,
     192,    10,     0,     0,     0,   163,     0,     0,    14,    15,
       0,     0,  -120,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     164,   165,     0,   166,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
    -120,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   167,     0,    71,     0,     0,    73,     0,
       0,   194,    77,     0,  -120,    79,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,     0,     0,     0,     0,
      90,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      91,     0,    92,   163,    93,    94,    14,    15,     0,     0,
      96,     0,   168,    19,    98,     0,   169,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   164,   165,
       0,   166,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   167,     0,    71,     0,     0,    73,   256,   257,     0,
      77,     0,     0,    79,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,     0,
      92,     0,    93,    94,     0,   258,     0,     0,    96,     0,
     168,     0,    98,     0,   169,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   163,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   164,
     165,     0,   166,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   167,     0,    71,     0,     0,    73,   256,   257,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
       0,    92,     0,    93,    94,   252,     0,     0,     0,    96,
       0,   168,     0,    98,     0,   169,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   163,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     164,   165,     0,   166,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,   733,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   167,     0,    71,     0,     0,    73,   256,
     257,     0,    77,     0,     0,    79,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,     0,     0,     0,     0,
      90,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      91,     0,    92,   163,    93,    94,    14,    15,     0,     0,
      96,     0,   168,    19,    98,     0,   169,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   164,   165,
       0,   166,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,   197,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   167,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    82,     0,     0,     0,
       0,     0,   198,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,     0,     0,     0,     0,    90,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    91,     0,
      92,   163,    93,    94,    14,    15,     0,     0,    96,     0,
     168,    19,    98,     0,   169,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   164,   165,     0,   166,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   167,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,     0,    82,     0,     0,     0,     0,     0,
     198,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,     0,    92,     0,
      93,    94,   252,     0,     0,     0,    96,     0,   168,     0,
      98,     0,   169,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   163,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   164,   165,     0,
     166,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     167,     0,    71,     0,     0,    73,   256,   257,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,     0,     0,     0,     0,    90,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    91,     0,    92,
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
      89,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,     0,    92,     0,    93,
      94,   252,     0,     0,     0,    96,     0,   168,   629,    98,
       0,   169,     5,     6,     7,     8,     9,    10,     0,     0,
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
     198,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,     0,     0,     0,     0,    90,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    91,     0,    92,   163,
      93,    94,    14,    15,     0,     0,    96,     0,   168,    19,
      98,     0,   169,     0,     0,     0,   238,    26,     0,     0,
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
       0,     0,     0,     0,    90,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    91,     0,    92,   163,    93,    94,
      14,    15,     0,     0,    96,     0,    97,    19,    98,     0,
     169,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   164,   165,     0,   166,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   167,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,    92,     0,    93,    94,   252,     0,
       0,     0,    96,     0,   168,     0,    98,     0,   169,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   163,     0,
       0,    14,   599,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,   600,     0,     0,     0,
       0,     0,    30,   164,   165,     0,   166,    35,     0,   601,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,   602,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,   603,   604,    67,    68,    69,   605,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,   606,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,     0,    92,     0,    93,    94,   252,
       0,     0,     0,    96,     0,   168,     0,    98,     0,   912,
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
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,     0,    92,     0,    93,    94,
     252,     0,     0,     0,    96,     0,   168,     0,    98,     0,
     912,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     163,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   164,   165,     0,   897,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   167,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,     0,    92,     0,    93,
      94,   252,     0,     0,     0,    96,     0,   168,     0,    98,
       0,   169,     5,     6,     7,     8,     9,    10,     0,     0,
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
      88,    89,     0,     0,     0,     0,    90,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    91,     0,    92,   163,
      93,    94,    14,    15,     0,     0,    96,     0,   168,    19,
      98,     0,   169,     0,     0,     0,     0,    26,     0,     0,
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
       0,     0,     0,     0,    90,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    91,     0,    92,   163,    93,    94,
      14,   495,     0,     0,    96,     0,    97,    19,    98,     0,
     169,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   164,   165,     0,   166,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   167,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,   496,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,     0,     0,
       0,     0,    90,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    91,     0,    92,   163,    93,    94,    14,    15,
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
       0,     0,    86,    87,    88,    89,     0,     0,     0,     0,
      90,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      91,     0,    92,   163,    93,    94,    14,   599,     0,     0,
     572,     0,   168,    19,    98,     0,   169,     0,     0,     0,
       0,   600,     0,     0,     0,     0,     0,    30,   164,   165,
       0,   166,    35,     0,   601,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,   602,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,   603,   604,    67,    68,
      69,   605,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,   606,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,     0,     0,     0,     0,    90,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    91,     0,
      92,   163,    93,    94,    14,    15,     0,     0,    96,     0,
     168,    19,    98,     0,   607,     0,     0,     0,     0,    26,
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
      88,    89,     0,     0,     0,     0,    90,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    91,     0,    92,   163,
      93,    94,    14,    15,     0,     0,    96,     0,   168,    19,
      98,     0,   607,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   164,   165,     0,   675,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   167,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
       0,     0,     0,     0,    90,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    91,     0,    92,   163,    93,    94,
      14,    15,     0,     0,    96,     0,   168,    19,    98,     0,
     169,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   164,   165,     0,   678,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   167,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,     0,     0,
       0,     0,    90,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    91,     0,    92,   163,    93,    94,    14,    15,
       0,     0,    96,     0,   168,    19,    98,     0,   169,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     164,   165,     0,   891,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   167,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,     0,     0,     0,     0,
      90,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      91,     0,    92,   163,    93,    94,    14,    15,     0,     0,
      96,     0,   168,    19,    98,     0,   169,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   164,   165,
       0,   892,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   167,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,     0,     0,     0,     0,    90,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    91,     0,
      92,   163,    93,    94,    14,    15,     0,     0,    96,     0,
     168,    19,    98,     0,   169,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   164,   165,     0,   894,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   167,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,     0,     0,     0,     0,    90,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    91,     0,    92,   163,
      93,    94,    14,    15,     0,     0,    96,     0,   168,    19,
      98,     0,   169,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   164,   165,     0,   895,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   167,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
       0,     0,     0,     0,    90,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    91,     0,    92,   163,    93,    94,
      14,    15,     0,     0,    96,     0,   168,    19,    98,     0,
     169,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   164,   165,     0,   896,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   167,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,     0,     0,
       0,     0,    90,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    91,     0,    92,   163,    93,    94,    14,    15,
       0,     0,    96,     0,   168,    19,    98,     0,   169,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     164,   165,     0,   897,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   167,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,     0,     0,     0,     0,
      90,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      91,     0,    92,   163,    93,    94,    14,   599,     0,     0,
      96,     0,   168,    19,    98,     0,   169,     0,     0,     0,
       0,   600,     0,     0,     0,     0,     0,    30,   164,   165,
       0,   166,    35,     0,   601,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,   602,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,   603,   604,    67,    68,
      69,   605,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,   606,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,     0,     0,     0,     0,    90,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    91,     0,
      92,   163,    93,    94,    14,    15,     0,     0,    96,     0,
     168,    19,    98,     0,   979,     0,     0,     0,     0,    26,
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
      88,    89,     0,     0,     0,     0,    90,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    91,     0,    92,   163,
      93,    94,    14,    15,     0,     0,    96,     0,     0,    19,
      98,     0,   979,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   164,   165,     0,   166,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   167,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    91,     0,    92,   163,     0,    94,
      14,    15,     0,     0,    96,     0,   168,    19,    98,     0,
     169,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   164,   165,     0,   166,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   167,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,  -389,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,  -389,
       0,     0,     0,  -389,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   163,    92,     0,    14,    15,   252,     0,
       0,     0,     0,    19,   168,     0,    98,  -389,   912,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   164,   165,
       0,   166,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,  -371,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,  -371,     0,     0,     0,    65,    66,    67,    68,
      69,   167,     0,    71,     0,  -371,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    82,     0,  -371,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,     0,     0,     0,     0,     0,     0,
       0,    87,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   163,     0,     0,    14,    15,     0,     0,     0,     0,
      92,    19,     0,   361,     0,  -371,  -512,     0,  -512,    26,
    -371,     0,    98,     0,   979,    30,   164,   165,     0,   166,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   167,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   163,
       0,     0,    14,    15,     0,     0,     0,     0,    92,    19,
       0,     0,     0,     0,     0,     0,     0,    26,   168,     0,
      98,     0,   169,    30,   164,   165,     0,   166,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   167,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   163,     0,     0,
      14,    15,     0,     0,     0,     0,    92,    19,     0,     0,
       0,     0,     0,     0,     0,    26,   168,     0,    98,     0,
     607,    30,   164,   165,     0,   166,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   940,
      65,    66,    67,    68,    69,   167,   765,    71,     0,     0,
      73,   210,   211,   767,    77,     0,     0,    79,   212,     0,
      82,     0,     0,     0,     0,     0,   213,     0,     0,     0,
       0,     0,   769,     0,     0,     0,     0,   215,     0,   216,
       0,     0,   217,     0,   772,    87,     0,     0,     0,     0,
     218,     0,     0,     0,     0,     0,     0,   219,     0,     0,
       0,   318,     0,   220,    92,     0,     0,     0,   319,     0,
       0,     0,   221,     0,     0,     0,    98,     0,   169,   320,
       0,   222,   223,     0,   224,     0,   225,     0,   941,     0,
       0,   778,     0,     0,     0,   228,     0,     0,   229,     0,
       0,   230,     0,     0,     0,     0,     0,     0,     0,     0,
     346,   347,   348,   349,   350,   351,     0,     0,   354,   355,
     356,   357,     0,   359,   360,   781,   782,   783,   784,   321,
       0,   785,     0,   322,     0,   786,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,   797,     0,   798,   318,
       0,   799,   800,   801,   802,     0,   319,     0,     0,     0,
       0,     0,     0,     0,   323,     0,     0,   320,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,     0,   325,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,   334,   335,     0,   336,   337,   338,   339,
       0,   340,   341,   342,     0,   318,   343,   344,   345,     0,
       0,     0,   319,     0,     0,   975,     0,   321,     0,     0,
       0,   322,     0,   320,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   318,     0,     0,     0,     0,     0,     0,   319,     0,
       0,     0,   323,     0,     0,     0,     0,     0,     0,   320,
       0,     0,     0,     0,     0,     0,     0,     0,   324,     0,
     325,   326,   327,   321,   328,   329,   330,   322,   331,   332,
     333,   334,   335,     0,   336,   337,   338,   339,     0,   340,
     341,   342,     0,   318,   343,   344,   345,     0,     0,     0,
     319,     0,     0,  1042,     0,     0,     0,     0,   323,   321,
       0,   320,   402,   322,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,   403,   325,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,   334,   335,     0,
     336,   337,   338,   339,   323,   340,   341,   342,     0,     0,
     343,   344,   345,     0,     0,     0,     0,   630,     0,     0,
     324,   321,   325,   326,   327,   322,   328,   329,   330,     0,
     331,   332,   333,   334,   335,     0,   336,   337,   338,   339,
       0,   340,   341,   342,   318,     0,   343,   344,   345,     0,
       0,   319,   399,   939,     0,     0,   323,     0,     0,     0,
       0,     0,   320,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,     0,   325,   326,   327,     0,   328,   329,
     330,     0,   331,   332,   333,   334,   335,     0,   336,   337,
     338,   339,     0,   340,   341,   342,     0,     0,   343,   344,
     345,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   321,     0,     0,   318,   322,     0,     0,     0,
       0,     0,   319,     0,     0,   410,     0,     0,     0,     0,
       0,     0,     0,   320,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   323,   346,   347,
     348,   349,   350,   351,     0,     0,   354,   355,   356,   357,
       0,   359,   360,   324,     0,   325,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,   334,   335,     0,   336,
     337,   338,   339,   321,   340,   341,   342,   322,     0,   343,
     344,   345,     0,   374,     0,   318,   826,     0,     0,     0,
       0,     0,   319,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   320,     0,     0,     0,     0,   323,   346,
     347,   348,   349,   350,   351,     0,     0,   354,   355,   356,
     357,     0,   359,   360,   324,     0,   325,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,   334,   335,     0,
     336,   337,   338,   339,     0,   340,   341,   342,     0,     0,
     343,   344,   345,   321,   374,     0,   318,   322,     0,     0,
       0,     0,     0,   319,     0,     0,   833,     0,     0,     0,
       0,     0,     0,     0,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   323,   346,
     347,   348,   349,   350,   351,     0,     0,   354,   355,   356,
     357,     0,   359,   360,   324,     0,   325,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,   334,   335,     0,
     336,   337,   338,   339,   321,   340,   341,   342,   322,     0,
     343,   344,   345,     0,   374,     0,   318,   952,     0,     0,
       0,     0,     0,   319,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   320,     0,     0,     0,     0,   323,
     346,   347,   348,   349,   350,   351,     0,     0,   354,   355,
     356,   357,     0,   359,   360,   324,     0,   325,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,   334,   335,
       0,   336,   337,   338,   339,     0,   340,   341,   342,     0,
       0,   343,   344,   345,   321,   374,     0,   318,   322,     0,
       0,     0,     0,     0,   319,     0,     0,   953,     0,     0,
       0,     0,     0,     0,     0,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     346,   347,   348,   349,   350,   351,     0,     0,   354,   355,
     356,   357,     0,   359,   360,   324,     0,   325,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,   334,   335,
       0,   336,   337,   338,   339,   321,   340,   341,   342,   322,
       0,   343,   344,   345,     0,   374,     0,   318,   954,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,     0,     0,     0,     0,
     323,   346,   347,   348,   349,   350,   351,     0,     0,   354,
     355,   356,   357,     0,   359,   360,   324,     0,   325,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,   334,
     335,     0,   336,   337,   338,   339,     0,   340,   341,   342,
       0,     0,   343,   344,   345,   321,   374,     0,   318,   322,
       0,     0,     0,     0,     0,   319,     0,     0,   955,     0,
       0,     0,     0,     0,     0,     0,   320,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     323,   346,   347,   348,   349,   350,   351,     0,     0,   354,
     355,   356,   357,     0,   359,   360,   324,     0,   325,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,   334,
     335,     0,   336,   337,   338,   339,   321,   340,   341,   342,
     322,     0,   343,   344,   345,     0,   374,     0,   318,   977,
       0,     0,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,     0,     0,     0,
       0,   323,   346,   347,   348,   349,   350,   351,     0,     0,
     354,   355,   356,   357,     0,   359,   360,   324,     0,   325,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
     334,   335,     0,   336,   337,   338,   339,     0,   340,   341,
     342,     0,     0,   343,   344,   345,   321,   374,     0,     0,
     322,     0,     0,     0,     0,     0,     0,     0,     0,   978,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     318,     0,     0,     0,     0,     0,     0,   319,     0,     0,
       0,   323,   346,   347,   348,   349,   350,   351,   320,   396,
     354,   355,   356,   357,     0,   359,   360,   324,     0,   325,
     326,   327,   397,   328,   329,   330,     0,   331,   332,   333,
     334,   335,     0,   336,   337,   338,   339,     0,   340,   341,
     342,     0,   318,   343,   344,   345,     0,   374,     0,   319,
       0,     0,     0,     0,     0,     0,     0,     0,   321,     0,
     320,   704,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   318,     0,
       0,     0,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,   323,     0,     0,   320,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
     321,   325,   326,   327,   322,   328,   329,   330,     0,   331,
     332,   333,   334,   335,     0,   336,   337,   338,   339,     0,
     340,   341,   342,     0,     0,   343,   344,   345,     0,   374,
       0,   399,   320,     0,     0,   323,   321,     0,     0,     0,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,     0,   325,   326,   327,     0,   328,   329,   330,
       0,   331,   332,   333,   334,   335,     0,   336,   337,   338,
     339,   323,   340,   341,   342,     0,     0,   343,   344,   345,
       0,   374,   321,     0,     0,     0,   322,   324,     0,   325,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
     334,   335,     0,   336,   337,   338,   339,     0,   340,   341,
     342,   318,     0,   343,   344,   345,     0,   374,   319,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   320,
     696,     0,     0,   324,     0,   325,   326,   327,     0,   328,
     329,   330,     0,     0,   332,   333,   334,   318,     0,   336,
     337,   338,   339,     0,   319,     0,   342,     0,     0,   343,
     344,   345,     0,     0,     0,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   321,
       0,     0,     0,   322,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,     0,     0,     0,     0,
       0,     0,     0,     0,   323,   321,     0,     0,     0,   322,
       0,     0,     0,     0,     0,     0,     0,     0,   714,     0,
     324,     0,   325,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,   334,   335,     0,   336,   337,   338,   339,
     323,   340,   341,   342,     0,   321,   343,   344,   345,   322,
     374,     0,     0,     0,     0,     0,   324,     0,   325,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,   334,
     335,     0,   336,   337,   338,   339,     0,   340,   341,   342,
     323,     0,   343,   344,   345,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,     0,   325,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,   334,
     335,     0,   336,   337,   338,   339,     0,   340,   341,   342,
     318,     0,   343,   344,   345,     0,   437,   319,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   320,     0,
       0,     0,     0,   407,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   318,   408,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,     0,   321,     0,
     320,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
     321,   325,   326,   327,   322,   328,   329,   330,     0,   331,
     332,   333,   334,   335,     0,   336,   337,   338,   339,     0,
     340,   341,   342,   318,   409,   343,   344,   345,     0,     0,
     319,   158,     0,     0,     0,   323,     0,     0,     0,     0,
       0,   320,     0,     0,     0,     0,   450,     0,     0,     0,
       0,   324,     0,   325,   326,   327,     0,   328,   329,   330,
       0,   331,   332,   333,   334,   335,     0,   336,   337,   338,
     339,     0,   340,   341,   342,   318,   451,   343,   344,   345,
       0,     0,   319,     0,     0,     0,     0,     0,     0,     0,
       0,   321,     0,   320,   539,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   540,     0,     0,
       0,   318,     0,     0,     0,     0,     0,     0,   319,     0,
       0,     0,     0,     0,     0,     0,   323,     0,     0,   320,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   321,   325,   326,   327,   322,   328,   329,
     330,     0,   331,   332,   333,   334,   335,     0,   336,   337,
     338,   339,     0,   340,   341,   342,     0,   318,   343,   344,
     345,     0,     0,     0,   319,     0,     0,     0,   323,   321,
       0,     0,     0,   322,     0,   320,   541,     0,     0,     0,
       0,     0,     0,     0,   324,     0,   325,   326,   327,   542,
     328,   329,   330,     0,   331,   332,   333,   334,   335,     0,
     336,   337,   338,   339,   323,   340,   341,   342,     0,     0,
     343,   344,   345,     0,     0,     0,     0,     0,     0,     0,
     324,     0,   325,   326,   327,   321,   328,   329,   330,   322,
     331,   332,   333,   334,   335,     0,   336,   337,   338,   339,
       0,   340,   341,   342,   318,   441,   343,   344,   345,     0,
       0,   319,     0,     0,     0,     0,     0,     0,     0,     0,
     323,     0,   320,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,     0,   325,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,   334,
     335,     0,   336,   337,   338,   339,     0,   340,   341,   342,
     318,     0,   343,   344,   345,     0,     0,   319,     0,     0,
       0,     0,   321,     0,     0,     0,   322,     0,   320,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   318,     0,     0,     0,
       0,     0,     0,   319,     0,     0,     0,   323,     0,     0,
       0,     0,     0,     0,   320,     0,     0,     0,     0,     0,
       0,     0,     0,   324,     0,   325,   326,   327,   321,   328,
     329,   330,   322,   331,   332,   333,   334,   335,     0,   336,
     337,   338,   339,     0,   340,   341,   342,     0,   443,   343,
     344,   345,   318,     0,     0,     0,     0,     0,     0,   319,
       0,     0,     0,   323,   321,     0,     0,     0,   322,     0,
     320,     0,     0,     0,     0,     0,     0,     0,     0,   324,
       0,   325,   326,   327,     0,   328,   329,   330,     0,   331,
     332,   333,   334,   335,     0,   336,   337,   338,   339,   323,
     340,   341,   342,     0,   445,   343,   344,   345,     0,     0,
       0,     0,     0,     0,     0,   324,     0,   325,   326,   327,
     321,   328,   329,   330,   322,   331,   332,   333,   334,   335,
       0,   336,   337,   338,   339,     0,   340,   341,   342,   318,
     453,   343,   344,   345,     0,     0,   319,     0,     0,     0,
       0,     0,     0,     0,   470,   323,     0,   320,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,     0,   325,   326,   327,     0,   328,   329,   330,
       0,   331,   332,   333,   334,   335,     0,   336,   337,   338,
     339,     0,   340,   341,   342,   318,     0,   343,   344,   345,
       0,     0,   319,     0,     0,     0,     0,   321,     0,     0,
       0,   322,     0,   320,     0,     0,     0,     0,     0,     0,
     543,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,     0,
     325,   326,   327,   321,   328,   329,   330,   322,   331,   332,
     333,   334,   335,     0,   336,   337,   338,   339,     0,   340,
     341,   342,   318,     0,   343,   344,   345,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,     0,   323,     0,
     320,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,     0,   325,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,   334,   335,  -484,
     336,   337,   338,   339,     0,   340,   341,   342,   318,     0,
     343,   344,   345,     0,     0,   319,     0,     0,     0,     0,
     321,     0,     0,     0,   322,     0,   320,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   158,     0,     0,   318,   323,     0,     0,     0,     0,
       0,   319,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,   320,   325,   326,   327,   321,   328,   329,   330,
     322,   331,   332,   333,   334,   335,     0,   336,   337,   338,
     339,     0,   340,   341,   342,   318,   709,   343,   344,   345,
       0,     0,   319,     0,     0,     0,     0,     0,     0,     0,
       0,   323,     0,   320,     0,     0,     0,     0,     0,     0,
       0,     0,   321,     0,     0,     0,   322,   324,     0,   325,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
     334,   335,     0,   336,   337,   338,   339,     0,   340,   341,
     342,   318,   685,   343,   344,   345,   754,   323,   319,     0,
       0,     0,     0,   321,     0,     0,     0,   322,     0,   320,
       0,     0,   712,   324,     0,   325,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,   334,   335,     0,   336,
     337,   338,   339,     0,   340,   341,   342,     0,   323,   343,
     344,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,     0,   325,   326,   327,   321,
     328,   329,   330,   322,   331,   332,   333,   334,   335,     0,
     336,   337,   338,   339,     0,   340,   341,   342,   318,     0,
     343,   344,   345,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,     0,   323,     0,   320,   816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,     0,   325,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,   334,   335,     0,   336,   337,   338,   339,
       0,   340,   341,   342,   318,     0,   343,   344,   345,     0,
       0,   319,     0,     0,     0,     0,   321,     0,     0,     0,
     322,     0,   320,   819,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     318,     0,     0,     0,     0,     0,     0,   319,     0,     0,
       0,   323,     0,     0,     0,     0,     0,     0,   320,     0,
       0,     0,     0,     0,     0,     0,     0,   324,     0,   325,
     326,   327,   321,   328,   329,   330,   322,   331,   332,   333,
     334,   335,     0,   336,   337,   338,   339,     0,   340,   341,
     342,     0,     0,   343,   344,   345,   318,   907,     0,     0,
       0,     0,     0,   319,     0,     0,     0,   323,   321,     0,
       0,     0,   322,     0,   320,     0,     0,     0,     0,     0,
       0,   890,     0,   324,     0,   325,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,   334,   335,     0,   336,
     337,   338,   339,   323,   340,   341,   342,     0,     0,   343,
     344,   345,     0,     0,     0,     0,     0,     0,     0,   324,
       0,   325,   326,   327,   321,   328,   329,   330,   322,   331,
     332,   333,   334,   335,     0,   336,   337,   338,   339,     0,
     340,   341,   342,   318,     0,   343,   344,   345,     0,     0,
     319,     0,     0,     0,     0,     0,     0,     0,     0,   323,
       0,   320,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,     0,   325,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,   334,   335,
       0,   336,   337,   338,   339,     0,   340,   341,   342,     0,
       0,   343,   344,   345,     0,     0,     0,     0,     0,     0,
       0,   321,     0,     0,   318,   322,     0,     0,     0,     0,
       0,   319,     0,     0,   893,     0,     0,     0,     0,     0,
       0,     0,   320,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,     0,   325,   326,   327,     0,   328,   329,
     330,     0,   331,   332,   333,   334,   335,     0,   336,   337,
     338,   339,   321,   340,   341,   342,   322,     0,   343,   344,
     345,     0,     0,     0,   318,   988,     0,     0,     0,     0,
       0,   319,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   320,     0,     0,     0,     0,   323,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,     0,   325,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,   334,   335,     0,   336,
     337,   338,   339,   320,   340,   341,   342,     0,     0,   343,
     344,   345,   321,     0,     0,   318,   322,     0,     0,     0,
       0,     0,   319,     0,     0,   989,     0,     0,     0,     0,
       0,     0,     0,   320,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   323,     0,     0,
       0,     0,     0,   321,     0,     0,     0,   322,     0,     0,
       0,     0,     0,   324,     0,   325,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,   334,   335,     0,   336,
     337,   338,   339,   321,   340,   341,   342,   322,   323,   343,
     344,   345,     0,     0,     0,   318,   990,     0,     0,     0,
       0,     0,   319,     0,   324,     0,   325,   326,   327,     0,
     328,   329,   330,   320,   331,   332,   333,   334,   323,     0,
     336,   337,   338,   339,     0,   340,   341,   342,     0,     0,
     343,   344,   345,     0,   324,     0,   325,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,   334,   335,     0,
     336,   337,   338,   339,   320,   340,   341,   342,     0,     0,
     343,   344,   345,   321,     0,     0,   318,   322,     0,     0,
       0,     0,     0,   319,     0,     0,   991,     0,     0,     0,
       0,     0,     0,     0,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   323,     0,
       0,     0,     0,     0,   321,     0,     0,     0,   322,     0,
       0,     0,     0,     0,   324,     0,   325,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,   334,   335,     0,
     336,   337,   338,   339,   321,   340,   341,   342,   322,   323,
     343,   344,   345,     0,     0,     0,   318,   992,     0,     0,
       0,     0,     0,   319,     0,   324,     0,   325,   326,   327,
       0,   328,   329,   330,   320,   331,   332,   333,   334,   323,
       0,   336,   337,   338,   339,     0,   340,     0,   342,     0,
       0,   343,   344,   345,     0,   324,     0,   325,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,   334,   335,
       0,   336,   337,   338,   339,     0,   340,   341,   342,     0,
       0,   343,   344,   345,   321,     0,     0,   318,   322,     0,
       0,     0,     0,     0,   319,     0,     0,   993,     0,     0,
       0,     0,     0,     0,     0,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1015,
       0,     0,     0,     0,     0,   324,     0,   325,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,   334,   335,
       0,   336,   337,   338,   339,   321,   340,   341,   342,   322,
       0,   343,   344,   345,     0,     0,     0,   318,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,     0,     0,     0,     0,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,     0,   325,   326,
     327,     0,   328,   329,   330,   320,   331,   332,   333,   334,
     335,     0,   336,   337,   338,   339,     0,   340,   341,   342,
       0,     0,   343,   344,   345,   321,     0,     0,   318,   322,
       0,     0,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,     0,     0,     0,
       0,     0,     0,     0,     0,   321,     0,     0,     0,   322,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,     0,   325,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,   334,
     335,     0,   336,   337,   338,   339,   673,   340,   341,   342,
     322,     0,   343,   344,   345,     0,   324,     0,   325,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,   334,
       0,     0,   336,   337,   338,   339,     0,   340,     0,   342,
       0,   323,   343,   344,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   324,     0,   325,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
     334,   335,     0,   336,   337,   338,   339,     0,   340,   341,
     342,     0,     0,   343,   344,   345,   651,     0,   209,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     210,   211,     0,     0,     0,     0,     0,   212,     0,     0,
       0,     0,     0,     0,     0,   213,     0,     0,     0,     0,
       0,   214,     0,     0,     0,     0,   215,     0,   216,     0,
       0,   217,     0,     0,     0,     0,     0,     0,     0,   218,
       0,     0,     0,     0,     0,   209,   219,     0,     0,     0,
       0,     0,   220,     0,     0,     0,     0,   210,   211,     0,
       0,   221,     0,     0,   212,     0,     0,     0,     0,     0,
     222,   223,   213,   224,     0,   225,     0,   226,   214,     0,
     227,     0,     0,   215,   228,   216,     0,   229,   217,     0,
     230,     0,     0,     0,     0,     0,   218,     0,     0,     0,
       0,     0,     0,   219,     0,     0,     0,     0,     0,   220,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
       0,     0,     0,     0,     0,     0,     0,   222,   223,     0,
     224,     0,   225,     0,   226,     0,     0,   227,     0,     0,
       0,   228,     0,     0,   229,     0,     0,   230
};

static const yytype_int16 yycheck[] =
{
      11,   143,   367,     2,    15,   565,   131,    18,    20,    56,
      57,    17,   202,    24,   446,   418,   181,    28,    29,   730,
      31,    32,    33,    34,   660,    28,   208,     1,     1,   194,
     756,    42,    29,    44,    45,   415,     1,   189,    49,   849,
      51,    29,    53,   598,   807,   808,   922,    64,    24,    42,
      61,    62,    63,    64,    65,    66,   208,   182,   846,    70,
     960,    72,    29,    74,    75,    29,   248,    42,    79,    80,
       1,    42,    83,    84,    29,     3,    87,    42,    62,    90,
      91,    92,    61,    94,  1020,   267,    97,    98,    99,   106,
     134,    64,    68,   109,    42,   114,   248,   161,     0,   163,
     811,    80,  1038,    79,    79,  1005,   154,  1005,    79,   109,
     670,   301,   302,   423,    98,   267,     3,   672,   162,   138,
      99,    52,    98,   911,   408,    42,   936,    42,    31,    32,
      42,    81,   274,   106,     1,    81,   446,   134,    42,   850,
     114,   134,   109,   160,  1042,   109,  1022,     3,   164,   114,
      81,   877,   163,   164,   165,   166,   167,   168,   169,   133,
     134,   109,   159,    94,    92,    79,   109,   451,   133,   134,
      28,   159,   727,   899,   180,   134,   107,   329,   189,   942,
     943,   363,   364,    19,   196,    52,   134,   160,   162,   154,
     169,   203,   159,   204,   597,   159,    99,   208,    52,   910,
     565,   204,   133,    47,   159,   164,    19,     3,   154,   159,
     161,   363,   364,   159,    81,    24,   164,   134,   134,   134,
      78,   164,   134,   154,    82,    81,   134,   134,   159,    73,
     134,    74,    75,   134,    30,   134,    92,   248,   249,    78,
     107,    77,   107,    82,   160,   256,   257,   164,   959,   164,
     886,    42,   164,   107,   162,   162,   267,   439,    54,    68,
     164,   162,   154,   162,    77,   101,   133,   392,   160,   134,
      79,   831,    98,    64,   134,   133,   161,   135,   136,     7,
     154,   134,   140,    74,    75,   296,   160,   439,   101,    98,
     148,    24,   159,    24,   293,   104,    92,   155,   156,   157,
     160,   154,    98,    89,   161,   670,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   721,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,    68,   161,    68,   163,   719,
     134,   352,   353,    24,   161,   134,    79,   358,    79,   159,
     361,   362,   363,   364,   365,   366,   367,   161,    74,   163,
     154,    42,   134,   189,    45,   376,   154,    98,   352,   353,
     381,   104,   383,   162,   358,   154,   756,   361,   362,   569,
     570,    28,   208,   394,    65,   396,   397,    68,   179,    70,
     162,   402,   403,   134,   406,   134,   407,   408,    79,   410,
     134,   412,   154,   555,   134,   134,   114,   134,   199,   200,
     201,   202,   144,   154,   425,   394,   207,    98,   321,   322,
     154,   432,   248,   162,   856,   154,   134,   154,   439,    78,
     138,    78,   162,    82,   460,    82,   462,   154,   134,   450,
     451,   267,   154,   114,   154,   154,   457,   637,   638,   460,
     241,   462,   154,   244,   162,   466,   467,   468,   154,   470,
     471,   626,   365,   366,   161,   159,   831,   478,   139,    24,
     141,   142,   263,   376,   145,   146,   460,   476,   462,   136,
     154,   108,   161,   161,   495,   496,   133,    42,   154,   136,
      45,   154,   283,   140,   397,   161,   161,   163,   133,   478,
     403,   136,   882,   114,   154,   140,     7,    56,   133,   154,
      65,   159,    42,    68,   114,    70,    52,   134,   660,   899,
     162,   154,    52,    42,    79,   161,    31,   134,   539,   540,
     541,   542,   543,   544,   545,   114,   162,   363,   364,   849,
     546,   547,   553,    98,   555,    81,   856,   162,   559,    28,
     561,    81,     2,   114,   565,   134,   567,   162,    94,   138,
     162,    11,    18,   466,   134,   164,   164,   159,   154,   553,
     154,   107,   154,    23,   154,    25,    64,   107,   154,   134,
      42,   162,   154,   162,   114,   154,   377,   164,   599,   134,
      52,   602,   603,   604,   605,   606,   607,   154,    31,    78,
     611,   160,   162,    82,   134,   134,   432,   162,   162,   160,
     401,   154,   154,   439,   405,   114,    79,    29,   154,    81,
      70,    29,   159,   159,   154,   164,   936,   611,   607,   159,
      31,   457,   162,   133,   460,    79,   462,   540,   164,   542,
     133,   467,   164,   164,   160,   107,   161,    97,    31,   136,
     960,   101,   114,   136,   133,   136,   135,   136,   136,   670,
     136,   140,   673,   674,   675,   975,   136,   678,   136,   148,
     136,   136,   134,   162,   160,   966,   136,   136,   157,   136,
     162,   136,   134,   694,    94,   696,   307,   164,   451,   450,
     701,    22,   154,   704,     2,  1005,   707,   159,   709,   741,
     162,   712,   275,    11,   195,   557,   824,   759,   886,   159,
    1020,   199,   200,   201,   202,    23,   882,    25,  1009,  1010,
    1011,  1012,  1013,  1014,   719,   762,   737,   553,  1038,   555,
    1022,   590,  1042,   959,   984,  1015,  1008,    33,   749,   426,
     751,    42,    -1,   754,   886,    -1,    -1,   758,   555,    -1,
      -1,    52,    -1,   241,    -1,    -1,   244,    52,   737,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    74,    75,    -1,    52,
     673,   674,   563,   564,    -1,   263,    -1,    -1,   569,   570,
      81,   231,    -1,    -1,    -1,    -1,    81,    -1,   238,    97,
     581,    -1,   803,   101,    -1,   283,    -1,    -1,    81,    94,
      -1,    -1,    -1,    -1,   815,   816,   107,   818,   819,   820,
     821,    94,   107,   114,    -1,   826,   827,    -1,   829,    -1,
     831,    -1,   833,   834,   107,    -1,   837,   874,   875,    -1,
      -1,   622,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   293,   855,   856,    -1,    -1,    -1,    -1,
     861,   159,   863,   154,    -1,    -1,   867,   307,   159,   154,
      -1,   162,   873,    -1,   159,    -1,    -1,    -1,    -1,    -1,
      -1,   154,    -1,    -1,    -1,    -1,   159,    -1,    -1,   890,
     891,   892,   893,   894,   895,   896,   897,    -1,    -1,   377,
      -1,    -1,   683,   684,    -1,    -1,   907,    -1,    -1,    -1,
     691,   912,   693,    -1,    -1,    -1,   917,    -1,    -1,   700,
     921,    -1,   703,   401,    -1,   706,     2,   405,    -1,    -1,
      -1,    -1,    -1,   231,   374,    11,    -1,    -1,    -1,    -1,
     238,    -1,    -1,   912,    -1,    -1,    -1,    23,    -1,    25,
      -1,   952,   953,   954,   955,    -1,   396,    -1,    -1,    -1,
      -1,    28,   402,   744,    -1,    -1,    -1,    -1,    -1,    -1,
     410,    -1,    -1,    -1,   975,     2,   977,   978,   979,    -1,
      -1,    -1,    -1,    -1,    11,    -1,    -1,   988,   989,   990,
     991,   992,   993,    -1,    70,   293,    23,    -1,    25,    -1,
      -1,    23,    24,  1004,  1005,    -1,  1007,    -1,    -1,   307,
     979,    78,    34,    35,    -1,    82,    -1,    -1,    -1,    -1,
      -1,    97,    44,    -1,   464,   101,    -1,    -1,   468,    -1,
      -1,    -1,    -1,   849,    -1,    -1,   476,  1038,    -1,    -1,
      -1,  1042,    -1,    70,    66,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    79,    -1,    24,
      -1,    -1,   129,    -1,   131,   132,   133,    -1,   135,   136,
      97,    -1,    -1,   140,   101,    -1,   374,    42,   859,    -1,
      45,   148,   104,   159,    -1,   563,   564,    -1,   155,   156,
     157,   569,   570,    -1,    -1,    -1,   536,    -1,   396,    -1,
      65,   917,    -1,    68,   402,    70,    78,    -1,    -1,    -1,
      82,    -1,   410,    -1,    79,    -1,    -1,    -1,    -1,    -1,
     901,   902,    -1,    -1,   905,   906,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    98,    -1,    -1,    -1,    -1,    -1,    -1,
     580,   922,    -1,    -1,   960,    39,    -1,   928,    -1,   930,
      -1,   932,    -1,    -1,    -1,   231,    -1,   129,    -1,   131,
     132,   133,   238,   135,   136,   137,   464,    -1,   140,    -1,
     468,    -1,    -1,    -1,    -1,   147,   148,    -1,   476,    -1,
     152,    -1,    76,   155,   156,   157,    -1,    -1,    -1,  1005,
      -1,    -1,    -1,    -1,    -1,    -1,   636,    23,    24,    -1,
      -1,    -1,   642,    -1,   231,   683,   684,    -1,    34,    35,
      -1,   238,    -1,   691,    -1,    -1,    -1,   293,    44,    -1,
     660,    47,    -1,    -1,    -1,   703,  1042,    -1,   706,    -1,
     124,   307,   126,   673,   674,    -1,    -1,    -1,    -1,    -1,
      66,  1022,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,
       3,    -1,    -1,    79,   694,    -1,   696,    -1,    -1,    -1,
      -1,   701,    15,    16,   704,    -1,   293,    -1,    -1,    22,
      -1,   711,    -1,    -1,   714,    -1,    -1,    30,   104,    -1,
     307,    -1,   580,    36,    -1,    -1,    -1,   181,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,   736,    -1,   374,    -1,
     194,    54,    -1,   197,    -1,    -1,    -1,    -1,    61,   749,
      -1,   751,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
     396,    -1,    -1,    76,    -1,    -1,   402,    -1,    -1,    -1,
      -1,    -1,    85,    86,   410,    88,    -1,    90,   636,    92,
     234,     2,    95,    -1,   642,    -1,    99,   374,    -1,   102,
      11,    -1,   105,    -1,    -1,    -1,    -1,    -1,   252,    -1,
      -1,    -1,    23,    -1,    25,    -1,    -1,    -1,    -1,   396,
      -1,    -1,    -1,    -1,    -1,   402,    -1,    -1,    -1,    -1,
      -1,   859,    -1,   410,    -1,    -1,   826,    -1,   464,     1,
      -1,    -1,   468,   833,    -1,    -1,   694,    -1,   696,    -1,
     476,    -1,    -1,   701,    -1,   845,   704,    19,   161,    70,
      -1,    23,    24,   711,    -1,    -1,   714,   857,    -1,    -1,
      32,    -1,    34,    35,    -1,    -1,   866,    39,    -1,    -1,
     870,    -1,    44,    -1,    -1,    47,    97,   464,   736,    -1,
     101,   468,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,
      -1,   749,    -1,   751,    66,    -1,    68,    -1,    70,    -1,
      72,    73,    -1,    75,    -1,    77,    -1,    79,   908,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,    -1,   391,   159,    -1,
      -1,    -1,    -1,    -1,   580,    19,    -1,    -1,    -1,    23,
      24,    -1,   952,   953,   954,   955,    -1,    -1,    32,    -1,
      34,    35,    -1,    78,   418,    39,    -1,    82,   826,    -1,
      44,    -1,    -1,    -1,    -1,   833,    -1,   977,   978,    -1,
      -1,    -1,   154,    -1,    -1,    -1,    -1,   845,   160,    -1,
      -1,    -1,    66,   580,    68,    -1,    70,    -1,    72,   857,
     636,    75,    -1,    77,    -1,    79,   642,    -1,   866,    -1,
     231,    -1,   870,    -1,   129,    -1,     3,   238,   133,    -1,
     135,   136,    -1,   477,    98,   140,   480,   101,    15,    -1,
     104,    -1,    -1,   148,    -1,    22,    -1,    -1,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,   636,
     908,    -1,    -1,    -1,    41,   642,    -1,    -1,   694,    46,
     696,    -1,    -1,    -1,    -1,   701,    -1,    54,   704,    -1,
       2,    -1,   293,    -1,    61,   711,    -1,    -1,   714,    11,
     154,    -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,    76,
      -1,    23,    -1,    25,   952,   953,   954,   955,    -1,    -1,
     736,    88,    -1,    -1,    -1,    92,    -1,   694,    -1,   696,
      -1,    -1,    99,   749,   701,   751,    -1,   704,   105,   977,
     978,    -1,    -1,    -1,   711,    -1,    -1,   714,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,   593,
      -1,    -1,    -1,   597,    -1,    -1,    -1,    -1,    -1,   736,
      -1,    -1,    -1,   374,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   749,    -1,   751,    97,    -1,     3,   622,   101,
      -1,    -1,   626,    -1,    -1,   396,    -1,    -1,    -1,    15,
      16,   402,    -1,    -1,    -1,    -1,    22,    -1,    -1,   410,
     826,    -1,    -1,     2,    30,    -1,    -1,   833,    -1,    -1,
      36,    -1,    11,    -1,    -1,    41,    -1,    43,    -1,   845,
      46,    -1,    -1,    -1,    23,    -1,    25,    -1,    54,    -1,
      -1,   857,    -1,    -1,    -1,    61,    -1,   159,    28,    -1,
     866,    67,    -1,    -1,   870,    -1,    -1,    -1,    -1,   826,
      76,    -1,    -1,   464,    -1,    -1,   833,   468,    -1,    85,
      86,    -1,    88,    -1,    90,   476,    92,    -1,   845,    95,
      -1,    70,    -1,    99,   718,    -1,   102,   721,    -1,   105,
     857,    -1,   908,    -1,    -1,    -1,    -1,    -1,    78,   866,
      -1,    -1,    82,   870,    -1,    -1,    -1,   741,    97,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,   231,
      -1,    -1,    -1,    -1,    -1,   759,   238,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   952,   953,   954,   955,
      -1,   908,    -1,   159,    -1,   161,    -1,    -1,   782,   129,
      -1,   131,   132,   133,    -1,   135,   136,    -1,    -1,    -1,
     140,   977,   978,    -1,    -1,    -1,    -1,   147,   148,    -1,
     159,    -1,   152,    -1,    -1,   155,   156,   157,    -1,   580,
      -1,   293,    -1,     1,    -1,   952,   953,   954,   955,    -1,
      -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    23,    24,    -1,    -1,    -1,
     977,   978,    -1,     2,    32,    -1,    34,    35,    28,    -1,
      -1,    39,    11,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    23,   636,    25,    -1,    -1,    -1,
      -1,   642,   231,    -1,    -1,    -1,    -1,    -1,    66,   238,
      68,    -1,    70,    -1,    72,    73,    -1,    75,    -1,    77,
      -1,    79,   374,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    70,    -1,   101,   396,    -1,   104,    -1,    -1,    -1,
     402,    -1,    -1,   694,    -1,   696,    -1,    -1,   410,    -1,
     701,    -1,    -1,   704,   293,    -1,     2,    -1,    97,    -1,
     711,    -1,   101,   714,    -1,    11,    -1,    -1,   307,   129,
      -1,    -1,   132,   133,    -1,   135,   136,    23,    -1,    25,
     140,    -1,    -1,    -1,    -1,   736,   154,    -1,   148,    -1,
      -1,    -1,   160,    -1,    -1,   155,   156,   157,   749,    -1,
     751,    -1,   464,    -1,    -1,    -1,   468,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   476,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   374,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    -1,     3,    -1,   101,    -1,   396,    -1,    -1,
      -1,    -1,    -1,   402,    -1,    15,    16,    -1,    -1,    -1,
      -1,   410,    22,    -1,    -1,   826,    -1,    -1,    -1,    -1,
      30,    -1,   833,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    41,   231,    43,   845,    -1,    46,    -1,    -1,   238,
      -1,    -1,    -1,    -1,    54,    -1,   857,    -1,    -1,    -1,
      -1,    61,    -1,   159,    -1,   866,    -1,    67,   580,   870,
      -1,    -1,    -1,    -1,    -1,   464,    76,    -1,    -1,   468,
      -1,    -1,    -1,    -1,    -1,    85,    86,   476,    88,    -1,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
     100,    -1,   102,    -1,   293,   105,    -1,   908,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   307,    -1,
      -1,    -1,    -1,    -1,   636,    -1,    -1,    -1,    -1,    -1,
     642,    -1,    -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,
      -1,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   952,   953,   954,   955,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   977,   978,    -1,    -1,
      -1,    -1,   694,    -1,   696,   374,    -1,    -1,    -1,   701,
      -1,   580,   704,    -1,    -1,    -1,    -1,   293,    -1,   711,
      -1,    -1,   714,    -1,    -1,    -1,    -1,   396,    -1,    -1,
      -1,   307,    -1,   402,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   410,    -1,    -1,   736,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   749,    -1,   751,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   636,    -1,    -1,
      -1,    -1,    -1,   642,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,    -1,    -1,   464,    -1,    -1,   374,   468,
      -1,    -1,    -1,    -1,   144,    -1,    -1,   476,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
     396,   161,    -1,   163,    -1,   694,   402,   696,    -1,    -1,
      -1,    -1,   701,    -1,   826,   704,    -1,    -1,    -1,    -1,
      -1,   833,   711,    -1,    -1,   714,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   845,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   857,    -1,   736,    15,    16,
      -1,    -1,    -1,    -1,   866,    22,    -1,    -1,   870,    -1,
     749,    -1,   751,    30,    -1,    -1,    -1,    -1,   464,    36,
      -1,    -1,   468,    19,    41,    -1,    43,    23,    24,    46,
     476,    -1,    -1,    -1,    -1,    -1,    32,    54,    34,    35,
      -1,   580,    -1,    39,    61,    -1,   908,    -1,    44,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      66,    88,    68,    90,    70,    92,    72,    -1,    95,    75,
      -1,    77,    99,    79,    -1,   102,    -1,   826,   105,    -1,
     952,   953,   954,   955,   833,    -1,    -1,   636,    -1,    -1,
      -1,    -1,    98,   642,    -1,   101,   845,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,   977,   978,    -1,   857,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   866,    -1,    -1,
      -1,   870,    -1,    -1,   580,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   694,    -1,   696,   154,    -1,
      -1,    -1,   701,    -1,   160,   704,    -1,    -1,    -1,   908,
      -1,    -1,   711,    -1,    -1,   714,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     636,    -1,    -1,    -1,    -1,    -1,   642,   736,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     749,    10,   751,   952,   953,   954,   955,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   977,   978,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   694,    -1,
     696,    -1,    -1,    -1,    -1,   701,    10,    -1,   704,    -1,
      -1,    -1,    -1,    17,    -1,   711,    -1,    -1,   714,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    82,    -1,    -1,    -1,   826,    -1,    -1,
     736,    -1,    -1,    -1,   833,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   749,    -1,   751,   845,    -1,    -1,    -1,
     109,    -1,    -1,    -1,   113,    -1,    -1,    -1,   857,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,   866,    82,    -1,
     129,   870,   131,   132,   133,    -1,   135,   136,   137,    -1,
     139,   140,   141,   142,   143,    -1,   145,   146,   147,   148,
      -1,   150,   151,   152,    -1,    -1,   155,   156,   157,   113,
      -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,   908,
      -1,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,   142,   143,
      -1,   145,   146,   147,   148,    -1,   150,   151,   152,   845,
      -1,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
     164,   857,    -1,   952,   953,   954,   955,    -1,    -1,    -1,
     866,    -1,    -1,    -1,   870,    -1,    -1,    -1,    -1,     0,
       1,    -1,     3,     4,     5,     6,     7,     8,   977,   978,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,   908,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    -1,    95,    96,    97,    98,    99,    -1,
     101,   102,   103,   104,   105,    -1,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,    -1,
     151,   152,    -1,   154,    -1,    -1,   157,    -1,   159,    -1,
     161,     1,   163,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
      -1,   151,   152,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,   161,    -1,   163,   164,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    12,    13,    14,    15,
      16,    17,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    -1,    95,
      96,    97,    98,    99,    -1,   101,   102,   103,   104,   105,
      -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,    -1,   159,   160,   161,     1,   163,     3,     4,
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
      -1,    -1,    -1,    -1,   129,   130,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,    -1,   149,    -1,   151,   152,   153,   154,
      -1,    -1,   157,    -1,   159,   160,   161,     1,   163,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    79,    80,    81,    -1,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      -1,    95,    96,    97,    98,    99,    -1,   101,   102,   103,
     104,   105,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,    -1,   149,    -1,   151,   152,    -1,
     154,    -1,    -1,   157,    -1,   159,   160,   161,     1,   163,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    -1,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    -1,    95,    96,    97,    98,    99,    -1,   101,   102,
     103,   104,   105,    -1,    -1,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,    -1,   149,    -1,   151,   152,
      -1,   154,    -1,    -1,   157,    -1,   159,   160,   161,     1,
     163,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    -1,    60,    61,
      62,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      -1,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    -1,    95,    96,    97,    98,    99,    -1,   101,
     102,   103,   104,   105,    -1,    -1,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,    -1,   149,    -1,   151,
     152,    -1,   154,    -1,    -1,   157,    -1,   159,   160,   161,
       1,   163,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    79,    80,
      81,    -1,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    -1,    95,    96,    97,    98,    99,    -1,
     101,   102,   103,   104,   105,    -1,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,    -1,
     151,   152,    -1,   154,    -1,    -1,   157,    -1,   159,   160,
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
     130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
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
     129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,
     149,    -1,   151,   152,    -1,   154,    -1,    -1,   157,    -1,
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
      -1,   129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
      -1,   149,    -1,   151,   152,    -1,   154,    -1,    -1,   157,
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
      -1,    -1,   129,   130,   131,   132,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,    -1,   149,    -1,   151,   152,    -1,   154,    -1,    -1,
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
      -1,    -1,    -1,   129,   130,   131,   132,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,    -1,   149,    -1,   151,   152,    -1,   154,    -1,
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
      -1,    -1,    -1,    -1,   129,   130,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,    -1,   149,    -1,   151,   152,    -1,   154,
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
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,    -1,   149,    -1,   151,   152,    -1,
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
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,    -1,   149,    -1,   151,   152,
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
     132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,    -1,   149,    -1,   151,
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
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,    -1,
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
     130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
      -1,   151,   152,    -1,   154,    -1,    -1,   157,    -1,   159,
      -1,   161,     1,   163,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      19,    60,    61,    -1,    23,    24,    -1,    -1,    67,    -1,
      69,    -1,    71,    32,    -1,    34,    35,    76,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,
      99,    -1,    -1,   102,    -1,    -1,   105,    66,    -1,    68,
      -1,    70,    -1,    72,   113,    -1,    75,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,    98,
      -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   147,    -1,
     149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,    -1,
     159,    -1,   161,    -1,   163,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,   154,    -1,    -1,    36,    37,
      38,   160,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
      -1,   159,    -1,   161,    -1,   163,   164,     3,     4,     5,
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
      -1,    -1,    -1,   129,   130,   131,   132,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,    -1,   149,    -1,   151,   152,   153,    -1,    -1,
      -1,   157,    -1,   159,    -1,   161,    -1,   163,   164,     3,
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
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,    -1,   149,    -1,   151,   152,    -1,
      -1,    -1,    -1,   157,    -1,   159,    -1,   161,    -1,   163,
     164,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,    -1,   129,   130,   131,
     132,    -1,    10,    -1,    -1,   137,    -1,    15,    16,    17,
      -1,    -1,    -1,    -1,    22,   147,    -1,   149,    -1,   151,
     152,    -1,    30,    -1,    -1,   157,    -1,   159,    36,   161,
      -1,   163,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      88,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,   132,    -1,    -1,   135,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,    -1,   152,    -1,    -1,   155,   156,   157,
     158,    -1,    -1,   161,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      19,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      69,    -1,    71,    -1,    -1,    -1,    -1,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,    98,
      99,    -1,   101,   102,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   147,    -1,
     149,    12,   151,   152,    15,    16,    -1,    -1,   157,    -1,
     159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    90,
      -1,    92,    -1,    -1,    95,    96,    97,    -1,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,    -1,    -1,    -1,    -1,   137,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,    -1,
     151,   152,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,
     161,    -1,   163,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    96,    97,    -1,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,
      -1,   151,   152,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,   161,   162,   163,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    -1,    95,    96,    97,    -1,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,
     149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,    -1,
     159,    -1,   161,   162,   163,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    42,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
      -1,   149,    -1,   151,   152,    -1,    -1,    -1,    -1,   157,
      -1,   159,    -1,   161,    -1,   163,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    19,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    98,    99,    -1,   101,   102,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     147,    -1,   149,    12,   151,   152,    15,    16,    -1,    -1,
     157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    -1,    95,    96,    97,    -1,
      99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,
     149,    -1,   151,   152,    -1,   154,    -1,    -1,   157,    -1,
     159,    -1,   161,    -1,   163,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    96,    97,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
      -1,   149,    -1,   151,   152,   153,    -1,    -1,    -1,   157,
      -1,   159,    -1,   161,    -1,   163,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     147,    -1,   149,    12,   151,   152,    15,    16,    -1,    -1,
     157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,
      99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,   111,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   147,    -1,
     149,    12,   151,   152,    15,    16,    -1,    -1,   157,    -1,
     159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    90,
      -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,   149,    -1,
     151,   152,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,
     161,    -1,   163,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    96,    97,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,   147,    -1,   149,
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
     132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,    -1,   149,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,    -1,   159,   160,   161,
      -1,   163,     3,     4,     5,     6,     7,     8,    -1,    -1,
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
     111,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   147,    -1,   149,    12,
     151,   152,    15,    16,    -1,    -1,   157,    -1,   159,    22,
     161,    -1,   163,    -1,    -1,    -1,    29,    30,    -1,    -1,
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
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   147,    -1,   149,    12,   151,   152,
      15,    16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,
     163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,    -1,   149,    -1,   151,   152,   153,    -1,
      -1,    -1,   157,    -1,   159,    -1,   161,    -1,   163,     3,
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
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   147,    -1,   149,    -1,   151,   152,   153,
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
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,    -1,   149,    -1,   151,   152,
     153,    -1,    -1,    -1,   157,    -1,   159,    -1,   161,    -1,
     163,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
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
     132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   147,    -1,   149,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,   161,
      -1,   163,     3,     4,     5,     6,     7,     8,    -1,    -1,
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
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   147,    -1,   149,    12,
     151,   152,    15,    16,    -1,    -1,   157,    -1,   159,    22,
     161,    -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,
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
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   147,    -1,   149,    12,   151,   152,
      15,    16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,
     163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   147,    -1,   149,    12,   151,   152,    15,    16,
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
      -1,    -1,   129,   130,   131,   132,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     147,    -1,   149,    12,   151,   152,    15,    16,    -1,    -1,
     157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,
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
     129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   147,    -1,
     149,    12,   151,   152,    15,    16,    -1,    -1,   157,    -1,
     159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,    30,
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
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   147,    -1,   149,    12,
     151,   152,    15,    16,    -1,    -1,   157,    -1,   159,    22,
     161,    -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,
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
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   147,    -1,   149,    12,   151,   152,
      15,    16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,
     163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   147,    -1,   149,    12,   151,   152,    15,    16,
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
      -1,    -1,   129,   130,   131,   132,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     147,    -1,   149,    12,   151,   152,    15,    16,    -1,    -1,
     157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,
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
     129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   147,    -1,
     149,    12,   151,   152,    15,    16,    -1,    -1,   157,    -1,
     159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,    30,
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
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   147,    -1,   149,    12,
     151,   152,    15,    16,    -1,    -1,   157,    -1,   159,    22,
     161,    -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,
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
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   147,    -1,   149,    12,   151,   152,
      15,    16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,
     163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   147,    -1,   149,    12,   151,   152,    15,    16,
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
      -1,    -1,   129,   130,   131,   132,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     147,    -1,   149,    12,   151,   152,    15,    16,    -1,    -1,
     157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,
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
     129,   130,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   147,    -1,
     149,    12,   151,   152,    15,    16,    -1,    -1,   157,    -1,
     159,    22,   161,    -1,   163,    -1,    -1,    -1,    -1,    30,
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
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   147,    -1,   149,    12,
     151,   152,    15,    16,    -1,    -1,   157,    -1,    -1,    22,
     161,    -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    87,    88,    89,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   147,    -1,   149,    12,    -1,   152,
      15,    16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,
     163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,    -1,    -1,   138,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,   149,    -1,    15,    16,   153,    -1,
      -1,    -1,    -1,    22,   159,    -1,   161,   162,   163,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    57,    58,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    94,    95,    -1,    -1,    -1,
      99,    -1,    -1,   102,    -1,    -1,   105,    -1,   107,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
     149,    22,    -1,   158,    -1,   154,   161,    -1,   163,    30,
     159,    -1,   161,    -1,   163,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    90,
      -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,   149,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,   159,    -1,
     161,    -1,   163,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    87,    88,    89,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,   149,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,   159,    -1,   161,    -1,
     163,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      85,    86,    87,    88,    89,    90,    10,    92,    -1,    -1,
      95,    15,    16,    17,    99,    -1,    -1,   102,    22,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    48,   130,    -1,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    10,    -1,    67,   149,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    76,    -1,    -1,    -1,   161,    -1,   163,    28,
      -1,    85,    86,    -1,    88,    -1,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,   132,    78,
      -1,   135,    -1,    82,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,    -1,   152,    10,
      -1,   155,   156,   157,   158,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,   133,    -1,   135,   136,   137,    -1,
     139,   140,   141,   142,   143,    -1,   145,   146,   147,   148,
      -1,   150,   151,   152,    -1,    10,   155,   156,   157,    -1,
      -1,    -1,    17,    -1,    -1,   164,    -1,    78,    -1,    -1,
      -1,    82,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,   132,   133,    78,   135,   136,   137,    82,   139,   140,
     141,   142,   143,    -1,   145,   146,   147,   148,    -1,   150,
     151,   152,    -1,    10,   155,   156,   157,    -1,    -1,    -1,
      17,    -1,    -1,   164,    -1,    -1,    -1,    -1,   113,    78,
      -1,    28,    29,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    42,   131,   132,   133,    -1,
     135,   136,   137,    -1,   139,   140,   141,   142,   143,    -1,
     145,   146,   147,   148,   113,   150,   151,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,    -1,   162,    -1,    -1,
     129,    78,   131,   132,   133,    82,   135,   136,   137,    -1,
     139,   140,   141,   142,   143,    -1,   145,   146,   147,   148,
      -1,   150,   151,   152,    10,    -1,   155,   156,   157,    -1,
      -1,    17,   109,   162,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,   131,   132,   133,    -1,   135,   136,
     137,    -1,   139,   140,   141,   142,   143,    -1,   145,   146,
     147,   148,    -1,   150,   151,   152,    -1,    -1,   155,   156,
     157,    -1,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    10,    82,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,   131,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,   142,   143,    -1,   145,
     146,   147,   148,    78,   150,   151,   152,    82,    -1,   155,
     156,   157,    -1,   159,    -1,    10,    91,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,   131,   132,   133,    -1,
     135,   136,   137,    -1,   139,   140,   141,   142,   143,    -1,
     145,   146,   147,   148,    -1,   150,   151,   152,    -1,    -1,
     155,   156,   157,    78,   159,    -1,    10,    82,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,   131,   132,   133,    -1,
     135,   136,   137,    -1,   139,   140,   141,   142,   143,    -1,
     145,   146,   147,   148,    78,   150,   151,   152,    82,    -1,
     155,   156,   157,    -1,   159,    -1,    10,    91,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,    -1,   131,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,   142,   143,
      -1,   145,   146,   147,   148,    -1,   150,   151,   152,    -1,
      -1,   155,   156,   157,    78,   159,    -1,    10,    82,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,    -1,   131,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,   142,   143,
      -1,   145,   146,   147,   148,    78,   150,   151,   152,    82,
      -1,   155,   156,   157,    -1,   159,    -1,    10,    91,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,   142,
     143,    -1,   145,   146,   147,   148,    -1,   150,   151,   152,
      -1,    -1,   155,   156,   157,    78,   159,    -1,    10,    82,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,   142,
     143,    -1,   145,   146,   147,   148,    78,   150,   151,   152,
      82,    -1,   155,   156,   157,    -1,   159,    -1,    10,    91,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,   131,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
     142,   143,    -1,   145,   146,   147,   148,    -1,   150,   151,
     152,    -1,    -1,   155,   156,   157,    78,   159,    -1,    -1,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   118,   119,    28,    29,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,   131,
     132,   133,    42,   135,   136,   137,    -1,   139,   140,   141,
     142,   143,    -1,   145,   146,   147,   148,    -1,   150,   151,
     152,    -1,    10,   155,   156,   157,    -1,   159,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      28,    29,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      78,   131,   132,   133,    82,   135,   136,   137,    -1,   139,
     140,   141,   142,   143,    -1,   145,   146,   147,   148,    -1,
     150,   151,   152,    -1,    -1,   155,   156,   157,    -1,   159,
      -1,   109,    28,    -1,    -1,   113,    78,    -1,    -1,    -1,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,   142,   143,    -1,   145,   146,   147,
     148,   113,   150,   151,   152,    -1,    -1,   155,   156,   157,
      -1,   159,    78,    -1,    -1,    -1,    82,   129,    -1,   131,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
     142,   143,    -1,   145,   146,   147,   148,    -1,   150,   151,
     152,    10,    -1,   155,   156,   157,    -1,   159,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      29,    -1,    -1,   129,    -1,   131,   132,   133,    -1,   135,
     136,   137,    -1,    -1,   140,   141,   142,    10,    -1,   145,
     146,   147,   148,    -1,    17,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    78,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
     129,    -1,   131,   132,   133,    -1,   135,   136,   137,    -1,
     139,   140,   141,   142,   143,    -1,   145,   146,   147,   148,
     113,   150,   151,   152,    -1,    78,   155,   156,   157,    82,
     159,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,   142,
     143,    -1,   145,   146,   147,   148,    -1,   150,   151,   152,
     113,    -1,   155,   156,   157,    -1,   159,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,   142,
     143,    -1,   145,   146,   147,   148,    -1,   150,   151,   152,
      10,    -1,   155,   156,   157,    -1,   159,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    63,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      28,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      78,   131,   132,   133,    82,   135,   136,   137,    -1,   139,
     140,   141,   142,   143,    -1,   145,   146,   147,   148,    -1,
     150,   151,   152,    10,   154,   155,   156,   157,    -1,    -1,
      17,   109,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,   142,   143,    -1,   145,   146,   147,
     148,    -1,   150,   151,   152,    10,    63,   155,   156,   157,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    28,    29,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    78,   131,   132,   133,    82,   135,   136,
     137,    -1,   139,   140,   141,   142,   143,    -1,   145,   146,
     147,   148,    -1,   150,   151,   152,    -1,    10,   155,   156,
     157,    -1,    -1,    -1,    17,    -1,    -1,    -1,   113,    78,
      -1,    -1,    -1,    82,    -1,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,   133,    42,
     135,   136,   137,    -1,   139,   140,   141,   142,   143,    -1,
     145,   146,   147,   148,   113,   150,   151,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,   133,    78,   135,   136,   137,    82,
     139,   140,   141,   142,   143,    -1,   145,   146,   147,   148,
      -1,   150,   151,   152,    10,   154,   155,   156,   157,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,   142,
     143,    -1,   145,   146,   147,   148,    -1,   150,   151,   152,
      10,    -1,   155,   156,   157,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    82,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,   133,    78,   135,
     136,   137,    82,   139,   140,   141,   142,   143,    -1,   145,
     146,   147,   148,    -1,   150,   151,   152,    -1,   154,   155,
     156,   157,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,   113,    78,    -1,    -1,    -1,    82,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,   133,    -1,   135,   136,   137,    -1,   139,
     140,   141,   142,   143,    -1,   145,   146,   147,   148,   113,
     150,   151,   152,    -1,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,   133,
      78,   135,   136,   137,    82,   139,   140,   141,   142,   143,
      -1,   145,   146,   147,   148,    -1,   150,   151,   152,    10,
     154,   155,   156,   157,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,   133,    -1,   135,   136,   137,
      -1,   139,   140,   141,   142,   143,    -1,   145,   146,   147,
     148,    -1,   150,   151,   152,    10,    -1,   155,   156,   157,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    82,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,   132,   133,    78,   135,   136,   137,    82,   139,   140,
     141,   142,   143,    -1,   145,   146,   147,   148,    -1,   150,
     151,   152,    10,    -1,   155,   156,   157,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,   133,    -1,
     135,   136,   137,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,    10,    -1,
     155,   156,   157,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    82,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    10,   113,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    28,   131,   132,   133,    78,   135,   136,   137,
      82,   139,   140,   141,   142,   143,    -1,   145,   146,   147,
     148,    -1,   150,   151,   152,    10,    11,   155,   156,   157,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    82,   129,    -1,   131,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
     142,   143,    -1,   145,   146,   147,   148,    -1,   150,   151,
     152,    10,   154,   155,   156,   157,   112,   113,    17,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,    28,
      -1,    -1,    31,   129,    -1,   131,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,   142,   143,    -1,   145,
     146,   147,   148,    -1,   150,   151,   152,    -1,   113,   155,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,   133,    78,
     135,   136,   137,    82,   139,   140,   141,   142,   143,    -1,
     145,   146,   147,   148,    -1,   150,   151,   152,    10,    -1,
     155,   156,   157,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,   133,    -1,   135,   136,   137,    -1,
     139,   140,   141,   142,   143,    -1,   145,   146,   147,   148,
      -1,   150,   151,   152,    10,    -1,   155,   156,   157,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      82,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,
     132,   133,    78,   135,   136,   137,    82,   139,   140,   141,
     142,   143,    -1,   145,   146,   147,   148,    -1,   150,   151,
     152,    -1,    -1,   155,   156,   157,    10,    11,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,   113,    78,    -1,
      -1,    -1,    82,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,   129,    -1,   131,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,   142,   143,    -1,   145,
     146,   147,   148,   113,   150,   151,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,   133,    78,   135,   136,   137,    82,   139,
     140,   141,   142,   143,    -1,   145,   146,   147,   148,    -1,
     150,   151,   152,    10,    -1,   155,   156,   157,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,   142,   143,
      -1,   145,   146,   147,   148,    -1,   150,   151,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    10,    82,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,   131,   132,   133,    -1,   135,   136,
     137,    -1,   139,   140,   141,   142,   143,    -1,   145,   146,
     147,   148,    78,   150,   151,   152,    82,    -1,   155,   156,
     157,    -1,    -1,    -1,    10,    91,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,   142,   143,    -1,   145,
     146,   147,   148,    28,   150,   151,   152,    -1,    -1,   155,
     156,   157,    78,    -1,    -1,    10,    82,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,   133,    -1,   135,
     136,   137,    -1,   139,   140,   141,   142,   143,    -1,   145,
     146,   147,   148,    78,   150,   151,   152,    82,   113,   155,
     156,   157,    -1,    -1,    -1,    10,    91,    -1,    -1,    -1,
      -1,    -1,    17,    -1,   129,    -1,   131,   132,   133,    -1,
     135,   136,   137,    28,   139,   140,   141,   142,   113,    -1,
     145,   146,   147,   148,    -1,   150,   151,   152,    -1,    -1,
     155,   156,   157,    -1,   129,    -1,   131,   132,   133,    -1,
     135,   136,   137,    -1,   139,   140,   141,   142,   143,    -1,
     145,   146,   147,   148,    28,   150,   151,   152,    -1,    -1,
     155,   156,   157,    78,    -1,    -1,    10,    82,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,   133,    -1,
     135,   136,   137,    -1,   139,   140,   141,   142,   143,    -1,
     145,   146,   147,   148,    78,   150,   151,   152,    82,   113,
     155,   156,   157,    -1,    -1,    -1,    10,    91,    -1,    -1,
      -1,    -1,    -1,    17,    -1,   129,    -1,   131,   132,   133,
      -1,   135,   136,   137,    28,   139,   140,   141,   142,   113,
      -1,   145,   146,   147,   148,    -1,   150,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,   129,    -1,   131,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,   142,   143,
      -1,   145,   146,   147,   148,    -1,   150,   151,   152,    -1,
      -1,   155,   156,   157,    78,    -1,    -1,    10,    82,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,   133,
      -1,   135,   136,   137,    -1,   139,   140,   141,   142,   143,
      -1,   145,   146,   147,   148,    78,   150,   151,   152,    82,
      -1,   155,   156,   157,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,    28,   139,   140,   141,   142,
     143,    -1,   145,   146,   147,   148,    -1,   150,   151,   152,
      -1,    -1,   155,   156,   157,    78,    -1,    -1,    10,    82,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,   142,
     143,    -1,   145,   146,   147,   148,    78,   150,   151,   152,
      82,    -1,   155,   156,   157,    -1,   129,    -1,   131,   132,
     133,    -1,   135,   136,   137,    -1,   139,   140,   141,   142,
      -1,    -1,   145,   146,   147,   148,    -1,   150,    -1,   152,
      -1,   113,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,
     132,   133,    -1,   135,   136,   137,    -1,   139,   140,   141,
     142,   143,    -1,   145,   146,   147,   148,    -1,   150,   151,
     152,    -1,    -1,   155,   156,   157,     1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,     3,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    15,    16,    -1,
      -1,    76,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      85,    86,    30,    88,    -1,    90,    -1,    92,    36,    -1,
      95,    -1,    -1,    41,    99,    43,    -1,   102,    46,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      88,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    99,    -1,    -1,   102,    -1,    -1,   105
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
     137,   147,   149,   151,   152,   154,   157,   159,   161,   163,
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
       1,   160,   171,   179,   267,   100,   138,   243,   269,   270,
     292,   201,   267,   290,   300,    70,   173,   159,    74,   177,
     188,   188,    47,    73,   238,   255,   154,   144,    10,    17,
      28,    78,    82,   113,   129,   131,   132,   133,   135,   136,
     137,   139,   140,   141,   142,   143,   145,   146,   147,   148,
     150,   151,   152,   155,   156,   157,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   158,   230,   161,   163,    78,    82,   161,   173,   154,
     290,   290,   290,   267,   159,   178,    42,   280,   251,   255,
     154,   134,   154,   108,   189,   243,   271,   272,   273,   292,
     251,   213,   253,   188,   161,   194,    29,    42,   194,   109,
     194,   283,    29,    42,   194,   283,   291,    33,    63,   154,
      91,   178,   230,   271,   173,   161,   232,   100,   161,   188,
     257,   258,     1,   133,   262,    42,   134,   173,   194,   194,
     271,   188,   134,   154,   290,   290,   154,   159,   194,   161,
     271,   154,   205,   154,   205,   154,   114,   252,   154,   154,
      33,    63,   194,   154,   160,   160,   171,   134,   160,   290,
     134,   162,   134,   162,   164,   283,    42,   134,   164,   283,
     112,   134,   164,     7,     1,   160,   179,    56,   133,   214,
     159,   225,   154,   290,   290,   290,   290,   201,   290,   201,
     290,   290,   290,   290,   290,    16,   102,   290,   290,    30,
      54,    98,   189,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   292,
     292,   292,   292,   292,   271,   271,   201,   290,   201,   290,
      24,    42,    79,   104,   282,   285,   286,   290,   305,    29,
      42,    29,    42,    91,    42,   164,   201,   290,   194,   154,
     154,   290,   290,   114,   162,   134,   154,   188,   255,    42,
     267,    40,   290,   201,   290,   161,   194,    40,   290,   201,
     290,   194,   157,   180,   182,   290,   180,   181,   173,   290,
      31,   290,   162,    24,    42,    45,    65,    68,    79,    98,
     172,   233,   234,   235,   236,   222,   258,   134,   162,    16,
      30,    43,    67,    85,    86,    90,   102,   163,   192,   263,
     275,   114,   259,   290,   256,   162,   243,   290,     1,   210,
     271,   162,    18,   206,   263,   275,   134,   182,   181,   160,
     162,   162,   269,   162,   269,   173,   164,   201,   290,   164,
     173,   290,   164,   290,   164,   290,   160,   160,   188,   267,
     159,     1,   188,   218,   219,    24,    68,    79,    98,   227,
     237,   154,   154,   154,   154,   154,   162,   164,    42,    79,
     134,   162,   274,    78,    78,    40,   201,   290,    40,   201,
     290,   201,   290,   280,   280,   154,   243,   292,   273,   214,
     154,   290,   162,   290,    29,   194,    29,   194,   284,   285,
     290,    29,   194,   283,    29,   194,   283,   134,   154,    11,
     154,    31,    31,   173,    91,   178,    42,    79,   235,   134,
     162,   161,   188,    24,    68,    79,    98,   239,   162,   258,
     262,     1,   267,    59,   292,   160,    64,   106,   160,   211,
     162,   161,   178,   188,   207,   251,   154,   154,   173,   164,
     283,   164,   283,   173,   112,     1,   215,   160,   114,   134,
     160,    79,   226,     1,     3,    10,    15,    17,    22,    36,
      41,    46,    48,    54,    61,    76,    88,    92,    95,    99,
     105,   129,   130,   131,   132,   135,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   152,   155,
     156,   157,   158,   161,   187,   190,   228,   229,   230,   274,
     285,   262,   274,   274,   290,    29,    29,   290,    29,    29,
     164,   164,   194,   194,   159,   194,    91,    40,   290,    40,
     290,   134,   162,    91,    40,   290,   194,    40,   290,   194,
     290,   290,   173,   290,   173,    31,   188,   234,   258,   133,
     266,    79,   262,   259,   164,    42,   164,    29,   173,   267,
     207,   133,   178,    40,   173,   290,   164,    40,   173,   290,
     164,   290,   160,    35,    72,    75,   160,   172,   196,   290,
     219,   239,   161,   231,   188,   290,   136,   231,   231,   259,
      91,    40,    40,    91,    40,    40,    40,    40,     1,   215,
     290,   290,   290,   285,   290,   290,   290,    11,    31,   173,
     266,   162,   163,   192,   243,   265,   275,   138,   245,   259,
      52,   107,   246,   290,   263,   275,   173,   194,   162,   290,
     290,   173,   290,   173,   196,     1,   133,   261,   233,   162,
       3,    92,   229,   230,   290,   290,   290,   290,   290,   290,
     160,   160,    91,    91,    91,    91,   290,   173,   259,   266,
     164,   267,   243,   244,   290,     3,    81,    92,   247,   248,
     249,   290,   178,   195,   242,   164,   178,    91,    91,   163,
     192,   260,   275,    94,   240,   162,   231,   231,    91,    91,
      91,    91,    91,    91,   290,   290,   290,   290,   245,   259,
     243,   264,   265,   275,    42,   164,   249,   107,   134,   114,
     139,   141,   142,   145,   146,    52,   275,   290,   290,     1,
     164,   267,   246,   290,   264,   265,   290,   248,   249,   249,
     249,   249,   249,   249,   247,   164,   260,   275,   164,   154,
     241,   242,   164,   260,   275,   264
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
     229,   229,   229,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   231,   231,   232,   233,   233,
     233,   234,   234,   234,   234,   234,   235,   235,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   237,   237,   237,
     237,   237,   237,   238,   238,   239,   239,   239,   239,   239,
     239,   240,   240,   241,   241,   242,   242,   243,   243,   244,
     244,   245,   245,   246,   246,   246,   246,   246,   247,   247,
     248,   248,   248,   248,   248,   248,   248,   249,   249,   250,
     250,   250,   251,   251,   252,   252,   252,   253,   253,   253,
     253,   253,   254,   254,   254,   255,   255,   256,   256,   257,
     257,   257,   258,   258,   258,   259,   259,   259,   260,   260,
     260,   260,   260,   260,   260,   261,   261,   261,   261,   261,
     262,   262,   262,   262,   262,   263,   263,   263,   263,   264,
     264,   264,   265,   265,   265,   265,   265,   266,   266,   266,
     266,   266,   267,   267,   267,   267,   268,   268,   269,   269,
     269,   270,   270,   271,   271,   272,   272,   273,   273,   273,
     273,   274,   274,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   277,   278,   279,   279,
     279,   279,   279,   279,   279,   279,   280,   280,   281,   282,
     282,   283,   284,   284,   285,   285,   285,   286,   286,   286,
     286,   286,   286,   287,   287,   288,   289,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   291,   291,   292,   292,   292,   293,   293,
     293,   293,   294,   294,   295,   295,   295,   296,   296,   296,
     296,   297,   297,   297,   297,   298,   298,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     300,   300,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   302,   302,   302,   302,   302,
     302,   303,   303,   303,   303,   304,   304,   304,   304,   305,
     305,   305,   305,   305,   305,   305
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     3,     3,     0,     1,
       3,     4,     5,     4,     6,     6,     0,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     1,     0,     1,     2,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     2,     1,     0,
       1,     2,     2,     0,     2,     2,     4,     4,     1,     3,
       3,     3,     3,     3,     3,     3,     2,     1,     1,     3,
       4,     4,     2,     4,     0,     2,     2,     1,     1,     1,
       2,     1,     4,     5,     3,     1,     3,     3,     5,     1,
       1,     3,     1,     2,     3,     0,     2,     2,     3,     2,
       4,     3,     3,     4,     3,     0,     2,     2,     2,     1,
       0,     2,     2,     2,     1,     4,     4,     6,     3,     0,
       1,     1,     3,     4,     3,     4,     6,     0,     2,     2,
       2,     2,     1,     1,     3,     3,     1,     3,     1,     1,
       1,     3,     3,     0,     1,     1,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     4,
       4,     4,     5,     2,     2,     2,     2,     2,     2,     6,
       6,     4,     9,     9,     7,     6,     6,     4,     9,     9,
       7,     4,     6,     6,     9,     9,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     1,     4,     1,
       3,     4,     1,     3,     4,     3,     3,     1,     1,     2,
       1,     2,     1,     1,     3,     2,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     3,     3,     3,
       2,     2,     1,     0,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     4,     3,     3,     3,
       3,     3,     4,     3,     4,     1,     1,     1,     1,     1,
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
#line 5655 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 3:
#line 492 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5661 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 4:
#line 493 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5667 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 6:
#line 500 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5673 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 7:
#line 505 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5679 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 8:
#line 506 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5685 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 23:
#line 525 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5691 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 24:
#line 526 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5697 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 25:
#line 527 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5703 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 26:
#line 528 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5709 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 27:
#line 529 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5715 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 28:
#line 530 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5721 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 29:
#line 531 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5727 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 30:
#line 532 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5733 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 31:
#line 533 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5739 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 32:
#line 534 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5745 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 33:
#line 535 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5751 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 34:
#line 536 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5757 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 35:
#line 537 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5763 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 36:
#line 538 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5769 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 37:
#line 539 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5775 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 38:
#line 540 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5781 "bison-chapel.cpp" /* yacc.c:1666  */
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
#line 5795 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 40:
#line 558 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 5804 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 41:
#line 563 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 5813 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 42:
#line 568 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 5822 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 43:
#line 575 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5828 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 44:
#line 576 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5834 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 45:
#line 577 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5840 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 46:
#line 581 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; }
#line 5846 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 47:
#line 582 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true;  }
#line 5852 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 48:
#line 595 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5858 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 49:
#line 596 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5864 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 50:
#line 597 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5870 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 51:
#line 602 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5876 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 52:
#line 603 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5882 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 53:
#line 608 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5892 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 54:
#line 613 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5902 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 55:
#line 618 "chapel.ypp" /* yacc.c:1666  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5911 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 56:
#line 622 "chapel.ypp" /* yacc.c:1666  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5920 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 57:
#line 629 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5930 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 59:
#line 638 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5940 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 61:
#line 647 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5946 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 62:
#line 648 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5952 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 63:
#line 649 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5958 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 64:
#line 653 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5964 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 65:
#line 658 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 5970 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 66:
#line 660 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5976 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 67:
#line 662 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5982 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 68:
#line 664 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5988 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 69:
#line 666 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5994 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 70:
#line 672 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = NULL; }
#line 6000 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 71:
#line 673 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6006 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 72:
#line 677 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6012 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 73:
#line 678 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "this"; }
#line 6018 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 74:
#line 679 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6024 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 75:
#line 680 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6030 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 76:
#line 681 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6036 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 77:
#line 684 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6042 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 78:
#line 685 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6048 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 79:
#line 686 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6054 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 80:
#line 687 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6060 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 81:
#line 688 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6066 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 82:
#line 700 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6072 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 83:
#line 701 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "this"; }
#line 6078 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 84:
#line 712 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "bool"; }
#line 6084 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 85:
#line 713 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "int"; }
#line 6090 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 86:
#line 714 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "uint"; }
#line 6096 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 87:
#line 715 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "real"; }
#line 6102 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 88:
#line 716 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "imag"; }
#line 6108 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 89:
#line 717 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "complex"; }
#line 6114 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 90:
#line 718 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "string"; }
#line 6120 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 91:
#line 719 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "sync"; }
#line 6126 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 92:
#line 720 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "single"; }
#line 6132 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 93:
#line 721 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "owned"; }
#line 6138 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 94:
#line 722 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "shared"; }
#line 6144 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 95:
#line 723 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "borrowed"; }
#line 6150 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 96:
#line 724 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "unmanaged"; }
#line 6156 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 97:
#line 725 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "domain"; }
#line 6162 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 98:
#line 726 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "index"; }
#line 6168 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 99:
#line 727 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "locale"; }
#line 6174 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 100:
#line 728 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "nothing"; }
#line 6180 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 101:
#line 729 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "void"; }
#line 6186 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 102:
#line 733 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6192 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 103:
#line 734 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6198 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 104:
#line 735 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6204 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 105:
#line 736 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6210 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 106:
#line 737 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6216 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 107:
#line 738 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6222 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 108:
#line 739 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6228 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 109:
#line 740 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6234 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 110:
#line 741 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 6240 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 111:
#line 742 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 6246 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 112:
#line 749 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_syncvar"; }
#line 6252 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 113:
#line 750 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_singlevar"; }
#line 6258 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 114:
#line 751 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_owned"; }
#line 6264 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 115:
#line 752 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_shared"; }
#line 6270 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 116:
#line 753 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_borrowed"; }
#line 6276 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 117:
#line 754 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_unmanaged"; }
#line 6282 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 118:
#line 755 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_domain"; }
#line 6288 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 119:
#line 756 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_index"; }
#line 6294 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 120:
#line 761 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = NULL; }
#line 6300 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 121:
#line 762 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6306 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 122:
#line 766 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6312 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 123:
#line 767 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6318 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 124:
#line 771 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6324 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 125:
#line 772 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6330 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 126:
#line 776 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6336 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 128:
#line 778 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6342 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 129:
#line 779 "chapel.ypp" /* yacc.c:1666  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6348 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 136:
#line 792 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6354 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 137:
#line 793 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6360 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 138:
#line 794 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6366 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 139:
#line 795 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6372 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 140:
#line 800 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6380 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 141:
#line 806 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6386 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 142:
#line 807 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6392 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 143:
#line 808 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 6398 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 144:
#line 809 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6404 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 145:
#line 810 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 6410 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 146:
#line 811 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6416 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 147:
#line 812 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6422 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 148:
#line 813 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6428 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 149:
#line 814 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6434 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 150:
#line 815 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6440 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 151:
#line 816 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6446 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 152:
#line 817 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6452 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 153:
#line 818 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6458 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 154:
#line 819 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6464 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 155:
#line 820 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6470 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 156:
#line 821 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6476 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 157:
#line 822 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6482 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 158:
#line 823 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6488 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 159:
#line 825 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6498 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 160:
#line 831 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6508 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 161:
#line 837 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6518 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 162:
#line 843 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6528 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 163:
#line 849 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6539 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 164:
#line 856 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6550 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 165:
#line 863 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6558 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 166:
#line 867 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6566 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 167:
#line 873 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 6572 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 168:
#line 877 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6578 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 169:
#line 878 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6584 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 170:
#line 879 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6590 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 171:
#line 880 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6596 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 172:
#line 881 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6603 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 173:
#line 883 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6610 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 174:
#line 885 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6617 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 175:
#line 887 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6624 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 176:
#line 892 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6630 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 177:
#line 895 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6636 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 178:
#line 896 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6642 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 179:
#line 897 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6648 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 180:
#line 898 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6654 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 181:
#line 899 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6660 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 182:
#line 900 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6666 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 183:
#line 904 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6672 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 184:
#line 905 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6678 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 185:
#line 909 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6684 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 186:
#line 910 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 6690 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 187:
#line 911 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 6696 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 188:
#line 915 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6702 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 189:
#line 916 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6708 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 190:
#line 920 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6714 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 191:
#line 924 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6720 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 192:
#line 926 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6726 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 193:
#line 930 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6732 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 194:
#line 931 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6738 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 195:
#line 936 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6744 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 196:
#line 938 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6750 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 197:
#line 940 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6756 "bison-chapel.cpp" /* yacc.c:1666  */
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
#line 6770 "bison-chapel.cpp" /* yacc.c:1666  */
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
#line 6784 "bison-chapel.cpp" /* yacc.c:1666  */
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
#line 6798 "bison-chapel.cpp" /* yacc.c:1666  */
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
#line 6812 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 202:
#line 989 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6822 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 203:
#line 994 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6832 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 204:
#line 999 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6842 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 205:
#line 1007 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = NULL; }
#line 6848 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 206:
#line 1008 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6854 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 207:
#line 1013 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6860 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 208:
#line 1015 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6866 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 209:
#line 1017 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6872 "bison-chapel.cpp" /* yacc.c:1666  */
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
#line 6892 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 211:
#line 1038 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6900 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 212:
#line 1045 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6910 "bison-chapel.cpp" /* yacc.c:1666  */
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
#line 6922 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 214:
#line 1062 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6930 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 215:
#line 1066 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6938 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 216:
#line 1072 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6944 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 217:
#line 1073 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6950 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 218:
#line 1078 "chapel.ypp" /* yacc.c:1666  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6959 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 219:
#line 1083 "chapel.ypp" /* yacc.c:1666  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6968 "bison-chapel.cpp" /* yacc.c:1666  */
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
#line 6990 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 221:
#line 1110 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7001 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 222:
#line 1116 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7013 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 223:
#line 1123 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7024 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 224:
#line 1129 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7035 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 225:
#line 1135 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7047 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 226:
#line 1146 "chapel.ypp" /* yacc.c:1666  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 7057 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 227:
#line 1152 "chapel.ypp" /* yacc.c:1666  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7068 "bison-chapel.cpp" /* yacc.c:1666  */
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
#line 7095 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 229:
#line 1185 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 7103 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 230:
#line 1189 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7112 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 231:
#line 1194 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 7120 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 232:
#line 1198 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7129 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 233:
#line 1203 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 7137 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 235:
#line 1210 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7143 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 236:
#line 1214 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7149 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 237:
#line 1215 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 7155 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 238:
#line 1216 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "&"; }
#line 7161 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 239:
#line 1217 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "|"; }
#line 7167 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 240:
#line 1218 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "^"; }
#line 7173 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 241:
#line 1219 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "~"; }
#line 7179 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 242:
#line 1220 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "=="; }
#line 7185 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 243:
#line 1221 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "!="; }
#line 7191 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 244:
#line 1222 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<="; }
#line 7197 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 245:
#line 1223 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">="; }
#line 7203 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 246:
#line 1224 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<"; }
#line 7209 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 247:
#line 1225 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">"; }
#line 7215 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 248:
#line 1226 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "+"; }
#line 7221 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 249:
#line 1227 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "-"; }
#line 7227 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 250:
#line 1228 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "*"; }
#line 7233 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 251:
#line 1229 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "/"; }
#line 7239 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 252:
#line 1230 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<<"; }
#line 7245 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 253:
#line 1231 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">>"; }
#line 7251 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 254:
#line 1232 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "%"; }
#line 7257 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 255:
#line 1233 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "**"; }
#line 7263 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 256:
#line 1234 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "!"; }
#line 7269 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 257:
#line 1235 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "chpl_by"; }
#line 7275 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 258:
#line 1236 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "#"; }
#line 7281 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 259:
#line 1237 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "chpl_align"; }
#line 7287 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 260:
#line 1238 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<=>"; }
#line 7293 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 261:
#line 1239 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<~>"; }
#line 7299 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 262:
#line 1240 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "init="; }
#line 7305 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 263:
#line 1244 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "="; }
#line 7311 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 264:
#line 1245 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "+="; }
#line 7317 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 265:
#line 1246 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "-="; }
#line 7323 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 266:
#line 1247 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "*="; }
#line 7329 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 267:
#line 1248 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "/="; }
#line 7335 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 268:
#line 1249 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "%="; }
#line 7341 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 269:
#line 1250 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "**="; }
#line 7347 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 270:
#line 1251 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "&="; }
#line 7353 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 271:
#line 1252 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "|="; }
#line 7359 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 272:
#line 1253 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "^="; }
#line 7365 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 273:
#line 1254 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">>="; }
#line 7371 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 274:
#line 1255 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<<="; }
#line 7377 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 275:
#line 1259 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7383 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 276:
#line 1260 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7389 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 277:
#line 1264 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7395 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 278:
#line 1268 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7401 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 279:
#line 1269 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7407 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 280:
#line 1270 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7413 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 281:
#line 1275 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7419 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 282:
#line 1277 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7425 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 283:
#line 1279 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7431 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 284:
#line 1281 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7437 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 285:
#line 1283 "chapel.ypp" /* yacc.c:1666  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7443 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 286:
#line 1287 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7449 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 287:
#line 1288 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7455 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 288:
#line 1292 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_IN; }
#line 7461 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 289:
#line 1293 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7467 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 290:
#line 1294 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_OUT; }
#line 7473 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 291:
#line 1295 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST; }
#line 7479 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 292:
#line 1296 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7485 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 293:
#line 1297 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7491 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 294:
#line 1298 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7497 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 295:
#line 1299 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_REF; }
#line 7503 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 296:
#line 1300 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7509 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 297:
#line 1304 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7515 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 298:
#line 1305 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7521 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 299:
#line 1306 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_REF;   }
#line 7527 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 300:
#line 1307 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 7533 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 301:
#line 1308 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST;   }
#line 7539 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 302:
#line 1309 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 7545 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 303:
#line 1313 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 7551 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 304:
#line 1314 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 7557 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 305:
#line 1318 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_VALUE; }
#line 7563 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 306:
#line 1319 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_VALUE; }
#line 7569 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 307:
#line 1320 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 7575 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 308:
#line 1321 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_REF; }
#line 7581 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 309:
#line 1322 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_PARAM; }
#line 7587 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 310:
#line 1323 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_TYPE; }
#line 7593 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 311:
#line 1327 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; }
#line 7599 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 312:
#line 1328 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true;  }
#line 7605 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 313:
#line 1331 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = NULL; }
#line 7611 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 316:
#line 1337 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7617 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 317:
#line 1342 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7623 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 318:
#line 1344 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7629 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 319:
#line 1348 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7635 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 321:
#line 1353 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7641 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 322:
#line 1354 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7647 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 323:
#line 1358 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 7653 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 324:
#line 1360 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 7659 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 325:
#line 1362 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 7665 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 326:
#line 1364 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7671 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 327:
#line 1366 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7677 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 328:
#line 1370 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7683 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 329:
#line 1371 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7689 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 330:
#line 1374 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7695 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 331:
#line 1375 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7701 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 332:
#line 1376 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7707 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 333:
#line 1377 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7713 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 334:
#line 1378 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7719 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 335:
#line 1379 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7725 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 336:
#line 1380 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 7731 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 337:
#line 1383 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 7737 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 338:
#line 1384 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 7743 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 339:
#line 1388 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7749 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 340:
#line 1390 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7755 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 341:
#line 1392 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7761 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 342:
#line 1397 "chapel.ypp" /* yacc.c:1666  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7778 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 343:
#line 1410 "chapel.ypp" /* yacc.c:1666  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7796 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 344:
#line 1426 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7802 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 345:
#line 1428 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7808 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 346:
#line 1430 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7814 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 347:
#line 1434 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 7820 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 348:
#line 1435 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 7826 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 349:
#line 1436 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 7832 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 350:
#line 1437 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 7838 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 351:
#line 1438 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 7844 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 352:
#line 1443 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7854 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 353:
#line 1449 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
      context->latestComment = NULL;
    }
#line 7864 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 354:
#line 1455 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7873 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 356:
#line 1464 "chapel.ypp" /* yacc.c:1666  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7882 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 357:
#line 1472 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7888 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 358:
#line 1474 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7894 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 359:
#line 1479 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7900 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 360:
#line 1481 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7906 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 361:
#line 1483 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7912 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 362:
#line 1488 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7918 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 363:
#line 1490 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7924 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 364:
#line 1492 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7930 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 365:
#line 1498 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7936 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 366:
#line 1499 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7942 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 367:
#line 1500 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7948 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 368:
#line 1506 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7954 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 369:
#line 1508 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7960 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 370:
#line 1510 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7968 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 371:
#line 1514 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7976 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 372:
#line 1518 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7982 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 373:
#line 1520 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7990 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 374:
#line 1524 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7998 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 375:
#line 1531 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8004 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 376:
#line 1532 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8010 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 377:
#line 1533 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8016 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 378:
#line 1534 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8022 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 379:
#line 1535 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8028 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 380:
#line 1540 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8034 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 381:
#line 1541 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8040 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 382:
#line 1542 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 8046 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 383:
#line 1543 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8052 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 384:
#line 1544 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8058 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 385:
#line 1565 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8066 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 386:
#line 1569 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 8074 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 387:
#line 1573 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 8086 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 388:
#line 1581 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8094 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 389:
#line 1587 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8100 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 390:
#line 1588 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8106 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 391:
#line 1589 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8112 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 392:
#line 1594 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8118 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 393:
#line 1596 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8124 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 394:
#line 1602 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8130 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 395:
#line 1604 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8136 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 396:
#line 1606 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 8142 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 397:
#line 1610 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8148 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 398:
#line 1611 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8154 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 399:
#line 1612 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8160 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 400:
#line 1613 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8166 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 401:
#line 1614 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8172 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 402:
#line 1620 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8178 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 403:
#line 1621 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8184 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 404:
#line 1622 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8190 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 405:
#line 1623 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8196 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 406:
#line 1627 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 8202 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 407:
#line 1628 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8208 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 408:
#line 1632 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 8214 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 409:
#line 1633 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8220 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 410:
#line 1634 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8226 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 411:
#line 1638 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8232 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 412:
#line 1639 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8238 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 413:
#line 1643 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 8244 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 415:
#line 1648 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8250 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 416:
#line 1649 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8256 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 417:
#line 1653 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 8262 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 418:
#line 1654 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8268 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 419:
#line 1655 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8274 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 420:
#line 1656 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8280 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 421:
#line 1660 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8286 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 422:
#line 1661 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8292 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 428:
#line 1678 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 8298 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 429:
#line 1680 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 8304 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 430:
#line 1682 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 8310 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 431:
#line 1684 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 8316 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 432:
#line 1686 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 8322 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 433:
#line 1688 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 8328 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 434:
#line 1690 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 8334 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 435:
#line 1692 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 8340 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 436:
#line 1694 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8346 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 437:
#line 1696 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 8352 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 438:
#line 1698 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8358 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 439:
#line 1703 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8364 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 440:
#line 1705 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8370 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 441:
#line 1707 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8376 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 442:
#line 1709 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8382 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 443:
#line 1711 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8388 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 444:
#line 1713 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8394 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 445:
#line 1715 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8400 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 446:
#line 1717 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8406 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 447:
#line 1719 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8412 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 448:
#line 1721 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8418 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 449:
#line 1723 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8424 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 450:
#line 1725 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8430 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 451:
#line 1727 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8441 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 452:
#line 1734 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8451 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 453:
#line 1740 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8461 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 454:
#line 1746 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8471 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 455:
#line 1752 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 8481 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 456:
#line 1761 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8487 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 457:
#line 1770 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 8493 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 465:
#line 1786 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8499 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 466:
#line 1790 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = NULL; }
#line 8505 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 468:
#line 1795 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8511 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 469:
#line 1799 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8517 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 470:
#line 1800 "chapel.ypp" /* yacc.c:1666  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8523 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 471:
#line 1804 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8529 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 472:
#line 1808 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8535 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 473:
#line 1809 "chapel.ypp" /* yacc.c:1666  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8541 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 474:
#line 1814 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 8549 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 475:
#line 1818 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8557 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 476:
#line 1822 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8565 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 477:
#line 1828 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 8571 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 478:
#line 1829 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8577 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 479:
#line 1830 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8583 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 480:
#line 1831 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8589 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 481:
#line 1832 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8595 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 482:
#line 1833 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8601 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 484:
#line 1839 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8607 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 485:
#line 1844 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8613 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 486:
#line 1849 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8619 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 495:
#line 1865 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8625 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 496:
#line 1867 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8631 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 497:
#line 1869 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 8637 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 498:
#line 1871 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8643 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 499:
#line 1873 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8649 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 500:
#line 1875 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8655 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 501:
#line 1877 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8661 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 502:
#line 1879 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8667 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 503:
#line 1883 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8673 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 504:
#line 1884 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8679 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 505:
#line 1887 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8685 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 506:
#line 1888 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8691 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 507:
#line 1889 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8697 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 514:
#line 1911 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8703 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 515:
#line 1912 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8709 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 516:
#line 1913 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8715 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 517:
#line 1917 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8721 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 518:
#line 1918 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8727 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 519:
#line 1919 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8733 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 520:
#line 1920 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 8739 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 521:
#line 1928 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8745 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 522:
#line 1929 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8751 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 523:
#line 1930 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8757 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 524:
#line 1931 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8763 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 525:
#line 1935 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 8769 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 526:
#line 1936 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 8775 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 528:
#line 1941 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 8781 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 529:
#line 1942 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8787 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 530:
#line 1943 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8793 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 531:
#line 1944 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8799 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 532:
#line 1945 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8805 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 533:
#line 1946 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 8811 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 534:
#line 1947 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8817 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 535:
#line 1948 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8823 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 536:
#line 1949 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8829 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 537:
#line 1950 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8835 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 538:
#line 1952 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8843 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 539:
#line 1956 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8851 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 540:
#line 1963 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8857 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 541:
#line 1964 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8863 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 542:
#line 1968 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8869 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 543:
#line 1969 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8875 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 544:
#line 1970 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8881 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 545:
#line 1971 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8887 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 546:
#line 1972 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8893 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 547:
#line 1973 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8899 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 548:
#line 1974 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8905 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 549:
#line 1975 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8911 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 550:
#line 1976 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8917 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 551:
#line 1977 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8923 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 552:
#line 1978 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8929 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 553:
#line 1979 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8935 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 554:
#line 1980 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8941 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 555:
#line 1981 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8947 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 556:
#line 1982 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8953 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 557:
#line 1983 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8959 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 558:
#line 1984 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8965 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 559:
#line 1985 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8971 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 560:
#line 1986 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8977 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 561:
#line 1987 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8983 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 562:
#line 1988 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8989 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 563:
#line 1989 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8995 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 564:
#line 1990 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9001 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 565:
#line 1994 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 9007 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 566:
#line 1995 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 9013 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 567:
#line 1996 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 9019 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 568:
#line 1997 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 9025 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 569:
#line 1998 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 9031 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 570:
#line 1999 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 9037 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 571:
#line 2003 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9043 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 572:
#line 2004 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9049 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 573:
#line 2005 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9055 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 574:
#line 2006 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9061 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 575:
#line 2010 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9067 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 576:
#line 2011 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9073 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 577:
#line 2012 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9079 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 578:
#line 2013 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9085 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 579:
#line 2018 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 9091 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 580:
#line 2019 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 9097 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 581:
#line 2020 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 9103 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 582:
#line 2021 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 9109 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 583:
#line 2022 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 9115 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 584:
#line 2023 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 9121 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 585:
#line 2024 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 9127 "bison-chapel.cpp" /* yacc.c:1666  */
    break;


#line 9131 "bison-chapel.cpp" /* yacc.c:1666  */
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
