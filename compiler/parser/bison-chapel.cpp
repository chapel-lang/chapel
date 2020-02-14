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
#define YYLAST   18102

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  173
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  146
/* YYNRULES -- Number of rules.  */
#define YYNRULES  623
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1117

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
     672,   676,   680,   682,   684,   686,   688,   695,   696,   700,
     701,   702,   703,   704,   705,   708,   709,   710,   711,   712,
     713,   725,   726,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,   750,   751,   752,   753,
     754,   755,   759,   760,   761,   762,   763,   764,   765,   766,
     767,   768,   769,   776,   777,   778,   779,   783,   784,   788,
     789,   793,   794,   795,   805,   805,   810,   811,   812,   813,
     814,   815,   816,   820,   821,   822,   823,   828,   827,   843,
     842,   859,   858,   874,   873,   889,   893,   898,   906,   917,
     924,   925,   926,   927,   928,   929,   930,   931,   932,   933,
     934,   935,   936,   937,   938,   939,   940,   941,   942,   948,
     954,   960,   966,   973,   980,   984,   991,   995,   996,   997,
     998,   999,  1001,  1003,  1005,  1010,  1013,  1014,  1015,  1016,
    1017,  1018,  1022,  1023,  1027,  1028,  1029,  1033,  1034,  1038,
    1041,  1043,  1048,  1049,  1053,  1055,  1057,  1064,  1074,  1088,
    1093,  1098,  1106,  1107,  1112,  1113,  1115,  1120,  1136,  1143,
    1152,  1160,  1164,  1171,  1172,  1177,  1182,  1176,  1209,  1212,
    1216,  1224,  1234,  1223,  1266,  1270,  1275,  1279,  1284,  1291,
    1292,  1296,  1297,  1298,  1299,  1300,  1301,  1302,  1303,  1304,
    1305,  1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,  1314,
    1315,  1316,  1317,  1318,  1319,  1320,  1321,  1322,  1323,  1327,
    1328,  1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,
    1338,  1342,  1343,  1347,  1351,  1352,  1353,  1357,  1359,  1361,
    1363,  1365,  1370,  1371,  1375,  1376,  1377,  1378,  1379,  1380,
    1381,  1382,  1383,  1387,  1388,  1389,  1390,  1391,  1392,  1396,
    1397,  1401,  1402,  1403,  1404,  1405,  1406,  1410,  1411,  1414,
    1415,  1419,  1420,  1424,  1429,  1433,  1434,  1435,  1443,  1444,
    1446,  1448,  1450,  1455,  1457,  1462,  1463,  1464,  1465,  1466,
    1467,  1468,  1472,  1474,  1479,  1481,  1483,  1488,  1501,  1518,
    1519,  1521,  1526,  1527,  1528,  1529,  1530,  1534,  1540,  1548,
    1549,  1557,  1559,  1564,  1566,  1568,  1573,  1575,  1577,  1584,
    1585,  1586,  1591,  1593,  1595,  1599,  1603,  1605,  1609,  1617,
    1618,  1619,  1620,  1621,  1626,  1627,  1628,  1629,  1630,  1650,
    1654,  1658,  1666,  1673,  1674,  1675,  1679,  1681,  1687,  1689,
    1691,  1696,  1697,  1698,  1699,  1700,  1706,  1707,  1708,  1709,
    1713,  1714,  1718,  1719,  1720,  1724,  1725,  1729,  1730,  1734,
    1735,  1739,  1740,  1741,  1742,  1746,  1747,  1758,  1760,  1762,
    1768,  1769,  1770,  1771,  1772,  1773,  1775,  1777,  1779,  1781,
    1783,  1785,  1788,  1790,  1792,  1794,  1796,  1798,  1800,  1802,
    1805,  1807,  1809,  1814,  1816,  1818,  1820,  1822,  1824,  1826,
    1828,  1830,  1832,  1834,  1836,  1838,  1845,  1851,  1857,  1863,
    1872,  1882,  1890,  1891,  1892,  1893,  1894,  1895,  1896,  1897,
    1902,  1903,  1907,  1911,  1912,  1916,  1920,  1921,  1925,  1929,
    1933,  1940,  1941,  1942,  1943,  1944,  1945,  1949,  1950,  1955,
    1957,  1961,  1965,  1969,  1977,  1982,  1988,  1994,  2001,  2010,
    2012,  2017,  2025,  2026,  2027,  2028,  2029,  2030,  2031,  2032,
    2033,  2035,  2037,  2039,  2041,  2043,  2048,  2049,  2053,  2054,
    2055,  2059,  2060,  2061,  2062,  2073,  2074,  2075,  2076,  2080,
    2081,  2082,  2086,  2087,  2088,  2089,  2090,  2098,  2099,  2100,
    2101,  2105,  2106,  2110,  2111,  2112,  2113,  2114,  2115,  2116,
    2117,  2118,  2119,  2120,  2121,  2122,  2126,  2134,  2135,  2139,
    2140,  2141,  2142,  2143,  2144,  2145,  2146,  2147,  2148,  2149,
    2150,  2151,  2152,  2153,  2154,  2155,  2156,  2157,  2158,  2159,
    2160,  2161,  2165,  2166,  2167,  2168,  2169,  2170,  2171,  2175,
    2176,  2177,  2178,  2182,  2183,  2184,  2185,  2190,  2191,  2192,
    2193,  2194,  2195,  2196
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

#define YYPACT_NINF -975

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-975)))

#define YYTABLE_NINF -574

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -975,   108,  2816,  -975,   -30,  -975,  -975,  -975,  -975,  -975,
    -975,  -975,  4302,    18,   151,  -975, 13075,  -975, 17711,    18,
   10006,  -975,   214,    74,   151,  4302, 10006,  4302,    25, 17769,
   10169,  7911,  -975,  8074,  8857,  6597, 10006,  -975,    44,    48,
    -975,  -975, 17806,  -975,  2530,  9020,  -975, 10006,   362,  -975,
    -975,  -975, 10006,  -975, 13075,  -975, 10006,   248,   123,    -1,
   17994,  -975, 17864,  -975,  9185,  7420, 10006,  9020, 13075, 10006,
     218,  -975,   147,  4302,  -975, 10006,  -975, 10332, 10332, 17806,
    -975,  -975, 13075,  -975,  -975, 10006, 10006,  -975, 10006,  -975,
   10006,  -975,  -975, 12614, 10006,  -975, 10006,  -975,  -975,  -975,
    3314,  6762,  8239,  -975,  4139,  -975,   196,  -975,   288,  -975,
     296,  -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,
    -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,
   17806,  -975, 17806,   350,    42,  -975,  -975,  2530,  -975,   252,
    -975,   260,  -975,  -975,   284,   287,   306, 10006,   314,   329,
   17153,  2008,   263,   341,   346,  -975,  -975,  -975,  -975,  -975,
    -975,  -975,    15,  -975,  -975, 17153,   276,  4302,  -975,  -975,
     351, 10006,  -975,  -975, 10006, 10006, 10006,  -975, 10006,  9185,
    9185,   302,  -975,  -975,  -975,  -975,   169,   365,  -975,  -975,
     360, 15390, 17806,  2530,  -975,   366,  -975,   150, 17153,   416,
    7585,   446, 17901, 17153,   411,   450,  -975, 17959, 17806,   411,
   17806,   373,     9, 14828,    -3, 14752,   197,  -975, 15240, 14028,
     381,  7585,  -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,
    -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,
    -975,  -975,  -975,  -975,  -975,  -975,  4302,  -975,   379,  1856,
      37,     5,  -975,  4302,  -975,  -975, 14980,   505,  -975,   380,
     387,  -975, 14980,   169,   505,  -975,  7585,  1550,  -975,  -975,
    9348,  -975,  -975,  -975, 17806,  -975,   224, 17153, 10006, 10006,
    -975, 17153,   407, 15141,  -975, 14980,   169, 17153,   392,  7585,
    -975, 17153, 15617,  -975,  -975, 15697,  2635,  -975,  -975, 15777,
     452,   413,   169, 14980, 15809,   146,   146,   688,   505,   505,
     159,  -975,  -975,  3479,   247,  -975, 10006,  -975,    81,    84,
    -975,   -42,    12, 15858,   -34,   571,  -975,  3644,  -975,   520,
   10006,   445,   429,  -975,  -975,  -975,   292,   384,  -975, 10006,
     453, 10006, 10006, 10006,  8857,  8857, 10006,   388, 10006, 10006,
   10006, 10006, 10006,   213, 12614, 10006, 10006, 10006, 10006, 10006,
   10006, 10006, 10006, 10006, 10006, 10006, 10006, 10006, 10006, 10006,
    -975,  -975,  -975,  -975,  -975,  -975,  8402,  8402,  -975,  -975,
    -975,  -975,  8402,  -975,  -975,  8402,  8402,  7585,  7585,  8857,
    8857,  7257,  -975,  -975, 15439, 15536, 15954,    53,  3809,  -975,
    8857,     9,   437,   300,  -975, 10006,  -975, 10006,   480,  -975,
     433,   468,  -975,  -975,  -975, 17806,  -975,  2530,  -975, 17806,
     462,  -975,  2530,   562,  9185,  -975,  4465,  8857,  -975,   457,
    -975,     9,  4628,  8857,  -975,     9, 10495, 10006,  -975,  4302,
     594, 10006,  -975,   463,  -975,   335,  -975,  -975,  1856,  -975,
     494,   466,  -975, 10658,   516, 10006,  2530,  -975,  -975, 10006,
   10006,  -975,   470,  -975,  -975,  9185,  -975, 17153, 17153,  -975,
      35,  -975,  7585,   471,  -975,   618,  -975,   618,  -975, 10821,
     503,  -975,  -975,  -975,  -975,  -975,  -975,  8567,  -975, 13723,
    6927,  -975,  7092,  -975,  4302,   476,  8857,  6099,  3149,   479,
   10006,  6431,  -975,  -975,   349,  -975,  3974, 17806,   353, 15293,
    9185,   489, 17674,   162,  -975, 16003, 17474, 17474,   396,  -975,
     396,  -975,   396, 13952,   656,  1187,   946,   169,   146,   488,
    -975,  -975,  -975,  -975,   688,  1582,   396,  1327,  1327, 17474,
    1327,  1327,  1694,   146,  1582, 17520,  1694,   505,   505,   146,
     498,   501,   506,   509,   515,   497,   507,  -975,   396,  -975,
     396,    21,  -975,  -975,  -975,    87,  -975,  1092, 17193,   301,
   10984,  8857, 11147,  8857, 10006,  8857, 13337,    18, 16048,  -975,
    -975,  -975, 17153, 16148,  7585,  -975,  7585, 17806,   445,   358,
   17806,   445,  -975,   363, 10006,   110, 10006, 17153,    11, 15056,
    7257,  -975, 10006, 17153,    -2, 14904,  -975,   518,   535,   525,
   16193,   535,   527,   644, 16233,  4302, 15088,  -975,    50,  -975,
    -975,  -975,  -975,  -975,  -975,   608,   116,  -975,  2610,  -975,
     179,   526,  1856,    37,    25,    48, 10006, 10006,  5934,  -975,
    -975,   250,  7748,  -975, 17153,  -975, 13819, 13876,  -975,  -975,
   17153,   517,    -8,   528,  -975,  2408,  -975,  -975,   364, 17806,
    -975,  -975,  -975,  -975,  -975,  -975,  -975,  4302,   -38, 13488,
    -975,  -975, 17153,  4302, 17153,  -975, 16345,  -975,  -975,  -975,
   10006,  -975, 10006, 10495, 10006,   547,  2154,   533,   583,   255,
    -975,   621,  -975,  -975,  -975,  -975,  8696,   539,  -975,  -975,
    -975,  -975,  -975,  -975,  -975,  -975,  -975,  7257,  -975,    31,
    8857,  8857, 10006,   675, 16385, 10006,   677, 16425,   543, 13577,
       9,     9,  -975,  -975,  -975,  -975,   445,   549,  -975,   445,
     553,  -975, 14980,  -975, 14108,  4791,  -975,  4954,  -975,   135,
    -975, 14188,  5117,  -975,     9,  5280,  -975,     9,  -975, 10006,
    -975,  4302, 10006,  -975,  4302,   683,  -975,  -975, 17806,   709,
    -975,  1856,   580,   637,  -975,  -975,  -975,    97,  -975,  -975,
     516,   554,    61,  -975,  -975,   556,   559,  -975,  5443,  9185,
    -975,  -975,  -975, 17806,  -975,   589,   360,  -975,  -975,  5606,
     560,  5769,   561,  -975, 10006, 16537, 15585,   573,   576,   579,
    2704,  -975, 10006, 17806,  -975,  -975,   179,   570,   299,  -975,
     603,  -975,   604,   606,   615,   611,   612,  -975,   619,   616,
     620,   624,   627,   321,   626,   630,   631,  -975,  -975, 17806,
    -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,
    -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975, 10006,
    -975,   629,   640,   634,   570,   570,  -975,  -975,  -975,   516,
     153,   175, 16577, 11310, 11473, 16617, 11636, 11799, 11962, 12125,
    -975,  -975,   617,  -975,   622,  -975,  -975,  4302, 10006, 17153,
   10006, 17153,  7257,  -975,  4302, 10006, 17153,  -975, 10006, 17153,
    -975, 17153,  -975, 17153,   750,  4302,   580,  -975,   625,  9513,
     280,  -975,    80,  -975,  -975,  8857, 13206,  7585,  7585,  4302,
    -975,    59,   632, 10006,  -975, 10006,  -975, 17153,  4302, 10006,
    -975, 17153,  4302, 17153, 10006, 10495, 10006,  -975,  -975,  -975,
    -975,  1550,  -975, 17492,  -975, 17153,  -975,    67,   335,  -975,
    -975, 13916,  -975, 13432,  -975,  -975,  -975, 10006, 10006, 10006,
   10006, 10006, 10006, 10006, 10006,  -975, 17329,  -975, 17343, 16233,
   14268, 14348,  -975, 16233, 14428, 14508,  4302,  -975,   516,   580,
    6265,  -975,  -975,  -975,   109,  9185,  -975,  -975,    47, 10006,
     -21, 13634,  -975,   415,   633,   636,   416,  -975,   360, 17153,
   14588,  -975, 14668,  -975, 17153,   628,   645,  -975,  -975, 12288,
     689,   219,  -975,   655,   671,   570,   570, 16729, 16769, 16809,
   16921, 16961, 17001, 17401,  -975, 17441,  -975,  4302,  4302,  4302,
    4302,  -975,  -975,   280,  9678,    72,  -975, 17153,  -975,    89,
    -975,   161,  -975,   459, 17113,  -975,  -975,  -975, 12125,   638,
     648,  -975,  4302,  4302,  -975,  -975,  2981,  -975,  -975,    73,
    -975,    80,  -975,  -975,  -975, 10006, 10006, 10006, 10006, 10006,
   10006,  -975,  -975, 16233, 16233, 16233, 16233,  -975,  -975,  -975,
    -975,  -975,   283,  8857, 12779,  -975, 10006,    47,    89,    89,
      89,    89,    89,    89,    47,   530,  -975,  -975, 16233, 16233,
     639, 12451,    76,   132, 13674,  -975,  -975, 17153,  -975,  -975,
    -975,  -975,  -975,  -975,  -975,   669,  -975,  -975,   209, 12944,
    -975,  -975,  -975,  9843,  -975,   458,  -975
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    91,   564,   565,   566,   567,
     568,   569,     0,   490,    77,   112,   458,   118,   460,   490,
       0,   117,     0,   363,    77,     0,     0,     0,     0,   462,
     536,   536,   561,     0,     0,     0,     0,   116,     0,     0,
     239,   113,     0,   235,     0,     0,   120,     0,   509,   481,
     570,   121,     0,   240,   452,   362,     0,     0,     0,   134,
      45,   115,   461,   364,     0,     0,     0,     0,   456,     0,
       0,   119,     0,     0,    92,     0,   562,     0,     0,     0,
     114,   221,   454,   366,   122,     0,     0,   619,     0,   621,
       0,   622,   623,   535,     0,   620,   617,   439,   131,   618,
       0,     0,     0,     4,     0,     5,     0,     9,    47,    10,
       0,    11,    12,    13,    15,   435,   436,    23,    14,   132,
     141,   142,    16,    18,    17,    20,    21,    22,    19,   140,
       0,   138,     0,   527,     0,   136,   139,     0,   137,   541,
     523,   437,   524,   442,   440,     0,     0,     0,   528,   529,
       0,   441,     0,   542,   543,   544,   563,   522,   444,   443,
     525,   526,     0,    39,    25,   450,     0,     0,   491,    78,
       0,     0,   460,   462,     0,     0,     0,   461,     0,     0,
       0,   527,   541,   440,   528,   529,   459,   441,   542,   543,
       0,   490,     0,     0,   365,     0,   195,     0,   420,     0,
     427,   567,   461,   537,   238,   567,   159,   461,     0,   238,
       0,     0,     0,     0,     0,     0,     0,   146,     0,     0,
       0,   427,    85,    93,   105,    99,    98,   107,    88,    97,
     108,    94,   109,    86,   110,   103,    96,   104,   102,   100,
     101,    87,    89,    95,   106,   111,     0,    90,     0,     0,
       0,     0,   369,     0,   128,    33,     0,   604,   513,   510,
     511,   512,     0,   453,   605,     7,   427,   238,   219,   229,
     536,   220,   133,   334,   439,   417,     0,   416,     0,     0,
     129,   540,     0,     0,    36,     0,   457,   445,     0,   427,
      37,   451,     0,   202,   198,     0,   441,   202,   199,     0,
     359,     0,   455,     0,     0,   606,   608,   534,   603,   602,
       0,    49,    52,     0,     0,   422,     0,   424,     0,     0,
     423,     0,     0,   416,     0,     0,     6,     0,    48,     0,
       0,   222,     0,   320,   319,   241,     0,   438,    24,     0,
     514,     0,     0,     0,     0,     0,     0,   607,     0,     0,
       0,     0,     0,     0,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     279,   286,   287,   288,   283,   285,     0,     0,   281,   284,
     282,   280,     0,   290,   289,     0,     0,   427,   427,     0,
       0,     0,    26,    27,     0,     0,     0,     0,     0,    28,
       0,     0,     0,     0,    29,     0,    30,     0,   435,   433,
       0,   428,   429,   434,   153,     0,   156,     0,   149,     0,
       0,   155,     0,     0,     0,   168,     0,     0,   167,     0,
     176,     0,     0,     0,   174,     0,     0,    58,   143,     0,
     188,     0,    70,     0,    31,   302,   236,   373,     0,   374,
     376,     0,   398,     0,   379,     0,     0,   127,    32,     0,
       0,    34,     0,   135,   333,     0,    71,   538,   539,   130,
       0,    35,   427,     0,   209,   200,   196,   201,   197,     0,
     357,   354,   161,    38,    51,    50,    53,     0,   571,     0,
       0,   557,     0,   559,     0,     0,     0,     0,     0,     0,
       0,     0,   575,     8,     0,    41,     0,     0,     0,    54,
       0,     0,     0,   313,   368,   489,   599,   598,   601,   610,
     609,   614,   613,   595,   592,   593,   594,   531,   582,     0,
     554,   555,   553,   552,   532,   586,   597,   591,   589,   600,
     590,   588,   580,   585,   587,   596,   579,   583,   584,   581,
       0,     0,     0,     0,     0,     0,     0,   612,   611,   616,
     615,   501,   502,   504,   506,     0,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   573,   490,   490,   164,
     355,   367,   421,     0,     0,   447,     0,     0,   222,     0,
       0,   222,   356,     0,     0,     0,     0,   465,     0,     0,
       0,   177,     0,   471,     0,     0,   175,   618,    61,     0,
      54,    59,     0,   187,     0,     0,     0,   446,   307,   304,
     305,   306,   310,   311,   312,   302,     0,   295,     0,   303,
     321,     0,   377,     0,   125,   126,   124,   123,     0,   397,
     396,   523,     0,   371,   521,   370,     0,     0,   551,   419,
     418,     0,     0,     0,   448,     0,   203,   361,   523,     0,
     572,   530,   558,   425,   560,   426,   184,     0,     0,     0,
     574,   182,   475,     0,   577,   576,     0,    43,    42,    40,
       0,    65,     0,     0,    58,   223,     0,     0,   233,     0,
     230,   317,   314,   315,   318,   242,     0,     0,    75,    76,
      74,    73,    72,   549,   550,   503,   505,     0,   492,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   160,   431,   432,   430,   222,     0,   158,   222,
       0,   157,     0,   186,     0,     0,   166,     0,   165,     0,
     496,     0,     0,   172,     0,     0,   170,     0,   144,     0,
     145,     0,     0,   190,     0,   192,   308,   309,     0,   302,
     293,     0,   411,   322,   325,   324,   326,     0,   375,   378,
     379,     0,     0,   380,   381,   519,   520,   211,     0,     0,
     210,   213,   449,     0,   204,   207,     0,   358,   185,     0,
       0,     0,     0,   183,     0,    56,    55,     0,     0,     0,
     238,   228,     0,   231,   227,   316,   321,   291,    79,   274,
      93,   272,    99,    98,    82,    97,    94,   277,   109,    80,
     110,    96,   100,    81,    83,    95,   111,   271,   253,   256,
     254,   255,   266,   257,   270,   262,   260,   273,   276,   261,
     259,   264,   269,   258,   263,   267,   268,   265,   275,     0,
     251,     0,    84,     0,   291,   291,   249,   556,   494,   379,
     541,   541,     0,     0,     0,     0,     0,     0,     0,     0,
     163,   162,     0,   224,     0,   224,   169,     0,     0,   464,
       0,   463,     0,   495,     0,     0,   470,   173,     0,   469,
     171,    55,   189,   480,   191,     0,   411,   296,     0,     0,
     379,   323,   338,   372,   402,     0,   573,   427,   427,     0,
     215,     0,     0,     0,   205,     0,   180,   477,     0,     0,
     178,   476,     0,   578,     0,     0,    58,    66,    68,   218,
     134,   238,   217,   238,   225,   234,   232,     0,   302,   248,
     252,     0,   278,     0,   244,   245,   498,     0,     0,     0,
       0,     0,     0,     0,     0,   224,   238,   224,   238,   468,
       0,     0,   497,   474,     0,     0,     0,   194,   379,   411,
       0,   414,   413,   415,   523,   335,   299,   297,     0,     0,
       0,     0,   400,   523,     0,     0,   216,   214,     0,   208,
       0,   181,     0,   179,    57,     0,     0,   226,   393,     0,
     327,     0,   250,    79,    81,   291,   291,     0,     0,     0,
       0,     0,     0,   238,   152,   238,   148,     0,     0,     0,
       0,   193,   298,   379,   403,     0,   337,   336,   352,     0,
     353,   340,   343,     0,   339,   331,   332,   237,     0,   515,
     516,   206,     0,     0,    67,    69,     0,   392,   391,   523,
     328,   338,   292,   246,   247,     0,     0,     0,     0,     0,
       0,   154,   150,   467,   466,   473,   472,   301,   300,   405,
     406,   408,   523,     0,   573,   351,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   517,   518,   479,   478,
       0,   383,     0,     0,     0,   407,   409,   342,   344,   345,
     348,   349,   350,   346,   347,   341,   388,   386,   523,   573,
     329,   243,   330,   403,   387,   523,   410
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -975,  -975,  -975,     0,  -307,  1617,  -975,  -975,  -975,  -975,
     399,   483,  -324,  -658,  -655,  -975,  -975,  -975,  -975,   165,
     789,  -975,  1909,   103,  -680,  -975,  -862,   949,  -946,  -754,
    -975,   -57,  -975,  -975,  -975,  -975,  -975,  -975,  -975,  -975,
      46,  -975,  -975,  -975,   521,  -975,    36,  -975,  -975,  -975,
    -975,  -975,  -975,  -543,  -672,  -975,  -975,  -975,    24,   739,
    -975,  -975,  -975,   261,  -975,  -975,  -975,  -975,  -129,  -150,
    -768,  -975,  -115,    79,   206,  -975,  -975,  -975,    26,  -975,
    -975,  -258,   662,  -181,  -206,  -238,  -224,  -109,  -975,  -177,
    -975,     3,   823,  -124,   403,  -975,  -436,  -750,  -852,  -975,
    -594,  -472,  -974,  -962,  -782,   -53,  -975,  -164,  -975,  -194,
    -975,   274,   493,  -359,  -975,  -975,  -975,  1010,  -975,   -10,
    -975,  -975,  -205,  -975,  -583,  -975,  -975,  -975,  1268,  1371,
     -12,   831,    20,   523,  -975,  1572,  1771,  -975,  -975,  -975,
    -975,  -975,  -975,  -975,  -975,  -386
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   312,   104,   613,   106,   107,   108,   329,
     109,   313,   608,   612,   609,   110,   111,   112,   113,   114,
     170,   850,   250,   115,   247,   116,   639,   255,   117,   118,
     267,   119,   120,   121,   419,   590,   415,   587,   122,   123,
     718,   124,   125,   126,   475,   656,   786,   127,   128,   652,
     781,   129,   130,   511,   800,   131,   132,   689,   690,   181,
     248,   630,   134,   135,   513,   806,   695,   853,   854,   441,
     939,   446,   626,   627,   628,   629,   696,   335,   767,  1051,
    1111,  1037,   275,   976,   980,  1031,  1032,  1033,   136,   301,
     480,   137,   138,   251,   252,   450,   451,   643,  1048,  1000,
     454,   640,  1070,   973,   900,   314,   197,   318,   319,   410,
     411,   412,   182,   140,   141,   142,   143,   183,   145,   167,
     168,   565,   431,   739,   566,   567,   146,   147,   184,   185,
     150,   209,   413,   187,   152,   188,   189,   155,   156,   157,
     324,   158,   159,   160,   161,   162
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     165,   386,   103,   272,   186,   569,   508,   657,   191,   190,
     435,   276,   631,   336,   198,   402,   852,   740,   203,   203,
     903,   213,   215,   218,   219,   193,   798,   443,   797,   253,
     742,   420,   452,   256,  1036,   257,   651,   971,   452,   770,
     262,   253,   263,   735,   264,   727,   934,     5,   730,   322,
    1028,   455,   277,   281,   283,   285,   286,   287,   496,   -46,
     778,   291,  1071,   292,    65,   295,   299,   705,   998,   403,
     302,   429,   462,   303,   304,   429,   305,   -46,   306,   212,
     214,   307,   308,  -394,   309,   282,   944,   945,   277,   281,
     323,   253,  1028,   333,   641,   473,   756,   389,   452,   575,
    1095,   390,   779,  -212,   706,   501,   -64,   905,     3,   946,
     429,   429,  1096,   611,   968,   859,   495,   499,  1073,   334,
     658,   320,   575,  -389,   858,   429,   494,   397,   163,  -390,
     789,   166,  1029,   757,   502,   340,   978,  1047,   625,  1116,
      74,   220,   398,  1030,   456,  -212,   386,  1036,   321,  -394,
     977,   497,  -389,  -394,   169,  -394,   780,   194,  -390,   165,
     398,   398,   394,   395,   396,  -389,   291,   277,   323,   453,
    -394,  -390,   398,   902,   398,   453,  -394,   343,  -389,   997,
     498,   401,  -394,   872,  -390,  1030,   874,  1023,   281,   691,
     200,   979,   497,   555,   556,  -394,   769,  -394,   465,  -212,
     497,   956,   934,   958,   934,   999,   763,   417,  -394,   281,
     463,   497,   422,   221,   569,   497,     5,    65,  1022,   193,
     490,   576,   398,   492,    23,  -389,   707,  -412,   344,   906,
    -389,  -390,   345,   529,   692,   453,  -390,  1053,  1054,  1107,
    1074,    23,   294,   298,  1109,   693,   530,   491,  -412,   465,
     493,   764,  -412,   708,   281,   759,   265,  1114,   203,   934,
    -394,   934,   765,   852,   694,  -382,   467,   468,   996,    55,
     995,   531,  1076,  1068,   882,  -412,   733,   281,   653,   347,
      63,   766,   760,  1013,   351,  1015,    55,   353,   266,   405,
    1110,   356,  -500,   589,  -382,   398,  -395,    63,   593,   962,
    1077,   883,   347,   408,   489,    83,  -395,  -382,   406,    74,
     353,   288,   289,   486,  -499,   532,   192,   163,   509,  -500,
    -382,   569,    83,   484,   408,   898,   663,   515,   665,   516,
     517,   518,   520,   522,   523,  -395,   524,   525,   526,   527,
     528,  -499,   534,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   759,   327,
     611,  -395,   618,   465,   281,   281,   328,  -382,  -395,   408,
     281,   595,  -382,   281,   281,   281,   281,   558,   560,   568,
     258,   619,   466,   711,   620,  1052,   487,   390,   578,  -395,
     519,   521,   408,   582,   803,   583,   550,   551,   642,   744,
     747,  -404,   552,   330,   621,   553,   554,   622,  -395,    57,
    -483,   488,   277,  -395,   597,   599,  -395,   337,   623,   804,
     603,   605,  -404,   975,   610,   610,  -404,   614,   387,   616,
     388,   456,   -85,   259,   982,   557,   559,   624,    23,   456,
     -91,   391,  -482,   644,   254,   338,   577,   646,   647,  -404,
     514,   260,   625,   650,   -87,   339,   533,   685,   581,    40,
     281,  -399,   -92,   790,   792,   416,   254,  -548,   261,  -548,
     421,  -399,  -487,   598,  -294,   650,   293,   297,   281,   604,
     281,    53,   787,    55,   669,   650,   672,  -488,   674,   676,
     408,   408,   680,   933,    63,   139,   569,   456,   277,  -485,
    -399,  -294,   456,  -360,  -484,   139,   486,   163,  -486,   393,
     320,   681,   320,   677,  -384,  -548,   728,  -548,   139,    83,
     139,   731,  -360,   398,   404,   151,  -399,   414,   407,   347,
    -545,   418,  -545,  -399,   351,   151,   343,   353,   424,   442,
     974,   356,   668,  -384,   445,   459,   855,   983,   151,  -547,
     151,  -547,   460,  -546,  -399,  -546,  -384,   472,   597,   714,
     603,   717,   614,   719,   672,   469,   139,   720,   721,  -384,
     479,   481,   281,  -399,   281,   408,  -401,  1078,  -399,   503,
     507,  -399,   732,   510,   734,   772,  -401,   344,   568,   399,
     741,   345,   512,   139,   353,   580,   151,   139,   584,   585,
     296,   296,   611,  1079,   724,  1080,  1081,   586,   594,  1082,
    1083,   254,   254,   254,   254,  -401,  -384,   713,   440,   716,
     592,  -384,   600,   151,   287,   291,   323,   151,   615,   617,
     281,   625,   633,   632,   642,   618,   648,   654,   347,   655,
    1049,  -401,   659,   351,   667,   352,   353,   673,  -401,   933,
     356,   933,   686,   697,   619,   254,   698,   620,   363,   699,
     139,   254,   774,   703,   700,  1072,   369,   701,   795,  -401,
     796,   610,   610,   702,   680,   704,   -60,   621,   751,  1085,
     622,   777,   325,   748,   254,   750,   465,   343,  -401,   408,
     151,   623,   768,  -401,   782,   568,  -401,   801,   520,   558,
     862,   802,   254,   865,   805,   857,   933,   863,   933,   866,
     624,   868,   873,   984,   985,  1072,   875,   895,   899,   343,
     901,   907,   904,   879,   908,   881,   911,   913,   918,   922,
     886,   927,  1108,   889,   928,   938,   618,   891,   344,   139,
     893,   133,   345,   929,  -112,  -118,   139,  -117,   -88,   -86,
    1115,   133,  -116,  -113,  1072,   619,   519,   557,   620,   -89,
    -120,  -121,   942,   317,   133,  -115,   133,   277,  -119,   151,
     344,  -114,  -122,   -90,   345,   943,   151,   917,   621,   921,
     955,   622,   923,    57,   966,   957,  1044,  1050,   -85,   347,
     935,   969,   623,  1006,   351,  1086,   352,   353,   988,  1039,
     254,   356,  1040,  1045,   -87,  1087,   139,  1106,  1077,   363,
     506,   624,   133,   195,  1005,   367,   368,   369,   477,   912,
     139,   347,   348,  1001,   349,   350,   351,   936,   352,   353,
     254,   758,   937,   356,   254,  1112,   151,   941,   897,   133,
     362,   363,  1067,   133,   366,  1093,  1105,   367,   368,   369,
     151,   879,   881,  1098,   886,   889,   917,   921,   217,   645,
     725,   204,   409,     0,     0,   959,   960,     0,   961,     0,
     568,     0,   963,   964,   272,     0,   965,     0,     0,     0,
       0,     0,     0,   409,     0,     0,     0,     0,     0,     0,
       0,   139,     0,   981,   672,   281,   281,     0,     0,     0,
       0,   989,     0,   990,     0,     0,   133,   992,     0,     0,
       0,     0,   994,   610,   610,     0,     0,  1025,     0,   139,
    1075,   151,     0,     0,     0,   139,     0,     0,   409,     0,
       0,     0,   139,     0,     0,   959,  1007,  1008,   963,  1009,
    1010,  1011,  1012,     0,     0,     0,     0,     0,     0,   151,
       0,   409,     0,     0,     0,   151,     0,     0,   323,     0,
       0,     0,   151,  1027,     0,     0,     0,  1034,     0,  1099,
    1100,  1101,  1102,  1103,  1104,     0,     0,   343,     0,     0,
       0,     0,     0,     0,     0,   133,     0,   139,     0,     0,
       0,   139,   133,  1092,     0,     0,     0,   254,   254,   139,
       0,     0,     0,   254,   254,  1063,  1064,  1065,  1066,     0,
     408,   408,   144,     0,     0,   755,   284,   151,     0,     0,
       0,   151,   144,     0,     0,     0,   921,     0,   344,   151,
    1088,  1089,   345,     0,   323,   144,     0,   144,     0,     0,
       0,     0,     0,  1063,  1064,  1065,  1066,  1088,  1089,   409,
     409,     0,   133,     0,   784,     0,     0,     0,     0,     0,
     709,  1094,   672,     0,  1097,     0,   133,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
     348,     0,     0,   144,   351,     0,   352,   353,     0,     0,
       0,   356,     0,     0,     0,     5,     0,   672,     0,   363,
       0,   921,     0,     0,     0,   367,   368,   369,   139,    15,
     144,     0,    17,     0,   144,     0,     0,    21,     0,   254,
     254,     0,     0,     0,     0,     0,     0,   649,     0,     0,
       0,   254,     0,   440,   409,     0,    37,   133,   151,     0,
     440,     0,    41,   254,     0,     0,   254,     0,     0,   649,
      46,     0,   317,     0,   317,     0,     0,    51,     0,   649,
     139,   425,   428,   430,   434,   133,   139,     0,     0,     0,
       0,   133,    61,     0,     0,     0,     0,   144,   133,     0,
       0,     0,     0,     0,    71,   914,     0,     0,    74,   856,
     151,     0,     0,     0,     0,    80,   151,     0,     0,     0,
       0,    84,     0,   860,   861,   458,     0,     0,     0,     0,
       0,   461,     0,     0,     0,     0,     0,     0,   343,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   139,     0,
     139,     0,     0,   133,   471,   139,     0,   133,   139,     0,
       0,     0,     0,     0,   139,   133,   723,   139,   409,     0,
       0,     0,   482,     0,     0,     0,   144,     0,   151,     0,
     151,     0,     0,   144,     0,   151,     0,     0,   151,   344,
     148,   139,     0,   345,   151,     0,     0,   151,     0,     0,
     148,     0,   139,     0,   139,     0,     0,     0,     0,     0,
       0,     0,     0,   148,     0,   148,     0,     0,     0,     0,
       0,   151,     0,     0,     0,     0,     0,     0,     0,     0,
     254,     0,   151,     0,   151,     0,     0,     0,     0,     0,
     347,   348,     0,   144,   350,   351,     0,   352,   353,     0,
       0,     0,   356,     0,     0,     0,     0,   144,     0,     0,
     363,   148,     0,     0,     0,     0,   367,   368,   369,     0,
     579,     0,     0,     0,   133,     0,     0,     0,   343,   440,
     440,     0,     0,   440,   440,     0,     0,     0,   148,     0,
     139,     0,   148,   149,     0,     0,     0,   139,     0,  1035,
     601,     0,     0,   149,   606,     0,     0,  1041,   139,   440,
       0,   440,     0,     0,     0,     0,   149,     0,   149,     0,
     151,     0,   139,     0,     0,     0,   133,   151,   144,   344,
       0,   139,   133,   345,     0,   139,     0,     0,   151,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   151,     0,     0,   148,   144,     0,     0,     0,
       0,   151,   144,     0,   149,   151,     0,     0,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   139,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
       0,   149,   356,     0,   133,   149,   133,     0,     0,   362,
     363,   133,     0,   366,   133,     0,   367,   368,   369,   151,
     133,     0,  1035,   133,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   144,     0,     0,     0,   144,     0,
     139,   139,   139,   139,   148,     0,   144,   133,     0,     0,
       0,   148,     0,     0,     0,     0,     0,     0,   133,     0,
     133,     0,     0,     0,     0,   139,   139,     0,   149,     0,
     151,   151,   151,   151,     0,     0,     0,   736,   738,     0,
       0,     0,     0,   743,   746,     0,     0,     0,     0,     0,
       0,   972,     0,     0,     0,   151,   151,     0,     0,   409,
     409,     0,   268,     0,   153,     0,    22,    23,     0,     0,
       0,   148,     0,     0,   153,   269,     0,    30,   270,     0,
       0,     0,    35,     0,     0,   148,     0,   153,    40,   153,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,     0,     0,   133,   149,     0,   105,
      53,     0,    55,   133,   149,   144,     0,     0,     0,   164,
       0,   271,     0,    63,   133,     0,     0,  1026,     0,     0,
       0,     0,   196,     0,   199,   153,     0,     0,   133,     0,
       0,     0,    79,     0,     0,    81,     0,   133,    83,     0,
       0,   133,     0,     0,   344,     0,   148,     0,   345,   870,
     871,     0,   153,     0,     0,     0,   153,   144,     0,     0,
       0,   876,     0,   144,   149,     0,  1069,     0,     0,     0,
     290,     0,     0,   887,   148,     0,   890,     0,   149,     0,
     148,     0,     0,     0,     0,   133,     0,   148,     0,     0,
       0,     0,     0,     0,     0,   347,   348,   105,   349,   350,
     351,   326,   352,   353,   354,   343,     0,   356,   357,   358,
       0,     0,   360,   361,   362,   363,  1069,     0,   366,   153,
       0,   367,   368,   369,     0,   144,     0,   144,     0,     0,
       0,     0,   144,     0,     0,   144,   133,   133,   133,   133,
       0,   144,   148,     0,   144,     0,   148,     0,     0,   149,
       0,     0,     0,   154,   148,  1069,   344,     0,     0,     0,
     345,   133,   133,   154,   392,     0,     0,     0,   144,     0,
       0,     0,     0,     0,     0,     0,   154,   149,   154,   144,
       0,   144,     0,   149,     0,     0,     0,     0,     0,     0,
     149,     0,     0,     0,     0,     0,     0,     0,   153,     0,
       0,     0,     0,     0,     0,   153,     0,   347,   348,     0,
     349,   350,   351,     0,   352,   353,     0,     0,     0,   356,
       0,     0,     0,     0,   154,     0,     0,   363,     0,     0,
       0,     0,     0,   367,   368,   369,     0,     0,     0,   222,
     987,     0,     0,   444,     0,   149,     0,     0,     0,   149,
     457,   154,     0,   223,   224,   154,   225,   149,     0,     0,
       0,   226,     0,   148,     0,   153,     0,   144,     0,   227,
       0,     0,     0,     0,   144,   228,     0,     0,     0,   153,
     229,     0,     0,   230,     0,   144,   231,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,   144,
     233,   234,     0,     0,     0,     0,     0,   235,   144,     0,
     105,     0,   144,     0,     0,   148,   236,     0,   154,     0,
       0,   148,     0,     0,   105,   237,   238,     0,   239,     0,
     240,   246,   241,     0,     0,   242,     0,     0,     0,   243,
     447,     0,   244,     0,     0,   245,     0,     0,     0,     0,
     153,     0,     0,     0,     0,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   149,     0,   300,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   153,     0,
       0,     0,     0,   148,   153,   148,     0,     0,     0,     0,
     148,   153,     0,   148,     0,   105,     0,   154,     0,   148,
       0,   448,   148,     0,   154,     0,     0,   144,   144,   144,
     144,     0,     0,     0,     0,     0,     0,     0,   149,   331,
       0,   332,     0,   457,   149,     0,   148,     0,     0,   457,
       0,     0,   144,   144,     0,     0,     0,   148,     0,   148,
       0,     0,     0,     0,     0,     0,   153,     0,     0,     0,
     153,     0,     0,     0,     0,     0,     0,     0,   153,     0,
       0,     0,     0,     0,   154,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   154,     0,
       0,   300,     0,     0,     0,     0,   149,     0,   149,     0,
       0,   666,     0,   149,     0,   671,   149,   300,     0,   423,
       0,     0,   149,   105,     0,   149,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   383,
     384,     0,     0,     0,     0,   148,     0,     0,     0,   149,
       0,     0,   148,     0,     0,   799,     0,  -507,   449,     0,
     149,     0,   149,   148,     0,     0,     0,     0,     0,   154,
     385,     0,     0,  -545,     0,  -545,  -224,   148,     0,     0,
    -224,  -224,     0,   464,     0,     0,   148,   153,     0,  -224,
     148,  -224,  -224,     0,     0,     0,  -224,   154,     0,     0,
       0,     0,  -224,   154,     0,  -224,     0,     0,     0,     0,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -224,     0,  -224,     0,  -224,     0,
    -224,  -224,   753,  -224,   148,  -224,     0,  -224,     0,   153,
       0,     0,     0,     0,     0,   153,     0,     0,   149,     0,
       0,     0,     0,     0,     0,   149,  -224,     0,     0,  -224,
       0,     0,  -224,     0,     0,   154,   149,     0,     0,   154,
       0,     0,     0,     0,     0,     0,     0,   154,     0,     0,
     149,     0,     0,     0,   788,   148,   148,   148,   148,   149,
     793,     0,     0,   149,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   153,     0,   153,
     148,   148,  -224,     0,   153,     0,     0,   153,  -224,     0,
       0,     0,     0,   153,   588,     0,   153,     0,   591,     0,
       0,     0,     0,     0,     0,     0,     0,   149,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     153,     0,   457,     0,   457,     0,     0,   449,     0,   457,
       0,   153,   457,   153,     0,     0,     0,     0,   892,     0,
       0,   894,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   154,     0,   149,   149,
     149,   149,     0,     0,     0,   910,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   916,     0,   920,     0,
       0,   222,     0,   149,   149,     0,   679,     0,     0,     0,
       0,   688,     0,     0,     0,   223,   224,     0,   225,     0,
       0,     0,     0,   226,     0,     0,     0,     0,   154,     0,
       0,   227,     0,     0,   154,     0,     0,   228,     0,   153,
       0,     0,   229,     0,     0,   230,   153,     0,   231,     0,
       0,     0,     0,     0,     0,     0,   232,   153,     0,     0,
       0,     0,   233,   234,     0,     0,     0,     0,     0,   235,
       0,   153,     0,     0,     0,     0,     0,     0,   236,     0,
     153,     0,     0,     0,   153,     0,   726,   237,   238,   729,
     239,     0,   240,     0,   241,     0,   154,   242,   154,     0,
       0,   243,   967,   154,   244,     0,   154,   245,     0,     0,
       0,     0,   154,     0,     0,   154,   986,     0,     0,     0,
       0,     0,     0,   222,     0,   991,     0,   762,   153,   993,
       0,   449,     0,     0,     0,     0,     0,   223,   224,   154,
     225,     0,     0,     0,     0,   226,     0,     0,     0,     0,
     154,     0,   154,   227,   785,     0,     0,     0,   300,   228,
       0,   398,     0,   783,   229,     0,     0,   230,     0,     0,
     231,     0,     0,  1021,     0,     0,     0,     0,   232,   153,
     153,   153,   153,     0,   233,   234,     0,     0,     0,     0,
       0,   235,     0,     0,     0,   851,     0,     0,     0,     0,
     236,     0,     0,   222,   153,   153,     0,     0,     0,   237,
     238,     0,   239,     0,   240,     0,   241,   223,   224,   242,
     225,     0,     0,   243,     0,   226,   244,     0,     0,   245,
       0,     0,     0,   227,     0,     0,     0,     0,   154,   228,
       0,     0,     0,     0,   229,   154,     0,   230,     0,     0,
     231,     0,     0,     0,     0,     0,   154,   896,   232,     0,
     449,     0,     0,     0,   233,   234,     0,     0,     0,     0,
     154,   235,     0,     0,     0,     0,     0,     0,     0,   154,
     236,     0,   785,   154,     0,   249,     0,     0,     0,   237,
     238,     0,   239,     0,   240,     0,   241,     0,     0,   242,
       0,     0,   688,   243,     0,     0,   244,     0,     0,   245,
       0,     0,     0,     0,     0,     0,   268,     0,     0,     0,
      22,    23,     0,     0,     0,     0,     0,   154,   940,   269,
       0,    30,   270,     0,     0,     0,    35,     0,     0,     0,
       0,     0,    40,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   383,   384,     0,     0,
       0,     0,     0,     0,    53,   761,    55,     0,    57,     0,
     930,     0,     0,   931,     0,   271,     0,    63,   154,   154,
     154,   154,     0,     0,     0,     0,     0,   385,     0,     0,
    -545,     0,  -545,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,   154,   154,     0,    -2,     4,     0,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,   851,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    98,    39,    40,     0,    41,  -238,   932,    42,
      43,    44,     0,    45,    46,    47,   -44,    48,    49,     0,
      50,    51,    52,     0,     0,     0,    53,    54,    55,    56,
      57,    58,    59,  -238,   -44,    60,    61,    62,     0,    63,
      64,    65,     0,    66,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
       0,    81,    82,   -62,    83,    84,     0,     0,    85,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
       0,   101,  1090,   102,     5,   273,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,   274,     0,
       0,     0,    99,     0,   179,     0,   101,     0,   180,  1091,
       4,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,  -573,     0,    12,    13,    14,    15,    16,  -573,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    -573,    27,    28,  -573,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,     0,    39,    40,     0,    41,
    -238,     0,    42,    43,    44,     0,    45,    46,    47,   -44,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,     0,    58,    59,  -238,   -44,    60,    61,
      62,  -573,    63,    64,    65,  -573,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -62,    83,    84,     0,
       0,    85,     0,    86,     0,     0,  -573,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -573,  -573,    90,  -573,  -573,  -573,  -573,  -573,
    -573,  -573,     0,  -573,  -573,  -573,  -573,  -573,     0,  -573,
    -573,  -573,  -573,  -573,  -573,  -573,  -573,    98,  -573,  -573,
    -573,     0,   100,  -573,   101,   310,   102,     5,   273,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
       0,    39,    40,     0,    41,  -238,     0,    42,    43,    44,
       0,    45,    46,    47,   -44,    48,    49,     0,    50,    51,
      52,     0,     0,     0,    53,    54,    55,    56,    57,    58,
      59,  -238,   -44,    60,    61,    62,     0,    63,    64,    65,
       0,    66,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,     0,    81,
      82,   -62,    83,    84,     0,     0,    85,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,   274,    98,     0,     0,    99,     0,   100,   311,   101,
       4,   102,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,     0,    39,    40,     0,    41,
    -238,     0,    42,    43,    44,     0,    45,    46,    47,   -44,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,    57,    58,    59,  -238,   -44,    60,    61,
      62,     0,    63,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -62,    83,    84,     0,
       0,    85,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,   485,   101,   504,   102,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
       0,    39,    40,     0,    41,  -238,     0,    42,    43,    44,
       0,    45,    46,    47,   -44,    48,    49,     0,    50,    51,
      52,     0,     0,     0,    53,    54,    55,    56,    57,    58,
      59,  -238,   -44,    60,    61,    62,     0,    63,    64,    65,
       0,    66,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,     0,    81,
      82,   -62,    83,    84,     0,     0,    85,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,     0,   100,   505,   101,
     310,   102,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,     0,    39,    40,     0,    41,
    -238,     0,    42,    43,    44,     0,    45,    46,    47,   -44,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,    57,    58,    59,  -238,   -44,    60,    61,
      62,     0,    63,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -62,    83,    84,     0,
       0,    85,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,   311,   101,     4,   102,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
       0,    39,    40,     0,    41,  -238,     0,    42,    43,    44,
       0,    45,    46,    47,   -44,    48,    49,     0,    50,    51,
      52,     0,     0,     0,    53,    54,    55,    56,    57,    58,
      59,  -238,   -44,    60,    61,    62,     0,    63,    64,    65,
       0,    66,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,     0,    81,
      82,   -62,    83,    84,     0,     0,    85,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,     0,   100,   678,   101,
       4,   102,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,     0,    39,    40,     0,    41,
    -238,     0,    42,    43,    44,     0,    45,    46,    47,   -44,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,   325,    58,    59,  -238,   -44,    60,    61,
      62,     0,    63,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -62,    83,    84,     0,
       0,    85,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,     4,   101,     5,   102,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,    38,     0,    39,
      40,     0,    41,  -238,     0,    42,    43,    44,     0,    45,
      46,    47,   -44,    48,    49,     0,    50,    51,    52,     0,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -238,
     -44,    60,    61,    62,     0,    63,    64,    65,     0,    66,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,     0,    81,    82,   -62,
      83,    84,     0,     0,    85,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
      98,     0,     0,    99,     0,   100,     4,   101,     5,   102,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,   596,    37,
      38,     0,    39,    40,     0,    41,  -238,     0,    42,    43,
      44,     0,    45,    46,    47,   -44,    48,    49,     0,    50,
      51,    52,     0,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -238,   -44,    60,    61,    62,     0,    63,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,     0,
      81,    82,   -62,    83,    84,     0,     0,    85,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,     4,
     101,     5,   102,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   602,    37,    38,     0,    39,    40,     0,    41,  -238,
       0,    42,    43,    44,     0,    45,    46,    47,   -44,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,     0,    58,    59,  -238,   -44,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -62,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,     4,   101,     5,   102,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   878,    37,    38,     0,    39,    40,
       0,    41,  -238,     0,    42,    43,    44,     0,    45,    46,
      47,   -44,    48,    49,     0,    50,    51,    52,     0,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -238,   -44,
      60,    61,    62,     0,    63,    64,    65,     0,    66,    67,
      68,    69,    70,    71,    72,    73,     0,    74,    75,     0,
      76,    77,    78,    79,    80,     0,    81,    82,   -62,    83,
      84,     0,     0,    85,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,    98,
       0,     0,    99,     0,   100,     4,   101,     5,   102,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   880,    37,    38,
       0,    39,    40,     0,    41,  -238,     0,    42,    43,    44,
       0,    45,    46,    47,   -44,    48,    49,     0,    50,    51,
      52,     0,     0,     0,    53,    54,    55,    56,     0,    58,
      59,  -238,   -44,    60,    61,    62,     0,    63,    64,    65,
       0,    66,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,     0,    81,
      82,   -62,    83,    84,     0,     0,    85,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,     0,   100,     4,   101,
       5,   102,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
     885,    37,    38,     0,    39,    40,     0,    41,  -238,     0,
      42,    43,    44,     0,    45,    46,    47,   -44,    48,    49,
       0,    50,    51,    52,     0,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -238,   -44,    60,    61,    62,     0,
      63,    64,    65,     0,    66,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,     0,    81,    82,   -62,    83,    84,     0,     0,    85,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,     4,   101,     5,   102,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   888,    37,    38,     0,    39,    40,     0,
      41,  -238,     0,    42,    43,    44,     0,    45,    46,    47,
     -44,    48,    49,     0,    50,    51,    52,     0,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -238,   -44,    60,
      61,    62,     0,    63,    64,    65,     0,    66,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,     0,    81,    82,   -62,    83,    84,
       0,     0,    85,     0,    86,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,    98,     0,
       0,    99,     0,   100,     4,   101,     5,   102,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,   909,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,     0,
      39,    40,     0,    41,  -238,     0,    42,    43,    44,     0,
      45,    46,    47,   -44,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,     0,    58,    59,
    -238,   -44,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -62,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,     4,   101,     5,
     102,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,   915,
      37,    38,     0,    39,    40,     0,    41,  -238,     0,    42,
      43,    44,     0,    45,    46,    47,   -44,    48,    49,     0,
      50,    51,    52,     0,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -238,   -44,    60,    61,    62,     0,    63,
      64,    65,     0,    66,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
       0,    81,    82,   -62,    83,    84,     0,     0,    85,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
       4,   101,     5,   102,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,   919,    37,    38,     0,    39,    40,     0,    41,
    -238,     0,    42,    43,    44,     0,    45,    46,    47,   -44,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,     0,    58,    59,  -238,   -44,    60,    61,
      62,     0,    63,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -62,    83,    84,     0,
       0,    85,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,     0,   101,   771,   102,     5,   273,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   177,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   178,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,   274,     0,     0,     0,    99,     0,   179,     0,   101,
       0,   180,     5,   273,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   171,     0,     0,    15,    16,     0,    17,
       0,   172,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   173,     0,     0,     0,    32,   174,
     175,     0,   176,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     177,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   178,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,   274,     0,     0,     0,
      99,     0,   179,     0,   101,     0,   180,   670,     5,   273,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   171,
       0,     0,    15,    16,     0,    17,     0,   172,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     173,     0,     0,     0,    32,   174,   175,     0,   176,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   177,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   178,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   274,     0,     0,     0,    99,     0,   179,     0,
     101,     0,   180,  1024,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   179,     0,   101,     0,   180,   675,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,   216,    23,     0,     0,     0,     0,     0,
      28,     0,   173,     0,     0,     0,    32,   174,   175,     0,
     176,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
      63,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,    83,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     179,     0,   101,     0,   180,     5,   273,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   173,     0,     0,
       0,    32,   174,   175,     0,   176,    37,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   177,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   178,     0,    74,     0,
       0,    76,   278,   279,     0,    80,   315,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,   316,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,   274,
       0,     0,     0,    99,     0,   179,     0,   101,     0,   180,
       5,   273,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   173,     0,     0,     0,    32,   174,   175,     0,
     176,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,   278,   279,     0,
      80,   315,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,   274,     0,     0,     0,    99,     0,
     179,     0,   101,   662,   180,     5,   273,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   173,     0,     0,
       0,    32,   174,   175,     0,   176,    37,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   177,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   178,     0,    74,     0,
       0,    76,   278,   279,     0,    80,   315,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,   274,
       0,     0,     0,    99,     0,   179,     0,   101,   664,   180,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,     0,   561,     0,     0,     0,     0,     0,
      28,     0,   173,     0,     0,     0,    32,   174,   175,     0,
     176,    37,     0,   562,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
     563,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,   564,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     179,     0,   101,     5,   180,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   177,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   178,     0,    74,     0,     0,    76,
     278,   279,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,   280,     0,
       0,    99,     0,   179,     0,   101,     0,   180,     5,   273,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   171,
       0,     0,    15,    16,     0,    17,     0,   172,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     173,     0,     0,     0,    32,   174,   175,     0,   176,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   177,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   178,
       0,    74,     0,     0,    76,   278,   279,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   274,     0,     0,     0,    99,     0,   179,     0,
     101,     5,   180,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,   773,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   177,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   178,     0,    74,     0,     0,    76,   278,   279,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   179,     0,   101,     5,   180,     6,     7,     8,   205,
      10,    11,   206,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   207,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,     0,     0,   208,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   179,     0,   101,     5,   180,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,   210,    56,     0,    58,
       0,     0,     0,     0,    61,   177,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   178,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,   211,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   179,     0,   101,
       0,   180,     5,   273,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   171,     0,     0,    15,    16,     0,    17,
       0,   172,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   173,     0,     0,     0,    32,   174,
     175,     0,   176,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     177,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   178,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,   211,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,   274,     0,     0,     0,
      99,     0,   179,     0,   101,     5,   180,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   173,     0,     0,
       0,    32,   174,   175,     0,   176,    37,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   177,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   178,     0,    74,     0,
       0,    76,   278,   279,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   179,     0,   101,     0,   180,
       5,   273,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   173,     0,     0,     0,    32,   174,   175,     0,
     176,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   807,     0,   808,
      88,    89,    90,    91,    92,     0,     0,     0,   809,    93,
       0,     0,     0,   810,   224,   811,   812,     0,     0,    94,
       0,   813,    95,    96,   274,     0,     0,     0,    99,   227,
     179,   660,   101,     0,   180,   814,     0,     0,     0,     0,
     815,     0,     0,   230,     0,     0,   816,     0,   817,     0,
       0,     0,     0,     0,   818,     0,     0,     0,     0,     0,
     819,   820,     0,     0,     0,     0,     0,   235,     0,     0,
       0,     0,     0,     0,     0,     0,   821,     0,     0,     0,
       0,     0,     0,     0,     0,   237,   238,     0,   822,     0,
     240,     0,   823,     0,     0,   824,     0,     0,     0,   825,
       0,     0,   244,     0,     0,   826,     0,     0,     0,     0,
       0,     0,     0,     0,   370,   371,   372,   373,   374,   375,
       0,     0,   378,   379,   380,   381,     0,   383,   384,   827,
     828,   829,   830,   831,     0,     0,   832,     0,     0,     0,
     833,   834,   835,   836,   837,   838,   839,   840,   841,   842,
     843,     0,   844,     0,     0,   845,   846,   847,   848,     0,
       5,   849,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   173,     0,     0,     0,    32,   174,   175,     0,
     176,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,   211,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     179,     0,   101,     5,   180,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,   253,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   177,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   178,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   100,     0,   101,     0,   180,     5,   273,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   171,
       0,     0,    15,    16,     0,    17,     0,   172,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     173,     0,     0,     0,    32,   174,   175,     0,   176,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   177,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   178,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   274,     0,     0,     0,    99,     0,   179,     0,
     101,     5,   180,     6,     7,     8,   205,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   207,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   178,     0,    74,     0,     0,    76,     0,     0,
     208,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   179,     0,   101,     0,   180,     5,   273,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   171,     0,     0,
      15,    16,     0,    17,     0,   172,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,   634,     0,   173,     0,
       0,     0,    32,   174,   175,     0,   176,    37,     0,     0,
     635,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   177,     0,     0,     0,     0,     0,
       0,     0,    68,   636,    70,    71,    72,   637,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
     274,     0,     0,     0,    99,     0,   179,     0,   101,     0,
     970,     5,   273,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   177,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   178,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,   274,     0,     0,     0,    99,
       0,   179,     0,   101,     0,   970,     5,   273,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   171,     0,     0,
      15,    16,     0,    17,     0,   172,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   173,     0,
       0,     0,    32,   174,   175,     0,   954,    37,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   177,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   178,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
     274,     0,     0,     0,    99,     0,   179,     0,   101,     5,
     180,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   179,
       0,   101,     5,   180,     6,     7,     8,   201,    10,    11,
       0,     0,     0,   171,     0,     0,    15,    16,     0,    17,
       0,   172,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   173,     0,     0,     0,    32,   174,
     175,     0,   176,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     202,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   178,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   179,     0,   101,     5,   180,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   173,     0,     0,
       0,    32,   174,   175,     0,   176,    37,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   177,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   178,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   100,     0,   101,     5,   180,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   171,
       0,     0,    15,    16,     0,    17,     0,   172,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     173,     0,     0,     0,    32,   174,   175,     0,   176,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   177,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   178,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,   607,     0,   179,     0,
     101,     5,   180,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,   634,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,   635,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   177,
       0,     0,     0,     0,     0,     0,     0,    68,   636,    70,
      71,    72,   637,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   179,     0,   101,     5,   638,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   179,     0,   101,     5,   638,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   712,    37,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   177,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   178,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   179,     0,   101,
       5,   180,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   173,     0,     0,     0,    32,   174,   175,     0,
     715,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     179,     0,   101,     5,   180,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   948,    37,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   177,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   178,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   179,     0,   101,     5,   180,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   171,     0,     0,
      15,    16,     0,    17,     0,   172,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   173,     0,
       0,     0,    32,   174,   175,     0,   949,    37,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   177,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   178,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   179,     0,   101,     5,
     180,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   951,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   179,
       0,   101,     5,   180,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   171,     0,     0,    15,    16,     0,    17,
       0,   172,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   173,     0,     0,     0,    32,   174,
     175,     0,   952,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     177,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   178,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   179,     0,   101,     5,   180,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   173,     0,     0,
       0,    32,   174,   175,     0,   953,    37,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   177,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   178,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   179,     0,   101,     5,   180,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   171,
       0,     0,    15,    16,     0,    17,     0,   172,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     173,     0,     0,     0,    32,   174,   175,     0,   954,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   177,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   178,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   179,     0,
     101,     5,   180,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,   634,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,   635,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   177,
       0,     0,     0,     0,     0,     0,     0,    68,   636,    70,
      71,    72,   637,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   179,     0,   101,     5,  1046,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,     0,     0,   101,     5,  1046,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   177,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   178,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,     0,
      96,    97,     0,     0,     0,    99,     0,   179,     0,   101,
       0,   180,     5,   273,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   171,     0,     0,    15,    16,     0,    17,
       0,   172,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   173,     0,     0,     0,    32,   174,
     175,     0,   176,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     177,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   178,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,  -403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,  -403,     0,
       0,     0,  -403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,     0,     0,     0,
       0,     0,   179,     0,   101,  -403,   970,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
    -385,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   177,     0,     0,     0,  -385,
       0,     0,     0,    68,    69,    70,    71,    72,   178,     0,
      74,     0,  -385,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,  -385,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,    90,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   171,
       0,     0,    15,    16,     0,    17,     0,   172,     0,     0,
      21,    97,  -385,     0,     0,     0,     0,  -385,    28,   101,
     173,  1046,     0,     0,    32,   174,   175,     0,   176,    37,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   177,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   178,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     5,
      90,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,    97,     0,     0,     0,     0,     0,   179,    28,
     101,   173,   180,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,    90,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,    97,     0,     0,     0,     0,     0,   179,
      28,   101,   173,   638,     0,     0,    32,   174,   175,     0,
     176,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,  1003,    76,     0,     0,     0,
      80,     0,     0,    82,   809,     0,    84,     0,     0,   223,
     224,   811,   225,     0,     0,     0,     0,   226,     0,     0,
       0,     0,     0,     0,     0,   227,     0,     0,     0,     0,
       0,   814,    90,     0,     0,     0,   229,     0,     0,   230,
       0,     0,   231,     0,   817,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,     0,   819,   234,     0,     0,
     341,     0,   101,   235,   180,     0,     0,   342,     0,     0,
       0,     0,   236,     0,     0,     0,     0,     0,     0,   343,
       0,   237,   238,     0,   239,     0,   240,     0,  1004,     0,
       0,   824,     0,     0,     0,   243,     0,     0,   244,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
     370,   371,   372,   373,   374,   375,     0,     0,   378,   379,
     380,   381,     0,   383,   384,   827,   828,   829,   830,   831,
     344,     0,   832,     0,   345,     0,   833,   834,   835,   836,
     837,   838,   839,   840,   841,   842,   843,     0,   844,   341,
       0,   845,   846,   847,   848,     0,   342,     0,     0,     0,
       0,   429,     0,     0,     0,   346,     0,     0,   343,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,   341,   367,   368,   369,
       0,     0,     0,   342,     0,     0,   791,     0,     0,   344,
       0,     0,     0,   345,     0,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   341,     0,     0,     0,
       0,     0,     0,   342,   346,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,     0,     0,     0,     0,
     347,   348,     0,   349,   350,   351,   344,   352,   353,   354,
     345,   355,   356,   357,   358,   359,     0,   360,   361,   362,
     363,   364,   365,   366,     0,   341,   367,   368,   369,     0,
       0,     0,   342,     0,     0,   869,     0,     0,     0,     0,
       0,   346,     0,     0,   343,     0,   344,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,   358,   359,     0,   360,   361,   362,   363,   364,   365,
     366,   346,     0,   367,   368,   369,     0,     0,     0,     0,
       0,     0,  1038,     0,     0,   344,     0,   347,   348,   345,
     349,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,   358,   359,     0,   360,   361,   362,   363,   364,   365,
     366,   341,     0,   367,   368,   369,     0,     0,   342,     0,
     346,     0,  1113,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,     0,     0,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
       0,     0,   367,   368,   369,     0,     0,     0,   341,   661,
       0,     0,     0,     0,     0,   342,     0,     0,     0,     0,
       0,   344,     0,     0,     0,   345,     0,   343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   341,     0,
       0,     0,     0,     0,     0,   342,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,     0,     0,
       0,     0,   347,   348,     0,   349,   350,   351,   344,   352,
     353,   354,   345,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,     0,     0,   367,   368,
     369,     0,     0,   343,     0,   775,     0,     0,     0,     0,
       0,     0,     0,   346,     0,     0,     0,     0,   344,     0,
       0,     0,   345,     0,     0,     0,     0,     0,     0,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,     0,
     355,   356,   357,   358,   359,     0,   360,   361,   362,   363,
     364,   365,   366,   346,   344,   367,   368,   369,   345,     0,
     341,     0,   776,     0,     0,     0,     0,   342,     0,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,   343,
     355,   356,   357,   358,   359,     0,   360,   361,   362,   363,
     364,   365,   366,     0,     0,   367,   368,   369,     0,     0,
       0,     0,  1002,     0,     0,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,   358,
       0,     0,   360,   361,   362,   363,   364,     0,   366,     0,
     344,   367,   368,   369,   345,     0,     0,     0,     0,     0,
     341,     0,     0,   439,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,   346,   370,   371,   372,   373,
     374,   375,     0,     0,   378,   379,   380,   381,     0,   383,
     384,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
     344,   398,     0,     0,   345,     0,     0,     0,     0,     0,
     341,     0,     0,   877,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,   346,   370,   371,   372,   373,
     374,   375,     0,     0,   378,   379,   380,   381,     0,   383,
     384,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
     344,   398,     0,     0,   345,     0,     0,     0,     0,     0,
     341,     0,     0,   884,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,   346,   370,   371,   372,   373,
     374,   375,     0,     0,   378,   379,   380,   381,     0,   383,
     384,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
     344,   398,     0,     0,   345,     0,     0,     0,     0,     0,
     341,     0,     0,  1017,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,   346,   370,   371,   372,   373,
     374,   375,     0,     0,   378,   379,   380,   381,     0,   383,
     384,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
     344,   398,     0,     0,   345,     0,     0,     0,     0,     0,
     341,     0,     0,  1018,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,   346,   370,   371,   372,   373,
     374,   375,     0,     0,   378,   379,   380,   381,     0,   383,
     384,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
     344,   398,     0,     0,   345,     0,     0,     0,     0,     0,
     341,     0,     0,  1019,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,   346,   370,   371,   372,   373,
     374,   375,     0,     0,   378,   379,   380,   381,     0,   383,
     384,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
     344,   398,     0,     0,   345,     0,     0,     0,     0,     0,
     341,     0,     0,  1020,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,   346,   370,   371,   372,   373,
     374,   375,     0,     0,   378,   379,   380,   381,     0,   383,
     384,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
     344,   398,     0,     0,   345,     0,     0,     0,     0,     0,
     341,     0,     0,  1042,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,   346,   370,   371,   372,   373,
     374,   375,     0,     0,   378,   379,   380,   381,     0,   383,
     384,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
     344,   398,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,  1043,   341,     0,     0,     0,     0,     0,
       0,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,   432,   346,   370,   371,   372,   373,
     374,   375,     0,     0,   378,   379,   380,   381,   433,   383,
     384,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
       0,   398,     0,     0,   344,     0,     0,     0,   345,     0,
     341,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     426,     0,     0,     0,     0,   429,     0,     0,     0,   346,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,   358,
     359,     0,   360,   361,   362,   363,   364,   365,   366,     0,
     344,   367,   368,   369,   345,   398,   341,     0,     0,     0,
       0,     0,     0,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,   745,     0,     0,     0,
       0,     0,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,   344,   367,   368,   369,
     345,   398,   341,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,   253,     0,     0,     0,     0,   429,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,   358,   359,     0,   360,   361,   362,   363,   364,   365,
     366,     0,   344,   367,   368,   369,   345,   398,   341,     0,
       0,     0,     0,     0,     0,   342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,   737,     0,
       0,     0,     0,     0,     0,     0,     0,   346,     0,     0,
     341,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,   347,   348,     0,   349,   350,   351,   343,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,   344,   367,
     368,   369,   345,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   341,     0,     0,     0,     0,     0,     0,
     342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     344,     0,   343,   346,   345,     0,     0,     0,     0,     0,
       0,     0,     0,   754,     0,     0,     0,     0,     0,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,     0,
     355,   356,   357,   358,   359,   346,   360,   361,   362,   363,
     364,   365,   366,     0,     0,   367,   368,   369,     0,   398,
       0,   347,   348,   344,   349,   350,   351,   345,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
       0,   398,   341,     0,     0,     0,     0,     0,   346,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,   347,   348,   436,   349,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,     0,     0,
     367,   368,   369,     0,   470,   341,   682,   437,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   344,     0,   343,     0,   345,     0,     0,   683,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   346,     0,     0,
     684,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,   348,   344,   349,   350,   351,   345,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,   438,   367,
     368,   369,   341,     0,     0,     0,     0,     0,     0,   342,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,   400,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
       0,   341,   367,   368,   369,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,   570,   344,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,   571,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   166,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,     0,   347,   348,   345,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,   341,   367,
     368,   369,     0,     0,     0,   342,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,   572,     0,
       0,     0,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,   573,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,     0,   341,   367,   368,
     369,     0,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,   344,     0,
       0,   925,   345,     0,     0,     0,     0,     0,     0,   341,
       0,     0,     0,     0,     0,     0,   342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,     0,
       0,     0,   926,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,   347,
     348,   345,   349,   350,   351,     0,   352,   353,   354,     0,
     355,   356,   357,   358,   359,     0,   360,   361,   362,   363,
     364,   365,   366,     0,     0,   367,   368,   369,     0,   344,
       0,     0,   346,   345,     0,     0,     0,     0,     0,   341,
       0,     0,     0,     0,     0,     0,   342,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   343,   355,
     356,   357,   358,   359,   346,   360,   361,   362,   363,   364,
     365,   366,     0,     0,   367,   368,   369,     0,     0,     0,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,   358,   359,     0,   360,   361,   362,
     363,   364,   365,   366,     0,   474,   367,   368,   369,   344,
       0,     0,     0,   345,     0,     0,     0,     0,     0,   341,
       0,     0,     0,     0,     0,     0,   342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,     0,
       0,     0,     0,     0,   346,     0,     0,     0,     0,     0,
       0,   341,     0,     0,     0,     0,     0,     0,   342,     0,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     343,   355,   356,   357,   358,   359,     0,   360,   361,   362,
     363,   364,   365,   366,     0,   476,   367,   368,   369,   344,
       0,     0,     0,   345,     0,     0,     0,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,   344,     0,     0,   346,   345,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,   358,   359,   346,   360,   361,   362,
     363,   364,   365,   366,     0,   478,   367,   368,   369,     0,
     344,     0,   347,   348,   345,   349,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,   341,   483,   367,   368,
     369,     0,     0,   342,   500,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,     0,     0,     0,     0,
       0,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,   341,     0,   367,   368,   369,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,   344,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,     0,     0,   574,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,   344,     0,   347,   348,   345,
     349,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,   358,   359,     0,   360,   361,   362,   363,   364,   365,
     366,     0,     0,   367,   368,   369,     0,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     344,     0,     0,     0,   345,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     358,   359,  -508,   360,   361,   362,   363,   364,   365,   366,
     341,   166,   367,   368,   369,   346,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,   341,   749,   367,   368,   369,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,     0,     0,     0,     0,
     344,     0,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   341,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,   346,     0,   752,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,     0,   345,
       0,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,   722,   367,   368,   369,
     346,     0,     0,     0,     0,   344,     0,     0,     0,   345,
       0,     0,     0,     0,     0,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
     346,     0,   367,   368,   369,     0,     0,   341,     0,     0,
       0,     0,     0,     0,   342,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,   343,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
       0,     0,   367,   368,   369,     0,     0,   341,     0,     0,
       0,     0,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   864,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,   345,     0,     0,     0,     0,     0,   341,     0,     0,
       0,     0,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   867,     0,     0,
       0,   794,   346,     0,     0,     0,     0,   344,     0,     0,
       0,   345,     0,     0,     0,     0,     0,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,     0,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,   346,     0,   367,   368,   369,   344,     0,     0,
       0,   345,     0,     0,     0,     0,     0,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,     0,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,   346,     0,   367,   368,   369,     0,     0,   341,
     924,     0,     0,     0,     0,     0,   342,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   343,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,     0,     0,   367,   368,   369,     0,     0,   341,
       0,     0,     0,     0,     0,     0,   342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
       0,     0,     0,   345,     0,     0,     0,     0,     0,   341,
       0,     0,     0,     0,     0,     0,   342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,     0,
       0,     0,     0,     0,   346,     0,     0,     0,     0,   344,
       0,     0,     0,   345,     0,     0,     0,     0,     0,     0,
     347,   348,   947,   349,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,   358,   359,     0,   360,   361,   362,
     363,   364,   365,   366,   346,     0,   367,   368,   369,   344,
       0,     0,     0,   345,     0,     0,     0,     0,     0,     0,
     347,   348,   950,   349,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,   358,   359,     0,   360,   361,   362,
     363,   364,   365,   366,   346,     0,   367,   368,   369,     0,
       0,   341,     0,     0,     0,     0,     0,     0,   342,     0,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
     343,   355,   356,   357,   358,   359,     0,   360,   361,   362,
     363,   364,   365,   366,     0,     0,   367,   368,   369,     0,
       0,   341,     0,     0,     0,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,     0,     0,     0,   345,     0,     0,     0,     0,
       0,   341,     0,     0,  1055,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,     0,     0,     0,   346,     0,     0,     0,
       0,   344,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   347,   348,  1056,   349,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,   346,     0,   367,   368,
     369,   344,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   347,   348,  1057,   349,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,   346,     0,   367,   368,
     369,     0,     0,   341,     0,     0,     0,     0,     0,     0,
     342,     0,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,   343,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,     0,     0,   367,   368,
     369,     0,     0,   341,     0,     0,     0,     0,     0,     0,
     342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   344,     0,     0,     0,   345,     0,     0,
       0,     0,     0,   341,     0,     0,  1058,     0,     0,     0,
     342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,   346,     0,
       0,     0,     0,   344,     0,     0,     0,   345,     0,     0,
       0,     0,     0,     0,   347,   348,  1059,   349,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,   346,     0,
     367,   368,   369,   344,     0,     0,     0,   345,     0,     0,
       0,     0,     0,     0,   347,   348,  1060,   349,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,   346,     0,
     367,   368,   369,     0,     0,   341,     0,     0,     0,     0,
       0,     0,   342,     0,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   343,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,     0,     0,
     367,   368,   369,     0,     0,   341,     0,     0,     0,  1084,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,     0,   345,
       0,     0,     0,     0,     0,   341,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,     0,     0,     0,     0,
     346,     0,     0,     0,     0,   344,     0,     0,     0,   345,
       0,     0,     0,     0,     0,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
     346,     0,   367,   368,   369,   710,     0,     0,     0,   345,
       0,     0,     0,     0,     0,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
     346,     0,   367,   368,   369,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
       0,   268,   367,   368,   369,    22,    23,     0,     0,     0,
       0,     0,     0,     0,   269,   268,    30,   270,     0,    22,
      23,    35,     0,     0,     0,     0,     0,    40,   269,     0,
      30,   270,     0,     0,     0,    35,     0,     0,     0,     0,
       0,    40,     0,     0,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,   930,     0,     0,   931,     0,
     271,     0,    63,    53,     0,    55,     0,    57,     0,   930,
       0,     0,   931,   268,   271,     0,    63,    22,    23,     0,
       0,    79,     0,     0,    81,     0,   269,    83,    30,   270,
       0,     0,     0,    35,     0,    79,     0,     0,    81,    40,
       0,    83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,     0,     0,     0,    22,    23,     0,
       0,    53,     0,    55,     0,    57,   269,   930,    30,   270,
     931,     0,   271,    35,    63,     0,     0,    98,     0,    40,
       0,     0,     0,  1014,     0,     0,     0,     0,     0,     0,
       0,    98,     0,    79,     0,   343,    81,  1016,     0,    83,
       0,    53,     0,    55,   268,    57,     0,   930,    22,    23,
     931,     0,   271,     0,    63,     0,     0,   269,     0,    30,
     270,     0,     0,     0,    35,     0,     0,     0,     0,     0,
      40,     0,     0,    79,     0,     0,    81,     0,     0,    83,
       0,   343,     0,     0,     0,     0,   344,     0,     0,    98,
     345,     0,    53,     0,    55,  1061,   325,     0,   930,     0,
       0,   931,     0,   271,     0,    63,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   346,     0,     0,    79,     0,     0,    81,     0,    98,
      83,     0,   344,     0,     0,  1062,   345,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,   358,     0,     0,   360,   361,   362,   363,   364,   365,
     366,     0,     0,   367,   368,   369,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,     0,     0,
     360,   361,   362,   363,   364,   687,   366,   222,     0,   367,
     368,   369,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   223,   224,     0,   225,     0,     0,     0,     0,   226,
       0,     0,     0,     0,     0,     0,     0,   227,     0,     0,
       0,     0,     0,   228,  -219,     0,     0,     0,   229,     0,
       0,   230,     0,     0,   231,     0,     0,     0,  -219,  -219,
       0,  -219,   232,     0,     0,     0,  -219,     0,   233,   234,
       0,     0,     0,     0,  -219,   235,     0,     0,     0,     0,
    -219,     0,     0,     0,   236,  -219,     0,     0,  -219,     0,
       0,  -219,     0,   237,   238,     0,   239,     0,   240,  -219,
     241,     0,  -229,   242,     0,  -219,  -219,   243,     0,     0,
     244,     0,  -219,   245,     0,     0,  -229,  -229,     0,  -229,
       0,  -219,     0,     0,  -229,     0,     0,     0,     0,     0,
    -219,  -219,  -229,  -219,     0,  -219,     0,  -219,  -229,   222,
    -219,     0,     0,  -229,  -219,     0,  -229,  -219,     0,  -229,
    -219,     0,     0,   223,   224,     0,   225,  -229,     0,     0,
       0,   226,     0,  -229,  -229,     0,     0,     0,     0,   227,
    -229,     0,     0,     0,     0,   228,     0,     0,     0,  -229,
     229,     0,     0,   230,     0,     0,   231,     0,  -229,  -229,
       0,  -229,     0,  -229,   232,  -229,     0,  -220,  -229,     0,
     233,   234,  -229,     0,     0,  -229,     0,   235,  -229,     0,
       0,  -220,  -220,     0,  -220,     0,   236,     0,     0,  -220,
       0,     0,     0,     0,     0,   237,   238,  -220,   239,     0,
     240,     0,   241,  -220,  -151,   242,     0,     0,  -220,   243,
       0,  -220,   244,     0,  -220,   245,     0,     0,  -151,  -151,
       0,  -151,  -220,     0,     0,     0,  -151,     0,  -220,  -220,
       0,     0,     0,     0,  -151,  -220,     0,     0,     0,     0,
    -151,     0,     0,     0,  -220,  -151,     0,     0,  -151,     0,
       0,  -151,     0,  -220,  -220,     0,  -220,     0,  -220,  -151,
    -220,     0,  -147,  -220,     0,  -151,  -151,  -220,     0,     0,
    -220,     0,  -151,  -220,     0,     0,  -147,  -147,     0,  -147,
       0,  -151,     0,     0,  -147,     0,     0,     0,     0,     0,
    -151,  -151,  -147,  -151,     0,  -151,     0,  -151,  -147,     0,
    -151,     0,     0,  -147,  -151,     0,  -147,  -151,     0,  -147,
    -151,     0,     0,     0,     0,     0,   268,  -147,     0,     0,
      22,    23,     0,  -147,  -147,     0,     0,     0,     0,   269,
    -147,    30,   270,     0,     0,     0,    35,     0,     0,  -147,
       0,     0,    40,     0,     0,  -238,     0,     0,  -147,  -147,
       0,  -147,     0,  -147,     0,  -147,     0,     0,  -147,     0,
       0,     0,  -147,     0,    53,  -147,    55,     0,  -147,     0,
       0,  -238,     0,     0,     0,   271,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,     0,     0,    81,
       0,   -63,    83
};

static const yytype_int16 yycheck[] =
{
      12,   151,     2,    60,    16,   391,   330,   479,    20,    19,
     215,    64,   448,   137,    26,   192,   696,   600,    30,    31,
     770,    33,    34,    35,    36,    22,   684,   221,   683,    32,
      32,   208,     1,    45,   980,    47,     1,   899,     1,   633,
      52,    32,    54,    32,    56,   588,   800,     3,   591,   102,
       3,    46,    64,    65,    66,    67,    68,    69,    46,    60,
      68,    73,  1024,    75,    85,    77,    78,    46,     1,   193,
      82,   113,   266,    85,    86,   113,    88,    78,    90,    33,
      34,    93,    94,    46,    96,    65,   854,   855,   100,   101,
     102,    32,     3,    51,   453,   289,    46,    82,     1,    46,
    1074,    86,   110,    68,    83,   139,   107,    46,     0,   859,
     113,   113,  1074,   437,   896,   709,   321,   322,    46,    77,
     479,   101,    46,    56,   707,   113,   168,   180,   158,    56,
     168,   113,    85,    83,   168,   147,    56,   999,   445,  1113,
      96,    38,   163,    96,   139,   110,   296,  1093,   102,   118,
     900,   139,    85,    56,     3,   118,   164,    83,    85,   171,
     163,   163,   174,   175,   176,    98,   178,   179,   180,   138,
     139,    98,   163,   767,   163,   138,   139,    31,   111,   933,
     168,   191,    85,   726,   111,    96,   729,   969,   200,    27,
     165,   111,   139,   387,   388,   158,   632,   166,   139,   164,
     139,   873,   956,   875,   958,   138,    27,   204,   111,   221,
     267,   139,   209,   165,   600,   139,     3,    85,   968,   216,
     139,   168,   163,   139,    27,   158,   139,   118,    82,   168,
     163,   158,    86,    20,    72,   138,   163,  1005,  1006,  1091,
     168,    27,    77,    78,   168,    83,    33,   166,   139,   139,
     166,    72,   143,   166,   266,   139,     8,  1109,   270,  1013,
     163,  1015,    83,   943,   102,    56,   278,   279,   926,    72,
     925,    58,   111,  1023,   139,   166,   166,   289,   472,   133,
      83,   102,   166,   955,   138,   957,    72,   141,   165,   139,
     158,   145,   139,   417,    85,   163,    46,    83,   422,   882,
     139,   166,   133,   200,   316,   108,    56,    98,   158,    96,
     141,    93,   165,   313,   139,   102,   102,   158,   330,   166,
     111,   707,   108,   164,   221,   761,   490,   339,   492,   341,
     342,   343,   344,   345,   346,    85,   348,   349,   350,   351,
     352,   166,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   139,   163,
     684,   111,    27,   139,   376,   377,    78,   158,   118,   266,
     382,   424,   163,   385,   386,   387,   388,   389,   390,   391,
      18,    46,   158,    82,    49,   166,   139,    86,   400,   139,
     344,   345,   289,   405,   139,   407,   376,   377,   118,   604,
     605,   118,   382,   107,    69,   385,   386,    72,   158,    74,
     158,   164,   424,   163,   426,   427,   166,   157,    83,   164,
     432,   433,   139,   143,   436,   437,   143,   439,   165,   441,
     167,   139,   133,    71,   906,   389,   390,   102,    27,   139,
     141,   165,   158,   455,    45,   158,   400,   459,   460,   166,
     158,    89,   759,   465,   133,   149,   353,   510,   158,    48,
     472,    46,   141,   668,   669,   204,    67,   165,   106,   167,
     209,    56,   158,   427,   139,   487,    77,    78,   490,   433,
     492,    70,   659,    72,   496,   497,   498,   158,   500,   501,
     387,   388,   139,   800,    83,     2,   882,   139,   510,   158,
      85,   166,   139,   139,   158,    12,   506,   158,   158,   158,
     490,   158,   492,   164,    56,   165,   158,   167,    25,   108,
      27,   158,   158,   163,   158,     2,   111,    81,   112,   133,
     165,    81,   167,   118,   138,    12,    31,   141,   165,   158,
     899,   145,   496,    85,   165,   165,   696,   906,    25,   165,
      27,   167,   165,   165,   139,   167,    98,   165,   570,   571,
     572,   573,   574,   575,   576,   158,    73,   577,   578,   111,
     118,   158,   584,   158,   586,   472,    46,   118,   163,     8,
      60,   166,   594,   138,   596,   638,    56,    82,   600,   190,
     602,    86,   163,   100,   141,   158,    73,   104,   118,   166,
      77,    78,   926,   144,   584,   146,   147,   139,    46,   150,
     151,   212,   213,   214,   215,    85,   158,   571,   219,   573,
     158,   163,   165,   100,   636,   637,   638,   104,    34,   166,
     642,   938,   166,   139,   118,    27,   166,   166,   133,    21,
     999,   111,   139,   138,   168,   140,   141,   168,   118,   956,
     145,   958,   163,   165,    46,   256,   158,    49,   153,   158,
     167,   262,   642,   166,   158,  1024,   161,   158,   680,   139,
     682,   683,   684,   158,   139,   168,   158,    69,    34,  1038,
      72,   164,    74,   158,   285,   158,   139,    31,   158,   586,
     167,    83,   166,   163,   166,   707,   166,   164,   710,   711,
     712,   118,   303,   715,    83,   166,  1013,    32,  1015,    32,
     102,   168,   163,   907,   908,  1074,   163,    34,   138,    31,
      83,   165,   168,   735,   165,   737,   779,   138,   168,   168,
     742,   158,  1091,   745,   158,   165,    27,   749,    82,   246,
     752,     2,    86,   164,   141,   141,   253,   141,   133,   133,
    1109,    12,   141,   141,  1113,    46,   710,   711,    49,   133,
     141,   141,   133,   101,    25,   141,    27,   779,   141,   246,
      82,   141,   141,   133,    86,   141,   253,   789,    69,   791,
     163,    72,   794,    74,    34,   163,   158,    98,   133,   133,
     802,   166,    83,   943,   138,   157,   140,   141,   166,   166,
     401,   145,   166,   158,   133,   157,   313,   168,   139,   153,
     327,   102,    73,    24,   943,   159,   160,   161,   297,   783,
     327,   133,   134,   938,   136,   137,   138,   803,   140,   141,
     431,   625,   806,   145,   435,  1093,   313,   849,   759,   100,
     152,   153,  1023,   104,   156,  1051,  1084,   159,   160,   161,
     327,   863,   864,  1077,   866,   867,   868,   869,    35,   456,
     586,    30,   200,    -1,    -1,   877,   878,    -1,   880,    -1,
     882,    -1,   884,   885,   931,    -1,   888,    -1,    -1,    -1,
      -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   398,    -1,   905,   906,   907,   908,    -1,    -1,    -1,
      -1,   913,    -1,   915,    -1,    -1,   167,   919,    -1,    -1,
      -1,    -1,   924,   925,   926,    -1,    -1,   970,    -1,   426,
    1029,   398,    -1,    -1,    -1,   432,    -1,    -1,   266,    -1,
      -1,    -1,   439,    -1,    -1,   947,   948,   949,   950,   951,
     952,   953,   954,    -1,    -1,    -1,    -1,    -1,    -1,   426,
      -1,   289,    -1,    -1,    -1,   432,    -1,    -1,   970,    -1,
      -1,    -1,   439,   975,    -1,    -1,    -1,   979,    -1,  1078,
    1079,  1080,  1081,  1082,  1083,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   246,    -1,   494,    -1,    -1,
      -1,   498,   253,  1046,    -1,    -1,    -1,   598,   599,   506,
      -1,    -1,    -1,   604,   605,  1017,  1018,  1019,  1020,    -1,
     907,   908,     2,    -1,    -1,   616,    67,   494,    -1,    -1,
      -1,   498,    12,    -1,    -1,    -1,  1038,    -1,    82,   506,
    1042,  1043,    86,    -1,  1046,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,  1055,  1056,  1057,  1058,  1059,  1060,   387,
     388,    -1,   313,    -1,   655,    -1,    -1,    -1,    -1,    -1,
     567,  1073,  1074,    -1,  1076,    -1,   327,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,    -1,    73,   138,    -1,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,     3,    -1,  1109,    -1,   153,
      -1,  1113,    -1,    -1,    -1,   159,   160,   161,   615,    17,
     100,    -1,    20,    -1,   104,    -1,    -1,    25,    -1,   720,
     721,    -1,    -1,    -1,    -1,    -1,    -1,   465,    -1,    -1,
      -1,   732,    -1,   734,   472,    -1,    44,   398,   615,    -1,
     741,    -1,    50,   744,    -1,    -1,   747,    -1,    -1,   487,
      58,    -1,   490,    -1,   492,    -1,    -1,    65,    -1,   497,
     667,   212,   213,   214,   215,   426,   673,    -1,    -1,    -1,
      -1,   432,    80,    -1,    -1,    -1,    -1,   167,   439,    -1,
      -1,    -1,    -1,    -1,    92,   786,    -1,    -1,    96,   696,
     667,    -1,    -1,    -1,    -1,   103,   673,    -1,    -1,    -1,
      -1,   109,    -1,   710,   711,   256,    -1,    -1,    -1,    -1,
      -1,   262,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   735,    -1,
     737,    -1,    -1,   494,   285,   742,    -1,   498,   745,    -1,
      -1,    -1,    -1,    -1,   751,   506,   584,   754,   586,    -1,
      -1,    -1,   303,    -1,    -1,    -1,   246,    -1,   735,    -1,
     737,    -1,    -1,   253,    -1,   742,    -1,    -1,   745,    82,
       2,   778,    -1,    86,   751,    -1,    -1,   754,    -1,    -1,
      12,    -1,   789,    -1,   791,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,   778,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     911,    -1,   789,    -1,   791,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   313,   137,   138,    -1,   140,   141,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,   327,    -1,    -1,
     153,    73,    -1,    -1,    -1,    -1,   159,   160,   161,    -1,
     401,    -1,    -1,    -1,   615,    -1,    -1,    -1,    31,   960,
     961,    -1,    -1,   964,   965,    -1,    -1,    -1,   100,    -1,
     877,    -1,   104,     2,    -1,    -1,    -1,   884,    -1,   980,
     431,    -1,    -1,    12,   435,    -1,    -1,   988,   895,   990,
      -1,   992,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
     877,    -1,   909,    -1,    -1,    -1,   667,   884,   398,    82,
      -1,   918,   673,    86,    -1,   922,    -1,    -1,   895,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   909,    -1,    -1,   167,   426,    -1,    -1,    -1,
      -1,   918,   432,    -1,    73,   922,    -1,    -1,    -1,   439,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   966,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   100,   145,    -1,   735,   104,   737,    -1,    -1,   152,
     153,   742,    -1,   156,   745,    -1,   159,   160,   161,   966,
     751,    -1,  1093,   754,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   494,    -1,    -1,    -1,   498,    -1,
    1017,  1018,  1019,  1020,   246,    -1,   506,   778,    -1,    -1,
      -1,   253,    -1,    -1,    -1,    -1,    -1,    -1,   789,    -1,
     791,    -1,    -1,    -1,    -1,  1042,  1043,    -1,   167,    -1,
    1017,  1018,  1019,  1020,    -1,    -1,    -1,   598,   599,    -1,
      -1,    -1,    -1,   604,   605,    -1,    -1,    -1,    -1,    -1,
      -1,   899,    -1,    -1,    -1,  1042,  1043,    -1,    -1,   907,
     908,    -1,    22,    -1,     2,    -1,    26,    27,    -1,    -1,
      -1,   313,    -1,    -1,    12,    35,    -1,    37,    38,    -1,
      -1,    -1,    42,    -1,    -1,   327,    -1,    25,    48,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,   877,   246,    -1,     2,
      70,    -1,    72,   884,   253,   615,    -1,    -1,    -1,    12,
      -1,    81,    -1,    83,   895,    -1,    -1,   975,    -1,    -1,
      -1,    -1,    25,    -1,    27,    73,    -1,    -1,   909,    -1,
      -1,    -1,   102,    -1,    -1,   105,    -1,   918,   108,    -1,
      -1,   922,    -1,    -1,    82,    -1,   398,    -1,    86,   720,
     721,    -1,   100,    -1,    -1,    -1,   104,   667,    -1,    -1,
      -1,   732,    -1,   673,   313,    -1,  1024,    -1,    -1,    -1,
      73,    -1,    -1,   744,   426,    -1,   747,    -1,   327,    -1,
     432,    -1,    -1,    -1,    -1,   966,    -1,   439,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   100,   136,   137,
     138,   104,   140,   141,   142,    31,    -1,   145,   146,   147,
      -1,    -1,   150,   151,   152,   153,  1074,    -1,   156,   167,
      -1,   159,   160,   161,    -1,   735,    -1,   737,    -1,    -1,
      -1,    -1,   742,    -1,    -1,   745,  1017,  1018,  1019,  1020,
      -1,   751,   494,    -1,   754,    -1,   498,    -1,    -1,   398,
      -1,    -1,    -1,     2,   506,  1113,    82,    -1,    -1,    -1,
      86,  1042,  1043,    12,   167,    -1,    -1,    -1,   778,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,   426,    27,   789,
      -1,   791,    -1,   432,    -1,    -1,    -1,    -1,    -1,    -1,
     439,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   246,    -1,
      -1,    -1,    -1,    -1,    -1,   253,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    73,    -1,    -1,   153,    -1,    -1,
      -1,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,     3,
     911,    -1,    -1,   246,    -1,   494,    -1,    -1,    -1,   498,
     253,   100,    -1,    17,    18,   104,    20,   506,    -1,    -1,
      -1,    25,    -1,   615,    -1,   313,    -1,   877,    -1,    33,
      -1,    -1,    -1,    -1,   884,    39,    -1,    -1,    -1,   327,
      44,    -1,    -1,    47,    -1,   895,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,   909,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,   918,    -1,
     313,    -1,   922,    -1,    -1,   667,    80,    -1,   167,    -1,
      -1,   673,    -1,    -1,   327,    89,    90,    -1,    92,    -1,
      94,    42,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
     104,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
     398,    -1,    -1,    -1,    -1,    -1,   966,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   615,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   426,    -1,
      -1,    -1,    -1,   735,   432,   737,    -1,    -1,    -1,    -1,
     742,   439,    -1,   745,    -1,   398,    -1,   246,    -1,   751,
      -1,   165,   754,    -1,   253,    -1,    -1,  1017,  1018,  1019,
    1020,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   667,   130,
      -1,   132,    -1,   426,   673,    -1,   778,    -1,    -1,   432,
      -1,    -1,  1042,  1043,    -1,    -1,    -1,   789,    -1,   791,
      -1,    -1,    -1,    -1,    -1,    -1,   494,    -1,    -1,    -1,
     498,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   506,    -1,
      -1,    -1,    -1,    -1,   313,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   327,    -1,
      -1,   192,    -1,    -1,    -1,    -1,   735,    -1,   737,    -1,
      -1,   494,    -1,   742,    -1,   498,   745,   208,    -1,   210,
      -1,    -1,   751,   506,    -1,   754,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,    -1,    -1,    -1,    -1,   877,    -1,    -1,    -1,   778,
      -1,    -1,   884,    -1,    -1,     1,    -1,   149,   249,    -1,
     789,    -1,   791,   895,    -1,    -1,    -1,    -1,    -1,   398,
     162,    -1,    -1,   165,    -1,   167,    22,   909,    -1,    -1,
      26,    27,    -1,   274,    -1,    -1,   918,   615,    -1,    35,
     922,    37,    38,    -1,    -1,    -1,    42,   426,    -1,    -1,
      -1,    -1,    48,   432,    -1,    51,    -1,    -1,    -1,    -1,
     439,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    -1,    74,    -1,
      76,    77,   615,    79,   966,    81,    -1,    83,    -1,   667,
      -1,    -1,    -1,    -1,    -1,   673,    -1,    -1,   877,    -1,
      -1,    -1,    -1,    -1,    -1,   884,   102,    -1,    -1,   105,
      -1,    -1,   108,    -1,    -1,   494,   895,    -1,    -1,   498,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   506,    -1,    -1,
     909,    -1,    -1,    -1,   667,  1017,  1018,  1019,  1020,   918,
     673,    -1,    -1,   922,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   735,    -1,   737,
    1042,  1043,   158,    -1,   742,    -1,    -1,   745,   164,    -1,
      -1,    -1,    -1,   751,   415,    -1,   754,    -1,   419,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   966,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     778,    -1,   735,    -1,   737,    -1,    -1,   448,    -1,   742,
      -1,   789,   745,   791,    -1,    -1,    -1,    -1,   751,    -1,
      -1,   754,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   615,    -1,  1017,  1018,
    1019,  1020,    -1,    -1,    -1,   778,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   789,    -1,   791,    -1,
      -1,     3,    -1,  1042,  1043,    -1,   507,    -1,    -1,    -1,
      -1,   512,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,   667,    -1,
      -1,    33,    -1,    -1,   673,    -1,    -1,    39,    -1,   877,
      -1,    -1,    44,    -1,    -1,    47,   884,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,   895,    -1,    -1,
      -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
      -1,   909,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
     918,    -1,    -1,    -1,   922,    -1,   587,    89,    90,   590,
      92,    -1,    94,    -1,    96,    -1,   735,    99,   737,    -1,
      -1,   103,   895,   742,   106,    -1,   745,   109,    -1,    -1,
      -1,    -1,   751,    -1,    -1,   754,   909,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,   918,    -1,   628,   966,   922,
      -1,   632,    -1,    -1,    -1,    -1,    -1,    17,    18,   778,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
     789,    -1,   791,    33,   655,    -1,    -1,    -1,   659,    39,
      -1,   163,    -1,   165,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,   966,    -1,    -1,    -1,    -1,    58,  1017,
    1018,  1019,  1020,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,   696,    -1,    -1,    -1,    -1,
      80,    -1,    -1,     3,  1042,  1043,    -1,    -1,    -1,    89,
      90,    -1,    92,    -1,    94,    -1,    96,    17,    18,    99,
      20,    -1,    -1,   103,    -1,    25,   106,    -1,    -1,   109,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,   877,    39,
      -1,    -1,    -1,    -1,    44,   884,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,   895,   758,    58,    -1,
     761,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
     909,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   918,
      80,    -1,   783,   922,    -1,   165,    -1,    -1,    -1,    89,
      90,    -1,    92,    -1,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,   803,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,   966,   829,    35,
      -1,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    48,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    70,   165,    72,    -1,    74,    -1,
      76,    -1,    -1,    79,    -1,    81,    -1,    83,  1017,  1018,
    1019,  1020,    -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,
     165,    -1,   167,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,    -1,   108,  1042,  1043,    -1,     0,     1,    -1,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,   943,    37,    38,    39,    40,    41,    42,    43,
      44,    45,   158,    47,    48,    -1,    50,    51,   164,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    62,    -1,
      64,    65,    66,    -1,    -1,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      84,    85,    -1,    87,    88,    89,    90,    91,    92,    93,
      94,    -1,    96,    97,    -1,    99,   100,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,    -1,    -1,   112,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155,   156,   157,   158,    -1,    -1,   161,    -1,   163,
      -1,   165,     1,   167,     3,     4,     5,     6,     7,     8,
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
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,   168,
       1,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    -1,    50,
      51,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    -1,    64,    65,    66,    -1,    -1,    -1,    70,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    -1,    96,    97,    -1,    99,   100,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,   163,   164,   165,     1,   167,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    -1,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    -1,    64,    65,
      66,    -1,    -1,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    83,    84,    85,
      -1,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      96,    97,    -1,    99,   100,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,   158,    -1,    -1,   161,    -1,   163,   164,   165,
       1,   167,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
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
     161,    -1,   163,   164,   165,     1,   167,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    -1,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    -1,    64,    65,
      66,    -1,    -1,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    83,    84,    85,
      -1,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      96,    97,    -1,    99,   100,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,   158,    -1,    -1,   161,    -1,   163,   164,   165,
       1,   167,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
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
     161,    -1,   163,   164,   165,     1,   167,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    -1,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    62,    -1,    64,    65,
      66,    -1,    -1,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    83,    84,    85,
      -1,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      96,    97,    -1,    99,   100,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,   155,
     156,   157,   158,    -1,    -1,   161,    -1,   163,   164,   165,
       1,   167,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
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
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,
     161,    -1,   163,    -1,   165,    -1,   167,   168,     3,     4,
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
     165,    -1,   167,   168,     3,    -1,     5,     6,     7,     8,
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
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,   167,   168,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,
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
      -1,    99,   100,   101,    -1,   103,   104,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,    -1,    -1,    -1,    -1,
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
      93,    94,    -1,    96,    -1,    -1,    99,   100,   101,    -1,
     103,   104,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,    -1,   165,   166,   167,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    96,    -1,
      -1,    99,   100,   101,    -1,   103,   104,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   155,   156,   157,
      -1,    -1,    -1,   161,    -1,   163,    -1,   165,   166,   167,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    -1,   108,   109,    -1,    -1,    -1,
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
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   155,   156,   157,   158,    -1,
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
      -1,    96,    -1,    -1,    99,   100,   101,    -1,   103,    -1,
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
      62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    12,   142,
      -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,   152,
      -1,    25,   155,   156,   157,    -1,    -1,    -1,   161,    33,
     163,   164,   165,    -1,   167,    39,    -1,    -1,    -1,    -1,
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
       3,   165,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
      -1,    -1,   115,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,    -1,   165,     3,   167,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    35,    -1,    -1,    -1,    39,
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
     102,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,    -1,
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
      -1,    -1,   161,    -1,    -1,    -1,   165,     3,   167,     5,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,    -1,
      -1,    -1,   163,    -1,   165,   166,   167,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      56,    -1,    58,    59,    -1,    61,    62,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      96,    -1,    98,    99,    -1,    -1,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,   135,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,   157,   158,    -1,    -1,    -1,    -1,   163,    33,   165,
      35,   167,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,    64,
      65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,
      -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,     3,
     135,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,   157,    -1,    -1,    -1,    -1,    -1,   163,    33,
     165,    35,   167,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    61,    62,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,   135,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,   157,    -1,    -1,    -1,    -1,    -1,   163,
      33,   165,    35,   167,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      73,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    96,    -1,     3,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,    12,    -1,   109,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,   135,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    64,    65,    -1,    -1,
      12,    -1,   165,    71,   167,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    89,    90,    -1,    92,    -1,    94,    -1,    96,    -1,
      -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,   120,   121,   122,   123,    -1,    -1,   126,   127,
     128,   129,    -1,   131,   132,   133,   134,   135,   136,   137,
      82,    -1,   140,    -1,    86,    -1,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,    -1,   156,    12,
      -1,   159,   160,   161,   162,    -1,    19,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    12,   159,   160,   161,
      -1,    -1,    -1,    19,    -1,    -1,   168,    -1,    -1,    82,
      -1,    -1,    -1,    86,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    82,   140,   141,   142,
      86,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    12,   159,   160,   161,    -1,
      -1,    -1,    19,    -1,    -1,   168,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    31,    -1,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   117,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,   168,    -1,    -1,    82,    -1,   133,   134,    86,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    12,    -1,   159,   160,   161,    -1,    -1,    19,    -1,
     117,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    -1,   159,   160,   161,    -1,    -1,    -1,    12,   166,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    86,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    82,   140,
     141,   142,    86,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    31,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    82,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,   117,    82,   159,   160,   161,    86,    -1,
      12,    -1,   166,    -1,    -1,    -1,    -1,    19,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    31,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,    -1,   166,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
      -1,    -1,   150,   151,   152,   153,   154,    -1,   156,    -1,
      82,   159,   160,   161,    86,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    95,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,
     122,   123,    -1,    -1,   126,   127,   128,   129,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      82,   163,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    95,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,
     122,   123,    -1,    -1,   126,   127,   128,   129,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      82,   163,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    95,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,
     122,   123,    -1,    -1,   126,   127,   128,   129,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      82,   163,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    95,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,
     122,   123,    -1,    -1,   126,   127,   128,   129,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      82,   163,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    95,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,
     122,   123,    -1,    -1,   126,   127,   128,   129,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      82,   163,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    95,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,
     122,   123,    -1,    -1,   126,   127,   128,   129,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      82,   163,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    95,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,
     122,   123,    -1,    -1,   126,   127,   128,   129,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      82,   163,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    95,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,
     122,   123,    -1,    -1,   126,   127,   128,   129,    -1,   131,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      82,   163,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,   117,   118,   119,   120,   121,
     122,   123,    -1,    -1,   126,   127,   128,   129,    46,   131,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      -1,   163,    -1,    -1,    82,    -1,    -1,    -1,    86,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      82,   159,   160,   161,    86,   163,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    82,   159,   160,   161,
      86,   163,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    82,   159,   160,   161,    86,   163,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    31,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    82,   159,
     160,   161,    86,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    31,   117,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,   117,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,   159,   160,   161,    -1,   163,
      -1,   133,   134,    82,   136,   137,   138,    86,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      -1,   163,    12,    -1,    -1,    -1,    -1,    -1,   117,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,   133,   134,    36,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    -1,   163,    12,    13,    67,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    31,    -1,    86,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    82,   136,   137,   138,    86,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,   158,   159,
     160,   161,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    46,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    12,   159,   160,   161,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    -1,   133,   134,    86,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    12,   159,
     160,   161,    -1,    -1,    -1,    19,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    46,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    12,   159,   160,
     161,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    82,    -1,
      -1,    36,    86,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    67,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,   133,
     134,    86,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    -1,   159,   160,   161,    -1,    82,
      -1,    -1,   117,    86,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    31,   144,
     145,   146,   147,   148,   117,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,    82,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      31,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,    82,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    82,    -1,    -1,   117,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,   117,   150,   151,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,    -1,
      82,    -1,   133,   134,    86,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    12,   158,   159,   160,
     161,    -1,    -1,    19,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    12,    -1,   159,   160,   161,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    82,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    82,    -1,   133,   134,    86,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    -1,    86,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
      12,   113,   159,   160,   161,   117,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    12,    13,   159,   160,   161,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,   117,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    -1,   158,   159,   160,   161,
     117,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     117,    -1,   159,   160,   161,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    31,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    -1,   159,   160,   161,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,   116,   117,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   117,    -1,   159,   160,   161,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   117,    -1,   159,   160,   161,    -1,    -1,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    31,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    95,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   117,    -1,   159,   160,   161,    82,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,    95,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   117,    -1,   159,   160,   161,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      31,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    -1,   159,   160,   161,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    95,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   117,    -1,   159,   160,
     161,    82,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    95,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   117,    -1,   159,   160,
     161,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    31,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    95,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   117,    -1,
     159,   160,   161,    82,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    95,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   117,    -1,
     159,   160,   161,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    31,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,    -1,    -1,    12,    -1,    -1,    -1,    56,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     117,    -1,   159,   160,   161,    82,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     117,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    22,   159,   160,   161,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    22,    37,    38,    -1,    26,
      27,    42,    -1,    -1,    -1,    -1,    -1,    48,    35,    -1,
      37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    79,    -1,
      81,    -1,    83,    70,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    79,    22,    81,    -1,    83,    26,    27,    -1,
      -1,   102,    -1,    -1,   105,    -1,    35,   108,    37,    38,
      -1,    -1,    -1,    42,    -1,   102,    -1,    -1,   105,    48,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    70,    -1,    72,    -1,    74,    35,    76,    37,    38,
      79,    -1,    81,    42,    83,    -1,    -1,   158,    -1,    48,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,   102,    -1,    31,   105,   164,    -1,   108,
      -1,    70,    -1,    72,    22,    74,    -1,    76,    26,    27,
      79,    -1,    81,    -1,    83,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,
      -1,    31,    -1,    -1,    -1,    -1,    82,    -1,    -1,   158,
      86,    -1,    70,    -1,    72,   164,    74,    -1,    76,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,   102,    -1,    -1,   105,    -1,   158,
     108,    -1,    82,    -1,    -1,   164,    86,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,    -1,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    -1,   159,   160,   161,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,    -1,    -1,
     150,   151,   152,   153,   154,     1,   156,     3,    -1,   159,
     160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,     3,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    17,    18,
      -1,    20,    58,    -1,    -1,    -1,    25,    -1,    64,    65,
      -1,    -1,    -1,    -1,    33,    71,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    80,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    89,    90,    -1,    92,    -1,    94,    58,
      96,    -1,     3,    99,    -1,    64,    65,   103,    -1,    -1,
     106,    -1,    71,   109,    -1,    -1,    17,    18,    -1,    20,
      -1,    80,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      89,    90,    33,    92,    -1,    94,    -1,    96,    39,     3,
      99,    -1,    -1,    44,   103,    -1,    47,   106,    -1,    50,
     109,    -1,    -1,    17,    18,    -1,    20,    58,    -1,    -1,
      -1,    25,    -1,    64,    65,    -1,    -1,    -1,    -1,    33,
      71,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    80,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    89,    90,
      -1,    92,    -1,    94,    58,    96,    -1,     3,    99,    -1,
      64,    65,   103,    -1,    -1,   106,    -1,    71,   109,    -1,
      -1,    17,    18,    -1,    20,    -1,    80,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    89,    90,    33,    92,    -1,
      94,    -1,    96,    39,     3,    99,    -1,    -1,    44,   103,
      -1,    47,   106,    -1,    50,   109,    -1,    -1,    17,    18,
      -1,    20,    58,    -1,    -1,    -1,    25,    -1,    64,    65,
      -1,    -1,    -1,    -1,    33,    71,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    80,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    89,    90,    -1,    92,    -1,    94,    58,
      96,    -1,     3,    99,    -1,    64,    65,   103,    -1,    -1,
     106,    -1,    71,   109,    -1,    -1,    17,    18,    -1,    20,
      -1,    80,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      89,    90,    33,    92,    -1,    94,    -1,    96,    39,    -1,
      99,    -1,    -1,    44,   103,    -1,    47,   106,    -1,    50,
     109,    -1,    -1,    -1,    -1,    -1,    22,    58,    -1,    -1,
      26,    27,    -1,    64,    65,    -1,    -1,    -1,    -1,    35,
      71,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,    80,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    89,    90,
      -1,    92,    -1,    94,    -1,    96,    -1,    -1,    99,    -1,
      -1,    -1,   103,    -1,    70,   106,    72,    -1,   109,    -1,
      -1,    77,    -1,    -1,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,   105,
      -1,   107,   108
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   174,   175,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    47,
      48,    50,    53,    54,    55,    57,    58,    59,    61,    62,
      64,    65,    66,    70,    71,    72,    73,    74,    75,    76,
      79,    80,    81,    83,    84,    85,    87,    88,    89,    90,
      91,    92,    93,    94,    96,    97,    99,   100,   101,   102,
     103,   105,   106,   108,   109,   112,   114,   117,   133,   134,
     135,   136,   137,   142,   152,   155,   156,   157,   158,   161,
     163,   165,   167,   176,   177,   178,   179,   180,   181,   183,
     188,   189,   190,   191,   192,   196,   198,   201,   202,   204,
     205,   206,   211,   212,   214,   215,   216,   220,   221,   224,
     225,   228,   229,   232,   235,   236,   261,   264,   265,   285,
     286,   287,   288,   289,   290,   291,   299,   300,   301,   302,
     303,   306,   307,   308,   309,   310,   311,   312,   314,   315,
     316,   317,   318,   158,   178,   303,   113,   292,   293,     3,
     193,    14,    22,    35,    40,    41,    43,    81,    94,   163,
     167,   232,   285,   290,   301,   302,   303,   306,   308,   309,
     292,   303,   102,   264,    83,   193,   178,   279,   303,   178,
     165,     8,    81,   303,   304,     8,    11,    81,   102,   304,
      72,   115,   213,   303,   213,   303,    26,   265,   303,   303,
     196,   165,     3,    17,    18,    20,    25,    33,    39,    44,
      47,    50,    58,    64,    65,    71,    80,    89,    90,    92,
      94,    96,    99,   103,   106,   109,   195,   197,   233,   165,
     195,   266,   267,    32,   183,   200,   303,   303,    18,    71,
      89,   106,   303,   303,   303,     8,   165,   203,    22,    35,
      38,    81,   204,     4,   157,   255,   278,   303,   100,   101,
     158,   303,   305,   303,   200,   303,   303,   303,    93,   165,
     178,   303,   303,   183,   192,   303,   306,   183,   192,   303,
     195,   262,   303,   303,   303,   303,   303,   303,   303,   303,
       1,   164,   176,   184,   278,   104,   143,   255,   280,   281,
     305,   213,   278,   303,   313,    74,   178,   163,    78,   182,
     107,   195,   195,    51,    77,   250,   266,   157,   158,   149,
     303,    12,    19,    31,    82,    86,   117,   133,   134,   136,
     137,   138,   140,   141,   142,   144,   145,   146,   147,   148,
     150,   151,   152,   153,   154,   155,   156,   159,   160,   161,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   162,   242,   165,   167,    82,
      86,   165,   178,   158,   303,   303,   303,   278,   163,   183,
      46,   292,   262,   266,   158,   139,   158,   112,   196,   255,
     282,   283,   284,   305,    81,   209,   236,   264,    81,   207,
     262,   236,   264,   195,   165,   200,    32,    46,   200,   113,
     200,   295,    32,    46,   200,   295,    36,    67,   158,    95,
     183,   242,   158,   282,   178,   165,   244,   104,   165,   195,
     268,   269,     1,   138,   273,    46,   139,   178,   200,   165,
     165,   200,   282,   204,   195,   139,   158,   303,   303,   158,
     163,   200,   165,   282,   158,   217,   158,   217,   158,   118,
     263,   158,   200,   158,   164,   164,   176,   139,   164,   303,
     139,   166,   139,   166,   168,   295,    46,   139,   168,   295,
     116,   139,   168,     8,     1,   164,   184,    60,   185,   303,
     138,   226,   163,   237,   158,   303,   303,   303,   303,   213,
     303,   213,   303,   303,   303,   303,   303,   303,   303,    20,
      33,    58,   102,   196,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     305,   305,   305,   305,   305,   282,   282,   213,   303,   213,
     303,    27,    46,    83,   108,   294,   297,   298,   303,   318,
      32,    46,    32,    46,    95,    46,   168,   213,   303,   200,
     158,   158,   303,   303,   118,   166,   139,   210,   195,   266,
     208,   195,   158,   266,    46,   278,    43,   303,   213,   303,
     165,   200,    43,   303,   213,   303,   200,   161,   185,   187,
     303,   185,   186,   178,   303,    34,   303,   166,    27,    46,
      49,    69,    72,    83,   102,   177,   245,   246,   247,   248,
     234,   269,   139,   166,    33,    47,    90,    94,   167,   199,
     274,   286,   118,   270,   303,   267,   303,   303,   166,   255,
     303,     1,   222,   282,   166,    21,   218,   274,   286,   139,
     164,   166,   166,   280,   166,   280,   178,   168,   213,   303,
     168,   178,   303,   168,   303,   168,   303,   164,   164,   195,
     139,   158,    13,    36,    67,   278,   163,     1,   195,   230,
     231,    27,    72,    83,   102,   239,   249,   165,   158,   158,
     158,   158,   158,   166,   168,    46,    83,   139,   166,   285,
      82,    82,    43,   213,   303,    43,   213,   303,   213,   303,
     292,   292,   158,   255,   305,   284,   195,   226,   158,   195,
     226,   158,   303,   166,   303,    32,   200,    32,   200,   296,
     297,   303,    32,   200,   295,    32,   200,   295,   158,    13,
     158,    34,    34,   178,    95,   183,    46,    83,   247,   139,
     166,   165,   195,    27,    72,    83,   102,   251,   166,   269,
     273,     1,   278,    63,   305,   166,   166,   164,    68,   110,
     164,   223,   166,   165,   183,   195,   219,   262,   178,   168,
     295,   168,   295,   178,   116,   303,   303,   187,   186,     1,
     227,   164,   118,   139,   164,    83,   238,     1,     3,    12,
      17,    19,    20,    25,    39,    44,    50,    52,    58,    64,
      65,    80,    92,    96,    99,   103,   109,   133,   134,   135,
     136,   137,   140,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   156,   159,   160,   161,   162,   165,
     194,   195,   197,   240,   241,   242,   285,   166,   297,   273,
     285,   285,   303,    32,    32,   303,    32,    32,   168,   168,
     200,   200,   226,   163,   226,   163,   200,    95,    43,   303,
      43,   303,   139,   166,    95,    43,   303,   200,    43,   303,
     200,   303,   178,   303,   178,    34,   195,   246,   269,   138,
     277,    83,   273,   270,   168,    46,   168,   165,   165,    32,
     178,   278,   219,   138,   183,    43,   178,   303,   168,    43,
     178,   303,   168,   303,    13,    36,    67,   158,   158,   164,
      76,    79,   164,   177,   202,   303,   231,   251,   165,   243,
     195,   303,   133,   141,   243,   243,   270,    95,    43,    43,
      95,    43,    43,    43,    43,   163,   227,   163,   227,   303,
     303,   303,   297,   303,   303,   303,    34,   178,   277,   166,
     167,   199,   255,   276,   286,   143,   256,   270,    56,   111,
     257,   303,   274,   286,   282,   282,   178,   200,   166,   303,
     303,   178,   303,   178,   303,   187,   186,   202,     1,   138,
     272,   245,   166,     3,    96,   241,   242,   303,   303,   303,
     303,   303,   303,   227,   164,   227,   164,    95,    95,    95,
      95,   178,   270,   277,   168,   278,   255,   303,     3,    85,
      96,   258,   259,   260,   303,   183,   201,   254,   168,   166,
     166,   183,    95,    95,   158,   158,   167,   199,   271,   286,
      98,   252,   166,   243,   243,    95,    95,    95,    95,    95,
      95,   164,   164,   303,   303,   303,   303,   256,   270,   255,
     275,   276,   286,    46,   168,   260,   111,   139,   118,   144,
     146,   147,   150,   151,    56,   286,   157,   157,   303,   303,
       1,   168,   278,   257,   303,   275,   276,   303,   259,   260,
     260,   260,   260,   260,   260,   258,   168,   271,   286,   168,
     158,   253,   254,   168,   271,   286,   275
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
     190,   191,   192,   192,   192,   192,   192,   193,   193,   194,
     194,   194,   194,   194,   194,   195,   195,   195,   195,   195,
     195,   196,   196,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   199,   199,   199,   199,   200,   200,   201,
     201,   202,   202,   202,   203,   202,   204,   204,   204,   204,
     204,   204,   204,   205,   205,   205,   205,   207,   206,   208,
     206,   209,   206,   210,   206,   206,   206,   206,   206,   211,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   213,   214,   214,   214,
     214,   214,   214,   214,   214,   215,   216,   216,   216,   216,
     216,   216,   217,   217,   218,   218,   218,   219,   219,   220,
     221,   221,   222,   222,   223,   223,   223,   224,   224,   225,
     225,   225,   226,   226,   227,   227,   227,   228,   228,   229,
     230,   230,   230,   231,   231,   233,   234,   232,   235,   235,
     235,   237,   238,   236,   239,   239,   239,   239,   239,   240,
     240,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   243,   243,   244,   245,   245,   245,   246,   246,   246,
     246,   246,   247,   247,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   249,   249,   249,   249,   249,   249,   250,
     250,   251,   251,   251,   251,   251,   251,   252,   252,   253,
     253,   254,   254,   255,   255,   256,   256,   256,   257,   257,
     257,   257,   257,   258,   258,   259,   259,   259,   259,   259,
     259,   259,   260,   260,   261,   261,   261,   262,   262,   263,
     263,   263,   264,   264,   264,   264,   264,   265,   265,   266,
     266,   267,   267,   268,   268,   268,   269,   269,   269,   270,
     270,   270,   271,   271,   271,   271,   271,   271,   271,   272,
     272,   272,   272,   272,   273,   273,   273,   273,   273,   274,
     274,   274,   274,   275,   275,   275,   276,   276,   276,   276,
     276,   277,   277,   277,   277,   277,   278,   278,   278,   278,
     279,   279,   280,   280,   280,   281,   281,   282,   282,   283,
     283,   284,   284,   284,   284,   285,   285,   286,   286,   286,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     289,   290,   291,   291,   291,   291,   291,   291,   291,   291,
     292,   292,   293,   294,   294,   295,   296,   296,   297,   297,
     297,   298,   298,   298,   298,   298,   298,   299,   299,   300,
     300,   300,   300,   300,   301,   301,   301,   301,   301,   301,
     301,   302,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   304,   304,   305,   305,
     305,   306,   306,   306,   306,   307,   307,   307,   307,   308,
     308,   308,   309,   309,   309,   309,   309,   310,   310,   310,
     310,   311,   311,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   313,   313,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   315,   315,   315,   315,   315,   315,   315,   316,
     316,   316,   316,   317,   317,   317,   317,   318,   318,   318,
     318,   318,   318,   318
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
       3,     3,     4,     4,     4,     4,     4,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       3,     1,     1,     2,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     5,     5,     2,     0,     8,     0,
       9,     0,     8,     0,     9,     3,     3,     5,     5,     2,
       5,     3,     6,     6,     4,     5,     5,     3,     3,     6,
       5,     6,     5,     6,     3,     4,     3,     4,     6,     7,
       6,     7,     4,     5,     4,     5,     4,     4,     3,     6,
       5,     6,     5,     8,     7,     2,     3,     3,     2,     2,
       3,     3,     0,     2,     2,     3,     5,     1,     3,     3,
       5,     5,     0,     2,     3,     2,     3,     6,     6,     1,
       1,     1,     0,     2,     0,     2,     3,     5,     5,     1,
       1,     2,     3,     1,     3,     0,     0,     8,     0,     1,
       1,     0,     0,    10,     3,     3,     5,     5,     3,     1,
       3,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     0,     1,     3,     4,     5,     4,
       6,     6,     0,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     0,     1,     2,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     2,     1,     1,     2,     2,     0,     2,
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
       1,     1,     1,     6,     6,     4,     9,     9,     7,     6,
       6,     4,     9,     9,     7,     4,     6,     6,     9,     9,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     4,     1,     3,     4,     1,     3,     4,     3,
       3,     1,     1,     2,     1,     2,     1,     1,     3,     1,
       2,     2,     2,     2,     2,     8,     8,     9,     9,     5,
       5,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     3,     2,     2,     1,     0,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     4,
       4,     4,     3,     3,     3,     3,     5,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     3,     4,     3,     4,     3,     5,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     1
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
#line 6009 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 3:
#line 502 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6015 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 4:
#line 503 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6021 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 6:
#line 510 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6027 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 7:
#line 515 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6033 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 8:
#line 516 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6039 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 24:
#line 536 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6045 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 25:
#line 537 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6051 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 26:
#line 538 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6057 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 27:
#line 539 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6063 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 28:
#line 540 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6069 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 29:
#line 541 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6075 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 30:
#line 542 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6081 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 31:
#line 543 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6087 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 32:
#line 544 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6093 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 33:
#line 545 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6099 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 34:
#line 546 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6105 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 35:
#line 547 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6111 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 36:
#line 548 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6117 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 37:
#line 549 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6123 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 38:
#line 550 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6129 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 39:
#line 551 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6135 "bison-chapel.cpp" /* yacc.c:1663  */
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
#line 6149 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 41:
#line 569 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6158 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 42:
#line 574 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6167 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 43:
#line 579 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6176 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 44:
#line 586 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6182 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 45:
#line 587 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6188 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 46:
#line 588 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6194 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 47:
#line 592 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6200 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 48:
#line 593 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 6206 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 49:
#line 606 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6212 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 50:
#line 607 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6218 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 51:
#line 608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6224 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 52:
#line 613 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6230 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 53:
#line 614 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6236 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 54:
#line 619 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6246 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 55:
#line 624 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6256 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 56:
#line 629 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6265 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 57:
#line 633 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6274 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 58:
#line 640 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6284 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 60:
#line 649 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6294 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 62:
#line 658 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = (currentModuleType != MOD_INTERNAL ? true : false); }
#line 6300 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 63:
#line 659 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6306 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 64:
#line 660 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6312 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 65:
#line 664 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 6318 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 66:
#line 665 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 6324 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 67:
#line 666 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6330 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 68:
#line 667 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 6336 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 69:
#line 668 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6342 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 70:
#line 672 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildImportStmt(new UnresolvedSymExpr((yyvsp[-1].pch))); }
#line 6348 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 71:
#line 676 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 6354 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 72:
#line 681 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 6360 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 73:
#line 683 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 6366 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 74:
#line 685 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 6372 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 75:
#line 687 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 6378 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 76:
#line 689 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 6384 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 77:
#line 695 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 6390 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 78:
#line 696 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6396 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 79:
#line 700 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6402 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 80:
#line 701 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6408 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 81:
#line 702 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6414 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 82:
#line 703 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6420 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 83:
#line 704 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6426 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 84:
#line 705 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6432 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 85:
#line 708 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6438 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 86:
#line 709 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6444 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 87:
#line 710 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6450 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 88:
#line 711 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6456 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 89:
#line 712 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6462 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 90:
#line 713 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6468 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 91:
#line 725 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6474 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 92:
#line 726 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6480 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 93:
#line 737 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bool"; }
#line 6486 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 94:
#line 738 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "int"; }
#line 6492 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 95:
#line 739 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "uint"; }
#line 6498 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 96:
#line 740 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "real"; }
#line 6504 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 97:
#line 741 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "imag"; }
#line 6510 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 98:
#line 742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "complex"; }
#line 6516 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 99:
#line 743 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bytes"; }
#line 6522 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 100:
#line 744 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "string"; }
#line 6528 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 101:
#line 745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "sync"; }
#line 6534 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 102:
#line 746 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "single"; }
#line 6540 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 103:
#line 747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "owned"; }
#line 6546 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 104:
#line 748 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "shared"; }
#line 6552 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 105:
#line 749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "borrowed"; }
#line 6558 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 106:
#line 750 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "unmanaged"; }
#line 6564 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 107:
#line 751 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "domain"; }
#line 6570 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 108:
#line 752 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "index"; }
#line 6576 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 109:
#line 753 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "locale"; }
#line 6582 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 110:
#line 754 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "nothing"; }
#line 6588 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 111:
#line 755 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "void"; }
#line 6594 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 112:
#line 759 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6600 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 113:
#line 760 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6606 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 114:
#line 761 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6612 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 115:
#line 762 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6618 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 116:
#line 763 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6624 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 117:
#line 764 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6630 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 118:
#line 765 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 6636 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 119:
#line 766 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6642 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 120:
#line 767 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6648 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 121:
#line 768 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 6654 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 122:
#line 769 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 6660 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 123:
#line 776 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_syncvar"; }
#line 6666 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 124:
#line 777 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_singlevar"; }
#line 6672 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 125:
#line 778 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_domain"; }
#line 6678 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 126:
#line 779 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_index"; }
#line 6684 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 127:
#line 783 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6690 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 128:
#line 784 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6696 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 129:
#line 788 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6702 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 130:
#line 789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6708 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 131:
#line 793 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6714 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 133:
#line 795 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6720 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 134:
#line 805 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 6726 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 135:
#line 806 "chapel.ypp" /* yacc.c:1663  */
    { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6732 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 143:
#line 820 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6738 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 144:
#line 821 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6744 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 145:
#line 822 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6750 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 146:
#line 823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6756 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 147:
#line 828 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6765 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 148:
#line 833 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 6779 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 149:
#line 843 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6788 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 150:
#line 848 "chapel.ypp" /* yacc.c:1663  */
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 6803 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 151:
#line 859 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6812 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 152:
#line 864 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 6826 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 153:
#line 874 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6835 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 154:
#line 879 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 6849 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 155:
#line 890 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 6857 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 156:
#line 894 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 6865 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 157:
#line 899 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 6877 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 158:
#line 907 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 6889 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 159:
#line 918 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6897 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 160:
#line 924 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6903 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 161:
#line 925 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6909 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 162:
#line 926 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 6915 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 163:
#line 927 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6921 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 164:
#line 928 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 6927 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 165:
#line 929 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6933 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 166:
#line 930 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 6939 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 167:
#line 931 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6945 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 168:
#line 932 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 6951 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 169:
#line 933 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6957 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 170:
#line 934 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6963 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 171:
#line 935 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6969 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 172:
#line 936 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6975 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 173:
#line 937 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6981 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 174:
#line 938 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6987 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 175:
#line 939 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6993 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 176:
#line 940 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6999 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 177:
#line 941 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7005 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 178:
#line 943 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7015 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 179:
#line 949 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7025 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 180:
#line 955 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7035 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 181:
#line 961 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7045 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 182:
#line 967 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7056 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 183:
#line 974 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7067 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 184:
#line 981 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7075 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 185:
#line 985 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7083 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 186:
#line 991 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7089 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 187:
#line 995 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7095 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 188:
#line 996 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7101 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 189:
#line 997 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7107 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 190:
#line 998 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7113 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 191:
#line 999 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7120 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 192:
#line 1001 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7127 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 193:
#line 1003 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7134 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 194:
#line 1005 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7141 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 195:
#line 1010 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 7147 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 196:
#line 1013 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 7153 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 197:
#line 1014 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 7159 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 198:
#line 1015 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 7165 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 199:
#line 1016 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 7171 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 200:
#line 1017 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7177 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 201:
#line 1018 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7183 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 202:
#line 1022 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7189 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 203:
#line 1023 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7195 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 204:
#line 1027 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 7201 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 205:
#line 1028 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7207 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 206:
#line 1029 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7213 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 207:
#line 1033 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 7219 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 208:
#line 1034 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 7225 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 209:
#line 1038 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 7231 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 210:
#line 1042 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 7237 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 211:
#line 1044 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 7243 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 212:
#line 1048 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7249 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 213:
#line 1049 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7255 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 214:
#line 1054 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 7261 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 215:
#line 1056 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7267 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 216:
#line 1058 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7273 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 217:
#line 1065 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7287 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 218:
#line 1075 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7301 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 219:
#line 1088 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7311 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 220:
#line 1093 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7321 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 221:
#line 1098 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7331 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 222:
#line 1106 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 7337 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 223:
#line 1107 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 7343 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 224:
#line 1112 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 7349 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 225:
#line 1114 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7355 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 226:
#line 1116 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 7361 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 227:
#line 1121 "chapel.ypp" /* yacc.c:1663  */
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
#line 7381 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 228:
#line 1137 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 7389 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 229:
#line 1144 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7399 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 230:
#line 1153 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 7411 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 231:
#line 1161 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 7419 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 232:
#line 1165 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 7427 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 233:
#line 1171 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 7433 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 234:
#line 1172 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 7439 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 235:
#line 1177 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 7448 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 236:
#line 1182 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7457 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 237:
#line 1187 "chapel.ypp" /* yacc.c:1663  */
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
#line 7479 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 238:
#line 1209 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 7487 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 239:
#line 1212 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 7496 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 240:
#line 1216 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 7505 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 241:
#line 1224 "chapel.ypp" /* yacc.c:1663  */
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 7519 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 242:
#line 1234 "chapel.ypp" /* yacc.c:1663  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7530 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 243:
#line 1241 "chapel.ypp" /* yacc.c:1663  */
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
#line 7557 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 244:
#line 1267 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 7565 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 245:
#line 1271 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7574 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 246:
#line 1276 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 7582 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 247:
#line 1280 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7591 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 248:
#line 1285 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 7599 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 250:
#line 1292 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7605 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 251:
#line 1296 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7611 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 252:
#line 1297 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 7617 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 253:
#line 1298 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&"; }
#line 7623 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 254:
#line 1299 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|"; }
#line 7629 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 255:
#line 1300 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^"; }
#line 7635 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 256:
#line 1301 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "~"; }
#line 7641 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 257:
#line 1302 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "=="; }
#line 7647 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 258:
#line 1303 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!="; }
#line 7653 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 259:
#line 1304 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<="; }
#line 7659 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 260:
#line 1305 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">="; }
#line 7665 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 261:
#line 1306 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<"; }
#line 7671 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 262:
#line 1307 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">"; }
#line 7677 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 263:
#line 1308 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+"; }
#line 7683 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 264:
#line 1309 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-"; }
#line 7689 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 265:
#line 1310 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*"; }
#line 7695 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 266:
#line 1311 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/"; }
#line 7701 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 267:
#line 1312 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<"; }
#line 7707 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 268:
#line 1313 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>"; }
#line 7713 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 269:
#line 1314 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%"; }
#line 7719 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 270:
#line 1315 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**"; }
#line 7725 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 271:
#line 1316 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!"; }
#line 7731 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 272:
#line 1317 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_by"; }
#line 7737 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 273:
#line 1318 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "#"; }
#line 7743 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 274:
#line 1319 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_align"; }
#line 7749 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 275:
#line 1320 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<=>"; }
#line 7755 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 276:
#line 1321 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<~>"; }
#line 7761 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 277:
#line 1322 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "init="; }
#line 7767 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 278:
#line 1323 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 7773 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 279:
#line 1327 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "="; }
#line 7779 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 280:
#line 1328 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+="; }
#line 7785 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 281:
#line 1329 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-="; }
#line 7791 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 282:
#line 1330 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*="; }
#line 7797 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 283:
#line 1331 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/="; }
#line 7803 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 284:
#line 1332 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%="; }
#line 7809 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 285:
#line 1333 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**="; }
#line 7815 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 286:
#line 1334 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&="; }
#line 7821 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 287:
#line 1335 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|="; }
#line 7827 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 288:
#line 1336 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^="; }
#line 7833 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 289:
#line 1337 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>="; }
#line 7839 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 290:
#line 1338 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<="; }
#line 7845 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 291:
#line 1342 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7851 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 292:
#line 1343 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7857 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 293:
#line 1347 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7863 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 294:
#line 1351 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7869 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 295:
#line 1352 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7875 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 296:
#line 1353 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7881 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 297:
#line 1358 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7887 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 298:
#line 1360 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7893 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 299:
#line 1362 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7899 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 300:
#line 1364 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7905 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 301:
#line 1366 "chapel.ypp" /* yacc.c:1663  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7911 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 302:
#line 1370 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7917 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 303:
#line 1371 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7923 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 304:
#line 1375 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_IN; }
#line 7929 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 305:
#line 1376 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7935 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 306:
#line 1377 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_OUT; }
#line 7941 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 307:
#line 1378 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST; }
#line 7947 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 308:
#line 1379 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7953 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 309:
#line 1380 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7959 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 310:
#line 1381 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7965 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 311:
#line 1382 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF; }
#line 7971 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 312:
#line 1383 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7977 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 313:
#line 1387 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7983 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 314:
#line 1388 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7989 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 315:
#line 1389 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF;   }
#line 7995 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 316:
#line 1390 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 8001 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 317:
#line 1391 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST;   }
#line 8007 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 318:
#line 1392 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 8013 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 319:
#line 1396 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 8019 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 320:
#line 1397 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 8025 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 321:
#line 1401 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 8031 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 322:
#line 1402 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 8037 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 323:
#line 1403 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 8043 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 324:
#line 1404 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_REF; }
#line 8049 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 325:
#line 1405 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_PARAM; }
#line 8055 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 326:
#line 1406 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_TYPE; }
#line 8061 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 327:
#line 1410 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 8067 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 328:
#line 1411 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 8073 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 329:
#line 1414 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = NULL; }
#line 8079 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 332:
#line 1420 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 8085 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 333:
#line 1424 "chapel.ypp" /* yacc.c:1663  */
    { 
      DefExpr * de = new DefExpr(new VarSymbol((yyvsp[0].pch)));
      USR_WARN(de, "support for space in query expressions is deprecated");
      (yyval.pexpr) = de;
  }
#line 8095 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 334:
#line 1429 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 8101 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 335:
#line 1433 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8107 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 336:
#line 1434 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8113 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 337:
#line 1435 "chapel.ypp" /* yacc.c:1663  */
    { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 8123 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 338:
#line 1443 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 8129 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 339:
#line 1445 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 8135 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 340:
#line 1447 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 8141 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 341:
#line 1449 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8147 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 342:
#line 1451 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8153 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 343:
#line 1456 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8159 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 344:
#line 1458 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8165 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 345:
#line 1462 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8171 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 346:
#line 1463 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8177 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 347:
#line 1464 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8183 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 348:
#line 1465 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8189 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 349:
#line 1466 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8195 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 350:
#line 1467 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8201 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 351:
#line 1468 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 8207 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 352:
#line 1473 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 8213 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 353:
#line 1475 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 8219 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 354:
#line 1480 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 8225 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 355:
#line 1482 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 8231 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 356:
#line 1484 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 8237 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 357:
#line 1489 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 8254 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 358:
#line 1502 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 8272 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 359:
#line 1518 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8278 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 360:
#line 1520 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8284 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 361:
#line 1522 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 8290 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 362:
#line 1526 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 8296 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 363:
#line 1527 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 8302 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 364:
#line 1528 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 8308 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 365:
#line 1529 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 8314 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 366:
#line 1530 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 8320 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 367:
#line 1535 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8330 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 368:
#line 1541 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8339 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 370:
#line 1550 "chapel.ypp" /* yacc.c:1663  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 8348 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 371:
#line 1558 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 8354 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 372:
#line 1560 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8360 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 373:
#line 1565 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 8366 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 374:
#line 1567 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 8372 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 375:
#line 1569 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 8378 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 376:
#line 1574 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 8384 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 377:
#line 1576 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 8390 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 378:
#line 1578 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8396 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 379:
#line 1584 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8402 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 380:
#line 1585 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 8408 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 381:
#line 1586 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8414 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 382:
#line 1592 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8420 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 383:
#line 1594 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 8426 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 384:
#line 1596 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8434 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 385:
#line 1600 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 8442 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 386:
#line 1604 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8448 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 387:
#line 1606 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8456 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 388:
#line 1610 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8464 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 389:
#line 1617 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8470 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 390:
#line 1618 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8476 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 391:
#line 1619 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8482 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 392:
#line 1620 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8488 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 393:
#line 1621 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8494 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 394:
#line 1626 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8500 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 395:
#line 1627 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8506 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 396:
#line 1628 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 8512 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 397:
#line 1629 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8518 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 398:
#line 1630 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8524 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 399:
#line 1651 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8532 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 400:
#line 1655 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 8540 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 401:
#line 1659 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 8552 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 402:
#line 1667 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8560 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 403:
#line 1673 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8566 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 404:
#line 1674 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8572 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 405:
#line 1675 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8578 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 406:
#line 1680 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8584 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 407:
#line 1682 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8590 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 408:
#line 1688 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8596 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 409:
#line 1690 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8602 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 410:
#line 1692 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 8608 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 411:
#line 1696 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8614 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 412:
#line 1697 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8620 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 413:
#line 1698 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8626 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 414:
#line 1699 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8632 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 415:
#line 1700 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8638 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 416:
#line 1706 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8644 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 417:
#line 1707 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8650 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 418:
#line 1708 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8656 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 419:
#line 1709 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8662 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 420:
#line 1713 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 8668 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 421:
#line 1714 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8674 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 422:
#line 1718 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 8680 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 423:
#line 1719 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8686 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 424:
#line 1720 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8692 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 425:
#line 1724 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8698 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 426:
#line 1725 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8704 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 427:
#line 1729 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 8710 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 429:
#line 1734 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8716 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 430:
#line 1735 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8722 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 431:
#line 1739 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8728 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 432:
#line 1740 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8734 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 433:
#line 1741 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8740 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 434:
#line 1742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8746 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 435:
#line 1746 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8752 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 436:
#line 1747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8758 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 437:
#line 1759 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8764 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 438:
#line 1761 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 8770 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 439:
#line 1763 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8776 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 445:
#line 1774 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 8782 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 446:
#line 1776 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 8788 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 447:
#line 1778 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 8794 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 448:
#line 1780 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 8800 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 449:
#line 1782 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 8806 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 450:
#line 1784 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 8812 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 451:
#line 1786 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 8818 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 452:
#line 1789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 8824 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 453:
#line 1791 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8830 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 454:
#line 1793 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 8836 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 455:
#line 1795 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 8842 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 456:
#line 1797 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 8848 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 457:
#line 1799 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8854 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 458:
#line 1801 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 8860 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 459:
#line 1803 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 8866 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 460:
#line 1806 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 8872 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 461:
#line 1808 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 8878 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 462:
#line 1810 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 8884 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 463:
#line 1815 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8890 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 464:
#line 1817 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8896 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 465:
#line 1819 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8902 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 466:
#line 1821 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8908 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 467:
#line 1823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8914 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 468:
#line 1825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8920 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 469:
#line 1827 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8926 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 470:
#line 1829 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8932 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 471:
#line 1831 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8938 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 472:
#line 1833 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8944 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 473:
#line 1835 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8950 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 474:
#line 1837 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8956 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 475:
#line 1839 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8967 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 476:
#line 1846 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8977 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 477:
#line 1852 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8987 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 478:
#line 1858 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8997 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 479:
#line 1864 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 9007 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 480:
#line 1873 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9013 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 481:
#line 1882 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 9019 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 489:
#line 1898 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9025 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 490:
#line 1902 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 9031 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 492:
#line 1907 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9037 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 493:
#line 1911 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9043 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 494:
#line 1912 "chapel.ypp" /* yacc.c:1663  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9049 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 495:
#line 1916 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9055 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 496:
#line 1920 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9061 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 497:
#line 1921 "chapel.ypp" /* yacc.c:1663  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9067 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 498:
#line 1926 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 9075 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 499:
#line 1930 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9083 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 500:
#line 1934 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9091 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 501:
#line 1940 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 9097 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 502:
#line 1941 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 9103 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 503:
#line 1942 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 9109 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 504:
#line 1943 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 9115 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 505:
#line 1944 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 9121 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 506:
#line 1945 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 9127 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 508:
#line 1951 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 509:
#line 1956 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 9139 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 510:
#line 1958 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 9147 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 511:
#line 1962 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 9155 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 512:
#line 1966 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 9163 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 513:
#line 1970 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 9171 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 514:
#line 1978 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 9178 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 515:
#line 1983 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9188 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 516:
#line 1989 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9198 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 517:
#line 1995 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9209 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 518:
#line 2002 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9220 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 519:
#line 2011 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtOwned->symbol, (yyvsp[-1].pexpr))); }
#line 9226 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 520:
#line 2013 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtShared->symbol, (yyvsp[-1].pexpr))); }
#line 9232 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 521:
#line 2018 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 9238 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 530:
#line 2034 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 9244 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 531:
#line 2036 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9250 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 532:
#line 2038 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9256 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 533:
#line 2040 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 9262 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 534:
#line 2042 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 9268 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 535:
#line 2044 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 9274 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 536:
#line 2048 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9280 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 537:
#line 2049 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9286 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 538:
#line 2053 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 9292 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 539:
#line 2054 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 9298 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 540:
#line 2055 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9304 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 545:
#line 2073 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9310 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 546:
#line 2074 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9316 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 547:
#line 2075 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 9322 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 548:
#line 2076 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9328 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 549:
#line 2080 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9334 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 550:
#line 2081 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9340 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 551:
#line 2082 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 9346 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 552:
#line 2086 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 9352 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 553:
#line 2087 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 9358 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 554:
#line 2088 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 9364 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 555:
#line 2089 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 9370 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 556:
#line 2090 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 9376 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 557:
#line 2098 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 9382 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 558:
#line 2099 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 9388 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 559:
#line 2100 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 9394 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 560:
#line 2101 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 9400 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 561:
#line 2105 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 9406 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 562:
#line 2106 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 9412 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 564:
#line 2111 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 9418 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 565:
#line 2112 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 9424 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 566:
#line 2113 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 9430 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 567:
#line 2114 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 9436 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 568:
#line 2115 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 9442 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 569:
#line 2116 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 9448 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 570:
#line 2117 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 9454 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 571:
#line 2118 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 9460 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 572:
#line 2119 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 9466 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 573:
#line 2120 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 9472 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 574:
#line 2121 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 9478 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 575:
#line 2123 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 9486 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 576:
#line 2127 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 9494 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 577:
#line 2134 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9500 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 578:
#line 2135 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9506 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 579:
#line 2139 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9512 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 580:
#line 2140 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9518 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 581:
#line 2141 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9524 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 582:
#line 2142 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9530 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 583:
#line 2143 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9536 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 584:
#line 2144 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9542 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 585:
#line 2145 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9548 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 586:
#line 2146 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9554 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 587:
#line 2147 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9560 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 588:
#line 2148 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9566 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 589:
#line 2149 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9572 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 590:
#line 2150 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9578 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 591:
#line 2151 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9584 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 592:
#line 2152 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9590 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 593:
#line 2153 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9596 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 594:
#line 2154 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9602 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 595:
#line 2155 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9608 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 596:
#line 2156 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9614 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 597:
#line 2157 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9620 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 598:
#line 2158 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9626 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 599:
#line 2159 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9632 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 600:
#line 2160 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9638 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 601:
#line 2161 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9644 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 602:
#line 2165 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 9650 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 603:
#line 2166 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 9656 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 604:
#line 2167 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 9662 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 605:
#line 2168 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 9668 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 606:
#line 2169 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 9674 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 607:
#line 2170 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9680 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 608:
#line 2171 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 9686 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 609:
#line 2175 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9692 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 610:
#line 2176 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9698 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 611:
#line 2177 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9704 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 612:
#line 2178 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9710 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 613:
#line 2182 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9716 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 614:
#line 2183 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9722 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 615:
#line 2184 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9728 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 616:
#line 2185 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9734 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 617:
#line 2190 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 9740 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 618:
#line 2191 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 9746 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 619:
#line 2192 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 9752 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 620:
#line 2193 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 9758 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 621:
#line 2194 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 9764 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 622:
#line 2195 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 9770 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 623:
#line 2196 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 9776 "bison-chapel.cpp" /* yacc.c:1663  */
    break;


#line 9780 "bison-chapel.cpp" /* yacc.c:1663  */
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
