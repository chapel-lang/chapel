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
#line 33 "chapel.ypp" /* yacc.c:355  */

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;
#line 47 "chapel.ypp" /* yacc.c:355  */

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif
#line 67 "chapel.ypp" /* yacc.c:355  */

  #ifndef _BISON_CHAPEL_DEFINES_1_
  #define _BISON_CHAPEL_DEFINES_1_

  #include "symbol.h"

  #include "ImportStmt.h"

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
  class NamedExpr;
  class Type;

  enum   ProcIterOp {
    ProcIterOp_PROC,
    ProcIterOp_ITER,
    ProcIterOp_OP
  };

  struct PotentialRename {
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
    ImportStmt*               pimportstmt;
    ProcIterOp                procIterOp;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<PotentialRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif
#line 152 "chapel.ypp" /* yacc.c:355  */

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
#line 175 "chapel.ypp" /* yacc.c:355  */

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

#line 243 "bison-chapel.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TIDENT = 258,
    TQUERIEDIDENT = 259,
    INTLITERAL = 260,
    REALLITERAL = 261,
    IMAGLITERAL = 262,
    STRINGLITERAL = 263,
    BYTESLITERAL = 264,
    CSTRINGLITERAL = 265,
    EXTERNCODE = 266,
    TALIGN = 267,
    TAS = 268,
    TATOMIC = 269,
    TBEGIN = 270,
    TBREAK = 271,
    TBOOL = 272,
    TBORROWED = 273,
    TBY = 274,
    TBYTES = 275,
    TCATCH = 276,
    TCLASS = 277,
    TCOBEGIN = 278,
    TCOFORALL = 279,
    TCOMPLEX = 280,
    TCONFIG = 281,
    TCONST = 282,
    TCONTINUE = 283,
    TDEFER = 284,
    TDELETE = 285,
    TDMAPPED = 286,
    TDO = 287,
    TDOMAIN = 288,
    TELSE = 289,
    TENUM = 290,
    TEXCEPT = 291,
    TEXPORT = 292,
    TEXTERN = 293,
    TFALSE = 294,
    TFOR = 295,
    TFORALL = 296,
    TFOREACH = 297,
    TFORWARDING = 298,
    TIF = 299,
    TIMAG = 300,
    TIMPORT = 301,
    TIN = 302,
    TINCLUDE = 303,
    TINDEX = 304,
    TINLINE = 305,
    TINOUT = 306,
    TINT = 307,
    TITER = 308,
    TINITEQUALS = 309,
    TIMPLEMENTS = 310,
    TINTERFACE = 311,
    TLABEL = 312,
    TLAMBDA = 313,
    TLET = 314,
    TLIFETIME = 315,
    TLOCAL = 316,
    TLOCALE = 317,
    TMINUSMINUS = 318,
    TMODULE = 319,
    TNEW = 320,
    TNIL = 321,
    TNOINIT = 322,
    TNONE = 323,
    TNOTHING = 324,
    TON = 325,
    TONLY = 326,
    TOPERATOR = 327,
    TOTHERWISE = 328,
    TOUT = 329,
    TOVERRIDE = 330,
    TOWNED = 331,
    TPARAM = 332,
    TPLUSPLUS = 333,
    TPRAGMA = 334,
    TPRIMITIVE = 335,
    TPRIVATE = 336,
    TPROC = 337,
    TPROTOTYPE = 338,
    TPUBLIC = 339,
    TREAL = 340,
    TRECORD = 341,
    TREDUCE = 342,
    TREF = 343,
    TREQUIRE = 344,
    TRETURN = 345,
    TSCAN = 346,
    TSELECT = 347,
    TSERIAL = 348,
    TSHARED = 349,
    TSINGLE = 350,
    TSPARSE = 351,
    TSTRING = 352,
    TSUBDOMAIN = 353,
    TSYNC = 354,
    TTHEN = 355,
    TTHIS = 356,
    TTHROW = 357,
    TTHROWS = 358,
    TTRUE = 359,
    TTRY = 360,
    TTRYBANG = 361,
    TTYPE = 362,
    TUINT = 363,
    TUNDERSCORE = 364,
    TUNION = 365,
    TUNMANAGED = 366,
    TUSE = 367,
    TVAR = 368,
    TVOID = 369,
    TWHEN = 370,
    TWHERE = 371,
    TWHILE = 372,
    TWITH = 373,
    TYIELD = 374,
    TZIP = 375,
    TALIAS = 376,
    TAND = 377,
    TASSIGN = 378,
    TASSIGNBAND = 379,
    TASSIGNBOR = 380,
    TASSIGNBXOR = 381,
    TASSIGNDIVIDE = 382,
    TASSIGNEXP = 383,
    TASSIGNLAND = 384,
    TASSIGNLOR = 385,
    TASSIGNMINUS = 386,
    TASSIGNMOD = 387,
    TASSIGNMULTIPLY = 388,
    TASSIGNPLUS = 389,
    TASSIGNREDUCE = 390,
    TASSIGNSL = 391,
    TASSIGNSR = 392,
    TBANG = 393,
    TBAND = 394,
    TBNOT = 395,
    TBOR = 396,
    TBXOR = 397,
    TCOLON = 398,
    TCOMMA = 399,
    TDIVIDE = 400,
    TDOT = 401,
    TDOTDOT = 402,
    TDOTDOTDOT = 403,
    TEQUAL = 404,
    TEXP = 405,
    TGREATER = 406,
    TGREATEREQUAL = 407,
    THASH = 408,
    TIO = 409,
    TLESS = 410,
    TLESSEQUAL = 411,
    TMINUS = 412,
    TMOD = 413,
    TNOTEQUAL = 414,
    TOR = 415,
    TPLUS = 416,
    TQUESTION = 417,
    TSEMI = 418,
    TSHIFTLEFT = 419,
    TSHIFTRIGHT = 420,
    TSTAR = 421,
    TSWAP = 422,
    TLCBR = 423,
    TRCBR = 424,
    TLP = 425,
    TRP = 426,
    TLSBR = 427,
    TRSBR = 428,
    TNOELSE = 429,
    TDOTDOTOPENHIGH = 430,
    TUPLUS = 431,
    TUMINUS = 432,
    TLNOT = 433
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
#line 206 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 467 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 473 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 39 "chapel.ypp" /* yacc.c:359  */

  #include <string>
  int         captureTokens;
  std::string captureString;
  bool        parsingPrivate=false;
#line 214 "chapel.ypp" /* yacc.c:359  */

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

#line 546 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   20323

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  179
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  161
/* YYNRULES -- Number of rules.  */
#define YYNRULES  694
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1246

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   433

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
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   511,   511,   516,   517,   523,   524,   529,   530,   535,
     536,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   572,   585,   590,   595,   603,   604,   605,
     609,   610,   614,   615,   616,   621,   620,   641,   642,   643,
     648,   649,   654,   659,   664,   668,   677,   682,   687,   692,
     696,   700,   708,   713,   717,   722,   726,   727,   728,   732,
     733,   734,   735,   736,   737,   738,   742,   747,   748,   749,
     753,   754,   758,   762,   764,   766,   768,   770,   772,   779,
     780,   784,   785,   786,   787,   788,   789,   792,   793,   794,
     795,   796,   797,   809,   810,   821,   822,   823,   824,   825,
     826,   827,   828,   829,   830,   831,   832,   833,   834,   835,
     836,   837,   838,   839,   843,   844,   845,   846,   847,   848,
     849,   850,   851,   852,   853,   854,   861,   862,   863,   864,
     868,   869,   873,   874,   878,   879,   880,   890,   890,   895,
     896,   897,   898,   899,   900,   901,   905,   906,   907,   908,
     913,   912,   928,   927,   944,   943,   959,   958,   974,   978,
     983,   991,  1002,  1009,  1010,  1011,  1012,  1013,  1014,  1015,
    1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,
    1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,  1035,
    1036,  1042,  1048,  1054,  1060,  1067,  1074,  1078,  1085,  1089,
    1090,  1091,  1092,  1094,  1095,  1096,  1097,  1099,  1101,  1103,
    1105,  1110,  1111,  1115,  1117,  1125,  1126,  1131,  1136,  1137,
    1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,  1147,
    1148,  1155,  1156,  1157,  1158,  1167,  1168,  1172,  1174,  1177,
    1183,  1185,  1188,  1194,  1197,  1198,  1199,  1200,  1201,  1202,
    1206,  1207,  1211,  1212,  1213,  1217,  1218,  1222,  1225,  1227,
    1232,  1233,  1237,  1239,  1241,  1248,  1258,  1272,  1277,  1282,
    1290,  1291,  1296,  1297,  1299,  1304,  1320,  1327,  1336,  1344,
    1348,  1355,  1356,  1361,  1366,  1360,  1393,  1396,  1400,  1408,
    1418,  1407,  1457,  1461,  1466,  1470,  1475,  1482,  1483,  1487,
    1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,
    1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,  1507,
    1508,  1509,  1510,  1511,  1512,  1513,  1514,  1518,  1519,  1520,
    1521,  1522,  1523,  1524,  1525,  1526,  1527,  1528,  1529,  1533,
    1534,  1538,  1542,  1543,  1544,  1548,  1550,  1552,  1554,  1556,
    1558,  1563,  1564,  1568,  1569,  1570,  1571,  1572,  1573,  1574,
    1575,  1576,  1580,  1581,  1582,  1583,  1584,  1585,  1589,  1590,
    1591,  1595,  1596,  1597,  1598,  1599,  1600,  1604,  1605,  1608,
    1609,  1613,  1614,  1618,  1622,  1623,  1624,  1632,  1633,  1635,
    1637,  1639,  1644,  1646,  1651,  1652,  1653,  1654,  1655,  1656,
    1657,  1661,  1663,  1668,  1670,  1672,  1677,  1690,  1707,  1708,
    1710,  1715,  1716,  1717,  1718,  1719,  1723,  1729,  1737,  1738,
    1746,  1748,  1753,  1755,  1757,  1762,  1764,  1766,  1773,  1774,
    1775,  1780,  1782,  1784,  1788,  1792,  1794,  1798,  1806,  1807,
    1808,  1809,  1810,  1815,  1816,  1817,  1818,  1819,  1839,  1843,
    1847,  1855,  1862,  1863,  1864,  1868,  1870,  1876,  1878,  1880,
    1885,  1886,  1887,  1888,  1889,  1895,  1896,  1897,  1898,  1902,
    1903,  1907,  1908,  1909,  1913,  1914,  1918,  1919,  1923,  1924,
    1928,  1929,  1930,  1931,  1935,  1936,  1947,  1949,  1951,  1957,
    1958,  1959,  1960,  1961,  1962,  1964,  1966,  1968,  1970,  1972,
    1974,  1977,  1979,  1981,  1983,  1985,  1987,  1989,  1991,  1994,
    1996,  2001,  2003,  2005,  2007,  2009,  2011,  2013,  2015,  2017,
    2019,  2021,  2023,  2025,  2032,  2038,  2044,  2050,  2059,  2069,
    2077,  2078,  2079,  2080,  2081,  2082,  2083,  2084,  2089,  2090,
    2094,  2098,  2099,  2103,  2107,  2108,  2112,  2116,  2120,  2127,
    2128,  2129,  2130,  2131,  2132,  2136,  2137,  2142,  2144,  2148,
    2152,  2156,  2164,  2169,  2175,  2181,  2188,  2198,  2206,  2207,
    2208,  2209,  2210,  2211,  2212,  2213,  2214,  2215,  2217,  2219,
    2221,  2236,  2238,  2240,  2242,  2247,  2248,  2252,  2253,  2254,
    2258,  2259,  2260,  2261,  2270,  2271,  2272,  2273,  2274,  2278,
    2279,  2280,  2284,  2285,  2286,  2287,  2288,  2296,  2297,  2298,
    2299,  2303,  2304,  2308,  2309,  2313,  2314,  2315,  2316,  2317,
    2318,  2319,  2320,  2322,  2324,  2325,  2326,  2330,  2338,  2339,
    2343,  2344,  2345,  2346,  2347,  2348,  2349,  2350,  2351,  2352,
    2353,  2354,  2355,  2356,  2357,  2358,  2359,  2360,  2361,  2362,
    2363,  2364,  2365,  2370,  2371,  2372,  2373,  2374,  2375,  2376,
    2380,  2381,  2382,  2383,  2387,  2388,  2389,  2390,  2395,  2396,
    2397,  2398,  2399,  2400,  2401
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "TQUERIEDIDENT", "INTLITERAL",
  "REALLITERAL", "IMAGLITERAL", "STRINGLITERAL", "BYTESLITERAL",
  "CSTRINGLITERAL", "EXTERNCODE", "TALIGN", "TAS", "TATOMIC", "TBEGIN",
  "TBREAK", "TBOOL", "TBORROWED", "TBY", "TBYTES", "TCATCH", "TCLASS",
  "TCOBEGIN", "TCOFORALL", "TCOMPLEX", "TCONFIG", "TCONST", "TCONTINUE",
  "TDEFER", "TDELETE", "TDMAPPED", "TDO", "TDOMAIN", "TELSE", "TENUM",
  "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE", "TFOR", "TFORALL", "TFOREACH",
  "TFORWARDING", "TIF", "TIMAG", "TIMPORT", "TIN", "TINCLUDE", "TINDEX",
  "TINLINE", "TINOUT", "TINT", "TITER", "TINITEQUALS", "TIMPLEMENTS",
  "TINTERFACE", "TLABEL", "TLAMBDA", "TLET", "TLIFETIME", "TLOCAL",
  "TLOCALE", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TNONE",
  "TNOTHING", "TON", "TONLY", "TOPERATOR", "TOTHERWISE", "TOUT",
  "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE",
  "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC", "TREAL", "TRECORD",
  "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL",
  "TSHARED", "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN", "TSYNC",
  "TTHEN", "TTHIS", "TTHROW", "TTHROWS", "TTRUE", "TTRY", "TTRYBANG",
  "TTYPE", "TUINT", "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUSE", "TVAR",
  "TVOID", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP",
  "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR",
  "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR",
  "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY", "TASSIGNPLUS",
  "TASSIGNREDUCE", "TASSIGNSL", "TASSIGNSR", "TBANG", "TBAND", "TBNOT",
  "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT",
  "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL", "THASH",
  "TIO", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOTEQUAL", "TOR",
  "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR",
  "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR", "TNOELSE",
  "TDOTDOTOPENHIGH", "TUPLUS", "TUMINUS", "TLNOT", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_start", "module_decl_stmt", "access_control",
  "opt_prototype", "include_access_control", "include_module_stmt", "$@1",
  "block_stmt", "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls",
  "except_ls", "use_access_control", "use_stmt", "import_stmt",
  "import_expr", "import_ls", "require_stmt", "assignment_stmt",
  "opt_label_ident", "ident_fn_def", "ident_def", "ident_use",
  "internal_type_ident_def", "scalar_type", "reserved_type_ident_use",
  "do_stmt", "return_stmt", "class_level_stmt", "@2", "private_decl",
  "forwarding_stmt", "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@7", "$@8", "linkage_spec",
  "fn_decl_stmt", "$@9", "$@10", "fn_decl_stmt_inner",
  "fn_decl_receiver_expr", "fn_ident", "assignop_ident", "opt_formal_ls",
  "req_formal_ls", "formal_ls", "formal", "opt_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_iter_or_op",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "var_arg_expr", "opt_lifetime_where",
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
  "shadow_var_prefix", "io_expr", "new_maybe_decorated", "new_expr",
  "let_expr", "expr", "opt_expr", "opt_try_expr", "lhs_expr",
  "call_base_expr", "call_expr", "dot_expr", "parenthesized_expr",
  "bool_literal", "str_bytes_literal", "literal_expr", "assoc_expr_ls",
  "binary_op_expr", "unary_op_expr", "reduce_expr", "scan_expr",
  "reduce_scan_op_expr", YY_NULLPTR
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
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433
};
# endif

#define YYPACT_NINF -1062

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1062)))

#define YYTABLE_NINF -645

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1062,   108,  3730, -1062,   -66,   234, -1062, -1062, -1062, -1062,
   -1062, -1062,  5114,    10,   319,   274, 14999,   290,  3559,    10,
   11658,   329,   688,   340,   319,  5114, 11658,  5114,   282, 19957,
   11829,  8564,   389,  8735,  9940,  9940,  7188,  8906,   460, -1062,
     327, -1062,   463, 20042, 20042, 20042, -1062,  3284, 10111,   501,
   11658,   217, -1062,   505,   508, 11658, -1062, 14999, -1062, 11658,
     572,   417,   192, 18568,   543, 20083, -1062, 10284,  8049, 11658,
   10111, 14999, 11658,   493,   563,   458,  5114,   575, 11658,   576,
   12000, 12000, 20042,   578, -1062, 14999, -1062,   580,  8906, 11658,
   -1062, 11658, -1062, 11658, -1062, -1062, 14523, 11658, -1062, 11658,
   -1062, -1062, -1062,  4076,  7361,  9079, 11658, -1062,  4941, -1062,
     469, -1062,   562, -1062, -1062,   209, -1062, -1062, -1062, -1062,
   -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062,
   -1062,   591, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062,
   20042, -1062, 20042,   295,   200, -1062, -1062,  3284, -1062,   488,
   -1062,   491, -1062, -1062,   497,   500,   502, 11658,   504,   529,
   19602,  2210,    93,   535,   540, -1062, -1062,   244, -1062, -1062,
   -1062, -1062, -1062,   442, -1062, -1062, 19602,   494,  5114, -1062,
   -1062,   542, 11658, -1062, -1062, 11658, 11658, 11658, 20042, -1062,
   11658, 10284, 10284,   599,   367, -1062, -1062, -1062, -1062,   -32,
     387, -1062, -1062,   517, 16724, 20042,  3284, -1062,   545, -1062,
     150, 19602,   548,  8222,   586, 20168, 19602,   231,   602, -1062,
   20209, 20042,   231, 20042,   549,    31, 16324,    20, 16166,    20,
   16242,   448, -1062, 16407, 20042, 20042,   132, 15452,   346,  8222,
   -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062,
   -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062,
   -1062, -1062, -1062, -1062,   552, -1062,   403,  5114,   554,  1354,
      97,    41, -1062,  5114, -1062, -1062, 16805,   686, -1062,   555,
     556, -1062, 16805,   -32,   686, -1062,  8222,  1840, -1062, -1062,
   10455, -1062, -1062, -1062, -1062,   189, 19602, 11658, 11658, -1062,
   19602,   564, 17355, -1062, 16805,   -32, 19602,   559,  8222, -1062,
   19602, 17396, -1062, -1062, 17441,  3288, -1062, -1062, 17546,   583,
     567,   -32,    31, 16805, 17591,   479,   479,  1538,   686,   686,
     136, -1062, -1062,  4249,   -29, -1062, 11658, -1062,   -40,   117,
   -1062,   153,    23, 17640,   -44,  1538,   691, -1062,  4422, -1062,
     654, 11658, 11658, 20042,   589,   568, -1062, -1062, -1062, -1062,
     306,   402, -1062, 11658,   592, 11658, 11658, 11658,  9940,  9940,
   11658,   409, 11658, 11658, 11658, 11658, 11658,   287, 14523, 11658,
   11658, 11658, 11658, 11658, 11658, 11658, 11658, 11658, 11658, 11658,
   11658, 11658, 11658, 11658, 11658,   668, -1062, -1062, -1062, -1062,
   -1062,  9250,  9250, -1062, -1062, -1062, -1062,  9250, -1062, -1062,
    9250,  9250,  8222,  8222,  9940,  9940,  7878, -1062, -1062, 16881,
   16962, 17741,   577,    25, 20042,  4595, -1062,  9940,    31,   579,
     315, -1062, 11658, -1062, 11658,   623, -1062,   582,   604, -1062,
   -1062, -1062, 20042, -1062,  3284, -1062, 20042,   593, -1062,  3284,
     705, 10284, -1062,  5287,  9940, -1062,   584, -1062,    31,  5460,
    9940, -1062,    31, -1062,    31,  9940, -1062,    31, 12171, 11658,
   -1062,   632,   635,  5114,   725,  5114, -1062,   727, 11658, -1062,
   -1062,   562,   597,  8222, 20042, -1062, -1062,   461, -1062, -1062,
    1354, -1062,   619,   598, -1062, 12342,   641, 11658,  3284, -1062,
   -1062, 11658, 11658, -1062,   603, -1062, 10284, -1062, 19602, 19602,
   -1062,    50, -1062,  8222,   607, -1062,   754, -1062,   754, -1062,
   12513,   643, -1062, -1062, -1062, -1062, -1062, -1062, -1062,  9423,
   -1062, 17790,  7534, -1062,  7707, -1062,  5114,   606,  9940,  9596,
    3903,   612, 11658, 10626, -1062, -1062,   344, -1062,  4768, 20042,
   -1062,   318, 17830,   330, 16490,   103, 10284,   621, 19916,    19,
   -1062, 17947,  2939,  2939,   509, -1062,   509, -1062,   509,  2744,
     334,  1393,   703,   -32,   479, -1062,   626, -1062, -1062, -1062,
   -1062, -1062,  1538, 19757,   509,  1353,  1353,  2939,  1353,  1353,
     536,   479, 19757,  2468,   536,   686,   686,   479,  1538,   628,
     630,   634,   636,   642,   646,   627,   631, -1062,   509, -1062,
     509,    63, -1062, -1062, -1062,   124, -1062,  2157, 19719,   459,
   12684,  9940, 12855,  9940, 11658,  8222,  9940, 15271,   644,    10,
   17992, -1062, -1062, -1062, 19602, 18032,  8222, -1062,  8222, 20042,
     589,   358, 20042,   589, -1062,   360, 11658,   139,  8906, 19602,
      46, 17038,  7878, -1062,  8906, 19602,    21, 16566, -1062, -1062,
      20, 16648, -1062,   648,   669,   653, 18149,   669,   656, 11658,
   11658,   783,  5114,   787, 18189,  5114, 17120,   758, -1062,   159,
   -1062,   239, -1062,   204, -1062, -1062, -1062, -1062, -1062, -1062,
     808,   252, -1062,  3518, -1062,    59,   652,  1354,    97,    56,
      69, 11658, 11658,  7017, -1062, -1062,   560,  8393, -1062, 19602,
   -1062, 18229, 18346, -1062, -1062, 19602,   659,    80,   666, -1062,
    2099, -1062, -1062,   361, 20042, -1062, -1062, -1062, -1062, -1062,
   -1062, -1062,  5114,   158, 17196, -1062, -1062, 19602,  5114, 19602,
   -1062, 18387, -1062, -1062, -1062, 11658, -1062,    51,   129, 11658,
   -1062, 10797, 12171, 11658, -1062,  8222,   694,  1039,   670,   717,
     115, -1062,   759, -1062, -1062, -1062, -1062, 14357,   679, -1062,
   -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062,  7878,
   -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062,
   -1062, -1062,    38,  9940,  9940, 11658,   819, 18427, 11658,   826,
   18577,   253,   687, 18617,  8222,    31,    31, -1062, -1062, -1062,
   -1062,   589,   695, -1062,   589,   696, -1062, 16805, -1062, 15528,
    5633, -1062,  5806, -1062,   263, -1062, 15610,  5979, -1062,    31,
    6152, -1062,    31, -1062,    31, -1062,    31, 11658, -1062, 11658,
   -1062, 19602, 19602,  5114, -1062,  5114, 11658, -1062,  5114,   828,
   20042,   707, 20042,   517, -1062, -1062, 20042,   857, -1062,  1354,
     723,   784, -1062, -1062, -1062,    43, -1062, -1062,   641,   698,
      58, -1062, -1062,   704,   706, -1062,  6325, 10284, -1062, -1062,
   -1062, 20042, -1062,   736,   517, -1062, -1062,  6498,   713,  6671,
     715, -1062, 11658, -1062, -1062, 11658, 18657,    66, 17279,   726,
     729,   264,   728,  1223, -1062, 11658, 20042, -1062, -1062,    59,
     730,   280, -1062,   749, -1062,   752,   753,   764,   757,   760,
   -1062,   761,   767,   765,   772,   773,   365,   775,   774,   776,
   -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062,
   -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062,
   -1062, -1062, -1062, 11658, -1062,   785,   786,   779,   730,   730,
   -1062, -1062, -1062,   641,   265,   305, 18774, 13026, 13197, 18814,
   13368, 13539, -1062, 13710, 13881,   324, -1062, -1062,   742, -1062,
     769, -1062, -1062,  5114,  8906, 19602,  8906, 19602,  7878, -1062,
    5114,  8906, 19602, -1062,  8906, 19602, -1062, -1062, -1062, 18854,
   19602, -1062, -1062, 19602,   893,  5114,   766, -1062, -1062, -1062,
     723, -1062,   762, 10970,   211, -1062,   182, -1062, -1062,  9940,
   15135,  8222,  8222,  5114, -1062,    39,   768, 11658, -1062,  8906,
   -1062, 19602,  5114,  8906, -1062, 19602,  5114, 19602,   268, 11141,
   12171, 11658, 12171, 11658, -1062, -1062,   777, -1062, -1062,  1840,
   -1062,  1725, -1062, 19602, -1062,    94,   461, -1062, 18971, -1062,
   15371, -1062, -1062, -1062, 11658, 11658, 11658, 11658, 11658, 11658,
   11658, 11658, -1062, -1062,  1820, -1062,  1929, 18189, 15686, 15768,
   -1062, 18189, 15844, 15926, 11658,  5114, -1062, -1062,   211,   723,
    9769, -1062, -1062, -1062,   338, 10284, -1062, -1062,   143, 11658,
       9, 19011, -1062,   710,   778,   781,   548, -1062,   517, 19602,
   16002, -1062, 16084, -1062, -1062, -1062, 19602,   780,   790,   791,
     794, -1062, -1062, -1062, 14052,   825,   343, -1062,   797,   800,
     730,   730, 19128, 19168, 19208, 19325, 19365, 19405,  2203, -1062,
    2425, -1062,  5114,  5114,  5114,  5114, 19602, -1062, -1062, -1062,
     211, 11314,    72, -1062, 19602, -1062,   127, -1062,     6, -1062,
     186, 19522, -1062, -1062, -1062, 13881,   770,   782, -1062,  5114,
    5114, -1062, -1062, -1062, -1062,  6844, -1062, -1062,   164, -1062,
     182, -1062, -1062, -1062, 11658, 11658, 11658, 11658, 11658, 11658,
   -1062, -1062, 18189, 18189, 18189, 18189, -1062, -1062, -1062, -1062,
   -1062,   525,  9940, 14693, -1062, 11658,   143,   127,   127,   127,
     127,   127,   127,   143,   870, -1062, -1062, 18189, 18189,   793,
   14223,    73,    -8, 19562, -1062, -1062, 19602, -1062, -1062, -1062,
   -1062, -1062, -1062, -1062,   803, -1062, -1062,   380, 14863, -1062,
   -1062, -1062, 11487, -1062,   581, -1062
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,   113,   637,   638,   639,   633,
     634,   640,     0,   558,    99,   134,   527,   141,   529,   558,
       0,   140,     0,   432,    99,     0,     0,     0,   255,   135,
     605,   605,   631,     0,     0,     0,     0,     0,   139,    55,
     256,   307,   136,     0,     0,     0,   303,     0,     0,   143,
       0,   577,   549,   641,   144,     0,   308,   521,   431,     0,
       0,     0,   157,   306,   138,   530,   433,     0,     0,     0,
       0,   525,     0,     0,   142,     0,     0,   114,     0,   632,
       0,     0,     0,   137,   289,   523,   435,   145,     0,     0,
     690,     0,   692,     0,   693,   694,   604,     0,   691,   688,
     508,   154,   689,     0,     0,     0,     0,     4,     0,     5,
       0,     9,    50,    10,    11,     0,    12,    13,    14,    16,
     504,   505,    26,    15,   155,   164,   165,    17,    21,    18,
      20,     0,   250,    19,   596,    23,    24,    25,    22,   163,
       0,   161,     0,   593,     0,   159,   162,     0,   160,   610,
     589,   506,   590,   511,   509,     0,     0,     0,   594,   595,
       0,   510,     0,   611,   612,   613,   635,   636,   588,   513,
     512,   591,   592,     0,    42,    28,   519,     0,     0,   559,
     100,     0,     0,   529,   135,     0,     0,     0,     0,   530,
       0,     0,     0,     0,   593,   610,   509,   594,   595,   528,
     510,   611,   612,     0,   558,     0,     0,   434,     0,   263,
       0,   489,     0,   496,   633,   530,   606,   306,   633,   182,
     530,     0,   306,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   169,     0,     0,     0,     0,     0,    52,   496,
     107,   115,   127,   121,   120,   129,   110,   119,   130,   116,
     131,   108,   132,   125,   118,   126,   124,   122,   123,   109,
     111,   117,   128,   133,     0,   112,     0,     0,     0,     0,
       0,     0,   438,     0,   151,    36,     0,   675,   581,   578,
     579,   580,     0,   522,   676,     7,   496,   306,   287,   297,
     605,   288,   156,   403,   486,     0,   485,     0,     0,   152,
     609,     0,     0,    39,     0,   526,   514,     0,   496,    40,
     520,     0,   270,   266,     0,   510,   270,   267,     0,   428,
       0,   524,     0,     0,     0,   677,   679,   602,   674,   673,
       0,    57,    60,     0,     0,   491,     0,   493,     0,     0,
     492,     0,     0,   485,     0,   603,     0,     6,     0,    51,
       0,     0,     0,     0,   290,     0,   389,   390,   388,   309,
       0,   507,    27,     0,   582,     0,     0,     0,     0,     0,
       0,   678,     0,     0,     0,     0,     0,     0,   601,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,   354,   355,   356,   351,
     353,     0,     0,   349,   352,   350,   348,     0,   358,   357,
       0,     0,   496,   496,     0,     0,     0,    29,    30,     0,
       0,     0,     0,     0,     0,     0,    31,     0,     0,     0,
       0,    32,     0,    33,     0,   504,   502,     0,   497,   498,
     503,   176,     0,   179,     0,   172,     0,     0,   178,     0,
       0,     0,   192,     0,     0,   191,     0,   200,     0,     0,
       0,   198,     0,   208,     0,     0,   206,     0,     0,    72,
     166,     0,     0,     0,   224,     0,   347,   220,     0,    54,
      53,    50,     0,     0,     0,   234,    34,   371,   304,   442,
       0,   443,   445,     0,   467,     0,   448,     0,     0,   150,
      35,     0,     0,    37,     0,   158,     0,    92,   607,   608,
     153,     0,    38,     0,     0,   277,   268,   264,   269,   265,
       0,   426,   423,   185,   184,    41,    59,    58,    61,     0,
     642,     0,     0,   627,     0,   629,     0,     0,     0,     0,
       0,     0,     0,     0,   646,     8,     0,    44,     0,     0,
      90,     0,    87,     0,    66,   261,     0,     0,     0,   382,
     437,   557,   670,   669,   672,   681,   680,   685,   684,   666,
     663,   664,   665,   598,   653,   113,     0,   624,   625,   114,
     623,   622,   599,   657,   668,   662,   660,   671,   661,   659,
     651,   656,   658,   667,   650,   654,   655,   652,   600,     0,
       0,     0,     0,     0,     0,     0,     0,   683,   682,   687,
     686,   569,   570,   572,   574,     0,   561,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   644,   261,   558,
     558,   188,   424,   436,   490,     0,     0,   516,     0,     0,
     290,     0,     0,   290,   425,     0,     0,     0,     0,   533,
       0,     0,     0,   201,     0,   539,     0,     0,   199,   209,
       0,     0,   207,   689,    75,     0,    62,    73,     0,     0,
       0,   223,     0,   219,     0,     0,     0,     0,   515,     0,
     237,     0,   235,   376,   373,   374,   375,   379,   380,   381,
     371,     0,   363,     0,   372,   391,     0,   446,     0,   148,
     149,   147,   146,     0,   466,   465,   589,     0,   440,   587,
     439,     0,     0,   621,   488,   487,     0,     0,     0,   517,
       0,   271,   430,   589,     0,   643,   597,   628,   494,   630,
     495,   216,     0,     0,     0,   645,   214,   543,     0,   648,
     647,     0,    46,    45,    43,     0,    86,     0,     0,     0,
      79,     0,     0,    72,   258,     0,   291,     0,     0,   301,
       0,   298,   386,   383,   384,   387,   310,     0,     0,    98,
      96,    97,    95,    94,    93,   619,   620,   571,   573,     0,
     560,   134,   141,   140,   139,   136,   143,   144,   138,   142,
     137,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,   500,   501,
     499,   290,     0,   181,   290,     0,   180,     0,   218,     0,
       0,   190,     0,   189,     0,   564,     0,     0,   196,     0,
       0,   194,     0,   204,     0,   202,     0,     0,   167,     0,
     168,   232,   231,     0,   226,     0,     0,   222,     0,   228,
       0,   260,     0,     0,   377,   378,     0,   371,   361,     0,
     480,   392,   395,   394,   396,     0,   444,   447,   448,     0,
       0,   449,   450,     0,     0,   279,     0,     0,   278,   281,
     518,     0,   272,   275,     0,   427,   217,     0,     0,     0,
       0,   215,     0,    91,    88,     0,    69,    68,    67,     0,
       0,     0,     0,   306,   296,     0,   299,   295,   385,   391,
     359,   101,   341,   115,   339,   121,   120,   104,   119,   116,
     344,   131,   102,   132,   118,   122,   103,   105,   117,   133,
     338,   320,   323,   321,   322,   345,   333,   324,   337,   329,
     327,   340,   343,   328,   326,   331,   336,   325,   330,   334,
     335,   332,   342,     0,   319,     0,   106,     0,   359,   359,
     317,   626,   562,   448,   610,   610,     0,     0,     0,     0,
       0,     0,   260,     0,     0,     0,   187,   186,     0,   292,
       0,   292,   193,     0,     0,   532,     0,   531,     0,   563,
       0,     0,   538,   197,     0,   537,   195,   205,   203,    64,
      63,   225,   221,   548,   227,     0,     0,   257,   236,   233,
     480,   364,     0,     0,   448,   393,   407,   441,   471,     0,
     644,   496,   496,     0,   283,     0,     0,     0,   273,     0,
     212,   545,     0,     0,   210,   544,     0,   649,     0,     0,
       0,    72,     0,    72,    80,    83,   262,   286,   157,   306,
     285,   306,   293,   302,   300,     0,   371,   316,     0,   346,
       0,   312,   313,   566,     0,     0,     0,     0,     0,     0,
       0,     0,   262,   292,   306,   292,   306,   536,     0,     0,
     565,   542,     0,     0,     0,     0,   230,    56,   448,   480,
       0,   483,   482,   484,   589,   404,   367,   365,     0,     0,
       0,     0,   469,   589,     0,     0,   284,   282,     0,   276,
       0,   213,     0,   211,    89,    71,    70,     0,     0,     0,
       0,   259,   294,   462,     0,   397,     0,   318,   101,   103,
     359,   359,     0,     0,     0,     0,     0,     0,   306,   175,
     306,   171,     0,     0,     0,     0,    65,   229,   368,   366,
     448,   472,     0,   406,   405,   421,     0,   422,   409,   412,
       0,   408,   401,   402,   305,     0,   583,   584,   274,     0,
       0,    82,    85,    81,    84,     0,   461,   460,   589,   398,
     407,   360,   314,   315,     0,     0,     0,     0,     0,     0,
     177,   173,   535,   534,   541,   540,   370,   369,   474,   475,
     477,   589,     0,   644,   420,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   589,   585,   586,   547,   546,     0,
     452,     0,     0,     0,   476,   478,   411,   413,   414,   417,
     418,   419,   415,   416,   410,   457,   455,   589,   644,   399,
     311,   400,   472,   456,   589,   479
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1062, -1062, -1062,     3,  -470,  2696, -1062, -1062, -1062,   482,
   -1062, -1062, -1062,   484,   620,  -466, -1062,  -727,  -725, -1062,
   -1062, -1062,   205, -1062, -1062,   522,   943, -1062,  2793,  -165,
    -758, -1062,  -952,   953, -1035,  -861, -1062,   -62, -1062, -1062,
   -1062, -1062, -1062, -1062, -1062, -1062,   -22, -1062,   885, -1062,
   -1062,   123,  1267, -1062, -1062, -1062, -1062, -1062,   661, -1062,
      99, -1062, -1062, -1062, -1062, -1062, -1062,  -608,  -656, -1062,
   -1062, -1062,    75,  1543, -1062, -1062, -1062,   336, -1062, -1062,
   -1062, -1062,   -77,  -151,  -909, -1062,   -71,   131,   294, -1062,
   -1062, -1062,    81, -1062, -1062,  -233,    13,  -987,  -189,  -218,
    -212,  -615, -1062,  -190, -1062,    12,   962,   -38,   503, -1062,
    -474,  -838,  -807, -1062,  -657,  -514,  -951, -1061,  -897,   -65,
   -1062,    87, -1062,  -219,  -454,  -438,   818,  -458, -1062, -1062,
   -1062,  1647, -1062,    14, -1062, -1062,   -72, -1062,  -645, -1062,
   -1062, -1062,  2028,  2128,   -12,   969,    44,   889, -1062,  2305,
    2404, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062, -1062,
    -388
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   332,   108,   673,   110,   111,   112,   350,
     481,   113,   238,   114,   333,   664,   553,   668,   665,   115,
     116,   117,   550,   551,   118,   119,   181,   954,   270,   120,
     265,   121,   704,   275,   122,   123,   287,   124,   125,   126,
     446,   642,   442,   639,   127,   128,   802,   129,   236,   130,
     681,   682,   193,   132,   133,   134,   135,   136,   516,   721,
     884,   137,   138,   717,   879,   139,   140,   557,   903,   141,
     142,   760,   761,   194,   268,   695,   144,   145,   559,   909,
     766,   957,   958,   478,  1057,   488,   691,   692,   693,   694,
     767,   359,   865,  1180,  1240,  1164,   436,  1096,  1100,  1158,
    1159,  1160,   146,   320,   521,   147,   148,   271,   272,   492,
     493,   708,  1177,  1125,   496,   705,  1199,  1093,  1014,   334,
     210,   338,   339,   437,   438,   439,   195,   150,   151,   152,
     153,   196,   155,   178,   179,   615,   458,   824,   616,   617,
     156,   157,   197,   198,   160,   222,   440,   200,   162,   201,
     202,   165,   166,   167,   168,   344,   169,   170,   171,   172,
     173
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     176,   292,   295,   667,   199,   107,   722,   825,   204,   956,
     411,   225,   227,   229,   211,   429,   696,   690,   216,   216,
     482,   226,   228,   230,   233,   237,   900,   899,   619,   679,
    1017,   447,   812,   203,   206,   815,   276,   706,   277,   494,
     342,   868,  1052,   282,   494,   283,   762,   284,   435,  1061,
    1062,   716,   273,   827,   575,   296,   300,   302,   304,   305,
     306,  1091,   723,   273,   310,  1163,   311,   504,   314,   318,
     538,   273,   626,   321,   435,   718,   323,   324,   820,   325,
     294,   326,    68,   341,   327,   328,   861,   329,   497,   514,
    1200,   296,   300,   343,   345,  1123,   763,   174,   494,    68,
     543,  1148,  1040,  -463,   532,  1019,   371,   764,     3,   360,
     777,  -255,   301,  1088,   377,   529,   294,   337,   294,  1202,
     626,   435,  1205,  -280,  -256,  1063,   765,   423,   177,   544,
    1155,   533,   575,  -463,   962,   963,   862,  1041,   456,   456,
     530,   456,  1225,   435,  -463,   364,  1155,   863,   340,   576,
    1206,   778,   579,   876,  -458,  1239,   462,   464,   467,  -463,
     425,  -463,   577,  1196,   411,  -280,   864,   539,   430,   539,
     176,   801,  1176,   419,   420,   421,  1097,   425,   310,   296,
     343,   495,  -463,   506,  -458,   498,   495,  1163,   425,   425,
    1122,   578,  1150,   605,   606,   877,   540,  -458,   627,   425,
     810,   300,   539,   978,   294,   294,   980,   425,  1016,  -463,
    -458,  -463,   581,  1052,   425,  1052,   539,   539,   428,  -280,
    -463,  1182,  1183,   867,  -459,   505,   213,   300,  1157,   444,
     579,  1020,   473,  1156,   449,   278,   580,  1124,   -78,   239,
     495,  -463,  1098,   206,  1157,  1203,  1238,   435,   435,   878,
    1149,   854,  1224,   356,  -459,   351,   -49,  -458,    23,   906,
    -463,   534,  -458,   412,   619,   413,   754,  -459,   779,   537,
     541,   456,   357,   755,   300,   -49,   456,  1052,   216,  1052,
    -459,    41,   358,   506,   907,   508,   509,   667,   535,  -238,
     575,  1245,   855,   279,   432,   780,   300,   895,  1099,   174,
     425,   901,   956,   638,   -78,   526,    56,   576,    58,  1207,
     818,   280,  1197,   433,  1118,  1117,  1120,  1119,   435,    66,
     577,   352,   180,  1074,   531,  1076,   536,  -459,   281,  -239,
     851,   887,  -459,   506,   707,  1208,   528,  1209,  1210,   552,
     554,  1211,  1212,  1080,    86,  -245,   565,   567,   435,   578,
     975,   561,   507,   562,   563,   564,   566,   568,   569,  1095,
     570,   571,   572,   573,   574,   367,   582,   583,   584,   585,
     586,   587,   588,   589,   590,   591,   592,   593,   594,   595,
     596,   597,   598,   852,  -244,  1012,   647,   690,   579,   300,
     300,   619,   607,   609,   580,   300,   857,   638,   300,   300,
     300,   300,   608,   610,   618,   629,   641,   988,   638,  -568,
     853,   645,   837,  1236,  -618,   630,  -618,  1138,  -107,  1140,
     634,   368,   635,   858,   972,   369,  -113,   479,   207,  1038,
     480,  1243,   650,  1051,   989,  1046,  -568,  1114,   656,   296,
    -451,   649,   651,   660,  -253,   600,   601,   655,   657,  -567,
     498,   602,   213,   661,   603,   604,   666,   666,  -554,   498,
     435,  -481,   745,   674,   294,  -617,   676,  -617,   638,   560,
    -451,   300,   371,   435,   749,    23,  -567,   375,   633,   376,
     377,   746,  -481,  -451,   380,   709,  -481,   857,   683,   711,
     712,   756,   387,   750,   715,  1072,  -451,   239,   391,   392,
     393,   300,   498,  -109,   498,  -429,  1102,   174,   684,  -481,
     367,  -114,   685,   742,  1181,  -243,   733,   715,  -240,   714,
     300,   813,   300,   816,  -429,    58,   734,   715,   737,   414,
     739,   741,   274,   415,   885,   686,    66,  -617,   687,  -617,
      60,  1204,   714,  -451,   296,   337,   794,   337,  -451,   688,
     415,   528,   714,   443,   274,  1094,  -247,  -614,   448,  -614,
    -251,    86,  1103,  -248,   312,   316,   368,   367,   689,   294,
     369,   425,  -616,   484,  -616,   667,   340,   667,   340,  -615,
     285,  -615,   894,   581,   829,   832,   690,   286,   834,   836,
     435,   307,  1228,  1229,  1230,  1231,  1232,  1233,  -242,   796,
     619,   799,   313,   317,  1051,  -362,  1051,  -464,   649,   797,
     655,   800,   674,   300,   803,   737,   959,   371,  -246,   728,
    -464,   730,   375,   368,   300,   377,   300,   369,   308,   380,
    -252,  -254,  -362,  -241,   817,  -249,   819,   348,   870,   435,
     618,  -453,   826,   805,   806,   349,   353,   371,  -473,   808,
    -464,  -551,   375,   361,   424,   377,   363,   841,   842,   380,
    -550,   888,   890,   362,   416,   434,  1178,  -555,  1051,  -473,
    1051,  -453,   441,  -473,   371,   372,  -464,   373,   374,   375,
     809,   376,   377,  -464,  -453,   425,   380,   426,   445,   306,
     310,   343,  -556,  1201,   387,   300,  -473,  -453,  -553,   545,
     391,   392,   393,  -552,  -464,   418,   520,  1214,   431,   274,
     274,   274,   274,   274,   274,    23,   294,   367,   549,   451,
     474,   477,   483,  -464,   487,   501,   502,   510,  -464,   513,
     522,  -464,   556,   552,   367,   599,   558,   896,   377,   898,
     666,   666,   632,   300,  -453,  1201,   636,   625,   638,  -453,
     485,   872,   646,   637,   652,   669,   644,  -468,   670,   672,
     274,   675,  1237,   697,   707,    58,   274,   618,   678,   698,
    -468,   565,   607,   368,   713,   720,    66,   369,   719,   732,
    1244,   566,   608,   966,  1201,   738,   969,   724,   274,   757,
     368,   769,   300,   770,   369,   205,   768,   771,   775,   772,
    -468,    86,  1104,  1105,   776,   773,   274,   274,   985,   774,
     987,   -74,  1025,   837,   804,   992,   838,   843,   995,   840,
     149,   845,   850,   866,   371,   999,  -468,  1000,   875,   375,
     149,   376,   377,  -468,  1003,   683,   380,   880,   506,   904,
     905,   371,   372,   149,   387,   149,   375,   908,   376,   377,
     961,   967,   393,   380,  -468,   684,   435,   435,   970,   685,
     973,   387,  1005,   979,   981,   296,  1013,   391,   392,   393,
    1007,  1018,  1015,  -468,  1021,  1031,  1022,  1035,  -468,  1027,
    1037,  -468,   686,   666,   683,   687,  1032,   346,  1036,  1044,
     294,   161,  1045,  1053,   149,  -134,   688,  1047,  -141,  -140,
    1056,   161,  -110,  -139,   684,  -108,  -136,  -143,   685,  1131,
    1073,  -144,   274,  -111,   161,   689,   161,  -470,  -138,  -142,
    -137,   149,  -145,  1059,  -112,  1060,   149,  1085,  1179,  1087,
    -470,   686,  1215,  1089,   687,  -107,    60,  1075,  -109,  1108,
    1121,  1058,   274,  1171,  1216,   688,   274,  1206,   274,  1166,
     893,   274,  1167,  1172,  1173,   985,   987,  1174,   992,   995,
    -470,  1031,  1035,   677,   689,   161,  1235,   208,   548,   315,
     315,  1077,  1078,   322,  1079,  1008,   618,   518,  1081,  1082,
    1026,  1054,  1083,  1130,   856,  1126,  -470,   292,  1011,  1241,
    1055,  1222,   161,  -470,  1227,  1234,   149,   161,   232,   217,
       0,   710,     0,     0,     0,     0,     0,  1101,   737,   300,
     300,     0,     0,     0,  -470,  1109,     0,  1110,     0,     0,
       0,  1112,     0,   303,     0,  1152,  1092,  1116,   666,   666,
     666,   666,     0,  -470,     0,     0,     0,     0,  -470,     0,
     902,  -470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1077,  1132,  1133,  1081,  1134,  1135,  1136,  1137,
       0,  -292,     0,     0,     0,  -292,  -292,   161,     0,     0,
       0,     0,  1146,     0,  -292,     0,  -292,  -292,   343,     0,
       0,     0,  -292,  1154,     0,   149,     0,  1161,     0,  -292,
       0,   149,  -292,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   294,     0,     0,     0,     0,  1153,     0,
    1221,  -292,     0,     0,  -292,     0,  -292,     0,  -292,     0,
    -292,  -292,     0,  -292,     0,  -292,     0,  -292,     0,     0,
    1192,  1193,  1194,  1195,   274,   274,     0,     0,     0,     0,
     274,   274,     0,     0,   274,   274,  -292,     0,     0,  -292,
       0,   149,  -292,  1035,     0,     0,   161,  1217,  1218,     0,
     849,     0,   161,   343,  1198,     0,   149,     0,     0,     0,
       0,     0,  1192,  1193,  1194,  1195,  1217,  1218,   452,   455,
     457,   461,   463,   466,     0,     0,     0,     0,   294,     0,
    1223,   737,     0,  1226,     0,     0,     0,     0,     0,     0,
       0,     0,  -292,     0,   882,     0,     0,     0,  -292,     0,
       0,     0,     0,     0,     0,     0,  1198,     0,     0,     0,
       0,     0,   161,     0,     0,     0,   737,     0,     0,   500,
    1035,     0,     0,     0,     0,   503,     0,   161,     0,     0,
       0,     0,     0,   149,     0,   288,     0,     0,     0,    22,
      23,     0,     0,     0,     0,  1198,     0,   512,   289,     0,
      30,   290,     0,     0,     0,     0,    36,     0,     0,   131,
       0,   149,     0,    41,     0,   523,   524,   149,     0,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   274,
     274,   149,   131,   149,   131,     0,     0,     0,    56,     0,
      58,   274,    60,   477,  1048,     0,     0,  1049,     0,   291,
     477,    66,     0,   274,   161,     0,   274,     0,   274,     0,
     274,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,  1009,     0,     0,
       0,     0,   161,   131,     0,     0,     0,     0,   161,     0,
       0,     0,     0,     0,   149,     0,     0,   240,   149,     0,
       0,     0,   161,     0,   161,     0,   149,     0,  1028,     0,
     131,   241,   242,     0,   243,   131,     0,     0,     0,   244,
       0,   631,     0,     0,   367,     0,   101,   245,     0,     0,
       0,     0,  1050,   246,     0,     0,     0,     0,     0,   247,
       0,     0,     0,   248,     0,     0,   249,     0,     0,     0,
       0,   653,     0,     0,     0,   658,   250,   659,     0,     0,
     662,     0,   251,   252,   367,   161,     0,     0,     0,   161,
     253,     0,     0,     0,     0,   792,     0,   161,     0,   254,
     368,     0,     0,     0,   369,   131,     0,     0,   255,   256,
       0,   257,     0,   258,     0,   259,     0,     0,   260,     0,
       0,     0,   261,   489,     0,   262,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     368,     0,     0,     0,   369,     0,     0,     0,     0,     0,
     149,   371,   372,   149,   373,   374,   375,     0,   376,   377,
     378,     0,     0,   380,     0,     0,     0,     0,     0,   274,
     386,   387,     0,     0,   390,     0,     0,   391,   392,   393,
       0,     0,     0,     0,   490,     0,     0,     0,   394,     0,
       0,   371,   372,     0,   131,   374,   375,     0,   376,   377,
     131,     0,     0,   380,     0,   143,     0,     0,     0,     0,
     149,   387,     0,     0,     0,   143,   149,   391,   392,   393,
       0,   161,   477,   477,   161,     0,   477,   477,   143,   367,
     143,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1162,   960,     0,     0,     0,     0,
       0,     0,  1168,     0,   477,     0,   477,     0,     0,     0,
     131,     0,     0,   821,   823,     0,     0,     0,     0,   828,
     831,   964,   965,   833,   835,   131,     0,     0,     0,   143,
       0,   161,     0,     0,     0,   368,     0,   161,     0,   369,
       0,     0,     0,     0,     0,     0,     0,     0,   149,     0,
     149,     0,     0,     0,     0,   149,   143,     0,   149,   154,
       0,   143,     0,     0,     0,     0,     0,     0,     0,   154,
       0,   149,     0,   149,     0,     0,   149,     0,     0,     0,
       0,     0,   154,     0,   154,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,     0,     0,     0,   380,     0,
       0,     0,   131,     0,   149,   386,   387,     0,     0,   390,
       0,     0,   391,   392,   393,   149,  1162,   149,     0,   161,
       0,   161,     0,     0,     0,     0,   161,     0,     0,   161,
     131,   143,     0,   154,     0,     0,   131,     0,     0,     0,
       0,     0,   161,     0,   161,     0,     0,   161,     0,     0,
     131,     0,   131,     0,     0,     0,     0,   288,     0,     0,
     154,    22,    23,     0,     0,   154,     0,     0,   976,   977,
     289,     0,    30,   290,     0,   161,     0,     0,    36,     0,
     982,     0,     0,     0,     0,    41,   161,     0,   161,     0,
       0,     0,   993,     0,     0,   996,     0,   997,     0,   998,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,   149,    58,   131,   346,     0,  1048,   131,   149,  1049,
     143,   291,     0,    66,     0,   131,   143,     0,     0,     0,
       0,     0,     0,   149,     0,   154,     0,     0,     0,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,   149,   288,     0,     0,     0,    22,    23,     0,     0,
     149,     0,     0,     0,   149,   289,     0,    30,   290,     0,
       0,     0,   288,    36,     0,     0,    22,    23,     0,     0,
      41,     0,   161,     0,     0,   289,   143,    30,   290,   161,
       0,     0,     0,    36,     0,     0,     0,     0,   101,     0,
      41,   143,     0,     0,   161,    56,     0,    58,     0,    60,
       0,  1048,     0,   149,  1049,     0,   291,     0,    66,     0,
       0,     0,   161,     0,   154,    56,     0,    58,     0,     0,
     154,   161,     0,     0,     0,   161,   291,    82,    66,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,   131,
       0,     0,   131,     0,     0,     0,     0,    82,     0,     0,
      84,   288,     0,    86,     0,    22,    23,     0,     0,     0,
     149,   149,   149,   149,   289,     0,    30,   290,   143,     0,
       0,     0,    36,     0,   161,     0,     0,     0,  1107,    41,
     154,     0,     0,   101,     0,     0,     0,   149,   149,  1139,
       0,     0,     0,     0,     0,   154,   143,     0,     0,   131,
       0,     0,   143,     0,    56,   131,    58,     0,    60,     0,
    1048,     0,     0,  1049,     0,   291,   143,    66,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     158,   161,   161,   161,   161,     0,    82,     0,     0,    84,
     158,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   158,     0,   158,     0,     0,   161,   161,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   154,     0,     0,     0,     0,     0,     0,   143,
       0,     0,     0,   143,     0,     0,     0,   131,     0,   131,
       0,   143,   101,     0,   131,     0,     0,   131,  1141,     0,
     154,     0,   240,     0,   158,     0,   154,     0,     0,     0,
     131,     0,   131,     0,     0,   131,   241,   242,     0,   243,
     154,     0,   154,     0,   244,     0,     0,     0,     0,     0,
     159,   158,   245,     0,     0,     0,   158,     0,   246,     0,
     159,     0,     0,   131,   247,     0,     0,     0,   248,     0,
       0,   249,     0,   159,   131,   159,   131,     0,     0,     0,
     575,   250,     0,     0,     0,     0,     0,   251,   252,     0,
       0,     0,     0,     0,   781,   253,     0,   782,     0,     0,
       0,     0,   783,   154,   254,     0,     0,   154,     0,     0,
       0,     0,   184,   255,   256,   154,   257,     0,   258,     0,
     259,     0,   784,   260,   159,     0,   158,   261,     0,   785,
     262,     0,     0,   263,     0,   143,     0,     0,   143,   786,
       0,     0,     0,     0,     0,   288,   787,     0,     0,    22,
      23,   159,     0,     0,     0,     0,   159,     0,   289,     0,
      30,   290,   788,     0,     0,     0,    36,     0,     0,     0,
     131,     0,     0,    41,   789,     0,     0,   131,   579,     0,
       0,     0,     0,     0,     0,   790,     0,   425,     0,   881,
       0,   791,   131,     0,     0,   143,     0,     0,    56,     0,
      58,   143,    60,     0,  1048,     0,     0,  1049,     0,   291,
     131,    66,     0,     0,     0,   158,     0,     0,     0,   131,
       0,   158,     0,   131,     0,     0,   159,   163,     0,     0,
      82,     0,     0,    84,     0,     0,    86,   163,     0,   154,
       0,     0,   154,     0,     0,     0,     0,     0,     0,     0,
     163,     0,   163,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,     0,     0,
       0,     0,   131,     0,     0,     0,     0,     0,     0,     0,
       0,   158,     0,   143,  -575,   143,   101,     0,     0,     0,
     143,     0,  1190,   143,     0,     0,   158,   410,     0,   154,
    -614,   163,  -614,     0,     0,   154,   143,     0,   143,     0,
       0,   143,     0,     0,     0,   159,     0,     0,     0,     0,
       0,   159,     0,     0,     0,     0,   164,     0,   163,   131,
     131,   131,   131,   163,     0,     0,   164,     0,     0,   143,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   164,
     143,   164,   143,     0,     0,     0,   131,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   288,     0,     0,
       0,    22,    23,   158,     0,     0,     0,     0,     0,     0,
     289,   159,    30,   290,     0,     0,     0,   154,    36,   154,
       0,     0,     0,     0,   154,    41,   159,   154,     0,     0,
     164,   158,     0,   163,     0,     0,     0,   158,     0,     0,
     154,     0,   154,     0,     0,   154,     0,     0,     0,   367,
      56,   158,    58,   158,    60,     0,  1048,   164,     0,  1049,
       0,   291,   164,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   154,     0,     0,   143,     0,     0,     0,
       0,     0,    82,   143,   154,    84,   154,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,   159,     0,   368,     0,     0,     0,   369,
       0,     0,     0,     0,   158,     0,   143,     0,   158,     0,
       0,     0,   163,     0,     0,   143,   158,     0,   163,   143,
       0,   159,   164,     0,     0,     0,     0,   159,   101,     0,
     370,     0,     0,     0,  1191,     0,     0,     0,     0,     0,
       0,   159,     0,   159,     0,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,     0,     0,   384,   385,   386,   387,   388,   143,   390,
     154,     0,   391,   392,   393,     0,     0,   154,   163,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,   154,   163,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   159,     0,     0,     0,   159,     0,
     154,   164,     0,     0,     0,     0,   159,   164,     0,   154,
       0,     0,     0,   154,     0,   143,   143,   143,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   109,     0,
     158,     0,     0,   158,     0,     0,     0,     0,   175,     0,
       0,     0,   143,   143,     0,     0,     0,     0,     0,     0,
       0,   209,     0,   212,     0,     0,     0,     0,     0,     0,
     163,     0,   154,     0,     0,     0,     0,   164,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   164,     0,     0,     0,     0,     0,   163,     0,
     158,     0,     0,     0,   163,     0,   158,     0,     0,     0,
       0,     0,   309,     0,     0,   367,     0,     0,   163,     0,
     163,     0,     0,     0,     0,     0,     0,     0,     0,   154,
     154,   154,   154,     0,     0,     0,     0,     0,     0,   109,
     159,     0,     0,   159,   347,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   154,   154,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   164,
       0,   368,     0,     0,     0,   369,   264,   266,   267,     0,
       0,   163,     0,     0,     0,   163,     0,     0,   158,     0,
     158,     0,     0,   163,     0,   158,     0,   164,   158,     0,
     159,     0,     0,   164,     0,     0,   159,     0,     0,     0,
       0,   158,     0,   158,   417,   319,   158,   164,     0,   164,
       0,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,     0,     0,   384,
     385,   386,   387,   388,   158,   390,     0,     0,   391,   392,
     393,     0,     0,     0,     0,   158,     0,   158,     0,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   354,     0,   355,     0,     0,     0,     0,
     164,     0,     0,     0,   164,     0,     0,     0,   159,     0,
     159,     0,   164,     0,     0,   159,     0,     0,   159,     0,
       0,     0,     0,   486,     0,     0,     0,     0,     0,   499,
     367,   159,     0,   159,     0,     0,   159,   163,     0,     0,
     163,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   319,     0,
       0,     0,     0,     0,   159,     0,     0,     0,     0,     0,
       0,   158,     0,     0,   319,   159,   450,   159,   158,     0,
       0,     0,     0,     0,     0,     0,   368,   471,   472,   109,
     369,     0,     0,   158,     0,     0,     0,   163,     0,     0,
       0,     0,     0,   163,   109,     0,     0,     0,     0,     0,
       0,   158,     0,     0,     0,     0,     0,     0,     0,     0,
     158,   370,   491,     0,   158,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   164,   371,   372,   164,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,     0,     0,   384,   385,   386,   387,   388,   389,
     390,     0,     0,   391,   392,   393,     0,     0,     0,     0,
       0,   159,     0,   158,   394,     0,     0,     0,   159,     0,
       0,   109,     0,     0,     0,   163,     0,   163,     0,     0,
       0,     0,   163,   159,     0,   163,   164,     0,     0,     0,
       0,     0,   164,     0,     0,     0,   555,     0,   163,   499,
     163,   159,     0,   163,     0,   499,     0,     0,     0,     0,
     159,     0,     0,     0,   159,     0,     0,     0,     0,   671,
     158,   158,   158,   158,     0,     0,     0,     0,     0,     0,
       0,   163,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   163,     0,   163,     0,     0,   158,   158,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   159,     0,     0,     0,   628,     0,     0,
       0,     0,     0,     0,   164,     0,   164,     0,     0,     0,
       0,   164,   731,     0,   164,   640,   736,     0,     0,   643,
       0,     0,     0,     0,   109,     0,     0,   164,     0,   164,
       0,     0,   164,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     159,   159,   159,   159,     0,     0,     0,   680,     0,     0,
     164,     0,     0,   491,     0,     0,     0,   240,   163,     0,
       0,   164,     0,   164,     0,   163,     0,   159,   159,     0,
       0,   241,   242,     0,   243,     0,     0,     0,     0,   244,
     163,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,   246,     0,     0,     0,     0,   163,   247,
       0,     0,     0,   248,     0,     0,   249,   163,     0,     0,
       0,   163,   744,     0,     0,     0,   250,     0,     0,     0,
       0,   759,   251,   252,     0,     0,     0,     0,     0,     0,
     253,     0,     0,     0,     0,     0,     0,     0,   844,   254,
       0,   847,     0,     0,     0,     0,     0,     0,   255,   256,
       0,   257,     0,   258,     0,   259,     0,   164,   260,     0,
     163,     0,   261,     0,   164,   262,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   164,
       0,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,     0,   164,   886,     0,
       0,     0,   811,     0,   891,   814,   164,     0,     0,     0,
     164,     0,     0,     0,     0,     0,     0,   163,   163,   163,
     163,     0,     0,     0,   269,   410,     0,     0,  -614,     0,
    -614,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   163,   163,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   860,     0,     0,   164,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   883,     0,     0,   499,   319,   499,     0,
       0,   240,     0,   499,     0,     0,   499,     0,     0,     0,
       0,     0,     0,     0,     0,   241,   242,     0,   243,  1001,
       0,  1002,     0,   244,  1004,     0,   164,   164,   164,   164,
       0,   245,     0,     0,     0,     0,     0,   246,     0,     0,
     955,     0,  -287,   247,     0,     0,     0,   248,     0,     0,
     249,     0,  1024,   164,   164,     0,  -287,  -287,     0,  -287,
     250,     0,     0,  1030,  -287,  1034,   251,   252,     0,     0,
       0,     0,  -287,     0,   253,     0,     0,     0,  -287,     0,
       0,     0,     0,   254,  -287,     0,     0,     0,  -287,     0,
       0,  -287,   255,   256,     0,   257,     0,   258,     0,   259,
       0,  -287,   260,     0,     0,     0,   261,  -287,  -287,   262,
       0,     0,   263,     0,     0,  -287,     0,     0,     0,     0,
       0,     0,     0,  1006,  -287,   680,     0,     0,     0,  1010,
       0,     0,   491,  -287,  -287,     0,  -287,     0,  -287,     0,
    -287,     0,     0,  -287,     0,     0,     0,  -287,     0,     0,
    -287,     0,     0,  -287,   883,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   859,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   759,
       0,  1086,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1106,
       0,     0,     0,     0,     0,     0,     0,     0,  1111,     0,
      -2,     4,  1113,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,    38,   -76,     0,    39,    40,
      41,  1147,    42,  -306,     0,    43,    44,    45,    46,    47,
       0,    48,    49,    50,   -47,    51,    52,     0,    53,    54,
      55,     0,  -306,     0,     0,    56,    57,    58,    59,    60,
      61,    62,  -306,   -47,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,    77,    78,     0,    79,    80,    81,    82,    83,     0,
      84,    85,   -76,    86,    87,     0,     0,    88,     0,    89,
       0,     0,    90,   955,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,   101,     0,     0,   102,     0,   103,     0,
     104,     0,   105,     0,     4,   106,     5,     0,     6,     7,
       8,     9,    10,    11,     0,  -644,     0,    12,    13,    14,
      15,    16,  -644,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,  -644,    27,    28,  -644,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,   -76,
       0,    39,    40,    41,     0,    42,  -306,     0,    43,    44,
      45,    46,    47,     0,    48,    49,    50,   -47,    51,    52,
       0,    53,    54,    55,     0,  -306,     0,     0,    56,    57,
      58,    59,     0,    61,    62,  -306,   -47,    63,    64,    65,
    -644,    66,    67,    68,  -644,    69,    70,    71,    72,    73,
      74,    75,    76,     0,    77,    78,     0,    79,    80,    81,
      82,    83,     0,    84,    85,   -76,    86,    87,     0,     0,
      88,     0,    89,     0,     0,  -644,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -644,  -644,    93,  -644,  -644,  -644,  -644,  -644,  -644,
    -644,     0,  -644,  -644,  -644,  -644,  -644,     0,  -644,  -644,
    -644,  -644,  -644,  -644,  -644,  -644,   101,  -644,  -644,  -644,
       0,   103,  -644,   104,     0,   105,     0,   330,  -644,     5,
     293,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,    38,   -76,     0,    39,    40,    41,     0,    42,  -306,
       0,    43,    44,    45,    46,    47,     0,    48,    49,    50,
     -47,    51,    52,     0,    53,    54,    55,     0,  -306,     0,
       0,    56,    57,    58,    59,    60,    61,    62,  -306,   -47,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,     0,    84,    85,   -76,    86,
      87,     0,     0,    88,     0,    89,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,   101,
       0,     0,   102,     0,   103,   331,   104,     0,   105,     0,
       4,   106,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,   -76,     0,    39,    40,    41,
       0,    42,  -306,     0,    43,    44,    45,    46,    47,     0,
      48,    49,    50,   -47,    51,    52,     0,    53,    54,    55,
       0,  -306,     0,     0,    56,    57,    58,    59,    60,    61,
      62,  -306,   -47,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,    76,     0,
      77,    78,     0,    79,    80,    81,    82,    83,     0,    84,
      85,   -76,    86,    87,     0,     0,    88,     0,    89,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,   101,     0,     0,   102,     0,   103,   527,   104,
       0,   105,     0,   546,   106,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,    38,   -76,     0,
      39,    40,    41,     0,    42,  -306,     0,    43,    44,    45,
      46,    47,     0,    48,    49,    50,   -47,    51,    52,     0,
      53,    54,    55,     0,  -306,     0,     0,    56,    57,    58,
      59,    60,    61,    62,  -306,   -47,    63,    64,    65,     0,
      66,    67,    68,     0,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,     0,    84,    85,   -76,    86,    87,     0,     0,    88,
       0,    89,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,   101,     0,     0,   102,     0,
     103,   547,   104,     0,   105,     0,   330,   106,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,   -76,     0,    39,    40,    41,     0,    42,  -306,     0,
      43,    44,    45,    46,    47,     0,    48,    49,    50,   -47,
      51,    52,     0,    53,    54,    55,     0,  -306,     0,     0,
      56,    57,    58,    59,    60,    61,    62,  -306,   -47,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,    76,     0,    77,    78,     0,    79,
      80,    81,    82,    83,     0,    84,    85,   -76,    86,    87,
       0,     0,    88,     0,    89,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,   101,     0,
       0,   102,     0,   103,   331,   104,     0,   105,     0,     4,
     106,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,   -76,     0,    39,    40,    41,     0,
      42,  -306,     0,    43,    44,    45,    46,    47,     0,    48,
      49,    50,   -47,    51,    52,     0,    53,    54,    55,     0,
    -306,     0,     0,    56,    57,    58,    59,    60,    61,    62,
    -306,   -47,    63,    64,    65,     0,    66,    67,    68,     0,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,    80,    81,    82,    83,     0,    84,    85,
     -76,    86,    87,     0,     0,    88,     0,    89,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,   101,     0,     0,   102,     0,   103,   743,   104,     0,
     105,     0,     4,   106,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,    38,   -76,     0,    39,
      40,    41,     0,    42,  -306,     0,    43,    44,    45,    46,
      47,     0,    48,    49,    50,   -47,    51,    52,     0,    53,
      54,    55,     0,  -306,     0,     0,    56,    57,    58,    59,
     346,    61,    62,  -306,   -47,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
      76,     0,    77,    78,     0,    79,    80,    81,    82,    83,
       0,    84,    85,   -76,    86,    87,     0,     0,    88,     0,
      89,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,   101,     0,     0,   102,     0,   103,
       0,   104,     0,   105,     0,     4,   106,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
     -76,     0,    39,    40,    41,     0,    42,  -306,     0,    43,
      44,    45,    46,    47,     0,    48,    49,    50,   -47,    51,
      52,     0,    53,    54,    55,     0,  -306,     0,     0,    56,
      57,    58,    59,     0,    61,    62,  -306,   -47,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,    76,     0,    77,    78,     0,    79,    80,
      81,    82,    83,     0,    84,    85,   -76,    86,    87,     0,
       0,    88,     0,    89,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,   101,     0,     0,
     102,     0,   103,     0,   104,     0,   105,     0,     4,   106,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,   648,    38,   -76,     0,    39,    40,    41,     0,    42,
    -306,     0,    43,    44,    45,    46,    47,     0,    48,    49,
      50,   -47,    51,    52,     0,    53,    54,    55,     0,  -306,
       0,     0,    56,    57,    58,    59,     0,    61,    62,  -306,
     -47,    63,    64,    65,     0,    66,    67,    68,     0,    69,
      70,    71,    72,    73,    74,    75,    76,     0,    77,    78,
       0,    79,    80,    81,    82,    83,     0,    84,    85,   -76,
      86,    87,     0,     0,    88,     0,    89,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
     101,     0,     0,   102,     0,   103,     0,   104,     0,   105,
       0,     4,   106,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,   654,    38,   -76,     0,    39,    40,
      41,     0,    42,  -306,     0,    43,    44,    45,    46,    47,
       0,    48,    49,    50,   -47,    51,    52,     0,    53,    54,
      55,     0,  -306,     0,     0,    56,    57,    58,    59,     0,
      61,    62,  -306,   -47,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,    77,    78,     0,    79,    80,    81,    82,    83,     0,
      84,    85,   -76,    86,    87,     0,     0,    88,     0,    89,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,   101,     0,     0,   102,     0,   103,     0,
     104,     0,   105,     0,     4,   106,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,   984,    38,   -76,
       0,    39,    40,    41,     0,    42,  -306,     0,    43,    44,
      45,    46,    47,     0,    48,    49,    50,   -47,    51,    52,
       0,    53,    54,    55,     0,  -306,     0,     0,    56,    57,
      58,    59,     0,    61,    62,  -306,   -47,    63,    64,    65,
       0,    66,    67,    68,     0,    69,    70,    71,    72,    73,
      74,    75,    76,     0,    77,    78,     0,    79,    80,    81,
      82,    83,     0,    84,    85,   -76,    86,    87,     0,     0,
      88,     0,    89,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,   101,     0,     0,   102,
       0,   103,     0,   104,     0,   105,     0,     4,   106,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
     986,    38,   -76,     0,    39,    40,    41,     0,    42,  -306,
       0,    43,    44,    45,    46,    47,     0,    48,    49,    50,
     -47,    51,    52,     0,    53,    54,    55,     0,  -306,     0,
       0,    56,    57,    58,    59,     0,    61,    62,  -306,   -47,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,     0,    84,    85,   -76,    86,
      87,     0,     0,    88,     0,    89,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,   101,
       0,     0,   102,     0,   103,     0,   104,     0,   105,     0,
       4,   106,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,   991,    38,   -76,     0,    39,    40,    41,
       0,    42,  -306,     0,    43,    44,    45,    46,    47,     0,
      48,    49,    50,   -47,    51,    52,     0,    53,    54,    55,
       0,  -306,     0,     0,    56,    57,    58,    59,     0,    61,
      62,  -306,   -47,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,    76,     0,
      77,    78,     0,    79,    80,    81,    82,    83,     0,    84,
      85,   -76,    86,    87,     0,     0,    88,     0,    89,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,   101,     0,     0,   102,     0,   103,     0,   104,
       0,   105,     0,     4,   106,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,   994,    38,   -76,     0,
      39,    40,    41,     0,    42,  -306,     0,    43,    44,    45,
      46,    47,     0,    48,    49,    50,   -47,    51,    52,     0,
      53,    54,    55,     0,  -306,     0,     0,    56,    57,    58,
      59,     0,    61,    62,  -306,   -47,    63,    64,    65,     0,
      66,    67,    68,     0,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,     0,    84,    85,   -76,    86,    87,     0,     0,    88,
       0,    89,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,   101,     0,     0,   102,     0,
     103,     0,   104,     0,   105,     0,     4,   106,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,  1023,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,   -76,     0,    39,    40,    41,     0,    42,  -306,     0,
      43,    44,    45,    46,    47,     0,    48,    49,    50,   -47,
      51,    52,     0,    53,    54,    55,     0,  -306,     0,     0,
      56,    57,    58,    59,     0,    61,    62,  -306,   -47,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,    76,     0,    77,    78,     0,    79,
      80,    81,    82,    83,     0,    84,    85,   -76,    86,    87,
       0,     0,    88,     0,    89,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,   101,     0,
       0,   102,     0,   103,     0,   104,     0,   105,     0,     4,
     106,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,  1029,    38,   -76,     0,    39,    40,    41,     0,
      42,  -306,     0,    43,    44,    45,    46,    47,     0,    48,
      49,    50,   -47,    51,    52,     0,    53,    54,    55,     0,
    -306,     0,     0,    56,    57,    58,    59,     0,    61,    62,
    -306,   -47,    63,    64,    65,     0,    66,    67,    68,     0,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,    80,    81,    82,    83,     0,    84,    85,
     -76,    86,    87,     0,     0,    88,     0,    89,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,   101,     0,     0,   102,     0,   103,     0,   104,     0,
     105,     0,     4,   106,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,  1033,    38,   -76,     0,    39,
      40,    41,     0,    42,  -306,     0,    43,    44,    45,    46,
      47,     0,    48,    49,    50,   -47,    51,    52,     0,    53,
      54,    55,     0,  -306,     0,     0,    56,    57,    58,    59,
       0,    61,    62,  -306,   -47,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
      76,     0,    77,    78,     0,    79,    80,    81,    82,    83,
       0,    84,    85,   -76,    86,    87,     0,     0,    88,     0,
      89,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,   101,     0,     0,   102,     0,   103,
       0,   104,     0,   105,     0,  1219,   106,     5,   293,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     189,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,     0,   104,     0,   192,  1220,   869,   106,
       5,   293,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,   187,    38,     0,     0,     0,    40,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,   192,
       0,     5,   106,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,   231,    23,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,    58,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,    66,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,    86,    87,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,     0,     0,     0,   102,     0,   191,     0,   104,     0,
     192,     0,     0,   106,     5,   293,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   184,     0,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,   297,   298,     0,    83,
     335,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,   336,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,     0,   192,     0,     0,   106,     5,   293,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     189,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,   297,
     298,     0,    83,   335,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,     0,   104,   727,   192,     0,     0,   106,
       5,   293,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,   187,    38,     0,     0,     0,    40,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,   297,   298,     0,    83,   335,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,   729,   192,
       0,     5,   106,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,   611,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,   612,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,   613,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,   614,    87,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,     0,     0,     0,   102,     0,   191,     0,   104,     0,
     192,     0,     5,   106,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,   297,   298,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,   299,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     0,   106,     5,   293,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,    40,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,     0,    57,     0,
      59,     0,    61,     0,     0,     0,     0,    64,   189,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   190,     0,    77,     0,     0,    79,   297,   298,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,     0,     0,     0,   102,     0,
     191,     0,   104,     0,   192,     0,     5,   106,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
     871,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,   297,   298,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     0,     5,   106,     6,
       7,     8,   218,    10,    11,   219,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     220,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,     0,
       0,   221,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,     0,   104,     0,   192,     0,     5,   106,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     184,     0,     0,     0,    32,   185,   186,     0,     0,   187,
      38,     0,     0,     0,    40,     0,     0,    42,     0,     0,
     188,     0,     0,    46,    47,     0,     0,    49,    50,     0,
      51,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,    57,   223,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,   224,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,   192,     0,     5,
     106,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,   234,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,     0,    85,     0,   235,
      87,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   102,     0,   191,     0,   104,     0,   192,     0,
       0,   106,     5,   293,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,   224,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   189,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   190,
       0,    77,     0,     0,    79,   297,   298,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,     0,     0,     0,   102,     0,   191,     0,
     104,     0,   192,     0,     0,   106,     5,   293,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,   725,   104,     0,   192,     0,     0,   106,     5,
     293,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   102,     0,   191,     0,   104,     0,   192,   735,
       0,   106,     5,   293,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,  1151,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   189,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   190,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
     224,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,     0,     0,     0,   102,     0,   191,     0,
     104,     0,   192,     0,     5,   106,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,     0,     0,     0,
       0,     0,     0,   273,    28,     0,   184,     0,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   103,
       0,   104,     0,   192,     0,     0,   106,     5,   293,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     189,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,     0,   104,     0,   192,     0,     5,   106,
       6,     7,     8,   218,    10,    11,     0,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     184,     0,     0,     0,    32,   185,   186,     0,     0,   187,
      38,     0,     0,     0,    40,     0,     0,    42,     0,     0,
     188,     0,     0,    46,    47,     0,     0,    49,    50,     0,
      51,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   220,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,   221,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,   192,     0,     5,
     106,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
     187,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   102,     0,   191,     0,   104,     0,   192,   740,
       5,   106,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,   187,    38,     0,     0,     0,    40,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,     0,     0,     0,    83,   897,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,   192,
       0,     0,   106,     5,   293,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   699,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,   700,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   189,     0,     0,     0,
       0,     0,     0,     0,    71,   701,    73,    74,    75,   702,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,     0,     0,     0,   102,     0,   191,     0,
     104,     0,  1090,     0,     5,   106,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   184,     0,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
    1115,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,     0,   192,     0,     0,   106,     5,   293,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   187,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     189,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,     0,   104,     0,  1090,     0,     0,   106,
       5,   293,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,  1071,    38,     0,     0,     0,    40,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,   192,
       0,     5,   106,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,   187,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,     0,     0,     0,   102,     0,   191,     0,   104,     0,
     192,     0,     5,   106,     6,     7,     8,   214,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   215,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,   187,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   189,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   190,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,     0,     0,     0,   102,     0,   103,     0,
     104,     0,   192,     0,     5,   106,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   184,     0,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   663,     0,   191,
       0,   104,     0,   192,     0,     5,   106,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,   699,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,   700,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,     0,    57,     0,
      59,     0,    61,     0,     0,     0,     0,    64,   189,     0,
       0,     0,     0,     0,     0,     0,    71,   701,    73,    74,
      75,   702,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,     0,     0,     0,   102,     0,
     191,     0,   104,     0,   703,     0,     5,   106,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,    98,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   703,     0,     5,   106,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   182,     0,
       0,    15,    16,     0,    17,     0,   183,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   184,
       0,     0,     0,    32,   185,   186,     0,     0,   795,    38,
       0,     0,     0,    40,     0,     0,    42,     0,     0,   188,
       0,     0,    46,    47,     0,     0,    49,    50,     0,    51,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,     0,     0,     0,    64,
     189,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   190,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,    93,    94,    95,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,   100,     0,     0,     0,
     102,     0,   191,     0,   104,     0,   192,     0,     5,   106,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     184,     0,     0,     0,    32,   185,   186,     0,     0,   798,
      38,     0,     0,     0,    40,     0,     0,    42,     0,     0,
     188,     0,     0,    46,    47,     0,     0,    49,    50,     0,
      51,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,    92,    93,    94,    95,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    97,     0,     0,    98,    99,   100,     0,     0,
       0,   102,     0,   191,     0,   104,     0,   192,     0,     5,
     106,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     182,     0,     0,    15,    16,     0,    17,     0,   183,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   184,     0,     0,     0,    32,   185,   186,     0,     0,
    1065,    38,     0,     0,     0,    40,     0,     0,    42,     0,
       0,   188,     0,     0,    46,    47,     0,     0,    49,    50,
       0,    51,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,     0,     0,
       0,    64,   189,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   190,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    92,    93,    94,    95,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,    98,    99,   100,     0,
       0,     0,   102,     0,   191,     0,   104,     0,   192,     0,
       5,   106,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   182,     0,     0,    15,    16,     0,    17,     0,   183,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   184,     0,     0,     0,    32,   185,   186,     0,
       0,  1066,    38,     0,     0,     0,    40,     0,     0,    42,
       0,     0,   188,     0,     0,    46,    47,     0,     0,    49,
      50,     0,    51,    52,     0,    53,    54,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,     0,
       0,     0,    64,   189,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   190,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,    92,    93,    94,    95,
       0,     0,     0,     0,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,    98,    99,   100,
       0,     0,     0,   102,     0,   191,     0,   104,     0,   192,
       0,     5,   106,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   182,     0,     0,    15,    16,     0,    17,     0,
     183,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   184,     0,     0,     0,    32,   185,   186,
       0,     0,  1068,    38,     0,     0,     0,    40,     0,     0,
      42,     0,     0,   188,     0,     0,    46,    47,     0,     0,
      49,    50,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
       0,     0,     0,    64,   189,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   190,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,    92,    93,    94,
      95,     0,     0,     0,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     0,     0,    98,    99,
     100,     0,     0,     0,   102,     0,   191,     0,   104,     0,
     192,     0,     5,   106,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   184,     0,     0,     0,    32,   185,
     186,     0,     0,  1069,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,    92,    93,
      94,    95,     0,     0,     0,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,   100,     0,     0,     0,   102,     0,   191,     0,   104,
       0,   192,     0,     5,   106,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   182,     0,     0,    15,    16,     0,
      17,     0,   183,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   184,     0,     0,     0,    32,
     185,   186,     0,     0,  1070,    38,     0,     0,     0,    40,
       0,     0,    42,     0,     0,   188,     0,     0,    46,    47,
       0,     0,    49,    50,     0,    51,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,     0,     0,     0,    64,   189,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   190,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,    92,
      93,    94,    95,     0,     0,     0,     0,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
      98,    99,   100,     0,     0,     0,   102,     0,   191,     0,
     104,     0,   192,     0,     5,   106,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   184,     0,     0,     0,
      32,   185,   186,     0,     0,  1071,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
      92,    93,    94,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,    98,    99,   100,     0,     0,     0,   102,     0,   191,
       0,   104,     0,   192,     0,     5,   106,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   182,     0,     0,    15,
      16,     0,    17,     0,   183,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,   699,     0,   184,     0,     0,
       0,    32,   185,   186,     0,     0,   187,    38,     0,     0,
       0,   700,     0,     0,    42,     0,     0,   188,     0,     0,
      46,    47,     0,     0,    49,    50,     0,    51,    52,     0,
      53,    54,     0,     0,     0,     0,     0,     0,    57,     0,
      59,     0,    61,     0,     0,     0,     0,    64,   189,     0,
       0,     0,     0,     0,     0,     0,    71,   701,    73,    74,
      75,   702,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,    93,    94,    95,     0,     0,     0,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,   100,     0,     0,     0,   102,     0,
     191,     0,   104,     0,  1175,     0,     5,   106,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   910,     0,
     911,    91,    92,    93,    94,    95,     0,     0,     0,   912,
      96,     0,     0,     0,   913,   242,   914,   915,     0,     0,
      97,     0,   916,    98,    99,   100,     0,     0,     0,   102,
     245,     0,   184,   104,     0,  1175,   917,     0,   106,     0,
       0,     0,   918,     0,     0,     0,   248,     0,     0,   919,
       0,   920,     0,     0,     0,     0,     0,     0,     0,   921,
       0,     0,     0,     0,     0,   922,   923,     0,     0,     0,
       0,     0,     0,   253,     0,     0,     0,     0,     0,     0,
       0,     0,   924,     0,     0,     0,     0,     0,     0,     0,
       0,   255,   256,     0,   925,     0,   258,     0,   926,     0,
       0,   927,     0,     0,     0,   928,     0,     0,   262,     0,
       0,   929,     0,     0,     0,     0,     0,     0,     0,     0,
     476,   396,   397,   398,   399,   400,     0,     0,   403,   404,
     405,   406,     0,   408,   409,   930,   931,   932,   933,   934,
     935,     0,   936,     0,     0,     0,   937,   938,   939,   940,
     941,   942,   943,   944,   945,   946,   947,     0,   948,     0,
       0,   949,   950,   951,   952,     0,     5,   953,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    92,    93,    94,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,     0,    99,   100,     0,     0,     0,   102,
       0,   191,     0,   104,     0,   192,     5,   293,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,     0,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,  -472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,  -472,     0,     0,
       0,  -472,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,     0,     0,
       0,   191,     0,   104,  -472,  1090,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   182,     0,     0,
      15,    16,     0,    17,     0,   183,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   184,     0,
       0,     0,    32,   185,   186,     0,     0,   187,    38,     0,
       0,     0,    40,     0,     0,    42,     0,     0,   188,     0,
       0,    46,    47,  -454,     0,    49,    50,     0,    51,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,     0,     0,     0,    64,   189,
       0,     0,     0,  -454,     0,     0,     0,    71,    72,    73,
      74,    75,   190,     0,    77,     0,  -454,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,  -454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,    93,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   182,     0,     0,    15,    16,     0,    17,
       0,   183,     0,     0,    21,   100,  -454,     0,     0,     0,
       0,  -454,    28,   104,   184,  1175,     0,     0,    32,   185,
     186,     0,     0,   187,    38,     0,     0,     0,    40,     0,
       0,    42,     0,     0,   188,     0,     0,    46,    47,     0,
       0,    49,    50,     0,    51,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,     0,     0,     0,    64,   189,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   190,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,     5,    93,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   182,
       0,     0,    15,    16,     0,    17,     0,   183,     0,     0,
      21,   100,     0,     0,     0,     0,     0,   191,    28,   104,
     184,   192,     0,     0,    32,   185,   186,     0,     0,   187,
      38,     0,     0,     0,    40,     0,     0,    42,     0,     0,
     188,     0,     0,    46,    47,     0,     0,    49,    50,     0,
      51,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,     0,     0,     0,
      64,   189,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   190,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,    93,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   182,     0,     0,    15,    16,
       0,    17,     0,   183,     0,     0,    21,   100,     0,     0,
       0,     0,     0,   191,    28,   104,   184,   703,     0,     0,
      32,   185,   186,     0,     0,   187,    38,     0,     0,     0,
      40,     0,     0,    42,     0,     0,   188,     0,     0,    46,
      47,     0,     0,    49,    50,     0,    51,    52,     0,    53,
      54,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,     0,     0,     0,    64,   189,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     190,     0,    77,     0,  1128,    79,     0,     0,     0,    83,
       0,     0,    85,   912,     0,    87,     0,     0,   241,   242,
     914,   243,     0,     0,     0,     0,   244,     0,     0,     0,
       0,     0,     0,     0,   245,     0,     0,     0,     0,     0,
     917,    93,     0,     0,     0,     0,   247,     0,     0,     0,
     248,     0,     0,   249,     0,   920,     0,     0,     0,     0,
       0,     0,     0,   250,     0,     0,     0,     0,     0,   922,
     252,   104,     0,   192,     0,     0,     0,   253,     0,     0,
       0,     0,     0,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,     0,   365,   255,   256,     0,   257,     0,
     258,   366,  1129,     0,     0,   927,     0,     0,     0,   261,
       0,     0,   262,   367,     0,   263,     0,     0,     0,     0,
       0,     0,     0,     0,   476,   396,   397,   398,   399,   400,
       0,     0,   403,   404,   405,   406,     0,   408,   409,   930,
     931,   932,   933,   934,   935,     0,   936,     0,     0,     0,
     937,   938,   939,   940,   941,   942,   943,   944,   945,   946,
     947,     0,   948,     0,     0,   949,   950,   951,   952,   368,
     365,     0,     0,   369,     0,     0,     0,   366,     0,     0,
       0,     0,   475,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,   476,   396,   397,   398,   399,
     400,     0,     0,   403,   404,   405,   406,     0,   408,   409,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,   368,   391,   392,   393,   369,
     425,     0,   365,     0,     0,     0,     0,   394,   983,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,     0,     0,     0,     0,
     370,   476,   396,   397,   398,   399,   400,     0,     0,   403,
     404,   405,   406,     0,   408,   409,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,     0,   391,   392,   393,     0,   425,   368,   365,     0,
       0,   369,     0,   394,     0,   366,     0,     0,     0,     0,
     990,     0,     0,     0,     0,     0,     0,   367,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,   476,   396,   397,   398,   399,   400,     0,
       0,   403,   404,   405,   406,     0,   408,   409,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,     0,   368,   391,   392,   393,   369,   425,     0,
     365,     0,     0,     0,     0,   394,  1142,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,     0,     0,     0,   370,   476,
     396,   397,   398,   399,   400,     0,     0,   403,   404,   405,
     406,     0,   408,   409,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,     0,
     391,   392,   393,     0,   425,   368,   365,     0,     0,   369,
       0,   394,     0,   366,     0,     0,     0,     0,  1143,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     370,   476,   396,   397,   398,   399,   400,     0,     0,   403,
     404,   405,   406,     0,   408,   409,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,   368,   391,   392,   393,   369,   425,     0,   365,     0,
       0,     0,     0,   394,  1144,   366,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,     0,     0,
       0,     0,     0,     0,     0,     0,   370,   476,   396,   397,
     398,   399,   400,     0,     0,   403,   404,   405,   406,     0,
     408,   409,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,   425,   368,   365,     0,     0,   369,     0,   394,
       0,   366,     0,     0,     0,     0,  1145,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   370,   476,
     396,   397,   398,   399,   400,     0,     0,   403,   404,   405,
     406,     0,   408,   409,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   368,
     391,   392,   393,   369,   425,     0,   365,     0,     0,     0,
       0,   394,  1169,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,   370,   476,   396,   397,   398,   399,
     400,     0,     0,   403,   404,   405,   406,     0,   408,   409,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,     0,   391,   392,   393,     0,
     425,   368,     0,     0,     0,   369,     0,   394,   365,     0,
       0,     0,     0,     0,  1170,   366,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,   459,     0,
       0,     0,     0,     0,     0,     0,   370,   476,   396,   397,
     398,   399,   400,   460,     0,   403,   404,   405,   406,     0,
     408,   409,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,   425,   368,   365,     0,     0,   369,     0,   394,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   456,     0,     0,     0,   370,   465,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   368,
     391,   392,   393,   369,   425,     0,   365,     0,     0,     0,
       0,   394,     0,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,   453,     0,     0,     0,
     456,     0,     0,     0,   370,     0,     0,     0,     0,     0,
       0,   454,     0,     0,     0,     0,     0,     0,     0,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,     0,   391,   392,   393,     0,
     425,   368,     0,     0,     0,   369,     0,   394,     0,   365,
       0,     0,     0,     0,     0,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,   468,     0,     0,   370,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,   469,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,   425,     0,   368,     0,     0,     0,   369,   394,
       0,     0,   365,   751,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,   752,     0,     0,   370,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,   753,   384,   385,   386,   387,   388,   389,   390,     0,
     470,   391,   392,   393,     0,     0,     0,   368,   365,     0,
       0,   369,   394,     0,     0,   366,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,   830,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,     0,   368,   391,   392,   393,   369,     0,     0,
     365,     0,     0,     0,     0,   394,     0,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
     273,     0,     0,     0,   456,     0,     0,     0,   370,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,     0,
     391,   392,   393,     0,   425,   368,   365,     0,     0,   369,
       0,   394,     0,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   456,     0,     0,     0,
     370,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,   368,   391,   392,   393,   369,   425,   365,     0,     0,
       0,     0,     0,   394,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,   273,     0,     0,
       0,     0,   177,     0,     0,     0,   370,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,   368,   365,     0,     0,   369,     0,     0,   394,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,   620,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   370,   621,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   368,   391,
     392,   393,   369,   425,   365,     0,     0,     0,     0,     0,
     394,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,   622,     0,     0,     0,     0,     0,
       0,     0,     0,   370,     0,     0,     0,     0,     0,   623,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,     0,   391,   392,   393,     0,   368,
     365,     0,     0,   369,     0,     0,   394,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
     822,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,   368,   391,   392,   393,   369,
       0,     0,   365,     0,     0,     0,     0,   394,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,     0,     0,     0,     0,
     370,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,     0,   391,   392,   393,     0,   425,   368,   365,     0,
       0,   369,     0,   394,     0,   366,     0,     0,     0,     0,
     848,     0,     0,     0,     0,     0,     0,   367,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,     0,   368,   391,   392,   393,   369,   425,     0,
       0,   365,     0,     0,     0,   394,     0,     0,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     367,     0,     0,     0,   456,  1042,     0,     0,   370,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
    1043,   384,   385,   386,   387,   388,   389,   390,     0,     0,
     391,   392,   393,     0,     0,     0,   368,   365,     0,   889,
     369,   394,     0,     0,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   370,     0,     0,     0,     0,     0,     0,   365,     0,
       0,     0,     0,     0,     0,   366,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,   367,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,     0,   368,   391,   392,   393,   369,     0,     0,     0,
       0,     0,     0,   365,   394,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,   370,     0,     0,
       0,     0,     0,   368,     0,     0,     0,   369,     0,     0,
       0,     0,     0,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,   370,   391,
     392,   393,     0,   511,     0,     0,     0,     0,   368,     0,
     394,     0,   369,     0,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,   365,   515,
     391,   392,   393,   370,     0,   366,     0,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,   367,     0,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,   365,   517,   391,   392,   393,     0,     0,
     366,     0,     0,     0,     0,     0,   394,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   368,     0,     0,     0,   369,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,     0,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,   370,     0,
       0,   367,     0,     0,     0,     0,     0,     0,   368,     0,
       0,     0,   369,     0,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,     0,   519,
     391,   392,   393,   370,     0,     0,     0,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,   368,     0,   371,
     372,   369,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,   365,   525,   391,   392,   393,     0,     0,
     366,   542,   370,     0,     0,     0,   394,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,   365,     0,   391,   392,   393,     0,     0,   366,
       0,     0,     0,     0,     0,   394,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,   624,   365,   747,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,   370,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   368,     0,   371,
     372,   369,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,     0,   391,   392,   393,     0,     0,
       0,     0,   370,     0,     0,     0,   394,   368,     0,     0,
       0,   369,     0,     0,     0,     0,     0,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   377,   378,     0,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,   370,     0,   391,   392,   393,     0,     0,   365,
       0,   726,     0,     0,     0,   394,   366,     0,   371,   372,
       0,   373,   374,   375,     0,   376,   748,   378,   367,   379,
     380,   381,   382,   383,     0,   384,   385,   386,   387,   388,
     389,   390,     0,     0,   391,   392,   393,     0,     0,     0,
       0,     0,     0,     0,   365,   394,     0,     0,     0,     0,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   368,     0,     0,     0,   369,     0,
       0,     0,     0,     0,   365,     0,     0,     0,     0,     0,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,     0,   370,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   368,
       0,     0,     0,   369,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,  -576,   384,   385,   386,   387,   388,   389,   390,     0,
     177,   391,   392,   393,   370,     0,     0,     0,     0,   368,
       0,     0,   394,   369,     0,     0,     0,     0,     0,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,   370,     0,   391,   392,   393,     0,
       0,   365,   839,     0,     0,     0,     0,   394,   366,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
     367,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,   807,   391,   392,   393,     0,
       0,   365,     0,     0,     0,     0,     0,   394,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     367,     0,     0,   846,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   368,     0,     0,     0,
     369,   365,     0,     0,     0,     0,     0,     0,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     367,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   370,     0,     0,     0,     0,   368,     0,     0,     0,
     369,     0,     0,     0,     0,     0,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,   370,     0,   391,   392,   393,   368,     0,     0,     0,
     369,     0,     0,     0,   394,     0,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,   370,     0,   391,   392,   393,     0,     0,   365,     0,
       0,     0,     0,     0,   394,   366,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,   367,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,     0,     0,   391,   392,   393,     0,     0,     0,   365,
     873,     0,     0,     0,   394,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   368,     0,     0,     0,   369,     0,   365,
       0,     0,     0,     0,     0,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,   968,
       0,     0,     0,     0,     0,     0,     0,     0,   370,     0,
       0,     0,     0,     0,   368,     0,     0,     0,   369,     0,
       0,     0,     0,     0,   371,   372,     0,   373,   374,   375,
       0,   376,   377,   378,     0,   379,   380,   381,   382,   383,
       0,   384,   385,   386,   387,   388,   389,   390,   892,   370,
     391,   392,   393,     0,   368,     0,     0,   874,   369,     0,
       0,   394,     0,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,   370,
       0,   391,   392,   393,     0,     0,     0,     0,     0,     0,
       0,     0,   394,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,   365,
     288,   391,   392,   393,    22,    23,   366,     0,     0,     0,
       0,     0,   394,   289,     0,    30,   290,     0,   367,   971,
       0,    36,     0,     0,   -77,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   365,
       0,     0,   -48,     0,     0,     0,   366,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,     0,   367,     0,
       0,   -48,     0,     0,   291,     0,    66,     0,     0,     0,
       0,     0,     0,     0,   368,     0,     0,     0,   369,   365,
    1039,     0,     0,     0,     0,    82,   366,     0,    84,     0,
     -77,    86,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   370,
       0,     0,     0,     0,   368,     0,     0,     0,   369,     0,
       0,     0,     0,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,   370,
       0,   391,   392,   393,   368,     0,     0,     0,   369,     0,
       0,     0,   394,     0,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,   370,
       0,   391,   392,   393,     0,     0,   365,     0,     0,     0,
     974,     0,   394,   366,     0,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,   367,   379,   380,   381,   382,
     383,     0,   384,   385,   386,   387,   388,   389,   390,     0,
       0,   391,   392,   393,     0,     0,   365,     0,     0,     0,
       0,     0,   394,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   368,     0,     0,     0,   369,   365,  1084,     0,     0,
       0,     0,     0,   366,  1064,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   370,     0,     0,     0,
       0,   368,     0,     0,     0,   369,     0,     0,     0,     0,
       0,     0,   371,   372,  1067,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,   370,     0,   391,   392,
     393,   368,     0,     0,     0,   369,     0,     0,     0,   394,
       0,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,     0,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,   370,     0,   391,   392,
     393,     0,     0,   365,     0,     0,     0,     0,     0,   394,
     366,     0,   371,   372,     0,   373,   374,   375,     0,   376,
     377,   378,   367,   379,   380,   381,   382,   383,     0,   384,
     385,   386,   387,   388,   389,   390,     0,     0,   391,   392,
     393,     0,     0,   365,     0,     0,     0,     0,     0,   394,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   370,     0,     0,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,     0,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,   370,     0,   391,   392,   393,     0,     0,
     365,     0,  1127,     0,     0,     0,   394,   366,     0,   371,
     372,     0,   373,   374,   375,     0,   376,   377,   378,   367,
     379,   380,   381,   382,   383,     0,   384,   385,   386,   387,
     388,   389,   390,     0,     0,   391,   392,   393,     0,     0,
     365,     0,     0,     0,  1165,     0,   394,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   368,     0,     0,     0,   369,
     365,     0,     0,     0,     0,     0,     0,   366,  1184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     370,     0,     0,     0,     0,   368,     0,     0,     0,   369,
       0,     0,     0,     0,     0,     0,   371,   372,  1185,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
     370,     0,   391,   392,   393,   368,     0,     0,     0,   369,
       0,     0,     0,   394,     0,     0,   371,   372,  1186,   373,
     374,   375,     0,   376,   377,   378,     0,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
     370,     0,   391,   392,   393,     0,     0,   365,     0,     0,
       0,     0,     0,   394,   366,     0,   371,   372,     0,   373,
     374,   375,     0,   376,   377,   378,   367,   379,   380,   381,
     382,   383,     0,   384,   385,   386,   387,   388,   389,   390,
       0,     0,   391,   392,   393,     0,     0,   365,     0,     0,
       0,     0,     0,   394,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   368,     0,     0,     0,   369,   365,     0,     0,
       0,     0,     0,     0,   366,  1187,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   370,     0,     0,
       0,     0,   368,     0,     0,     0,   369,     0,     0,     0,
       0,     0,     0,   371,   372,  1188,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,   370,     0,   391,
     392,   393,   368,     0,     0,     0,   369,     0,     0,     0,
     394,     0,     0,   371,   372,  1189,   373,   374,   375,     0,
     376,   377,   378,     0,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,   370,     0,   391,
     392,   393,     0,     0,   365,     0,     0,     0,     0,     0,
     394,   366,     0,   371,   372,     0,   373,   374,   375,     0,
     376,   377,   378,   367,   379,   380,   381,   382,   383,     0,
     384,   385,   386,   387,   388,   389,   390,     0,     0,   391,
     392,   393,     0,     0,   365,     0,     0,     0,     0,     0,
     394,   366,  1213,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   368,
       0,     0,     0,   369,   365,     0,     0,     0,     0,     0,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,     0,     0,     0,     0,   368,
       0,     0,     0,   369,     0,     0,     0,     0,     0,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,   370,     0,   391,   392,   393,   368,
       0,     0,     0,   369,     0,     0,     0,   394,     0,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
       0,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,   370,     0,   391,   392,   393,     0,
       0,   365,     0,     0,     0,  1242,     0,   394,   366,     0,
     371,   372,     0,   373,   374,   375,     0,   376,   377,   378,
     367,   379,   380,   381,   382,   383,     0,   384,   385,   386,
     387,   388,   389,   390,     0,     0,   391,   392,   393,     0,
       0,     0,     0,     0,     0,     0,     0,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   793,     0,     0,     0,
     369,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   370,     0,     0,   368,     0,     0,     0,   369,     0,
       0,     0,     0,     0,     0,     0,     0,   371,   372,     0,
     373,   374,   375,     0,   376,   377,   378,     0,   379,   380,
     381,   382,   383,     0,   384,   385,   386,   387,   388,   389,
     390,     0,     0,   391,   392,   393,     0,     0,     0,     0,
       0,     0,     0,     0,   394,   371,   372,     0,   373,   374,
     375,     0,   376,   377,   378,     0,     0,   380,   381,   382,
       0,     0,   384,   385,   386,   387,     0,   758,   390,   240,
       0,   391,   392,   393,     0,     0,     0,     0,     0,     0,
       0,     0,   394,   241,   242,     0,   243,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,   245,
       0,     0,     0,     0,     0,   246,     0,     0,     0,     0,
    -297,   247,     0,     0,     0,   248,     0,     0,   249,     0,
       0,     0,     0,     0,  -297,  -297,     0,  -297,   250,     0,
       0,     0,  -297,     0,   251,   252,     0,     0,     0,     0,
    -297,     0,   253,     0,     0,     0,  -297,     0,     0,     0,
       0,   254,  -297,     0,     0,     0,  -297,     0,     0,  -297,
     255,   256,     0,   257,     0,   258,     0,   259,     0,  -297,
     260,     0,     0,     0,   261,  -297,  -297,   262,     0,     0,
     263,     0,     0,  -297,     0,     0,     0,     0,     0,     0,
       0,     0,  -297,     0,     0,   240,     0,     0,     0,     0,
       0,  -297,  -297,     0,  -297,     0,  -297,     0,  -297,   241,
     242,  -297,   243,     0,     0,  -297,     0,   244,  -297,     0,
       0,  -297,     0,     0,     0,   245,     0,     0,     0,     0,
       0,   246,     0,     0,     0,     0,  -288,   247,     0,     0,
       0,   248,     0,     0,   249,     0,     0,     0,     0,     0,
    -288,  -288,     0,  -288,   250,     0,     0,     0,  -288,     0,
     251,   252,     0,     0,     0,     0,  -288,     0,   253,     0,
       0,     0,  -288,     0,     0,     0,     0,   254,  -288,     0,
       0,     0,  -288,     0,     0,  -288,   255,   256,     0,   257,
       0,   258,     0,   259,     0,  -288,   260,     0,     0,     0,
     261,  -288,  -288,   262,     0,     0,   263,     0,     0,  -288,
       0,     0,     0,     0,     0,     0,     0,     0,  -288,     0,
       0,  -174,     0,     0,     0,     0,     0,  -288,  -288,     0,
    -288,     0,  -288,     0,  -288,  -174,  -174,  -288,  -174,     0,
       0,  -288,     0,  -174,  -288,     0,     0,  -288,     0,     0,
       0,  -174,     0,     0,     0,     0,     0,  -174,     0,     0,
       0,     0,  -170,  -174,     0,     0,     0,  -174,     0,     0,
    -174,     0,     0,     0,     0,     0,  -170,  -170,     0,  -170,
    -174,     0,     0,     0,  -170,     0,  -174,  -174,     0,     0,
       0,     0,  -170,     0,  -174,     0,     0,     0,  -170,     0,
       0,     0,     0,  -174,  -170,     0,     0,     0,  -170,     0,
       0,  -170,  -174,  -174,     0,  -174,     0,  -174,     0,  -174,
       0,  -170,  -174,     0,     0,     0,  -174,  -170,  -170,  -174,
       0,     0,  -174,     0,     0,  -170,     0,     0,     0,     0,
       0,     0,     0,     0,  -170,     0,     0,     0,     0,     0,
       0,     0,     0,  -170,  -170,     0,  -170,     0,  -170,     0,
    -170,     0,     0,  -170,     0,     0,     0,  -170,     0,     0,
    -170,     0,     0,  -170
};

static const yytype_int16 yycheck[] =
{
      12,    63,    67,   469,    16,     2,   520,   652,    20,   767,
     161,    33,    34,    35,    26,   205,   490,   487,    30,    31,
     239,    33,    34,    35,    36,    37,   753,   752,   416,   483,
     868,   221,   640,    19,    22,   643,    48,   495,    50,     1,
     105,   698,   903,    55,     1,    57,    27,    59,   213,   958,
     959,     1,    32,    32,     3,    67,    68,    69,    70,    71,
      72,  1013,   520,    32,    76,  1100,    78,   286,    80,    81,
      47,    32,    47,    85,   239,   513,    88,    89,    32,    91,
      67,    93,    90,   105,    96,    97,    27,    99,    47,   308,
    1151,   103,   104,   105,   106,     1,    77,   163,     1,    90,
     144,  1088,    36,    60,   144,    47,   138,    88,     0,   147,
      47,    55,    68,  1010,   146,   144,   103,   104,   105,    47,
      47,   286,   116,    73,    55,   963,   107,   192,   118,   173,
       3,   171,     3,    90,   779,   792,    77,    71,   118,   118,
     169,   118,  1203,   308,    47,   157,     3,    88,   104,    20,
     144,    88,   101,    73,    60,   163,   228,   229,   230,   116,
     168,   123,    33,  1150,   315,   115,   107,   144,   206,   144,
     182,   625,  1124,   185,   186,   187,  1014,   168,   190,   191,
     192,   143,   144,   144,    90,   144,   143,  1222,   168,   168,
    1051,    62,  1089,   412,   413,   115,   173,   103,   173,   168,
     638,   213,   144,   811,   191,   192,   814,   168,   865,   171,
     116,   168,   377,  1074,   168,  1076,   144,   144,   204,   169,
     123,  1130,  1131,   697,    60,   287,   170,   239,   101,   217,
     101,   173,   100,    90,   222,    18,   107,   143,    46,   170,
     143,   144,    60,   231,   101,   173,   173,   412,   413,   169,
    1088,    47,  1203,    53,    90,    46,    64,   163,    27,   144,
     163,   144,   168,   170,   652,   172,   163,   103,   144,   341,
     342,   118,    72,   170,   286,    83,   118,  1138,   290,  1140,
     116,    50,    82,   144,   169,   297,   298,   753,   171,    55,
       3,  1242,    88,    76,   144,   171,   308,   168,   116,   163,
     168,   755,  1060,   144,   112,   169,    75,    20,    77,   123,
     171,    94,  1150,   163,  1041,  1040,  1043,  1042,   483,    88,
      33,   112,     3,   979,   336,   981,   173,   163,   111,    55,
     171,   173,   168,   144,   123,   149,   333,   151,   152,   351,
     352,   155,   156,   988,   113,    55,   368,   369,   513,    62,
     804,   363,   163,   365,   366,   367,   368,   369,   370,   148,
     372,   373,   374,   375,   376,    31,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   144,    55,   859,   451,   857,   101,   401,
     402,   779,   414,   415,   107,   407,   144,   144,   410,   411,
     412,   413,   414,   415,   416,   427,   444,   144,   144,   144,
     171,   449,   144,  1220,   170,   427,   172,  1073,   138,  1075,
     432,    87,   434,   171,   171,    91,   146,    81,    88,   895,
      84,  1238,   454,   903,   171,   171,   171,   169,   460,   451,
      60,   453,   454,   465,    55,   401,   402,   459,   460,   144,
     144,   407,   170,   465,   410,   411,   468,   469,   163,   144,
     625,   123,   144,   475,   451,   170,   478,   172,   144,   163,
      90,   483,   138,   638,   144,    27,   171,   143,   163,   145,
     146,   163,   144,   103,   150,   497,   148,   144,    27,   501,
     502,   556,   158,   163,   506,   171,   116,   170,   164,   165,
     166,   513,   144,   138,   144,   144,  1020,   163,    47,   171,
      31,   146,    51,   169,   171,    55,   538,   529,    55,   506,
     532,   163,   534,   163,   163,    77,   538,   539,   540,    87,
     542,   543,    48,    91,   724,    74,    88,   170,    77,   172,
      79,  1156,   529,   163,   556,   532,    87,   534,   168,    88,
      91,   548,   539,   217,    70,  1013,    55,   170,   222,   172,
      55,   113,  1020,    55,    80,    81,    87,    31,   107,   556,
      91,   168,   170,   170,   172,  1041,   532,  1043,   534,   170,
       8,   172,   747,   748,   656,   657,  1056,   170,   660,   661,
     755,    98,  1207,  1208,  1209,  1210,  1211,  1212,    55,   621,
     988,   623,    80,    81,  1074,   144,  1076,    47,   620,   621,
     622,   623,   624,   625,   626,   627,   767,   138,    55,   532,
      60,   534,   143,    87,   636,   146,   638,    91,   170,   150,
      55,    55,   171,    55,   646,    55,   648,   168,   703,   804,
     652,    60,   654,   629,   630,    83,    55,   138,   123,   636,
      90,   163,   143,   162,    55,   146,   154,   669,   670,   150,
     163,   733,   734,   163,   170,   117,  1124,   163,  1138,   144,
    1140,    90,    86,   148,   138,   139,   116,   141,   142,   143,
     636,   145,   146,   123,   103,   168,   150,   203,    86,   701,
     702,   703,   163,  1151,   158,   707,   171,   116,   163,     8,
     164,   165,   166,   163,   144,   163,   123,  1165,   163,   225,
     226,   227,   228,   229,   230,    27,   703,    31,    64,   170,
     236,   237,   170,   163,   170,   170,   170,   163,   168,   170,
     163,   171,   143,   745,    31,    67,   168,   749,   146,   751,
     752,   753,   163,   755,   163,  1203,   123,   170,   144,   168,
     266,   707,    47,   171,   170,   123,   163,    47,   123,    34,
     276,    34,  1220,   144,   123,    77,   282,   779,   171,   171,
      60,   793,   794,    87,   171,    21,    88,    91,   171,   173,
    1238,   793,   794,   795,  1242,   173,   798,   144,   304,   168,
      87,   163,   804,   163,    91,   107,   170,   163,   171,   163,
      90,   113,  1021,  1022,   173,   163,   322,   323,   820,   163,
     822,   163,   877,   144,   170,   827,   163,    34,   830,   163,
       2,    34,    64,   171,   138,   837,   116,   839,   169,   143,
      12,   145,   146,   123,   846,    27,   150,   171,   144,   169,
     123,   138,   139,    25,   158,    27,   143,    88,   145,   146,
     171,    32,   166,   150,   144,    47,  1021,  1022,    32,    51,
     173,   158,    34,   168,   168,   877,   143,   164,   165,   166,
     163,   173,    88,   163,   170,   887,   170,   889,   168,   143,
     892,   171,    74,   895,    27,    77,   173,    79,   173,   163,
     877,     2,   163,   905,    76,   146,    88,   169,   146,   146,
     170,    12,   138,   146,    47,   138,   146,   146,    51,  1060,
     168,   146,   428,   138,    25,   107,    27,    47,   146,   146,
     146,   103,   146,   138,   138,   146,   108,    34,   103,   163,
      60,    74,   162,   171,    77,   138,    79,   168,   138,   171,
     163,   953,   458,   163,   162,    88,   462,   144,   464,   171,
     745,   467,   171,   163,   163,   967,   968,   163,   970,   971,
      90,   973,   974,   481,   107,    76,   173,    24,   348,    80,
      81,   983,   984,    88,   986,   852,   988,   316,   990,   991,
     881,   906,   994,  1060,   690,  1056,   116,  1049,   857,  1222,
     909,  1180,   103,   123,  1206,  1213,   178,   108,    36,    30,
      -1,   498,    -1,    -1,    -1,    -1,    -1,  1019,  1020,  1021,
    1022,    -1,    -1,    -1,   144,  1027,    -1,  1029,    -1,    -1,
      -1,  1033,    -1,    70,    -1,  1090,  1013,  1039,  1040,  1041,
    1042,  1043,    -1,   163,    -1,    -1,    -1,    -1,   168,    -1,
       1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,
      -1,    22,    -1,    -1,    -1,    26,    27,   178,    -1,    -1,
      -1,    -1,  1084,    -1,    35,    -1,    37,    38,  1090,    -1,
      -1,    -1,    43,  1095,    -1,   267,    -1,  1099,    -1,    50,
      -1,   273,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1090,    -1,    -1,    -1,    -1,  1095,    -1,
    1175,    72,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,
      81,    82,    -1,    84,    -1,    86,    -1,    88,    -1,    -1,
    1142,  1143,  1144,  1145,   650,   651,    -1,    -1,    -1,    -1,
     656,   657,    -1,    -1,   660,   661,   107,    -1,    -1,   110,
      -1,   333,   113,  1165,    -1,    -1,   267,  1169,  1170,    -1,
     676,    -1,   273,  1175,  1151,    -1,   348,    -1,    -1,    -1,
      -1,    -1,  1184,  1185,  1186,  1187,  1188,  1189,   225,   226,
     227,   228,   229,   230,    -1,    -1,    -1,    -1,  1175,    -1,
    1202,  1203,    -1,  1205,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,   720,    -1,    -1,    -1,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1203,    -1,    -1,    -1,
      -1,    -1,   333,    -1,    -1,    -1,  1238,    -1,    -1,   276,
    1242,    -1,    -1,    -1,    -1,   282,    -1,   348,    -1,    -1,
      -1,    -1,    -1,   425,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,  1242,    -1,   304,    35,    -1,
      37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,     2,
      -1,   453,    -1,    50,    -1,   322,   323,   459,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   805,
     806,   473,    25,   475,    27,    -1,    -1,    -1,    75,    -1,
      77,   817,    79,   819,    81,    -1,    -1,    84,    -1,    86,
     826,    88,    -1,   829,   425,    -1,   832,    -1,   834,    -1,
     836,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,   113,   853,    -1,    -1,
      -1,    -1,   453,    76,    -1,    -1,    -1,    -1,   459,    -1,
      -1,    -1,    -1,    -1,   536,    -1,    -1,     3,   540,    -1,
      -1,    -1,   473,    -1,   475,    -1,   548,    -1,   884,    -1,
     103,    17,    18,    -1,    20,   108,    -1,    -1,    -1,    25,
      -1,   428,    -1,    -1,    31,    -1,   163,    33,    -1,    -1,
      -1,    -1,   169,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,
      -1,   458,    -1,    -1,    -1,   462,    62,   464,    -1,    -1,
     467,    -1,    68,    69,    31,   536,    -1,    -1,    -1,   540,
      76,    -1,    -1,    -1,    -1,   617,    -1,   548,    -1,    85,
      87,    -1,    -1,    -1,    91,   178,    -1,    -1,    94,    95,
      -1,    97,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,   108,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
     672,   138,   139,   675,   141,   142,   143,    -1,   145,   146,
     147,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,  1025,
     157,   158,    -1,    -1,   161,    -1,    -1,   164,   165,   166,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,   175,    -1,
      -1,   138,   139,    -1,   267,   142,   143,    -1,   145,   146,
     273,    -1,    -1,   150,    -1,     2,    -1,    -1,    -1,    -1,
     732,   158,    -1,    -1,    -1,    12,   738,   164,   165,   166,
      -1,   672,  1078,  1079,   675,    -1,  1082,  1083,    25,    31,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1100,   767,    -1,    -1,    -1,    -1,
      -1,    -1,  1108,    -1,  1110,    -1,  1112,    -1,    -1,    -1,
     333,    -1,    -1,   650,   651,    -1,    -1,    -1,    -1,   656,
     657,   793,   794,   660,   661,   348,    -1,    -1,    -1,    76,
      -1,   732,    -1,    -1,    -1,    87,    -1,   738,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   820,    -1,
     822,    -1,    -1,    -1,    -1,   827,   103,    -1,   830,     2,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,   843,    -1,   845,    -1,    -1,   848,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,   425,    -1,   876,   157,   158,    -1,    -1,   161,
      -1,    -1,   164,   165,   166,   887,  1222,   889,    -1,   820,
      -1,   822,    -1,    -1,    -1,    -1,   827,    -1,    -1,   830,
     453,   178,    -1,    76,    -1,    -1,   459,    -1,    -1,    -1,
      -1,    -1,   843,    -1,   845,    -1,    -1,   848,    -1,    -1,
     473,    -1,   475,    -1,    -1,    -1,    -1,    22,    -1,    -1,
     103,    26,    27,    -1,    -1,   108,    -1,    -1,   805,   806,
      35,    -1,    37,    38,    -1,   876,    -1,    -1,    43,    -1,
     817,    -1,    -1,    -1,    -1,    50,   887,    -1,   889,    -1,
      -1,    -1,   829,    -1,    -1,   832,    -1,   834,    -1,   836,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,   983,    77,   536,    79,    -1,    81,   540,   990,    84,
     267,    86,    -1,    88,    -1,   548,   273,    -1,    -1,    -1,
      -1,    -1,    -1,  1005,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,  1023,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,
    1032,    -1,    -1,    -1,  1036,    35,    -1,    37,    38,    -1,
      -1,    -1,    22,    43,    -1,    -1,    26,    27,    -1,    -1,
      50,    -1,   983,    -1,    -1,    35,   333,    37,    38,   990,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,   163,    -1,
      50,   348,    -1,    -1,  1005,    75,    -1,    77,    -1,    79,
      -1,    81,    -1,  1085,    84,    -1,    86,    -1,    88,    -1,
      -1,    -1,  1023,    -1,   267,    75,    -1,    77,    -1,    -1,
     273,  1032,    -1,    -1,    -1,  1036,    86,   107,    88,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,   672,
      -1,    -1,   675,    -1,    -1,    -1,    -1,   107,    -1,    -1,
     110,    22,    -1,   113,    -1,    26,    27,    -1,    -1,    -1,
    1142,  1143,  1144,  1145,    35,    -1,    37,    38,   425,    -1,
      -1,    -1,    43,    -1,  1085,    -1,    -1,    -1,  1025,    50,
     333,    -1,    -1,   163,    -1,    -1,    -1,  1169,  1170,   169,
      -1,    -1,    -1,    -1,    -1,   348,   453,    -1,    -1,   732,
      -1,    -1,   459,    -1,    75,   738,    77,    -1,    79,    -1,
      81,    -1,    -1,    84,    -1,    86,   473,    88,   475,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       2,  1142,  1143,  1144,  1145,    -1,   107,    -1,    -1,   110,
      12,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,  1169,  1170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   425,    -1,    -1,    -1,    -1,    -1,    -1,   536,
      -1,    -1,    -1,   540,    -1,    -1,    -1,   820,    -1,   822,
      -1,   548,   163,    -1,   827,    -1,    -1,   830,   169,    -1,
     453,    -1,     3,    -1,    76,    -1,   459,    -1,    -1,    -1,
     843,    -1,   845,    -1,    -1,   848,    17,    18,    -1,    20,
     473,    -1,   475,    -1,    25,    -1,    -1,    -1,    -1,    -1,
       2,   103,    33,    -1,    -1,    -1,   108,    -1,    39,    -1,
      12,    -1,    -1,   876,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    25,   887,    27,   889,    -1,    -1,    -1,
       3,    62,    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    17,    76,    -1,    20,    -1,    -1,
      -1,    -1,    25,   536,    85,    -1,    -1,   540,    -1,    -1,
      -1,    -1,    35,    94,    95,   548,    97,    -1,    99,    -1,
     101,    -1,    45,   104,    76,    -1,   178,   108,    -1,    52,
     111,    -1,    -1,   114,    -1,   672,    -1,    -1,   675,    62,
      -1,    -1,    -1,    -1,    -1,    22,    69,    -1,    -1,    26,
      27,   103,    -1,    -1,    -1,    -1,   108,    -1,    35,    -1,
      37,    38,    85,    -1,    -1,    -1,    43,    -1,    -1,    -1,
     983,    -1,    -1,    50,    97,    -1,    -1,   990,   101,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,   168,    -1,   170,
      -1,   114,  1005,    -1,    -1,   732,    -1,    -1,    75,    -1,
      77,   738,    79,    -1,    81,    -1,    -1,    84,    -1,    86,
    1023,    88,    -1,    -1,    -1,   267,    -1,    -1,    -1,  1032,
      -1,   273,    -1,  1036,    -1,    -1,   178,     2,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,   113,    12,    -1,   672,
      -1,    -1,   675,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,  1085,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   333,    -1,   820,   154,   822,   163,    -1,    -1,    -1,
     827,    -1,   169,   830,    -1,    -1,   348,   167,    -1,   732,
     170,    76,   172,    -1,    -1,   738,   843,    -1,   845,    -1,
      -1,   848,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,
      -1,   273,    -1,    -1,    -1,    -1,     2,    -1,   103,  1142,
    1143,  1144,  1145,   108,    -1,    -1,    12,    -1,    -1,   876,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
     887,    27,   889,    -1,    -1,    -1,  1169,  1170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,   425,    -1,    -1,    -1,    -1,    -1,    -1,
      35,   333,    37,    38,    -1,    -1,    -1,   820,    43,   822,
      -1,    -1,    -1,    -1,   827,    50,   348,   830,    -1,    -1,
      76,   453,    -1,   178,    -1,    -1,    -1,   459,    -1,    -1,
     843,    -1,   845,    -1,    -1,   848,    -1,    -1,    -1,    31,
      75,   473,    77,   475,    79,    -1,    81,   103,    -1,    84,
      -1,    86,   108,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   876,    -1,    -1,   983,    -1,    -1,    -1,
      -1,    -1,   107,   990,   887,   110,   889,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1005,    -1,
      -1,    -1,    -1,   425,    -1,    87,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,   536,    -1,  1023,    -1,   540,    -1,
      -1,    -1,   267,    -1,    -1,  1032,   548,    -1,   273,  1036,
      -1,   453,   178,    -1,    -1,    -1,    -1,   459,   163,    -1,
     122,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,
      -1,   473,    -1,   475,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,    -1,    -1,   155,   156,   157,   158,   159,  1085,   161,
     983,    -1,   164,   165,   166,    -1,    -1,   990,   333,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1005,   348,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   536,    -1,    -1,    -1,   540,    -1,
    1023,   267,    -1,    -1,    -1,    -1,   548,   273,    -1,  1032,
      -1,    -1,    -1,  1036,    -1,  1142,  1143,  1144,  1145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,
     672,    -1,    -1,   675,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,  1169,  1170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
     425,    -1,  1085,    -1,    -1,    -1,    -1,   333,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   348,    -1,    -1,    -1,    -1,    -1,   453,    -1,
     732,    -1,    -1,    -1,   459,    -1,   738,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    31,    -1,    -1,   473,    -1,
     475,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1142,
    1143,  1144,  1145,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     672,    -1,    -1,   675,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1169,  1170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   425,
      -1,    87,    -1,    -1,    -1,    91,    43,    44,    45,    -1,
      -1,   536,    -1,    -1,    -1,   540,    -1,    -1,   820,    -1,
     822,    -1,    -1,   548,    -1,   827,    -1,   453,   830,    -1,
     732,    -1,    -1,   459,    -1,    -1,   738,    -1,    -1,    -1,
      -1,   843,    -1,   845,   178,    82,   848,   473,    -1,   475,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,    -1,    -1,   155,
     156,   157,   158,   159,   876,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    -1,    -1,   887,    -1,   889,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,   142,    -1,    -1,    -1,    -1,
     536,    -1,    -1,    -1,   540,    -1,    -1,    -1,   820,    -1,
     822,    -1,   548,    -1,    -1,   827,    -1,    -1,   830,    -1,
      -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,   273,
      31,   843,    -1,   845,    -1,    -1,   848,   672,    -1,    -1,
     675,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      -1,    -1,    -1,    -1,   876,    -1,    -1,    -1,    -1,    -1,
      -1,   983,    -1,    -1,   221,   887,   223,   889,   990,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,   234,   235,   333,
      91,    -1,    -1,  1005,    -1,    -1,    -1,   732,    -1,    -1,
      -1,    -1,    -1,   738,   348,    -1,    -1,    -1,    -1,    -1,
      -1,  1023,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1032,   122,   269,    -1,  1036,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   672,   138,   139,   675,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,    -1,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,   983,    -1,  1085,   175,    -1,    -1,    -1,   990,    -1,
      -1,   425,    -1,    -1,    -1,   820,    -1,   822,    -1,    -1,
      -1,    -1,   827,  1005,    -1,   830,   732,    -1,    -1,    -1,
      -1,    -1,   738,    -1,    -1,    -1,   353,    -1,   843,   453,
     845,  1023,    -1,   848,    -1,   459,    -1,    -1,    -1,    -1,
    1032,    -1,    -1,    -1,  1036,    -1,    -1,    -1,    -1,   473,
    1142,  1143,  1144,  1145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   876,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   887,    -1,   889,    -1,    -1,  1169,  1170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1085,    -1,    -1,    -1,   424,    -1,    -1,
      -1,    -1,    -1,    -1,   820,    -1,   822,    -1,    -1,    -1,
      -1,   827,   536,    -1,   830,   442,   540,    -1,    -1,   446,
      -1,    -1,    -1,    -1,   548,    -1,    -1,   843,    -1,   845,
      -1,    -1,   848,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1142,  1143,  1144,  1145,    -1,    -1,    -1,   484,    -1,    -1,
     876,    -1,    -1,   490,    -1,    -1,    -1,     3,   983,    -1,
      -1,   887,    -1,   889,    -1,   990,    -1,  1169,  1170,    -1,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
    1005,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,  1023,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,  1032,    -1,    -1,
      -1,  1036,   549,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,   558,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   672,    85,
      -1,   675,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      -1,    97,    -1,    99,    -1,   101,    -1,   983,   104,    -1,
    1085,    -1,   108,    -1,   990,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1005,
      -1,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,  1023,   732,    -1,
      -1,    -1,   639,    -1,   738,   642,  1032,    -1,    -1,    -1,
    1036,    -1,    -1,    -1,    -1,    -1,    -1,  1142,  1143,  1144,
    1145,    -1,    -1,    -1,   170,   167,    -1,    -1,   170,    -1,
     172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1169,  1170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   693,    -1,    -1,  1085,
     697,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   720,    -1,    -1,   820,   724,   822,    -1,
      -1,     3,    -1,   827,    -1,    -1,   830,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,   843,
      -1,   845,    -1,    25,   848,    -1,  1142,  1143,  1144,  1145,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
     767,    -1,     3,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,   876,  1169,  1170,    -1,    17,    18,    -1,    20,
      62,    -1,    -1,   887,    25,   889,    68,    69,    -1,    -1,
      -1,    -1,    33,    -1,    76,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    85,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    94,    95,    -1,    97,    -1,    99,    -1,   101,
      -1,    62,   104,    -1,    -1,    -1,   108,    68,    69,   111,
      -1,    -1,   114,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   850,    85,   852,    -1,    -1,    -1,   856,
      -1,    -1,   859,    94,    95,    -1,    97,    -1,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,   881,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   906,
      -1,  1005,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1023,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1032,    -1,
       0,     1,  1036,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,  1085,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    -1,    72,    -1,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,   101,   102,    -1,   104,   105,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
      -1,    -1,   122,  1060,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,     1,   175,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    -1,    -1,    75,    76,
      77,    78,    -1,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,   101,   102,    -1,   104,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,    -1,   149,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,   168,   169,   170,    -1,   172,    -1,     1,   175,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    -1,    68,    69,    70,    -1,    72,    -1,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    92,    93,
      94,    95,    96,    97,    98,    99,    -1,   101,   102,    -1,
     104,   105,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,
      -1,    -1,   166,    -1,   168,   169,   170,    -1,   172,    -1,
       1,   175,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    -1,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    90,
      -1,    92,    93,    94,    95,    96,    97,    98,    99,    -1,
     101,   102,    -1,   104,   105,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,   163,    -1,    -1,   166,    -1,   168,   169,   170,
      -1,   172,    -1,     1,   175,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      68,    69,    70,    -1,    72,    -1,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    92,    93,    94,    95,    96,    97,
      98,    99,    -1,   101,   102,    -1,   104,   105,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,    -1,    -1,   117,
      -1,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,
     168,   169,   170,    -1,   172,    -1,     1,   175,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    -1,    68,    69,    70,    -1,    72,    -1,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,   101,   102,    -1,   104,
     105,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,
      -1,   166,    -1,   168,   169,   170,    -1,   172,    -1,     1,
     175,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    -1,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    90,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,   101,
     102,    -1,   104,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,   163,    -1,    -1,   166,    -1,   168,   169,   170,    -1,
     172,    -1,     1,   175,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    -1,    68,
      69,    70,    -1,    72,    -1,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    90,    -1,    92,    93,    94,    95,    96,    97,    98,
      99,    -1,   101,   102,    -1,   104,   105,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,     1,   175,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    -1,    -1,    75,
      76,    77,    78,    -1,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    92,    93,    94,    95,
      96,    97,    98,    99,    -1,   101,   102,    -1,   104,   105,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,    -1,     1,   175,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    -1,    68,    69,    70,    -1,    72,
      -1,    -1,    75,    76,    77,    78,    -1,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    92,
      93,    94,    95,    96,    97,    98,    99,    -1,   101,   102,
      -1,   104,   105,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
     163,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,     1,   175,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    -1,    72,    -1,    -1,    75,    76,    77,    78,    -1,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,   101,   102,    -1,   104,   105,   106,   107,   108,    -1,
     110,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,     1,   175,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    -1,    72,    -1,    -1,    75,    76,
      77,    78,    -1,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,   101,   102,    -1,   104,   105,   106,
     107,   108,    -1,   110,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,     1,   175,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    -1,    68,    69,    70,    -1,    72,    -1,
      -1,    75,    76,    77,    78,    -1,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    92,    93,
      94,    95,    96,    97,    98,    99,    -1,   101,   102,    -1,
     104,   105,   106,   107,   108,    -1,   110,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,
       1,   175,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    -1,    -1,    75,    76,    77,    78,    -1,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    90,
      -1,    92,    93,    94,    95,    96,    97,    98,    99,    -1,
     101,   102,    -1,   104,   105,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,    -1,     1,   175,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      68,    69,    70,    -1,    72,    -1,    -1,    75,    76,    77,
      78,    -1,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    92,    93,    94,    95,    96,    97,
      98,    99,    -1,   101,   102,    -1,   104,   105,   106,   107,
     108,    -1,   110,   111,   112,   113,   114,    -1,    -1,   117,
      -1,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,   172,    -1,     1,   175,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    -1,    68,    69,    70,    -1,    72,    -1,    -1,
      75,    76,    77,    78,    -1,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,   101,   102,    -1,   104,
     105,   106,   107,   108,    -1,   110,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,     1,
     175,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    -1,    -1,    75,    76,    77,    78,    -1,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    90,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,   101,
     102,    -1,   104,   105,   106,   107,   108,    -1,   110,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,   163,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,    -1,     1,   175,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    -1,    68,
      69,    70,    -1,    72,    -1,    -1,    75,    76,    77,    78,
      -1,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    90,    -1,    92,    93,    94,    95,    96,    97,    98,
      99,    -1,   101,   102,    -1,   104,   105,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,     1,   175,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,   173,     1,   175,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,     3,   175,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    77,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,    -1,    -1,   175,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,    -1,    -1,   104,   105,   106,    -1,   108,
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,   175,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,   105,
     106,    -1,   108,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,   171,   172,    -1,    -1,   175,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,    -1,
      -1,   104,   105,   106,    -1,   108,   109,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,   171,   172,
      -1,     3,   175,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    47,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,    -1,     3,   175,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,   105,   106,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,   175,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,    -1,   101,    -1,    -1,   104,   105,   106,    -1,
     108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,   172,    -1,     3,   175,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,    66,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,    -1,   104,   105,   106,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,     3,   175,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,   107,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,   175,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    77,    78,    -1,    80,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,
     175,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,
      -1,   175,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,    -1,     3,   175,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,    -1,    -1,   104,   105,   106,    -1,   108,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,   175,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,   169,   170,    -1,   172,    -1,    -1,   175,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,   173,
      -1,   175,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,   173,     3,   175,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,     3,   175,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,   175,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,   175,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,   107,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,
     175,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,   173,
       3,   175,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,   108,   109,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,    -1,   175,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,     3,   175,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,   175,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,   175,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,     3,   175,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,    -1,     3,   175,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,    -1,     3,   175,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,     3,   175,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,     3,   175,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,   172,    -1,     3,   175,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,     3,   175,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,   175,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,     3,
     175,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,
      -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,
       3,   175,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,     3,   175,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
     142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,    -1,     3,   175,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,
     141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,    -1,     3,   175,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,     3,   175,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,     3,   175,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,   172,    -1,     3,   175,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,   138,   139,   140,   141,   142,    -1,    -1,    -1,    12,
     147,    -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,
     157,    -1,    25,   160,   161,   162,    -1,    -1,    -1,   166,
      33,    -1,    35,   170,    -1,   172,    39,    -1,   175,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    97,    -1,    99,    -1,   101,    -1,
      -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,    -1,    -1,   131,   132,
     133,   134,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,    -1,   145,    -1,    -1,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,    -1,   161,    -1,
      -1,   164,   165,   166,   167,    -1,     3,   170,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,    -1,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,
      -1,   168,    -1,   170,   171,   172,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,    -1,   101,    -1,   103,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,   140,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,   162,   163,    -1,    -1,    -1,
      -1,   168,    33,   170,    35,   172,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    63,    -1,    65,    66,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,     3,   140,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,   162,    -1,    -1,    -1,    -1,    -1,   168,    33,   170,
      35,   172,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    63,    -1,
      65,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,    -1,   101,    -1,    -1,   104,
      -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,   140,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,   162,    -1,    -1,
      -1,    -1,    -1,   168,    33,   170,    35,   172,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    63,    -1,    65,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,    -1,   101,    -1,     3,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,    12,    -1,   114,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,   140,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    68,
      69,   170,    -1,   172,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    94,    95,    -1,    97,    -1,
      99,    19,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,    31,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
      -1,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
     139,   140,   141,   142,   143,    -1,   145,    -1,    -1,    -1,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,    -1,   161,    -1,    -1,   164,   165,   166,   167,    87,
      12,    -1,    -1,    91,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    87,   164,   165,   166,    91,
     168,    -1,    12,    -1,    -1,    -1,    -1,   175,   100,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    -1,   168,    87,    12,    -1,
      -1,    91,    -1,   175,    -1,    19,    -1,    -1,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    -1,    87,   164,   165,   166,    91,   168,    -1,
      12,    -1,    -1,    -1,    -1,   175,   100,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,   131,   132,   133,
     134,    -1,   136,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,    -1,   168,    87,    12,    -1,    -1,    91,
      -1,   175,    -1,    19,    -1,    -1,    -1,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,    87,   164,   165,   166,    91,   168,    -1,    12,    -1,
      -1,    -1,    -1,   175,   100,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
     126,   127,   128,    -1,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,   168,    87,    12,    -1,    -1,    91,    -1,   175,
      -1,    19,    -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,   131,   132,   133,
     134,    -1,   136,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    -1,    87,
     164,   165,   166,    91,   168,    -1,    12,    -1,    -1,    -1,
      -1,   175,   100,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
     168,    87,    -1,    -1,    -1,    91,    -1,   175,    12,    -1,
      -1,    -1,    -1,    -1,   100,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
     126,   127,   128,    47,    -1,   131,   132,   133,   134,    -1,
     136,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,   168,    87,    12,    -1,    -1,    91,    -1,   175,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   122,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    -1,    87,
     164,   165,   166,    91,   168,    -1,    12,    -1,    -1,    -1,
      -1,   175,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
     118,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
     168,    87,    -1,    -1,    -1,    91,    -1,   175,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    36,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    71,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,   168,    -1,    87,    -1,    -1,    -1,    91,   175,
      -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    71,   155,   156,   157,   158,   159,   160,   161,    -1,
     163,   164,   165,   166,    -1,    -1,    -1,    87,    12,    -1,
      -1,    91,   175,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    -1,    87,   164,   165,   166,    91,    -1,    -1,
      12,    -1,    -1,    -1,    -1,   175,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,   118,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,    -1,   168,    87,    12,    -1,    -1,    91,
      -1,   175,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
     122,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,    87,   164,   165,   166,    91,   168,    12,    -1,    -1,
      -1,    -1,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,    87,    12,    -1,    -1,    91,    -1,    -1,   175,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,    87,   164,
     165,   166,    91,   168,    12,    -1,    -1,    -1,    -1,    -1,
     175,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,    87,
      12,    -1,    -1,    91,    -1,    -1,   175,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    87,   164,   165,   166,    91,
      -1,    -1,    12,    -1,    -1,    -1,    -1,   175,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    -1,   168,    87,    12,    -1,
      -1,    91,    -1,   175,    -1,    19,    -1,    -1,    -1,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    -1,    87,   164,   165,   166,    91,   168,    -1,
      -1,    12,    -1,    -1,    -1,   175,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   118,    36,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      71,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,    -1,    -1,    -1,    87,    12,    -1,   173,
      91,   175,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    31,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    87,   164,   165,   166,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    12,   175,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,   122,   164,
     165,   166,    -1,   168,    -1,    -1,    -1,    -1,    87,    -1,
     175,    -1,    91,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    12,   163,
     164,   165,   166,   122,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    31,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    12,   163,   164,   165,   166,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,    -1,   163,
     164,   165,   166,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    87,    -1,   138,
     139,    91,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    12,   163,   164,   165,   166,    -1,    -1,
      19,   121,   122,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    12,    -1,   164,   165,   166,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,   138,
     139,    91,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,   175,    87,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    -1,    -1,    12,
      -1,   171,    -1,    -1,    -1,   175,    19,    -1,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,    31,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,   175,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    91,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
     118,   164,   165,   166,   122,    -1,    -1,    -1,    -1,    87,
      -1,    -1,   175,    91,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   122,    -1,   164,   165,   166,    -1,
      -1,    12,    13,    -1,    -1,    -1,    -1,   175,    19,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      31,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,   164,   165,   166,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   175,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   122,    -1,   164,   165,   166,    87,    -1,    -1,    -1,
      91,    -1,    -1,    -1,   175,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   122,    -1,   164,   165,   166,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   175,    19,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    31,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,    12,
     171,    -1,    -1,    -1,   175,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,   160,   161,   121,   122,
     164,   165,   166,    -1,    87,    -1,    -1,   171,    91,    -1,
      -1,   175,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,    12,
      22,   164,   165,   166,    26,    27,    19,    -1,    -1,    -1,
      -1,    -1,   175,    35,    -1,    37,    38,    -1,    31,    32,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    64,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    77,    -1,    -1,    31,    -1,
      -1,    83,    -1,    -1,    86,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    12,
      13,    -1,    -1,    -1,    -1,   107,    19,    -1,   110,    -1,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,   175,    -1,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,    -1,
     173,    -1,   175,    19,    -1,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    31,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   100,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,   175,
      -1,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   175,
      19,    -1,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,    31,   149,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   175,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    -1,    -1,
      12,    -1,   171,    -1,    -1,    -1,   175,    19,    -1,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,    31,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,
      12,    -1,    -1,    -1,   173,    -1,   175,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   100,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     122,    -1,   164,   165,   166,    87,    -1,    -1,    -1,    91,
      -1,    -1,    -1,   175,    -1,    -1,   138,   139,   100,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
     122,    -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   175,    19,    -1,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,    31,   149,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    91,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,   100,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   122,    -1,   164,
     165,   166,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,
     175,    -1,    -1,   138,   139,   100,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,   122,    -1,   164,
     165,   166,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     175,    19,    -1,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,    31,   149,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     175,    19,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    91,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   122,    -1,   164,   165,   166,    87,
      -1,    -1,    -1,    91,    -1,    -1,    -1,   175,    -1,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,   122,    -1,   164,   165,   166,    -1,
      -1,    12,    -1,    -1,    -1,   173,    -1,   175,    19,    -1,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
      31,   149,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,    -1,    -1,   150,   151,   152,
      -1,    -1,   155,   156,   157,   158,    -1,     1,   161,     3,
      -1,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
       3,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,    62,    -1,
      -1,    -1,    25,    -1,    68,    69,    -1,    -1,    -1,    -1,
      33,    -1,    76,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    85,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      94,    95,    -1,    97,    -1,    99,    -1,   101,    -1,    62,
     104,    -1,    -1,    -1,   108,    68,    69,   111,    -1,    -1,
     114,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    97,    -1,    99,    -1,   101,    17,
      18,   104,    20,    -1,    -1,   108,    -1,    25,   111,    -1,
      -1,   114,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,     3,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    62,    -1,    -1,    -1,    25,    -1,
      68,    69,    -1,    -1,    -1,    -1,    33,    -1,    76,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    85,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    94,    95,    -1,    97,
      -1,    99,    -1,   101,    -1,    62,   104,    -1,    -1,    -1,
     108,    68,    69,   111,    -1,    -1,   114,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,
      97,    -1,    99,    -1,   101,    17,    18,   104,    20,    -1,
      -1,   108,    -1,    25,   111,    -1,    -1,   114,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,     3,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      62,    -1,    -1,    -1,    25,    -1,    68,    69,    -1,    -1,
      -1,    -1,    33,    -1,    76,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    85,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    94,    95,    -1,    97,    -1,    99,    -1,   101,
      -1,    62,   104,    -1,    -1,    -1,   108,    68,    69,   111,
      -1,    -1,   114,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    97,    -1,    99,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   180,   181,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    48,
      49,    50,    52,    55,    56,    57,    58,    59,    61,    62,
      63,    65,    66,    68,    69,    70,    75,    76,    77,    78,
      79,    80,    81,    84,    85,    86,    88,    89,    90,    92,
      93,    94,    95,    96,    97,    98,    99,   101,   102,   104,
     105,   106,   107,   108,   110,   111,   113,   114,   117,   119,
     122,   138,   139,   140,   141,   142,   147,   157,   160,   161,
     162,   163,   166,   168,   170,   172,   175,   182,   183,   184,
     185,   186,   187,   190,   192,   198,   199,   200,   203,   204,
     208,   210,   213,   214,   216,   217,   218,   223,   224,   226,
     228,   231,   232,   233,   234,   235,   236,   240,   241,   244,
     245,   248,   249,   252,   255,   256,   281,   284,   285,   305,
     306,   307,   308,   309,   310,   311,   319,   320,   321,   322,
     323,   326,   327,   328,   329,   330,   331,   332,   333,   335,
     336,   337,   338,   339,   163,   184,   323,   118,   312,   313,
       3,   205,    14,    22,    35,    40,    41,    44,    55,    86,
      99,   168,   172,   231,   252,   305,   310,   321,   322,   323,
     326,   328,   329,   312,   323,   107,   284,    88,   205,   184,
     299,   323,   184,   170,     8,    86,   323,   324,     8,    11,
      86,   107,   324,    77,   120,   225,   323,   225,   323,   225,
     323,    26,   285,   323,    27,   113,   227,   323,   191,   170,
       3,    17,    18,    20,    25,    33,    39,    45,    49,    52,
      62,    68,    69,    76,    85,    94,    95,    97,    99,   101,
     104,   108,   111,   114,   207,   209,   207,   207,   253,   170,
     207,   286,   287,    32,   192,   212,   323,   323,    18,    76,
      94,   111,   323,   323,   323,     8,   170,   215,    22,    35,
      38,    86,   216,     4,   275,   298,   323,   105,   106,   163,
     323,   325,   323,   212,   323,   323,   323,    98,   170,   184,
     323,   323,   192,   204,   323,   326,   192,   204,   323,   207,
     282,   323,   227,   323,   323,   323,   323,   323,   323,   323,
       1,   169,   182,   193,   298,   109,   148,   275,   300,   301,
     325,   225,   298,   323,   334,   323,    79,   184,   168,    83,
     188,    46,   112,    55,   207,   207,    53,    72,    82,   270,
     286,   162,   163,   154,   323,    12,    19,    31,    87,    91,
     122,   138,   139,   141,   142,   143,   145,   146,   147,   149,
     150,   151,   152,   153,   155,   156,   157,   158,   159,   160,
     161,   164,   165,   166,   175,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     167,   262,   170,   172,    87,    91,   170,   184,   163,   323,
     323,   323,   207,   298,    55,   168,   192,    47,   312,   282,
     286,   163,   144,   163,   117,   208,   275,   302,   303,   304,
     325,    86,   221,   256,   284,    86,   219,   282,   256,   284,
     207,   170,   212,    32,    47,   212,   118,   212,   315,    32,
      47,   212,   315,   212,   315,    47,   212,   315,    36,    71,
     163,   207,   207,   100,   192,   100,   123,   192,   262,    81,
      84,   189,   302,   170,   170,   192,   184,   170,   264,   109,
     170,   207,   288,   289,     1,   143,   293,    47,   144,   184,
     212,   170,   170,   212,   302,   216,   144,   163,   323,   323,
     163,   168,   212,   170,   302,   163,   237,   163,   237,   163,
     123,   283,   163,   212,   212,   163,   169,   169,   182,   144,
     169,   323,   144,   171,   144,   171,   173,   315,    47,   144,
     173,   315,   121,   144,   173,     8,     1,   169,   193,    64,
     201,   202,   323,   195,   323,   207,   143,   246,   168,   257,
     163,   323,   323,   323,   323,   225,   323,   225,   323,   323,
     323,   323,   323,   323,   323,     3,    20,    33,    62,   101,
     107,   208,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,    67,
     325,   325,   325,   325,   325,   302,   302,   225,   323,   225,
     323,    27,    47,    88,   113,   314,   317,   318,   323,   339,
      32,    47,    32,    47,   100,   170,    47,   173,   207,   225,
     323,   212,   163,   163,   323,   323,   123,   171,   144,   222,
     207,   286,   220,   207,   163,   286,    47,   298,    44,   323,
     225,   323,   170,   212,    44,   323,   225,   323,   212,   212,
     225,   323,   212,   166,   194,   197,   323,   194,   196,   123,
     123,   184,    34,   184,   323,    34,   323,   188,   171,   303,
     207,   229,   230,    27,    47,    51,    74,    77,    88,   107,
     183,   265,   266,   267,   268,   254,   289,   144,   171,    33,
      49,    95,    99,   172,   211,   294,   306,   123,   290,   323,
     287,   323,   323,   171,   275,   323,     1,   242,   304,   171,
      21,   238,   294,   306,   144,   169,   171,   171,   300,   171,
     300,   184,   173,   225,   323,   173,   184,   323,   173,   323,
     173,   323,   169,   169,   207,   144,   163,    13,   146,   144,
     163,    13,    36,    71,   163,   170,   298,   168,     1,   207,
     250,   251,    27,    77,    88,   107,   259,   269,   170,   163,
     163,   163,   163,   163,   163,   171,   173,    47,    88,   144,
     171,    17,    20,    25,    45,    52,    62,    69,    85,    97,
     108,   114,   305,    87,    87,    44,   225,   323,    44,   225,
     323,   303,   225,   323,   170,   312,   312,   163,   275,   325,
     304,   207,   246,   163,   207,   246,   163,   323,   171,   323,
      32,   212,    32,   212,   316,   317,   323,    32,   212,   315,
      32,   212,   315,   212,   315,   212,   315,   144,   163,    13,
     163,   323,   323,    34,   184,    34,    34,   184,   100,   192,
      64,   171,   144,   171,    47,    88,   267,   144,   171,   170,
     207,    27,    77,    88,   107,   271,   171,   289,   293,     1,
     298,    67,   325,   171,   171,   169,    73,   115,   169,   243,
     171,   170,   192,   207,   239,   282,   184,   173,   315,   173,
     315,   184,   121,   201,   208,   168,   323,   109,   323,   197,
     196,   303,     1,   247,   169,   123,   144,   169,    88,   258,
       1,     3,    12,    17,    19,    20,    25,    39,    45,    52,
      54,    62,    68,    69,    85,    97,   101,   104,   108,   114,
     138,   139,   140,   141,   142,   143,   145,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   161,   164,
     165,   166,   167,   170,   206,   207,   209,   260,   261,   262,
     305,   171,   317,   293,   305,   305,   323,    32,    32,   323,
      32,    32,   171,   173,   173,   303,   212,   212,   246,   168,
     246,   168,   212,   100,    44,   323,    44,   323,   144,   171,
     100,    44,   323,   212,    44,   323,   212,   212,   212,   323,
     323,   184,   184,   323,   184,    34,   207,   163,   230,   192,
     207,   266,   289,   143,   297,    88,   293,   290,   173,    47,
     173,   170,   170,    32,   184,   298,   239,   143,   192,    44,
     184,   323,   173,    44,   184,   323,   173,   323,   194,    13,
      36,    71,    36,    71,   163,   163,   171,   169,    81,    84,
     169,   183,   214,   323,   251,   271,   170,   263,   323,   138,
     146,   263,   263,   290,   100,    44,    44,   100,    44,    44,
      44,    44,   171,   168,   247,   168,   247,   323,   323,   323,
     317,   323,   323,   323,    13,    34,   184,   163,   297,   171,
     172,   211,   275,   296,   306,   148,   276,   290,    60,   116,
     277,   323,   294,   306,   302,   302,   184,   212,   171,   323,
     323,   184,   323,   184,   169,   109,   323,   197,   196,   197,
     196,   163,   214,     1,   143,   292,   265,   171,     3,   101,
     261,   262,   323,   323,   323,   323,   323,   323,   247,   169,
     247,   169,   100,   100,   100,   100,   323,   184,   276,   290,
     297,   173,   298,   275,   323,     3,    90,   101,   278,   279,
     280,   323,   192,   213,   274,   173,   171,   171,   192,   100,
     100,   163,   163,   163,   163,   172,   211,   291,   306,   103,
     272,   171,   263,   263,   100,   100,   100,   100,   100,   100,
     169,   169,   323,   323,   323,   323,   276,   290,   275,   295,
     296,   306,    47,   173,   280,   116,   144,   123,   149,   151,
     152,   155,   156,    60,   306,   162,   162,   323,   323,     1,
     173,   298,   277,   323,   295,   296,   323,   279,   280,   280,
     280,   280,   280,   280,   278,   173,   291,   306,   173,   163,
     273,   274,   173,   291,   306,   295
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   179,   180,   181,   181,   182,   182,   183,   183,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   185,   186,   186,   186,   187,   187,   187,
     188,   188,   189,   189,   189,   191,   190,   192,   192,   192,
     193,   193,   194,   194,   194,   194,   195,   195,   195,   195,
     195,   195,   196,   196,   197,   197,   198,   198,   198,   199,
     199,   199,   199,   199,   199,   199,   200,   201,   201,   201,
     202,   202,   203,   204,   204,   204,   204,   204,   204,   205,
     205,   206,   206,   206,   206,   206,   206,   207,   207,   207,
     207,   207,   207,   208,   208,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   211,   211,   211,   211,
     212,   212,   213,   213,   214,   214,   214,   215,   214,   216,
     216,   216,   216,   216,   216,   216,   217,   217,   217,   217,
     219,   218,   220,   218,   221,   218,   222,   218,   218,   218,
     218,   218,   223,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   225,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   227,   227,   228,   228,   229,   229,   230,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   232,   232,   232,   232,   232,   232,   233,   233,   233,
     234,   234,   234,   235,   236,   236,   236,   236,   236,   236,
     237,   237,   238,   238,   238,   239,   239,   240,   241,   241,
     242,   242,   243,   243,   243,   244,   244,   245,   245,   245,
     246,   246,   247,   247,   247,   248,   248,   249,   250,   250,
     250,   251,   251,   253,   254,   252,   255,   255,   255,   257,
     258,   256,   259,   259,   259,   259,   259,   260,   260,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   263,
     263,   264,   265,   265,   265,   266,   266,   266,   266,   266,
     266,   267,   267,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   269,   269,   269,   269,   269,   269,   270,   270,
     270,   271,   271,   271,   271,   271,   271,   272,   272,   273,
     273,   274,   274,   275,   276,   276,   276,   277,   277,   277,
     277,   277,   278,   278,   279,   279,   279,   279,   279,   279,
     279,   280,   280,   281,   281,   281,   282,   282,   283,   283,
     283,   284,   284,   284,   284,   284,   285,   285,   286,   286,
     287,   287,   288,   288,   288,   289,   289,   289,   290,   290,
     290,   291,   291,   291,   291,   291,   291,   291,   292,   292,
     292,   292,   292,   293,   293,   293,   293,   293,   294,   294,
     294,   294,   295,   295,   295,   296,   296,   296,   296,   296,
     297,   297,   297,   297,   297,   298,   298,   298,   298,   299,
     299,   300,   300,   300,   301,   301,   302,   302,   303,   303,
     304,   304,   304,   304,   305,   305,   306,   306,   306,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   309,   310,
     311,   311,   311,   311,   311,   311,   311,   311,   312,   312,
     313,   314,   314,   315,   316,   316,   317,   317,   317,   318,
     318,   318,   318,   318,   318,   319,   319,   320,   320,   320,
     320,   320,   321,   321,   321,   321,   321,   322,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   324,   324,   325,   325,   325,
     326,   326,   326,   326,   327,   327,   327,   327,   327,   328,
     328,   328,   329,   329,   329,   329,   329,   330,   330,   330,
     330,   331,   331,   332,   332,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   334,   334,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   336,   336,   336,   336,   336,   336,   336,
     337,   337,   337,   337,   338,   338,   338,   338,   339,   339,
     339,   339,   339,   339,   339
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     3,     2,
       2,     3,     2,     4,     3,     4,     4,     0,     1,     1,
       0,     1,     0,     1,     1,     0,     7,     2,     3,     3,
       1,     2,     1,     3,     3,     5,     1,     3,     3,     3,
       5,     5,     0,     1,     1,     1,     0,     1,     1,     4,
       6,     8,     8,     6,     8,     8,     4,     1,     3,     5,
       1,     3,     3,     4,     4,     4,     4,     4,     4,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     3,     1,     1,     2,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     5,     5,     2,
       0,     8,     0,     9,     0,     8,     0,     9,     3,     3,
       5,     5,     2,     5,     3,     3,     6,     6,     4,     5,
       5,     3,     3,     6,     5,     6,     5,     6,     3,     4,
       3,     4,     5,     6,     5,     6,     3,     4,     3,     4,
       6,     7,     6,     7,     4,     5,     4,     5,     4,     4,
       3,     6,     5,     4,     3,     6,     5,     6,     5,     8,
       7,     4,     4,     6,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     4,     7,
       5,     3,     6,     2,     3,     3,     2,     2,     3,     3,
       0,     2,     2,     3,     5,     1,     3,     3,     5,     5,
       0,     2,     3,     2,     3,     6,     6,     1,     1,     1,
       0,     2,     0,     2,     3,     5,     5,     1,     1,     2,
       3,     1,     3,     0,     0,     8,     0,     1,     1,     0,
       0,    10,     3,     3,     5,     5,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     3,     0,     1,     3,     4,     5,     4,     5,     6,
       6,     0,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     0,     1,     2,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     0,     2,     2,
       4,     4,     1,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     3,     4,     4,     2,     4,     0,     2,
       2,     1,     1,     1,     2,     1,     4,     3,     1,     3,
       3,     5,     1,     1,     3,     1,     2,     3,     0,     2,
       2,     3,     2,     4,     3,     3,     4,     3,     0,     2,
       2,     2,     1,     0,     2,     2,     2,     1,     4,     4,
       6,     3,     0,     1,     1,     3,     4,     3,     4,     6,
       0,     2,     2,     2,     2,     1,     1,     3,     3,     1,
       3,     1,     1,     1,     3,     3,     0,     1,     1,     3,
       3,     3,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     2,     4,     4,     4,     5,     2,
       2,     1,     2,     1,     2,     1,     2,     1,     2,     1,
       1,     6,     6,     4,     9,     9,     7,     6,     6,     4,
       9,     9,     7,     4,     6,     6,     9,     9,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       4,     1,     3,     4,     1,     3,     4,     3,     3,     1,
       1,     2,     1,     2,     1,     1,     3,     1,     2,     2,
       2,     2,     2,     8,     8,     9,     9,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     3,
       3,     2,     2,     2,     1,     0,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     4,
       4,     4,     3,     3,     3,     3,     5,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     4,     3,     4,     3,     4,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1
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
#line 511 "chapel.ypp" /* yacc.c:1663  */
    { yyblock = (yyval.pblockstmt); }
#line 6534 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 3:
#line 516 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6540 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 4:
#line 517 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6546 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 6:
#line 524 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6552 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 7:
#line 529 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6558 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 8:
#line 530 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6564 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 27:
#line 553 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6570 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 28:
#line 554 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6576 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 29:
#line 555 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6582 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 30:
#line 556 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6588 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 31:
#line 557 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6594 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 32:
#line 558 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6600 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 33:
#line 559 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6606 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 34:
#line 560 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6612 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 35:
#line 561 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6618 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 36:
#line 562 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6624 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 37:
#line 563 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6630 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 38:
#line 564 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6636 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 39:
#line 565 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6642 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 40:
#line 566 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6648 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 41:
#line 567 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6654 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 42:
#line 568 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6660 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 43:
#line 573 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 6674 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 44:
#line 586 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6683 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 45:
#line 591 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6692 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 46:
#line 596 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6701 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 47:
#line 603 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6707 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 48:
#line 604 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6713 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 49:
#line 605 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6719 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 50:
#line 609 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6725 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 51:
#line 610 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 6731 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 52:
#line 614 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6737 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 53:
#line 615 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6743 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 54:
#line 616 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6749 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 55:
#line 621 "chapel.ypp" /* yacc.c:1663  */
    {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6758 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 56:
#line 626 "chapel.ypp" /* yacc.c:1663  */
    {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6766 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 57:
#line 641 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6772 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 58:
#line 642 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6778 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 59:
#line 643 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6784 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 60:
#line 648 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6790 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 61:
#line 649 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6796 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 62:
#line 654 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6806 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 63:
#line 659 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6816 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 64:
#line 664 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6825 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 65:
#line 668 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6834 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 66:
#line 677 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6844 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 67:
#line 682 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6854 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 68:
#line 687 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6864 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 69:
#line 692 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6873 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 70:
#line 696 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6882 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 71:
#line 700 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6891 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 72:
#line 708 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6901 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 74:
#line 717 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6911 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 76:
#line 726 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6917 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 77:
#line 727 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6923 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 78:
#line 728 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6929 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 79:
#line 732 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 6935 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 80:
#line 733 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 6941 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 81:
#line 734 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6947 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 82:
#line 735 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6953 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 83:
#line 736 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 6959 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 84:
#line 737 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6965 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 85:
#line 738 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6971 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 86:
#line 742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 6978 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 87:
#line 747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 6984 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 88:
#line 748 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6990 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 89:
#line 749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 6996 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 90:
#line 753 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 7002 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 91:
#line 754 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 7008 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 92:
#line 758 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 7014 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 93:
#line 763 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 7020 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 94:
#line 765 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 7026 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 95:
#line 767 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 7032 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 96:
#line 769 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 7038 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 97:
#line 771 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 7044 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 98:
#line 773 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 7050 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 99:
#line 779 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 7056 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 100:
#line 780 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7062 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 101:
#line 784 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7068 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 102:
#line 785 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7074 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 103:
#line 786 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 7080 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 104:
#line 787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7086 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 105:
#line 788 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7092 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 106:
#line 789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7098 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 107:
#line 792 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7104 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 108:
#line 793 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7110 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 109:
#line 794 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7116 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 110:
#line 795 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7122 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 111:
#line 796 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7128 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 112:
#line 797 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7134 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 113:
#line 809 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7140 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 114:
#line 810 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 7146 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 115:
#line 821 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bool"; }
#line 7152 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 116:
#line 822 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "int"; }
#line 7158 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 117:
#line 823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "uint"; }
#line 7164 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 118:
#line 824 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "real"; }
#line 7170 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 119:
#line 825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "imag"; }
#line 7176 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 120:
#line 826 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "complex"; }
#line 7182 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 121:
#line 827 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bytes"; }
#line 7188 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 122:
#line 828 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "string"; }
#line 7194 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 123:
#line 829 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "sync"; }
#line 7200 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 124:
#line 830 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "single"; }
#line 7206 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 125:
#line 831 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "owned"; }
#line 7212 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 126:
#line 832 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "shared"; }
#line 7218 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 127:
#line 833 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "borrowed"; }
#line 7224 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 128:
#line 834 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "unmanaged"; }
#line 7230 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 129:
#line 835 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "domain"; }
#line 7236 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 130:
#line 836 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "index"; }
#line 7242 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 131:
#line 837 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "locale"; }
#line 7248 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 132:
#line 838 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "nothing"; }
#line 7254 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 133:
#line 839 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "void"; }
#line 7260 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 134:
#line 843 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7266 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 135:
#line 844 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7272 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 136:
#line 845 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7278 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 137:
#line 846 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7284 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 138:
#line 847 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7290 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 139:
#line 848 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7296 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 140:
#line 849 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7302 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 141:
#line 850 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7308 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 142:
#line 851 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7314 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 143:
#line 852 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7320 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 144:
#line 853 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7326 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 145:
#line 854 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7332 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 146:
#line 861 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_syncvar"; }
#line 7338 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 147:
#line 862 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_singlevar"; }
#line 7344 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 148:
#line 863 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_domain"; }
#line 7350 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 149:
#line 864 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_index"; }
#line 7356 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 150:
#line 868 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7362 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 151:
#line 869 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7368 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 152:
#line 873 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7374 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 153:
#line 874 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7380 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 154:
#line 878 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7386 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 156:
#line 880 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7392 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 157:
#line 890 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7398 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 158:
#line 891 "chapel.ypp" /* yacc.c:1663  */
    { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7404 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 166:
#line 905 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7410 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 167:
#line 906 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7416 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 168:
#line 907 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7422 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 169:
#line 908 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7428 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 170:
#line 913 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7437 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 171:
#line 918 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7451 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 172:
#line 928 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7460 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 173:
#line 933 "chapel.ypp" /* yacc.c:1663  */
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7475 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 174:
#line 944 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7484 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 175:
#line 949 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7498 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 176:
#line 959 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7507 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 177:
#line 964 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7521 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 178:
#line 975 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7529 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 179:
#line 979 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7537 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 180:
#line 984 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7549 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 181:
#line 992 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7561 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 182:
#line 1003 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7569 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 183:
#line 1009 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7575 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 184:
#line 1010 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7581 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 185:
#line 1011 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7587 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 186:
#line 1012 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7593 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 187:
#line 1013 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7599 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 188:
#line 1014 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7605 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 189:
#line 1015 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7611 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 190:
#line 1016 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7617 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 191:
#line 1017 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7623 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 192:
#line 1018 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7629 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 193:
#line 1019 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7635 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 194:
#line 1020 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7641 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 195:
#line 1021 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7647 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 196:
#line 1022 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7653 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 197:
#line 1023 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7659 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 198:
#line 1024 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7665 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 199:
#line 1025 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7671 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 200:
#line 1026 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7677 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 201:
#line 1027 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7683 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 202:
#line 1028 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7689 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 203:
#line 1029 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7695 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 204:
#line 1030 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7701 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 205:
#line 1031 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7707 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 206:
#line 1032 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7713 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 207:
#line 1033 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7719 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 208:
#line 1034 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7725 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 209:
#line 1035 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7731 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 210:
#line 1037 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7741 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 211:
#line 1043 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7751 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 212:
#line 1049 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7761 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 213:
#line 1055 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7771 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 214:
#line 1061 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7782 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 215:
#line 1068 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7793 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 216:
#line 1075 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7801 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 217:
#line 1079 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7809 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 218:
#line 1085 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7815 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 219:
#line 1089 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7821 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 220:
#line 1090 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7827 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 221:
#line 1091 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7833 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 222:
#line 1092 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7839 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 223:
#line 1094 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7845 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 224:
#line 1095 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7851 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 225:
#line 1096 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7857 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 226:
#line 1097 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7863 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 227:
#line 1099 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7870 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 228:
#line 1101 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7877 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 229:
#line 1103 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7884 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 230:
#line 1105 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7891 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 231:
#line 1110 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), false); }
#line 7897 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 232:
#line 1111 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), true);  }
#line 7903 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 233:
#line 1116 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), (yyvsp[0].pblockstmt))); }
#line 7909 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 234:
#line 1118 "chapel.ypp" /* yacc.c:1663  */
    { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-1].pch), ls, (yyvsp[0].pblockstmt))); }
#line 7918 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 235:
#line 1125 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7924 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 236:
#line 1126 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7930 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 237:
#line 1131 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[0].pch), INTENT_TYPE); }
#line 7936 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 238:
#line 1136 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7942 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 239:
#line 1137 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bool"; }
#line 7948 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 240:
#line 1138 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "int"; }
#line 7954 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 241:
#line 1139 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "uint"; }
#line 7960 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 242:
#line 1140 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "real"; }
#line 7966 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 243:
#line 1141 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "imag"; }
#line 7972 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 244:
#line 1142 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "complex"; }
#line 7978 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 245:
#line 1143 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bytes"; }
#line 7984 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 246:
#line 1144 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "string"; }
#line 7990 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 247:
#line 1145 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "locale"; }
#line 7996 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 248:
#line 1146 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "nothing"; }
#line 8002 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 249:
#line 1147 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "void"; }
#line 8008 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 250:
#line 1149 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[0].pch)); }
#line 8015 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 251:
#line 1155 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; }
#line 8021 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 252:
#line 1156 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 8027 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 253:
#line 1157 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; }
#line 8033 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 254:
#line 1158 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; }
#line 8039 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 255:
#line 1167 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "domain"; }
#line 8045 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 256:
#line 1168 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "index"; }
#line 8051 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 257:
#line 1173 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8057 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 258:
#line 1175 "chapel.ypp" /* yacc.c:1663  */
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-3].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-1].pch), act, NULL)); }
#line 8064 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 259:
#line 1178 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-6].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8071 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 260:
#line 1184 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8077 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 261:
#line 1186 "chapel.ypp" /* yacc.c:1663  */
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-2].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[0].pch), act); }
#line 8084 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 262:
#line 1189 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-5].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8091 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 263:
#line 1194 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 8097 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 264:
#line 1197 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 8103 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 265:
#line 1198 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 8109 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 266:
#line 1199 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 8115 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 267:
#line 1200 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 8121 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 268:
#line 1201 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8127 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 269:
#line 1202 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 270:
#line 1206 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8139 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 271:
#line 1207 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8145 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 272:
#line 1211 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 8151 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 273:
#line 1212 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8157 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 274:
#line 1213 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8163 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 275:
#line 1217 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 8169 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 276:
#line 1218 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 8175 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 277:
#line 1222 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 8181 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 278:
#line 1226 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 8187 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 279:
#line 1228 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 8193 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 280:
#line 1232 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8199 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 281:
#line 1233 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8205 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 282:
#line 1238 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 8211 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 283:
#line 1240 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8217 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 284:
#line 1242 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8223 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 285:
#line 1249 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8237 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 286:
#line 1259 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8251 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 287:
#line 1272 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8261 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 288:
#line 1277 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8271 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 289:
#line 1282 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8281 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 290:
#line 1290 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 8287 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 291:
#line 1291 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 8293 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 292:
#line 1296 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 8299 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 293:
#line 1298 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 8305 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 294:
#line 1300 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 8311 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 295:
#line 1305 "chapel.ypp" /* yacc.c:1663  */
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
#line 8331 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 296:
#line 1321 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 8339 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 297:
#line 1328 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8349 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 298:
#line 1337 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8361 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 299:
#line 1345 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8369 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 300:
#line 1349 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8377 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 301:
#line 1355 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8383 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 302:
#line 1356 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8389 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 303:
#line 1361 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8398 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 304:
#line 1366 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8407 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 305:
#line 1371 "chapel.ypp" /* yacc.c:1663  */
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
#line 8429 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 306:
#line 1393 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8437 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 307:
#line 1396 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8446 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 308:
#line 1400 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8455 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 309:
#line 1408 "chapel.ypp" /* yacc.c:1663  */
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8469 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 310:
#line 1418 "chapel.ypp" /* yacc.c:1663  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8480 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 311:
#line 1425 "chapel.ypp" /* yacc.c:1663  */
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

      if ((yyvsp[-8].procIterOp) == ProcIterOp_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }
      if ((yyvsp[-8].procIterOp) == ProcIterOp_OP) {
        fn->addFlag(FLAG_OPERATOR);
        if (fn->_this != NULL) {
          updateOpThisTagOrErr(fn);
          setupTypeIntentArg(toArgSymbol(fn->_this));
        }
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[-6].pfnsymbol), (yyvsp[-4].retTag), (yyvsp[-3].pexpr), (yyvsp[-2].b), (yyvsp[-1].lifetimeAndWhere).where, (yyvsp[-1].lifetimeAndWhere).lifetime, (yyvsp[0].pblockstmt), (yylsp[-8]).comment);
      context->latestComment = NULL;
    }
#line 8514 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 312:
#line 1458 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8522 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 313:
#line 1462 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8531 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 314:
#line 1467 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8539 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 315:
#line 1471 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8548 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 316:
#line 1476 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8556 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 318:
#line 1483 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8562 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 319:
#line 1487 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 8568 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 320:
#line 1488 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&"; }
#line 8574 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 321:
#line 1489 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|"; }
#line 8580 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 322:
#line 1490 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^"; }
#line 8586 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 323:
#line 1491 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "~"; }
#line 8592 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 324:
#line 1492 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "=="; }
#line 8598 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 325:
#line 1493 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!="; }
#line 8604 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 326:
#line 1494 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<="; }
#line 8610 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 327:
#line 1495 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">="; }
#line 8616 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 328:
#line 1496 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<"; }
#line 8622 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 329:
#line 1497 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">"; }
#line 8628 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 330:
#line 1498 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+"; }
#line 8634 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 331:
#line 1499 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-"; }
#line 8640 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 332:
#line 1500 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*"; }
#line 8646 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 333:
#line 1501 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/"; }
#line 8652 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 334:
#line 1502 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<"; }
#line 8658 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 335:
#line 1503 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>"; }
#line 8664 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 336:
#line 1504 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%"; }
#line 8670 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 337:
#line 1505 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**"; }
#line 8676 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 338:
#line 1506 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!"; }
#line 8682 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 339:
#line 1507 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_by"; }
#line 8688 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 340:
#line 1508 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "#"; }
#line 8694 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 341:
#line 1509 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_align"; }
#line 8700 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 342:
#line 1510 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<=>"; }
#line 8706 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 343:
#line 1511 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<~>"; }
#line 8712 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 344:
#line 1512 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "init="; }
#line 8718 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 345:
#line 1513 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ":"; }
#line 8724 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 346:
#line 1514 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8730 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 347:
#line 1518 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "="; }
#line 8736 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 348:
#line 1519 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+="; }
#line 8742 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 349:
#line 1520 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-="; }
#line 8748 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 350:
#line 1521 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*="; }
#line 8754 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 351:
#line 1522 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/="; }
#line 8760 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 352:
#line 1523 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%="; }
#line 8766 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 353:
#line 1524 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**="; }
#line 8772 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 354:
#line 1525 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&="; }
#line 8778 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 355:
#line 1526 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|="; }
#line 8784 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 356:
#line 1527 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^="; }
#line 8790 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 357:
#line 1528 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>="; }
#line 8796 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 358:
#line 1529 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<="; }
#line 8802 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 359:
#line 1533 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 8808 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 360:
#line 1534 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8814 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 361:
#line 1538 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8820 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 362:
#line 1542 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 8826 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 363:
#line 1543 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 8832 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 364:
#line 1544 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 8838 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 365:
#line 1549 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 8844 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 366:
#line 1551 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 8850 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 367:
#line 1553 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 8856 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 368:
#line 1555 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 8862 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 369:
#line 1557 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8868 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 370:
#line 1559 "chapel.ypp" /* yacc.c:1663  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 8874 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 371:
#line 1563 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 8880 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 372:
#line 1564 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 8886 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 373:
#line 1568 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_IN; }
#line 8892 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 374:
#line 1569 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_INOUT; }
#line 8898 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 375:
#line 1570 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_OUT; }
#line 8904 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 376:
#line 1571 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST; }
#line 8910 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 377:
#line 1572 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 8916 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 378:
#line 1573 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 8922 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 379:
#line 1574 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 8928 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 380:
#line 1575 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF; }
#line 8934 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 381:
#line 1576 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE; }
#line 8940 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 382:
#line 1580 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 8946 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 383:
#line 1581 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 8952 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 384:
#line 1582 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF;   }
#line 8958 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 385:
#line 1583 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 8964 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 386:
#line 1584 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST;   }
#line 8970 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 387:
#line 1585 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 8976 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 388:
#line 1589 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIterOp) = ProcIterOp_PROC; }
#line 8982 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 389:
#line 1590 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIterOp) = ProcIterOp_ITER; }
#line 8988 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 390:
#line 1591 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIterOp) = ProcIterOp_OP; }
#line 8994 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 391:
#line 1595 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 9000 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 392:
#line 1596 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 9006 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 393:
#line 1597 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 9012 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 394:
#line 1598 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_REF; }
#line 9018 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 395:
#line 1599 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_PARAM; }
#line 9024 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 396:
#line 1600 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_TYPE; }
#line 9030 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 397:
#line 1604 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 9036 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 398:
#line 1605 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 9042 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 399:
#line 1608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = NULL; }
#line 9048 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 402:
#line 1614 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 9054 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 403:
#line 1618 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 9060 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 404:
#line 1622 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9066 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 405:
#line 1623 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9072 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 406:
#line 1624 "chapel.ypp" /* yacc.c:1663  */
    { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 9082 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 407:
#line 1632 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 9088 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 408:
#line 1634 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 9094 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 409:
#line 1636 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 9100 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 410:
#line 1638 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9106 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 411:
#line 1640 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9112 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 412:
#line 1645 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9118 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 413:
#line 1647 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9124 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 414:
#line 1651 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9130 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 415:
#line 1652 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9136 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 416:
#line 1653 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9142 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 417:
#line 1654 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9148 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 418:
#line 1655 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9154 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 419:
#line 1656 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9160 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 420:
#line 1657 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 9166 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 421:
#line 1662 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 9172 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 422:
#line 1664 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 9178 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 423:
#line 1669 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 9184 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 424:
#line 1671 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 9190 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 425:
#line 1673 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 9196 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 426:
#line 1678 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 9213 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 427:
#line 1691 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 9231 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 428:
#line 1707 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9237 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 429:
#line 1709 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9243 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 430:
#line 1711 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 9249 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 431:
#line 1715 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 9255 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 432:
#line 1716 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 9261 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 433:
#line 1717 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 9267 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 434:
#line 1718 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 9273 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 435:
#line 1719 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 9279 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 436:
#line 1724 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9289 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 437:
#line 1730 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9298 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 439:
#line 1739 "chapel.ypp" /* yacc.c:1663  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 9307 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 440:
#line 1747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 9313 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 441:
#line 1749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9319 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 442:
#line 1754 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 9325 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 443:
#line 1756 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 9331 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 444:
#line 1758 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 9337 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 445:
#line 1763 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 9343 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 446:
#line 1765 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 9349 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 447:
#line 1767 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 9355 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 448:
#line 1773 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9361 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 449:
#line 1774 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9367 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 450:
#line 1775 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9373 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 451:
#line 1781 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9379 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 452:
#line 1783 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9385 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 453:
#line 1785 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9393 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 454:
#line 1789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9401 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 455:
#line 1793 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9407 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 456:
#line 1795 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9415 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 457:
#line 1799 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9423 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 458:
#line 1806 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9429 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 459:
#line 1807 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9435 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 460:
#line 1808 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9441 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 461:
#line 1809 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9447 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 462:
#line 1810 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9453 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 463:
#line 1815 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9459 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 464:
#line 1816 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9465 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 465:
#line 1817 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9471 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 466:
#line 1818 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9477 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 467:
#line 1819 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9483 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 468:
#line 1840 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9491 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 469:
#line 1844 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9499 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 470:
#line 1848 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9511 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 471:
#line 1856 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9519 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 472:
#line 1862 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9525 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 473:
#line 1863 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9531 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 474:
#line 1864 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9537 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 475:
#line 1869 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9543 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 476:
#line 1871 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9549 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 477:
#line 1877 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9555 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 478:
#line 1879 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9561 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 479:
#line 1881 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9567 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 480:
#line 1885 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9573 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 481:
#line 1886 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9579 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 482:
#line 1887 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9585 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 483:
#line 1888 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9591 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 484:
#line 1889 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9597 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 485:
#line 1895 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9603 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 486:
#line 1896 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9609 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 487:
#line 1897 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9615 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 488:
#line 1898 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9621 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 489:
#line 1902 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9627 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 490:
#line 1903 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9633 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 491:
#line 1907 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9639 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 492:
#line 1908 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9645 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 493:
#line 1909 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9651 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 494:
#line 1913 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9657 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 495:
#line 1914 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9663 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 496:
#line 1918 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9669 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 498:
#line 1923 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9675 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 499:
#line 1924 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9681 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 500:
#line 1928 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9687 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 501:
#line 1929 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9693 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 502:
#line 1930 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9699 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 503:
#line 1931 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9705 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 504:
#line 1935 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9711 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 505:
#line 1936 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9717 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 506:
#line 1948 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9723 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 507:
#line 1950 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9729 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 508:
#line 1952 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9735 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 514:
#line 1963 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9741 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 515:
#line 1965 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9747 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 516:
#line 1967 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9753 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 517:
#line 1969 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9759 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 518:
#line 1971 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pexpr)); }
#line 9765 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 519:
#line 1973 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9771 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 520:
#line 1975 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 9777 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 521:
#line 1978 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 9783 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 522:
#line 1980 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 9789 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 523:
#line 1982 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 9795 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 524:
#line 1984 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9801 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 525:
#line 1986 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 9807 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 526:
#line 1988 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 9813 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 527:
#line 1990 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 9819 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 528:
#line 1992 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9825 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 529:
#line 1995 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 9831 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 530:
#line 1997 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 9837 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 531:
#line 2002 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9843 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 532:
#line 2004 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9849 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 533:
#line 2006 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9855 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 534:
#line 2008 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9861 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 535:
#line 2010 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9867 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 536:
#line 2012 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9873 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 537:
#line 2014 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9879 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 538:
#line 2016 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9885 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 539:
#line 2018 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9891 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 540:
#line 2020 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9897 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 541:
#line 2022 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9903 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 542:
#line 2024 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9909 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 543:
#line 2026 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 9920 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 544:
#line 2033 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 9930 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 545:
#line 2039 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 9940 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 546:
#line 2045 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9950 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 547:
#line 2051 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 9960 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 548:
#line 2060 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9966 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 549:
#line 2069 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 9972 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 557:
#line 2085 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9978 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 558:
#line 2089 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 9984 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 560:
#line 2094 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9990 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 561:
#line 2098 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9996 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 562:
#line 2099 "chapel.ypp" /* yacc.c:1663  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10002 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 563:
#line 2103 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10008 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 564:
#line 2107 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10014 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 565:
#line 2108 "chapel.ypp" /* yacc.c:1663  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10020 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 566:
#line 2113 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 10028 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 567:
#line 2117 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10036 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 568:
#line 2121 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10044 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 569:
#line 2127 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 10050 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 570:
#line 2128 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 10056 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 571:
#line 2129 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 10062 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 572:
#line 2130 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 10068 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 573:
#line 2131 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 10074 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 574:
#line 2132 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 10080 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 576:
#line 2138 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10086 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 577:
#line 2143 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 10092 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 578:
#line 2145 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 10100 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 579:
#line 2149 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 10108 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 580:
#line 2153 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 10116 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 581:
#line 2157 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 10124 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 582:
#line 2165 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 10131 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 583:
#line 2170 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10141 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 584:
#line 2176 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10151 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 585:
#line 2182 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10162 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 586:
#line 2189 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10173 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 587:
#line 2199 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 10179 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 597:
#line 2216 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 10185 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 598:
#line 2218 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10191 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 599:
#line 2220 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10197 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 600:
#line 2222 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 10203 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 601:
#line 2237 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 10209 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 602:
#line 2239 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 10215 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 603:
#line 2241 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 10221 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 604:
#line 2243 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 10227 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 605:
#line 2247 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 10233 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 606:
#line 2248 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10239 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 607:
#line 2252 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 10245 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 608:
#line 2253 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 10251 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 609:
#line 2254 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10257 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 614:
#line 2270 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10263 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 615:
#line 2271 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10269 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 616:
#line 2272 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 10275 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 617:
#line 2273 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10281 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 619:
#line 2278 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10287 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 620:
#line 2279 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10293 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 621:
#line 2280 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 10299 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 622:
#line 2284 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 10305 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 623:
#line 2285 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 10311 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 624:
#line 2286 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 10317 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 625:
#line 2287 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 10323 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 626:
#line 2288 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 10329 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 627:
#line 2296 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 10335 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 628:
#line 2297 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 10341 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 629:
#line 2298 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 10347 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 630:
#line 2299 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 10353 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 631:
#line 2303 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10359 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 632:
#line 2304 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10365 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 633:
#line 2308 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10371 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 634:
#line 2309 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10377 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 637:
#line 2315 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10383 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 638:
#line 2316 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10389 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 639:
#line 2317 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10395 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 640:
#line 2318 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10401 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 641:
#line 2319 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 10407 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 642:
#line 2320 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10414 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 643:
#line 2322 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10421 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 644:
#line 2324 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10427 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 645:
#line 2325 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10433 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 646:
#line 2327 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10441 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 647:
#line 2331 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10449 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 648:
#line 2338 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10455 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 649:
#line 2339 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10461 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 650:
#line 2343 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10467 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 651:
#line 2344 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10473 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 652:
#line 2345 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10479 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 653:
#line 2346 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10485 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 654:
#line 2347 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10491 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 655:
#line 2348 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10497 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 656:
#line 2349 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10503 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 657:
#line 2350 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10509 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 658:
#line 2351 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10515 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 659:
#line 2352 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10521 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 660:
#line 2353 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10527 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 661:
#line 2354 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10533 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 662:
#line 2355 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10539 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 663:
#line 2356 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10545 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 664:
#line 2357 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10551 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 665:
#line 2358 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10557 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 666:
#line 2359 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10563 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 667:
#line 2360 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10569 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 668:
#line 2361 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10575 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 669:
#line 2362 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10581 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 670:
#line 2363 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10587 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 671:
#line 2364 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10593 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 672:
#line 2365 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10600 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 673:
#line 2370 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10606 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 674:
#line 2371 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10612 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 675:
#line 2372 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10618 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 676:
#line 2373 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10624 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 677:
#line 2374 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10630 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 678:
#line 2375 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10636 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 679:
#line 2376 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10642 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 680:
#line 2380 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10648 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 681:
#line 2381 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10654 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 682:
#line 2382 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10660 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 683:
#line 2383 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10666 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 684:
#line 2387 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10672 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 685:
#line 2388 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10678 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 686:
#line 2389 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10684 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 687:
#line 2390 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10690 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 688:
#line 2395 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10696 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 689:
#line 2396 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10702 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 690:
#line 2397 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10708 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 691:
#line 2398 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10714 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 692:
#line 2399 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10720 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 693:
#line 2400 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10726 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 694:
#line 2401 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10732 "bison-chapel.cpp" /* yacc.c:1663  */
    break;


#line 10736 "bison-chapel.cpp" /* yacc.c:1663  */
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
