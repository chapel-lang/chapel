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
    TDEPRECATED = 286,
    TDMAPPED = 287,
    TDO = 288,
    TDOMAIN = 289,
    TELSE = 290,
    TENUM = 291,
    TEXCEPT = 292,
    TEXPORT = 293,
    TEXTERN = 294,
    TFALSE = 295,
    TFOR = 296,
    TFORALL = 297,
    TFOREACH = 298,
    TFORWARDING = 299,
    TIF = 300,
    TIMAG = 301,
    TIMPORT = 302,
    TIN = 303,
    TINCLUDE = 304,
    TINDEX = 305,
    TINLINE = 306,
    TINOUT = 307,
    TINT = 308,
    TITER = 309,
    TINITEQUALS = 310,
    TIMPLEMENTS = 311,
    TINTERFACE = 312,
    TLABEL = 313,
    TLAMBDA = 314,
    TLET = 315,
    TLIFETIME = 316,
    TLOCAL = 317,
    TLOCALE = 318,
    TMINUSMINUS = 319,
    TMODULE = 320,
    TNEW = 321,
    TNIL = 322,
    TNOINIT = 323,
    TNONE = 324,
    TNOTHING = 325,
    TON = 326,
    TONLY = 327,
    TOPERATOR = 328,
    TOTHERWISE = 329,
    TOUT = 330,
    TOVERRIDE = 331,
    TOWNED = 332,
    TPARAM = 333,
    TPLUSPLUS = 334,
    TPRAGMA = 335,
    TPRIMITIVE = 336,
    TPRIVATE = 337,
    TPROC = 338,
    TPROTOTYPE = 339,
    TPUBLIC = 340,
    TREAL = 341,
    TRECORD = 342,
    TREDUCE = 343,
    TREF = 344,
    TREQUIRE = 345,
    TRETURN = 346,
    TSCAN = 347,
    TSELECT = 348,
    TSERIAL = 349,
    TSHARED = 350,
    TSINGLE = 351,
    TSPARSE = 352,
    TSTRING = 353,
    TSUBDOMAIN = 354,
    TSYNC = 355,
    TTHEN = 356,
    TTHIS = 357,
    TTHROW = 358,
    TTHROWS = 359,
    TTRUE = 360,
    TTRY = 361,
    TTRYBANG = 362,
    TTYPE = 363,
    TUINT = 364,
    TUNDERSCORE = 365,
    TUNION = 366,
    TUNMANAGED = 367,
    TUSE = 368,
    TVAR = 369,
    TVOID = 370,
    TWHEN = 371,
    TWHERE = 372,
    TWHILE = 373,
    TWITH = 374,
    TYIELD = 375,
    TZIP = 376,
    TALIAS = 377,
    TAND = 378,
    TASSIGN = 379,
    TASSIGNBAND = 380,
    TASSIGNBOR = 381,
    TASSIGNBXOR = 382,
    TASSIGNDIVIDE = 383,
    TASSIGNEXP = 384,
    TASSIGNLAND = 385,
    TASSIGNLOR = 386,
    TASSIGNMINUS = 387,
    TASSIGNMOD = 388,
    TASSIGNMULTIPLY = 389,
    TASSIGNPLUS = 390,
    TASSIGNREDUCE = 391,
    TASSIGNSL = 392,
    TASSIGNSR = 393,
    TBANG = 394,
    TBAND = 395,
    TBNOT = 396,
    TBOR = 397,
    TBXOR = 398,
    TCOLON = 399,
    TCOMMA = 400,
    TDIVIDE = 401,
    TDOT = 402,
    TDOTDOT = 403,
    TDOTDOTDOT = 404,
    TEQUAL = 405,
    TEXP = 406,
    TGREATER = 407,
    TGREATEREQUAL = 408,
    THASH = 409,
    TIO = 410,
    TLESS = 411,
    TLESSEQUAL = 412,
    TMINUS = 413,
    TMOD = 414,
    TNOTEQUAL = 415,
    TOR = 416,
    TPLUS = 417,
    TQUESTION = 418,
    TSEMI = 419,
    TSHIFTLEFT = 420,
    TSHIFTRIGHT = 421,
    TSTAR = 422,
    TSWAP = 423,
    TLCBR = 424,
    TRCBR = 425,
    TLP = 426,
    TRP = 427,
    TLSBR = 428,
    TRSBR = 429,
    TNOELSE = 430,
    TDOTDOTOPENHIGH = 431,
    TUPLUS = 432,
    TUMINUS = 433,
    TLNOT = 434
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

#line 468 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 474 "bison-chapel.cpp" /* yacc.c:358  */
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

#line 547 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   21343

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  168
/* YYNRULES -- Number of rules.  */
#define YYNRULES  705
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1271

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   434

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
     175,   176,   177,   178,   179
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   517,   517,   522,   523,   529,   530,   535,   536,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,   573,   577,   578,   580,   585,   586,   590,   603,   608,
     613,   621,   622,   623,   627,   628,   632,   633,   634,   639,
     638,   659,   660,   661,   666,   667,   672,   677,   682,   686,
     695,   700,   705,   710,   714,   718,   726,   731,   735,   740,
     744,   745,   746,   750,   751,   752,   753,   754,   755,   756,
     760,   765,   766,   767,   771,   772,   776,   780,   782,   784,
     786,   788,   790,   797,   798,   802,   803,   804,   805,   806,
     807,   810,   811,   812,   813,   814,   815,   827,   828,   839,
     840,   841,   842,   843,   844,   845,   846,   847,   848,   849,
     850,   851,   852,   853,   854,   855,   856,   857,   861,   862,
     863,   864,   865,   866,   867,   868,   869,   870,   871,   872,
     879,   880,   881,   882,   886,   887,   891,   892,   896,   897,
     899,   904,   905,   906,   916,   916,   921,   922,   923,   924,
     925,   926,   927,   931,   932,   933,   934,   939,   938,   955,
     954,   972,   971,   988,   987,  1005,  1004,  1020,  1019,  1035,
    1039,  1044,  1052,  1063,  1070,  1071,  1072,  1073,  1074,  1075,
    1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,  1085,
    1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,  1099,  1105,
    1111,  1117,  1124,  1131,  1135,  1142,  1146,  1147,  1148,  1149,
    1151,  1152,  1153,  1154,  1156,  1158,  1160,  1162,  1167,  1168,
    1172,  1174,  1182,  1183,  1188,  1193,  1194,  1195,  1196,  1197,
    1198,  1199,  1200,  1201,  1202,  1203,  1204,  1205,  1212,  1213,
    1214,  1215,  1224,  1225,  1229,  1231,  1234,  1240,  1242,  1245,
    1251,  1254,  1255,  1256,  1257,  1258,  1259,  1263,  1264,  1268,
    1269,  1270,  1274,  1275,  1279,  1282,  1284,  1289,  1290,  1294,
    1296,  1298,  1305,  1315,  1329,  1334,  1339,  1347,  1348,  1353,
    1354,  1356,  1361,  1377,  1384,  1393,  1401,  1405,  1412,  1413,
    1415,  1420,  1421,  1426,  1431,  1425,  1458,  1461,  1465,  1473,
    1483,  1472,  1522,  1526,  1531,  1535,  1540,  1547,  1548,  1552,
    1553,  1554,  1555,  1556,  1557,  1558,  1559,  1560,  1561,  1562,
    1563,  1564,  1565,  1566,  1567,  1568,  1569,  1570,  1571,  1572,
    1573,  1574,  1575,  1576,  1577,  1578,  1579,  1583,  1584,  1585,
    1586,  1587,  1588,  1589,  1590,  1591,  1592,  1593,  1594,  1598,
    1599,  1603,  1607,  1608,  1612,  1613,  1617,  1619,  1621,  1623,
    1625,  1627,  1632,  1633,  1637,  1638,  1639,  1640,  1641,  1642,
    1643,  1644,  1645,  1649,  1650,  1651,  1652,  1653,  1654,  1658,
    1659,  1660,  1664,  1665,  1666,  1667,  1668,  1669,  1673,  1674,
    1677,  1678,  1682,  1683,  1687,  1691,  1692,  1693,  1701,  1702,
    1704,  1706,  1708,  1713,  1715,  1720,  1721,  1722,  1723,  1724,
    1725,  1726,  1730,  1732,  1737,  1739,  1741,  1746,  1759,  1776,
    1777,  1779,  1784,  1785,  1786,  1787,  1788,  1792,  1798,  1806,
    1807,  1815,  1817,  1822,  1824,  1826,  1831,  1833,  1835,  1842,
    1843,  1844,  1849,  1851,  1853,  1857,  1861,  1863,  1867,  1875,
    1876,  1877,  1878,  1879,  1884,  1885,  1886,  1887,  1888,  1908,
    1912,  1916,  1924,  1931,  1932,  1933,  1937,  1939,  1945,  1947,
    1949,  1954,  1955,  1956,  1957,  1958,  1964,  1965,  1966,  1967,
    1971,  1972,  1976,  1977,  1978,  1982,  1983,  1987,  1988,  1992,
    1993,  1997,  1998,  1999,  2000,  2004,  2005,  2016,  2018,  2020,
    2026,  2027,  2028,  2029,  2030,  2031,  2033,  2035,  2037,  2039,
    2041,  2043,  2046,  2048,  2050,  2052,  2054,  2056,  2058,  2060,
    2063,  2065,  2070,  2072,  2074,  2076,  2078,  2080,  2082,  2084,
    2086,  2088,  2090,  2092,  2094,  2101,  2107,  2113,  2119,  2128,
    2138,  2146,  2147,  2148,  2149,  2150,  2151,  2152,  2153,  2158,
    2159,  2163,  2167,  2168,  2172,  2176,  2177,  2181,  2185,  2189,
    2196,  2197,  2198,  2199,  2200,  2201,  2205,  2206,  2211,  2213,
    2217,  2221,  2225,  2233,  2238,  2244,  2250,  2257,  2267,  2275,
    2276,  2277,  2278,  2279,  2280,  2281,  2282,  2283,  2284,  2286,
    2288,  2290,  2305,  2307,  2309,  2311,  2316,  2317,  2321,  2322,
    2323,  2327,  2328,  2329,  2330,  2339,  2340,  2341,  2342,  2343,
    2347,  2348,  2349,  2353,  2354,  2355,  2356,  2357,  2365,  2366,
    2367,  2368,  2372,  2373,  2377,  2378,  2382,  2383,  2384,  2385,
    2386,  2387,  2388,  2389,  2391,  2393,  2394,  2395,  2399,  2407,
    2408,  2412,  2413,  2414,  2415,  2416,  2417,  2418,  2419,  2420,
    2421,  2422,  2423,  2424,  2425,  2426,  2427,  2428,  2429,  2430,
    2431,  2432,  2433,  2434,  2439,  2440,  2441,  2442,  2443,  2444,
    2445,  2449,  2450,  2451,  2452,  2456,  2457,  2458,  2459,  2464,
    2465,  2466,  2467,  2468,  2469,  2470
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
  "TDEFER", "TDELETE", "TDEPRECATED", "TDMAPPED", "TDO", "TDOMAIN",
  "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE", "TFOR",
  "TFORALL", "TFOREACH", "TFORWARDING", "TIF", "TIMAG", "TIMPORT", "TIN",
  "TINCLUDE", "TINDEX", "TINLINE", "TINOUT", "TINT", "TITER",
  "TINITEQUALS", "TIMPLEMENTS", "TINTERFACE", "TLABEL", "TLAMBDA", "TLET",
  "TLIFETIME", "TLOCAL", "TLOCALE", "TMINUSMINUS", "TMODULE", "TNEW",
  "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY", "TOPERATOR",
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
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TDOTDOTOPENHIGH", "TUPLUS",
  "TUMINUS", "TLNOT", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "deprecated_decl_stmt",
  "deprecated_decl_base", "module_decl_start", "module_decl_stmt",
  "access_control", "opt_prototype", "include_access_control",
  "include_module_stmt", "$@1", "block_stmt", "stmt_ls", "renames_ls",
  "use_renames_ls", "opt_only_ls", "except_ls", "use_access_control",
  "use_stmt", "import_stmt", "import_expr", "import_ls", "require_stmt",
  "assignment_stmt", "opt_label_ident", "ident_fn_def", "ident_def",
  "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt",
  "deprecated_class_level_stmt", "class_level_stmt", "@2", "private_decl",
  "forwarding_stmt", "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6",
  "$@7", "$@8", "extern_block_stmt", "loop_stmt", "zippered_iterator",
  "if_stmt", "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "deprecated_enum_item", "enum_item", "lambda_decl_expr",
  "$@9", "$@10", "linkage_spec", "fn_decl_stmt", "$@11", "$@12",
  "fn_decl_stmt_inner", "fn_decl_receiver_expr", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls_inner",
  "formal_ls", "formal", "opt_intent_tag", "required_intent_tag",
  "opt_this_intent_tag", "proc_iter_or_op", "opt_ret_tag",
  "opt_throws_error", "opt_function_body_stmt", "function_body_stmt",
  "query_expr", "var_arg_expr", "opt_lifetime_where",
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
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434
};
# endif

#define YYPACT_NINF -1146

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1146)))

#define YYTABLE_NINF -656

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1146,   242,  3649, -1146,   -54,   171, -1146, -1146, -1146, -1146,
   -1146, -1146,  5041,   134,   262,   244, 14984,   258, 20857,   134,
   11623,   284,   327,   269,   262,  5041, 11623,  1907,  5041,   231,
   20943, 11795,  7993,   358,  8683, 10067, 10067,  7127,  8855,   363,
   -1146,   286, -1146,   415, 20977, 20977, 20977, -1146,  3371, 10239,
     427, 11623,   187, -1146,   436,   452, 11623, -1146, 14984, -1146,
   11623,   510,   379,    32,  2102,   506, 21063, -1146, 10413,  8165,
   11623, 10239, 14984, 11623,   476,   509,   420,  5041,   546, 11623,
     549, 11967, 11967, 20977,   553, -1146, 14984, -1146,   571,  8855,
   11623, -1146, 11623, -1146, 11623, -1146, -1146, 14502, 11623, -1146,
   11623, -1146, -1146, -1146,  3997,  7301,  9029, 11623, -1146,  4867,
   -1146, -1146, -1146,   463, -1146,   556, -1146, -1146,     8, -1146,
   -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146,   587, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, 20977, -1146, 20977,   443,   212, -1146, -1146,
    3371, -1146,   485, -1146,   487, -1146, -1146,   488,   494,   499,
   11623,   497,   498, 20062, 14495,   361,   501,   504, -1146, -1146,
     399, -1146, -1146, -1146, -1146, -1146,   429, -1146, -1146, 20062,
     492,  5041, -1146, -1146,   505, 11623, -1146, -1146, 11623, 11623,
   11623, 20977, -1146, 11623, 10413, 10413,   614,   416, -1146, -1146,
   -1146, -1146,   -40,   455, -1146, -1146,   502, 16694, 20977,  3371,
   -1146,   508, -1146,   -45, 20062,  3032, -1146, -1146,  9201,   272,
   21229, -1146, -1146,   557,  8339,   589, 21097, 20062,   227,   375,
   -1146, 21183, 20977, -1146,   227, 20977,   503,    38, 16280,    20,
   16198,    38, 16363,   367, -1146, 16446, 20977, 20977,   -52, 15451,
     354,  8339, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146, -1146, -1146,   507, -1146,   430,  5041,
     515,  1683,   117,    14, -1146,  5041, -1146, -1146, 16776,   664,
   -1146,   522,   523, -1146, 16776,   -40,   664, -1146,  8339, 17446,
   -1146, -1146, -1146,   309, 20062, 11623, 11623, -1146, 20062,   513,
   17352, -1146, 16776,   -40, 20062,   524,  8339, -1146, 20062, 17435,
   -1146, -1146, 17517,  3233, -1146, -1146, 17598,   574,   539,   -40,
      38, 16776, 17679,   304,   304,   838,   664,   664,   254, -1146,
   -1146,  4171,   173, -1146, 11623, -1146,   125,   145, -1146,    -6,
     -10, 17760,   165,   838,   696, -1146,  4345, -1146,   641, 11623,
   11623, 20977,   563,   540, -1146, -1146, -1146, -1146,   332,   465,
   -1146, 11623,   561, 11623, 11623, 11623, 10067, 10067, 11623,   473,
   11623, 11623, 11623, 11623, 11623,   243, 14502, 11623, 11623, 11623,
   11623, 11623, 11623, 11623, 11623, 11623, 11623, 11623, 11623, 11623,
   11623, 11623, 11623,   642, -1146, -1146, -1146, -1146, -1146,  9373,
    9373, -1146, -1146, -1146, -1146,  9373, -1146, -1146,  9373,  9373,
    8339,  8339, 10067, 10067,  7821, -1146, -1146, 16859, 16941, 17841,
     541,    27, 20977,  4519, -1146, 10067,    38,   552,   340, -1146,
   11623, -1146, -1146, 11623,   593, -1146,   551,   568, -1146, -1146,
   -1146, 20977, -1146,  3371, -1146, -1146, 20977,   562, 20977, -1146,
    3371,   673, 10413, -1146,  5215, 10067, -1146,   554, -1146,    38,
    5389, 10067, -1146,    38, -1146, 10067, -1146, 12139, 11623, -1146,
     603,   604,  5041,   694,  5041, -1146,   698, 11623, -1146, -1146,
     556,   559,  8339, 20977, -1146, -1146,   422, -1146, -1146,  1683,
   -1146,   590,   565, -1146, 12311,   610, 11623,  3371, -1146, -1146,
   11623, 11623, -1146,   566, -1146, 10413, -1146, 20062, 20062, -1146,
      40, -1146,  8339,   572, -1146,   719, -1146,   719, -1146, 12483,
     612, -1146, -1146, -1146, -1146, -1146, -1146, -1146,  9547, -1146,
   17922,  7475, -1146,  7649, -1146,  5041,   581, 10067,  9721,  3823,
     586, 11623, 10585, -1146, -1146,   352, -1146,  4693, 20977, -1146,
     356, 18003,   364, 16529,   383, 10413,   579, 20703,   190, -1146,
   18084, 16982, 16982,   490, -1146,   490, -1146,   490,  2491,   755,
    1441,  1354,   -40,   304, -1146,   591, -1146, -1146, -1146, -1146,
   -1146,   838,  1498,   490,   798,   798, 16982,   798,   798,  1680,
     304,  1498, 20481,  1680,   664,   664,   304,   838,   597,   601,
     605,   606,   607,   611,   595,   600, -1146,   490, -1146,   490,
      77, -1146, -1146, -1146,   151, -1146,  1701, 20213,   496, 12655,
   10067, 12827, 10067, 11623,  8339, 10067, 15264,   609,   134, 18165,
   -1146, -1146, -1146, 20062, 18246,  8339, -1146,  8339, 20977,   563,
     397, 20977, 20977,   563, -1146,   563,   414, 11623,   170,  8855,
   20062,    39, 17022,  7821, -1146,  8855, 20062,    33, 16611, -1146,
      38, 16776,   618,   623,   619, 18327,   623,   620, 11623, 11623,
     743,  5041,   750, 18408,  5041, 17104,   723, -1146,   183, -1146,
     250, -1146,   185, -1146, -1146, -1146, -1146, -1146, -1146,   931,
     645,   624, -1146,  3499, -1146,   466,   627,  1683,   117,    45,
      49, 11623, 11623,  6955, -1146, -1146,   428,  8511, -1146, 20062,
   -1146, 18489, 18570, -1146, -1146, 20062,   621,   110,   634, -1146,
    2785, -1146, -1146,   419, 20977, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146,  5041,    84, 17187, -1146, -1146, 20062,  5041, 20062,
   -1146, 18651, -1146, -1146, -1146, 11623, -1146,   133,    86, 11623,
   -1146, 10757, 12139, 11623, -1146,  8339,   662,  1333,   639, 20737,
     688,   176, -1146, -1146,   724, -1146, -1146, -1146, -1146, 14335,
     644, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146,  7821, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146,    46, 10067, 10067, 11623,   781, 18732,
   11623,   784, 18813,   255,   646, 18894,  8339,    38,    38, -1146,
   -1146, -1146, -1146,   563,   649, -1146,   563,   563,   650,   652,
   -1146, 16776, -1146, 15534,  5563, -1146,  5737, -1146,   270, -1146,
   15617,  5911, -1146,    38,  6085, -1146,    38, -1146, -1146, 11623,
   -1146, 11623, -1146, 20062, 20062,  5041, -1146,  5041, 11623, -1146,
    5041,   790, 20977,   663, 20977,   502, -1146, -1146, 20977,  1010,
   -1146,  1683,   682,   739, -1146, -1146, -1146,   139, -1146, -1146,
     610,   659,    93, -1146, -1146,   665,   667, -1146,  6259, 10413,
   -1146, -1146, -1146, 20977, -1146,   690,   502, -1146, -1146,  6433,
     666,  6607,   668, -1146, 11623, -1146, -1146, 11623, 18975,    36,
   17270,   677,   680,   275,   675,  2007, -1146, 20977, -1146, 11623,
   20823, -1146, -1146,   466,   679,   210, -1146,   704, -1146,   705,
     706,   716,   709,   710, -1146,   711,   720,   713,   717,   718,
     278,   727,   722,   725, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146, -1146, -1146, -1146, 11623, -1146,   728,
     732,   729,   679,   679, -1146, -1146, -1146,   610,   288,   292,
   19057, 12999, 13171, 19138, 13343, 13515, -1146, 13687, 13859,   295,
   -1146, -1146,   712, -1146,   714,   715, -1146, -1146, -1146,  5041,
    8855, 20062,  8855, 20062,  7821, -1146,  5041,  8855, 20062, -1146,
    8855, 20062, -1146, 19219, 20062, -1146, -1146, 20062,   828,  5041,
     721, -1146, -1146, -1146,   682, -1146,   702, 10931,   211, -1146,
       7, -1146, -1146, 10067, 15124,  8339,  8339,  5041, -1146,    18,
     703, 11623, -1146,  8855, -1146, 20062,  5041,  8855, -1146, 20062,
    5041, 20062,   256, 11103, 12139, 11623, 12139, 11623, -1146, -1146,
     736, -1146,  1202, -1146, 17446, -1146, 20627, -1146, -1146, -1146,
   20062, -1146,   143,   422, -1146, 19300, -1146, 15369, -1146, -1146,
   -1146, 11623, 11623, 11623, 11623, 11623, 11623, 11623, 11623, -1146,
   -1146,  2399, -1146, -1146,  3222, 20217, 18408, 15700, 15783, -1146,
   18408, 15866, 15949, 11623,  5041, -1146, -1146,   211,   682,  9895,
   -1146, -1146, -1146,   289, 10413, -1146, -1146,    51, 11623,    -7,
   19381, -1146,   434,   707,   731,   557, -1146,   502, 20062, 16032,
   -1146, 16115, -1146, -1146, -1146, 20062,   740,   741,   745,   746,
   -1146,  2238, -1146, -1146, -1146, 14031,   778,   747, -1146,   749,
     752,   679,   679, 19462, 19547, 19628, 19709, 19790, 19871, 20364,
   -1146, 20430, 20504, -1146, -1146,  5041,  5041,  5041,  5041, 20062,
   -1146, -1146, -1146,   211, 11277,    95, -1146, 20062, -1146,   155,
   -1146,   -14, -1146,   177, 19965, -1146, -1146, -1146, 13859,   730,
     733, -1146,  5041,  5041, -1146, -1146, -1146, -1146, -1146,  6781,
   -1146, -1146,   628, -1146,     7, -1146, -1146, -1146, 11623, 11623,
   11623, 11623, 11623, 11623, -1146, -1146, -1146, 18408, 18408, 18408,
   18408, -1146, -1146, -1146, -1146, -1146,   432, 10067, 14673, -1146,
   11623,    51,   155,   155,   155,   155,   155,   155,    51,   594,
   -1146, -1146, 18408, 18408,   738, 14203,    99,   235, 20022, -1146,
   -1146, 20062, -1146, -1146, -1146, -1146, -1146, -1146, -1146,   771,
   -1146, -1146,   685, 14844, -1146, -1146, -1146, 11451, -1146,   744,
   -1146
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,   117,   648,   649,   650,   644,
     645,   651,     0,   569,   103,   138,   538,   145,   540,   569,
       0,   144,     0,   443,   103,     0,     0,   316,     0,   262,
     139,   616,   616,   642,     0,     0,     0,     0,     0,   143,
      59,   263,   317,   140,     0,     0,     0,   313,     0,     0,
     147,     0,   588,   560,   652,   148,     0,   318,   532,   442,
       0,     0,     0,   164,   316,   142,   541,   444,     0,     0,
       0,     0,   536,     0,     0,   146,     0,     0,   118,     0,
     643,     0,     0,     0,   141,   296,   534,   446,   149,     0,
       0,   701,     0,   703,     0,   704,   705,   615,     0,   702,
     699,   519,   161,   700,     0,     0,     0,     0,     4,     0,
       5,     9,    42,     0,    45,    54,    10,    11,     0,    12,
      13,    14,    15,   515,   516,    25,    46,   162,   171,   172,
      16,    20,    17,    19,     0,   257,    18,   607,    22,    23,
      24,    21,   170,     0,   168,     0,   604,     0,   166,   169,
       0,   167,   621,   600,   517,   601,   522,   520,     0,     0,
       0,   605,   606,     0,   521,     0,   622,   623,   624,   646,
     647,   599,   524,   523,   602,   603,     0,    41,    27,   530,
       0,     0,   570,   104,     0,     0,   540,   139,     0,     0,
       0,     0,   541,     0,     0,     0,     0,   604,   621,   520,
     605,   606,   539,   521,   622,   623,     0,   569,     0,     0,
     445,     0,   270,     0,   500,   316,   294,   304,   616,   164,
     316,   295,    44,     0,   507,   644,   541,   617,   316,   644,
     193,   541,     0,   181,   316,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   176,     0,     0,     0,     0,     0,
      56,   507,   111,   119,   131,   125,   124,   133,   114,   123,
     134,   120,   135,   112,   136,   129,   122,   130,   128,   126,
     127,   113,   115,   121,   132,   137,     0,   116,     0,     0,
       0,     0,     0,     0,   449,     0,   155,    35,     0,   686,
     592,   589,   590,   591,     0,   533,   687,     7,   507,   316,
     163,   414,   497,     0,   496,     0,     0,   156,   620,     0,
       0,    38,     0,   537,   525,     0,   507,    39,   531,     0,
     277,   273,     0,   521,   277,   274,     0,   439,     0,   535,
       0,     0,     0,   688,   690,   613,   685,   684,     0,    61,
      64,     0,     0,   502,     0,   504,     0,     0,   503,     0,
       0,   496,     0,   614,     0,     6,     0,    55,     0,     0,
       0,     0,   297,     0,   400,   401,   399,   319,     0,   518,
      26,     0,   593,     0,     0,     0,     0,     0,     0,   689,
       0,     0,     0,     0,     0,     0,   612,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   357,   364,   365,   366,   361,   363,     0,
       0,   359,   362,   360,   358,     0,   368,   367,     0,     0,
     507,   507,     0,     0,     0,    28,    29,     0,     0,     0,
       0,     0,     0,     0,    30,     0,     0,     0,     0,    31,
       0,    32,    43,     0,   515,   513,     0,   508,   509,   514,
     187,     0,   190,     0,   179,   183,     0,     0,     0,   189,
       0,     0,     0,   203,     0,     0,   202,     0,   211,     0,
       0,     0,   209,     0,   216,     0,   215,     0,    76,   173,
       0,     0,     0,   231,     0,   357,   227,     0,    58,    57,
      54,     0,     0,     0,   241,    33,   382,   314,   453,     0,
     454,   456,     0,   478,     0,   459,     0,     0,   154,    34,
       0,     0,    36,     0,   165,     0,    96,   618,   619,   157,
       0,    37,     0,     0,   284,   275,   271,   276,   272,     0,
     437,   434,   196,   195,    40,    63,    62,    65,     0,   653,
       0,     0,   638,     0,   640,     0,     0,     0,     0,     0,
       0,     0,     0,   657,     8,     0,    48,     0,     0,    94,
       0,    91,     0,    70,   268,     0,     0,     0,   393,   448,
     568,   681,   680,   683,   692,   691,   696,   695,   677,   674,
     675,   676,   609,   664,   117,     0,   635,   636,   118,   634,
     633,   610,   668,   679,   673,   671,   682,   672,   670,   662,
     667,   669,   678,   661,   665,   666,   663,   611,     0,     0,
       0,     0,     0,     0,     0,     0,   694,   693,   698,   697,
     580,   581,   583,   585,     0,   572,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   655,   268,   569,   569,
     199,   435,   447,   501,     0,     0,   527,     0,     0,   297,
       0,     0,     0,   297,   436,   297,     0,     0,     0,     0,
     544,     0,     0,     0,   212,     0,   550,     0,     0,   210,
       0,     0,   700,    79,     0,    66,    77,     0,     0,     0,
     230,     0,   226,     0,     0,     0,     0,   526,     0,   244,
       0,   242,   387,   384,   385,   386,   390,   391,   392,   382,
     375,     0,   372,     0,   383,   402,     0,   457,     0,   152,
     153,   151,   150,     0,   477,   476,   600,     0,   451,   598,
     450,     0,     0,   632,   499,   498,     0,     0,     0,   528,
       0,   278,   441,   600,     0,   654,   608,   639,   505,   641,
     506,   223,     0,     0,     0,   656,   221,   554,     0,   659,
     658,     0,    50,    49,    47,     0,    90,     0,     0,     0,
      83,     0,     0,    76,   265,     0,   298,     0,     0,     0,
     311,     0,   305,   308,   397,   394,   395,   398,   320,     0,
       0,   102,   100,   101,    99,    98,    97,   630,   631,   582,
     584,     0,   571,   138,   145,   144,   143,   140,   147,   148,
     142,   146,   141,   149,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   194,
     511,   512,   510,   297,     0,   192,   297,   297,     0,     0,
     191,     0,   225,     0,     0,   201,     0,   200,     0,   575,
       0,     0,   207,     0,     0,   205,     0,   214,   213,     0,
     174,     0,   175,   239,   238,     0,   233,     0,     0,   229,
       0,   235,     0,   267,     0,     0,   388,   389,     0,   382,
     371,     0,   491,   403,   406,   405,   407,     0,   455,   458,
     459,     0,     0,   460,   461,     0,     0,   286,     0,     0,
     285,   288,   529,     0,   279,   282,     0,   438,   224,     0,
       0,     0,     0,   222,     0,    95,    92,     0,    73,    72,
      71,     0,     0,     0,     0,   316,   303,     0,   310,     0,
     306,   302,   396,   402,   369,   105,   351,   119,   349,   125,
     124,   108,   123,   120,   354,   135,   106,   136,   122,   126,
     107,   109,   121,   137,   348,   330,   333,   331,   332,   355,
     343,   334,   347,   339,   337,   350,   353,   338,   336,   341,
     346,   335,   340,   344,   345,   342,   352,     0,   329,     0,
     110,     0,   369,   369,   327,   637,   573,   459,   621,   621,
       0,     0,     0,     0,     0,     0,   267,     0,     0,     0,
     198,   197,     0,   299,     0,     0,   299,   299,   204,     0,
       0,   543,     0,   542,     0,   574,     0,     0,   549,   208,
       0,   548,   206,    68,    67,   232,   228,   559,   234,     0,
       0,   264,   243,   240,   491,   373,     0,     0,   459,   404,
     418,   452,   482,     0,   655,   507,   507,     0,   290,     0,
       0,     0,   280,     0,   219,   556,     0,     0,   217,   555,
       0,   660,     0,     0,     0,    76,     0,    76,    84,    87,
     269,   293,   316,   164,   316,   292,   316,   300,   158,   309,
     312,   307,     0,   382,   326,     0,   356,     0,   322,   323,
     577,     0,     0,     0,     0,     0,     0,     0,     0,   269,
     299,   316,   299,   299,   316,   316,   547,     0,     0,   576,
     553,     0,     0,     0,     0,   237,    60,   459,   491,     0,
     494,   493,   495,   600,   415,   378,   376,     0,     0,     0,
       0,   480,   600,     0,     0,   291,   289,     0,   283,     0,
     220,     0,   218,    93,    75,    74,     0,     0,     0,     0,
     266,   316,   160,   301,   473,     0,   408,     0,   328,   105,
     107,   369,   369,     0,     0,     0,     0,     0,     0,   316,
     186,   316,   316,   178,   182,     0,     0,     0,     0,    69,
     236,   379,   377,   459,   483,     0,   417,   416,   432,     0,
     433,   420,   423,     0,   419,   412,   413,   315,     0,   594,
     595,   281,     0,     0,    86,    89,    85,    88,   159,     0,
     472,   471,   600,   409,   418,   370,   324,   325,     0,     0,
       0,     0,     0,     0,   188,   180,   184,   546,   545,   552,
     551,   381,   380,   485,   486,   488,   600,     0,   655,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   600,
     596,   597,   558,   557,     0,   463,     0,     0,     0,   487,
     489,   422,   424,   425,   428,   429,   430,   426,   427,   421,
     468,   466,   600,   655,   410,   321,   411,   483,   467,   600,
     490
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1146, -1146, -1146,     3,    11,  2381, -1146,    13, -1146, -1146,
   -1146,   435, -1146, -1146, -1146,   442,   573,  -475, -1146,  -746,
    -732, -1146, -1146, -1146,   163, -1146, -1146,   150,   903, -1146,
    2725,  -101,  -764, -1146,  -936,  2377, -1086,  -138,  -870, -1146,
     -63, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146,    92, -1146,   842, -1146, -1146,    68,  1113, -1146, -1146,
   -1146, -1146, -1146,   626, -1146,    43, -1146, -1146, -1146, -1146,
   -1146, -1146,  -551,  -842, -1146, -1146, -1146,    23,  -742,  1351,
   -1146, -1146, -1146,   362, -1146, -1146, -1146, -1146,  -144,  -154,
    -938, -1146, -1146,  -126,    79,   252, -1146, -1146, -1146,    29,
   -1146, -1146,  -294,    28, -1043,  -250,  -279,  -270,   -87, -1146,
    -196, -1146,     9,   925,  -122,   459, -1146,  -478,  -848, -1052,
   -1146,  -658,  -522, -1145,  -979,  -948,   -16, -1146,    72, -1146,
    -235,  -463,  -480,   583,  -486, -1146, -1146, -1146,  1368, -1146,
      -8, -1146, -1146,  -238, -1146,  -654, -1146, -1146, -1146,  1828,
    1858,   -12,   936,    69,  1054, -1146,  2271,  2455, -1146, -1146,
   -1146, -1146, -1146, -1146, -1146, -1146, -1146,  -418
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   340,  1066,   682,   111,   112,   113,   114,
     115,   358,   490,   116,   250,   117,   341,   673,   562,   677,
     674,   118,   119,   120,   559,   560,   121,   122,   184,   968,
     282,   123,   277,   124,   714,   287,   125,  1067,   126,   299,
     127,   128,   129,   456,   651,   458,   652,   451,   648,   130,
     131,   814,   132,   248,   133,   690,   691,   196,   135,   136,
     137,   138,   139,   525,   731,   896,   140,   141,   727,   891,
     142,   143,   566,   915,   144,   145,   771,   772,   773,   197,
     280,   705,   147,   148,   568,   923,   778,   971,   972,   487,
    1074,   497,   700,   701,   702,   703,   704,   779,   367,   877,
    1204,  1265,  1187,   445,  1115,  1119,  1181,  1182,  1183,   149,
     328,   530,   150,   151,   283,   284,   501,   502,   718,  1201,
    1146,   505,   715,  1224,  1112,  1028,   342,   213,   346,   347,
     446,   447,   448,   198,   153,   154,   155,   156,   199,   158,
     181,   182,   624,   469,   838,   625,   626,   159,   160,   200,
     201,   163,   234,   449,   203,   165,   204,   205,   168,   169,
     170,   171,   352,   172,   173,   174,   175,   176
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     179,   300,   473,   676,   202,   108,   628,   732,   207,   839,
     419,   206,   437,   109,   214,   970,   491,   912,   716,   227,
     227,   706,   238,   240,   242,   245,   249,   918,   368,   688,
     911,   209,  1031,  1186,  1078,  1079,   457,   288,   547,   289,
     222,   726,   728,   733,   294,  1068,   295,   503,   296,   482,
     880,   285,   303,   285,  1178,   359,   304,   308,   310,   312,
     313,   314,   506,   513,  1171,   318,   841,   319,  1117,   322,
     326,   285,   834,  1054,   329,   635,  1107,   331,   332,   -82,
     333,   523,   334,  1249,    69,   335,   336,   438,   337,   584,
     350,  1110,   304,   308,   351,   353,   302,   -53,   824,   379,
     440,  -262,   828,  1230,   829,  -263,   585,   385,  1055,   467,
     177,   546,   550,   467,  -287,   109,   -53,   433,   503,   441,
     586,   360,  1270,   444,  1118,   789,   237,   239,   241,  1080,
    1221,  1231,   302,   345,   302,   548,   584,   976,   309,   467,
     503,  1033,  1179,  1227,  1144,   -82,   977,   635,   372,   587,
     444,  1091,   467,  1180,  1094,  1095,  -287,   300,  1178,   507,
    1173,  1186,   433,   515,   549,  -474,   790,   822,   545,   419,
    -474,   813,   548,   179,   348,  1069,   427,   428,   429,   431,
    1116,   318,   304,   351,   888,   614,   615,   433,   588,   433,
     504,  -474,  1142,  1261,   589,  1225,  1068,   444,   349,   436,
    -474,   636,   433,   467,  -469,   290,   227,   433,   433,  1200,
    -287,  1268,   308,  1206,  1207,   444,   224,   774,  -474,  1030,
     251,  1068,   302,   302,  1068,  1068,   889,  -245,   442,   879,
    -474,   321,   325,   866,  -469,   588,   514,   453,   548,   308,
     548,  -474,     3,   460,   548,   628,   584,  -469,  1159,  1250,
    1161,  1162,   209,   180,    23,   907,  -474,  1180,   899,  1172,
    -469,   504,  -474,   585,   291,   183,   364,  1034,   775,  1228,
     541,  1198,   992,  1263,   867,   994,   995,   586,    42,   776,
     890,  -474,   292,   504,   590,   365,   308,  1145,   676,  1068,
     543,  1068,  1068,   517,   518,   366,   791,   542,   777,   293,
    -246,  1232,   913,    57,   308,    59,   587,  -469,  -474,  1137,
     552,  1139,  -469,   970,  -252,   515,    67,   544,   538,   444,
     444,   920,  1136,   792,  1138,  1222,    69,  1233,   647,  1234,
    1235,   650,   540,  1236,  1237,   717,   375,   -53,   656,   553,
    -251,    87,   832,   539,   537,   588,   921,   561,   563,  -111,
    1099,   589,   109,   989,    23,   863,   -53,  -117,   210,   570,
    1114,   571,   572,   573,   575,   577,   578,   109,   579,   580,
     581,   582,   583,   628,   591,   592,   593,   594,   595,   596,
     597,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,   444,   376,  1026,    23,   864,   377,   308,   308,  1264,
     647,   849,   224,   308,   433,    59,   308,   308,   308,   308,
     617,   619,   627,  -492,  -260,  1004,    67,  -113,   177,  -250,
     647,   444,   865,   639,   535,  -118,  1133,   986,   643,   843,
     846,   644,  1052,  -579,  -492,   208,   488,  -578,  -492,   489,
     647,    87,  1005,   379,   109,    59,   658,  1060,   383,   692,
     304,   385,   660,   662,   515,   388,    67,   251,   666,   668,
    -579,  -492,   454,   671,  -578,   675,   675,  1089,   574,   576,
     693,  -247,   683,   516,   694,   685,  -475,   507,   609,   610,
     308,    87,  -479,  -254,   611,   507,   455,   612,   613,  -475,
     302,   286,  -258,   873,   719,  -479,   569,   695,   721,   722,
     696,   755,    61,   725,   642,   900,   902,   699,  -255,   759,
     308,   697,  1121,   286,   616,   618,   177,   422,   297,  -475,
     756,   423,   752,   320,   324,  -479,   725,   638,   760,   308,
     698,   308,   420,   444,   421,   744,   725,   747,   897,   749,
     751,  1113,   507,   724,   874,  -475,   444,   764,  1122,   766,
     298,  -479,  -475,   304,   765,   875,  -484,   661,  -479,   507,
     537,   825,  -249,   667,  -440,  -253,   724,   670,   109,   345,
    -629,   345,  -629,  -475,   876,   315,   724,  -484,   830,  -479,
     676,  -484,   676,  -440,   806,   152,   628,  -628,   423,  -628,
     452,   316,  -475,   302,  -374,   152,   459,  -475,  -479,   433,
    -475,   493,  -259,  -479,  -484,  -261,  -479,  -565,   152,  -248,
     348,   152,   348,   738,  -628,   740,  -628,   660,   809,   666,
     812,   683,   308,   815,   747,   973,  -625,  -256,  -625,   379,
     817,   818,   356,   308,   383,   308,  -627,   385,  -627,   743,
     357,   388,  -481,   361,  -626,   831,  -626,   833,   434,  -562,
     369,   627,  -561,   840,   371,  -481,   906,   590,   370,  1202,
     152,  -566,  -567,   424,   444,  -564,   853,   854,  -563,   426,
     432,   433,   439,   820,   462,   443,   450,   519,   492,   286,
     286,   286,   286,   286,   286,  -481,   496,   152,  1226,  -470,
     483,   486,   152,   510,   511,   522,   375,   882,   529,   314,
     318,   351,  1239,   531,   554,   308,   558,   565,   385,   567,
     608,  -481,   634,   647,   821,   444,   641,   645,  -481,  -470,
     494,   657,   808,   646,   811,   663,   654,   678,   679,   681,
     286,   687,  -470,   684,   717,   707,   286,   708,   723,  -481,
     730,   302,  1226,   561,   729,  -470,  -462,   908,   767,   910,
     675,   675,   376,   308,   286,   742,   377,   734,  -481,  1262,
     748,   781,   780,  -481,   152,   782,  -481,   787,   849,   783,
     784,   785,   286,   286,   788,   786,  -462,  1269,   855,   627,
     816,  1226,   -78,   850,   852,   857,   884,   375,   862,  -462,
     869,   887,  -470,   575,   617,   980,   870,  -470,   983,   878,
    1123,  1124,  -462,   379,   308,  -464,   892,   515,   383,   916,
     384,   385,   919,   922,   981,   388,   975,   984,   993,   996,
     987,   997,  1001,   395,  1003,  1019,  1027,  1021,  1029,  1008,
     375,   401,  1011,  1032,  1041,  -464,  1035,  1013,  1036,  1014,
    1046,  1058,  1050,   376,  1059,  1061,  1017,   377,  -464,  -462,
    1073,  -138,  -145,  -144,  -462,  -114,  -143,  -140,  -147,  -112,
    -148,  -464,   152,  1104,  -142,  -146,  -115,  1076,   152,  -141,
     375,  -116,  -149,  1039,  1108,  1127,  1077,   304,   286,  1189,
     699,  1090,  1203,  1092,  1093,  1106,   376,  1045,  -111,  1049,
     377,  -113,  1051,  1240,   379,   675,  1241,   574,   616,   383,
    1140,   384,   385,  1190,  1194,  1195,   388,  1070,  -464,  1196,
    1197,   286,  1260,  -464,   395,   286,  1231,   302,   905,  1205,
     399,   400,   401,  1152,   152,   686,   376,   211,  1143,   557,
     377,   330,  1022,  1151,   444,   444,  1040,   379,   380,   152,
     381,   382,   383,  1071,   384,   385,   386,  1147,  1025,   388,
     527,   868,  1072,  1266,  1247,  1075,   394,   395,   692,  1259,
     398,  1252,   244,   399,   400,   401,   720,   228,     0,  1001,
    1003,     0,  1008,  1011,   402,  1045,  1049,   379,   380,   693,
     381,   382,   383,   694,   384,   385,     0,  1096,  1097,   388,
    1098,     0,   627,     0,  1100,  1101,   394,   395,  1102,     0,
     398,   300,     0,   399,   400,   401,   695,     0,     0,   696,
       0,   354,     0,     0,     0,     0,   152,     0,     0,     0,
     697,  1120,   747,   308,   308,     0,     0,     0,     0,  1128,
       0,  1129,     0,     0,     0,  1131,     0,   692,     0,   698,
       0,  1135,   675,   675,   675,   675,     0,   152,     0,     0,
       0,     0,     0,   152,     0,  1111,   164,     0,   693,     0,
       0,     0,   694,     0,     0,   152,   164,   152,     0,  1096,
    1153,  1154,  1100,  1155,  1156,  1157,  1158,     0,     0,   164,
       0,     0,   164,     0,   699,   695,     0,     0,   696,     0,
      61,  1169,  1229,  1175,     0,     0,     0,   351,     0,   697,
       0,     0,  1177,   286,   286,     0,  1184,     0,     0,   286,
     286,     0,   286,   286,     0,   134,     0,     0,   698,     0,
       0,     0,     0,     0,     0,   134,     0,   861,   152,     0,
       0,   164,   152,     0,     0,   323,   323,   302,   134,     0,
     152,   134,  1176,     0,     0,  1253,  1254,  1255,  1256,  1257,
    1258,     0,     0,  1217,  1218,  1219,  1220,     0,   164,     0,
       0,     0,     0,   164,     0,     0,     0,     0,     0,     0,
       0,     0,   894,     0,     0,     0,  1049,     0,     0,     0,
    1242,  1243,     0,  1246,     0,     0,     0,   351,     0,     0,
     134,     0,     0,     0,     0,     0,  1217,  1218,  1219,  1220,
    1242,  1243,  1223,     0,     0,     0,     0,     0,     0,   804,
    1141,     0,     0,     0,     0,  1248,   747,   134,  1251,     0,
       0,     0,   134,     0,   216,     0,     0,   302,    22,    23,
       0,     0,     0,     0,     0,   164,     0,     0,   217,     0,
      31,   218,     0,     0,     0,     0,    37,     0,     0,     0,
       0,   747,     0,    42,     0,  1049,  1223,     0,     0,   286,
     286,     0,     0,     0,   152,     0,     0,   152,     0,     0,
       0,     0,     0,   286,     0,   486,     0,     0,    57,     0,
      59,     0,   486,     0,  1063,   286,     0,  1064,   286,   221,
       0,    67,     0,     0,   134,  1223,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1023,     0,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,   152,     0,     0,     0,     0,
       0,   152,     0,   164,   914,     0,     0,     0,  1042,   164,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   146,     0,  -299,     0,     0,     0,  -299,
    -299,     0,   974,   146,  -299,     0,   102,     0,     0,  -299,
     157,  -299,  -299,     0,     0,     0,   146,  -299,     0,   146,
     157,     0,     0,     0,  -299,     0,   375,  -299,   978,   979,
       0,     0,   134,   157,     0,   164,   157,     0,   134,     0,
       0,     0,     0,     0,     0,     0,  -299,     0,     0,  -299,
     164,  -299,     0,  -299,     0,  -299,  -299,   152,  -299,   152,
    -299,     0,  -299,     0,   152,     0,     0,   152,   146,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   152,     0,
     152,  -299,   376,   152,  -299,   157,   377,  -299,     0,     0,
       0,     0,     0,     0,   134,   146,     0,     0,     0,     0,
     146,     0,     0,     0,     0,     0,     0,     0,     0,   134,
       0,   152,   157,   375,     0,     0,     0,   157,     0,     0,
       0,   286,   152,     0,   152,     0,     0,   164,     0,     0,
       0,     0,     0,   379,   380,     0,     0,  -299,   383,     0,
     384,   385,     0,  -299,     0,   388,     0,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,   164,   399,
     400,   401,     0,     0,   164,     0,     0,     0,     0,   376,
     375,     0,   146,   377,     0,     0,   164,     0,   164,   486,
     486,     0,     0,   486,   486,     0,   134,     0,     0,   157,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1185,     0,     0,     0,     0,     0,     0,     0,  1191,
       0,   486,     0,   486,     0,     0,     0,   134,     0,     0,
     379,   380,   152,   134,   382,   383,   376,   384,   385,   152,
     377,     0,   388,     0,     0,   134,     0,   134,     0,   164,
     395,     0,   152,   164,     0,     0,   399,   400,   401,     0,
       0,   164,     0,     0,     0,     0,     0,     0,     0,     0,
     152,     0,     0,     0,     0,     0,     0,     0,     0,   152,
     146,     0,     0,   152,     0,     0,   146,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,   157,     0,   388,
     389,   390,     0,   157,   392,   393,   394,   395,   134,     0,
     398,     0,   134,   399,   400,   401,     0,     0,     0,     0,
     134,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   252,   152,     0,  1185,
       0,     0,   146,     0,     0,     0,     0,     0,     0,     0,
     253,   254,     0,   255,   584,     0,     0,   146,   256,   157,
       0,     0,   375,     0,     0,     0,     0,   257,   793,     0,
       0,   794,     0,   258,   157,     0,   795,     0,     0,   259,
       0,     0,     0,   260,     0,   164,   261,   187,   164,     0,
       0,     0,     0,     0,     0,     0,   262,   796,   152,   152,
     152,   152,   263,   264,   797,     0,     0,     0,     0,     0,
     265,     0,     0,     0,   798,     0,     0,     0,   376,   266,
       0,   799,   377,     0,     0,   152,   152,     0,   267,   268,
       0,   269,     0,   270,   146,   271,     0,   800,   272,     0,
       0,     0,   273,   498,   134,   274,   164,   134,   275,   801,
       0,   157,   164,   588,     0,     0,     0,     0,     0,     0,
     802,     0,     0,     0,     0,   146,   803,     0,     0,   379,
     380,   146,   381,   382,   383,     0,   384,   385,     0,     0,
     161,   388,   157,   146,     0,   146,     0,     0,   157,   395,
     161,     0,     0,     0,     0,   399,   400,   401,     0,     0,
     157,     0,   157,   161,   499,   134,   161,     0,     0,     0,
     162,   134,     0,     0,     0,     0,     0,     0,     0,     0,
     162,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   162,     0,     0,   162,     0,   164,     0,
     164,     0,     0,     0,     0,   164,   146,     0,   164,     0,
     146,     0,     0,     0,     0,   161,     0,     0,   146,   164,
       0,   164,     0,   157,   164,   215,     0,   157,     0,     0,
       0,     0,     0,     0,     0,   157,     0,     0,     0,   216,
       0,     0,   161,    22,    23,   162,     0,   161,     0,     0,
       0,     0,   164,   217,     0,    31,   218,   134,     0,   134,
       0,    37,     0,   164,   134,   164,     0,   134,    42,     0,
       0,     0,   162,     0,     0,     0,     0,   162,   134,     0,
     134,     0,   -51,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,     0,     0,   219,
       0,   -51,   220,     0,   221,     0,    67,     0,     0,     0,
       0,   134,     0,     0,     0,     0,     0,     0,     0,   161,
       0,     0,   134,     0,   134,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,   216,
       0,     0,   146,    22,    23,   146,     0,     0,  1062,   162,
       0,     0,     0,   217,     0,    31,   218,     0,     0,   157,
       0,    37,   157,   164,     0,     0,     0,     0,    42,     0,
     164,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,   164,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,  1063,
       0,   164,  1064,   146,   221,     0,    67,     0,     0,   146,
     164,     0,     0,     0,   164,     0,     0,   161,     0,     0,
     157,     0,   134,   161,     0,    83,   157,     0,    85,   134,
       0,    87,     0,     0,   216,     0,     0,     0,    22,    23,
       0,     0,   134,     0,     0,     0,     0,   162,   217,     0,
      31,   218,     0,   162,     0,     0,    37,     0,     0,   -81,
     134,     0,     0,    42,     0,     0,     0,     0,   164,   134,
       0,     0,     0,   134,     0,     0,     0,   -52,     0,   161,
       0,   102,     0,     0,     0,     0,     0,  1065,    57,     0,
      59,     0,     0,     0,   161,   146,   -52,   146,     0,   221,
       0,    67,   146,     0,     0,   146,     0,     0,     0,   162,
       0,     0,   157,     0,   157,     0,   146,     0,   146,   157,
      83,   146,   157,    85,   162,   -81,    87,   134,     0,   164,
     164,   164,   164,   157,     0,   157,     0,     0,   157,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   146,
       0,     0,     0,     0,     0,     0,   164,   164,     0,     0,
     146,     0,   146,     0,     0,     0,   157,     0,     0,     0,
     216,   161,     0,     0,    22,    23,     0,   157,     0,   157,
       0,     0,     0,   166,   217,     0,    31,   218,   134,   134,
     134,   134,    37,   166,     0,     0,     0,     0,     0,    42,
       0,   162,   161,     0,     0,     0,   166,     0,   161,   166,
       0,     0,     0,     0,     0,   134,   134,     0,     0,     0,
     161,     0,   161,     0,    57,     0,    59,     0,     0,     0,
    1063,     0,   162,  1064,     0,   221,     0,    67,   162,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     162,     0,   162,     0,     0,     0,    83,     0,   166,    85,
     146,     0,    87,     0,     0,     0,     0,   146,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   157,     0,     0,
     146,     0,     0,   161,   157,   166,     0,   161,     0,     0,
     166,     0,     0,   110,     0,   161,     0,   157,   146,     0,
       0,     0,     0,   178,     0,     0,     0,   146,     0,     0,
       0,   146,   102,   162,     0,   157,   212,   162,     0,   223,
       0,     0,     0,     0,   157,   162,     0,     0,   157,     0,
       0,   216,     0,     0,     0,    22,    23,     0,     0,     0,
    1062,     0,     0,     0,     0,   217,     0,    31,   218,     0,
       0,     0,     0,    37,     0,     0,     0,     0,   311,     0,
      42,     0,   166,     0,     0,   146,     0,   167,   317,     0,
       0,     0,     0,     0,     0,     0,     0,   167,     0,     0,
       0,     0,   157,     0,     0,    57,     0,    59,     0,    61,
     167,  1063,     0,   167,  1064,   110,   221,     0,    67,     0,
     355,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,     0,   161,
      85,     0,   161,    87,     0,     0,   146,   146,   146,   146,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,   167,   157,   157,   157,   157,     0,     0,   162,
       0,     0,   162,   146,   146,     0,     0,     0,     0,     0,
     166,     0,     0,     0,     0,     0,   166,     0,     0,   167,
     157,   157,   425,   102,   167,     0,     0,     0,     0,  1160,
     161,     0,     0,     0,     0,     0,   161,     0,     0,   376,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     162,     0,     0,     0,     0,     0,   162,     0,     0,     0,
       0,     0,   166,     0,   463,   466,   468,   472,   474,   476,
       0,     0,     0,     0,     0,     0,     0,   166,     0,     0,
     379,   380,     0,   381,   382,   383,   167,   384,   385,   386,
       0,   387,   388,   389,   390,     0,     0,   392,   393,   394,
     395,   396,     0,   398,     0,     0,   399,   400,   401,     0,
     495,     0,   161,     0,   161,   509,   508,   402,     0,   161,
       0,   512,   161,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   161,     0,   161,     0,     0,   161,   521,
       0,     0,   162,     0,   162,     0,     0,     0,     0,   162,
       0,     0,   162,     0,   166,     0,     0,   532,   533,     0,
       0,     0,     0,   162,     0,   162,   161,     0,   162,     0,
       0,     0,   110,     0,     0,     0,     0,   161,     0,   161,
       0,     0,     0,     0,   167,   166,     0,   110,     0,     0,
     167,   166,     0,     0,     0,     0,   162,     0,     0,     0,
       0,     0,     0,   166,     0,   166,     0,   162,     0,   162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   276,
     278,   279,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,     0,     0,     0,     0,   167,     0,     0,     0,
       0,     0,   253,   254,     0,   255,     0,     0,   327,     0,
     256,   167,     0,   640,   110,     0,   166,     0,     0,   257,
     166,     0,     0,     0,     0,   258,     0,   161,   166,     0,
       0,   259,     0,     0,   161,   260,     0,     0,   261,     0,
       0,     0,     0,     0,     0,   508,   664,   161,   262,     0,
     669,   508,     0,     0,   263,   264,     0,   162,     0,     0,
       0,     0,   265,   680,   162,   161,     0,     0,   362,     0,
     363,   266,     0,     0,   161,     0,     0,   162,   161,     0,
     267,   268,     0,   269,     0,   270,     0,   271,   167,     0,
     272,     0,     0,     0,   273,   162,     0,   274,     0,     0,
     275,     0,     0,     0,   162,     0,     0,     0,   162,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,   167,
       0,     0,     0,     0,     0,   167,   741,     0,     0,     0,
     746,     0,   161,   327,     0,     0,     0,   167,   110,   167,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   166,     0,   433,   166,   893,   327,     0,     0,
     461,     0,   162,     0,     0,     0,     0,     0,     0,     0,
       0,   480,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   161,   161,   161,   161,     0,     0,     0,
     167,     0,     0,     0,   167,     0,   500,     0,     0,     0,
       0,     0,   167,   166,     0,     0,     0,     0,     0,   166,
     161,   161,     0,   162,   162,   162,   162,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   835,   837,
       0,     0,     0,     0,   842,   845,     0,   847,   848,     0,
     162,   162,     0,     0,   216,     0,     0,     0,    22,    23,
       0,     0,   856,     0,     0,   859,     0,     0,   217,     0,
      31,   218,     0,     0,     0,     0,    37,     0,     0,     0,
       0,     0,     0,    42,     0,     0,   564,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   -51,     0,     0,
       0,     0,     0,     0,     0,   166,     0,   166,    57,     0,
      59,     0,   166,     0,   219,   166,   -51,   220,     0,   221,
       0,    67,     0,   898,     0,     0,   166,     0,   166,   903,
       0,   166,     0,     0,     0,     0,   167,     0,     0,   167,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   637,     0,   166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     166,     0,   166,     0,     0,     0,   649,     0,     0,     0,
       0,   653,     0,   655,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   990,   991,   102,   167,     0,     0,
       0,     0,     0,   167,     0,     0,     0,     0,   998,     0,
       0,     0,     0,     0,     0,   508,     0,   508,   689,     0,
    1009,     0,   508,  1012,   500,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1015,     0,  1016,     0,
       0,  1018,     0,     0,   216,     0,     0,     0,    22,    23,
       0,     0,     0,  1062,     0,     0,     0,     0,   217,     0,
      31,   218,     0,     0,     0,     0,    37,     0,     0,  1038,
     166,     0,     0,    42,     0,     0,     0,   166,     0,     0,
    1044,     0,  1048,   754,     0,     0,     0,     0,     0,   167,
     166,   167,   770,     0,     0,     0,   167,     0,    57,   167,
      59,     0,    61,     0,  1063,     0,     0,  1064,   166,   221,
     167,    67,   167,     0,     0,   167,     0,   166,     0,     0,
       0,   166,     0,     0,     0,     0,     0,     0,     0,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,   167,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,   167,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,     0,   823,   252,   166,   826,   827,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,   253,   254,
       0,   255,  1163,     0,     0,     0,   256,     0,     0,     0,
    1105,   418,     0,     0,  -625,   257,  -625,     0,     0,     0,
       0,   258,     0,     0,     0,     0,  1126,   259,  1125,     0,
       0,   260,     0,     0,   261,     0,     0,  1130,   872,     0,
       0,  1132,   500,     0,   262,     0,   166,   166,   166,   166,
     263,   264,     0,     0,     0,     0,     0,     0,   265,     0,
       0,     0,     0,     0,   167,   895,     0,   266,     0,   327,
       0,   167,     0,   166,   166,     0,   267,   268,     0,   269,
       0,   270,     0,   271,   167,     0,   272,     0,     0,     0,
     273,     0,     0,   274,     0,  1170,   275,     0,     0,     0,
       0,     0,   167,     0,   770,     0,     0,     0,     0,     0,
       0,   167,   252,     0,   969,   167,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   253,   254,     0,   255,
       0,     0,     0,     0,   256,     0,     0,     0,     0,     0,
       0,     0,     0,   257,     0,     0,     0,     0,     0,   258,
       0,     0,   281,     0,     0,   259,     0,     0,     0,   260,
       0,     0,   261,     0,     0,     0,     0,     0,     0,   167,
       0,     0,   262,     0,     0,     0,     0,     0,   263,   264,
       0,     0,     0,     0,     0,     0,   265,     0,     0,     0,
       0,     0,     0,     0,     0,   266,     0,  1020,     0,   689,
       0,     0,     0,  1024,   267,   268,   500,   269,     0,   270,
       0,   271,     0,     0,   272,     0,     0,     0,   273,     0,
       0,   274,     0,     0,   275,     0,     0,     0,   895,     0,
     167,   167,   167,   167,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   770,     0,     0,   770,     0,   167,   167,    -2,
       4,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
     871,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -80,     0,    40,    41,
      42,     0,    43,  -316,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,   -51,    52,    53,     0,    54,    55,
      56,     0,  -316,     0,     0,    57,    58,    59,    60,    61,
      62,    63,  -316,   -51,    64,    65,    66,     0,    67,    68,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
       0,    78,    79,     0,    80,    81,    82,    83,    84,     0,
      85,    86,   -80,    87,    88,     0,     0,    89,     0,    90,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,   969,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,   102,     0,     0,   103,     0,   104,     0,
     105,     0,   106,     0,     4,   107,     5,     0,     6,     7,
       8,     9,    10,    11,     0,  -655,     0,    12,    13,    14,
      15,    16,  -655,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,  -655,    28,    29,  -655,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -80,     0,    40,    41,    42,     0,    43,  -316,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,   -51,    52,
      53,     0,    54,    55,    56,     0,  -316,     0,     0,    57,
      58,    59,    60,     0,    62,    63,  -316,   -51,    64,    65,
      66,  -655,    67,    68,    69,  -655,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,     0,    80,    81,
      82,    83,    84,     0,    85,    86,   -80,    87,    88,     0,
       0,    89,     0,    90,     0,     0,  -655,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -655,  -655,    94,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,     0,  -655,  -655,  -655,  -655,  -655,     0,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,   102,  -655,  -655,
    -655,     0,   104,  -655,   105,     0,   106,     0,   338,  -655,
       5,   301,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -80,     0,    40,    41,    42,     0,
      43,  -316,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -51,    52,    53,     0,    54,    55,    56,     0,
    -316,     0,     0,    57,    58,    59,    60,    61,    62,    63,
    -316,   -51,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -80,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,   339,   105,     0,
     106,     0,     4,   107,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -80,     0,
      40,    41,    42,     0,    43,  -316,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,   -51,    52,    53,     0,
      54,    55,    56,     0,  -316,     0,     0,    57,    58,    59,
      60,    61,    62,    63,  -316,   -51,    64,    65,    66,     0,
      67,    68,    69,     0,    70,    71,    72,    73,    74,    75,
      76,    77,     0,    78,    79,     0,    80,    81,    82,    83,
      84,     0,    85,    86,   -80,    87,    88,     0,     0,    89,
       0,    90,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,   102,     0,     0,   103,     0,
     104,   536,   105,     0,   106,     0,   555,   107,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -80,     0,    40,    41,    42,     0,    43,  -316,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
     -51,    52,    53,     0,    54,    55,    56,     0,  -316,     0,
       0,    57,    58,    59,    60,    61,    62,    63,  -316,   -51,
      64,    65,    66,     0,    67,    68,    69,     0,    70,    71,
      72,    73,    74,    75,    76,    77,     0,    78,    79,     0,
      80,    81,    82,    83,    84,     0,    85,    86,   -80,    87,
      88,     0,     0,    89,     0,    90,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,   102,
       0,     0,   103,     0,   104,   556,   105,     0,   106,     0,
     338,   107,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -80,     0,    40,    41,
      42,     0,    43,  -316,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,   -51,    52,    53,     0,    54,    55,
      56,     0,  -316,     0,     0,    57,    58,    59,    60,    61,
      62,    63,  -316,   -51,    64,    65,    66,     0,    67,    68,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
       0,    78,    79,     0,    80,    81,    82,    83,    84,     0,
      85,    86,   -80,    87,    88,     0,     0,    89,     0,    90,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,   102,     0,     0,   103,     0,   104,   339,
     105,     0,   106,     0,     4,   107,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -80,     0,    40,    41,    42,     0,    43,  -316,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,   -51,    52,
      53,     0,    54,    55,    56,     0,  -316,     0,     0,    57,
      58,    59,    60,    61,    62,    63,  -316,   -51,    64,    65,
      66,     0,    67,    68,    69,     0,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,     0,    80,    81,
      82,    83,    84,     0,    85,    86,   -80,    87,    88,     0,
       0,    89,     0,    90,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,   102,     0,     0,
     103,     0,   104,   753,   105,     0,   106,     0,     4,   107,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -80,     0,    40,    41,    42,     0,
      43,  -316,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -51,    52,    53,     0,    54,    55,    56,     0,
    -316,     0,     0,    57,    58,    59,    60,   354,    62,    63,
    -316,   -51,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -80,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,     0,   105,     0,
     106,     0,     4,   107,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -80,     0,
      40,    41,    42,     0,    43,  -316,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,   -51,    52,    53,     0,
      54,    55,    56,     0,  -316,     0,     0,    57,    58,    59,
      60,     0,    62,    63,  -316,   -51,    64,    65,    66,     0,
      67,    68,    69,     0,    70,    71,    72,    73,    74,    75,
      76,    77,     0,    78,    79,     0,    80,    81,    82,    83,
      84,     0,    85,    86,   -80,    87,    88,     0,     0,    89,
       0,    90,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,   102,     0,     0,   103,     0,
     104,     0,   105,     0,   106,     0,     4,   107,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
     659,    39,   -80,     0,    40,    41,    42,     0,    43,  -316,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
     -51,    52,    53,     0,    54,    55,    56,     0,  -316,     0,
       0,    57,    58,    59,    60,     0,    62,    63,  -316,   -51,
      64,    65,    66,     0,    67,    68,    69,     0,    70,    71,
      72,    73,    74,    75,    76,    77,     0,    78,    79,     0,
      80,    81,    82,    83,    84,     0,    85,    86,   -80,    87,
      88,     0,     0,    89,     0,    90,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,   102,
       0,     0,   103,     0,   104,     0,   105,     0,   106,     0,
       4,   107,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,   665,    39,   -80,     0,    40,    41,
      42,     0,    43,  -316,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,   -51,    52,    53,     0,    54,    55,
      56,     0,  -316,     0,     0,    57,    58,    59,    60,     0,
      62,    63,  -316,   -51,    64,    65,    66,     0,    67,    68,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
       0,    78,    79,     0,    80,    81,    82,    83,    84,     0,
      85,    86,   -80,    87,    88,     0,     0,    89,     0,    90,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,   102,     0,     0,   103,     0,   104,     0,
     105,     0,   106,     0,     4,   107,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1000,    39,
     -80,     0,    40,    41,    42,     0,    43,  -316,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,   -51,    52,
      53,     0,    54,    55,    56,     0,  -316,     0,     0,    57,
      58,    59,    60,     0,    62,    63,  -316,   -51,    64,    65,
      66,     0,    67,    68,    69,     0,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,     0,    80,    81,
      82,    83,    84,     0,    85,    86,   -80,    87,    88,     0,
       0,    89,     0,    90,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,   102,     0,     0,
     103,     0,   104,     0,   105,     0,   106,     0,     4,   107,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1002,    39,   -80,     0,    40,    41,    42,     0,
      43,  -316,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -51,    52,    53,     0,    54,    55,    56,     0,
    -316,     0,     0,    57,    58,    59,    60,     0,    62,    63,
    -316,   -51,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -80,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,     0,   105,     0,
     106,     0,     4,   107,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,  1007,    39,   -80,     0,
      40,    41,    42,     0,    43,  -316,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,   -51,    52,    53,     0,
      54,    55,    56,     0,  -316,     0,     0,    57,    58,    59,
      60,     0,    62,    63,  -316,   -51,    64,    65,    66,     0,
      67,    68,    69,     0,    70,    71,    72,    73,    74,    75,
      76,    77,     0,    78,    79,     0,    80,    81,    82,    83,
      84,     0,    85,    86,   -80,    87,    88,     0,     0,    89,
       0,    90,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,   102,     0,     0,   103,     0,
     104,     0,   105,     0,   106,     0,     4,   107,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
    1010,    39,   -80,     0,    40,    41,    42,     0,    43,  -316,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
     -51,    52,    53,     0,    54,    55,    56,     0,  -316,     0,
       0,    57,    58,    59,    60,     0,    62,    63,  -316,   -51,
      64,    65,    66,     0,    67,    68,    69,     0,    70,    71,
      72,    73,    74,    75,    76,    77,     0,    78,    79,     0,
      80,    81,    82,    83,    84,     0,    85,    86,   -80,    87,
      88,     0,     0,    89,     0,    90,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,   102,
       0,     0,   103,     0,   104,     0,   105,     0,   106,     0,
       4,   107,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,  1037,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -80,     0,    40,    41,
      42,     0,    43,  -316,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,   -51,    52,    53,     0,    54,    55,
      56,     0,  -316,     0,     0,    57,    58,    59,    60,     0,
      62,    63,  -316,   -51,    64,    65,    66,     0,    67,    68,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
       0,    78,    79,     0,    80,    81,    82,    83,    84,     0,
      85,    86,   -80,    87,    88,     0,     0,    89,     0,    90,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,   102,     0,     0,   103,     0,   104,     0,
     105,     0,   106,     0,     4,   107,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1043,    39,
     -80,     0,    40,    41,    42,     0,    43,  -316,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,   -51,    52,
      53,     0,    54,    55,    56,     0,  -316,     0,     0,    57,
      58,    59,    60,     0,    62,    63,  -316,   -51,    64,    65,
      66,     0,    67,    68,    69,     0,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,     0,    80,    81,
      82,    83,    84,     0,    85,    86,   -80,    87,    88,     0,
       0,    89,     0,    90,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,   102,     0,     0,
     103,     0,   104,     0,   105,     0,   106,     0,     4,   107,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1047,    39,   -80,     0,    40,    41,    42,     0,
      43,  -316,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -51,    52,    53,     0,    54,    55,    56,     0,
    -316,     0,     0,    57,    58,    59,    60,     0,    62,    63,
    -316,   -51,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -80,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,     0,   105,     0,
     106,     0,  1244,   107,     5,   301,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,  1245,   881,   107,     5,   301,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,     0,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,   243,    23,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,    59,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,    67,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,    87,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,     0,   105,     0,
     195,     0,     0,   107,     5,   301,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,   305,   306,     0,
      84,   343,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
     344,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,     0,     0,   107,     5,   301,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,   305,   306,     0,    84,   343,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,   737,   195,     0,
       0,   107,     5,   301,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,   190,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,   305,   306,     0,    84,   343,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   194,     0,
     105,   739,   195,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,   620,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,   621,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
     622,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,   623,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,     0,     5,   107,     6,     7,
       8,   229,    10,    11,   230,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     231,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,   232,    84,     0,   233,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,   305,   306,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,   307,
       0,     0,   103,     0,   194,     0,   105,     0,   195,     0,
       0,   107,     5,   301,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,   190,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,   305,   306,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   194,     0,
     105,     0,   195,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,   883,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,   305,   306,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,   235,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,   236,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,   246,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,   247,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,     0,
       0,   107,     5,   301,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,   190,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
     236,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   194,     0,
     105,     0,   195,     0,     5,   107,     6,     7,     8,   229,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   231,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,   232,
      84,     0,   233,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,   305,
     306,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     0,   107,
       5,   301,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,   735,   105,     0,
     195,     0,     0,   107,     5,   301,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,   745,     0,   107,     5,   301,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,  1174,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,   236,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,     0,   105,     0,
     195,     0,     5,   107,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,   285,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,   190,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   104,     0,
     105,     0,   195,     0,     0,   107,     5,   301,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,   750,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,   909,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,     0,   105,     0,
     195,     0,     0,   107,     5,   301,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   709,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,   710,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,   711,    74,    75,
      76,   712,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,  1109,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,  1134,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     0,   107,
       5,   301,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,     0,   105,     0,
    1109,     0,     0,   107,     5,   301,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,  1088,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     5,   107,
       6,     7,     8,   225,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   226,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,     0,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   104,     0,   105,     0,
     195,     0,     5,   107,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,   190,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   672,     0,   194,     0,
     105,     0,   195,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   709,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,   710,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,   711,    74,    75,
      76,   712,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   713,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   713,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     807,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,     0,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   810,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,     0,   105,     0,
     195,     0,     5,   107,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,  1082,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   194,     0,
     105,     0,   195,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,  1083,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,  1085,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
    1086,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,     0,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,  1087,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,     0,   105,     0,
     195,     0,     5,   107,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,  1088,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   194,     0,
     105,     0,   195,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   709,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,   710,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,   711,    74,    75,
      76,   712,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,  1199,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   924,     0,   925,     0,
       0,     0,    92,    93,    94,    95,    96,   926,     0,     0,
       0,    97,   927,   254,   928,   929,     0,     0,     0,     0,
     930,    98,     0,     0,    99,   100,   101,     0,     0,   257,
     103,   187,     0,     0,   105,   931,  1199,     0,     0,   107,
       0,   932,     0,     0,     0,   260,     0,     0,   933,     0,
     934,     0,     0,     0,     0,     0,     0,     0,   935,     0,
       0,     0,     0,     0,   936,   937,     0,     0,     0,     0,
       0,     0,   265,     0,     0,     0,     0,     0,     0,     0,
       0,   938,     0,     0,     0,     0,     0,     0,     0,     0,
     267,   268,     0,   939,     0,   270,     0,   940,     0,     0,
     941,     0,     0,     0,   942,     0,     0,   274,     0,     0,
     943,     0,     0,     0,     0,     0,     0,     0,     0,   485,
     404,   405,   406,   407,   408,     0,     0,   411,   412,   413,
     414,     0,   416,   417,   944,   945,   946,   947,   948,   949,
       0,   950,     0,     0,     0,   951,   952,   953,   954,   955,
     956,   957,   958,   959,   960,   961,     0,   962,     0,     0,
     963,   964,   965,   966,     0,     5,   967,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   185,     0,     0,    15,
      16,     0,    17,     0,   186,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   187,     0,
       0,     0,    33,   188,   189,     0,     0,   190,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   191,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   192,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   193,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   417,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
    -586,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,   418,   100,   101,  -625,     0,  -625,   103,
       0,   194,     0,   105,     0,   195,     5,   301,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,  -483,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,     0,  -483,     0,
       0,     0,  -483,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,   194,     0,   105,  -483,  1109,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,  -465,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,  -465,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,  -465,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,  -465,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,   101,  -465,    21,
       0,     0,     0,  -465,     0,   105,     0,  1199,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,    94,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,   101,     0,    21,
       0,     0,     0,   194,     0,   105,     0,   195,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,   101,     0,    21,
       0,     0,     0,   194,     0,   105,     0,   713,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
       0,     0,  1149,    84,     0,     0,    86,     0,     0,    88,
       0,   926,     0,     0,     0,     0,   253,   254,   928,   255,
       0,     0,     0,     0,   256,     0,     0,     0,     0,     0,
       0,     0,     0,   257,     0,    94,     0,     0,     0,   931,
       0,     0,     0,     0,     0,   259,     0,     0,     0,   260,
       0,     0,   261,     0,   934,     0,     0,     0,     0,     0,
       0,     0,   262,     0,     0,   105,     0,   195,   936,   264,
       0,     0,     0,     0,     0,     0,   265,     0,     0,     0,
       0,     0,     0,     0,     0,   266,     0,     0,     0,     0,
       0,     0,     0,   373,   267,   268,     0,   269,     0,   270,
     374,  1150,     0,     0,   941,     0,     0,     0,   273,     0,
       0,   274,     0,   375,   275,     0,     0,     0,     0,     0,
       0,     0,     0,   485,   404,   405,   406,   407,   408,     0,
       0,   411,   412,   413,   414,     0,   416,   417,   944,   945,
     946,   947,   948,   949,     0,   950,     0,     0,     0,   951,
     952,   953,   954,   955,   956,   957,   958,   959,   960,   961,
       0,   962,     0,     0,   963,   964,   965,   966,     0,   376,
       0,     0,     0,   377,     0,     0,   373,     0,     0,     0,
       0,     0,   484,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,   378,   485,   404,   405,   406,   407,
     408,     0,     0,   411,   412,   413,   414,     0,   416,   417,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,     0,
     433,     0,   376,     0,     0,     0,   377,   402,     0,   373,
       0,     0,     0,     0,     0,   999,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,   378,   485,   404,
     405,   406,   407,   408,     0,     0,   411,   412,   413,   414,
       0,   416,   417,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,     0,     0,   399,
     400,   401,     0,   433,     0,   376,     0,     0,     0,   377,
     402,     0,   373,     0,     0,     0,     0,     0,  1006,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
     378,   485,   404,   405,   406,   407,   408,     0,     0,   411,
     412,   413,   414,     0,   416,   417,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,     0,   399,   400,   401,     0,   433,     0,   376,     0,
       0,     0,   377,   402,     0,   373,     0,     0,     0,     0,
       0,  1165,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,   378,   485,   404,   405,   406,   407,   408,
       0,     0,   411,   412,   413,   414,     0,   416,   417,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,   433,
       0,   376,     0,     0,     0,   377,   402,     0,   373,     0,
       0,     0,     0,     0,  1166,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,     0,
       0,     0,     0,     0,     0,     0,   378,   485,   404,   405,
     406,   407,   408,     0,     0,   411,   412,   413,   414,     0,
     416,   417,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,     0,     0,   399,   400,
     401,     0,   433,     0,   376,     0,     0,     0,   377,   402,
       0,   373,     0,     0,     0,     0,     0,  1167,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,   378,
     485,   404,   405,   406,   407,   408,     0,     0,   411,   412,
     413,   414,     0,   416,   417,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,     0,
       0,   399,   400,   401,     0,   433,     0,   376,     0,     0,
       0,   377,   402,     0,   373,     0,     0,     0,     0,     0,
    1168,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,   378,   485,   404,   405,   406,   407,   408,     0,
       0,   411,   412,   413,   414,     0,   416,   417,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,     0,   399,   400,   401,     0,   433,     0,
     376,     0,     0,     0,   377,   402,     0,   373,     0,     0,
       0,     0,     0,  1192,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,   378,   485,   404,   405,   406,
     407,   408,     0,     0,   411,   412,   413,   414,     0,   416,
     417,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,   433,     0,   376,     0,     0,     0,   377,   402,     0,
     373,     0,     0,     0,     0,     0,  1193,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,   470,     0,     0,     0,     0,     0,     0,   378,   485,
     404,   405,   406,   407,   408,     0,   471,   411,   412,   413,
     414,     0,   416,   417,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,     0,     0,
     399,   400,   401,     0,   433,     0,   376,     0,     0,     0,
     377,   402,   373,     0,     0,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,   464,     0,     0,     0,   467,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,   465,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,     0,   399,   400,   401,     0,   433,   376,     0,
       0,     0,   377,     0,   402,   373,     0,     0,     0,     0,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   285,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,   433,
       0,   376,     0,     0,     0,   377,   402,     0,   373,     0,
       0,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,     0,
       0,     0,     0,   477,     0,     0,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,   478,   392,
     393,   394,   395,   396,   397,   398,     0,     0,   399,   400,
     401,     0,   433,     0,   376,     0,     0,     0,   377,   402,
       0,   373,   761,     0,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,   762,     0,     0,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,   763,   392,   393,   394,   395,   396,   397,   398,     0,
     479,   399,   400,   401,     0,     0,     0,   376,     0,     0,
       0,   377,   402,   373,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   844,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,     0,   399,   400,   401,     0,     0,   376,
       0,     0,     0,   377,     0,   402,   373,     0,     0,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
     467,     0,     0,     0,   378,     0,     0,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,     0,
     433,     0,   376,     0,     0,     0,   377,   402,   373,     0,
       0,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,   285,
       0,     0,     0,   180,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,     0,     0,   399,
     400,   401,     0,     0,   376,     0,     0,     0,   377,     0,
     402,   373,     0,     0,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,   629,     0,     0,     0,     0,     0,     0,   378,
       0,     0,     0,     0,     0,     0,     0,   630,     0,     0,
       0,     0,     0,     0,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,     0,
       0,   399,   400,   401,     0,   433,     0,   376,     0,     0,
       0,   377,   402,   373,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   631,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,   375,   392,   393,   394,   395,   396,
     397,   398,     0,     0,   399,   400,   401,     0,     0,   376,
       0,     0,     0,   377,   373,   402,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   836,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,     0,     0,     0,
     376,     0,     0,     0,   377,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,   378,   399,   400,   401,     0,
     376,     0,     0,     0,   377,     0,   373,   402,     0,     0,
       0,   379,   380,   374,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   375,     0,   392,   393,
     394,   395,   396,   397,   398,   378,     0,   399,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,   433,   376,     0,     0,     0,   377,     0,   402,   373,
       0,     0,     0,     0,     0,   860,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,     0,     0,   399,
     400,   401,     0,   433,     0,   376,     0,     0,     0,   377,
     402,     0,   373,     0,     0,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,   467,  1056,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,  1057,   392,   393,   394,   395,   396,   397,   398,
       0,     0,   399,   400,   401,     0,     0,     0,   376,     0,
       0,   901,   377,   402,   373,     0,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,     0,
     376,     0,     0,     0,   377,     0,   402,   373,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   216,     0,
       0,     0,    22,    23,     0,   378,     0,     0,     0,     0,
       0,     0,   217,     0,    31,   218,     0,     0,     0,     0,
      37,   379,   380,     0,   381,   382,   383,    42,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,   520,    57,   376,    59,     0,     0,   377,   402,   373,
       0,     0,     0,   221,     0,    67,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,    83,     0,     0,    85,   378,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,     0,   524,
     399,   400,   401,     0,     0,   376,     0,     0,     0,   377,
     373,   402,     0,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,   526,   399,   400,   401,     0,   376,     0,     0,     0,
     377,   373,     0,   402,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,   528,   399,   400,   401,     0,   376,     0,     0,
       0,   377,   373,     0,   402,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,   534,   399,   400,   401,     0,   376,     0,
       0,     0,   377,   373,     0,   402,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,   551,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,   376,
       0,     0,     0,   377,   373,     0,   402,     0,     0,     0,
       0,   374,   633,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,     0,
     376,     0,     0,     0,   377,   373,   757,   402,     0,     0,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,   376,     0,     0,   736,   377,   373,     0,   402,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     758,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,     0,     0,   399,   400,
     401,     0,   376,     0,     0,     0,   377,   373,     0,   402,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,  -587,
     392,   393,   394,   395,   396,   397,   398,     0,     0,   399,
     400,   401,     0,   376,     0,     0,     0,   377,   373,     0,
     402,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,     0,
       0,     0,     0,     0,   180,     0,     0,     0,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,     0,     0,
     399,   400,   401,     0,   376,     0,     0,     0,   377,   373,
     851,   402,     0,     0,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,     0,
     819,   399,   400,   401,     0,   376,     0,     0,     0,   377,
     373,     0,   402,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,     0,     0,   858,     0,     0,     0,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,     0,   399,   400,   401,     0,   376,     0,     0,     0,
     377,   373,     0,   402,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,     0,   399,   400,   401,     0,   376,     0,     0,
       0,   377,   373,     0,   402,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,     0,   399,   400,   401,     0,   376,     0,
       0,   885,   377,   373,     0,   402,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,   376,
       0,     0,   886,   377,   373,     0,   402,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   982,     0,     0,     0,     0,
       0,     0,     0,   904,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,     0,
     376,     0,     0,     0,   377,   373,     0,   402,     0,     0,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   985,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,   376,     0,     0,     0,   377,   373,     0,   402,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,     0,     0,   399,   400,
     401,     0,   376,     0,     0,     0,   377,   373,  1053,   402,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,     0,     0,   399,
     400,   401,     0,   376,     0,     0,     0,   377,   988,   373,
     402,     0,     0,     0,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,     0,     0,
     399,   400,   401,     0,     0,   376,     0,     0,     0,   377,
     373,   402,     0,     0,     0,     0,     0,   374,  1081,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,     0,   399,   400,   401,     0,   376,     0,     0,     0,
     377,   373,  1103,   402,     0,     0,     0,     0,   374,  1084,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,     0,   399,   400,   401,     0,   376,     0,     0,
       0,   377,   373,     0,   402,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,     0,   399,   400,   401,     0,   376,     0,
       0,     0,   377,   373,     0,   402,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,   376,
       0,     0,  1148,   377,   373,     0,   402,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,     0,
     376,     0,     0,     0,   377,  1188,     0,   402,     0,   373,
       0,     0,     0,  1208,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,     0,     0,     0,     0,   376,     0,     0,   402,   377,
     373,     0,     0,     0,     0,     0,     0,   374,  1209,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,     0,   399,   400,   401,     0,   376,     0,     0,     0,
     377,   373,     0,   402,     0,     0,     0,     0,   374,  1210,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,     0,   399,   400,   401,     0,   376,     0,     0,
       0,   377,   373,     0,   402,     0,     0,     0,     0,   374,
    1211,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,     0,   399,   400,   401,     0,   376,     0,
       0,     0,   377,   373,     0,   402,     0,     0,     0,     0,
     374,  1212,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,   376,
       0,     0,     0,   377,     0,     0,   402,     0,     0,     0,
       0,     0,  1213,     0,     0,     0,     0,   373,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,  1238,   392,   393,   394,
     395,   396,   397,   398,   373,     0,   399,   400,   401,     0,
       0,   374,     0,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,   376,   375,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   373,     0,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,   378,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
     376,   384,   385,   386,   377,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,     0,     0,
     399,   400,   401,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,   378,     0,     0,     0,     0,
     376,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,   378,     0,   399,   400,   401,
       0,     0,     0,     0,     0,     0,  1267,     0,   402,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,   373,     0,   399,   400,   401,
       0,     0,   374,     0,     0,     0,     0,     0,   402,   216,
       0,     0,     0,    22,    23,   375,     0,     0,  1062,     0,
       0,     0,     0,   217,     0,    31,   218,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,  1063,
       0,   805,  1064,     0,   221,   377,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,     0,     0,   399,   400,
     401,   102,     0,     0,     0,     0,   216,  1164,     0,   402,
      22,    23,     0,     0,     0,  1062,     0,     0,     0,     0,
     217,     0,    31,   218,     0,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,  1063,     0,     0,  1064,
       0,   221,   216,    67,     0,     0,    22,    23,     0,     0,
       0,  1062,     0,     0,     0,     0,   217,     0,    31,   218,
       0,     0,    83,     0,    37,    85,     0,     0,    87,     0,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,  1063,   375,     0,  1064,     0,   221,     0,    67,
       0,     0,     0,     0,     0,     0,   216,     0,   102,     0,
      22,    23,     0,     0,  1214,  1062,     0,     0,    83,     0,
     217,    85,    31,   218,    87,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   376,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,  1063,     0,     0,  1064,
       0,   221,     0,    67,   102,     0,     0,     0,     0,     0,
    1215,     0,     0,     0,   378,     0,     0,     0,     0,     0,
       0,     0,    83,     0,     0,    85,     0,     0,    87,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,     0,     0,   392,   393,   394,
     395,   396,     0,   398,     0,     0,   399,   400,   401,   216,
       0,     0,     0,    22,    23,     0,     0,   402,  1062,     0,
       0,     0,     0,   217,     0,    31,   218,     0,   102,     0,
       0,    37,     0,     0,  1216,     0,     0,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,   768,    59,   252,   354,     0,  1063,
       0,     0,  1064,     0,   221,     0,    67,     0,     0,     0,
     253,   254,     0,   255,     0,     0,     0,     0,   256,     0,
       0,     0,     0,     0,   769,    83,     0,   257,    85,     0,
     252,    87,     0,   258,     0,   917,     0,     0,     0,   259,
       0,     0,     0,   260,   253,   254,   261,   255,     0,     0,
       0,     0,   256,     0,     0,     0,   262,     0,     0,     0,
       0,   257,   263,   264,     0,     0,     0,   258,     0,     0,
     265,     0,     0,   259,     0,     0,     0,   260,     0,   266,
     261,   102,     0,     0,     0,     0,     0,     0,   267,   268,
     262,   269,     0,   270,     0,   271,   263,   264,   272,     0,
       0,     0,   273,     0,   265,   274,     0,     0,   275,     0,
       0,     0,     0,   266,     0,     0,   252,     0,     0,     0,
       0,     0,   267,   268,     0,   269,     0,   270,     0,   271,
     253,   254,   272,   255,     0,     0,   273,     0,   256,   274,
       0,     0,   275,     0,   769,     0,     0,   257,     0,     0,
    -294,     0,     0,   258,     0,     0,     0,     0,     0,   259,
       0,     0,     0,   260,  -294,  -294,   261,  -294,     0,     0,
       0,     0,  -294,     0,     0,     0,   262,     0,     0,     0,
       0,  -294,   263,   264,     0,     0,     0,  -294,     0,     0,
     265,     0,     0,  -294,     0,     0,     0,  -294,     0,   266,
    -294,     0,     0,     0,     0,     0,     0,     0,   267,   268,
    -294,   269,     0,   270,     0,   271,  -294,  -294,   272,     0,
       0,     0,   273,     0,  -294,   274,     0,     0,   275,     0,
       0,     0,     0,  -294,     0,     0,  -304,     0,     0,     0,
       0,     0,  -294,  -294,     0,  -294,     0,  -294,     0,  -294,
    -304,  -304,  -294,  -304,     0,     0,  -294,     0,  -304,  -294,
       0,     0,  -294,     0,     0,     0,     0,  -304,     0,     0,
     252,     0,     0,  -304,     0,     0,     0,     0,     0,  -304,
       0,     0,     0,  -304,   253,   254,  -304,   255,     0,     0,
       0,     0,   256,     0,     0,     0,  -304,     0,     0,     0,
       0,   257,  -304,  -304,     0,     0,     0,   258,     0,     0,
    -304,     0,     0,   259,     0,     0,     0,   260,     0,  -304,
     261,     0,     0,     0,     0,     0,     0,     0,  -304,  -304,
     262,  -304,     0,  -304,     0,  -304,   263,   264,  -304,     0,
       0,     0,  -304,     0,   265,  -304,     0,     0,  -304,     0,
       0,     0,     0,   266,     0,     0,  -295,     0,     0,     0,
       0,     0,   267,   268,     0,   269,     0,   270,     0,   271,
    -295,  -295,   272,  -295,     0,     0,   273,     0,  -295,   274,
       0,     0,   275,     0,     0,     0,     0,  -295,     0,     0,
    -185,     0,     0,  -295,     0,     0,     0,     0,     0,  -295,
       0,     0,     0,  -295,  -185,  -185,  -295,  -185,     0,     0,
       0,     0,  -185,     0,     0,     0,  -295,     0,     0,     0,
       0,  -185,  -295,  -295,     0,     0,     0,  -185,     0,     0,
    -295,     0,     0,  -185,     0,     0,     0,  -185,     0,  -295,
    -185,     0,     0,     0,     0,     0,     0,     0,  -295,  -295,
    -185,  -295,     0,  -295,     0,  -295,  -185,  -185,  -295,     0,
       0,     0,  -295,     0,  -185,  -295,     0,     0,  -295,     0,
       0,     0,     0,  -185,     0,     0,  -177,     0,     0,     0,
       0,     0,  -185,  -185,     0,  -185,     0,  -185,     0,  -185,
    -177,  -177,  -185,  -177,     0,     0,  -185,     0,  -177,  -185,
       0,     0,  -185,     0,     0,     0,     0,  -177,     0,     0,
       0,     0,     0,  -177,     0,     0,     0,     0,     0,  -177,
       0,     0,     0,  -177,     0,     0,  -177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -177,     0,     0,     0,
       0,   216,  -177,  -177,     0,    22,    23,     0,     0,     0,
    -177,     0,     0,     0,     0,   217,     0,    31,   218,  -177,
       0,     0,     0,    37,     0,     0,     0,     0,  -177,  -177,
      42,  -177,     0,  -177,     0,  -177,     0,     0,  -177,     0,
       0,     0,  -177,     0,   -52,  -177,     0,     0,  -177,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,     0,
       0,     0,     0,   -52,     0,     0,   221,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87
};

static const yytype_int16 yycheck[] =
{
      12,    64,   240,   478,    16,     2,   424,   529,    20,   663,
     164,    19,   208,     2,    26,   779,   251,   763,   504,    31,
      32,   499,    34,    35,    36,    37,    38,   769,   150,   492,
     762,    22,   880,  1119,   972,   973,   232,    49,    48,    51,
      27,     1,   522,   529,    56,   915,    58,     1,    60,   101,
     708,    33,    68,    33,     3,    47,    68,    69,    70,    71,
      72,    73,    48,   298,  1107,    77,    33,    79,    61,    81,
      82,    33,    33,    37,    86,    48,  1024,    89,    90,    47,
      92,   316,    94,  1228,    91,    97,    98,   209,   100,     3,
     106,  1027,   104,   105,   106,   107,    68,    65,   649,   139,
     145,    56,   653,   117,   655,    56,    20,   147,    72,   119,
     164,   349,   350,   119,    74,   104,    84,   169,     1,   164,
      34,   113,  1267,   224,   117,    48,    34,    35,    36,   977,
    1173,   145,   104,   105,   106,   145,     3,   791,    69,   119,
       1,    48,    91,    48,     1,   113,   804,    48,   160,    63,
     251,   993,   119,   102,   996,   997,   116,   220,     3,   145,
    1108,  1247,   169,   145,   174,    48,    89,   647,   174,   323,
     124,   634,   145,   185,   105,   917,   188,   189,   190,   195,
    1028,   193,   194,   195,    74,   420,   421,   169,   102,   169,
     144,   145,  1062,  1245,   108,  1174,  1066,   298,   106,   207,
      61,   174,   169,   119,    61,    18,   218,   169,   169,  1145,
     170,  1263,   224,  1151,  1152,   316,   171,    27,   172,   877,
     171,  1091,   194,   195,  1094,  1095,   116,    56,   215,   707,
      91,    81,    82,    48,    91,   102,   299,   228,   145,   251,
     145,   124,     0,   234,   145,   663,     3,   104,  1090,  1228,
    1092,  1093,   243,   119,    27,   169,   117,   102,   174,  1107,
     117,   144,   145,    20,    77,     3,    54,   174,    78,   174,
     145,  1141,   823,   174,    89,   826,   827,    34,    51,    89,
     170,   164,    95,   144,   385,    73,   298,   144,   763,  1159,
     145,  1161,  1162,   305,   306,    83,   145,   172,   108,   112,
      56,   124,   765,    76,   316,    78,    63,   164,   169,  1055,
     145,  1057,   169,  1077,    56,   145,    89,   172,   145,   420,
     421,   145,  1054,   172,  1056,  1173,    91,   150,   145,   152,
     153,   453,   344,   156,   157,   124,    32,    65,   460,   174,
      56,   114,   172,   170,   341,   102,   170,   359,   360,   139,
    1004,   108,   341,   816,    27,   172,    84,   147,    89,   371,
     149,   373,   374,   375,   376,   377,   378,   356,   380,   381,
     382,   383,   384,   791,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   492,    88,   871,    27,   145,    92,   409,   410,   164,
     145,   145,   171,   415,   169,    78,   418,   419,   420,   421,
     422,   423,   424,   124,    56,   145,    89,   139,   164,    56,
     145,   522,   172,   435,   170,   147,   170,   172,   440,   667,
     668,   443,   907,   145,   145,   108,    82,   145,   149,    85,
     145,   114,   172,   139,   433,    78,   462,   172,   144,    27,
     462,   147,   464,   465,   145,   151,    89,   171,   470,   471,
     172,   172,    87,   475,   172,   477,   478,   172,   376,   377,
      48,    56,   484,   164,    52,   487,    48,   145,   409,   410,
     492,   114,    48,    56,   415,   145,   111,   418,   419,    61,
     462,    49,    56,    27,   506,    61,   164,    75,   510,   511,
      78,   145,    80,   515,   164,   743,   744,   496,    56,   145,
     522,    89,  1034,    71,   422,   423,   164,    88,     8,    91,
     164,    92,   170,    81,    82,    91,   538,   435,   164,   541,
     108,   543,   171,   634,   173,   547,   548,   549,   734,   551,
     552,  1027,   145,   515,    78,   117,   647,   164,  1034,   565,
     171,   117,   124,   565,   171,    89,   124,   465,   124,   145,
     557,   164,    56,   471,   145,    56,   538,   475,   557,   541,
     171,   543,   173,   145,   108,    99,   548,   145,   164,   145,
    1055,   149,  1057,   164,    88,     2,  1004,   171,    92,   173,
     228,   171,   164,   565,   172,    12,   234,   169,   164,   169,
     172,   171,    56,   169,   172,    56,   172,   164,    25,    56,
     541,    28,   543,   541,   171,   543,   173,   629,   630,   631,
     632,   633,   634,   635,   636,   779,   171,    56,   173,   139,
     638,   639,   169,   645,   144,   647,   171,   147,   173,   547,
      84,   151,    48,    56,   171,   657,   173,   659,   206,   164,
     163,   663,   164,   665,   155,    61,   757,   758,   164,  1145,
      77,   164,   164,   171,   765,   164,   678,   679,   164,   164,
      56,   169,   164,   645,   171,   118,    87,   164,   171,   237,
     238,   239,   240,   241,   242,    91,   171,   104,  1174,    61,
     248,   249,   109,   171,   171,   171,    32,   713,   124,   711,
     712,   713,  1188,   164,     8,   717,    65,   144,   147,   169,
      68,   117,   171,   145,   645,   816,   164,   124,   124,    91,
     278,    48,   630,   172,   632,   171,   164,   124,   124,    35,
     288,   172,   104,    35,   124,   145,   294,   172,   172,   145,
      21,   713,  1228,   755,   172,   117,    61,   759,   169,   761,
     762,   763,    88,   765,   312,   174,    92,   145,   164,  1245,
     174,   164,   171,   169,   181,   164,   172,   172,   145,   164,
     164,   164,   330,   331,   174,   164,    91,  1263,    35,   791,
     171,  1267,   164,   164,   164,    35,   717,    32,    65,   104,
     145,   170,   164,   805,   806,   807,   172,   169,   810,   172,
    1035,  1036,   117,   139,   816,    61,   172,   145,   144,   170,
     146,   147,   124,    89,    33,   151,   172,    33,   169,   169,
     174,   169,   834,   159,   836,    35,   144,   164,    89,   841,
      32,   167,   844,   174,   144,    91,   171,   849,   171,   851,
     174,   164,   174,    88,   164,   170,   858,    92,   104,   164,
     171,   147,   147,   147,   169,   139,   147,   147,   147,   139,
     147,   117,   279,    35,   147,   147,   139,   139,   285,   147,
      32,   139,   147,   889,   172,   172,   147,   889,   436,   172,
     869,   169,   104,   169,   169,   164,    88,   899,   139,   901,
      92,   139,   904,   163,   139,   907,   163,   805,   806,   144,
     164,   146,   147,   172,   164,   164,   151,   919,   164,   164,
     164,   469,   174,   169,   159,   473,   145,   889,   755,   172,
     165,   166,   167,  1077,   341,   490,    88,    24,  1066,   356,
      92,    89,   864,  1077,  1035,  1036,   893,   139,   140,   356,
     142,   143,   144,   920,   146,   147,   148,  1073,   869,   151,
     324,   699,   923,  1247,  1204,   967,   158,   159,    27,  1238,
     162,  1231,    37,   165,   166,   167,   507,    31,    -1,   981,
     982,    -1,   984,   985,   176,   987,   988,   139,   140,    48,
     142,   143,   144,    52,   146,   147,    -1,   999,  1000,   151,
    1002,    -1,  1004,    -1,  1006,  1007,   158,   159,  1010,    -1,
     162,  1064,    -1,   165,   166,   167,    75,    -1,    -1,    78,
      -1,    80,    -1,    -1,    -1,    -1,   433,    -1,    -1,    -1,
      89,  1033,  1034,  1035,  1036,    -1,    -1,    -1,    -1,  1041,
      -1,  1043,    -1,    -1,    -1,  1047,    -1,    27,    -1,   108,
      -1,  1053,  1054,  1055,  1056,  1057,    -1,   464,    -1,    -1,
      -1,    -1,    -1,   470,    -1,  1027,     2,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,   482,    12,   484,    -1,  1081,
    1082,  1083,  1084,  1085,  1086,  1087,  1088,    -1,    -1,    25,
      -1,    -1,    28,    -1,  1073,    75,    -1,    -1,    78,    -1,
      80,  1103,  1179,  1109,    -1,    -1,    -1,  1109,    -1,    89,
      -1,    -1,  1114,   661,   662,    -1,  1118,    -1,    -1,   667,
     668,    -1,   670,   671,    -1,     2,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,   685,   545,    -1,
      -1,    77,   549,    -1,    -1,    81,    82,  1109,    25,    -1,
     557,    28,  1114,    -1,    -1,  1232,  1233,  1234,  1235,  1236,
    1237,    -1,    -1,  1165,  1166,  1167,  1168,    -1,   104,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   730,    -1,    -1,    -1,  1188,    -1,    -1,    -1,
    1192,  1193,    -1,  1199,    -1,    -1,    -1,  1199,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,  1208,  1209,  1210,  1211,
    1212,  1213,  1174,    -1,    -1,    -1,    -1,    -1,    -1,   626,
       8,    -1,    -1,    -1,    -1,  1227,  1228,   104,  1230,    -1,
      -1,    -1,   109,    -1,    22,    -1,    -1,  1199,    26,    27,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,  1263,    -1,    51,    -1,  1267,  1228,    -1,    -1,   817,
     818,    -1,    -1,    -1,   681,    -1,    -1,   684,    -1,    -1,
      -1,    -1,    -1,   831,    -1,   833,    -1,    -1,    76,    -1,
      78,    -1,   840,    -1,    82,   843,    -1,    85,   846,    87,
      -1,    89,    -1,    -1,   181,  1267,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   865,    -1,    -1,
     108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   742,    -1,    -1,    -1,    -1,
      -1,   748,    -1,   279,     1,    -1,    -1,    -1,   896,   285,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     2,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,   779,    12,    31,    -1,   164,    -1,    -1,    36,
       2,    38,    39,    -1,    -1,    -1,    25,    44,    -1,    28,
      12,    -1,    -1,    -1,    51,    -1,    32,    54,   805,   806,
      -1,    -1,   279,    25,    -1,   341,    28,    -1,   285,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    76,
     356,    78,    -1,    80,    -1,    82,    83,   834,    85,   836,
      87,    -1,    89,    -1,   841,    -1,    -1,   844,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   855,    -1,
     857,   108,    88,   860,   111,    77,    92,   114,    -1,    -1,
      -1,    -1,    -1,    -1,   341,   104,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,
      -1,   888,   104,    32,    -1,    -1,    -1,   109,    -1,    -1,
      -1,  1039,   899,    -1,   901,    -1,    -1,   433,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,    -1,   164,   144,    -1,
     146,   147,    -1,   170,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,   464,   165,
     166,   167,    -1,    -1,   470,    -1,    -1,    -1,    -1,    88,
      32,    -1,   181,    92,    -1,    -1,   482,    -1,   484,  1097,
    1098,    -1,    -1,  1101,  1102,    -1,   433,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1127,
      -1,  1129,    -1,  1131,    -1,    -1,    -1,   464,    -1,    -1,
     139,   140,   999,   470,   143,   144,    88,   146,   147,  1006,
      92,    -1,   151,    -1,    -1,   482,    -1,   484,    -1,   545,
     159,    -1,  1019,   549,    -1,    -1,   165,   166,   167,    -1,
      -1,   557,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1037,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1046,
     279,    -1,    -1,  1050,    -1,    -1,   285,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,   279,    -1,   151,
     152,   153,    -1,   285,   156,   157,   158,   159,   545,    -1,
     162,    -1,   549,   165,   166,   167,    -1,    -1,    -1,    -1,
     557,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,  1104,    -1,  1247,
      -1,    -1,   341,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,     3,    -1,    -1,   356,    25,   341,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    34,    17,    -1,
      -1,    20,    -1,    40,   356,    -1,    25,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,   681,    53,    36,   684,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    46,  1165,  1166,
    1167,  1168,    69,    70,    53,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    63,    -1,    -1,    -1,    88,    86,
      -1,    70,    92,    -1,    -1,  1192,  1193,    -1,    95,    96,
      -1,    98,    -1,   100,   433,   102,    -1,    86,   105,    -1,
      -1,    -1,   109,   110,   681,   112,   742,   684,   115,    98,
      -1,   433,   748,   102,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,   464,   115,    -1,    -1,   139,
     140,   470,   142,   143,   144,    -1,   146,   147,    -1,    -1,
       2,   151,   464,   482,    -1,   484,    -1,    -1,   470,   159,
      12,    -1,    -1,    -1,    -1,   165,   166,   167,    -1,    -1,
     482,    -1,   484,    25,   171,   742,    28,    -1,    -1,    -1,
       2,   748,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    28,    -1,   834,    -1,
     836,    -1,    -1,    -1,    -1,   841,   545,    -1,   844,    -1,
     549,    -1,    -1,    -1,    -1,    77,    -1,    -1,   557,   855,
      -1,   857,    -1,   545,   860,     8,    -1,   549,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   557,    -1,    -1,    -1,    22,
      -1,    -1,   104,    26,    27,    77,    -1,   109,    -1,    -1,
      -1,    -1,   888,    36,    -1,    38,    39,   834,    -1,   836,
      -1,    44,    -1,   899,   841,   901,    -1,   844,    51,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,   109,   855,    -1,
     857,    -1,    65,   860,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    -1,    -1,    82,
      -1,    84,    85,    -1,    87,    -1,    89,    -1,    -1,    -1,
      -1,   888,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,   899,    -1,   901,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,   681,    26,    27,   684,    -1,    -1,    31,   181,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,   681,
      -1,    44,   684,   999,    -1,    -1,    -1,    -1,    51,    -1,
    1006,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,  1019,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    82,
      -1,  1037,    85,   742,    87,    -1,    89,    -1,    -1,   748,
    1046,    -1,    -1,    -1,  1050,    -1,    -1,   279,    -1,    -1,
     742,    -1,   999,   285,    -1,   108,   748,    -1,   111,  1006,
      -1,   114,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,  1019,    -1,    -1,    -1,    -1,   279,    36,    -1,
      38,    39,    -1,   285,    -1,    -1,    44,    -1,    -1,    47,
    1037,    -1,    -1,    51,    -1,    -1,    -1,    -1,  1104,  1046,
      -1,    -1,    -1,  1050,    -1,    -1,    -1,    65,    -1,   341,
      -1,   164,    -1,    -1,    -1,    -1,    -1,   170,    76,    -1,
      78,    -1,    -1,    -1,   356,   834,    84,   836,    -1,    87,
      -1,    89,   841,    -1,    -1,   844,    -1,    -1,    -1,   341,
      -1,    -1,   834,    -1,   836,    -1,   855,    -1,   857,   841,
     108,   860,   844,   111,   356,   113,   114,  1104,    -1,  1165,
    1166,  1167,  1168,   855,    -1,   857,    -1,    -1,   860,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   888,
      -1,    -1,    -1,    -1,    -1,    -1,  1192,  1193,    -1,    -1,
     899,    -1,   901,    -1,    -1,    -1,   888,    -1,    -1,    -1,
      22,   433,    -1,    -1,    26,    27,    -1,   899,    -1,   901,
      -1,    -1,    -1,     2,    36,    -1,    38,    39,  1165,  1166,
    1167,  1168,    44,    12,    -1,    -1,    -1,    -1,    -1,    51,
      -1,   433,   464,    -1,    -1,    -1,    25,    -1,   470,    28,
      -1,    -1,    -1,    -1,    -1,  1192,  1193,    -1,    -1,    -1,
     482,    -1,   484,    -1,    76,    -1,    78,    -1,    -1,    -1,
      82,    -1,   464,    85,    -1,    87,    -1,    89,   470,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     482,    -1,   484,    -1,    -1,    -1,   108,    -1,    77,   111,
     999,    -1,   114,    -1,    -1,    -1,    -1,  1006,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   999,    -1,    -1,
    1019,    -1,    -1,   545,  1006,   104,    -1,   549,    -1,    -1,
     109,    -1,    -1,     2,    -1,   557,    -1,  1019,  1037,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,  1046,    -1,    -1,
      -1,  1050,   164,   545,    -1,  1037,    25,   549,    -1,    28,
      -1,    -1,    -1,    -1,  1046,   557,    -1,    -1,  1050,    -1,
      -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    71,    -1,
      51,    -1,   181,    -1,    -1,  1104,    -1,     2,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,  1104,    -1,    -1,    76,    -1,    78,    -1,    80,
      25,    82,    -1,    28,    85,   104,    87,    -1,    89,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,   681,
     111,    -1,   684,   114,    -1,    -1,  1165,  1166,  1167,  1168,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,  1165,  1166,  1167,  1168,    -1,    -1,   681,
      -1,    -1,   684,  1192,  1193,    -1,    -1,    -1,    -1,    -1,
     279,    -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,   104,
    1192,  1193,   181,   164,   109,    -1,    -1,    -1,    -1,   170,
     742,    -1,    -1,    -1,    -1,    -1,   748,    -1,    -1,    88,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     742,    -1,    -1,    -1,    -1,    -1,   748,    -1,    -1,    -1,
      -1,    -1,   341,    -1,   237,   238,   239,   240,   241,   242,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,    -1,    -1,
     139,   140,    -1,   142,   143,   144,   181,   146,   147,   148,
      -1,   150,   151,   152,   153,    -1,    -1,   156,   157,   158,
     159,   160,    -1,   162,    -1,    -1,   165,   166,   167,    -1,
     279,    -1,   834,    -1,   836,   288,   285,   176,    -1,   841,
      -1,   294,   844,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   855,    -1,   857,    -1,    -1,   860,   312,
      -1,    -1,   834,    -1,   836,    -1,    -1,    -1,    -1,   841,
      -1,    -1,   844,    -1,   433,    -1,    -1,   330,   331,    -1,
      -1,    -1,    -1,   855,    -1,   857,   888,    -1,   860,    -1,
      -1,    -1,   341,    -1,    -1,    -1,    -1,   899,    -1,   901,
      -1,    -1,    -1,    -1,   279,   464,    -1,   356,    -1,    -1,
     285,   470,    -1,    -1,    -1,    -1,   888,    -1,    -1,    -1,
      -1,    -1,    -1,   482,    -1,   484,    -1,   899,    -1,   901,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   341,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    83,    -1,
      25,   356,    -1,   436,   433,    -1,   545,    -1,    -1,    34,
     549,    -1,    -1,    -1,    -1,    40,    -1,   999,   557,    -1,
      -1,    46,    -1,    -1,  1006,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,   464,   469,  1019,    63,    -1,
     473,   470,    -1,    -1,    69,    70,    -1,   999,    -1,    -1,
      -1,    -1,    77,   482,  1006,  1037,    -1,    -1,   143,    -1,
     145,    86,    -1,    -1,  1046,    -1,    -1,  1019,  1050,    -1,
      95,    96,    -1,    98,    -1,   100,    -1,   102,   433,    -1,
     105,    -1,    -1,    -1,   109,  1037,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,  1046,    -1,    -1,    -1,  1050,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,    -1,    -1,   464,
      -1,    -1,    -1,    -1,    -1,   470,   545,    -1,    -1,    -1,
     549,    -1,  1104,   208,    -1,    -1,    -1,   482,   557,   484,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   681,    -1,   169,   684,   171,   232,    -1,    -1,
     235,    -1,  1104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   246,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1165,  1166,  1167,  1168,    -1,    -1,    -1,
     545,    -1,    -1,    -1,   549,    -1,   281,    -1,    -1,    -1,
      -1,    -1,   557,   742,    -1,    -1,    -1,    -1,    -1,   748,
    1192,  1193,    -1,  1165,  1166,  1167,  1168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   661,   662,
      -1,    -1,    -1,    -1,   667,   668,    -1,   670,   671,    -1,
    1192,  1193,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,   681,    -1,    -1,   684,    -1,    -1,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    -1,    -1,   361,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   834,    -1,   836,    76,    -1,
      78,    -1,   841,    -1,    82,   844,    84,    85,    -1,    87,
      -1,    89,    -1,   742,    -1,    -1,   855,    -1,   857,   748,
      -1,   860,    -1,    -1,    -1,    -1,   681,    -1,    -1,   684,
     108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   432,    -1,   888,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     899,    -1,   901,    -1,    -1,    -1,   451,    -1,    -1,    -1,
      -1,   456,    -1,   458,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   817,   818,   164,   742,    -1,    -1,
      -1,    -1,    -1,   748,    -1,    -1,    -1,    -1,   831,    -1,
      -1,    -1,    -1,    -1,    -1,   834,    -1,   836,   493,    -1,
     843,    -1,   841,   846,   499,   844,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   855,    -1,   857,    -1,
      -1,   860,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,   888,
     999,    -1,    -1,    51,    -1,    -1,    -1,  1006,    -1,    -1,
     899,    -1,   901,   558,    -1,    -1,    -1,    -1,    -1,   834,
    1019,   836,   567,    -1,    -1,    -1,   841,    -1,    76,   844,
      78,    -1,    80,    -1,    82,    -1,    -1,    85,  1037,    87,
     855,    89,   857,    -1,    -1,   860,    -1,  1046,    -1,    -1,
      -1,  1050,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,   888,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   899,    -1,   901,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,   648,     3,  1104,   651,   652,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    17,    18,
      -1,    20,   170,    -1,    -1,    -1,    25,    -1,    -1,    -1,
    1019,   168,    -1,    -1,   171,    34,   173,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,  1039,    46,  1037,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,  1046,   703,    -1,
      -1,  1050,   707,    -1,    63,    -1,  1165,  1166,  1167,  1168,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,   999,   730,    -1,    86,    -1,   734,
      -1,  1006,    -1,  1192,  1193,    -1,    95,    96,    -1,    98,
      -1,   100,    -1,   102,  1019,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,  1104,   115,    -1,    -1,    -1,
      -1,    -1,  1037,    -1,   769,    -1,    -1,    -1,    -1,    -1,
      -1,  1046,     3,    -1,   779,  1050,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,   171,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,  1104,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,   862,    -1,   864,
      -1,    -1,    -1,   868,    95,    96,   871,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,   893,    -1,
    1165,  1166,  1167,  1168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   917,    -1,    -1,   920,    -1,  1192,  1193,     0,
       1,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
     171,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,  1077,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     1,   176,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,   169,   170,   171,    -1,   173,    -1,     1,   176,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,   170,   171,    -1,
     173,    -1,     1,   176,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    93,    94,    95,    96,    97,    98,
      99,   100,    -1,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
     169,   170,   171,    -1,   173,    -1,     1,   176,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,   170,   171,    -1,   173,    -1,
       1,   176,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,   170,
     171,    -1,   173,    -1,     1,   176,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,   170,   171,    -1,   173,    -1,     1,   176,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     1,   176,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    93,    94,    95,    96,    97,    98,
      99,   100,    -1,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     1,   176,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       1,   176,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     1,   176,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     1,   176,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    93,    94,    95,    96,    97,    98,
      99,   100,    -1,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     1,   176,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       1,   176,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     1,   176,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,   174,     1,   176,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    78,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,    -1,   176,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,   106,   107,    -1,   109,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,   172,   173,    -1,
      -1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,   172,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    48,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,   114,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    11,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,   108,   109,    -1,   111,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,   106,   107,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
     121,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,
     109,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,   106,
     107,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,   170,   171,    -1,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,   174,    -1,   176,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,   174,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     3,   176,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,   176,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,   174,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     3,   176,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     3,   176,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     3,   176,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    12,    -1,    -1,
      -1,   148,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    34,
     167,    36,    -1,    -1,   171,    40,   173,    -1,    -1,   176,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    -1,    98,    -1,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
      -1,   146,    -1,    -1,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,   162,    -1,    -1,
     165,   166,   167,   168,    -1,     3,   171,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   168,   162,   163,   171,    -1,   173,   167,
      -1,   169,    -1,   171,    -1,   173,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,   145,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,
      -1,    -1,   169,    -1,   171,   172,   173,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    61,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,   104,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,   163,   164,    25,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,   141,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,   163,    -1,    25,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,   163,    -1,    25,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,     3,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    12,    -1,    -1,    -1,    -1,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,   141,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,   171,    -1,   173,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    95,    96,    -1,    98,    -1,   100,
      19,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    32,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,    -1,
      -1,   132,   133,   134,   135,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,    -1,   146,    -1,    -1,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      -1,   162,    -1,    -1,   165,   166,   167,   168,    -1,    88,
      -1,    -1,    -1,    92,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   101,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,    -1,    -1,   132,   133,   134,   135,    -1,   137,   138,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    -1,    88,    -1,    -1,    -1,    92,   176,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   101,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,   132,   133,   134,   135,
      -1,   137,   138,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,
     176,    -1,    12,    -1,    -1,    -1,    -1,    -1,   101,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,    -1,   137,   138,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,
      -1,    -1,    92,   176,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   101,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,    -1,   137,   138,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    88,    -1,    -1,    -1,    92,   176,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   101,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,    -1,    -1,   132,   133,   134,   135,    -1,
     137,   138,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   101,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,   125,   126,   127,   128,   129,    -1,    -1,   132,   133,
     134,   135,    -1,   137,   138,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,
      -1,    92,   176,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     101,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,    -1,
      -1,   132,   133,   134,   135,    -1,   137,   138,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,
      88,    -1,    -1,    -1,    92,   176,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   101,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,   129,    -1,    -1,   132,   133,   134,   135,    -1,   137,
     138,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   101,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
     125,   126,   127,   128,   129,    -1,    48,   132,   133,   134,
     135,    -1,   137,   138,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,
      92,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,   119,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,   169,    88,    -1,
      -1,    -1,    92,    -1,   176,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    88,    -1,    -1,    -1,    92,   176,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    37,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    72,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,
      -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    72,   156,   157,   158,   159,   160,   161,   162,    -1,
     164,   165,   166,   167,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,    88,
      -1,    -1,    -1,    92,    -1,   176,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
     119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    -1,    88,    -1,    -1,    -1,    92,   176,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,
     176,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,
      -1,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    32,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,    88,
      -1,    -1,    -1,    92,    12,   176,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,   123,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    -1,    12,   176,    -1,    -1,
      -1,   139,   140,    19,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,    32,    -1,   156,   157,
     158,   159,   160,   161,   162,   123,    -1,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    88,    -1,    -1,    -1,    92,    -1,   176,    12,
      -1,    -1,    -1,    -1,    -1,   101,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,
     176,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,   119,    37,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    72,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    -1,    -1,    88,    -1,
      -1,   174,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,   176,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    22,    -1,
      -1,    -1,    26,    27,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,   139,   140,    -1,   142,   143,   144,    51,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    76,    88,    78,    -1,    -1,    92,   176,    12,
      -1,    -1,    -1,    87,    -1,    89,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,   108,    -1,    -1,   111,   123,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,   164,
     165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,    92,
      12,   176,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,   164,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,   164,   165,   166,   167,    -1,    88,    -1,
      -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    12,    13,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,   172,    92,    12,    -1,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,
     176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,
      13,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
     164,   165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,
      12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,
      -1,   172,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,   172,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,    12,    13,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,   174,    12,
     176,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,    92,
      12,   176,    -1,    -1,    -1,    -1,    -1,    19,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    12,    13,   176,    -1,    -1,    -1,    -1,    19,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,
      -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,   172,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,   174,    -1,   176,    -1,    12,
      -1,    -1,    -1,   101,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,   176,    92,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,
      -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,
      19,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    61,   156,   157,   158,
     159,   160,   161,   162,    12,    -1,   165,   166,   167,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,    -1,    -1,    88,    32,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      88,   146,   147,   148,    92,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   123,    -1,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    12,    -1,   165,   166,   167,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   176,    22,
      -1,    -1,    -1,    26,    27,    32,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    82,
      -1,    88,    85,    -1,    87,    92,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,   164,    -1,    -1,    -1,    -1,    22,   170,    -1,   176,
      26,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,
      -1,    87,    22,    89,    -1,    -1,    26,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,   108,    -1,    44,   111,    -1,    -1,   114,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    82,    32,    -1,    85,    -1,    87,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   164,    -1,
      26,    27,    -1,    -1,   170,    31,    -1,    -1,   108,    -1,
      36,   111,    38,    39,   114,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,
      -1,    87,    -1,    89,   164,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,    -1,    -1,   156,   157,   158,
     159,   160,    -1,   162,    -1,    -1,   165,   166,   167,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,   176,    31,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,   164,    -1,
      -1,    44,    -1,    -1,   170,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,     1,    78,     3,    80,    -1,    82,
      -1,    -1,    85,    -1,    87,    -1,    89,    -1,    -1,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,   108,    -1,    34,   111,    -1,
       3,   114,    -1,    40,    -1,     8,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    17,    18,    53,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    34,    69,    70,    -1,    -1,    -1,    40,    -1,    -1,
      77,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    86,
      53,   164,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      63,    98,    -1,   100,    -1,   102,    69,    70,   105,    -1,
      -1,    -1,   109,    -1,    77,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    86,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,
      17,    18,   105,    20,    -1,    -1,   109,    -1,    25,   112,
      -1,    -1,   115,    -1,    31,    -1,    -1,    34,    -1,    -1,
       3,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    17,    18,    53,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    34,    69,    70,    -1,    -1,    -1,    40,    -1,    -1,
      77,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    86,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      63,    98,    -1,   100,    -1,   102,    69,    70,   105,    -1,
      -1,    -1,   109,    -1,    77,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    86,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,
      17,    18,   105,    20,    -1,    -1,   109,    -1,    25,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    34,    -1,    -1,
       3,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    17,    18,    53,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    34,    69,    70,    -1,    -1,    -1,    40,    -1,    -1,
      77,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    86,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      63,    98,    -1,   100,    -1,   102,    69,    70,   105,    -1,
      -1,    -1,   109,    -1,    77,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    86,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,
      17,    18,   105,    20,    -1,    -1,   109,    -1,    25,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    34,    -1,    -1,
       3,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    17,    18,    53,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    34,    69,    70,    -1,    -1,    -1,    40,    -1,    -1,
      77,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    86,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      63,    98,    -1,   100,    -1,   102,    69,    70,   105,    -1,
      -1,    -1,   109,    -1,    77,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    86,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,
      17,    18,   105,    20,    -1,    -1,   109,    -1,    25,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    22,    69,    70,    -1,    26,    27,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,    86,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    95,    96,
      51,    98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    65,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    87,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   181,   182,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    33,    34,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      49,    50,    51,    53,    56,    57,    58,    59,    60,    62,
      63,    64,    66,    67,    69,    70,    71,    76,    77,    78,
      79,    80,    81,    82,    85,    86,    87,    89,    90,    91,
      93,    94,    95,    96,    97,    98,    99,   100,   102,   103,
     105,   106,   107,   108,   109,   111,   112,   114,   115,   118,
     120,   123,   139,   140,   141,   142,   143,   148,   158,   161,
     162,   163,   164,   167,   169,   171,   173,   176,   183,   184,
     185,   186,   187,   188,   189,   190,   193,   195,   201,   202,
     203,   206,   207,   211,   213,   216,   218,   220,   221,   222,
     229,   230,   232,   234,   237,   238,   239,   240,   241,   242,
     246,   247,   250,   251,   254,   255,   259,   262,   263,   289,
     292,   293,   313,   314,   315,   316,   317,   318,   319,   327,
     328,   329,   330,   331,   334,   335,   336,   337,   338,   339,
     340,   341,   343,   344,   345,   346,   347,   164,   185,   331,
     119,   320,   321,     3,   208,    14,    22,    36,    41,    42,
      45,    56,    87,   100,   169,   173,   237,   259,   313,   318,
     329,   330,   331,   334,   336,   337,   320,   331,   108,   292,
      89,   208,   185,   307,   331,     8,    22,    36,    39,    82,
      85,    87,   187,   185,   171,     8,    87,   331,   332,     8,
      11,    87,   108,   111,   332,    78,   121,   231,   331,   231,
     331,   231,   331,    26,   293,   331,    27,   114,   233,   331,
     194,   171,     3,    17,    18,    20,    25,    34,    40,    46,
      50,    53,    63,    69,    70,    77,    86,    95,    96,    98,
     100,   102,   105,   109,   112,   115,   210,   212,   210,   210,
     260,   171,   210,   294,   295,    33,   195,   215,   331,   331,
      18,    77,    95,   112,   331,   331,   331,     8,   171,   219,
     220,     4,   283,   306,   331,   106,   107,   164,   331,   333,
     331,   215,   331,   331,   331,    99,   171,   185,   331,   331,
     195,   207,   331,   334,   195,   207,   331,   210,   290,   331,
     233,   331,   331,   331,   331,   331,   331,   331,     1,   170,
     183,   196,   306,   110,   149,   283,   308,   309,   333,   231,
     306,   331,   342,   331,    80,   185,   169,    84,   191,    47,
     113,    56,   210,   210,    54,    73,    83,   278,   294,   163,
     164,   155,   331,    12,    19,    32,    88,    92,   123,   139,
     140,   142,   143,   144,   146,   147,   148,   150,   151,   152,
     153,   154,   156,   157,   158,   159,   160,   161,   162,   165,
     166,   167,   176,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   168,   269,
     171,   173,    88,    92,   171,   185,   164,   331,   331,   331,
     210,   306,    56,   169,   195,    48,   320,   290,   294,   164,
     145,   164,   187,   118,   211,   283,   310,   311,   312,   333,
      87,   227,   263,   292,    87,   111,   223,   290,   225,   263,
     292,   210,   171,   215,    33,    48,   215,   119,   215,   323,
      33,    48,   215,   323,   215,    48,   215,    37,    72,   164,
     210,   210,   101,   195,   101,   124,   195,   269,    82,    85,
     192,   310,   171,   171,   195,   185,   171,   271,   110,   171,
     210,   296,   297,     1,   144,   301,    48,   145,   185,   215,
     171,   171,   215,   310,   220,   145,   164,   331,   331,   164,
     169,   215,   171,   310,   164,   243,   164,   243,   164,   124,
     291,   164,   215,   215,   164,   170,   170,   183,   145,   170,
     331,   145,   172,   145,   172,   174,   323,    48,   145,   174,
     323,   122,   145,   174,     8,     1,   170,   196,    65,   204,
     205,   331,   198,   331,   210,   144,   252,   169,   264,   164,
     331,   331,   331,   331,   231,   331,   231,   331,   331,   331,
     331,   331,   331,   331,     3,    20,    34,    63,   102,   108,
     211,   331,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,    68,   333,
     333,   333,   333,   333,   310,   310,   231,   331,   231,   331,
      27,    48,    89,   114,   322,   325,   326,   331,   347,    33,
      48,    33,    48,   101,   171,    48,   174,   210,   231,   331,
     215,   164,   164,   331,   331,   124,   172,   145,   228,   210,
     294,   224,   226,   210,   164,   210,   294,    48,   306,    45,
     331,   231,   331,   171,   215,    45,   331,   231,   331,   215,
     231,   331,   167,   197,   200,   331,   197,   199,   124,   124,
     185,    35,   185,   331,    35,   331,   191,   172,   311,   210,
     235,   236,    27,    48,    52,    75,    78,    89,   108,   184,
     272,   273,   274,   275,   276,   261,   297,   145,   172,    34,
      50,    96,   100,   173,   214,   302,   314,   124,   298,   331,
     295,   331,   331,   172,   283,   331,     1,   248,   312,   172,
      21,   244,   302,   314,   145,   170,   172,   172,   308,   172,
     308,   185,   174,   231,   331,   174,   185,   331,   174,   331,
     174,   331,   170,   170,   210,   145,   164,    13,   147,   145,
     164,    13,    37,    72,   164,   171,   306,   169,     1,    31,
     210,   256,   257,   258,    27,    78,    89,   108,   266,   277,
     171,   164,   164,   164,   164,   164,   164,   172,   174,    48,
      89,   145,   172,    17,    20,    25,    46,    53,    63,    70,
      86,    98,   109,   115,   313,    88,    88,    45,   231,   331,
      45,   231,   331,   311,   231,   331,   171,   320,   320,   164,
     283,   333,   312,   210,   252,   164,   210,   210,   252,   252,
     164,   331,   172,   331,    33,   215,    33,   215,   324,   325,
     331,    33,   215,   323,    33,   215,   323,   215,   215,   145,
     164,    13,   164,   331,   331,    35,   185,    35,    35,   185,
     101,   195,    65,   172,   145,   172,    48,    89,   275,   145,
     172,   171,   210,    27,    78,    89,   108,   279,   172,   297,
     301,     1,   306,    68,   333,   172,   172,   170,    74,   116,
     170,   249,   172,   171,   195,   210,   245,   290,   185,   174,
     323,   174,   323,   185,   122,   204,   211,   169,   331,   110,
     331,   200,   199,   311,     1,   253,   170,     8,   258,   124,
     145,   170,    89,   265,     1,     3,    12,    17,    19,    20,
      25,    40,    46,    53,    55,    63,    69,    70,    86,    98,
     102,   105,   109,   115,   139,   140,   141,   142,   143,   144,
     146,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   162,   165,   166,   167,   168,   171,   209,   210,
     212,   267,   268,   269,   313,   172,   325,   301,   313,   313,
     331,    33,    33,   331,    33,    33,   172,   174,   174,   311,
     215,   215,   252,   169,   252,   252,   169,   169,   215,   101,
      45,   331,    45,   331,   145,   172,   101,    45,   331,   215,
      45,   331,   215,   331,   331,   185,   185,   331,   185,    35,
     210,   164,   236,   195,   210,   274,   297,   144,   305,    89,
     301,   298,   174,    48,   174,   171,   171,    33,   185,   306,
     245,   144,   195,    45,   185,   331,   174,    45,   185,   331,
     174,   331,   197,    13,    37,    72,    37,    72,   164,   164,
     172,   170,    31,    82,    85,   170,   184,   217,   218,   258,
     331,   257,   279,   171,   270,   331,   139,   147,   270,   270,
     298,   101,    45,    45,   101,    45,    45,    45,    45,   172,
     169,   253,   169,   169,   253,   253,   331,   331,   331,   325,
     331,   331,   331,    13,    35,   185,   164,   305,   172,   173,
     214,   283,   304,   314,   149,   284,   298,    61,   117,   285,
     331,   302,   314,   310,   310,   185,   215,   172,   331,   331,
     185,   331,   185,   170,   110,   331,   200,   199,   200,   199,
     164,     8,   218,   217,     1,   144,   300,   273,   172,     3,
     102,   268,   269,   331,   331,   331,   331,   331,   331,   253,
     170,   253,   253,   170,   170,   101,   101,   101,   101,   331,
     185,   284,   298,   305,   174,   306,   283,   331,     3,    91,
     102,   286,   287,   288,   331,   195,   216,   282,   174,   172,
     172,   195,   101,   101,   164,   164,   164,   164,   218,   173,
     214,   299,   314,   104,   280,   172,   270,   270,   101,   101,
     101,   101,   101,   101,   170,   170,   170,   331,   331,   331,
     331,   284,   298,   283,   303,   304,   314,    48,   174,   288,
     117,   145,   124,   150,   152,   153,   156,   157,    61,   314,
     163,   163,   331,   331,     1,   174,   306,   285,   331,   303,
     304,   331,   287,   288,   288,   288,   288,   288,   288,   286,
     174,   299,   314,   174,   164,   281,   282,   174,   299,   314,
     303
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   180,   181,   182,   182,   183,   183,   184,   184,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   186,   186,   186,   187,   187,   188,   189,   189,
     189,   190,   190,   190,   191,   191,   192,   192,   192,   194,
     193,   195,   195,   195,   196,   196,   197,   197,   197,   197,
     198,   198,   198,   198,   198,   198,   199,   199,   200,   200,
     201,   201,   201,   202,   202,   202,   202,   202,   202,   202,
     203,   204,   204,   204,   205,   205,   206,   207,   207,   207,
     207,   207,   207,   208,   208,   209,   209,   209,   209,   209,
     209,   210,   210,   210,   210,   210,   210,   211,   211,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     214,   214,   214,   214,   215,   215,   216,   216,   217,   217,
     217,   218,   218,   218,   219,   218,   220,   220,   220,   220,
     220,   220,   220,   221,   221,   221,   221,   223,   222,   224,
     222,   225,   222,   226,   222,   227,   222,   228,   222,   222,
     222,   222,   222,   229,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   231,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   233,   233,
     234,   234,   235,   235,   236,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   238,   238,
     238,   238,   238,   238,   239,   239,   239,   240,   240,   240,
     241,   242,   242,   242,   242,   242,   242,   243,   243,   244,
     244,   244,   245,   245,   246,   247,   247,   248,   248,   249,
     249,   249,   250,   250,   251,   251,   251,   252,   252,   253,
     253,   253,   254,   254,   255,   256,   256,   256,   257,   257,
     257,   258,   258,   260,   261,   259,   262,   262,   262,   264,
     265,   263,   266,   266,   266,   266,   266,   267,   267,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   270,
     270,   271,   272,   272,   273,   273,   274,   274,   274,   274,
     274,   274,   275,   275,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   277,   277,   277,   277,   277,   277,   278,
     278,   278,   279,   279,   279,   279,   279,   279,   280,   280,
     281,   281,   282,   282,   283,   284,   284,   284,   285,   285,
     285,   285,   285,   286,   286,   287,   287,   287,   287,   287,
     287,   287,   288,   288,   289,   289,   289,   290,   290,   291,
     291,   291,   292,   292,   292,   292,   292,   293,   293,   294,
     294,   295,   295,   296,   296,   296,   297,   297,   297,   298,
     298,   298,   299,   299,   299,   299,   299,   299,   299,   300,
     300,   300,   300,   300,   301,   301,   301,   301,   301,   302,
     302,   302,   302,   303,   303,   303,   304,   304,   304,   304,
     304,   305,   305,   305,   305,   305,   306,   306,   306,   306,
     307,   307,   308,   308,   308,   309,   309,   310,   310,   311,
     311,   312,   312,   312,   312,   313,   313,   314,   314,   314,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   317,
     318,   319,   319,   319,   319,   319,   319,   319,   319,   320,
     320,   321,   322,   322,   323,   324,   324,   325,   325,   325,
     326,   326,   326,   326,   326,   326,   327,   327,   328,   328,
     328,   328,   328,   329,   329,   329,   329,   329,   330,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   332,   332,   333,   333,
     333,   334,   334,   334,   334,   335,   335,   335,   335,   335,
     336,   336,   336,   337,   337,   337,   337,   337,   338,   338,
     338,   338,   339,   339,   340,   340,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   342,
     342,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   344,   344,   344,   344,   344,   344,
     344,   345,   345,   345,   345,   346,   346,   346,   346,   347,
     347,   347,   347,   347,   347,   347
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     2,     3,     3,     2,     2,
       3,     2,     1,     3,     2,     1,     1,     4,     3,     4,
       4,     0,     1,     1,     0,     1,     0,     1,     1,     0,
       7,     2,     3,     3,     1,     2,     1,     3,     3,     5,
       1,     3,     3,     3,     5,     5,     0,     1,     1,     1,
       0,     1,     1,     4,     6,     8,     8,     6,     8,     8,
       4,     1,     3,     5,     1,     3,     3,     4,     4,     4,
       4,     4,     4,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     3,     1,     3,
       2,     1,     1,     2,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     5,     5,     2,     0,     8,     0,
       9,     0,     8,     0,     9,     0,     8,     0,     9,     3,
       3,     5,     5,     2,     5,     3,     3,     6,     6,     4,
       5,     5,     3,     3,     6,     5,     6,     5,     6,     3,
       4,     3,     4,     5,     5,     3,     3,     6,     7,     6,
       7,     4,     5,     4,     5,     4,     4,     3,     6,     5,
       4,     3,     6,     5,     6,     5,     8,     7,     4,     4,
       6,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     6,     4,     7,     5,     3,     6,
       2,     3,     3,     2,     2,     3,     3,     0,     2,     2,
       3,     5,     1,     3,     3,     5,     5,     0,     2,     3,
       2,     3,     6,     6,     1,     1,     1,     0,     2,     0,
       2,     3,     5,     5,     1,     1,     2,     3,     1,     3,
       2,     1,     3,     0,     0,     8,     0,     1,     1,     0,
       0,    10,     3,     3,     5,     5,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     3,     1,     3,     0,     1,     4,     5,     4,     5,
       6,     6,     0,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     0,     1,     2,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     0,     2,
       2,     4,     4,     1,     3,     3,     3,     3,     3,     3,
       3,     2,     1,     1,     3,     4,     4,     2,     4,     0,
       2,     2,     1,     1,     1,     2,     1,     4,     3,     1,
       3,     3,     5,     1,     1,     3,     1,     2,     3,     0,
       2,     2,     3,     2,     4,     3,     3,     4,     3,     0,
       2,     2,     2,     1,     0,     2,     2,     2,     1,     4,
       4,     6,     3,     0,     1,     1,     3,     4,     3,     4,
       6,     0,     2,     2,     2,     2,     1,     1,     3,     3,
       1,     3,     1,     1,     1,     3,     3,     0,     1,     1,
       3,     3,     3,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     2,     4,     4,     4,     5,
       2,     2,     1,     2,     1,     2,     1,     2,     1,     2,
       1,     1,     6,     6,     4,     9,     9,     7,     6,     6,
       4,     9,     9,     7,     4,     6,     6,     9,     9,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     4,     1,     3,     4,     1,     3,     4,     3,     3,
       1,     1,     2,     1,     2,     1,     1,     3,     1,     2,
       2,     2,     2,     2,     8,     8,     9,     9,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     3,
       3,     3,     2,     2,     2,     1,     0,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       4,     4,     4,     3,     3,     3,     3,     5,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     3,     4,     3,     4,     3,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
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
#line 517 "chapel.ypp" /* yacc.c:1663  */
    { yyblock = (yyval.pblockstmt); }
#line 6754 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 3:
#line 522 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6760 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 4:
#line 523 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6766 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 6:
#line 530 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6772 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 7:
#line 535 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6778 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 8:
#line 536 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6784 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 26:
#line 558 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6790 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 27:
#line 559 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6796 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 28:
#line 560 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6802 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 29:
#line 561 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6808 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 30:
#line 562 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6814 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 31:
#line 563 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6820 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 32:
#line 564 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6826 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 33:
#line 565 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6832 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 34:
#line 566 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6838 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 35:
#line 567 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6844 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 36:
#line 568 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6850 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 37:
#line 569 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6856 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 38:
#line 570 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6862 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 39:
#line 571 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6868 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 40:
#line 572 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6874 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 41:
#line 573 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6880 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 43:
#line 579 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 6886 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 44:
#line 581 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 6892 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 47:
#line 591 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 6906 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 48:
#line 604 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6915 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 49:
#line 609 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6924 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 50:
#line 614 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6933 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 51:
#line 621 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6939 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 52:
#line 622 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6945 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 53:
#line 623 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6951 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 54:
#line 627 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6957 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 55:
#line 628 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 6963 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 56:
#line 632 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6969 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 57:
#line 633 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6975 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 58:
#line 634 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6981 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 59:
#line 639 "chapel.ypp" /* yacc.c:1663  */
    {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6990 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 60:
#line 644 "chapel.ypp" /* yacc.c:1663  */
    {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6998 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 61:
#line 659 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 7004 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 62:
#line 660 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 7010 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 63:
#line 661 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 7016 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 64:
#line 666 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 7022 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 65:
#line 667 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 7028 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 66:
#line 672 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 7038 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 67:
#line 677 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 7048 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 68:
#line 682 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 7057 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 69:
#line 686 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7066 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 70:
#line 695 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 7076 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 71:
#line 700 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 7086 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 72:
#line 705 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 7096 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 73:
#line 710 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 7105 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 74:
#line 714 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7114 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 75:
#line 718 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7123 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 76:
#line 726 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 7133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 78:
#line 735 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 7143 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 80:
#line 744 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 7149 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 81:
#line 745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 7155 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 82:
#line 746 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 7161 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 83:
#line 750 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 7167 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 84:
#line 751 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 7173 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 85:
#line 752 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7179 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 86:
#line 753 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7185 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 87:
#line 754 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 7191 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 88:
#line 755 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7197 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 89:
#line 756 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7203 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 90:
#line 760 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 7210 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 91:
#line 765 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 7216 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 92:
#line 766 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7222 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 93:
#line 767 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 7228 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 94:
#line 771 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 7234 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 95:
#line 772 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 7240 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 96:
#line 776 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 7246 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 97:
#line 781 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 7252 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 98:
#line 783 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 7258 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 99:
#line 785 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 7264 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 100:
#line 787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 7270 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 101:
#line 789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 7276 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 102:
#line 791 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 7282 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 103:
#line 797 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 7288 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 104:
#line 798 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7294 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 105:
#line 802 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7300 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 106:
#line 803 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7306 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 107:
#line 804 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 7312 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 108:
#line 805 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7318 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 109:
#line 806 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7324 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 110:
#line 807 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7330 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 111:
#line 810 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7336 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 112:
#line 811 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7342 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 113:
#line 812 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7348 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 114:
#line 813 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7354 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 115:
#line 814 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7360 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 116:
#line 815 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7366 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 117:
#line 827 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7372 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 118:
#line 828 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 7378 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 119:
#line 839 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bool"; }
#line 7384 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 120:
#line 840 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "int"; }
#line 7390 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 121:
#line 841 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "uint"; }
#line 7396 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 122:
#line 842 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "real"; }
#line 7402 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 123:
#line 843 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "imag"; }
#line 7408 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 124:
#line 844 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "complex"; }
#line 7414 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 125:
#line 845 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bytes"; }
#line 7420 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 126:
#line 846 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "string"; }
#line 7426 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 127:
#line 847 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "sync"; }
#line 7432 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 128:
#line 848 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "single"; }
#line 7438 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 129:
#line 849 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "owned"; }
#line 7444 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 130:
#line 850 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "shared"; }
#line 7450 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 131:
#line 851 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "borrowed"; }
#line 7456 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 132:
#line 852 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "unmanaged"; }
#line 7462 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 133:
#line 853 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "domain"; }
#line 7468 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 134:
#line 854 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "index"; }
#line 7474 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 135:
#line 855 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "locale"; }
#line 7480 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 136:
#line 856 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "nothing"; }
#line 7486 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 137:
#line 857 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "void"; }
#line 7492 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 138:
#line 861 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7498 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 139:
#line 862 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7504 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 140:
#line 863 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7510 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 141:
#line 864 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7516 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 142:
#line 865 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7522 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 143:
#line 866 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7528 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 144:
#line 867 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7534 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 145:
#line 868 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7540 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 146:
#line 869 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7546 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 147:
#line 870 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7552 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 148:
#line 871 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7558 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 149:
#line 872 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7564 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 150:
#line 879 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_syncvar"; }
#line 7570 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 151:
#line 880 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_singlevar"; }
#line 7576 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 152:
#line 881 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_domain"; }
#line 7582 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 153:
#line 882 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_index"; }
#line 7588 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 154:
#line 886 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7594 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 155:
#line 887 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7600 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 156:
#line 891 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7606 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 157:
#line 892 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7612 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 159:
#line 898 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 7618 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 160:
#line 900 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 7624 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 161:
#line 904 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7630 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 163:
#line 906 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7636 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 164:
#line 916 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7642 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 165:
#line 917 "chapel.ypp" /* yacc.c:1663  */
    { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7648 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 173:
#line 931 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7654 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 174:
#line 932 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7660 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 175:
#line 933 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7666 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 176:
#line 934 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7672 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 177:
#line 939 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7681 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 178:
#line 944 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7695 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 179:
#line 955 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7704 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 180:
#line 960 "chapel.ypp" /* yacc.c:1663  */
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7719 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 181:
#line 972 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7728 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 182:
#line 977 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7742 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 183:
#line 988 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7751 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 184:
#line 993 "chapel.ypp" /* yacc.c:1663  */
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7766 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 185:
#line 1005 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7775 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 186:
#line 1010 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7789 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 187:
#line 1020 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7798 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 188:
#line 1025 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7812 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 189:
#line 1036 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7820 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 190:
#line 1040 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7828 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 191:
#line 1045 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7840 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 192:
#line 1053 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7852 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 193:
#line 1064 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7860 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 194:
#line 1070 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7866 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 195:
#line 1071 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7872 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 196:
#line 1072 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7878 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 197:
#line 1073 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7884 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 198:
#line 1074 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7890 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 199:
#line 1075 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7896 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 200:
#line 1076 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7902 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 201:
#line 1077 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7908 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 202:
#line 1078 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7914 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 203:
#line 1079 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7920 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 204:
#line 1080 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7926 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 205:
#line 1081 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7932 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 206:
#line 1082 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7938 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 207:
#line 1083 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7944 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 208:
#line 1084 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7950 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 209:
#line 1085 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7956 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 210:
#line 1086 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7962 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 211:
#line 1087 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7968 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 212:
#line 1088 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7974 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 213:
#line 1089 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7980 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 214:
#line 1090 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7986 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 215:
#line 1091 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7992 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 216:
#line 1092 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7998 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 217:
#line 1094 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 8008 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 218:
#line 1100 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 8018 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 219:
#line 1106 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8028 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 220:
#line 1112 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8038 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 221:
#line 1118 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 8049 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 222:
#line 1125 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 8060 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 223:
#line 1132 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8068 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 224:
#line 1136 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8076 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 225:
#line 1142 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 8082 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 226:
#line 1146 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 8088 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 227:
#line 1147 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8094 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 228:
#line 1148 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8100 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 229:
#line 1149 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8106 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 230:
#line 1151 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 8112 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 231:
#line 1152 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8118 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 232:
#line 1153 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8124 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 233:
#line 1154 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8130 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 234:
#line 1156 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8137 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 235:
#line 1158 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8144 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 236:
#line 1160 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8151 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 237:
#line 1162 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8158 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 238:
#line 1167 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), false); }
#line 8164 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 239:
#line 1168 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), true);  }
#line 8170 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 240:
#line 1173 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), (yyvsp[0].pblockstmt))); }
#line 8176 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 241:
#line 1175 "chapel.ypp" /* yacc.c:1663  */
    { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-1].pch), ls, (yyvsp[0].pblockstmt))); }
#line 8185 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 242:
#line 1182 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8191 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 243:
#line 1183 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8197 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 244:
#line 1188 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[0].pch), INTENT_TYPE); }
#line 8203 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 245:
#line 1193 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 8209 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 246:
#line 1194 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bool"; }
#line 8215 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 247:
#line 1195 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "int"; }
#line 8221 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 248:
#line 1196 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "uint"; }
#line 8227 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 249:
#line 1197 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "real"; }
#line 8233 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 250:
#line 1198 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "imag"; }
#line 8239 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 251:
#line 1199 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "complex"; }
#line 8245 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 252:
#line 1200 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bytes"; }
#line 8251 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 253:
#line 1201 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "string"; }
#line 8257 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 254:
#line 1202 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "locale"; }
#line 8263 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 255:
#line 1203 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "nothing"; }
#line 8269 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 256:
#line 1204 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "void"; }
#line 8275 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 257:
#line 1206 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[0].pch)); }
#line 8282 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 258:
#line 1212 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; }
#line 8288 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 259:
#line 1213 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 8294 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 260:
#line 1214 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; }
#line 8300 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 261:
#line 1215 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; }
#line 8306 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 262:
#line 1224 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "domain"; }
#line 8312 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 263:
#line 1225 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "index"; }
#line 8318 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 264:
#line 1230 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8324 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 265:
#line 1232 "chapel.ypp" /* yacc.c:1663  */
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-3].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-1].pch), act, NULL)); }
#line 8331 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 266:
#line 1235 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-6].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8338 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 267:
#line 1241 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8344 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 268:
#line 1243 "chapel.ypp" /* yacc.c:1663  */
    { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-2].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[0].pch), act); }
#line 8351 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 269:
#line 1246 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-5].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8358 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 270:
#line 1251 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 8364 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 271:
#line 1254 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 8370 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 272:
#line 1255 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 8376 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 273:
#line 1256 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 8382 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 274:
#line 1257 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 8388 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 275:
#line 1258 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8394 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 276:
#line 1259 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8400 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 277:
#line 1263 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8406 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 278:
#line 1264 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8412 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 279:
#line 1268 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 8418 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 280:
#line 1269 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8424 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 281:
#line 1270 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8430 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 282:
#line 1274 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 8436 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 283:
#line 1275 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 8442 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 284:
#line 1279 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 8448 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 285:
#line 1283 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 8454 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 286:
#line 1285 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 8460 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 287:
#line 1289 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8466 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 288:
#line 1290 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8472 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 289:
#line 1295 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 8478 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 290:
#line 1297 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8484 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 291:
#line 1299 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8490 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 292:
#line 1306 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8504 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 293:
#line 1316 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8518 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 294:
#line 1329 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8528 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 295:
#line 1334 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8538 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 296:
#line 1339 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8548 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 297:
#line 1347 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 8554 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 298:
#line 1348 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 8560 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 299:
#line 1353 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 8566 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 300:
#line 1355 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 8572 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 301:
#line 1357 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 8578 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 302:
#line 1362 "chapel.ypp" /* yacc.c:1663  */
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
#line 8598 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 303:
#line 1378 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 8606 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 304:
#line 1385 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8616 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 305:
#line 1394 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8628 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 306:
#line 1402 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8636 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 307:
#line 1406 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8644 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 309:
#line 1414 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr), (yyvsp[-1].pch)); }
#line 8650 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 310:
#line 1416 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr)); }
#line 8656 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 311:
#line 1420 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8662 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 312:
#line 1421 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8668 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 313:
#line 1426 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8677 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 314:
#line 1431 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8686 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 315:
#line 1436 "chapel.ypp" /* yacc.c:1663  */
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
#line 8708 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 316:
#line 1458 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8716 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 317:
#line 1461 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8725 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 318:
#line 1465 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8734 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 319:
#line 1473 "chapel.ypp" /* yacc.c:1663  */
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8748 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 320:
#line 1483 "chapel.ypp" /* yacc.c:1663  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8759 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 321:
#line 1490 "chapel.ypp" /* yacc.c:1663  */
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
#line 8793 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 322:
#line 1523 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8801 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 323:
#line 1527 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8810 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 324:
#line 1532 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8818 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 325:
#line 1536 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8827 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 326:
#line 1541 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8835 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 328:
#line 1548 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8841 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 329:
#line 1552 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 8847 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 330:
#line 1553 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&"; }
#line 8853 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 331:
#line 1554 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|"; }
#line 8859 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 332:
#line 1555 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^"; }
#line 8865 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 333:
#line 1556 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "~"; }
#line 8871 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 334:
#line 1557 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "=="; }
#line 8877 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 335:
#line 1558 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!="; }
#line 8883 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 336:
#line 1559 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<="; }
#line 8889 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 337:
#line 1560 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">="; }
#line 8895 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 338:
#line 1561 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<"; }
#line 8901 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 339:
#line 1562 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">"; }
#line 8907 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 340:
#line 1563 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+"; }
#line 8913 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 341:
#line 1564 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-"; }
#line 8919 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 342:
#line 1565 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*"; }
#line 8925 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 343:
#line 1566 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/"; }
#line 8931 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 344:
#line 1567 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<"; }
#line 8937 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 345:
#line 1568 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>"; }
#line 8943 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 346:
#line 1569 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%"; }
#line 8949 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 347:
#line 1570 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**"; }
#line 8955 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 348:
#line 1571 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!"; }
#line 8961 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 349:
#line 1572 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_by"; }
#line 8967 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 350:
#line 1573 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "#"; }
#line 8973 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 351:
#line 1574 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_align"; }
#line 8979 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 352:
#line 1575 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<=>"; }
#line 8985 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 353:
#line 1576 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<~>"; }
#line 8991 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 354:
#line 1577 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "init="; }
#line 8997 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 355:
#line 1578 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ":"; }
#line 9003 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 356:
#line 1579 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 9009 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 357:
#line 1583 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "="; }
#line 9015 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 358:
#line 1584 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+="; }
#line 9021 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 359:
#line 1585 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-="; }
#line 9027 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 360:
#line 1586 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*="; }
#line 9033 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 361:
#line 1587 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/="; }
#line 9039 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 362:
#line 1588 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%="; }
#line 9045 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 363:
#line 1589 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**="; }
#line 9051 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 364:
#line 1590 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&="; }
#line 9057 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 365:
#line 1591 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|="; }
#line 9063 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 366:
#line 1592 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^="; }
#line 9069 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 367:
#line 1593 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>="; }
#line 9075 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 368:
#line 1594 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<="; }
#line 9081 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 369:
#line 1598 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 9087 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 370:
#line 1599 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 9093 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 371:
#line 1603 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 9099 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 372:
#line 1607 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 9105 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 373:
#line 1608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 9111 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 374:
#line 1612 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 9117 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 375:
#line 1613 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[0].pfnsymbol); }
#line 9123 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 376:
#line 1618 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 9129 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 377:
#line 1620 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 9135 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 378:
#line 1622 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 9141 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 379:
#line 1624 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 9147 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 380:
#line 1626 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9153 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 381:
#line 1628 "chapel.ypp" /* yacc.c:1663  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 9159 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 382:
#line 1632 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 9165 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 383:
#line 1633 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 9171 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 384:
#line 1637 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_IN; }
#line 9177 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 385:
#line 1638 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_INOUT; }
#line 9183 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 386:
#line 1639 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_OUT; }
#line 9189 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 387:
#line 1640 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST; }
#line 9195 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 388:
#line 1641 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 9201 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 389:
#line 1642 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 9207 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 390:
#line 1643 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 9213 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 391:
#line 1644 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF; }
#line 9219 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 392:
#line 1645 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE; }
#line 9225 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 393:
#line 1649 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 9231 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 394:
#line 1650 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 9237 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 395:
#line 1651 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF;   }
#line 9243 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 396:
#line 1652 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 9249 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 397:
#line 1653 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST;   }
#line 9255 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 398:
#line 1654 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 9261 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 399:
#line 1658 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIterOp) = ProcIterOp_PROC; }
#line 9267 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 400:
#line 1659 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIterOp) = ProcIterOp_ITER; }
#line 9273 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 401:
#line 1660 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIterOp) = ProcIterOp_OP; }
#line 9279 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 402:
#line 1664 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 9285 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 403:
#line 1665 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 9291 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 404:
#line 1666 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 9297 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 405:
#line 1667 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_REF; }
#line 9303 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 406:
#line 1668 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_PARAM; }
#line 9309 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 407:
#line 1669 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_TYPE; }
#line 9315 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 408:
#line 1673 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 9321 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 409:
#line 1674 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 9327 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 410:
#line 1677 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = NULL; }
#line 9333 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 413:
#line 1683 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 9339 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 414:
#line 1687 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 9345 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 415:
#line 1691 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9351 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 416:
#line 1692 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9357 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 417:
#line 1693 "chapel.ypp" /* yacc.c:1663  */
    { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 9367 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 418:
#line 1701 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 9373 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 419:
#line 1703 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 9379 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 420:
#line 1705 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 9385 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 421:
#line 1707 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9391 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 422:
#line 1709 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9397 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 423:
#line 1714 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9403 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 424:
#line 1716 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9409 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 425:
#line 1720 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9415 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 426:
#line 1721 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9421 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 427:
#line 1722 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9427 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 428:
#line 1723 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9433 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 429:
#line 1724 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9439 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 430:
#line 1725 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9445 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 431:
#line 1726 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 9451 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 432:
#line 1731 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 9457 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 433:
#line 1733 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 9463 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 434:
#line 1738 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 9469 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 435:
#line 1740 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 9475 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 436:
#line 1742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 9481 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 437:
#line 1747 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 9498 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 438:
#line 1760 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 9516 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 439:
#line 1776 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9522 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 440:
#line 1778 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9528 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 441:
#line 1780 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 9534 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 442:
#line 1784 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 9540 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 443:
#line 1785 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 9546 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 444:
#line 1786 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 9552 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 445:
#line 1787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 9558 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 446:
#line 1788 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 9564 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 447:
#line 1793 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9574 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 448:
#line 1799 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9583 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 450:
#line 1808 "chapel.ypp" /* yacc.c:1663  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 9592 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 451:
#line 1816 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 9598 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 452:
#line 1818 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9604 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 453:
#line 1823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 9610 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 454:
#line 1825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 9616 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 455:
#line 1827 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 9622 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 456:
#line 1832 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 9628 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 457:
#line 1834 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 9634 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 458:
#line 1836 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 9640 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 459:
#line 1842 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9646 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 460:
#line 1843 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9652 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 461:
#line 1844 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9658 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 462:
#line 1850 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9664 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 463:
#line 1852 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9670 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 464:
#line 1854 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9678 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 465:
#line 1858 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9686 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 466:
#line 1862 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9692 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 467:
#line 1864 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9700 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 468:
#line 1868 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9708 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 469:
#line 1875 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9714 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 470:
#line 1876 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9720 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 471:
#line 1877 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9726 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 472:
#line 1878 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9732 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 473:
#line 1879 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9738 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 474:
#line 1884 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9744 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 475:
#line 1885 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9750 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 476:
#line 1886 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9756 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 477:
#line 1887 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9762 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 478:
#line 1888 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9768 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 479:
#line 1909 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9776 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 480:
#line 1913 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9784 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 481:
#line 1917 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9796 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 482:
#line 1925 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9804 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 483:
#line 1931 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9810 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 484:
#line 1932 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9816 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 485:
#line 1933 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9822 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 486:
#line 1938 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9828 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 487:
#line 1940 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9834 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 488:
#line 1946 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9840 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 489:
#line 1948 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9846 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 490:
#line 1950 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9852 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 491:
#line 1954 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9858 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 492:
#line 1955 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9864 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 493:
#line 1956 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9870 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 494:
#line 1957 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9876 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 495:
#line 1958 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9882 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 496:
#line 1964 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9888 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 497:
#line 1965 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9894 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 498:
#line 1966 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9900 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 499:
#line 1967 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9906 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 500:
#line 1971 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9912 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 501:
#line 1972 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9918 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 502:
#line 1976 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9924 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 503:
#line 1977 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9930 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 504:
#line 1978 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9936 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 505:
#line 1982 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9942 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 506:
#line 1983 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9948 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 507:
#line 1987 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9954 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 509:
#line 1992 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9960 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 510:
#line 1993 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9966 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 511:
#line 1997 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9972 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 512:
#line 1998 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9978 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 513:
#line 1999 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9984 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 514:
#line 2000 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9990 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 515:
#line 2004 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9996 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 516:
#line 2005 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10002 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 517:
#line 2017 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10008 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 518:
#line 2019 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 10014 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 519:
#line 2021 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 10020 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 525:
#line 2032 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 10026 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 526:
#line 2034 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 10032 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 527:
#line 2036 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 10038 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 528:
#line 2038 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 10044 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 529:
#line 2040 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pexpr)); }
#line 10050 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 530:
#line 2042 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 10056 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 531:
#line 2044 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 10062 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 532:
#line 2047 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 10068 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 533:
#line 2049 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 10074 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 534:
#line 2051 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 10080 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 535:
#line 2053 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 10086 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 536:
#line 2055 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 10092 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 537:
#line 2057 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 10098 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 538:
#line 2059 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 10104 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 539:
#line 2061 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 10110 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 540:
#line 2064 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 10116 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 541:
#line 2066 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 10122 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 542:
#line 2071 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10128 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 543:
#line 2073 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10134 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 544:
#line 2075 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10140 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 545:
#line 2077 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10146 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 546:
#line 2079 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10152 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 547:
#line 2081 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10158 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 548:
#line 2083 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10164 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 549:
#line 2085 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10170 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 550:
#line 2087 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10176 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 551:
#line 2089 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10182 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 552:
#line 2091 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10188 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 553:
#line 2093 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10194 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 554:
#line 2095 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 10205 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 555:
#line 2102 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 10215 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 556:
#line 2108 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 10225 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 557:
#line 2114 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10235 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 558:
#line 2120 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 10245 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 559:
#line 2129 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10251 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 560:
#line 2138 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 10257 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 568:
#line 2154 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10263 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 569:
#line 2158 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 10269 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 571:
#line 2163 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10275 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 572:
#line 2167 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10281 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 573:
#line 2168 "chapel.ypp" /* yacc.c:1663  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10287 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 574:
#line 2172 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10293 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 575:
#line 2176 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10299 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 576:
#line 2177 "chapel.ypp" /* yacc.c:1663  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10305 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 577:
#line 2182 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 10313 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 578:
#line 2186 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10321 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 579:
#line 2190 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10329 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 580:
#line 2196 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 10335 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 581:
#line 2197 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 10341 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 582:
#line 2198 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 10347 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 583:
#line 2199 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 10353 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 584:
#line 2200 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 10359 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 585:
#line 2201 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 10365 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 587:
#line 2207 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10371 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 588:
#line 2212 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 10377 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 589:
#line 2214 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 10385 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 590:
#line 2218 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 10393 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 591:
#line 2222 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 10401 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 592:
#line 2226 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 10409 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 593:
#line 2234 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 10416 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 594:
#line 2239 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10426 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 595:
#line 2245 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10436 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 596:
#line 2251 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10447 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 597:
#line 2258 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10458 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 598:
#line 2268 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 10464 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 608:
#line 2285 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 10470 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 609:
#line 2287 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10476 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 610:
#line 2289 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10482 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 611:
#line 2291 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 10488 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 612:
#line 2306 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 10494 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 613:
#line 2308 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 10500 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 614:
#line 2310 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 10506 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 615:
#line 2312 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 10512 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 616:
#line 2316 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 10518 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 617:
#line 2317 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10524 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 618:
#line 2321 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 10530 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 619:
#line 2322 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 10536 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 620:
#line 2323 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10542 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 625:
#line 2339 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10548 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 626:
#line 2340 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10554 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 627:
#line 2341 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 10560 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 628:
#line 2342 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10566 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 630:
#line 2347 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10572 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 631:
#line 2348 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10578 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 632:
#line 2349 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 10584 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 633:
#line 2353 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 10590 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 634:
#line 2354 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 10596 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 635:
#line 2355 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 10602 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 636:
#line 2356 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 10608 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 637:
#line 2357 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 10614 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 638:
#line 2365 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 10620 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 639:
#line 2366 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 10626 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 640:
#line 2367 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 10632 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 641:
#line 2368 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 10638 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 642:
#line 2372 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10644 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 643:
#line 2373 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10650 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 644:
#line 2377 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10656 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 645:
#line 2378 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10662 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 648:
#line 2384 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10668 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 649:
#line 2385 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10674 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 650:
#line 2386 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10680 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 651:
#line 2387 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10686 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 652:
#line 2388 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 10692 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 653:
#line 2389 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10699 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 654:
#line 2391 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10706 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 655:
#line 2393 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10712 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 656:
#line 2394 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10718 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 657:
#line 2396 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10726 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 658:
#line 2400 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10734 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 659:
#line 2407 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10740 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 660:
#line 2408 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10746 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 661:
#line 2412 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10752 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 662:
#line 2413 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10758 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 663:
#line 2414 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10764 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 664:
#line 2415 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10770 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 665:
#line 2416 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10776 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 666:
#line 2417 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10782 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 667:
#line 2418 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10788 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 668:
#line 2419 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10794 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 669:
#line 2420 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10800 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 670:
#line 2421 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10806 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 671:
#line 2422 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10812 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 672:
#line 2423 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10818 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 673:
#line 2424 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10824 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 674:
#line 2425 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10830 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 675:
#line 2426 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10836 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 676:
#line 2427 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10842 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 677:
#line 2428 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10848 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 678:
#line 2429 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10854 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 679:
#line 2430 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10860 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 680:
#line 2431 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10866 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 681:
#line 2432 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10872 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 682:
#line 2433 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10878 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 683:
#line 2434 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10885 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 684:
#line 2439 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10891 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 685:
#line 2440 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10897 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 686:
#line 2441 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10903 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 687:
#line 2442 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10909 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 688:
#line 2443 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10915 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 689:
#line 2444 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10921 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 690:
#line 2445 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10927 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 691:
#line 2449 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10933 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 692:
#line 2450 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10939 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 693:
#line 2451 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10945 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 694:
#line 2452 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10951 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 695:
#line 2456 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10957 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 696:
#line 2457 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10963 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 697:
#line 2458 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10969 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 698:
#line 2459 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10975 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 699:
#line 2464 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10981 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 700:
#line 2465 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10987 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 701:
#line 2466 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10993 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 702:
#line 2467 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10999 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 703:
#line 2468 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 11005 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 704:
#line 2469 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 11011 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 705:
#line 2470 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 11017 "bison-chapel.cpp" /* yacc.c:1663  */
    break;


#line 11021 "bison-chapel.cpp" /* yacc.c:1663  */
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
