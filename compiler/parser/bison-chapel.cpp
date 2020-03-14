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
#define YYLAST   18076

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  173
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  146
/* YYNRULES -- Number of rules.  */
#define YYNRULES  624
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1120

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
     672,   673,   678,   682,   684,   686,   688,   690,   697,   698,
     702,   703,   704,   705,   706,   707,   710,   711,   712,   713,
     714,   715,   727,   728,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,   750,   751,   752,   753,   754,
     755,   756,   757,   761,   762,   763,   764,   765,   766,   767,
     768,   769,   770,   771,   772,   779,   780,   781,   782,   786,
     787,   791,   792,   796,   797,   798,   808,   808,   813,   814,
     815,   816,   817,   818,   819,   823,   824,   825,   826,   831,
     830,   846,   845,   862,   861,   877,   876,   892,   896,   901,
     909,   920,   927,   928,   929,   930,   931,   932,   933,   934,
     935,   936,   937,   938,   939,   940,   941,   942,   943,   944,
     945,   951,   957,   963,   969,   976,   983,   987,   994,   998,
     999,  1000,  1001,  1002,  1004,  1006,  1008,  1013,  1016,  1017,
    1018,  1019,  1020,  1021,  1025,  1026,  1030,  1031,  1032,  1036,
    1037,  1041,  1044,  1046,  1051,  1052,  1056,  1058,  1060,  1067,
    1077,  1091,  1096,  1101,  1109,  1110,  1115,  1116,  1118,  1123,
    1139,  1146,  1155,  1163,  1167,  1174,  1175,  1180,  1185,  1179,
    1212,  1215,  1219,  1227,  1237,  1226,  1269,  1273,  1278,  1282,
    1287,  1294,  1295,  1299,  1300,  1301,  1302,  1303,  1304,  1305,
    1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,  1314,  1315,
    1316,  1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1326,  1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,
    1339,  1340,  1341,  1345,  1346,  1350,  1354,  1355,  1356,  1360,
    1362,  1364,  1366,  1368,  1373,  1374,  1378,  1379,  1380,  1381,
    1382,  1383,  1384,  1385,  1386,  1390,  1391,  1392,  1393,  1394,
    1395,  1399,  1400,  1404,  1405,  1406,  1407,  1408,  1409,  1413,
    1414,  1417,  1418,  1422,  1423,  1427,  1432,  1436,  1437,  1438,
    1446,  1447,  1449,  1451,  1453,  1458,  1460,  1465,  1466,  1467,
    1468,  1469,  1470,  1471,  1475,  1477,  1482,  1484,  1486,  1491,
    1504,  1521,  1522,  1524,  1529,  1530,  1531,  1532,  1533,  1537,
    1543,  1551,  1552,  1560,  1562,  1567,  1569,  1571,  1576,  1578,
    1580,  1587,  1588,  1589,  1594,  1596,  1598,  1602,  1606,  1608,
    1612,  1620,  1621,  1622,  1623,  1624,  1629,  1630,  1631,  1632,
    1633,  1653,  1657,  1661,  1669,  1676,  1677,  1678,  1682,  1684,
    1690,  1692,  1694,  1699,  1700,  1701,  1702,  1703,  1709,  1710,
    1711,  1712,  1716,  1717,  1721,  1722,  1723,  1727,  1728,  1732,
    1733,  1737,  1738,  1742,  1743,  1744,  1745,  1749,  1750,  1761,
    1763,  1765,  1771,  1772,  1773,  1774,  1775,  1776,  1778,  1780,
    1782,  1784,  1786,  1788,  1791,  1793,  1795,  1797,  1799,  1801,
    1803,  1805,  1808,  1810,  1815,  1817,  1819,  1821,  1823,  1825,
    1827,  1829,  1831,  1833,  1835,  1837,  1839,  1846,  1852,  1858,
    1864,  1873,  1883,  1891,  1892,  1893,  1894,  1895,  1896,  1897,
    1898,  1903,  1904,  1908,  1912,  1913,  1917,  1921,  1922,  1926,
    1930,  1934,  1941,  1942,  1943,  1944,  1945,  1946,  1950,  1951,
    1956,  1958,  1962,  1966,  1970,  1978,  1983,  1989,  1995,  2002,
    2011,  2015,  2022,  2030,  2031,  2032,  2033,  2034,  2035,  2036,
    2037,  2038,  2040,  2042,  2044,  2046,  2048,  2053,  2054,  2058,
    2059,  2060,  2064,  2065,  2066,  2067,  2078,  2079,  2080,  2081,
    2085,  2086,  2087,  2091,  2092,  2093,  2094,  2095,  2103,  2104,
    2105,  2106,  2110,  2111,  2115,  2116,  2117,  2118,  2119,  2120,
    2121,  2122,  2123,  2124,  2125,  2126,  2127,  2131,  2139,  2140,
    2144,  2145,  2146,  2147,  2148,  2149,  2150,  2151,  2152,  2153,
    2154,  2155,  2156,  2157,  2158,  2159,  2160,  2161,  2162,  2163,
    2164,  2165,  2166,  2170,  2171,  2172,  2173,  2174,  2175,  2176,
    2180,  2181,  2182,  2183,  2187,  2188,  2189,  2190,  2195,  2196,
    2197,  2198,  2199,  2200,  2201
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

#define YYPACT_NINF -1000

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1000)))

#define YYTABLE_NINF -575

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1000,   110,  2885, -1000,  -100, -1000, -1000, -1000, -1000, -1000,
   -1000, -1000,  4371,    67,   120, -1000, 12977, -1000, 17680,    67,
   10075, -1000,   702,    61,   120,  4371, 10075,  4371,    24, 17738,
   10238,  8107, -1000,  8270,  8926,  6666, 10075, -1000,   122, -1000,
   -1000, 17775, -1000, 13319,  9089, -1000, 10075,   157, -1000, -1000,
   -1000, 10075, -1000, 12977, -1000, 10075,   218,   129,   190, 17968,
   -1000, 17833, -1000,  9254,  7489, 10075,  9089, 12977, 10075,   214,
   -1000,   155,  4371, -1000, 10075, -1000, 10401, 10401, 17775, -1000,
   -1000, 12977, -1000, -1000, 10075, 10075, -1000, 10075, -1000, 10075,
   -1000, -1000, 12683, 10075, -1000, 10075, -1000, -1000, -1000,  3383,
    6831,  8435, -1000,  4208, -1000,   164, -1000,   287, -1000,    46,
   -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
   -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, 17775,
   -1000, 17775,   331,    58, -1000, -1000, 13319, -1000,   263, -1000,
     286, -1000, -1000,   276,   307,   319, 10075,   313,   321, 17169,
   12676,   346,   322,   327, -1000, -1000, -1000, -1000, -1000, -1000,
   -1000,   159, -1000, -1000, 17169,   330,  4371, -1000, -1000,   344,
   10075, -1000, -1000, 10075, 10075, 10075, -1000, 10075,  9254,  9254,
     354, -1000, -1000, -1000, -1000,   -30,   365, -1000, -1000,   345,
   15393, 17775, 13319, -1000,   357, -1000,   152, 17169,   360,  7654,
     431, 17870, 17169,   483,   437, -1000, 17928, 17775,   483, 17775,
     364,    29, 14831,     8, 14755,   226, -1000, 15243, 14031,  7654,
   -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
   -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
   -1000, -1000, -1000, -1000,  4371, -1000,   368,  2388,    85,   -13,
   -1000,  4371, -1000, -1000, 14983,   604, -1000,   369,   376, -1000,
   14983,   -30,   604, -1000,  7654,  1723, -1000, -1000,  9417, -1000,
   -1000, -1000, 17775, -1000,   165, 17169, 10075, 10075, -1000, 17169,
     366, 15144, -1000, 14983,   -30, 17169,   380,  7654, -1000, 17169,
   15691, -1000, -1000, 15723, 12970, -1000, -1000, 15780,   428,   367,
     -30, 14983, 15868,   406,   406,  2188,   604,   604,   248, -1000,
   -1000,  3548,   132, -1000, 10075, -1000,    10,    83, -1000,   -54,
       2, 15925,   109,   546, -1000,  3713, -1000,   508, 10075, 10075,
     426,   412, -1000, -1000, -1000,   231,   373, -1000, 10075,   436,
   10075, 10075, 10075,  8926,  8926, 10075,   407, 10075, 10075, 10075,
   10075, 10075,   397, 12683, 10075, 10075, 10075, 10075, 10075, 10075,
   10075, 10075, 10075, 10075, 10075, 10075, 10075, 10075, 10075, -1000,
   -1000, -1000, -1000, -1000, -1000,  8598,  8598, -1000, -1000, -1000,
   -1000,  8598, -1000, -1000,  8598,  8598,  7654,  7654,  8926,  8926,
    7326, -1000, -1000, 15442, 15539, 15957,    66,  3878, -1000,  8926,
      29,   421,   241, -1000, 10075, -1000, 10075,   470, -1000,   429,
     453, -1000, -1000, -1000, 17775, -1000, 13319, -1000, 17775,   438,
   -1000, 13319,   551,  9254, -1000,  4534,  8926, -1000,   439, -1000,
      29,  4697,  8926, -1000,    29, 10564, 10075, -1000,  4371,   568,
   10075,   442, -1000,   454, -1000, -1000,  2388, -1000,   464,   444,
   -1000, 10727,   495, 10075, 13319, -1000, -1000, 10075, 10075, -1000,
     448, -1000, -1000,  9254, -1000, 17169, 17169, -1000,    26, -1000,
    7654,   451, -1000,   598, -1000,   598, -1000, 10890,   489, -1000,
   -1000, -1000, -1000, -1000, -1000,  8763, -1000, 13726,  6996, -1000,
    7161, -1000,  4371,   462,  8926,  6168,  3218,   463, 10075,  6500,
   -1000, -1000,   274, -1000,  4043, 17775, 15482,   243, 15296,  9254,
     469, 17643,    23, -1000, 16037, 17450, 17450,   309, -1000,   309,
   -1000,   309,  1874,   504,  1060,  1513,   -30,   406,   468, -1000,
   -1000, -1000, -1000,  2188, 13955,   309, 16447, 16447, 17450, 16447,
   16447,  1387,   406, 13955, 17482,  1387,   604,   604,   406,   476,
     482,   488,   490,   492,   485,   484, -1000,   309, -1000,   309,
      33, -1000, -1000, -1000,   140, -1000,  1836, 17265,   347, 11053,
    8926, 11216,  8926, 10075,  8926, 13239,    67, 16113, -1000, -1000,
   -1000, 17169, 16145,  7654, -1000,  7654, 17775,   426,   247, 17775,
     426, -1000,   277, 10075,   142, 10075, 17169,    31, 15059,  7326,
   -1000, 10075, 17169,    25, 14907, -1000,   497,   514,   498, 16194,
     514,   502,   627, 16290,  4371, 15091, -1000,    59, -1000, -1000,
   -1000, -1000, -1000, -1000,   778,   222, -1000, 13377, -1000,   168,
     496,  2388,    85,    24,   122, 10075, 10075,  6003, -1000, -1000,
     677,  7817, -1000, 17169, -1000, 13822, 13879, -1000, -1000, 17169,
     503,    17,   500, -1000,  2153, -1000, -1000,   317, 17775, -1000,
   -1000, -1000, -1000, -1000, -1000, -1000,  4371,   -23, 13491, -1000,
   -1000, 17169,  4371, 17169, -1000, 16339, -1000, -1000, -1000,    95,
   -1000, 10075, -1000, 10075, 10564, 10075,   529,  1649,   510,   558,
     160, -1000,   594, -1000, -1000, -1000, -1000,  7946,   512, -1000,
   -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,  7326, -1000,
      34,  8926,  8926, 10075,   647, 16371, 10075,   648, 16516,   515,
   13580,    29,    29, -1000, -1000, -1000, -1000,   426,   519, -1000,
     426,   521, -1000, 14983, -1000, 14111,  4860, -1000,  5023, -1000,
     227, -1000, 14191,  5186, -1000,    29,  5349, -1000,    29, -1000,
   10075, -1000,  4371, 10075, -1000,  4371,   657, -1000, -1000, 17775,
    1011, -1000,  2388,   554,   611, -1000, -1000, -1000,    99, -1000,
   -1000,   495,   534,    74, -1000, -1000,   532,   542, -1000,  5512,
    9254, -1000, -1000, -1000, 17775, -1000,   567,   345, -1000, -1000,
    5675,   541,  5838,   543, -1000, 10075,   555, 16548, 15634,   559,
     560,   548,  1201, -1000, 10075, 17775, -1000, -1000,   168,   562,
     106, -1000,   591, -1000,   595,   605,   614,   615,   617, -1000,
     618,   622,   619,   623,   626,   170,   636,   629,   630, -1000,
   -1000, 17775, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
   -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
   -1000, 10075, -1000,   639,   640,   634,   562,   562, -1000, -1000,
   -1000,   495,   229,   232, 16624, 11379, 11542, 16704, 11705, 11868,
   12031, 12194, -1000, -1000,   613, -1000,   631, -1000, -1000,  4371,
   10075, 17169, 10075, 17169,  7326, -1000,  4371, 10075, 17169, -1000,
   10075, 17169, -1000, 17169, -1000, 17169,   743,  4371,   554, -1000,
     620,  9582,    50, -1000,    45, -1000, -1000,  8926, 13108,  7654,
    7654,  4371, -1000,    15,   624, 10075, -1000, 10075, -1000, 17169,
    4371, 10075, -1000, 17169,  4371, 17169, -1000, 10075, 10564, 10075,
   -1000, -1000, -1000, -1000,  1723, -1000,  2567, -1000, 17169, -1000,
      48,   454, -1000, -1000, 13919, -1000, 13435, -1000, -1000, -1000,
   10075, 10075, 10075, 10075, 10075, 10075, 10075, 10075, -1000,  1373,
   -1000, 17401, 16290, 14271, 14351, -1000, 16290, 14431, 14511,  4371,
   -1000,   495,   554,  6334, -1000, -1000, -1000,   149,  9254, -1000,
   -1000,    89, 10075,   -15, 13637, -1000,   697,   632,   635,   360,
   -1000,   345, 17169, 14591, -1000, 14671, -1000, 17169,   638,   642,
   -1000, -1000, 12357,   683,   244, -1000,   656,   660,   562,   562,
   16736, 16816, 16896, 16928, 17008, 17088, 17415, -1000, 17473, -1000,
    4371,  4371,  4371,  4371, -1000, -1000,    50,  9747,    76, -1000,
   17169, -1000,   105, -1000,    -9, -1000,   139, 17120, -1000, -1000,
   -1000, 12194,   645,   646, -1000,  4371,  4371, -1000, -1000,  3050,
   -1000, -1000,   311, -1000,    45, -1000, -1000, -1000, 10075, 10075,
   10075, 10075, 10075, 10075, -1000, -1000, 16290, 16290, 16290, 16290,
   -1000, -1000, -1000, -1000, -1000,   242,  8926,  2676, -1000, 10075,
      89,   105,   105,   105,   105,   105,   105,    89,   913, -1000,
   -1000, 16290, 16290,   643, 12520,    86,    54, 13677, -1000, -1000,
   17169, -1000, -1000, -1000, -1000, -1000, -1000, -1000,   658, -1000,
   -1000,   351, 12846, -1000, -1000, -1000,  9912, -1000,   359, -1000
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    92,   565,   566,   567,   568,
     569,   570,     0,   491,    78,   113,   460,   120,   462,   491,
       0,   119,     0,   365,    78,     0,     0,     0,     0,   114,
     537,   537,   562,     0,     0,     0,     0,   118,     0,   241,
     115,     0,   237,     0,     0,   122,     0,   510,   482,   571,
     123,     0,   242,   454,   364,     0,     0,     0,   136,    45,
     117,   463,   366,     0,     0,     0,     0,   458,     0,     0,
     121,     0,     0,    93,     0,   563,     0,     0,     0,   116,
     223,   456,   368,   124,     0,     0,   620,     0,   622,     0,
     623,   624,   536,     0,   621,   618,   441,   133,   619,     0,
       0,     0,     4,     0,     5,     0,     9,    47,    10,     0,
      11,    12,    13,    15,   437,   438,    23,    14,   134,   143,
     144,    16,    18,    17,    20,    21,    22,    19,   142,     0,
     140,     0,   528,     0,   138,   141,     0,   139,   542,   524,
     439,   525,   444,   442,     0,     0,     0,   529,   530,     0,
     443,     0,   543,   544,   545,   564,   523,   446,   445,   526,
     527,     0,    39,    25,   452,     0,     0,   492,    79,     0,
       0,   462,   114,     0,     0,     0,   463,     0,     0,     0,
     528,   542,   442,   529,   530,   461,   443,   543,   544,     0,
     491,     0,     0,   367,     0,   197,     0,   422,     0,   429,
     568,   463,   538,   240,   568,   161,   463,     0,   240,     0,
       0,     0,     0,     0,     0,     0,   148,     0,     0,   429,
      86,    94,   106,   100,    99,   108,    89,    98,   109,    95,
     110,    87,   111,   104,    97,   105,   103,   101,   102,    88,
      90,    96,   107,   112,     0,    91,     0,     0,     0,     0,
     371,     0,   130,    33,     0,   605,   514,   511,   512,   513,
       0,   455,   606,     7,   429,   240,   221,   231,   537,   222,
     135,   336,   441,   419,     0,   418,     0,     0,   131,   541,
       0,     0,    36,     0,   459,   447,     0,   429,    37,   453,
       0,   204,   200,     0,   443,   204,   201,     0,   361,     0,
     457,     0,     0,   607,   609,   535,   604,   603,     0,    49,
      52,     0,     0,   424,     0,   426,     0,     0,   425,     0,
       0,   418,     0,     0,     6,     0,    48,     0,     0,     0,
     224,     0,   322,   321,   243,     0,   440,    24,     0,   515,
       0,     0,     0,     0,     0,     0,   608,     0,     0,     0,
       0,     0,     0,   534,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   281,
     288,   289,   290,   285,   287,     0,     0,   283,   286,   284,
     282,     0,   292,   291,     0,     0,   429,   429,     0,     0,
       0,    26,    27,     0,     0,     0,     0,     0,    28,     0,
       0,     0,     0,    29,     0,    30,     0,   437,   435,     0,
     430,   431,   436,   155,     0,   158,     0,   151,     0,     0,
     157,     0,     0,     0,   170,     0,     0,   169,     0,   178,
       0,     0,     0,   176,     0,     0,    58,   145,     0,   190,
       0,     0,    31,   304,   238,   375,     0,   376,   378,     0,
     400,     0,   381,     0,     0,   129,    32,     0,     0,    34,
       0,   137,   335,     0,    72,   539,   540,   132,     0,    35,
     429,     0,   211,   202,   198,   203,   199,     0,   359,   356,
     163,    38,    51,    50,    53,     0,   572,     0,     0,   558,
       0,   560,     0,     0,     0,     0,     0,     0,     0,     0,
     576,     8,     0,    41,     0,     0,     0,     0,    54,     0,
       0,     0,   315,   370,   490,   600,   599,   602,   611,   610,
     615,   614,   596,   593,   594,   595,   532,   583,     0,   555,
     556,   554,   553,   533,   587,   598,   592,   590,   601,   591,
     589,   581,   586,   588,   597,   580,   584,   585,   582,     0,
       0,     0,     0,     0,     0,     0,   613,   612,   617,   616,
     502,   503,   505,   507,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   574,   491,   491,   166,   357,
     369,   423,     0,     0,   449,     0,     0,   224,     0,     0,
     224,   358,     0,     0,     0,     0,   466,     0,     0,     0,
     179,     0,   472,     0,     0,   177,   619,    61,     0,    54,
      59,     0,   189,     0,     0,     0,   448,   309,   306,   307,
     308,   312,   313,   314,   304,     0,   297,     0,   305,   323,
       0,   379,     0,   127,   128,   126,   125,     0,   399,   398,
     524,     0,   373,   522,   372,     0,     0,   552,   421,   420,
       0,     0,     0,   450,     0,   205,   363,   524,     0,   573,
     531,   559,   427,   561,   428,   186,     0,     0,     0,   575,
     184,   476,     0,   578,   577,     0,    43,    42,    40,     0,
      70,     0,    65,     0,     0,    58,   225,     0,     0,   235,
       0,   232,   319,   316,   317,   320,   244,     0,     0,    76,
      77,    75,    74,    73,   550,   551,   504,   506,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   162,   433,   434,   432,   224,     0,   160,
     224,     0,   159,     0,   188,     0,     0,   168,     0,   167,
       0,   497,     0,     0,   174,     0,     0,   172,     0,   146,
       0,   147,     0,     0,   192,     0,   194,   310,   311,     0,
     304,   295,     0,   413,   324,   327,   326,   328,     0,   377,
     380,   381,     0,     0,   382,   383,   520,   521,   213,     0,
       0,   212,   215,   451,     0,   206,   209,     0,   360,   187,
       0,     0,     0,     0,   185,     0,     0,    56,    55,     0,
       0,     0,   240,   230,     0,   233,   229,   318,   323,   293,
      80,   276,    94,   274,   100,    99,    83,    98,    95,   279,
     110,    81,   111,    97,   101,    82,    84,    96,   112,   273,
     255,   258,   256,   257,   268,   259,   272,   264,   262,   275,
     278,   263,   261,   266,   271,   260,   265,   269,   270,   267,
     277,     0,   253,     0,    85,     0,   293,   293,   251,   557,
     495,   381,   542,   542,     0,     0,     0,     0,     0,     0,
       0,     0,   165,   164,     0,   226,     0,   226,   171,     0,
       0,   465,     0,   464,     0,   496,     0,     0,   471,   175,
       0,   470,   173,    55,   191,   481,   193,     0,   413,   298,
       0,     0,   381,   325,   340,   374,   404,     0,   574,   429,
     429,     0,   217,     0,     0,     0,   207,     0,   182,   478,
       0,     0,   180,   477,     0,   579,    71,     0,     0,    58,
      66,    68,   220,   136,   240,   219,   240,   227,   236,   234,
       0,   304,   250,   254,     0,   280,     0,   246,   247,   499,
       0,     0,     0,     0,     0,     0,     0,     0,   226,   240,
     226,   240,   469,     0,     0,   498,   475,     0,     0,     0,
     196,   381,   413,     0,   416,   415,   417,   524,   337,   301,
     299,     0,     0,     0,     0,   402,   524,     0,     0,   218,
     216,     0,   210,     0,   183,     0,   181,    57,     0,     0,
     228,   395,     0,   329,     0,   252,    80,    82,   293,   293,
       0,     0,     0,     0,     0,     0,   240,   154,   240,   150,
       0,     0,     0,     0,   195,   300,   381,   405,     0,   339,
     338,   354,     0,   355,   342,   345,     0,   341,   333,   334,
     239,     0,   516,   517,   208,     0,     0,    67,    69,     0,
     394,   393,   524,   330,   340,   294,   248,   249,     0,     0,
       0,     0,     0,     0,   156,   152,   468,   467,   474,   473,
     303,   302,   407,   408,   410,   524,     0,   574,   353,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   524,   518,
     519,   480,   479,     0,   385,     0,     0,     0,   409,   411,
     344,   346,   347,   350,   351,   352,   348,   349,   343,   390,
     388,   524,   574,   331,   245,   332,   405,   389,   524,   412
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1000, -1000, -1000,     3,  -266,  2092, -1000, -1000, -1000, -1000,
     387,   481,  -323,  -669,  -659, -1000, -1000, -1000, -1000,   314,
     783, -1000,   507,  -188,  -680, -1000,  -865,    18,  -953,  -757,
   -1000,   -57, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
     117, -1000, -1000, -1000,   517, -1000,    30, -1000, -1000, -1000,
   -1000, -1000, -1000,  -494,  -601, -1000, -1000, -1000,    13,   877,
   -1000, -1000, -1000,   176, -1000, -1000, -1000, -1000,  -133,  -147,
    -789, -1000,  -122,    60,   198, -1000, -1000, -1000,    22, -1000,
   -1000,  -265,   240,  -194,  -217,  -249,  -239,    87, -1000,  -181,
   -1000,     6,   807,  -121,   390, -1000,  -404,  -733,  -839, -1000,
    -586,  -468,  -999,  -920,  -832,   -19, -1000,    92, -1000,  -190,
   -1000,   260,   540,  -193, -1000, -1000, -1000,  1080, -1000,    -7,
   -1000, -1000,  -201, -1000,  -579, -1000, -1000, -1000,  1337,  1540,
     -12,   816,   -63,   582, -1000,  1632,  1835, -1000, -1000, -1000,
   -1000, -1000, -1000, -1000, -1000,  -383
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   310,   103,   612,   105,   106,   107,   327,
     108,   311,   607,   611,   608,   109,   110,   111,   112,   113,
     169,   852,   248,   114,   245,   115,   638,   253,   116,   117,
     265,   118,   119,   120,   418,   589,   414,   586,   121,   122,
     719,   123,   124,   125,   473,   655,   787,   126,   127,   651,
     782,   128,   129,   510,   802,   130,   131,   690,   691,   180,
     246,   629,   133,   134,   512,   808,   696,   855,   856,   440,
     942,   444,   625,   626,   627,   628,   697,   334,   768,  1054,
    1114,  1040,   273,   979,   983,  1034,  1035,  1036,   135,   299,
     478,   136,   137,   249,   250,   448,   449,   642,  1051,  1003,
     452,   639,  1073,   976,   902,   312,   196,   316,   317,   409,
     410,   411,   181,   139,   140,   141,   142,   182,   144,   166,
     167,   564,   430,   740,   565,   566,   145,   146,   183,   184,
     149,   208,   412,   186,   151,   187,   188,   154,   155,   156,
     322,   157,   158,   159,   160,   161
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     164,   280,   270,   385,   185,   102,   507,   568,   190,   656,
     401,   407,   189,   434,   197,   335,   800,   854,   202,   202,
     741,   212,   214,   217,   218,   799,   419,   650,   192,   441,
    1039,   407,   254,   453,   255,   450,   974,   318,   905,   260,
     251,   261,   630,   262,   274,   937,   771,   251,   494,  1001,
     692,   275,   279,   281,   283,   284,   285,   743,   162,   428,
     289,   251,   290,   736,   293,   297,   971,   947,   948,   300,
      64,   402,   301,   302,   460,   303,   407,   304,  1098,   706,
     305,   306,   320,   307,   282,   779,   450,   275,   279,   321,
     428,   328,  1031,   728,  -214,   693,   731,   471,     5,   407,
     450,   981,  1079,   346,  -391,   757,   694,  1074,  1031,   332,
       3,   352,   574,   610,   492,   428,   707,  1119,   493,   497,
     907,   428,  1076,   168,   861,   695,   454,   780,   949,   860,
    1080,  -396,   574,  -391,   339,   333,  -214,  1050,   428,    64,
    1026,   495,   758,  1039,   193,   790,  -391,   385,   397,   488,
     211,   213,  -396,   329,   463,  -396,   982,  1099,   164,  -391,
     396,   393,   394,   395,   532,   289,   275,   321,   641,   980,
     496,   397,   451,  -396,  1032,   256,   489,   624,   397,  1000,
     165,   781,   904,   400,  -396,  1033,  1002,   279,   397,   199,
    -214,    73,   397,   978,   397,   764,   554,   555,   407,   407,
    -396,  1033,   937,  -396,   937,   495,  -391,   279,   461,   416,
    -396,  -391,  1113,   495,   421,   495,   568,   397,   319,  1056,
    1057,   192,   490,   451,  -396,   495,   263,   770,   257,   424,
     427,   429,   433,   874,   575,   -64,   876,   451,  1025,   -86,
     765,   388,   908,  -396,  1077,   389,   258,   -92,   499,   491,
     -46,   766,   279,    23,  1112,  1110,   202,  1081,   640,   937,
     999,   937,  -396,   259,   465,   466,   854,  -414,   -46,   998,
     767,   485,   456,  1117,   959,   279,   961,   500,   459,   708,
     652,   463,   407,  1082,   657,  1083,  1084,   219,  -414,  1085,
    1086,   404,  -414,  1071,   264,   588,   486,   -64,    54,   805,
     592,   469,   487,   -88,   463,   965,   709,   286,   734,    62,
     405,   -93,   549,   550,   484,  -414,   506,   508,   551,   480,
     287,   552,   553,   464,   806,   568,   514,   325,   515,   516,
     517,   519,   521,   522,    82,   523,   524,   525,   526,   527,
     315,   533,   534,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,  1016,   900,  1018,
    -406,   760,   610,   279,   279,   326,   884,  -392,  -501,   279,
     454,  -500,   279,   279,   279,   279,   557,   559,   567,   415,
     454,  -406,   681,   760,   420,  -406,   454,   577,   761,   513,
     292,   296,   581,   885,   582,  -501,  -392,   407,  -500,   580,
       5,   682,   745,   748,   594,   729,   162,  -384,  -406,  -392,
    1055,   275,   482,   596,   598,  -386,   454,   528,   578,   602,
     604,  -484,  -392,   609,   609,   318,   613,   318,   615,   712,
     529,   252,   162,   389,  -483,   732,  -384,   342,   676,   408,
     985,   643,   346,   336,  -386,   645,   646,   350,   600,  -384,
     352,   649,   605,   252,   355,   530,  -362,  -386,   279,   408,
     518,   520,  -384,   291,   295,   337,   791,   793,   338,  -392,
    -386,  -488,   406,   649,  -392,  -362,   279,   788,   279,  -489,
    -486,   617,   668,   649,   671,  -485,   673,   675,   343,  -487,
     686,   796,   344,    73,   624,   390,  -549,   275,  -549,   531,
     618,   568,   392,   619,   408,   556,   558,   484,   397,  -384,
      23,   386,   413,   387,  -384,   403,   576,  -386,   417,  -549,
     725,  -549,  -386,   620,   467,   479,   621,   408,    56,   423,
    -546,    39,  -546,   443,   457,   342,   936,   622,  -548,   346,
    -548,   458,   138,   597,   350,   470,   477,   352,   244,   603,
     857,   355,   138,    52,   501,    54,   623,   596,   715,   602,
     718,   613,   720,   671,   509,   138,    62,   138,   505,   721,
     722,   279,  -547,   279,  -547,   511,   398,   352,   775,   579,
     662,   733,   664,   735,   150,   298,   343,   567,   583,   742,
     344,    82,   585,  -296,   150,   584,   591,   593,   252,   252,
     252,   252,   614,   631,   599,   439,   610,   150,   616,   150,
     632,   667,   138,   641,   647,   737,   739,   653,   773,   654,
    -296,   744,   747,   285,   289,   321,   408,   408,   658,   279,
     666,   672,   687,   698,   699,   342,   330,   346,   331,   138,
     700,   252,   350,   138,   351,   352,   701,   252,   702,   355,
     703,   704,   705,   681,   150,   -60,   749,   362,   294,   294,
     751,   752,   769,   366,   367,   368,   783,   778,   463,   797,
     252,   798,   609,   609,   803,   624,   804,   807,   859,   865,
     868,   150,   875,   870,   877,   150,   343,   714,   252,   717,
     344,   897,   901,   936,   903,   936,   567,   909,   298,   519,
     557,   864,   906,   648,   867,   915,   138,   910,   977,   920,
     408,   924,   932,   926,   298,   986,   422,   930,   931,   987,
     988,   407,   407,  -397,   881,   648,   883,   941,   315,    23,
     315,   888,  -113,  -397,   891,   648,  -120,   346,   893,   872,
     873,   895,   350,  -401,   351,   352,  -119,   -89,   150,   355,
     936,   878,   936,  -401,   447,   -87,  -118,   362,  -115,  -122,
    -123,   913,  -397,   889,  -117,   368,   892,  -121,   275,   -90,
    -116,  -124,   945,   -91,    54,   946,   958,   969,   919,   462,
     923,  1053,  -401,   925,   138,    62,   972,   252,  -397,   -86,
     991,   138,   938,   -88,   960,  -397,  1047,  1080,  1042,  1009,
    1048,  1043,  1089,  1090,   191,   617,   504,   194,  -401,  1052,
      82,  1109,   475,  1008,   914,  -401,  -397,   252,   939,  1004,
     899,   252,   759,   724,   618,   408,   150,   619,   518,   556,
     940,  1115,  1070,   150,  1075,  -397,  -401,  1096,  1108,   944,
    -397,  1101,   216,  -397,   644,   726,   203,   620,  1088,     0,
     621,   138,   323,   881,   883,  -401,   888,   891,   919,   923,
    -401,   622,     0,  -401,     0,   138,     0,   962,   963,     0,
     964,     0,   567,     0,   966,   967,     0,   270,   968,   132,
     623,     0,     0,     0,  1075,     0,     0,     0,     0,   132,
       0,     0,     0,   150,     0,   984,   671,   279,   279,     0,
       0,  1111,   132,   992,   132,   993,     0,   150,     0,   995,
       0,     0,     0,     0,     0,   997,   609,   609,     0,  1118,
       0,   587,     0,  1075,     0,   590,     0,     0,     0,     0,
       0,   990,     0,     0,     0,     0,     0,   138,   962,  1010,
    1011,   966,  1012,  1013,  1014,  1015,     0,     0,     0,   132,
       0,     0,     0,   447,  1028,     0,     0,     0,     0,  -403,
       0,   321,     0,     0,     0,   138,  1030,     0,     0,  -403,
    1037,   138,     0,     0,     0,     0,   132,     0,   138,   150,
     132,     0,     0,     0,   252,   252,     0,     0,     0,     0,
     252,   252,     0,     0,     0,     0,     0,     0,  -403,     0,
       0,     0,   756,     0,     0,     0,     0,   150,  1066,  1067,
    1068,  1069,   678,   150,     0,     0,     0,     0,   689,     0,
     150,     0,     0,     0,  -403,     0,     0,     0,     0,   923,
    1095,  -403,   138,  1091,  1092,     0,   138,   321,   617,     0,
       0,   785,     0,   132,   138,     0,  1066,  1067,  1068,  1069,
    1091,  1092,  -403,     0,     0,     0,     0,   618,     0,     0,
     619,     0,     0,     0,  1097,   671,     0,  1100,     0,     0,
       0,  -403,     0,     0,   150,     0,  -403,     0,   150,  -403,
     620,     0,   143,   621,     0,    56,   150,     0,     0,     0,
       0,   342,   143,   727,   622,     0,   730,     0,     0,     0,
     671,     0,     0,     0,   923,   143,   710,   143,   252,   252,
       0,     0,     0,   623,     0,     0,     0,     0,     0,  1078,
     252,   132,   439,     0,     0,     0,     0,     0,   132,   439,
       0,     0,   252,     0,   763,   252,     0,     0,   447,     0,
       0,   975,   343,     0,     0,     0,   344,     0,     0,   408,
     408,     0,   143,     0,   138,     0,     0,     0,     0,     0,
       0,   786,     0,     0,     0,   298,     0,     0,  1102,  1103,
    1104,  1105,  1106,  1107,   916,     0,     0,     0,     0,   143,
       0,     0,     0,   143,     0,     0,     0,     0,   132,     0,
       0,     0,     0,   346,   347,     0,   150,   349,   350,     0,
     351,   352,   132,     0,   853,   355,   138,     0,     0,     0,
       0,     0,   138,   362,     0,     0,     0,     0,  1029,   366,
     367,   368,     0,   266,     0,     0,     0,    22,    23,     0,
       0,     0,     0,     0,     0,     0,   267,   858,    30,   268,
       0,     0,     0,    35,     0,     0,   143,     0,   150,    39,
       0,   862,   863,     0,   150,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   898,  1072,     0,   447,
       0,    52,     0,    54,   132,    56,   138,   933,   138,     0,
     934,     0,   269,   138,    62,     0,   138,     0,     0,     0,
       0,   786,   138,     0,     0,   138,     0,     0,     0,     0,
     252,     0,   132,    78,     0,     0,    80,     0,   132,    82,
       0,     0,   689,     0,     0,   132,     0,  1072,   150,   138,
     150,     0,     0,     0,   143,   150,     0,     0,   150,     0,
     138,   143,   138,     0,   150,     0,     0,   150,   943,   147,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   147,
     439,   439,     0,     0,   439,   439,  1072,     0,     0,    97,
       0,   150,   147,     0,   147,   935,     0,     0,     0,   132,
    1038,     0,   150,   132,   150,     0,     0,     0,  1044,     0,
     439,   132,   439,     0,     0,     0,     0,     0,     0,     0,
       0,   143,     0,     0,     0,   266,     0,     0,     0,    22,
      23,     0,     0,     0,     0,   143,     0,     0,   267,   147,
      30,   268,     0,     0,     0,    35,     0,     0,   342,   138,
       0,    39,     0,     0,     0,     0,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   147,   138,     0,     0,
     147,     0,     0,    52,     0,    54,     0,    56,     0,   933,
       0,   138,   934,   853,   269,     0,    62,     0,     0,     0,
     138,   150,     0,     0,   138,     0,     0,     0,   150,   343,
       0,     0,     0,   344,     0,    78,     0,   143,    80,   150,
       0,    82,     0,  1038,     0,     0,     0,     0,     0,     0,
       0,   132,     0,   150,     0,     0,     0,     0,     0,     0,
       0,     0,   150,   147,     0,   143,   150,     0,     0,   138,
       0,   143,     0,     0,     0,     0,     0,     0,   143,     0,
     346,   347,     0,   348,   349,   350,     0,   351,   352,     0,
       0,    97,   355,     0,     0,     0,     0,  1017,     0,     0,
     362,     0,   148,   132,   342,     0,   366,   367,   368,   132,
       0,   150,   148,     0,     0,     0,     0,     0,     0,     0,
     138,   138,   138,   138,     0,   148,     0,   148,     0,     0,
       0,     0,   143,     0,     0,     0,   143,     0,     0,     0,
       0,   147,     0,     0,   143,   138,   138,     0,   147,     0,
       0,     0,     0,     0,     0,   343,     0,     0,     0,   344,
       0,     0,   150,   150,   150,   150,     0,     0,     0,     0,
       0,     0,   148,   132,     0,   132,     0,     0,     0,     0,
     132,     0,     0,   132,     0,     0,     0,   150,   150,   132,
       0,     0,   132,     0,   152,     0,     0,     0,     0,   148,
       0,     0,     0,   148,   152,     0,   346,   347,   147,     0,
     801,   350,     0,   351,   352,     0,   132,   152,   355,   152,
       0,     0,   147,     0,     0,     0,   362,   132,     0,   132,
       0,  -226,   366,   367,   368,  -226,  -226,     0,     0,     0,
       0,     0,     0,     0,  -226,     0,  -226,  -226,     0,     0,
       0,  -226,     0,     0,   143,     0,     0,  -226,     0,     0,
    -226,     0,     0,     0,   152,     0,   148,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -226,
       0,  -226,     0,  -226,     0,  -226,  -226,     0,  -226,     0,
    -226,   152,  -226,     0,   147,   152,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   266,   143,     0,     0,    22,
      23,  -226,   143,     0,  -226,     0,   132,  -226,   267,     0,
      30,   268,   147,   132,     0,    35,     0,     0,   147,     0,
       0,    39,     0,     0,   132,   147,     0,     0,     0,     0,
       0,     0,     0,     0,   148,     0,     0,     0,   132,     0,
       0,   148,     0,    52,     0,    54,     0,   132,   152,     0,
       0,   132,     0,     0,   269,     0,    62,  -226,     0,     0,
       0,     0,     0,  -226,     0,     0,   143,     0,   143,     0,
       0,     0,     0,   143,     0,    78,   143,     0,    80,   147,
       0,    82,   143,   147,     0,   143,     0,   153,     0,     5,
       0,   147,     0,     0,     0,     0,   132,   153,     0,     0,
       0,   148,     0,    15,     0,     0,    17,     0,     0,   143,
     153,    21,   153,     0,     0,   148,     0,     0,     0,     0,
     143,   172,   143,     0,     0,     0,   152,     0,     0,     0,
      37,     0,     0,   152,     0,     0,    40,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,   132,   132,   132,
     132,    50,     0,     0,     0,   342,     0,   153,     0,     0,
       0,     0,     0,     0,     0,     0,    60,     0,     0,     0,
       0,     0,   132,   132,     0,     0,     0,     0,    70,     0,
       0,     0,    73,     0,   153,     0,     0,   148,   153,    79,
       0,     0,     0,   152,     0,    83,     0,     0,     0,     0,
       0,   147,     0,     0,     0,     0,   343,   152,     0,   143,
     344,     0,     0,     0,     0,   148,   143,     0,     0,     0,
       0,   148,     0,     0,     0,     0,     0,   143,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,     0,     0,     0,     0,     0,     0,     0,     0,
     143,   153,     0,   147,   143,     0,     0,   346,   347,   147,
     348,   349,   350,     0,   351,   352,   353,     0,   354,   355,
     356,   357,     0,     0,   359,   360,   361,   362,   363,   152,
     365,     0,   148,   366,   367,   368,   148,     0,     0,     0,
       0,     0,     0,     0,   148,     0,     0,     0,     0,   143,
       0,     0,     0,     0,     0,     0,     0,   152,     0,     0,
       0,     0,     0,   152,     0,     0,     0,     0,     0,     0,
     152,     0,     0,   147,     0,   147,     0,     0,     0,   153,
     147,     0,     0,   147,     0,     0,   153,     0,     0,   147,
       0,     0,   147,     0,   104,     0,     0,     0,     0,     0,
     143,   143,   143,   143,   163,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   147,   195,     0,   198,
       0,     0,     0,     0,   152,   143,   143,   147,   152,   147,
       0,     0,     0,     0,     0,     0,   152,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   153,     0,     0,     0,
       0,     0,     0,     0,   148,     0,   220,     0,     0,     0,
     153,     0,     0,     0,   288,     0,     0,     0,     0,     0,
     221,   222,     0,   223,     0,     0,     0,     0,   224,     0,
       0,     0,     0,     0,     0,     0,   225,     0,     0,     0,
       0,   104,   226,     0,     0,   324,     0,   227,     0,     0,
     228,     0,     0,   229,     0,     0,   148,     0,     0,     0,
       0,   230,   148,     0,     0,     0,   147,   231,   232,   342,
       0,     0,     0,   147,   233,     0,     0,     0,     0,     0,
       0,     0,   153,   234,   147,     0,     0,     0,     0,     0,
       0,     0,   235,   236,     0,   237,   152,   238,   147,   239,
       0,     0,   240,     0,     0,     0,   241,   147,   391,   242,
     153,   147,   243,     0,     0,     0,   153,     0,     0,     0,
     343,     0,     0,   153,   344,     0,   148,     0,   148,     0,
       0,     0,     0,   148,     0,     0,   148,     0,     0,     0,
       0,     0,   148,     0,     0,   148,     0,     0,   152,     0,
       0,     0,     0,     0,   152,     0,   147,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,     0,   784,   148,
       0,   346,   347,     0,   348,   349,   350,   153,   351,   352,
     148,   153,   148,   355,     0,     0,   442,     0,     0,   153,
     361,   362,     0,   455,   365,     0,     0,   366,   367,   368,
       0,     0,     0,     0,     0,     0,     0,   147,   147,   147,
     147,     0,     0,     0,     0,     0,     0,     0,   152,     0,
     152,     0,     0,     0,     0,   152,     0,     0,   152,     0,
       0,     0,   147,   147,   152,     0,     0,   152,     0,     0,
       0,   220,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   104,     0,   221,   222,     0,   223,     0,
       0,   152,     0,   224,     0,     0,     0,   104,     0,   148,
       0,   225,   152,     0,   152,     0,   148,   226,     0,     0,
       0,     0,   227,     0,     0,   228,     0,   148,   229,     0,
       0,     0,     0,     0,     0,     0,   230,     0,     0,   153,
       0,   148,   231,   232,     0,     0,     0,     0,     0,   233,
     148,     0,     0,     0,   148,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,   235,   236,     0,
     237,     0,   238,     0,   239,     0,     0,   240,     0,   104,
       0,   241,   445,     0,   242,     0,     0,   243,     0,     0,
       0,   153,     0,     0,     0,     0,     0,   153,     0,   148,
       0,   152,     0,     0,     0,     0,     0,   455,   152,     0,
       0,     0,     0,   455,     0,     0,     0,     0,     0,   152,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   152,     0,     0,     0,     0,     0,     0,
       0,     0,   152,   446,     0,     0,   152,     0,     0,     0,
     148,   148,   148,   148,     0,     0,     0,     0,     0,     0,
       0,   153,     0,   153,     0,     0,     0,     0,   153,     0,
       0,   153,     0,     0,   665,   148,   148,   153,   670,   266,
     153,     0,     0,    22,    23,     0,   104,     0,     0,     0,
       0,   152,   267,     0,    30,   268,     0,     0,     0,    35,
       0,     0,     0,     0,   153,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   153,     0,   153,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,    54,
       0,   323,     0,   933,     0,     0,   934,     0,   269,     0,
      62,     0,   152,   152,   152,   152,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,    80,     0,     0,    82,     0,   152,   152,     5,
     271,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     170,     0,     0,    15,    16,     0,    17,     0,   171,     0,
       0,    21,     0,     0,     0,     0,   754,     0,     0,    28,
       0,   172,     0,     0,   153,    32,   173,   174,     0,   175,
      37,   153,     0,    38,     0,    97,    40,     0,     0,     0,
      42,    43,   153,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,   153,    53,     0,    55,
       0,    57,     0,     0,     0,   153,    60,   176,   789,   153,
       0,     0,     0,     0,   794,    67,    68,    69,    70,    71,
     177,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,     0,  -405,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   153,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,  -405,     0,     0,     0,  -405,
       0,     0,     0,     0,     0,     0,     0,     0,   455,     0,
     455,     0,     0,   272,     0,   455,     0,     0,   455,   178,
       0,   100,  -405,   973,   894,     0,     0,   896,     0,     0,
       0,     0,     0,     0,     0,   153,   153,   153,   153,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   912,     0,     0,     0,     0,     0,     0,     0,     0,
     153,   153,   918,     0,   922,    -2,     4,     0,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     -62,     0,    38,    39,     0,    40,  -240,     0,    41,    42,
      43,     0,    44,    45,    46,   -44,    47,    48,     0,    49,
      50,    51,     0,     0,     0,    52,    53,    54,    55,    56,
      57,    58,  -240,   -44,    59,    60,    61,     0,    62,    63,
      64,     0,    65,    66,    67,    68,    69,    70,    71,    72,
       0,    73,    74,     0,    75,    76,    77,    78,    79,   970,
      80,    81,   -62,    82,    83,     0,     0,    84,     0,    85,
       0,     0,    86,   989,     0,     0,     0,     0,     0,     0,
       0,     0,   994,     0,     0,     0,   996,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,     0,
     100,  1093,   101,     5,   271,     6,     7,     8,     9,    10,
      11,  1024,     0,     0,   170,     0,     0,    15,    16,     0,
      17,     0,   171,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   172,     0,     0,     0,    32,
     173,   174,     0,   175,    37,     0,     0,    38,     0,     0,
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
       0,     0,    93,     0,     0,    94,    95,   272,     0,     0,
       0,    98,     0,   178,     0,   100,     0,   179,  1094,     4,
       0,     5,     0,     6,     7,     8,     9,    10,    11,     0,
    -574,     0,    12,    13,    14,    15,    16,  -574,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,  -574,
      27,    28,  -574,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -62,     0,    38,    39,     0,    40,  -240,
       0,    41,    42,    43,     0,    44,    45,    46,   -44,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,     0,    57,    58,  -240,   -44,    59,    60,    61,
    -574,    62,    63,    64,  -574,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -62,    82,    83,     0,     0,
      84,     0,    85,     0,     0,  -574,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -574,  -574,    89,  -574,  -574,  -574,  -574,  -574,  -574,
    -574,     0,  -574,  -574,  -574,  -574,  -574,     0,  -574,  -574,
    -574,  -574,  -574,  -574,  -574,  -574,    97,  -574,  -574,  -574,
       0,    99,  -574,   100,   308,   101,     5,   271,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -62,     0,
      38,    39,     0,    40,  -240,     0,    41,    42,    43,     0,
      44,    45,    46,   -44,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,    56,    57,    58,
    -240,   -44,    59,    60,    61,     0,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -62,    82,    83,     0,     0,    84,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
     272,    97,     0,     0,    98,     0,    99,   309,   100,     4,
     101,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -62,     0,    38,    39,     0,    40,  -240,
       0,    41,    42,    43,     0,    44,    45,    46,   -44,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,    56,    57,    58,  -240,   -44,    59,    60,    61,
       0,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -62,    82,    83,     0,     0,
      84,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,   483,   100,   502,   101,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -62,     0,
      38,    39,     0,    40,  -240,     0,    41,    42,    43,     0,
      44,    45,    46,   -44,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,    56,    57,    58,
    -240,   -44,    59,    60,    61,     0,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -62,    82,    83,     0,     0,    84,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,   503,   100,   308,
     101,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -62,     0,    38,    39,     0,    40,  -240,
       0,    41,    42,    43,     0,    44,    45,    46,   -44,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,    56,    57,    58,  -240,   -44,    59,    60,    61,
       0,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -62,    82,    83,     0,     0,
      84,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,   309,   100,     4,   101,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -62,     0,
      38,    39,     0,    40,  -240,     0,    41,    42,    43,     0,
      44,    45,    46,   -44,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,    56,    57,    58,
    -240,   -44,    59,    60,    61,     0,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      74,     0,    75,    76,    77,    78,    79,     0,    80,    81,
     -62,    82,    83,     0,     0,    84,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,   677,   100,     4,
     101,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -62,     0,    38,    39,     0,    40,  -240,
       0,    41,    42,    43,     0,    44,    45,    46,   -44,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,   323,    57,    58,  -240,   -44,    59,    60,    61,
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
      32,    33,    34,    35,    36,    37,   -62,     0,    38,    39,
       0,    40,  -240,     0,    41,    42,    43,     0,    44,    45,
      46,   -44,    47,    48,     0,    49,    50,    51,     0,     0,
       0,    52,    53,    54,    55,     0,    57,    58,  -240,   -44,
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
       0,    30,    31,    32,    33,    34,    35,   595,    37,   -62,
       0,    38,    39,     0,    40,  -240,     0,    41,    42,    43,
       0,    44,    45,    46,   -44,    47,    48,     0,    49,    50,
      51,     0,     0,     0,    52,    53,    54,    55,     0,    57,
      58,  -240,   -44,    59,    60,    61,     0,    62,    63,    64,
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
     601,    37,   -62,     0,    38,    39,     0,    40,  -240,     0,
      41,    42,    43,     0,    44,    45,    46,   -44,    47,    48,
       0,    49,    50,    51,     0,     0,     0,    52,    53,    54,
      55,     0,    57,    58,  -240,   -44,    59,    60,    61,     0,
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
      33,    34,    35,   880,    37,   -62,     0,    38,    39,     0,
      40,  -240,     0,    41,    42,    43,     0,    44,    45,    46,
     -44,    47,    48,     0,    49,    50,    51,     0,     0,     0,
      52,    53,    54,    55,     0,    57,    58,  -240,   -44,    59,
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
      30,    31,    32,    33,    34,    35,   882,    37,   -62,     0,
      38,    39,     0,    40,  -240,     0,    41,    42,    43,     0,
      44,    45,    46,   -44,    47,    48,     0,    49,    50,    51,
       0,     0,     0,    52,    53,    54,    55,     0,    57,    58,
    -240,   -44,    59,    60,    61,     0,    62,    63,    64,     0,
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
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,   887,
      37,   -62,     0,    38,    39,     0,    40,  -240,     0,    41,
      42,    43,     0,    44,    45,    46,   -44,    47,    48,     0,
      49,    50,    51,     0,     0,     0,    52,    53,    54,    55,
       0,    57,    58,  -240,   -44,    59,    60,    61,     0,    62,
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
      34,    35,   890,    37,   -62,     0,    38,    39,     0,    40,
    -240,     0,    41,    42,    43,     0,    44,    45,    46,   -44,
      47,    48,     0,    49,    50,    51,     0,     0,     0,    52,
      53,    54,    55,     0,    57,    58,  -240,   -44,    59,    60,
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
      24,    25,    26,     0,   911,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -62,     0,    38,
      39,     0,    40,  -240,     0,    41,    42,    43,     0,    44,
      45,    46,   -44,    47,    48,     0,    49,    50,    51,     0,
       0,     0,    52,    53,    54,    55,     0,    57,    58,  -240,
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
      29,     0,    30,    31,    32,    33,    34,    35,   917,    37,
     -62,     0,    38,    39,     0,    40,  -240,     0,    41,    42,
      43,     0,    44,    45,    46,   -44,    47,    48,     0,    49,
      50,    51,     0,     0,     0,    52,    53,    54,    55,     0,
      57,    58,  -240,   -44,    59,    60,    61,     0,    62,    63,
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
      35,   921,    37,   -62,     0,    38,    39,     0,    40,  -240,
       0,    41,    42,    43,     0,    44,    45,    46,   -44,    47,
      48,     0,    49,    50,    51,     0,     0,     0,    52,    53,
      54,    55,     0,    57,    58,  -240,   -44,    59,    60,    61,
       0,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,     0,    73,    74,     0,    75,    76,    77,
      78,    79,     0,    80,    81,   -62,    82,    83,     0,     0,
      84,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,     0,   100,   772,   101,     5,   271,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   170,     0,     0,
      15,    16,     0,    17,     0,   171,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   172,     0,
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
     272,     0,     0,     0,    98,     0,   178,     0,   100,     0,
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
       0,   178,     0,   100,     0,   179,   669,     5,   271,     6,
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
       0,   179,  1027,     5,     0,     6,     7,     8,     9,    10,
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
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   178,     0,   100,     0,   179,   674,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     170,     0,     0,    15,    16,     0,    17,     0,   171,     0,
       0,    21,   215,    23,     0,     0,     0,     0,     0,    28,
       0,   172,     0,     0,     0,    32,   173,   174,     0,   175,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,    54,    55,
       0,    57,     0,     0,     0,     0,    60,   176,     0,    62,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     177,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,    82,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   178,
       0,   100,     0,   179,     5,   271,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   170,     0,     0,    15,    16,
       0,    17,     0,   171,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   172,     0,     0,     0,
      32,   173,   174,     0,   175,    37,     0,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   176,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   177,     0,    73,     0,     0,
      75,   276,   277,     0,    79,   313,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,   314,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,   272,     0,
       0,     0,    98,     0,   178,     0,   100,     0,   179,     5,
     271,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     170,     0,     0,    15,    16,     0,    17,     0,   171,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   172,     0,     0,     0,    32,   173,   174,     0,   175,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   176,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     177,     0,    73,     0,     0,    75,   276,   277,     0,    79,
     313,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,   272,     0,     0,     0,    98,     0,   178,
       0,   100,   661,   179,     5,   271,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   170,     0,     0,    15,    16,
       0,    17,     0,   171,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   172,     0,     0,     0,
      32,   173,   174,     0,   175,    37,     0,     0,    38,     0,
       0,    40,     0,     0,     0,    42,    43,     0,     0,    45,
      46,     0,    47,    48,     0,    49,    50,     0,     0,     0,
       0,     0,    53,     0,    55,     0,    57,     0,     0,     0,
       0,    60,   176,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,   177,     0,    73,     0,     0,
      75,   276,   277,     0,    79,   313,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,   272,     0,
       0,     0,    98,     0,   178,     0,   100,   663,   179,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     170,     0,     0,    15,    16,     0,    17,     0,   171,     0,
       0,    21,     0,   560,     0,     0,     0,     0,     0,    28,
       0,   172,     0,     0,     0,    32,   173,   174,     0,   175,
      37,     0,   561,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   176,     0,   562,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     177,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,   563,    83,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   178,
       0,   100,     5,   179,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   170,     0,     0,    15,    16,     0,    17,
       0,   171,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   172,     0,     0,     0,    32,   173,
     174,     0,   175,    37,     0,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     176,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   177,     0,    73,     0,     0,    75,   276,
     277,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,   278,     0,     0,
      98,     0,   178,     0,   100,     0,   179,     5,   271,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   170,     0,
       0,    15,    16,     0,    17,     0,   171,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   172,
       0,     0,     0,    32,   173,   174,     0,   175,    37,     0,
       0,    38,     0,     0,    40,     0,     0,     0,    42,    43,
       0,     0,    45,    46,     0,    47,    48,     0,    49,    50,
       0,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,     0,     0,     0,    60,   176,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,   177,     0,
      73,     0,     0,    75,   276,   277,     0,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,   272,     0,     0,     0,    98,     0,   178,     0,   100,
       5,   179,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   170,     0,     0,    15,    16,     0,    17,     0,   171,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   172,     0,     0,     0,    32,   173,   174,     0,
     175,    37,     0,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
     774,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   176,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   177,     0,    73,     0,     0,    75,   276,   277,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   809,     0,   810,
      87,    88,    89,    90,    91,     0,     0,     0,   811,    92,
       0,     0,     0,   812,   222,   813,   814,     0,     0,    93,
       0,   815,    94,    95,    96,     0,     0,     0,    98,   225,
     178,   172,   100,     0,   179,   816,     0,     0,     0,     0,
     817,     0,     0,   228,     0,     0,   818,     0,   819,     0,
       0,     0,     0,     0,   820,     0,     0,     0,     0,     0,
     821,   822,     0,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,     0,   823,     0,     0,     0,
       0,     0,     0,     0,     0,   235,   236,     0,   824,     0,
     238,     0,   825,     0,     0,   826,     0,     0,     0,   827,
       0,     0,   242,     0,     0,   828,     0,     0,     0,     0,
       0,     0,     0,     0,   369,   370,   371,   372,   373,   374,
       0,     0,   377,   378,   379,   380,     0,   382,   383,   829,
     830,   831,   832,   833,     0,     0,   834,     0,     0,     0,
     835,   836,   837,   838,   839,   840,   841,   842,   843,   844,
     845,     0,   846,     0,     0,   847,   848,   849,   850,     0,
       5,   851,     6,     7,     8,   204,    10,    11,   205,     0,
       0,   170,     0,     0,    15,    16,     0,    17,     0,   171,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   172,     0,     0,     0,    32,   173,   174,     0,
     175,    37,     0,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   206,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   177,     0,    73,     0,     0,    75,     0,     0,   207,
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
     173,   174,     0,   175,    37,     0,     0,    38,     0,     0,
      40,     0,     0,     0,    42,    43,     0,     0,    45,    46,
       0,    47,    48,     0,    49,    50,     0,     0,     0,     0,
       0,    53,   209,    55,     0,    57,     0,     0,     0,     0,
      60,   176,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,   177,     0,    73,     0,     0,    75,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,   210,     0,    86,     0,     0,
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
       0,    73,     0,     0,    75,     0,     0,     0,    79,     0,
       0,    81,     0,     0,    83,     0,     0,     0,     0,     0,
     210,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,   272,     0,     0,     0,    98,     0,   178,     0,
     100,     5,   179,     6,     7,     8,     9,    10,    11,     0,
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
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   178,     0,   100,     0,   179,     5,   271,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   170,     0,     0,
      15,    16,     0,    17,     0,   171,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   172,     0,
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
     272,     0,     0,     0,    98,     0,   178,   659,   100,     5,
     179,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     170,     0,     0,    15,    16,     0,    17,     0,   171,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   172,     0,     0,     0,    32,   173,   174,     0,   175,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,     0,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   176,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
     177,     0,    73,     0,     0,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,   210,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   178,
       0,   100,     5,   179,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   170,     0,     0,    15,    16,     0,    17,
       0,   171,     0,     0,    21,     0,     0,     0,     0,     0,
       0,   251,    28,     0,   172,     0,     0,     0,    32,   173,
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
      98,     0,    99,     0,   100,     0,   179,     5,   271,     6,
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
       5,   179,     6,     7,     8,   204,    10,    11,     0,     0,
       0,   170,     0,     0,    15,    16,     0,    17,     0,   171,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   172,     0,     0,     0,    32,   173,   174,     0,
     175,    37,     0,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   206,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   177,     0,    73,     0,     0,    75,     0,     0,   207,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     178,     0,   100,     0,   179,     5,   271,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   170,     0,     0,    15,
      16,     0,    17,     0,   171,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,   633,     0,   172,     0,     0,
       0,    32,   173,   174,     0,   175,    37,     0,     0,   634,
       0,     0,    40,     0,     0,     0,    42,    43,     0,     0,
      45,    46,     0,    47,    48,     0,    49,    50,     0,     0,
       0,     0,     0,    53,     0,    55,     0,    57,     0,     0,
       0,     0,    60,   176,     0,     0,     0,     0,     0,     0,
       0,    67,   635,    69,    70,    71,   636,     0,    73,     0,
       0,    75,     0,     0,     0,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,   272,
       0,     0,     0,    98,     0,   178,     0,   100,     0,   973,
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
     178,     0,   100,     0,   973,     5,   271,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   170,     0,     0,    15,
      16,     0,    17,     0,   171,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   172,     0,     0,
       0,    32,   173,   174,     0,   957,    37,     0,     0,    38,
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
      94,    95,    96,     0,     0,     0,    98,     0,   178,     0,
     100,     5,   179,     6,     7,     8,   200,    10,    11,     0,
       0,     0,   170,     0,     0,    15,    16,     0,    17,     0,
     171,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   172,     0,     0,     0,    32,   173,   174,
       0,   175,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   201,
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
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,    99,     0,   100,     5,   179,     6,
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
      95,    96,     0,     0,     0,   606,     0,   178,     0,   100,
       5,   179,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   170,     0,     0,    15,    16,     0,    17,     0,   171,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     633,     0,   172,     0,     0,     0,    32,   173,   174,     0,
     175,    37,     0,     0,   634,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   176,     0,
       0,     0,     0,     0,     0,     0,    67,   635,    69,    70,
      71,   636,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     178,     0,   100,     5,   637,     6,     7,     8,     9,    10,
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
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   178,     0,   100,     5,   637,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   170,     0,     0,
      15,    16,     0,    17,     0,   171,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   172,     0,
       0,     0,    32,   173,   174,     0,   713,    37,     0,     0,
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
       0,   172,     0,     0,     0,    32,   173,   174,     0,   716,
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
     174,     0,   951,    37,     0,     0,    38,     0,     0,    40,
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
       0,    32,   173,   174,     0,   952,    37,     0,     0,    38,
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
     172,     0,     0,     0,    32,   173,   174,     0,   954,    37,
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
       0,   955,    37,     0,     0,    38,     0,     0,    40,     0,
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
       0,     0,     0,     0,    28,     0,   172,     0,     0,     0,
      32,   173,   174,     0,   956,    37,     0,     0,    38,     0,
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
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   178,     0,   100,     5,   179,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   170,     0,
       0,    15,    16,     0,    17,     0,   171,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   172,
       0,     0,     0,    32,   173,   174,     0,   957,    37,     0,
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
       5,   179,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   170,     0,     0,    15,    16,     0,    17,     0,   171,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     633,     0,   172,     0,     0,     0,    32,   173,   174,     0,
     175,    37,     0,     0,   634,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   176,     0,
       0,     0,     0,     0,     0,     0,    67,   635,    69,    70,
      71,   636,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     178,     0,   100,     5,  1049,     6,     7,     8,     9,    10,
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
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,     0,     0,   100,     5,  1049,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   170,     0,     0,
      15,    16,     0,    17,     0,   171,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   172,     0,
       0,     0,    32,   173,   174,     0,   175,    37,     0,     0,
      38,     0,     0,    40,     0,     0,     0,    42,    43,     0,
       0,    45,    46,     0,    47,    48,     0,    49,    50,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
       0,     0,     0,    60,   176,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,   177,     0,    73,
       0,     0,    75,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,     0,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,  -508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,   384,    95,
      96,  -546,     0,  -546,    98,     0,   178,     0,   100,     5,
     179,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     170,     0,     0,    15,    16,     0,    17,     0,   171,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   172,     0,     0,     0,    32,   173,   174,     0,   175,
      37,     0,     0,    38,     0,     0,    40,     0,     0,     0,
      42,    43,  -387,     0,    45,    46,     0,    47,    48,     0,
      49,    50,     0,     0,     0,     0,     0,    53,     0,    55,
       0,    57,     0,     0,     0,     0,    60,   176,     0,     0,
       0,  -387,     0,     0,     0,    67,    68,    69,    70,    71,
     177,     0,    73,     0,  -387,    75,     0,     0,     0,    79,
       0,     0,    81,     0,     0,    83,     0,  -387,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,    89,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   170,     0,     0,    15,    16,     0,    17,     0,   171,
       0,     0,    21,    96,  -387,     0,     0,     0,     0,  -387,
      28,   100,   172,  1049,     0,     0,    32,   173,   174,     0,
     175,    37,     0,     0,    38,     0,     0,    40,     0,     0,
       0,    42,    43,     0,     0,    45,    46,     0,    47,    48,
       0,    49,    50,     0,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,     0,     0,     0,    60,   176,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,   177,     0,    73,     0,     0,    75,     0,     0,     0,
      79,     0,     0,    81,     0,     0,    83,     0,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,     0,     0,     0,     0,     0,     0,     0,
      87,     5,    89,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   170,     0,     0,    15,    16,     0,    17,     0,
     171,     0,   384,    21,    96,  -546,     0,  -546,     0,     0,
     178,    28,   100,   172,   179,     0,     0,    32,   173,   174,
       0,   175,    37,     0,     0,    38,     0,     0,    40,     0,
       0,     0,    42,    43,     0,     0,    45,    46,     0,    47,
      48,     0,    49,    50,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,     0,     0,     0,    60,   176,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,   177,     0,    73,     0,     0,    75,     0,     0,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,    89,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   170,     0,     0,    15,    16,     0,    17,
       0,   171,     0,     0,    21,    96,     0,     0,     0,     0,
       0,   178,    28,   100,   172,   637,     0,     0,    32,   173,
     174,     0,   175,    37,     0,     0,    38,     0,     0,    40,
       0,     0,     0,    42,    43,     0,     0,    45,    46,     0,
      47,    48,     0,    49,    50,     0,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,     0,     0,     0,    60,
     176,     0,   220,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,   177,     0,    73,   221,   222,    75,   223,
       0,     0,    79,     0,   224,    81,     0,     0,    83,     0,
       0,     0,   225,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,   227,     0,     0,   228,     0,     0,   229,
       0,     0,     0,     0,    89,     0,     0,   230,     0,     0,
     220,     0,     0,   231,   232,     0,     0,     0,     0,     0,
     233,     0,     0,     0,   221,   222,     0,   223,     0,   234,
       0,     0,   224,     0,   100,     0,   179,     0,   235,   236,
     225,   237,     0,   238,     0,   239,   226,     0,   240,     0,
       0,   227,   241,     0,   228,   242,     0,   229,   243,     0,
       0,     0,     0,     0,     0,   230,     0,     0,  1006,     0,
       0,   231,   232,     0,     0,     0,     0,   811,   233,     0,
       0,     0,   221,   222,   813,   223,     0,   234,     0,     0,
     224,     0,     0,     0,     0,     0,   235,   236,   225,   237,
       0,   238,     0,   239,   816,     0,   240,     0,     0,   227,
     241,     0,   228,   242,   247,   229,   243,   819,     0,     0,
       0,     0,     0,   230,     0,     0,     0,     0,     0,   821,
     232,     0,     0,   340,     0,     0,   233,     0,     0,     0,
     341,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,   342,     0,   235,   236,     0,   237,     0,   238,
       0,  1007,     0,     0,   826,     0,     0,     0,   241,     0,
       0,   242,   762,     0,   243,     0,     0,     0,     0,     0,
       0,     0,     0,   369,   370,   371,   372,   373,   374,     0,
       0,   377,   378,   379,   380,     0,   382,   383,   829,   830,
     831,   832,   833,   343,     0,   834,     0,   344,     0,   835,
     836,   837,   838,   839,   840,   841,   842,   843,   844,   845,
       0,   846,   340,     0,   847,   848,   849,   850,     0,   341,
       0,     0,     0,     0,   428,     0,     0,     0,   345,     0,
       0,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,   340,
     366,   367,   368,     0,     0,     0,   341,     0,     0,   792,
       0,     0,   343,     0,     0,     0,   344,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   340,
       0,     0,     0,     0,     0,     0,   341,   345,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   342,     0,
       0,     0,     0,   346,   347,     0,   348,   349,   350,   343,
     351,   352,   353,   344,   354,   355,   356,   357,   358,     0,
     359,   360,   361,   362,   363,   364,   365,     0,   340,   366,
     367,   368,     0,     0,     0,   341,     0,     0,   871,     0,
       0,     0,     0,     0,   345,     0,     0,   342,     0,   343,
       0,     0,     0,   344,     0,     0,     0,     0,     0,     0,
     346,   347,     0,   348,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,   357,   358,     0,   359,   360,   361,
     362,   363,   364,   365,   345,     0,   366,   367,   368,     0,
       0,     0,     0,     0,     0,  1041,     0,     0,   343,     0,
     346,   347,   344,   348,   349,   350,     0,   351,   352,   353,
       0,   354,   355,   356,   357,   358,     0,   359,   360,   361,
     362,   363,   364,   365,   340,     0,   366,   367,   368,     0,
       0,   341,     0,   345,     0,  1116,     0,     0,     0,     0,
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
       0,   366,   367,   368,     0,     0,   342,     0,   776,     0,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,   343,     0,     0,     0,   344,     0,     0,     0,     0,
       0,     0,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,   357,   358,     0,   359,
     360,   361,   362,   363,   364,   365,   345,   343,   366,   367,
     368,   344,     0,   340,     0,   777,     0,     0,     0,     0,
     341,     0,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,   342,   354,   355,   356,   357,   358,     0,   359,
     360,   361,   362,   363,   364,   365,     0,     0,   366,   367,
     368,     0,     0,     0,     0,  1005,     0,     0,   346,   347,
       0,   348,   349,   350,     0,   351,   352,   353,     0,     0,
     355,   356,   357,     0,     0,   359,   360,   361,   362,     0,
       0,   365,     0,   343,   366,   367,   368,   344,     0,     0,
       0,     0,     0,   340,     0,     0,   438,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,   879,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,   886,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,  1020,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,  1021,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,  1022,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,  1023,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,   340,     0,     0,  1045,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   345,   369,
     370,   371,   372,   373,   374,     0,     0,   377,   378,   379,
     380,     0,   382,   383,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,     0,     0,
     366,   367,   368,   343,   397,     0,     0,   344,     0,     0,
       0,     0,     0,     0,     0,     0,  1046,   340,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,   342,   746,
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
     342,   738,     0,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,   340,     0,     0,     0,     0,     0,     0,
     341,     0,     0,     0,     0,     0,   346,   347,     0,   348,
     349,   350,   342,   351,   352,   353,     0,   354,   355,   356,
     357,   358,     0,   359,   360,   361,   362,   363,   364,   365,
       0,   343,   366,   367,   368,   344,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   340,     0,     0,     0,
       0,     0,     0,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,     0,   342,   345,   344,     0,     0,
       0,     0,     0,     0,     0,     0,   755,     0,     0,     0,
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
     365,     0,     0,   366,   367,   368,     0,   468,   340,   683,
     436,     0,     0,     0,     0,   341,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,     0,   342,     0,   344,
       0,     0,   684,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,   685,     0,     0,     0,     0,     0,     0,
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
     350,   343,   351,   352,   353,   344,   354,   355,   356,   357,
     358,     0,   359,   360,   361,   362,   363,   364,   365,     0,
     680,   366,   367,   368,     0,     0,   340,     0,     0,     0,
       0,     0,     0,   341,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,   342,     0,     0,     0,     0,
     928,     0,   346,   347,     0,   348,   349,   350,     0,   351,
     352,   353,     0,   354,   355,   356,   357,   358,     0,   359,
     360,   361,   362,   363,   364,   365,     0,     0,   366,   367,
     368,   929,     0,   340,     0,     0,     0,     0,     0,     0,
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
       0,   354,   355,   356,   357,   358,  -509,   359,   360,   361,
     362,   363,   364,   365,     0,   343,   366,   367,   368,   344,
       0,     0,     0,     0,     0,     0,   340,   750,     0,     0,
       0,     0,     0,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   342,   165,   343,     0,     0,
     345,   344,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,   347,     0,   348,
     349,   350,     0,   351,   352,   353,     0,   354,   355,   356,
     357,   358,   345,   359,   360,   361,   362,   363,   364,   365,
       0,     0,   366,   367,   368,     0,   343,     0,   346,   347,
     344,   348,   349,   350,     0,   351,   352,   353,     0,   354,
     355,   356,   357,   358,     0,   359,   360,   361,   362,   363,
     364,   365,   340,   723,   366,   367,   368,     0,     0,   341,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   342,     0,     0,   753,     0,     0,   346,   347,     0,
     348,   349,   350,     0,   351,   352,   353,     0,   354,   355,
     356,   357,   358,     0,   359,   360,   361,   362,   363,   364,
     365,   340,     0,   366,   367,   368,     0,     0,   341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     342,     0,   343,     0,     0,     0,   344,     0,     0,     0,
       0,     0,     0,   340,     0,     0,     0,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,   866,     0,     0,     0,   345,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,   346,   347,   344,   348,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,   357,   358,     0,
     359,   360,   361,   362,   363,   364,   365,     0,     0,   366,
     367,   368,     0,   343,     0,   795,   345,   344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   346,   347,     0,   348,   349,   350,   342,   351,
     352,   353,     0,   354,   355,   356,   357,   358,   345,   359,
     360,   361,   362,   363,   364,   365,     0,     0,   366,   367,
     368,     0,     0,     0,   346,   347,     0,   348,   349,   350,
       0,   351,   352,   353,     0,   354,   355,   356,   357,   358,
       0,   359,   360,   361,   362,   363,   364,   365,   340,   343,
     366,   367,   368,   344,     0,   341,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   342,   869,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     340,   927,     0,     0,     0,     0,     0,   341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   342,
     346,   347,     0,   348,   349,   350,     0,   351,   352,   353,
       0,     0,   355,     0,     0,     0,     0,     0,   343,   361,
     362,     0,   344,   365,     0,     0,   366,   367,   368,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,   345,   344,     0,   340,     0,     0,     0,
       0,     0,     0,   341,     0,     0,     0,     0,     0,   346,
     347,     0,   348,   349,   350,   342,   351,   352,   353,     0,
     354,   355,   356,   357,   358,   345,   359,   360,   361,   362,
     363,   364,   365,     0,     0,   366,   367,   368,     0,     0,
       0,   346,   347,     0,   348,   349,   350,     0,   351,   352,
     353,     0,   354,   355,   356,   357,   358,     0,   359,   360,
     361,   362,   363,   364,   365,     0,   343,   366,   367,   368,
     344,     0,     0,     0,     0,     0,   340,     0,     0,   950,
       0,     0,     0,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   342,     0,     0,     0,     0,
       0,   345,     0,     0,     0,     0,     0,     0,   340,     0,
       0,     0,     0,     0,     0,   341,     0,   346,   347,     0,
     348,   349,   350,     0,   351,   352,   353,   342,   354,   355,
     356,   357,   358,     0,   359,   360,   361,   362,   363,   364,
     365,     0,     0,   366,   367,   368,   343,     0,     0,     0,
     344,     0,     0,     0,     0,     0,     0,     0,     0,   953,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,     0,
       0,   345,   344,     0,     0,     0,     0,     0,   340,     0,
       0,  1058,     0,     0,     0,   341,     0,   346,   347,     0,
     348,   349,   350,     0,   351,   352,   353,   342,   354,   355,
     356,   357,   358,   345,   359,   360,   361,   362,   363,   364,
     365,     0,     0,   366,   367,   368,     0,     0,     0,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,     0,
     354,   355,   356,   357,   358,     0,   359,   360,   361,   362,
     363,   364,   365,     0,     0,   366,   367,   368,   343,     0,
       0,     0,   344,     0,     0,     0,     0,     0,   340,     0,
       0,  1059,     0,     0,     0,   341,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,   345,     0,     0,     0,     0,     0,     0,
     340,     0,     0,     0,     0,     0,     0,   341,     0,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,   342,
     354,   355,   356,   357,   358,     0,   359,   360,   361,   362,
     363,   364,   365,     0,     0,   366,   367,   368,   343,     0,
       0,     0,   344,     0,     0,     0,     0,     0,     0,     0,
       0,  1060,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,   345,   344,     0,     0,     0,     0,     0,
     340,     0,     0,  1061,     0,     0,     0,   341,     0,   346,
     347,     0,   348,   349,   350,     0,   351,   352,   353,   342,
     354,   355,   356,   357,   358,   345,   359,   360,   361,   362,
     363,   364,   365,     0,     0,   366,   367,   368,     0,     0,
       0,   346,   347,     0,   348,   349,   350,     0,   351,   352,
     353,     0,   354,   355,   356,   357,   358,     0,   359,   360,
     361,   362,   363,   364,   365,     0,     0,   366,   367,   368,
     343,     0,     0,     0,   344,     0,     0,     0,     0,     0,
     340,     0,     0,  1062,     0,     0,     0,   341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   340,     0,     0,     0,     0,     0,     0,   341,
       0,   346,   347,     0,   348,   349,   350,     0,   351,   352,
     353,   342,   354,   355,   356,   357,   358,     0,   359,   360,
     361,   362,   363,   364,   365,     0,     0,   366,   367,   368,
     343,     0,     0,     0,   344,     0,  1087,     0,     0,     0,
       0,   340,     0,  1063,     0,     0,     0,     0,   341,     0,
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
       0,     0,     0,     0,     0,     0,     0,   711,     0,     0,
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
       0,    52,     0,    54,     0,    56,     0,   933,     0,     0,
     934,   342,   269,     0,    62,    52,     0,    54,     0,    56,
       0,   933,     0,     0,   934,   266,   269,     0,    62,    22,
      23,     0,     0,    78,     0,     0,    80,     0,   267,    82,
      30,   268,     0,   342,     0,    35,     0,    78,     0,     0,
      80,    39,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,     0,   344,     0,     0,     0,
       0,     0,     0,    52,     0,    54,     0,    56,     0,   933,
       0,     0,   934,     0,   269,     0,    62,     0,     0,    97,
       0,     0,     0,     0,   343,  1019,     0,   345,   344,     0,
       0,     0,     0,    97,     0,    78,     0,     0,    80,  1064,
       0,    82,     0,   346,   347,     0,   348,   349,   350,     0,
     351,   352,   353,     0,   354,   355,   356,   357,     0,   345,
     359,   360,   361,   362,   363,   364,   365,     0,     0,   366,
     367,   368,     0,     0,     0,   346,   347,     0,   348,   349,
     350,     0,   351,   352,   353,     0,   354,   355,   356,   357,
       0,    97,   359,   360,   361,   362,   363,  1065,   365,     0,
       0,   366,   367,   368,   688,     0,   220,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,   222,     0,   223,     0,     0,     0,     0,   224,     0,
       0,     0,     0,     0,     0,     0,   225,     0,     0,     0,
       0,     0,   226,  -221,     0,     0,     0,   227,     0,     0,
     228,     0,     0,   229,     0,     0,     0,  -221,  -221,     0,
    -221,   230,     0,     0,     0,  -221,     0,   231,   232,     0,
       0,     0,     0,  -221,   233,     0,     0,     0,     0,  -221,
       0,     0,     0,   234,  -221,     0,     0,  -221,     0,     0,
    -221,     0,   235,   236,     0,   237,     0,   238,  -221,   239,
       0,  -231,   240,     0,  -221,  -221,   241,     0,     0,   242,
       0,  -221,   243,     0,     0,  -231,  -231,     0,  -231,     0,
    -221,     0,     0,  -231,     0,     0,     0,     0,     0,  -221,
    -221,  -231,  -221,     0,  -221,     0,  -221,  -231,   220,  -221,
       0,     0,  -231,  -221,     0,  -231,  -221,     0,  -231,  -221,
       0,     0,   221,   222,     0,   223,  -231,     0,     0,     0,
     224,     0,  -231,  -231,     0,     0,     0,     0,   225,  -231,
       0,     0,     0,     0,   226,     0,     0,     0,  -231,   227,
       0,     0,   228,     0,     0,   229,     0,  -231,  -231,     0,
    -231,     0,  -231,   230,  -231,     0,  -222,  -231,     0,   231,
     232,  -231,     0,     0,  -231,     0,   233,  -231,     0,     0,
    -222,  -222,     0,  -222,     0,   234,     0,     0,  -222,     0,
       0,     0,     0,     0,   235,   236,  -222,   237,     0,   238,
       0,   239,  -222,  -153,   240,     0,     0,  -222,   241,     0,
    -222,   242,     0,  -222,   243,     0,     0,  -153,  -153,     0,
    -153,  -222,     0,     0,     0,  -153,     0,  -222,  -222,     0,
       0,     0,     0,  -153,  -222,     0,     0,     0,     0,  -153,
       0,     0,     0,  -222,  -153,     0,     0,  -153,     0,     0,
    -153,     0,  -222,  -222,     0,  -222,     0,  -222,  -153,  -222,
       0,  -149,  -222,     0,  -153,  -153,  -222,     0,     0,  -222,
       0,  -153,  -222,     0,     0,  -149,  -149,     0,  -149,     0,
    -153,     0,     0,  -149,     0,     0,     0,     0,     0,  -153,
    -153,  -149,  -153,     0,  -153,     0,  -153,  -149,     0,  -153,
       0,     0,  -149,  -153,     0,  -149,  -153,     0,  -149,  -153,
       0,     0,     0,     0,     0,     0,  -149,     0,     0,     0,
     266,     0,  -149,  -149,    22,    23,     0,     0,     0,  -149,
       0,     0,     0,   267,     0,    30,   268,     0,  -149,     0,
      35,     0,     0,   -63,     0,     0,    39,  -149,  -149,  -240,
    -149,     0,  -149,     0,  -149,     0,     0,  -149,     0,     0,
       0,  -149,     0,     0,  -149,     0,     0,  -149,    52,     0,
      54,     0,     0,     0,     0,  -240,     0,     0,     0,   269,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      78,     0,     0,    80,     0,   -63,    82
};

static const yytype_int16 yycheck[] =
{
      12,    64,    59,   150,    16,     2,   329,   390,    20,   477,
     191,   199,    19,   214,    26,   136,   685,   697,    30,    31,
     599,    33,    34,    35,    36,   684,   207,     1,    22,   219,
     983,   219,    44,    46,    46,     1,   901,   100,   771,    51,
      32,    53,   446,    55,    63,   802,   632,    32,    46,     1,
      27,    63,    64,    65,    66,    67,    68,    32,   158,   113,
      72,    32,    74,    32,    76,    77,   898,   856,   857,    81,
      85,   192,    84,    85,   264,    87,   264,    89,  1077,    46,
      92,    93,   101,    95,    66,    68,     1,    99,   100,   101,
     113,    45,     3,   587,    68,    72,   590,   287,     3,   287,
       1,    56,   111,   133,    56,    46,    83,  1027,     3,    51,
       0,   141,    46,   436,   168,   113,    83,  1116,   319,   320,
      46,   113,    46,     3,   710,   102,   139,   110,   861,   708,
     139,    46,    46,    85,   146,    77,   110,  1002,   113,    85,
     972,   139,    83,  1096,    83,   168,    98,   294,   163,   139,
      33,    34,   118,   107,   139,    56,   111,  1077,   170,   111,
     179,   173,   174,   175,   352,   177,   178,   179,   118,   902,
     168,   163,   138,   139,    85,    18,   166,   443,   163,   936,
     113,   164,   768,   190,    85,    96,   138,   199,   163,   165,
     164,    96,   163,   143,   163,    27,   386,   387,   386,   387,
     166,    96,   959,   118,   961,   139,   158,   219,   265,   203,
     111,   163,   158,   139,   208,   139,   599,   163,   101,  1008,
    1009,   215,   139,   138,   139,   139,     8,   631,    71,   211,
     212,   213,   214,   727,   168,    45,   730,   138,   971,   133,
      72,    82,   168,   158,   168,    86,    89,   141,   139,   166,
      60,    83,   264,    27,   168,  1094,   268,   118,   451,  1016,
     929,  1018,   163,   106,   276,   277,   946,   118,    78,   928,
     102,   139,   254,  1112,   875,   287,   877,   168,   260,   139,
     470,   139,   470,   144,   477,   146,   147,   165,   139,   150,
     151,   139,   143,  1026,   165,   416,   164,   107,    72,   139,
     421,   283,   314,   133,   139,   884,   166,    93,   166,    83,
     158,   141,   375,   376,   311,   166,   328,   329,   381,   301,
     165,   384,   385,   158,   164,   708,   338,   163,   340,   341,
     342,   343,   344,   345,   108,   347,   348,   349,   350,   351,
     100,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   958,   762,   960,
     118,   139,   685,   375,   376,    78,   139,    56,   139,   381,
     139,   139,   384,   385,   386,   387,   388,   389,   390,   203,
     139,   139,   139,   139,   208,   143,   139,   399,   166,   158,
      76,    77,   404,   166,   406,   166,    85,   585,   166,   158,
       3,   158,   603,   604,   423,   158,   158,    56,   166,    98,
     166,   423,   164,   425,   426,    56,   139,    20,   400,   431,
     432,   158,   111,   435,   436,   488,   438,   490,   440,    82,
      33,    44,   158,    86,   158,   158,    85,    31,   164,   199,
     908,   453,   133,   157,    85,   457,   458,   138,   430,    98,
     141,   463,   434,    66,   145,    58,   139,    98,   470,   219,
     343,   344,   111,    76,    77,   158,   667,   668,   149,   158,
     111,   158,   112,   485,   163,   158,   488,   658,   490,   158,
     158,    27,   494,   495,   496,   158,   498,   499,    82,   158,
     509,   679,    86,    96,   760,   165,   165,   509,   167,   102,
      46,   884,   158,    49,   264,   388,   389,   504,   163,   158,
      27,   165,    81,   167,   163,   158,   399,   158,    81,   165,
     583,   167,   163,    69,   158,   158,    72,   287,    74,   165,
     165,    48,   167,   165,   165,    31,   802,    83,   165,   133,
     167,   165,     2,   426,   138,   165,   118,   141,    41,   432,
     697,   145,    12,    70,     8,    72,   102,   569,   570,   571,
     572,   573,   574,   575,   138,    25,    83,    27,    60,   576,
     577,   583,   165,   585,   167,   163,   189,   141,   641,   158,
     488,   593,   490,   595,     2,    78,    82,   599,   118,   601,
      86,   108,   139,   139,    12,   166,   158,    46,   211,   212,
     213,   214,    34,   139,   165,   218,   929,    25,   166,    27,
     166,   494,    72,   118,   166,   597,   598,   166,   637,    21,
     166,   603,   604,   635,   636,   637,   386,   387,   139,   641,
     168,   168,   163,   165,   158,    31,   129,   133,   131,    99,
     158,   254,   138,   103,   140,   141,   158,   260,   158,   145,
     158,   166,   168,   139,    72,   158,   158,   153,    76,    77,
     158,    34,   166,   159,   160,   161,   166,   164,   139,   681,
     283,   683,   684,   685,   164,   941,   118,    83,   166,    32,
      32,    99,   163,   168,   163,   103,    82,   570,   301,   572,
      86,    34,   138,   959,    83,   961,   708,   165,   191,   711,
     712,   713,   168,   463,   716,   138,   166,   165,   901,   168,
     470,   168,   164,   158,   207,   908,   209,   158,   158,   909,
     910,   909,   910,    46,   736,   485,   738,   165,   488,    27,
     490,   743,   141,    56,   746,   495,   141,   133,   750,   721,
     722,   753,   138,    46,   140,   141,   141,   133,   166,   145,
    1016,   733,  1018,    56,   247,   133,   141,   153,   141,   141,
     141,   780,    85,   745,   141,   161,   748,   141,   780,   133,
     141,   141,   133,   133,    72,   141,   163,    34,   790,   272,
     792,    98,    85,   795,   244,    83,   166,   400,   111,   133,
     166,   251,   804,   133,   163,   118,   158,   139,   166,   946,
     158,   166,   157,   157,   102,    27,   325,    24,   111,  1002,
     108,   168,   295,   946,   784,   118,   139,   430,   805,   941,
     760,   434,   624,   583,    46,   585,   244,    49,   711,   712,
     808,  1096,  1026,   251,  1027,   158,   139,  1054,  1087,   851,
     163,  1080,    35,   166,   454,   585,    30,    69,  1041,    -1,
      72,   311,    74,   865,   866,   158,   868,   869,   870,   871,
     163,    83,    -1,   166,    -1,   325,    -1,   879,   880,    -1,
     882,    -1,   884,    -1,   886,   887,    -1,   934,   890,     2,
     102,    -1,    -1,    -1,  1077,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,   311,    -1,   907,   908,   909,   910,    -1,
      -1,  1094,    25,   915,    27,   917,    -1,   325,    -1,   921,
      -1,    -1,    -1,    -1,    -1,   927,   928,   929,    -1,  1112,
      -1,   414,    -1,  1116,    -1,   418,    -1,    -1,    -1,    -1,
      -1,   913,    -1,    -1,    -1,    -1,    -1,   397,   950,   951,
     952,   953,   954,   955,   956,   957,    -1,    -1,    -1,    72,
      -1,    -1,    -1,   446,   973,    -1,    -1,    -1,    -1,    46,
      -1,   973,    -1,    -1,    -1,   425,   978,    -1,    -1,    56,
     982,   431,    -1,    -1,    -1,    -1,    99,    -1,   438,   397,
     103,    -1,    -1,    -1,   597,   598,    -1,    -1,    -1,    -1,
     603,   604,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,   615,    -1,    -1,    -1,    -1,   425,  1020,  1021,
    1022,  1023,   505,   431,    -1,    -1,    -1,    -1,   511,    -1,
     438,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,  1041,
    1049,   118,   492,  1045,  1046,    -1,   496,  1049,    27,    -1,
      -1,   654,    -1,   166,   504,    -1,  1058,  1059,  1060,  1061,
    1062,  1063,   139,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,  1076,  1077,    -1,  1079,    -1,    -1,
      -1,   158,    -1,    -1,   492,    -1,   163,    -1,   496,   166,
      69,    -1,     2,    72,    -1,    74,   504,    -1,    -1,    -1,
      -1,    31,    12,   586,    83,    -1,   589,    -1,    -1,    -1,
    1112,    -1,    -1,    -1,  1116,    25,   566,    27,   721,   722,
      -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,  1032,
     733,   244,   735,    -1,    -1,    -1,    -1,    -1,   251,   742,
      -1,    -1,   745,    -1,   627,   748,    -1,    -1,   631,    -1,
      -1,   901,    82,    -1,    -1,    -1,    86,    -1,    -1,   909,
     910,    -1,    72,    -1,   614,    -1,    -1,    -1,    -1,    -1,
      -1,   654,    -1,    -1,    -1,   658,    -1,    -1,  1081,  1082,
    1083,  1084,  1085,  1086,   787,    -1,    -1,    -1,    -1,    99,
      -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,   311,    -1,
      -1,    -1,    -1,   133,   134,    -1,   614,   137,   138,    -1,
     140,   141,   325,    -1,   697,   145,   666,    -1,    -1,    -1,
      -1,    -1,   672,   153,    -1,    -1,    -1,    -1,   978,   159,
     160,   161,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,   697,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,   166,    -1,   666,    48,
      -1,   711,   712,    -1,   672,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   759,  1027,    -1,   762,
      -1,    70,    -1,    72,   397,    74,   736,    76,   738,    -1,
      79,    -1,    81,   743,    83,    -1,   746,    -1,    -1,    -1,
      -1,   784,   752,    -1,    -1,   755,    -1,    -1,    -1,    -1,
     913,    -1,   425,   102,    -1,    -1,   105,    -1,   431,   108,
      -1,    -1,   805,    -1,    -1,   438,    -1,  1077,   736,   779,
     738,    -1,    -1,    -1,   244,   743,    -1,    -1,   746,    -1,
     790,   251,   792,    -1,   752,    -1,    -1,   755,   831,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
     963,   964,    -1,    -1,   967,   968,  1116,    -1,    -1,   158,
      -1,   779,    25,    -1,    27,   164,    -1,    -1,    -1,   492,
     983,    -1,   790,   496,   792,    -1,    -1,    -1,   991,    -1,
     993,   504,   995,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   311,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,   325,    -1,    -1,    35,    72,
      37,    38,    -1,    -1,    -1,    42,    -1,    -1,    31,   879,
      -1,    48,    -1,    -1,    -1,    -1,   886,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   897,    -1,    -1,
     103,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,    76,
      -1,   911,    79,   946,    81,    -1,    83,    -1,    -1,    -1,
     920,   879,    -1,    -1,   924,    -1,    -1,    -1,   886,    82,
      -1,    -1,    -1,    86,    -1,   102,    -1,   397,   105,   897,
      -1,   108,    -1,  1096,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   614,    -1,   911,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   920,   166,    -1,   425,   924,    -1,    -1,   969,
      -1,   431,    -1,    -1,    -1,    -1,    -1,    -1,   438,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,    -1,
      -1,   158,   145,    -1,    -1,    -1,    -1,   164,    -1,    -1,
     153,    -1,     2,   666,    31,    -1,   159,   160,   161,   672,
      -1,   969,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1020,  1021,  1022,  1023,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,   492,    -1,    -1,    -1,   496,    -1,    -1,    -1,
      -1,   244,    -1,    -1,   504,  1045,  1046,    -1,   251,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,
      -1,    -1,  1020,  1021,  1022,  1023,    -1,    -1,    -1,    -1,
      -1,    -1,    72,   736,    -1,   738,    -1,    -1,    -1,    -1,
     743,    -1,    -1,   746,    -1,    -1,    -1,  1045,  1046,   752,
      -1,    -1,   755,    -1,     2,    -1,    -1,    -1,    -1,    99,
      -1,    -1,    -1,   103,    12,    -1,   133,   134,   311,    -1,
       1,   138,    -1,   140,   141,    -1,   779,    25,   145,    27,
      -1,    -1,   325,    -1,    -1,    -1,   153,   790,    -1,   792,
      -1,    22,   159,   160,   161,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,
      -1,    42,    -1,    -1,   614,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    72,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    76,    77,    -1,    79,    -1,
      81,    99,    83,    -1,   397,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,   666,    -1,    -1,    26,
      27,   102,   672,    -1,   105,    -1,   879,   108,    35,    -1,
      37,    38,   425,   886,    -1,    42,    -1,    -1,   431,    -1,
      -1,    48,    -1,    -1,   897,   438,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   244,    -1,    -1,    -1,   911,    -1,
      -1,   251,    -1,    70,    -1,    72,    -1,   920,   166,    -1,
      -1,   924,    -1,    -1,    81,    -1,    83,   158,    -1,    -1,
      -1,    -1,    -1,   164,    -1,    -1,   736,    -1,   738,    -1,
      -1,    -1,    -1,   743,    -1,   102,   746,    -1,   105,   492,
      -1,   108,   752,   496,    -1,   755,    -1,     2,    -1,     3,
      -1,   504,    -1,    -1,    -1,    -1,   969,    12,    -1,    -1,
      -1,   311,    -1,    17,    -1,    -1,    20,    -1,    -1,   779,
      25,    25,    27,    -1,    -1,   325,    -1,    -1,    -1,    -1,
     790,    35,   792,    -1,    -1,    -1,   244,    -1,    -1,    -1,
      44,    -1,    -1,   251,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,  1020,  1021,  1022,
    1023,    65,    -1,    -1,    -1,    31,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,  1045,  1046,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    96,    -1,    99,    -1,    -1,   397,   103,   103,
      -1,    -1,    -1,   311,    -1,   109,    -1,    -1,    -1,    -1,
      -1,   614,    -1,    -1,    -1,    -1,    82,   325,    -1,   879,
      86,    -1,    -1,    -1,    -1,   425,   886,    -1,    -1,    -1,
      -1,   431,    -1,    -1,    -1,    -1,    -1,   897,   438,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   911,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     920,   166,    -1,   666,   924,    -1,    -1,   133,   134,   672,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,    -1,    -1,   150,   151,   152,   153,   154,   397,
     156,    -1,   492,   159,   160,   161,   496,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   504,    -1,    -1,    -1,    -1,   969,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   425,    -1,    -1,
      -1,    -1,    -1,   431,    -1,    -1,    -1,    -1,    -1,    -1,
     438,    -1,    -1,   736,    -1,   738,    -1,    -1,    -1,   244,
     743,    -1,    -1,   746,    -1,    -1,   251,    -1,    -1,   752,
      -1,    -1,   755,    -1,     2,    -1,    -1,    -1,    -1,    -1,
    1020,  1021,  1022,  1023,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   779,    25,    -1,    27,
      -1,    -1,    -1,    -1,   492,  1045,  1046,   790,   496,   792,
      -1,    -1,    -1,    -1,    -1,    -1,   504,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   311,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   614,    -1,     3,    -1,    -1,    -1,
     325,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    99,    39,    -1,    -1,   103,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,   666,    -1,    -1,    -1,
      -1,    58,   672,    -1,    -1,    -1,   879,    64,    65,    31,
      -1,    -1,    -1,   886,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   397,    80,   897,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    -1,    92,   614,    94,   911,    96,
      -1,    -1,    99,    -1,    -1,    -1,   103,   920,   166,   106,
     425,   924,   109,    -1,    -1,    -1,   431,    -1,    -1,    -1,
      82,    -1,    -1,   438,    86,    -1,   736,    -1,   738,    -1,
      -1,    -1,    -1,   743,    -1,    -1,   746,    -1,    -1,    -1,
      -1,    -1,   752,    -1,    -1,   755,    -1,    -1,   666,    -1,
      -1,    -1,    -1,    -1,   672,    -1,   969,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,   779,
      -1,   133,   134,    -1,   136,   137,   138,   492,   140,   141,
     790,   496,   792,   145,    -1,    -1,   244,    -1,    -1,   504,
     152,   153,    -1,   251,   156,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1020,  1021,  1022,
    1023,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   736,    -1,
     738,    -1,    -1,    -1,    -1,   743,    -1,    -1,   746,    -1,
      -1,    -1,  1045,  1046,   752,    -1,    -1,   755,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   311,    -1,    17,    18,    -1,    20,    -1,
      -1,   779,    -1,    25,    -1,    -1,    -1,   325,    -1,   879,
      -1,    33,   790,    -1,   792,    -1,   886,    39,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,   897,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,   614,
      -1,   911,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
     920,    -1,    -1,    -1,   924,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,
      92,    -1,    94,    -1,    96,    -1,    -1,    99,    -1,   397,
      -1,   103,   104,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,   666,    -1,    -1,    -1,    -1,    -1,   672,    -1,   969,
      -1,   879,    -1,    -1,    -1,    -1,    -1,   425,   886,    -1,
      -1,    -1,    -1,   431,    -1,    -1,    -1,    -1,    -1,   897,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   911,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   920,   165,    -1,    -1,   924,    -1,    -1,    -1,
    1020,  1021,  1022,  1023,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   736,    -1,   738,    -1,    -1,    -1,    -1,   743,    -1,
      -1,   746,    -1,    -1,   492,  1045,  1046,   752,   496,    22,
     755,    -1,    -1,    26,    27,    -1,   504,    -1,    -1,    -1,
      -1,   969,    35,    -1,    37,    38,    -1,    -1,    -1,    42,
      -1,    -1,    -1,    -1,   779,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   790,    -1,   792,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,
      -1,    74,    -1,    76,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,  1020,  1021,  1022,  1023,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,   108,    -1,  1045,  1046,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,   614,    -1,    -1,    33,
      -1,    35,    -1,    -1,   879,    39,    40,    41,    -1,    43,
      44,   886,    -1,    47,    -1,   158,    50,    -1,    -1,    -1,
      54,    55,   897,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,   911,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,   920,    80,    81,   666,   924,
      -1,    -1,    -1,    -1,   672,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   969,    -1,    -1,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   736,    -1,
     738,    -1,    -1,   157,    -1,   743,    -1,    -1,   746,   163,
      -1,   165,   166,   167,   752,    -1,    -1,   755,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1020,  1021,  1022,  1023,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   779,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1045,  1046,   790,    -1,   792,     0,     1,    -1,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    -1,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    -1,    64,
      65,    66,    -1,    -1,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    -1,    83,    84,
      85,    -1,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    96,    97,    -1,    99,   100,   101,   102,   103,   897,
     105,   106,   107,   108,   109,    -1,    -1,   112,    -1,   114,
      -1,    -1,   117,   911,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   920,    -1,    -1,    -1,   924,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,   158,    -1,    -1,   161,    -1,   163,    -1,
     165,     1,   167,     3,     4,     5,     6,     7,     8,     9,
      10,   969,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
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
      -1,   161,    -1,   163,    -1,   165,    -1,   167,   168,     1,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    -1,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    96,    97,    -1,    99,   100,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,   163,   164,   165,     1,   167,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    -1,    50,    51,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    -1,    64,    65,    66,
      -1,    -1,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      87,    88,    89,    90,    91,    92,    93,    94,    -1,    96,
      97,    -1,    99,   100,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,   158,    -1,    -1,   161,    -1,   163,   164,   165,     1,
     167,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    -1,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    96,    97,    -1,    99,   100,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,
      -1,   163,   164,   165,     1,   167,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    -1,    50,    51,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    -1,    64,    65,    66,
      -1,    -1,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      87,    88,    89,    90,    91,    92,    93,    94,    -1,    96,
      97,    -1,    99,   100,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,   158,    -1,    -1,   161,    -1,   163,   164,   165,     1,
     167,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    -1,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    96,    97,    -1,    99,   100,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,   161,
      -1,   163,   164,   165,     1,   167,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    -1,    50,    51,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    -1,    64,    65,    66,
      -1,    -1,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      87,    88,    89,    90,    91,    92,    93,    94,    -1,    96,
      97,    -1,    99,   100,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,
     157,   158,    -1,    -1,   161,    -1,   163,   164,   165,     1,
     167,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    -1,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
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
      -1,   163,    -1,   165,     1,   167,     3,     4,     5,     6,
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
      -1,   163,    -1,   165,    -1,   167,   168,     3,     4,     5,
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
      -1,   161,    -1,   163,    -1,   165,    -1,   167,   168,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,    -1,
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
      99,   100,   101,    -1,   103,   104,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,    -1,    -1,    -1,    -1,    -1,
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
      94,    -1,    96,    -1,    -1,    99,   100,   101,    -1,   103,
     104,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,
      -1,   165,   166,   167,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,   100,   101,    -1,   103,   104,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,   166,   167,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,    -1,
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
      91,    92,    93,    94,    -1,    96,    -1,    -1,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,   158,    -1,    -1,
     161,    -1,   163,    -1,   165,    -1,   167,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    -1,    99,   100,   101,    -1,   103,    -1,    -1,
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
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    12,   142,
      -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,   152,
      -1,    25,   155,   156,   157,    -1,    -1,    -1,   161,    33,
     163,    35,   165,    -1,   167,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    92,    -1,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,   123,
      -1,    -1,   126,   127,   128,   129,    -1,   131,   132,   133,
     134,   135,   136,   137,    -1,    -1,   140,    -1,    -1,    -1,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,    -1,   156,    -1,    -1,   159,   160,   161,   162,    -1,
       3,   165,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,   102,
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
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,   115,    -1,   117,    -1,    -1,
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
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
     115,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      92,    93,    94,    -1,    96,    -1,    -1,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,
      -1,   163,    -1,   165,    -1,   167,     3,     4,     5,     6,
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
     157,    -1,    -1,    -1,   161,    -1,   163,   164,   165,     3,
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
      -1,   115,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,   163,
      -1,   165,     3,   167,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
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
     161,    -1,   163,    -1,   165,    -1,   167,     3,     4,     5,
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
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,   102,
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
      -1,   161,    -1,    -1,    -1,   165,     3,   167,     5,     6,
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
      -1,    -1,   109,    -1,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   162,   156,
     157,   165,    -1,   167,   161,    -1,   163,    -1,   165,     3,
     167,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    56,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    98,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,   135,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,   157,   158,    -1,    -1,    -1,    -1,   163,
      33,   165,    35,   167,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,    -1,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,     3,   135,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,   162,    25,   157,   165,    -1,   167,    -1,    -1,
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
      -1,    -1,     3,   135,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,   157,    -1,    -1,    -1,    -1,
      -1,   163,    33,   165,    35,   167,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    80,
      81,    -1,     3,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    96,    17,    18,    99,    20,
      -1,    -1,   103,    -1,    25,   106,    -1,    -1,   109,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    58,    -1,    -1,
       3,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,    80,
      -1,    -1,    25,    -1,   165,    -1,   167,    -1,    89,    90,
      33,    92,    -1,    94,    -1,    96,    39,    -1,    99,    -1,
      -1,    44,   103,    -1,    47,   106,    -1,    50,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,     3,    -1,
      -1,    64,    65,    -1,    -1,    -1,    -1,    12,    71,    -1,
      -1,    -1,    17,    18,    19,    20,    -1,    80,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    89,    90,    33,    92,
      -1,    94,    -1,    96,    39,    -1,    99,    -1,    -1,    44,
     103,    -1,    47,   106,   165,    50,   109,    52,    -1,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    64,
      65,    -1,    -1,    12,    -1,    -1,    71,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    89,    90,    -1,    92,    -1,    94,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,
      -1,   106,   165,    -1,   109,    -1,    -1,    -1,    -1,    -1,
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
      -1,   159,   160,   161,    -1,    -1,    31,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   117,    82,   159,   160,
     161,    86,    -1,    12,    -1,   166,    -1,    -1,    -1,    -1,
      19,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    31,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    -1,    -1,   166,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,    -1,
     145,   146,   147,    -1,    -1,   150,   151,   152,   153,    -1,
      -1,   156,    -1,    82,   159,   160,   161,    86,    -1,    -1,
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
      -1,    -1,   133,   134,    -1,   136,   137,   138,    31,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,   117,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    12,    82,
     159,   160,   161,    86,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    82,   152,
     153,    -1,    86,   156,    -1,    -1,   159,   160,   161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,   117,    86,    -1,    12,    -1,    -1,    -1,
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
      -1,   158,   150,   151,   152,   153,   154,   164,   156,    -1,
      -1,   159,   160,   161,     1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,     3,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    17,    18,    -1,
      20,    58,    -1,    -1,    -1,    25,    -1,    64,    65,    -1,
      -1,    -1,    -1,    33,    71,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    80,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    89,    90,    -1,    92,    -1,    94,    58,    96,
      -1,     3,    99,    -1,    64,    65,   103,    -1,    -1,   106,
      -1,    71,   109,    -1,    -1,    17,    18,    -1,    20,    -1,
      80,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    89,
      90,    33,    92,    -1,    94,    -1,    96,    39,     3,    99,
      -1,    -1,    44,   103,    -1,    47,   106,    -1,    50,   109,
      -1,    -1,    17,    18,    -1,    20,    58,    -1,    -1,    -1,
      25,    -1,    64,    65,    -1,    -1,    -1,    -1,    33,    71,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    80,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    89,    90,    -1,
      92,    -1,    94,    58,    96,    -1,     3,    99,    -1,    64,
      65,   103,    -1,    -1,   106,    -1,    71,   109,    -1,    -1,
      17,    18,    -1,    20,    -1,    80,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    89,    90,    33,    92,    -1,    94,
      -1,    96,    39,     3,    99,    -1,    -1,    44,   103,    -1,
      47,   106,    -1,    50,   109,    -1,    -1,    17,    18,    -1,
      20,    58,    -1,    -1,    -1,    25,    -1,    64,    65,    -1,
      -1,    -1,    -1,    33,    71,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    80,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    89,    90,    -1,    92,    -1,    94,    58,    96,
      -1,     3,    99,    -1,    64,    65,   103,    -1,    -1,   106,
      -1,    71,   109,    -1,    -1,    17,    18,    -1,    20,    -1,
      80,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    89,
      90,    33,    92,    -1,    94,    -1,    96,    39,    -1,    99,
      -1,    -1,    44,   103,    -1,    47,   106,    -1,    50,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      22,    -1,    64,    65,    26,    27,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    80,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    89,    90,    51,
      92,    -1,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,    70,    -1,
      72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    81,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,    -1,   105,    -1,   107,   108
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
     158,   139,   158,    13,    36,    67,   278,   163,     1,   195,
     230,   231,    27,    72,    83,   102,   239,   249,   165,   158,
     158,   158,   158,   158,   166,   168,    46,    83,   139,   166,
     285,    82,    82,    43,   213,   303,    43,   213,   303,   213,
     303,   292,   292,   158,   255,   305,   284,   195,   226,   158,
     195,   226,   158,   303,   166,   303,    32,   200,    32,   200,
     296,   297,   303,    32,   200,   295,    32,   200,   295,   158,
      13,   158,    34,    34,   178,    95,   183,    46,    83,   247,
     139,   166,   165,   195,    27,    72,    83,   102,   251,   166,
     269,   273,     1,   278,    63,   305,   166,   166,   164,    68,
     110,   164,   223,   166,   165,   183,   195,   219,   262,   178,
     168,   295,   168,   295,   178,   116,   196,   303,   303,   187,
     186,     1,   227,   164,   118,   139,   164,    83,   238,     1,
       3,    12,    17,    19,    20,    25,    39,    44,    50,    52,
      58,    64,    65,    80,    92,    96,    99,   103,   109,   133,
     134,   135,   136,   137,   140,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   156,   159,   160,   161,
     162,   165,   194,   195,   197,   240,   241,   242,   285,   166,
     297,   273,   285,   285,   303,    32,    32,   303,    32,    32,
     168,   168,   200,   200,   226,   163,   226,   163,   200,    95,
      43,   303,    43,   303,   139,   166,    95,    43,   303,   200,
      43,   303,   200,   303,   178,   303,   178,    34,   195,   246,
     269,   138,   277,    83,   273,   270,   168,    46,   168,   165,
     165,    32,   178,   278,   219,   138,   183,    43,   178,   303,
     168,    43,   178,   303,   168,   303,   158,    13,    36,    67,
     158,   158,   164,    76,    79,   164,   177,   202,   303,   231,
     251,   165,   243,   195,   303,   133,   141,   243,   243,   270,
      95,    43,    43,    95,    43,    43,    43,    43,   163,   227,
     163,   227,   303,   303,   303,   297,   303,   303,   303,    34,
     178,   277,   166,   167,   199,   255,   276,   286,   143,   256,
     270,    56,   111,   257,   303,   274,   286,   282,   282,   178,
     200,   166,   303,   303,   178,   303,   178,   303,   187,   186,
     202,     1,   138,   272,   245,   166,     3,    96,   241,   242,
     303,   303,   303,   303,   303,   303,   227,   164,   227,   164,
      95,    95,    95,    95,   178,   270,   277,   168,   278,   255,
     303,     3,    85,    96,   258,   259,   260,   303,   183,   201,
     254,   168,   166,   166,   183,    95,    95,   158,   158,   167,
     199,   271,   286,    98,   252,   166,   243,   243,    95,    95,
      95,    95,    95,    95,   164,   164,   303,   303,   303,   303,
     256,   270,   255,   275,   276,   286,    46,   168,   260,   111,
     139,   118,   144,   146,   147,   150,   151,    56,   286,   157,
     157,   303,   303,     1,   168,   278,   257,   303,   275,   276,
     303,   259,   260,   260,   260,   260,   260,   260,   258,   168,
     271,   286,   168,   158,   253,   254,   168,   271,   286,   275
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
     190,   190,   191,   192,   192,   192,   192,   192,   193,   193,
     194,   194,   194,   194,   194,   194,   195,   195,   195,   195,
     195,   195,   196,   196,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   199,   199,   199,   199,   200,
     200,   201,   201,   202,   202,   202,   203,   202,   204,   204,
     204,   204,   204,   204,   204,   205,   205,   205,   205,   207,
     206,   208,   206,   209,   206,   210,   206,   206,   206,   206,
     206,   211,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   213,   214,
     214,   214,   214,   214,   214,   214,   214,   215,   216,   216,
     216,   216,   216,   216,   217,   217,   218,   218,   218,   219,
     219,   220,   221,   221,   222,   222,   223,   223,   223,   224,
     224,   225,   225,   225,   226,   226,   227,   227,   227,   228,
     228,   229,   230,   230,   230,   231,   231,   233,   234,   232,
     235,   235,   235,   237,   238,   236,   239,   239,   239,   239,
     239,   240,   240,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   243,   243,   244,   245,   245,   245,   246,
     246,   246,   246,   246,   247,   247,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   249,   249,   249,   249,   249,
     249,   250,   250,   251,   251,   251,   251,   251,   251,   252,
     252,   253,   253,   254,   254,   255,   255,   256,   256,   256,
     257,   257,   257,   257,   257,   258,   258,   259,   259,   259,
     259,   259,   259,   259,   260,   260,   261,   261,   261,   262,
     262,   263,   263,   263,   264,   264,   264,   264,   264,   265,
     265,   266,   266,   267,   267,   268,   268,   268,   269,   269,
     269,   270,   270,   270,   271,   271,   271,   271,   271,   271,
     271,   272,   272,   272,   272,   272,   273,   273,   273,   273,
     273,   274,   274,   274,   274,   275,   275,   275,   276,   276,
     276,   276,   276,   277,   277,   277,   277,   277,   278,   278,
     278,   278,   279,   279,   280,   280,   280,   281,   281,   282,
     282,   283,   283,   284,   284,   284,   284,   285,   285,   286,
     286,   286,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   289,   290,   291,   291,   291,   291,   291,   291,   291,
     291,   292,   292,   293,   294,   294,   295,   296,   296,   297,
     297,   297,   298,   298,   298,   298,   298,   298,   299,   299,
     300,   300,   300,   300,   300,   301,   301,   301,   301,   301,
     301,   301,   302,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   304,   304,   305,
     305,   305,   306,   306,   306,   306,   307,   307,   307,   307,
     308,   308,   308,   309,   309,   309,   309,   309,   310,   310,
     310,   310,   311,   311,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   313,   313,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   315,   315,   315,   315,   315,   315,   315,
     316,   316,   316,   316,   317,   317,   317,   317,   318,   318,
     318,   318,   318,   318,   318
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
       4,     6,     3,     4,     4,     4,     4,     4,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     2,     3,     1,     1,     2,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     5,     5,     2,     0,
       8,     0,     9,     0,     8,     0,     9,     3,     3,     5,
       5,     2,     5,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     3,     4,
       6,     7,     6,     7,     4,     5,     4,     5,     4,     4,
       3,     6,     5,     6,     5,     8,     7,     2,     3,     3,
       2,     2,     3,     3,     0,     2,     2,     3,     5,     1,
       3,     3,     5,     5,     0,     2,     3,     2,     3,     6,
       6,     1,     1,     1,     0,     2,     0,     2,     3,     5,
       5,     1,     1,     2,     3,     1,     3,     0,     0,     8,
       0,     1,     1,     0,     0,    10,     3,     3,     5,     5,
       3,     1,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     3,     0,     1,     3,     4,
       5,     4,     6,     6,     0,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     0,     1,     2,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     2,     1,     1,     2,     2,
       0,     2,     2,     4,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     3,     4,     4,     2,
       4,     0,     2,     2,     1,     1,     1,     2,     1,     4,
       3,     1,     3,     3,     5,     1,     1,     3,     1,     2,
       3,     0,     2,     2,     3,     2,     4,     3,     3,     4,
       3,     0,     2,     2,     2,     1,     0,     2,     2,     2,
       1,     4,     4,     6,     3,     0,     1,     1,     3,     4,
       3,     4,     6,     0,     2,     2,     2,     2,     1,     1,
       3,     3,     1,     3,     1,     1,     1,     3,     3,     0,
       1,     1,     3,     3,     3,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     2,     4,     4,
       4,     5,     2,     2,     1,     2,     1,     2,     1,     2,
       1,     2,     1,     1,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     4,     1,     3,     4,
       3,     3,     1,     1,     2,     1,     2,     1,     1,     3,
       1,     2,     2,     2,     2,     2,     8,     8,     9,     9,
       5,     5,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     3,     3,     2,     2,     1,     0,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       4,     4,     4,     3,     3,     3,     3,     5,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
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
#line 497 "chapel.ypp" /* yacc.c:1663  */
    { yyblock = (yyval.pblockstmt); }
#line 6003 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 3:
#line 502 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6009 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 4:
#line 503 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6015 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 6:
#line 510 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6021 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 7:
#line 515 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6027 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 8:
#line 516 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6033 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 24:
#line 536 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6039 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 25:
#line 537 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6045 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 26:
#line 538 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6051 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 27:
#line 539 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6057 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 28:
#line 540 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6063 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 29:
#line 541 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6069 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 30:
#line 542 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6075 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 31:
#line 543 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6081 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 32:
#line 544 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6087 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 33:
#line 545 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6093 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 34:
#line 546 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6099 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 35:
#line 547 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6105 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 36:
#line 548 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6111 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 37:
#line 549 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6117 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 38:
#line 550 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6123 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 39:
#line 551 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6129 "bison-chapel.cpp" /* yacc.c:1663  */
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
#line 6143 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 41:
#line 569 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6152 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 42:
#line 574 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6161 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 43:
#line 579 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6170 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 44:
#line 586 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6176 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 45:
#line 587 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6182 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 46:
#line 588 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6188 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 47:
#line 592 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6194 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 48:
#line 593 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 6200 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 49:
#line 606 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6206 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 50:
#line 607 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6212 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 51:
#line 608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6218 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 52:
#line 613 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6224 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 53:
#line 614 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6230 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 54:
#line 619 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6240 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 55:
#line 624 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6250 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 56:
#line 629 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6259 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 57:
#line 633 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6268 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 58:
#line 640 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6278 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 60:
#line 649 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6288 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 62:
#line 658 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = (currentModuleType != MOD_INTERNAL ? true : false); }
#line 6294 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 63:
#line 659 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6300 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 64:
#line 660 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6306 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 65:
#line 664 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 6312 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 66:
#line 665 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 6318 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 67:
#line 666 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6324 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 68:
#line 667 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 6330 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 69:
#line 668 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6336 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 70:
#line 672 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildImportStmt((yyvsp[-1].pexpr), (yyvsp[-3].b)); }
#line 6342 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 71:
#line 673 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildImportStmt((yyvsp[-3].pexpr), (yyvsp[-1].pch), (yyvsp[-5].b)); }
#line 6348 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 72:
#line 678 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 6354 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 73:
#line 683 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 6360 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 74:
#line 685 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 6366 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 75:
#line 687 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 6372 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 76:
#line 689 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 6378 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 77:
#line 691 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 6384 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 78:
#line 697 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 6390 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 79:
#line 698 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6396 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 80:
#line 702 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6402 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 81:
#line 703 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6408 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 82:
#line 704 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6414 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 83:
#line 705 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6420 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 84:
#line 706 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6426 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 85:
#line 707 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6432 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 86:
#line 710 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6438 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 87:
#line 711 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6444 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 88:
#line 712 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6450 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 89:
#line 713 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6456 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 90:
#line 714 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6462 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 91:
#line 715 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6468 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 92:
#line 727 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6474 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 93:
#line 728 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6480 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 94:
#line 739 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bool"; }
#line 6486 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 95:
#line 740 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "int"; }
#line 6492 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 96:
#line 741 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "uint"; }
#line 6498 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 97:
#line 742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "real"; }
#line 6504 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 98:
#line 743 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "imag"; }
#line 6510 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 99:
#line 744 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "complex"; }
#line 6516 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 100:
#line 745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bytes"; }
#line 6522 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 101:
#line 746 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "string"; }
#line 6528 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 102:
#line 747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "sync"; }
#line 6534 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 103:
#line 748 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "single"; }
#line 6540 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 104:
#line 749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "owned"; }
#line 6546 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 105:
#line 750 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "shared"; }
#line 6552 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 106:
#line 751 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "borrowed"; }
#line 6558 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 107:
#line 752 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "unmanaged"; }
#line 6564 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 108:
#line 753 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "domain"; }
#line 6570 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 109:
#line 754 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "index"; }
#line 6576 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 110:
#line 755 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "locale"; }
#line 6582 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 111:
#line 756 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "nothing"; }
#line 6588 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 112:
#line 757 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "void"; }
#line 6594 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 113:
#line 761 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6600 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 114:
#line 762 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 6606 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 115:
#line 763 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6612 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 116:
#line 764 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6618 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 117:
#line 765 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6624 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 118:
#line 766 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6630 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 119:
#line 767 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6636 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 120:
#line 768 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 6642 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 121:
#line 769 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6648 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 122:
#line 770 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6654 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 123:
#line 771 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 6660 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 124:
#line 772 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 6666 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 125:
#line 779 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_syncvar"; }
#line 6672 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 126:
#line 780 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_singlevar"; }
#line 6678 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 127:
#line 781 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_domain"; }
#line 6684 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 128:
#line 782 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_index"; }
#line 6690 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 129:
#line 786 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6696 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 130:
#line 787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6702 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 131:
#line 791 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6708 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 132:
#line 792 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6714 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 133:
#line 796 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6720 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 135:
#line 798 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6726 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 136:
#line 808 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 6732 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 137:
#line 809 "chapel.ypp" /* yacc.c:1663  */
    { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6738 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 145:
#line 823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6744 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 146:
#line 824 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6750 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 147:
#line 825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6756 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 148:
#line 826 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6762 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 149:
#line 831 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6771 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 150:
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
#line 6785 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 151:
#line 846 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6794 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 152:
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
#line 6809 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 153:
#line 862 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6818 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 154:
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
#line 6832 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 155:
#line 877 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6841 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 156:
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
#line 6855 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 157:
#line 893 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 6863 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 158:
#line 897 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 6871 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 159:
#line 902 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 6883 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 160:
#line 910 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 6895 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 161:
#line 921 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6903 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 162:
#line 927 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6909 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 163:
#line 928 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6915 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 164:
#line 929 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 6921 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 165:
#line 930 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6927 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 166:
#line 931 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 6933 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 167:
#line 932 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6939 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 168:
#line 933 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 6945 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 169:
#line 934 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6951 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 170:
#line 935 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 6957 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 171:
#line 936 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6963 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 172:
#line 937 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6969 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 173:
#line 938 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6975 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 174:
#line 939 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6981 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 175:
#line 940 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6987 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 176:
#line 941 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6993 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 177:
#line 942 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6999 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 178:
#line 943 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7005 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 179:
#line 944 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7011 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 180:
#line 946 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7021 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 181:
#line 952 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7031 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 182:
#line 958 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7041 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 183:
#line 964 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7051 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 184:
#line 970 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7062 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 185:
#line 977 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7073 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 186:
#line 984 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7081 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 187:
#line 988 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7089 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 188:
#line 994 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7095 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 189:
#line 998 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7101 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 190:
#line 999 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7107 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 191:
#line 1000 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7113 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 192:
#line 1001 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7119 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 193:
#line 1002 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7126 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 194:
#line 1004 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 195:
#line 1006 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7140 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 196:
#line 1008 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7147 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 197:
#line 1013 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 7153 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 198:
#line 1016 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 7159 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 199:
#line 1017 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 7165 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 200:
#line 1018 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 7171 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 201:
#line 1019 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 7177 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 202:
#line 1020 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7183 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 203:
#line 1021 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7189 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 204:
#line 1025 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7195 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 205:
#line 1026 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7201 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 206:
#line 1030 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 7207 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 207:
#line 1031 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7213 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 208:
#line 1032 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7219 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 209:
#line 1036 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 7225 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 210:
#line 1037 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 7231 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 211:
#line 1041 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 7237 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 212:
#line 1045 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 7243 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 213:
#line 1047 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 7249 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 214:
#line 1051 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7255 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 215:
#line 1052 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7261 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 216:
#line 1057 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 7267 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 217:
#line 1059 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7273 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 218:
#line 1061 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7279 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 219:
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
#line 7293 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 220:
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
#line 7307 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 221:
#line 1091 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7317 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 222:
#line 1096 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7327 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 223:
#line 1101 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7337 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 224:
#line 1109 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 7343 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 225:
#line 1110 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 7349 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 226:
#line 1115 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 7355 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 227:
#line 1117 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7361 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 228:
#line 1119 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 7367 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 229:
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
#line 7387 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 230:
#line 1140 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 7395 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 231:
#line 1147 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7405 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 232:
#line 1156 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 7417 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 233:
#line 1164 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 7425 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 234:
#line 1168 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 7433 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 235:
#line 1174 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 7439 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 236:
#line 1175 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 7445 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 237:
#line 1180 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 7454 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 238:
#line 1185 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7463 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 239:
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
#line 7485 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 240:
#line 1212 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 7493 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 241:
#line 1215 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 7502 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 242:
#line 1219 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 7511 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 243:
#line 1227 "chapel.ypp" /* yacc.c:1663  */
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 7525 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 244:
#line 1237 "chapel.ypp" /* yacc.c:1663  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7536 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 245:
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
#line 7563 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 246:
#line 1270 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 7571 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 247:
#line 1274 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7580 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 248:
#line 1279 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 7588 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 249:
#line 1283 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7597 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 250:
#line 1288 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 7605 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 252:
#line 1295 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7611 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 253:
#line 1299 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7617 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 254:
#line 1300 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 7623 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 255:
#line 1301 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&"; }
#line 7629 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 256:
#line 1302 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|"; }
#line 7635 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 257:
#line 1303 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^"; }
#line 7641 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 258:
#line 1304 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "~"; }
#line 7647 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 259:
#line 1305 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "=="; }
#line 7653 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 260:
#line 1306 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!="; }
#line 7659 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 261:
#line 1307 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<="; }
#line 7665 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 262:
#line 1308 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">="; }
#line 7671 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 263:
#line 1309 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<"; }
#line 7677 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 264:
#line 1310 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">"; }
#line 7683 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 265:
#line 1311 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+"; }
#line 7689 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 266:
#line 1312 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-"; }
#line 7695 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 267:
#line 1313 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*"; }
#line 7701 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 268:
#line 1314 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/"; }
#line 7707 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 269:
#line 1315 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<"; }
#line 7713 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 270:
#line 1316 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>"; }
#line 7719 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 271:
#line 1317 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%"; }
#line 7725 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 272:
#line 1318 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**"; }
#line 7731 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 273:
#line 1319 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!"; }
#line 7737 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 274:
#line 1320 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_by"; }
#line 7743 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 275:
#line 1321 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "#"; }
#line 7749 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 276:
#line 1322 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_align"; }
#line 7755 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 277:
#line 1323 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<=>"; }
#line 7761 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 278:
#line 1324 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<~>"; }
#line 7767 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 279:
#line 1325 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "init="; }
#line 7773 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 280:
#line 1326 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 7779 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 281:
#line 1330 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "="; }
#line 7785 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 282:
#line 1331 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+="; }
#line 7791 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 283:
#line 1332 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-="; }
#line 7797 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 284:
#line 1333 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*="; }
#line 7803 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 285:
#line 1334 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/="; }
#line 7809 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 286:
#line 1335 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%="; }
#line 7815 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 287:
#line 1336 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**="; }
#line 7821 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 288:
#line 1337 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&="; }
#line 7827 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 289:
#line 1338 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|="; }
#line 7833 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 290:
#line 1339 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^="; }
#line 7839 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 291:
#line 1340 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>="; }
#line 7845 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 292:
#line 1341 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<="; }
#line 7851 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 293:
#line 1345 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7857 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 294:
#line 1346 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7863 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 295:
#line 1350 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7869 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 296:
#line 1354 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7875 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 297:
#line 1355 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7881 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 298:
#line 1356 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7887 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 299:
#line 1361 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7893 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 300:
#line 1363 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7899 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 301:
#line 1365 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7905 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 302:
#line 1367 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7911 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 303:
#line 1369 "chapel.ypp" /* yacc.c:1663  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7917 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 304:
#line 1373 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7923 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 305:
#line 1374 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7929 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 306:
#line 1378 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_IN; }
#line 7935 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 307:
#line 1379 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7941 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 308:
#line 1380 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_OUT; }
#line 7947 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 309:
#line 1381 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST; }
#line 7953 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 310:
#line 1382 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7959 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 311:
#line 1383 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7965 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 312:
#line 1384 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7971 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 313:
#line 1385 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF; }
#line 7977 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 314:
#line 1386 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7983 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 315:
#line 1390 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7989 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 316:
#line 1391 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7995 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 317:
#line 1392 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF;   }
#line 8001 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 318:
#line 1393 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 8007 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 319:
#line 1394 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST;   }
#line 8013 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 320:
#line 1395 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 8019 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 321:
#line 1399 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 8025 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 322:
#line 1400 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 8031 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 323:
#line 1404 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 8037 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 324:
#line 1405 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 8043 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 325:
#line 1406 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 8049 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 326:
#line 1407 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_REF; }
#line 8055 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 327:
#line 1408 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_PARAM; }
#line 8061 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 328:
#line 1409 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_TYPE; }
#line 8067 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 329:
#line 1413 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 8073 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 330:
#line 1414 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 8079 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 331:
#line 1417 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = NULL; }
#line 8085 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 334:
#line 1423 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 8091 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 335:
#line 1427 "chapel.ypp" /* yacc.c:1663  */
    { 
      DefExpr * de = new DefExpr(new VarSymbol((yyvsp[0].pch)));
      USR_WARN(de, "support for space in query expressions is deprecated");
      (yyval.pexpr) = de;
  }
#line 8101 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 336:
#line 1432 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 8107 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 337:
#line 1436 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8113 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 338:
#line 1437 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8119 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 339:
#line 1438 "chapel.ypp" /* yacc.c:1663  */
    { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 8129 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 340:
#line 1446 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 8135 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 341:
#line 1448 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 8141 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 342:
#line 1450 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 8147 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 343:
#line 1452 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8153 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 344:
#line 1454 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8159 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 345:
#line 1459 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8165 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 346:
#line 1461 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8171 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 347:
#line 1465 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8177 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 348:
#line 1466 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8183 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 349:
#line 1467 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8189 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 350:
#line 1468 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8195 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 351:
#line 1469 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8201 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 352:
#line 1470 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8207 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 353:
#line 1471 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 8213 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 354:
#line 1476 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 8219 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 355:
#line 1478 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 8225 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 356:
#line 1483 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 8231 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 357:
#line 1485 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 8237 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 358:
#line 1487 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 8243 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 359:
#line 1492 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 8260 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 360:
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
#line 8278 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 361:
#line 1521 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8284 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 362:
#line 1523 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8290 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 363:
#line 1525 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 8296 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 364:
#line 1529 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 8302 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 365:
#line 1530 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 8308 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 366:
#line 1531 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 8314 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 367:
#line 1532 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 8320 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 368:
#line 1533 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 8326 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 369:
#line 1538 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8336 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 370:
#line 1544 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8345 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 372:
#line 1553 "chapel.ypp" /* yacc.c:1663  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 8354 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 373:
#line 1561 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 8360 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 374:
#line 1563 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8366 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 375:
#line 1568 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 8372 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 376:
#line 1570 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 8378 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 377:
#line 1572 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 8384 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 378:
#line 1577 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 8390 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 379:
#line 1579 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 8396 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 380:
#line 1581 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8402 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 381:
#line 1587 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8408 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 382:
#line 1588 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 8414 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 383:
#line 1589 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8420 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 384:
#line 1595 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8426 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 385:
#line 1597 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 8432 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 386:
#line 1599 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8440 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 387:
#line 1603 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 8448 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 388:
#line 1607 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8454 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 389:
#line 1609 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8462 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 390:
#line 1613 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8470 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 391:
#line 1620 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8476 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 392:
#line 1621 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8482 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 393:
#line 1622 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8488 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 394:
#line 1623 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8494 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 395:
#line 1624 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8500 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 396:
#line 1629 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8506 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 397:
#line 1630 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8512 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 398:
#line 1631 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 8518 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 399:
#line 1632 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8524 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 400:
#line 1633 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8530 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 401:
#line 1654 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8538 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 402:
#line 1658 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 8546 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 403:
#line 1662 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 8558 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 404:
#line 1670 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8566 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 405:
#line 1676 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8572 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 406:
#line 1677 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8578 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 407:
#line 1678 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8584 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 408:
#line 1683 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8590 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 409:
#line 1685 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8596 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 410:
#line 1691 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8602 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 411:
#line 1693 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8608 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 412:
#line 1695 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 8614 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 413:
#line 1699 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8620 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 414:
#line 1700 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8626 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 415:
#line 1701 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8632 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 416:
#line 1702 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8638 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 417:
#line 1703 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8644 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 418:
#line 1709 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8650 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 419:
#line 1710 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8656 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 420:
#line 1711 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8662 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 421:
#line 1712 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8668 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 422:
#line 1716 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 8674 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 423:
#line 1717 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8680 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 424:
#line 1721 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 8686 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 425:
#line 1722 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8692 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 426:
#line 1723 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8698 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 427:
#line 1727 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8704 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 428:
#line 1728 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8710 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 429:
#line 1732 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 8716 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 431:
#line 1737 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8722 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 432:
#line 1738 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8728 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 433:
#line 1742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8734 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 434:
#line 1743 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8740 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 435:
#line 1744 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8746 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 436:
#line 1745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8752 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 437:
#line 1749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8758 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 438:
#line 1750 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8764 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 439:
#line 1762 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8770 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 440:
#line 1764 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 8776 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 441:
#line 1766 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8782 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 447:
#line 1777 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 8788 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 448:
#line 1779 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 8794 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 449:
#line 1781 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 8800 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 450:
#line 1783 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 8806 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 451:
#line 1785 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 8812 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 452:
#line 1787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 8818 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 453:
#line 1789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 8824 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 454:
#line 1792 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 8830 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 455:
#line 1794 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8836 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 456:
#line 1796 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 8842 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 457:
#line 1798 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 8848 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 458:
#line 1800 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 8854 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 459:
#line 1802 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8860 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 460:
#line 1804 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 8866 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 461:
#line 1806 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 8872 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 462:
#line 1809 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 8878 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 463:
#line 1811 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 8884 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 464:
#line 1816 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8890 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 465:
#line 1818 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8896 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 466:
#line 1820 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8902 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 467:
#line 1822 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8908 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 468:
#line 1824 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8914 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 469:
#line 1826 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8920 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 470:
#line 1828 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8926 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 471:
#line 1830 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8932 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 472:
#line 1832 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8938 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 473:
#line 1834 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8944 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 474:
#line 1836 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8950 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 475:
#line 1838 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8956 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 476:
#line 1840 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8967 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 477:
#line 1847 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8977 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 478:
#line 1853 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8987 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 479:
#line 1859 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8997 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 480:
#line 1865 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 9007 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 481:
#line 1874 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9013 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 482:
#line 1883 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 9019 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 490:
#line 1899 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9025 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 491:
#line 1903 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 9031 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 493:
#line 1908 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9037 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 494:
#line 1912 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9043 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 495:
#line 1913 "chapel.ypp" /* yacc.c:1663  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9049 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 496:
#line 1917 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9055 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 497:
#line 1921 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9061 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 498:
#line 1922 "chapel.ypp" /* yacc.c:1663  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9067 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 499:
#line 1927 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 9075 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 500:
#line 1931 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9083 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 501:
#line 1935 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9091 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 502:
#line 1941 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 9097 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 503:
#line 1942 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 9103 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 504:
#line 1943 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 9109 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 505:
#line 1944 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 9115 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 506:
#line 1945 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 9121 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 507:
#line 1946 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 9127 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 509:
#line 1952 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 510:
#line 1957 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 9139 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 511:
#line 1959 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 9147 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 512:
#line 1963 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 9155 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 513:
#line 1967 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 9163 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 514:
#line 1971 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 9171 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 515:
#line 1979 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 9178 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 516:
#line 1984 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9188 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 517:
#line 1990 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9198 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 518:
#line 1996 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9209 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 519:
#line 2003 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9220 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 520:
#line 2012 "chapel.ypp" /* yacc.c:1663  */
    { USR_WARN((yyvsp[-1].pexpr), "the syntax 'new owned(an expression)' is deprecated");
      USR_PRINT((yyvsp[-1].pexpr), "instead use 'owned.create(an expression)'");
      (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtOwned->symbol, (yyvsp[-1].pexpr))); }
#line 9228 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 521:
#line 2016 "chapel.ypp" /* yacc.c:1663  */
    { USR_WARN((yyvsp[-1].pexpr), "the syntax 'new shared(an expression)' is deprecated");
      USR_PRINT((yyvsp[-1].pexpr), "instead use 'shared.create(an expression)'");
      (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtShared->symbol, (yyvsp[-1].pexpr))); }
#line 9236 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 522:
#line 2023 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 9242 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 531:
#line 2039 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 9248 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 532:
#line 2041 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9254 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 533:
#line 2043 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9260 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 534:
#line 2045 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 9266 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 535:
#line 2047 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 9272 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 536:
#line 2049 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 9278 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 537:
#line 2053 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9284 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 538:
#line 2054 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9290 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 539:
#line 2058 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 9296 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 540:
#line 2059 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 9302 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 541:
#line 2060 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9308 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 546:
#line 2078 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9314 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 547:
#line 2079 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9320 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 548:
#line 2080 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 9326 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 549:
#line 2081 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9332 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 550:
#line 2085 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9338 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 551:
#line 2086 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9344 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 552:
#line 2087 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 9350 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 553:
#line 2091 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 9356 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 554:
#line 2092 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 9362 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 555:
#line 2093 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 9368 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 556:
#line 2094 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 9374 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 557:
#line 2095 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 9380 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 558:
#line 2103 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 9386 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 559:
#line 2104 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 9392 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 560:
#line 2105 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 9398 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 561:
#line 2106 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 9404 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 562:
#line 2110 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 9410 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 563:
#line 2111 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 9416 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 565:
#line 2116 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 9422 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 566:
#line 2117 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 9428 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 567:
#line 2118 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 9434 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 568:
#line 2119 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 9440 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 569:
#line 2120 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 9446 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 570:
#line 2121 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 9452 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 571:
#line 2122 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 9458 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 572:
#line 2123 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 9464 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 573:
#line 2124 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 9470 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 574:
#line 2125 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 9476 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 575:
#line 2126 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 9482 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 576:
#line 2128 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 9490 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 577:
#line 2132 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 9498 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 578:
#line 2139 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9504 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 579:
#line 2140 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9510 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 580:
#line 2144 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9516 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 581:
#line 2145 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9522 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 582:
#line 2146 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9528 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 583:
#line 2147 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9534 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 584:
#line 2148 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9540 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 585:
#line 2149 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9546 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 586:
#line 2150 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9552 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 587:
#line 2151 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9558 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 588:
#line 2152 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9564 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 589:
#line 2153 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9570 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 590:
#line 2154 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9576 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 591:
#line 2155 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9582 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 592:
#line 2156 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9588 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 593:
#line 2157 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9594 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 594:
#line 2158 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9600 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 595:
#line 2159 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9606 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 596:
#line 2160 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9612 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 597:
#line 2161 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9618 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 598:
#line 2162 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9624 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 599:
#line 2163 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9630 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 600:
#line 2164 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9636 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 601:
#line 2165 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9642 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 602:
#line 2166 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9648 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 603:
#line 2170 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 9654 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 604:
#line 2171 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 9660 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 605:
#line 2172 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 9666 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 606:
#line 2173 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 9672 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 607:
#line 2174 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 9678 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 608:
#line 2175 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9684 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 609:
#line 2176 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 9690 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 610:
#line 2180 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9696 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 611:
#line 2181 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9702 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 612:
#line 2182 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9708 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 613:
#line 2183 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9714 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 614:
#line 2187 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9720 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 615:
#line 2188 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9726 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 616:
#line 2189 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9732 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 617:
#line 2190 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9738 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 618:
#line 2195 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 9744 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 619:
#line 2196 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 9750 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 620:
#line 2197 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 9756 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 621:
#line 2198 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 9762 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 622:
#line 2199 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 9768 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 623:
#line 2200 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 9774 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 624:
#line 2201 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 9780 "bison-chapel.cpp" /* yacc.c:1663  */
    break;


#line 9784 "bison-chapel.cpp" /* yacc.c:1663  */
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
