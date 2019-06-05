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
#define YYLAST   16497

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  168
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  139
/* YYNRULES -- Number of rules.  */
#define YYNRULES  590
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1047

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
       0,   488,   488,   493,   494,   500,   501,   506,   507,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,   545,
     558,   563,   568,   576,   577,   578,   582,   583,   596,   597,
     598,   603,   604,   609,   614,   619,   623,   630,   635,   639,
     644,   648,   649,   650,   654,   658,   660,   662,   664,   666,
     673,   674,   678,   679,   680,   681,   682,   683,   686,   687,
     688,   689,   690,   691,   703,   704,   715,   716,   717,   718,
     719,   720,   721,   722,   723,   724,   725,   726,   727,   728,
     729,   730,   731,   732,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   752,   753,   758,   759,   764,   765,
     769,   770,   774,   775,   779,   780,   781,   782,   783,   784,
     785,   786,   790,   791,   795,   796,   797,   798,   802,   809,
     810,   811,   812,   813,   814,   815,   816,   817,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   833,   839,
     845,   851,   858,   865,   869,   876,   880,   881,   882,   883,
     884,   886,   888,   890,   895,   898,   899,   900,   901,   902,
     903,   907,   908,   912,   913,   914,   918,   919,   923,   926,
     928,   933,   934,   938,   940,   942,   949,   959,   969,   979,
     992,   997,  1002,  1010,  1011,  1016,  1017,  1019,  1024,  1040,
    1047,  1056,  1064,  1068,  1075,  1076,  1081,  1086,  1080,  1113,
    1119,  1126,  1132,  1138,  1149,  1155,  1148,  1187,  1191,  1196,
    1200,  1205,  1212,  1213,  1217,  1218,  1219,  1220,  1221,  1222,
    1223,  1224,  1225,  1226,  1227,  1228,  1229,  1230,  1231,  1232,
    1233,  1234,  1235,  1236,  1237,  1238,  1239,  1240,  1241,  1242,
    1243,  1244,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,
    1256,  1257,  1258,  1259,  1263,  1264,  1268,  1272,  1273,  1274,
    1278,  1280,  1282,  1284,  1286,  1291,  1292,  1296,  1297,  1298,
    1299,  1300,  1301,  1302,  1303,  1304,  1308,  1309,  1310,  1311,
    1312,  1313,  1317,  1318,  1322,  1323,  1324,  1325,  1326,  1327,
    1331,  1332,  1335,  1336,  1340,  1341,  1345,  1347,  1352,  1353,
    1357,  1358,  1362,  1363,  1365,  1367,  1369,  1374,  1375,  1378,
    1379,  1380,  1381,  1382,  1383,  1384,  1387,  1388,  1391,  1393,
    1395,  1400,  1413,  1430,  1431,  1433,  1438,  1439,  1440,  1441,
    1442,  1446,  1452,  1458,  1466,  1467,  1475,  1477,  1482,  1484,
    1486,  1491,  1493,  1495,  1502,  1503,  1504,  1509,  1511,  1513,
    1517,  1521,  1523,  1527,  1535,  1536,  1537,  1538,  1539,  1544,
    1545,  1546,  1547,  1548,  1568,  1572,  1576,  1584,  1591,  1592,
    1593,  1597,  1599,  1605,  1607,  1609,  1614,  1615,  1616,  1617,
    1618,  1624,  1625,  1626,  1627,  1631,  1632,  1636,  1637,  1638,
    1642,  1643,  1647,  1648,  1652,  1653,  1657,  1658,  1659,  1660,
    1664,  1665,  1676,  1678,  1683,  1684,  1685,  1686,  1687,  1688,
    1690,  1692,  1694,  1696,  1698,  1700,  1703,  1705,  1707,  1709,
    1711,  1713,  1715,  1717,  1722,  1724,  1726,  1728,  1730,  1732,
    1734,  1736,  1738,  1740,  1742,  1744,  1746,  1753,  1759,  1765,
    1771,  1780,  1790,  1798,  1799,  1800,  1801,  1802,  1803,  1804,
    1805,  1810,  1811,  1815,  1819,  1820,  1824,  1828,  1829,  1833,
    1837,  1841,  1848,  1849,  1850,  1851,  1852,  1853,  1857,  1858,
    1863,  1868,  1876,  1877,  1878,  1879,  1880,  1881,  1882,  1883,
    1884,  1886,  1888,  1890,  1892,  1894,  1899,  1900,  1903,  1904,
    1905,  1908,  1909,  1910,  1911,  1922,  1923,  1924,  1928,  1929,
    1930,  1934,  1935,  1936,  1937,  1945,  1946,  1947,  1948,  1952,
    1953,  1957,  1958,  1959,  1960,  1961,  1962,  1963,  1964,  1965,
    1966,  1967,  1968,  1972,  1980,  1981,  1985,  1986,  1987,  1988,
    1989,  1990,  1991,  1992,  1993,  1994,  1995,  1996,  1997,  1998,
    1999,  2000,  2001,  2002,  2003,  2004,  2005,  2006,  2007,  2011,
    2012,  2013,  2014,  2015,  2016,  2017,  2021,  2022,  2023,  2024,
    2028,  2029,  2030,  2031,  2036,  2037,  2038,  2039,  2040,  2041,
    2042
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
     415,   416,   417,   418,   419,   420,   421,   422
};
# endif

#define YYPACT_NINF -902

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-902)))

#define YYTABLE_NINF -541

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -902,    93,  2707,  -902,   -96,  -902,  -902,  -902,  -902,  -902,
    -902,  4159,    14,   129,  -902, 12044,  -902,    14,  9596,  -902,
     343,   118,   129,  4159,  9596,  4159,    57,  -902,  9596,  6698,
    -902,  8078,  8676,  6537,  9596,  -902,    67,  -902,  -902, 16392,
    -902, 12353,  8814,  -902,  9596,  9596,  -902,  -902,  -902,  9596,
    -902, 12044,  -902,  9596,   226,   135,  1677,  1677,  -902,  -902,
    -902,  8952,  7618,  9596,  8814, 12044,  9596,   252,  -902,   191,
    4159,  -902,  9596,  -902,  9734,  9734, 16392,  -902,  -902, 12044,
    8952,  -902,  -902,  9596,  9596,  -902,  9596,  -902,  9596,  -902,
    -902, 11666,  9596,  -902,  9596,  -902,  -902,  3193,  6836,  8216,
    -902,  3998,  -902,   194,  -902,   288,  -902,  -902,  -902,  -902,
    -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,
    -902,  -902,  -902,  -902, 16392,  -902, 16392,   216,   207,  -902,
    -902, 12353,  -902,   219,  -902,   227,  -902,  -902,   231,   260,
     275,   274,   280, 16050,  1969,    25,   281,   292,  -902,  -902,
    -902,  -902,  -902,  -902,  -902,   355,  -902,  -902, 16050,   293,
    4159,  -902,  -902,   308,  9596,  9596,  9596,  9596,  9596,  8952,
    8952,   236,  -902,  -902,  -902,  -902,   328,   254,  -902,  -902,
     320, 14248, 16392, 12353,  -902,   333,  -902,   114, 16050,   383,
    7779, 16050,  -902,   158,  -902, 16392,   161,   197, 16392,   332,
      10, 13767,     3, 13726,   197,  9596,  -902, 14151, 12971,  7779,
    -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,
    -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,
    -902,  -902,  -902,  4159,  -902,   334,  2172,    75,     5,  -902,
    4159,  -902,  -902, 13910,   360,   328, 13910,   328,   360,  -902,
    7779,  9596,  -902,  -902, 16392,  -902,   116, 16050,  9596,  9596,
    -902, 16050,   344, 14098,  -902, 13910,   328, 16050,   338,  7779,
    -902, 16050, 14484,  -902,  -902, 14541, 11663,  -902,  -902, 14627,
     386,   348,   328,   146, 14301, 13910, 14684,   589,   589,  1279,
     360,   360,   128,  -902,  -902,  3354,   131,  -902,  9596,  -902,
     -38,   -31,  -902,   -30,    43, 14770,    92,   497,  -902,  3515,
    -902,   449,   372,   349,  -902,  -902,  -902,   148,  -902,  -902,
    9596,  9596,  9596,  9596,  8676,  8676,  9596,   309,  9596,  9596,
    9596,  9596,  9596,   201, 11666,  9596,  9596,  9596,  9596,  9596,
    9596,  9596,  9596,  9596,  9596,  9596,  9596,  9596,  9596,  9596,
    -902,  -902,  -902,  -902,  -902,  -902,  8377,  8377,  -902,  -902,
    -902,  -902,  8377,  -902,  -902,  8377,  8377,  7779,  7779,  8676,
    8676,  7319,  -902,  -902, 14398, 14444, 14724,    49,  3676,  -902,
    8676,    10,   353,   151,  -902,  9596,  -902,  9596,   396,  -902,
     351,   388,  -902,  -902,   364, 16392, 12353,   483,  8952,  -902,
    4320,  8676,  -902,   366,  -902,    10,  4481,  8676,  -902,    10,
     197,  9872,  9596,  -902,  4159,   495,  9596,   375,  -902,    50,
    -902,  -902,  2172,  -902,   403,   380,  -902, 10010,   433,  9596,
   12353,  -902,  -902,  -902,   389,  -902,  8952,  -902, 16050, 16050,
    -902,    55,  -902,  7779,   392,  -902,   530,  -902,   530,  -902,
   10148,   420,  -902,  -902,  9872,  9596,  -902,  -902,  -902,  -902,
    -902,  8515,  -902, 12805,  6997,  -902,  7158,  -902,  4159,   393,
    8676,  5930,  3032,   397,  9596,  6254,  -902,  -902,   259,  -902,
    3837, 16392,  8952,   400, 16325,   206,  -902, 14867, 13865, 13865,
     315,  -902,   315,  -902,   315, 14581,  1313,  1375,  1466,   328,
     589,  -902,  -902,  -902,  -902,  1279, 15241,   315,   628,   628,
   13865,   628,   628,  1217,   589, 15241, 14000,  1217,   360,   360,
     589,   406,   408,   409,   410,   415,   412,   411,  -902,   315,
    -902,   315,    28,  -902,  -902,  -902,   102,  -902,  1558, 16099,
     395, 10286,  8676, 10424,  8676,  9596,  8676, 12272,    14, 14920,
    -902,  -902,  -902, 16050, 14960,  7779,  -902,  7779,  -902,   372,
     228,  9596,   107,  9596, 16050,    16, 13955,  7319,  -902,  9596,
   16050,     7, 13812,  -902,   416,   437,   423, 15017,   437,   424,
     549, 15103,  4159, 14053,  -902,    68,  -902,  -902,  -902,  -902,
    -902,  -902,   831,   181,  -902, 12431,  -902,   210,   425,  2172,
      75,    57,    67,  9596,  9596,  5769,  -902,  -902,   472,  7940,
    -902, 16050,  -902,  -902,  -902, 16050,   429,    64,   428,  -902,
    2465,  -902,  -902,   230, 16392,   430,   442,  -902,  -902,  -902,
    -902,  -902,  -902,  -902,  4159,   -13, 12568,  -902,  -902, 16050,
    4159, 16050,  -902, 15160,  -902,  -902,  -902,   447,  1100,   431,
     485,   212,  -902,   518,  -902,  -902,  -902,  -902,  6378,  -902,
    -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,  7319,  -902,
      29,  8676,  8676,  9596,   571, 15192,  9596,   572, 15335,   438,
   12655,    10,    10,  -902,  -902,  -902,  -902,   444,  -902, 13910,
    -902, 13051,  4642,  -902,  4803,  -902,   187,  -902, 13127,  4964,
    -902,    10,  5125,  -902,    10,  9596,  -902,  9596,  -902,  4159,
    9596,  -902,  4159,   573,  -902,  -902, 16392,   853,  -902,  2172,
     476,   527,  -902,  -902,  -902,    61,  -902,  -902,   433,   455,
      58,  -902,  -902,  -902,  5286,  8952,  -902,  -902,  -902, 16392,
    -902,   486,   320,  -902,  -902,  -902,  -902,  5447,   458,  5608,
     460,  -902,  9596,   465,  2539,  -902,  9596, 16392,  -902,  -902,
     210,   466,   249,  -902,   491,  -902,   492,   506,   499,   500,
    -902,   501,   510,   503,   505,   508,   279,   514,   509,   512,
    -902,  -902, 16392,  -902,  -902,  -902,  -902,  -902,  -902,  -902,
    -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,
    -902,  -902,  9596,  -902,   522,   523,   517,   466,   466,  -902,
    -902,   433,   204,   209, 15303, 10562, 10700, 15446, 10838, 10976,
   11114, 11252,  -902,  -902,  2305,  -902,  4159,  9596, 16050,  9596,
   16050,  7319,  -902,  4159,  9596, 16050,  -902,  9596, 16050,  -902,
   15478, 16050,  -902, 16050,   616,  4159,   476,  -902,   496,  9113,
     159,  -902,   156,  -902,  -902,  8676, 12158,  4159,  -902,     6,
     502,  9596,  -902,  9596,  -902, 16050,  4159,  9596,  -902, 16050,
    4159, 16050,  -902,  7480,   824,   824,  -902, 16248,  -902, 16050,
    -902,   138,    50,  -902,  -902, 12895,  -902, 12512,  -902,  -902,
    -902,  9596,  9596,  9596,  9596,  9596,  9596,  9596,  9596,   513,
   16234, 15103, 13203, 13279,  -902, 15103, 13355, 13431,  9596,  4159,
    -902,   433,   476,  6092,  -902,  -902,  -902,   122,  8952,  -902,
    -902,    54,  9596,   -10, 12712,  -902,   563,   383,  -902,   320,
   16050, 13507,  -902, 13583,  -902,  -902,  -902, 11390,   565,   213,
    -902,   534,   539,   466,   466, 15589, 15621, 15732, 15764, 15875,
   15907,  -902,  -902,  4159,  4159,  4159,  4159, 16050,  -902,  -902,
     159,  9274,    79,  -902,  -902, 16050,  -902,    46,  -902,   -29,
    -902,   284, 16018,  -902,  -902,  -902, 11252,  -902,  4159,  4159,
    2868,  -902,  -902,   329,  -902,   156,  -902,  -902,  -902,  9596,
    9596,  9596,  9596,  9596,  9596, 15103, 15103, 15103, 15103,  -902,
    -902,  -902,  -902,  -902,   132,  8676, 11800,  -902,  9596,    54,
      46,    46,    46,    46,    46,    46,    54,   852, 15103, 15103,
     511, 11528,    88,    17, 12752,  -902,  -902, 16050,  -902,  -902,
    -902,  -902,  -902,  -902,  -902,   537,  -902,  -902,   422, 11914,
    -902,  -902,  -902,  9435,  -902,   614,  -902
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    84,   532,   533,   534,   535,
     536,     0,   471,    70,   104,   442,   200,   471,     0,   109,
       0,   347,    70,     0,     0,     0,     0,   210,   506,   506,
     529,     0,     0,     0,     0,   108,     0,   220,   105,     0,
     216,     0,     0,   111,     0,     0,   462,   537,   112,     0,
     223,   436,   346,     0,     0,     0,    45,    44,   107,   201,
     348,     0,     0,     0,     0,   440,     0,     0,   110,     0,
       0,    85,     0,   530,     0,     0,     0,   106,   202,   438,
       0,   350,   113,     0,     0,   586,     0,   588,     0,   589,
     590,   505,     0,   587,   584,   124,   585,     0,     0,     0,
       4,     0,     5,     0,     9,    46,    10,    11,    12,    14,
     420,   421,    22,    13,   125,   131,    15,    17,    16,    19,
      20,    21,    18,   130,     0,   128,     0,   497,     0,   132,
     129,     0,   133,   511,   493,   422,   494,   426,   424,     0,
       0,   498,   499,     0,   425,     0,   512,   513,   514,   531,
     492,   428,   427,   495,   496,     0,    38,    24,   434,     0,
       0,   472,    71,     0,     0,     0,     0,     0,     0,     0,
       0,   497,   511,   424,   498,   499,   443,   425,   512,   513,
       0,   471,     0,     0,   349,     0,   174,     0,   405,     0,
     412,   507,   221,   535,   138,     0,     0,   222,     0,     0,
       0,     0,     0,     0,     0,   506,   137,     0,     0,   412,
      78,    86,    97,    91,    99,    81,    90,   100,    87,   101,
      79,   102,    95,    89,    96,    94,    92,    93,    80,    82,
      88,    98,   103,     0,    83,     0,     0,     0,     0,   354,
       0,   121,    32,     0,   571,   490,     0,   437,   572,     7,
     412,   506,   127,   126,   317,   402,     0,   401,     0,     0,
     122,   510,     0,     0,    35,     0,   441,   429,     0,   412,
      36,   435,     0,   181,   177,     0,   425,   181,   178,     0,
     343,     0,   439,     0,   401,     0,     0,   573,   575,   504,
     570,   569,     0,    48,    51,     0,     0,   407,     0,   409,
       0,     0,   408,     0,     0,   401,     0,     0,     6,     0,
      47,     0,   203,     0,   303,   302,   224,     0,   423,    23,
       0,     0,     0,     0,     0,     0,     0,   574,     0,     0,
       0,     0,     0,     0,   503,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,   269,   270,   271,   266,   268,     0,     0,   264,   267,
     265,   263,     0,   273,   272,     0,     0,   412,   412,     0,
       0,     0,    25,    26,     0,     0,     0,     0,     0,    27,
       0,     0,     0,     0,    28,     0,    29,     0,   420,   418,
       0,   413,   414,   419,     0,     0,     0,     0,     0,   147,
       0,     0,   146,     0,   155,     0,     0,     0,   153,     0,
       0,     0,    57,   134,     0,   167,     0,     0,    30,   285,
     217,   358,     0,   359,   361,     0,   383,     0,   364,     0,
       0,   120,    31,    33,     0,   316,     0,    64,   508,   509,
     123,     0,    34,   412,     0,   188,   179,   175,   180,   176,
       0,   341,   338,    61,     0,    57,   140,    37,    50,    49,
      52,     0,   538,     0,     0,   525,     0,   527,     0,     0,
       0,     0,     0,     0,     0,     0,   542,     8,     0,    40,
       0,     0,     0,     0,     0,   296,   353,   470,   566,   565,
     568,   577,   576,   581,   580,   562,   559,   560,   561,   501,
     549,   523,   524,   522,   521,   502,   553,   564,   558,   556,
     567,   557,   555,   547,   552,   554,   563,   546,   550,   551,
     548,     0,     0,     0,     0,     0,     0,     0,   579,   578,
     583,   582,   482,   483,   485,   487,     0,   474,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   540,   471,   471,
     143,   339,   351,   406,     0,     0,   431,     0,   340,   203,
       0,     0,     0,     0,   446,     0,     0,     0,   156,     0,
     452,     0,     0,   154,   585,    60,     0,    53,    58,     0,
     166,     0,     0,     0,   430,   290,   287,   288,   289,   293,
     294,   295,   285,     0,   278,     0,   286,   304,     0,   362,
       0,   116,   117,   115,   114,     0,   382,   381,   493,     0,
     356,   491,   355,   520,   404,   403,     0,     0,     0,   432,
       0,   182,   345,   493,     0,     0,     0,   539,   500,   526,
     410,   528,   411,   163,     0,     0,     0,   541,   161,   456,
       0,   544,   543,     0,    42,    41,    39,   204,     0,     0,
     214,     0,   211,   300,   297,   298,   301,   225,     0,    68,
      69,    67,    66,    65,   518,   519,   484,   486,     0,   473,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   139,   416,   417,   415,     0,   352,     0,
     165,     0,     0,   145,     0,   144,     0,   477,     0,     0,
     151,     0,     0,   149,     0,     0,   135,     0,   136,     0,
       0,   169,     0,   171,   291,   292,     0,   285,   276,     0,
     396,   305,   308,   307,   309,     0,   360,   363,   364,     0,
       0,   365,   366,   190,     0,     0,   189,   192,   433,     0,
     183,   186,     0,   342,    62,    63,   164,     0,     0,     0,
       0,   162,     0,     0,   219,   209,     0,   212,   208,   299,
     304,   274,    72,   257,    86,   255,    91,    75,    90,    87,
     260,   101,    73,   102,    89,    92,    74,    76,    88,   103,
     254,   236,   239,   237,   238,   249,   240,   253,   245,   243,
     256,   259,   244,   242,   247,   252,   241,   246,   250,   251,
     248,   258,     0,   234,     0,    77,     0,   274,   274,   232,
     475,   364,   511,   511,     0,     0,     0,     0,     0,     0,
       0,     0,   142,   141,     0,   148,     0,     0,   445,     0,
     444,     0,   476,     0,     0,   451,   152,     0,   450,   150,
      55,    54,   168,   461,   170,     0,   396,   279,     0,     0,
     364,   306,   322,   357,   387,     0,   540,     0,   194,     0,
       0,     0,   184,     0,   159,   458,     0,     0,   157,   457,
       0,   545,   198,   506,   219,   219,   196,   219,   206,   215,
     213,     0,   285,   231,   235,     0,   261,     0,   227,   228,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     219,   449,     0,     0,   478,   455,     0,     0,     0,     0,
     173,   364,   396,     0,   399,   398,   400,   493,   318,   282,
     280,     0,     0,     0,     0,   385,   493,   195,   193,     0,
     187,     0,   160,     0,   158,   207,   378,     0,   310,     0,
     233,    72,    74,   274,   274,     0,     0,     0,     0,     0,
       0,   199,   197,     0,     0,     0,     0,    56,   172,   281,
     364,   388,     0,   319,   321,   320,   336,     0,   337,   324,
     327,     0,   323,   314,   315,   218,     0,   185,     0,     0,
       0,   377,   376,   493,   311,   322,   275,   229,   230,     0,
       0,     0,     0,     0,     0,   448,   447,   454,   453,   284,
     283,   390,   391,   393,   493,     0,   540,   335,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   493,   460,   459,
       0,   368,     0,     0,     0,   392,   394,   326,   328,   329,
     332,   333,   334,   330,   331,   325,   373,   371,   493,   540,
     312,   226,   313,   388,   372,   493,   395
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -902,  -902,  -902,     0,  -414,  1956,  -902,  -902,  -902,  -902,
     365,   370,  -310,   214,   233,  -902,  -902,    34,   658,  -902,
    1057,  -166,  -643,  -902,  -755,  -902,  1326,  -879,  -717,   -45,
    -902,  -902,  -902,   142,  -902,  -902,  -902,   405,  -902,   -54,
    -902,  -902,  -902,  -902,  -902,   493,   133,  -127,  -902,  -902,
    -902,   -55,   731,  -902,  -902,  -902,  -902,  -902,  -902,  -902,
    -902,  -184,  -143,  -718,  -902,  -170,    -4,   124,  -902,  -902,
    -902,   -46,  -902,  -902,  -304,   152,  -902,  -240,  -256,  -284,
    -275,  -175,  -902,  -179,  -902,    -1,   702,  -117,   307,  -902,
    -412,  -700,  -671,  -902,  -533,  -444,  -790,  -901,  -798,   -34,
    -902,    26,  -902,  -168,  -902,   182,   381,  -328,  -902,  -902,
    -902,  1083,  -902,    -9,  -902,  -902,  -178,  -902,  -541,  -902,
    -902,  1165,  1465,   -11,     1,   119,   630,  -902,  1667,  1723,
    -902,  -902,  -902,  -902,  -902,  -902,  -902,  -902,  -362
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   294,   101,   580,   103,   104,   105,   311,
     106,   295,   575,   579,   576,   107,   108,   109,   163,   803,
     237,   110,   234,   111,   606,   196,   242,   112,   113,   114,
     115,   116,   117,   679,   118,   119,   120,   446,   621,   742,
     121,   122,   617,   737,   123,   124,   483,   754,   125,   126,
     651,   652,   171,   235,   597,   128,   129,   485,   760,   657,
     806,   807,   416,   883,   420,   593,   594,   595,   596,   658,
     316,   725,   985,  1041,   975,   255,   964,   919,   923,   969,
     970,   971,   130,   281,   451,   131,   132,   238,   239,   424,
     425,   610,   982,   938,   428,   607,  1002,   916,   850,   296,
     187,   300,   301,   390,   391,   392,   172,   134,   135,   136,
     137,   173,   139,   160,   161,   536,   405,   696,   537,   538,
     140,   174,   175,   143,   197,   393,   177,   145,   178,   179,
     148,   149,   150,   306,   151,   152,   153,   154,   155
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     158,   366,   100,   382,   176,   592,   622,   181,   180,   540,
     598,   252,   253,   188,   317,   805,   394,   191,   191,   183,
     201,   203,   207,   208,   388,   409,   697,   256,   853,   192,
     426,   243,   240,   244,   245,   240,   699,   878,   246,   240,
     247,   417,   248,   388,   974,   692,   283,   429,   911,   966,
     257,   261,   263,   265,   266,   267,   616,   966,   156,   271,
    1003,   272,   426,   275,   279,   304,   383,   728,   282,   284,
     666,    62,   285,   286,   585,   287,   426,   288,  1008,   403,
     289,   290,   434,   291,   388,   470,   257,   261,   305,   888,
     889,   546,   586,     3,   914,   587,   403,   464,    62,   608,
     855,   444,   578,   388,   466,  1026,  1009,   667,   274,   278,
     714,   890,   403,  -379,   960,   588,   403,  -379,   589,  -191,
      54,  1005,   623,   159,   465,   469,   473,   810,   734,   590,
     546,   467,   162,   366,   468,   967,   377,   811,   968,   936,
     430,   436,  -379,  -379,   974,   578,   968,   715,   591,   378,
     920,   747,   403,   158,   374,   375,   376,   271,   257,   305,
     935,  -191,   378,   427,  -379,   378,   378,   504,  -379,   378,
     735,  1040,   381,   200,   202,   378,   378,  -119,   471,   261,
      16,   262,   981,   878,   471,  -277,   367,   727,   368,  -379,
    -374,  -379,   852,   471,   191,   427,   396,   184,   261,   526,
     527,   388,   388,   183,     5,   540,   410,   472,   921,   427,
    -379,   959,  -277,   547,   471,  -191,  1025,   302,   190,  -374,
    -379,    21,   856,   471,   736,   987,   988,   475,   209,  -379,
     653,   501,  -374,   249,   721,  -119,  -397,   668,    59,   261,
     191,   303,   436,  1006,   805,  -374,  -389,   438,   439,   385,
     299,   436,  1039,  1046,   314,   502,   476,  -397,   261,  -119,
    1000,  -397,    78,   922,   669,    52,   461,  -389,   386,   690,
     437,  -389,   937,   609,   654,   618,    60,   388,   722,   560,
     315,   436,   156,   430,  -397,   655,   430,   463,   458,   723,
     904,   462,  -374,    71,  -389,   460,   250,  -374,   918,   503,
     453,    81,   486,   592,   656,   552,   540,   848,   724,   487,
     488,   489,   490,   492,   494,   495,   717,   496,   497,   498,
     499,   500,   831,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,  -481,
     877,   268,   389,   718,  -480,   261,   261,   757,   717,   832,
    1037,   261,   269,   309,   261,   261,   261,   261,   529,   531,
     539,   389,   310,   430,   562,  -344,  -481,    21,  1044,   549,
    -467,  -480,   758,  -464,   553,   986,   554,  -517,   -78,  -517,
     318,  -375,   688,   133,  -344,  -463,   -84,   257,   323,   564,
     566,   388,   133,   701,   704,   570,   572,  -517,  1010,  -517,
     577,   577,   389,   581,   133,   583,   133,   241,   -80,   396,
    -375,    52,   925,   156,   319,  -515,   -85,  -515,   611,   644,
     320,   389,    60,  -375,  1011,   615,  1012,  1013,  -468,   241,
    1014,  1015,   261,   369,  -469,  -466,  -375,   370,   324,   273,
     277,   182,   325,   577,   577,   743,  -465,    81,   647,   331,
     615,   133,   333,   261,   371,   261,   336,   748,   750,   636,
     615,   639,   373,   641,   643,   333,   491,   493,   592,   540,
    -516,   257,  -516,   672,  -367,   521,   522,   370,   133,   378,
     460,   523,   133,  -375,   524,   525,   877,   384,  -375,   327,
     630,   387,   632,   398,   331,   419,   332,   333,   440,   443,
     450,   336,   452,  -367,   477,   481,   482,   551,   484,   343,
     555,   528,   530,   556,  -380,   808,  -367,   349,   558,   389,
     389,   917,   548,   557,  -380,   561,   582,   567,   926,  -367,
     564,   675,   570,   678,   581,   680,   639,   584,   599,   681,
     682,   133,   600,   565,   261,   379,   261,   609,   620,   571,
     689,   613,   691,  -380,   619,   624,   539,   634,   698,   648,
     659,   640,   660,   661,   662,   241,   241,   241,   241,   663,
     -59,   730,   705,   415,   664,   665,  -367,   706,   708,  -380,
     709,  -367,   436,   302,   744,   302,  -380,   726,   614,   733,
     738,   755,   267,   271,   305,   389,   745,   759,   261,   756,
     815,   818,   820,   824,   845,  -384,   851,  -380,   241,   983,
     849,   241,   635,   614,   133,  -384,   299,   323,   299,   854,
     861,   133,   866,   614,   870,   872,  -380,   882,  -104,  -109,
     241,  -380,   144,  1004,  -380,   -81,  -108,  -105,  -111,   -79,
    -112,   144,  -107,   -82,  -384,  -110,  -106,   909,  1017,  -113,
     241,   886,   -83,   144,   887,   144,   323,   539,   912,   984,
     492,   529,   814,   -78,   929,   817,  -369,   324,   -80,   626,
    -384,   325,  1009,   951,   685,  1036,   133,  -384,  1004,   480,
     185,   828,   448,   830,   674,   860,   677,   625,   835,   395,
     133,   838,   687,  1038,   840,  -369,   841,   900,  -384,   843,
     144,   859,   880,   943,   276,   276,   324,   684,  -369,   389,
     325,  1045,   939,   847,   881,  1004,   716,  -384,   327,  1042,
     999,  -369,  -384,   331,   257,  -384,   333,   144,   732,  1023,
     336,   144,  1035,   127,  1028,   206,   865,   612,   869,   686,
       0,   871,   127,     0,   944,   879,   241,     0,     0,     0,
       0,     0,     0,     0,   127,     0,   127,   327,   328,   133,
     329,   330,   331,     0,   332,   333,   334,     0,  -369,   336,
     241,     0,     0,  -369,   241,     0,   342,   343,     0,     0,
     346,   133,     0,   347,   348,   349,     0,   133,     0,     0,
     144,   885,  1007,     0,     0,   133,     0,     0,     0,     0,
       0,   127,     0,     0,   828,   830,     0,   835,   838,   865,
     869,     0,     0,   491,   528,   901,   902,     0,   903,     0,
     539,     0,   905,   906,     0,     0,   907,     0,   127,   252,
     253,     0,   127,     0,     0,  1029,  1030,  1031,  1032,  1033,
    1034,     0,     0,     0,   924,   639,     0,   204,    21,   133,
     930,     0,   931,   133,     0,   585,   933,     0,    28,   251,
       0,   133,   191,   144,     0,     0,     0,     0,    37,     0,
     144,     0,     0,   586,     0,     0,   587,   585,     0,   962,
     901,   945,   946,   905,   947,   948,   949,   950,     0,     0,
      50,   127,    52,     0,  -386,   586,   588,   957,   587,   589,
       0,   307,   305,    60,  -386,     0,     0,   965,     0,     0,
     590,   972,     0,     0,     0,     0,     0,     0,   588,   670,
       0,   589,     0,    54,     0,   144,     0,     0,    81,   591,
     241,   241,   590,  -386,     0,     0,   241,   241,     0,   144,
       0,     0,   995,   996,   997,   998,  1022,     0,   713,     0,
       0,   591,     0,     0,     0,     0,     0,     0,     0,  -386,
       0,     0,     0,   133,   127,   869,  -386,  1018,  1019,   305,
       0,   127,     0,     0,     0,     0,     0,     0,   995,   996,
     997,   998,  1018,  1019,     0,   740,     0,  -386,     0,     0,
       0,     0,     0,     0,  1024,   639,     0,  1027,     0,     0,
       0,   915,     0,     0,     0,     0,  -386,     0,   144,     0,
       0,  -386,     0,     0,  -386,   133,     0,     0,     0,     0,
       0,   133,     0,     0,     0,     0,   127,     0,   639,     0,
     144,     0,   869,     0,     0,     0,   144,     0,     0,   809,
     127,     0,     0,     0,   144,     0,   241,   241,     0,     0,
       0,     0,   812,   813,   241,     0,   415,     0,     0,     0,
       0,     0,     0,   415,     0,     0,   241,     0,     0,   241,
     963,     0,     0,   133,     0,   133,     0,     0,     0,     0,
     133,     0,     0,   133,     0,   138,     0,     0,     0,     0,
     133,     0,     0,   133,   138,     0,   233,     0,   144,     0,
       0,   753,   144,     0,     0,     0,   138,   862,   138,   127,
     144,     0,     0,  1001,     0,   133,     0,     0,     0,  -205,
       0,     0,     0,  -205,  -205,     0,     0,     0,   133,     0,
     133,   127,  -205,   280,  -205,  -205,     0,   127,     0,  -205,
       0,     0,     0,     0,  -205,   127,     0,  -205,     0,     0,
       0,     0,     0,   138,     0,     0,     0,     0,  1001,     0,
       0,     0,     0,     0,     0,     0,  -205,   141,  -205,     0,
    -205,     0,  -205,  -205,     0,  -205,   141,  -205,     0,  -205,
     138,   312,     0,   313,   138,     0,     0,     0,   141,     0,
     141,     0,     0,     0,     0,  1001,     0,     0,  -205,   127,
       0,  -205,     0,   127,  -205,     0,     0,   133,     0,     0,
       0,   127,   144,     0,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   241,     0,   133,     0,     0,     0,
       0,     0,     0,     0,     0,   141,     0,     0,   133,   280,
       0,     0,     0,   138,     0,   323,     0,   133,     0,     0,
       0,   133,   280,     0,  -205,   397,     0,     0,     0,     0,
    -205,     0,   141,     0,   144,     0,   141,   415,   415,     0,
     144,   415,   415,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   973,     0,
     133,     0,     0,   423,   977,   324,   415,     0,   415,   325,
       0,     0,     0,     0,     0,     0,     0,   323,     0,     0,
       0,   435,     0,   127,     0,     0,   138,     0,     0,     0,
       0,     0,   144,   138,   144,   141,     0,     0,     0,   144,
       0,     0,   144,     0,   133,   133,   133,   133,     0,   144,
       0,   323,   144,     0,     0,     0,   327,   328,     0,   329,
     330,   331,     0,   332,   333,     0,     0,   324,   336,   133,
     133,   325,     0,     0,   144,   127,   343,     0,     0,     0,
       0,   127,   347,   348,   349,     0,     0,   144,   138,   144,
       0,     0,     0,     0,     0,     0,     0,     0,   973,     0,
     264,   324,   138,     0,     0,   325,     0,     0,   141,     0,
       0,     0,     0,   323,     0,   141,     0,     0,   327,   328,
       0,   329,   330,   331,     0,   332,   333,     0,     0,     0,
     336,     0,     0,   127,     0,   127,     0,   342,   343,     0,
     127,   346,     0,   127,   347,   348,   349,     0,     0,     0,
     127,     0,   327,   127,     0,     0,     0,   331,     0,   332,
     333,     0,   559,   324,   336,     0,   144,   325,     0,     0,
     141,   138,   343,   144,     0,   127,     0,   142,   347,   348,
     349,     0,     0,     0,   141,   144,   142,     0,   127,   423,
     127,     0,     0,   138,     0,     0,     0,   144,   142,   138,
     142,     0,     0,     0,   323,     0,   144,   138,     0,     0,
     144,     0,     0,     0,   327,   328,     0,     0,   330,   331,
       0,   332,   333,     0,     0,     0,   336,     0,     0,     0,
       0,     0,     0,     0,   343,     0,   399,   402,   404,   408,
     347,   348,   349,     0,     0,   142,     0,     0,   646,   144,
       0,   650,     0,   141,   324,     0,     0,     0,   325,     0,
       0,   138,     0,     0,     0,   138,     0,   127,     0,     0,
       0,     5,   142,   138,   127,   141,   142,     0,     0,   432,
       0,   141,   433,    14,     0,     0,   127,     0,     0,   141,
      19,     0,     0,   144,   144,   144,   144,     0,   127,     0,
       0,   442,     0,     0,     0,   327,   328,   127,     0,    35,
     331,   127,   332,   333,    38,     0,     0,   336,   144,   144,
       0,   456,    43,     0,     0,   343,     0,     0,     0,    48,
       0,   347,   348,   349,     0,   142,     0,     0,     0,     0,
       0,     0,     0,   141,    58,     0,     0,   141,     0,     0,
     127,     0,     0,     0,     0,   141,    68,     0,     0,     0,
      71,     0,   720,     0,     0,     0,   423,    77,     0,     0,
       0,     0,     0,    82,     0,   138,     0,     0,     0,   146,
       0,     0,     0,     0,     0,     0,     0,   741,   146,     0,
       0,   280,     0,     0,   127,   127,   127,   127,     0,     0,
     146,     0,   146,     0,     0,     0,     0,     0,   142,     0,
     204,    21,     0,     0,     0,   142,     0,   550,     0,   127,
     127,    28,   251,     0,     0,   804,     0,   138,     0,     0,
       0,    37,     0,   138,  -219,   147,     0,     0,     0,     0,
       0,   568,     0,     0,   147,   573,     0,   146,     0,     0,
       0,     0,     0,    50,     0,    52,   147,   141,   147,     0,
    -219,     0,     0,     0,     0,     0,    60,     0,     0,     0,
     142,     0,     0,     0,   146,     0,     0,     0,   146,     0,
       0,     0,     0,   846,   142,   138,   423,   138,     0,     0,
       0,    81,   138,     0,     0,   138,     0,     0,     0,     0,
       0,     0,   138,   147,     0,   138,   741,     0,     0,   141,
       0,     0,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,     0,   650,     0,     0,   138,     0,     0,
     147,     0,     0,     0,   147,     0,     0,   146,     0,     0,
     138,     0,   138,     0,     0,     0,     0,     0,     0,   884,
       0,     0,     0,   142,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   141,     0,   141,
       0,     0,     0,     0,   141,   142,     0,   141,     0,     0,
       0,   142,     0,     0,   141,     0,     0,   141,     0,   142,
       0,     0,     0,   147,     0,     0,     0,     0,     0,     0,
       0,   693,   695,     0,     0,     0,     0,   700,   703,   141,
     146,     0,     0,     0,     0,     0,     0,   146,     0,   138,
       0,     0,   141,     0,   141,     0,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   138,     0,
       0,     0,     0,   142,     0,     0,     0,   142,     0,     0,
     138,     0,     0,     0,   804,   142,     0,     0,     0,   138,
       0,     0,     0,   138,     0,     0,   147,     0,   102,     0,
       0,     0,   146,   147,     0,     0,     0,   157,     0,     0,
       0,     0,     0,     0,     0,     0,   146,     0,     0,   186,
       0,   189,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   141,   138,     0,     0,     0,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,   822,   823,     0,
     141,     0,     0,     0,     0,   825,     0,     0,   147,     0,
       0,     0,   141,     0,     0,     0,   270,   836,     0,     0,
     839,   141,   147,     0,     0,   141,   138,   138,   138,   138,
       0,     0,     0,     0,     0,   146,     0,   142,     0,     0,
       0,     0,     0,   102,     0,     0,     0,   308,     0,     0,
       0,   138,   138,     0,     0,     0,     0,   146,     0,     0,
       0,     0,     0,   146,   141,     0,     0,     0,     0,     0,
       0,   146,     0,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,     0,   142,
       0,   147,     0,     0,     0,   142,     0,     0,     0,     0,
       0,     0,     0,     0,  -488,     0,   372,     0,   141,   141,
     141,   141,     0,   147,     0,     0,     0,   365,     0,   147,
    -515,     0,  -515,     0,     0,   146,     0,   147,     0,   146,
       0,     0,     0,   141,   141,     0,     0,   146,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   142,     0,   142,
       0,     0,     0,     0,   142,     0,     0,   142,     0,     0,
       0,     0,     0,     0,   142,   210,     0,   142,     0,     0,
       0,     0,     0,     0,     0,   928,     0,   211,   212,   418,
       0,   147,     0,     0,   213,   147,   431,     0,     0,   142,
       0,     0,   214,   147,     0,     0,     0,     0,   215,     0,
       0,     0,   142,   216,   142,   217,     0,     0,   218,     0,
       0,     0,     0,     0,     0,     0,   219,     0,     0,     0,
       0,     0,   220,   221,     0,     0,     0,     0,     0,   222,
       0,     0,     0,     0,     0,     0,     0,     0,   223,   146,
       0,   102,     0,     0,     0,     0,     0,   224,   225,     0,
     226,     0,   227,     0,   228,   102,     0,   229,     0,     0,
       0,   230,   421,     0,   231,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   142,     0,     0,     0,     0,     0,     0,   142,     0,
       0,   146,     0,     0,     0,   147,   899,   146,     0,     0,
     142,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   142,     0,  -205,     0,     0,     0,  -205,  -205,
       0,   142,     0,   422,   102,   142,     0,  -205,     0,  -205,
    -205,     0,     0,     0,  -205,     0,     0,     0,     0,  -205,
       0,     0,  -205,     0,     0,     0,   431,   147,     0,   146,
       0,   146,   431,   147,     0,     0,   146,     0,     0,   146,
       0,  -205,     0,  -205,   142,  -205,   146,  -205,  -205,   146,
    -205,     0,  -205,     0,  -205,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   146,     0,  -205,     0,     0,  -205,     0,     0,  -205,
       0,     0,     0,     0,   146,   147,   146,   147,   142,   142,
     142,   142,   147,     0,   633,   147,     0,     0,   638,     0,
       0,     0,   147,     0,     0,   147,   102,     0,     0,     0,
       0,     0,     0,   142,   142,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   147,     0,  -205,
       0,     0,     0,     0,     0,  -205,     0,     0,   210,     0,
     147,     0,   147,     0,     0,     0,     0,     0,     0,     0,
     211,   212,     0,     0,     0,     0,     0,   213,     0,     0,
       0,     0,     0,   146,     0,   214,     0,     0,     0,     0,
     146,   215,     0,     0,     0,     0,   216,     0,   217,     0,
       0,   218,   146,     0,     0,     0,     0,     0,     0,   219,
       0,     0,     0,     0,   146,   220,   221,     0,     0,     0,
       0,     0,   222,   146,     0,     0,     0,   146,   711,     0,
       0,   223,     0,     0,     0,     0,     0,     0,     0,   147,
     224,   225,     0,   226,     0,   227,   147,   228,    16,     0,
     229,     0,    20,    21,   230,     0,     0,   231,   147,     0,
     232,    27,     0,    28,   873,     0,   146,     0,    33,     0,
     147,     0,     0,    37,     0,     0,     0,     0,     0,   147,
     746,     0,     0,   147,     0,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,    52,     0,    54,
       0,   874,     0,     0,   875,     0,    59,     0,    60,     0,
     146,   146,   146,   146,   378,     0,   739,     0,     0,     0,
       0,     0,   147,     0,     0,     0,     0,    76,     0,     0,
      78,     0,     0,    81,     0,   146,   146,     0,   431,     0,
     431,     0,     0,     0,     0,   431,     0,     0,   431,     0,
       0,     0,     0,     0,     0,   842,     0,     0,   844,     0,
       0,     0,     0,     0,     0,     0,   147,   147,   147,   147,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     858,     0,     0,    95,     0,     0,     0,     0,     0,   876,
       0,   147,   147,   864,     0,   868,     0,    -2,     4,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,    37,     0,    38,  -219,     0,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,    48,    49,
       0,     0,     0,    50,    51,    52,    53,    54,    55,    56,
    -219,   -43,    57,    58,    59,     0,    60,    61,    62,     0,
      63,    64,    65,    66,    67,    68,    69,    70,     0,    71,
      72,   910,    73,    74,    75,    76,    77,     0,    78,    79,
      80,    81,    82,   927,     0,    83,     0,    84,     0,     0,
      85,     0,   932,     0,     0,     0,   934,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,    95,     0,     0,    96,   958,    97,     0,    98,  1020,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     164,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   167,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,   254,     0,     0,     0,    96,     0,   169,     0,    98,
       0,   170,  1021,     4,     0,     5,     6,     7,     8,     9,
      10,     0,  -540,     0,    11,    12,    13,    14,    15,  -540,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    -540,    25,    26,  -540,    27,     0,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,    37,     0,    38,  -219,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,    48,    49,     0,     0,     0,    50,    51,
      52,    53,     0,    55,    56,  -219,   -43,    57,    58,    59,
    -540,    60,    61,    62,  -540,    63,    64,    65,    66,    67,
      68,    69,    70,     0,    71,    72,     0,    73,    74,    75,
      76,    77,     0,    78,    79,    80,    81,    82,     0,     0,
      83,     0,    84,     0,     0,  -540,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -540,  -540,    88,  -540,  -540,  -540,  -540,  -540,  -540,
    -540,     0,  -540,  -540,  -540,  -540,  -540,     0,  -540,  -540,
    -540,  -540,  -540,  -540,  -540,  -540,    95,  -540,  -540,  -540,
       0,    97,  -540,    98,   292,    99,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,    37,     0,    38,
    -219,     0,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,    47,    48,    49,     0,     0,     0,    50,
      51,    52,    53,    54,    55,    56,  -219,   -43,    57,    58,
      59,     0,    60,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,     0,    71,    72,     0,    73,    74,
      75,    76,    77,     0,    78,    79,    80,    81,    82,     0,
       0,    83,     0,    84,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,   254,    95,     0,     0,
      96,     0,    97,   293,    98,     4,    99,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -219,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,    48,    49,     0,     0,     0,
      50,    51,    52,    53,    54,    55,    56,  -219,   -43,    57,
      58,    59,     0,    60,    61,    62,     0,    63,    64,    65,
      66,    67,    68,    69,    70,     0,    71,    72,     0,    73,
      74,    75,    76,    77,     0,    78,    79,    80,    81,    82,
       0,     0,    83,     0,    84,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,     0,    95,     0,
       0,    96,     0,    97,   459,    98,   478,    99,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,    34,    35,     0,    36,    37,
       0,    38,  -219,     0,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,    48,    49,     0,     0,
       0,    50,    51,    52,    53,    54,    55,    56,  -219,   -43,
      57,    58,    59,     0,    60,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,     0,    71,    72,     0,
      73,    74,    75,    76,    77,     0,    78,    79,    80,    81,
      82,     0,     0,    83,     0,    84,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,     0,    95,
       0,     0,    96,     0,    97,   479,    98,   292,    99,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -219,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,    48,    49,     0,
       0,     0,    50,    51,    52,    53,    54,    55,    56,  -219,
     -43,    57,    58,    59,     0,    60,    61,    62,     0,    63,
      64,    65,    66,    67,    68,    69,    70,     0,    71,    72,
       0,    73,    74,    75,    76,    77,     0,    78,    79,    80,
      81,    82,     0,     0,    83,     0,    84,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,     0,
      95,     0,     0,    96,     0,    97,   293,    98,     4,    99,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,    37,     0,    38,  -219,     0,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,    48,    49,
       0,     0,     0,    50,    51,    52,    53,    54,    55,    56,
    -219,   -43,    57,    58,    59,     0,    60,    61,    62,     0,
      63,    64,    65,    66,    67,    68,    69,    70,     0,    71,
      72,     0,    73,    74,    75,    76,    77,     0,    78,    79,
      80,    81,    82,     0,     0,    83,     0,    84,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,    95,     0,     0,    96,     0,    97,   645,    98,     4,
      99,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -219,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,    48,
      49,     0,     0,     0,    50,    51,    52,    53,   307,    55,
      56,  -219,   -43,    57,    58,    59,     0,    60,    61,    62,
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
       0,    27,     0,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -219,     0,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
      48,    49,     0,     0,     0,    50,    51,    52,    53,     0,
      55,    56,  -219,   -43,    57,    58,    59,     0,    60,    61,
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
     563,    35,     0,    36,    37,     0,    38,  -219,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,    48,    49,     0,     0,     0,    50,    51,    52,    53,
       0,    55,    56,  -219,   -43,    57,    58,    59,     0,    60,
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
      33,   569,    35,     0,    36,    37,     0,    38,  -219,     0,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,    47,    48,    49,     0,     0,     0,    50,    51,    52,
      53,     0,    55,    56,  -219,   -43,    57,    58,    59,     0,
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
      32,    33,   827,    35,     0,    36,    37,     0,    38,  -219,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,    48,    49,     0,     0,     0,    50,    51,
      52,    53,     0,    55,    56,  -219,   -43,    57,    58,    59,
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
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,   829,    35,     0,    36,    37,     0,    38,
    -219,     0,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,    47,    48,    49,     0,     0,     0,    50,
      51,    52,    53,     0,    55,    56,  -219,   -43,    57,    58,
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
      30,    31,    32,    33,   834,    35,     0,    36,    37,     0,
      38,  -219,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,    48,    49,     0,     0,     0,
      50,    51,    52,    53,     0,    55,    56,  -219,   -43,    57,
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
      29,    30,    31,    32,    33,   837,    35,     0,    36,    37,
       0,    38,  -219,     0,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,    48,    49,     0,     0,
       0,    50,    51,    52,    53,     0,    55,    56,  -219,   -43,
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
      21,    22,    23,    24,     0,   857,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -219,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,    48,    49,     0,
       0,     0,    50,    51,    52,    53,     0,    55,    56,  -219,
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
       0,    28,    29,    30,    31,    32,    33,   863,    35,     0,
      36,    37,     0,    38,  -219,     0,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,    48,    49,
       0,     0,     0,    50,    51,    52,    53,     0,    55,    56,
    -219,   -43,    57,    58,    59,     0,    60,    61,    62,     0,
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
      27,     0,    28,    29,    30,    31,    32,    33,   867,    35,
       0,    36,    37,     0,    38,  -219,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,    48,
      49,     0,     0,     0,    50,    51,    52,    53,     0,    55,
      56,  -219,   -43,    57,    58,    59,     0,    60,    61,    62,
       0,    63,    64,    65,    66,    67,    68,    69,    70,     0,
      71,    72,     0,    73,    74,    75,    76,    77,     0,    78,
      79,    80,    81,    82,     0,     0,    83,     0,    84,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,     0,    95,     0,     0,    96,     0,    97,     0,    98,
     729,    99,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   164,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   165,   166,     0,   167,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   168,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,   254,     0,     0,     0,    96,     0,   169,     0,
      98,     0,   170,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   164,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     167,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     168,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,    93,    94,   254,     0,     0,     0,    96,     0,   169,
       0,    98,     0,   170,   637,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   164,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,   254,     0,     0,     0,    96,
       0,   169,     0,    98,     0,   170,   961,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   164,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   167,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   761,
       0,   762,     0,    86,    87,    88,    89,    90,   763,     0,
       0,     0,    91,   764,   212,   765,     0,     0,     0,     0,
     766,     0,    92,     0,     0,    93,    94,     0,   214,     0,
       0,    96,     0,   169,   767,    98,     0,   170,   642,   768,
       0,   217,     0,     0,   769,     0,   770,     0,     0,     0,
       0,     0,   771,     0,     0,     0,     0,     0,   772,   773,
       0,     0,     0,     0,     0,   222,     0,     0,     0,     0,
       0,     0,     0,     0,   774,     0,     0,     0,     0,     0,
       0,     0,     0,   224,   225,     0,   775,     0,   227,     0,
     776,     0,     0,   777,     0,     0,     0,   778,     0,     0,
     231,     0,     0,   779,     0,     0,     0,     0,     0,     0,
       0,     0,   350,   351,   352,   353,   354,   355,     0,     0,
     358,   359,   360,   361,     0,   363,   364,   780,   781,   782,
     783,   784,     0,     0,   785,     0,     0,     0,   786,   787,
     788,   789,   790,   791,   792,   793,   794,   795,   796,     0,
     797,     0,     0,   798,   799,   800,   801,     0,     0,   802,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   164,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
     204,    21,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,   205,    30,   165,   166,     0,   167,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,    52,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,    60,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   168,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,    81,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
       0,     0,     0,     0,    96,     0,   169,     0,    98,     0,
     170,     5,     6,     7,     8,   193,    10,   194,     0,     0,
     164,     0,     0,    14,    15,     0,     0,  -118,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   167,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,  -118,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,   195,    77,     0,  -118,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   164,    93,
      94,    14,    15,     0,     0,    96,     0,   169,    19,    98,
       0,   170,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   167,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,   258,   259,     0,    77,   297,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,   298,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,   254,
       0,     0,     0,    96,     0,   169,     0,    98,     0,   170,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   164,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   165,   166,     0,   167,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   168,     0,    71,
       0,     0,    73,   258,   259,     0,    77,   297,     0,    79,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,    93,    94,
     254,     0,     0,     0,    96,     0,   169,     0,    98,   629,
     170,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     164,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   167,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,   258,   259,     0,    77,   297,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,   254,     0,     0,     0,    96,     0,   169,     0,    98,
     631,   170,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   164,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,   532,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   165,   166,     0,   167,
      35,   533,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,   534,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   168,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,   535,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,     0,     0,     0,     0,    96,     0,   169,     0,
      98,     0,   170,     5,     6,     7,     8,   193,    10,     0,
       0,     0,   164,     0,     0,    14,    15,     0,     0,  -118,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     167,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,  -118,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     168,     0,    71,     0,     0,    73,     0,     0,   195,    77,
       0,  -118,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    92,     0,
     164,    93,    94,    14,    15,     0,     0,    96,     0,   169,
      19,    98,     0,   170,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   167,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,   258,   259,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,    93,
      94,     0,   260,     0,     0,    96,     0,   169,     0,    98,
       0,   170,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   164,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   165,   166,     0,   167,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   168,
       0,    71,     0,     0,    73,   258,   259,     0,    77,     0,
       0,    79,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
      93,    94,   254,     0,     0,     0,    96,     0,   169,     0,
      98,     0,   170,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   164,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     167,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,   731,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     168,     0,    71,     0,     0,    73,   258,   259,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    92,     0,
     164,    93,    94,    14,    15,     0,     0,    96,     0,   169,
      19,    98,     0,   170,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   167,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,   198,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,   199,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   164,    93,
      94,    14,    15,     0,     0,    96,     0,   169,    19,    98,
       0,   170,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   167,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,   199,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,    93,    94,   254,
       0,     0,     0,    96,     0,   169,     0,    98,     0,   170,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   164,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   165,   166,     0,   167,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   168,     0,    71,
       0,     0,    73,   258,   259,     0,    77,     0,     0,    79,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    87,    88,    89,
      90,     0,     0,     0,     0,    91,     0,     0,     5,     6,
       7,     8,     9,    10,     0,    92,     0,   164,    93,    94,
      14,    15,     0,     0,    96,     0,   169,    19,    98,     0,
     170,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   165,   166,     0,   167,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   168,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,   254,     0,
       0,     0,    96,     0,   169,   627,    98,     0,   170,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   164,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   167,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,   199,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    92,     0,   164,    93,    94,    14,
      15,     0,     0,    96,     0,   169,    19,    98,     0,   170,
       0,     0,     0,   240,    26,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   167,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   164,    93,    94,    14,    15,     0,
       0,    96,     0,    97,    19,    98,     0,   170,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,    93,    94,   254,     0,     0,     0,    96,
       0,   169,     0,    98,     0,   170,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   164,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,   601,     0,     0,     0,     0,     0,    30,
     165,   166,     0,   167,    35,     0,   602,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,   603,
      67,    68,    69,   604,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,    88,    89,    90,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,    93,    94,   254,     0,     0,     0,
      96,     0,   169,     0,    98,     0,   913,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   164,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   167,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,    93,    94,   254,     0,     0,
       0,    96,     0,   169,     0,    98,     0,   913,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   164,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   165,   166,     0,   898,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   168,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,    93,    94,   254,     0,
       0,     0,    96,     0,   169,     0,    98,     0,   170,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   164,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   167,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    92,     0,   164,    93,    94,    14,
      15,     0,     0,    96,     0,   169,    19,    98,     0,   170,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   167,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   164,    93,    94,    14,    15,     0,
       0,    96,     0,    97,    19,    98,     0,   170,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     5,     6,     7,     8,     9,    10,     0,
      92,     0,   164,    93,    94,    14,    15,     0,     0,   574,
       0,   169,    19,    98,     0,   170,     0,     0,     0,     0,
     601,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     167,    35,     0,   602,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,   603,    67,    68,    69,
     604,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    92,     0,
     164,    93,    94,    14,    15,     0,     0,    96,     0,   169,
      19,    98,     0,   605,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   167,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   164,    93,
      94,    14,    15,     0,     0,    96,     0,   169,    19,    98,
       0,   605,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   673,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    92,     0,   164,    93,    94,    14,
      15,     0,     0,    96,     0,   169,    19,    98,     0,   170,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   676,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   164,    93,    94,    14,    15,     0,
       0,    96,     0,   169,    19,    98,     0,   170,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   892,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     5,     6,     7,     8,     9,    10,     0,
      92,     0,   164,    93,    94,    14,    15,     0,     0,    96,
       0,   169,    19,    98,     0,   170,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     893,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     168,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    92,     0,
     164,    93,    94,    14,    15,     0,     0,    96,     0,   169,
      19,    98,     0,   170,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   895,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   164,    93,
      94,    14,    15,     0,     0,    96,     0,   169,    19,    98,
       0,   170,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   896,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    91,     0,     0,     5,     6,     7,
       8,     9,    10,     0,    92,     0,   164,    93,    94,    14,
      15,     0,     0,    96,     0,   169,    19,    98,     0,   170,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   897,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,     0,     0,
       0,     0,    91,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    92,     0,   164,    93,    94,    14,    15,     0,
       0,    96,     0,   169,    19,    98,     0,   170,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   898,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      91,     0,     0,     5,     6,     7,     8,     9,    10,     0,
      92,     0,   164,    93,    94,    14,    15,     0,     0,    96,
       0,   169,    19,    98,     0,   170,     0,     0,     0,     0,
     601,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     167,    35,     0,   602,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,   603,    67,    68,    69,
     604,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,     0,     0,     0,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    92,     0,
     164,    93,    94,    14,    15,     0,     0,    96,     0,   169,
      19,    98,     0,   980,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   167,    35,
       0,    36,     0,     0,    38,     0,     0,     0,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,    87,    88,
      89,    90,     0,     0,     0,     0,    91,     0,     0,     5,
       6,     7,     8,     9,    10,     0,    92,     0,   164,    93,
      94,    14,    15,     0,     0,    96,     0,     0,    19,    98,
       0,   980,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   167,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
       0,    82,     0,     0,     0,     0,     0,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,     0,     0,     0,    86,    87,    88,    89,    90,
       0,     0,     0,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   164,     0,    92,    14,    15,     0,    94,     0,
       0,   365,    19,    96,  -515,   169,  -515,    98,     0,   170,
      26,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     167,    35,     0,    36,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
     168,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,  -388,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   164,     0,     0,    14,
      15,    88,     0,     0,     0,  -388,    19,     0,     0,  -388,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   165,   166,   254,   167,    35,     0,    36,     0,   169,
      38,    98,  -388,   913,    40,    41,  -370,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,  -370,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,  -370,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,  -370,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   164,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,  -370,     0,
       0,     0,     0,  -370,    26,    98,     0,   980,     0,     0,
      30,   165,   166,     0,   167,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     164,     0,     0,    14,    15,    88,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   165,   166,     0,   167,    35,
       0,    36,     0,   169,    38,    98,     0,   170,    40,    41,
       0,     0,    43,    44,     0,    45,    46,     0,    47,    48,
       0,     0,     0,     0,     0,    51,     0,    53,     0,    55,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,   168,     0,
      71,     0,     0,    73,     0,     0,     0,    77,     0,     0,
      79,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   164,     0,     0,    14,    15,    88,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,   169,    38,    98,
       0,   605,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   210,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,   211,   212,
       0,    77,     0,     0,    79,   213,     0,    82,     0,     0,
       0,     0,     0,   214,     0,     0,     0,     0,     0,   215,
       0,     0,     0,     0,   216,     0,   217,     0,     0,   218,
       0,     0,     0,    88,     0,     0,     0,   219,     0,     0,
       0,     0,     0,   220,   221,     0,     0,     0,     0,     0,
     222,     0,     0,     0,     0,     0,     0,     0,     0,   223,
       0,     0,     0,    98,   210,   170,     0,     0,   224,   225,
       0,   226,     0,   227,     0,   228,   211,   212,   229,     0,
       0,     0,   230,   213,     0,   231,     0,     0,   232,     0,
       0,   214,     0,     0,     0,     0,     0,   215,     0,     0,
       0,     0,   216,     0,   217,     0,     0,   218,     0,     0,
       0,     0,     0,     0,     0,   219,     0,     0,     0,     0,
       0,   220,   221,     0,     0,     0,     0,     0,   222,     0,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,     0,     0,   236,   941,   224,   225,     0,   226,
       0,   227,   763,   228,     0,     0,   229,   211,   212,   765,
     230,     0,     0,   231,   213,     0,   232,     0,     0,     0,
       0,     0,   214,     0,     0,     0,     0,     0,   767,     0,
       0,     0,     0,   216,     0,   217,     0,     0,   218,     0,
     770,     0,     0,     0,     0,     0,   219,     0,     0,     0,
       0,     0,   772,   221,     0,     0,     0,     0,   321,   222,
       0,     0,     0,     0,     0,   322,     0,     0,   223,     0,
       0,     0,   719,     0,     0,     0,   323,   224,   225,     0,
     226,     0,   227,     0,   942,     0,     0,   777,     0,     0,
       0,   230,     0,     0,   231,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,   350,   351,   352,   353,
     354,   355,     0,     0,   358,   359,   360,   361,     0,   363,
     364,   780,   781,   782,   783,   784,   324,     0,   785,     0,
     325,     0,   786,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,     0,   797,   321,     0,   798,   799,   800,
     801,     0,   322,     0,     0,     0,     0,   403,     0,     0,
       0,   326,     0,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   327,   328,     0,
     329,   330,   331,     0,   332,   333,   334,     0,   335,   336,
     337,   338,   339,     0,   340,   341,   342,   343,   344,   345,
     346,     0,   321,   347,   348,   349,     0,     0,     0,   322,
       0,     0,   749,   324,     0,     0,     0,   325,     0,     0,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   321,     0,     0,     0,     0,     0,   326,   322,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     323,     0,     0,     0,   327,   328,     0,   329,   330,   331,
     324,   332,   333,   334,   325,   335,   336,   337,   338,   339,
       0,   340,   341,   342,   343,   344,   345,   346,     0,     0,
     347,   348,   349,     0,     0,   321,     0,     0,     0,   821,
       0,     0,   322,     0,     0,   326,     0,     0,     0,     0,
     324,     0,     0,   323,   325,     0,     0,     0,     0,     0,
       0,   327,   328,     0,   329,   330,   331,     0,   332,   333,
     334,     0,   335,   336,   337,   338,   339,     0,   340,   341,
     342,   343,   344,   345,   346,   326,     0,   347,   348,   349,
       0,     0,     0,     0,     0,     0,   976,     0,     0,     0,
       0,   327,   328,   324,   329,   330,   331,   325,   332,   333,
     334,     0,   335,   336,   337,   338,   339,     0,   340,   341,
     342,   343,   344,   345,   346,   321,     0,   347,   348,   349,
       0,     0,   322,     0,     0,     0,  1043,     0,   326,     0,
       0,     0,     0,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   327,   328,     0,   329,   330,   331,
       0,   332,   333,   334,     0,   335,   336,   337,   338,   339,
       0,   340,   341,   342,   343,   344,   345,   346,     0,     0,
     347,   348,   349,     0,     0,     0,     0,   628,     0,     0,
       0,     0,     0,   324,     0,     0,     0,   325,     0,     0,
       0,   321,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
       0,     0,     0,     0,     0,     0,     0,     0,   326,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   327,   328,     0,   329,   330,   331,
       0,   332,   333,   334,     0,   335,   336,   337,   338,   339,
       0,   340,   341,   342,   343,   344,   345,   346,     0,   324,
     347,   348,   349,   325,     0,     0,     0,   940,     0,     0,
       0,   321,   414,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
       0,     0,     0,     0,   326,   350,   351,   352,   353,   354,
     355,     0,     0,   358,   359,   360,   361,     0,   363,   364,
     327,   328,     0,   329,   330,   331,     0,   332,   333,   334,
       0,   335,   336,   337,   338,   339,     0,   340,   341,   342,
     343,   344,   345,   346,     0,     0,   347,   348,   349,   324,
     378,     0,     0,   325,     0,     0,     0,   321,     0,     0,
       0,     0,   826,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   323,     0,     0,     0,     0,
       0,     0,     0,     0,   326,   350,   351,   352,   353,   354,
     355,     0,     0,   358,   359,   360,   361,     0,   363,   364,
     327,   328,     0,   329,   330,   331,     0,   332,   333,   334,
       0,   335,   336,   337,   338,   339,     0,   340,   341,   342,
     343,   344,   345,   346,     0,   324,   347,   348,   349,   325,
     378,     0,     0,   321,     0,     0,     0,     0,   833,     0,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,     0,     0,     0,     0,     0,     0,     0,     0,
     326,   350,   351,   352,   353,   354,   355,     0,     0,   358,
     359,   360,   361,     0,   363,   364,   327,   328,     0,   329,
     330,   331,     0,   332,   333,   334,     0,   335,   336,   337,
     338,   339,     0,   340,   341,   342,   343,   344,   345,   346,
       0,   324,   347,   348,   349,   325,   378,     0,     0,   321,
       0,     0,     0,     0,   953,     0,   322,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   323,     0,     0,
       0,     0,     0,     0,     0,     0,   326,   350,   351,   352,
     353,   354,   355,     0,     0,   358,   359,   360,   361,     0,
     363,   364,   327,   328,     0,   329,   330,   331,     0,   332,
     333,   334,     0,   335,   336,   337,   338,   339,     0,   340,
     341,   342,   343,   344,   345,   346,     0,   324,   347,   348,
     349,   325,   378,     0,     0,   321,     0,     0,     0,     0,
     954,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,     0,     0,     0,     0,     0,     0,
       0,     0,   326,   350,   351,   352,   353,   354,   355,     0,
       0,   358,   359,   360,   361,     0,   363,   364,   327,   328,
       0,   329,   330,   331,     0,   332,   333,   334,     0,   335,
     336,   337,   338,   339,     0,   340,   341,   342,   343,   344,
     345,   346,     0,   324,   347,   348,   349,   325,   378,     0,
       0,   321,     0,     0,     0,     0,   955,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
       0,     0,     0,     0,     0,     0,     0,     0,   326,   350,
     351,   352,   353,   354,   355,     0,     0,   358,   359,   360,
     361,     0,   363,   364,   327,   328,     0,   329,   330,   331,
       0,   332,   333,   334,     0,   335,   336,   337,   338,   339,
       0,   340,   341,   342,   343,   344,   345,   346,     0,   324,
     347,   348,   349,   325,   378,     0,     0,   321,     0,     0,
       0,     0,   956,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   323,     0,     0,     0,     0,
       0,     0,     0,     0,   326,   350,   351,   352,   353,   354,
     355,     0,     0,   358,   359,   360,   361,     0,   363,   364,
     327,   328,     0,   329,   330,   331,     0,   332,   333,   334,
       0,   335,   336,   337,   338,   339,     0,   340,   341,   342,
     343,   344,   345,   346,     0,   324,   347,   348,   349,   325,
     378,     0,     0,   321,     0,     0,     0,     0,   978,     0,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,     0,     0,     0,     0,     0,     0,     0,     0,
     326,   350,   351,   352,   353,   354,   355,     0,     0,   358,
     359,   360,   361,     0,   363,   364,   327,   328,     0,   329,
     330,   331,     0,   332,   333,   334,     0,   335,   336,   337,
     338,   339,     0,   340,   341,   342,   343,   344,   345,   346,
       0,   324,   347,   348,   349,   325,   378,     0,     0,     0,
       0,     0,     0,     0,   979,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   326,   350,   351,   352,
     353,   354,   355,     0,     0,   358,   359,   360,   361,     0,
     363,   364,   327,   328,     0,   329,   330,   331,     0,   332,
     333,   334,     0,   335,   336,   337,   338,   339,     0,   340,
     341,   342,   343,   344,   345,   346,   321,     0,   347,   348,
     349,     0,   378,   322,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   323,   406,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,   321,     0,     0,
       0,     0,     0,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   323,   400,     0,     0,     0,
       0,     0,     0,     0,   324,     0,     0,     0,   325,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   321,     0,     0,     0,     0,     0,     0,   322,
       0,     0,     0,     0,     0,   403,     0,     0,     0,   326,
     323,   702,     0,     0,     0,   324,     0,     0,     0,   325,
       0,     0,     0,     0,     0,   327,   328,     0,   329,   330,
     331,     0,   332,   333,   334,     0,   335,   336,   337,   338,
     339,     0,   340,   341,   342,   343,   344,   345,   346,     0,
     326,   347,   348,   349,     0,   378,     0,     0,     0,     0,
     324,     0,     0,   323,   325,     0,   327,   328,     0,   329,
     330,   331,     0,   332,   333,   334,     0,   335,   336,   337,
     338,   339,     0,   340,   341,   342,   343,   344,   345,   346,
     321,   403,   347,   348,   349,   326,   378,   322,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   323,   240,
       0,   327,   328,   324,   329,   330,   331,   325,   332,   333,
     334,     0,   335,   336,   337,   338,   339,     0,   340,   341,
     342,   343,   344,   345,   346,   321,     0,   347,   348,   349,
       0,   378,   322,     0,     0,     0,     0,     0,   326,     0,
       0,     0,     0,   323,   694,     0,     0,     0,   324,     0,
       0,     0,   325,     0,   327,   328,     0,   329,   330,   331,
       0,   332,   333,   334,     0,   335,   336,   337,   338,     0,
       0,   340,   341,   342,   343,   344,   345,   346,     0,     0,
     347,   348,   349,   326,     0,     0,     0,     0,   323,     0,
       0,     0,     0,   324,     0,     0,     0,   325,     0,   327,
     328,     0,   329,   330,   331,     0,   332,   333,   334,     0,
     335,   336,   337,   338,   339,     0,   340,   341,   342,   343,
     344,   345,   346,   321,     0,   347,   348,   349,   326,   378,
     322,     0,     0,     0,     0,     0,     0,     0,   324,     0,
       0,   323,   325,     0,   327,   328,     0,   329,   330,   331,
       0,   332,   333,   334,     0,   335,   336,   337,   338,   339,
       0,   340,   341,   342,   343,   344,   345,   346,   321,     0,
     347,   348,   349,   326,   378,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,     0,   327,
     328,   324,   329,   330,   331,   325,   332,   333,   334,     0,
     335,   336,   337,   338,   712,     0,   340,   341,   342,   343,
     344,     0,   346,     0,     0,   347,   348,   349,     0,     0,
       0,   321,     0,     0,     0,     0,   326,     0,   322,     0,
       0,     0,     0,     0,     0,     0,   324,     0,     0,   323,
     325,     0,   327,   328,   411,   329,   330,   331,     0,   332,
     333,   334,     0,   335,   336,   337,   338,   339,     0,   340,
     341,   342,   343,   344,   345,   346,     0,     0,   347,   348,
     349,   326,   378,     0,   412,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   327,   328,   324,
     329,   330,   331,   325,   332,   333,   334,     0,   335,   336,
     337,   338,   339,     0,   340,   341,   342,   343,   344,   345,
     346,     0,     0,   347,   348,   349,     0,   441,   321,     0,
       0,     0,     0,     0,   326,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,     0,     0,
     327,   328,     0,   329,   330,   331,     0,   332,   333,   334,
     380,   335,   336,   337,   338,   339,     0,   340,   341,   342,
     343,   344,   345,   346,     0,   413,   347,   348,   349,     0,
       0,   321,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,   324,     0,     0,   323,
     325,     0,     0,     0,   454,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   159,     0,     0,
       0,   326,     0,     0,   455,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   327,   328,   324,
     329,   330,   331,   325,   332,   333,   334,     0,   335,   336,
     337,   338,   339,     0,   340,   341,   342,   343,   344,   345,
     346,     0,     0,   347,   348,   349,     0,     0,   321,     0,
       0,     0,     0,     0,   326,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,   541,     0,     0,
     327,   328,     0,   329,   330,   331,     0,   332,   333,   334,
     542,   335,   336,   337,   338,   339,     0,   340,   341,   342,
     343,   344,   345,   346,   321,     0,   347,   348,   349,     0,
       0,   322,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   323,   543,     0,     0,   324,     0,     0,     0,
     325,     0,     0,     0,     0,     0,   544,     0,     0,     0,
       0,     0,     0,     0,   321,     0,     0,     0,     0,     0,
       0,   322,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   326,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,     0,     0,     0,   325,   327,   328,     0,
     329,   330,   331,     0,   332,   333,   334,     0,   335,   336,
     337,   338,   339,     0,   340,   341,   342,   343,   344,   345,
     346,   321,     0,   347,   348,   349,     0,   326,   322,     0,
       0,     0,   324,     0,     0,     0,   325,     0,     0,   323,
       0,     0,     0,   327,   328,     0,   329,   330,   331,     0,
     332,   333,   334,     0,   335,   336,   337,   338,   339,     0,
     340,   341,   342,   343,   344,   345,   346,   326,     0,   347,
     348,   349,     0,     0,     0,     0,     0,     0,     0,   323,
       0,     0,     0,   327,   328,     0,   329,   330,   331,   324,
     332,   333,   334,   325,   335,   336,   337,   338,   339,     0,
     340,   341,   342,   343,   344,   345,   346,   321,   445,   347,
     348,   349,     0,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   326,   323,     0,     0,     0,   324,
       0,     0,     0,   325,     0,     0,     0,     0,     0,     0,
     327,   328,     0,   329,   330,   331,     0,   332,   333,   334,
       0,   335,   336,   337,   338,   339,     0,   340,   341,   342,
     343,   344,   345,   346,   321,   447,   347,   348,   349,     0,
       0,   322,     0,     0,     0,   324,     0,     0,     0,   325,
     327,   328,   323,   329,   330,   331,     0,   332,   333,   334,
       0,   335,   336,   337,   338,     0,     0,   340,   341,   342,
     343,   344,     0,   346,   321,     0,   347,   348,   349,     0,
     326,   322,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   323,     0,     0,     0,   327,   328,     0,   329,
     330,   331,   324,   332,   333,   334,   325,   335,   336,   337,
     338,   339,     0,   340,   341,   342,   343,   344,   345,   346,
     321,   449,   347,   348,   349,     0,     0,   322,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   326,   323,     0,
       0,     0,   324,     0,     0,     0,   325,     0,     0,     0,
       0,     0,     0,   327,   328,   545,   329,   330,   331,     0,
     332,   333,   334,     0,   335,   336,   337,   338,   339,     0,
     340,   341,   342,   343,   344,   345,   346,   326,   457,   347,
     348,   349,     0,     0,     0,     0,     0,     0,   324,     0,
       0,     0,   325,   327,   328,     0,   329,   330,   331,     0,
     332,   333,   334,     0,   335,   336,   337,   338,   339,     0,
     340,   341,   342,   343,   344,   345,   346,   321,     0,   347,
     348,   349,   474,   326,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   323,     0,     0,     0,   327,
     328,     0,   329,   330,   331,     0,   332,   333,   334,     0,
     335,   336,   337,   338,   339,     0,   340,   341,   342,   343,
     344,   345,   346,     0,     0,   347,   348,   349,     0,     0,
     321,     0,     0,     0,     0,     0,     0,   322,     0,     0,
       0,     0,     0,     0,     0,   324,     0,     0,   323,   325,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     321,     0,     0,     0,     0,     0,     0,   322,     0,     0,
     326,     0,     0,     0,     0,     0,     0,     0,   323,     0,
       0,     0,     0,     0,     0,     0,   327,   328,   324,   329,
     330,   331,   325,   332,   333,   334,     0,   335,   336,   337,
     338,   339,  -489,   340,   341,   342,   343,   344,   345,   346,
       0,     0,   347,   348,   349,     0,     0,   321,   707,   159,
       0,     0,     0,   326,   322,     0,     0,     0,   324,     0,
       0,     0,   325,     0,     0,   323,     0,     0,     0,   327,
     328,     0,   329,   330,   331,     0,   332,   333,   334,     0,
     335,   336,   337,   338,   339,     0,   340,   341,   342,   343,
     344,   345,   346,   326,     0,   347,   348,   349,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   327,
     328,     0,   329,   330,   331,   324,   332,   333,   334,   325,
     335,   336,   337,   338,   339,     0,   340,   341,   342,   343,
     344,   345,   346,   321,   683,   347,   348,   349,     0,     0,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     326,   323,     0,     0,   710,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   327,   328,     0,   329,
     330,   331,     0,   332,   333,   334,     0,   335,   336,   337,
     338,   339,     0,   340,   341,   342,   343,   344,   345,   346,
     321,     0,   347,   348,   349,     0,     0,   322,     0,     0,
       0,   324,     0,     0,     0,   325,     0,     0,   323,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   321,     0,     0,     0,     0,     0,     0,   322,
       0,     0,     0,     0,     0,     0,   326,     0,     0,     0,
     323,   816,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   327,   328,     0,   329,   330,   331,   324,   332,
     333,   334,   325,   335,   336,   337,   338,   339,     0,   340,
     341,   342,   343,   344,   345,   346,     0,     0,   347,   348,
     349,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     324,     0,   752,   326,   325,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   327,
     328,     0,   329,   330,   331,     0,   332,   333,   334,     0,
     335,   336,   337,   338,   339,   326,   340,   341,   342,   343,
     344,   345,   346,   321,     0,   347,   348,   349,     0,   324,
     322,   327,   328,   325,   329,   330,   331,     0,   332,   333,
     334,   323,   335,   336,   337,   338,   339,     0,   340,   341,
     342,   343,   344,   345,   346,   321,     0,   347,   348,   349,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   819,     0,     0,     0,     0,     0,
     327,   328,     0,   329,   330,   331,     0,   332,   333,   334,
       0,   324,   336,   337,   338,   325,     0,   340,   341,   342,
     343,     0,     0,   346,   891,     0,   347,   348,   349,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,     0,     0,   326,   325,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   327,   328,     0,   329,   330,   331,     0,   332,
     333,   334,     0,   335,   336,   337,   338,   339,   326,   340,
     341,   342,   343,   344,   345,   346,   321,     0,   347,   348,
     349,     0,     0,   322,   327,   328,     0,   329,   330,   331,
       0,   332,   333,   334,   323,   335,   336,   337,   338,   339,
       0,   340,   341,   342,   343,   344,   345,   346,   321,   908,
     347,   348,   349,     0,     0,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,     0,     0,     0,   325,     0,
       0,     0,     0,     0,     0,     0,     0,   894,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,     0,     0,   326,
     325,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   327,   328,     0,   329,   330,
     331,     0,   332,   333,   334,     0,   335,   336,   337,   338,
     339,   326,   340,   341,   342,   343,   344,   345,   346,   321,
       0,   347,   348,   349,     0,     0,   322,   327,   328,     0,
     329,   330,   331,     0,   332,   333,   334,   323,   335,   336,
     337,   338,   339,     0,   340,   341,   342,   343,   344,   345,
     346,   321,     0,   347,   348,   349,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   324,     0,     0,
       0,   325,     0,     0,     0,     0,     0,     0,     0,     0,
     989,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   324,
       0,     0,   326,   325,     0,     0,     0,     0,     0,     0,
       0,     0,   990,     0,     0,     0,     0,     0,   327,   328,
       0,   329,   330,   331,     0,   332,   333,   334,     0,   335,
     336,   337,   338,   339,   326,   340,   341,   342,   343,   344,
     345,   346,   321,     0,   347,   348,   349,     0,     0,   322,
     327,   328,     0,   329,   330,   331,     0,   332,   333,   334,
     323,   335,   336,   337,   338,   339,     0,   340,   341,   342,
     343,   344,   345,   346,   321,     0,   347,   348,   349,     0,
       0,   322,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,     0,     0,     0,   325,     0,     0,     0,     0,     0,
       0,     0,     0,   991,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,     0,     0,   326,   325,     0,     0,     0,
       0,     0,     0,     0,     0,   992,     0,     0,     0,     0,
       0,   327,   328,     0,   329,   330,   331,     0,   332,   333,
     334,     0,   335,   336,   337,   338,   339,   326,   340,   341,
     342,   343,   344,   345,   346,   321,     0,   347,   348,   349,
       0,     0,   322,   327,   328,     0,   329,   330,   331,     0,
     332,   333,   334,   323,   335,   336,   337,   338,   339,     0,
     340,   341,   342,   343,   344,   345,   346,   321,     0,   347,
     348,   349,     0,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   323,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   324,     0,     0,     0,   325,     0,     0,
       0,     0,     0,     0,     0,     0,   993,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,     0,     0,   326,   325,
       0,     0,     0,     0,     0,     0,     0,     0,   994,     0,
       0,     0,     0,     0,   327,   328,     0,   329,   330,   331,
       0,   332,   333,   334,     0,   335,   336,   337,   338,   339,
     326,   340,   341,   342,   343,   344,   345,   346,   321,     0,
     347,   348,   349,     0,     0,   322,   327,   328,     0,   329,
     330,   331,     0,   332,   333,   334,   323,   335,   336,   337,
     338,   339,     0,   340,   341,   342,   343,   344,   345,   346,
     321,     0,   347,   348,   349,     0,     0,   322,     0,     0,
    1016,     0,     0,     0,     0,     0,     0,     0,   323,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   324,     0,     0,     0,
     325,     0,     0,     0,     0,     0,     0,     0,     0,   321,
       0,     0,     0,     0,     0,     0,   322,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   323,   324,     0,
       0,   326,   325,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   327,   328,     0,
     329,   330,   331,     0,   332,   333,   334,     0,   335,   336,
     337,   338,   339,   326,   340,   341,   342,   343,   344,   345,
     346,     0,     0,   347,   348,   349,     0,   671,     0,   327,
     328,   325,   329,   330,   331,     0,   332,   333,   334,     0,
     335,   336,   337,   338,   339,     0,   340,   341,   342,   343,
     344,   345,   346,     0,     0,   347,   348,   349,     0,     0,
       0,     0,   326,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   327,   328,
       0,   329,   330,   331,     0,   332,   333,   334,     0,   335,
     336,   337,   338,   339,     0,   340,   341,   342,   343,   344,
     345,   346,     0,    16,   347,   348,   349,    20,    21,     0,
       0,     0,     0,     0,     0,     0,    27,    16,    28,   873,
       0,    20,    21,    33,     0,     0,     0,     0,    37,     0,
      27,     0,    28,   873,     0,     0,     0,    33,     0,     0,
       0,     0,    37,     0,     0,     0,     0,     0,     0,     0,
      50,     0,    52,     0,    54,     0,   874,     0,     0,   875,
       0,    59,     0,    60,    50,     0,    52,     0,   307,     0,
     874,     0,     0,   875,     0,    59,   649,    60,   210,     0,
       0,     0,    76,     0,     0,    78,     0,     0,    81,     0,
     211,   212,     0,     0,     0,     0,    76,   213,     0,    78,
       0,     0,    81,     0,     0,   214,     0,     0,     0,     0,
       0,   215,     0,     0,     0,     0,   216,     0,   217,     0,
       0,   218,     0,     0,     0,     0,     0,     0,     0,   219,
       0,     0,     0,     0,     0,   220,   221,     0,    95,     0,
       0,     0,   222,     0,   952,   210,     0,     0,     0,     0,
       0,   223,    95,     0,     0,     0,     0,   211,   212,     0,
     224,   225,     0,   226,   213,   227,     0,   228,     0,     0,
     229,     0,   214,     0,   230,     0,     0,   231,   215,     0,
     232,     0,     0,   216,     0,   217,     0,     0,   218,     0,
       0,     0,     0,     0,     0,     0,   219,     0,     0,     0,
       0,     0,   220,   221,     0,     0,     0,     0,     0,   222,
       0,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,     0,     0,     0,     0,   224,   225,     0,
     226,     0,   227,     0,   228,     0,     0,   229,     0,     0,
       0,   230,     0,     0,   231,     0,     0,   232
};

static const yytype_int16 yycheck[] =
{
      11,   144,     2,   182,    15,   419,   450,    18,    17,   371,
     422,    56,    57,    24,   131,   658,   195,    28,    29,    20,
      31,    32,    33,    34,   190,   203,   567,    61,   728,    28,
       1,    42,    29,    44,    45,    29,    29,   754,    49,    29,
      51,   209,    53,   209,   923,    29,    80,    42,   846,     3,
      61,    62,    63,    64,    65,    66,     1,     3,   154,    70,
     961,    72,     1,    74,    75,    99,   183,   600,    79,    80,
      42,    81,    83,    84,    24,    86,     1,    88,   107,   109,
      91,    92,   250,    94,   250,    42,    97,    98,    99,   807,
     808,    42,    42,     0,   849,    45,   109,   135,    81,   427,
      42,   269,   412,   269,   135,  1006,   135,    79,    74,    75,
      42,   811,   109,    52,   912,    65,   109,    42,    68,    64,
      70,    42,   450,   109,   162,   303,   304,   668,    64,    79,
      42,   162,     3,   276,   164,    81,   170,   670,    92,     1,
     135,   135,    81,   114,  1023,   455,    92,    79,    98,   159,
     850,   164,   109,   164,   165,   166,   167,   168,   169,   170,
     877,   106,   159,   134,   135,   159,   159,   333,   107,   159,
     106,   154,   181,    31,    32,   159,   159,    19,   135,   190,
      19,    62,   937,   900,   135,   135,   161,   599,   163,   114,
      52,   162,   725,   135,   205,   134,   197,    79,   209,   367,
     368,   367,   368,   204,     3,   567,   205,   164,    52,   134,
     135,   911,   162,   164,   135,   160,  1006,    98,   161,    81,
     159,    24,   164,   135,   160,   943,   944,   135,   161,   154,
      24,    30,    94,     7,    24,    77,   114,   135,    77,   250,
     251,    99,   135,   164,   887,   107,   114,   258,   259,   135,
      98,   135,   164,  1043,    47,    54,   164,   135,   269,   101,
     960,   139,   101,   107,   162,    68,   135,   135,   154,   162,
     154,   139,   134,   114,    68,   443,    79,   443,    68,   396,
      73,   135,   154,   135,   162,    79,   135,   298,   160,    79,
     831,   160,   154,    92,   162,   295,   161,   159,   139,    98,
     154,   104,   154,   717,    98,   154,   668,   719,    98,   320,
     321,   322,   323,   324,   325,   326,   135,   328,   329,   330,
     331,   332,   135,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   135,
     754,    89,   190,   162,   135,   356,   357,   135,   135,   162,
    1021,   362,   161,   159,   365,   366,   367,   368,   369,   370,
     371,   209,    74,   135,   398,   135,   162,    24,  1039,   380,
     154,   162,   160,   154,   385,   162,   387,   161,   129,   163,
     153,    52,   154,     2,   154,   154,   137,   398,    28,   400,
     401,   557,    11,   571,   572,   406,   407,   161,   114,   163,
     411,   412,   250,   414,    23,   416,    25,    42,   129,   410,
      81,    68,   856,   154,   154,   161,   137,   163,   429,   160,
     145,   269,    79,    94,   140,   436,   142,   143,   154,    64,
     146,   147,   443,    78,   154,   154,   107,    82,    78,    74,
      75,    98,    82,   454,   455,   624,   154,   104,   482,   134,
     461,    70,   137,   464,   161,   466,   141,   635,   636,   470,
     471,   472,   154,   474,   475,   137,   324,   325,   882,   831,
     161,   482,   163,    78,    52,   356,   357,    82,    97,   159,
     480,   362,   101,   154,   365,   366,   900,   154,   159,   129,
     464,   108,   466,   161,   134,   161,   136,   137,   154,   161,
     114,   141,   154,    81,     7,    56,   134,   154,   159,   149,
     114,   369,   370,   162,    42,   658,    94,   157,   154,   367,
     368,   849,   380,   135,    52,    42,    31,   161,   856,   107,
     541,   542,   543,   544,   545,   546,   547,   162,   135,   548,
     549,   160,   162,   401,   555,   180,   557,   114,    18,   407,
     561,   162,   563,    81,   162,   135,   567,   164,   569,   159,
     154,   164,   154,   154,   154,   200,   201,   202,   203,   154,
     154,   605,   135,   208,   162,   164,   154,   154,   154,   107,
      31,   159,   135,   464,   154,   466,   114,   162,   436,   160,
     162,   160,   603,   604,   605,   443,   154,    79,   609,   114,
      29,    29,   164,   159,    31,    42,    79,   135,   243,   937,
     134,   246,   470,   461,   233,    52,   464,    28,   466,   164,
     134,   240,   164,   471,   164,   160,   154,   161,   137,   137,
     265,   159,     2,   961,   162,   129,   137,   137,   137,   129,
     137,    11,   137,   129,    81,   137,   137,    31,   976,   137,
     285,   129,   129,    23,   137,    25,    28,   668,   162,    94,
     671,   672,   673,   129,   162,   676,    52,    78,   129,   455,
     107,    82,   135,   160,   555,   164,   295,   114,  1006,   309,
      22,   692,   277,   694,   542,   739,   544,   454,   699,   196,
     309,   702,   559,  1021,   705,    81,   707,   824,   135,   710,
      70,   735,   757,   887,    74,    75,    78,   555,    94,   557,
      82,  1039,   882,   717,   760,  1043,   592,   154,   129,  1023,
     960,   107,   159,   134,   735,   162,   137,    97,   609,   985,
     141,   101,  1016,     2,  1009,    33,   747,   430,   749,   557,
      -1,   752,    11,    -1,   887,   756,   381,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    25,   129,   130,   378,
     132,   133,   134,    -1,   136,   137,   138,    -1,   154,   141,
     405,    -1,    -1,   159,   409,    -1,   148,   149,    -1,    -1,
     152,   400,    -1,   155,   156,   157,    -1,   406,    -1,    -1,
     160,   802,   967,    -1,    -1,   414,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    -1,   815,   816,    -1,   818,   819,   820,
     821,    -1,    -1,   671,   672,   826,   827,    -1,   829,    -1,
     831,    -1,   833,   834,    -1,    -1,   837,    -1,    97,   874,
     875,    -1,   101,    -1,    -1,  1010,  1011,  1012,  1013,  1014,
    1015,    -1,    -1,    -1,   855,   856,    -1,    23,    24,   468,
     861,    -1,   863,   472,    -1,    24,   867,    -1,    34,    35,
      -1,   480,   873,   233,    -1,    -1,    -1,    -1,    44,    -1,
     240,    -1,    -1,    42,    -1,    -1,    45,    24,    -1,   913,
     891,   892,   893,   894,   895,   896,   897,   898,    -1,    -1,
      66,   160,    68,    -1,    42,    42,    65,   908,    45,    68,
      -1,    70,   913,    79,    52,    -1,    -1,   918,    -1,    -1,
      79,   922,    -1,    -1,    -1,    -1,    -1,    -1,    65,   538,
      -1,    68,    -1,    70,    -1,   295,    -1,    -1,   104,    98,
     565,   566,    79,    81,    -1,    -1,   571,   572,    -1,   309,
      -1,    -1,   953,   954,   955,   956,   980,    -1,   583,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,   582,   233,   976,   114,   978,   979,   980,
      -1,   240,    -1,    -1,    -1,    -1,    -1,    -1,   989,   990,
     991,   992,   993,   994,    -1,   620,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,  1005,  1006,    -1,  1008,    -1,    -1,
      -1,   849,    -1,    -1,    -1,    -1,   154,    -1,   378,    -1,
      -1,   159,    -1,    -1,   162,   634,    -1,    -1,    -1,    -1,
      -1,   640,    -1,    -1,    -1,    -1,   295,    -1,  1039,    -1,
     400,    -1,  1043,    -1,    -1,    -1,   406,    -1,    -1,   658,
     309,    -1,    -1,    -1,   414,    -1,   681,   682,    -1,    -1,
      -1,    -1,   671,   672,   689,    -1,   691,    -1,    -1,    -1,
      -1,    -1,    -1,   698,    -1,    -1,   701,    -1,    -1,   704,
     918,    -1,    -1,   692,    -1,   694,    -1,    -1,    -1,    -1,
     699,    -1,    -1,   702,    -1,     2,    -1,    -1,    -1,    -1,
     709,    -1,    -1,   712,    11,    -1,    39,    -1,   468,    -1,
      -1,     1,   472,    -1,    -1,    -1,    23,   742,    25,   378,
     480,    -1,    -1,   961,    -1,   734,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    23,    24,    -1,    -1,    -1,   747,    -1,
     749,   400,    32,    76,    34,    35,    -1,   406,    -1,    39,
      -1,    -1,    -1,    -1,    44,   414,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,  1006,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,     2,    68,    -1,
      70,    -1,    72,    73,    -1,    75,    11,    77,    -1,    79,
      97,   124,    -1,   126,   101,    -1,    -1,    -1,    23,    -1,
      25,    -1,    -1,    -1,    -1,  1043,    -1,    -1,    98,   468,
      -1,   101,    -1,   472,   104,    -1,    -1,   826,    -1,    -1,
      -1,   480,   582,    -1,   833,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   859,    -1,   845,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,   857,   182,
      -1,    -1,    -1,   160,    -1,    28,    -1,   866,    -1,    -1,
      -1,   870,   195,    -1,   154,   198,    -1,    -1,    -1,    -1,
     160,    -1,    97,    -1,   634,    -1,   101,   902,   903,    -1,
     640,   906,   907,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   923,    -1,
     909,    -1,    -1,   236,   929,    78,   931,    -1,   933,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,   254,    -1,   582,    -1,    -1,   233,    -1,    -1,    -1,
      -1,    -1,   692,   240,   694,   160,    -1,    -1,    -1,   699,
      -1,    -1,   702,    -1,   953,   954,   955,   956,    -1,   709,
      -1,    28,   712,    -1,    -1,    -1,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,    -1,    -1,    78,   141,   978,
     979,    82,    -1,    -1,   734,   634,   149,    -1,    -1,    -1,
      -1,   640,   155,   156,   157,    -1,    -1,   747,   295,   749,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1023,    -1,
      64,    78,   309,    -1,    -1,    82,    -1,    -1,   233,    -1,
      -1,    -1,    -1,    28,    -1,   240,    -1,    -1,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,    -1,    -1,    -1,
     141,    -1,    -1,   692,    -1,   694,    -1,   148,   149,    -1,
     699,   152,    -1,   702,   155,   156,   157,    -1,    -1,    -1,
     709,    -1,   129,   712,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,   395,    78,   141,    -1,   826,    82,    -1,    -1,
     295,   378,   149,   833,    -1,   734,    -1,     2,   155,   156,
     157,    -1,    -1,    -1,   309,   845,    11,    -1,   747,   422,
     749,    -1,    -1,   400,    -1,    -1,    -1,   857,    23,   406,
      25,    -1,    -1,    -1,    28,    -1,   866,   414,    -1,    -1,
     870,    -1,    -1,    -1,   129,   130,    -1,    -1,   133,   134,
      -1,   136,   137,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,   200,   201,   202,   203,
     155,   156,   157,    -1,    -1,    70,    -1,    -1,   481,   909,
      -1,   484,    -1,   378,    78,    -1,    -1,    -1,    82,    -1,
      -1,   468,    -1,    -1,    -1,   472,    -1,   826,    -1,    -1,
      -1,     3,    97,   480,   833,   400,   101,    -1,    -1,   243,
      -1,   406,   246,    15,    -1,    -1,   845,    -1,    -1,   414,
      22,    -1,    -1,   953,   954,   955,   956,    -1,   857,    -1,
      -1,   265,    -1,    -1,    -1,   129,   130,   866,    -1,    41,
     134,   870,   136,   137,    46,    -1,    -1,   141,   978,   979,
      -1,   285,    54,    -1,    -1,   149,    -1,    -1,    -1,    61,
      -1,   155,   156,   157,    -1,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   468,    76,    -1,    -1,   472,    -1,    -1,
     909,    -1,    -1,    -1,    -1,   480,    88,    -1,    -1,    -1,
      92,    -1,   595,    -1,    -1,    -1,   599,    99,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   582,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   620,    11,    -1,
      -1,   624,    -1,    -1,   953,   954,   955,   956,    -1,    -1,
      23,    -1,    25,    -1,    -1,    -1,    -1,    -1,   233,    -1,
      23,    24,    -1,    -1,    -1,   240,    -1,   381,    -1,   978,
     979,    34,    35,    -1,    -1,   658,    -1,   634,    -1,    -1,
      -1,    44,    -1,   640,    47,     2,    -1,    -1,    -1,    -1,
      -1,   405,    -1,    -1,    11,   409,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    68,    23,   582,    25,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
     295,    -1,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,   716,   309,   692,   719,   694,    -1,    -1,
      -1,   104,   699,    -1,    -1,   702,    -1,    -1,    -1,    -1,
      -1,    -1,   709,    70,    -1,   712,   739,    -1,    -1,   634,
      -1,    -1,    -1,    -1,    -1,   640,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   757,    -1,    -1,   734,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   160,    -1,    -1,
     747,    -1,   749,    -1,    -1,    -1,    -1,    -1,    -1,   782,
      -1,    -1,    -1,   378,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   692,    -1,   694,
      -1,    -1,    -1,    -1,   699,   400,    -1,   702,    -1,    -1,
      -1,   406,    -1,    -1,   709,    -1,    -1,   712,    -1,   414,
      -1,    -1,    -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   565,   566,    -1,    -1,    -1,    -1,   571,   572,   734,
     233,    -1,    -1,    -1,    -1,    -1,    -1,   240,    -1,   826,
      -1,    -1,   747,    -1,   749,    -1,   833,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   845,    -1,
      -1,    -1,    -1,   468,    -1,    -1,    -1,   472,    -1,    -1,
     857,    -1,    -1,    -1,   887,   480,    -1,    -1,    -1,   866,
      -1,    -1,    -1,   870,    -1,    -1,   233,    -1,     2,    -1,
      -1,    -1,   295,   240,    -1,    -1,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   309,    -1,    -1,    23,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   826,   909,    -1,    -1,    -1,    -1,    -1,   833,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   681,   682,    -1,
     845,    -1,    -1,    -1,    -1,   689,    -1,    -1,   295,    -1,
      -1,    -1,   857,    -1,    -1,    -1,    70,   701,    -1,    -1,
     704,   866,   309,    -1,    -1,   870,   953,   954,   955,   956,
      -1,    -1,    -1,    -1,    -1,   378,    -1,   582,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,
      -1,   978,   979,    -1,    -1,    -1,    -1,   400,    -1,    -1,
      -1,    -1,    -1,   406,   909,    -1,    -1,    -1,    -1,    -1,
      -1,   414,    -1,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,   634,
      -1,   378,    -1,    -1,    -1,   640,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,    -1,   160,    -1,   953,   954,
     955,   956,    -1,   400,    -1,    -1,    -1,   158,    -1,   406,
     161,    -1,   163,    -1,    -1,   468,    -1,   414,    -1,   472,
      -1,    -1,    -1,   978,   979,    -1,    -1,   480,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   692,    -1,   694,
      -1,    -1,    -1,    -1,   699,    -1,    -1,   702,    -1,    -1,
      -1,    -1,    -1,    -1,   709,     3,    -1,   712,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   859,    -1,    15,    16,   233,
      -1,   468,    -1,    -1,    22,   472,   240,    -1,    -1,   734,
      -1,    -1,    30,   480,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,   747,    41,   749,    43,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,   582,
      -1,   295,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      88,    -1,    90,    -1,    92,   309,    -1,    95,    -1,    -1,
      -1,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   826,    -1,    -1,    -1,    -1,    -1,    -1,   833,    -1,
      -1,   634,    -1,    -1,    -1,   582,     1,   640,    -1,    -1,
     845,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   857,    -1,    19,    -1,    -1,    -1,    23,    24,
      -1,   866,    -1,   161,   378,   870,    -1,    32,    -1,    34,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,   400,   634,    -1,   692,
      -1,   694,   406,   640,    -1,    -1,   699,    -1,    -1,   702,
      -1,    66,    -1,    68,   909,    70,   709,    72,    73,   712,
      75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   734,    -1,    98,    -1,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,    -1,   747,   692,   749,   694,   953,   954,
     955,   956,   699,    -1,   468,   702,    -1,    -1,   472,    -1,
      -1,    -1,   709,    -1,    -1,   712,   480,    -1,    -1,    -1,
      -1,    -1,    -1,   978,   979,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   734,    -1,   154,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,     3,    -1,
     747,    -1,   749,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,   826,    -1,    30,    -1,    -1,    -1,    -1,
     833,    36,    -1,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,   845,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,   857,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,   866,    -1,    -1,    -1,   870,   582,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   826,
      85,    86,    -1,    88,    -1,    90,   833,    92,    19,    -1,
      95,    -1,    23,    24,    99,    -1,    -1,   102,   845,    -1,
     105,    32,    -1,    34,    35,    -1,   909,    -1,    39,    -1,
     857,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,   866,
     634,    -1,    -1,   870,    -1,    -1,   640,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    68,    -1,    70,
      -1,    72,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,
     953,   954,   955,   956,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,   909,    -1,    -1,    -1,    -1,    98,    -1,    -1,
     101,    -1,    -1,   104,    -1,   978,   979,    -1,   692,    -1,
     694,    -1,    -1,    -1,    -1,   699,    -1,    -1,   702,    -1,
      -1,    -1,    -1,    -1,    -1,   709,    -1,    -1,   712,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   953,   954,   955,   956,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     734,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,   160,
      -1,   978,   979,   747,    -1,   749,    -1,     0,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    79,    80,    81,    -1,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,   845,    95,    96,    97,    98,    99,    -1,   101,   102,
     103,   104,   105,   857,    -1,   108,    -1,   110,    -1,    -1,
     113,    -1,   866,    -1,    -1,    -1,   870,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
      -1,   154,    -1,    -1,   157,   909,   159,    -1,   161,     1,
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
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,   161,
      -1,   163,   164,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    13,    14,    15,    16,    17,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    -1,    95,    96,    97,
      98,    99,    -1,   101,   102,   103,   104,   105,    -1,    -1,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
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
      -1,   148,    -1,    -1,   151,   152,   153,   154,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,    -1,
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
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
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
     161,    -1,   163,     3,     4,     5,     6,     7,     8,    -1,
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
     130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,   161,    -1,   163,   164,     3,     4,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,    -1,   129,   130,   131,   132,   133,    10,    -1,
      -1,    -1,   138,    15,    16,    17,    -1,    -1,    -1,    -1,
      22,    -1,   148,    -1,    -1,   151,   152,    -1,    30,    -1,
      -1,   157,    -1,   159,    36,   161,    -1,   163,   164,    41,
      -1,    43,    -1,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    88,    -1,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
     132,   133,    -1,    -1,   136,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,    -1,
     152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    35,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    -1,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    85,    86,    87,    88,    89,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
      -1,    -1,    -1,    -1,   157,    -1,   159,    -1,   161,    -1,
     163,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
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
      -1,    95,    96,    97,    -1,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,    -1,    -1,    -1,    -1,
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
      -1,    -1,    95,    96,    97,    -1,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,    -1,   157,    -1,   159,    -1,   161,   162,
     163,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    96,    97,    -1,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,   161,
     162,   163,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    42,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    90,
      -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,
      -1,   102,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,
     151,   152,    -1,    -1,    -1,    -1,   157,    -1,   159,    -1,
     161,    -1,   163,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    19,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    98,    99,
      -1,   101,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
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
      92,    -1,    -1,    95,    96,    97,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,
     152,    -1,   154,    -1,    -1,   157,    -1,   159,    -1,   161,
      -1,   163,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    90,
      -1,    92,    -1,    -1,    95,    96,    97,    -1,    99,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,
     161,    -1,   163,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    96,    97,    -1,    99,
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
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    -1,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,   111,
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
      -1,   105,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,
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
      -1,    -1,    95,    96,    97,    -1,    99,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,    -1,    -1,   138,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,   148,    -1,    12,   151,   152,
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
      -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,
      -1,    -1,   157,    -1,   159,   160,   161,    -1,   163,     3,
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
      -1,   105,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,   148,    -1,    12,   151,   152,    15,
      16,    -1,    -1,   157,    -1,   159,    22,   161,    -1,   163,
      -1,    -1,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,
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
      95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,
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
     152,    15,    16,    -1,    -1,   157,    -1,    -1,    22,   161,
      -1,   163,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,   148,    15,    16,    -1,   152,    -1,
      -1,   158,    22,   157,   161,   159,   163,   161,    -1,   163,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,   131,    -1,    -1,    -1,   135,    22,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,   153,    40,    41,    -1,    43,    -1,   159,
      46,   161,   162,   163,    50,    51,    52,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    94,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,   154,    -1,
      -1,    -1,    -1,   159,    30,   161,    -1,   163,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
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
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,   131,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,   159,    46,   161,
      -1,   163,    50,    51,    -1,    -1,    54,    55,    -1,    57,
      58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    85,    86,    87,
      88,    89,    90,    -1,    92,    -1,    -1,    95,    15,    16,
      -1,    99,    -1,    -1,   102,    22,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,   131,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,   161,     3,   163,    -1,    -1,    85,    86,
      -1,    88,    -1,    90,    -1,    92,    15,    16,    95,    -1,
      -1,    -1,    99,    22,    -1,   102,    -1,    -1,   105,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,   161,     3,    85,    86,    -1,    88,
      -1,    90,    10,    92,    -1,    -1,    95,    15,    16,    17,
      99,    -1,    -1,   102,    22,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    10,    67,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    76,    -1,
      -1,    -1,   161,    -1,    -1,    -1,    28,    85,    86,    -1,
      88,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,
      -1,   129,   130,    78,   132,   133,   134,    82,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,    10,    -1,   155,   156,   157,
      -1,    -1,    17,    -1,    -1,    -1,   164,    -1,   113,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,    -1,   162,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
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
      -1,   159,    17,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    82,    -1,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
     155,   156,   157,   113,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,   129,
     130,    -1,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,    10,    -1,   155,   156,   157,   113,   159,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    28,    82,    -1,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    10,    -1,
     155,   156,   157,   113,   159,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,   129,
     130,    78,   132,   133,   134,    82,   136,   137,   138,    -1,
     140,   141,   142,   143,    91,    -1,   146,   147,   148,   149,
     150,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
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
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,    78,
     136,   137,   138,    82,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,    10,   154,   155,
     156,   157,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    28,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    10,   154,   155,   156,   157,    -1,
      -1,    17,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,
     129,   130,    28,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    10,    -1,   155,   156,   157,    -1,
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
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      78,    -1,   112,   113,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,    -1,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,   113,   146,   147,   148,   149,
     150,   151,   152,    10,    -1,   155,   156,   157,    -1,    78,
      17,   129,   130,    82,   132,   133,   134,    -1,   136,   137,
     138,    28,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,    10,    -1,   155,   156,   157,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      -1,    78,   141,   142,   143,    82,    -1,   146,   147,   148,
     149,    -1,    -1,   152,    91,    -1,   155,   156,   157,    -1,
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
     151,   152,    -1,    19,   155,   156,   157,    23,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    19,    34,    35,
      -1,    23,    24,    39,    -1,    -1,    -1,    -1,    44,    -1,
      32,    -1,    34,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    68,    -1,    70,    -1,    72,    -1,    -1,    75,
      -1,    77,    -1,    79,    66,    -1,    68,    -1,    70,    -1,
      72,    -1,    -1,    75,    -1,    77,     1,    79,     3,    -1,
      -1,    -1,    98,    -1,    -1,   101,    -1,    -1,   104,    -1,
      15,    16,    -1,    -1,    -1,    -1,    98,    22,    -1,   101,
      -1,    -1,   104,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    60,    61,    -1,   154,    -1,
      -1,    -1,    67,    -1,   160,     3,    -1,    -1,    -1,    -1,
      -1,    76,   154,    -1,    -1,    -1,    -1,    15,    16,    -1,
      85,    86,    -1,    88,    22,    90,    -1,    92,    -1,    -1,
      95,    -1,    30,    -1,    99,    -1,    -1,   102,    36,    -1,
     105,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
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
     133,   138,   148,   151,   152,   154,   157,   159,   161,   163,
     171,   172,   173,   174,   175,   176,   178,   183,   184,   185,
     189,   191,   195,   196,   197,   198,   199,   200,   202,   203,
     204,   208,   209,   212,   213,   216,   217,   220,   223,   224,
     250,   253,   254,   274,   275,   276,   277,   278,   279,   280,
     288,   289,   290,   291,   294,   295,   296,   297,   298,   299,
     300,   302,   303,   304,   305,   306,   154,   173,   291,   109,
     281,   282,     3,   186,    12,    37,    38,    40,    90,   159,
     163,   220,   274,   279,   289,   290,   291,   294,   296,   297,
     281,   291,    98,   253,    79,   186,   173,   268,   291,   173,
     161,   291,   292,     7,     9,    98,   193,   292,    68,   111,
     201,   291,   201,   291,    23,    35,   254,   291,   291,   161,
       3,    15,    16,    22,    30,    36,    41,    43,    46,    54,
      60,    61,    67,    76,    85,    86,    88,    90,    92,    95,
      99,   102,   105,   188,   190,   221,   161,   188,   255,   256,
      29,   178,   194,   291,   291,   291,   291,   291,   291,     7,
     161,    35,   197,   197,   153,   243,   267,   291,    96,    97,
     154,   291,   293,   291,   194,   291,   291,   291,    89,   161,
     173,   291,   291,   178,   185,   291,   294,   178,   185,   291,
     188,   251,   291,   267,   291,   291,   291,   291,   291,   291,
     291,   291,     1,   160,   171,   179,   267,   100,   139,   243,
     269,   270,   293,   201,   267,   291,   301,    70,   173,   159,
      74,   177,   188,   188,    47,    73,   238,   255,   153,   154,
     145,    10,    17,    28,    78,    82,   113,   129,   130,   132,
     133,   134,   136,   137,   138,   140,   141,   142,   143,   144,
     146,   147,   148,   149,   150,   151,   152,   155,   156,   157,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   158,   230,   161,   163,    78,
      82,   161,   173,   154,   291,   291,   291,   267,   159,   178,
      42,   281,   251,   255,   154,   135,   154,   108,   189,   243,
     271,   272,   273,   293,   251,   213,   253,   188,   161,   194,
      29,    42,   194,   109,   194,   284,    29,    42,   194,   284,
     292,    33,    63,   154,    91,   178,   230,   271,   173,   161,
     232,   100,   161,   188,   257,   258,     1,   134,   262,    42,
     135,   173,   194,   194,   271,   188,   135,   154,   291,   291,
     154,   159,   194,   161,   271,   154,   205,   154,   205,   154,
     114,   252,   154,   154,    33,    63,   194,   154,   160,   160,
     171,   135,   160,   291,   135,   162,   135,   162,   164,   284,
      42,   135,   164,   284,   112,   135,   164,     7,     1,   160,
     179,    56,   134,   214,   159,   225,   154,   291,   291,   291,
     291,   201,   291,   201,   291,   291,   291,   291,   291,   291,
     291,    30,    54,    98,   189,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   293,   293,   293,   293,   293,   271,   271,   201,   291,
     201,   291,    24,    42,    79,   104,   283,   286,   287,   291,
     306,    29,    42,    29,    42,    91,    42,   164,   201,   291,
     194,   154,   154,   291,   291,   114,   162,   135,   154,   188,
     255,    42,   267,    40,   291,   201,   291,   161,   194,    40,
     291,   201,   291,   194,   157,   180,   182,   291,   180,   181,
     173,   291,    31,   291,   162,    24,    42,    45,    65,    68,
      79,    98,   172,   233,   234,   235,   236,   222,   258,   135,
     162,    30,    43,    86,    90,   163,   192,   263,   275,   114,
     259,   291,   256,   162,   243,   291,     1,   210,   271,   162,
      18,   206,   263,   275,   135,   182,   181,   160,   162,   162,
     269,   162,   269,   173,   164,   201,   291,   164,   173,   291,
     164,   291,   164,   291,   160,   160,   188,   267,   159,     1,
     188,   218,   219,    24,    68,    79,    98,   227,   237,   154,
     154,   154,   154,   154,   162,   164,    42,    79,   135,   162,
     274,    78,    78,    40,   201,   291,    40,   201,   291,   201,
     291,   281,   281,   154,   243,   293,   273,   214,   154,   291,
     162,   291,    29,   194,    29,   194,   285,   286,   291,    29,
     194,   284,    29,   194,   284,   135,   154,    11,   154,    31,
      31,   173,    91,   178,    42,    79,   235,   135,   162,   161,
     188,    24,    68,    79,    98,   239,   162,   258,   262,     1,
     267,    59,   293,   160,    64,   106,   160,   211,   162,   161,
     178,   188,   207,   251,   154,   154,   173,   164,   284,   164,
     284,   173,   112,     1,   215,   160,   114,   135,   160,    79,
     226,     1,     3,    10,    15,    17,    22,    36,    41,    46,
      48,    54,    60,    61,    76,    88,    92,    95,    99,   105,
     129,   130,   131,   132,   133,   136,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   152,   155,   156,
     157,   158,   161,   187,   188,   190,   228,   229,   230,   274,
     286,   262,   274,   274,   291,    29,    29,   291,    29,    29,
     164,   164,   194,   194,   159,   194,    91,    40,   291,    40,
     291,   135,   162,    91,    40,   291,   194,    40,   291,   194,
     291,   291,   173,   291,   173,    31,   188,   234,   258,   134,
     266,    79,   262,   259,   164,    42,   164,    29,   173,   267,
     207,   134,   178,    40,   173,   291,   164,    40,   173,   291,
     164,   291,   160,    35,    72,    75,   160,   172,   196,   291,
     219,   239,   161,   231,   188,   291,   129,   137,   231,   231,
     259,    91,    40,    40,    91,    40,    40,    40,    40,     1,
     215,   291,   291,   291,   286,   291,   291,   291,    11,    31,
     173,   266,   162,   163,   192,   243,   265,   275,   139,   245,
     259,    52,   107,   246,   291,   263,   275,   173,   194,   162,
     291,   291,   173,   291,   173,   196,     1,   134,   261,   233,
     162,     3,    92,   229,   230,   291,   291,   291,   291,   291,
     291,   160,   160,    91,    91,    91,    91,   291,   173,   259,
     266,   164,   267,   243,   244,   291,     3,    81,    92,   247,
     248,   249,   291,   178,   195,   242,   164,   178,    91,    91,
     163,   192,   260,   275,    94,   240,   162,   231,   231,    91,
      91,    91,    91,    91,    91,   291,   291,   291,   291,   245,
     259,   243,   264,   265,   275,    42,   164,   249,   107,   135,
     114,   140,   142,   143,   146,   147,    52,   275,   291,   291,
       1,   164,   267,   246,   291,   264,   265,   291,   248,   249,
     249,   249,   249,   249,   249,   247,   164,   260,   275,   164,
     154,   241,   242,   164,   260,   275,   264
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
     186,   186,   187,   187,   187,   187,   187,   187,   188,   188,
     188,   188,   188,   188,   189,   189,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   192,   192,   192,   192,   193,   193,
     194,   194,   195,   195,   196,   196,   196,   196,   196,   196,
     196,   196,   197,   197,   198,   198,   198,   198,   199,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   201,   202,   202,   202,   202,
     202,   202,   202,   202,   203,   204,   204,   204,   204,   204,
     204,   205,   205,   206,   206,   206,   207,   207,   208,   209,
     209,   210,   210,   211,   211,   211,   212,   212,   212,   212,
     213,   213,   213,   214,   214,   215,   215,   215,   216,   216,
     217,   218,   218,   218,   219,   219,   221,   222,   220,   223,
     223,   223,   223,   223,   225,   226,   224,   227,   227,   227,
     227,   227,   228,   228,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   231,   231,   232,   233,   233,   233,
     234,   234,   234,   234,   234,   235,   235,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   237,   237,   237,   237,
     237,   237,   238,   238,   239,   239,   239,   239,   239,   239,
     240,   240,   241,   241,   242,   242,   243,   243,   244,   244,
     245,   245,   246,   246,   246,   246,   246,   247,   247,   248,
     248,   248,   248,   248,   248,   248,   249,   249,   250,   250,
     250,   251,   251,   252,   252,   252,   253,   253,   253,   253,
     253,   254,   254,   254,   255,   255,   256,   256,   257,   257,
     257,   258,   258,   258,   259,   259,   259,   260,   260,   260,
     260,   260,   260,   260,   261,   261,   261,   261,   261,   262,
     262,   262,   262,   262,   263,   263,   263,   263,   264,   264,
     264,   265,   265,   265,   265,   265,   266,   266,   266,   266,
     266,   267,   267,   267,   267,   268,   268,   269,   269,   269,
     270,   270,   271,   271,   272,   272,   273,   273,   273,   273,
     274,   274,   275,   275,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   278,   279,   280,   280,   280,   280,   280,   280,   280,
     280,   281,   281,   282,   283,   283,   284,   285,   285,   286,
     286,   286,   287,   287,   287,   287,   287,   287,   288,   288,
     289,   290,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   292,   292,   293,   293,
     293,   294,   294,   294,   294,   295,   295,   295,   296,   296,
     296,   297,   297,   297,   297,   298,   298,   298,   298,   299,
     299,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   301,   301,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   303,
     303,   303,   303,   303,   303,   303,   304,   304,   304,   304,
     305,   305,   305,   305,   306,   306,   306,   306,   306,   306,
     306
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
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       2,     1,     2,     3,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     3,     5,     5,     2,     2,     5,
       3,     6,     6,     4,     5,     5,     3,     3,     6,     5,
       6,     5,     6,     3,     4,     3,     4,     6,     7,     6,
       7,     4,     5,     4,     5,     4,     4,     3,     6,     5,
       6,     5,     8,     7,     2,     3,     3,     2,     2,     3,
       3,     0,     2,     2,     3,     5,     1,     3,     3,     5,
       5,     0,     2,     3,     2,     3,     6,     8,     6,     8,
       1,     1,     1,     0,     2,     0,     2,     3,     5,     5,
       1,     1,     2,     3,     1,     3,     0,     0,     8,     0,
       1,     2,     2,     1,     0,     0,    10,     3,     3,     5,
       5,     3,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     3,     0,     1,     3,
       4,     5,     4,     6,     6,     0,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     2,     1,     0,     1,
       2,     2,     0,     2,     2,     4,     4,     1,     3,     3,
       3,     3,     3,     3,     3,     2,     1,     1,     3,     4,
       4,     2,     4,     0,     2,     2,     1,     1,     1,     2,
       1,     4,     5,     3,     1,     3,     3,     5,     1,     1,
       3,     1,     2,     3,     0,     2,     2,     3,     2,     4,
       3,     3,     4,     3,     0,     2,     2,     2,     1,     0,
       2,     2,     2,     1,     4,     4,     6,     3,     0,     1,
       1,     3,     4,     3,     4,     6,     0,     2,     2,     2,
       2,     1,     1,     3,     3,     1,     3,     1,     1,     1,
       3,     3,     0,     1,     1,     3,     3,     3,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     2,
       4,     4,     4,     5,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     2,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     4,     1,     3,     4,
       3,     3,     1,     1,     2,     1,     2,     1,     1,     3,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     3,     2,     2,     1,     0,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     1,     4,     4,
       4,     3,     3,     3,     3,     3,     4,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       3,     4,     3,     4,     3,     5,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1
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
#line 488 "chapel.ypp" /* yacc.c:1666  */
    { yyblock = (yyval.pblockstmt); }
#line 5650 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 3:
#line 493 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5656 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 4:
#line 494 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5662 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 6:
#line 501 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5668 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 7:
#line 506 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5674 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 8:
#line 507 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5680 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 23:
#line 526 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5686 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 24:
#line 527 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5692 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 25:
#line 528 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5698 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 26:
#line 529 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5704 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 27:
#line 530 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5710 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 28:
#line 531 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5716 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 29:
#line 532 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5722 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 30:
#line 533 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5728 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 31:
#line 534 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5734 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 32:
#line 535 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5740 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 33:
#line 536 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5746 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 34:
#line 537 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5752 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 35:
#line 538 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5758 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 36:
#line 539 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5764 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 37:
#line 540 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5770 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 38:
#line 541 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5776 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 39:
#line 546 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 5790 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 40:
#line 559 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 5799 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 41:
#line 564 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 5808 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 42:
#line 569 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 5817 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 43:
#line 576 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5823 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 44:
#line 577 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5829 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 45:
#line 578 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5835 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 46:
#line 582 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; }
#line 5841 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 47:
#line 583 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true;  }
#line 5847 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 48:
#line 596 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5853 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 49:
#line 597 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5859 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 50:
#line 598 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5865 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 51:
#line 603 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5871 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 52:
#line 604 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5877 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 53:
#line 609 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5887 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 54:
#line 614 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5897 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 55:
#line 619 "chapel.ypp" /* yacc.c:1666  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5906 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 56:
#line 623 "chapel.ypp" /* yacc.c:1666  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5915 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 57:
#line 630 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5925 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 59:
#line 639 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5935 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 61:
#line 648 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5941 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 62:
#line 649 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5947 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 63:
#line 650 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5953 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 64:
#line 654 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5959 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 65:
#line 659 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 5965 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 66:
#line 661 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5971 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 67:
#line 663 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5977 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 68:
#line 665 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5983 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 69:
#line 667 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5989 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 70:
#line 673 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = NULL; }
#line 5995 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 71:
#line 674 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6001 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 72:
#line 678 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6007 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 73:
#line 679 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6013 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 74:
#line 680 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "this"; }
#line 6019 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 75:
#line 681 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6025 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 76:
#line 682 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6031 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 77:
#line 683 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6037 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 78:
#line 686 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6043 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 79:
#line 687 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6049 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 80:
#line 688 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6055 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 81:
#line 689 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6061 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 82:
#line 690 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6067 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 83:
#line 691 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6073 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 84:
#line 703 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6079 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 85:
#line 704 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "this"; }
#line 6085 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 86:
#line 715 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "bool"; }
#line 6091 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 87:
#line 716 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "int"; }
#line 6097 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 88:
#line 717 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "uint"; }
#line 6103 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 89:
#line 718 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "real"; }
#line 6109 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 90:
#line 719 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "imag"; }
#line 6115 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 91:
#line 720 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "complex"; }
#line 6121 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 92:
#line 721 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "string"; }
#line 6127 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 93:
#line 722 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "sync"; }
#line 6133 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 94:
#line 723 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "single"; }
#line 6139 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 95:
#line 724 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "owned"; }
#line 6145 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 96:
#line 725 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "shared"; }
#line 6151 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 97:
#line 726 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "borrowed"; }
#line 6157 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 98:
#line 727 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "unmanaged"; }
#line 6163 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 99:
#line 728 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "domain"; }
#line 6169 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 100:
#line 729 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "index"; }
#line 6175 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 101:
#line 730 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "locale"; }
#line 6181 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 102:
#line 731 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "nothing"; }
#line 6187 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 103:
#line 732 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "void"; }
#line 6193 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 104:
#line 736 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6199 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 105:
#line 737 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6205 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 106:
#line 738 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6211 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 107:
#line 739 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6217 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 108:
#line 740 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6223 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 109:
#line 741 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6229 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 110:
#line 742 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6235 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 111:
#line 743 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6241 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 112:
#line 744 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 6247 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 113:
#line 745 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 6253 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 114:
#line 752 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_syncvar"; }
#line 6259 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 115:
#line 753 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_singlevar"; }
#line 6265 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 116:
#line 758 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_domain"; }
#line 6271 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 117:
#line 759 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "_index"; }
#line 6277 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 118:
#line 764 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = NULL; }
#line 6283 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 119:
#line 765 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6289 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 120:
#line 769 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6295 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 121:
#line 770 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6301 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 122:
#line 774 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6307 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 123:
#line 775 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6313 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 124:
#line 779 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6319 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 126:
#line 781 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6325 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 127:
#line 782 "chapel.ypp" /* yacc.c:1666  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6331 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 134:
#line 795 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6337 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 135:
#line 796 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6343 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 136:
#line 797 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6349 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 137:
#line 798 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6355 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 138:
#line 803 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6363 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 139:
#line 809 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6369 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 140:
#line 810 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6375 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 141:
#line 811 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 6381 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 142:
#line 812 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6387 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 143:
#line 813 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 6393 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 144:
#line 814 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6399 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 145:
#line 815 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6405 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 146:
#line 816 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6411 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 147:
#line 817 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6417 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 148:
#line 818 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6423 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 149:
#line 819 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6429 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 150:
#line 820 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6435 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 151:
#line 821 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6441 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 152:
#line 822 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6447 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 153:
#line 823 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6453 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 154:
#line 824 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6459 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 155:
#line 825 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6465 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 156:
#line 826 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6471 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 157:
#line 828 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6481 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 158:
#line 834 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6491 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 159:
#line 840 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6501 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 160:
#line 846 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6511 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 161:
#line 852 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6522 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 162:
#line 859 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6533 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 163:
#line 866 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6541 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 164:
#line 870 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6549 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 165:
#line 876 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 6555 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 166:
#line 880 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6561 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 167:
#line 881 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6567 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 168:
#line 882 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6573 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 169:
#line 883 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6579 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 170:
#line 884 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6586 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 171:
#line 886 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6593 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 172:
#line 888 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6600 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 173:
#line 890 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6607 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 174:
#line 895 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6613 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 175:
#line 898 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6619 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 176:
#line 899 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6625 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 177:
#line 900 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6631 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 178:
#line 901 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6637 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 179:
#line 902 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6643 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 180:
#line 903 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6649 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 181:
#line 907 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6655 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 182:
#line 908 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6661 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 183:
#line 912 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6667 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 184:
#line 913 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 6673 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 185:
#line 914 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 6679 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 186:
#line 918 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6685 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 187:
#line 919 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6691 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 188:
#line 923 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6697 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 189:
#line 927 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6703 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 190:
#line 929 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6709 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 191:
#line 933 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6715 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 192:
#line 934 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6721 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 193:
#line 939 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6727 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 194:
#line 941 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6733 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 195:
#line 943 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6739 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 196:
#line 950 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6753 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 197:
#line 960 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6767 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 198:
#line 970 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6781 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 199:
#line 980 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6795 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 200:
#line 992 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6805 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 201:
#line 997 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6815 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 202:
#line 1002 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6825 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 203:
#line 1010 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = NULL; }
#line 6831 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 204:
#line 1011 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6837 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 205:
#line 1016 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6843 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 206:
#line 1018 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6849 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 207:
#line 1020 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6855 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 208:
#line 1025 "chapel.ypp" /* yacc.c:1666  */
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
#line 6875 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 209:
#line 1041 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6883 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 210:
#line 1048 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6893 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 211:
#line 1057 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6905 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 212:
#line 1065 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6913 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 213:
#line 1069 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6921 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 214:
#line 1075 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6927 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 215:
#line 1076 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6933 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 216:
#line 1081 "chapel.ypp" /* yacc.c:1666  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6942 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 217:
#line 1086 "chapel.ypp" /* yacc.c:1666  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6951 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 218:
#line 1091 "chapel.ypp" /* yacc.c:1666  */
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
#line 6973 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 219:
#line 1113 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6984 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 220:
#line 1119 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6996 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 221:
#line 1126 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7007 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 222:
#line 1132 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7018 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 223:
#line 1138 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7030 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 224:
#line 1149 "chapel.ypp" /* yacc.c:1666  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 7040 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 225:
#line 1155 "chapel.ypp" /* yacc.c:1666  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7051 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 226:
#line 1162 "chapel.ypp" /* yacc.c:1666  */
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
#line 7078 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 227:
#line 1188 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 7086 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 228:
#line 1192 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7095 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 229:
#line 1197 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 7103 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 230:
#line 1201 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7112 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 231:
#line 1206 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 7120 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 233:
#line 1213 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7126 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 234:
#line 1217 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7132 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 235:
#line 1218 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 7138 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 236:
#line 1219 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "&"; }
#line 7144 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 237:
#line 1220 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "|"; }
#line 7150 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 238:
#line 1221 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "^"; }
#line 7156 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 239:
#line 1222 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "~"; }
#line 7162 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 240:
#line 1223 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "=="; }
#line 7168 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 241:
#line 1224 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "!="; }
#line 7174 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 242:
#line 1225 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<="; }
#line 7180 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 243:
#line 1226 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">="; }
#line 7186 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 244:
#line 1227 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<"; }
#line 7192 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 245:
#line 1228 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">"; }
#line 7198 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 246:
#line 1229 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "+"; }
#line 7204 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 247:
#line 1230 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "-"; }
#line 7210 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 248:
#line 1231 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "*"; }
#line 7216 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 249:
#line 1232 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "/"; }
#line 7222 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 250:
#line 1233 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<<"; }
#line 7228 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 251:
#line 1234 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">>"; }
#line 7234 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 252:
#line 1235 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "%"; }
#line 7240 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 253:
#line 1236 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "**"; }
#line 7246 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 254:
#line 1237 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "!"; }
#line 7252 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 255:
#line 1238 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "chpl_by"; }
#line 7258 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 256:
#line 1239 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "#"; }
#line 7264 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 257:
#line 1240 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "chpl_align"; }
#line 7270 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 258:
#line 1241 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<=>"; }
#line 7276 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 259:
#line 1242 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<~>"; }
#line 7282 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 260:
#line 1243 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "init="; }
#line 7288 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 261:
#line 1244 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 7294 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 262:
#line 1248 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "="; }
#line 7300 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 263:
#line 1249 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "+="; }
#line 7306 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 264:
#line 1250 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "-="; }
#line 7312 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 265:
#line 1251 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "*="; }
#line 7318 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 266:
#line 1252 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "/="; }
#line 7324 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 267:
#line 1253 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "%="; }
#line 7330 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 268:
#line 1254 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "**="; }
#line 7336 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 269:
#line 1255 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "&="; }
#line 7342 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 270:
#line 1256 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "|="; }
#line 7348 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 271:
#line 1257 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "^="; }
#line 7354 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 272:
#line 1258 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">>="; }
#line 7360 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 273:
#line 1259 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<<="; }
#line 7366 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 274:
#line 1263 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7372 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 275:
#line 1264 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7378 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 276:
#line 1268 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7384 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 277:
#line 1272 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7390 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 278:
#line 1273 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7396 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 279:
#line 1274 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7402 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 280:
#line 1279 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7408 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 281:
#line 1281 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7414 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 282:
#line 1283 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7420 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 283:
#line 1285 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7426 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 284:
#line 1287 "chapel.ypp" /* yacc.c:1666  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7432 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 285:
#line 1291 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7438 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 286:
#line 1292 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7444 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 287:
#line 1296 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_IN; }
#line 7450 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 288:
#line 1297 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7456 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 289:
#line 1298 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_OUT; }
#line 7462 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 290:
#line 1299 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST; }
#line 7468 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 291:
#line 1300 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7474 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 292:
#line 1301 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7480 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 293:
#line 1302 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7486 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 294:
#line 1303 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_REF; }
#line 7492 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 295:
#line 1304 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7498 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 296:
#line 1308 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7504 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 297:
#line 1309 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7510 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 298:
#line 1310 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_REF;   }
#line 7516 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 299:
#line 1311 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 7522 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 300:
#line 1312 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST;   }
#line 7528 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 301:
#line 1313 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 7534 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 302:
#line 1317 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 7540 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 303:
#line 1318 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 7546 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 304:
#line 1322 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_VALUE; }
#line 7552 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 305:
#line 1323 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_VALUE; }
#line 7558 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 306:
#line 1324 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 7564 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 307:
#line 1325 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_REF; }
#line 7570 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 308:
#line 1326 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_PARAM; }
#line 7576 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 309:
#line 1327 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_TYPE; }
#line 7582 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 310:
#line 1331 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; }
#line 7588 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 311:
#line 1332 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true;  }
#line 7594 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 312:
#line 1335 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = NULL; }
#line 7600 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 315:
#line 1341 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7606 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 316:
#line 1346 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7612 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 317:
#line 1348 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7618 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 318:
#line 1352 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7624 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 320:
#line 1357 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7630 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 321:
#line 1358 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7636 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 322:
#line 1362 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 7642 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 323:
#line 1364 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 7648 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 324:
#line 1366 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 7654 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 325:
#line 1368 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7660 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 326:
#line 1370 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7666 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 327:
#line 1374 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7672 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 328:
#line 1375 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7678 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 329:
#line 1378 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7684 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 330:
#line 1379 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7690 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 331:
#line 1380 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7696 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 332:
#line 1381 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7702 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 333:
#line 1382 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7708 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 334:
#line 1383 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7714 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 335:
#line 1384 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 7720 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 336:
#line 1387 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 7726 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 337:
#line 1388 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 7732 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 338:
#line 1392 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7738 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 339:
#line 1394 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7744 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 340:
#line 1396 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7750 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 341:
#line 1401 "chapel.ypp" /* yacc.c:1666  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7767 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 342:
#line 1414 "chapel.ypp" /* yacc.c:1666  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7785 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 343:
#line 1430 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7791 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 344:
#line 1432 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7797 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 345:
#line 1434 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7803 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 346:
#line 1438 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 7809 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 347:
#line 1439 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 7815 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 348:
#line 1440 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 7821 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 349:
#line 1441 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 7827 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 350:
#line 1442 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 7833 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 351:
#line 1447 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7843 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 352:
#line 1453 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
      context->latestComment = NULL;
    }
#line 7853 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 353:
#line 1459 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7862 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 355:
#line 1468 "chapel.ypp" /* yacc.c:1666  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7871 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 356:
#line 1476 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7877 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 357:
#line 1478 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7883 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 358:
#line 1483 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7889 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 359:
#line 1485 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7895 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 360:
#line 1487 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7901 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 361:
#line 1492 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7907 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 362:
#line 1494 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7913 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 363:
#line 1496 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7919 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 364:
#line 1502 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7925 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 365:
#line 1503 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7931 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 366:
#line 1504 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7937 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 367:
#line 1510 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7943 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 368:
#line 1512 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7949 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 369:
#line 1514 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7957 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 370:
#line 1518 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7965 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 371:
#line 1522 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7971 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 372:
#line 1524 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7979 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 373:
#line 1528 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7987 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 374:
#line 1535 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7993 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 375:
#line 1536 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7999 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 376:
#line 1537 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8005 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 377:
#line 1538 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8011 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 378:
#line 1539 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8017 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 379:
#line 1544 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8023 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 380:
#line 1545 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8029 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 381:
#line 1546 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 8035 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 382:
#line 1547 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8041 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 383:
#line 1548 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8047 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 384:
#line 1569 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8055 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 385:
#line 1573 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 8063 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 386:
#line 1577 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 8075 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 387:
#line 1585 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8083 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 388:
#line 1591 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8089 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 389:
#line 1592 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8095 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 390:
#line 1593 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8101 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 391:
#line 1598 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8107 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 392:
#line 1600 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8113 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 393:
#line 1606 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8119 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 394:
#line 1608 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8125 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 395:
#line 1610 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 8131 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 396:
#line 1614 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8137 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 397:
#line 1615 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8143 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 398:
#line 1616 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8149 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 399:
#line 1617 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8155 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 400:
#line 1618 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8161 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 401:
#line 1624 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8167 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 402:
#line 1625 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8173 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 403:
#line 1626 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8179 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 404:
#line 1627 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8185 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 405:
#line 1631 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 8191 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 406:
#line 1632 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8197 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 407:
#line 1636 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 8203 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 408:
#line 1637 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8209 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 409:
#line 1638 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8215 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 410:
#line 1642 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8221 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 411:
#line 1643 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8227 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 412:
#line 1647 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 8233 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 414:
#line 1652 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8239 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 415:
#line 1653 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8245 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 416:
#line 1657 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 8251 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 417:
#line 1658 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8257 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 418:
#line 1659 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8263 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 419:
#line 1660 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8269 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 420:
#line 1664 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8275 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 421:
#line 1665 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8281 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 422:
#line 1677 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8287 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 423:
#line 1679 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS, (yyvsp[-1].pexpr)); }
#line 8293 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 429:
#line 1689 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 8299 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 430:
#line 1691 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 8305 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 431:
#line 1693 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 8311 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 432:
#line 1695 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 8317 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 433:
#line 1697 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 8323 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 434:
#line 1699 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 8329 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 435:
#line 1701 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 8335 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 436:
#line 1704 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 8341 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 437:
#line 1706 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8347 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 438:
#line 1708 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_unmanaged"); }
#line 8353 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 439:
#line 1710 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 8359 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 440:
#line 1712 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 8365 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 441:
#line 1714 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8371 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 442:
#line 1716 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_borrowed"); }
#line 8377 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 443:
#line 1718 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 8383 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 444:
#line 1723 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8389 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 445:
#line 1725 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8395 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 446:
#line 1727 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8401 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 447:
#line 1729 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8407 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 448:
#line 1731 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8413 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 449:
#line 1733 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8419 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 450:
#line 1735 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8425 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 451:
#line 1737 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8431 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 452:
#line 1739 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8437 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 453:
#line 1741 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8443 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 454:
#line 1743 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8449 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 455:
#line 1745 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8455 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 456:
#line 1747 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8466 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 457:
#line 1754 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8476 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 458:
#line 1760 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8486 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 459:
#line 1766 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8496 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 460:
#line 1772 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 8506 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 461:
#line 1781 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8512 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 462:
#line 1790 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 8518 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 470:
#line 1806 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8524 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 471:
#line 1810 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = NULL; }
#line 8530 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 473:
#line 1815 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8536 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 474:
#line 1819 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8542 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 475:
#line 1820 "chapel.ypp" /* yacc.c:1666  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8548 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 476:
#line 1824 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8554 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 477:
#line 1828 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8560 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 478:
#line 1829 "chapel.ypp" /* yacc.c:1666  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8566 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 479:
#line 1834 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 8574 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 480:
#line 1838 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8582 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 481:
#line 1842 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8590 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 482:
#line 1848 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 8596 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 483:
#line 1849 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8602 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 484:
#line 1850 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8608 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 485:
#line 1851 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8614 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 486:
#line 1852 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8620 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 487:
#line 1853 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8626 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 489:
#line 1859 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8632 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 490:
#line 1864 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8638 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 491:
#line 1869 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8644 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 500:
#line 1885 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8650 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 501:
#line 1887 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8656 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 502:
#line 1889 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8662 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 503:
#line 1891 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8668 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 504:
#line 1893 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8674 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 505:
#line 1895 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8680 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 506:
#line 1899 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8686 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 507:
#line 1900 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8692 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 508:
#line 1903 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8698 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 509:
#line 1904 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8704 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 510:
#line 1905 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8710 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 515:
#line 1922 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8716 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 516:
#line 1923 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 8722 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 517:
#line 1924 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8728 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 518:
#line 1928 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8734 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 519:
#line 1929 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8740 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 520:
#line 1930 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8746 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 521:
#line 1934 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8752 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 522:
#line 1935 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8758 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 523:
#line 1936 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8764 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 524:
#line 1937 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 8770 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 525:
#line 1945 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8776 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 526:
#line 1946 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8782 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 527:
#line 1947 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8788 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 528:
#line 1948 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8794 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 529:
#line 1952 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 8800 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 530:
#line 1953 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 8806 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 532:
#line 1958 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 8812 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 533:
#line 1959 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8818 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 534:
#line 1960 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8824 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 535:
#line 1961 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8830 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 536:
#line 1962 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8836 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 537:
#line 1963 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 8842 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 538:
#line 1964 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8848 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 539:
#line 1965 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8854 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 540:
#line 1966 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8860 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 541:
#line 1967 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8866 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 542:
#line 1969 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8874 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 543:
#line 1973 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8882 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 544:
#line 1980 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8888 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 545:
#line 1981 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8894 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 546:
#line 1985 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8900 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 547:
#line 1986 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8906 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 548:
#line 1987 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8912 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 549:
#line 1988 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8918 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 550:
#line 1989 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8924 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 551:
#line 1990 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8930 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 552:
#line 1991 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8936 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 553:
#line 1992 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8942 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 554:
#line 1993 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8948 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 555:
#line 1994 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8954 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 556:
#line 1995 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8960 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 557:
#line 1996 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8966 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 558:
#line 1997 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8972 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 559:
#line 1998 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8978 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 560:
#line 1999 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8984 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 561:
#line 2000 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8990 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 562:
#line 2001 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8996 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 563:
#line 2002 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9002 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 564:
#line 2003 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9008 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 565:
#line 2004 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9014 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 566:
#line 2005 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9020 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 567:
#line 2006 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9026 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 568:
#line 2007 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9032 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 569:
#line 2011 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 9038 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 570:
#line 2012 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 9044 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 571:
#line 2013 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 9050 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 572:
#line 2014 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 9056 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 573:
#line 2015 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 9062 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 574:
#line 2016 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9068 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 575:
#line 2017 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 9074 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 576:
#line 2021 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9080 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 577:
#line 2022 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9086 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 578:
#line 2023 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9092 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 579:
#line 2024 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9098 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 580:
#line 2028 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9104 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 581:
#line 2029 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9110 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 582:
#line 2030 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9116 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 583:
#line 2031 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9122 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 584:
#line 2036 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 9128 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 585:
#line 2037 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 9134 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 586:
#line 2038 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 9140 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 587:
#line 2039 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 9146 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 588:
#line 2040 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 9152 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 589:
#line 2041 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 9158 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 590:
#line 2042 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 9164 "bison-chapel.cpp" /* yacc.c:1666  */
    break;


#line 9168 "bison-chapel.cpp" /* yacc.c:1666  */
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
