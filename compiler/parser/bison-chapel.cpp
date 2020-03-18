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
#line 46 "chapel.ypp" /* yacc.c:355  */

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif
#line 66 "chapel.ypp" /* yacc.c:355  */

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
  class NamedExpr;
  class Type;

  enum   ProcIter {
    ProcIter_PROC,
    ProcIter_ITER
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
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<PotentialRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif
#line 147 "chapel.ypp" /* yacc.c:355  */

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
#line 170 "chapel.ypp" /* yacc.c:355  */

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

#line 239 "bison-chapel.cpp" /* yacc.c:355  */

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
    TFORWARDING = 297,
    TIF = 298,
    TIMAG = 299,
    TIMPORT = 300,
    TIN = 301,
    TINDEX = 302,
    TINLINE = 303,
    TINOUT = 304,
    TINT = 305,
    TITER = 306,
    TINITEQUALS = 307,
    TLABEL = 308,
    TLAMBDA = 309,
    TLET = 310,
    TLIFETIME = 311,
    TLOCAL = 312,
    TLOCALE = 313,
    TMINUSMINUS = 314,
    TMODULE = 315,
    TNEW = 316,
    TNIL = 317,
    TNOINIT = 318,
    TNONE = 319,
    TNOTHING = 320,
    TON = 321,
    TONLY = 322,
    TOTHERWISE = 323,
    TOUT = 324,
    TOVERRIDE = 325,
    TOWNED = 326,
    TPARAM = 327,
    TPLUSPLUS = 328,
    TPRAGMA = 329,
    TPRIMITIVE = 330,
    TPRIVATE = 331,
    TPROC = 332,
    TPROTOTYPE = 333,
    TPUBLIC = 334,
    TREAL = 335,
    TRECORD = 336,
    TREDUCE = 337,
    TREF = 338,
    TREQUIRE = 339,
    TRETURN = 340,
    TSCAN = 341,
    TSELECT = 342,
    TSERIAL = 343,
    TSHARED = 344,
    TSINGLE = 345,
    TSPARSE = 346,
    TSTRING = 347,
    TSUBDOMAIN = 348,
    TSYNC = 349,
    TTHEN = 350,
    TTHIS = 351,
    TTHROW = 352,
    TTHROWS = 353,
    TTRUE = 354,
    TTRY = 355,
    TTRYBANG = 356,
    TTYPE = 357,
    TUINT = 358,
    TUNDERSCORE = 359,
    TUNION = 360,
    TUNMANAGED = 361,
    TUSE = 362,
    TVAR = 363,
    TVOID = 364,
    TWHEN = 365,
    TWHERE = 366,
    TWHILE = 367,
    TWITH = 368,
    TYIELD = 369,
    TZIP = 370,
    TALIAS = 371,
    TAND = 372,
    TASSIGN = 373,
    TASSIGNBAND = 374,
    TASSIGNBOR = 375,
    TASSIGNBXOR = 376,
    TASSIGNDIVIDE = 377,
    TASSIGNEXP = 378,
    TASSIGNLAND = 379,
    TASSIGNLOR = 380,
    TASSIGNMINUS = 381,
    TASSIGNMOD = 382,
    TASSIGNMULTIPLY = 383,
    TASSIGNPLUS = 384,
    TASSIGNREDUCE = 385,
    TASSIGNSL = 386,
    TASSIGNSR = 387,
    TBANG = 388,
    TBAND = 389,
    TBNOT = 390,
    TBOR = 391,
    TBXOR = 392,
    TCOLON = 393,
    TCOMMA = 394,
    TDIVIDE = 395,
    TDOT = 396,
    TDOTDOT = 397,
    TDOTDOTDOT = 398,
    TEQUAL = 399,
    TEXP = 400,
    TGREATER = 401,
    TGREATEREQUAL = 402,
    THASH = 403,
    TIO = 404,
    TLESS = 405,
    TLESSEQUAL = 406,
    TMINUS = 407,
    TMOD = 408,
    TNOTEQUAL = 409,
    TOR = 410,
    TPLUS = 411,
    TQUESTION = 412,
    TSEMI = 413,
    TSHIFTLEFT = 414,
    TSHIFTRIGHT = 415,
    TSTAR = 416,
    TSWAP = 417,
    TLCBR = 418,
    TRCBR = 419,
    TLP = 420,
    TRP = 421,
    TLSBR = 422,
    TRSBR = 423,
    TNOELSE = 424,
    TUPLUS = 425,
    TUMINUS = 426,
    TLNOT = 427
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
#line 201 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 457 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 463 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 38 "chapel.ypp" /* yacc.c:359  */

  #include <string>
  int         captureTokens;
  std::string captureString;
  bool        parsingPrivate=false;
#line 209 "chapel.ypp" /* yacc.c:359  */

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

#line 536 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   18151

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  173
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  146
/* YYNRULES -- Number of rules.  */
#define YYNRULES  625
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1124

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   427

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
     165,   166,   167,   168,   169,   170,   171,   172
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   497,   497,   502,   503,   509,   510,   515,   516,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   550,   551,
     555,   568,   573,   578,   586,   587,   588,   592,   593,   606,
     607,   608,   613,   614,   619,   624,   629,   633,   640,   645,
     649,   654,   658,   659,   660,   664,   665,   666,   667,   668,
     672,   673,   674,   678,   682,   684,   686,   688,   690,   697,
     698,   702,   703,   704,   705,   706,   707,   710,   711,   712,
     713,   714,   715,   727,   728,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,   750,   751,   752,   753,
     754,   755,   756,   757,   761,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,   772,   779,   780,   781,   782,
     786,   787,   791,   792,   796,   797,   798,   808,   808,   813,
     814,   815,   816,   817,   818,   819,   823,   824,   825,   826,
     831,   830,   846,   845,   862,   861,   877,   876,   892,   896,
     901,   909,   920,   927,   928,   929,   930,   931,   932,   933,
     934,   935,   936,   937,   938,   939,   940,   941,   942,   943,
     944,   945,   951,   957,   963,   969,   976,   983,   987,   994,
     998,   999,  1000,  1001,  1002,  1004,  1006,  1008,  1013,  1016,
    1017,  1018,  1019,  1020,  1021,  1025,  1026,  1030,  1031,  1032,
    1036,  1037,  1041,  1044,  1046,  1051,  1052,  1056,  1058,  1060,
    1067,  1077,  1091,  1096,  1101,  1109,  1110,  1115,  1116,  1118,
    1123,  1139,  1146,  1155,  1163,  1167,  1174,  1175,  1180,  1185,
    1179,  1212,  1215,  1219,  1227,  1237,  1226,  1269,  1273,  1278,
    1282,  1287,  1294,  1295,  1299,  1300,  1301,  1302,  1303,  1304,
    1305,  1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,  1314,
    1315,  1316,  1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,  1326,  1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,
    1338,  1339,  1340,  1341,  1345,  1346,  1350,  1354,  1355,  1356,
    1360,  1362,  1364,  1366,  1368,  1373,  1374,  1378,  1379,  1380,
    1381,  1382,  1383,  1384,  1385,  1386,  1390,  1391,  1392,  1393,
    1394,  1395,  1399,  1400,  1404,  1405,  1406,  1407,  1408,  1409,
    1413,  1414,  1417,  1418,  1422,  1423,  1427,  1432,  1436,  1437,
    1438,  1446,  1447,  1449,  1451,  1453,  1458,  1460,  1465,  1466,
    1467,  1468,  1469,  1470,  1471,  1475,  1477,  1482,  1484,  1486,
    1491,  1504,  1521,  1522,  1524,  1529,  1530,  1531,  1532,  1533,
    1537,  1543,  1551,  1552,  1560,  1562,  1567,  1569,  1571,  1576,
    1578,  1580,  1587,  1588,  1589,  1594,  1596,  1598,  1602,  1606,
    1608,  1612,  1620,  1621,  1622,  1623,  1624,  1629,  1630,  1631,
    1632,  1633,  1653,  1657,  1661,  1669,  1676,  1677,  1678,  1682,
    1684,  1690,  1692,  1694,  1699,  1700,  1701,  1702,  1703,  1709,
    1710,  1711,  1712,  1716,  1717,  1721,  1722,  1723,  1727,  1728,
    1732,  1733,  1737,  1738,  1742,  1743,  1744,  1745,  1749,  1750,
    1761,  1763,  1765,  1771,  1772,  1773,  1774,  1775,  1776,  1778,
    1780,  1782,  1784,  1786,  1788,  1791,  1793,  1795,  1797,  1799,
    1801,  1803,  1805,  1808,  1810,  1815,  1817,  1819,  1821,  1823,
    1825,  1827,  1829,  1831,  1833,  1835,  1837,  1839,  1846,  1852,
    1858,  1864,  1873,  1883,  1891,  1892,  1893,  1894,  1895,  1896,
    1897,  1898,  1903,  1904,  1908,  1912,  1913,  1917,  1921,  1922,
    1926,  1930,  1934,  1941,  1942,  1943,  1944,  1945,  1946,  1950,
    1951,  1956,  1958,  1962,  1966,  1970,  1978,  1983,  1989,  1995,
    2002,  2011,  2015,  2022,  2030,  2031,  2032,  2033,  2034,  2035,
    2036,  2037,  2038,  2040,  2042,  2044,  2046,  2048,  2053,  2054,
    2058,  2059,  2060,  2064,  2065,  2066,  2067,  2078,  2079,  2080,
    2081,  2085,  2086,  2087,  2091,  2092,  2093,  2094,  2095,  2103,
    2104,  2105,  2106,  2110,  2111,  2115,  2116,  2117,  2118,  2119,
    2120,  2121,  2122,  2123,  2124,  2125,  2126,  2127,  2131,  2139,
    2140,  2144,  2145,  2146,  2147,  2148,  2149,  2150,  2151,  2152,
    2153,  2154,  2155,  2156,  2157,  2158,  2159,  2160,  2161,  2162,
    2163,  2164,  2165,  2166,  2170,  2171,  2172,  2173,  2174,  2175,
    2176,  2180,  2181,  2182,  2183,  2187,  2188,  2189,  2190,  2195,
    2196,  2197,  2198,  2199,  2200,  2201
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
  "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE", "TFOR", "TFORALL",
  "TFORWARDING", "TIF", "TIMAG", "TIMPORT", "TIN", "TINDEX", "TINLINE",
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
  "TNOELSE", "TUPLUS", "TUMINUS", "TLNOT", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_start", "module_decl_stmt", "access_control",
  "opt_prototype", "block_stmt", "stmt_ls", "renames_ls", "opt_only_ls",
  "except_ls", "use_access_control", "use_stmt", "import_stmt",
  "require_stmt", "assignment_stmt", "opt_label_ident", "ident_fn_def",
  "ident_def", "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt", "class_level_stmt",
  "@1", "private_decl", "forwarding_stmt", "extern_export_decl_stmt",
  "$@2", "$@3", "$@4", "$@5", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@6", "$@7", "linkage_spec",
  "fn_decl_stmt", "$@8", "$@9", "fn_decl_stmt_inner",
  "fn_decl_receiver_expr", "fn_ident", "assignop_ident", "opt_formal_ls",
  "req_formal_ls", "formal_ls", "formal", "opt_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_or_iter",
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
  "let_expr", "expr", "opt_expr", "opt_try_expr", "lhs_expr", "fun_expr",
  "call_expr", "dot_expr", "parenthesized_expr", "bool_literal",
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
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427
};
# endif

#define YYPACT_NINF -984

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-984)))

#define YYTABLE_NINF -576

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -984,    79,  2939,  -984,   -63,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  4425,    -4,   143,  -984, 13198,  -984, 16646,    -4,
   10129,  -984,   391,   157,   143,  4425, 10129,  4425,    78, 17793,
   10292,  8161,  -984,  8324,  8980,  6720, 10129,  -984,    95,  -984,
    -984, 17851,  -984,  2616,  9143,  -984, 10129,    53,  -984,  -984,
    -984, 10129,  -984, 13198,  -984, 10129,   281,   131,   213, 18043,
    -984, 17888,  -984,  9308,  7543, 10129,  9143, 13198, 10129,   272,
    -984,   203,  4425,  -984, 10129,  -984, 10455, 10455, 17851,  -984,
    -984, 13198,  -984,  -984, 10129, 10129,  -984, 10129,  -984, 10129,
    -984,  -984, 12737, 10129,  -984, 10129,  -984,  -984,  -984,  3437,
    6885,  8489,  -984,  4262,  -984,   225,  -984,   321,  -984,    13,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984, 17851,
    -984, 17851,   369,    49,  -984,  -984,  2616,  -984,   277,  -984,
     286,  -984,  -984,   340,   356,   379, 10129,   371,   383, 17289,
   12730,   142,   389,   390,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,   319,  -984,  -984, 17289,   314,  4425,  -984,  -984,   392,
   10129,  -984,  -984, 10129, 10129, 10129,  -984, 10129,  9308,  9308,
     233,  -984,  -984,  -984,  -984,    11,   300,  -984,  -984,   393,
   15513, 17851,  2616,  -984,   394,  -984,   -47, 17289,   452,  7708,
     484, 17946, 17289,   636,   485,  -984, 17983, 17851,   636, 17851,
     403,    50, 14951,    12, 14875,   422,  -984, 15363, 14151,  7708,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  4425,  -984,   404,  1495,    67,    75,
    -984,  4425,  -984,  -984, 15103,   652,  -984,   405,   412,  -984,
   15103,    11,   652,  -984,  7708,  2773,  -984,  -984,  9471,  -984,
    -984,  -984, 17851,  -984,   164, 17289, 10129, 10129,  -984, 17289,
     420, 15264,  -984, 15103,    11, 17289,   415,  7708,  -984, 17289,
   15811,  -984,  -984, 15843,  1364,  -984,  -984, 15900,   468,   432,
      11, 15103, 15988,   309,   309,  1542,   652,   652,   251,  -984,
    -984,  3602,   -29,  -984, 10129,  -984,   -36,     2,  -984,    18,
     -17, 16045,   109,   585,  -984,  3767,  -984,   534, 10129, 10129,
     458,   437,  -984,  -984,  -984,   228,   348,  -984, 10129,   460,
   10129, 10129, 10129,  8980,  8980, 10129,   353, 10129, 10129, 10129,
   10129, 10129,   486, 12737, 10129, 10129, 10129, 10129, 10129, 10129,
   10129, 10129, 10129, 10129, 10129, 10129, 10129, 10129, 10129,  -984,
    -984,  -984,  -984,  -984,  -984,  8652,  8652,  -984,  -984,  -984,
    -984,  8652,  -984,  -984,  8652,  8652,  7708,  7708,  8980,  8980,
    7380,  -984,  -984, 15562, 15659, 16077,   -11,  3932,  -984,  8980,
      50,   444,   231,  -984, 10129,  -984, 10129,   489,  -984,   442,
     464,  -984,  -984,  -984, 17851,  -984,  2616,  -984, 17851,   451,
    -984,  2616,   564,  9308,  -984,  4588,  8980,  -984,   446,  -984,
      50,  4751,  8980,  -984,    50, 10618, 10129,  -984,  4425,   578,
   10129,   455,  -984,   592,  -984,  -984,  1495,  -984,   478,   461,
    -984, 10781,   500, 10129,  2616,  -984,  -984, 10129, 10129,  -984,
     465,  -984,  -984,  9308,  -984, 17289, 17289,  -984,    44,  -984,
    7708,   469,  -984,   613,  -984,   613,  -984, 10944,   497,  -984,
    -984,  -984,  -984,  -984,  -984,  8817,  -984, 13846,  7050,  -984,
    7215,  -984,  4425,   471,  8980,  6222,  3272,   472, 10129,  6554,
    -984,  -984,   263,  -984,  4097, 17851, 15602,   264, 15416,  9308,
     474, 17756,   197,  -984, 16157, 17570, 17570,   362,  -984,   362,
    -984,   362,  2074,   145,  1166,  1259,    11,   309,   482,  -984,
    -984,  -984,  -984,  1542,  2383,   362, 12890, 12890, 17570, 12890,
   12890,  1032,   309,  2383, 17602,  1032,   652,   652,   309,   491,
     493,   494,   507,   510,   477,   503,  -984,   362,  -984,   362,
      21,  -984,  -984,  -984,    97,  -984,  1838, 17385,   430, 11107,
    8980, 11270,  8980, 10129,  8980, 13460,    -4, 16233,  -984,  -984,
    -984, 17289, 16265,  7708,  -984,  7708, 17851,   458,   297, 17851,
     458,  -984,   317, 10129,   115, 10129, 17289,    54, 15179,  7380,
    -984, 10129, 17289,    14, 15027,  -984,   518,   540,   522, 16314,
     540,   529,   654, 16410,  4425, 15211,  -984,    55,  -984,  -984,
    -984,  -984,  -984,  -984,   680,   118,  -984,  2759,  -984,   288,
     523,  1495,    67,    78,    95, 10129, 10129,  6057,  -984,  -984,
     487,  7871,  -984, 17289,  -984, 13942, 13999,  -984,  -984, 17289,
     527,     6,   526,  -984,  2093,  -984,  -984,   330, 17851,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  4425,    69, 13611,  -984,
    -984, 17289,  4425, 17289,  -984, 16459,  -984,  -984,  -984,    87,
      82,  -984, 10129,  -984, 10129, 10618, 10129,   556,  1152,   546,
     581,   218,  -984,   621,  -984,  -984,  -984,  -984,  8000,   547,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  7380,
    -984,    35,  8980,  8980, 10129,   683, 16491, 10129,   689, 16636,
     544, 13700,    50,    50,  -984,  -984,  -984,  -984,   458,   553,
    -984,   458,   559,  -984, 15103,  -984, 14231,  4914,  -984,  5077,
    -984,   155,  -984, 14311,  5240,  -984,    50,  5403,  -984,    50,
    -984, 10129,  -984,  4425, 10129,  -984,  4425,   694,  -984,  -984,
   17851,   883,  -984,  1495,   595,   647,  -984,  -984,  -984,    58,
    -984,  -984,   500,   568,    51,  -984,  -984,   572,   575,  -984,
    5566,  9308,  -984,  -984,  -984, 17851,  -984,   605,   393,  -984,
    -984,  5729,   579,  5892,   580,  -984, 10129,   588, 10129, 16668,
   15754,   593,   597,   586,  1716,  -984, 10129, 17851,  -984,  -984,
     288,   594,   160,  -984,   616,  -984,   623,   624,   629,   626,
     630,  -984,   631,   637,   632,   633,   634,   186,   643,   639,
     650,  -984,  -984, 17851,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,  -984, 10129,  -984,   644,   645,   655,   594,   594,
    -984,  -984,  -984,   500,   227,   240, 16744, 11433, 11596, 16824,
   11759, 11922, 12085, 12248,  -984,  -984,   625,  -984,   635,  -984,
    -984,  4425, 10129, 17289, 10129, 17289,  7380,  -984,  4425, 10129,
   17289,  -984, 10129, 17289,  -984, 17289,  -984, 17289,   761,  4425,
     595,  -984,   640,  9636,   267,  -984,   177,  -984,  -984,  8980,
   13329,  7708,  7708,  4425,  -984,    16,   641, 10129,  -984, 10129,
    -984, 17289,  4425, 10129,  -984, 17289,  4425, 17289,  -984,   265,
   10129, 10618, 10129,  -984,  -984,  -984,  -984,  2773,  -984, 17593,
    -984, 17289,  -984,   104,   592,  -984,  -984, 14039,  -984, 13555,
    -984,  -984,  -984, 10129, 10129, 10129, 10129, 10129, 10129, 10129,
   10129,  -984,  2303,  -984,  2524, 16410, 14391, 14471,  -984, 16410,
   14551, 14631,  4425,  -984,   500,   595,  6388,  -984,  -984,  -984,
     129,  9308,  -984,  -984,    60, 10129,   -24, 13757,  -984,   764,
     646,   648,   452,  -984,   393, 17289, 14711,  -984, 14791,  -984,
    -984, 17289,   651,   658,  -984,  -984, 12411,   701,   242,  -984,
     667,   668,   594,   594, 16856, 16936, 17016, 17048, 17128, 17208,
   17521,  -984, 17535,  -984,  4425,  4425,  4425,  4425,  -984,  -984,
     267,  9801,    71,  -984, 17289,  -984,   126,  -984,   148,  -984,
     671, 17240,  -984,  -984,  -984, 12248,   662,   666,  -984,  4425,
    4425,  -984,  -984,  3104,  -984,  -984,   327,  -984,   177,  -984,
    -984,  -984, 10129, 10129, 10129, 10129, 10129, 10129,  -984,  -984,
   16410, 16410, 16410, 16410,  -984,  -984,  -984,  -984,  -984,   241,
    8980, 12902,  -984, 10129,    60,   126,   126,   126,   126,   126,
     126,    60,  1155,  -984,  -984, 16410, 16410,   656, 12574,    73,
      86, 13797,  -984,  -984, 17289,  -984,  -984,  -984,  -984,  -984,
    -984,  -984,   664,  -984,  -984,   346, 13067,  -984,  -984,  -984,
    9966,  -984,   535,  -984
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    93,   566,   567,   568,   569,
     570,   571,     0,   492,    79,   114,   461,   121,   463,   492,
       0,   120,     0,   366,    79,     0,     0,     0,     0,   115,
     538,   538,   563,     0,     0,     0,     0,   119,     0,   242,
     116,     0,   238,     0,     0,   123,     0,   511,   483,   572,
     124,     0,   243,   455,   365,     0,     0,     0,   137,    45,
     118,   464,   367,     0,     0,     0,     0,   459,     0,     0,
     122,     0,     0,    94,     0,   564,     0,     0,     0,   117,
     224,   457,   369,   125,     0,     0,   621,     0,   623,     0,
     624,   625,   537,     0,   622,   619,   442,   134,   620,     0,
       0,     0,     4,     0,     5,     0,     9,    47,    10,     0,
      11,    12,    13,    15,   438,   439,    23,    14,   135,   144,
     145,    16,    18,    17,    20,    21,    22,    19,   143,     0,
     141,     0,   529,     0,   139,   142,     0,   140,   543,   525,
     440,   526,   445,   443,     0,     0,     0,   530,   531,     0,
     444,     0,   544,   545,   546,   565,   524,   447,   446,   527,
     528,     0,    39,    25,   453,     0,     0,   493,    80,     0,
       0,   463,   115,     0,     0,     0,   464,     0,     0,     0,
     529,   543,   443,   530,   531,   462,   444,   544,   545,     0,
     492,     0,     0,   368,     0,   198,     0,   423,     0,   430,
     569,   464,   539,   241,   569,   162,   464,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   149,     0,     0,   430,
      87,    95,   107,   101,   100,   109,    90,    99,   110,    96,
     111,    88,   112,   105,    98,   106,   104,   102,   103,    89,
      91,    97,   108,   113,     0,    92,     0,     0,     0,     0,
     372,     0,   131,    33,     0,   606,   515,   512,   513,   514,
       0,   456,   607,     7,   430,   241,   222,   232,   538,   223,
     136,   337,   442,   420,     0,   419,     0,     0,   132,   542,
       0,     0,    36,     0,   460,   448,     0,   430,    37,   454,
       0,   205,   201,     0,   444,   205,   202,     0,   362,     0,
     458,     0,     0,   608,   610,   536,   605,   604,     0,    49,
      52,     0,     0,   425,     0,   427,     0,     0,   426,     0,
       0,   419,     0,     0,     6,     0,    48,     0,     0,     0,
     225,     0,   323,   322,   244,     0,   441,    24,     0,   516,
       0,     0,     0,     0,     0,     0,   609,     0,     0,     0,
       0,     0,     0,   535,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   282,
     289,   290,   291,   286,   288,     0,     0,   284,   287,   285,
     283,     0,   293,   292,     0,     0,   430,   430,     0,     0,
       0,    26,    27,     0,     0,     0,     0,     0,    28,     0,
       0,     0,     0,    29,     0,    30,     0,   438,   436,     0,
     431,   432,   437,   156,     0,   159,     0,   152,     0,     0,
     158,     0,     0,     0,   171,     0,     0,   170,     0,   179,
       0,     0,     0,   177,     0,     0,    58,   146,     0,   191,
       0,     0,    31,   305,   239,   376,     0,   377,   379,     0,
     401,     0,   382,     0,     0,   130,    32,     0,     0,    34,
       0,   138,   336,     0,    73,   540,   541,   133,     0,    35,
     430,     0,   212,   203,   199,   204,   200,     0,   360,   357,
     164,    38,    51,    50,    53,     0,   573,     0,     0,   559,
       0,   561,     0,     0,     0,     0,     0,     0,     0,     0,
     577,     8,     0,    41,     0,     0,     0,     0,    54,     0,
       0,     0,   316,   371,   491,   601,   600,   603,   612,   611,
     616,   615,   597,   594,   595,   596,   533,   584,     0,   556,
     557,   555,   554,   534,   588,   599,   593,   591,   602,   592,
     590,   582,   587,   589,   598,   581,   585,   586,   583,     0,
       0,     0,     0,     0,     0,     0,   614,   613,   618,   617,
     503,   504,   506,   508,     0,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   575,   492,   492,   167,   358,
     370,   424,     0,     0,   450,     0,     0,   225,     0,     0,
     225,   359,     0,     0,     0,     0,   467,     0,     0,     0,
     180,     0,   473,     0,     0,   178,   620,    61,     0,    54,
      59,     0,   190,     0,     0,     0,   449,   310,   307,   308,
     309,   313,   314,   315,   305,     0,   298,     0,   306,   324,
       0,   380,     0,   128,   129,   127,   126,     0,   400,   399,
     525,     0,   374,   523,   373,     0,     0,   553,   422,   421,
       0,     0,     0,   451,     0,   206,   364,   525,     0,   574,
     532,   560,   428,   562,   429,   187,     0,     0,     0,   576,
     185,   477,     0,   579,   578,     0,    43,    42,    40,     0,
       0,    70,     0,    65,     0,     0,    58,   226,     0,     0,
     236,     0,   233,   320,   317,   318,   321,   245,     0,     0,
      77,    78,    76,    75,    74,   551,   552,   505,   507,     0,
     494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   163,   434,   435,   433,   225,     0,
     161,   225,     0,   160,     0,   189,     0,     0,   169,     0,
     168,     0,   498,     0,     0,   175,     0,     0,   173,     0,
     147,     0,   148,     0,     0,   193,     0,   195,   311,   312,
       0,   305,   296,     0,   414,   325,   328,   327,   329,     0,
     378,   381,   382,     0,     0,   383,   384,   521,   522,   214,
       0,     0,   213,   216,   452,     0,   207,   210,     0,   361,
     188,     0,     0,     0,     0,   186,     0,     0,     0,    56,
      55,     0,     0,     0,   241,   231,     0,   234,   230,   319,
     324,   294,    81,   277,    95,   275,   101,   100,    84,    99,
      96,   280,   111,    82,   112,    98,   102,    83,    85,    97,
     113,   274,   256,   259,   257,   258,   269,   260,   273,   265,
     263,   276,   279,   264,   262,   267,   272,   261,   266,   270,
     271,   268,   278,     0,   254,     0,    86,     0,   294,   294,
     252,   558,   496,   382,   543,   543,     0,     0,     0,     0,
       0,     0,     0,     0,   166,   165,     0,   227,     0,   227,
     172,     0,     0,   466,     0,   465,     0,   497,     0,     0,
     472,   176,     0,   471,   174,    55,   192,   482,   194,     0,
     414,   299,     0,     0,   382,   326,   341,   375,   405,     0,
     575,   430,   430,     0,   218,     0,     0,     0,   208,     0,
     183,   479,     0,     0,   181,   478,     0,   580,    71,     0,
       0,     0,    58,    66,    68,   221,   137,   241,   220,   241,
     228,   237,   235,     0,   305,   251,   255,     0,   281,     0,
     247,   248,   500,     0,     0,     0,     0,     0,     0,     0,
       0,   227,   241,   227,   241,   470,     0,     0,   499,   476,
       0,     0,     0,   197,   382,   414,     0,   417,   416,   418,
     525,   338,   302,   300,     0,     0,     0,     0,   403,   525,
       0,     0,   219,   217,     0,   211,     0,   184,     0,   182,
      72,    57,     0,     0,   229,   396,     0,   330,     0,   253,
      81,    83,   294,   294,     0,     0,     0,     0,     0,     0,
     241,   155,   241,   151,     0,     0,     0,     0,   196,   301,
     382,   406,     0,   340,   339,   355,     0,   356,   343,   346,
       0,   342,   334,   335,   240,     0,   517,   518,   209,     0,
       0,    67,    69,     0,   395,   394,   525,   331,   341,   295,
     249,   250,     0,     0,     0,     0,     0,     0,   157,   153,
     469,   468,   475,   474,   304,   303,   408,   409,   411,   525,
       0,   575,   354,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   519,   520,   481,   480,     0,   386,     0,
       0,     0,   410,   412,   345,   347,   348,   351,   352,   353,
     349,   350,   344,   391,   389,   525,   575,   332,   246,   333,
     406,   390,   525,   413
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -984,  -984,  -984,     7,  -365,  2003,  -984,  -984,  -984,  -984,
     248,   501,  -328,  -671,  -652,  -984,  -984,  -984,  -984,   376,
     801,  -984,  1016,  -188,  -678,  -984,  -856,   402,  -920,  -767,
    -984,   -56,  -984,  -984,  -984,  -984,  -984,  -984,  -984,  -984,
     196,  -984,  -984,  -984,   532,  -984,    43,  -984,  -984,  -984,
    -984,  -984,  -984,  -496,  -651,  -984,  -984,  -984,    22,   976,
    -984,  -984,  -984,   293,  -984,  -984,  -984,  -984,  -119,  -145,
    -809,  -984,  -113,    72,   210,  -984,  -984,  -984,    25,  -984,
    -984,  -263,    47,  -192,  -219,  -251,  -242,  -564,  -984,  -181,
    -984,     8,   808,  -108,   399,  -984,  -440,  -756,  -878,  -984,
    -575,  -470,  -983,  -831,  -794,   -61,  -984,   -58,  -984,  -194,
    -984,   259,   642,  -439,  -984,  -984,  -984,  1018,  -984,    -2,
    -984,  -984,  -187,  -984,  -586,  -984,  -984,  -984,  1310,  1531,
     -12,   817,   170,   684,  -984,  1627,  1865,  -984,  -984,  -984,
    -984,  -984,  -984,  -984,  -984,  -348
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   310,   103,   612,   105,   106,   107,   327,
     108,   311,   607,   611,   608,   109,   110,   111,   112,   113,
     169,   854,   248,   114,   245,   115,   638,   253,   116,   117,
     265,   118,   119,   120,   418,   589,   414,   586,   121,   122,
     720,   123,   124,   125,   473,   655,   788,   126,   127,   651,
     783,   128,   129,   510,   804,   130,   131,   691,   692,   180,
     246,   629,   133,   134,   512,   810,   697,   857,   858,   440,
     945,   444,   625,   626,   627,   628,   698,   334,   769,  1058,
    1118,  1044,   273,   982,   986,  1038,  1039,  1040,   135,   299,
     478,   136,   137,   249,   250,   448,   449,   642,  1055,  1007,
     452,   639,  1077,   979,   904,   312,   196,   316,   317,   409,
     410,   411,   181,   139,   140,   141,   142,   182,   144,   166,
     167,   564,   430,   741,   565,   566,   145,   146,   183,   184,
     149,   208,   412,   186,   151,   187,   188,   154,   155,   156,
     322,   157,   158,   159,   160,   161
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     164,   507,   274,   270,   185,   385,   630,   656,   190,   102,
     401,   407,   640,   742,   197,   802,   907,   189,   202,   202,
     856,   212,   214,   217,   218,   441,   419,   434,   335,   494,
     192,   407,   254,   801,   255,   574,   450,   940,   657,   260,
     320,   261,   568,   262,   251,   650,   744,   977,   251,   950,
     951,   275,   279,   281,   283,   284,   285,   772,   328,   450,
     289,    64,   290,  1035,   293,   297,  1043,   707,   450,   300,
     460,   256,   301,   302,   780,   303,   407,   304,   624,     3,
     305,   306,   251,   307,   402,     5,   737,   275,   279,   321,
       5,   729,   404,   471,   732,   162,   428,   909,  1102,   407,
     332,   758,   528,   488,   708,  1005,   974,   952,   610,   165,
     485,   405,  -215,  -397,  -397,   529,   781,  1080,   396,   574,
     329,   453,   495,   862,   257,   428,   333,   428,   495,  1035,
     489,   428,   493,   497,   339,   486,   863,  1123,   759,   397,
     530,   490,   258,  -397,   346,  1036,   168,   315,   983,   385,
    1054,   496,   352,  -397,  -215,   463,  1037,   575,   164,   259,
    -392,   393,   394,   395,   532,   289,   275,   321,   491,  -397,
     782,    64,  1004,   451,  -397,   397,   342,   397,    73,   397,
    1043,  1030,   428,    73,   531,  -397,   492,   279,   400,  -392,
     495,   771,   554,   555,   906,   940,   451,   940,   407,   407,
    1078,  -397,  -392,  1060,  1061,   451,  -397,   279,  -215,   461,
     495,   416,   495,   397,   454,  -392,   421,   397,  1029,   910,
    1114,  -397,  1037,   192,   693,  -397,   962,   343,   964,   211,
     213,   344,   876,   984,   280,   878,   709,   791,  1121,  1081,
     193,  1116,  1006,   199,  1117,   798,   408,  -415,   499,   397,
    1103,   568,   279,   940,   463,   940,   202,   761,   -64,  1083,
     219,  1003,  -392,   710,   465,   466,   408,  -392,  -415,   694,
     318,   856,  -415,   -46,  1075,   279,   652,   500,   346,  1002,
     695,   735,   407,   350,   762,   351,   352,  1084,   985,   263,
     355,   -46,   252,   -87,   886,  -415,   264,   319,   362,   696,
     968,   -93,   487,   463,   366,   367,   368,   386,   588,   387,
    1020,   408,  1022,   592,   252,   765,   506,   508,   484,   -89,
     -64,   887,   464,   902,   291,   295,   514,   -94,   515,   516,
     517,   519,   521,   522,   408,   523,   524,   525,   526,   527,
     342,   533,   534,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   807,   610,  -407,
     766,   568,   594,   279,   279,   286,  -502,   454,   287,   279,
     454,   767,   279,   279,   279,   279,   557,   559,   567,  -501,
    -407,   761,   808,  -393,  -407,   641,   513,   577,   325,   580,
     768,   343,   581,  -502,   582,   344,   624,   407,  -550,   326,
    -550,   388,  -385,   682,   682,   389,  -501,  -407,  1059,   162,
     981,   275,  -393,   596,   598,   482,   746,   749,    23,   602,
     604,   162,   683,   609,   609,  -393,   613,   676,   615,  1000,
     662,  -385,   664,   408,   408,  -485,   454,   398,  -393,   939,
     988,   643,   346,   336,  -385,   645,   646,   350,   687,    23,
     352,   649,   292,   296,   355,   730,   454,  -385,   279,   252,
     252,   252,   252,    54,   980,  -547,   439,  -547,   282,  -363,
     929,   989,  1082,   649,    62,   733,   279,   789,   279,   390,
     792,   794,   668,   649,   671,  -393,   673,   675,  -363,     5,
    -393,   797,   532,   191,    54,   346,   415,   275,  -484,    82,
     350,   420,   252,   352,  -385,    62,   528,   355,   252,  -385,
     648,   484,   713,  -549,   337,  -549,   389,   408,  -548,   529,
    -548,  1106,  1107,  1108,  1109,  1110,  1111,  -488,   338,  -489,
      82,   252,   648,  -398,  -550,   315,  -550,   315,   568,   518,
     520,  -490,   648,  -398,   530,   549,   550,  -487,  -486,   252,
     392,   551,   403,   859,   552,   553,   397,   596,   716,   602,
     719,   613,   721,   671,   406,   413,   417,  1056,   423,   443,
     457,   279,  -398,   279,   722,   723,   774,   458,   467,   624,
     470,   734,    73,   736,   556,   558,   477,   567,   531,   743,
     479,  -387,  1079,   501,   505,   576,   509,   939,  -398,   939,
     511,   352,   579,   585,   610,  -398,  1092,   583,   584,   591,
     593,   599,   614,   424,   427,   429,   433,   631,   641,   617,
    -387,   616,   597,   285,   289,   321,  -398,   632,   603,   279,
     725,   647,   408,  -387,   654,   653,   658,   688,   618,   666,
     672,   619,  1079,   705,   138,  -398,  -387,   699,   252,   700,
    -398,   701,   702,  -398,   138,   939,   456,   939,   318,  1115,
     318,   620,   459,    23,   621,   703,    56,   138,   704,   138,
     799,   706,   800,   609,   609,   622,   -60,  1122,   252,   682,
     750,  1079,   252,   342,    39,   469,   150,   752,   753,   770,
     667,   779,   784,  -387,   623,   463,   150,   567,  -387,   806,
     519,   557,   866,   480,   809,   869,    52,   617,    54,   150,
     805,   150,   872,   861,   138,   867,   877,   990,   991,    62,
     915,   870,   879,   407,   407,   883,   618,   885,   899,   619,
     905,  -297,   890,   903,   343,   893,   908,   911,   344,   895,
     912,   138,   897,   917,    82,   138,   928,   922,   926,   620,
     935,   933,   621,   726,   323,   934,   150,  -114,  -297,   944,
     294,   294,   -90,   622,  -121,  -120,   715,  -119,   718,   275,
     -88,  -116,  -123,  -124,  -118,  -122,   -91,   948,   -92,   921,
    -117,   925,   623,   150,   927,   346,   609,   150,   961,  1085,
     350,  -125,   351,   352,   941,   972,   949,   355,   963,  1057,
     -87,   -89,   578,  1084,  1013,   362,   975,   994,   138,  1051,
    -402,   776,  1046,   368,  1047,  1086,  1052,  1087,  1088,  1093,
    -402,  1089,  1090,  1094,  1113,   194,   504,   475,   916,   942,
    1012,  1008,   600,   901,   760,   943,   605,  1119,  1074,  1100,
    1112,   947,  1105,   216,   727,   252,   252,   203,     0,  -402,
     150,   252,   252,   644,     0,   883,   885,     0,   890,   893,
     921,   925,     0,   757,     0,     0,     0,     0,     0,   965,
     966,     0,   967,     0,   567,  -402,   969,   970,     0,     0,
     971,   270,  -402,     0,     0,     0,   138,     0,     0,     0,
       0,     0,     0,   138,     0,     0,     0,   987,   671,   279,
     279,     0,   786,  -402,     0,   995,     0,   996,   518,   556,
     617,   998,     0,     0,     0,  1032,     0,     0,  1001,   609,
     609,     0,  -402,     0,     0,     0,     0,  -402,   150,   618,
    -402,     0,   619,     0,     0,   150,     0,     0,     0,     0,
       0,   965,  1014,  1015,   969,  1016,  1017,  1018,  1019,     0,
     978,     0,   620,   138,     0,   621,     0,    56,   408,   408,
       0,     0,     0,     0,   321,     0,   622,   138,     0,  1034,
     252,   252,     0,  1041,     0,     0,     0,     0,   132,     0,
       0,     0,   252,     0,   439,   623,     0,     0,   132,     0,
       0,   439,  1099,     0,   252,   150,     0,   252,     0,   738,
     740,   132,     0,   132,     0,   745,   748,     0,     0,   150,
       0,     0,  1070,  1071,  1072,  1073,     0,     0,     0,     0,
     143,     0,     0,     0,     0,     0,     0,     0,  1033,     0,
     143,     0,     0,   925,     0,     0,   918,  1095,  1096,   138,
       0,   321,     0,   143,     0,   143,     0,     0,   132,     0,
    1070,  1071,  1072,  1073,  1095,  1096,     0,   244,     0,     0,
       0,     0,     0,   342,     0,     0,     0,   138,  1101,   671,
       0,  1104,     0,   138,     0,   132,     0,     0,  1076,   132,
     138,   150,     0,     0,     0,     0,     0,     0,     0,     0,
     143,     0,     0,     0,   298,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   671,     0,     0,     0,   925,   150,
       0,     0,     0,     0,   343,   150,     0,   143,   344,     0,
       0,   143,   150,     0,   874,   875,     0,     0,  1076,     0,
       0,     0,     0,     0,   138,     0,   880,     0,   138,     0,
       0,     0,   132,     0,     0,   330,   138,   331,   891,     0,
       0,   894,     0,   803,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,   346,   347,  1076,   348,   349,
     350,     0,   351,   352,  -227,     0,   150,   355,  -227,  -227,
     150,     0,     0,     0,   143,   362,     0,  -227,   150,  -227,
    -227,   366,   367,   368,  -227,     0,     0,   342,     0,     0,
    -227,  -404,     0,  -227,     0,     0,     0,   298,   711,     0,
       0,  -404,     0,     0,   439,   439,     0,     0,   439,   439,
     132,     0,  -227,   298,  -227,   422,  -227,   132,  -227,  -227,
       0,  -227,     0,  -227,  1042,  -227,     0,     0,     0,     0,
    -404,     0,  1048,     0,   439,     0,   439,     0,   343,     0,
       0,     0,   344,     0,  -227,     0,   138,  -227,     0,     0,
    -227,     0,   143,   447,     0,     0,  -404,     0,     0,   143,
       0,     0,     0,  -404,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   132,   462,     0,
     342,     0,     0,     0,  -404,     0,     0,     0,   150,   346,
     347,   132,     0,   349,   350,     0,   351,   352,   138,     0,
    -227,   355,   147,  -404,   138,     0,  -227,   993,  -404,   362,
       0,  -404,   147,     0,     0,   366,   367,   368,     0,   143,
       0,     0,     0,     0,     0,   147,     0,   147,     0,     0,
     860,   343,     0,   143,     0,   344,     0,     0,  1042,     0,
     150,     0,     0,     0,   864,   865,   150,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   132,     0,     0,     0,     0,     0,   138,
       0,   138,   147,     0,     0,     0,   138,     0,     0,   138,
       0,     0,   346,   347,     0,   138,     0,   350,   138,   351,
     352,   132,     0,     0,   355,     0,     0,   132,     0,   147,
       0,     0,   362,   147,   132,   143,     0,     0,   366,   367,
     368,   150,   138,   150,     0,     0,     0,     0,   150,     0,
     587,   150,     0,   138,   590,   138,     0,   150,     0,     0,
     150,     0,     0,   143,     0,     0,     0,     0,     0,   143,
       0,     0,     0,     0,     0,     0,   143,     0,     0,     0,
       0,     0,   447,     0,   150,     0,     0,     0,   132,     0,
       0,     0,   132,     0,     0,   150,   147,   150,     0,     0,
     132,     0,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   383,     0,   220,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     143,     0,   221,   222,   143,   223,     0,     0,     0,     0,
     224,   678,   143,   138,     0,     0,   384,   690,   225,  -547,
     138,  -547,     0,   148,   226,     0,     0,     0,     0,   227,
       0,   138,   228,   148,     0,   229,     0,     0,     0,     0,
       0,     0,     0,   230,   147,   138,   148,     0,   148,   231,
     232,   147,     0,     0,   138,   150,   233,     0,   138,     0,
       0,     0,   150,   342,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   150,   235,   236,     0,   237,     0,   238,
     132,   239,     0,     0,   240,     0,     0,   150,   241,   445,
       0,   242,   728,   148,   243,   731,   150,     0,     0,     0,
     150,     0,     0,     0,   138,     0,     0,     0,     0,     0,
       0,   147,     0,     0,   343,     0,     0,     0,   344,   152,
     148,     0,   143,     0,   148,   147,     0,     0,     0,   152,
       0,     0,   132,   764,     0,     0,     0,   447,   132,     0,
       0,     0,   152,     0,   152,     0,   150,     0,     0,     0,
     446,     0,     0,     0,     0,     0,   138,   138,   138,   138,
     787,     0,     0,     0,   298,   346,   347,     0,   348,   349,
     350,     0,   351,   352,   143,     0,     0,   355,     0,     0,
     143,   138,   138,     0,   361,   362,     0,   148,   365,   152,
       0,   366,   367,   368,     0,     0,     0,   147,   150,   150,
     150,   150,     0,   132,   855,   132,     0,     0,     0,     0,
     132,     0,     0,   132,     0,     0,   152,     0,     0,   132,
     152,     0,   132,   150,   150,   147,     0,     0,   266,     0,
       0,   147,    22,    23,     0,     0,     0,     0,   147,     0,
       0,   267,     0,    30,   268,   143,   132,   143,    35,     0,
       0,     0,   143,     0,    39,   143,     0,   132,     0,   132,
       0,   143,     0,     0,   143,   148,   900,     0,     0,   447,
       0,     0,   148,     0,     0,     0,    52,     0,    54,     0,
      56,     0,   936,   152,     0,   937,     0,   269,   143,    62,
       0,   787,   147,     0,     0,     0,   147,     0,     0,   143,
       0,   143,     0,     0,   147,     0,     0,     0,    78,     0,
       0,    80,     0,   690,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,   148,     0,     0,     0,     0,     0,     0,   946,
       0,     0,     0,     0,     0,    15,   148,   132,    17,     0,
       0,     0,     0,    21,   132,     0,     0,   153,     0,     0,
       0,   152,     0,   172,    97,   132,     0,   153,   152,     0,
     938,     0,    37,     0,     0,     0,     0,     0,    40,   132,
     153,     0,   153,     0,     0,     0,    45,     0,   132,   143,
       0,     0,   132,    50,     0,     0,   143,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   143,    60,     0,
       0,     0,     0,     0,   147,     0,     0,     0,   148,     0,
      70,   143,     0,     0,    73,     0,     0,   153,   152,     0,
     143,    79,     0,     0,   143,     0,     0,    83,   132,     0,
       0,     0,   152,     0,     0,     0,   148,     0,     0,     0,
       0,     0,   148,     0,   153,   855,     0,     0,   153,   148,
       0,     0,     0,     0,     0,     0,   147,     0,     0,     0,
       0,     0,   147,     0,     0,     0,     0,     0,     0,     0,
     143,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     132,   132,   132,   132,     0,   104,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   163,     0,     0,     0,     0,
       0,     0,     0,   148,   152,   132,   132,   148,   195,     0,
     198,   153,     0,     0,     0,   148,     0,     0,     0,     0,
       0,     0,   143,   143,   143,   143,     0,   147,     0,   147,
       0,     0,   152,     0,   147,     0,     0,   147,   152,     0,
       0,     0,     0,   147,     0,   152,   147,   143,   143,     0,
       0,     0,     0,     0,     0,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     147,     0,     0,     0,     0,     0,   220,     0,     0,     0,
       0,   147,   104,   147,     0,   342,   324,     0,     0,   153,
     221,   222,     0,   223,     0,     0,   153,     0,   224,   152,
       0,     0,     0,   152,     0,     0,   225,     0,     0,     0,
       0,   152,   226,     0,     0,     0,     0,   227,     0,     0,
     228,     0,     0,   229,     0,   148,     0,     0,     0,     0,
       0,   230,     0,     0,     0,     0,   343,   231,   232,     0,
     344,     0,     0,     0,   233,     0,     0,     0,     0,   391,
       0,     0,     0,   234,     0,     0,   153,     0,     0,     0,
       0,     0,   235,   236,     0,   237,     0,   238,     0,   239,
     153,   147,   240,     0,     0,     0,   241,   148,   147,   242,
       0,     0,   243,   148,     0,     0,     0,   346,   347,   147,
     348,   349,   350,     0,   351,   352,   353,     0,   354,   355,
     356,   357,     0,   147,   359,   360,   361,   362,   363,     0,
     365,     0,   147,   366,   367,   368,   147,     0,     0,     0,
       0,   152,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,   455,     0,   397,     0,   785,     0,
       0,     0,   153,     0,     0,     0,     0,     0,   148,     0,
     148,     0,     0,     0,     0,   148,     0,     0,   148,     0,
       0,     0,   147,     0,   148,     0,     0,   148,     0,     0,
     153,     0,     0,   152,     0,     0,   153,     0,     0,   152,
       0,     0,     0,   153,     0,     0,     0,     0,     0,     0,
       0,   148,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,   148,     0,   148,   266,     0,     0,   104,    22,
      23,     0,     0,     0,   147,   147,   147,   147,   267,     0,
      30,   268,     0,     0,     0,    35,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,   153,     0,   147,
     147,   153,     0,     0,   152,     0,   152,     0,     0,   153,
       0,   152,     0,    52,   152,    54,     0,    56,     0,   936,
     152,     0,   937,   152,   269,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104,     0,     0,     0,     0,    78,     0,   152,    80,     0,
       0,    82,   148,     0,   342,     0,     0,     0,   152,   148,
     152,     0,     0,     0,     0,     0,     0,     0,   455,     0,
     148,     0,     0,     0,   455,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   148,     0,     0,     0,     0,     0,
       0,     0,     0,   148,     0,     0,     0,   148,     0,     0,
       0,    97,     0,     0,     0,   343,     0,  1021,     0,   344,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   153,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   665,     0,     0,     0,   670,
       0,     0,     0,   148,     0,     0,     0,   104,   152,     0,
       0,     0,     0,     0,     0,   152,   346,   347,     0,   348,
     349,   350,     0,   351,   352,   353,   152,     0,   355,   356,
     357,   153,     0,   359,   360,   361,   362,   153,     0,   365,
     152,     0,   366,   367,   368,     0,   266,     0,     0,   152,
      22,    23,     0,   152,     0,   148,   148,   148,   148,   267,
       0,    30,   268,     0,     0,     0,    35,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,     0,     0,
     148,   148,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,    54,     0,    56,   152,
     936,     0,   153,   937,   153,   269,     0,    62,     0,   153,
       0,     0,   153,     0,     0,     0,     0,   755,   153,   220,
       0,   153,     0,     0,     0,     0,    78,     0,     0,    80,
       0,     0,    82,   221,   222,     0,   223,     0,     0,     0,
       0,   224,     0,     0,     0,   153,     0,     0,     0,   225,
       0,   152,   152,   152,   152,   226,   153,     0,   153,     0,
     227,     0,     0,   228,     0,     0,   229,     0,     0,   790,
       0,     0,     0,     0,   230,   795,   152,   152,     0,     0,
     231,   232,    97,     0,     0,     0,     0,   233,  1023,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,   235,   236,     0,   237,     0,
     238,     0,   239,     0,     0,   240,     0,     0,     0,   241,
       0,     0,   242,     0,     0,   243,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     455,     0,   455,     0,     0,     0,   153,   455,     0,     0,
     455,     0,     0,   153,     0,     0,   896,     0,     0,   898,
       0,     0,   220,     0,   153,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,   222,   153,   223,
       0,   247,     0,   914,   224,     0,     0,   153,     0,     0,
       0,   153,   225,     0,   920,   266,   924,     0,   226,    22,
      23,     0,     0,   227,     0,     0,   228,     0,   267,   229,
      30,   268,     0,     0,     0,    35,     0,   230,     0,     0,
       0,    39,     0,   231,   232,     0,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,   153,     0,   234,
       0,     0,     0,    52,     0,    54,     0,     0,   235,   236,
       0,   237,     0,   238,   269,   239,    62,     0,   240,     0,
       0,     0,   241,     0,     0,   242,     0,     0,   243,     0,
       0,     0,     0,     0,     0,    78,     0,     0,    80,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,   153,
     153,   153,   153,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   973,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   153,   153,   992,     0,     0,     0,
       0,     0,     0,     0,   763,   997,     0,     0,     0,   999,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    -2,
       4,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,  1028,    30,    31,    32,    33,
      34,    35,    36,    37,   -62,     0,    38,    39,     0,    40,
    -241,     0,    41,    42,    43,     0,    44,    45,    46,   -44,
      47,    48,     0,    49,    50,    51,     0,     0,     0,    52,
      53,    54,    55,    56,    57,    58,  -241,   -44,    59,    60,
      61,     0,    62,    63,    64,     0,    65,    66,    67,    68,
      69,    70,    71,    72,     0,    73,    74,     0,    75,    76,
      77,    78,    79,     0,    80,    81,   -62,    82,    83,     0,
       0,    84,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      98,     0,    99,     0,   100,  1097,   101,     5,   271,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   170,     0,
       0,    15,    16,     0,    17,     0,   171,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   172,
       0,     0,     0,    32,   173,   174,     0,   175,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   176,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   177,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,   272,     0,     0,     0,    98,     0,   178,     0,   100,
       0,   179,  1098,     4,     0,     5,     0,     6,     7,     8,
       9,    10,    11,     0,  -575,     0,    12,    13,    14,    15,
      16,  -575,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,  -575,    27,    28,  -575,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -62,     0,    38,
      39,     0,    40,  -241,     0,    41,    42,    43,     0,    44,
      45,    46,   -44,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,     0,    57,    58,  -241,
     -44,    59,    60,    61,  -575,    62,    63,    64,  -575,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -62,
      82,    83,     0,     0,    84,     0,    85,     0,     0,  -575,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -575,  -575,    89,  -575,  -575,
    -575,  -575,  -575,  -575,  -575,     0,  -575,  -575,  -575,  -575,
    -575,     0,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,
      97,  -575,  -575,  -575,     0,    99,  -575,   100,   308,   101,
       5,   271,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,   -62,     0,    38,    39,     0,    40,  -241,     0,
      41,    42,    43,     0,    44,    45,    46,   -44,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,    56,    57,    58,  -241,   -44,    59,    60,    61,     0,
      62,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -62,    82,    83,     0,     0,    84,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,   272,    97,     0,     0,    98,     0,
      99,   309,   100,     4,   101,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -62,     0,    38,
      39,     0,    40,  -241,     0,    41,    42,    43,     0,    44,
      45,    46,   -44,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,    56,    57,    58,  -241,
     -44,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -62,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,   483,   100,   502,   101,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,   -62,     0,    38,    39,     0,    40,  -241,     0,
      41,    42,    43,     0,    44,    45,    46,   -44,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,    56,    57,    58,  -241,   -44,    59,    60,    61,     0,
      62,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -62,    82,    83,     0,     0,    84,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,   503,   100,   308,   101,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -62,     0,    38,
      39,     0,    40,  -241,     0,    41,    42,    43,     0,    44,
      45,    46,   -44,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,    56,    57,    58,  -241,
     -44,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -62,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,   309,   100,     4,   101,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,   -62,     0,    38,    39,     0,    40,  -241,     0,
      41,    42,    43,     0,    44,    45,    46,   -44,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,    56,    57,    58,  -241,   -44,    59,    60,    61,     0,
      62,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -62,    82,    83,     0,     0,    84,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,   677,   100,     4,   101,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -62,     0,    38,
      39,     0,    40,  -241,     0,    41,    42,    43,     0,    44,
      45,    46,   -44,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,   323,    57,    58,  -241,
     -44,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -62,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     4,   100,     5,   101,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     -62,     0,    38,    39,     0,    40,  -241,     0,    41,    42,
      43,     0,    44,    45,    46,   -44,    47,    48,     0,    49,
      50,    51,     0,     0,     0,    52,    53,    54,    55,     0,
      57,    58,  -241,   -44,    59,    60,    61,     0,    62,    63,
      64,     0,    65,    66,    67,    68,    69,    70,    71,    72,
       0,    73,    74,     0,    75,    76,    77,    78,    79,     0,
      80,    81,   -62,    82,    83,     0,     0,    84,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,     4,
     100,     5,   101,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   595,    37,   -62,     0,    38,    39,     0,    40,  -241,
       0,    41,    42,    43,     0,    44,    45,    46,   -44,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,     0,    57,    58,  -241,   -44,    59,    60,    61,
       0,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -62,    82,    83,     0,     0,
      84,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,     4,   100,     5,   101,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   601,    37,   -62,     0,    38,    39,
       0,    40,  -241,     0,    41,    42,    43,     0,    44,    45,
      46,   -44,    47,    48,     0,    49,    50,    51,     0,     0,
       0,    52,    53,    54,    55,     0,    57,    58,  -241,   -44,
      59,    60,    61,     0,    62,    63,    64,     0,    65,    66,
      67,    68,    69,    70,    71,    72,     0,    73,    74,     0,
      75,    76,    77,    78,    79,     0,    80,    81,   -62,    82,
      83,     0,     0,    84,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,    98,     0,    99,     4,   100,     5,   101,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   882,    37,   -62,
       0,    38,    39,     0,    40,  -241,     0,    41,    42,    43,
       0,    44,    45,    46,   -44,    47,    48,     0,    49,    50,
      51,     0,     0,     0,    52,    53,    54,    55,     0,    57,
      58,  -241,   -44,    59,    60,    61,     0,    62,    63,    64,
       0,    65,    66,    67,    68,    69,    70,    71,    72,     0,
      73,    74,     0,    75,    76,    77,    78,    79,     0,    80,
      81,   -62,    82,    83,     0,     0,    84,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,    97,     0,     0,    98,     0,    99,     4,   100,
       5,   101,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
     884,    37,   -62,     0,    38,    39,     0,    40,  -241,     0,
      41,    42,    43,     0,    44,    45,    46,   -44,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,     0,    57,    58,  -241,   -44,    59,    60,    61,     0,
      62,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,    72,     0,    73,    74,     0,    75,    76,    77,    78,
      79,     0,    80,    81,   -62,    82,    83,     0,     0,    84,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,     4,   100,     5,   101,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   889,    37,   -62,     0,    38,    39,     0,
      40,  -241,     0,    41,    42,    43,     0,    44,    45,    46,
     -44,    47,    48,     0,    49,    50,    51,     0,     0,     0,
      52,    53,    54,    55,     0,    57,    58,  -241,   -44,    59,
      60,    61,     0,    62,    63,    64,     0,    65,    66,    67,
      68,    69,    70,    71,    72,     0,    73,    74,     0,    75,
      76,    77,    78,    79,     0,    80,    81,   -62,    82,    83,
       0,     0,    84,     0,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,    97,     0,
       0,    98,     0,    99,     4,   100,     5,   101,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,   892,    37,   -62,     0,
      38,    39,     0,    40,  -241,     0,    41,    42,    43,     0,
      44,    45,    46,   -44,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,     0,    57,    58,
    -241,   -44,    59,    60,    61,     0,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -62,    82,    83,     0,     0,    84,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,     4,   100,     5,
     101,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,   913,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -62,     0,    38,    39,     0,    40,  -241,     0,    41,
      42,    43,     0,    44,    45,    46,   -44,    47,    48,     0,
      49,    50,    51,     0,     0,     0,    52,    53,    54,    55,
       0,    57,    58,  -241,   -44,    59,    60,    61,     0,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,     0,    73,    74,     0,    75,    76,    77,    78,    79,
       0,    80,    81,   -62,    82,    83,     0,     0,    84,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
       4,   100,     5,   101,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,   919,    37,   -62,     0,    38,    39,     0,    40,
    -241,     0,    41,    42,    43,     0,    44,    45,    46,   -44,
      47,    48,     0,    49,    50,    51,     0,     0,     0,    52,
      53,    54,    55,     0,    57,    58,  -241,   -44,    59,    60,
      61,     0,    62,    63,    64,     0,    65,    66,    67,    68,
      69,    70,    71,    72,     0,    73,    74,     0,    75,    76,
      77,    78,    79,     0,    80,    81,   -62,    82,    83,     0,
       0,    84,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      98,     0,    99,     4,   100,     5,   101,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   923,    37,   -62,     0,    38,
      39,     0,    40,  -241,     0,    41,    42,    43,     0,    44,
      45,    46,   -44,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,     0,    57,    58,  -241,
     -44,    59,    60,    61,     0,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
       0,    75,    76,    77,    78,    79,     0,    80,    81,   -62,
      82,    83,     0,     0,    84,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     0,   100,   773,   101,
       5,   271,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   170,     0,     0,    15,    16,     0,    17,     0,   171,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   172,     0,     0,     0,    32,   173,   174,     0,
     175,    37,     0,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   176,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   177,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,   272,     0,     0,     0,    98,     0,
     178,     0,   100,     0,   179,     5,   271,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   170,     0,     0,    15,
      16,     0,    17,     0,   171,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   172,     0,     0,
       0,    32,   173,   174,     0,   175,    37,     0,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   176,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   177,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   272,
       0,     0,     0,    98,     0,   178,     0,   100,     0,   179,
     669,     5,   271,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   170,     0,     0,    15,    16,     0,    17,     0,
     171,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   172,     0,     0,     0,    32,   173,   174,
       0,   175,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   176,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   177,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   272,     0,     0,     0,    98,
       0,   178,     0,   100,     0,   179,  1031,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   170,     0,
       0,    15,    16,     0,    17,     0,   171,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   172,
       0,     0,     0,    32,   173,   174,     0,   175,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   176,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   177,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   178,     0,   100,
       0,   179,   674,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   170,     0,     0,    15,    16,     0,
      17,     0,   171,     0,     0,    21,   215,    23,     0,     0,
       0,     0,     0,    28,     0,   172,     0,     0,     0,    32,
     173,   174,     0,   175,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,    54,    55,     0,    57,     0,     0,     0,     0,
      60,   176,     0,    62,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   177,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,    82,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   178,     0,   100,     0,   179,     5,   271,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   170,
       0,     0,    15,    16,     0,    17,     0,   171,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     172,     0,     0,     0,    32,   173,   174,     0,   175,    37,
       0,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   176,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   177,
       0,    73,     0,     0,    75,   276,   277,     0,    79,   313,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,   314,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,   272,     0,     0,     0,    98,     0,   178,     0,
     100,     0,   179,     5,   271,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   170,     0,     0,    15,    16,     0,
      17,     0,   171,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   172,     0,     0,     0,    32,
     173,   174,     0,   175,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   176,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   177,     0,    73,     0,     0,    75,
     276,   277,     0,    79,   313,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,   272,     0,     0,
       0,    98,     0,   178,     0,   100,   661,   179,     5,   271,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   170,
       0,     0,    15,    16,     0,    17,     0,   171,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     172,     0,     0,     0,    32,   173,   174,     0,   175,    37,
       0,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   176,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   177,
       0,    73,     0,     0,    75,   276,   277,     0,    79,   313,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,   272,     0,     0,     0,    98,     0,   178,     0,
     100,   663,   179,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   170,     0,     0,    15,    16,     0,
      17,     0,   171,     0,     0,    21,     0,   560,     0,     0,
       0,     0,     0,    28,     0,   172,     0,     0,     0,    32,
     173,   174,     0,   175,    37,     0,   561,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   176,     0,   562,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   177,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,   563,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   178,     0,   100,     5,   179,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   170,     0,     0,
      15,    16,     0,    17,     0,   171,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   172,     0,
       0,     0,    32,   173,   174,     0,   175,    37,     0,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   176,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   177,     0,    73,
       0,     0,    75,   276,   277,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,   278,     0,     0,    98,     0,   178,     0,   100,     0,
     179,     5,   271,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   170,     0,     0,    15,    16,     0,    17,     0,
     171,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   172,     0,     0,     0,    32,   173,   174,
       0,   175,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   176,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   177,     0,    73,     0,     0,    75,   276,   277,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   272,     0,     0,     0,    98,
       0,   178,     0,   100,     5,   179,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   170,     0,     0,    15,    16,
       0,    17,     0,   171,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   172,     0,     0,     0,
      32,   173,   174,     0,   175,    37,     0,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,   775,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   176,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   177,     0,    73,     0,     0,
      75,   276,   277,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   811,     0,   812,    87,    88,    89,    90,    91,     0,
       0,     0,   813,    92,     0,     0,     0,   814,   222,   815,
     816,     0,     0,    93,     0,   817,    94,    95,    96,     0,
       0,     0,    98,   225,   178,   172,   100,     0,   179,   818,
       0,     0,     0,     0,   819,     0,     0,   228,     0,     0,
     820,     0,   821,     0,     0,     0,     0,     0,   822,     0,
       0,     0,     0,     0,   823,   824,     0,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,     0,
     825,     0,     0,     0,     0,     0,     0,     0,     0,   235,
     236,     0,   826,     0,   238,     0,   827,     0,     0,   828,
       0,     0,     0,   829,     0,     0,   242,     0,     0,   830,
       0,     0,     0,     0,     0,     0,     0,     0,   369,   370,
     371,   372,   373,   374,     0,     0,   377,   378,   379,   380,
       0,   382,   383,   831,   832,   833,   834,   835,     0,     0,
     836,     0,     0,     0,   837,   838,   839,   840,   841,   842,
     843,   844,   845,   846,   847,     0,   848,     0,     0,   849,
     850,   851,   852,     0,     5,   853,     6,     7,     8,   204,
      10,    11,   205,     0,     0,   170,     0,     0,    15,    16,
       0,    17,     0,   171,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   172,     0,     0,     0,
      32,   173,   174,     0,   175,    37,     0,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   206,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   177,     0,    73,     0,     0,
      75,     0,     0,   207,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   178,     0,   100,     5,   179,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   170,     0,
       0,    15,    16,     0,    17,     0,   171,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   172,
       0,     0,     0,    32,   173,   174,     0,   175,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,   209,    55,     0,    57,
       0,     0,     0,     0,    60,   176,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   177,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,   210,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   178,     0,   100,
       0,   179,     5,   271,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   170,     0,     0,    15,    16,     0,    17,
       0,   171,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   172,     0,     0,     0,    32,   173,
     174,     0,   175,    37,     0,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     176,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   177,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,   210,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,   272,     0,     0,     0,
      98,     0,   178,     0,   100,     5,   179,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   170,     0,     0,    15,
      16,     0,    17,     0,   171,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   172,     0,     0,
       0,    32,   173,   174,     0,   175,    37,     0,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   176,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   177,     0,    73,     0,
       0,    75,   276,   277,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   178,     0,   100,     0,   179,
       5,   271,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   170,     0,     0,    15,    16,     0,    17,     0,   171,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   172,     0,     0,     0,    32,   173,   174,     0,
     175,    37,     0,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   176,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   177,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,   272,     0,     0,     0,    98,     0,
     178,   659,   100,     5,   179,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   170,     0,     0,    15,    16,     0,
      17,     0,   171,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   172,     0,     0,     0,    32,
     173,   174,     0,   175,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   176,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   177,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,   210,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   178,     0,   100,     5,   179,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   170,     0,     0,
      15,    16,     0,    17,     0,   171,     0,     0,    21,     0,
       0,     0,     0,     0,     0,   251,    28,     0,   172,     0,
       0,     0,    32,   173,   174,     0,   175,    37,     0,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   176,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   177,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,    99,     0,   100,     0,
     179,     5,   271,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   170,     0,     0,    15,    16,     0,    17,     0,
     171,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   172,     0,     0,     0,    32,   173,   174,
       0,   175,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   176,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   177,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,   272,     0,     0,     0,    98,
       0,   178,     0,   100,     5,   179,     6,     7,     8,   204,
      10,    11,     0,     0,     0,   170,     0,     0,    15,    16,
       0,    17,     0,   171,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   172,     0,     0,     0,
      32,   173,   174,     0,   175,    37,     0,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   206,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   177,     0,    73,     0,     0,
      75,     0,     0,   207,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   178,     0,   100,     0,   179,     5,
     271,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     170,     0,     0,    15,    16,     0,    17,     0,   171,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   633,
       0,   172,     0,     0,     0,    32,   173,   174,     0,   175,
      37,     0,     0,   634,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   176,     0,     0,
       0,     0,     0,     0,     0,    67,   635,    69,    70,    71,
     636,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,   272,     0,     0,     0,    98,     0,   178,
       0,   100,     0,   976,     5,   271,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   170,     0,     0,    15,    16,
       0,    17,     0,   171,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   172,     0,     0,     0,
      32,   173,   174,     0,   175,    37,     0,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   176,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   177,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,   272,     0,
       0,     0,    98,     0,   178,     0,   100,     0,   976,     5,
     271,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     170,     0,     0,    15,    16,     0,    17,     0,   171,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   172,     0,     0,     0,    32,   173,   174,     0,   960,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   176,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     177,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,   272,     0,     0,     0,    98,     0,   178,
       0,   100,     5,   179,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   170,     0,     0,    15,    16,     0,    17,
       0,   171,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   172,     0,     0,     0,    32,   173,
     174,     0,   175,    37,     0,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     176,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   177,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   178,     0,   100,     5,   179,     6,     7,     8,
     200,    10,    11,     0,     0,     0,   170,     0,     0,    15,
      16,     0,    17,     0,   171,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   172,     0,     0,
       0,    32,   173,   174,     0,   175,    37,     0,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   201,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   177,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   178,     0,   100,     5,   179,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   170,
       0,     0,    15,    16,     0,    17,     0,   171,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     172,     0,     0,     0,    32,   173,   174,     0,   175,    37,
       0,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   176,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   177,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,    99,     0,
     100,     5,   179,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   170,     0,     0,    15,    16,     0,    17,     0,
     171,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   172,     0,     0,     0,    32,   173,   174,
       0,   175,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   176,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   177,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,   606,
       0,   178,     0,   100,     5,   179,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   170,     0,     0,    15,    16,
       0,    17,     0,   171,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   633,     0,   172,     0,     0,     0,
      32,   173,   174,     0,   175,    37,     0,     0,   634,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   176,     0,     0,     0,     0,     0,     0,     0,
      67,   635,    69,    70,    71,   636,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   178,     0,   100,     5,   637,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   170,     0,
       0,    15,    16,     0,    17,     0,   171,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   172,
       0,     0,     0,    32,   173,   174,     0,   175,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   176,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   177,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   178,     0,   100,
       5,   637,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   170,     0,     0,    15,    16,     0,    17,     0,   171,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   172,     0,     0,     0,    32,   173,   174,     0,
     714,    37,     0,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   176,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   177,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     178,     0,   100,     5,   179,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   170,     0,     0,    15,    16,     0,
      17,     0,   171,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   172,     0,     0,     0,    32,
     173,   174,     0,   717,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   176,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   177,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   178,     0,   100,     5,   179,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   170,     0,     0,
      15,    16,     0,    17,     0,   171,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   172,     0,
       0,     0,    32,   173,   174,     0,   954,    37,     0,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   176,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   177,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   178,     0,   100,     5,
     179,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     170,     0,     0,    15,    16,     0,    17,     0,   171,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   172,     0,     0,     0,    32,   173,   174,     0,   955,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   176,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     177,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   178,
       0,   100,     5,   179,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   170,     0,     0,    15,    16,     0,    17,
       0,   171,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   172,     0,     0,     0,    32,   173,
     174,     0,   957,    37,     0,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     176,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   177,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   178,     0,   100,     5,   179,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   170,     0,     0,    15,
      16,     0,    17,     0,   171,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   172,     0,     0,
       0,    32,   173,   174,     0,   958,    37,     0,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   176,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   177,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   178,     0,   100,     5,   179,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   170,
       0,     0,    15,    16,     0,    17,     0,   171,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     172,     0,     0,     0,    32,   173,   174,     0,   959,    37,
       0,     0,    38,     0,     0,    40,     0,     0,     0,    42,
      43,     0,     0,    45,    46,     0,    47,    48,     0,    49,
      50,     0,     0,     0,     0,     0,    53,     0,    55,     0,
      57,     0,     0,     0,     0,    60,   176,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,   177,
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   178,     0,
     100,     5,   179,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   170,     0,     0,    15,    16,     0,    17,     0,
     171,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   172,     0,     0,     0,    32,   173,   174,
       0,   960,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   176,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   177,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   178,     0,   100,     5,   179,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   170,     0,     0,    15,    16,
       0,    17,     0,   171,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   633,     0,   172,     0,     0,     0,
      32,   173,   174,     0,   175,    37,     0,     0,   634,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   176,     0,     0,     0,     0,     0,     0,     0,
      67,   635,    69,    70,    71,   636,     0,    73,     0,     0,
      75,     0,     0,     0,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   178,     0,   100,     5,  1053,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   170,     0,
       0,    15,    16,     0,    17,     0,   171,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   172,
       0,     0,     0,    32,   173,   174,     0,   175,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   176,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   177,     0,
      73,     0,     0,    75,     0,     0,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,     0,     0,   100,
       5,  1053,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   170,     0,     0,    15,    16,     0,    17,     0,   171,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   172,     0,     0,     0,    32,   173,   174,     0,
     175,    37,     0,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   176,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   177,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,  -509,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,   384,    95,    96,  -547,     0,  -547,    98,     0,
     178,     0,   100,     0,   179,     5,   271,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   170,     0,     0,    15,
      16,   342,    17,     0,   171,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   172,     0,     0,
       0,    32,   173,   174,     0,   175,    37,     0,     0,    38,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,   343,    53,     0,    55,   344,    57,     0,     0,
       0,     0,    60,   176,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,    70,    71,   177,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,     0,
    -406,     0,     0,   346,   347,     0,   348,   349,   350,     0,
     351,   352,   353,     0,     0,   355,     0,    89,     0,     0,
       0,  -406,   361,   362,     0,  -406,   365,     0,     0,   366,
     367,   368,     0,     0,     0,     0,     0,     0,     0,   272,
       0,     0,     0,     0,     0,   178,     0,   100,  -406,   976,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   170,     0,     0,    15,    16,     0,    17,     0,   171,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   172,     0,     0,     0,    32,   173,   174,     0,
     175,    37,     0,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,  -388,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   176,     0,
       0,     0,  -388,     0,     0,     0,    67,    68,    69,    70,
      71,   177,     0,    73,     0,  -388,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,  -388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,    89,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   170,     0,     0,    15,    16,     0,    17,     0,
     171,     0,     0,    21,    96,  -388,     0,     0,     0,     0,
    -388,    28,   100,   172,  1053,     0,     0,    32,   173,   174,
       0,   175,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   176,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   177,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,     5,    89,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   170,     0,     0,    15,    16,     0,    17,
       0,   171,     0,     0,    21,    96,     0,     0,     0,     0,
       0,   178,    28,   100,   172,   179,     0,     0,    32,   173,
     174,     0,   175,    37,     0,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     176,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   177,     0,    73,     0,     0,    75,     0,
       0,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     5,    89,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   170,     0,     0,    15,    16,     0,
      17,     0,   171,     0,     0,    21,    96,     0,     0,     0,
       0,     0,   178,    28,   100,   172,   637,     0,     0,    32,
     173,   174,     0,   175,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,     0,     0,     0,
      60,   176,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   177,     0,    73,     0,  1010,    75,
       0,     0,     0,    79,     0,     0,    81,   813,     0,    83,
       0,     0,   221,   222,   815,   223,     0,     0,     0,     0,
     224,     0,     0,     0,     0,     0,     0,     0,   225,     0,
       0,     0,     0,     0,   818,    89,     0,     0,     0,   227,
       0,     0,   228,     0,     0,   229,     0,   821,     0,     0,
       0,     0,     0,   230,     0,     0,     0,     0,     0,   823,
     232,     0,     0,   340,     0,   100,   233,   179,     0,     0,
     341,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,   342,     0,   235,   236,     0,   237,     0,   238,
       0,  1011,     0,     0,   828,     0,     0,     0,   241,     0,
       0,   242,     0,     0,   243,     0,     0,     0,     0,     0,
       0,     0,     0,   369,   370,   371,   372,   373,   374,     0,
       0,   377,   378,   379,   380,     0,   382,   383,   831,   832,
     833,   834,   835,   343,     0,   836,     0,   344,     0,   837,
     838,   839,   840,   841,   842,   843,   844,   845,   846,   847,
       0,   848,   340,     0,   849,   850,   851,   852,     0,   341,
       0,     0,     0,     0,   428,     0,     0,     0,   345,     0,
       0,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,   340,
     366,   367,   368,     0,     0,     0,   341,     0,     0,   793,
       0,     0,   343,     0,     0,     0,   344,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   340,
       0,     0,     0,     0,     0,     0,   341,   345,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   342,     0,
       0,     0,     0,   346,   347,     0,   348,   349,   350,   343,
     351,   352,   353,   344,   354,   355,   356,   357,   358,     0,
     359,   360,   361,   362,   363,   364,   365,     0,   340,   366,
     367,   368,     0,     0,     0,   341,     0,     0,   873,     0,
       0,     0,     0,     0,   345,     0,     0,   342,     0,   343,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
     346,   347,     0,   348,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,   357,   358,     0,   359,   360,   361,
     362,   363,   364,   365,   345,     0,   366,   367,   368,     0,
       0,     0,     0,     0,     0,  1045,     0,     0,   343,     0,
     346,   347,   344,   348,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,   357,   358,     0,   359,   360,   361,
     362,   363,   364,   365,   340,     0,   366,   367,   368,     0,
       0,   341,     0,   345,     0,  1120,     0,     0,     0,     0,
       0,     0,     0,   342,     0,     0,     0,     0,     0,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,     0,
     354,   355,   356,   357,   358,     0,   359,   360,   361,   362,
     363,   364,   365,     0,     0,   366,   367,   368,     0,     0,
       0,   340,   660,     0,     0,     0,     0,     0,   341,     0,
       0,     0,     0,     0,   343,     0,     0,     0,   344,     0,
     342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   340,     0,     0,     0,     0,     0,     0,   341,   345,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     342,     0,     0,     0,     0,   346,   347,     0,   348,   349,
     350,   343,   351,   352,   353,   344,   354,   355,   356,   357,
     358,     0,   359,   360,   361,   362,   363,   364,   365,     0,
       0,   366,   367,   368,     0,     0,     0,     0,   777,     0,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,   343,     0,     0,     0,   344,     0,     0,     0,     0,
       0,     0,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,   357,   358,     0,   359,
     360,   361,   362,   363,   364,   365,   345,     0,   366,   367,
     368,     0,     0,   340,     0,   778,     0,     0,     0,     0,
     341,     0,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,   342,   354,   355,   356,   357,   358,     0,   359,
     360,   361,   362,   363,   364,   365,     0,     0,   366,   367,
     368,     0,     0,     0,     0,  1009,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,     0,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,   438,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,   881,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,   888,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,  1024,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,  1025,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,  1026,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,  1027,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,  1049,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,     0,     0,     0,  1050,   340,     0,     0,
       0,     0,     0,     0,   341,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   342,   431,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,   432,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,     0,   397,     0,     0,   343,     0,     0,
       0,   344,     0,   340,     0,     0,     0,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,   425,     0,     0,     0,     0,   428,     0,
       0,     0,   345,     0,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   346,   347,
       0,   348,   349,   350,     0,   351,   352,   353,     0,   354,
     355,   356,   357,   358,     0,   359,   360,   361,   362,   363,
     364,   365,     0,   343,   366,   367,   368,   344,   397,   340,
       0,     0,     0,     0,     0,     0,   341,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   342,   747,
       0,     0,     0,     0,     0,     0,     0,     0,   345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,   343,
     366,   367,   368,   344,   397,   340,     0,     0,     0,     0,
       0,     0,   341,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   342,   251,     0,     0,     0,     0,
     428,     0,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,   347,     0,   348,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,   357,   358,     0,   359,   360,   361,
     362,   363,   364,   365,     0,   343,   366,   367,   368,   344,
     397,   340,     0,     0,     0,     0,     0,     0,   341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     342,   739,     0,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,   340,     0,     0,     0,     0,     0,     0,
     341,     0,     0,     0,     0,     0,   346,   347,     0,   348,
     349,   350,   342,   351,   352,   353,     0,   354,   355,   356,
     357,   358,     0,   359,   360,   361,   362,   363,   364,   365,
       0,   343,   366,   367,   368,   344,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   340,     0,     0,     0,
       0,     0,     0,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,     0,   342,   345,   344,     0,     0,
       0,     0,     0,     0,     0,     0,   756,     0,     0,     0,
       0,     0,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,   357,   358,   345,   359,
     360,   361,   362,   363,   364,   365,     0,     0,   366,   367,
     368,     0,   397,     0,   346,   347,   343,   348,   349,   350,
     344,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,     0,   397,   340,     0,     0,     0,     0,
       0,   345,   341,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   342,     0,     0,   346,   347,   435,
     348,   349,   350,     0,   351,   352,   353,     0,   354,   355,
     356,   357,   358,     0,   359,   360,   361,   362,   363,   364,
     365,     0,     0,   366,   367,   368,     0,   468,   340,   684,
     436,     0,     0,     0,     0,   341,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,     0,   342,     0,   344,
       0,     0,   685,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,   686,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,   347,   343,   348,
     349,   350,   344,   351,   352,   353,     0,   354,   355,   356,
     357,   358,     0,   359,   360,   361,   362,   363,   364,   365,
       0,   437,   366,   367,   368,   340,     0,     0,     0,     0,
       0,     0,   341,   345,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   342,     0,     0,     0,     0,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,   399,
     354,   355,   356,   357,   358,     0,   359,   360,   361,   362,
     363,   364,   365,     0,   340,   366,   367,   368,     0,     0,
       0,   341,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   342,   569,   343,     0,     0,     0,   344,
       0,     0,     0,     0,     0,     0,     0,     0,   570,     0,
       0,     0,     0,     0,   340,   679,     0,     0,     0,     0,
       0,   341,     0,     0,     0,     0,   165,     0,     0,     0,
     345,     0,     0,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,   346,   347,   344,   348,
     349,   350,     0,   351,   352,   353,     0,   354,   355,   356,
     357,   358,     0,   359,   360,   361,   362,   363,   364,   365,
       0,   340,   366,   367,   368,     0,     0,     0,   341,   345,
       0,     0,     0,     0,   343,     0,     0,     0,   344,     0,
     342,   571,     0,     0,     0,   346,   347,     0,   348,   349,
     350,     0,   351,   352,   353,   572,   354,   355,   356,   357,
     358,     0,   359,   360,   361,   362,   363,   364,   365,   345,
       0,   366,   367,   368,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   346,   347,     0,   348,   349,
     350,   343,   351,   680,   353,   344,   354,   355,   356,   357,
     358,     0,   359,   360,   361,   362,   363,   364,   365,     0,
     681,   366,   367,   368,     0,     0,   340,     0,     0,     0,
       0,     0,     0,   341,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,   342,     0,     0,     0,     0,
     931,     0,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,   357,   358,     0,   359,
     360,   361,   362,   363,   364,   365,     0,     0,   366,   367,
     368,   932,     0,   340,     0,     0,     0,     0,     0,     0,
     341,     0,     0,     0,     0,     0,   343,     0,     0,     0,
     344,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   340,     0,     0,     0,     0,
       0,     0,   341,     0,     0,     0,     0,     0,     0,     0,
       0,   345,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   346,   347,     0,
     348,   349,   350,   343,   351,   352,   353,   344,   354,   355,
     356,   357,   358,     0,   359,   360,   361,   362,   363,   364,
     365,     0,   340,   366,   367,   368,     0,     0,     0,   341,
       0,     0,     0,     0,     0,   343,     0,     0,   345,   344,
       0,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
     345,   359,   360,   361,   362,   363,   364,   365,     0,   472,
     366,   367,   368,     0,     0,     0,   346,   347,     0,   348,
     349,   350,   343,   351,   352,   353,   344,   354,   355,   356,
     357,   358,     0,   359,   360,   361,   362,   363,   364,   365,
     340,   474,   366,   367,   368,     0,     0,   341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   345,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   346,   347,     0,   348,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,   357,   358,     0,
     359,   360,   361,   362,   363,   364,   365,   340,   476,   366,
     367,   368,     0,     0,   341,     0,     0,     0,     0,     0,
     343,     0,     0,     0,   344,     0,   342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   340,
       0,     0,     0,     0,     0,     0,   341,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   346,   347,     0,   348,   349,   350,   343,   351,   352,
     353,   344,   354,   355,   356,   357,   358,     0,   359,   360,
     361,   362,   363,   364,   365,     0,   481,   366,   367,   368,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,   498,   345,   344,     0,     0,     0,     0,     0,   340,
       0,     0,   573,     0,     0,     0,   341,     0,   346,   347,
       0,   348,   349,   350,     0,   351,   352,   353,   342,   354,
     355,   356,   357,   358,   345,   359,   360,   361,   362,   363,
     364,   365,     0,     0,   366,   367,   368,     0,     0,     0,
     346,   347,     0,   348,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,   357,   358,     0,   359,   360,   361,
     362,   363,   364,   365,     0,     0,   366,   367,   368,   343,
       0,     0,     0,   344,     0,   340,     0,     0,     0,     0,
       0,     0,   341,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,   340,     0,     0,
       0,     0,     0,     0,   341,     0,     0,     0,     0,     0,
     346,   347,     0,   348,   349,   350,   342,   351,   352,   353,
       0,   354,   355,   356,   357,   358,  -510,   359,   360,   361,
     362,   363,   364,   365,     0,   343,   366,   367,   368,   344,
       0,     0,     0,     0,     0,     0,   340,   751,     0,     0,
       0,     0,     0,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   342,   165,   343,     0,     0,
     345,   344,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,   347,     0,   348,
     349,   350,     0,   351,   352,   353,     0,   354,   355,   356,
     357,   358,   345,   359,   360,   361,   362,   363,   364,   365,
       0,     0,   366,   367,   368,     0,   343,     0,   346,   347,
     344,   348,   349,   350,     0,   351,   352,   353,     0,   354,
     355,   356,   357,   358,     0,   359,   360,   361,   362,   363,
     364,   365,   340,   724,   366,   367,   368,     0,     0,   341,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   342,     0,     0,   754,     0,     0,   346,   347,     0,
     348,   349,   350,     0,   351,   352,   353,     0,   354,   355,
     356,   357,   358,     0,   359,   360,   361,   362,   363,   364,
     365,   340,     0,   366,   367,   368,     0,     0,   341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     342,     0,   343,     0,     0,     0,   344,     0,     0,     0,
       0,     0,     0,   340,     0,     0,     0,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,   868,     0,     0,     0,   345,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,   346,   347,   344,   348,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,   357,   358,     0,
     359,   360,   361,   362,   363,   364,   365,     0,     0,   366,
     367,   368,     0,   343,     0,   796,   345,   344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,   357,   358,   345,   359,
     360,   361,   362,   363,   364,   365,     0,     0,   366,   367,
     368,     0,     0,     0,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,   340,  -222,
     366,   367,   368,     0,     0,   341,     0,     0,     0,     0,
       0,     0,     0,  -222,  -222,     0,  -222,   342,   871,     0,
       0,  -222,     0,     0,     0,     0,     0,     0,     0,  -222,
     340,   930,     0,     0,     0,  -222,     0,   341,     0,     0,
    -222,     0,     0,  -222,     0,     0,  -222,     0,     0,   342,
       0,     0,     0,     0,  -222,     0,     0,     0,     0,     0,
    -222,  -222,     0,     0,     0,     0,     0,  -222,   343,     0,
       0,     0,   344,     0,     0,     0,  -222,     0,     0,     0,
       0,     0,     0,     0,     0,  -222,  -222,     0,  -222,     0,
    -222,     0,  -222,     0,     0,  -222,     0,     0,     0,  -222,
     343,     0,  -222,   345,   344,  -222,   340,     0,     0,     0,
       0,     0,     0,   341,     0,     0,     0,     0,     0,   346,
     347,     0,   348,   349,   350,   342,   351,   352,   353,     0,
     354,   355,   356,   357,   358,   345,   359,   360,   361,   362,
     363,   364,   365,     0,     0,   366,   367,   368,     0,     0,
       0,   346,   347,     0,   348,   349,   350,     0,   351,   352,
     353,     0,   354,   355,   356,   357,   358,     0,   359,   360,
     361,   362,   363,   364,   365,     0,   343,   366,   367,   368,
     344,     0,     0,     0,     0,     0,   340,     0,     0,   953,
       0,     0,     0,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   342,     0,     0,     0,     0,
       0,   345,     0,     0,     0,     0,     0,     0,   340,     0,
       0,     0,     0,     0,     0,   341,     0,   346,   347,     0,
     348,   349,   350,     0,   351,   352,   353,   342,   354,   355,
     356,   357,   358,     0,   359,   360,   361,   362,   363,   364,
     365,     0,     0,   366,   367,   368,   343,     0,     0,     0,
     344,     0,     0,     0,     0,     0,     0,     0,     0,   956,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,     0,
       0,   345,   344,     0,     0,     0,     0,     0,   340,     0,
       0,  1062,     0,     0,     0,   341,     0,   346,   347,     0,
     348,   349,   350,     0,   351,   352,   353,   342,   354,   355,
     356,   357,   358,   345,   359,   360,   361,   362,   363,   364,
     365,     0,     0,   366,   367,   368,     0,     0,     0,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,     0,
     354,   355,   356,   357,   358,     0,   359,   360,   361,   362,
     363,   364,   365,     0,     0,   366,   367,   368,   343,     0,
       0,     0,   344,     0,     0,     0,     0,     0,   340,     0,
       0,  1063,     0,     0,     0,   341,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,   345,     0,     0,     0,     0,     0,     0,
     340,     0,     0,     0,     0,     0,     0,   341,     0,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,   342,
     354,   355,   356,   357,   358,     0,   359,   360,   361,   362,
     363,   364,   365,     0,     0,   366,   367,   368,   343,     0,
       0,     0,   344,     0,     0,     0,     0,     0,     0,     0,
       0,  1064,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,   345,   344,     0,     0,     0,     0,     0,
     340,     0,     0,  1065,     0,     0,     0,   341,     0,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,   342,
     354,   355,   356,   357,   358,   345,   359,   360,   361,   362,
     363,   364,   365,     0,     0,   366,   367,   368,     0,     0,
       0,   346,   347,     0,   348,   349,   350,     0,   351,   352,
     353,     0,   354,   355,   356,   357,   358,     0,   359,   360,
     361,   362,   363,   364,   365,     0,     0,   366,   367,   368,
     343,     0,     0,     0,   344,     0,     0,     0,     0,     0,
     340,     0,     0,  1066,     0,     0,     0,   341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   340,     0,     0,     0,     0,     0,     0,   341,
       0,   346,   347,     0,   348,   349,   350,     0,   351,   352,
     353,   342,   354,   355,   356,   357,   358,     0,   359,   360,
     361,   362,   363,   364,   365,     0,     0,   366,   367,   368,
     343,     0,     0,     0,   344,     0,  1091,     0,     0,     0,
       0,   340,     0,  1067,     0,     0,     0,     0,   341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     342,     0,   343,     0,     0,   345,   344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   346,   347,     0,   348,   349,   350,     0,   351,   352,
     353,     0,   354,   355,   356,   357,   358,   345,   359,   360,
     361,   362,   363,   364,   365,     0,     0,   366,   367,   368,
       0,   343,     0,   346,   347,   344,   348,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,   357,   358,     0,
     359,   360,   361,   362,   363,   364,   365,   340,     0,   366,
     367,   368,     0,     0,   341,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   342,     0,     0,     0,
       0,     0,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,   357,   358,     0,   359,
     360,   361,   362,   363,   364,   365,     0,     0,   366,   367,
     368,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   712,     0,     0,
       0,   344,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   346,   347,
       0,   348,   349,   350,     0,   351,   352,   353,     0,   354,
     355,   356,   357,   358,     0,   359,   360,   361,   362,   363,
     364,   365,     0,   266,   366,   367,   368,    22,    23,     0,
       0,     0,     0,     0,     0,     0,   267,   266,    30,   268,
       0,    22,    23,    35,     0,     0,     0,     0,     0,    39,
     267,     0,    30,   268,     0,     0,     0,    35,     0,     0,
       0,     0,     0,    39,     0,     0,     0,     0,     0,     0,
       0,    52,     0,    54,     0,    56,     0,   936,     0,     0,
     937,   342,   269,     0,    62,    52,     0,    54,     0,    56,
       0,   936,     0,     0,   937,   266,   269,     0,    62,    22,
      23,     0,     0,    78,     0,     0,    80,     0,   267,    82,
      30,   268,     0,   342,     0,    35,     0,    78,     0,     0,
      80,    39,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,     0,   344,     0,     0,     0,
       0,     0,     0,    52,     0,    54,     0,   323,     0,   936,
       0,     0,   937,     0,   269,     0,    62,     0,     0,    97,
       0,     0,     0,     0,   343,  1068,     0,   345,   344,     0,
       0,     0,     0,    97,     0,    78,     0,     0,    80,  1069,
       0,    82,     0,   346,   347,     0,   348,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,   357,     0,   345,
     359,   360,   361,   362,   363,   364,   365,     0,     0,   366,
     367,   368,     0,     0,     0,   346,   347,     0,   348,   349,
     350,     0,   351,   352,   353,     0,   354,   355,   356,   357,
       0,    97,   359,   360,   361,   362,   363,   689,   365,   220,
       0,   366,   367,   368,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   221,   222,     0,   223,     0,     0,     0,
       0,   224,     0,     0,     0,     0,     0,     0,     0,   225,
       0,     0,     0,     0,     0,   226,  -232,     0,     0,     0,
     227,     0,     0,   228,     0,     0,   229,     0,     0,     0,
    -232,  -232,     0,  -232,   230,     0,     0,     0,  -232,     0,
     231,   232,     0,     0,     0,     0,  -232,   233,     0,     0,
       0,     0,  -232,     0,     0,     0,   234,  -232,     0,     0,
    -232,     0,     0,  -232,     0,   235,   236,     0,   237,     0,
     238,  -232,   239,     0,   220,   240,     0,  -232,  -232,   241,
       0,     0,   242,     0,  -232,   243,     0,     0,   221,   222,
       0,   223,     0,  -232,     0,     0,   224,     0,     0,     0,
       0,     0,  -232,  -232,   225,  -232,     0,  -232,     0,  -232,
     226,  -223,  -232,     0,     0,   227,  -232,     0,   228,  -232,
       0,   229,  -232,     0,     0,  -223,  -223,     0,  -223,   230,
       0,     0,     0,  -223,     0,   231,   232,     0,     0,     0,
       0,  -223,   233,     0,     0,     0,     0,  -223,     0,     0,
       0,   234,  -223,     0,     0,  -223,     0,     0,  -223,     0,
     235,   236,     0,   237,     0,   238,  -223,   239,     0,  -154,
     240,     0,  -223,  -223,   241,     0,     0,   242,     0,  -223,
     243,     0,     0,  -154,  -154,     0,  -154,     0,  -223,     0,
       0,  -154,     0,     0,     0,     0,     0,  -223,  -223,  -154,
    -223,     0,  -223,     0,  -223,  -154,  -150,  -223,     0,     0,
    -154,  -223,     0,  -154,  -223,     0,  -154,  -223,     0,     0,
    -150,  -150,     0,  -150,  -154,     0,     0,     0,  -150,     0,
    -154,  -154,     0,     0,     0,     0,  -150,  -154,     0,     0,
       0,     0,  -150,     0,     0,     0,  -154,  -150,     0,     0,
    -150,     0,     0,  -150,     0,  -154,  -154,     0,  -154,     0,
    -154,  -150,  -154,     0,     0,  -154,     0,  -150,  -150,  -154,
       0,     0,  -154,     0,  -150,  -154,     0,     0,     0,     0,
       0,     0,     0,  -150,     0,   266,     0,     0,     0,    22,
      23,     0,  -150,  -150,     0,  -150,     0,  -150,   267,  -150,
      30,   268,  -150,     0,     0,    35,  -150,     0,   -63,  -150,
       0,    39,  -150,     0,  -241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,    54,     0,     0,     0,     0,
    -241,     0,     0,     0,   269,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    78,     0,     0,    80,     0,
     -63,    82
};

static const yytype_int16 yycheck[] =
{
      12,   329,    63,    59,    16,   150,   446,   477,    20,     2,
     191,   199,   451,   599,    26,   686,   772,    19,    30,    31,
     698,    33,    34,    35,    36,   219,   207,   214,   136,    46,
      22,   219,    44,   685,    46,    46,     1,   804,   477,    51,
     101,    53,   390,    55,    32,     1,    32,   903,    32,   858,
     859,    63,    64,    65,    66,    67,    68,   632,    45,     1,
      72,    85,    74,     3,    76,    77,   986,    46,     1,    81,
     264,    18,    84,    85,    68,    87,   264,    89,   443,     0,
      92,    93,    32,    95,   192,     3,    32,    99,   100,   101,
       3,   587,   139,   287,   590,   158,   113,    46,  1081,   287,
      51,    46,    20,   139,    83,     1,   900,   863,   436,   113,
     139,   158,    68,    46,    56,    33,   110,    46,   179,    46,
     107,    46,   139,   709,    71,   113,    77,   113,   139,     3,
     166,   113,   319,   320,   146,   164,   711,  1120,    83,   163,
      58,   139,    89,    85,   133,    85,     3,   100,   904,   294,
    1006,   168,   141,   118,   110,   139,    96,   168,   170,   106,
      56,   173,   174,   175,   352,   177,   178,   179,   166,   111,
     164,    85,   939,   138,   139,   163,    31,   163,    96,   163,
    1100,   975,   113,    96,   102,   118,   168,   199,   190,    85,
     139,   631,   386,   387,   769,   962,   138,   964,   386,   387,
    1031,   166,    98,  1012,  1013,   138,   139,   219,   164,   265,
     139,   203,   139,   163,   139,   111,   208,   163,   974,   168,
    1098,   163,    96,   215,    27,   158,   877,    82,   879,    33,
      34,    86,   728,    56,    64,   731,   139,   168,  1116,   168,
      83,   168,   138,   165,   158,   163,   199,   118,   139,   163,
    1081,   599,   264,  1020,   139,  1022,   268,   139,    45,   111,
     165,   932,   158,   166,   276,   277,   219,   163,   139,    72,
     100,   949,   143,    60,  1030,   287,   470,   168,   133,   931,
      83,   166,   470,   138,   166,   140,   141,   139,   111,     8,
     145,    78,    44,   133,   139,   166,   165,   101,   153,   102,
     886,   141,   314,   139,   159,   160,   161,   165,   416,   167,
     961,   264,   963,   421,    66,    27,   328,   329,   311,   133,
     107,   166,   158,   763,    76,    77,   338,   141,   340,   341,
     342,   343,   344,   345,   287,   347,   348,   349,   350,   351,
      31,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   139,   686,   118,
      72,   709,   423,   375,   376,    93,   139,   139,   165,   381,
     139,    83,   384,   385,   386,   387,   388,   389,   390,   139,
     139,   139,   164,    56,   143,   118,   158,   399,   163,   158,
     102,    82,   404,   166,   406,    86,   761,   585,   165,    78,
     167,    82,    56,   139,   139,    86,   166,   166,   166,   158,
     143,   423,    85,   425,   426,   164,   603,   604,    27,   431,
     432,   158,   158,   435,   436,    98,   438,   164,   440,   164,
     488,    85,   490,   386,   387,   158,   139,   189,   111,   804,
     910,   453,   133,   157,    98,   457,   458,   138,   509,    27,
     141,   463,    76,    77,   145,   158,   139,   111,   470,   211,
     212,   213,   214,    72,   903,   165,   218,   167,    66,   139,
     798,   910,  1036,   485,    83,   158,   488,   658,   490,   165,
     667,   668,   494,   495,   496,   158,   498,   499,   158,     3,
     163,   679,   680,   102,    72,   133,   203,   509,   158,   108,
     138,   208,   254,   141,   158,    83,    20,   145,   260,   163,
     463,   504,    82,   165,   158,   167,    86,   470,   165,    33,
     167,  1085,  1086,  1087,  1088,  1089,  1090,   158,   149,   158,
     108,   283,   485,    46,   165,   488,   167,   490,   886,   343,
     344,   158,   495,    56,    58,   375,   376,   158,   158,   301,
     158,   381,   158,   698,   384,   385,   163,   569,   570,   571,
     572,   573,   574,   575,   112,    81,    81,  1006,   165,   165,
     165,   583,    85,   585,   576,   577,   637,   165,   158,   944,
     165,   593,    96,   595,   388,   389,   118,   599,   102,   601,
     158,    56,  1031,     8,    60,   399,   138,   962,   111,   964,
     163,   141,   158,   139,   932,   118,  1045,   118,   166,   158,
      46,   165,    34,   211,   212,   213,   214,   139,   118,    27,
      85,   166,   426,   635,   636,   637,   139,   166,   432,   641,
     583,   166,   585,    98,    21,   166,   139,   163,    46,   168,
     168,    49,  1081,   166,     2,   158,   111,   165,   400,   158,
     163,   158,   158,   166,    12,  1020,   254,  1022,   488,  1098,
     490,    69,   260,    27,    72,   158,    74,    25,   158,    27,
     682,   168,   684,   685,   686,    83,   158,  1116,   430,   139,
     158,  1120,   434,    31,    48,   283,     2,   158,    34,   166,
     494,   164,   166,   158,   102,   139,    12,   709,   163,   118,
     712,   713,   714,   301,    83,   717,    70,    27,    72,    25,
     164,    27,   168,   166,    72,    32,   163,   911,   912,    83,
     781,    32,   163,   911,   912,   737,    46,   739,    34,    49,
      83,   139,   744,   138,    82,   747,   168,   165,    86,   751,
     165,    99,   754,   138,   108,   103,   158,   168,   168,    69,
     164,   158,    72,   583,    74,   158,    72,   141,   166,   165,
      76,    77,   133,    83,   141,   141,   570,   141,   572,   781,
     133,   141,   141,   141,   141,   141,   133,   133,   133,   791,
     141,   793,   102,    99,   796,   133,   798,   103,   163,   118,
     138,   141,   140,   141,   806,    34,   141,   145,   163,    98,
     133,   133,   400,   139,   949,   153,   166,   166,   166,   158,
      46,   641,   166,   161,   166,   144,   158,   146,   147,   157,
      56,   150,   151,   157,   168,    24,   325,   295,   785,   807,
     949,   944,   430,   761,   624,   810,   434,  1100,  1030,  1058,
    1091,   853,  1084,    35,   585,   597,   598,    30,    -1,    85,
     166,   603,   604,   454,    -1,   867,   868,    -1,   870,   871,
     872,   873,    -1,   615,    -1,    -1,    -1,    -1,    -1,   881,
     882,    -1,   884,    -1,   886,   111,   888,   889,    -1,    -1,
     892,   937,   118,    -1,    -1,    -1,   244,    -1,    -1,    -1,
      -1,    -1,    -1,   251,    -1,    -1,    -1,   909,   910,   911,
     912,    -1,   654,   139,    -1,   917,    -1,   919,   712,   713,
      27,   923,    -1,    -1,    -1,   976,    -1,    -1,   930,   931,
     932,    -1,   158,    -1,    -1,    -1,    -1,   163,   244,    46,
     166,    -1,    49,    -1,    -1,   251,    -1,    -1,    -1,    -1,
      -1,   953,   954,   955,   956,   957,   958,   959,   960,    -1,
     903,    -1,    69,   311,    -1,    72,    -1,    74,   911,   912,
      -1,    -1,    -1,    -1,   976,    -1,    83,   325,    -1,   981,
     722,   723,    -1,   985,    -1,    -1,    -1,    -1,     2,    -1,
      -1,    -1,   734,    -1,   736,   102,    -1,    -1,    12,    -1,
      -1,   743,  1053,    -1,   746,   311,    -1,   749,    -1,   597,
     598,    25,    -1,    27,    -1,   603,   604,    -1,    -1,   325,
      -1,    -1,  1024,  1025,  1026,  1027,    -1,    -1,    -1,    -1,
       2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   981,    -1,
      12,    -1,    -1,  1045,    -1,    -1,   788,  1049,  1050,   397,
      -1,  1053,    -1,    25,    -1,    27,    -1,    -1,    72,    -1,
    1062,  1063,  1064,  1065,  1066,  1067,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,   425,  1080,  1081,
      -1,  1083,    -1,   431,    -1,    99,    -1,    -1,  1031,   103,
     438,   397,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1116,    -1,    -1,    -1,  1120,   425,
      -1,    -1,    -1,    -1,    82,   431,    -1,    99,    86,    -1,
      -1,   103,   438,    -1,   722,   723,    -1,    -1,  1081,    -1,
      -1,    -1,    -1,    -1,   492,    -1,   734,    -1,   496,    -1,
      -1,    -1,   166,    -1,    -1,   129,   504,   131,   746,    -1,
      -1,   749,    -1,     1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   915,    -1,   133,   134,  1120,   136,   137,
     138,    -1,   140,   141,    22,    -1,   492,   145,    26,    27,
     496,    -1,    -1,    -1,   166,   153,    -1,    35,   504,    37,
      38,   159,   160,   161,    42,    -1,    -1,    31,    -1,    -1,
      48,    46,    -1,    51,    -1,    -1,    -1,   191,   566,    -1,
      -1,    56,    -1,    -1,   966,   967,    -1,    -1,   970,   971,
     244,    -1,    70,   207,    72,   209,    74,   251,    76,    77,
      -1,    79,    -1,    81,   986,    83,    -1,    -1,    -1,    -1,
      85,    -1,   994,    -1,   996,    -1,   998,    -1,    82,    -1,
      -1,    -1,    86,    -1,   102,    -1,   614,   105,    -1,    -1,
     108,    -1,   244,   247,    -1,    -1,   111,    -1,    -1,   251,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   311,   272,    -1,
      31,    -1,    -1,    -1,   139,    -1,    -1,    -1,   614,   133,
     134,   325,    -1,   137,   138,    -1,   140,   141,   666,    -1,
     158,   145,     2,   158,   672,    -1,   164,   915,   163,   153,
      -1,   166,    12,    -1,    -1,   159,   160,   161,    -1,   311,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
     698,    82,    -1,   325,    -1,    86,    -1,    -1,  1100,    -1,
     666,    -1,    -1,    -1,   712,   713,   672,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   397,    -1,    -1,    -1,    -1,    -1,   737,
      -1,   739,    72,    -1,    -1,    -1,   744,    -1,    -1,   747,
      -1,    -1,   133,   134,    -1,   753,    -1,   138,   756,   140,
     141,   425,    -1,    -1,   145,    -1,    -1,   431,    -1,    99,
      -1,    -1,   153,   103,   438,   397,    -1,    -1,   159,   160,
     161,   737,   780,   739,    -1,    -1,    -1,    -1,   744,    -1,
     414,   747,    -1,   791,   418,   793,    -1,   753,    -1,    -1,
     756,    -1,    -1,   425,    -1,    -1,    -1,    -1,    -1,   431,
      -1,    -1,    -1,    -1,    -1,    -1,   438,    -1,    -1,    -1,
      -1,    -1,   446,    -1,   780,    -1,    -1,    -1,   492,    -1,
      -1,    -1,   496,    -1,    -1,   791,   166,   793,    -1,    -1,
     504,    -1,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     492,    -1,    17,    18,   496,    20,    -1,    -1,    -1,    -1,
      25,   505,   504,   881,    -1,    -1,   162,   511,    33,   165,
     888,   167,    -1,     2,    39,    -1,    -1,    -1,    -1,    44,
      -1,   899,    47,    12,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,   244,   913,    25,    -1,    27,    64,
      65,   251,    -1,    -1,   922,   881,    71,    -1,   926,    -1,
      -1,    -1,   888,    31,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   899,    89,    90,    -1,    92,    -1,    94,
     614,    96,    -1,    -1,    99,    -1,    -1,   913,   103,   104,
      -1,   106,   586,    72,   109,   589,   922,    -1,    -1,    -1,
     926,    -1,    -1,    -1,   972,    -1,    -1,    -1,    -1,    -1,
      -1,   311,    -1,    -1,    82,    -1,    -1,    -1,    86,     2,
      99,    -1,   614,    -1,   103,   325,    -1,    -1,    -1,    12,
      -1,    -1,   666,   627,    -1,    -1,    -1,   631,   672,    -1,
      -1,    -1,    25,    -1,    27,    -1,   972,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,  1024,  1025,  1026,  1027,
     654,    -1,    -1,    -1,   658,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   666,    -1,    -1,   145,    -1,    -1,
     672,  1049,  1050,    -1,   152,   153,    -1,   166,   156,    72,
      -1,   159,   160,   161,    -1,    -1,    -1,   397,  1024,  1025,
    1026,  1027,    -1,   737,   698,   739,    -1,    -1,    -1,    -1,
     744,    -1,    -1,   747,    -1,    -1,    99,    -1,    -1,   753,
     103,    -1,   756,  1049,  1050,   425,    -1,    -1,    22,    -1,
      -1,   431,    26,    27,    -1,    -1,    -1,    -1,   438,    -1,
      -1,    35,    -1,    37,    38,   737,   780,   739,    42,    -1,
      -1,    -1,   744,    -1,    48,   747,    -1,   791,    -1,   793,
      -1,   753,    -1,    -1,   756,   244,   760,    -1,    -1,   763,
      -1,    -1,   251,    -1,    -1,    -1,    70,    -1,    72,    -1,
      74,    -1,    76,   166,    -1,    79,    -1,    81,   780,    83,
      -1,   785,   492,    -1,    -1,    -1,   496,    -1,    -1,   791,
      -1,   793,    -1,    -1,   504,    -1,    -1,    -1,   102,    -1,
      -1,   105,    -1,   807,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,   311,    -1,    -1,    -1,    -1,    -1,    -1,   833,
      -1,    -1,    -1,    -1,    -1,    17,   325,   881,    20,    -1,
      -1,    -1,    -1,    25,   888,    -1,    -1,     2,    -1,    -1,
      -1,   244,    -1,    35,   158,   899,    -1,    12,   251,    -1,
     164,    -1,    44,    -1,    -1,    -1,    -1,    -1,    50,   913,
      25,    -1,    27,    -1,    -1,    -1,    58,    -1,   922,   881,
      -1,    -1,   926,    65,    -1,    -1,   888,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   899,    80,    -1,
      -1,    -1,    -1,    -1,   614,    -1,    -1,    -1,   397,    -1,
      92,   913,    -1,    -1,    96,    -1,    -1,    72,   311,    -1,
     922,   103,    -1,    -1,   926,    -1,    -1,   109,   972,    -1,
      -1,    -1,   325,    -1,    -1,    -1,   425,    -1,    -1,    -1,
      -1,    -1,   431,    -1,    99,   949,    -1,    -1,   103,   438,
      -1,    -1,    -1,    -1,    -1,    -1,   666,    -1,    -1,    -1,
      -1,    -1,   672,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     972,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1024,  1025,  1026,  1027,    -1,     2,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   492,   397,  1049,  1050,   496,    25,    -1,
      27,   166,    -1,    -1,    -1,   504,    -1,    -1,    -1,    -1,
      -1,    -1,  1024,  1025,  1026,  1027,    -1,   737,    -1,   739,
      -1,    -1,   425,    -1,   744,    -1,    -1,   747,   431,    -1,
      -1,    -1,    -1,   753,    -1,   438,   756,  1049,  1050,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     780,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      -1,   791,    99,   793,    -1,    31,   103,    -1,    -1,   244,
      17,    18,    -1,    20,    -1,    -1,   251,    -1,    25,   492,
      -1,    -1,    -1,   496,    -1,    -1,    33,    -1,    -1,    -1,
      -1,   504,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,   614,    -1,    -1,    -1,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    82,    64,    65,    -1,
      86,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    80,    -1,    -1,   311,    -1,    -1,    -1,
      -1,    -1,    89,    90,    -1,    92,    -1,    94,    -1,    96,
     325,   881,    99,    -1,    -1,    -1,   103,   666,   888,   106,
      -1,    -1,   109,   672,    -1,    -1,    -1,   133,   134,   899,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,    -1,   913,   150,   151,   152,   153,   154,    -1,
     156,    -1,   922,   159,   160,   161,   926,    -1,    -1,    -1,
      -1,   614,    -1,    -1,    -1,    -1,    -1,   244,    -1,    -1,
      -1,    -1,    -1,    -1,   251,    -1,   163,    -1,   165,    -1,
      -1,    -1,   397,    -1,    -1,    -1,    -1,    -1,   737,    -1,
     739,    -1,    -1,    -1,    -1,   744,    -1,    -1,   747,    -1,
      -1,    -1,   972,    -1,   753,    -1,    -1,   756,    -1,    -1,
     425,    -1,    -1,   666,    -1,    -1,   431,    -1,    -1,   672,
      -1,    -1,    -1,   438,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   780,    -1,    -1,   311,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   791,    -1,   793,    22,    -1,    -1,   325,    26,
      27,    -1,    -1,    -1,  1024,  1025,  1026,  1027,    35,    -1,
      37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,   492,    -1,  1049,
    1050,   496,    -1,    -1,   737,    -1,   739,    -1,    -1,   504,
      -1,   744,    -1,    70,   747,    72,    -1,    74,    -1,    76,
     753,    -1,    79,   756,    81,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     397,    -1,    -1,    -1,    -1,   102,    -1,   780,   105,    -1,
      -1,   108,   881,    -1,    31,    -1,    -1,    -1,   791,   888,
     793,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   425,    -1,
     899,    -1,    -1,    -1,   431,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   913,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   922,    -1,    -1,    -1,   926,    -1,    -1,
      -1,   158,    -1,    -1,    -1,    82,    -1,   164,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   614,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   492,    -1,    -1,    -1,   496,
      -1,    -1,    -1,   972,    -1,    -1,    -1,   504,   881,    -1,
      -1,    -1,    -1,    -1,    -1,   888,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,   899,    -1,   145,   146,
     147,   666,    -1,   150,   151,   152,   153,   672,    -1,   156,
     913,    -1,   159,   160,   161,    -1,    22,    -1,    -1,   922,
      26,    27,    -1,   926,    -1,  1024,  1025,  1026,  1027,    35,
      -1,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1049,  1050,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,   972,
      76,    -1,   737,    79,   739,    81,    -1,    83,    -1,   744,
      -1,    -1,   747,    -1,    -1,    -1,    -1,   614,   753,     3,
      -1,   756,    -1,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,   780,    -1,    -1,    -1,    33,
      -1,  1024,  1025,  1026,  1027,    39,   791,    -1,   793,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,   666,
      -1,    -1,    -1,    -1,    58,   672,  1049,  1050,    -1,    -1,
      64,    65,   158,    -1,    -1,    -1,    -1,    71,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    92,    -1,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     737,    -1,   739,    -1,    -1,    -1,   881,   744,    -1,    -1,
     747,    -1,    -1,   888,    -1,    -1,   753,    -1,    -1,   756,
      -1,    -1,     3,    -1,   899,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,   913,    20,
      -1,   165,    -1,   780,    25,    -1,    -1,   922,    -1,    -1,
      -1,   926,    33,    -1,   791,    22,   793,    -1,    39,    26,
      27,    -1,    -1,    44,    -1,    -1,    47,    -1,    35,    50,
      37,    38,    -1,    -1,    -1,    42,    -1,    58,    -1,    -1,
      -1,    48,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,   972,    -1,    80,
      -1,    -1,    -1,    70,    -1,    72,    -1,    -1,    89,    90,
      -1,    92,    -1,    94,    81,    96,    83,    -1,    99,    -1,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1024,
    1025,  1026,  1027,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   899,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1049,  1050,   913,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,   922,    -1,    -1,    -1,   926,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
       1,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,   972,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    -1,    50,
      51,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    -1,    83,    84,    85,    -1,    87,    88,    89,    90,
      91,    92,    93,    94,    -1,    96,    97,    -1,    99,   100,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,
     161,    -1,   163,    -1,   165,     1,   167,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,   168,     1,    -1,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    -1,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    -1,    64,    65,    66,    -1,
      -1,    -1,    70,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    96,    97,
      -1,    99,   100,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,   164,   165,     1,   167,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    -1,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      83,    84,    85,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    -1,    96,    97,    -1,    99,   100,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,    -1,
     163,   164,   165,     1,   167,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    -1,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    -1,    64,    65,    66,    -1,
      -1,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    84,    85,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    96,    97,
      -1,    99,   100,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
     158,    -1,    -1,   161,    -1,   163,   164,   165,     1,   167,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    -1,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      83,    84,    85,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    -1,    96,    97,    -1,    99,   100,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,    -1,
     163,   164,   165,     1,   167,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    -1,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    -1,    64,    65,    66,    -1,
      -1,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    84,    85,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    96,    97,
      -1,    99,   100,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
     158,    -1,    -1,   161,    -1,   163,   164,   165,     1,   167,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    -1,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      83,    84,    85,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    -1,    96,    97,    -1,    99,   100,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,    -1,
     163,   164,   165,     1,   167,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    -1,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    -1,    64,    65,    66,    -1,
      -1,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    84,    85,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    96,    97,
      -1,    99,   100,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
     158,    -1,    -1,   161,    -1,   163,     1,   165,     3,   167,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    -1,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    -1,    64,
      65,    66,    -1,    -1,    -1,    70,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    -1,    83,    84,
      85,    -1,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    96,    97,    -1,    99,   100,   101,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,    -1,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,   158,    -1,    -1,   161,    -1,   163,     1,
     165,     3,   167,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    -1,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    96,    97,    -1,    99,   100,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,
      -1,   163,     1,   165,     3,   167,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      -1,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    -1,    64,    65,    66,    -1,    -1,
      -1,    70,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    -1,    83,    84,    85,    -1,    87,    88,
      89,    90,    91,    92,    93,    94,    -1,    96,    97,    -1,
      99,   100,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,   158,
      -1,    -1,   161,    -1,   163,     1,   165,     3,   167,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    -1,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    -1,    64,    65,
      66,    -1,    -1,    -1,    70,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    -1,    83,    84,    85,
      -1,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      96,    97,    -1,    99,   100,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,   158,    -1,    -1,   161,    -1,   163,     1,   165,
       3,   167,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    -1,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    62,
      -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    -1,
      83,    84,    85,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    -1,    96,    97,    -1,    99,   100,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,    -1,
     163,     1,   165,     3,   167,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    -1,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    -1,    64,    65,    66,    -1,    -1,    -1,
      70,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    87,    88,    89,
      90,    91,    92,    93,    94,    -1,    96,    97,    -1,    99,
     100,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
      -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,   158,    -1,
      -1,   161,    -1,   163,     1,   165,     3,   167,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    -1,    50,    51,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    -1,    64,    65,    66,
      -1,    -1,    -1,    70,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      87,    88,    89,    90,    91,    92,    93,    94,    -1,    96,
      97,    -1,    99,   100,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,   158,    -1,    -1,   161,    -1,   163,     1,   165,     3,
     167,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    -1,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    62,    -1,
      64,    65,    66,    -1,    -1,    -1,    70,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      84,    85,    -1,    87,    88,    89,    90,    91,    92,    93,
      94,    -1,    96,    97,    -1,    99,   100,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,    -1,    -1,   112,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,   158,    -1,    -1,   161,    -1,   163,
       1,   165,     3,   167,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    -1,    50,
      51,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    -1,    83,    84,    85,    -1,    87,    88,    89,    90,
      91,    92,    93,    94,    -1,    96,    97,    -1,    99,   100,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,
     161,    -1,   163,     1,   165,     3,   167,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    -1,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    62,    -1,    64,    65,    66,    -1,
      -1,    -1,    70,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    84,    85,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    96,    97,
      -1,    99,   100,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
     158,    -1,    -1,   161,    -1,   163,    -1,   165,     1,   167,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,    -1,   165,    -1,   167,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    96,    -1,
      -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
      -1,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
     168,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,
      -1,   163,    -1,   165,    -1,   167,   168,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,   168,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    83,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,    -1,   167,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,
      65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    96,    -1,    -1,    99,   100,   101,    -1,   103,   104,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,
     165,    -1,   167,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
     100,   101,    -1,   103,   104,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,   166,   167,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,
      65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    96,    -1,    -1,    99,   100,   101,    -1,   103,   104,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,
     165,   166,   167,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    83,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,     3,   167,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    61,    62,    -1,    64,    65,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,
      -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    96,
      -1,    -1,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,   158,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,
     167,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,
      -1,   163,    -1,   165,     3,   167,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    12,   142,    -1,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,   152,    -1,    25,   155,   156,   157,    -1,
      -1,    -1,   161,    33,   163,    35,   165,    -1,   167,    39,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    -1,    92,    -1,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,   135,   136,   137,    -1,    -1,
     140,    -1,    -1,    -1,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,   156,    -1,    -1,   159,
     160,   161,   162,    -1,     3,   165,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,     3,   167,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,   115,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,
      -1,   167,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,
     161,    -1,   163,    -1,   165,     3,   167,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    96,    -1,
      -1,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
      -1,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,   164,   165,     3,   167,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,   115,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,     3,   167,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    61,    62,    -1,    64,    65,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,
      -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,    -1,
     167,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,
      -1,   163,    -1,   165,     3,   167,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,
      -1,   165,    -1,   167,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,
      -1,   165,     3,   167,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,
     161,    -1,   163,    -1,   165,     3,   167,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    96,    -1,
      -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
      -1,    -1,    -1,   161,    -1,   163,    -1,   165,     3,   167,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,
      65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,
     165,     3,   167,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,
      -1,   163,    -1,   165,     3,   167,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,     3,   167,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,
       3,   167,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,    -1,   165,     3,   167,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,
      -1,   161,    -1,   163,    -1,   165,     3,   167,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    61,    62,    -1,    64,    65,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,
      -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,    -1,    -1,    -1,   161,    -1,   163,    -1,   165,     3,
     167,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,
      -1,   165,     3,   167,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,
     161,    -1,   163,    -1,   165,     3,   167,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    96,    -1,
      -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
      -1,    -1,    -1,   161,    -1,   163,    -1,   165,     3,   167,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,
      65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,    -1,
     165,     3,   167,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,
      -1,   163,    -1,   165,     3,   167,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,     3,   167,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,    -1,    -1,    -1,   161,    -1,    -1,    -1,   165,
       3,   167,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   162,   156,   157,   165,    -1,   167,   161,    -1,
     163,    -1,   165,    -1,   167,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    31,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,
      -1,    -1,    82,    71,    -1,    73,    86,    75,    -1,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    96,    -1,
      -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,    -1,   145,    -1,   135,    -1,    -1,
      -1,   139,   152,   153,    -1,   143,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,    -1,    -1,    -1,   163,    -1,   165,   166,   167,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    56,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    98,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,   135,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,   157,   158,    -1,    -1,    -1,    -1,
     163,    33,   165,    35,   167,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,     3,   135,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,   157,    -1,    -1,    -1,    -1,
      -1,   163,    33,   165,    35,   167,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,   135,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,   157,    -1,    -1,    -1,
      -1,    -1,   163,    33,   165,    35,   167,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,     3,    99,
      -1,    -1,    -1,   103,    -1,    -1,   106,    12,    -1,   109,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,   135,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    64,
      65,    -1,    -1,    12,    -1,   165,    71,   167,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    89,    90,    -1,    92,    -1,    94,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,   120,   121,   122,   123,    -1,
      -1,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,    82,    -1,   140,    -1,    86,    -1,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      -1,   156,    12,    -1,   159,   160,   161,   162,    -1,    19,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    12,
     159,   160,   161,    -1,    -1,    -1,    19,    -1,    -1,   168,
      -1,    -1,    82,    -1,    -1,    -1,    86,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    82,
     140,   141,   142,    86,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    12,   159,
     160,   161,    -1,    -1,    -1,    19,    -1,    -1,   168,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    31,    -1,    82,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   117,    -1,   159,   160,   161,    -1,
      -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,    82,    -1,
     133,   134,    86,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    12,    -1,   159,   160,   161,    -1,
      -1,    19,    -1,   117,    -1,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,    12,   166,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    82,   140,   141,   142,    86,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   117,    -1,   159,   160,
     161,    -1,    -1,    12,    -1,   166,    -1,    -1,    -1,    -1,
      19,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    31,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,    -1,    -1,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,    -1,    -1,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,    -1,    -1,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,    -1,    -1,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,    -1,    -1,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,    -1,    -1,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,    -1,    -1,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,    -1,    -1,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,   117,   118,
     119,   120,   121,   122,   123,    -1,    -1,   126,   127,   128,
     129,    46,   131,   132,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    -1,   163,    -1,    -1,    82,    -1,    -1,
      -1,    86,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    82,   159,   160,   161,    86,   163,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    82,
     159,   160,   161,    86,   163,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    82,   159,   160,   161,    86,
     163,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    31,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    82,   159,   160,   161,    86,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    31,   117,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,   117,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    -1,   163,    -1,   133,   134,    82,   136,   137,   138,
      86,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    -1,   163,    12,    -1,    -1,    -1,    -1,
      -1,   117,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   133,   134,    36,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    -1,   163,    12,    13,
      67,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    31,    -1,    86,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    82,   136,
     137,   138,    86,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    46,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    12,   159,   160,   161,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    82,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
     117,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,   133,   134,    86,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    12,   159,   160,   161,    -1,    -1,    -1,    19,   117,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,    -1,
      31,    32,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    46,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   117,
      -1,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    82,   140,   141,   142,    86,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
     158,   159,   160,   161,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    67,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      86,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    82,   140,   141,   142,    86,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    12,   159,   160,   161,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,   117,    86,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
     117,   150,   151,   152,   153,   154,   155,   156,    -1,   158,
     159,   160,   161,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    82,   140,   141,   142,    86,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      12,   158,   159,   160,   161,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    12,   158,   159,
     160,   161,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    -1,    86,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    82,   140,   141,
     142,    86,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,   158,   159,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,   116,   117,    86,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    95,    -1,    -1,    -1,    19,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    31,   144,
     145,   146,   147,   148,   117,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    -1,   159,   160,   161,    82,
      -1,    -1,    -1,    86,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    31,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,    -1,    82,   159,   160,   161,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,   113,    82,    -1,    -1,
     117,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,   117,   150,   151,   152,   153,   154,   155,   156,
      -1,    -1,   159,   160,   161,    -1,    82,    -1,   133,   134,
      86,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    12,   158,   159,   160,   161,    -1,    -1,    19,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    34,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    12,    -1,   159,   160,   161,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    -1,   133,   134,    86,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    -1,    82,    -1,   116,   117,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,   117,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    12,     3,
     159,   160,   161,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    31,    32,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      12,    13,    -1,    -1,    -1,    39,    -1,    19,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    82,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    92,    -1,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      82,    -1,   106,   117,    86,   109,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    31,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,   117,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    82,   159,   160,   161,
      86,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    95,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    31,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,
      -1,   117,    86,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    95,    -1,    -1,    -1,    19,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    31,   144,   145,
     146,   147,   148,   117,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,   159,   160,   161,    82,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    95,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    31,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,   159,   160,   161,    82,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,   117,    86,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    95,    -1,    -1,    -1,    19,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    31,
     144,   145,   146,   147,   148,   117,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    95,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    31,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      82,    -1,    -1,    -1,    86,    -1,    56,    -1,    -1,    -1,
      -1,    12,    -1,    95,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    82,    -1,    -1,   117,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,   117,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      -1,    82,    -1,   133,   134,    86,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    12,    -1,   159,
     160,   161,    -1,    -1,    19,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    22,   159,   160,   161,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    22,    37,    38,
      -1,    26,    27,    42,    -1,    -1,    -1,    -1,    -1,    48,
      35,    -1,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      79,    31,    81,    -1,    83,    70,    -1,    72,    -1,    74,
      -1,    76,    -1,    -1,    79,    22,    81,    -1,    83,    26,
      27,    -1,    -1,   102,    -1,    -1,   105,    -1,    35,   108,
      37,    38,    -1,    31,    -1,    42,    -1,   102,    -1,    -1,
     105,    48,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    82,   164,    -1,   117,    86,    -1,
      -1,    -1,    -1,   158,    -1,   102,    -1,    -1,   105,   164,
      -1,   108,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,    -1,   117,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
      -1,   158,   150,   151,   152,   153,   154,     1,   156,     3,
      -1,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,     3,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      17,    18,    -1,    20,    58,    -1,    -1,    -1,    25,    -1,
      64,    65,    -1,    -1,    -1,    -1,    33,    71,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    80,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    89,    90,    -1,    92,    -1,
      94,    58,    96,    -1,     3,    99,    -1,    64,    65,   103,
      -1,    -1,   106,    -1,    71,   109,    -1,    -1,    17,    18,
      -1,    20,    -1,    80,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    89,    90,    33,    92,    -1,    94,    -1,    96,
      39,     3,    99,    -1,    -1,    44,   103,    -1,    47,   106,
      -1,    50,   109,    -1,    -1,    17,    18,    -1,    20,    58,
      -1,    -1,    -1,    25,    -1,    64,    65,    -1,    -1,    -1,
      -1,    33,    71,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    80,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      89,    90,    -1,    92,    -1,    94,    58,    96,    -1,     3,
      99,    -1,    64,    65,   103,    -1,    -1,   106,    -1,    71,
     109,    -1,    -1,    17,    18,    -1,    20,    -1,    80,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    89,    90,    33,
      92,    -1,    94,    -1,    96,    39,     3,    99,    -1,    -1,
      44,   103,    -1,    47,   106,    -1,    50,   109,    -1,    -1,
      17,    18,    -1,    20,    58,    -1,    -1,    -1,    25,    -1,
      64,    65,    -1,    -1,    -1,    -1,    33,    71,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    80,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    89,    90,    -1,    92,    -1,
      94,    58,    96,    -1,    -1,    99,    -1,    64,    65,   103,
      -1,    -1,   106,    -1,    71,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    89,    90,    -1,    92,    -1,    94,    35,    96,
      37,    38,    99,    -1,    -1,    42,   103,    -1,    45,   106,
      -1,    48,   109,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,   105,    -1,
     107,   108
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   174,   175,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    47,    48,
      50,    53,    54,    55,    57,    58,    59,    61,    62,    64,
      65,    66,    70,    71,    72,    73,    74,    75,    76,    79,
      80,    81,    83,    84,    85,    87,    88,    89,    90,    91,
      92,    93,    94,    96,    97,    99,   100,   101,   102,   103,
     105,   106,   108,   109,   112,   114,   117,   133,   134,   135,
     136,   137,   142,   152,   155,   156,   157,   158,   161,   163,
     165,   167,   176,   177,   178,   179,   180,   181,   183,   188,
     189,   190,   191,   192,   196,   198,   201,   202,   204,   205,
     206,   211,   212,   214,   215,   216,   220,   221,   224,   225,
     228,   229,   232,   235,   236,   261,   264,   265,   285,   286,
     287,   288,   289,   290,   291,   299,   300,   301,   302,   303,
     306,   307,   308,   309,   310,   311,   312,   314,   315,   316,
     317,   318,   158,   178,   303,   113,   292,   293,     3,   193,
      14,    22,    35,    40,    41,    43,    81,    94,   163,   167,
     232,   285,   290,   301,   302,   303,   306,   308,   309,   292,
     303,   102,   264,    83,   193,   178,   279,   303,   178,   165,
       8,    81,   303,   304,     8,    11,    81,   102,   304,    72,
     115,   213,   303,   213,   303,    26,   265,   303,   303,   165,
       3,    17,    18,    20,    25,    33,    39,    44,    47,    50,
      58,    64,    65,    71,    80,    89,    90,    92,    94,    96,
      99,   103,   106,   109,   195,   197,   233,   165,   195,   266,
     267,    32,   183,   200,   303,   303,    18,    71,    89,   106,
     303,   303,   303,     8,   165,   203,    22,    35,    38,    81,
     204,     4,   157,   255,   278,   303,   100,   101,   158,   303,
     305,   303,   200,   303,   303,   303,    93,   165,   178,   303,
     303,   183,   192,   303,   306,   183,   192,   303,   195,   262,
     303,   303,   303,   303,   303,   303,   303,   303,     1,   164,
     176,   184,   278,   104,   143,   255,   280,   281,   305,   213,
     278,   303,   313,    74,   178,   163,    78,   182,    45,   107,
     195,   195,    51,    77,   250,   266,   157,   158,   149,   303,
      12,    19,    31,    82,    86,   117,   133,   134,   136,   137,
     138,   140,   141,   142,   144,   145,   146,   147,   148,   150,
     151,   152,   153,   154,   155,   156,   159,   160,   161,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   162,   242,   165,   167,    82,    86,
     165,   178,   158,   303,   303,   303,   278,   163,   183,    46,
     292,   262,   266,   158,   139,   158,   112,   196,   255,   282,
     283,   284,   305,    81,   209,   236,   264,    81,   207,   262,
     236,   264,   195,   165,   200,    32,    46,   200,   113,   200,
     295,    32,    46,   200,   295,    36,    67,   158,    95,   183,
     242,   282,   178,   165,   244,   104,   165,   195,   268,   269,
       1,   138,   273,    46,   139,   178,   200,   165,   165,   200,
     282,   204,   195,   139,   158,   303,   303,   158,   163,   200,
     165,   282,   158,   217,   158,   217,   158,   118,   263,   158,
     200,   158,   164,   164,   176,   139,   164,   303,   139,   166,
     139,   166,   168,   295,    46,   139,   168,   295,   116,   139,
     168,     8,     1,   164,   184,    60,   303,   185,   303,   138,
     226,   163,   237,   158,   303,   303,   303,   303,   213,   303,
     213,   303,   303,   303,   303,   303,   303,   303,    20,    33,
      58,   102,   196,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   305,
     305,   305,   305,   305,   282,   282,   213,   303,   213,   303,
      27,    46,    83,   108,   294,   297,   298,   303,   318,    32,
      46,    32,    46,    95,    46,   168,   213,   303,   200,   158,
     158,   303,   303,   118,   166,   139,   210,   195,   266,   208,
     195,   158,   266,    46,   278,    43,   303,   213,   303,   165,
     200,    43,   303,   213,   303,   200,   161,   185,   187,   303,
     185,   186,   178,   303,    34,   303,   166,    27,    46,    49,
      69,    72,    83,   102,   177,   245,   246,   247,   248,   234,
     269,   139,   166,    33,    47,    90,    94,   167,   199,   274,
     286,   118,   270,   303,   267,   303,   303,   166,   255,   303,
       1,   222,   282,   166,    21,   218,   274,   286,   139,   164,
     166,   166,   280,   166,   280,   178,   168,   213,   303,   168,
     178,   303,   168,   303,   168,   303,   164,   164,   195,    13,
     141,   158,   139,   158,    13,    36,    67,   278,   163,     1,
     195,   230,   231,    27,    72,    83,   102,   239,   249,   165,
     158,   158,   158,   158,   158,   166,   168,    46,    83,   139,
     166,   285,    82,    82,    43,   213,   303,    43,   213,   303,
     213,   303,   292,   292,   158,   255,   305,   284,   195,   226,
     158,   195,   226,   158,   303,   166,   303,    32,   200,    32,
     200,   296,   297,   303,    32,   200,   295,    32,   200,   295,
     158,    13,   158,    34,    34,   178,    95,   183,    46,    83,
     247,   139,   166,   165,   195,    27,    72,    83,   102,   251,
     166,   269,   273,     1,   278,    63,   305,   166,   166,   164,
      68,   110,   164,   223,   166,   165,   183,   195,   219,   262,
     178,   168,   295,   168,   295,   178,   116,   196,   163,   303,
     303,   187,   186,     1,   227,   164,   118,   139,   164,    83,
     238,     1,     3,    12,    17,    19,    20,    25,    39,    44,
      50,    52,    58,    64,    65,    80,    92,    96,    99,   103,
     109,   133,   134,   135,   136,   137,   140,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   156,   159,
     160,   161,   162,   165,   194,   195,   197,   240,   241,   242,
     285,   166,   297,   273,   285,   285,   303,    32,    32,   303,
      32,    32,   168,   168,   200,   200,   226,   163,   226,   163,
     200,    95,    43,   303,    43,   303,   139,   166,    95,    43,
     303,   200,    43,   303,   200,   303,   178,   303,   178,    34,
     195,   246,   269,   138,   277,    83,   273,   270,   168,    46,
     168,   165,   165,    32,   178,   278,   219,   138,   183,    43,
     178,   303,   168,    43,   178,   303,   168,   303,   158,   185,
      13,    36,    67,   158,   158,   164,    76,    79,   164,   177,
     202,   303,   231,   251,   165,   243,   195,   303,   133,   141,
     243,   243,   270,    95,    43,    43,    95,    43,    43,    43,
      43,   163,   227,   163,   227,   303,   303,   303,   297,   303,
     303,   303,    34,   178,   277,   166,   167,   199,   255,   276,
     286,   143,   256,   270,    56,   111,   257,   303,   274,   286,
     282,   282,   178,   200,   166,   303,   303,   178,   303,   178,
     164,   303,   187,   186,   202,     1,   138,   272,   245,   166,
       3,    96,   241,   242,   303,   303,   303,   303,   303,   303,
     227,   164,   227,   164,    95,    95,    95,    95,   178,   270,
     277,   168,   278,   255,   303,     3,    85,    96,   258,   259,
     260,   303,   183,   201,   254,   168,   166,   166,   183,    95,
      95,   158,   158,   167,   199,   271,   286,    98,   252,   166,
     243,   243,    95,    95,    95,    95,    95,    95,   164,   164,
     303,   303,   303,   303,   256,   270,   255,   275,   276,   286,
      46,   168,   260,   111,   139,   118,   144,   146,   147,   150,
     151,    56,   286,   157,   157,   303,   303,     1,   168,   278,
     257,   303,   275,   276,   303,   259,   260,   260,   260,   260,
     260,   260,   258,   168,   271,   286,   168,   158,   253,   254,
     168,   271,   286,   275
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   173,   174,   175,   175,   176,   176,   177,   177,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     179,   180,   180,   180,   181,   181,   181,   182,   182,   183,
     183,   183,   184,   184,   185,   185,   185,   185,   186,   186,
     187,   187,   188,   188,   188,   189,   189,   189,   189,   189,
     190,   190,   190,   191,   192,   192,   192,   192,   192,   193,
     193,   194,   194,   194,   194,   194,   194,   195,   195,   195,
     195,   195,   195,   196,   196,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   199,   199,   199,   199,
     200,   200,   201,   201,   202,   202,   202,   203,   202,   204,
     204,   204,   204,   204,   204,   204,   205,   205,   205,   205,
     207,   206,   208,   206,   209,   206,   210,   206,   206,   206,
     206,   206,   211,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   213,
     214,   214,   214,   214,   214,   214,   214,   214,   215,   216,
     216,   216,   216,   216,   216,   217,   217,   218,   218,   218,
     219,   219,   220,   221,   221,   222,   222,   223,   223,   223,
     224,   224,   225,   225,   225,   226,   226,   227,   227,   227,
     228,   228,   229,   230,   230,   230,   231,   231,   233,   234,
     232,   235,   235,   235,   237,   238,   236,   239,   239,   239,
     239,   239,   240,   240,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   243,   243,   244,   245,   245,   245,
     246,   246,   246,   246,   246,   247,   247,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   249,   249,   249,   249,
     249,   249,   250,   250,   251,   251,   251,   251,   251,   251,
     252,   252,   253,   253,   254,   254,   255,   255,   256,   256,
     256,   257,   257,   257,   257,   257,   258,   258,   259,   259,
     259,   259,   259,   259,   259,   260,   260,   261,   261,   261,
     262,   262,   263,   263,   263,   264,   264,   264,   264,   264,
     265,   265,   266,   266,   267,   267,   268,   268,   268,   269,
     269,   269,   270,   270,   270,   271,   271,   271,   271,   271,
     271,   271,   272,   272,   272,   272,   272,   273,   273,   273,
     273,   273,   274,   274,   274,   274,   275,   275,   275,   276,
     276,   276,   276,   276,   277,   277,   277,   277,   277,   278,
     278,   278,   278,   279,   279,   280,   280,   280,   281,   281,
     282,   282,   283,   283,   284,   284,   284,   284,   285,   285,
     286,   286,   286,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   289,   290,   291,   291,   291,   291,   291,   291,
     291,   291,   292,   292,   293,   294,   294,   295,   296,   296,
     297,   297,   297,   298,   298,   298,   298,   298,   298,   299,
     299,   300,   300,   300,   300,   300,   301,   301,   301,   301,
     301,   301,   301,   302,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   304,   304,
     305,   305,   305,   306,   306,   306,   306,   307,   307,   307,
     307,   308,   308,   308,   309,   309,   309,   309,   309,   310,
     310,   310,   310,   311,   311,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   313,
     313,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   315,   315,   315,   315,   315,   315,
     315,   316,   316,   316,   316,   317,   317,   317,   317,   318,
     318,   318,   318,   318,   318,   318
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     2,     3,     3,     2,     2,     3,     2,
       4,     3,     4,     4,     0,     1,     1,     0,     1,     2,
       3,     3,     1,     2,     1,     3,     3,     5,     0,     1,
       1,     1,     0,     1,     1,     4,     6,     8,     6,     8,
       4,     6,     7,     3,     4,     4,     4,     4,     4,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     3,     1,     1,     2,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     5,     5,     2,
       0,     8,     0,     9,     0,     8,     0,     9,     3,     3,
       5,     5,     2,     5,     3,     6,     6,     4,     5,     5,
       3,     3,     6,     5,     6,     5,     6,     3,     4,     3,
       4,     6,     7,     6,     7,     4,     5,     4,     5,     4,
       4,     3,     6,     5,     6,     5,     8,     7,     2,     3,
       3,     2,     2,     3,     3,     0,     2,     2,     3,     5,
       1,     3,     3,     5,     5,     0,     2,     3,     2,     3,
       6,     6,     1,     1,     1,     0,     2,     0,     2,     3,
       5,     5,     1,     1,     2,     3,     1,     3,     0,     0,
       8,     0,     1,     1,     0,     0,    10,     3,     3,     5,
       5,     3,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     3,     0,     1,     3,
       4,     5,     4,     6,     6,     0,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     2,     1,     1,     2,
       2,     0,     2,     2,     4,     4,     1,     3,     3,     3,
       3,     3,     3,     3,     2,     1,     1,     3,     4,     4,
       2,     4,     0,     2,     2,     1,     1,     1,     2,     1,
       4,     3,     1,     3,     3,     5,     1,     1,     3,     1,
       2,     3,     0,     2,     2,     3,     2,     4,     3,     3,
       4,     3,     0,     2,     2,     2,     1,     0,     2,     2,
       2,     1,     4,     4,     6,     3,     0,     1,     1,     3,
       4,     3,     4,     6,     0,     2,     2,     2,     2,     1,
       1,     3,     3,     1,     3,     1,     1,     1,     3,     3,
       0,     1,     1,     3,     3,     3,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     2,     4,
       4,     4,     5,     2,     2,     1,     2,     1,     2,     1,
       2,     1,     2,     1,     1,     6,     6,     4,     9,     9,
       7,     6,     6,     4,     9,     9,     7,     4,     6,     6,
       9,     9,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     1,     4,     1,     3,     4,     1,     3,
       4,     3,     3,     1,     1,     2,     1,     2,     1,     1,
       3,     1,     2,     2,     2,     2,     2,     8,     8,     9,
       9,     5,     5,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     3,     2,     2,     1,     0,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     4,     4,     4,     3,     3,     3,     3,     5,     3,
       4,     3,     4,     1,     1,     1,     1,     1,     1,     1,
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
#line 497 "chapel.ypp" /* yacc.c:1663  */
    { yyblock = (yyval.pblockstmt); }
#line 6022 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 3:
#line 502 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6028 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 4:
#line 503 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6034 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 6:
#line 510 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6040 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 7:
#line 515 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6046 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 8:
#line 516 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6052 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 24:
#line 536 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6058 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 25:
#line 537 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6064 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 26:
#line 538 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6070 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 27:
#line 539 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6076 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 28:
#line 540 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6082 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 29:
#line 541 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6088 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 30:
#line 542 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6094 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 31:
#line 543 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6100 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 32:
#line 544 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6106 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 33:
#line 545 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6112 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 34:
#line 546 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6118 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 35:
#line 547 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6124 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 36:
#line 548 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6130 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 37:
#line 549 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6136 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 38:
#line 550 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6142 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 39:
#line 551 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6148 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 40:
#line 556 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 6162 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 41:
#line 569 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6171 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 42:
#line 574 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6180 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 43:
#line 579 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6189 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 44:
#line 586 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6195 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 45:
#line 587 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6201 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 46:
#line 588 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6207 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 47:
#line 592 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6213 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 48:
#line 593 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 6219 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 49:
#line 606 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6225 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 50:
#line 607 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6231 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 51:
#line 608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6237 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 52:
#line 613 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6243 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 53:
#line 614 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6249 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 54:
#line 619 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6259 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 55:
#line 624 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6269 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 56:
#line 629 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6278 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 57:
#line 633 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6287 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 58:
#line 640 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6297 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 60:
#line 649 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6307 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 62:
#line 658 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = (currentModuleType != MOD_INTERNAL ? true : false); }
#line 6313 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 63:
#line 659 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6319 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 64:
#line 660 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6325 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 65:
#line 664 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 6331 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 66:
#line 665 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 6337 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 67:
#line 666 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6343 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 68:
#line 667 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 6349 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 69:
#line 668 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6355 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 70:
#line 672 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildImportStmt((yyvsp[-1].pexpr), (yyvsp[-3].b)); }
#line 6361 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 71:
#line 673 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildImportStmt((yyvsp[-3].pexpr), (yyvsp[-1].pch), (yyvsp[-5].b)); }
#line 6367 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 72:
#line 674 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist), (yyvsp[-6].b)); }
#line 6373 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 73:
#line 678 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 6379 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 74:
#line 683 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 6385 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 75:
#line 685 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 6391 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 76:
#line 687 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 6397 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 77:
#line 689 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 6403 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 78:
#line 691 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 6409 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 79:
#line 697 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 6415 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 80:
#line 698 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6421 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 81:
#line 702 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6427 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 82:
#line 703 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6433 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 83:
#line 704 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6439 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 84:
#line 705 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6445 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 85:
#line 706 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6451 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 86:
#line 707 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6457 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 87:
#line 710 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6463 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 88:
#line 711 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6469 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 89:
#line 712 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6475 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 90:
#line 713 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6481 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 91:
#line 714 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6487 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 92:
#line 715 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6493 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 93:
#line 727 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6499 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 94:
#line 728 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6505 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 95:
#line 739 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bool"; }
#line 6511 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 96:
#line 740 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "int"; }
#line 6517 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 97:
#line 741 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "uint"; }
#line 6523 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 98:
#line 742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "real"; }
#line 6529 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 99:
#line 743 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "imag"; }
#line 6535 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 100:
#line 744 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "complex"; }
#line 6541 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 101:
#line 745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bytes"; }
#line 6547 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 102:
#line 746 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "string"; }
#line 6553 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 103:
#line 747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "sync"; }
#line 6559 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 104:
#line 748 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "single"; }
#line 6565 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 105:
#line 749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "owned"; }
#line 6571 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 106:
#line 750 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "shared"; }
#line 6577 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 107:
#line 751 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "borrowed"; }
#line 6583 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 108:
#line 752 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "unmanaged"; }
#line 6589 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 109:
#line 753 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "domain"; }
#line 6595 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 110:
#line 754 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "index"; }
#line 6601 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 111:
#line 755 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "locale"; }
#line 6607 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 112:
#line 756 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "nothing"; }
#line 6613 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 113:
#line 757 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "void"; }
#line 6619 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 114:
#line 761 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6625 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 115:
#line 762 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 6631 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 116:
#line 763 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6637 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 117:
#line 764 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6643 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 118:
#line 765 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6649 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 119:
#line 766 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6655 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 120:
#line 767 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6661 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 121:
#line 768 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 6667 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 122:
#line 769 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6673 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 123:
#line 770 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6679 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 124:
#line 771 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 6685 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 125:
#line 772 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 6691 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 126:
#line 779 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_syncvar"; }
#line 6697 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 127:
#line 780 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_singlevar"; }
#line 6703 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 128:
#line 781 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_domain"; }
#line 6709 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 129:
#line 782 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_index"; }
#line 6715 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 130:
#line 786 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6721 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 131:
#line 787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6727 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 132:
#line 791 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6733 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 133:
#line 792 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6739 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 134:
#line 796 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6745 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 136:
#line 798 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6751 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 137:
#line 808 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 6757 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 138:
#line 809 "chapel.ypp" /* yacc.c:1663  */
    { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6763 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 146:
#line 823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6769 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 147:
#line 824 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6775 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 148:
#line 825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6781 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 149:
#line 826 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6787 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 150:
#line 831 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6796 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 151:
#line 836 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 6810 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 152:
#line 846 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6819 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 153:
#line 851 "chapel.ypp" /* yacc.c:1663  */
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 6834 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 154:
#line 862 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6843 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 155:
#line 867 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 6857 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 156:
#line 877 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6866 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 157:
#line 882 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 6880 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 158:
#line 893 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 6888 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 159:
#line 897 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 6896 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 160:
#line 902 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 6908 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 161:
#line 910 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 6920 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 162:
#line 921 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6928 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 163:
#line 927 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6934 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 164:
#line 928 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6940 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 165:
#line 929 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 6946 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 166:
#line 930 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6952 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 167:
#line 931 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 6958 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 168:
#line 932 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6964 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 169:
#line 933 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 6970 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 170:
#line 934 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6976 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 171:
#line 935 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 6982 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 172:
#line 936 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6988 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 173:
#line 937 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6994 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 174:
#line 938 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7000 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 175:
#line 939 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7006 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 176:
#line 940 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7012 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 177:
#line 941 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7018 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 178:
#line 942 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7024 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 179:
#line 943 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7030 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 180:
#line 944 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7036 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 181:
#line 946 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7046 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 182:
#line 952 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7056 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 183:
#line 958 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7066 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 184:
#line 964 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7076 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 185:
#line 970 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7087 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 186:
#line 977 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7098 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 187:
#line 984 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7106 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 188:
#line 988 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7114 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 189:
#line 994 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7120 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 190:
#line 998 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7126 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 191:
#line 999 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7132 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 192:
#line 1000 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7138 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 193:
#line 1001 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7144 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 194:
#line 1002 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7151 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 195:
#line 1004 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7158 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 196:
#line 1006 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7165 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 197:
#line 1008 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7172 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 198:
#line 1013 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 7178 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 199:
#line 1016 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 7184 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 200:
#line 1017 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 7190 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 201:
#line 1018 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 7196 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 202:
#line 1019 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 7202 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 203:
#line 1020 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7208 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 204:
#line 1021 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7214 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 205:
#line 1025 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7220 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 206:
#line 1026 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7226 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 207:
#line 1030 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 7232 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 208:
#line 1031 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7238 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 209:
#line 1032 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7244 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 210:
#line 1036 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 7250 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 211:
#line 1037 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 7256 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 212:
#line 1041 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 7262 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 213:
#line 1045 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 7268 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 214:
#line 1047 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 7274 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 215:
#line 1051 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7280 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 216:
#line 1052 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7286 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 217:
#line 1057 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 7292 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 218:
#line 1059 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7298 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 219:
#line 1061 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7304 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 220:
#line 1068 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7318 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 221:
#line 1078 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7332 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 222:
#line 1091 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7342 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 223:
#line 1096 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7352 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 224:
#line 1101 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7362 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 225:
#line 1109 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 7368 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 226:
#line 1110 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 7374 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 227:
#line 1115 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 7380 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 228:
#line 1117 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7386 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 229:
#line 1119 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 7392 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 230:
#line 1124 "chapel.ypp" /* yacc.c:1663  */
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
#line 7412 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 231:
#line 1140 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 7420 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 232:
#line 1147 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7430 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 233:
#line 1156 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 7442 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 234:
#line 1164 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 7450 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 235:
#line 1168 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 7458 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 236:
#line 1174 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 7464 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 237:
#line 1175 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 7470 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 238:
#line 1180 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 7479 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 239:
#line 1185 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7488 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 240:
#line 1190 "chapel.ypp" /* yacc.c:1663  */
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
#line 7510 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 241:
#line 1212 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 7518 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 242:
#line 1215 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 7527 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 243:
#line 1219 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 7536 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 244:
#line 1227 "chapel.ypp" /* yacc.c:1663  */
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 7550 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 245:
#line 1237 "chapel.ypp" /* yacc.c:1663  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7561 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 246:
#line 1244 "chapel.ypp" /* yacc.c:1663  */
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

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[-6].pfnsymbol), (yyvsp[-4].retTag), (yyvsp[-3].pexpr), (yyvsp[-2].b), (yyvsp[-1].lifetimeAndWhere).where, (yyvsp[-1].lifetimeAndWhere).lifetime, (yyvsp[0].pblockstmt), (yylsp[-8]).comment);
      context->latestComment = NULL;
    }
#line 7588 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 247:
#line 1270 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 7596 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 248:
#line 1274 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7605 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 249:
#line 1279 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 7613 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 250:
#line 1283 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7622 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 251:
#line 1288 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 7630 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 253:
#line 1295 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7636 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 254:
#line 1299 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7642 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 255:
#line 1300 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 7648 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 256:
#line 1301 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&"; }
#line 7654 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 257:
#line 1302 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|"; }
#line 7660 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 258:
#line 1303 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^"; }
#line 7666 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 259:
#line 1304 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "~"; }
#line 7672 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 260:
#line 1305 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "=="; }
#line 7678 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 261:
#line 1306 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!="; }
#line 7684 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 262:
#line 1307 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<="; }
#line 7690 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 263:
#line 1308 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">="; }
#line 7696 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 264:
#line 1309 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<"; }
#line 7702 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 265:
#line 1310 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">"; }
#line 7708 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 266:
#line 1311 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+"; }
#line 7714 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 267:
#line 1312 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-"; }
#line 7720 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 268:
#line 1313 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*"; }
#line 7726 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 269:
#line 1314 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/"; }
#line 7732 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 270:
#line 1315 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<"; }
#line 7738 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 271:
#line 1316 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>"; }
#line 7744 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 272:
#line 1317 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%"; }
#line 7750 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 273:
#line 1318 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**"; }
#line 7756 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 274:
#line 1319 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!"; }
#line 7762 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 275:
#line 1320 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_by"; }
#line 7768 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 276:
#line 1321 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "#"; }
#line 7774 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 277:
#line 1322 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_align"; }
#line 7780 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 278:
#line 1323 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<=>"; }
#line 7786 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 279:
#line 1324 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<~>"; }
#line 7792 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 280:
#line 1325 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "init="; }
#line 7798 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 281:
#line 1326 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 7804 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 282:
#line 1330 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "="; }
#line 7810 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 283:
#line 1331 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+="; }
#line 7816 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 284:
#line 1332 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-="; }
#line 7822 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 285:
#line 1333 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*="; }
#line 7828 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 286:
#line 1334 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/="; }
#line 7834 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 287:
#line 1335 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%="; }
#line 7840 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 288:
#line 1336 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**="; }
#line 7846 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 289:
#line 1337 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&="; }
#line 7852 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 290:
#line 1338 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|="; }
#line 7858 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 291:
#line 1339 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^="; }
#line 7864 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 292:
#line 1340 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>="; }
#line 7870 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 293:
#line 1341 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<="; }
#line 7876 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 294:
#line 1345 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7882 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 295:
#line 1346 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7888 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 296:
#line 1350 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7894 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 297:
#line 1354 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7900 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 298:
#line 1355 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7906 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 299:
#line 1356 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7912 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 300:
#line 1361 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7918 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 301:
#line 1363 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7924 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 302:
#line 1365 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7930 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 303:
#line 1367 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7936 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 304:
#line 1369 "chapel.ypp" /* yacc.c:1663  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7942 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 305:
#line 1373 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7948 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 306:
#line 1374 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7954 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 307:
#line 1378 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_IN; }
#line 7960 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 308:
#line 1379 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7966 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 309:
#line 1380 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_OUT; }
#line 7972 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 310:
#line 1381 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST; }
#line 7978 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 311:
#line 1382 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7984 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 312:
#line 1383 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7990 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 313:
#line 1384 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7996 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 314:
#line 1385 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF; }
#line 8002 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 315:
#line 1386 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE; }
#line 8008 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 316:
#line 1390 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 8014 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 317:
#line 1391 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 8020 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 318:
#line 1392 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF;   }
#line 8026 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 319:
#line 1393 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 8032 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 320:
#line 1394 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST;   }
#line 8038 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 321:
#line 1395 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 8044 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 322:
#line 1399 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 8050 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 323:
#line 1400 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 8056 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 324:
#line 1404 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 8062 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 325:
#line 1405 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 8068 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 326:
#line 1406 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 8074 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 327:
#line 1407 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_REF; }
#line 8080 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 328:
#line 1408 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_PARAM; }
#line 8086 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 329:
#line 1409 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_TYPE; }
#line 8092 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 330:
#line 1413 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 8098 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 331:
#line 1414 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 8104 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 332:
#line 1417 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = NULL; }
#line 8110 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 335:
#line 1423 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 8116 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 336:
#line 1427 "chapel.ypp" /* yacc.c:1663  */
    { 
      DefExpr * de = new DefExpr(new VarSymbol((yyvsp[0].pch)));
      USR_WARN(de, "support for space in query expressions is deprecated");
      (yyval.pexpr) = de;
  }
#line 8126 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 337:
#line 1432 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 8132 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 338:
#line 1436 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8138 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 339:
#line 1437 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8144 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 340:
#line 1438 "chapel.ypp" /* yacc.c:1663  */
    { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 8154 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 341:
#line 1446 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 8160 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 342:
#line 1448 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 8166 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 343:
#line 1450 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 8172 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 344:
#line 1452 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8178 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 345:
#line 1454 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8184 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 346:
#line 1459 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8190 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 347:
#line 1461 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8196 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 348:
#line 1465 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8202 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 349:
#line 1466 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8208 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 350:
#line 1467 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8214 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 351:
#line 1468 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8220 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 352:
#line 1469 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8226 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 353:
#line 1470 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8232 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 354:
#line 1471 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 8238 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 355:
#line 1476 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 8244 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 356:
#line 1478 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 8250 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 357:
#line 1483 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 8256 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 358:
#line 1485 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 8262 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 359:
#line 1487 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 8268 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 360:
#line 1492 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 8285 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 361:
#line 1505 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 8303 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 362:
#line 1521 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8309 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 363:
#line 1523 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8315 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 364:
#line 1525 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 8321 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 365:
#line 1529 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 8327 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 366:
#line 1530 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 8333 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 367:
#line 1531 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 8339 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 368:
#line 1532 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 8345 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 369:
#line 1533 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 8351 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 370:
#line 1538 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8361 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 371:
#line 1544 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8370 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 373:
#line 1553 "chapel.ypp" /* yacc.c:1663  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 8379 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 374:
#line 1561 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 8385 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 375:
#line 1563 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8391 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 376:
#line 1568 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 8397 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 377:
#line 1570 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 8403 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 378:
#line 1572 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 8409 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 379:
#line 1577 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 8415 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 380:
#line 1579 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 8421 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 381:
#line 1581 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8427 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 382:
#line 1587 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8433 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 383:
#line 1588 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 8439 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 384:
#line 1589 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8445 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 385:
#line 1595 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8451 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 386:
#line 1597 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 8457 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 387:
#line 1599 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8465 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 388:
#line 1603 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 8473 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 389:
#line 1607 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8479 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 390:
#line 1609 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8487 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 391:
#line 1613 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8495 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 392:
#line 1620 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8501 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 393:
#line 1621 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8507 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 394:
#line 1622 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8513 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 395:
#line 1623 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8519 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 396:
#line 1624 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8525 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 397:
#line 1629 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8531 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 398:
#line 1630 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8537 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 399:
#line 1631 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 8543 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 400:
#line 1632 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8549 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 401:
#line 1633 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8555 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 402:
#line 1654 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8563 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 403:
#line 1658 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 8571 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 404:
#line 1662 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 8583 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 405:
#line 1670 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8591 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 406:
#line 1676 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8597 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 407:
#line 1677 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8603 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 408:
#line 1678 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8609 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 409:
#line 1683 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8615 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 410:
#line 1685 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8621 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 411:
#line 1691 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8627 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 412:
#line 1693 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8633 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 413:
#line 1695 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 8639 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 414:
#line 1699 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8645 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 415:
#line 1700 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8651 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 416:
#line 1701 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8657 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 417:
#line 1702 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8663 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 418:
#line 1703 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8669 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 419:
#line 1709 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8675 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 420:
#line 1710 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8681 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 421:
#line 1711 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8687 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 422:
#line 1712 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8693 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 423:
#line 1716 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 8699 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 424:
#line 1717 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8705 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 425:
#line 1721 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 8711 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 426:
#line 1722 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8717 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 427:
#line 1723 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8723 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 428:
#line 1727 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8729 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 429:
#line 1728 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8735 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 430:
#line 1732 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 8741 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 432:
#line 1737 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8747 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 433:
#line 1738 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8753 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 434:
#line 1742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8759 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 435:
#line 1743 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8765 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 436:
#line 1744 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8771 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 437:
#line 1745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8777 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 438:
#line 1749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8783 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 439:
#line 1750 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8789 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 440:
#line 1762 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8795 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 441:
#line 1764 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 8801 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 442:
#line 1766 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8807 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 448:
#line 1777 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 8813 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 449:
#line 1779 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 8819 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 450:
#line 1781 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 8825 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 451:
#line 1783 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 8831 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 452:
#line 1785 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 8837 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 453:
#line 1787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 8843 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 454:
#line 1789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 8849 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 455:
#line 1792 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 8855 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 456:
#line 1794 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8861 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 457:
#line 1796 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 8867 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 458:
#line 1798 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 8873 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 459:
#line 1800 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 8879 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 460:
#line 1802 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8885 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 461:
#line 1804 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 8891 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 462:
#line 1806 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 8897 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 463:
#line 1809 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 8903 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 464:
#line 1811 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 8909 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 465:
#line 1816 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8915 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 466:
#line 1818 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8921 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 467:
#line 1820 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8927 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 468:
#line 1822 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8933 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 469:
#line 1824 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8939 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 470:
#line 1826 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8945 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 471:
#line 1828 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8951 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 472:
#line 1830 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8957 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 473:
#line 1832 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8963 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 474:
#line 1834 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8969 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 475:
#line 1836 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8975 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 476:
#line 1838 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8981 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 477:
#line 1840 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8992 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 478:
#line 1847 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 9002 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 479:
#line 1853 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 9012 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 480:
#line 1859 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9022 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 481:
#line 1865 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 9032 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 482:
#line 1874 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9038 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 483:
#line 1883 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 9044 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 491:
#line 1899 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9050 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 492:
#line 1903 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 9056 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 494:
#line 1908 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9062 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 495:
#line 1912 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9068 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 496:
#line 1913 "chapel.ypp" /* yacc.c:1663  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9074 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 497:
#line 1917 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9080 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 498:
#line 1921 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9086 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 499:
#line 1922 "chapel.ypp" /* yacc.c:1663  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9092 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 500:
#line 1927 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 9100 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 501:
#line 1931 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9108 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 502:
#line 1935 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9116 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 503:
#line 1941 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 9122 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 504:
#line 1942 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 9128 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 505:
#line 1943 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 9134 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 506:
#line 1944 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 9140 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 507:
#line 1945 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 9146 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 508:
#line 1946 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 9152 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 510:
#line 1952 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9158 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 511:
#line 1957 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 9164 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 512:
#line 1959 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 9172 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 513:
#line 1963 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 9180 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 514:
#line 1967 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 9188 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 515:
#line 1971 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 9196 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 516:
#line 1979 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 9203 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 517:
#line 1984 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9213 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 518:
#line 1990 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9223 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 519:
#line 1996 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9234 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 520:
#line 2003 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9245 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 521:
#line 2012 "chapel.ypp" /* yacc.c:1663  */
    { USR_WARN((yyvsp[-1].pexpr), "the syntax 'new owned(an expression)' is deprecated");
      USR_PRINT((yyvsp[-1].pexpr), "instead use 'owned.create(an expression)'");
      (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtOwned->symbol, (yyvsp[-1].pexpr))); }
#line 9253 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 522:
#line 2016 "chapel.ypp" /* yacc.c:1663  */
    { USR_WARN((yyvsp[-1].pexpr), "the syntax 'new shared(an expression)' is deprecated");
      USR_PRINT((yyvsp[-1].pexpr), "instead use 'shared.create(an expression)'");
      (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtShared->symbol, (yyvsp[-1].pexpr))); }
#line 9261 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 523:
#line 2023 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 9267 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 532:
#line 2039 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 9273 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 533:
#line 2041 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9279 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 534:
#line 2043 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9285 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 535:
#line 2045 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 9291 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 536:
#line 2047 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 9297 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 537:
#line 2049 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 9303 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 538:
#line 2053 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9309 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 539:
#line 2054 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9315 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 540:
#line 2058 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 9321 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 541:
#line 2059 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 9327 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 542:
#line 2060 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9333 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 547:
#line 2078 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9339 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 548:
#line 2079 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9345 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 549:
#line 2080 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 9351 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 550:
#line 2081 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9357 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 551:
#line 2085 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9363 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 552:
#line 2086 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9369 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 553:
#line 2087 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 9375 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 554:
#line 2091 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 9381 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 555:
#line 2092 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 9387 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 556:
#line 2093 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 9393 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 557:
#line 2094 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 9399 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 558:
#line 2095 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 9405 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 559:
#line 2103 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 9411 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 560:
#line 2104 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 9417 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 561:
#line 2105 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 9423 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 562:
#line 2106 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 9429 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 563:
#line 2110 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 9435 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 564:
#line 2111 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 9441 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 566:
#line 2116 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 9447 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 567:
#line 2117 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 9453 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 568:
#line 2118 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 9459 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 569:
#line 2119 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 9465 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 570:
#line 2120 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 9471 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 571:
#line 2121 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 9477 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 572:
#line 2122 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 9483 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 573:
#line 2123 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 9489 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 574:
#line 2124 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 9495 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 575:
#line 2125 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 9501 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 576:
#line 2126 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 9507 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 577:
#line 2128 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 9515 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 578:
#line 2132 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 9523 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 579:
#line 2139 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9529 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 580:
#line 2140 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9535 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 581:
#line 2144 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9541 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 582:
#line 2145 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9547 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 583:
#line 2146 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9553 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 584:
#line 2147 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9559 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 585:
#line 2148 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9565 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 586:
#line 2149 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9571 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 587:
#line 2150 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9577 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 588:
#line 2151 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9583 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 589:
#line 2152 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9589 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 590:
#line 2153 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9595 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 591:
#line 2154 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9601 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 592:
#line 2155 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9607 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 593:
#line 2156 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9613 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 594:
#line 2157 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9619 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 595:
#line 2158 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9625 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 596:
#line 2159 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9631 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 597:
#line 2160 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9637 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 598:
#line 2161 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9643 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 599:
#line 2162 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9649 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 600:
#line 2163 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9655 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 601:
#line 2164 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9661 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 602:
#line 2165 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9667 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 603:
#line 2166 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9673 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 604:
#line 2170 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 9679 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 605:
#line 2171 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 9685 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 606:
#line 2172 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 9691 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 607:
#line 2173 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 9697 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 608:
#line 2174 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 9703 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 609:
#line 2175 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9709 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 610:
#line 2176 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 9715 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 611:
#line 2180 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9721 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 612:
#line 2181 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9727 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 613:
#line 2182 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9733 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 614:
#line 2183 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9739 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 615:
#line 2187 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9745 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 616:
#line 2188 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9751 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 617:
#line 2189 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9757 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 618:
#line 2190 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9763 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 619:
#line 2195 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 9769 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 620:
#line 2196 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 9775 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 621:
#line 2197 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 9781 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 622:
#line 2198 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 9787 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 623:
#line 2199 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 9793 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 624:
#line 2200 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 9799 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 625:
#line 2201 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 9805 "bison-chapel.cpp" /* yacc.c:1663  */
    break;


#line 9809 "bison-chapel.cpp" /* yacc.c:1663  */
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
