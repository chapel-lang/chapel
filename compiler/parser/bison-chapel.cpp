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
#define YYLAST   16368

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  168
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  140
/* YYNRULES -- Number of rules.  */
#define YYNRULES  593
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1048

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   422

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
     644,   648,   649,   650,   654,   655,   656,   660,   664,   666,
     668,   670,   672,   679,   680,   684,   685,   686,   687,   688,
     689,   692,   693,   694,   695,   696,   697,   709,   710,   721,
     722,   723,   724,   725,   726,   727,   728,   729,   730,   731,
     732,   733,   734,   735,   736,   737,   738,   742,   743,   744,
     745,   746,   747,   748,   749,   750,   751,   758,   759,   764,
     765,   770,   771,   775,   776,   780,   781,   785,   786,   787,
     788,   789,   790,   791,   792,   796,   797,   801,   802,   803,
     804,   808,   815,   816,   817,   818,   819,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,   830,   831,   832,
     833,   839,   845,   851,   857,   864,   871,   875,   882,   886,
     887,   888,   889,   890,   892,   894,   896,   901,   904,   905,
     906,   907,   908,   909,   913,   914,   918,   919,   920,   924,
     925,   929,   932,   934,   939,   940,   944,   946,   948,   955,
     965,   975,   985,   998,  1003,  1008,  1016,  1017,  1022,  1023,
    1025,  1030,  1046,  1053,  1062,  1070,  1074,  1081,  1082,  1087,
    1092,  1086,  1119,  1125,  1132,  1138,  1144,  1155,  1161,  1154,
    1193,  1197,  1202,  1206,  1211,  1218,  1219,  1223,  1224,  1225,
    1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,
    1236,  1237,  1238,  1239,  1240,  1241,  1242,  1243,  1244,  1245,
    1246,  1247,  1248,  1249,  1250,  1254,  1255,  1256,  1257,  1258,
    1259,  1260,  1261,  1262,  1263,  1264,  1265,  1269,  1270,  1274,
    1278,  1279,  1280,  1284,  1286,  1288,  1290,  1292,  1297,  1298,
    1302,  1303,  1304,  1305,  1306,  1307,  1308,  1309,  1310,  1314,
    1315,  1316,  1317,  1318,  1319,  1323,  1324,  1328,  1329,  1330,
    1331,  1332,  1333,  1337,  1338,  1341,  1342,  1346,  1347,  1351,
    1353,  1358,  1359,  1363,  1364,  1368,  1369,  1371,  1373,  1375,
    1380,  1381,  1384,  1385,  1386,  1387,  1388,  1389,  1390,  1393,
    1394,  1397,  1399,  1401,  1406,  1419,  1436,  1437,  1439,  1444,
    1445,  1446,  1447,  1448,  1452,  1458,  1464,  1472,  1473,  1481,
    1483,  1488,  1490,  1492,  1497,  1499,  1501,  1508,  1509,  1510,
    1515,  1517,  1519,  1523,  1527,  1529,  1533,  1541,  1542,  1543,
    1544,  1545,  1550,  1551,  1552,  1553,  1554,  1574,  1578,  1582,
    1590,  1597,  1598,  1599,  1603,  1605,  1611,  1613,  1615,  1620,
    1621,  1622,  1623,  1624,  1630,  1631,  1632,  1633,  1637,  1638,
    1642,  1643,  1644,  1648,  1649,  1653,  1654,  1658,  1659,  1663,
    1664,  1665,  1666,  1670,  1671,  1682,  1684,  1689,  1690,  1691,
    1692,  1693,  1694,  1696,  1698,  1700,  1702,  1704,  1706,  1709,
    1711,  1713,  1715,  1717,  1719,  1721,  1723,  1728,  1730,  1732,
    1734,  1736,  1738,  1740,  1742,  1744,  1746,  1748,  1750,  1752,
    1759,  1765,  1771,  1777,  1786,  1796,  1804,  1805,  1806,  1807,
    1808,  1809,  1810,  1811,  1816,  1817,  1821,  1825,  1826,  1830,
    1834,  1835,  1839,  1843,  1847,  1854,  1855,  1856,  1857,  1858,
    1859,  1863,  1864,  1869,  1874,  1882,  1883,  1884,  1885,  1886,
    1887,  1888,  1889,  1890,  1892,  1894,  1896,  1898,  1900,  1905,
    1906,  1909,  1910,  1911,  1914,  1915,  1916,  1917,  1928,  1929,
    1930,  1934,  1935,  1936,  1940,  1941,  1942,  1943,  1951,  1952,
    1953,  1954,  1958,  1959,  1963,  1964,  1965,  1966,  1967,  1968,
    1969,  1970,  1971,  1972,  1973,  1974,  1978,  1986,  1987,  1991,
    1992,  1993,  1994,  1995,  1996,  1997,  1998,  1999,  2000,  2001,
    2002,  2003,  2004,  2005,  2006,  2007,  2008,  2009,  2010,  2011,
    2012,  2013,  2017,  2018,  2019,  2020,  2021,  2022,  2023,  2027,
    2028,  2029,  2030,  2034,  2035,  2036,  2037,  2042,  2043,  2044,
    2045,  2046,  2047,  2048
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
  "stmt_ls", "only_ls", "opt_only_ls", "except_ls", "use_access_control",
  "use_stmt", "require_stmt", "assignment_stmt", "opt_label_ident",
  "ident_fn_def", "ident_def", "ident_use", "internal_type_ident_def",
  "scalar_type", "reserved_type_ident_use", "opt_string", "do_stmt",
  "return_stmt", "class_level_stmt", "private_decl", "forwarding_stmt",
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

#define YYPACT_NINF -935

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-935)))

#define YYTABLE_NINF -544

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -935,   102,  2654,  -935,    28,  -935,  -935,  -935,  -935,  -935,
    -935,  4106,    17,   211,  -935, 11991,  -935,    17,  9543,  -935,
     281,   145,   211,  4106,  9543,  4106,    67,  -935,  9543,  6645,
    -935,  8025,  8623,  6484,  9543,  -935,   121,  -935,  -935, 16263,
    -935, 12300,  8761,  -935,  9543,  9543,  -935,  -935,  -935,  9543,
    -935, 11991,  -935,  9543,   228,   129,  1040,  1220,  -935,  -935,
    -935,  8899,  7565,  9543,  8761, 11991,  9543,   252,  -935,   191,
    4106,  -935,  9543,  -935,  9681,  9681, 16263,  -935,  -935, 11991,
    -935,  -935,  9543,  9543,  -935,  9543,  -935,  9543,  -935,  -935,
   11613,  9543,  -935,  9543,  -935,  -935,  3140,  6783,  8163,  -935,
    3945,  -935,   210,  -935,   308,  -935,   284,  -935,  -935,  -935,
    -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,
    -935,  -935,  -935,  -935, 16263,  -935, 16263,   220,    54,  -935,
    -935, 12300,  -935,   242,  -935,   244,  -935,  -935,   278,   279,
     313,   311,   316, 15990, 11856,   247,   320,   324,  -935,  -935,
    -935,  -935,  -935,  -935,  -935,   341,  -935,  -935, 15990,   323,
    4106,  -935,  -935,   327,  9543,  9543,  9543,  9543,  9543,  8899,
    8899,   310,  -935,  -935,  -935,  -935,   348,   330,  -935,  -935,
     329, 14195, 16263, 12300,  -935,   335,  -935,   152, 15990,   388,
    7726, 15990,  -935,    38,  -935, 16263,    71,   322, 16263,   346,
      29, 13714,    10, 13673,   322,  9543,  -935, 14098, 12918,  7726,
    -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,
    -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,
    -935,  -935,  -935,  4106,  -935,   352,  1951,    76,     7,  -935,
    4106,  -935,  -935, 13857,   634,   348, 13857,   348,   634,  -935,
    7726,  9543,  -935,  -935, 16263,  -935,   213, 15990,  9543,  9543,
    -935, 15990,   355, 14045,  -935, 13857,   348, 15990,   353,  7726,
    -935, 15990, 14424,  -935,  -935, 14481, 11610,  -935,  -935, 14567,
     409,   370,   348, 13857, 14624,   338,   338,  1709,   634,   634,
      91,  -935,  -935,  3301,    77,  -935,  9543,  -935,   114,   119,
    -935,   -34,    14, 14710,   -39,   520,  -935,  3462,  -935,   479,
    8899,   404,   380,  -935,  -935,  -935,   230,  -935,  -935,  9543,
    9543,  9543,  9543,  8623,  8623,  9543,   331,  9543,  9543,  9543,
    9543,  9543,    75, 11613,  9543,  9543,  9543,  9543,  9543,  9543,
    9543,  9543,  9543,  9543,  9543,  9543,  9543,  9543,  9543,  -935,
    -935,  -935,  -935,  -935,  -935,  8324,  8324,  -935,  -935,  -935,
    -935,  8324,  -935,  -935,  8324,  8324,  7726,  7726,  8623,  8623,
    7266,  -935,  -935, 14241, 14338, 14664,    49,  3623,  -935,  8623,
      29,   393,   286,  -935,  9543,  -935,  9543,   443,  -935,   390,
     420,  -935,  -935,   405, 16263, 12300,   516,  8899,  -935,  4267,
    8623,  -935,   399,  -935,    29,  4428,  8623,  -935,    29,   322,
    9819,  9543,  -935,  4106,   530,  9543,   407,  -935,   438,  -935,
    -935,  1951,  -935,   436,   412,  -935,  9957,   461,  9543, 12300,
    -935,  -935,  -935,   416,  -935,  8899,  -935, 15990, 15990,  -935,
      35,  -935,  7726,   417,  -935,   565,  -935,   565,  -935, 10095,
     451,  -935,  -935,  -935,  -935,  -935,  -935,  8462,  -935, 12752,
    6944,  -935,  7105,  -935,  4106,   430,  8623,  5877,  2979,   433,
    9543,  6201,  -935,  -935,   238,  -935,  3784, 16263,   290, 14384,
    8899,   439, 16196,   339,  -935, 14807, 13812, 13812,   163,  -935,
     163,  -935,   163, 14521,  2029,  1417,   770,   348,   338,  -935,
    -935,  -935,  -935,  1709, 15181,   163,  2059,  2059, 13812,  2059,
    2059,  1081,   338, 15181, 13947,  1081,   634,   634,   338,   448,
     458,   463,   467,   468,   462,   459,  -935,   163,  -935,   163,
      61,  -935,  -935,  -935,   122,  -935,  2540, 16039,   363, 10233,
    8623, 10371,  8623,  9543,  8623, 12219,    17, 14860,  -935,  -935,
    -935, 15990, 14900,  7726,  -935,  7726,  -935,   404,   293,  9543,
     124,  9543, 15990,    33, 13902,  7266,  -935,  9543, 15990,    12,
   13759,  -935,   472,   470,   473, 14957,   470,   474,   598, 15043,
    4106, 14000,  -935,   165,  -935,  -935,  -935,  -935,  -935,  -935,
     539,   126,  -935, 12378,  -935,   546,   476,  1951,    76,    67,
     121,  9543,  9543,  5716,  -935,  -935,   328,  7887,  -935, 15990,
    -935,  -935,  -935, 15990,   475,    58,   477,  -935,  2423,  -935,
    -935,   299, 16263,  -935,  -935,  -935,  -935,  -935,  -935,  -935,
    4106,   -28, 12515,  -935,  -935, 15990,  4106, 15990,  -935, 15100,
    -935,  -935,  -935,  -935,  9819,  9543,   497,  2246,   481,   522,
      86,  -935,   564,  -935,  -935,  -935,  -935,  6325,  -935,  -935,
    -935,  -935,  -935,  -935,  -935,  -935,  -935,  7266,  -935,    31,
    8623,  8623,  9543,   616, 15132,  9543,   617, 15275,   483, 12602,
      29,    29,  -935,  -935,  -935,  -935,   491,  -935, 13857,  -935,
   12998,  4589,  -935,  4750,  -935,   159,  -935, 13074,  4911,  -935,
      29,  5072,  -935,    29,  9543,  -935,  9543,  -935,  4106,  9543,
    -935,  4106,   620,  -935,  -935, 16263,   697,  -935,  1951,   519,
     576,  -935,  -935,  -935,    43,  -935,  -935,   461,   499,    56,
    -935,  -935,  -935,  5233,  8899,  -935,  -935,  -935, 16263,  -935,
     533,   329,  -935,  -935,  5394,   504,  5555,   505,  -935,  9543,
     524,   525,   513,  1599,  -935,  9543, 16263,  -935,  -935,   546,
     515,   137,  -935,   548,  -935,   549,   554,   551,   555,  -935,
     563,   562,   566,   570,   572,   146,   573,   577,   578,  -935,
    -935, 16263,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,
    -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,
    -935,  9543,  -935,   581,   584,   582,   515,   515,  -935,  -935,
     461,   180,   216, 15243, 10509, 10647, 15386, 10785, 10923, 11061,
   11199,  -935,  -935,  2307,  -935,  4106,  9543, 15990,  9543, 15990,
    7266,  -935,  4106,  9543, 15990,  -935,  9543, 15990,  -935, 15418,
   15990,  -935, 15990,   689,  4106,   519,  -935,   560,  9060,   113,
    -935,    92,  -935,  -935,  8623, 12105,  4106,  -935,    16,   569,
    9543,  -935,  9543,  -935, 15990,  4106,  9543,  -935, 15990,  4106,
   15990,  -935,  -935,  -935,  7427,   199,   199,  -935,  1409,  -935,
   15990,  -935,    82,   438,  -935,  -935, 12842,  -935, 12459,  -935,
    -935,  -935,  9543,  9543,  9543,  9543,  9543,  9543,  9543,  9543,
     567,  2463, 15043, 13150, 13226,  -935, 15043, 13302, 13378,  9543,
    4106,  -935,   461,   519,  6039,  -935,  -935,  -935,   387,  8899,
    -935,  -935,   125,  9543,   -16, 12659,  -935,   535,   388,  -935,
     329, 15990, 13454,  -935, 13530,  -935,  -935,  -935, 11337,   630,
     229,  -935,   597,   599,   515,   515, 15529, 15561, 15672, 15704,
   15815, 15847,  -935,  -935,  4106,  4106,  4106,  4106, 15990,  -935,
    -935,   113,  9221,    62,  -935,  -935, 15990,  -935,    89,  -935,
     -38,  -935,   149, 15958,  -935,  -935,  -935, 11199,  -935,  4106,
    4106,  2815,  -935,  -935,   148,  -935,    92,  -935,  -935,  -935,
    9543,  9543,  9543,  9543,  9543,  9543, 15043, 15043, 15043, 15043,
    -935,  -935,  -935,  -935,  -935,   437,  8623, 11747,  -935,  9543,
     125,    89,    89,    89,    89,    89,    89,   125,   966, 15043,
   15043,   579, 11475,    74,   -21, 12699,  -935,  -935, 15990,  -935,
    -935,  -935,  -935,  -935,  -935,  -935,   601,  -935,  -935,   512,
   11861,  -935,  -935,  -935,  9382,  -935,   623,  -935
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    87,   535,   536,   537,   538,
     539,     0,   474,    73,   107,   445,   203,   474,     0,   112,
       0,   350,    73,     0,     0,     0,     0,   213,   509,   509,
     532,     0,     0,     0,     0,   111,     0,   223,   108,     0,
     219,     0,     0,   114,     0,     0,   465,   540,   115,     0,
     226,   439,   349,     0,     0,     0,    45,    44,   110,   204,
     351,     0,     0,     0,     0,   443,     0,     0,   113,     0,
       0,    88,     0,   533,     0,     0,     0,   109,   205,   441,
     353,   116,     0,     0,   589,     0,   591,     0,   592,   593,
     508,     0,   590,   587,   127,   588,     0,     0,     0,     4,
       0,     5,     0,     9,    46,    10,     0,    11,    12,    14,
     423,   424,    22,    13,   128,   134,    15,    17,    16,    19,
      20,    21,    18,   133,     0,   131,     0,   500,     0,   135,
     132,     0,   136,   514,   496,   425,   497,   429,   427,     0,
       0,   501,   502,     0,   428,     0,   515,   516,   517,   534,
     495,   431,   430,   498,   499,     0,    38,    24,   437,     0,
       0,   475,    74,     0,     0,     0,     0,     0,     0,     0,
       0,   500,   514,   427,   501,   502,   446,   428,   515,   516,
       0,   474,     0,     0,   352,     0,   177,     0,   408,     0,
     415,   510,   224,   538,   141,     0,     0,   225,     0,     0,
       0,     0,     0,     0,     0,   509,   140,     0,     0,   415,
      81,    89,   100,    94,   102,    84,    93,   103,    90,   104,
      82,   105,    98,    92,    99,    97,    95,    96,    83,    85,
      91,   101,   106,     0,    86,     0,     0,     0,     0,   357,
       0,   124,    32,     0,   574,   493,     0,   440,   575,     7,
     415,   509,   130,   129,   320,   405,     0,   404,     0,     0,
     125,   513,     0,     0,    35,     0,   444,   432,     0,   415,
      36,   438,     0,   184,   180,     0,   428,   184,   181,     0,
     346,     0,   442,     0,     0,   576,   578,   507,   573,   572,
       0,    48,    51,     0,     0,   410,     0,   412,     0,     0,
     411,     0,     0,   404,     0,     0,     6,     0,    47,     0,
       0,   206,     0,   306,   305,   227,     0,   426,    23,     0,
       0,     0,     0,     0,     0,     0,   577,     0,     0,     0,
       0,     0,     0,   506,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   265,
     272,   273,   274,   269,   271,     0,     0,   267,   270,   268,
     266,     0,   276,   275,     0,     0,   415,   415,     0,     0,
       0,    25,    26,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,    28,     0,    29,     0,   423,   421,     0,
     416,   417,   422,     0,     0,     0,     0,     0,   150,     0,
       0,   149,     0,   158,     0,     0,     0,   156,     0,     0,
       0,    57,   137,     0,   170,     0,     0,    30,   288,   220,
     361,     0,   362,   364,     0,   386,     0,   367,     0,     0,
     123,    31,    33,     0,   319,     0,    67,   511,   512,   126,
       0,    34,   415,     0,   191,   182,   178,   183,   179,     0,
     344,   341,   143,    37,    50,    49,    52,     0,   541,     0,
       0,   528,     0,   530,     0,     0,     0,     0,     0,     0,
       0,     0,   545,     8,     0,    40,     0,     0,     0,   404,
       0,     0,     0,   299,   356,   473,   569,   568,   571,   580,
     579,   584,   583,   565,   562,   563,   564,   504,   552,   526,
     527,   525,   524,   505,   556,   567,   561,   559,   570,   560,
     558,   550,   555,   557,   566,   549,   553,   554,   551,     0,
       0,     0,     0,     0,     0,     0,   582,   581,   586,   585,
     485,   486,   488,   490,     0,   477,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   543,   474,   474,   146,   342,
     354,   409,     0,     0,   434,     0,   343,   206,     0,     0,
       0,     0,   449,     0,     0,     0,   159,     0,   455,     0,
       0,   157,   588,    60,     0,    53,    58,     0,   169,     0,
       0,     0,   433,   293,   290,   291,   292,   296,   297,   298,
     288,     0,   281,     0,   289,   307,     0,   365,     0,   119,
     120,   118,   117,     0,   385,   384,   496,     0,   359,   494,
     358,   523,   407,   406,     0,     0,     0,   435,     0,   185,
     348,   496,     0,   542,   503,   529,   413,   531,   414,   166,
       0,     0,     0,   544,   164,   459,     0,   547,   546,     0,
      42,    41,    39,    64,     0,    57,   207,     0,     0,   217,
       0,   214,   303,   300,   301,   304,   228,     0,    71,    72,
      70,    69,    68,   521,   522,   487,   489,     0,   476,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   142,   419,   420,   418,     0,   355,     0,   168,
       0,     0,   148,     0,   147,     0,   480,     0,     0,   154,
       0,     0,   152,     0,     0,   138,     0,   139,     0,     0,
     172,     0,   174,   294,   295,     0,   288,   279,     0,   399,
     308,   311,   310,   312,     0,   363,   366,   367,     0,     0,
     368,   369,   193,     0,     0,   192,   195,   436,     0,   186,
     189,     0,   345,   167,     0,     0,     0,     0,   165,     0,
       0,     0,     0,   222,   212,     0,   215,   211,   302,   307,
     277,    75,   260,    89,   258,    94,    78,    93,    90,   263,
     104,    76,   105,    92,    95,    77,    79,    91,   106,   257,
     239,   242,   240,   241,   252,   243,   256,   248,   246,   259,
     262,   247,   245,   250,   255,   244,   249,   253,   254,   251,
     261,     0,   237,     0,    80,     0,   277,   277,   235,   478,
     367,   514,   514,     0,     0,     0,     0,     0,     0,     0,
       0,   145,   144,     0,   151,     0,     0,   448,     0,   447,
       0,   479,     0,     0,   454,   155,     0,   453,   153,    55,
      54,   171,   464,   173,     0,   399,   282,     0,     0,   367,
     309,   325,   360,   390,     0,   543,     0,   197,     0,     0,
       0,   187,     0,   162,   461,     0,     0,   160,   460,     0,
     548,    65,    66,   201,   509,   222,   222,   199,   222,   209,
     218,   216,     0,   288,   234,   238,     0,   264,     0,   230,
     231,   482,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   222,   452,     0,     0,   481,   458,     0,     0,     0,
       0,   176,   367,   399,     0,   402,   401,   403,   496,   321,
     285,   283,     0,     0,     0,     0,   388,   496,   198,   196,
       0,   190,     0,   163,     0,   161,   210,   381,     0,   313,
       0,   236,    75,    77,   277,   277,     0,     0,     0,     0,
       0,     0,   202,   200,     0,     0,     0,     0,    56,   175,
     284,   367,   391,     0,   322,   324,   323,   339,     0,   340,
     327,   330,     0,   326,   317,   318,   221,     0,   188,     0,
       0,     0,   380,   379,   496,   314,   325,   278,   232,   233,
       0,     0,     0,     0,     0,     0,   451,   450,   457,   456,
     287,   286,   393,   394,   396,   496,     0,   543,   338,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   463,
     462,     0,   371,     0,     0,     0,   395,   397,   329,   331,
     332,   335,   336,   337,   333,   334,   328,   376,   374,   496,
     543,   315,   229,   316,   391,   375,   496,   398
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -935,  -935,  -935,     0,  -415,  1777,  -935,  -935,  -935,  -935,
     297,   440,  -392,    95,    93,  -935,  -935,  -935,    57,   724,
    -935,  1495,  -162,  -632,  -935,  -764,  -935,   365,  -878,  -716,
     -48,  -935,  -935,  -935,   985,  -935,  -935,  -935,   478,  -935,
      13,  -935,  -935,  -935,  -935,  -935,   556,   193,   -70,  -935,
    -935,  -935,     1,   629,  -935,  -935,  -935,  -935,  -935,  -935,
    -935,  -935,  -134,  -139,  -713,  -935,  -125,    44,   169,  -935,
    -935,  -935,     2,  -935,  -935,  -260,   153,  -935,  -195,  -214,
    -238,  -230,  -743,  -935,  -168,  -935,     4,   748,  -116,   356,
    -935,  -378,  -697,  -905,  -935,  -563,  -443,  -934,  -896,  -745,
     -50,  -935,    42,  -935,  -180,  -935,   231,   425,  -266,  -935,
    -935,  -935,   939,  -935,    -1,  -935,  -935,  -193,  -935,  -553,
    -935,  -935,  1065,  1249,   -11,    -2,    50,   441,  -935,  1435,
    1591,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -935,  -369
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   292,   100,   578,   102,   103,   104,   309,
     105,   293,   573,   577,   574,   106,   107,   108,   109,   163,
     802,   237,   110,   234,   111,   604,   196,   242,   112,   113,
     114,   115,   116,   117,   678,   118,   119,   120,   445,   619,
     741,   121,   122,   615,   736,   123,   124,   481,   753,   125,
     126,   650,   651,   171,   235,   595,   128,   129,   483,   759,
     656,   805,   806,   415,   884,   419,   591,   592,   593,   594,
     657,   315,   724,   986,  1042,   976,   255,   965,   920,   924,
     970,   971,   972,   130,   281,   450,   131,   132,   238,   239,
     423,   424,   608,   983,   939,   427,   605,  1003,   917,   849,
     294,   187,   298,   299,   389,   390,   391,   172,   134,   135,
     136,   137,   173,   139,   160,   161,   534,   404,   695,   535,
     536,   140,   174,   175,   143,   197,   392,   177,   145,   178,
     179,   148,   149,   150,   304,   151,   152,   153,   154,   155
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     158,   538,    99,   590,   176,   365,   620,   181,   252,   253,
     408,   256,   696,   188,   381,   316,   180,   191,   191,   576,
     201,   203,   207,   208,   183,   804,   192,   393,   387,   416,
     852,   243,   425,   244,   245,   727,   614,   879,   246,   240,
     247,   698,   248,   596,   425,   240,   975,   387,   302,   428,
     257,   261,   263,   265,   266,   267,   466,  -122,   240,   271,
      62,   272,   691,   275,   279,    62,  1004,   382,   282,  1009,
     433,   283,   284,  1026,   285,   402,   286,   425,     5,   287,
     288,   402,   289,   937,   915,   257,   261,   303,   387,   443,
      16,   544,   967,   889,   890,  -382,   471,  1010,   854,  -194,
     912,   313,     3,   665,  1006,   499,   810,   387,   465,   469,
    1047,  1027,   262,   891,   809,  -122,   544,  1038,  -382,   402,
     376,   402,   733,   402,  -382,   472,   159,   314,   967,   500,
     464,   274,   278,  1041,  -377,  1045,   744,   365,   377,  -122,
     666,  -194,   429,   377,   922,  -382,   975,   300,    59,   467,
    -382,   435,   921,   158,   373,   374,   375,   271,   257,   303,
     606,   851,   936,  -377,   734,   426,  -382,    71,   961,   377,
     502,   377,    78,   501,   982,   377,  -377,   426,   468,   261,
     380,   969,   156,   621,   467,   879,   524,   525,   377,  -377,
    -382,   467,   377,  -382,   191,  -194,   538,   467,   261,   923,
    -378,   395,  -382,   409,   387,   387,   968,   713,   183,   467,
     426,  -382,   457,   545,   162,   960,   938,   969,   735,   726,
     855,   756,   204,    21,   184,  1008,  1007,   607,   190,  -378,
    -382,   988,   989,    28,   251,   249,  -377,   458,  1040,   261,
     191,  -377,  -378,    37,   714,   156,   757,   437,   438,   460,
     297,   454,   919,   576,   462,  -378,   804,   667,   261,   435,
     478,   716,   616,  1011,  1001,    50,   -81,    52,  1030,  1031,
    1032,  1033,  1034,  1035,   -87,   -83,   461,   905,    60,   558,
     387,   463,   209,   -88,   668,   459,   689,   384,   717,  1012,
     250,  1013,  1014,   456,   830,  1015,  1016,   330,   538,   479,
     332,   590,  -378,    80,   335,    21,   385,  -378,   485,   486,
     487,   488,   490,   492,   493,  -484,   494,   495,   496,   497,
     498,   831,   503,   504,   505,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   517,   518,   878,   241,
     847,   268,  -484,   388,   261,   261,    21,   560,   435,    52,
     261,  -483,   269,   261,   261,   261,   261,   527,   529,   537,
      60,   241,   388,   652,   716,   429,   322,   436,   547,   307,
    -383,   273,   277,   551,  -470,   552,   700,   703,  -483,   182,
    -383,  -520,   308,  -520,   484,    80,   257,   310,   562,   564,
      52,   987,   156,   387,   568,   570,  -467,   317,   640,   575,
     575,    60,   579,   388,   581,   519,   520,   653,   366,  -383,
     367,   521,   926,   395,   522,   523,   323,   609,   654,   368,
     324,   429,   388,   369,   613,   435,    80,   133,   429,   264,
     646,   261,  -466,   318,  -347,  -383,   133,   655,   745,   747,
     550,   671,  -383,   144,   643,   369,   613,   687,   133,   261,
     133,   261,   144,  -347,   742,   632,   613,   635,   319,   637,
     639,   538,   583,  -383,   144,  -471,   144,   326,   590,   257,
    -472,  -520,   330,  -520,  -469,   332,   456,   378,  -468,   335,
     584,   372,  -383,   585,   370,   332,   878,  -383,   377,   383,
    -383,  -518,  -519,  -518,  -519,   133,   386,   241,   241,   241,
     241,  -400,   626,   586,   628,   414,   587,   397,    54,   439,
     300,   144,   300,   418,   442,   276,   276,   588,   807,   388,
     388,   133,  -400,   449,   451,   133,  -400,   473,   562,   674,
     568,   677,   579,   679,   635,   477,   589,   144,   480,   482,
     241,   144,   261,   241,   261,   680,   681,   549,   688,  -400,
     690,  -392,   554,   729,   537,   555,   697,   553,   559,   556,
     565,   580,   241,   583,  -370,   398,   401,   403,   407,   582,
     720,   597,  -392,  -280,   598,   607,  -392,  -387,   611,   617,
     241,   584,   918,   618,   585,   133,   622,  -387,   612,   927,
     267,   271,   303,  -370,   630,   388,   261,   636,   647,  -392,
    -280,   144,   658,   684,   586,   704,  -370,   587,   431,   305,
     612,   432,   659,   297,   721,   297,  -387,   660,   588,  -370,
     612,   661,   662,   664,   663,   722,   -59,   705,   707,   708,
     441,   127,   435,   575,   575,   732,   755,   589,   725,   737,
     127,   754,  -387,   758,   723,   814,   817,   819,   452,  -387,
     823,   844,   127,   848,   127,   850,   537,   731,   133,   490,
     527,   813,   322,   853,   816,   133,  -370,   860,   865,   869,
    -387,  -370,   984,   873,   144,  -372,   883,   241,   871,   872,
     827,   144,   829,   -84,   858,  -107,  -112,   834,  -111,  -387,
     837,   -82,  -108,   839,  -387,   840,  1005,  -387,   842,   127,
    -114,   241,   -85,  -115,  -372,   241,   683,  -110,   388,  -113,
     887,  1018,   323,   -86,  -109,  -116,   324,  -372,   133,   888,
     910,   583,   913,   257,   985,   127,   -81,   952,   -83,   127,
    -372,   930,   133,   864,   144,   868,  1010,   750,   870,   584,
     751,  1005,   585,  1037,   880,   548,   185,   476,   144,   945,
     686,   859,   394,   901,   944,   447,  1039,   881,   940,   715,
     846,   882,   586,   326,  1043,   587,  1000,    54,   330,   566,
     331,   332,  1024,   571,  1046,   335,   588,  -372,  1005,  1036,
    1029,   206,  -372,   342,     0,   610,   685,     0,     0,   127,
     886,   348,     0,     0,     0,   589,     0,     0,   322,     0,
       0,     0,   133,   827,   829,     0,   834,   837,   864,   868,
       0,     0,     0,     0,   902,   903,     0,   904,   144,   537,
       0,   906,   907,     0,   133,   908,     0,   252,   253,     0,
     133,     0,     0,     0,     0,     0,     0,     0,   133,     0,
     144,     0,     0,   925,   635,     0,   144,     0,   323,   931,
       0,   932,   324,     0,   144,   934,     0,     0,     0,     0,
     241,   241,   127,   191,   963,     0,   241,   241,     0,   127,
       0,     0,     0,     0,     0,     0,     0,     0,   712,     0,
       0,   902,   946,   947,   906,   948,   949,   950,   951,   133,
       0,     0,     0,   133,     0,     0,     0,     0,   958,   326,
     327,   133,     0,   303,   330,   144,   331,   332,   966,   144,
       0,   335,   973,     0,     0,   739,     0,   144,     0,   342,
       0,     0,   127,     0,     0,   346,   347,   348,   692,   694,
       0,  1023,     0,     0,   699,   702,   127,     0,     0,     0,
       0,   138,     0,   996,   997,   998,   999,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   669,   138,     0,   138,     0,   868,     0,  1019,  1020,
     303,     0,     0,     0,     0,     0,     0,   241,   241,   996,
     997,   998,   999,  1019,  1020,   241,     0,   414,     0,     0,
       0,     0,     0,     0,   414,  1025,   635,   241,  1028,     0,
     241,   916,     0,     0,     0,   133,   127,     0,  -389,   138,
       0,     0,     0,     0,     0,     0,   200,   202,  -389,     0,
       0,   144,     0,     0,     0,     0,     0,     0,   127,   635,
       0,     0,     0,   868,   127,   138,     0,     0,   861,   138,
       0,     0,   127,     0,     0,   821,   822,  -389,     0,     0,
       0,     0,     0,   824,     0,   133,     0,     0,     0,     0,
       0,   133,     0,   204,    21,   835,     0,   141,   838,     0,
       0,   144,   964,  -389,    28,   251,   141,   144,     0,     0,
    -389,     0,   808,   301,    37,     0,     0,  -222,   141,     0,
     141,     0,     0,   127,     0,   811,   812,   127,     0,   138,
       0,  -389,     0,     0,     0,   127,    50,     0,    52,   322,
       0,     0,     0,  -222,     0,  1002,   133,     0,   133,    60,
    -389,     0,     0,   133,     0,  -389,   133,     0,  -389,     0,
       0,     0,   144,   133,   144,   141,   133,     0,     0,   144,
       0,     0,   144,   -63,    80,     0,     0,     0,     0,   144,
       0,     0,   144,     0,     0,   241,     0,     0,   133,   323,
    1002,   141,     0,   324,     0,   141,     0,     0,     0,   133,
       0,   133,   138,     0,   144,     0,     0,     0,     0,   138,
       0,     0,     0,     0,     0,   144,     0,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1002,     0,     0,
     414,   414,     0,     0,   414,   414,     0,     0,     0,   127,
     326,   327,     0,   328,   329,   330,     0,   331,   332,     0,
       0,   974,   335,   929,     0,   141,     0,   978,     0,   414,
     342,   414,   138,     0,     0,     0,   346,   347,   348,     0,
       0,     0,     0,   204,    21,     0,   138,     0,     0,     0,
     133,   142,     0,     0,    28,   251,     0,   133,     0,   127,
     142,     0,     0,     0,    37,   127,   144,  -222,     0,   133,
       0,     0,   142,   144,   142,     0,     0,     0,     0,     0,
       0,   133,     0,     0,     0,   144,    50,     0,    52,     0,
     133,     0,     0,  -222,   133,     0,     0,   144,   141,    60,
       0,     0,     0,     0,     0,   141,   144,     0,   489,   491,
     144,     0,     0,     0,     0,     0,   138,     0,     0,   142,
     127,   974,   127,   -62,    80,     0,     0,   127,     0,     0,
     127,     0,     0,     0,     0,   133,     0,   127,   138,     0,
     127,     0,     0,     0,   138,   142,     0,     0,     0,   142,
       0,   144,   138,   526,   528,     0,     0,     0,   141,     0,
       0,     0,   127,     0,   546,     0,     0,     0,     0,     0,
       0,     0,   141,   127,     0,   127,     0,     0,     0,   133,
     133,   133,   133,     0,     0,   563,     0,     0,     0,     0,
       0,   569,     0,     0,     0,   144,   144,   144,   144,     0,
       0,     0,     0,   138,   133,   133,     0,   138,     0,   142,
       0,     0,     0,     0,     0,   138,     0,     0,     0,     0,
     144,   144,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     0,    20,    21,     0,     0,     0,   146,     0,     0,
       0,    27,   141,    28,   874,   322,   146,     0,    33,     0,
       0,   631,     0,    37,   127,     0,     0,     0,   146,     0,
     146,   127,     0,     0,   141,     0,     0,     0,     0,     0,
     141,     0,     0,   127,     0,    50,     0,    52,   141,   305,
       0,   875,   142,     0,   876,   127,    59,     0,    60,   142,
       0,     0,     0,     0,   127,   323,     0,     0,   127,   324,
       0,     0,     0,     0,     0,   146,     0,    76,     0,     0,
      78,     0,     0,    80,     0,     0,     0,     0,     0,   138,
       0,     0,     0,     0,     0,   673,     0,   676,     0,   141,
       0,   146,     0,   141,   233,   146,     0,     0,     0,   127,
       0,   141,   142,     0,     0,     0,   326,   327,     0,     0,
     329,   330,     0,   331,   332,     0,   142,     0,   335,     0,
       0,     0,     0,    94,     0,     0,   342,     0,     0,   138,
       0,   280,   346,   347,   348,   138,     0,     0,     0,     0,
       0,     0,     0,   127,   127,   127,   127,     0,     0,     0,
       0,     0,     0,   147,     0,   146,     0,     0,     0,     0,
       0,     0,   147,     0,     0,     0,     0,     0,   127,   127,
       0,     0,     0,     0,   147,     0,   147,     0,    16,   311,
       0,   312,    20,    21,     0,     0,   142,     0,     0,     0,
     138,    27,   138,    28,   874,     0,     0,   138,    33,     0,
     138,     0,     0,    37,     0,   141,     0,   138,   142,     0,
     138,     0,     0,     0,   142,   489,   526,     0,     0,     0,
       0,   147,   142,     0,     0,    50,     0,    52,   146,    54,
       0,   875,   138,     0,   876,   146,    59,   280,    60,     0,
       0,     0,     0,   138,     0,   138,     0,   147,     0,     0,
     280,   147,     0,   396,     0,   141,     0,    76,     0,     0,
      78,   141,     0,    80,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   142,     0,     0,     0,   142,     0,     0,
       0,     0,     0,     0,     0,   142,     0,     0,   146,     0,
       0,   422,     0,     0,     0,     0,     0,   322,     0,     0,
       0,     0,   146,     0,     0,     0,     0,     0,     0,   434,
       0,   147,     0,    94,     0,     0,   141,     0,   141,   877,
       0,     0,     0,   141,   138,     0,   141,     0,     0,     0,
       0,   138,     0,   141,     0,     0,   141,     0,     0,   101,
       0,     0,     0,   138,     0,     0,     0,   323,   157,     0,
       0,   324,     0,     0,     0,   138,     0,     0,   141,     0,
     186,     0,   189,     0,   138,     0,     0,     0,   138,   141,
       0,   141,   146,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   147,     0,     0,     0,     0,   142,
       0,   147,     0,     0,   146,     0,     0,     0,   326,   327,
     146,   328,   329,   330,     0,   331,   332,   270,   146,   138,
     335,     0,     0,     0,     0,     0,     0,   341,   342,     0,
       0,   345,     0,     0,   346,   347,   348,     0,     0,     0,
       0,     0,     0,   101,     0,     0,     0,   306,     0,   142,
       0,     0,     0,     0,   147,   142,     0,     0,     0,   557,
     141,     0,     0,   138,   138,   138,   138,   141,   147,   146,
       0,     0,     0,   146,     0,     0,     0,     0,     0,   141,
       0,   146,     0,     0,     0,     0,   422,     0,   138,   138,
       0,   141,     0,     0,     0,     0,     0,     0,     0,     0,
     141,     0,     0,     0,   141,     0,     0,   371,     0,     0,
     142,     0,   142,     0,     0,     0,     0,   142,     0,     0,
     142,     0,     0,     0,   210,     0,     0,   142,     0,     0,
     142,     0,     0,     0,     0,     0,   211,   212,   147,     0,
       0,     0,   642,   213,     0,   141,     0,   649,     0,     0,
       0,   214,   142,     0,     0,     0,     0,   215,     0,     0,
     147,     0,   216,   142,   217,   142,   147,   218,     0,     0,
       0,     0,     0,     0,   147,   219,     0,     0,     0,     0,
     417,   220,   221,     0,     0,   146,     0,   430,   222,   141,
     141,   141,   141,     0,     0,     0,     0,   223,     0,     0,
       0,     0,     0,     0,     0,     0,   224,   225,     0,   226,
       0,   227,     0,   228,   141,   141,   229,     0,     0,     0,
     230,   420,     0,   231,     0,   147,   232,   322,     0,   147,
       0,     0,     0,     0,     0,   146,     0,   147,     0,     0,
     101,   146,     0,     0,   142,     0,     0,     0,     0,     0,
       0,   142,     0,     0,   101,     0,     0,   322,   719,     0,
       0,     0,   422,   142,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   142,     0,   323,     0,     0,
       0,   324,   421,   740,   142,     0,     0,   280,   142,     0,
       0,     0,     0,     0,     0,     0,   146,     0,   146,     0,
       0,     0,     0,   146,     0,     0,   146,   323,     0,     0,
       0,   324,     0,   146,     0,     0,   146,     0,     0,     0,
       0,     0,   803,     0,   101,     0,     0,     0,   326,   142,
       0,     0,     0,   330,     0,   331,   332,     0,   146,     0,
     335,   147,     0,     0,     0,     0,   430,     0,   342,   146,
       0,   146,   430,     0,   346,   347,   348,     0,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,     0,     0,
     335,     0,     0,   142,   142,   142,   142,   341,   342,     0,
     845,   345,     0,   422,   346,   347,   348,     0,     0,     0,
       0,   147,     0,     0,     0,     0,     0,   147,   142,   142,
       0,     0,     0,   740,     0,     0,     0,     0,     0,     0,
       0,   629,     0,     0,     0,   634,     0,   752,     0,     0,
       0,   649,     0,   101,     0,     0,     0,     0,     0,     0,
     146,     0,     0,     0,     0,  -208,     0,   146,     0,  -208,
    -208,     0,     0,     0,     0,     0,   885,     0,  -208,   146,
    -208,  -208,   147,     0,   147,  -208,     0,     0,     0,   147,
    -208,   146,   147,  -208,     0,     0,     0,     0,     0,   147,
     146,     0,   147,     0,   146,     0,     0,     0,   900,     0,
       0,     0,  -208,     0,  -208,     0,  -208,     0,  -208,  -208,
       0,  -208,     0,  -208,   147,  -208,  -208,     0,     0,     0,
    -208,  -208,     0,     0,     0,   147,     0,   147,     0,  -208,
       0,  -208,  -208,     0,  -208,   146,  -208,  -208,     0,     0,
    -208,  -208,     0,     0,  -208,     0,     0,   710,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -208,     0,  -208,     0,  -208,     0,  -208,
    -208,     0,  -208,   803,  -208,     0,  -208,     0,     0,   146,
     146,   146,   146,     0,     0,     0,     0,     0,     0,     0,
    -208,     0,     0,     0,     0,  -208,  -208,   743,  -208,     0,
       0,  -208,     0,   748,   146,   146,   147,     0,     0,     0,
       0,     0,     0,   147,     0,     0,   210,     0,     0,     0,
       0,     0,     0,     0,     0,   147,     0,     0,   211,   212,
       0,     0,     0,     0,     0,   213,     0,   147,     0,     0,
       0,     0,     0,   214,     0,     0,   147,     0,     0,   215,
     147,  -208,     0,     0,   216,     0,   217,  -208,   430,   218,
     430,     0,     0,     0,     0,   430,     0,   219,   430,     0,
       0,     0,    16,   220,   221,   841,    20,    21,   843,     0,
     222,     0,     0,     0,     0,    27,     0,    28,   874,   223,
       0,   147,    33,     0,     0,     0,     0,    37,   224,   225,
     857,   226,     0,   227,     0,   228,     0,     0,   229,     0,
       0,   863,   230,   867,     0,   231,     0,     0,   232,    50,
       0,    52,     0,    54,     0,   875,     0,     0,   876,     0,
      59,     0,    60,     5,     0,   147,   147,   147,   147,     0,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
       0,    76,    19,     0,    78,     0,     0,    80,     0,     0,
     147,   147,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    35,   377,     0,   738,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,     0,     0,     0,
       0,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    94,     0,     0,
       0,   911,     0,   953,     0,     0,     0,     0,    68,     0,
       0,     0,    71,   928,     0,     0,     0,     0,     0,    77,
       0,     0,   933,     0,     0,    81,   935,     0,     0,     0,
       0,     0,     0,     0,    -2,     4,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,   959,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -222,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,    48,    49,     0,     0,     0,
      50,    51,    52,    53,    54,    55,    56,  -222,   -43,    57,
      58,    59,     0,    60,    61,    62,     0,    63,    64,    65,
      66,    67,    68,    69,    70,     0,    71,    72,     0,    73,
      74,    75,    76,    77,     0,    78,    79,   -61,    80,    81,
       0,     0,    82,     0,    83,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,    86,    87,    88,    89,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,    92,    93,     0,    94,     0,
       0,    95,     0,    96,     0,    97,  1021,    98,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   164,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   165,   166,     0,   167,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   168,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,    92,    93,   254,     0,
       0,     0,    95,     0,   169,     0,    97,     0,   170,  1022,
       4,     0,     5,     6,     7,     8,     9,    10,     0,  -543,
       0,    11,    12,    13,    14,    15,  -543,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,  -543,    25,    26,
    -543,    27,     0,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,    38,  -222,     0,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
      48,    49,     0,     0,     0,    50,    51,    52,    53,     0,
      55,    56,  -222,   -43,    57,    58,    59,  -543,    60,    61,
      62,  -543,    63,    64,    65,    66,    67,    68,    69,    70,
       0,    71,    72,     0,    73,    74,    75,    76,    77,     0,
      78,    79,   -61,    80,    81,     0,     0,    82,     0,    83,
       0,     0,  -543,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -543,  -543,
      87,  -543,  -543,  -543,  -543,  -543,  -543,  -543,     0,  -543,
    -543,  -543,  -543,  -543,     0,  -543,  -543,  -543,  -543,  -543,
    -543,  -543,  -543,    94,  -543,  -543,  -543,     0,    96,  -543,
      97,   290,    98,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,    37,     0,    38,  -222,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,    48,    49,     0,     0,     0,    50,    51,    52,    53,
      54,    55,    56,  -222,   -43,    57,    58,    59,     0,    60,
      61,    62,     0,    63,    64,    65,    66,    67,    68,    69,
      70,     0,    71,    72,     0,    73,    74,    75,    76,    77,
       0,    78,    79,   -61,    80,    81,     0,     0,    82,     0,
      83,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    85,
      86,    87,    88,    89,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,    92,    93,   254,    94,     0,     0,    95,     0,    96,
     291,    97,     4,    98,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,    37,     0,    38,  -222,     0,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,    47,    48,    49,     0,     0,     0,    50,    51,    52,
      53,    54,    55,    56,  -222,   -43,    57,    58,    59,     0,
      60,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,     0,    71,    72,     0,    73,    74,    75,    76,
      77,     0,    78,    79,   -61,    80,    81,     0,     0,    82,
       0,    83,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      85,    86,    87,    88,    89,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,    92,    93,     0,    94,     0,     0,    95,     0,
      96,   455,    97,   474,    98,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,    37,     0,    38,  -222,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,    48,    49,     0,     0,     0,    50,    51,
      52,    53,    54,    55,    56,  -222,   -43,    57,    58,    59,
       0,    60,    61,    62,     0,    63,    64,    65,    66,    67,
      68,    69,    70,     0,    71,    72,     0,    73,    74,    75,
      76,    77,     0,    78,    79,   -61,    80,    81,     0,     0,
      82,     0,    83,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,    86,    87,    88,    89,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,    92,    93,     0,    94,     0,     0,    95,
       0,    96,   475,    97,   290,    98,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,    25,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,    37,     0,    38,
    -222,     0,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,    47,    48,    49,     0,     0,     0,    50,
      51,    52,    53,    54,    55,    56,  -222,   -43,    57,    58,
      59,     0,    60,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,     0,    71,    72,     0,    73,    74,
      75,    76,    77,     0,    78,    79,   -61,    80,    81,     0,
       0,    82,     0,    83,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,    86,    87,    88,    89,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,    92,    93,     0,    94,     0,     0,
      95,     0,    96,   291,    97,     4,    98,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -222,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,    48,    49,     0,     0,     0,
      50,    51,    52,    53,    54,    55,    56,  -222,   -43,    57,
      58,    59,     0,    60,    61,    62,     0,    63,    64,    65,
      66,    67,    68,    69,    70,     0,    71,    72,     0,    73,
      74,    75,    76,    77,     0,    78,    79,   -61,    80,    81,
       0,     0,    82,     0,    83,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,    86,    87,    88,    89,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,    92,    93,     0,    94,     0,
       0,    95,     0,    96,   641,    97,     4,    98,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,    34,    35,     0,    36,    37,
       0,    38,  -222,     0,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,    48,    49,     0,     0,
       0,    50,    51,    52,    53,   305,    55,    56,  -222,   -43,
      57,    58,    59,     0,    60,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,     0,    71,    72,     0,
      73,    74,    75,    76,    77,     0,    78,    79,   -61,    80,
      81,     0,     0,    82,     0,    83,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,    92,    93,     0,    94,
       0,     0,    95,     0,    96,     0,    97,     4,    98,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -222,     0,    39,    40,    41,     0,    42,
      43,    44,   -43,    45,    46,     0,    47,    48,    49,     0,
       0,     0,    50,    51,    52,    53,     0,    55,    56,  -222,
     -43,    57,    58,    59,     0,    60,    61,    62,     0,    63,
      64,    65,    66,    67,    68,    69,    70,     0,    71,    72,
       0,    73,    74,    75,    76,    77,     0,    78,    79,   -61,
      80,    81,     0,     0,    82,     0,    83,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    85,    86,    87,    88,    89,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,    92,    93,     0,
      94,     0,     0,    95,     0,    96,     0,    97,     4,    98,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,    26,     0,    27,
       0,    28,    29,    30,    31,    32,    33,   561,    35,     0,
      36,    37,     0,    38,  -222,     0,    39,    40,    41,     0,
      42,    43,    44,   -43,    45,    46,     0,    47,    48,    49,
       0,     0,     0,    50,    51,    52,    53,     0,    55,    56,
    -222,   -43,    57,    58,    59,     0,    60,    61,    62,     0,
      63,    64,    65,    66,    67,    68,    69,    70,     0,    71,
      72,     0,    73,    74,    75,    76,    77,     0,    78,    79,
     -61,    80,    81,     0,     0,    82,     0,    83,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    85,    86,    87,    88,
      89,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,    92,    93,
       0,    94,     0,     0,    95,     0,    96,     0,    97,     4,
      98,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,   567,    35,
       0,    36,    37,     0,    38,  -222,     0,    39,    40,    41,
       0,    42,    43,    44,   -43,    45,    46,     0,    47,    48,
      49,     0,     0,     0,    50,    51,    52,    53,     0,    55,
      56,  -222,   -43,    57,    58,    59,     0,    60,    61,    62,
       0,    63,    64,    65,    66,    67,    68,    69,    70,     0,
      71,    72,     0,    73,    74,    75,    76,    77,     0,    78,
      79,   -61,    80,    81,     0,     0,    82,     0,    83,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    85,    86,    87,
      88,    89,     0,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,    92,
      93,     0,    94,     0,     0,    95,     0,    96,     0,    97,
       4,    98,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,    26,
       0,    27,     0,    28,    29,    30,    31,    32,    33,   826,
      35,     0,    36,    37,     0,    38,  -222,     0,    39,    40,
      41,     0,    42,    43,    44,   -43,    45,    46,     0,    47,
      48,    49,     0,     0,     0,    50,    51,    52,    53,     0,
      55,    56,  -222,   -43,    57,    58,    59,     0,    60,    61,
      62,     0,    63,    64,    65,    66,    67,    68,    69,    70,
       0,    71,    72,     0,    73,    74,    75,    76,    77,     0,
      78,    79,   -61,    80,    81,     0,     0,    82,     0,    83,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    85,    86,
      87,    88,    89,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
      92,    93,     0,    94,     0,     0,    95,     0,    96,     0,
      97,     4,    98,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
     828,    35,     0,    36,    37,     0,    38,  -222,     0,    39,
      40,    41,     0,    42,    43,    44,   -43,    45,    46,     0,
      47,    48,    49,     0,     0,     0,    50,    51,    52,    53,
       0,    55,    56,  -222,   -43,    57,    58,    59,     0,    60,
      61,    62,     0,    63,    64,    65,    66,    67,    68,    69,
      70,     0,    71,    72,     0,    73,    74,    75,    76,    77,
       0,    78,    79,   -61,    80,    81,     0,     0,    82,     0,
      83,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    85,
      86,    87,    88,    89,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,    92,    93,     0,    94,     0,     0,    95,     0,    96,
       0,    97,     4,    98,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
      25,    26,     0,    27,     0,    28,    29,    30,    31,    32,
      33,   833,    35,     0,    36,    37,     0,    38,  -222,     0,
      39,    40,    41,     0,    42,    43,    44,   -43,    45,    46,
       0,    47,    48,    49,     0,     0,     0,    50,    51,    52,
      53,     0,    55,    56,  -222,   -43,    57,    58,    59,     0,
      60,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,     0,    71,    72,     0,    73,    74,    75,    76,
      77,     0,    78,    79,   -61,    80,    81,     0,     0,    82,
       0,    83,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      85,    86,    87,    88,    89,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,    92,    93,     0,    94,     0,     0,    95,     0,
      96,     0,    97,     4,    98,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,   836,    35,     0,    36,    37,     0,    38,  -222,
       0,    39,    40,    41,     0,    42,    43,    44,   -43,    45,
      46,     0,    47,    48,    49,     0,     0,     0,    50,    51,
      52,    53,     0,    55,    56,  -222,   -43,    57,    58,    59,
       0,    60,    61,    62,     0,    63,    64,    65,    66,    67,
      68,    69,    70,     0,    71,    72,     0,    73,    74,    75,
      76,    77,     0,    78,    79,   -61,    80,    81,     0,     0,
      82,     0,    83,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,    86,    87,    88,    89,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,    92,    93,     0,    94,     0,     0,    95,
       0,    96,     0,    97,     4,    98,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,   856,    26,     0,    27,     0,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,    37,     0,    38,
    -222,     0,    39,    40,    41,     0,    42,    43,    44,   -43,
      45,    46,     0,    47,    48,    49,     0,     0,     0,    50,
      51,    52,    53,     0,    55,    56,  -222,   -43,    57,    58,
      59,     0,    60,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,     0,    71,    72,     0,    73,    74,
      75,    76,    77,     0,    78,    79,   -61,    80,    81,     0,
       0,    82,     0,    83,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,    86,    87,    88,    89,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,    92,    93,     0,    94,     0,     0,
      95,     0,    96,     0,    97,     4,    98,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,   862,    35,     0,    36,    37,     0,
      38,  -222,     0,    39,    40,    41,     0,    42,    43,    44,
     -43,    45,    46,     0,    47,    48,    49,     0,     0,     0,
      50,    51,    52,    53,     0,    55,    56,  -222,   -43,    57,
      58,    59,     0,    60,    61,    62,     0,    63,    64,    65,
      66,    67,    68,    69,    70,     0,    71,    72,     0,    73,
      74,    75,    76,    77,     0,    78,    79,   -61,    80,    81,
       0,     0,    82,     0,    83,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,    86,    87,    88,    89,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,    92,    93,     0,    94,     0,
       0,    95,     0,    96,     0,    97,     4,    98,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,    25,    26,     0,    27,     0,    28,
      29,    30,    31,    32,    33,   866,    35,     0,    36,    37,
       0,    38,  -222,     0,    39,    40,    41,     0,    42,    43,
      44,   -43,    45,    46,     0,    47,    48,    49,     0,     0,
       0,    50,    51,    52,    53,     0,    55,    56,  -222,   -43,
      57,    58,    59,     0,    60,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,     0,    71,    72,     0,
      73,    74,    75,    76,    77,     0,    78,    79,   -61,    80,
      81,     0,     0,    82,     0,    83,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,    92,    93,     0,    94,
       0,     0,    95,     0,    96,     0,    97,   728,    98,     5,
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
       0,    81,     0,     0,     0,     0,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    85,    86,    87,    88,    89,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,    92,    93,   254,
       0,     0,     0,    95,     0,   169,     0,    97,     0,   170,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   164,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   165,   166,     0,   167,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   168,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    85,    86,    87,    88,
      89,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,    92,    93,
     254,     0,     0,     0,    95,     0,   169,     0,    97,     0,
     170,   633,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   164,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   165,   166,     0,   167,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   168,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,     0,    81,     0,     0,     0,     0,     0,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    85,    86,
      87,    88,    89,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
      92,    93,   254,     0,     0,     0,    95,     0,   169,     0,
      97,     0,   170,   962,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   164,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   165,   166,
       0,   167,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   168,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   760,     0,   761,     0,
      85,    86,    87,    88,    89,   762,     0,     0,     0,    90,
     763,   212,   764,     0,     0,     0,     0,   765,     0,    91,
       0,     0,    92,    93,     0,   214,     0,     0,    95,     0,
     169,   766,    97,     0,   170,   638,   767,     0,   217,     0,
       0,   768,     0,   769,     0,     0,     0,     0,     0,   770,
       0,     0,     0,     0,     0,   771,   772,     0,     0,     0,
       0,     0,   222,     0,     0,     0,     0,     0,     0,     0,
       0,   773,     0,     0,     0,     0,     0,     0,     0,     0,
     224,   225,     0,   774,     0,   227,     0,   775,     0,     0,
     776,     0,     0,     0,   777,     0,     0,   231,     0,     0,
     778,     0,     0,     0,     0,     0,     0,     0,     0,   349,
     350,   351,   352,   353,   354,     0,     0,   357,   358,   359,
     360,     0,   362,   363,   779,   780,   781,   782,   783,     0,
       0,   784,     0,     0,     0,   785,   786,   787,   788,   789,
     790,   791,   792,   793,   794,   795,     0,   796,     0,     0,
     797,   798,   799,   800,     0,     0,   801,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   164,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,   204,    21,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,   205,
      30,   165,   166,     0,   167,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,    52,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,    60,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
       0,     0,     0,    77,     0,     0,    79,     0,    80,    81,
       0,     0,     0,     0,     0,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,    86,    87,    88,    89,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,    92,    93,     0,     0,     0,
       0,    95,     0,   169,     0,    97,     0,   170,     5,     6,
       7,     8,   193,    10,   194,     0,     0,   164,     0,     0,
      14,    15,     0,     0,  -121,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   165,   166,     0,   167,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,  -121,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   168,     0,    71,     0,     0,
      73,     0,     0,   195,    77,     0,  -121,    79,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     5,     6,     7,     8,
       9,    10,     0,    91,     0,   164,    92,    93,    14,    15,
       0,     0,    95,     0,   169,    19,    97,     0,   170,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     165,   166,     0,   167,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   168,     0,    71,     0,     0,    73,   258,
     259,     0,    77,   295,     0,    79,     0,     0,    81,     0,
       0,     0,     0,     0,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,    86,    87,    88,    89,     0,     0,     0,
       0,    90,   296,     0,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,    92,    93,   254,     0,     0,     0,
      95,     0,   169,     0,    97,     0,   170,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   164,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   167,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
     258,   259,     0,    77,   295,     0,    79,     0,     0,    81,
       0,     0,     0,     0,     0,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,    86,    87,    88,    89,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,    92,    93,   254,     0,     0,
       0,    95,     0,   169,     0,    97,   625,   170,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   164,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   165,   166,     0,   167,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   168,     0,    71,     0,     0,
      73,   258,   259,     0,    77,   295,     0,    79,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,    92,    93,   254,     0,
       0,     0,    95,     0,   169,     0,    97,   627,   170,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   164,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
     530,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   167,    35,   531,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,   532,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,     0,     0,     0,    77,     0,     0,    79,     0,
     533,    81,     0,     0,     0,     0,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    85,    86,    87,    88,    89,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,    92,    93,     0,
       0,     0,     0,    95,     0,   169,     0,    97,     0,   170,
       5,     6,     7,     8,   193,    10,     0,     0,     0,   164,
       0,     0,    14,    15,     0,     0,  -121,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   165,   166,     0,   167,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,  -121,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   168,     0,    71,
       0,     0,    73,     0,     0,   195,    77,     0,  -121,    79,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    85,    86,    87,    88,
      89,     0,     0,     0,     0,    90,     0,     0,     5,     6,
       7,     8,     9,    10,     0,    91,     0,   164,    92,    93,
      14,    15,     0,     0,    95,     0,   169,    19,    97,     0,
     170,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   165,   166,     0,   167,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   168,     0,    71,     0,     0,
      73,   258,   259,     0,    77,     0,     0,    79,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,    92,    93,     0,   260,
       0,     0,    95,     0,   169,     0,    97,     0,   170,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   164,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   165,   166,     0,   167,    35,     0,    36,
       0,     0,    38,     0,     0,     0,    40,    41,     0,     0,
      43,    44,     0,    45,    46,     0,    47,    48,     0,     0,
       0,     0,     0,    51,     0,    53,     0,    55,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    69,   168,     0,    71,     0,
       0,    73,   258,   259,     0,    77,     0,     0,    79,     0,
       0,    81,     0,     0,     0,     0,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    85,    86,    87,    88,    89,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,    92,    93,   254,
       0,     0,     0,    95,     0,   169,     0,    97,     0,   170,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   164,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   165,   166,     0,   167,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,   730,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   168,     0,    71,
       0,     0,    73,   258,   259,     0,    77,     0,     0,    79,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    85,    86,    87,    88,
      89,     0,     0,     0,     0,    90,     0,     0,     5,     6,
       7,     8,     9,    10,     0,    91,     0,   164,    92,    93,
      14,    15,     0,     0,    95,     0,   169,    19,    97,     0,
     170,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   165,   166,     0,   167,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,   198,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   168,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      81,     0,     0,     0,     0,     0,   199,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     5,     6,     7,     8,
       9,    10,     0,    91,     0,   164,    92,    93,    14,    15,
       0,     0,    95,     0,   169,    19,    97,     0,   170,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     165,   166,     0,   167,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   168,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,     0,    81,     0,
       0,     0,     0,     0,   199,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,    86,    87,    88,    89,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,    92,    93,   254,     0,     0,     0,
      95,     0,   169,     0,    97,     0,   170,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   164,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   165,   166,     0,   167,    35,     0,    36,     0,     0,
      38,     0,     0,     0,    40,    41,     0,     0,    43,    44,
       0,    45,    46,     0,    47,    48,     0,     0,     0,     0,
       0,    51,     0,    53,     0,    55,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,    68,    69,   168,     0,    71,     0,     0,    73,
     258,   259,     0,    77,     0,     0,    79,     0,     0,    81,
       0,     0,     0,     0,     0,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,    86,    87,    88,    89,     0,     0,
       0,     0,    90,     0,     0,     5,     6,     7,     8,     9,
      10,     0,    91,     0,   164,    92,    93,    14,    15,     0,
       0,    95,     0,   169,    19,    97,     0,   170,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   167,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    81,     0,     0,
       0,     0,     0,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,    86,    87,    88,    89,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,    92,    93,   254,     0,     0,     0,    95,
       0,   169,   623,    97,     0,   170,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   164,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     165,   166,     0,   167,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   168,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,     0,    81,     0,
       0,     0,     0,     0,   199,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,    86,    87,    88,    89,     0,     0,     0,
       0,    90,     0,     0,     5,     6,     7,     8,     9,    10,
       0,    91,     0,   164,    92,    93,    14,    15,     0,     0,
      95,     0,   169,    19,    97,     0,   170,     0,     0,     0,
     240,    26,     0,     0,     0,     0,     0,    30,   165,   166,
       0,   167,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   168,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      85,    86,    87,    88,    89,     0,     0,     0,     0,    90,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    91,
       0,   164,    92,    93,    14,    15,     0,     0,    95,     0,
      96,    19,    97,     0,   170,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   165,   166,     0,   167,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   168,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,     0,    81,     0,     0,     0,     0,     0,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    85,    86,
      87,    88,    89,     0,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
      92,    93,   254,     0,     0,     0,    95,     0,   169,     0,
      97,     0,   170,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   164,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
     599,     0,     0,     0,     0,     0,    30,   165,   166,     0,
     167,    35,     0,   600,     0,     0,    38,     0,     0,     0,
      40,    41,     0,     0,    43,    44,     0,    45,    46,     0,
      47,    48,     0,     0,     0,     0,     0,    51,     0,    53,
       0,    55,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    65,   601,    67,    68,    69,
     602,     0,    71,     0,     0,    73,     0,     0,     0,    77,
       0,     0,    79,     0,     0,    81,     0,     0,     0,     0,
       0,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    85,
      86,    87,    88,    89,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,    92,    93,   254,     0,     0,     0,    95,     0,   169,
       0,    97,     0,   914,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   164,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   165,   166,
       0,   167,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   168,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      85,    86,    87,    88,    89,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,    92,    93,   254,     0,     0,     0,    95,     0,
     169,     0,    97,     0,   914,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   164,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   165,
     166,     0,   899,    35,     0,    36,     0,     0,    38,     0,
       0,     0,    40,    41,     0,     0,    43,    44,     0,    45,
      46,     0,    47,    48,     0,     0,     0,     0,     0,    51,
       0,    53,     0,    55,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
      68,    69,   168,     0,    71,     0,     0,    73,     0,     0,
       0,    77,     0,     0,    79,     0,     0,    81,     0,     0,
       0,     0,     0,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,    86,    87,    88,    89,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,    92,    93,   254,     0,     0,     0,    95,
       0,   169,     0,    97,     0,   170,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   164,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     165,   166,     0,   167,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   168,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,     0,    81,     0,
       0,     0,     0,     0,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,    86,    87,    88,    89,     0,     0,     0,
       0,    90,     0,     0,     5,     6,     7,     8,     9,    10,
       0,    91,     0,   164,    92,    93,    14,    15,     0,     0,
      95,     0,   169,    19,    97,     0,   170,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   165,   166,
       0,   167,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   168,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      85,    86,    87,    88,    89,     0,     0,     0,     0,    90,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    91,
       0,   164,    92,    93,    14,    15,     0,     0,    95,     0,
      96,    19,    97,     0,   170,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   165,   166,     0,   167,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   168,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,     0,    81,     0,     0,     0,     0,     0,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    85,    86,
      87,    88,    89,     0,     0,     0,     0,    90,     0,     0,
       5,     6,     7,     8,     9,    10,     0,    91,     0,   164,
      92,    93,    14,    15,     0,     0,   572,     0,   169,    19,
      97,     0,   170,     0,     0,     0,     0,   599,     0,     0,
       0,     0,     0,    30,   165,   166,     0,   167,    35,     0,
     600,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,   601,    67,    68,    69,   602,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    85,    86,    87,    88,
      89,     0,     0,     0,     0,    90,     0,     0,     5,     6,
       7,     8,     9,    10,     0,    91,     0,   164,    92,    93,
      14,    15,     0,     0,    95,     0,   169,    19,    97,     0,
     603,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   165,   166,     0,   167,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   168,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     5,     6,     7,     8,
       9,    10,     0,    91,     0,   164,    92,    93,    14,    15,
       0,     0,    95,     0,   169,    19,    97,     0,   603,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     165,   166,     0,   672,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   168,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,     0,    81,     0,
       0,     0,     0,     0,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,    86,    87,    88,    89,     0,     0,     0,
       0,    90,     0,     0,     5,     6,     7,     8,     9,    10,
       0,    91,     0,   164,    92,    93,    14,    15,     0,     0,
      95,     0,   169,    19,    97,     0,   170,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   165,   166,
       0,   675,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   168,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      85,    86,    87,    88,    89,     0,     0,     0,     0,    90,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    91,
       0,   164,    92,    93,    14,    15,     0,     0,    95,     0,
     169,    19,    97,     0,   170,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   165,   166,     0,   893,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   168,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,     0,    81,     0,     0,     0,     0,     0,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    85,    86,
      87,    88,    89,     0,     0,     0,     0,    90,     0,     0,
       5,     6,     7,     8,     9,    10,     0,    91,     0,   164,
      92,    93,    14,    15,     0,     0,    95,     0,   169,    19,
      97,     0,   170,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   165,   166,     0,   894,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   168,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    85,    86,    87,    88,
      89,     0,     0,     0,     0,    90,     0,     0,     5,     6,
       7,     8,     9,    10,     0,    91,     0,   164,    92,    93,
      14,    15,     0,     0,    95,     0,   169,    19,    97,     0,
     170,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   165,   166,     0,   896,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   168,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     5,     6,     7,     8,
       9,    10,     0,    91,     0,   164,    92,    93,    14,    15,
       0,     0,    95,     0,   169,    19,    97,     0,   170,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     165,   166,     0,   897,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   168,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,     0,    81,     0,
       0,     0,     0,     0,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,    86,    87,    88,    89,     0,     0,     0,
       0,    90,     0,     0,     5,     6,     7,     8,     9,    10,
       0,    91,     0,   164,    92,    93,    14,    15,     0,     0,
      95,     0,   169,    19,    97,     0,   170,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   165,   166,
       0,   898,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   168,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      85,    86,    87,    88,    89,     0,     0,     0,     0,    90,
       0,     0,     5,     6,     7,     8,     9,    10,     0,    91,
       0,   164,    92,    93,    14,    15,     0,     0,    95,     0,
     169,    19,    97,     0,   170,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   165,   166,     0,   899,
      35,     0,    36,     0,     0,    38,     0,     0,     0,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    66,    67,    68,    69,   168,
       0,    71,     0,     0,    73,     0,     0,     0,    77,     0,
       0,    79,     0,     0,    81,     0,     0,     0,     0,     0,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    85,    86,
      87,    88,    89,     0,     0,     0,     0,    90,     0,     0,
       5,     6,     7,     8,     9,    10,     0,    91,     0,   164,
      92,    93,    14,    15,     0,     0,    95,     0,   169,    19,
      97,     0,   170,     0,     0,     0,     0,   599,     0,     0,
       0,     0,     0,    30,   165,   166,     0,   167,    35,     0,
     600,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,   601,    67,    68,    69,   602,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    85,    86,    87,    88,
      89,     0,     0,     0,     0,    90,     0,     0,     5,     6,
       7,     8,     9,    10,     0,    91,     0,   164,    92,    93,
      14,    15,     0,     0,    95,     0,   169,    19,    97,     0,
     981,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   165,   166,     0,   167,    35,     0,    36,     0,
       0,    38,     0,     0,     0,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   168,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    88,    89,     0,
       0,     0,     0,    90,     0,     0,     5,     6,     7,     8,
       9,    10,     0,    91,     0,   164,    92,    93,    14,    15,
       0,     0,    95,     0,     0,    19,    97,     0,   981,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     165,   166,     0,   167,    35,     0,    36,     0,     0,    38,
       0,     0,     0,    40,    41,     0,     0,    43,    44,     0,
      45,    46,     0,    47,    48,     0,     0,     0,     0,     0,
      51,     0,    53,     0,    55,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,   168,     0,    71,     0,     0,    73,     0,
       0,     0,    77,     0,     0,    79,     0,     0,    81,     0,
       0,     0,     0,     0,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,     0,
       0,     0,    85,    86,    87,    88,    89,     0,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   164,
       0,    91,    14,    15,     0,    93,     0,     0,   364,    19,
      95,  -518,   169,  -518,    97,     0,   170,    26,     0,     0,
       0,     0,     0,    30,   165,   166,     0,   167,    35,     0,
      36,     0,     0,    38,     0,     0,     0,    40,    41,     0,
       0,    43,    44,     0,    45,    46,     0,    47,    48,     0,
       0,     0,     0,     0,    51,     0,    53,     0,    55,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,   168,     0,    71,
       0,     0,    73,     0,     0,     0,    77,     0,     0,    79,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
       0,  -391,     0,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   164,     0,     0,    14,    15,    87,     0,
       0,     0,  -391,    19,     0,     0,  -391,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   165,   166,
     254,   167,    35,     0,    36,     0,   169,    38,    97,  -391,
     914,    40,    41,  -373,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,  -373,     0,     0,     0,    65,    66,    67,    68,
      69,   168,     0,    71,     0,  -373,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    81,     0,  -373,     0,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     5,     6,     7,     8,     9,    10,
       0,  -491,     0,   164,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,   364,  -373,     0,  -518,     0,  -518,
    -373,    26,    97,     0,   981,     0,     0,    30,   165,   166,
       0,   167,    35,     0,    36,     0,     0,    38,     0,     0,
       0,    40,    41,     0,     0,    43,    44,     0,    45,    46,
       0,    47,    48,     0,     0,     0,     0,     0,    51,     0,
      53,     0,    55,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,   168,     0,    71,     0,     0,    73,     0,     0,     0,
      77,     0,     0,    79,     0,     0,    81,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   164,     0,     0,
      14,    15,    87,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   165,   166,     0,   167,    35,     0,    36,     0,
     169,    38,    97,     0,   170,    40,    41,     0,     0,    43,
      44,     0,    45,    46,     0,    47,    48,     0,     0,     0,
       0,     0,    51,     0,    53,     0,    55,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,    69,   168,     0,    71,     0,     0,
      73,     0,     0,     0,    77,     0,     0,    79,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   164,     0,     0,    14,    15,    87,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   165,   166,     0,   167,
      35,     0,    36,     0,   169,    38,    97,     0,   603,    40,
      41,     0,     0,    43,    44,     0,    45,    46,     0,    47,
      48,     0,     0,     0,     0,     0,    51,     0,    53,     0,
      55,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   210,    65,    66,    67,    68,    69,   168,
       0,    71,     0,     0,    73,   211,   212,     0,    77,     0,
       0,    79,   213,     0,    81,     0,     0,     0,     0,     0,
     214,     0,     0,     0,     0,     0,   215,     0,     0,     0,
       0,   216,     0,   217,     0,     0,   218,     0,     0,     0,
      87,     0,     0,     0,   219,     0,     0,     0,     0,     0,
     220,   221,     0,     0,     0,     0,     0,   222,     0,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
      97,   210,   170,     0,     0,   224,   225,     0,   226,     0,
     227,     0,   228,   211,   212,   229,     0,     0,     0,   230,
     213,     0,   231,     0,     0,   232,     0,     0,   214,     0,
       0,     0,     0,     0,   215,     0,     0,     0,     0,   216,
       0,   217,     0,     0,   218,     0,     0,     0,     0,     0,
       0,     0,   219,     0,     0,     0,     0,     0,   220,   221,
       0,     0,     0,     0,     0,   222,     0,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,     0,
       0,   236,   942,   224,   225,     0,   226,     0,   227,   762,
     228,     0,     0,   229,   211,   212,   764,   230,     0,     0,
     231,   213,     0,   232,     0,     0,     0,     0,     0,   214,
       0,     0,     0,     0,     0,   766,     0,     0,     0,     0,
     216,     0,   217,     0,     0,   218,     0,   769,     0,     0,
       0,     0,     0,   219,     0,     0,     0,     0,     0,   771,
     221,     0,     0,     0,     0,   320,   222,     0,     0,     0,
       0,     0,   321,     0,     0,   223,     0,     0,     0,   718,
       0,     0,     0,   322,   224,   225,     0,   226,     0,   227,
       0,   943,     0,     0,   776,     0,     0,     0,   230,     0,
       0,   231,     0,     0,   232,     0,     0,     0,     0,     0,
       0,     0,     0,   349,   350,   351,   352,   353,   354,     0,
       0,   357,   358,   359,   360,     0,   362,   363,   779,   780,
     781,   782,   783,   323,     0,   784,     0,   324,     0,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,   795,
       0,   796,   320,     0,   797,   798,   799,   800,     0,   321,
       0,     0,     0,     0,   402,     0,     0,     0,   325,     0,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   326,   327,     0,   328,   329,   330,
       0,   331,   332,   333,     0,   334,   335,   336,   337,   338,
       0,   339,   340,   341,   342,   343,   344,   345,     0,   320,
     346,   347,   348,     0,     0,     0,   321,     0,     0,   746,
     323,     0,     0,     0,   324,     0,     0,   322,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   320,
       0,     0,     0,     0,     0,   325,   321,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   322,     0,     0,
       0,   326,   327,     0,   328,   329,   330,   323,   331,   332,
     333,   324,   334,   335,   336,   337,   338,     0,   339,   340,
     341,   342,   343,   344,   345,     0,     0,   346,   347,   348,
       0,     0,   320,     0,     0,     0,   820,     0,     0,   321,
       0,     0,   325,     0,     0,     0,     0,   323,     0,     0,
     322,   324,     0,     0,     0,     0,     0,     0,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,     0,   334,
     335,   336,   337,   338,     0,   339,   340,   341,   342,   343,
     344,   345,   325,     0,   346,   347,   348,     0,     0,     0,
       0,     0,     0,   977,     0,     0,     0,     0,   326,   327,
     323,   328,   329,   330,   324,   331,   332,   333,     0,   334,
     335,   336,   337,   338,     0,   339,   340,   341,   342,   343,
     344,   345,   320,     0,   346,   347,   348,     0,     0,   321,
       0,     0,     0,  1044,     0,   325,     0,     0,     0,     0,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   326,   327,     0,   328,   329,   330,     0,   331,   332,
     333,     0,   334,   335,   336,   337,   338,     0,   339,   340,
     341,   342,   343,   344,   345,     0,     0,   346,   347,   348,
       0,     0,     0,     0,   624,     0,     0,     0,     0,     0,
     323,     0,     0,     0,   324,     0,     0,     0,   320,     0,
       0,     0,     0,     0,     0,   321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,     0,     0,     0,
       0,     0,     0,     0,     0,   325,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   326,   327,     0,   328,   329,   330,     0,   331,   332,
     333,     0,   334,   335,   336,   337,   338,     0,   339,   340,
     341,   342,   343,   344,   345,     0,   323,   346,   347,   348,
     324,     0,     0,     0,   941,     0,     0,     0,   320,   413,
       0,     0,     0,     0,     0,   321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,     0,     0,     0,
       0,   325,   349,   350,   351,   352,   353,   354,     0,     0,
     357,   358,   359,   360,     0,   362,   363,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,     0,   334,   335,
     336,   337,   338,     0,   339,   340,   341,   342,   343,   344,
     345,     0,     0,   346,   347,   348,   323,   377,     0,     0,
     324,     0,     0,     0,   320,     0,     0,     0,     0,   825,
       0,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,   325,   349,   350,   351,   352,   353,   354,     0,     0,
     357,   358,   359,   360,     0,   362,   363,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,     0,   334,   335,
     336,   337,   338,     0,   339,   340,   341,   342,   343,   344,
     345,     0,   323,   346,   347,   348,   324,   377,     0,     0,
     320,     0,     0,     0,     0,   832,     0,   321,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,   325,   349,   350,
     351,   352,   353,   354,     0,     0,   357,   358,   359,   360,
       0,   362,   363,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,     0,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,     0,   323,   346,
     347,   348,   324,   377,     0,     0,   320,     0,     0,     0,
       0,   954,     0,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,   325,   349,   350,   351,   352,   353,   354,
       0,     0,   357,   358,   359,   360,     0,   362,   363,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,     0,
     334,   335,   336,   337,   338,     0,   339,   340,   341,   342,
     343,   344,   345,     0,   323,   346,   347,   348,   324,   377,
       0,     0,   320,     0,     0,     0,     0,   955,     0,   321,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     322,     0,     0,     0,     0,     0,     0,     0,     0,   325,
     349,   350,   351,   352,   353,   354,     0,     0,   357,   358,
     359,   360,     0,   362,   363,   326,   327,     0,   328,   329,
     330,     0,   331,   332,   333,     0,   334,   335,   336,   337,
     338,     0,   339,   340,   341,   342,   343,   344,   345,     0,
     323,   346,   347,   348,   324,   377,     0,     0,   320,     0,
       0,     0,     0,   956,     0,   321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,     0,     0,     0,
       0,     0,     0,     0,     0,   325,   349,   350,   351,   352,
     353,   354,     0,     0,   357,   358,   359,   360,     0,   362,
     363,   326,   327,     0,   328,   329,   330,     0,   331,   332,
     333,     0,   334,   335,   336,   337,   338,     0,   339,   340,
     341,   342,   343,   344,   345,     0,   323,   346,   347,   348,
     324,   377,     0,     0,   320,     0,     0,     0,     0,   957,
       0,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,   325,   349,   350,   351,   352,   353,   354,     0,     0,
     357,   358,   359,   360,     0,   362,   363,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,     0,   334,   335,
     336,   337,   338,     0,   339,   340,   341,   342,   343,   344,
     345,     0,   323,   346,   347,   348,   324,   377,     0,     0,
     320,     0,     0,     0,     0,   979,     0,   321,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,   325,   349,   350,
     351,   352,   353,   354,     0,     0,   357,   358,   359,   360,
       0,   362,   363,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,     0,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,     0,   323,   346,
     347,   348,   324,   377,     0,     0,     0,     0,     0,     0,
       0,   980,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   325,   349,   350,   351,   352,   353,   354,
       0,     0,   357,   358,   359,   360,     0,   362,   363,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,     0,
     334,   335,   336,   337,   338,     0,   339,   340,   341,   342,
     343,   344,   345,   320,     0,   346,   347,   348,     0,   377,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   322,   405,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   406,     0,     0,     0,     0,
       0,     0,     0,     0,   320,     0,     0,     0,     0,     0,
       0,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,   399,     0,     0,     0,     0,     0,     0,
       0,   323,     0,     0,     0,   324,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   320,
       0,     0,     0,     0,     0,     0,   321,     0,     0,     0,
       0,     0,   402,     0,     0,     0,   325,   322,   701,     0,
       0,     0,   323,     0,     0,     0,   324,     0,     0,     0,
       0,     0,   326,   327,     0,   328,   329,   330,     0,   331,
     332,   333,     0,   334,   335,   336,   337,   338,     0,   339,
     340,   341,   342,   343,   344,   345,     0,   325,   346,   347,
     348,     0,   377,     0,     0,     0,     0,   323,     0,     0,
     322,   324,     0,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,     0,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,   320,   402,   346,
     347,   348,   325,   377,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   322,   240,     0,   326,   327,
     323,   328,   329,   330,   324,   331,   332,   333,     0,   334,
     335,   336,   337,   338,     0,   339,   340,   341,   342,   343,
     344,   345,   320,     0,   346,   347,   348,     0,   377,   321,
       0,     0,     0,     0,     0,   325,     0,     0,     0,     0,
     322,   693,     0,     0,     0,   323,     0,     0,     0,   324,
       0,   326,   327,     0,   328,   329,   330,     0,   331,   332,
     333,     0,   334,   335,   336,   337,     0,     0,   339,   340,
     341,   342,   343,   344,   345,     0,     0,   346,   347,   348,
     325,     0,     0,     0,     0,   322,     0,     0,     0,     0,
     323,     0,     0,     0,   324,     0,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,     0,   334,   335,   336,
     337,   338,     0,   339,   340,   341,   342,   343,   344,   345,
     320,     0,   346,   347,   348,   325,   377,   321,     0,     0,
       0,     0,     0,     0,     0,   323,     0,     0,   322,   324,
       0,   326,   327,     0,   328,   329,   330,     0,   331,   332,
     333,     0,   334,   335,   336,   337,   338,     0,   339,   340,
     341,   342,   343,   344,   345,   320,     0,   346,   347,   348,
     325,   377,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   322,     0,     0,   326,   327,   323,   328,
     329,   330,   324,   331,   332,   333,     0,   334,   335,   336,
     337,   711,     0,   339,   340,   341,   342,   343,     0,   345,
       0,     0,   346,   347,   348,     0,     0,     0,   320,     0,
       0,     0,     0,   325,     0,   321,     0,     0,     0,     0,
       0,     0,     0,   323,     0,     0,   322,   324,     0,   326,
     327,   410,   328,   329,   330,     0,   331,   332,   333,     0,
     334,   335,   336,   337,   338,     0,   339,   340,   341,   342,
     343,   344,   345,     0,     0,   346,   347,   348,   325,   377,
       0,   411,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   326,   327,   323,   328,   329,   330,
     324,   331,   332,   333,     0,   334,   335,   336,   337,   338,
       0,   339,   340,   341,   342,   343,   344,   345,     0,     0,
     346,   347,   348,     0,   440,   320,     0,     0,     0,     0,
       0,   325,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   322,     0,     0,     0,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,   379,   334,   335,
     336,   337,   338,     0,   339,   340,   341,   342,   343,   344,
     345,   320,   412,   346,   347,   348,     0,     0,   321,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   322,
     539,     0,     0,   323,     0,     0,     0,   324,     0,     0,
       0,     0,     0,   540,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   159,     0,     0,     0,   325,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
       0,     0,     0,   324,   326,   327,     0,   328,   329,   330,
       0,   331,   332,   333,     0,   334,   335,   336,   337,   338,
       0,   339,   340,   341,   342,   343,   344,   345,   320,     0,
     346,   347,   348,     0,   325,   321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,   541,     0,     0,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
     542,   334,   335,   336,   337,   338,     0,   339,   340,   341,
     342,   343,   344,   345,   320,     0,   346,   347,   348,     0,
       0,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,     0,     0,     0,   323,   644,     0,     0,
     324,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   320,     0,     0,     0,     0,     0,
       0,   321,     0,     0,     0,     0,     0,   645,     0,     0,
       0,   325,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   323,     0,     0,     0,   324,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,     0,   334,   335,
     336,   337,   338,     0,   339,   340,   341,   342,   343,   344,
     345,   320,     0,   346,   347,   348,     0,   325,   321,     0,
       0,     0,   323,     0,     0,     0,   324,     0,     0,   322,
       0,     0,     0,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,     0,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,   325,     0,   346,
     347,   348,     0,     0,     0,     0,     0,     0,     0,   322,
       0,     0,     0,   326,   327,     0,   328,   329,   330,   323,
     331,   332,   333,   324,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,   320,   444,   346,
     347,   348,     0,     0,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   325,   322,     0,     0,     0,   323,
       0,     0,     0,   324,     0,     0,     0,     0,     0,     0,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
       0,   334,   335,   336,   337,   338,     0,   339,   340,   341,
     342,   343,   344,   345,   320,   446,   346,   347,   348,     0,
       0,   321,     0,     0,     0,   323,     0,     0,     0,   324,
     326,   327,   322,   328,   329,   330,     0,   331,   332,   333,
       0,   334,   335,   336,   337,     0,     0,   339,   340,   341,
     342,   343,     0,   345,   320,     0,   346,   347,   348,     0,
     325,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,     0,     0,     0,   326,   327,     0,   328,
     329,   330,   323,   331,   332,   333,   324,   334,   335,   336,
     337,   338,     0,   339,   340,   341,   342,   343,   344,   345,
     320,   448,   346,   347,   348,     0,     0,   321,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,   322,     0,
       0,     0,   323,     0,     0,     0,   324,     0,     0,     0,
       0,     0,     0,   326,   327,   543,   328,   329,   330,     0,
     331,   332,   333,     0,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,   325,   453,   346,
     347,   348,     0,     0,     0,     0,     0,     0,   323,     0,
       0,     0,   324,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,     0,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,   320,     0,   346,
     347,   348,   470,   325,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   322,     0,     0,     0,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,     0,
     334,   335,   336,   337,   338,     0,   339,   340,   341,   342,
     343,   344,   345,     0,     0,   346,   347,   348,     0,     0,
     320,     0,     0,     0,     0,     0,     0,   321,     0,     0,
       0,     0,     0,     0,     0,   323,     0,     0,   322,   324,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,     0,     0,     0,     0,     0,     0,   321,     0,     0,
     325,     0,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,   326,   327,   323,   328,
     329,   330,   324,   331,   332,   333,     0,   334,   335,   336,
     337,   338,  -492,   339,   340,   341,   342,   343,   344,   345,
       0,     0,   346,   347,   348,     0,     0,   320,   706,   159,
       0,     0,     0,   325,   321,     0,     0,     0,   323,     0,
       0,     0,   324,     0,     0,   322,     0,     0,     0,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,     0,
     334,   335,   336,   337,   338,     0,   339,   340,   341,   342,
     343,   344,   345,   325,     0,   346,   347,   348,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   326,
     327,     0,   328,   329,   330,   323,   331,   332,   333,   324,
     334,   335,   336,   337,   338,     0,   339,   340,   341,   342,
     343,   344,   345,   320,   682,   346,   347,   348,     0,     0,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     325,   322,     0,     0,   709,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,     0,   334,   335,   336,
     337,   338,     0,   339,   340,   341,   342,   343,   344,   345,
     320,     0,   346,   347,   348,     0,     0,   321,     0,     0,
       0,   323,     0,     0,     0,   324,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   320,     0,     0,     0,     0,     0,     0,   321,
       0,     0,     0,     0,     0,     0,   325,     0,     0,     0,
     322,   815,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   326,   327,     0,   328,   329,   330,   323,   331,
     332,   333,   324,   334,   335,   336,   337,   338,     0,   339,
     340,   341,   342,   343,   344,   345,     0,     0,   346,   347,
     348,     0,     0,     0,     0,     0,     0,     0,     0,   322,
     323,     0,   749,   325,   324,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   326,
     327,     0,   328,   329,   330,     0,   331,   332,   333,     0,
     334,   335,   336,   337,   338,   325,   339,   340,   341,   342,
     343,   344,   345,   320,     0,   346,   347,   348,     0,   323,
     321,   326,   327,   324,   328,   329,   330,     0,   331,   332,
     333,   322,   334,   335,   336,   337,   338,     0,   339,   340,
     341,   342,   343,   344,   345,   320,     0,   346,   347,   348,
       0,     0,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   322,   818,     0,     0,     0,     0,     0,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
       0,   323,   335,   336,   337,   324,     0,   339,   340,   341,
     342,     0,     0,   345,   892,     0,   346,   347,   348,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,     0,     0,   325,   324,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   326,   327,     0,   328,   329,   330,     0,   331,
     332,   333,     0,   334,   335,   336,   337,   338,   325,   339,
     340,   341,   342,   343,   344,   345,   320,     0,   346,   347,
     348,     0,     0,   321,   326,   327,     0,   328,   329,   330,
       0,   331,   332,   333,   322,   334,   335,   336,   337,   338,
       0,   339,   340,   341,   342,   343,   344,   345,   320,   909,
     346,   347,   348,     0,     0,   321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   323,     0,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,     0,   895,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,     0,   325,
     324,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   326,   327,     0,   328,   329,
     330,     0,   331,   332,   333,     0,   334,   335,   336,   337,
     338,   325,   339,   340,   341,   342,   343,   344,   345,   320,
       0,   346,   347,   348,     0,     0,   321,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,   322,   334,   335,
     336,   337,   338,     0,   339,   340,   341,   342,   343,   344,
     345,   320,     0,   346,   347,   348,     0,     0,   321,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   322,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   323,     0,     0,
       0,   324,     0,     0,     0,     0,     0,     0,     0,     0,
     990,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
       0,     0,   325,   324,     0,     0,     0,     0,     0,     0,
       0,     0,   991,     0,     0,     0,     0,     0,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,     0,   334,
     335,   336,   337,   338,   325,   339,   340,   341,   342,   343,
     344,   345,   320,     0,   346,   347,   348,     0,     0,   321,
     326,   327,     0,   328,   329,   330,     0,   331,   332,   333,
     322,   334,   335,   336,   337,   338,     0,   339,   340,   341,
     342,   343,   344,   345,   320,     0,   346,   347,   348,     0,
       0,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     323,     0,     0,     0,   324,     0,     0,     0,     0,     0,
       0,     0,     0,   992,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   323,     0,     0,   325,   324,     0,     0,     0,
       0,     0,     0,     0,     0,   993,     0,     0,     0,     0,
       0,   326,   327,     0,   328,   329,   330,     0,   331,   332,
     333,     0,   334,   335,   336,   337,   338,   325,   339,   340,
     341,   342,   343,   344,   345,   320,     0,   346,   347,   348,
       0,     0,   321,   326,   327,     0,   328,   329,   330,     0,
     331,   332,   333,   322,   334,   335,   336,   337,   338,     0,
     339,   340,   341,   342,   343,   344,   345,   320,     0,   346,
     347,   348,     0,     0,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,     0,     0,     0,   324,     0,     0,
       0,     0,     0,     0,     0,     0,   994,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   323,     0,     0,   325,   324,
       0,     0,     0,     0,     0,     0,     0,     0,   995,     0,
       0,     0,     0,     0,   326,   327,     0,   328,   329,   330,
       0,   331,   332,   333,     0,   334,   335,   336,   337,   338,
     325,   339,   340,   341,   342,   343,   344,   345,   320,     0,
     346,   347,   348,     0,     0,   321,   326,   327,     0,   328,
     329,   330,     0,   331,   332,   333,   322,   334,   335,   336,
     337,   338,     0,   339,   340,   341,   342,   343,   344,   345,
     320,     0,   346,   347,   348,     0,     0,   321,     0,     0,
    1017,     0,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,     0,     0,
     324,     0,     0,     0,     0,     0,     0,     0,     0,   320,
       0,     0,     0,     0,     0,     0,   321,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   322,   323,     0,
       0,   325,   324,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   326,   327,     0,
     328,   329,   330,     0,   331,   332,   333,     0,   334,   335,
     336,   337,   338,   325,   339,   340,   341,   342,   343,   344,
     345,     0,     0,   346,   347,   348,     0,   670,     0,   326,
     327,   324,   328,   329,   330,     0,   331,   332,   333,     0,
     334,   335,   336,   337,   338,     0,   339,   340,   341,   342,
     343,   344,   345,     0,     0,   346,   347,   348,     0,     0,
       0,     0,   325,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   326,   327,
       0,   328,   329,   330,     0,   331,   332,   333,     0,   334,
     335,   336,   337,   338,     0,   339,   340,   341,   342,   343,
     344,   345,     0,     0,   346,   347,   348,   648,     0,   210,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   211,   212,     0,     0,     0,     0,     0,   213,     0,
       0,     0,     0,     0,     0,     0,   214,     0,     0,     0,
       0,     0,   215,     0,     0,     0,     0,   216,     0,   217,
       0,     0,   218,     0,     0,     0,     0,     0,     0,     0,
     219,     0,     0,     0,     0,     0,   220,   221,     0,     0,
       0,     0,     0,   222,     0,     0,   210,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,     0,   211,   212,
       0,   224,   225,     0,   226,   213,   227,     0,   228,     0,
       0,   229,     0,   214,     0,   230,     0,     0,   231,   215,
       0,   232,     0,     0,   216,     0,   217,     0,     0,   218,
       0,     0,     0,     0,     0,     0,     0,   219,     0,     0,
       0,     0,     0,   220,   221,     0,     0,     0,     0,     0,
     222,     0,     0,     0,     0,     0,     0,     0,     0,   223,
       0,     0,     0,     0,     0,     0,     0,     0,   224,   225,
       0,   226,     0,   227,     0,   228,     0,     0,   229,     0,
       0,     0,   230,     0,     0,   231,     0,     0,   232
};

static const yytype_int16 yycheck[] =
{
      11,   370,     2,   418,    15,   144,   449,    18,    56,    57,
     203,    61,   565,    24,   182,   131,    17,    28,    29,   411,
      31,    32,    33,    34,    20,   657,    28,   195,   190,   209,
     727,    42,     1,    44,    45,   598,     1,   753,    49,    29,
      51,    29,    53,   421,     1,    29,   924,   209,    98,    42,
      61,    62,    63,    64,    65,    66,    42,    19,    29,    70,
      81,    72,    29,    74,    75,    81,   962,   183,    79,   107,
     250,    82,    83,  1007,    85,   109,    87,     1,     3,    90,
      91,   109,    93,     1,   848,    96,    97,    98,   250,   269,
      19,    42,     3,   806,   807,    52,   135,   135,    42,    64,
     845,    47,     0,    42,    42,    30,   669,   269,   301,   302,
    1044,  1007,    62,   810,   667,    77,    42,  1022,    42,   109,
     170,   109,    64,   109,    81,   164,   109,    73,     3,    54,
     164,    74,    75,   154,    52,  1040,   164,   276,   159,   101,
      79,   106,   135,   159,    52,   114,  1024,    97,    77,   135,
     107,   135,   849,   164,   165,   166,   167,   168,   169,   170,
     426,   724,   878,    81,   106,   134,   135,    92,   913,   159,
     332,   159,   101,    98,   938,   159,    94,   134,   164,   190,
     181,    92,   154,   449,   135,   901,   366,   367,   159,   107,
     114,   135,   159,   162,   205,   160,   565,   135,   209,   107,
      52,   197,   159,   205,   366,   367,    81,    42,   204,   135,
     134,   135,   135,   164,     3,   912,   134,    92,   160,   597,
     164,   135,    23,    24,    79,   968,   164,   114,   161,    81,
     154,   944,   945,    34,    35,     7,   154,   160,   164,   250,
     251,   159,    94,    44,    79,   154,   160,   258,   259,   135,
      97,   160,   139,   645,   135,   107,   888,   135,   269,   135,
     310,   135,   442,   114,   961,    66,   129,    68,  1011,  1012,
    1013,  1014,  1015,  1016,   137,   129,   162,   830,    79,   395,
     442,   162,   161,   137,   162,   296,   162,   135,   162,   140,
     161,   142,   143,   293,   135,   146,   147,   134,   667,   310,
     137,   716,   154,   104,   141,    24,   154,   159,   319,   320,
     321,   322,   323,   324,   325,   135,   327,   328,   329,   330,
     331,   162,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   753,    42,
     718,    89,   162,   190,   355,   356,    24,   397,   135,    68,
     361,   135,   161,   364,   365,   366,   367,   368,   369,   370,
      79,    64,   209,    24,   135,   135,    28,   154,   379,   159,
      42,    74,    75,   384,   154,   386,   569,   570,   162,    98,
      52,   161,    74,   163,   154,   104,   397,   103,   399,   400,
      68,   162,   154,   555,   405,   406,   154,   153,   160,   410,
     411,    79,   413,   250,   415,   355,   356,    68,   161,    81,
     163,   361,   855,   409,   364,   365,    78,   428,    79,    78,
      82,   135,   269,    82,   435,   135,   104,     2,   135,    64,
     480,   442,   154,   154,   135,   107,    11,    98,   631,   632,
     154,    78,   114,     2,   154,    82,   457,   154,    23,   460,
      25,   462,    11,   154,   622,   466,   467,   468,   145,   470,
     471,   830,    24,   135,    23,   154,    25,   129,   883,   480,
     154,   161,   134,   163,   154,   137,   476,   180,   154,   141,
      42,   154,   154,    45,   161,   137,   901,   159,   159,   154,
     162,   161,   161,   163,   163,    70,   108,   200,   201,   202,
     203,   114,   460,    65,   462,   208,    68,   161,    70,   154,
     460,    70,   462,   161,   161,    74,    75,    79,   657,   366,
     367,    96,   135,   114,   154,   100,   139,     7,   539,   540,
     541,   542,   543,   544,   545,    56,    98,    96,   134,   159,
     243,   100,   553,   246,   555,   546,   547,   154,   559,   162,
     561,   114,   162,   603,   565,   135,   567,   114,    42,   154,
     161,    31,   265,    24,    52,   200,   201,   202,   203,   162,
      24,   135,   135,   135,   162,   114,   139,    42,   162,   162,
     283,    42,   848,    18,    45,   160,   135,    52,   435,   855,
     601,   602,   603,    81,   164,   442,   607,   164,   159,   162,
     162,   160,   154,   553,    65,   135,    94,    68,   243,    70,
     457,   246,   154,   460,    68,   462,    81,   154,    79,   107,
     467,   154,   154,   164,   162,    79,   154,   154,   154,    31,
     265,     2,   135,   644,   645,   160,   114,    98,   162,   162,
      11,   160,   107,    79,    98,    29,    29,   164,   283,   114,
     159,    31,    23,   134,    25,    79,   667,   607,   233,   670,
     671,   672,    28,   164,   675,   240,   154,   134,   164,   164,
     135,   159,   938,   160,   233,    52,   161,   380,   154,   154,
     691,   240,   693,   129,   734,   137,   137,   698,   137,   154,
     701,   129,   137,   704,   159,   706,   962,   162,   709,    70,
     137,   404,   129,   137,    81,   408,   553,   137,   555,   137,
     129,   977,    78,   129,   137,   137,    82,    94,   293,   137,
      31,    24,   162,   734,    94,    96,   129,   160,   129,   100,
     107,   162,   307,   744,   293,   746,   135,   644,   749,    42,
     645,  1007,    45,   164,   755,   380,    22,   307,   307,   888,
     557,   738,   196,   823,   888,   277,  1022,   756,   883,   590,
     716,   759,    65,   129,  1024,    68,   961,    70,   134,   404,
     136,   137,   986,   408,  1040,   141,    79,   154,  1044,  1017,
    1010,    33,   159,   149,    -1,   429,   555,    -1,    -1,   160,
     801,   157,    -1,    -1,    -1,    98,    -1,    -1,    28,    -1,
      -1,    -1,   377,   814,   815,    -1,   817,   818,   819,   820,
      -1,    -1,    -1,    -1,   825,   826,    -1,   828,   377,   830,
      -1,   832,   833,    -1,   399,   836,    -1,   875,   876,    -1,
     405,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   413,    -1,
     399,    -1,    -1,   854,   855,    -1,   405,    -1,    78,   860,
      -1,   862,    82,    -1,   413,   866,    -1,    -1,    -1,    -1,
     563,   564,   233,   874,   914,    -1,   569,   570,    -1,   240,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   581,    -1,
      -1,   892,   893,   894,   895,   896,   897,   898,   899,   464,
      -1,    -1,    -1,   468,    -1,    -1,    -1,    -1,   909,   129,
     130,   476,    -1,   914,   134,   464,   136,   137,   919,   468,
      -1,   141,   923,    -1,    -1,   618,    -1,   476,    -1,   149,
      -1,    -1,   293,    -1,    -1,   155,   156,   157,   563,   564,
      -1,   981,    -1,    -1,   569,   570,   307,    -1,    -1,    -1,
      -1,     2,    -1,   954,   955,   956,   957,    -1,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   536,    23,    -1,    25,    -1,   977,    -1,   979,   980,
     981,    -1,    -1,    -1,    -1,    -1,    -1,   680,   681,   990,
     991,   992,   993,   994,   995,   688,    -1,   690,    -1,    -1,
      -1,    -1,    -1,    -1,   697,  1006,  1007,   700,  1009,    -1,
     703,   848,    -1,    -1,    -1,   580,   377,    -1,    42,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    52,    -1,
      -1,   580,    -1,    -1,    -1,    -1,    -1,    -1,   399,  1040,
      -1,    -1,    -1,  1044,   405,    96,    -1,    -1,   741,   100,
      -1,    -1,   413,    -1,    -1,   680,   681,    81,    -1,    -1,
      -1,    -1,    -1,   688,    -1,   630,    -1,    -1,    -1,    -1,
      -1,   636,    -1,    23,    24,   700,    -1,     2,   703,    -1,
      -1,   630,   919,   107,    34,    35,    11,   636,    -1,    -1,
     114,    -1,   657,    98,    44,    -1,    -1,    47,    23,    -1,
      25,    -1,    -1,   464,    -1,   670,   671,   468,    -1,   160,
      -1,   135,    -1,    -1,    -1,   476,    66,    -1,    68,    28,
      -1,    -1,    -1,    73,    -1,   962,   691,    -1,   693,    79,
     154,    -1,    -1,   698,    -1,   159,   701,    -1,   162,    -1,
      -1,    -1,   691,   708,   693,    70,   711,    -1,    -1,   698,
      -1,    -1,   701,   103,   104,    -1,    -1,    -1,    -1,   708,
      -1,    -1,   711,    -1,    -1,   858,    -1,    -1,   733,    78,
    1007,    96,    -1,    82,    -1,   100,    -1,    -1,    -1,   744,
      -1,   746,   233,    -1,   733,    -1,    -1,    -1,    -1,   240,
      -1,    -1,    -1,    -1,    -1,   744,    -1,   746,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1044,    -1,    -1,
     903,   904,    -1,    -1,   907,   908,    -1,    -1,    -1,   580,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,    -1,
      -1,   924,   141,   858,    -1,   160,    -1,   930,    -1,   932,
     149,   934,   293,    -1,    -1,    -1,   155,   156,   157,    -1,
      -1,    -1,    -1,    23,    24,    -1,   307,    -1,    -1,    -1,
     825,     2,    -1,    -1,    34,    35,    -1,   832,    -1,   630,
      11,    -1,    -1,    -1,    44,   636,   825,    47,    -1,   844,
      -1,    -1,    23,   832,    25,    -1,    -1,    -1,    -1,    -1,
      -1,   856,    -1,    -1,    -1,   844,    66,    -1,    68,    -1,
     865,    -1,    -1,    73,   869,    -1,    -1,   856,   233,    79,
      -1,    -1,    -1,    -1,    -1,   240,   865,    -1,   323,   324,
     869,    -1,    -1,    -1,    -1,    -1,   377,    -1,    -1,    70,
     691,  1024,   693,   103,   104,    -1,    -1,   698,    -1,    -1,
     701,    -1,    -1,    -1,    -1,   910,    -1,   708,   399,    -1,
     711,    -1,    -1,    -1,   405,    96,    -1,    -1,    -1,   100,
      -1,   910,   413,   368,   369,    -1,    -1,    -1,   293,    -1,
      -1,    -1,   733,    -1,   379,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   307,   744,    -1,   746,    -1,    -1,    -1,   954,
     955,   956,   957,    -1,    -1,   400,    -1,    -1,    -1,    -1,
      -1,   406,    -1,    -1,    -1,   954,   955,   956,   957,    -1,
      -1,    -1,    -1,   464,   979,   980,    -1,   468,    -1,   160,
      -1,    -1,    -1,    -1,    -1,   476,    -1,    -1,    -1,    -1,
     979,   980,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    23,    24,    -1,    -1,    -1,     2,    -1,    -1,
      -1,    32,   377,    34,    35,    28,    11,    -1,    39,    -1,
      -1,   466,    -1,    44,   825,    -1,    -1,    -1,    23,    -1,
      25,   832,    -1,    -1,   399,    -1,    -1,    -1,    -1,    -1,
     405,    -1,    -1,   844,    -1,    66,    -1,    68,   413,    70,
      -1,    72,   233,    -1,    75,   856,    77,    -1,    79,   240,
      -1,    -1,    -1,    -1,   865,    78,    -1,    -1,   869,    82,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    98,    -1,    -1,
     101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,   580,
      -1,    -1,    -1,    -1,    -1,   540,    -1,   542,    -1,   464,
      -1,    96,    -1,   468,    39,   100,    -1,    -1,    -1,   910,
      -1,   476,   293,    -1,    -1,    -1,   129,   130,    -1,    -1,
     133,   134,    -1,   136,   137,    -1,   307,    -1,   141,    -1,
      -1,    -1,    -1,   154,    -1,    -1,   149,    -1,    -1,   630,
      -1,    76,   155,   156,   157,   636,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   954,   955,   956,   957,    -1,    -1,    -1,
      -1,    -1,    -1,     2,    -1,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,   979,   980,
      -1,    -1,    -1,    -1,    23,    -1,    25,    -1,    19,   124,
      -1,   126,    23,    24,    -1,    -1,   377,    -1,    -1,    -1,
     691,    32,   693,    34,    35,    -1,    -1,   698,    39,    -1,
     701,    -1,    -1,    44,    -1,   580,    -1,   708,   399,    -1,
     711,    -1,    -1,    -1,   405,   670,   671,    -1,    -1,    -1,
      -1,    70,   413,    -1,    -1,    66,    -1,    68,   233,    70,
      -1,    72,   733,    -1,    75,   240,    77,   182,    79,    -1,
      -1,    -1,    -1,   744,    -1,   746,    -1,    96,    -1,    -1,
     195,   100,    -1,   198,    -1,   630,    -1,    98,    -1,    -1,
     101,   636,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   464,    -1,    -1,    -1,   468,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   476,    -1,    -1,   293,    -1,
      -1,   236,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   307,    -1,    -1,    -1,    -1,    -1,    -1,   254,
      -1,   160,    -1,   154,    -1,    -1,   691,    -1,   693,   160,
      -1,    -1,    -1,   698,   825,    -1,   701,    -1,    -1,    -1,
      -1,   832,    -1,   708,    -1,    -1,   711,    -1,    -1,     2,
      -1,    -1,    -1,   844,    -1,    -1,    -1,    78,    11,    -1,
      -1,    82,    -1,    -1,    -1,   856,    -1,    -1,   733,    -1,
      23,    -1,    25,    -1,   865,    -1,    -1,    -1,   869,   744,
      -1,   746,   377,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   233,    -1,    -1,    -1,    -1,   580,
      -1,   240,    -1,    -1,   399,    -1,    -1,    -1,   129,   130,
     405,   132,   133,   134,    -1,   136,   137,    70,   413,   910,
     141,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,    -1,
      -1,   152,    -1,    -1,   155,   156,   157,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    -1,    -1,   100,    -1,   630,
      -1,    -1,    -1,    -1,   293,   636,    -1,    -1,    -1,   394,
     825,    -1,    -1,   954,   955,   956,   957,   832,   307,   464,
      -1,    -1,    -1,   468,    -1,    -1,    -1,    -1,    -1,   844,
      -1,   476,    -1,    -1,    -1,    -1,   421,    -1,   979,   980,
      -1,   856,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     865,    -1,    -1,    -1,   869,    -1,    -1,   160,    -1,    -1,
     691,    -1,   693,    -1,    -1,    -1,    -1,   698,    -1,    -1,
     701,    -1,    -1,    -1,     3,    -1,    -1,   708,    -1,    -1,
     711,    -1,    -1,    -1,    -1,    -1,    15,    16,   377,    -1,
      -1,    -1,   477,    22,    -1,   910,    -1,   482,    -1,    -1,
      -1,    30,   733,    -1,    -1,    -1,    -1,    36,    -1,    -1,
     399,    -1,    41,   744,    43,   746,   405,    46,    -1,    -1,
      -1,    -1,    -1,    -1,   413,    54,    -1,    -1,    -1,    -1,
     233,    60,    61,    -1,    -1,   580,    -1,   240,    67,   954,
     955,   956,   957,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    88,
      -1,    90,    -1,    92,   979,   980,    95,    -1,    -1,    -1,
      99,   100,    -1,   102,    -1,   464,   105,    28,    -1,   468,
      -1,    -1,    -1,    -1,    -1,   630,    -1,   476,    -1,    -1,
     293,   636,    -1,    -1,   825,    -1,    -1,    -1,    -1,    -1,
      -1,   832,    -1,    -1,   307,    -1,    -1,    28,   593,    -1,
      -1,    -1,   597,   844,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   856,    -1,    78,    -1,    -1,
      -1,    82,   161,   618,   865,    -1,    -1,   622,   869,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   691,    -1,   693,    -1,
      -1,    -1,    -1,   698,    -1,    -1,   701,    78,    -1,    -1,
      -1,    82,    -1,   708,    -1,    -1,   711,    -1,    -1,    -1,
      -1,    -1,   657,    -1,   377,    -1,    -1,    -1,   129,   910,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,   733,    -1,
     141,   580,    -1,    -1,    -1,    -1,   399,    -1,   149,   744,
      -1,   746,   405,    -1,   155,   156,   157,    -1,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,    -1,    -1,
     141,    -1,    -1,   954,   955,   956,   957,   148,   149,    -1,
     715,   152,    -1,   718,   155,   156,   157,    -1,    -1,    -1,
      -1,   630,    -1,    -1,    -1,    -1,    -1,   636,   979,   980,
      -1,    -1,    -1,   738,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   464,    -1,    -1,    -1,   468,    -1,     1,    -1,    -1,
      -1,   756,    -1,   476,    -1,    -1,    -1,    -1,    -1,    -1,
     825,    -1,    -1,    -1,    -1,    19,    -1,   832,    -1,    23,
      24,    -1,    -1,    -1,    -1,    -1,   781,    -1,    32,   844,
      34,    35,   691,    -1,   693,    39,    -1,    -1,    -1,   698,
      44,   856,   701,    47,    -1,    -1,    -1,    -1,    -1,   708,
     865,    -1,   711,    -1,   869,    -1,    -1,    -1,     1,    -1,
      -1,    -1,    66,    -1,    68,    -1,    70,    -1,    72,    73,
      -1,    75,    -1,    77,   733,    79,    19,    -1,    -1,    -1,
      23,    24,    -1,    -1,    -1,   744,    -1,   746,    -1,    32,
      -1,    34,    35,    -1,    98,   910,    39,   101,    -1,    -1,
     104,    44,    -1,    -1,    47,    -1,    -1,   580,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    68,    -1,    70,    -1,    72,
      73,    -1,    75,   888,    77,    -1,    79,    -1,    -1,   954,
     955,   956,   957,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,    98,   160,   630,   101,    -1,
      -1,   104,    -1,   636,   979,   980,   825,    -1,    -1,    -1,
      -1,    -1,    -1,   832,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   844,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,   856,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,   865,    -1,    -1,    36,
     869,   154,    -1,    -1,    41,    -1,    43,   160,   691,    46,
     693,    -1,    -1,    -1,    -1,   698,    -1,    54,   701,    -1,
      -1,    -1,    19,    60,    61,   708,    23,    24,   711,    -1,
      67,    -1,    -1,    -1,    -1,    32,    -1,    34,    35,    76,
      -1,   910,    39,    -1,    -1,    -1,    -1,    44,    85,    86,
     733,    88,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,   744,    99,   746,    -1,   102,    -1,    -1,   105,    66,
      -1,    68,    -1,    70,    -1,    72,    -1,    -1,    75,    -1,
      77,    -1,    79,     3,    -1,   954,   955,   956,   957,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    98,    22,    -1,   101,    -1,    -1,   104,    -1,    -1,
     979,   980,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,   159,    -1,   161,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,   154,    -1,    -1,
      -1,   844,    -1,   160,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    92,   856,    -1,    -1,    -1,    -1,    -1,    99,
      -1,    -1,   865,    -1,    -1,   105,   869,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     0,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,   910,    34,    35,
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
     151,   152,   153,   154,   155,   156,   157,    -1,   159,   160,
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
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,
     129,   130,   131,   132,   133,    10,    -1,    -1,    -1,   138,
      15,    16,    17,    -1,    -1,    -1,    -1,    22,    -1,   148,
      -1,    -1,   151,   152,    -1,    30,    -1,    -1,   157,    -1,
     159,    36,   161,    -1,   163,   164,    41,    -1,    43,    -1,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    -1,    88,    -1,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,    -1,
      -1,   136,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    -1,   152,    -1,    -1,
     155,   156,   157,   158,    -1,    -1,   161,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    85,
      86,    87,    88,    89,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    99,    -1,    -1,   102,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,    -1,    -1,   151,   152,    -1,    -1,    -1,
      -1,   157,    -1,   159,    -1,   161,    -1,   163,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    19,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    98,    99,    -1,   101,   102,    -1,    -1,
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
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    -1,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      96,    97,    -1,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,    -1,
      -1,   157,    -1,   159,    -1,   161,   162,   163,     3,     4,
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
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    42,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,    -1,
      -1,    -1,    -1,   157,    -1,   159,    -1,   161,    -1,   163,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    19,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,
      -1,    -1,    -1,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    87,    88,    89,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    98,    99,    -1,   101,   102,
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
      95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132,   133,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,    -1,    -1,   151,   152,    -1,   154,
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
      -1,    95,    96,    97,    -1,    99,    -1,    -1,   102,    -1,
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
      -1,    54,    55,    -1,    57,    58,    59,    60,    61,    -1,
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
      -1,    -1,    67,    68,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,    -1,
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
      -1,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,    -1,
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
      96,    97,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,
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
      -1,   159,   160,   161,    -1,   163,     3,     4,     5,     6,
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
      -1,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,
      -1,   138,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,   148,    -1,    12,   151,   152,    15,    16,    -1,    -1,
     157,    -1,   159,    22,   161,    -1,   163,    -1,    -1,    -1,
      29,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
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
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   148,
      -1,    12,   151,   152,    15,    16,    -1,    -1,   157,    -1,
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
      -1,   138,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,   148,    -1,    12,   151,   152,    15,    16,    -1,    -1,
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
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   148,
      -1,    12,   151,   152,    15,    16,    -1,    -1,   157,    -1,
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
     131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,   148,    -1,    12,
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
      -1,   138,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,   148,    -1,    12,   151,   152,    15,    16,    -1,    -1,
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
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   148,
      -1,    12,   151,   152,    15,    16,    -1,    -1,   157,    -1,
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
     131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,   148,    -1,    12,
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
      -1,   138,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,   148,    -1,    12,   151,   152,    15,    16,    -1,    -1,
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
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,   148,
      -1,    12,   151,   152,    15,    16,    -1,    -1,   157,    -1,
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
     131,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,   148,    -1,    12,
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
      -1,    -1,    -1,   138,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,   148,    -1,    12,   151,   152,    15,    16,
      -1,    -1,   157,    -1,    -1,    22,   161,    -1,   163,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    -1,    -1,    54,    55,    -1,
      57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    69,    -1,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    99,    -1,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,    -1,   129,   130,   131,   132,   133,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,   148,    15,    16,    -1,   152,    -1,    -1,   158,    22,
     157,   161,   159,   163,   161,    -1,   163,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    55,    -1,    57,    58,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    69,    -1,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    87,    88,    89,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    99,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,   131,    -1,
      -1,    -1,   135,    22,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
     153,    40,    41,    -1,    43,    -1,   159,    46,   161,   162,
     163,    50,    51,    52,    -1,    54,    55,    -1,    57,    58,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    94,    95,    -1,    -1,    -1,
      99,    -1,    -1,   102,    -1,    -1,   105,    -1,   107,    -1,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,    -1,     3,     4,     5,     6,     7,     8,
      -1,   145,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,   158,   154,    -1,   161,    -1,   163,
     159,    30,   161,    -1,   163,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    -1,    -1,    54,    55,    -1,    57,    58,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      69,    -1,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,
      99,    -1,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,   131,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
     159,    46,   161,    -1,   163,    50,    51,    -1,    -1,    54,
      55,    -1,    57,    58,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    99,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,   131,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,   159,    46,   161,    -1,   163,    50,
      51,    -1,    -1,    54,    55,    -1,    57,    58,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    85,    86,    87,    88,    89,    90,
      -1,    92,    -1,    -1,    95,    15,    16,    -1,    99,    -1,
      -1,   102,    22,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
     131,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
     161,     3,   163,    -1,    -1,    85,    86,    -1,    88,    -1,
      90,    -1,    92,    15,    16,    95,    -1,    -1,    -1,    99,
      22,    -1,   102,    -1,    -1,   105,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,   161,     3,    85,    86,    -1,    88,    -1,    90,    10,
      92,    -1,    -1,    95,    15,    16,    17,    99,    -1,    -1,
     102,    22,    -1,   105,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,
      61,    -1,    -1,    -1,    -1,    10,    67,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    76,    -1,    -1,    -1,   161,
      -1,    -1,    -1,    28,    85,    86,    -1,    88,    -1,    90,
      -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    99,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,    78,    -1,   136,    -1,    82,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      -1,   152,    10,    -1,   155,   156,   157,   158,    -1,    17,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,   113,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    -1,    10,
     155,   156,   157,    -1,    -1,    -1,    17,    -1,    -1,   164,
      78,    -1,    -1,    -1,    82,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,   113,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,   129,   130,    -1,   132,   133,   134,    78,   136,   137,
     138,    82,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,    -1,    -1,   155,   156,   157,
      -1,    -1,    10,    -1,    -1,    -1,   164,    -1,    -1,    17,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      28,    82,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,   113,    -1,   155,   156,   157,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,   129,   130,
      78,   132,   133,   134,    82,   136,   137,   138,    -1,   140,
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,    10,    -1,   155,   156,   157,    -1,    -1,    17,
      -1,    -1,    -1,   164,    -1,   113,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,    -1,    -1,   155,   156,   157,
      -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    82,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,    -1,    78,   155,   156,   157,
      82,    -1,    -1,    -1,   162,    -1,    -1,    -1,    10,    91,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,    -1,    -1,   155,   156,   157,    78,   159,    -1,    -1,
      82,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    91,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,    -1,    78,   155,   156,   157,    82,   159,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    91,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,    -1,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,    -1,    78,   155,
     156,   157,    82,   159,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    91,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,    -1,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,    -1,    78,   155,   156,   157,    82,   159,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    91,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,    -1,   132,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,   143,
     144,    -1,   146,   147,   148,   149,   150,   151,   152,    -1,
      78,   155,   156,   157,    82,   159,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    91,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,    -1,    78,   155,   156,   157,
      82,   159,    -1,    -1,    10,    -1,    -1,    -1,    -1,    91,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,    -1,    78,   155,   156,   157,    82,   159,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    91,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,    -1,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,    -1,    78,   155,
     156,   157,    82,   159,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,    -1,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,    10,    -1,   155,   156,   157,    -1,   159,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    82,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,   113,    28,    29,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      -1,    -1,   129,   130,    -1,   132,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,    -1,   113,   155,   156,
     157,    -1,   159,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      28,    82,    -1,   129,   130,    -1,   132,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,    10,   109,   155,
     156,   157,   113,   159,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    -1,   129,   130,
      78,   132,   133,   134,    82,   136,   137,   138,    -1,   140,
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,    10,    -1,   155,   156,   157,    -1,   159,    17,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,    78,    -1,    -1,    -1,    82,
      -1,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,   150,   151,   152,    -1,    -1,   155,   156,   157,
     113,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    82,    -1,   129,   130,    -1,   132,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
      10,    -1,   155,   156,   157,   113,   159,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    28,    82,
      -1,   129,   130,    -1,   132,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,    -1,   146,   147,
     148,   149,   150,   151,   152,    10,    -1,   155,   156,   157,
     113,   159,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,   129,   130,    78,   132,
     133,   134,    82,   136,   137,   138,    -1,   140,   141,   142,
     143,    91,    -1,   146,   147,   148,   149,   150,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,   113,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    28,    82,    -1,   129,
     130,    33,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,    -1,    -1,   155,   156,   157,   113,   159,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,    78,   132,   133,   134,
      82,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    -1,    -1,
     155,   156,   157,    -1,   159,    10,    -1,    -1,    -1,    -1,
      -1,   113,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,   129,   130,    -1,
     132,   133,   134,    -1,   136,   137,   138,    42,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,    10,   154,   155,   156,   157,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      29,    -1,    -1,    78,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    82,   129,   130,    -1,   132,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,    10,    -1,
     155,   156,   157,    -1,   113,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
     129,   130,    -1,   132,   133,   134,    -1,   136,   137,   138,
      42,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,    10,    -1,   155,   156,   157,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    78,    33,    -1,    -1,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
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
     151,   152,    -1,    -1,   155,   156,   157,     1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    15,    16,
      -1,    85,    86,    -1,    88,    22,    90,    -1,    92,    -1,
      -1,    95,    -1,    30,    -1,    99,    -1,    -1,   102,    36,
      -1,   105,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
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
     104,   105,   108,   110,   113,   129,   130,   131,   132,   133,
     138,   148,   151,   152,   154,   157,   159,   161,   163,   171,
     172,   173,   174,   175,   176,   178,   183,   184,   185,   186,
     190,   192,   196,   197,   198,   199,   200,   201,   203,   204,
     205,   209,   210,   213,   214,   217,   218,   221,   224,   225,
     251,   254,   255,   275,   276,   277,   278,   279,   280,   281,
     289,   290,   291,   292,   295,   296,   297,   298,   299,   300,
     301,   303,   304,   305,   306,   307,   154,   173,   292,   109,
     282,   283,     3,   187,    12,    37,    38,    40,    90,   159,
     163,   221,   275,   280,   290,   291,   292,   295,   297,   298,
     282,   292,    98,   254,    79,   187,   173,   269,   292,   173,
     161,   292,   293,     7,     9,    98,   194,   293,    68,   111,
     202,   292,   202,   292,    23,    35,   255,   292,   292,   161,
       3,    15,    16,    22,    30,    36,    41,    43,    46,    54,
      60,    61,    67,    76,    85,    86,    88,    90,    92,    95,
      99,   102,   105,   189,   191,   222,   161,   189,   256,   257,
      29,   178,   195,   292,   292,   292,   292,   292,   292,     7,
     161,    35,   198,   198,   153,   244,   268,   292,    96,    97,
     154,   292,   294,   292,   195,   292,   292,   292,    89,   161,
     173,   292,   292,   178,   186,   292,   295,   178,   186,   292,
     189,   252,   292,   292,   292,   292,   292,   292,   292,   292,
       1,   160,   171,   179,   268,   100,   139,   244,   270,   271,
     294,   202,   268,   292,   302,    70,   173,   159,    74,   177,
     103,   189,   189,    47,    73,   239,   256,   153,   154,   145,
      10,    17,    28,    78,    82,   113,   129,   130,   132,   133,
     134,   136,   137,   138,   140,   141,   142,   143,   144,   146,
     147,   148,   149,   150,   151,   152,   155,   156,   157,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   158,   231,   161,   163,    78,    82,
     161,   173,   154,   292,   292,   292,   268,   159,   178,    42,
     282,   252,   256,   154,   135,   154,   108,   190,   244,   272,
     273,   274,   294,   252,   214,   254,   189,   161,   195,    29,
      42,   195,   109,   195,   285,    29,    42,   195,   285,   293,
      33,    63,   154,    91,   178,   231,   272,   173,   161,   233,
     100,   161,   189,   258,   259,     1,   134,   263,    42,   135,
     173,   195,   195,   272,   189,   135,   154,   292,   292,   154,
     159,   195,   161,   272,   154,   206,   154,   206,   154,   114,
     253,   154,   195,   154,   160,   160,   171,   135,   160,   292,
     135,   162,   135,   162,   164,   285,    42,   135,   164,   285,
     112,   135,   164,     7,     1,   160,   179,    56,   268,   292,
     134,   215,   159,   226,   154,   292,   292,   292,   292,   202,
     292,   202,   292,   292,   292,   292,   292,   292,   292,    30,
      54,    98,   190,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   294,
     294,   294,   294,   294,   272,   272,   202,   292,   202,   292,
      24,    42,    79,   104,   284,   287,   288,   292,   307,    29,
      42,    29,    42,    91,    42,   164,   202,   292,   195,   154,
     154,   292,   292,   114,   162,   135,   154,   189,   256,    42,
     268,    40,   292,   202,   292,   161,   195,    40,   292,   202,
     292,   195,   157,   180,   182,   292,   180,   181,   173,   292,
      31,   292,   162,    24,    42,    45,    65,    68,    79,    98,
     172,   234,   235,   236,   237,   223,   259,   135,   162,    30,
      43,    86,    90,   163,   193,   264,   276,   114,   260,   292,
     257,   162,   244,   292,     1,   211,   272,   162,    18,   207,
     264,   276,   135,   160,   162,   162,   270,   162,   270,   173,
     164,   202,   292,   164,   173,   292,   164,   292,   164,   292,
     160,   160,   189,   154,    33,    63,   268,   159,     1,   189,
     219,   220,    24,    68,    79,    98,   228,   238,   154,   154,
     154,   154,   154,   162,   164,    42,    79,   135,   162,   275,
      78,    78,    40,   202,   292,    40,   202,   292,   202,   292,
     282,   282,   154,   244,   294,   274,   215,   154,   292,   162,
     292,    29,   195,    29,   195,   286,   287,   292,    29,   195,
     285,    29,   195,   285,   135,   154,    11,   154,    31,    31,
     173,    91,   178,    42,    79,   236,   135,   162,   161,   189,
      24,    68,    79,    98,   240,   162,   259,   263,     1,   268,
      59,   294,   160,    64,   106,   160,   212,   162,   161,   178,
     189,   208,   252,   173,   164,   285,   164,   285,   173,   112,
     182,   181,     1,   216,   160,   114,   135,   160,    79,   227,
       1,     3,    10,    15,    17,    22,    36,    41,    46,    48,
      54,    60,    61,    76,    88,    92,    95,    99,   105,   129,
     130,   131,   132,   133,   136,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   152,   155,   156,   157,
     158,   161,   188,   189,   191,   229,   230,   231,   275,   287,
     263,   275,   275,   292,    29,    29,   292,    29,    29,   164,
     164,   195,   195,   159,   195,    91,    40,   292,    40,   292,
     135,   162,    91,    40,   292,   195,    40,   292,   195,   292,
     292,   173,   292,   173,    31,   189,   235,   259,   134,   267,
      79,   263,   260,   164,    42,   164,    29,   173,   268,   208,
     134,   178,    40,   173,   292,   164,    40,   173,   292,   164,
     292,   154,   154,   160,    35,    72,    75,   160,   172,   197,
     292,   220,   240,   161,   232,   189,   292,   129,   137,   232,
     232,   260,    91,    40,    40,    91,    40,    40,    40,    40,
       1,   216,   292,   292,   292,   287,   292,   292,   292,    11,
      31,   173,   267,   162,   163,   193,   244,   266,   276,   139,
     246,   260,    52,   107,   247,   292,   264,   276,   173,   195,
     162,   292,   292,   173,   292,   173,   197,     1,   134,   262,
     234,   162,     3,    92,   230,   231,   292,   292,   292,   292,
     292,   292,   160,   160,    91,    91,    91,    91,   292,   173,
     260,   267,   164,   268,   244,   245,   292,     3,    81,    92,
     248,   249,   250,   292,   178,   196,   243,   164,   178,    91,
      91,   163,   193,   261,   276,    94,   241,   162,   232,   232,
      91,    91,    91,    91,    91,    91,   292,   292,   292,   292,
     246,   260,   244,   265,   266,   276,    42,   164,   250,   107,
     135,   114,   140,   142,   143,   146,   147,    52,   276,   292,
     292,     1,   164,   268,   247,   292,   265,   266,   292,   249,
     250,   250,   250,   250,   250,   250,   248,   164,   261,   276,
     164,   154,   242,   243,   164,   261,   276,   265
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
     182,   183,   183,   183,   184,   184,   184,   185,   186,   186,
     186,   186,   186,   187,   187,   188,   188,   188,   188,   188,
     188,   189,   189,   189,   189,   189,   189,   190,   190,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   193,   193,   193,
     193,   194,   194,   195,   195,   196,   196,   197,   197,   197,
     197,   197,   197,   197,   197,   198,   198,   199,   199,   199,
     199,   200,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   202,   203,
     203,   203,   203,   203,   203,   203,   203,   204,   205,   205,
     205,   205,   205,   205,   206,   206,   207,   207,   207,   208,
     208,   209,   210,   210,   211,   211,   212,   212,   212,   213,
     213,   213,   213,   214,   214,   214,   215,   215,   216,   216,
     216,   217,   217,   218,   219,   219,   219,   220,   220,   222,
     223,   221,   224,   224,   224,   224,   224,   226,   227,   225,
     228,   228,   228,   228,   228,   229,   229,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   232,   232,   233,
     234,   234,   234,   235,   235,   235,   235,   235,   236,   236,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   238,
     238,   238,   238,   238,   238,   239,   239,   240,   240,   240,
     240,   240,   240,   241,   241,   242,   242,   243,   243,   244,
     244,   245,   245,   246,   246,   247,   247,   247,   247,   247,
     248,   248,   249,   249,   249,   249,   249,   249,   249,   250,
     250,   251,   251,   251,   252,   252,   253,   253,   253,   254,
     254,   254,   254,   254,   255,   255,   255,   256,   256,   257,
     257,   258,   258,   258,   259,   259,   259,   260,   260,   260,
     261,   261,   261,   261,   261,   261,   261,   262,   262,   262,
     262,   262,   263,   263,   263,   263,   263,   264,   264,   264,
     264,   265,   265,   265,   266,   266,   266,   266,   266,   267,
     267,   267,   267,   267,   268,   268,   268,   268,   269,   269,
     270,   270,   270,   271,   271,   272,   272,   273,   273,   274,
     274,   274,   274,   275,   275,   276,   276,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   279,   280,   281,   281,   281,   281,
     281,   281,   281,   281,   282,   282,   283,   284,   284,   285,
     286,   286,   287,   287,   287,   288,   288,   288,   288,   288,
     288,   289,   289,   290,   291,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   293,
     293,   294,   294,   294,   295,   295,   295,   295,   296,   296,
     296,   297,   297,   297,   298,   298,   298,   298,   299,   299,
     299,   299,   300,   300,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   302,   302,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   304,   304,   304,   304,   304,   304,   304,   305,
     305,   305,   305,   306,   306,   306,   306,   307,   307,   307,
     307,   307,   307,   307
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
       1,     0,     1,     2,     1,     2,     3,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     3,     5,     5,
       2,     2,     5,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     3,     4,
       6,     7,     6,     7,     4,     5,     4,     5,     4,     4,
       3,     6,     5,     6,     5,     8,     7,     2,     3,     3,
       2,     2,     3,     3,     0,     2,     2,     3,     5,     1,
       3,     3,     5,     5,     0,     2,     3,     2,     3,     6,
       8,     6,     8,     1,     1,     1,     0,     2,     0,     2,
       3,     5,     5,     1,     1,     2,     3,     1,     3,     0,
       0,     8,     0,     1,     2,     2,     1,     0,     0,    10,
       3,     3,     5,     5,     3,     1,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     3,
       0,     1,     3,     4,     5,     4,     6,     6,     0,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     0,
       1,     1,     2,     1,     1,     1,     1,     0,     1,     2,
       1,     1,     1,     0,     1,     1,     1,     1,     1,     2,
       1,     0,     1,     2,     2,     0,     2,     2,     4,     4,
       1,     3,     3,     3,     3,     3,     3,     3,     2,     1,
       1,     3,     4,     4,     2,     4,     0,     2,     2,     1,
       1,     1,     2,     1,     4,     5,     3,     1,     3,     3,
       5,     1,     1,     3,     1,     2,     3,     0,     2,     2,
       3,     2,     4,     3,     3,     4,     3,     0,     2,     2,
       2,     1,     0,     2,     2,     2,     1,     4,     4,     6,
       3,     0,     1,     1,     3,     4,     3,     4,     6,     0,
       2,     2,     2,     2,     1,     1,     3,     3,     1,     3,
       1,     1,     1,     3,     3,     0,     1,     1,     3,     3,
       3,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     2,     4,     4,     4,     5,     2,     2,     1,
       2,     1,     2,     1,     2,     1,     2,     6,     6,     4,
       9,     9,     7,     6,     6,     4,     9,     9,     7,     4,
       6,     6,     9,     9,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     1,     4,     1,     3,     4,
       1,     3,     4,     3,     3,     1,     1,     2,     1,     2,
       1,     1,     3,     2,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     3,     2,     2,     1,     0,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       1,     4,     4,     4,     3,     3,     3,     3,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
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
#line 488 "chapel.ypp" /* yacc.c:1663  */
    { yyblock = (yyval.pblockstmt); }
#line 5621 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 3:
#line 493 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5627 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 4:
#line 494 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5633 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 6:
#line 501 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5639 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 7:
#line 506 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5645 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 8:
#line 507 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5651 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 23:
#line 526 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5657 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 24:
#line 527 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5663 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 25:
#line 528 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5669 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 26:
#line 529 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5675 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 27:
#line 530 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5681 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 28:
#line 531 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5687 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 29:
#line 532 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5693 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 30:
#line 533 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5699 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 31:
#line 534 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5705 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 32:
#line 535 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5711 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 33:
#line 536 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5717 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 34:
#line 537 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5723 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 35:
#line 538 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5729 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 36:
#line 539 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5735 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 37:
#line 540 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5741 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 38:
#line 541 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5747 "bison-chapel.cpp" /* yacc.c:1663  */
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
#line 5761 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 40:
#line 559 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 5770 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 41:
#line 564 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 5779 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 42:
#line 569 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 5788 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 43:
#line 576 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5794 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 44:
#line 577 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5800 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 45:
#line 578 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5806 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 46:
#line 582 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 5812 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 47:
#line 583 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 5818 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 48:
#line 596 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5824 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 49:
#line 597 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5830 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 50:
#line 598 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5836 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 51:
#line 603 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5842 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 52:
#line 604 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5848 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 53:
#line 609 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5858 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 54:
#line 614 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5868 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 55:
#line 619 "chapel.ypp" /* yacc.c:1663  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5877 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 56:
#line 623 "chapel.ypp" /* yacc.c:1663  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5886 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 57:
#line 630 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5896 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 59:
#line 639 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5906 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 61:
#line 648 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 5912 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 62:
#line 649 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 5918 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 63:
#line 650 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 5924 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 64:
#line 654 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr), (yyvsp[-3].b)); }
#line 5930 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 65:
#line 655 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 5936 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 66:
#line 656 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 5942 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 67:
#line 660 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5948 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 68:
#line 665 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 5954 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 69:
#line 667 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5960 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 70:
#line 669 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5966 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 71:
#line 671 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5972 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 72:
#line 673 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5978 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 73:
#line 679 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 5984 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 74:
#line 680 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5990 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 75:
#line 684 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5996 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 76:
#line 685 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6002 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 77:
#line 686 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6008 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 78:
#line 687 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6014 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 79:
#line 688 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6020 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 80:
#line 689 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6026 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 81:
#line 692 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6032 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 82:
#line 693 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6038 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 83:
#line 694 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6044 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 84:
#line 695 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6050 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 85:
#line 696 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6056 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 86:
#line 697 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6062 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 87:
#line 709 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6068 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 88:
#line 710 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6074 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 89:
#line 721 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bool"; }
#line 6080 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 90:
#line 722 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "int"; }
#line 6086 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 91:
#line 723 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "uint"; }
#line 6092 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 92:
#line 724 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "real"; }
#line 6098 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 93:
#line 725 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "imag"; }
#line 6104 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 94:
#line 726 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "complex"; }
#line 6110 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 95:
#line 727 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "string"; }
#line 6116 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 96:
#line 728 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "sync"; }
#line 6122 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 97:
#line 729 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "single"; }
#line 6128 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 98:
#line 730 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "owned"; }
#line 6134 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 99:
#line 731 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "shared"; }
#line 6140 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 100:
#line 732 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "borrowed"; }
#line 6146 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 101:
#line 733 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "unmanaged"; }
#line 6152 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 102:
#line 734 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "domain"; }
#line 6158 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 103:
#line 735 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "index"; }
#line 6164 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 104:
#line 736 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "locale"; }
#line 6170 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 105:
#line 737 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "nothing"; }
#line 6176 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 106:
#line 738 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "void"; }
#line 6182 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 107:
#line 742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6188 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 108:
#line 743 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6194 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 109:
#line 744 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6200 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 110:
#line 745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6206 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 111:
#line 746 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6212 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 112:
#line 747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6218 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 113:
#line 748 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6224 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 114:
#line 749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6230 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 115:
#line 750 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 6236 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 116:
#line 751 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 6242 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 117:
#line 758 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_syncvar"; }
#line 6248 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 118:
#line 759 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_singlevar"; }
#line 6254 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 119:
#line 764 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_domain"; }
#line 6260 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 120:
#line 765 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_index"; }
#line 6266 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 121:
#line 770 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 6272 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 122:
#line 771 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6278 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 123:
#line 775 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6284 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 124:
#line 776 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6290 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 125:
#line 780 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6296 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 126:
#line 781 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6302 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 127:
#line 785 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6308 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 129:
#line 787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6314 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 130:
#line 788 "chapel.ypp" /* yacc.c:1663  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6320 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 137:
#line 801 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6326 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 138:
#line 802 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6332 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 139:
#line 803 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6338 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 140:
#line 804 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6344 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 141:
#line 809 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6352 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 142:
#line 815 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6358 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 143:
#line 816 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6364 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 144:
#line 817 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 6370 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 145:
#line 818 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6376 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 146:
#line 819 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 6382 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 147:
#line 820 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6388 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 148:
#line 821 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6394 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 149:
#line 822 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6400 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 150:
#line 823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6406 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 151:
#line 824 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6412 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 152:
#line 825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6418 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 153:
#line 826 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6424 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 154:
#line 827 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6430 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 155:
#line 828 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6436 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 156:
#line 829 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6442 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 157:
#line 830 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6448 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 158:
#line 831 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6454 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 159:
#line 832 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6460 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 160:
#line 834 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6470 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 161:
#line 840 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6480 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 162:
#line 846 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6490 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 163:
#line 852 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6500 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 164:
#line 858 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6511 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 165:
#line 865 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6522 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 166:
#line 872 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6530 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 167:
#line 876 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 6538 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 168:
#line 882 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 6544 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 169:
#line 886 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6550 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 170:
#line 887 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6556 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 171:
#line 888 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6562 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 172:
#line 889 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6568 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 173:
#line 890 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6575 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 174:
#line 892 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6582 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 175:
#line 894 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6589 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 176:
#line 896 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6596 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 177:
#line 901 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6602 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 178:
#line 904 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6608 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 179:
#line 905 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6614 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 180:
#line 906 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6620 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 181:
#line 907 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6626 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 182:
#line 908 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6632 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 183:
#line 909 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6638 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 184:
#line 913 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6644 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 185:
#line 914 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6650 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 186:
#line 918 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6656 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 187:
#line 919 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 6662 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 188:
#line 920 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 6668 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 189:
#line 924 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6674 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 190:
#line 925 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6680 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 191:
#line 929 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6686 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 192:
#line 933 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6692 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 193:
#line 935 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6698 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 194:
#line 939 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6704 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 195:
#line 940 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6710 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 196:
#line 945 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6716 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 197:
#line 947 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6722 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 198:
#line 949 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6728 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 199:
#line 956 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6742 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 200:
#line 966 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6756 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 201:
#line 976 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6770 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 202:
#line 986 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6784 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 203:
#line 998 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6794 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 204:
#line 1003 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6804 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 205:
#line 1008 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6814 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 206:
#line 1016 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 6820 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 207:
#line 1017 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6826 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 208:
#line 1022 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6832 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 209:
#line 1024 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6838 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 210:
#line 1026 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6844 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 211:
#line 1031 "chapel.ypp" /* yacc.c:1663  */
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
#line 6864 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 212:
#line 1047 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6872 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 213:
#line 1054 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6882 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 214:
#line 1063 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6894 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 215:
#line 1071 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6902 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 216:
#line 1075 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6910 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 217:
#line 1081 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6916 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 218:
#line 1082 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6922 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 219:
#line 1087 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6931 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 220:
#line 1092 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6940 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 221:
#line 1097 "chapel.ypp" /* yacc.c:1663  */
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
#line 6962 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 222:
#line 1119 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6973 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 223:
#line 1125 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6985 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 224:
#line 1132 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6996 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 225:
#line 1138 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7007 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 226:
#line 1144 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 7019 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 227:
#line 1155 "chapel.ypp" /* yacc.c:1663  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 7029 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 228:
#line 1161 "chapel.ypp" /* yacc.c:1663  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7040 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 229:
#line 1168 "chapel.ypp" /* yacc.c:1663  */
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
#line 7067 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 230:
#line 1194 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 7075 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 231:
#line 1198 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7084 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 232:
#line 1203 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 7092 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 233:
#line 1207 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7101 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 234:
#line 1212 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 7109 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 236:
#line 1219 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7115 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 237:
#line 1223 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7121 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 238:
#line 1224 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 7127 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 239:
#line 1225 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&"; }
#line 7133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 240:
#line 1226 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|"; }
#line 7139 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 241:
#line 1227 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^"; }
#line 7145 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 242:
#line 1228 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "~"; }
#line 7151 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 243:
#line 1229 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "=="; }
#line 7157 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 244:
#line 1230 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!="; }
#line 7163 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 245:
#line 1231 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<="; }
#line 7169 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 246:
#line 1232 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">="; }
#line 7175 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 247:
#line 1233 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<"; }
#line 7181 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 248:
#line 1234 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">"; }
#line 7187 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 249:
#line 1235 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+"; }
#line 7193 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 250:
#line 1236 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-"; }
#line 7199 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 251:
#line 1237 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*"; }
#line 7205 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 252:
#line 1238 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/"; }
#line 7211 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 253:
#line 1239 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<"; }
#line 7217 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 254:
#line 1240 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>"; }
#line 7223 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 255:
#line 1241 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%"; }
#line 7229 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 256:
#line 1242 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**"; }
#line 7235 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 257:
#line 1243 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!"; }
#line 7241 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 258:
#line 1244 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_by"; }
#line 7247 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 259:
#line 1245 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "#"; }
#line 7253 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 260:
#line 1246 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_align"; }
#line 7259 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 261:
#line 1247 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<=>"; }
#line 7265 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 262:
#line 1248 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<~>"; }
#line 7271 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 263:
#line 1249 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "init="; }
#line 7277 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 264:
#line 1250 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 7283 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 265:
#line 1254 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "="; }
#line 7289 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 266:
#line 1255 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+="; }
#line 7295 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 267:
#line 1256 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-="; }
#line 7301 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 268:
#line 1257 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*="; }
#line 7307 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 269:
#line 1258 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/="; }
#line 7313 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 270:
#line 1259 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%="; }
#line 7319 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 271:
#line 1260 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**="; }
#line 7325 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 272:
#line 1261 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&="; }
#line 7331 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 273:
#line 1262 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|="; }
#line 7337 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 274:
#line 1263 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^="; }
#line 7343 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 275:
#line 1264 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>="; }
#line 7349 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 276:
#line 1265 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<="; }
#line 7355 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 277:
#line 1269 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7361 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 278:
#line 1270 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7367 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 279:
#line 1274 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7373 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 280:
#line 1278 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7379 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 281:
#line 1279 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7385 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 282:
#line 1280 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7391 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 283:
#line 1285 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7397 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 284:
#line 1287 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7403 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 285:
#line 1289 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7409 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 286:
#line 1291 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7415 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 287:
#line 1293 "chapel.ypp" /* yacc.c:1663  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7421 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 288:
#line 1297 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7427 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 289:
#line 1298 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7433 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 290:
#line 1302 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_IN; }
#line 7439 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 291:
#line 1303 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7445 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 292:
#line 1304 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_OUT; }
#line 7451 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 293:
#line 1305 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST; }
#line 7457 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 294:
#line 1306 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7463 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 295:
#line 1307 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7469 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 296:
#line 1308 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7475 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 297:
#line 1309 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF; }
#line 7481 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 298:
#line 1310 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7487 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 299:
#line 1314 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7493 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 300:
#line 1315 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7499 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 301:
#line 1316 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF;   }
#line 7505 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 302:
#line 1317 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 7511 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 303:
#line 1318 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST;   }
#line 7517 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 304:
#line 1319 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 7523 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 305:
#line 1323 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 7529 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 306:
#line 1324 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 7535 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 307:
#line 1328 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 7541 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 308:
#line 1329 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 7547 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 309:
#line 1330 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 7553 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 310:
#line 1331 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_REF; }
#line 7559 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 311:
#line 1332 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_PARAM; }
#line 7565 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 312:
#line 1333 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_TYPE; }
#line 7571 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 313:
#line 1337 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 7577 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 314:
#line 1338 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 7583 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 315:
#line 1341 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = NULL; }
#line 7589 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 318:
#line 1347 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7595 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 319:
#line 1352 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7601 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 320:
#line 1354 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7607 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 321:
#line 1358 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7613 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 323:
#line 1363 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7619 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 324:
#line 1364 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7625 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 325:
#line 1368 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 7631 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 326:
#line 1370 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 7637 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 327:
#line 1372 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 7643 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 328:
#line 1374 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7649 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 329:
#line 1376 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7655 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 330:
#line 1380 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7661 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 331:
#line 1381 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7667 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 332:
#line 1384 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7673 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 333:
#line 1385 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7679 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 334:
#line 1386 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7685 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 335:
#line 1387 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7691 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 336:
#line 1388 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7697 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 337:
#line 1389 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7703 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 338:
#line 1390 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 7709 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 339:
#line 1393 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 7715 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 340:
#line 1394 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 7721 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 341:
#line 1398 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7727 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 342:
#line 1400 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7733 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 343:
#line 1402 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7739 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 344:
#line 1407 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7756 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 345:
#line 1420 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7774 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 346:
#line 1436 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 7780 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 347:
#line 1438 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7786 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 348:
#line 1440 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7792 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 349:
#line 1444 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 7798 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 350:
#line 1445 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 7804 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 351:
#line 1446 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 7810 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 352:
#line 1447 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 7816 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 353:
#line 1448 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 7822 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 354:
#line 1453 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7832 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 355:
#line 1459 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
      context->latestComment = NULL;
    }
#line 7842 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 356:
#line 1465 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7851 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 358:
#line 1474 "chapel.ypp" /* yacc.c:1663  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7860 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 359:
#line 1482 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7866 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 360:
#line 1484 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7872 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 361:
#line 1489 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7878 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 362:
#line 1491 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7884 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 363:
#line 1493 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7890 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 364:
#line 1498 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7896 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 365:
#line 1500 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7902 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 366:
#line 1502 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7908 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 367:
#line 1508 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 7914 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 368:
#line 1509 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7920 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 369:
#line 1510 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7926 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 370:
#line 1516 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7932 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 371:
#line 1518 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7938 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 372:
#line 1520 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7946 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 373:
#line 1524 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7954 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 374:
#line 1528 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7960 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 375:
#line 1530 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7968 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 376:
#line 1534 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7976 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 377:
#line 1541 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 7982 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 378:
#line 1542 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7988 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 379:
#line 1543 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7994 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 380:
#line 1544 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8000 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 381:
#line 1545 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8006 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 382:
#line 1550 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8012 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 383:
#line 1551 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8018 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 384:
#line 1552 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 8024 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 385:
#line 1553 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8030 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 386:
#line 1554 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8036 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 387:
#line 1575 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8044 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 388:
#line 1579 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 8052 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 389:
#line 1583 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 8064 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 390:
#line 1591 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8072 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 391:
#line 1597 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8078 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 392:
#line 1598 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8084 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 393:
#line 1599 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8090 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 394:
#line 1604 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8096 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 395:
#line 1606 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8102 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 396:
#line 1612 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8108 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 397:
#line 1614 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8114 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 398:
#line 1616 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 8120 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 399:
#line 1620 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8126 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 400:
#line 1621 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8132 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 401:
#line 1622 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8138 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 402:
#line 1623 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8144 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 403:
#line 1624 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8150 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 404:
#line 1630 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8156 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 405:
#line 1631 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8162 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 406:
#line 1632 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8168 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 407:
#line 1633 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8174 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 408:
#line 1637 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 8180 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 409:
#line 1638 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8186 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 410:
#line 1642 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 8192 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 411:
#line 1643 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8198 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 412:
#line 1644 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8204 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 413:
#line 1648 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8210 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 414:
#line 1649 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8216 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 415:
#line 1653 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 8222 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 417:
#line 1658 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8228 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 418:
#line 1659 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8234 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 419:
#line 1663 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 8240 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 420:
#line 1664 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8246 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 421:
#line 1665 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8252 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 422:
#line 1666 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8258 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 423:
#line 1670 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8264 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 424:
#line 1671 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8270 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 425:
#line 1683 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8276 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 426:
#line 1685 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS, (yyvsp[-1].pexpr)); }
#line 8282 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 432:
#line 1695 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 8288 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 433:
#line 1697 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 8294 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 434:
#line 1699 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 8300 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 435:
#line 1701 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 8306 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 436:
#line 1703 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 8312 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 437:
#line 1705 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 8318 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 438:
#line 1707 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 8324 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 439:
#line 1710 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 8330 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 440:
#line 1712 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8336 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 441:
#line 1714 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_unmanaged"); }
#line 8342 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 442:
#line 1716 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 8348 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 443:
#line 1718 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 8354 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 444:
#line 1720 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8360 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 445:
#line 1722 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_borrowed"); }
#line 8366 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 446:
#line 1724 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 8372 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 447:
#line 1729 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8378 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 448:
#line 1731 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8384 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 449:
#line 1733 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8390 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 450:
#line 1735 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8396 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 451:
#line 1737 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8402 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 452:
#line 1739 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8408 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 453:
#line 1741 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8414 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 454:
#line 1743 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8420 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 455:
#line 1745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8426 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 456:
#line 1747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8432 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 457:
#line 1749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8438 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 458:
#line 1751 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8444 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 459:
#line 1753 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8455 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 460:
#line 1760 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8465 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 461:
#line 1766 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8475 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 462:
#line 1772 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8485 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 463:
#line 1778 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 8495 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 464:
#line 1787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8501 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 465:
#line 1796 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 8507 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 473:
#line 1812 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8513 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 474:
#line 1816 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 8519 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 476:
#line 1821 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8525 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 477:
#line 1825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8531 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 478:
#line 1826 "chapel.ypp" /* yacc.c:1663  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8537 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 479:
#line 1830 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8543 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 480:
#line 1834 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8549 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 481:
#line 1835 "chapel.ypp" /* yacc.c:1663  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8555 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 482:
#line 1840 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 8563 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 483:
#line 1844 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8571 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 484:
#line 1848 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8579 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 485:
#line 1854 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 8585 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 486:
#line 1855 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8591 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 487:
#line 1856 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8597 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 488:
#line 1857 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8603 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 489:
#line 1858 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8609 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 490:
#line 1859 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8615 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 492:
#line 1865 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8621 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 493:
#line 1870 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8627 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 494:
#line 1875 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8633 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 503:
#line 1891 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8639 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 504:
#line 1893 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8645 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 505:
#line 1895 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8651 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 506:
#line 1897 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8657 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 507:
#line 1899 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8663 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 508:
#line 1901 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8669 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 509:
#line 1905 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8675 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 510:
#line 1906 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8681 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 511:
#line 1909 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8687 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 512:
#line 1910 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8693 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 513:
#line 1911 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8699 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 518:
#line 1928 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8705 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 519:
#line 1929 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 8711 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 520:
#line 1930 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8717 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 521:
#line 1934 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8723 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 522:
#line 1935 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8729 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 523:
#line 1936 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8735 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 524:
#line 1940 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8741 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 525:
#line 1941 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8747 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 526:
#line 1942 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8753 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 527:
#line 1943 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 8759 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 528:
#line 1951 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8765 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 529:
#line 1952 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8771 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 530:
#line 1953 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8777 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 531:
#line 1954 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8783 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 532:
#line 1958 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 8789 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 533:
#line 1959 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 8795 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 535:
#line 1964 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 8801 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 536:
#line 1965 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8807 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 537:
#line 1966 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8813 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 538:
#line 1967 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8819 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 539:
#line 1968 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8825 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 540:
#line 1969 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 8831 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 541:
#line 1970 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8837 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 542:
#line 1971 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8843 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 543:
#line 1972 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8849 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 544:
#line 1973 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8855 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 545:
#line 1975 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8863 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 546:
#line 1979 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8871 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 547:
#line 1986 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8877 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 548:
#line 1987 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8883 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 549:
#line 1991 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8889 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 550:
#line 1992 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8895 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 551:
#line 1993 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8901 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 552:
#line 1994 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8907 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 553:
#line 1995 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8913 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 554:
#line 1996 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8919 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 555:
#line 1997 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8925 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 556:
#line 1998 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8931 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 557:
#line 1999 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8937 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 558:
#line 2000 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8943 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 559:
#line 2001 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8949 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 560:
#line 2002 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8955 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 561:
#line 2003 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8961 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 562:
#line 2004 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8967 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 563:
#line 2005 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8973 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 564:
#line 2006 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8979 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 565:
#line 2007 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8985 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 566:
#line 2008 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8991 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 567:
#line 2009 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8997 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 568:
#line 2010 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9003 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 569:
#line 2011 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9009 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 570:
#line 2012 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9015 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 571:
#line 2013 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9021 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 572:
#line 2017 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 9027 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 573:
#line 2018 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 9033 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 574:
#line 2019 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 9039 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 575:
#line 2020 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 9045 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 576:
#line 2021 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 9051 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 577:
#line 2022 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9057 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 578:
#line 2023 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 9063 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 579:
#line 2027 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9069 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 580:
#line 2028 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9075 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 581:
#line 2029 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9081 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 582:
#line 2030 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9087 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 583:
#line 2034 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9093 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 584:
#line 2035 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9099 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 585:
#line 2036 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9105 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 586:
#line 2037 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9111 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 587:
#line 2042 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 9117 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 588:
#line 2043 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 9123 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 589:
#line 2044 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 9129 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 590:
#line 2045 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 9135 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 591:
#line 2046 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 9141 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 592:
#line 2047 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 9147 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 593:
#line 2048 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 9153 "bison-chapel.cpp" /* yacc.c:1663  */
    break;


#line 9157 "bison-chapel.cpp" /* yacc.c:1663  */
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
